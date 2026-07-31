#!/usr/bin/env python3
"""[#23 step 1] Apply the deterministic keyword classifier to the re-compiled full errors and
write data/taxonomy/rq1_auto.csv (the AUTOMATED labels = primary labeling for RQ1). A human then
AUDITS these (analysis/make_rq1_audit_html.py) -> accuracy, NOT kappa (RQ1 is mechanical).

  python3 analysis/rq1_classify.py
"""
import csv
import sys
from collections import Counter
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(STUDY / "analysis"))
from taxonomy_rq1 import categorize, first_error          # noqa: E402

TAX = STUDY / "data/taxonomy"
# label prefix -> numeric code, aligned with RQ1 codebook v2 (11=fence retired as a tooling
# artifact; 12=test-harness/macro misuse). Order matters: check the distinctive macro prefix.
CODE_BY_PREFIX = [
    (12, "Test-harness"), (1, "UDL/namespace"), (2, "static_assert"),
    (3, "Hallucinated/wrong API"), (4, "Missing include"),
    (5, "Template / overload deduction"), (6, "Access control"),
    (7, "Reference / value-category"), (8, "Type mismatch"), (9, "Malformed C++ syntax"),
]


def code_of(label):
    for code, pref in CODE_BY_PREFIX:
        if label.startswith(pref):
            return code
    return 10  # Other / uncategorized


def main():
    idx = list(csv.DictReader((TAX / "rq1_index.csv").open()))
    out = open(TAX / "rq1_auto.csv", "w", newline="")
    w = csv.DictWriter(out, fieldnames=["id", "defect_id", "model", "run_id", "project",
                                        "auto_code", "auto_label", "first_error"])
    w.writeheader()
    labs = []
    for r in idx:
        err = (TAX / r["err_file"]).read_text() if (TAX / r["err_file"]).is_file() else ""
        label = categorize(err)
        labs.append(label)
        w.writerow({"id": r["id"], "defect_id": r["defect_id"], "model": r["model"],
                    "run_id": r["run_id"], "project": r["project"],
                    "auto_code": code_of(label), "auto_label": label,
                    "first_error": first_error(err)[:200]})
    out.close()
    n = len(labs)
    other = labs.count("Other / uncategorized")
    print(f"wrote data/taxonomy/rq1_auto.csv  ({n} cells)")
    print(f"Other = {other}/{n} ({100*other/n:.1f}%)\n")
    for k, v in Counter(labs).most_common():
        print(f"  {v:3d}  {k}")


if __name__ == "__main__":
    main()
