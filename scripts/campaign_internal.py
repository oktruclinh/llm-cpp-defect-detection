#!/usr/bin/env python3
"""Exp 2: broaden the internal-focal Tier-B/C evidence (was 5 defects, 1 run → make
the ~100% non-compile claim robust). nng(3) + cppcheck(5) × 3 models × 2 runs, GUIDED,
build-caching (defect-outer → project built once per defect). Run with cwd=study/."""
import json
import sys
import time
from pathlib import Path

sys.path[:0] = ["generate", "harness", "benchmarks", "measure"]
from generate_tests import generate, repo_for          # noqa: E402
from detection import evaluate                          # noqa: E402
from defects4c_adapter import load_all                  # noqa: E402

alld = load_all(Path("/home/azureuser/d4c"))
defects = ([d for d in alld if d["project"] == "nanomsg___nng"]
           + [d for d in alld if d["project"] == "danmar___cppcheck"][:5])
models = ["deepseek-chat", "qwen3-coder", "gemini-2.5-flash"]
RUNS = 2

Path("data/results").mkdir(parents=True, exist_ok=True)
out = open("data/results/campaign_internal.jsonl", "a")
for d in defects:                       # defect OUTER → build reused across models/runs
    for m in models:
        for run in range(1, RUNS + 1):
            t0 = time.time()
            try:
                meta = generate(d, m, run, condition="guided")
                o = evaluate(Path(meta["test_path"]), d, repo_for(d), repeat=3)
                row = {"defect_id": d["defect_id"], "project": d["project"], "model": m,
                       "run": run, "outcome": o["outcome"], "compiled": o["compiled_fixed"],
                       "n_valid": o["n_valid"], "detected": o["real_bug_detected"]}
            except Exception as e:
                row = {"defect_id": d["defect_id"], "project": d["project"], "model": m,
                       "run": run, "outcome": "ERROR", "err": str(e)[:200]}
            out.write(json.dumps(row) + "\n")
            out.flush()
            print(f"{d['defect_id'][:26]:26s} {m[:8]:8s} r{run} -> {row['outcome']:16s} "
                  f"comp={row.get('compiled')} ({time.time()-t0:.0f}s)", flush=True)
print("CAMPAIGN INTERNAL DONE", flush=True)
