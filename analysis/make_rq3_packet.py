#!/usr/bin/env python3
"""[#23] Bundle the RQ3 cells into ONE self-contained markdown packet so a REMOTE coder
(the labmate, on another machine) can judge oracle-weakness WITHOUT the raw repo: for each
id it embeds the focal function (focal.txt) + the generated test (test.cpp). Paths are read
from data/taxonomy/rq3_coder1.csv. Output: data/taxonomy/rq3_packet.md.
"""
import csv
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
TAX = STUDY / "data/taxonomy"
CAP = 200  # max embedded lines per file (avoid pathologically long tests)


def resolve_focal(p):
    """focal.txt is identical across runs of the same defect/model (same code under test),
    so if this run's copy is missing, fall back to any sibling run that has it."""
    fp = Path(p)
    if fp.is_file():
        return fp
    for alt in sorted(fp.parent.parent.glob("run*/focal.txt")):
        if alt.is_file():
            return alt
    return fp


def read_capped(p):
    fp = Path(p)
    if not fp.is_file():
        return f"(khong tim thay file: {p})"
    lines = fp.read_text(errors="replace").splitlines()
    body = "\n".join(lines[:CAP])
    if len(lines) > CAP:
        body += f"\n... (cat bot {len(lines) - CAP} dong)"
    return body


def main():
    rows = list(csv.DictReader((TAX / "rq3_coder1.csv").open()))
    out = ["# RQ3 - goi du lieu de gan nhan (doc cung file rq3_coder2.csv)\n",
           "Voi moi dong CSV, tim muc `## id N` tuong ung o duoi, doc HAM FOCAL + BAI TEST,",
           "roi dien 1 so 1..5 vao cot `label` (dinh nghia trong file HUONG_DAN_LABMATE.md).\n",
           f"Tong cong: {len(rows)} o.\n", "---\n"]
    miss = 0
    for r in rows:
        out.append(f"## id {r['id']} - `{r['defect_id']}` - model {r['model']}\n")
        out.append("**Ham focal (ban code DA SUA, dung):**\n\n```cpp")
        f = read_capped(resolve_focal(r["focal_path"]))
        out.append(f)
        out.append("```\n")
        out.append("**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**\n\n```cpp")
        t = read_capped(r["test_path"])
        out.append(t)
        out.append("```\n\n---\n")
        miss += f.startswith("(khong tim") + t.startswith("(khong tim")
    (TAX / "rq3_packet.md").write_text("\n".join(out))
    print(f"wrote data/taxonomy/rq3_packet.md  ({len(rows)} cells"
          + (f", {miss} missing files)" if miss else ")"))


if __name__ == "__main__":
    main()
