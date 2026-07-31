#!/usr/bin/env python3
"""[#23] Build a TWO-HUMAN annotation kit so two INDEPENDENT coders (no peeking) can label
the RQ1 compile-failure + RQ3 oracle-weakness taxonomies -> Cohen's kappa (analysis/kappa_merge.py).

Produces per-coder BLANK files (each coder only ever opens their OWN file -> independence is
structural, not just a request). Labels are NUMERIC (1..N) to avoid string-typo mismatches that
would spuriously deflate kappa. Rebuilds from the FROZEN data/taxonomy/rq{1,3}_coding_sheet.csv
(the exact 74/47 cells the paper's taxonomy is based on) -- deliberately does NOT re-scan
data/raw, which now also holds the later api_rich/cpp_peglib cells we must not fold in.
"""
import csv
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
TAX = STUDY / "data/taxonomy"

# Numeric legend -- MUST stay in sync with analysis/kappa_merge.py and the RULES in taxonomy_rq1.py
RQ1_CATS = [
    "UDL/namespace (e.g. _hs without `using namespace ...::literals`)",
    "static_assert / unsatisfied trait (type not formattable, etc.)",
    "Hallucinated/wrong API (no such member / function / overload)",
    "Missing include / unknown type",
    "Template / overload deduction failure",
    "Access control (private / protected member)",
    "Reference / value-category binding (lvalue/rvalue, const)",
    "Type mismatch / bad conversion / ambiguous overload",
    "Malformed C++ syntax (template/generic syntax errors)",
    "Other / uncategorized (add a note)",
]
RQ3_CATS = [
    "O1 Echo / self-confirming (assert encodes CURRENT output -> buggy still passes)",
    "O2 Wrong API surface (bug only observable via an API the test never calls)",
    "O3 Too-weak assertion (only non-empty / no-crash / type, not the value)",
    "O4 Insufficient input (inputs never trigger the buggy branch/condition)",
    "O5 Other / unclear",
]


def _two_blank_copies(src, prefix, ctx_cols, derive, ncats):
    rows = list(csv.DictReader((TAX / src).open()))
    fields = ["id", *ctx_cols, "label", "notes"]
    for coder in (1, 2):
        with (TAX / f"{prefix}_coder{coder}.csv").open("w", newline="") as f:
            w = csv.DictWriter(f, fieldnames=fields)
            w.writeheader()
            for i, r in enumerate(rows, 1):
                out = {"id": i, "label": "", "notes": ""}
                out.update(derive(r))
                w.writerow(out)
    return len(rows)


def main():
    n1 = _two_blank_copies(
        "rq1_coding_sheet.csv", "rq1",
        ctx_cols=["defect_id", "model", "project", "error"],
        derive=lambda r: {k: r.get(k, "") for k in ("defect_id", "model", "project", "error")},
        ncats=len(RQ1_CATS))

    def rq3_ctx(r):
        tp = r.get("test_path", "")
        return {"defect_id": r.get("defect_id", ""), "model": r.get("model", ""),
                "project": r.get("project", ""),
                "focal_path": tp.replace("test.cpp", "focal.txt"), "test_path": tp}

    n3 = _two_blank_copies(
        "rq3_coding_sheet.csv", "rq3",
        ctx_cols=["defect_id", "model", "project", "focal_path", "test_path"],
        derive=rq3_ctx, ncats=len(RQ3_CATS))

    # A compact numbered legend the coders keep open while labelling.
    legend = ["# Bang ma so danh nhan (dien SO vao cot `label`)\n",
              "## RQ1 — loi compile (chon 1 so tu cot `error`)"]
    legend += [f"{i}. {c}" for i, c in enumerate(RQ1_CATS, 1)]
    legend += ["\n## RQ3 — oracle yeu (mo focal_path + test_path roi chon 1 so)"]
    legend += [f"{i}. {c}" for i, c in enumerate(RQ3_CATS, 1)]
    (TAX / "category_legend.md").write_text("\n".join(legend) + "\n")

    print(f"RQ1: wrote rq1_coder1.csv + rq1_coder2.csv  ({n1} cells each, blank `label`)")
    print(f"RQ3: wrote rq3_coder1.csv + rq3_coder2.csv  ({n3} cells each, blank `label`)")
    print("legend -> data/taxonomy/category_legend.md")
    print("\nNext: each coder fills ONLY their own file, then:")
    print("  python3 analysis/kappa_merge.py rq1")
    print("  python3 analysis/kappa_merge.py rq3")


if __name__ == "__main__":
    main()
