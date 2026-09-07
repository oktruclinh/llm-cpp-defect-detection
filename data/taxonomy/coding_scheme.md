# Coding scheme — RQ1 (compile failures) & RQ3 (oracle weakness)

**Protocol (2 coders → Cohen's κ):** Coder 1 fills the `coder1` column, Coder 2 fills `coder2`,
**independently** (each works on their own copy; do NOT look at the other's labels, and do NOT
consult `rq1_auto_suggest.csv` while coding — it is a separate machine first-pass for cross-check only).
Assign exactly ONE category per row. Then `python3 analysis/kappa.py data/taxonomy/rq1_coding_sheet.csv`
computes κ(coder1, coder2); resolve disagreements by discussion and report the adjudicated labels +
the pre-adjudication κ. Do the same for `rq3_coding_sheet.csv` (open each `test.cpp` to judge why a
valid test missed the bug).

## RQ1 — compile-failure categories (assign ONE per case, from the first compiler error)
- **Test-harness / assertion-macro misuse**
- **UDL/namespace (e.g. _hs without `using namespace ...::literals`)**
- **static_assert / unsatisfied trait (e.g. type not formattable)**
- **Hallucinated/wrong API (member/function/overload does not exist)**
- **Missing include / unknown type**
- **Template / overload deduction failure**
- **Access control (private/protected member)**
- **Reference / value-category binding (lvalue/rvalue, const)**
- **Type mismatch / bad conversion / ambiguous overload**
- **Malformed C++ syntax (template/generic syntax errors)**
- **Other / uncategorized** (use only if none fit; add a note)

## RQ3 — oracle-weakness categories (inspect test.cpp; why did a valid test miss the bug?)
- **O1 Echo/self-confirming** — assertions encode the code's CURRENT output, so a buggy version still passes
- **O2 Wrong API surface** — bug is observable only through a different public API the test never calls (e.g. fmt `format_to_n`)
- **O3 Too-weak assertion** — only checks non-empty / no-crash / type, not the actual value
- **O4 Insufficient input** — inputs don't trigger the specific buggy condition/branch
- **O5 Other / unclear** — does not fit the above