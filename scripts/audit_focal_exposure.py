#!/usr/bin/env python3
"""#5 — Focal-accessibility audit (de-risks the 99% non-compile claim + feeds the
2-annotator kappa on the accessibility labels). RUNS ON THE BOX, $0 API.

For each internal-campaign defect (nng/cppcheck/SPIRV, read from the result
jsonls) plus the arrow .cc candidates, it:
  1. slices the focal-function signature region (git blob at commit_after),
  2. extracts candidate symbol names from that region,
  3. git-greps each candidate in the project's PUBLIC headers (and checks for the
     project's export macro) and in the project's OWN test dir,
  4. prints a readable report and writes evidence to data/taxonomy/focal_exposure_audit.jsonl.

The SUGGESTED level is a heuristic seed for manual coding, NOT a final label:
  exported-public  : symbol declared in a public header WITH the export macro
                     (e.g. nng NNG_DECL, arrow ARROW_EXPORT) -> unit-testable like Tier A
  harness-internal : in an includable header but NO export macro, or referenced by
                     the project's own tests (e.g. cppcheck lib/*.h + test/test*.cpp)
                     -> reachable only via the project's bespoke harness
  unexposed        : no public-header hit at all -> internal, integration-only or unreachable

No checkout needed: `git grep <commit> -- <pathspec>` searches the tree at that commit.
Repo paths resolve from config/repos.json, then REPO_<PROJECT> env, then ./<guess>.
"""
import json
import os
import re
import subprocess
import sys
from pathlib import Path

sys.path[:0] = ["benchmarks"]
from defects4c_adapter import load_all, extract_focal     # noqa: E402

D4C = Path(os.environ.get("D4C_ROOT", "/home/azureuser/d4c"))
STUDY = Path(__file__).resolve().parent.parent
RESULTS = STUDY / "data" / "results"
OUT = STUDY / "data" / "taxonomy" / "focal_exposure_audit.jsonl"

# Per-project: public-header globs, export macro (None = app, no library export),
# and the project's own test dir globs.
PROJ = {
    "nanomsg___nng": dict(
        pub=[":(glob)include/nng/**/*.h", ":(glob)src/compat/nanomsg/*.h"],
        macro="NNG_DECL", tests=[":(glob)src/**/*_test.c", ":(glob)tests/**"]),
    "danmar___cppcheck": dict(
        pub=[":(glob)lib/*.h"],
        macro=None, tests=[":(glob)test/test*.cpp"]),
    "KhronosGroup___SPIRV-Tools": dict(
        pub=[":(glob)include/spirv-tools/*.h*"],
        macro="SPIRV_TOOLS_EXPORT", tests=[":(glob)test/**"]),
    "apache___arrow": dict(
        pub=[":(glob)cpp/src/arrow/**/*.h"],
        macro="ARROW_EXPORT", tests=[":(glob)cpp/src/arrow/**/*test*.cc"]),
}
# arrow .cc candidates to (re)check explicitly even if not in a campaign jsonl.
ARROW_EXTRA = ["apache___arrow@0b4fa2a2bf",  # table_builder.cc  RecordBatchBuilder::Flush
               "apache___arrow@68e0fa7499",  # concatenate.cc    Concatenate visitor
               "apache___arrow@c4f8436e25",  # compare.cc        BaseFloatingEquals
               "apache___arrow@4a7e19e118",  # function.h        Arity (known exported-public)
               "apache___arrow@3d0a9d58b6"]  # basic_decimal.h   BasicDecimal128 (known public)

KEYWORDS = {"if", "for", "while", "switch", "return", "sizeof", "do", "else",
            "case", "static_cast", "reinterpret_cast", "const_cast", "dynamic_cast",
            "RETURN_NOT_OK", "ARROW_RETURN_IF", "assert", "DCHECK", "and", "or", "not"}


def repo_for(project: str) -> Path:
    env = os.environ.get("REPO_" + project.split("___")[-1].upper().replace("-", "_"))
    if env:
        return Path(env)
    repos = json.loads((STUDY / "config" / "repos.json").read_text())
    if project in repos:
        return Path(repos[project])
    # fallbacks: ~/<short>, ./<short>
    short = project.split("___")[-1]
    for c in [Path.home() / short, STUDY.parent / (short + "_spike"), Path.cwd() / short]:
        if (c / ".git").is_dir():
            return c
    return Path.home() / short          # last resort (will error visibly)


def gitgrep(repo: Path, commit: str, pat: str, pathspec: list) -> list:
    try:
        r = subprocess.run(["git", "-C", str(repo), "grep", "-nE", pat, commit, "--", *pathspec],
                           capture_output=True, text=True, timeout=60)
        return [l for l in r.stdout.splitlines() if l.strip()]
    except Exception:
        return []


def candidates(region: str) -> list:
    cands = []
    cands += re.findall(r"\b([A-Za-z_]\w*::[A-Za-z_~]\w*)\s*\(", region)   # Class::method
    cands += re.findall(r"\b([a-z][a-z0-9_]*_[a-z0-9_]+)\s*\(", region)    # snake_case C funcs
    cands += re.findall(r"\b([A-Z][A-Za-z0-9_]+)\s*\(", region)            # PascalCase free fns
    # for a qualified Class::method, ALSO probe the bare Class name — export macros
    # (ARROW_EXPORT, NNG_DECL) usually sit on the `class EXPORT Foo` line, not the method.
    expanded = []
    for c in cands:
        expanded.append(c)
        if "::" in c:
            expanded.append(c.split("::")[0])
    seen, out = set(), []
    for c in expanded:
        bare = c.split("::")[-1]
        if c in seen or bare in KEYWORDS or len(bare) < 3:
            continue
        seen.add(c); out.append(c)
    return out[:8]


def commit_present(repo: Path, commit: str) -> bool:
    r = subprocess.run(["git", "-C", str(repo), "cat-file", "-e", commit + "^{commit}"],
                       capture_output=True)
    return r.returncode == 0


def main():
    # gather defect_ids actually run in the internal campaigns
    ran = set()
    for f in ["campaign_internal.jsonl", "campaign_internal_v2.jsonl", "campaign_spirv.jsonl"]:
        p = RESULTS / f
        if p.exists():
            for line in p.read_text().splitlines():
                if line.strip():
                    ran.add(json.loads(line)["defect_id"])
    targets = sorted(ran | set(ARROW_EXTRA))
    by_id = {d["defect_id"]: d for d in load_all(D4C)}

    OUT.parent.mkdir(parents=True, exist_ok=True)
    fout = open(OUT, "w")
    print(f"auditing {len(targets)} defects -> {OUT}\n", flush=True)
    levels = {}
    for did in targets:
        d = by_id.get(did)
        if not d:
            print(f"### {did}  -- NOT FOUND in adapter (skip)\n", flush=True); continue
        proj, cfg = d["project"], PROJ.get(d["project"])
        repo = repo_for(proj)
        fs, fe = d["func_start"], d["func_end"]
        ext = (d["focal_src"] or "").rsplit(".", 1)[-1]
        print(f"### {did}   focal={d['focal_src']} L{fs}-{fe}  (proxy={'PUB' if ext in ('h','hpp','hh') else 'int'})")
        if not repo.exists() or not commit_present(repo, d["commit_after"]):
            print(f"  !! repo/commit missing: repo={repo} present={repo.exists()} "
                  f"commit={d['commit_after'][:10]} -> set REPO_{proj.split('___')[-1].upper()} env\n", flush=True)
            fout.write(json.dumps({"defect_id": did, "error": "repo_or_commit_missing",
                                   "repo": str(repo)}) + "\n"); continue
        # signature region above the hunk
        try:
            blob = subprocess.run(["git", "-C", str(repo), "show", f"{d['commit_after']}:{d['focal_src']}"],
                                  capture_output=True, text=True, check=True).stdout.splitlines()
        except subprocess.CalledProcessError:
            print("  !! cannot read focal blob\n", flush=True); continue
        region = "\n".join(blob[max(0, fs - 18):fs + 1])
        anon_ns = bool(re.search(r"namespace\s*\{", "\n".join(blob[:fs]))) and \
            "namespace {" in "\n".join(blob[max(0, fs - 40):fs])
        print("  --- signature region ---")
        for ln in blob[max(0, fs - 8):fs + 1]:
            print("   |", ln[:100])
        cands = candidates(region)
        print(f"  candidates: {cands}   anon-namespace nearby: {anon_ns}")
        ev = {"defect_id": did, "project": proj, "focal_src": d["focal_src"],
              "proxy": "PUB" if ext in ("h", "hpp", "hh") else "int",
              "anon_namespace": anon_ns, "candidates": cands, "pub_hits": {}, "test_hits": {}, "macro": False}
        for c in cands:
            ph = gitgrep(repo, d["commit_after"], r"\b" + re.escape(c.split("::")[-1]) + r"\b", cfg["pub"]) if cfg else []
            th = gitgrep(repo, d["commit_after"], r"\b" + re.escape(c.split("::")[-1]) + r"\b", cfg["tests"]) if cfg else []
            if ph:
                ev["pub_hits"][c] = len(ph)
                if cfg and cfg["macro"] and any(cfg["macro"] in h for h in ph):
                    ev["macro"] = True
                print(f"    [{c}] public-hdr hits={len(ph)}  e.g. {ph[0][:90] if ph else ''}")
            if th:
                ev["test_hits"][c] = len(th)
                print(f"    [{c}] project-TEST hits={len(th)}")
        # heuristic level
        if ev["macro"]:
            lvl = "exported-public"
        elif ev["pub_hits"] or ev["test_hits"]:
            lvl = "harness-internal/integration (REVIEW)"
        else:
            lvl = "unexposed-internal"
        ev["suggested_level"] = lvl
        levels[lvl] = levels.get(lvl, 0) + 1
        print(f"  => SUGGESTED: {lvl}\n", flush=True)
        fout.write(json.dumps(ev) + "\n")
    fout.close()
    print("=== SUMMARY (suggested levels — MANUAL review required) ===")
    for k, v in sorted(levels.items()):
        print(f"  {v:3d}  {k}")
    print(f"\nevidence -> {OUT}\nNOTE: 'exported-public' on an internal-campaign defect = a MISLABEL to fix "
          "(its 99%-non-compile is model API-misuse, not a reachability wall).")


if __name__ == "__main__":
    main()
