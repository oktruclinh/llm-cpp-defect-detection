#!/usr/bin/env python3
"""[#23] Build data/taxonomy/ADJUDICATION.md: the cells where the two coders disagreed
(RQ1 sample + RQ3), each with the evidence + both coders' label & reason + a blank AGREED
line. The two humans discuss each, write the consensus label -> final category counts.
The reported kappa stays PRE-adjudication (RQ1 0.56, RQ3 0.80). cwd = study/.
"""
import csv
from pathlib import Path

TAX = Path(__file__).resolve().parent.parent / "data/taxonomy"
RQ1 = {1: "UDL/namespace", 2: "static_assert/trait", 3: "wrong-API", 4: "missing-include/type",
       5: "template/overload", 6: "access-control", 7: "reference/value-cat",
       8: "type-mismatch", 9: "malformed-syntax", 10: "Other", 12: "macro-misuse"}
RQ3 = {2: "O2 wrong-surface", 3: "O3 too-weak", 4: "O4 insufficient-input", 5: "O5 other"}


def main():
    out = ["# Phiếu ADJUDICATE — 13 ô bất đồng\n",
           "Với mỗi ô: đọc **bằng chứng**, xem lý do 2 bên, **bàn rồi ghi 1 nhãn** vào dòng `AGREED:`.",
           "κ giữ nguyên (RQ1 0.56 / RQ3 0.80) — đây chỉ chốt nhãn CUỐI cho bảng phân bố.\n", "---\n"]

    # ---- RQ1: Linh audit (rq1_final) vs labmate (rq1_labmate) ----
    # Evidence = the RESOLVED error in rq1_errors/<id>.txt (fence cells were re-compiled to their
    # real underlying error), NOT audit_review's first_error/reason which still show the stale fence.
    def first_err(s):
        for ln in (s or "").splitlines():
            if " error:" in ln:
                return ln.split(" error:", 1)[1].strip()[:150]
        return "(no error line)"
    fin1 = {r["id"]: r for r in csv.DictReader((TAX / "rq1_final.csv").open())}
    linh = {i: r["final_code"].strip() for i, r in fin1.items()}
    lab = {r["id"]: r["label"].strip() for r in csv.DictReader((TAX / "rq1_labmate.csv").open()) if r["label"].strip()}
    rev = {r["id"]: r for r in csv.DictReader((TAX / "rq1_audit_review.csv").open(encoding="utf-8-sig"))}
    disp1 = sorted((i for i in set(linh) & set(lab) if linh[i] != lab[i]), key=int)
    out.append(f"## RQ1 — {len(disp1)} ô bất đồng (lỗi compile)\n")
    for i in disp1:
        rv = rev.get(i, {})
        ef = TAX / "rq1_errors" / f"{i}.txt"
        fe = first_err(ef.read_text(errors="replace")) if ef.is_file() else rv.get("first_error", "?")
        fenced = fin1.get(i, {}).get("source") == "fence->underlying"
        out.append(f"### id {i} — `{rv.get('defect','?')}` · {rv.get('model','?')}")
        out.append(f"- **Lỗi:** `{fe}`" + ("  *(ô có markdown fence; đây là lỗi THẬT bên dưới sau khi strip)*" if fenced else ""))
        reason = "" if fenced else rv.get("reason", "")[:150]   # audit reason stale for fence cells
        out.append(f"- **Coder 1 (Linh):** {linh[i]}. {RQ1.get(int(linh[i]),'?')}" + (f" — *{reason}*" if reason else ""))
        out.append(f"- **Coder 2 (labmate):** {lab[i]}. {RQ1.get(int(lab[i]),'?')}")
        out.append(f"- **AGREED:** ___\n")

    # ---- RQ3: Linh firstpass vs labmate (rq3_coder2) ----
    O2N = {"O1": "1", "O2": "2", "O3": "3", "O4": "4", "O5": "5"}
    fp = {r["id"]: r for r in csv.DictReader((TAX / "RQ3_coder1/rq3_firstpass_labels.csv").open(encoding="utf-8-sig"))}
    c1 = {i: O2N.get(r["proposed_label"].strip(), r["proposed_label"].strip()) for i, r in fp.items()}
    c2 = {r["id"]: r["label"].strip() for r in csv.DictReader((TAX / "rq3_coder2.csv").open()) if r["label"].strip()}
    c2n = {r["id"]: r.get("notes", "") for r in csv.DictReader((TAX / "rq3_coder2.csv").open())}
    disp3 = sorted((i for i in set(c1) & set(c2) if c1[i] != c2[i]), key=int)
    out.append(f"## RQ3 — {len(disp3)} ô bất đồng (oracle yếu)\n")
    for i in disp3:
        r = fp.get(i, {})
        out.append(f"### id {i} — `{r.get('defect','?')}` · {r.get('model','?')}")
        out.append(f"- **Bug:** {r.get('bug_summary','?')}")
        out.append(f"- **Coder 1 (Linh):** {RQ3.get(int(c1[i]),'?')} — *{r.get('reason','')[:170]}*")
        out.append(f"- **Coder 2 (labmate):** {RQ3.get(int(c2[i]),'?')} — *{c2n.get(i,'')[:120]}*")
        out.append(f"- **AGREED:** ___\n")

    (TAX / "ADJUDICATION.md").write_text("\n".join(out))
    print(f"wrote data/taxonomy/ADJUDICATION.md  (RQ1 {len(disp1)} + RQ3 {len(disp3)} = {len(disp1)+len(disp3)} cells)")


if __name__ == "__main__":
    main()
