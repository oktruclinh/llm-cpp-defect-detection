#!/usr/bin/env python3
"""Reconcile the RQ3 per-coder label files with the frozen analysis set, so that the
published kappa = 0.72 can be recomputed from shipped files alone.

The package shipped `rq3_coder1.csv` / `rq3_coder2.csv` from the FIRST annotation round:
47 suites, 31 from the main campaign plus 16 from the buggy-generation arm that never
enter the paper, annotated before the GPT-5.4 campaign existed. Those files show four
disagreements. The paper's kappa belongs to the merged `coder1` / `coder2` columns of
`rq3_frozen44.csv`, which add a second round covering the 13 GPT-5.4 suites and show six
disagreements. The README already referenced `rq3_delta_coder{1,2}.csv` for that second
round, but those files were never written.

This script closes the gap without rewriting history:
  * round 1 stays exactly as shipped (other annotation tooling joins it positionally
    against rq3_coding_sheet.csv by row order, so it must not be reordered or filtered);
  * the missing round 2 is emitted as rq3_delta_coder{1,2}.csv;
  * a self-contained merged record is emitted as rq3_labels_merged44.csv, keyed by
    (defect_id, model, run) rather than by row position;
  * the merge is verified against rq3_frozen44.csv cell by cell, and kappa is recomputed.

Outputs (into data/taxonomy/):
  rq3_delta_coder1.csv, rq3_delta_coder2.csv, rq3_labels_merged44.csv
  rq3_kappa_verification.json

Run with cwd = artifact/ :  python3 analysis/rq3_reconcile_labels.py
"""
import csv
import json
import re
import sys
from collections import Counter
from pathlib import Path

ART = Path(__file__).resolve().parent.parent
TAX = ART / "data" / "taxonomy"


def cohen_kappa(a, b):
    n = len(a)
    cats = sorted(set(a) | set(b))
    po = sum(1 for x, y in zip(a, b) if x == y) / n
    ca, cb = Counter(a), Counter(b)
    pe = sum((ca[c] / n) * (cb[c] / n) for c in cats)
    return (po - pe) / (1 - pe) if pe < 1 else 1.0, po, pe


def run_of(test_path):
    m = re.search(r"/run(\d)/", test_path)
    return m.group(1) if m else "?"


def is_main_campaign(test_path):
    """Round 1 also covered the buggy-generation arm; those cells are not in the paper."""
    return "/buggy/" not in test_path


def main():
    sheet = list(csv.DictReader((TAX / "rq3_coding_sheet.csv").open()))
    c1 = list(csv.DictReader((TAX / "rq3_coder1.csv").open()))
    c2 = list(csv.DictReader((TAX / "rq3_coder2.csv").open()))
    frozen = list(csv.DictReader((TAX / "rq3_frozen44.csv").open()))

    if not (len(sheet) == len(c1) == len(c2)):
        sys.exit(f"round-1 files disagree in length: sheet={len(sheet)} "
                 f"c1={len(c1)} c2={len(c2)}")
    if not all(s["defect_id"] == a["defect_id"] and s["model"] == a["model"]
               for s, a in zip(sheet, c1)):
        sys.exit("rq3_coder1.csv does not align positionally with rq3_coding_sheet.csv")

    # ---- round 1, restricted to the main campaign ----
    round1 = {}
    for s, a, b in zip(sheet, c1, c2):
        if not is_main_campaign(s["test_path"]):
            continue
        round1[(s["defect_id"], s["model"], run_of(s["test_path"]))] = (
            "O" + a["label"].strip(), "O" + b["label"].strip())

    # ---- round 2 = the suites in the analysis set that round 1 never saw ----
    delta = [r for r in frozen
             if (r["defect_id"], r["model"], r["run"]) not in round1]

    # ---- verify the merge reproduces the frozen columns ----
    mism = []
    for r in frozen:
        k = (r["defect_id"], r["model"], r["run"])
        if k in round1 and round1[k] != (r["coder1"], r["coder2"]):
            mism.append((k, round1[k], (r["coder1"], r["coder2"])))
    if mism:
        for m in mism:
            print("MISMATCH", m)
        sys.exit(f"{len(mism)} round-1 labels disagree with rq3_frozen44.csv")

    # ---- write the missing round-2 files ----
    for who, col in (("1", "coder1"), ("2", "coder2")):
        p = TAX / f"rq3_delta_coder{who}.csv"
        with p.open("w", newline="") as f:
            w = csv.writer(f)
            w.writerow(["id", "defect_id", "model", "run", "label"])
            for i, r in enumerate(delta, 1):
                w.writerow([i, r["defect_id"], r["model"], r["run"], r[col][1:]])
        print(f"[written] {p}  ({len(delta)} rows)")

    # ---- write the self-contained merged record ----
    merged = TAX / "rq3_labels_merged44.csv"
    with merged.open("w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["defect_id", "model", "run", "round", "coder1", "coder2",
                    "agree", "final_after_adjudication"])
        for r in frozen:
            k = (r["defect_id"], r["model"], r["run"])
            w.writerow([r["defect_id"], r["model"], r["run"],
                        "1" if k in round1 else "2",
                        r["coder1"], r["coder2"],
                        "yes" if r["coder1"] == r["coder2"] else "NO",
                        r["final_after_adjudication"]])
    print(f"[written] {merged}  ({len(frozen)} rows)")

    # ---- recompute kappa from the merged record ----
    rows = list(csv.DictReader(merged.open()))
    A = [r["coder1"] for r in rows]
    B = [r["coder2"] for r in rows]
    k, po, pe = cohen_kappa(A, B)
    dis = sum(1 for x, y in zip(A, B) if x != y)

    k1, po1, _ = cohen_kappa([a["label"] for a in c1], [b["label"] for b in c2])
    dis1 = sum(1 for a, b in zip(c1, c2) if a["label"] != b["label"])

    report = {
        "analysis_set": {
            "n": len(rows), "kappa": round(k, 3),
            "observed_agreement": round(po, 4), "expected_agreement": round(pe, 4),
            "disagreements": dis,
            "round1_suites": sum(1 for r in rows if r["round"] == "1"),
            "round2_suites": sum(1 for r in rows if r["round"] == "2"),
            "source": "data/taxonomy/rq3_labels_merged44.csv",
        },
        "round1_file_as_shipped": {
            "n": len(c1), "kappa": round(k1, 3), "observed_agreement": round(po1, 4),
            "disagreements": dis1,
            "note": ("includes 16 buggy-generation-arm suites that are not in the paper "
                     "and predates the 13 GPT-5.4 suites; this is NOT the published kappa"),
        },
        "merge_verified_against_frozen44": True,
        "reproduces_published_kappa": abs(k - 0.72) < 0.005,
    }
    (TAX / "rq3_kappa_verification.json").write_text(json.dumps(report, indent=2))

    print()
    print(f"  analysis set (44):  kappa = {k:.3f}  agreement {100*po:.1f}%  "
          f"disagreements {dis}   <- the published value")
    print(f"  round-1 file (47):  kappa = {k1:.3f}  agreement {100*po1:.1f}%  "
          f"disagreements {dis1}   <- a different population")
    print(f"  merge verified against rq3_frozen44.csv: 0 mismatches")
    print(f"  reproduces published kappa = 0.72: {report['reproduces_published_kappa']}")
    print(f"[written] {TAX/'rq3_kappa_verification.json'}")


if __name__ == "__main__":
    main()
