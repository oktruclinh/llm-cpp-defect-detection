#!/usr/bin/env python3
"""Prompt robustness arm: does a richer PUBLIC-API context lift DETECTION (not just
compilation)? Paired comparison guided <-> api_rich on the SAME defects + model, on the
two header-only projects we can build on the laptop ($0 compute): fmt (19) + cpp_peglib (10).
1 model (deepseek, the C11 ablation anchor). guided cells reuse cache (free); api_rich is fresh.
Writes data/results/campaign_api_rich.jsonl. Run AFTER campaign_cpp_peglib finishes. cwd=study/.
"""
import json, os, sys, time
from pathlib import Path
sys.path[:0] = ["generate", "harness", "measure", "benchmarks", "scripts"]
from generate_tests import generate, repo_for          # noqa: E402
from detection import evaluate                          # noqa: E402
from defects4c_adapter import load_all                  # noqa: E402
from cpp_peglib_adapter import build_defects            # noqa: E402

D4C = Path(os.environ.get("D4C_ROOT", "/home/truclinh/Thesis/kse2026/defects4c/defectsc_tpl"))
MODEL = "deepseek-chat"
RUNS = int(os.environ.get("ARM_RUNS", "2"))
CONDITIONS = ["guided", "api_rich"]

fmt = [d for d in load_all(D4C) if d["project"] == "fmtlib___fmt"]   # Tier-A 19 header-only
peg = build_defects(setup=True)                                      # cpp_peglib 10
defects = fmt + peg
print(f"{len(defects)} defects (fmt {len(fmt)} + cpp_peglib {len(peg)}) x {len(CONDITIONS)} "
      f"conditions x {RUNS} runs, model={MODEL}", flush=True)

out = open("data/results/campaign_api_rich.jsonl", "a")
for d in defects:
    for cond in CONDITIONS:
        for run in range(1, RUNS + 1):
            t0 = time.time()
            try:
                meta = generate(d, MODEL, run, condition=cond)
                o = evaluate(Path(meta["test_path"]), d, repo_for(d), repeat=3)
                row = {"defect_id": d["defect_id"], "project": d["project"], "cond": cond,
                       "run": run, "outcome": o["outcome"], "compiled": o["compiled_fixed"],
                       "n_valid": o["n_valid"], "reached": o.get("reached_buggy"),
                       "detected": o["real_bug_detected"]}
            except Exception as e:
                row = {"defect_id": d["defect_id"], "project": d["project"], "cond": cond,
                       "run": run, "outcome": "ERROR", "err": str(e)[:200]}
            out.write(json.dumps(row) + "\n"); out.flush()
            print(f"{d['defect_id'][:26]:26s} {cond:8s} r{run} -> {row['outcome']:16s} "
                  f"comp={row.get('compiled')} det={row.get('detected')} ({time.time()-t0:.0f}s)", flush=True)
print("CAMPAIGN api_rich DONE", flush=True)
