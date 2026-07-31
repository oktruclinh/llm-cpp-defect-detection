#!/usr/bin/env python3
"""Real-bug detection — the MAIN, tool-free signal (KSE2026 §5.1).

Standard fault-detection protocol (mirrors Yang et al. ASE2024 / Wang et al.):
generate on the FIXED code, then work at the TEST level, not the suite level:
  1. Run the suite on FIXED. Tests that FAIL on correct code have a wrong/flaky
     oracle -> they are INVALID and excluded (their count is itself an RQ1/RQ3
     metric). Tests that PASS on fixed are the VALID subset.
  2. Run ONLY the valid subset on BUGGY. If any valid test fails -> the suite
     detects the real bug.
This avoids discarding a whole suite just because a few hallucinated assertions
fail on correct code.
"""
import json
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "harness"))
from cpp_harness import build_and_run  # noqa: E402
from coverage import hunk_coverage    # noqa: E402


def classify(out: dict) -> str:
    if not out["compiled_fixed"]:
        return "non_compilable"           # RQ1
    if out["n_valid"] == 0:
        return "no_valid_tests"           # every test wrong/flaky on correct code -> RQ1/RQ3
    if not out["compiled_buggy"]:
        return "buggy_build_error"        # rare: focal swap breaks the test's build
    if out["real_bug_detected"]:
        return "detected"
    if out.get("reached_buggy"):
        return "reached_not_detected"     # valid tests run the bug but miss it -> RQ3 (weak oracle)
    if out.get("reached_buggy") is False:
        return "not_reached"              # coverage gap -> RQ2
    return "missed_unknown_reach"         # coverage tool inconclusive


def evaluate(test_cpp: Path, defect: dict, repo: Path, with_coverage: bool = True,
             repeat: int = 5) -> dict:
    """`repeat` = executions per version for flakiness control. A test is VALID only if
    it passes ALL runs on fixed; DETECTING only if it fails ALL runs on buggy. Reports
    both the test-level rate (drop invalid tests) and the strict suite-level lower bound
    (suite must be entirely valid + non-flaky)."""
    out = {
        "defect_id": defect["defect_id"], "project": defect["project"],
        "repeat": repeat, "compiled_fixed": False, "compiled_buggy": False,
        "n_tests": 0, "n_valid": 0, "n_invalid_on_fixed": 0,
        "n_flaky_fixed": 0, "n_flaky_buggy": 0, "n_detecting": 0,
        "real_bug_detected": None,          # test-level (drop invalid/flaky)
        "real_bug_detected_strict": None,   # suite-level lower bound (clean suite only)
        "invalid_on_fixed_tests": [], "flaky_fixed_tests": [], "detecting_tests": [],
        "compile_err_fixed": "", "compile_err_buggy": "",
    }

    def split(pass_counts, n):
        stable_pass = [t for t, c in pass_counts.items() if c == n]
        stable_fail = [t for t, c in pass_counts.items() if c == 0]
        flaky = [t for t, c in pass_counts.items() if 0 < c < n]
        return stable_pass, stable_fail, flaky

    with tempfile.TemporaryDirectory() as td:
        wd = Path(td)
        # 1) FIXED
        fx = build_and_run(test_cpp, defect, repo, "fixed", wd, repeat=repeat)
        out["compiled_fixed"] = fx.compiled
        if not fx.compiled:
            out["compile_err_fixed"] = fx.compile_err[-800:]
            out["outcome"] = classify(out)
            return out
        pc, n = fx.extra.get("pass_counts", {}), fx.extra.get("n_runs", 1)
        valid, invalid, flaky_f = split(pc, n)
        out["n_tests"] = len(pc)
        out["n_valid"] = len(valid)
        out["n_invalid_on_fixed"] = len(invalid)
        out["n_flaky_fixed"] = len(flaky_f)
        out["invalid_on_fixed_tests"] = invalid
        out["flaky_fixed_tests"] = flaky_f
        if not valid:
            out["outcome"] = classify(out)
            return out
        # 2) BUGGY — only the valid (stable-pass-on-fixed) subset
        bg = build_and_run(test_cpp, defect, repo, "buggy", wd,
                           gtest_filter=":".join(valid), repeat=repeat)
        out["compiled_buggy"] = bg.compiled
        if not bg.compiled:
            out["compile_err_buggy"] = bg.compile_err[-800:]
            out["outcome"] = classify(out)
            return out
        pb, nb = bg.extra.get("pass_counts", {}), bg.extra.get("n_runs", 1)
        detecting = [t for t in valid if pb.get(t, nb) == 0]          # stable-fail on buggy
        flaky_b = [t for t in valid if 0 < pb.get(t, nb) < nb]
        out["n_detecting"] = len(detecting)
        out["n_flaky_buggy"] = len(flaky_b)
        out["detecting_tests"] = detecting
        out["real_bug_detected"] = len(detecting) > 0
        # strict lower bound: suite had NO invalid + NO flaky tests on fixed
        out["real_bug_detected_strict"] = bool(
            out["real_bug_detected"] and out["n_invalid_on_fixed"] == 0
            and out["n_flaky_fixed"] == 0)
        if with_coverage and not out["real_bug_detected"]:
            cov = hunk_coverage(test_cpp, defect, repo, "buggy", wd)
            out["reached_buggy"] = bool(cov.get("hunk_executed")) if cov.get("compiled") else None
            out["coverage"] = cov
    out["outcome"] = classify(out)
    return out


if __name__ == "__main__":
    jsonl, defect_id, repo, test_cpp = sys.argv[1:5]
    recs = [json.loads(l) for l in Path(jsonl).read_text().splitlines() if l.strip()]
    defect = next(r for r in recs if r["defect_id"] == defect_id)
    print(json.dumps(evaluate(Path(test_cpp), defect, Path(repo)), indent=2))
