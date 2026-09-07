# Annotation Codebook

Two labelling tasks. Both were done by two human annotators; no language model was used.
Agreement statistics reported in the paper are computed from the independently assigned
labels, before disagreements were resolved.

---

## RQ1 — compile-failure categories

Unit of analysis: one non-compiling suite. The label describes the **first** compiler error.
A rule-based classifier assigns the label; a human audit refined the rules and the scheme.

| Code | Category | Signature |
|---|---|---|
| 1 | User-defined literal / namespace | UDL used without the required `using namespace ...::literals` |
| 2 | `static_assert` / unsatisfied trait | type rejected by a library trait, e.g. not formattable |
| 3 | Wrong or unavailable API | no such member, function, or overload at the tested revision |
| 4 | Missing include / unknown type | header absent, or a type is not found in the expected namespace |
| 5 | Template / overload deduction | deduction fails for a template or overload set |
| 6 | Access control | private or protected member accessed |
| 7 | Reference / value-category binding | lvalue/rvalue or const binding error |
| 8 | Type mismatch / conversion | no conversion between two concrete types (core error, not a note) |
| 9 | Malformed C++ syntax | syntactically invalid template or generic code |
| 10 | Other | none of the above |
| 12 | Test-harness / macro misuse | GoogleTest macro used incorrectly, e.g. wrong `EXPECT_EQ` arity |

**Boundary rule.** `is not a member` (expression context) → 3; `does not name a type`
(type context) → 4. A `no known conversion` note under a `no matching function` error → 3,
not 8.

Table II of the paper groups these into five categories: 3 → *wrong or unavailable API*;
12 → *GoogleTest misuse*; 4 → *missing include or unknown type*; 1, 2, 5, 7, 9 → *C++ language
constraints*; 6, 10 → *other*.

---

## RQ2/RQ3 — why a reached defect is missed

Unit of analysis: one suite that compiles, contains at least one valid test case, executes the
patched hunk, and still passes. Annotators see the buggy–fixed diff and the generated test.

Apply the decision tree top-down and take the **first** match.

1. Does the assertion check **no value** — only `EXPECT_NO_THROW`, a `try`/`catch` where both
   branches pass, `!empty()`, or a size check that ignores content? → **O3**
2. Does the test **not touch** the patched code, or observe a surface on which the fault cannot
   appear (e.g. it checks a high-level string while the fault is in a returned iterator)?
   → **O2**
3. Does the input **not trigger** the faulty behaviour — buggy and fixed produce the same result
   for every input the test supplies? → **O4**
4. Is the expected value copied from the current output, so the buggy version also passes?
   → **O1**
5. Otherwise → **O5**

| Code | Name | RIPR link that breaks |
|---|---|---|
| O1 | Echo / self-confirming oracle | oracle |
| O2 | Wrong API surface | propagation / observability |
| O3 | Too-weak assertion | revealability |
| O4 | Insufficient input | infection |
| O5 | Other or unclear | — |

**O1 is empty by construction** in this study: tests are generated from the fixed code, so an
echoed expected value is the *correct* value and does not cause a miss. O1 could only apply to
a generate-on-buggy arm.

**Separating O2 from O4** requires the diff. O2 means no input reachable through the tested
interface could expose the fault; O4 means such an input exists but the test does not supply it.
Where the two coders disagreed, the case was decided by building the library at both commits and
running the disputed input; those probes ship as `rq3_probe_*.json`.

---

## Label files

| File | Contents |
|---|---|
| `rq1_coder1.csv`, `rq1_labmate.csv` | RQ1 labels per coder |
| `rq1_final.csv`, `rq1_final_adjudicated.csv` | RQ1 labels used in Table II |
| `rq3_coder1.csv`, `rq3_coder2.csv` | RQ3 labels per coder, **annotation round 1 only**: 47 suites = 31 from the main campaign plus 16 from the buggy-generation arm that are not in the paper. Joined positionally against `rq3_coding_sheet.csv` (row order = `id`), which carries the run number. κ over these 47 rows is 0.80, **not** the published value |
| `rq3_delta_coder1.csv`, `rq3_delta_coder2.csv` | round 2: the 13 GPT-5.4 suites, added after round 1. Keyed by `(defect_id, model, run)` |
| `rq3_labels_merged44.csv` | **the record behind the published κ = 0.72**: round 1 restricted to the analysis set, merged with round 2, one row per suite with a `round` column. Self-contained — no positional join needed |
| `rq3_kappa_verification.json` | output of `analysis/rq3_reconcile_labels.py`: κ for both populations, and the cell-by-cell check that the merge reproduces `rq3_frozen44.csv` |
| `rq3_frozen44.csv` | the 44-suite analysis set with both coders' labels, the final label, and `adjudication_reason` (the probe evidence for each of the six resolutions) |
| `e4_suite_classification.csv` (in `rq3/`) | per-case reachability re-analysis: which of the 44 keep a **valid** case that reaches the patch. 41 do; the RQ3 population in the paper is those 41 |
| `rq3_adjudicate6.csv` | the six disagreements and their resolved labels |
| `rq3_probe_*.json`, `rq3_confirmation.json` | executable checks supporting the resolutions and the two mechanisms |
