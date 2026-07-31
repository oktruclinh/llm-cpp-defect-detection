#!/usr/bin/env python3
"""P1 stats on the hardened Tier-A data (no new generation). Wilson CIs for the
headline rates, per-defect detection stability across runs, and a contamination
check (defect commit dates vs model era)."""
import json
import math
import sys
from collections import Counter, defaultdict
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(STUDY / "benchmarks"))
from defects4c_adapter import load_all  # noqa: E402
DEFECTS4C_ROOT = "/home/truclinh/Thesis/kse2026/defects4c/defectsc_tpl"


def wilson(k, n, z=1.96):
    if n == 0:
        return (0.0, 0.0)
    p = k / n
    d = 1 + z * z / n
    c = (p + z * z / (2 * n)) / d
    h = (z * math.sqrt(p * (1 - p) / n + z * z / (4 * n * n))) / d
    return (max(0.0, c - h), min(1.0, c + h))


def pct(k, n):
    lo, hi = wilson(k, n)
    return f"{k}/{n} = {100*k/n:.0f}% (95% CI {100*lo:.0f}–{100*hi:.0f}%)"


def main():
    rows = [json.loads(l) for l in
            (STUDY / "data/results/rows_tierA_deepseek-chat_fixed.jsonl").read_text().splitlines()
            if l.strip()]
    n = len(rows)
    det = sum(1 for r in rows if r["real_bug_detected"])
    strict = sum(1 for r in rows if r["real_bug_detected_strict"])
    nc = sum(1 for r in rows if r["outcome"] == "non_compilable")
    inv = sum(r.get("n_invalid_on_fixed") or 0 for r in rows)
    tot = sum(r.get("n_tests") or 0 for r in rows)

    print(f"=== HEADLINE RATES with 95pct Wilson CI (Tier A fixed, {n} cells) ===")
    print("  detected (test-level): ", pct(det, n))
    print("  detected (strict)    : ", pct(strict, n))
    print("  non-compilable       : ", pct(nc, n))
    print("  wrong-oracle tests   : ", pct(inv, tot))

    # per-defect detection stability across runs
    bydef = defaultdict(list)
    for r in rows:
        bydef[r["defect_id"]].append(bool(r["real_bug_detected"]))
    stab = Counter(sum(v) for v in bydef.values())  # how many runs detected (0..3)
    print(f"\n=== per-defect detection stability (runs detected / {len(bydef)} defects) ===")
    for k in sorted(stab):
        print(f"  detected in {k}/3 runs: {stab[k]} defects")
    atk = sum(1 for v in bydef.values() if any(v))
    print("  detection@3 (union)  : ", pct(atk, len(bydef)), "defects")

    # contamination: commit dates vs model era
    recs = {d["defect_id"]: d for d in load_all(Path(DEFECTS4C_ROOT))}
    dates = sorted((recs[d]["commit_date"] or "?")[:10] for d in bydef if d in recs)
    print("\n=== CONTAMINATION (defect fix-commit dates vs model era) ===")
    print(f"  date range: {dates[0]} … {dates[-1]}  (n={len(dates)})")
    pre2020 = sum(1 for x in dates if x < "2020-01-01")
    print(f"  fixes before 2020: {pre2020}/{len(dates)} — all predate any 2026-era model cutoff")
    print("  => subjects are old public code, almost certainly in training data; LOW detection")
    print("     holds DESPITE likely memorization (works against an over-pessimistic reading).")


if __name__ == "__main__":
    main()
