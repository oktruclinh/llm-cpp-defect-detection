#!/usr/bin/env python3
"""[#23 task3] RQ1 human-human agreement on the labmate sample: Cohen's kappa + percent
agreement between Truc Linh's audited labels (rq1_final.csv) and the labmate's independent
labels (rq1_labmate.csv). Report BOTH kappa and % agreement (kappa can deflate under the
dominant 'wrong-API' category -- prevalence paradox -- at this small n).

  python3 analysis/rq1_labmate_kappa.py   (after the labmate exports rq1_labmate.csv)
"""
import csv
from collections import Counter
from pathlib import Path

TAX = Path(__file__).resolve().parent.parent / "data/taxonomy"
CATS = {1: "UDL", 2: "static_assert", 3: "wrong-API", 4: "missing-include", 5: "template",
        6: "access", 7: "reference", 8: "type-mismatch", 9: "malformed", 10: "Other",
        12: "macro-misuse"}


def cohen_kappa(a, b):
    n = len(a)
    po = sum(1 for x, y in zip(a, b) if x == y) / n
    ca, cb = Counter(a), Counter(b)
    pe = sum((ca[c] / n) * (cb[c] / n) for c in set(a) | set(b))
    return (po - pe) / (1 - pe) if pe < 1 else 1.0, po


def main():
    lab_p = TAX / "rq1_labmate.csv"
    if not lab_p.is_file():
        print("chua co rq1_labmate.csv -- labmate mo rq1_labmate.html, gan nhan, bam Xuat CSV.")
        return
    linh = {r["id"]: r["final_code"].strip() for r in csv.DictReader((TAX / "rq1_final.csv").open())}
    lab = {r["id"]: r["label"].strip() for r in csv.DictReader(lab_p.open()) if r["label"].strip()}
    ids = sorted(set(linh) & set(lab), key=int)
    if not ids:
        print("khong co o nao chung.")
        return
    a = [linh[i] for i in ids]
    b = [lab[i] for i in ids]
    k, po = cohen_kappa(a, b)
    interp = ("poor" if k < 0.2 else "fair" if k < 0.4 else "moderate" if k < 0.6
              else "substantial" if k < 0.8 else "almost perfect")
    print(f"=== RQ1 human-human agreement (Linh vs labmate, n={len(ids)}) ===")
    print(f"  percent agreement = {100*po:.0f}%")
    print(f"  Cohen's kappa     = {k:.3f} ({interp})")
    dis = [(i, linh[i], lab[i]) for i in ids if linh[i] != lab[i]]
    print(f"\n  disagreements ({len(dis)}):")
    for i, x, y in dis:
        xn, yn = CATS.get(int(x), x), CATS.get(int(y), y)
        print(f"    id {i:>2}: Linh={x}.{xn}  vs  labmate={y}.{yn}")


if __name__ == "__main__":
    main()
