#!/usr/bin/env python3
"""Cohen's Kappa for a coding sheet (label_A vs label_B), over rows where BOTH are filled.
Usage: kappa.py data/taxonomy/rq1_coding_sheet.csv"""
import csv
import sys
from collections import Counter
from pathlib import Path


def cohen_kappa(a, b):
    n = len(a)
    if n == 0:
        return None, 0.0, 0
    po = sum(1 for x, y in zip(a, b) if x == y) / n
    ca, cb = Counter(a), Counter(b)
    cats = set(a) | set(b)
    pe = sum((ca[c] / n) * (cb[c] / n) for c in cats)
    k = (po - pe) / (1 - pe) if pe < 1 else 1.0
    return k, po, n


def main():
    path = Path(sys.argv[1])
    rows = list(csv.DictReader(path.open()))
    pairs = [(r["coder1"].strip(), r["coder2"].strip()) for r in rows
             if r.get("coder1", "").strip() and r.get("coder2", "").strip()]
    if not pairs:
        print(f"{path.name}: no rows with BOTH coder1 and coder2 filled yet "
              f"({len(rows)} cases total). Each coder fills their column independently, then re-run.")
        return
    a, b = zip(*pairs)
    k, po, n = cohen_kappa(list(a), list(b))
    interp = ("poor" if k < 0.2 else "fair" if k < 0.4 else "moderate" if k < 0.6
              else "substantial" if k < 0.8 else "almost perfect")
    print(f"{path.name}: n={n} double-coded | observed agreement={100*po:.0f}% | "
          f"Cohen's κ={k:.3f} ({interp})")


if __name__ == "__main__":
    main()
