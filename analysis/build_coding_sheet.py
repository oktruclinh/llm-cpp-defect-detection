#!/usr/bin/env python3
"""[#23 prep] Build the annotation coding sheets + coding scheme so a 2nd annotator can
independently code the RQ1 (compile-failure) and RQ3 (oracle-weakness) taxonomies →
Cohen's Kappa. label_A = the heuristic first-pass; label_B = blank for the 2nd coder."""
import csv
import json
import sys
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(STUDY / "analysis"))
from taxonomy_rq1 import categorize, first_error, RULES   # noqa: E402

RQ3_CATS = [
    ("O1 Echo/self-confirming", "assertions encode the code's CURRENT output, so a buggy version still passes"),
    ("O2 Wrong API surface", "bug is observable only through a different public API the test never calls (e.g. fmt `format_to_n`)"),
    ("O3 Too-weak assertion", "only checks non-empty / no-crash / type, not the actual value"),
    ("O4 Insufficient input", "inputs don't trigger the specific buggy condition/branch"),
    ("O5 Other / unclear", "does not fit the above"),
]


def model_of(p: Path):
    parts = p.parts
    return parts[parts.index("raw") + 2] if "raw" in parts else "?"


def main():
    out = STUDY / "data/taxonomy"
    out.mkdir(parents=True, exist_ok=True)
    rq1, rq3, auto = [], [], []
    for ev_path in (STUDY / "data/raw").glob("**/eval.json"):
        try:
            ev = json.loads(ev_path.read_text())
        except Exception:
            continue
        oc = ev.get("outcome")
        model = model_of(ev_path)
        if oc == "non_compilable":
            err = ev.get("compile_err_fixed") or ev.get("compile_err_buggy") or ""
            fe = first_error(err)
            excerpt = fe or " ".join(err.replace("\n", " ").split())[-220:]
            # two-coder sheet: coder1/coder2 BLANK (fill independently). auto-suggest kept SEPARATELY.
            rq1.append({"defect_id": ev.get("defect_id"), "model": model,
                        "project": ev.get("project"), "error": excerpt,
                        "coder1": "", "coder2": "", "notes": ""})
            auto.append({"defect_id": ev.get("defect_id"), "model": model,
                         "auto_suggest": categorize(err)})
        elif oc == "reached_not_detected":
            rq3.append({"defect_id": ev.get("defect_id"), "model": model,
                        "project": ev.get("project"),
                        "test_path": str(ev_path.parent / "test.cpp"),
                        "coder1": "", "coder2": "", "notes": ""})

    def write(name, rows, fields):
        with (out / name).open("w", newline="") as f:
            w = csv.DictWriter(f, fieldnames=fields)
            w.writeheader()
            w.writerows(rows)

    write("rq1_coding_sheet.csv", rq1,
          ["defect_id", "model", "project", "error", "coder1", "coder2", "notes"])
    write("rq3_coding_sheet.csv", rq3,
          ["defect_id", "model", "project", "test_path", "coder1", "coder2", "notes"])
    write("rq1_auto_suggest.csv", auto, ["defect_id", "model", "auto_suggest"])

    scheme = ["# Coding scheme — RQ1 (compile failures) & RQ3 (oracle weakness)\n",
              "**Protocol (2 coders → Cohen's κ):** Coder 1 fills the `coder1` column, Coder 2 fills `coder2`,",
              "**independently** (each works on their own copy; do NOT look at the other's labels, and do NOT",
              "consult `rq1_auto_suggest.csv` while coding — it is a separate machine first-pass for cross-check only).",
              "Assign exactly ONE category per row. Then `python3 analysis/kappa.py data/taxonomy/rq1_coding_sheet.csv`",
              "computes κ(coder1, coder2); resolve disagreements by discussion and report the adjudicated labels +",
              "the pre-adjudication κ. Do the same for `rq3_coding_sheet.csv` (open each `test.cpp` to judge why a",
              "valid test missed the bug).\n",
              "## RQ1 — compile-failure categories (assign ONE per case, from the first compiler error)"]
    for label, _pat in RULES:
        scheme.append(f"- **{label}**")
    scheme.append("- **Other / uncategorized** (use only if none fit; add a note)\n")
    scheme.append("## RQ3 — oracle-weakness categories (inspect test.cpp; why did a valid test miss the bug?)")
    for c, d in RQ3_CATS:
        scheme.append(f"- **{c}** — {d}")
    (out / "coding_scheme.md").write_text("\n".join(scheme))

    print(f"RQ1 coding sheet: {len(rq1)} cases -> data/taxonomy/rq1_coding_sheet.csv")
    print(f"RQ3 coding sheet: {len(rq3)} cases -> data/taxonomy/rq3_coding_sheet.csv")
    print("coding scheme   -> data/taxonomy/coding_scheme.md")
    print("auto-suggest    -> data/taxonomy/rq1_auto_suggest.csv (machine first-pass, NOT for coders)")
    from collections import Counter
    print("\nRQ1 machine auto-suggest distribution (reference only):")
    for k, v in Counter(r["auto_suggest"] for r in auto).most_common():
        print(f"  {v:3d}  {k}")


if __name__ == "__main__":
    main()
