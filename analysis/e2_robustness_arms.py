#!/usr/bin/env python3
"""E2 -- Full disclosure of every robustness arm (defects, models, prompts, runs,
provenance), and reconciliation of the temperature 0.2 arm against the main Tier A
campaign (Reviewer #3, Major Comment 2).

Reviewer #3 observed: Section IV-D reports 5 detections in 12 cells at temperature 0.2,
whereas DeepSeek-V4-flash detects 5 of 57 at the same temperature in the main Tier A
campaign (Fisher exact p = 0.011). This script establishes which of the two possible
explanations holds ("easier subset" vs "the arm differs in an unstated way").

Outputs:
  data/results/e2_robustness_arms.json   machine-readable arm inventory
  data/results/e2_robustness_arms.md     human-readable table for the paper + response letter

Run with cwd = artifact/ :  python3 analysis/e2_robustness_arms.py
"""
import json
from collections import Counter, defaultdict
from pathlib import Path

ART = Path(__file__).resolve().parent.parent
RES = ART / "data" / "results"

# The four Tier A defects reported in the paper as detected by at least one model/run.
DETECTABLE4 = {"020a21afc6", "611cf0b3c6", "6a13464059", "96c18b26c2"}

# Sample hard-coded in aggregate/sensitivity_temp.py, with its selection comment:
#   "sample spanning outcomes seen at 0.2 (2 detected, 1 reached-not-detected, 1 non-compilable)"
TEMP_SAMPLE = ["fmtlib___fmt@6a13464059", "fmtlib___fmt@611cf0b3c6",
               "fmtlib___fmt@287eaab3b2", "fmtlib___fmt@0cc73ebf79"]

MODEL_FILES = {
    "gemini-2.5-flash": "rows_tierA_gemini-2.5-flash_fixed.jsonl",
    "qwen3-coder": "rows_tierA_qwen3-coder_fixed.jsonl",
    "deepseek-chat": "rows_tierA_deepseek-chat_fixed.jsonl",
    "gpt-5.4-or": "rows_tierA_gpt-5.4-or_fixed.jsonl",
}


def load(name):
    return [json.loads(l) for l in (RES / name).read_text().splitlines() if l.strip()]


def short(defect_id):
    return defect_id.split("@")[1]


def fisher_2x2(a, b, c, d):
    """Two-sided Fisher exact p for [[a,b],[c,d]] without scipy."""
    from math import comb
    n = a + b + c + d
    r1, c1 = a + b, a + c

    def p(x):
        return comb(r1, x) * comb(n - r1, c1 - x) / comb(n, c1)

    p_obs = p(a)
    lo = max(0, c1 - (n - r1))
    hi = min(r1, c1)
    return sum(p(x) for x in range(lo, hi + 1) if p(x) <= p_obs * (1 + 1e-9))


def main():
    report = {}
    md = ["# E2 -- Robustness arm inventory and temperature reconciliation", ""]

    # ---------------- Main Tier A campaign (reference denominators) ----------------
    tierA = {}
    for model, fname in MODEL_FILES.items():
        rows = load(fname)
        tierA[model] = rows
    all_tierA = [r for rows in tierA.values() for r in rows]
    tierA_defects = sorted({short(r["defect_id"]) for r in all_tierA})
    detected_defects = sorted({short(r["defect_id"]) for r in all_tierA if r["real_bug_detected"]})

    report["main_tierA"] = {
        "cells": len(all_tierA),
        "defects": len(tierA_defects),
        "models": list(MODEL_FILES),
        "runs_per_defect_model": 3,
        "temperature": 0.2,
        "prompt": "minimal (focal function source + path)",
        "condition": "fixed",
        "detected_defects": detected_defects,
        "per_model_detections": {m: sum(1 for r in rs if r["real_bug_detected"])
                                 for m, rs in tierA.items()},
    }

    md += ["## Reference: main Tier A campaign", "",
           f"- {len(all_tierA)} cells = {len(tierA_defects)} defects x {len(MODEL_FILES)} models x 3 runs",
           f"- temperature 0.2, minimal prompt, condition=fixed",
           f"- defects detected by at least one model/run: {len(detected_defects)} "
           f"({', '.join(d[:8] for d in detected_defects)})", ""]

    # ---------------- ARM 1: temperature ----------------
    temp_rows = load("sensitivity_temp.jsonl")
    sample_short = [short(d) for d in TEMP_SAMPLE]
    overlap = sorted(set(sample_short) & DETECTABLE4)

    ds_fixed = tierA["deepseek-chat"]
    ds_sub = [r for r in ds_fixed if short(r["defect_id"]) in set(sample_short)]
    ds_sub_det = sum(1 for r in ds_sub if r["real_bug_detected"])
    ds_all_det = sum(1 for r in ds_fixed if r["real_bug_detected"])

    # Does the 0.2 arm reproduce the main-campaign cells exactly?
    arm02 = {(r["defect"], r["run"]): r for r in temp_rows if r["temp"] == 0.2}
    main02 = {(short(r["defect_id"])[:7], r["run_id"]): r for r in ds_sub}
    identical = all(
        arm02[k]["outcome"] == main02[k]["outcome"] and
        arm02[k]["det"] == main02[k]["real_bug_detected"]
        for k in arm02 if k in main02
    ) and set(arm02) == set(main02)

    p_fisher = fisher_2x2(ds_sub_det, len(ds_sub) - ds_sub_det,
                          ds_all_det - ds_sub_det, (len(ds_fixed) - len(ds_sub)) - (ds_all_det - ds_sub_det))

    temp_summary = {}
    for t in sorted({r["temp"] for r in temp_rows}):
        rs = [r for r in temp_rows if r["temp"] == t]
        per_defect = defaultdict(lambda: [0, 0])
        for r in rs:
            per_defect[r["defect"]][1] += 1
            if r["det"]:
                per_defect[r["defect"]][0] += 1
        temp_summary[str(t)] = {
            "cells": len(rs),
            "detections": sum(1 for r in rs if r["det"]),
            "non_compilable": sum(1 for r in rs if r["outcome"] == "non_compilable"),
            "per_defect": {k: f"{v[0]}/{v[1]}" for k, v in sorted(per_defect.items())},
            "outcomes": dict(Counter(r["outcome"] for r in rs)),
        }

    report["arm_temperature"] = {
        "script": "aggregate/sensitivity_temp.py",
        "data": "data/results/sensitivity_temp.jsonl",
        "design": "4 fmt defects x 3 temperatures x 3 runs x 1 model (deepseek-chat) = 36 cells",
        "model": "deepseek-chat (deepseek-v4-flash) only",
        "project_coverage": "fmt only -- CLI11 and entt absent",
        "defects": sample_short,
        "selection_rule_verbatim": ("sample spanning outcomes seen at 0.2 "
                                    "(2 detected, 1 reached-not-detected, 1 non-compilable)"),
        "selection_is_outcome_dependent": True,
        "overlap_with_detectable4": overlap,
        "overlap_rate": f"{len(overlap)}/4",
        "tierA_base_rate": f"{len(detected_defects)}/{len(tierA_defects)}",
        "provenance": {
            "0.0": "newly generated (model config deepseek-chat-t00)",
            "0.2": "REUSED from the main Tier A campaign cache (no new generation)",
            "0.8": "newly generated (model config deepseek-chat-t08)",
        },
        "arm02_reproduces_main_campaign_cells": identical,
        "deepseek_detections_in_subset": ds_sub_det,
        "deepseek_detections_in_full_arm": ds_all_det,
        "subset_captures_all_detections": ds_sub_det == ds_all_det,
        "fisher_p_subset_vs_remainder": round(p_fisher, 4),
        "per_temperature": temp_summary,
        "bookkeeping_error": False,
    }

    md += ["## Arm 1 -- Temperature (Section IV-D)", "",
           "| field | value |", "|---|---|",
           "| script | `aggregate/sensitivity_temp.py` |",
           "| data | `data/results/sensitivity_temp.jsonl` |",
           "| design | 4 fmt defects x 3 temperatures x 3 runs x 1 model = 36 cells |",
           "| model | deepseek-chat (deepseek-v4-flash) only |",
           "| projects | fmt only (no CLI11, no entt) |",
           f"| defects | {', '.join(d[:8] for d in sample_short)} |",
           f"| selection rule (verbatim from source) | \"sample spanning outcomes seen at 0.2 "
           f"(2 detected, 1 reached-not-detected, 1 non-compilable)\" |",
           "| selection is outcome-dependent | **YES** |",
           f"| overlap with the 4 detectable Tier A defects | **{len(overlap)}/4** "
           f"({', '.join(d[:8] for d in overlap)}) vs base rate {len(detected_defects)}/{len(tierA_defects)} |",
           "| t=0.2 provenance | **reused** from the main campaign cache |",
           "| t=0.0, t=0.8 provenance | newly generated (configs `deepseek-chat-t00` / `-t08`) |", "",
           f"**Reconciliation.** The 12 cells at t=0.2 are exactly the main-campaign DeepSeek cells "
           f"restricted to these 4 defects (outcome-for-outcome identical: {identical}). "
           f"DeepSeek detects {ds_all_det}/{len(ds_fixed)} in the full Tier A arm and "
           f"{ds_sub_det}/{len(ds_sub)} within this subset, i.e. the subset captures "
           f"**{ds_sub_det}/{ds_all_det} = 100% of that model's Tier A detections in "
           f"{len(ds_sub)}/{len(ds_fixed)} = 21% of its cells** "
           f"(Fisher exact vs the remaining 45 cells, p = {p_fisher:.4f}).", "",
           "**The reported counts 4/12, 5/12, 2/12 are arithmetically correct; this is not a "
           "bookkeeping error.** The discrepancy Reviewer #3 identified is real and is explained "
           "by the first of their two hypotheses: the arm is a purposively selected, "
           "outcome-enriched subset. The selection was made *after* observing outcomes at t=0.2, "
           "so the arm cannot support an unbiased estimate of the detection rate at any "
           "temperature, and the t=0.2 column is not an independent observation of the main "
           "campaign but a re-reading of the same cells.", "",
           "**What survives.** Within-arm *contrasts* remain interpretable because the same 4 "
           "defects and 3 runs are held fixed across the three temperatures (a paired design): "
           "detection does not increase with temperature (4, 5, 2), and non-compilation is flat "
           "(3/12 at every temperature). What does not survive is any statement about the *level* "
           "of detection, and the non-compile flatness is carried entirely by a single defect "
           "(0cc73ebf, non-compilable in all 9 of its cells).", ""]

    md += ["| temp | cells | detections | non-compilable | per-defect detections |",
           "|---|---|---|---|---|"]
    for t, s in temp_summary.items():
        pd_str = ", ".join(f"{k[:7]} {v}" for k, v in s["per_defect"].items())
        md += [f"| {t} | {s['cells']} | {s['detections']} | {s['non_compilable']} | {pd_str} |"]
    md += [""]

    # ---------------- ARM 2: generation source ----------------
    fixed = load("rows_tierA_deepseek-chat_fixed.jsonl")
    buggy = load("rows_tierA_deepseek-chat_buggy.jsonl")
    src = {}
    for label, rows in (("fixed", fixed), ("buggy", buggy)):
        src[label] = {
            "cells": len(rows),
            "defects": len({short(r["defect_id"]) for r in rows}),
            "compiled": sum(1 for r in rows if r["compiled_fixed"]),
            "compile_rate": round(100 * sum(1 for r in rows if r["compiled_fixed"]) / len(rows)),
            "detections": sum(1 for r in rows if r["real_bug_detected"]),
            "detected_defects": sorted({short(r["defect_id"]) for r in rows if r["real_bug_detected"]}),
        }
    report["arm_generation_source"] = {
        "script": "aggregate/run_tier.py --condition {fixed,buggy}",
        "data": ["data/results/rows_tierA_deepseek-chat_fixed.jsonl",
                 "data/results/rows_tierA_deepseek-chat_buggy.jsonl"],
        "design": "19 Tier A defects x 3 runs x 1 model (deepseek-chat), paired on defect and run",
        "model": "deepseek-chat only",
        "projects": "fmt, CLI11, entt (full Tier A)",
        "temperature": 0.2,
        "arms": src,
        "note": "This arm is NOT outcome-selected: it covers the full Tier A defect set.",
    }
    md += ["## Arm 2 -- Generation source (Section IV-D)", "",
           "| field | value |", "|---|---|",
           "| design | 19 Tier A defects x 3 runs x 1 model, **paired** on (defect, run) |",
           "| model | deepseek-chat only |",
           "| projects | fmt, CLI11, entt -- the full Tier A set |",
           "| selection | **not outcome-selected** |", "",
           f"- fixed-version arm: {src['fixed']['detections']}/{src['fixed']['cells']} detections, "
           f"compile {src['fixed']['compiled']}/{src['fixed']['cells']} = {src['fixed']['compile_rate']}%",
           f"- buggy-version arm: {src['buggy']['detections']}/{src['buggy']['cells']} detections, "
           f"compile {src['buggy']['compiled']}/{src['buggy']['cells']} = {src['buggy']['compile_rate']}%",
           f"- detected defects, fixed: {', '.join(d[:8] for d in src['fixed']['detected_defects'])}",
           f"- detected defects, buggy: {', '.join(d[:8] for d in src['buggy']['detected_defects'])}", "",
           "This arm covers the whole Tier A defect set and is therefore free of the selection "
           "problem affecting the temperature arm.", ""]

    # ---------------- ARM 3: prompt design ----------------
    api_rich = load("campaign_api_rich.jsonl")
    by_cond = defaultdict(list)
    for r in api_rich:
        by_cond[r["cond"]].append(r)
    paired = {}
    for cond, rows in sorted(by_cond.items()):
        projs = Counter(r["project"] for r in rows)
        paired[cond] = {
            "cells": len(rows),
            "defects": len({r["defect_id"] for r in rows}),
            "projects": dict(projs),
            "compiled": sum(1 for r in rows if r["compiled"]),
            "detections": sum(1 for r in rows if r["detected"]),
            "detected_defects": sorted({short(r["defect_id"]) for r in rows if r["detected"]}),
        }
    report["arm_prompt_paired"] = {
        "script": "scripts/campaign_api_rich.py",
        "data": "data/results/campaign_api_rich.jsonl",
        "design": "24 defects x 2 prompts x 1 run x 1 model (deepseek-chat) = 48 cells",
        "model": "deepseek-chat only",
        "MIXED_BENCHMARK_WARNING": ("the 24 defects are fmt(14) from Defects4C + "
                                    "cpp-peglib(10) from BugsC++ -- the paper does not disclose "
                                    "that this arm crosses the two benchmarks"),
        "arms": paired,
    }
    report["arm_prompt_threeway"] = {
        "script": "scripts/ablation_a.py (+ a third 'guided' variant)",
        "source_of_record": "study/FINDINGS.md C11 (console log: study/logs/api_rich.log)",
        "design": "14 fmt defects x 3 prompts x 1 run x 1 model (deepseek-chat) = 14 cells per prompt",
        "model": "deepseek-chat only",
        "projects": "fmt only",
        "compile_rate": {"minimal": "71%", "context_rich": "42%", "guided": "78%"},
        "detections": {"minimal": "2/14", "context_rich": "2/14", "guided": "2/14"},
        "note": "per-cell rows for the three-way arm are not in data/results/; only the "
                "aggregate is recorded in FINDINGS.md C11",
    }

    md += ["## Arm 3 -- Prompt design (Section IV-D)", "",
           "### 3a. Three-way arm (minimal / context-rich / guided)", "",
           "| field | value |", "|---|---|",
           "| design | 14 fmt defects x 3 prompts x 1 run x 1 model = 14 cells per prompt |",
           "| model | deepseek-chat only |",
           "| projects | **fmt only** |",
           "| compile rate | minimal 71%, context-rich 42%, guided 78% |",
           "| detections | 2/14 under each of the three prompts |",
           "| per-cell rows | **not present** in `data/results/`; aggregate only, in FINDINGS.md C11 |", "",
           "### 3b. Paired arm (guided vs public-API-rich)", "",
           "| field | value |", "|---|---|",
           "| script | `scripts/campaign_api_rich.py` |",
           "| data | `data/results/campaign_api_rich.jsonl` (48 rows) |",
           "| design | 24 defects x 2 prompts x 1 run x 1 model = 48 cells |",
           "| model | deepseek-chat only |"]
    for cond in sorted(paired):
        p = paired[cond]
        md += [f"| {cond} | {p['detections']}/{p['cells']} detections, "
               f"{p['compiled']}/{p['cells']} compile, defects: "
               f"{', '.join(d[:8] for d in p['detected_defects'])} |"]
    md += ["", "**Disclosure gap.** The 24 defects in the paired arm are fmt (14, Defects4C) plus "
           "cpp-peglib (10, BugsC++). The paper describes this arm as running \"on the same 24 "
           "defects\" without stating that it crosses the two benchmarks, and that 10 of the 24 "
           "come from the benchmark on which detection is 0/80 in the main campaign. Both prompts "
           "detect 2/24, and both of those detections are fmt defects.", ""]

    # ---------------- ARM 4: model families ----------------
    fam = {}
    for model, rows in tierA.items():
        eligible = [r for r in rows if r["real_bug_detected"] is not None]
        rnc = [r for r in eligible if r["outcome"] == "reached_not_detected"]
        fam[model] = {
            "cells": len(rows),
            "eligible": len(eligible),
            "detections": sum(1 for r in eligible if r["real_bug_detected"]),
            "reached_not_caught": len(rnc),
            "reached_not_caught_share": round(100 * len(rnc) / len(eligible)) if eligible else None,
        }
    report["arm_model_families"] = {
        "data": [f"data/results/{f}" for f in MODEL_FILES.values()],
        "design": "19 Tier A defects x 4 models x 3 runs = 228 cells (the main campaign itself)",
        "per_model": fam,
    }
    md += ["## Arm 4 -- Model families (Section IV-D)", "",
           "This arm is the main campaign re-tabulated, not a separate experiment: "
           "19 defects x 4 models x 3 runs.", "",
           "| model | cells | eligible | detections | reached-not-caught | share |",
           "|---|---|---|---|---|---|"]
    for m, f in fam.items():
        md += [f"| {m} | {f['cells']} | {f['eligible']} | {f['detections']} | "
               f"{f['reached_not_caught']} | {f['reached_not_caught_share']}% |"]
    md += [""]

    # ---------------- Summary of disclosure gaps ----------------
    md += ["## Summary of disclosure gaps to close in the revision", "",
           "1. **Temperature arm is outcome-selected.** 4 fmt defects picked to span the outcomes "
           "already observed at t=0.2; 2 of the 4 are among the only 4 Tier A defects any model "
           "ever detects. It captures 100% of DeepSeek's Tier A detections in 21% of its cells. "
           "The level of detection in this arm carries no information; only the paired "
           "within-arm contrast across temperatures does.",
           "2. **The t=0.2 column is not an independent replication** -- it re-reads the main "
           "campaign's cached cells, while t=0.0 and t=0.8 are freshly generated.",
           "3. **Every robustness arm uses a single model** (deepseek-chat) and, except for the "
           "generation-source arm, a single project (fmt).",
           "4. **The paired prompt arm silently mixes benchmarks** (fmt 14 + cpp-peglib 10).",
           "5. **The three-way prompt arm has no per-cell record** in `data/results/`; only the "
           "aggregate survives, in FINDINGS.md C11.", ""]

    (RES / "e2_robustness_arms.json").write_text(json.dumps(report, indent=2))
    (RES / "e2_robustness_arms.md").write_text("\n".join(md))
    print("\n".join(md))
    print(f"\n[written] {RES/'e2_robustness_arms.json'}")
    print(f"[written] {RES/'e2_robustness_arms.md'}")


if __name__ == "__main__":
    main()
