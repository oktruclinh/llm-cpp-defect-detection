# Replication Package

**Reaching Is Not Revealing: Why LLM-Generated Unit Tests Miss Real C++ Bugs** (KSE 2026)

This package contains everything behind the numbers in the paper: the test suites the models
actually produced, the raw API responses that produced them, the harness that compiled and ran
them, the analysis scripts, and the human annotation labels with the probes used to settle
disagreements.

The **generated suites are the reproducibility anchor**, not re-sampling. Providers do not
guarantee that a supplied seed is honoured — non-determinism was observed even at temperature 0 —
so re-running generation will not reproduce the exact suites. Every generated suite is therefore
shipped verbatim, and every number in the paper can be recomputed from the shipped data without
spending a cent on API calls.

---

## 1. Quick start

Everything except generation runs offline from the shipped data.

```bash
python3 analysis/conditioned_stats.py      # RQ2 detection rates + Wilson CIs
python3 analysis/taxonomy_rq1.py           # RQ1 compile-failure taxonomy
python3 benchmarks/defects4c_adapter.py --summary   # subject pool (241 -> 93)
```

Requires Python 3.10+ and no third-party packages. Re-running the *generation* or the C++
*build/execution* steps needs more setup — see §5.

---

## 2. Layout

| Path | What it holds |
|---|---|
| `data/raw/`, `data/raw_n3/` | The **453** generated `test.cpp` files, one directory per `defect / model / run`, each with `meta.json` (model version, seed, temperature, token counts) and `eval.json` (per-test-case outcomes). |
| `data/results/*.jsonl` | One row per generation cell: compilation, validity, detection, reachability. These files back every rate in the paper. |
| `data/taxonomy/` | Human annotation: `CODEBOOK.md` (label definitions and decision tree), per-coder labels, the frozen 44-suite analysis set, the six adjudicated disagreements, and the executable probes that resolved them. Free-text annotator notes are not included. |
| `generate/` | Prompt templates (`prompt.py`), API client (`llm_client.py`), generation with one compile-repair round (`generate_tests.py`). |
| `harness/` | `cpp_harness.py` (header-only builds, g++), `tierbc_harness.py` (project builds, clang-16 + libc++). Builds the suite, runs it, swaps the focal file to the pre-fix commit, re-runs. |
| `measure/` | `detection.py` (validity + detection protocol), `coverage.py` (gcov reachability of the patched hunk). |
| `analysis/` | Statistics, taxonomy classifier, κ computation, annotation tooling. |
| `benchmarks/` | `defects4c_adapter.py`, `cpp_peglib_adapter.py` — normalise each benchmark into flat defect records. |
| `aggregate/`, `scripts/` | Campaign drivers (one per campaign in Table I). |
| `config/` | `models.json` (endpoints, decoding), `repos.json` (local project checkouts), `MODELS_PROVENANCE.md` (provider, API model id, access dates, seed-support caveat). |

---

## 3. Paper number → where it comes from

Every quantity in the paper, with the file that contains it and the command that recomputes it.

### Study Design

| Paper | Value | Source |
|---|---|---|
| Subject pool | 241 → 93 defects (19/26/48) | `benchmarks/defects4c_adapter.py --summary` |
| Campaigns (Table I) | 45 defects, 378 cells | `data/results/*.jsonl` (row counts per campaign) |
| Accessibility labels | 24 of 93 directly callable | audit described in Sec. III of the paper; labels in `data/taxonomy/` |

### RQ1 — compilation

| Paper | Value | Source |
|---|---|---|
| Per-model non-compile | 16 / 26 / 30 / 54 % | `data/results/rows_tierA_*_fixed.jsonl`, field `outcome == "non_compilable"` |
| Taxonomy (Table II) | 44 / 17 / 15 / 18 / 6 % | `data/taxonomy/rq1_final_adjudicated.csv`, field `final_code`; categories defined in `data/taxonomy/CODEBOOK.md`; classifier `analysis/taxonomy_rq1.py` |
| Internal wall | 126 of 126 | `campaign_internal.jsonl` + `campaign_internal_v2.jsonl` + `campaign_spirv.jsonl`, minus the 12 cells of the two reclassified nng defects |
| Pre-audit rate | 136 of 138 | same three files, all 23 candidates |

### RQ2 — detection

| Paper | Value | Source |
|---|---|---|
| Pooled detection | 16 / 141 = 11 % (CI 7–18) | `analysis/conditioned_stats.py` |
| All attempts | 16 / 228 = 7 % (CI 4–11) | same |
| Per model (unconditional) | 1 / 2 / 5 / 8 detections of 57 | `rows_tierA_<model>_fixed.jsonl` |
| Never reached / reached-but-not-caught | 57 % / 31 % | `outcome` field: `not_reached`, `reached_not_detected` |
| Invalid test cases | 674 / 1810 = 37 % | fields `n_tests`, `n_invalid_on_fixed` |
| Strict detection | 0 of 14 clean suites | suites with `n_invalid_on_fixed == 0` and `n_flaky_fixed == 0` |
| Arrow probes | 2 of 6 compile; `Flush` 0 of 6 | `arrow_public.jsonl`, `campaign_arrow_cc.jsonl` |
| BugsC++ | 0 of 80 | `campaign_cpp_peglib.jsonl` |

### RQ3 — why reached defects are missed

| Paper | Value | Source |
|---|---|---|
| Frozen analysis set | 44 suites | `data/taxonomy/rq3_frozen44.csv` |
| Split | 30 infection / 14 propagation | field `final_after_adjudication` (`O4` / `O2`) |
| Agreement | 86.4 %, κ = 0.72, 6 disagreements | `rq3_coder1.csv`, `rq3_coder2.csv`, `rq3_delta_coder{1,2}.csv`; κ computed **before** adjudication |
| Disagreement resolution | 6 cells | `rq3_adjudicate6.csv` |
| Executable probes | 3 defects | `rq3_probe_971fb584.json`, `rq3_probe_c04fb91b.json`, `rq3_probe_96c18b26c2.json` |
| Intervention checks | 2 infection + 1 propagation | `rq3_confirmation.json` |

### Robustness and reliability

| Paper | Value | Source |
|---|---|---|
| Generation source | 3 vs 5 detections of 57 | `rows_tierA_deepseek-chat_buggy.jsonl` vs `..._fixed.jsonl` |
| Public-API-rich prompt | 2 of 24 either way | `campaign_api_rich.jsonl`, field `cond` |
| Temperature | 4 / 5 / 2 of 12 | `sensitivity_temp.jsonl`, field `temp` |
| RQ1 agreement | κ = 0.56 on 25 cells | `rq1_labmate.csv` vs `rq1_final.csv`; `analysis/rq1_labmate_kappa.py` |
| Classifier fit | 94 % of audited labels | `analysis/rq1_finalize.py` |

The three-way prompt ablation (71 / 42 / 78 % compilation, 2 of 14 detections) predates
per-cell logging: its generated suites are under `data/raw/`, but no aggregated `.jsonl` exists
for that arm.

---

## 4. Protocol in one paragraph

A generation attempt produces one GoogleTest suite of one or more test cases. Compilation is
judged per suite. A test case is **valid** if it passes every run on the fixed version — five
runs for header-only builds, three for project builds; a case that always fails is invalid, one
with mixed outcomes is flaky, and both are excluded. A valid case **detects** the defect only if
it fails every run on the buggy version, which is the fixed tree with the focal file replaced by
its pre-fix version. For suites that compile and contain a valid case but detect nothing, `gcov`
checks whether the patched hunk executed, splitting the miss into *reached but not caught* and
*never reached*. Crashes and timeouts without a test report count as misses.

---

## 5. Re-running the pipeline

**Analysis only (offline, free)** — everything in §3 recomputes from shipped data.

**Build and execution** requires the project checkouts listed in `config/repos.json` and, for
project builds, clang-16 with libc++ (older commits do not compile under modern libstdc++). Header-only builds need only g++ and a local GoogleTest.

**Generation** requires API keys in `config/secrets.env` (never committed; see below) and costs
real money. It is not needed to verify any number in the paper.

---

## 6. Notes and caveats

- **No secrets in this repository.** `config/secrets.env` is git-ignored and has never been
  committed. Provide your own keys if you re-run generation.
- **Seeds are not a reproducibility guarantee.** The run index is passed as `seed` where the API
  accepts it, but adherence is provider-dependent. Reproduce from the shipped outputs.
- **Model versions drift.** Hosted model identifiers change; `config/MODELS_PROVENANCE.md`
  records the exact provider, API model id, endpoint, decoding parameters, and access dates.
- **No LLM was used for annotation.** All RQ1 and RQ3 labels are human. Agreement statistics use
  the independently assigned labels, computed before disagreements were resolved.
- **Known limitations** are stated in the paper's Threats to Validity.
