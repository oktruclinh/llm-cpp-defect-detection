#!/usr/bin/env python3
"""BugsCpp cross-benchmark campaign: 10 cpp_peglib defects x 4 models x 2 runs (header-only,
laptop, $0 compute). Reuses the Tier-A harness via cpp_peglib_adapter. Tests whether the
two-level focal-accessibility pattern REPLICATES on a 2nd independent C/C++ benchmark.
Writes data/results/campaign_cpp_peglib.jsonl. cwd=study/.
"""
import json, sys, time
from pathlib import Path
sys.path[:0] = ["generate", "harness", "measure", "benchmarks", "scripts"]
from generate_tests import generate, repo_for          # noqa: E402
from detection import evaluate                          # noqa: E402
from cpp_peglib_adapter import build_defects            # noqa: E402

MODELS = ["deepseek-chat", "qwen3-coder", "gemini-2.5-flash", "gpt-5.4-or"]
RUNS = 2
defects = build_defects(setup=True)   # idempotent: reuses existing buggy-<id> tags
print(f"{len(defects)} defects x {len(MODELS)} models x {RUNS} runs = "
      f"{len(defects)*len(MODELS)*RUNS} cells", flush=True)

Path("data/results").mkdir(parents=True, exist_ok=True)
out = open("data/results/campaign_cpp_peglib.jsonl", "a")
for d in defects:
    for m in MODELS:
        for run in range(1, RUNS + 1):
            t0 = time.time()
            try:
                meta = generate(d, m, run, condition="guided")
                o = evaluate(Path(meta["test_path"]), d, repo_for(d), repeat=3)
                row = {"defect_id": d["defect_id"], "d_id": d["_id"], "model": m, "run": run,
                       "outcome": o["outcome"], "compiled": o["compiled_fixed"],
                       "n_valid": o["n_valid"], "reached": o.get("reached_buggy"),
                       "detected": o["real_bug_detected"]}
            except Exception as e:
                row = {"defect_id": d["defect_id"], "d_id": d["_id"], "model": m, "run": run,
                       "outcome": "ERROR", "err": str(e)[:200]}
            out.write(json.dumps(row) + "\n"); out.flush()
            print(f"d{d['_id']:>2} {m[:8]:8s} r{run} -> {row['outcome']:16s} "
                  f"comp={row.get('compiled')} det={row.get('detected')} ({time.time()-t0:.0f}s)", flush=True)
print("CAMPAIGN cpp_peglib DONE", flush=True)
