# Revision evidence — traceability table (P9)

Every reviewer comment mapped to the experiment that answers it, the script that produced
the answer, the output file, and the finding. All paths are relative to `artifact/`.
Nothing here required a new generation campaign except E5d (12 cells, ~$0.05).

Run everything from `artifact/`. E4 and E5 need a C++ toolchain and the vendored
googletest; the scripts locate it in `artifact/third_party/` or `../study/third_party/`.

---

## 1. Reviewer #3, Major Comment 2 — the temperature arm (E2)

> "Section IV-D reports 5 detections in 12 cells at temperature 0.2, whereas
> DeepSeek-V4-flash detects 5 of 57 at the same temperature in the main Tier A campaign
> (Fisher exact p = 0.011). Either those cells are an easier subset or the arm differs in
> a way not stated."

| | |
|---|---|
| script | `analysis/e2_robustness_arms.py` |
| outputs | `data/results/e2_robustness_arms.md`, `.json` |
| cost | $0 |

**The reviewer's first hypothesis is correct, and it is worse than they could see.** The
temperature arm is 4 fmt defects × 3 temperatures × 3 runs × **one** model
(`aggregate/sensitivity_temp.py`). The source comments its own sample selection verbatim:

> `# sample spanning outcomes seen at 0.2 (2 detected, 1 reached-not-detected, 1 non-compilable)`

The four defects were therefore chosen **after** observing their outcomes at t=0.2.

- 2 of the 4 (`611cf0b3`, `6a134640`) are among the only 4 Tier A defects any model ever
  detects — base rate 4/19.
- The 12 cells at t=0.2 are **exactly** the main-campaign DeepSeek cells restricted to
  those defects, outcome for outcome.
- They contain **5 of DeepSeek's 5 Tier A detections in 21% of its cells**
  (Fisher exact against the remaining 45 cells, p = 0.0002).

**The published counts 4/12, 5/12, 2/12 are arithmetically correct — this is not a
bookkeeping error.** What is wrong is that the arm is presented as evidence about the
level of detection when its denominator was chosen to contain the detections.

What survives: the three temperatures share the same defects and runs, so the *paired*
contrast is still interpretable — detection does not rise with temperature (4, 5, 2) and
non-compilation is flat. What does not survive: any statement about the detection level,
and the non-compile flatness is carried entirely by one defect (`0cc73ebf`, non-compilable
in all 9 of its cells).

Also newly documented for the other arms, as requested:

| arm | defects | models | projects | runs | outcome-selected? |
|---|---|---|---|---|---|
| temperature | 4 | 1 (deepseek) | fmt only | 3 | **yes** |
| generation source | 19 (all Tier A) | 1 (deepseek) | fmt, CLI11, entt | 3, paired | no |
| prompt, three-way | 14 | 1 (deepseek) | fmt only | 1 | no |
| prompt, paired guided vs api-rich | 24 | 1 (deepseek) | **fmt 14 + cpp-peglib 10** | 1 | no |
| model families | 19 | 4 | fmt, CLI11, entt | 3 | no (it is the main campaign) |

Two further disclosure gaps found: t=0.2 **reuses** the main campaign's cached cells while
t=0.0 and t=0.8 are freshly generated; and the paired prompt arm silently **crosses the two
benchmarks** (10 of its 24 defects come from BugsC++, where detection is 0/80).

---

## 2. Reviewer #3, Major Comment 1 — unit of analysis (E1)

> "Cells are nested within defects and projects, so intervals computed over cells
> overstate precision. Please report both, with primary intervals from a defect-level
> bootstrap or a mixed-effects model."

| | |
|---|---|
| script | `analysis/cluster_bootstrap.py` |
| outputs | `data/results/e1_defect_level.md`, `.json` |
| cost | $0 |

**Both claims verified against the data.**

- Internal campaign: 23 pre-audit defects / 138 cells; the audit moved `nng@6a59b15fba`
  (`nng_recv`) and `nng@e3b8f31b04` (`nn_setsockopt`) to exported public, leaving
  **21 defects / 126 cells** (cppcheck 15, SPIRV-Tools 5, nng 1). Cells that compiled: **0**.
  Defects with any compiling suite: **0**. Both suites that compiled pre-audit belonged to
  the reclassified `nng_recv`.
- Tier A: **4 of 19** defects detected by at least one model or run, and the four hashes
  match the paper exactly (`020a21af`, `611cf0b3`, `6a134640`, `96c18b26`).

Both estimands, with a defect-cluster bootstrap (resample the 19 defects with replacement
carrying all their cells, 10,000 resamples, percentile CI; suites are never resampled):

| estimand | point | cell-level Wilson | **defect-cluster bootstrap** |
|---|---|---|---|
| per-attempt, conditional on eligibility | 16/141 = 11% | 7.1–17.6% | **1.5–25.5%** |
| per-attempt, all attempts | 16/228 = 7% | 4.4–11.1% | **0.9–15.8%** |
| per-defect, detected at least once | 4/19 = 21% | 8.5–43.3% | — |
| internal, per-attempt | 0/126 = 0% | 0–3.0% | — |
| internal, per-defect | 0/21 = 0% | **0–15.5%** | — |

The cluster bootstrap widens the headline interval by a factor of 2.3.

Per model, the cluster intervals all span each other's point estimates, so the ordering
Gemini < Qwen < DeepSeek < GPT-5.4 is not supported. A new observation worth stating:
**GPT-5.4 alone accounts for all four detectable defects**, so pooling four models adds
nothing over GPT-5.4 by itself.

---

## 3. Reviewer #3, Major Comment 3 — per-case reachability (E4)

> "Reachability is measured per suite but interpreted per case … A reachability failure
> could then appear as an infection or propagation failure. Per-case coverage via
> `--gtest_filter` would settle this."

| | |
|---|---|
| script | `analysis/e4_per_case_coverage.py` |
| outputs | `rq3/per_case_coverage.csv` (537 rows), `rq3/e4_suite_classification.csv`, `rq3/e4_summary.md`, `.json` |
| cost | $0, ~3 min |

The mechanism the reviewer suspected is real and visible in the code: `measure/detection.py`
runs the buggy version with `gtest_filter` set to the valid cases only, but then calls
`hunk_coverage`, which runs the **whole binary with no filter**.

Every case of every one of the 44 suites was re-run individually on the buggy version
under gcov.

| group | meaning | count |
|---|---|---|
| A | ≥1 **valid** case reaches the patched region on its own | **41** |
| B | no valid case reaches; only invalid/flaky cases do | **3** |
| C | suite reaches but no single case does | 0 |
| D | whole-suite run did not reproduce the recorded reach | 0 |

**The RQ3 population changes from 44 to 41.** The three suites that leave are
`279d698e` deepseek run3 (O4), `fc6e0fe9` gemini run3 (O2), `fc6e0fe9` gpt-5.4 run1 (O2).

| population | n | O4 infection | O2 propagation | split |
|---|---|---|---|---|
| all 44 (as published) | 44 | 30 | 14 | 68/32 |
| **group A** | **41** | **29** | **12** | **71/29** |

### A second finding: gcov line attribution in templates

`coverage.py` stores gcov line entries in a dict keyed by line number. In template-heavy
code gcov emits **several entries per source line**, one per instantiation, and they
disagree — so the last entry silently overwrites the earlier ones. **16 of 44 suites**
contain at least one such line. For `fmt@96c18b26` the published record lists only line 263
although 258–261 also execute. This is the "line attribution in template code is coarse"
threat, now quantified.

Under the last-entry rule the partition is A=39, B=5 (split 69/31); under the correct
"any instantiation" rule it is A=41, B=3 (split 71/29). Both are reported. Under the
last-entry rule this pipeline reproduces the published whole-suite line set in **44/44**
suites, which validates it against the original campaign.

**Either way the qualitative claims hold**: infection dominates, and no miss is an
assertion failure. The proportions move by ~3 points.

---

## 4. Reviewer #3, Major Comment 4 — adjudication vs κ (E3)

> "κ = 0.72 is computed on pre-adjudication labels while the reported labels are
> post-adjudication … How many labels the probes changed would be more informative than κ."

| | |
|---|---|
| script | `analysis/e3_adjudication_trace.py` |
| outputs | `rq3/e3_adjudication.md`, `rq3/e3_adjudication_trace.csv`, `.json` |
| cost | $0 |

κ = 0.72 reproduced exactly (observed agreement 86.4%, chance 51.2%, n = 44, 6 disagreements).

**The number the reviewer asked for:**

| quantity | value |
|---|---|
| suites requiring adjudication | 6/44 |
| distinct defects probed | 3 |
| probe overturned coder 1 | **2/6** |
| probe overturned coder 2 | **4/6** |
| probe produced a label neither annotator proposed | **0/6** |

So the probes changed **6 of the 12 individual annotator judgements** they examined and
never introduced a third label: adjudication was a tie-break, not an independent relabelling.

**Answer to the specific question about the three weak-assertion labels: they overlap the
disagreements completely.** All three O3 labels came from one annotator, on one defect
(`96c18b26`), and all three are among the six disagreements; none survives. The claim that
no miss is attributable to a weak assertion therefore rests on adjudicating three suites of
a single defect against a probe, not on independent agreement. It should be stated that way.

**A traceability defect found in the package:** `rq3_coder1.csv` / `rq3_coder2.csv` hold the
**first annotation round only** — 47 suites (31 from the main campaign plus 16 from the
buggy-generation arm that never enter the paper) with 4 disagreements. The paper's κ comes
from the `coder1`/`coder2` columns of `rq3_frozen44.csv`, which merge that round with a
second one covering the 13 GPT-5.4 suites. Anyone recomputing κ from the shipped per-coder
files will not reproduce 0.72. These two records must be reconciled before submission.

Also: the artifact copy of `rq3_frozen44.csv` has dropped the `adjudication_reason` column
that the working copy in `study/` retains. That column is probe evidence, not annotator
notes, and should ship.

---

## 5. Reviewer #3, Major Comment 1 (last paragraph) — cross-configuration probe (E5)

> "Running the three internal-campaign models on a few Tier A public defects under the
> internal prompt, compiler, and zero-repair configuration would cost about a dozen cells
> and would separate accessibility from toolchain and repair budget."

The internal-vs-public contrast confounds four factors. Three of them are isolated on the
**full 19-defect Tier A set** from data already in the package, which is stronger than a
12-cell probe, and the fourth is the 12 new cells.

### Stages A–C — $0

| | |
|---|---|
| script | `analysis/e5_cross_config_probe.py` |
| outputs | `data/results/e5_cross_config.md`, `.json`, `e5_clang_recompile.csv` |

| factor | effect on public-focal compilation | explains 0/126? |
|---|---|---|
| **repair budget removed** (`shots == 1` ⟺ first generation compiled) | 156/228 = 68% → **102/228 = 45%** | no |
| **compiler changed** (all 228 suites recompiled with clang++-18) | 68% → **154/228 = 68%** | no |
| **prompt switched to guided** (existing paired arm, fmt) | 76% → 79%, no decrease | no |

Removing the repair round costs 24 points and still leaves 45% of public-focal suites
compiling. Changing the front end costs 2 cells out of 228. Neither approaches zero.

Toolchain caveat: the internal campaign used clang-16 + libc++; neither is installed on
this machine, so clang-18 + libstdc++ is the substitute. It tests "a different front end",
not the exact internal toolchain.

### Stage D — the reviewer's 12 cells

| | |
|---|---|
| script | `analysis/e5d_probe_generate.py` |
| outputs | `data/results/e5d_probe.md`, `.jsonl`; artifacts under `data/raw/<defect>/<model>/guided/run1/` |
| cost | ~$0.05 |

**Pre-registered selection**, fixed before generation and referring only to project
membership and defect hash, never to an outcome: cover all three Tier A projects, take the
lexicographically smallest hash in each, give the spare slot to fmt via its next smallest.
Selected: `020a21afc6` (CLI11), `ad5bb5198b` (entt), `0cc73ebf79` and `279d698e1b` (fmt).

Zero repair is enforced by reading `shot1.cpp`, the first generation, which
`generate_tests.py` always persists before deciding whether to take a repair shot.

DeepSeek is routed via OpenRouter (`deepseek-v4-flash-or` in `config/models.json`) because
the direct DeepSeek endpoint returned **402 Payment Required** on 2026-09-07. Same model
identifier, different provider endpoint.

**Result — 12/12 cells generated.**

| configuration | compiles |
|---|---|
| guided prompt, **zero repair**, g++ | **4/12** |
| guided prompt, **zero repair**, clang++-18 | **4/12** |
| guided prompt, one repair round, g++ | 8/12 |
| guided prompt, one repair round, clang++-18 | 8/12 |

| defect | project | zero repair | one repair |
|---|---|---|---|
| `020a21af` | CLI11 | 2/3 | 3/3 |
| `ad5bb519` | entt | **0/3** | **0/3** |
| `0cc73ebf` | fmt | 1/3 | 2/3 |
| `279d698e` | fmt | 1/3 | 3/3 |

**Under the internal campaign's own prompt and with no repair round, 4 of 12 public-focal
suites still compile, against 0 of 126 for internal focal functions under the same prompt
and repair budget.** Together with stages A–C this leaves focal accessibility as the factor
standing.

Two honest caveats. One of the four pre-registered defects (`ad5bb519`, entt) fails in all
three of its cells even with a repair round, so the probe's rate is carried by the other
three; it was fixed by the selection rule before generation and is reported rather than
replaced. And no cell in the probe detects its defect (0/12), which is consistent with the
main campaign but is not what this probe was powered to measure.

---

## 6. Reviewer #2 — novelty relative to Yang et al.

No experiment required. The three analytical differences are already in the paper
(`main.tex:147`) but are buried in Background; they should be stated in the Introduction:
focal accessibility as a gate preceding test quality, separation of propagation failures
from input failures, and intervention-checked mechanism labels. The unit-isolation wall
(0/126) has no counterpart in Defects4J, so it is not a "language difference" in the
trivial sense. The Java comparison the reviewer proposes is a second paper, and the current
text already defers it as future work.

---

## 7. Summary of paper edits implied

| location | change | source |
|---|---|---|
| Abstract | add defect-level companion (4/19); narrow "None is classified as an assertion failure" to the analysed suites | E1, E3 |
| Abstract, Fig. 1, Findings 2–3, §RQ3 | RQ3 population 44 → 41; split 30/14 → 29/12 | E4 |
| Finding 1 | add "none of the 21 audited defects obtains a compiling suite (95% CI 0–15.5%)" | E1 |
| Finding 2 | add "across 4 of 19 distinct defects" | E1 |
| Finding 3 | move the clustering qualification inside the box; state the split as descriptive | E4 |
| Table III | replace pooled Wilson CI with the defect-cluster bootstrap CI (1.5–25.5%) | E1 |
| §IV-D temperature | disclose the outcome-dependent selection; restrict the claim to the paired contrast | E2 |
| §IV-D all arms | add the arm composition table (defects, models, projects, runs) | E2 |
| §IV-D prompt arm | disclose that the paired arm mixes Defects4C and BugsC++ | E2 |
| §III-D | replace the κ sentence with the probe-change counts | E3 |
| §III-C / Threats | quantify the gcov template-attribution issue (16/44 suites) | E4 |
| Threats (internal validity) | replace "campaigns differ in prompt, compiler, repair" with the measured bounds | E5 |
| Conclusion | add the Figure 1 caption qualification and the defect-level rate | E1 |
| Introduction | foreground the three differences from Yang et al. | R2 |
| Threats | "39 of the 41 RQ3 suites come from fmt"; "8 of the 29 infection and 8 of the 12 propagation" | E4 derived |
| §IV-D model families | share range 17-37% -> **13-37%** (recomputed on the 41-suite population) | E4 derived |
| §RQ2 inline CI | 11% (95% CI 7-18%) -> **1.5-25.5%**, matching the cluster-bootstrap convention the table caption declares | E1 |
| Preamble | delete the unused `\DetectPooled` / `\InternalWall` / `\ReachedMissed` macros, which still carried pre-revision numbers | housekeeping |
| Data availability | point at the immutable tag `kse2026-camera-ready`, not the branch | release |

---

## 8. File index

| path | produced by | contents |
|---|---|---|
| `data/results/e1_defect_level.{md,json}` | `analysis/cluster_bootstrap.py` | both estimands, Wilson + cluster bootstrap, verification of 0/21 and 4/19 |
| `data/results/e2_robustness_arms.{md,json}` | `analysis/e2_robustness_arms.py` | full arm inventory, temperature reconciliation |
| `rq3/per_case_coverage.csv` | `analysis/e4_per_case_coverage.py` | per (suite, case) reachability under both gcov rules |
| `rq3/e4_suite_classification.csv` | " | per-suite group A/B/C/D and reaching case names |
| `rq3/e4_summary.{md,json}` | " | recomputed RQ3 table |
| `rq3/e3_adjudication.{md,json}` | `analysis/e3_adjudication_trace.py` | κ recomputation, probe-change counts, provenance warning |
| `rq3/e3_adjudication_trace.csv` | " | per-disagreement trace |
| `data/results/e5_cross_config.{md,json}` | `analysis/e5_cross_config_probe.py` | repair / compiler / prompt decomposition |
| `data/results/e5_clang_recompile.csv` | " | per-cell g++ vs clang outcome |
| `data/results/e5d_probe.{md,jsonl}` | `analysis/e5d_probe_generate.py` | the 12-cell probe |
| `rq3/e4_derived_numbers.{md,json}` | `analysis/e4_derived_numbers.py` | every quantity the paper derives from the 41-suite population |
| `data/taxonomy/rq3_delta_coder{1,2}.csv` | `analysis/rq3_reconcile_labels.py` | annotation round 2 (13 GPT-5.4 suites), previously referenced by the README but never written |
| `data/taxonomy/rq3_labels_merged44.csv` | " | self-contained per-coder record; kappa = 0.72 recomputes from it |
| `data/taxonomy/rq3_kappa_verification.json` | " | kappa for both populations plus the cell-by-cell merge check |

---

## 9. Traceability defects fixed in the package

Three problems that a reviewer recomputing our figures from the shipped files would have hit.
All are closed by `analysis/rq3_reconcile_labels.py` plus one file restore.

| defect | symptom | fix |
|---|---|---|
| κ not reproducible | `rq3_coder1.csv` / `rq3_coder2.csv` cover annotation **round 1 only** — 47 suites, 16 of them from the buggy-generation arm that is not in the paper — and give **κ = 0.80** over 4 disagreements | `rq3_labels_merged44.csv` now carries the 44-suite record keyed by (defect, model, run), with a `round` column. κ = 0.72 over 6 disagreements recomputes from it directly |
| files promised but absent | the README referenced `rq3_delta_coder{1,2}.csv` for annotation round 2; they were never written | emitted from the 13 GPT-5.4 rows of `rq3_frozen44.csv`; the merge is verified cell by cell against that file (0 mismatches) |
| probe evidence dropped | the artifact copy of `rq3_frozen44.csv` had lost the `adjudication_reason` column that the working copy kept | restored; all six resolutions now ship with the reason that settled them |

Round 1 is left exactly as shipped, because the annotation tooling (`apply_adjudication.py`,
`make_rq3_html.py`, `make_rq3_packet.py`) joins it positionally against
`rq3_coding_sheet.csv` by row order and would break if it were filtered or reordered.
`CODEBOOK.md` now states which file backs which number.

Verification, one command:

```
$ python3 analysis/rq3_reconcile_labels.py
  analysis set (44):  kappa = 0.720  agreement 86.4%  disagreements 6   <- the published value
  round-1 file (47):  kappa = 0.796  agreement 91.5%  disagreements 4   <- a different population
  merge verified against rq3_frozen44.csv: 0 mismatches
```

---

## 10. Camera-ready verification

Built from `study/paper/ieee/main.tex` with a clean `latexmk -C && latexmk -pdf`:
**6 pages, 0 errors, 0 overfull vboxes, no oversized overfull hboxes.**

Every edit in §7 was checked against the extracted PDF text, and every superseded number was
searched for:

| superseded | occurrences in the PDF |
|---|---|
| CI 7–18 %, 17 % to 37 %, 68/32, 44/141, 30 infection, 14 propagation, 81 never, 57 %, 31 % | **0** |
| `[TODO:` / `[VERIFY:` render | **0** |

The five remaining occurrences of the literal "44" are all correct: three read "the 44 suites
flagged by the whole-suite measurement" (the pre-reclassification population, which the paper
now names explicitly) and two are the 44 % row of the RQ1 taxonomy table.

Two stale values were found and corrected during this pass rather than merely reported:

- §RQ2 quoted the conditional rate as `11% (95% CI 7--18%)`, a cell-level Wilson interval, while
  the table caption declares that pooled intervals are defect-cluster bootstraps. It now reads
  `11% (95% CI 1.5--25.5%)`, which is the bootstrap interval for the same quantity. Quoting the
  Wilson interval here was exactly the overstated precision Reviewer #3 objected to.
- §IV-D still carried the model-families share range `17% to 37%`, computed on the 44-suite
  population. On the 41-suite population it is `13% to 37%` (Gemini 3/24, Qwen 14/38,
  DeepSeek 12/39, GPT-5.4 12/40).

Also removed: the unused `\DetectPooled`, `\InternalWall` and `\ReachedMissed` macros, which
were referenced nowhere but still held pre-revision numbers (7–18 %, 126/126, 31 %).

Gemini's share is **3/24 = 12.5 % exactly**. Rounded half up that is 13 %, which is what the
paper quotes; Python's default banker's rounding would print 12 %. A reviewer recomputing gets
12.5 %, consistent with 13 % and not with 12 %.

**Release.** The package is tagged `kse2026-camera-ready`, and the paper's Data availability
statement points at that tag rather than at `main`.
