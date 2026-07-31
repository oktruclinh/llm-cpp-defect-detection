#!/usr/bin/env python3
"""Interactive coding helper for ONE human coder (you).

  python3 analysis/annotate.py rq1        # code RQ1 as coder 1 (default)
  python3 analysis/annotate.py rq3        # code RQ3
  python3 analysis/annotate.py rq1 2      # code as coder 2 (the labmate)

Shows each cell + the rubric; you type ONE number = YOUR OWN judgement; it saves to your
coder CSV after every entry (safe to quit & resume anytime). It deliberately NEVER shows the
machine auto-suggest or anyone else's labels -> your coding stays independent, which is the
whole point of Cohen's kappa. Run it in your OWN terminal (it needs the keyboard).
"""
import csv
import sys
import textwrap
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
TAX = STUDY / "data/taxonomy"

CATS = {
    "rq1": ["UDL/namespace", "static_assert/trait", "Hallucinated/wrong API",
            "Missing include/unknown type", "Template/overload deduction", "Access control",
            "Reference/value-category", "Type mismatch/conversion", "Malformed C++ syntax",
            "Other"],
    "rq3": ["O1 Echo/self-confirming", "O2 Wrong API surface", "O3 Too-weak assertion",
            "O4 Insufficient input", "O5 Other/unclear"],
}

HELP = {
    "rq1": """RQ1 - chon theo LOI DAU TIEN (goc re):
  1 UDL/namespace          : 'string literal operator', hau to _hs, thieu using ...::literals
  2 static_assert/trait    : 'static assertion failed', 'no formatter for', type khong format duoc
  3 Hallucinated/wrong API : 'is not a member of', 'has no member', 'was not declared',
                             'no matching function for call'  (ham/overload KHONG ton tai)
  4 Missing include/type   : 'No such file or directory', 'does not name a type', 'unknown type'
  5 Template/overload deduc: 'deduction/substitution failed', 'could not deduce',
                             'no matching ... template'  (ham CO that nhung suy kieu hong)
  6 Access control         : 'is private', 'is protected', 'within this context'
  7 Reference/value-cat    : 'cannot bind', 'lvalue/rvalue', 'discards qualifiers'
  8 Type mismatch/convert  : 'cannot convert', 'no known conversion', 'ambiguous'
  9 Malformed syntax       : 'expected ; before', 'expected primary-expression'
 10 Other                  : khong khop 1-9 (ghi note)
  De phan biet: 3 vs 5 -> API khong ton tai(3) / co nhung suy kieu hong(5); uu tien dong loi DAU.""",
    "rq3": """RQ3 - doc HAM FOCAL + BAI TEST, hoi 'vi sao test pass ca tren ban LOI?':
  1 O1 Echo/self-confirming : assert ghi dung gia tri code DANG tra ra (nhu chep tu output)
  2 O2 Wrong API surface    : bug chi lo qua 1 API khac ma test KHONG goi
  3 O3 Too-weak assertion   : chi check khong rong/khong crash/dung kieu, KHONG check gia tri
  4 O4 Insufficient input   : input khong kich hoat nhanh/dieu kien loi
  5 O5 Other/unclear        : khong ro""",
}

CTX = {"rq1": ["defect_id", "model", "project", "error"],
       "rq3": ["defect_id", "model", "project", "focal_path", "test_path"]}


def resolve_focal(p):
    fp = Path(p)
    if fp.is_file():
        return fp
    for alt in sorted(fp.parent.parent.glob("run*/focal.txt")):
        if alt.is_file():
            return alt
    return fp


def read_capped(p, cap=120):
    fp = Path(p)
    if not fp.is_file():
        return f"(khong tim thay {p})"
    ls = fp.read_text(errors="replace").splitlines()
    return "\n".join(ls[:cap]) + (f"\n... (+{len(ls)-cap} dong)" if len(ls) > cap else "")


def show(rq, r):
    print("\n" + "=" * 78)
    print(f"  id {r['id']}  |  {r['defect_id']}  |  model {r['model']}")
    print("=" * 78)
    if rq == "rq1":
        print("LOI COMPILER:\n" + "\n".join(
            "  " + l for l in textwrap.wrap(r["error"], 74)) + "\n")
    else:
        print("HAM FOCAL (ban da sua, dung):")
        print(read_capped(resolve_focal(r["focal_path"])))
        print("\nBAI TEST (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):")
        print(read_capped(r["test_path"]))
        print()
    cats = CATS[rq]
    print("  ".join(f"[{i+1}] {c}" for i, c in enumerate(cats)))


def main():
    rq = sys.argv[1] if len(sys.argv) > 1 else "rq1"
    coder = sys.argv[2] if len(sys.argv) > 2 else "1"
    if rq not in CATS:
        sys.exit("usage: annotate.py rq1|rq3 [coder=1]")
    path = TAX / f"{rq}_coder{coder}.csv"
    if not path.is_file():
        sys.exit(f"khong thay {path} -- chay: python3 analysis/make_annotation_kit.py")
    rows = list(csv.DictReader(path.open()))
    fields = list(rows[0].keys())
    ncat = len(CATS[rq])

    def save():
        with path.open("w", newline="") as f:
            w = csv.DictWriter(f, fieldnames=fields)
            w.writeheader()
            w.writerows(rows)

    # start at the first row with no label
    i = next((k for k, r in enumerate(rows) if not r.get("label", "").strip()), 0)
    print(f"\nFile: {path.name}   |   {sum(1 for r in rows if r.get('label','').strip())}"
          f"/{len(rows)} da gan.   Go 'h' xem huong dan, 'q' luu & thoat.")
    while True:
        done = sum(1 for r in rows if r.get("label", "").strip())
        if i >= len(rows):
            print(f"\n*** Het {len(rows)} o. Da gan {done}/{len(rows)}. ***")
            if done == len(rows):
                print("HOAN TAT! Gui file nay cho nguoi dieu phoi de tinh kappa.")
                save()
                return
            i = next((k for k, r in enumerate(rows) if not r.get("label", "").strip()), 0)
        r = rows[i]
        show(rq, r)
        cur = r.get("label", "").strip()
        tag = f" (dang la: {cur})" if cur else ""
        ans = input(f"\n[{done}/{len(rows)} done · o {i+1}] nhap 1-{ncat}"
                    f" (b=lui s=bo-qua n=note h=help q=luu&thoat){tag} > ").strip().lower()
        if ans == "q":
            save()
            print(f"Da luu {path.name}. Chay lai lenh nay de tiep tuc.")
            return
        if ans == "h":
            print("\n" + HELP[rq])
            input("  (Enter de tiep tuc)")
            continue
        if ans == "b":
            i = max(0, i - 1)
            continue
        if ans == "s":
            i += 1
            continue
        if ans == "n":
            r["notes"] = input("  note: ").strip()
            save()
            continue
        if ans.isdigit() and 1 <= int(ans) <= ncat:
            r["label"] = ans
            save()
            i += 1
        else:
            print(f"  !! go 1-{ncat}, hoac b/s/n/h/q")


if __name__ == "__main__":
    main()
