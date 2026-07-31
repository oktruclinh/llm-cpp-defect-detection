#!/usr/bin/env python3
"""SPIKE: validate the BugsCpp cross-benchmark cheap-mine path on ONE cpp_peglib defect.
cpp-peglib is single-file header-only (peglib.h) → reuse the Tier-A header-only harness
unchanged. Defect 1 = CVE-2020-23914: perform_core() fails to reject a start rule with the
ignore operator (the fix sets ret=false; the buggy version drops it). Reachable via the
PUBLIC peg::parser API (invalid grammar -> (bool)parser == false). cwd=study/, $0 compute.
"""
import json, sys
from pathlib import Path
sys.path[:0] = ["generate", "harness", "measure", "benchmarks"]
from generate_tests import generate, repo_for   # noqa: E402
from detection import evaluate                   # noqa: E402

DEFECT = {
    "defect_id": "yhirose___cpp_peglib@0061f393de",
    "project": "yhirose___cpp_peglib",
    "focal_src": "peglib.h",
    "commit_after": "0061f393de54cf0326621c079dc2988336d1ebb3",   # FIXED (has ret=false)
    "commit_before": "97d5b1f20bcab6e2114ec0e2062e94efe159be1d",  # BUGGY (ret=false removed)
    "func_start": 3389, "func_end": 3460,                          # perform_core(...)
    "bug_type": "Validation omission: a start rule using the ignore operator '~' is not "
                "rejected (CVE-2020-23914). Public trigger: peg::parser with such a grammar "
                "should be invalid, i.e. (bool)parser == false.",
}
MODEL = sys.argv[1] if len(sys.argv) > 1 else "deepseek-chat"

print(f"=== SPIKE cpp_peglib defect 1 · model={MODEL} ===", flush=True)
print("repo:", repo_for(DEFECT), flush=True)
meta = generate(DEFECT, MODEL, 1, condition="guided")
tp = Path(meta["test_path"])
print("generated test:", tp, f"({tp.stat().st_size} bytes)", flush=True)
o = evaluate(tp, DEFECT, repo_for(DEFECT), repeat=3)
print("\n=== RESULT ===")
print(f"  outcome        : {o.get('outcome')}")
print(f"  compiled_fixed : {o.get('compiled_fixed')}")
print(f"  n_valid        : {o.get('n_valid')}")
print(f"  reached_buggy  : {o.get('reached_buggy')}")
print(f"  DETECTED bug   : {o.get('real_bug_detected')}")
if not o.get("compiled_fixed"):
    err = (o.get("compile_err_fixed") or o.get("compile_error") or "")[:1200]
    print("\n--- compile error (head) ---\n", err)
