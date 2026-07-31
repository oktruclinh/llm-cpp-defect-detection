#!/usr/bin/env python3
"""Ablation A — does a CONTEXT-RICH prompt (build context + focal-file includes +
focal class surface + CityWalk-style rules) reduce non-compile vs the NAIVE prompt
(focal function only)? Compares condition 'fixed' (naive) vs 'context_rich' on a project.
Usage: ablation_a.py <project> [model]   (run with cwd=study/)"""
import sys
from collections import Counter
from pathlib import Path

sys.path[:0] = ["generate", "harness", "benchmarks", "measure"]
from generate_tests import generate, repo_for          # noqa: E402
from detection import evaluate                          # noqa: E402
from defects4c_adapter import load_all                  # noqa: E402

ROOT = sys.argv[3] if len(sys.argv) > 3 else "/home/truclinh/Thesis/kse2026/defects4c/defectsc_tpl"
project = sys.argv[1] if len(sys.argv) > 1 else "fmtlib___fmt"
model = sys.argv[2] if len(sys.argv) > 2 else "deepseek-chat"

recs = [d for d in load_all(Path(ROOT)) if d["project"] == project]
print(f"{project}: {len(recs)} defects, model={model}\n")

res = {}
for cond in ["fixed", "context_rich"]:
    oc = Counter()
    compiled = 0
    for d in recs:
        meta = generate(d, model, 1, condition=cond)
        o = evaluate(Path(meta["test_path"]), d, repo_for(d), repeat=3)
        oc[o["outcome"]] += 1
        if o["compiled_fixed"]:
            compiled += 1
        print(f"  [{cond:12s}] {d['defect_id'][:34]:34s} {o['outcome']:20s} "
              f"compiled={o['compiled_fixed']} detected={o['real_bug_detected']}", flush=True)
    res[cond] = (oc, compiled, len(recs))
    print()

print("===== ABLATION A SUMMARY =====")
for cond, (oc, comp, n) in res.items():
    print(f"{cond:13s}: compile-rate {comp}/{n} = {100*comp//n}%  | outcomes={dict(oc)}")
