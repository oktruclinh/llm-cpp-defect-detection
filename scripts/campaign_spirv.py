#!/usr/bin/env python3
"""Phase 2: SPIRV-Tools internal-focal batch (3rd C++ project, generalization).

UNVERIFIED recipe (abseil/git-sync-deps) — run with SPIRV_N=1 FIRST to validate the
build+link on one defect, inspect WHY rows fail (artifact vs finding) before the rest.
Focal funcs are all source/opt/*.cpp (optimizer passes, internal). Separate output
file so it can be discarded if the recipe proves broken. Run on box, cwd=study/.
"""
import json
import os
import sys
import time
from pathlib import Path

sys.path[:0] = ["generate", "harness", "benchmarks", "measure"]
from generate_tests import generate, repo_for          # noqa: E402
from detection import evaluate                          # noqa: E402
from defects4c_adapter import load_all                  # noqa: E402

D4C = Path(os.environ.get("D4C_ROOT", "/home/azureuser/d4c"))
N = int(os.environ.get("SPIRV_N", "1"))                 # default 1 = validation run
MODELS = ["deepseek-chat", "qwen3-coder", "gemini-2.5-flash"]
RUNS = 2

alld = load_all(D4C)
defects = [d for d in alld if d["project"] == "KhronosGroup___SPIRV-Tools"][:N]
print(f"SPIRV_N={N}  defects={len(defects)} x {len(MODELS)} models x {RUNS} runs "
      f"= {len(defects)*len(MODELS)*RUNS} cells", flush=True)
for d in defects:
    print("  target:", d["defect_id"], "focal:", d.get("focal_src"), flush=True)

Path("data/results").mkdir(parents=True, exist_ok=True)
out = open("data/results/campaign_spirv.jsonl", "a")
for d in defects:
    for m in MODELS:
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
                       "run": run, "outcome": "ERROR", "err": str(e)[:300]}
            out.write(json.dumps(row) + "\n")
            out.flush()
            print(f"{d['defect_id'][:30]:30s} {m[:8]:8s} r{run} -> {row['outcome']:16s} "
                  f"comp={row.get('compiled')} ({time.time()-t0:.0f}s)", flush=True)
print("CAMPAIGN SPIRV DONE", flush=True)
