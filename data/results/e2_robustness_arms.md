# E2 -- Robustness arm inventory and temperature reconciliation

## Reference: main Tier A campaign

- 228 cells = 19 defects x 4 models x 3 runs
- temperature 0.2, minimal prompt, condition=fixed
- defects detected by at least one model/run: 4 (020a21af, 611cf0b3, 6a134640, 96c18b26)

## Arm 1 -- Temperature (Section IV-D)

| field | value |
|---|---|
| script | `aggregate/sensitivity_temp.py` |
| data | `data/results/sensitivity_temp.jsonl` |
| design | 4 fmt defects x 3 temperatures x 3 runs x 1 model = 36 cells |
| model | deepseek-chat (deepseek-v4-flash) only |
| projects | fmt only (no CLI11, no entt) |
| defects | 6a134640, 611cf0b3, 287eaab3, 0cc73ebf |
| selection rule (verbatim from source) | "sample spanning outcomes seen at 0.2 (2 detected, 1 reached-not-detected, 1 non-compilable)" |
| selection is outcome-dependent | **YES** |
| overlap with the 4 detectable Tier A defects | **2/4** (611cf0b3, 6a134640) vs base rate 4/19 |
| t=0.2 provenance | **reused** from the main campaign cache |
| t=0.0, t=0.8 provenance | newly generated (configs `deepseek-chat-t00` / `-t08`) |

**Reconciliation.** The 12 cells at t=0.2 are exactly the main-campaign DeepSeek cells restricted to these 4 defects (outcome-for-outcome identical: True). DeepSeek detects 5/57 in the full Tier A arm and 5/12 within this subset, i.e. the subset captures **5/5 = 100% of that model's Tier A detections in 12/57 = 21% of its cells** (Fisher exact vs the remaining 45 cells, p = 0.0002).

**The reported counts 4/12, 5/12, 2/12 are arithmetically correct; this is not a bookkeeping error.** The discrepancy Reviewer #3 identified is real and is explained by the first of their two hypotheses: the arm is a purposively selected, outcome-enriched subset. The selection was made *after* observing outcomes at t=0.2, so the arm cannot support an unbiased estimate of the detection rate at any temperature, and the t=0.2 column is not an independent observation of the main campaign but a re-reading of the same cells.

**What survives.** Within-arm *contrasts* remain interpretable because the same 4 defects and 3 runs are held fixed across the three temperatures (a paired design): detection does not increase with temperature (4, 5, 2), and non-compilation is flat (3/12 at every temperature). What does not survive is any statement about the *level* of detection, and the non-compile flatness is carried entirely by a single defect (0cc73ebf, non-compilable in all 9 of its cells).

| temp | cells | detections | non-compilable | per-defect detections |
|---|---|---|---|---|
| 0.0 | 12 | 4 | 3 | 0cc73eb 0/3, 287eaab 0/3, 611cf0b 3/3, 6a13464 1/3 |
| 0.2 | 12 | 5 | 3 | 0cc73eb 0/3, 287eaab 0/3, 611cf0b 3/3, 6a13464 2/3 |
| 0.8 | 12 | 2 | 3 | 0cc73eb 0/3, 287eaab 0/3, 611cf0b 2/3, 6a13464 0/3 |

## Arm 2 -- Generation source (Section IV-D)

| field | value |
|---|---|
| design | 19 Tier A defects x 3 runs x 1 model, **paired** on (defect, run) |
| model | deepseek-chat only |
| projects | fmt, CLI11, entt -- the full Tier A set |
| selection | **not outcome-selected** |

- fixed-version arm: 5/57 detections, compile 42/57 = 74%
- buggy-version arm: 3/57 detections, compile 38/57 = 67%
- detected defects, fixed: 611cf0b3, 6a134640
- detected defects, buggy: 611cf0b3

This arm covers the whole Tier A defect set and is therefore free of the selection problem affecting the temperature arm.

## Arm 3 -- Prompt design (Section IV-D)

### 3a. Three-way arm (minimal / context-rich / guided)

| field | value |
|---|---|
| design | 14 fmt defects x 3 prompts x 1 run x 1 model = 14 cells per prompt |
| model | deepseek-chat only |
| projects | **fmt only** |
| compile rate | minimal 71%, context-rich 42%, guided 78% |
| detections | 2/14 under each of the three prompts |
| per-cell rows | **not present** in `data/results/`; aggregate only, in FINDINGS.md C11 |

### 3b. Paired arm (guided vs public-API-rich)

| field | value |
|---|---|
| script | `scripts/campaign_api_rich.py` |
| data | `data/results/campaign_api_rich.jsonl` (48 rows) |
| design | 24 defects x 2 prompts x 1 run x 1 model = 48 cells |
| model | deepseek-chat only |
| api_rich | 2/24 detections, 15/24 compile, defects: 287eaab3, 611cf0b3 |
| guided | 2/24 detections, 14/24 compile, defects: 611cf0b3, 6a134640 |

**Disclosure gap.** The 24 defects in the paired arm are fmt (14, Defects4C) plus cpp-peglib (10, BugsC++). The paper describes this arm as running "on the same 24 defects" without stating that it crosses the two benchmarks, and that 10 of the 24 come from the benchmark on which detection is 0/80 in the main campaign. Both prompts detect 2/24, and both of those detections are fmt defects.

## Arm 4 -- Model families (Section IV-D)

This arm is the main campaign re-tabulated, not a separate experiment: 19 defects x 4 models x 3 runs.

| model | cells | eligible | detections | reached-not-caught | share |
|---|---|---|---|---|---|
| gemini-2.5-flash | 57 | 24 | 1 | 4 | 17% |
| qwen3-coder | 57 | 38 | 2 | 14 | 37% |
| deepseek-chat | 57 | 39 | 5 | 13 | 33% |
| gpt-5.4-or | 57 | 40 | 8 | 13 | 32% |

## Summary of disclosure gaps to close in the revision

1. **Temperature arm is outcome-selected.** 4 fmt defects picked to span the outcomes already observed at t=0.2; 2 of the 4 are among the only 4 Tier A defects any model ever detects. It captures 100% of DeepSeek's Tier A detections in 21% of its cells. The level of detection in this arm carries no information; only the paired within-arm contrast across temperatures does.
2. **The t=0.2 column is not an independent replication** -- it re-reads the main campaign's cached cells, while t=0.0 and t=0.8 are freshly generated.
3. **Every robustness arm uses a single model** (deepseek-chat) and, except for the generation-source arm, a single project (fmt).
4. **The paired prompt arm silently mixes benchmarks** (fmt 14 + cpp-peglib 10).
5. **The three-way prompt arm has no per-cell record** in `data/results/`; only the aggregate survives, in FINDINGS.md C11.
