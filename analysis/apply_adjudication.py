#!/usr/bin/env python3
"""[#23] Parse the filled data/taxonomy/ADJUDICATION.md, apply the consensus labels to the
disputed cells, and recompute the FINAL category distributions (RQ1 over the 72 audited cells
with the adjudicated corrections; RQ3 over the 47). Prints before/after so any headline shift
is visible. Cohen's kappa stays PRE-adjudication (RQ1 0.56, RQ3 0.80). cwd = study/.
"""
import csv
import re
from collections import Counter
from pathlib import Path

TAX = Path(__file__).resolve().parent.parent / "data/taxonomy"
RQ1 = {1: "UDL/namespace", 2: "static_assert/trait", 3: "wrong-API", 4: "missing-include/type",
       5: "template/overload", 6: "access-control", 7: "reference/value-cat",
       8: "type-mismatch", 9: "malformed-syntax", 10: "Other", 12: "macro-misuse"}
RQ3 = {2: "O2 wrong-surface", 3: "O3 too-weak", 4: "O4 insufficient-input", 5: "O5 other"}
O2N = {"O2": 2, "O3": 3, "O4": 4, "O5": 5}


def parse():
    section, cid, rq1, rq3 = None, None, {}, {}
    for ln in (TAX / "ADJUDICATION.md").read_text().splitlines():
        if ln.startswith("## RQ1"):
            section = "rq1"
        elif ln.startswith("## RQ3"):
            section = "rq3"
        m = re.match(r"### id (\d+)", ln)
        if m:
            cid = m.group(1)
        a = re.search(r"AGREED:\*\*\s*(\S+)", ln)
        if a and cid and a.group(1) != "___":
            v = a.group(1)
            (rq1 if section == "rq1" else rq3)[cid] = v
    return rq1, rq3


def dist(counter, names):
    return ", ".join(f"{names[k]} {counter[k]}" for k in sorted(counter, key=lambda k: -counter[k]))


def main():
    adj1, adj3 = parse()
    print(f"parsed adjudicated labels: RQ1 {len(adj1)}, RQ3 {len(adj3)}\n")

    # RQ1: 72 audited cells, override the adjudicated ones
    fin1 = {r["id"]: int(r["final_code"]) for r in csv.DictReader((TAX / "rq1_final.csv").open())}
    before1 = Counter(fin1.values())
    changed1 = []
    for cid, v in adj1.items():
        if cid in fin1 and fin1[cid] != int(v):
            changed1.append((cid, fin1[cid], int(v)))
            fin1[cid] = int(v)
    after1 = Counter(fin1.values())
    print("=== RQ1 (n=72) ===")
    print(f"  before adjudication: {dist(before1, RQ1)}")
    print(f"  AFTER  adjudication: {dist(after1, RQ1)}")
    print(f"  changed {len(changed1)}: " + ", ".join(f"id{c}:{RQ1[a]}->{RQ1[b]}" for c, a, b in changed1))
    tot = sum(after1.values())
    print("  headline: " + " · ".join(f"{RQ1[k]} {100*after1[k]/tot:.0f}%" for k in (3, 12, 4)))

    # RQ3: 47 cells; base = Linh coder1, override adjudicated
    fin3 = {r["id"]: int(r["label"]) for r in csv.DictReader((TAX / "rq3_coder1.csv").open()) if r["label"].strip()}
    before3 = Counter(fin3.values())
    changed3 = []
    for cid, v in adj3.items():
        code = O2N.get(v, None)
        if code and cid in fin3 and fin3[cid] != code:
            changed3.append((cid, fin3[cid], code))
            fin3[cid] = code
    after3 = Counter(fin3.values())
    print("\n=== RQ3 (n=47) ===")
    print(f"  before adjudication: {dist(before3, RQ3)}")
    print(f"  AFTER  adjudication: {dist(after3, RQ3)}")
    print(f"  changed {len(changed3)}: " + (", ".join(f"id{c}:{RQ3[a]}->{RQ3[b]}" for c, a, b in changed3) or "none"))
    tot3 = sum(after3.values())
    print("  headline: " + " · ".join(f"{RQ3[k]} {100*after3[k]/tot3:.0f}%" for k in (4, 2)))

    # persist final adjudicated labels
    with (TAX / "rq1_final_adjudicated.csv").open("w", newline="") as f:
        w = csv.writer(f); w.writerow(["id", "final_code"])
        for cid in sorted(fin1, key=int): w.writerow([cid, fin1[cid]])
    with (TAX / "rq3_final_adjudicated.csv").open("w", newline="") as f:
        w = csv.writer(f); w.writerow(["id", "final_code"])
        for cid in sorted(fin3, key=int): w.writerow([cid, fin3[cid]])
    print("\nwrote rq1_final_adjudicated.csv + rq3_final_adjudicated.csv")


if __name__ == "__main__":
    main()
