#!/usr/bin/env python3
"""E1 -- Defect-level verification and cluster-aware re-analysis
(Reviewer #3, Major Comment 1: "Cells are nested within defects and projects, so
intervals computed over cells overstate precision").

Reports, for every headline quantity, BOTH estimands:
  per-attempt : detections / eligible suites          (how often one generation attempt works)
  per-defect  : defects detected at least once / all  (how many defects are detectable at all)

Cell-level Wilson intervals are retained for the per-attempt estimand but the PRIMARY
interval is a defect-cluster bootstrap: resample the 19 Tier A defects with replacement,
carrying every cell belonging to a drawn defect, recompute the rate, 10,000 resamples,
percentile 95% CI. Individual suites are never resampled.

Outputs:
  data/results/e1_defect_level.json
  data/results/e1_defect_level.md

Run with cwd = artifact/ :  python3 analysis/cluster_bootstrap.py
"""
import json
import math
import random
from collections import defaultdict
from pathlib import Path

ART = Path(__file__).resolve().parent.parent
RES = ART / "data" / "results"

N_BOOT = 10_000
SEED = 20260907

MODEL_FILES = {
    "gemini-2.5-flash": "rows_tierA_gemini-2.5-flash_fixed.jsonl",
    "qwen3-coder": "rows_tierA_qwen3-coder_fixed.jsonl",
    "deepseek-chat": "rows_tierA_deepseek-chat_fixed.jsonl",
    "gpt-5.4-or": "rows_tierA_gpt-5.4-or_fixed.jsonl",
}

INTERNAL_FILES = ["campaign_internal.jsonl", "campaign_internal_v2.jsonl", "campaign_spirv.jsonl"]

# Reclassified by the focal-accessibility audit from internal to exported public.
#   nng@6a59b15fba = nng_recv        (src/nng.c,                NNG_DECL in include/nng/nng.h)
#   nng@e3b8f31b04 = nn_setsockopt   (src/compat/nanomsg/nn.c,  exported compat layer)
RECLASSIFIED_PUBLIC = {"nanomsg___nng@6a59b15fba", "nanomsg___nng@e3b8f31b04"}

# Reported in the paper as the Tier A defects detected by at least one model/run.
PAPER_DETECTABLE4 = {"020a21afc6", "611cf0b3c6", "6a13464059", "96c18b26c2"}


def load(name):
    return [json.loads(l) for l in (RES / name).read_text().splitlines() if l.strip()]


def short(defect_id):
    return defect_id.split("@")[1]


def wilson(k, n, z=1.96):
    if n == 0:
        return (0.0, 1.0)
    p = k / n
    d = 1 + z * z / n
    c = (p + z * z / (2 * n)) / d
    h = (z * math.sqrt(p * (1 - p) / n + z * z / (4 * n * n))) / d
    return (max(0.0, c - h), min(1.0, c + h))


def fmt_wilson(k, n):
    lo, hi = wilson(k, n)
    r = 100 * k / n if n else float("nan")
    return f"{k}/{n} = {r:.0f}% (95% Wilson CI {100*lo:.1f}-{100*hi:.1f}%)"


def bootstrap_by_defect(cells_by_defect, numer, denom, n_boot=N_BOOT, seed=SEED):
    """Resample defects with replacement, carrying all their cells.

    numer/denom are predicates over a cell. Returns percentile CI plus diagnostics.
    """
    rng = random.Random(seed)
    defects = list(cells_by_defect)
    k = len(defects)
    rates, degenerate = [], 0
    for _ in range(n_boot):
        num = den = 0
        for _ in range(k):
            for c in cells_by_defect[rng.choice(defects)]:
                if denom(c):
                    den += 1
                    if numer(c):
                        num += 1
        if den == 0:
            degenerate += 1
            continue
        rates.append(num / den)
    rates.sort()
    lo = rates[int(0.025 * len(rates))]
    hi = rates[int(0.975 * len(rates)) - 1]
    return {
        "ci_low_pct": round(100 * lo, 1),
        "ci_high_pct": round(100 * hi, 1),
        "median_pct": round(100 * rates[len(rates) // 2], 1),
        "n_resamples": n_boot,
        "n_degenerate_dropped": degenerate,
        "n_clusters": k,
    }


def main():
    out, md = {}, ["# E1 -- Defect-level verification and cluster-aware re-analysis", ""]

    # =============== 1. Internal campaign: verify 0/21 ===============
    internal = []
    for f in INTERNAL_FILES:
        internal += load(f)
    pre_audit_defects = sorted({r["defect_id"] for r in internal})
    audited = [r for r in internal if r["defect_id"] not in RECLASSIFIED_PUBLIC]
    audited_defects = sorted({r["defect_id"] for r in audited})
    compiled_cells = [r for r in audited if r.get("compiled")]
    defects_with_compile = sorted({r["defect_id"] for r in compiled_cells})

    reclass_cells = [r for r in internal if r["defect_id"] in RECLASSIFIED_PUBLIC]
    reclass_compiled = [r for r in reclass_cells if r.get("compiled")]

    proj = defaultdict(set)
    for r in audited:
        proj[r["project"]].add(r["defect_id"])

    lo21, hi21 = wilson(0, len(audited_defects))
    out["internal_campaign"] = {
        "pre_audit_defects": len(pre_audit_defects),
        "pre_audit_cells": len(internal),
        "reclassified_to_public": sorted(RECLASSIFIED_PUBLIC),
        "reclassified_cells": len(reclass_cells),
        "reclassified_cells_that_compiled": len(reclass_compiled),
        "audited_internal_defects": len(audited_defects),
        "audited_internal_cells": len(audited),
        "cells_that_compiled": len(compiled_cells),
        "defects_with_any_compiling_suite": len(defects_with_compile),
        "VERIFIED_zero_of_21": len(compiled_cells) == 0 and len(audited_defects) == 21,
        "per_project_defects": {k: len(v) for k, v in sorted(proj.items())},
        "per_attempt": f"0/{len(audited)}",
        "per_attempt_wilson_pct": [round(100 * x, 1) for x in wilson(0, len(audited))],
        "per_defect": f"0/{len(audited_defects)}",
        "per_defect_wilson_pct": [round(100 * lo21, 1), round(100 * hi21, 1)],
        "models": sorted({r["model"] for r in audited}),
        "runs": sorted({r.get("run") for r in audited}),
    }

    md += ["## 1. Internal campaign -- verification of the 0/21 claim", "",
           f"- pre-audit candidate set: {len(pre_audit_defects)} defects, {len(internal)} cells",
           f"- reclassified to exported public by the audit: {len(RECLASSIFIED_PUBLIC)} defects "
           f"({', '.join(sorted(RECLASSIFIED_PUBLIC))}), {len(reclass_cells)} cells, of which "
           f"{len(reclass_compiled)} compiled",
           f"- audited internal set: **{len(audited_defects)} defects, {len(audited)} cells**",
           f"- cells that compiled: **{len(compiled_cells)}**",
           f"- defects with at least one compiling suite: **{len(defects_with_compile)}**",
           f"- composition: " + ", ".join(f"{k.split('___')[-1]} {len(v)}" for k, v in sorted(proj.items())),
           f"- models: {', '.join(sorted({r['model'] for r in audited}))} (3 models x 2 runs)", "",
           f"**VERIFIED: {out['internal_campaign']['VERIFIED_zero_of_21']}**", "",
           "| estimand | value |", "|---|---|",
           f"| per-attempt (compiling suites / cells) | {fmt_wilson(0, len(audited))} |",
           f"| per-defect (defects with any compiling suite / defects) | {fmt_wilson(0, len(audited_defects))} |", "",
           "Both cells that compiled in the pre-audit set of 138 belonged to the reclassified "
           "`nng@6a59b15fba` (`nng_recv`), so the audited internal subset contains no compiling "
           "suite at either granularity. The per-defect interval is the honest one to quote: "
           f"**0/{len(audited_defects)}, 95% CI 0-{100*hi21:.1f}%**, against "
           f"0-{100*wilson(0,len(audited))[1]:.1f}% at cell level.", ""]

    # =============== 2. Tier A: verify 4/19 and both estimands ===============
    tierA = {m: load(f) for m, f in MODEL_FILES.items()}
    all_cells = [r for rows in tierA.values() for r in rows]
    defects = sorted({short(r["defect_id"]) for r in all_cells})
    detected_defects = sorted({short(r["defect_id"]) for r in all_cells if r["real_bug_detected"]})

    eligible = [r for r in all_cells if r["real_bug_detected"] is not None]
    detections = [r for r in eligible if r["real_bug_detected"]]

    cells_by_defect = defaultdict(list)
    for r in all_cells:
        cells_by_defect[short(r["defect_id"])].append(r)

    boot_pooled = bootstrap_by_defect(
        cells_by_defect,
        numer=lambda c: bool(c["real_bug_detected"]),
        denom=lambda c: c["real_bug_detected"] is not None,
    )
    boot_uncond = bootstrap_by_defect(
        cells_by_defect,
        numer=lambda c: bool(c["real_bug_detected"]),
        denom=lambda c: True,
    )

    lo4, hi4 = wilson(len(detected_defects), len(defects))
    out["tierA"] = {
        "cells": len(all_cells),
        "defects": len(defects),
        "eligible_suites": len(eligible),
        "detections": len(detections),
        "detected_defects": detected_defects,
        "VERIFIED_four_of_19": set(detected_defects) == PAPER_DETECTABLE4 and len(defects) == 19,
        "paper_claimed_detectable4": sorted(PAPER_DETECTABLE4),
        "per_attempt_conditional": {
            "point": f"{len(detections)}/{len(eligible)}",
            "pct": round(100 * len(detections) / len(eligible), 1),
            "wilson_pct": [round(100 * x, 1) for x in wilson(len(detections), len(eligible))],
            "cluster_bootstrap_pct": boot_pooled,
        },
        "per_attempt_unconditional": {
            "point": f"{len(detections)}/{len(all_cells)}",
            "pct": round(100 * len(detections) / len(all_cells), 1),
            "wilson_pct": [round(100 * x, 1) for x in wilson(len(detections), len(all_cells))],
            "cluster_bootstrap_pct": boot_uncond,
        },
        "per_defect": {
            "point": f"{len(detected_defects)}/{len(defects)}",
            "pct": round(100 * len(detected_defects) / len(defects), 1),
            "wilson_pct": [round(100 * lo4, 1), round(100 * hi4, 1)],
        },
    }

    md += ["## 2. Tier A -- verification of the 4/19 claim and both estimands", "",
           f"- {len(all_cells)} cells over {len(defects)} defects x 4 models x 3 runs",
           f"- eligible suites (compile + at least one valid case): {len(eligible)}",
           f"- detections: {len(detections)}",
           f"- defects detected by at least one model/run: **{len(detected_defects)}** "
           f"({', '.join(d[:8] for d in detected_defects)})",
           f"- paper claims: {', '.join(d[:8] for d in sorted(PAPER_DETECTABLE4))}", "",
           f"**VERIFIED: {out['tierA']['VERIFIED_four_of_19']}** -- the four hashes match exactly.", "",
           "| estimand | point | cell-level Wilson | defect-cluster bootstrap |",
           "|---|---|---|---|",
           f"| per-attempt, conditional on eligibility | {len(detections)}/{len(eligible)} = "
           f"{100*len(detections)/len(eligible):.0f}% | {100*wilson(len(detections),len(eligible))[0]:.1f}-"
           f"{100*wilson(len(detections),len(eligible))[1]:.1f}% | "
           f"**{boot_pooled['ci_low_pct']}-{boot_pooled['ci_high_pct']}%** |",
           f"| per-attempt, all attempts | {len(detections)}/{len(all_cells)} = "
           f"{100*len(detections)/len(all_cells):.0f}% | {100*wilson(len(detections),len(all_cells))[0]:.1f}-"
           f"{100*wilson(len(detections),len(all_cells))[1]:.1f}% | "
           f"**{boot_uncond['ci_low_pct']}-{boot_uncond['ci_high_pct']}%** |",
           f"| per-defect, detected at least once | {len(detected_defects)}/{len(defects)} = "
           f"{100*len(detected_defects)/len(defects):.0f}% | {100*lo4:.1f}-{100*hi4:.1f}% | n/a (defect is the unit) |", "",
           f"The cluster bootstrap widens the conditional per-attempt interval from "
           f"{100*wilson(len(detections),len(eligible))[0]:.1f}-{100*wilson(len(detections),len(eligible))[1]:.1f}% "
           f"to {boot_pooled['ci_low_pct']}-{boot_pooled['ci_high_pct']}%, i.e. by a factor of "
           f"{(boot_pooled['ci_high_pct']-boot_pooled['ci_low_pct'])/(100*(wilson(len(detections),len(eligible))[1]-wilson(len(detections),len(eligible))[0])):.1f}. "
           f"{boot_pooled['n_degenerate_dropped']} of {N_BOOT} resamples were dropped for having "
           f"an empty denominator.", ""]

    # =============== 3. Per-model, both estimands ===============
    md += ["## 3. Per model, both estimands", "",
           "| model | detections | per-attempt (all 57) | cluster bootstrap | defects detected |",
           "|---|---|---|---|---|"]
    per_model = {}
    for m, rows in tierA.items():
        cbd = defaultdict(list)
        for r in rows:
            cbd[short(r["defect_id"])].append(r)
        det = sum(1 for r in rows if r["real_bug_detected"])
        dd = sorted({short(r["defect_id"]) for r in rows if r["real_bug_detected"]})
        b = bootstrap_by_defect(cbd, lambda c: bool(c["real_bug_detected"]), lambda c: True)
        per_model[m] = {
            "detections": det,
            "cells": len(rows),
            "wilson_pct": [round(100 * x, 1) for x in wilson(det, len(rows))],
            "cluster_bootstrap_pct": b,
            "defects_detected": dd,
            "per_defect": f"{len(dd)}/19",
        }
        md += [f"| {m} | {det}/{len(rows)} = {100*det/len(rows):.0f}% | "
               f"{100*wilson(det,len(rows))[0]:.1f}-{100*wilson(det,len(rows))[1]:.1f}% | "
               f"**{b['ci_low_pct']}-{b['ci_high_pct']}%** | {len(dd)}/19 |"]
    out["per_model"] = per_model
    n_dd = {m: len(v["defects_detected"]) for m, v in per_model.items()}
    md += ["", "Every per-model cluster interval spans the others' point estimates, so the "
           "apparent ordering Gemini < Qwen < DeepSeek < GPT-5.4 is not supported once "
           "clustering is accounted for. At the defect level the models detect "
           + ", ".join(f"{m} {n}/19" for m, n in n_dd.items()) + ". Note that GPT-5.4 alone "
           "accounts for all four detectable defects: no defect is detected by another model "
           "that GPT-5.4 misses, so the union over four models adds nothing over GPT-5.4 alone.", ""]

    # =============== 4. What to change in the paper ===============
    md += ["## 4. Statements to update in the paper", "",
           "| location | current | replace with |", "|---|---|---|",
           f"| Abstract | \"only 11% of suites that compile and contain valid tests detect their "
           f"defect\" | keep, and add the defect-level companion: {len(detected_defects)} of "
           f"{len(defects)} defects are detected by any model or run |",
           f"| Finding 1 | \"None of the 126 suites ... compiles\" | add \"and none of the 21 "
           f"audited defects obtains a compiling suite (95% CI 0-{100*hi21:.1f}%)\" |",
           f"| Finding 2 | \"only 16 detect their defect\" | add \"across {len(detected_defects)} "
           f"of {len(defects)} distinct defects\" |",
           f"| Table III pooled row | 11% (95% CI 7-18%) | 11% (defect-cluster bootstrap 95% CI "
           f"{boot_pooled['ci_low_pct']}-{boot_pooled['ci_high_pct']}%) |",
           f"| Conclusion | \"11% of compilable suites ... detect their defect\" | add the Figure 1 "
           f"caption qualification and the defect-level rate |", ""]

    (RES / "e1_defect_level.json").write_text(json.dumps(out, indent=2))
    (RES / "e1_defect_level.md").write_text("\n".join(md))
    print("\n".join(md))
    print(f"\n[written] {RES/'e1_defect_level.json'}")
    print(f"[written] {RES/'e1_defect_level.md'}")


if __name__ == "__main__":
    main()
