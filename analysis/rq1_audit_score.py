#!/usr/bin/env python3
"""[#23 step 1] Score the human RQ1 audit (data/taxonomy/rq1_audit.csv exported from the audit
HTML): classifier accuracy (final vs auto), the corrected cells, and the FINAL category
distribution that goes in the paper. Accuracy -- not kappa -- is the right validity number for
this mechanical task.

  python3 analysis/rq1_audit_score.py
"""
import csv
from collections import Counter
from pathlib import Path

TAX = Path(__file__).resolve().parent.parent / "data/taxonomy"
CATS = ["UDL/namespace", "static_assert/trait", "Hallucinated/wrong API",
        "Missing include/unknown type", "Template/overload deduction", "Access control",
        "Reference/value-category", "Type mismatch/conversion", "Malformed C++ syntax", "Other"]


def main():
    p = TAX / "rq1_audit.csv"
    if not p.is_file():
        print("chua co rq1_audit.csv -- mo data/taxonomy/rq1_audit.html, duyet, bam Xuat CSV.")
        return
    rows = list(csv.DictReader(p.open()))
    n = len(rows)
    agree = sum(1 for r in rows if r["auto_code"].strip() == r["final_code"].strip())
    print(f"=== RQ1 classifier audit ({n} cells) ===")
    print(f"accuracy (final == auto) = {agree}/{n} = {100*agree/n:.1f}%\n")
    corr = [r for r in rows if r["auto_code"].strip() != r["final_code"].strip()]
    print(f"human corrected {len(corr)} cell(s):")
    for r in corr:
        a, f = int(r["auto_code"]), int(r["final_code"])
        print(f"  id {r['id']:>2}: {a}.{CATS[a-1]} -> {f}.{CATS[f-1]}"
              + (f"   ({r['notes']})" if r.get("notes") else ""))
    print("\n=== FINAL category distribution (post-audit; for the paper) ===")
    for code, c in sorted(Counter(int(r["final_code"]) for r in rows).items(),
                          key=lambda kv: -kv[1]):
        print(f"  {c:3d}  {code}. {CATS[code-1]}")


if __name__ == "__main__":
    main()
