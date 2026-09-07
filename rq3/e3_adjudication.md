# E3 -- RQ3 adjudication trace

## 1. Provenance of the annotation records

- `rq3_frozen44.csv` (`/home/truclinh/Thesis/kse2026/study/data/taxonomy/rq3_frozen44.csv`): 44 suites, the analysis set
- `rq3_coding_sheet.csv`: 47 rows -- 16 buggy-arm, 31 main campaign
- `rq3_coder1.csv` / `rq3_coder2.csv`: 47 rows each, 4 disagreements

**These are not the same population.** The shipped per-coder files record the first annotation round: 31 suites from the main campaign plus 16 from the buggy-generation arm that never enter the paper, and they predate the GPT-5.4 campaign. The 13 GPT-5.4 suites in the analysis set were annotated in a second round, so the analysis set is those same 31 main-campaign suites plus 13 new ones. The agreement figure reported in the paper (86.4%, 38/44) belongs to the merged `coder1`/`coder2` columns of `rq3_frozen44.csv`, not to the shipped per-coder files, which show 4 disagreements over a different population and give kappa = 0.80.

**Resolved.** `analysis/rq3_reconcile_labels.py` emits the round-2 files the README already referenced but that were never written (`rq3_delta_coder{1,2}.csv`) and a self-contained merged record keyed by (defect, model, run) (`rq3_labels_merged44.csv`), verifies the merge cell by cell against `rq3_frozen44.csv`, and recomputes kappa = 0.72 from shipped files alone.

## 2. Pre-adjudication agreement, recomputed

- Cohen's kappa = **0.72** (observed agreement 86.4%, chance agreement 51.2%), n = 44
- disagreements: **6/44**
- coder 1 label distribution: {'O4': 28, 'O2': 16}
- coder 2 label distribution: {'O4': 28, 'O2': 13, 'O3': 3}

This reproduces the value reported in the paper. It describes labels that were then replaced.

## 3. The three weak-assertion labels

- proposed by coder 1: 0; by coder 2: 3
- suites carrying any O3 proposal: **3**, all on defect(s) 96c18b26
- of those, inside the six disagreements: **3**
- surviving in the final labels: **0**

**Answer to the question asked: yes, completely.** Every O3 label ever proposed came from one annotator, on one defect, and every one of them is one of the six disagreements. The paper's statement that no miss is attributed to a weak assertion is therefore not a finding on which the two annotators independently agreed: it is the outcome of adjudicating three suites of a single defect against a probe. The claim should be stated that way.

## 4. What the probes actually changed

Six suites disagreed. They cluster on **3 distinct defects**, each settled by one executable probe that built the fixed and buggy versions and ran chosen inputs on both:

- `96c18b26` (3 suites) -- O3 vs O4 -- `data/taxonomy/rq3_probe_96c18b26c2.json`
- `971fb584` (2 suites) -- O2 vs O4 — is the get_arg(named-after-auto) bug observable through fmt::format? -- `data/taxonomy/rq3_probe_971fb584.json`
- `c04fb91b` (1 suites) -- O2 vs O4 -- `data/taxonomy/rq3_probe_c04fb91b.json`

| suite | coder 1 | coder 2 | final | overturned | new label? |
|---|---|---|---|---|---|
| `96c18b26` deepseek-chat run1 | O4 | O3 | **O4** | coder2 | no |
| `96c18b26` deepseek-chat run2 | O4 | O3 | **O4** | coder2 | no |
| `96c18b26` deepseek-chat run3 | O4 | O3 | **O4** | coder2 | no |
| `c04fb91b` gemini-2.5-flash run3 | O2 | O4 | **O2** | coder2 | no |
| `971fb584` gpt-5.4-or run1 | O2 | O4 | **O4** | coder1 | no |
| `971fb584` gpt-5.4-or run2 | O2 | O4 | **O4** | coder1 | no |

### The number Reviewer #3 asked for

| quantity | value |
|---|---|
| suites requiring adjudication | **6/44** |
| distinct defects probed | **3** |
| probe overturned coder 1 | **2/6** |
| probe overturned coder 2 | **4/6** |
| probe produced a label neither annotator proposed | **0/6** |
| suites where the two annotators already agreed and the probe was not invoked | 38/44 |

So the probes changed **6 of the 12 individual annotator judgements** they examined (2 of coder 1's, 4 of coder 2's), and in no case did the probe reach a label that neither annotator had proposed. Adjudication acted as a tie-break between two proposed labels rather than as an independent relabelling. This is a more useful reliability statement than kappa, because the reported labels are the post-adjudication ones and kappa describes the labels that were replaced.

### Suggested replacement text for Section III-D

> For RQ3, two annotators independently classified the 44 reached but not caught suites, agreeing on 38 of 44 (kappa = 0.72). The 6 disagreements fall on 3 defects and were settled by building both program versions and running chosen inputs on each. The probes overturned 2 of coder 1's labels and 4 of coder 2's, and never produced a label that neither annotator had proposed. All three too-weak-assertion labels occur inside these disagreements, on a single defect, and were resolved to insufficient input; the finding that no miss is attributed to a weak assertion therefore rests on adjudication rather than on independent agreement.
