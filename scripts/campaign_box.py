#!/usr/bin/env python3
"""Real Tier-B/C campaign on the box: nng + cppcheck sample × 3 models, GUIDED prompt,
build-caching (defect-outer loop → project built once per defect, reused across models).
Appends rows to data/results/campaign_box.jsonl. Run with cwd=study/."""
import json
import sys
import time
from pathlib import Path

sys.path[:0] = ["generate", "harness", "benchmarks", "measure"]
from generate_tests import generate, repo_for          # noqa: E402
from detection import evaluate                          # noqa: E402
from defects4c_adapter import load_all                  # noqa: E402

ROOT = Path("/home/azureuser/d4c")
alld = load_all(ROOT)
defects = ([d for d in alld if d["project"] == "nanomsg___nng"]
           + [d for d in alld if d["project"] == "danmar___cppcheck"][:2])
models = ["deepseek-chat", "qwen3-coder", "gemini-2.5-flash"]

Path("data/results").mkdir(parents=True, exist_ok=True)
out = open("data/results/campaign_box.jsonl", "a")
for d in defects:                       # defect OUTER → build reused across models
    for m in models:
        t0 = time.time()
        try:
            meta = generate(d, m, 1, condition="guided")
            o = evaluate(Path(meta["test_path"]), d, repo_for(d), repeat=3)
            row = {"defect_id": d["defect_id"], "project": d["project"], "model": m,
                   "condition": "guided", "outcome": o["outcome"],
                   "compiled": o["compiled_fixed"], "n_valid": o["n_valid"],
                   "detected": o["real_bug_detected"]}
        except Exception as e:
            row = {"defect_id": d["defect_id"], "project": d["project"], "model": m,
                   "outcome": "ERROR", "err": str(e)[:200]}
        out.write(json.dumps(row) + "\n")
        out.flush()
        print(f"{d['defect_id'][:30]:30s} {m[:8]:8s} -> {row['outcome']:18s} "
              f"comp={row.get('compiled')} det={row.get('detected')} ({time.time()-t0:.0f}s)",
              flush=True)
print("CAMPAIGN BOX DONE", flush=True)
