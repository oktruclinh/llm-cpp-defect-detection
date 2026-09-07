# E4 -- Per-case reachability for the 44 RQ3 suites

Source suite list: `/home/truclinh/Thesis/kse2026/study/data/taxonomy/rq3_frozen44.csv` (44 suites). Every case of every suite was re-run individually on the buggy version under gcov.

## A prior finding: gcov line attribution in templates

gcov emits several entries for the same source line in template-heavy code, one per instantiation, and they disagree. `measure/coverage.py` keys them into a dict, so the **last entry silently overwrites the earlier ones** and a line covered only by an earlier instantiation is recorded as uncovered. The paper names this threat ("line attribution in template code is coarse"); it is measurable. **16 of 44 suites** contain at least one hunk line whose entries disagree. For `fmt@96c18b26` the published record lists only line 263, although lines 258-261 also execute. Under the last-entry rule this pipeline reproduces the published whole-suite line set exactly in **44/44** suites, which validates it against the campaign; the disagreement is confined to which lines are credited, and at suite level it never changes the reached / not-reached verdict.

Everything below is therefore reported under two rules: **any** (a line counts as executed if any instantiation executed it -- the correct reading) and **last-entry** (the published pipeline's behaviour).

## Partition

| group | meaning | any (primary) | last-entry (as published) |
|---|---|---|---|
| A | at least one **valid** case reaches the patched region on its own | **41** | 39 |
| B | no valid case reaches; only invalid/flaky cases do | **3** | 5 |
| C | suite reaches but no single case does (aggregation artefact) | **0** | 0 |
| D | whole-suite run did not reproduce the recorded reach | **0** | 0 |
| ERR | could not rebuild | 0 | 0 |

## Verdict

**The RQ3 population changes from 44 to 41.** 3 suites reach the patched region only through cases that the detection analysis excludes, so their infection/propagation labels do not describe a reachable-and-observed defect. Every downstream quantity must be recomputed.

## Infection / propagation, recomputed

Labels: `O4` = infection failure (no differentiating input), `O2` = propagation failure (interface hides the difference).

| population | n | O4 infection | O2 propagation | split |
|---|---|---|---|---|
| all 44 suites (as published) | 44 | 30 | 14 | 68/32 |
| group A, any rule (primary) | 41 | 29 | 12 | 71/29 |
| group A, last-entry rule | 39 | 27 | 12 | 69/31 |

Group A spans 11 distinct defects (CLIUtils 2, fmtlib 39).

## The suites that leave the RQ3 population

| suite | published label | leaves under | only case reaching the patch |
|---|---|---|---|
| `279d698e` deepseek-chat run3 | O4 | both rules | `FormatIntegerTest.CharType` |
| `cd7202e0` deepseek-chat run1 | O4 | last-entry rule only | `ChronoWriteFractionalSecondsTest.BoundarySeconds` |
| `cd7202e0` deepseek-chat run3 | O4 | last-entry rule only | `ChronoFractionalSecondsTest.DefaultFormatSeconds` |
| `fc6e0fe9` gemini-2.5-flash run3 | O2 | both rules | `FmtAlignSignTest.DifferentTypes` |
| `fc6e0fe9` gpt-5.4-or run1 | O2 | both rules | `FmtFormatSignAlignmentTest.FloatingPointNumericAlignmentPlacesSignBeforeZeros` |

In each of these the case that executes the patched region fails on the fixed version, so it never enters the detection analysis. Among its valid cases the suite is a reachability failure and belongs with the "never reached" group, not with the reached-but-not-caught population that carries an infection or propagation label.

## Files

| path | contents |
|---|---|
| `rq3/per_case_coverage.csv` | one row per (suite, test case): case kind (valid/invalid/flaky), whether it reaches the patch alone, which hunk lines |
| `rq3/e4_suite_classification.csv` | one row per suite: group, counts, the reaching cases by name, and whether the suite-level measurement reproduced |
| `rq3/e4_summary.json` | the counts above, machine-readable |
