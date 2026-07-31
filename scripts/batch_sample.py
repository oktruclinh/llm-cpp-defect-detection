#!/usr/bin/env python3
"""First REAL Tier-B/C batch on the box: a small sample (nng + a few cppcheck) ×
DeepSeek × 1 run, full pipeline (generate -> build project -> compile/link -> detect).
Appends rows to data/results/box_sample.jsonl. Run with cwd=study/."""
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
sample = ([d for d in alld if d["project"] == "nanomsg___nng"]
          + [d for d in alld if d["project"] == "danmar___cppcheck"][:2])

Path("data/results").mkdir(parents=True, exist_ok=True)
out = open("data/results/box_sample.jsonl", "a")
for d in sample:
    t0 = time.time()
    try:
        meta = generate(d, "deepseek-chat", 1, condition="fixed")
        o = evaluate(Path(meta["test_path"]), d, repo_for(d), repeat=3)
        row = {"defect_id": d["defect_id"], "project": d["project"],
               "outcome": o["outcome"], "compiled": o["compiled_fixed"],
               "n_valid": o["n_valid"], "n_invalid": o.get("n_invalid_on_fixed"),
               "detected": o["real_bug_detected"]}
    except Exception as e:
        row = {"defect_id": d["defect_id"], "project": d["project"],
               "outcome": "ERROR", "err": str(e)[:200]}
    out.write(json.dumps(row) + "\n")
    out.flush()
    print(d["defect_id"], "->", row["outcome"], f"({time.time()-t0:.0f}s)", flush=True)
print("BOX SAMPLE DONE", flush=True)
