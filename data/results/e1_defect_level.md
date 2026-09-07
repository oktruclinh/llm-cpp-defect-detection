# E1 -- Defect-level verification and cluster-aware re-analysis

## 1. Internal campaign -- verification of the 0/21 claim

- pre-audit candidate set: 23 defects, 138 cells
- reclassified to exported public by the audit: 2 defects (nanomsg___nng@6a59b15fba, nanomsg___nng@e3b8f31b04), 12 cells, of which 2 compiled
- audited internal set: **21 defects, 126 cells**
- cells that compiled: **0**
- defects with at least one compiling suite: **0**
- composition: SPIRV-Tools 5, cppcheck 15, nng 1
- models: deepseek-chat, gemini-2.5-flash, qwen3-coder (3 models x 2 runs)

**VERIFIED: True**

| estimand | value |
|---|---|
| per-attempt (compiling suites / cells) | 0/126 = 0% (95% Wilson CI 0.0-3.0%) |
| per-defect (defects with any compiling suite / defects) | 0/21 = 0% (95% Wilson CI 0.0-15.5%) |

Both cells that compiled in the pre-audit set of 138 belonged to the reclassified `nng@6a59b15fba` (`nng_recv`), so the audited internal subset contains no compiling suite at either granularity. The per-defect interval is the honest one to quote: **0/21, 95% CI 0-15.5%**, against 0-3.0% at cell level.

## 2. Tier A -- verification of the 4/19 claim and both estimands

- 228 cells over 19 defects x 4 models x 3 runs
- eligible suites (compile + at least one valid case): 141
- detections: 16
- defects detected by at least one model/run: **4** (020a21af, 611cf0b3, 6a134640, 96c18b26)
- paper claims: 020a21af, 611cf0b3, 6a134640, 96c18b26

**VERIFIED: True** -- the four hashes match exactly.

| estimand | point | cell-level Wilson | defect-cluster bootstrap |
|---|---|---|---|
| per-attempt, conditional on eligibility | 16/141 = 11% | 7.1-17.6% | **1.5-25.5%** |
| per-attempt, all attempts | 16/228 = 7% | 4.4-11.1% | **0.9-15.8%** |
| per-defect, detected at least once | 4/19 = 21% | 8.5-43.3% | n/a (defect is the unit) |

The cluster bootstrap widens the conditional per-attempt interval from 7.1-17.6% to 1.5-25.5%, i.e. by a factor of 2.3. 0 of 10000 resamples were dropped for having an empty denominator.

## 3. Per model, both estimands

| model | detections | per-attempt (all 57) | cluster bootstrap | defects detected |
|---|---|---|---|---|
| gemini-2.5-flash | 1/57 = 2% | 0.3-9.3% | **0.0-5.3%** | 1/19 |
| qwen3-coder | 2/57 = 4% | 1.0-11.9% | **0.0-10.5%** | 1/19 |
| deepseek-chat | 5/57 = 9% | 3.8-18.9% | **0.0-22.8%** | 2/19 |
| gpt-5.4-or | 8/57 = 14% | 7.3-25.3% | **3.5-28.1%** | 4/19 |

Every per-model cluster interval spans the others' point estimates, so the apparent ordering Gemini < Qwen < DeepSeek < GPT-5.4 is not supported once clustering is accounted for. At the defect level the models detect gemini-2.5-flash 1/19, qwen3-coder 1/19, deepseek-chat 2/19, gpt-5.4-or 4/19. Note that GPT-5.4 alone accounts for all four detectable defects: no defect is detected by another model that GPT-5.4 misses, so the union over four models adds nothing over GPT-5.4 alone.

## 4. Statements to update in the paper

| location | current | replace with |
|---|---|---|
| Abstract | "only 11% of suites that compile and contain valid tests detect their defect" | keep, and add the defect-level companion: 4 of 19 defects are detected by any model or run |
| Finding 1 | "None of the 126 suites ... compiles" | add "and none of the 21 audited defects obtains a compiling suite (95% CI 0-15.5%)" |
| Finding 2 | "only 16 detect their defect" | add "across 4 of 19 distinct defects" |
| Table III pooled row | 11% (95% CI 7-18%) | 11% (defect-cluster bootstrap 95% CI 1.5-25.5%) |
| Conclusion | "11% of compilable suites ... detect their defect" | add the Figure 1 caption qualification and the defect-level rate |
