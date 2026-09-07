# Derived numbers after the E4 reclassification (44 -> 41)

Suites removed from the RQ3 population: `279d698e` deepseek-chat run3 (O4), `fc6e0fe9` gemini-2.5-flash run3 (O2), `fc6e0fe9` gpt-5.4-or run1 (O2)

## 1a. Project composition

| population | fmt | CLI11 | total |
|---|---|---|---|
| 44 (as published) | 42 | 2 | 44 |
| **41 (group A)** | **39** | **2** | **41** |

All three removed suites are fmt, so the claim "39 of the 41 RQ3 suites come from fmt" is **correct**.

## 1b. Single-defect concentration in Threats

| population | infection (O4) | top defect | propagation (O2) | top defect |
|---|---|---|---|---|
| 44 | 30 | `96c18b26` 8/30 | 14 | `287eaab3` 8/14 |
| 41 | 29 | `96c18b26` 8/29 | 12 | `287eaab3` 8/12 |

Neither `96c18b26` nor `287eaab3` loses a suite in the reclassification, so both numerators stay at 8 and the claim "**8 of the 29 infection and 8 of the 12 propagation**" is **correct**.

Worth noting for the wording: the propagation concentration *tightens* from 8/14 = 57% to 8/12 = 67%. The Threats sentence understates the clustering if the percentage is left implicit.

## 1c. Model families -- reached but not caught share

Eligibility (compile + at least one valid case) is unaffected by the E4 reclassification, so only the numerators move.

| model | eligible | pop 44 | share | pop 41 | share |
|---|---|---|---|---|---|
| Gemini-2.5-Flash | 24 | 4 | 17% | **3** | **13%** (exact 12.5%) |
| Qwen3-Coder-30B | 38 | 14 | 37% | **14** | **37%** (exact 36.8%) |
| DeepSeek-V4-flash | 39 | 13 | 33% | **12** | **31%** (exact 30.8%) |
| GPT-5.4 | 40 | 13 | 33% | **12** | **30%** (exact 30.0%) |

**The published range 17% to 37% must become 13% to 37%.** The low end moves because Gemini-2.5-Flash has the smallest eligible set (24) and loses one of its 4 flagged suites; the high end is unchanged because Qwen3-Coder-30B loses none.

Caution: Gemini-2.5-Flash's share is 3/24 = 12.5% exactly, a tie. Rounded half up it is 13%; Python's default banker's rounding would print 12%. Quote 13% and a reviewer recomputing gets 12.5%, which is consistent; quoting the lower value is not.

Suggested sentence: "Among each model's suites that compile and contain at least one valid test case, the reached but not caught share ranges from 13\% to 37\%."

## Cross-check

- eligible suites, all models: 141 (paper: 141) -> OK
- reached but not caught, pop 41: 41 -> 29% of 141
- never reached, pop 41: 84 -> 60% of 141 (16 detections are unchanged by E4)
