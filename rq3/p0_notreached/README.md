# Per-case re-measurement of the suites recorded as never reaching

The published pipeline classified a miss as *never reached* when the whole-suite `gcov` run
showed no executed line inside the patched region. That measurement uses the last-entry rule:
in template-heavy code `gcov` emits one entry per instantiation for the same source line, the
entries disagree, and `measure/coverage.py` keeps only the last. The rule can therefore
**under-report** coverage, so a suite that does reach can be recorded as never reaching. The
original per-case reanalysis (`rq3/e4_summary.md`) only covered the 44 suites the whole-suite
measurement had flagged as *reaching*, where this direction of error cannot change the verdict.

This directory covers the other side. The paper's never-reached population is 84 suites. Three
of them were already re-run case by case in the original pass, because they are the three that
moved out of the RQ3 population when it became clear that only an invalid case reached the
patch (`279d698e` deepseek-chat run3, `fc6e0fe9` gemini-2.5-flash run3, `fc6e0fe9` gpt-5.4-or
run1); all three are group B under the *any* rule, with no valid case reaching. The remaining
**81** are the ones re-run here, case by case on the buggy version under both rules. Across the
full 84, therefore, six suites have a valid case that reaches the patched region.

## Result

| | any (every instantiation credited) | last-entry (as published) |
|---|---|---|
| suite reaches, **a valid case reaches on its own** (group A) | **6** | 0 |
| suite reaches, only invalid/flaky cases reach (group B) | 3 | 0 |
| suite does not reach (group D) | 72 | 81 |

Under the published last-entry rule the pipeline reproduces the recorded state for **81 of 81**
suites, which validates this re-run against the campaign. The whole difference is the rule.

The six group-A suites would move from *never reached* to *reached but not caught*:
**78 / 141 never reached (55 %)** and **47 / 141 reached but not caught (33 %)**, against the
published 84 / 141 (60 %) and 41 / 141 (29 %). The paper reports the published figures and
states in Threats to Validity that they are an upper and a lower bound respectively. The six
suites carry no infection/propagation label, so the 29 / 12 split and κ = 0.72 continue to
describe the 41-suite population only.

The six span two defects, both fmt: `6b7bfed40c` (5 suites) and `96c18b26c2` (1). Nine of the
81 suites contain at least one hunk line whose `gcov` entries disagree.

## Files

| path | contents |
|---|---|
| `input_suites.csv` | the 81 suites, taken from `data/results/rows_tierA_*_fixed.jsonl` where `outcome == "not_reached"` |
| `per_case_coverage.csv` | one row per (suite, test case): case kind, whether it reaches the patch alone under each rule, which hunk lines |
| `suite_classification.csv` | one row per suite: group under each rule, counts, the reaching cases by name. Compare `suite_lines_any` with `suite_lines_lastentry` |
| `summary.json` | the counts above |

Regenerate with `python3 analysis/e4_notreached_probe.py` (same harness as
`analysis/e4_per_case_coverage.py`, different suite list and output directory).

## Reading the group labels

The group names are inherited from `e4_per_case_coverage.py`, which was written for suites
recorded as *reaching*. For this population the sense of group D is inverted: `D` here means the
re-run agreed that the suite does not reach, which is the expected outcome, not a failure.
For the same reason the label fields in `summary.json` (`labels_over_44`, `labels_over_groupA`)
are empty: these suites were never annotated, because they were outside the RQ3 population.
`rq3_population_changes` is a flag the script sets whenever group A is non-empty; the paper does
not reclassify these suites.
