# E5 stage D -- 12-cell cross-configuration probe

Four Tier A **public-focal** defects x the three internal-campaign models x 1 run, generated with the internal campaign's **guided prompt**. Zero-repair compilation is read off `shot1.cpp`, the first generation.

**Pre-registered selection** (fixed before generation; refers only to project and defect hash, never to an outcome): cover all three Tier A projects, take the lexicographically smallest hash in each, and give the spare slot to fmt via its next smallest hash. Selected: `020a21af`, `ad5bb519`, `0cc73ebf`, `279d698e`.

Cells generated: **12/12**.

## Compilation

| configuration | compiles |
|---|---|
| guided prompt, **zero repair**, g++ | **4/12** |
| guided prompt, **zero repair**, clang++-18 | **4/12** |
| guided prompt, one repair round, g++ | 8/12 |
| guided prompt, one repair round, clang++-18 | 8/12 |

### By defect

| defect | project | zero repair, g++ | one repair, g++ |
|---|---|---|---|
| `020a21af` | CLI11 | 2/3 | 3/3 |
| `ad5bb519` | entt | 0/3 | 0/3 |
| `0cc73ebf` | fmt | 1/3 | 2/3 |
| `279d698e` | fmt | 1/3 | 3/3 |

## Verdict

Under the internal campaign's prompt and with no repair round, **4 of 12** suites for public focal functions still compile, against **0 of 126** for internal focal functions under the same prompt and repair budget. Combined with stages A-C, which show that removing the repair round costs 24 points and changing the front end costs none on the full Tier A set, the accessibility of the focal function is the factor left standing.

This remains a probe, not a controlled estimate: 12 cells, one run each, three projects, and the toolchain is clang-18 + libstdc++ rather than the internal campaign's clang-16 + libc++. One of the four pre-registered defects (`ad5bb519`, entt) fails to compile in all three of its cells even with the repair round, so the probe's public-focal compile rate is itself carried by the other three defects; that defect was fixed by the selection rule before generation and is reported rather than replaced.

## Files

| path | contents |
|---|---|
| `data/results/e5d_probe.jsonl` | one row per cell |
| `data/raw/<defect>/<model>/guided/run1/` | prompt, raw response, shot1.cpp, shot2.cpp, test.cpp, meta.json |
