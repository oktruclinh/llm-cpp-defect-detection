#!/usr/bin/env python3
"""Exp 3 (internal-batch v2): tighten the "internal-focal => non-compile" CI.

Adds MORE internal-focal Tier-C defects beyond the v1 sample (nng×3 + cppcheck×5).
Phase 1 (default, BATCH=cppcheck): +10 NEW cppcheck defects (proven recipe, C++,
zero new infra). Phase 2 (BATCH=cppcheck+spirv): also try 5 SPIRV-Tools defects
(C++, recipe UNVERIFIED — will validate live; failures are caught per-cell).

Same protocol as v1: 3 models (deepseek/qwen/gemini, NO gpt-5.4 — keep internal
pooled comparable), 2 runs, GUIDED prompt, repeat=3 flakiness, build-caching
(defect OUTER -> project built once per defect). Writes to a SEPARATE results file
so v1 data is never clobbered. Run on the box with cwd=study/.
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
BATCH = os.environ.get("BATCH", "cppcheck")             # "cppcheck" | "cppcheck+spirv"
MODELS = ["deepseek-chat", "qwen3-coder", "gemini-2.5-flash"]
RUNS = 2

# v1 already ran these 5 cppcheck defects — exclude by ID (robust to load ordering).
CPPCHECK_DONE = {
    "danmar___cppcheck@099b4435c3", "danmar___cppcheck@4996ec190e",
    "danmar___cppcheck@4ad90bf6f1", "danmar___cppcheck@caa6ff7c2a",
    "danmar___cppcheck@d0b6079a83",
}

alld = load_all(D4C)
cppcheck_new = [d for d in alld
                if d["project"] == "danmar___cppcheck"
                and d["defect_id"] not in CPPCHECK_DONE][:10]
defects = list(cppcheck_new)
if "spirv" in BATCH:
    spirv = [d for d in alld if d["project"] == "KhronosGroup___SPIRV-Tools"][:5]
    defects += spirv

print(f"BATCH={BATCH}  defects={len(defects)} "
      f"(cppcheck={len(cppcheck_new)}"
      f"{', spirv='+str(len(defects)-len(cppcheck_new)) if 'spirv' in BATCH else ''})  "
      f"x {len(MODELS)} models x {RUNS} runs = {len(defects)*len(MODELS)*RUNS} cells",
      flush=True)
for d in defects:
    print("  target:", d["defect_id"], "focal:", d.get("focal_src"), flush=True)

Path("data/results").mkdir(parents=True, exist_ok=True)
out = open("data/results/campaign_internal_v2.jsonl", "a")
for d in defects:                       # defect OUTER -> build reused across models/runs
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
                       "run": run, "outcome": "ERROR", "err": str(e)[:200]}
            out.write(json.dumps(row) + "\n")
            out.flush()
            print(f"{d['defect_id'][:26]:26s} {m[:8]:8s} r{run} -> {row['outcome']:16s} "
                  f"comp={row.get('compiled')} ({time.time()-t0:.0f}s)", flush=True)
print("CAMPAIGN INTERNAL v2 DONE", flush=True)
