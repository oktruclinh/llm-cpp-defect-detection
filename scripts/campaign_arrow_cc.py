#!/usr/bin/env python3
"""#4 — arrow .cc focals: 1 exported-public + 2 integration-reachable (re-audit C17).

Three arrow Tier-B defects whose focal is DEFINED in a .cc (proxy mislabelled them
"internal") but is in fact reachable:
  N=1  0b4fa2a2bf  table_builder.cc   RecordBatchBuilder::Flush  -> EXPORTED-PUBLIC
                   (class ARROW_EXPORT; directly unit-testable like Tier-A) -- the SPIKE
  N=2  68e0fa7499  concatenate.cc     Concatenate visitor        -> integration-reachable
                   (call public ARROW_EXPORT Concatenate())
  N=3  c4f8436e25  compare.cc         BaseFloatingEquals (anon)  -> integration-reachable
                   (call public ArrayApproxEquals())

WHY: (a) lifts exported-public n from 2 -> 3 WITHIN Defects4C (keeps human-validated
ground truth); (b) concatenate/compare are a direct empirical demo of the C17 reframe
— an isolation test can't name the focal, but the GUIDED public-API prompt may route
through the public entry point and reach it. Either outcome is a result.

RUN SPIKE FIRST: ARROW_N=1 validates the (UNVERIFIED) arrow build+link on table_builder
before spending on the rest. arrow build is LONG — see BOX_SESSION_4_5.md. cwd=study/.
Output: data/results/campaign_arrow_cc.jsonl (separate file; discardable if build breaks).
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
N = int(os.environ.get("ARROW_N", "1"))                 # default 1 = table_builder spike
MODELS = os.environ.get("ARROW_MODELS", "deepseek-chat,qwen3-coder,gemini-2.5-flash").split(",")
RUNS = int(os.environ.get("ARROW_RUNS", "2"))

# fixed order: exported-public spike first, then the two integration-reachable demos
ORDER = ["apache___arrow@0b4fa2a2bf", "apache___arrow@68e0fa7499", "apache___arrow@c4f8436e25"]
by_id = {d["defect_id"]: d for d in load_all(D4C)}
defects = [by_id[i] for i in ORDER[:N] if i in by_id]

print(f"ARROW_N={N}  defects={len(defects)} x {len(MODELS)} models x {RUNS} runs "
      f"= {len(defects)*len(MODELS)*RUNS} cells   models={MODELS}", flush=True)
for d in defects:
    print("  target:", d["defect_id"], "focal:", d.get("focal_src"),
          f"L{d.get('func_start')}-{d.get('func_end')}", flush=True)

Path("data/results").mkdir(parents=True, exist_ok=True)
out = open("data/results/campaign_arrow_cc.jsonl", "a")
for d in defects:
    for m in MODELS:
        for run in range(1, RUNS + 1):
            t0 = time.time()
            try:
                meta = generate(d, m, run, condition="guided")
                o = evaluate(Path(meta["test_path"]), d, repo_for(d), repeat=3)
                row = {"defect_id": d["defect_id"], "project": d["project"], "model": m,
                       "run": run, "focal_src": d.get("focal_src"),
                       "outcome": o["outcome"], "compiled": o["compiled_fixed"],
                       "n_valid": o["n_valid"], "detected": o["real_bug_detected"]}
            except Exception as e:
                row = {"defect_id": d["defect_id"], "project": d["project"], "model": m,
                       "run": run, "outcome": "ERROR", "err": str(e)[:300]}
            out.write(json.dumps(row) + "\n")
            out.flush()
            print(f"{d['defect_id'][:30]:30s} {m[:8]:8s} r{run} -> {row['outcome']:16s} "
                  f"comp={row.get('compiled')} det={row.get('detected')} ({time.time()-t0:.0f}s)", flush=True)
print("CAMPAIGN ARROW-CC DONE", flush=True)
