#!/usr/bin/env python3
"""[#23] Cohen's kappa from TWO independent coder files, joined on `id`.

Usage:
  python3 analysis/kappa_merge.py rq1
  python3 analysis/kappa_merge.py rq3

Reads data/taxonomy/<rq>_coder1.csv and _coder2.csv, computes kappa over the rows BOTH
coders labelled, and prints: observed agreement, Cohen's kappa (+interpretation), per-coder
label distribution, coverage gaps, and the exact DISAGREEMENTS (id + both labels) so the two
coders can adjudicate them face-to-face. Report the PRE-adjudication kappa in the paper, then
adjudicate -> the agreed labels become the final taxonomy counts.
"""
import csv
import sys
from collections import Counter
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
TAX = STUDY / "data/taxonomy"

LEGENDS = {
    "rq1": [
        "UDL/namespace", "static_assert/trait", "Hallucinated/wrong API",
        "Missing include/unknown type", "Template/overload deduction",
        "Access control", "Reference/value-category", "Type mismatch/conversion",
        "Malformed C++ syntax", "Other",
    ],
    "rq3": [
        "O1 Echo/self-confirming", "O2 Wrong API surface", "O3 Too-weak assertion",
        "O4 Insufficient input", "O5 Other/unclear",
    ],
}


def cohen_kappa(a, b):
    n = len(a)
    if n == 0:
        return None, 0.0, 0
    po = sum(1 for x, y in zip(a, b) if x == y) / n
    ca, cb = Counter(a), Counter(b)
    pe = sum((ca[c] / n) * (cb[c] / n) for c in set(a) | set(b))
    k = (po - pe) / (1 - pe) if pe < 1 else 1.0
    return k, po, n


def load(rq, coder):
    p = TAX / f"{rq}_coder{coder}.csv"
    if not p.exists():
        sys.exit(f"missing {p} -- run: python3 analysis/make_annotation_kit.py")
    out = {}
    for r in csv.DictReader(p.open()):
        lab = (r.get("label") or "").strip()
        if lab:
            out[r["id"].strip()] = lab
    return out


def name(rq, lab):
    leg = LEGENDS[rq]
    return f"{lab} {leg[int(lab) - 1]}" if lab.isdigit() and 1 <= int(lab) <= len(leg) else lab


def main():
    if len(sys.argv) < 2 or sys.argv[1] not in LEGENDS:
        sys.exit("usage: kappa_merge.py rq1|rq3")
    rq = sys.argv[1]
    c1, c2 = load(rq, 1), load(rq, 2)
    both = sorted(set(c1) & set(c2), key=lambda x: int(x) if x.isdigit() else 0)
    only1, only2 = set(c1) - set(c2), set(c2) - set(c1)

    print(f"== {rq.upper()}  (coder1={len(c1)} coded, coder2={len(c2)} coded) ==")
    if not both:
        print("  no rows labelled by BOTH coders yet -- fill the `label` column in each "
              "*_coder1.csv / *_coder2.csv, then re-run.")
        return

    a, b = [c1[i] for i in both], [c2[i] for i in both]
    k, po, n = cohen_kappa(a, b)
    interp = ("poor" if k < 0.2 else "fair" if k < 0.4 else "moderate" if k < 0.6
              else "substantial" if k < 0.8 else "almost perfect")
    print(f"  n double-coded = {n}")
    print(f"  observed agreement = {100 * po:.1f}%")
    print(f"  Cohen's kappa = {k:.3f}  ({interp})")
    if only1 or only2:
        print(f"  NOTE coverage gap: {len(only1)} rows only coder1, {len(only2)} only coder2 "
              f"(kappa uses the {n} both-coded rows only)")

    print("\n  per-coder distribution:")
    for who, c in (("coder1", Counter(a)), ("coder2", Counter(b))):
        dist = ", ".join(f"{name(rq, lab)}={cnt}" for lab, cnt in
                         sorted(c.items(), key=lambda kv: -kv[1]))
        print(f"    {who}: {dist}")

    dis = [(i, c1[i], c2[i]) for i in both if c1[i] != c2[i]]
    print(f"\n  DISAGREEMENTS to adjudicate ({len(dis)}/{n}):")
    for i, x, y in dis:
        print(f"    id {i:>3}:  coder1 = {name(rq, x):<32}  coder2 = {name(rq, y)}")
    if not dis:
        print("    (none -- perfect agreement)")


if __name__ == "__main__":
    main()
