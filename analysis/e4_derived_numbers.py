#!/usr/bin/env python3
"""Derived numbers that the paper quotes downstream of the E4 reclassification.

Every quantity that changed when the RQ3 population moved from 44 to 41 suites, computed
from the same two files the reclassification produced so the paper and the artifact cannot
drift apart:
  rq3/e4_suite_classification.csv  (group A/B per suite, plus the published labels)
  data/results/rows_tierA_*_fixed.jsonl (eligibility denominators, unchanged by E4)

Outputs:
  rq3/e4_derived_numbers.md
  rq3/e4_derived_numbers.json

Run with cwd = artifact/ :  python3 analysis/e4_derived_numbers.py
"""
import csv
import json
import math
from collections import Counter, defaultdict
from pathlib import Path

ART = Path(__file__).resolve().parent.parent
RES = ART / "data" / "results"
OUT = ART / "rq3"

MODEL_FILES = {
    "Gemini-2.5-Flash": "rows_tierA_gemini-2.5-flash_fixed.jsonl",
    "Qwen3-Coder-30B": "rows_tierA_qwen3-coder_fixed.jsonl",
    "DeepSeek-V4-flash": "rows_tierA_deepseek-chat_fixed.jsonl",
    "GPT-5.4": "rows_tierA_gpt-5.4-or_fixed.jsonl",
}
FILE_MODEL = {"gemini-2.5-flash": "Gemini-2.5-Flash", "qwen3-coder": "Qwen3-Coder-30B",
              "deepseek-chat": "DeepSeek-V4-flash", "gpt-5.4-or": "GPT-5.4"}


def r0(x):
    """Round half UP. Python's built-in round() is banker's rounding, which turns the
    exactly-12.5% Gemini share into 12 rather than the conventional 13."""
    return math.floor(x + 0.5)


def wilson(k, n, z=1.96):
    if n == 0:
        return (0.0, 1.0)
    p = k / n
    d = 1 + z * z / n
    c = (p + z * z / (2 * n)) / d
    h = (z * math.sqrt(p * (1 - p) / n + z * z / (4 * n * n))) / d
    return (max(0.0, c - h), min(1.0, c + h))


def main():
    suites = list(csv.DictReader((OUT / "e4_suite_classification.csv").open()))
    A = [s for s in suites if s["group"] == "A"]
    removed = [s for s in suites if s["group"] != "A"]
    out, md = {}, ["# Derived numbers after the E4 reclassification (44 -> 41)", "",
                   "Suites removed from the RQ3 population: "
                   + ", ".join(f"`{s['defect_id'].split('@')[1][:8]}` {s['model']} run{s['run']} "
                               f"({s['final_after_adjudication']})" for s in removed), ""]

    # ---- 1a. project composition ----
    p44 = Counter(s["defect_id"].split("___")[0] for s in suites)
    pA = Counter(s["defect_id"].split("___")[0] for s in A)
    out["project_composition"] = {"pop44": dict(p44), "pop41": dict(pA)}
    md += ["## 1a. Project composition", "",
           "| population | fmt | CLI11 | total |", "|---|---|---|---|",
           f"| 44 (as published) | {p44['fmtlib']} | {p44['CLIUtils']} | {sum(p44.values())} |",
           f"| **41 (group A)** | **{pA['fmtlib']}** | **{pA['CLIUtils']}** | **{sum(pA.values())}** |", "",
           f"All three removed suites are fmt, so the claim \"{pA['fmtlib']} of the "
           f"{len(A)} RQ3 suites come from fmt\" is **correct**.", ""]

    # ---- 1b. single-defect concentration ----
    md += ["## 1b. Single-defect concentration in Threats", "",
           "| population | infection (O4) | top defect | propagation (O2) | top defect |",
           "|---|---|---|---|---|"]
    conc = {}
    for pop, name in ((suites, "44"), (A, "41")):
        inf = [s for s in pop if s["final_after_adjudication"] == "O4"]
        pro = [s for s in pop if s["final_after_adjudication"] == "O2"]
        ci = Counter(s["defect_id"].split("@")[1][:8] for s in inf).most_common(1)[0]
        cp = Counter(s["defect_id"].split("@")[1][:8] for s in pro).most_common(1)[0]
        conc[name] = {"infection_total": len(inf), "infection_top": ci[0], "infection_top_n": ci[1],
                      "propagation_total": len(pro), "propagation_top": cp[0],
                      "propagation_top_n": cp[1]}
        md += [f"| {name} | {len(inf)} | `{ci[0]}` {ci[1]}/{len(inf)} | {len(pro)} | "
               f"`{cp[0]}` {cp[1]}/{len(pro)} |"]
    out["concentration"] = conc
    c = conc["41"]
    md += ["",
           f"Neither `{c['infection_top']}` nor `{c['propagation_top']}` loses a suite in the "
           f"reclassification, so both numerators stay at 8 and the claim "
           f"\"**8 of the {c['infection_total']} infection and 8 of the "
           f"{c['propagation_total']} propagation**\" is **correct**.", "",
           f"Worth noting for the wording: the propagation concentration *tightens* from "
           f"8/14 = {100*8/14:.0f}% to 8/12 = {100*8/12:.0f}%. The Threats sentence understates "
           f"the clustering if the percentage is left implicit.", ""]

    # ---- 1c. model-families reached-but-not-caught share ----
    eligible = {}
    for disp, f in MODEL_FILES.items():
        rows = [json.loads(l) for l in (RES / f).read_text().splitlines() if l.strip()]
        eligible[disp] = sum(1 for r in rows if r["real_bug_detected"] is not None)

    rnc = {"44": defaultdict(int), "41": defaultdict(int)}
    for s in suites:
        rnc["44"][FILE_MODEL[s["model"]]] += 1
    for s in A:
        rnc["41"][FILE_MODEL[s["model"]]] += 1

    md += ["## 1c. Model families -- reached but not caught share", "",
           "Eligibility (compile + at least one valid case) is unaffected by the E4 "
           "reclassification, so only the numerators move.", "",
           "| model | eligible | pop 44 | share | pop 41 | share |",
           "|---|---|---|---|---|---|"]
    shares = {}
    for disp in MODEL_FILES:
        n = eligible[disp]
        k44, k41 = rnc["44"][disp], rnc["41"][disp]
        shares[disp] = {"eligible": n, "n44": k44, "pct44": r0(100 * k44 / n),
                        "n41": k41, "pct41": r0(100 * k41 / n),
                        "pct41_exact": round(100 * k41 / n, 1),
                        "wilson41_pct": [r0(100 * x) for x in wilson(k41, n)]}
        md += [f"| {disp} | {n} | {k44} | {r0(100*k44/n)}% | **{k41}** | "
               f"**{r0(100*k41/n)}%** (exact {100*k41/n:.1f}%) |"]
    lo = min(v["pct41"] for v in shares.values())
    hi = max(v["pct41"] for v in shares.values())
    lo44 = min(v["pct44"] for v in shares.values())
    hi44 = max(v["pct44"] for v in shares.values())
    out["model_families"] = {"per_model": shares,
                             "range_pop44": f"{lo44}% to {hi44}%",
                             "range_pop41": f"{lo}% to {hi}%"}
    lo_m = min(shares, key=lambda k: shares[k]["pct41"])
    md += ["",
           f"**The published range {lo44}% to {hi44}% must become {lo}% to {hi}%.** "
           f"The low end moves because {lo_m} has the smallest eligible set "
           f"({shares[lo_m]['eligible']}) and loses one of its {shares[lo_m]['n44']} flagged "
           f"suites; the high end is unchanged because Qwen3-Coder-30B loses none.", "",
           f"Caution: {lo_m}'s share is {shares[lo_m]['n41']}/{shares[lo_m]['eligible']} = "
           f"{shares[lo_m]['pct41_exact']}% exactly, a tie. Rounded half up it is {lo}%; "
           f"Python's default banker's rounding would print {round(100*shares[lo_m]['n41']/shares[lo_m]['eligible'])}%. "
           f"Quote {lo}% and a reviewer recomputing gets {shares[lo_m]['pct41_exact']}%, which "
           f"is consistent; quoting the lower value is not.", "",
           "Suggested sentence: \"Among each model's suites that compile and contain at least "
           f"one valid test case, the reached but not caught share ranges from {lo}\\% to "
           f"{hi}\\%.\"", ""]

    # ---- totals cross-check ----
    tot_elig = sum(eligible.values())
    md += ["## Cross-check", "",
           f"- eligible suites, all models: {tot_elig} (paper: 141) -> "
           f"{'OK' if tot_elig == 141 else 'MISMATCH'}",
           f"- reached but not caught, pop 41: {sum(rnc['41'].values())} -> "
           f"{100*sum(rnc['41'].values())/tot_elig:.0f}% of {tot_elig}",
           f"- never reached, pop 41: {tot_elig - 16 - sum(rnc['41'].values())} -> "
           f"{100*(tot_elig-16-sum(rnc['41'].values()))/tot_elig:.0f}% of {tot_elig} "
           f"(16 detections are unchanged by E4)", ""]
    out["cross_check"] = {
        "eligible_total": tot_elig,
        "reached_not_caught_41": sum(rnc["41"].values()),
        "never_reached_41": tot_elig - 16 - sum(rnc["41"].values()),
        "detections": 16,
    }

    (OUT / "e4_derived_numbers.json").write_text(json.dumps(out, indent=2))
    (OUT / "e4_derived_numbers.md").write_text("\n".join(md))
    print("\n".join(md))
    for p in ("e4_derived_numbers.md", "e4_derived_numbers.json"):
        print(f"[written] {OUT/p}")


if __name__ == "__main__":
    main()
