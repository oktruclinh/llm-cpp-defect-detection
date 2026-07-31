#!/usr/bin/env python3
"""[#22a] Report metrics CONDITIONED ON COMPILATION, decoupling the headline
(execution≠detection / oracle weakness) from the non-compile rate.

Two layers:
  RQ1 (validity):     non-compilable % and no-valid-suite % over ALL cells.
  Headline (RQ2/RQ3): among VALID suites (compiled + ≥1 valid test), the
                      execution≠detection split — detected / reached-but-missed
                      (weak oracle) / never-reached (coverage gap).
The headline denominator excludes non-compile, so it is robust even if harder
tiers are mostly non-compilable (the §C10/R1 risk)."""
import glob
import json
import math
import sys
from collections import Counter
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent


def wilson(k, n, z=1.96):
    if n == 0:
        return (0.0, 0.0)
    p = k / n
    d = 1 + z * z / n
    c = (p + z * z / (2 * n)) / d
    h = (z * math.sqrt(p * (1 - p) / n + z * z / (4 * n * n))) / d
    return (max(0.0, c - h), min(1.0, c + h))


def pct(k, n):
    if n == 0:
        return f"{k}/{n} = n/a"
    lo, hi = wilson(k, n)
    return f"{k}/{n} = {100*k/n:.0f}% (CI {100*lo:.0f}-{100*hi:.0f}%)"


def report(rows, label):
    n = len(rows)
    oc = Counter(r.get("outcome") for r in rows)
    non_comp = oc.get("non_compilable", 0)
    no_valid = oc.get("no_valid_tests", 0)
    # VALID suites = compiled + >=1 valid test (the headline denominator)
    valid = [r for r in rows if r.get("outcome") in
             ("detected", "reached_not_detected", "not_reached")]
    nv = len(valid)
    detected = sum(1 for r in valid if r.get("outcome") == "detected")
    weak = sum(1 for r in valid if r.get("outcome") == "reached_not_detected")
    gap = sum(1 for r in valid if r.get("outcome") == "not_reached")
    print(f"\n### {label}  (N={n} cells)")
    print(f"  RQ1 validity:  non-compilable {pct(non_comp, n)} | no-valid-suite {pct(no_valid, n)}")
    print(f"  -- headline conditioned on VALID suites (denominator {nv}) --")
    print(f"  detected:              {pct(detected, nv)}")
    print(f"  reached-but-missed (weak oracle):  {pct(weak, nv)}")
    print(f"  never-reached (coverage gap):      {pct(gap, nv)}")
    if weak + gap > 0:
        print(f"  >> among MISSED valid suites: weak-oracle {pct(weak, weak+gap)} "
              f"vs coverage-gap {gap}/{weak+gap}")


def main():
    files = sys.argv[1:] or glob.glob(str(STUDY / "data/results/rows_tierA_*_fixed.jsonl"))
    pooled = []
    for f in sorted(files):
        rows = [json.loads(l) for l in Path(f).read_text().splitlines() if l.strip()]
        pooled += rows
        report(rows, Path(f).stem.replace("rows_tierA_", "").replace("_fixed", ""))
    if len(files) > 1:
        report(pooled, "POOLED (all models)")


if __name__ == "__main__":
    main()
