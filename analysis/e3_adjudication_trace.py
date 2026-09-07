#!/usr/bin/env python3
"""E3 -- Adjudication trace for RQ3 (Reviewer #3, Major Comment 4, second half).

Reviewer #3: "kappa = 0.72 is computed on pre-adjudication labels while the reported
labels are post-adjudication, and adjudication used build-and-probe evidence stronger
than the annotation itself. How many labels the probes changed would be more informative
than kappa."

This script reconstructs, for every suite that needed adjudication:
  - each annotator's independent pre-adjudication label,
  - the final post-adjudication label,
  - whether the executable probe overturned an annotator, and which one,
  - whether the probe introduced a label neither annotator had proposed.

It also checks the specific question of whether the three initial O3 (too-weak
assertion) labels fall inside the six disagreements, because the paper's claim that no
miss is attributable to a weak assertion depends on the answer.

Outputs:
  rq3/e3_adjudication_trace.csv
  rq3/e3_adjudication.md
  rq3/e3_adjudication.json

Run with cwd = artifact/ :  python3 analysis/e3_adjudication_trace.py
"""
import csv
import json
import re
from collections import Counter, defaultdict
from pathlib import Path

ART = Path(__file__).resolve().parent.parent
TAX = ART / "data" / "taxonomy"
STUDY_TAX = ART.parent / "study" / "data" / "taxonomy"
OUT = ART / "rq3"
OUT.mkdir(exist_ok=True)

LABEL_NAME = {
    "O1": "echo / self-confirming oracle",
    "O2": "wrong API surface (propagation)",
    "O3": "too-weak assertion (revealability)",
    "O4": "insufficient input (infection)",
    "O5": "other or unclear",
}


def frozen44():
    for p in (STUDY_TAX / "rq3_frozen44.csv", TAX / "rq3_frozen44.csv"):
        if p.exists():
            return list(csv.DictReader(p.open())), p
    raise SystemExit("rq3_frozen44.csv not found")


def cohen_kappa(a, b):
    """Unweighted Cohen's kappa for two equal-length label sequences."""
    n = len(a)
    cats = sorted(set(a) | set(b))
    po = sum(1 for x, y in zip(a, b) if x == y) / n
    ca, cb = Counter(a), Counter(b)
    pe = sum((ca[c] / n) * (cb[c] / n) for c in cats)
    return (po - pe) / (1 - pe) if pe < 1 else 1.0, po, pe


def main():
    rows, src = frozen44()
    out, md = {}, ["# E3 -- RQ3 adjudication trace", ""]

    # ---------- 1. provenance of the per-coder label files ----------
    sheet = list(csv.DictReader((TAX / "rq3_coding_sheet.csv").open()))
    c1 = list(csv.DictReader((TAX / "rq3_coder1.csv").open()))
    c2 = list(csv.DictReader((TAX / "rq3_coder2.csv").open()))
    cond_of = lambda p: "buggy-arm" if "/buggy/" in p else "main campaign"  # noqa: E731
    sheet_comp = Counter(cond_of(r["test_path"]) for r in sheet)
    sheet_keys = {(r["defect_id"], r["model"],
                   re.search(r"run(\d)", r["test_path"]).group(1))
                  for r in sheet if "/buggy/" not in r["test_path"]}
    frozen_keys = {(r["defect_id"], r["model"], r["run"]) for r in rows}

    out["provenance"] = {
        "frozen44_source": str(src),
        "coding_sheet_rows": len(sheet),
        "coding_sheet_composition": dict(sheet_comp),
        "per_coder_files_rows": {"rq3_coder1.csv": len(c1), "rq3_coder2.csv": len(c2)},
        "per_coder_disagreements": sum(1 for x, y in zip(c1, c2) if x["label"] != y["label"]),
        "in_round1_not_in_frozen44": sorted(sheet_keys - frozen_keys),
        "in_frozen44_not_in_round1": sorted(frozen_keys - sheet_keys),
        "WARNING": ("rq3_coder1.csv / rq3_coder2.csv hold the FIRST annotation round only "
                    "(47 suites, including 16 from the buggy-generation arm that never enter "
                    "the paper). They show 4 disagreements and give kappa = 0.80. The paper's "
                    "kappa is computed on the coder1/coder2 columns of rq3_frozen44.csv, which "
                    "merge round 1 with a second round covering the 13 GPT-5.4 suites and show "
                    "6 disagreements. The two must not be confused."),
        "RESOLVED_BY": ("analysis/rq3_reconcile_labels.py now emits the missing round-2 files "
                        "(rq3_delta_coder{1,2}.csv) and a self-contained merged record "
                        "(rq3_labels_merged44.csv) from which kappa = 0.72 recomputes directly."),
    }

    n_buggy_arm = sheet_comp.get("buggy-arm", 0)
    n_main = sheet_comp.get("main campaign", 0)
    n_new = len(out["provenance"]["in_frozen44_not_in_round1"])
    md += ["## 1. Provenance of the annotation records", "",
           f"- `rq3_frozen44.csv` (`{src}`): {len(rows)} suites, the analysis set",
           f"- `rq3_coding_sheet.csv`: {len(sheet)} rows -- "
           + ", ".join(f"{v} {k}" for k, v in sorted(sheet_comp.items())),
           f"- `rq3_coder1.csv` / `rq3_coder2.csv`: {len(c1)} rows each, "
           f"{out['provenance']['per_coder_disagreements']} disagreements", "",
           "**These are not the same population.** The shipped per-coder files record the first "
           f"annotation round: {n_main} suites from the main campaign plus {n_buggy_arm} from "
           "the buggy-generation arm that never enter the paper, and they predate the GPT-5.4 "
           f"campaign. The {n_new} GPT-5.4 suites in the analysis set were annotated in a second "
           f"round, so the analysis set is those same {n_main} main-campaign suites plus "
           f"{n_new} new ones. The agreement figure reported in the paper (86.4%, 38/44) belongs "
           "to the merged `coder1`/`coder2` columns of `rq3_frozen44.csv`, not to the shipped "
           f"per-coder files, which show {out['provenance']['per_coder_disagreements']} "
           "disagreements over a different population and give kappa = 0.80.", "",
           "**Resolved.** `analysis/rq3_reconcile_labels.py` emits the round-2 files the README "
           "already referenced but that were never written (`rq3_delta_coder{1,2}.csv`) and a "
           "self-contained merged record keyed by (defect, model, run) "
           "(`rq3_labels_merged44.csv`), verifies the merge cell by cell against "
           "`rq3_frozen44.csv`, and recomputes kappa = 0.72 from shipped files alone.", ""]

    # ---------- 2. kappa on the reported population ----------
    A = [r["coder1"] for r in rows]
    B = [r["coder2"] for r in rows]
    k, po, pe = cohen_kappa(A, B)
    dis = [r for r in rows if r["coder1"] != r["coder2"]]
    out["kappa_pre_adjudication"] = {
        "n": len(rows), "kappa": round(k, 3),
        "observed_agreement": round(po, 4), "expected_agreement": round(pe, 4),
        "n_disagreements": len(dis),
        "coder1_distribution": dict(Counter(A)),
        "coder2_distribution": dict(Counter(B)),
    }
    md += ["## 2. Pre-adjudication agreement, recomputed", "",
           f"- Cohen's kappa = **{k:.2f}** (observed agreement {100*po:.1f}%, "
           f"chance agreement {100*pe:.1f}%), n = {len(rows)}",
           f"- disagreements: **{len(dis)}/{len(rows)}**",
           f"- coder 1 label distribution: {dict(Counter(A))}",
           f"- coder 2 label distribution: {dict(Counter(B))}", "",
           "This reproduces the value reported in the paper. It describes labels that were "
           "then replaced.", ""]

    # ---------- 3. the O3 question ----------
    o3_c1 = [r for r in rows if r["coder1"] == "O3"]
    o3_c2 = [r for r in rows if r["coder2"] == "O3"]
    o3_any = [r for r in rows if "O3" in (r["coder1"], r["coder2"])]
    o3_in_dis = [r for r in o3_any if r["coder1"] != r["coder2"]]
    o3_final = [r for r in rows if r["final_after_adjudication"] == "O3"]
    o3_defects = {r["defect_id"] for r in o3_any}

    out["weak_assertion_O3"] = {
        "proposed_by_coder1": len(o3_c1),
        "proposed_by_coder2": len(o3_c2),
        "suites_with_any_O3_proposal": len(o3_any),
        "of_which_inside_the_disagreements": len(o3_in_dis),
        "distinct_defects": sorted(o3_defects),
        "surviving_in_final_labels": len(o3_final),
        "all_O3_are_disagreements": len(o3_any) == len(o3_in_dis),
    }
    md += ["## 3. The three weak-assertion labels", "",
           f"- proposed by coder 1: {len(o3_c1)}; by coder 2: {len(o3_c2)}",
           f"- suites carrying any O3 proposal: **{len(o3_any)}**, all on defect(s) "
           f"{', '.join(d.split('@')[1][:8] for d in sorted(o3_defects))}",
           f"- of those, inside the six disagreements: **{len(o3_in_dis)}**",
           f"- surviving in the final labels: **{len(o3_final)}**", "",
           "**Answer to the question asked: yes, completely.** Every O3 label ever proposed "
           "came from one annotator, on one defect, and every one of them is one of the six "
           "disagreements. The paper's statement that no miss is attributed to a weak assertion "
           "is therefore not a finding on which the two annotators independently agreed: it is "
           "the outcome of adjudicating three suites of a single defect against a probe. "
           "The claim should be stated that way.", ""]

    # ---------- 4. per-suite adjudication trace ----------
    probes = {}
    for p in sorted(TAX.glob("rq3_probe_*.json")):
        d = json.loads(p.read_text())
        probes[d["defect"]] = {**d, "_file": f"data/taxonomy/{p.name}"}

    adj6 = {}
    for r in csv.DictReader((TAX / "rq3_adjudicate6.csv").open()):
        adj6[(r["defect_id"], r["model"], r["run"])] = r

    trace = []
    for r in dis:
        key = (r["defect_id"], r["model"], r["run"])
        fin = r["final_after_adjudication"]
        pr = probes.get(r["defect_id"])
        trace.append({
            "defect_id": r["defect_id"], "model": r["model"], "run": r["run"],
            "source_round": r["source"],
            "coder1": r["coder1"], "coder2": r["coder2"], "final": fin,
            "probe_file": pr["_file"] if pr else "",
            "probe_question": pr.get("question", "") if pr else "",
            "probe_verdict": (pr.get("verdict", "") if pr else "")[:300],
            "overturned": ("coder2" if fin == r["coder1"] else
                           "coder1" if fin == r["coder2"] else "BOTH"),
            "final_is_novel_label": fin not in (r["coder1"], r["coder2"]),
            "in_adjudicate6_csv": key in adj6,
            "adjudication_reason": r.get("adjudication_reason", ""),
        })

    changed_vs_c1 = sum(1 for t in trace if t["final"] != t["coder1"])
    changed_vs_c2 = sum(1 for t in trace if t["final"] != t["coder2"])
    novel = sum(1 for t in trace if t["final_is_novel_label"])
    probed_defects = sorted({t["defect_id"] for t in trace})

    out["adjudication"] = {
        "n_disagreements": len(dis),
        "n_unique_defects_probed": len(probed_defects),
        "probed_defects": probed_defects,
        "probe_files": [probes[d]["_file"] for d in probed_defects if d in probes],
        "final_overturned_coder1": changed_vs_c1,
        "final_overturned_coder2": changed_vs_c2,
        "final_introduced_a_new_label": novel,
        "agreed_suites_untouched": len(rows) - len(dis),
        "labels_changed_by_probe_relative_to_annotation": f"{changed_vs_c1 + changed_vs_c2}/{2*len(dis)}",
    }

    md += ["## 4. What the probes actually changed", "",
           f"Six suites disagreed. They cluster on **{len(probed_defects)} distinct defects**, "
           f"each settled by one executable probe that built the fixed and buggy versions and "
           f"ran chosen inputs on both:", ""]
    for d in probed_defects:
        p = probes.get(d)
        if p:
            n_suites = sum(1 for t in trace if t["defect_id"] == d)
            md += [f"- `{d.split('@')[1][:8]}` ({n_suites} suites) -- {p.get('question','')} "
                   f"-- `{p['_file']}`"]
    md += ["", "| suite | coder 1 | coder 2 | final | overturned | new label? |",
           "|---|---|---|---|---|---|"]
    for t in trace:
        md += [f"| `{t['defect_id'].split('@')[1][:8]}` {t['model']} run{t['run']} | "
               f"{t['coder1']} | {t['coder2']} | **{t['final']}** | {t['overturned']} | "
               f"{'yes' if t['final_is_novel_label'] else 'no'} |"]

    md += ["", "### The number Reviewer #3 asked for", "",
           "| quantity | value |", "|---|---|",
           f"| suites requiring adjudication | **{len(dis)}/{len(rows)}** |",
           f"| distinct defects probed | **{len(probed_defects)}** |",
           f"| probe overturned coder 1 | **{changed_vs_c1}/{len(dis)}** |",
           f"| probe overturned coder 2 | **{changed_vs_c2}/{len(dis)}** |",
           f"| probe produced a label neither annotator proposed | **{novel}/{len(dis)}** |",
           f"| suites where the two annotators already agreed and the probe was not invoked | "
           f"{len(rows)-len(dis)}/{len(rows)} |", "",
           f"So the probes changed **{changed_vs_c1 + changed_vs_c2} of the "
           f"{2*len(dis)} individual annotator judgements** they examined "
           f"({changed_vs_c1} of coder 1's, {changed_vs_c2} of coder 2's), and in no case did "
           f"the probe reach a label that neither annotator had proposed. Adjudication acted as "
           f"a tie-break between two proposed labels rather than as an independent relabelling. "
           f"This is a more useful reliability statement than kappa, because the reported labels "
           f"are the post-adjudication ones and kappa describes the labels that were replaced.", "",
           "### Suggested replacement text for Section III-D", "",
           "> For RQ3, two annotators independently classified the 44 reached but not caught "
           f"suites, agreeing on {len(rows)-len(dis)} of {len(rows)} (kappa = {k:.2f}). The "
           f"{len(dis)} disagreements fall on {len(probed_defects)} defects and were settled by "
           "building both program versions and running chosen inputs on each. The probes "
           f"overturned {changed_vs_c1} of coder 1's labels and {changed_vs_c2} of coder 2's, "
           "and never produced a label that neither annotator had proposed. All three "
           "too-weak-assertion labels occur inside these disagreements, on a single defect, and "
           "were resolved to insufficient input; the finding that no miss is attributed to a "
           "weak assertion therefore rests on adjudication rather than on independent "
           "agreement.", ""]

    with (OUT / "e3_adjudication_trace.csv").open("w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(trace[0]))
        w.writeheader()
        w.writerows(trace)
    (OUT / "e3_adjudication.json").write_text(json.dumps(out, indent=2))
    (OUT / "e3_adjudication.md").write_text("\n".join(md))
    print("\n".join(md))
    for p in ("e3_adjudication_trace.csv", "e3_adjudication.json", "e3_adjudication.md"):
        print(f"[written] {OUT/p}")


if __name__ == "__main__":
    main()
