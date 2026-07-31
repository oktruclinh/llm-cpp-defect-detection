#!/usr/bin/env python3
"""Generate a GoogleTest suite for one defect with one model + one run.

Pipeline: extract focal code (at fixed commit) -> standard prompt -> LLM -> extract
code -> compile against FIXED -> if it fails, ONE compile-repair shot. Everything is
cached under data/raw/<defect_id>/<model>/run<k>/ (idempotent, resumable).
"""
import json
import sys
import tempfile
from datetime import datetime
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
sys.path[:0] = [str(STUDY / "generate"), str(STUDY / "harness"),
                str(STUDY / "benchmarks"), str(STUDY / "measure")]
from llm_client import LLMClient                       # noqa: E402
from prompt import (gen_messages, gen_messages_context_rich, gen_messages_guided,  # noqa: E402
                    gen_messages_api_rich, PUBLIC_API, repair_messages, extract_code)
from context import focal_includes, focal_region, public_api_surface   # noqa: E402
from defects4c_adapter import extract_focal            # noqa: E402
from cpp_harness import compile_test, prepare_version, RECIPES  # noqa: E402

REPOS = json.loads((STUDY / "config/repos.json").read_text())


def repo_for(defect):
    p = REPOS.get(defect["project"])
    if not p:
        raise RuntimeError(f"no local repo registered for {defect['project']} (config/repos.json)")
    return Path(p)


def _compiles_on_fixed(test_code: str, defect: dict, repo: Path):
    # Tier B/C (project-build): the generate-time compile gate would need a full project
    # build per shot; skip it (the real compile validation happens in detection/evaluate).
    if defect["project"] not in RECIPES:
        return True, ""
    prepare_version(repo, defect, "fixed")
    with tempfile.TemporaryDirectory() as td:
        tf = Path(td) / "t.cpp"
        tf.write_text(test_code)
        b = compile_test(tf, defect, repo, Path(td) / "b")
        return b.compiled, b.stderr


def _signature_only(focal: str) -> str:
    """Drop function bodies — keep up to the first '{' (for the no_focal ablation)."""
    cut = focal.find("{")
    return (focal[:cut].rstrip() + ";") if cut > 0 else focal


def generate(defect: dict, model_name: str, run_id: int, force: bool = False,
             condition: str = "fixed") -> dict:
    # cache path: legacy (no condition subdir) for 'fixed' so the existing run-1 cache
    # is reused and NOT re-charged; condition subdir for buggy/no_focal.
    base = STUDY / "data/raw" / defect["defect_id"] / model_name
    cache = (base / f"run{run_id}") if condition == "fixed" else (base / condition / f"run{run_id}")
    final = cache / "test.cpp"
    meta_p = cache / "meta.json"
    if final.is_file() and meta_p.is_file() and not force:
        return json.loads(meta_p.read_text())
    cache.mkdir(parents=True, exist_ok=True)

    def dump(name, obj):
        (cache / name).write_text(json.dumps(obj, indent=1, ensure_ascii=False))

    repo = repo_for(defect)
    # focal source: buggy condition reads commit_before; fixed/no_focal read commit_after
    src_commit = defect["commit_before"] if condition == "buggy" else defect["commit_after"]
    focal = extract_focal(repo, src_commit, defect["focal_src"],
                          defect["func_start"], defect["func_end"]) or "(focal source unavailable)"
    if condition == "no_focal":
        focal = _signature_only(focal)
    (cache / "focal.txt").write_text(focal)   # exact LLM input, frozen
    client = LLMClient(model_name)
    seed = run_id   # reproducible-but-varying sampling across runs

    # shot 1 — persist prompt + FULL raw response (the paid artifact) before anything else
    if condition == "context_rich":
        incs = focal_includes(repo, src_commit, defect["focal_src"])
        region = focal_region(repo, src_commit, defect["focal_src"],
                              defect["func_start"], defect["func_end"])
        (cache / "context.txt").write_text(
            "INCLUDES:\n" + "\n".join(incs) + "\n\nREGION:\n" + (region or ""))
        msgs1 = gen_messages_context_rich(defect, focal, incs, region)
    elif condition == "api_rich":
        api = PUBLIC_API.get(defect["project"]) or "\n".join(
            public_api_surface(repo, src_commit, defect["focal_src"]))
        (cache / "api_surface.txt").write_text(api)
        msgs1 = gen_messages_api_rich(defect, focal, api)
    elif condition == "guided":
        msgs1 = gen_messages_guided(defect, focal)
    else:
        msgs1 = gen_messages(defect, focal, condition=condition)
    dump("prompt_shot1.json", msgs1)
    r1 = client.chat(msgs1, seed=seed)
    dump("response_shot1.json", r1["raw"])
    code = extract_code(r1["text"])
    (cache / "shot1.cpp").write_text(code)
    compiled, err = _compiles_on_fixed(code, defect, repo)
    shots = 1
    toks_in, toks_out = r1["tokens_in"] or 0, r1["tokens_out"] or 0

    # shot 2 — compile-repair only
    if not compiled:
        msgs2 = repair_messages(defect, focal, r1["text"], err)
        dump("prompt_shot2.json", msgs2)
        r2 = client.chat(msgs2, seed=seed)
        dump("response_shot2.json", r2["raw"])
        code2 = extract_code(r2["text"])
        (cache / "shot2.cpp").write_text(code2)
        c2, err2 = _compiles_on_fixed(code2, defect, repo)
        shots = 2
        toks_in += r2["tokens_in"] or 0
        toks_out += r2["tokens_out"] or 0
        if c2:
            code, compiled, err = code2, True, ""
        else:
            err = err2

    final.write_text(code)
    meta = {
        "defect_id": defect["defect_id"], "project": defect["project"],
        "model": model_name, "model_version": r1["model_version"], "run_id": run_id,
        "condition": condition, "seed": seed,
        "timestamp": datetime.now().isoformat(timespec="seconds"),
        "temperature": r1.get("temperature"),
        "shots": shots, "compiled_on_fixed": compiled,
        "tokens_in": toks_in, "tokens_out": toks_out,
        "test_path": str(final),
        "compile_err_tail": (err or "")[-500:] if not compiled else "",
    }
    meta_p.write_text(json.dumps(meta, indent=2))
    return meta


if __name__ == "__main__":
    jsonl, defect_id, model = sys.argv[1:4]
    run_id = int(sys.argv[4]) if len(sys.argv) > 4 else 1
    recs = [json.loads(l) for l in Path(jsonl).read_text().splitlines() if l.strip()]
    d = next(r for r in recs if r["defect_id"] == defect_id)
    print(json.dumps(generate(d, model, run_id), indent=2))
