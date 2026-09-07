# E5 -- Cross-configuration probe

The internal-vs-public contrast confounds four factors: focal accessibility, prompt, compiler, and compile-repair budget. Three of them are isolated below on the full 19-defect Tier A set from data already in the package.

## A. Repair budget

`shots` is 1 exactly when the first generation compiled on the fixed version and 2 when a compile-repair round was taken, so the zero-repair compile rate is the share of cells with `shots == 1`. The internal campaign had no repair round; Tier A had one.

| model | compile rate, one repair (as published) | compile rate, zero repair | points contributed by repair |
|---|---|---|---|
| gemini-2.5-flash | 26/57 = 46% | 17/57 = 30% | +16 |
| qwen3-coder | 48/57 = 84% | 30/57 = 53% | +32 |
| deepseek-chat | 42/57 = 74% | 28/57 = 49% | +25 |
| gpt-5.4-or | 40/57 = 70% | 27/57 = 47% | +23 |
| **pooled** | **156/228 = 68%** | **102/228 = 45%** | **+24** |

The repair round is worth about 24 percentage points. **Removing it entirely still leaves 45% of public-focal suites compiling, against 0% for internal focal functions.** The repair budget therefore cannot account for the internal result.

## B. Compiler

Every Tier A suite recompiled with `clang++-18` instead of g++. The suite text is the post-repair `test.cpp`, and the repair round was driven by g++ diagnostics, so this measures how far a g++-adapted suite carries over to another front end. clang-16 + libc++ (the internal campaign's toolchain) is not installed on this machine; clang-18 + libstdc++ is the substitute.

| model | g++ | clang++-18 | both | g++ only | clang only |
|---|---|---|---|---|---|
| gemini-2.5-flash | 26/57 = 46% | 26/57 = 46% | 26 | 0 | 0 |
| qwen3-coder | 48/57 = 84% | 48/57 = 84% | 48 | 0 | 0 |
| deepseek-chat | 42/57 = 74% | 40/57 = 70% | 40 | 2 | 0 |
| gpt-5.4-or | 40/57 = 70% | 40/57 = 70% | 40 | 0 | 0 |
| **pooled** | **156/228 = 68%** | **154/228 = 68%** | 154 | 2 | 0 |

Switching front end moves the public-focal compile rate from 68% to 68%. **It does not move it to 0%**, so the compiler cannot account for the internal result either.

## C. Prompt

The guided prompt used by the internal campaign was also run on Tier A fmt defects in the paired prompt arm, same model (deepseek-chat).

| prompt | compile rate on fmt |
|---|---|
| minimal (main Tier A campaign, 3 runs/defect) | 32/42 = 76% |
| guided (paired prompt arm, 1 run/defect) | 11/14 = 79% |

The guided prompt does not depress compilation on public focal functions; the three-way prompt arm reported in the paper puts it slightly ahead of minimal (78% vs 71%). So the prompt cannot account for the internal result.

## Verdict

| factor | its effect on public-focal compilation | can it explain 0/126? |
|---|---|---|
| compile-repair budget removed | 68% -> 45% | no |
| compiler front end changed | 68% -> 68% | no |
| prompt switched to guided | no decrease | no |

Each confound the campaign design leaves open moves the public-focal compile rate by a bounded amount and none of them approaches zero. The remaining difference between the campaigns is focal accessibility. This is a bound on the alternatives rather than a controlled estimate of the accessibility effect, because the factors are varied one at a time on the public side and not crossed with accessibility; stage D generates the fully combined configuration.

## Files

| path | contents |
|---|---|
| `data/results/e5_cross_config.json` | all counts, machine-readable |
| `data/results/e5_clang_recompile.csv` | per-cell g++ vs clang outcome and the first clang error |
