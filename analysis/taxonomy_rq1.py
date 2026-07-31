#!/usr/bin/env python3
"""First-pass RQ1 taxonomy: cluster the COMPILE failures of LLM-generated C++ tests
from saved eval.json files. Heuristic auto-categorization -> a draft for the 2nd
human annotator (NOT the final taxonomy). Writes a per-case CSV + category counts."""
import csv
import json
import re
from collections import Counter
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
RAW = STUDY / "data/raw"

# ordered (first match wins) — C++-specific failure buckets
# v2 (post-audit): + category 12 (test-harness/macro misuse) at top priority; refined 3/4/5/9.
# Fence/markdown (was proposed as 11) is NOT a category: it was an extract_code() bug that
# masked the real error; robust extraction + re-compile now surface the underlying cause.
RULES = [
    ("Test-harness / assertion-macro misuse",
     r'macro ".*" passed \d+ argument|unterminated argument list invoking macro'),
    ("UDL/namespace (e.g. _hs without `using namespace ...::literals`)",
     r"string literal operator|unable to find string literal"),
    ("static_assert / unsatisfied trait (e.g. type not formattable)",
     r"static assertion failed|static_assert|formattable|provide a formatter|no formatter"),
    ("Hallucinated/wrong API (member/function/overload does not exist)",
     r"is not a member of|was not declared in this scope|has not been declared|"
     r"has no member|no member named|no matching function for call|but does not override"),
    ("Missing include / unknown type",
     r"No such file or directory|does not name a type|does not name a template type|"
     r"incomplete type|unknown type name"),
    ("Template / overload deduction failure",
     r"no type named|template argument|could not (?:deduce|convert)|no matching .* template|"
     r"candidate template ignored|ambiguous .*instantiation"),
    ("Access control (private/protected member)",
     r"is private|is protected|within this context"),
    ("Reference / value-category binding (lvalue/rvalue, const)",
     r"cannot bind|lvalue reference|rvalue|binding reference|discards qualifiers"),
    ("Type mismatch / bad conversion / ambiguous overload",
     r"cannot convert|invalid conversion|no known conversion|ambiguous|is ambiguous"),
    ("Malformed C++ syntax (template/generic syntax errors)",
     r"expected .*before|expected primary-expression|expected unqualified-id|"
     r"expected ['‘].*['’]|expected initializer|size .* of array .* is negative"),
]


def first_error(stderr: str) -> str:
    for ln in (stderr or "").splitlines():
        if " error:" in ln:
            return ln.split(" error:", 1)[1].strip()[:160]
    return ""


def categorize(stderr: str) -> str:
    # categorize on the FIRST error line primarily (root cause), fall back to full tail
    primary = first_error(stderr)
    for hay in (primary, stderr or ""):
        for label, pat in RULES:
            if re.search(pat, hay, re.IGNORECASE):
                return label
    return "Other / uncategorized"


def main():
    cases = []
    for ev_path in RAW.glob("**/eval.json"):
        try:
            ev = json.loads(ev_path.read_text())
        except Exception:
            continue
        if ev.get("outcome") != "non_compilable":
            continue
        err = ev.get("compile_err_fixed") or ev.get("compile_err_buggy") or ""
        # path: .../<defect>/deepseek-chat[/<condition>]/run<k>/eval.json
        parts = ev_path.parts
        cond = "buggy" if "buggy" in parts else ("no_focal" if "no_focal" in parts else "fixed")
        cases.append({
            "defect_id": ev.get("defect_id"), "project": ev.get("project"),
            "condition": cond, "category": categorize(err), "first_error": first_error(err),
        })

    out = STUDY / "data/taxonomy"
    out.mkdir(parents=True, exist_ok=True)
    with (out / "rq1_compile_failures_firstpass.csv").open("w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=["defect_id", "project", "condition", "category", "first_error"])
        w.writeheader()
        w.writerows(cases)

    print(f"=== RQ1 compile-failure first-pass taxonomy ({len(cases)} non-compilable cells) ===")
    for cat, n in Counter(c["category"] for c in cases).most_common():
        print(f"  {n:3d}  {cat}")
    print("\nexamples per category:")
    seen = set()
    for c in cases:
        if c["category"] not in seen and c["first_error"]:
            seen.add(c["category"])
            print(f"  [{c['category'][:35]:35s}] {c['project'].split('___')[1]}: {c['first_error'][:90]}")
    print(f"\nCSV -> {out/'rq1_compile_failures_firstpass.csv'} (draft for 2nd annotator)")


if __name__ == "__main__":
    main()
