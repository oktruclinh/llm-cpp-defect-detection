#!/usr/bin/env python3
"""[#23 task3] Build a BLANK RQ1 coding tool for the labmate on a SAMPLE of cells, so we get a
human-human agreement number for RQ1 (pre-empts the 'single-coder' objection). Stratified
sample = the cells Truc Linh changed vs the machine + the low-confidence cell + a random
remainder. The labmate codes INDEPENDENTLY (blind to Linh's labels) in the v2 codebook
(11 categories: 1-10 + 12; fence retired). Exports rq1_labmate.csv -> analysis/rq1_labmate_kappa.py.

  python3 analysis/make_rq1_labmate_html.py   ->   data/taxonomy/rq1_labmate.html
"""
import csv
import json
import random
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
TAX = STUDY / "data/taxonomy"
SAMPLE_RANDOM = 8      # random cells on top of the informative (changed + low-conf) ones
SEED = 42


def main():
    audit = list(csv.DictReader((TAX / "rq1_audit_review.csv").open(encoding="utf-8-sig")))
    idx = {r["id"]: r for r in csv.DictReader((TAX / "rq1_index.csv").open())}
    informative = [r["id"] for r in audit
                   if r["changed"].strip().upper() == "YES" or r["confidence"].strip() == "low"]
    rest = [r["id"] for r in audit if r["id"] not in informative]
    random.Random(SEED).shuffle(rest)
    sample = sorted(set(informative) | set(rest[:SAMPLE_RANDOM]), key=int)

    data = []
    for cid in sample:
        a = next(r for r in audit if r["id"] == cid)
        ef = TAX / "rq1_errors" / f"{cid}.txt"     # real error (fence cells already resolved)
        err = ef.read_text(errors="replace") if ef.is_file() else "(no error file)"
        lines = err.splitlines()
        data.append({"id": cid, "defect": a["defect"], "model": a["model"],
                     "err": "\n".join(lines[:110]) + (f"\n... (+{len(lines)-110})" if len(lines) > 110 else "")})
    blob = json.dumps(data, ensure_ascii=False).replace("</", "<\\/")
    (TAX / "rq1_labmate.html").write_text(_TPL.replace("/*__DATA__*/", blob))
    print(f"wrote data/taxonomy/rq1_labmate.html  ({len(data)} cells: "
          f"{len(informative)} informative + {len(sample)-len(informative)} random)")


_TPL = r"""<!DOCTYPE html><html lang="vi"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1">
<title>RQ1 — gán nhãn độc lập (labmate)</title>
<style>
:root{--bg:#f6f8fa;--card:#fff;--ink:#1f2328;--mut:#59636e;--line:#d1d9e0;--blue:#0969da;--green:#1a7f37}
*{box-sizing:border-box}body{margin:0;background:var(--bg);color:var(--ink);font:15px/1.55 -apple-system,Segoe UI,Roboto,Arial,sans-serif}
header{position:sticky;top:0;z-index:9;background:#fffffff2;backdrop-filter:blur(6px);border-bottom:1px solid var(--line);padding:12px 18px}
header h1{margin:0 0 6px;font-size:18px}.bar{height:8px;background:#eaeef2;border-radius:6px;overflow:hidden;margin:8px 0}.bar>span{display:block;height:100%;background:var(--green);width:0}
main{max-width:1000px;margin:0 auto;padding:18px}.note-box{background:#fff8c5;border:1px solid #e7c200;border-radius:8px;padding:10px 14px;margin:12px 0;font-size:13.5px}
.legend{background:#ddf4ff;border:1px solid #54aeff;border-radius:8px;padding:10px 14px;margin:12px 0;font-size:12.5px}
.cell{background:var(--card);border:1px solid var(--line);border-radius:12px;padding:14px 16px;margin:14px 0}.cell.done{border-color:#1a7f37;box-shadow:0 0 0 1px #1a7f3733}
.cid{font-weight:700}.meta{color:var(--mut);font-size:12.5px;font-family:monospace}
pre{background:#0d1117;color:#e6edf3;border-radius:8px;padding:10px 12px;overflow:auto;font-size:12px;line-height:1.4;max-height:240px}
.lbl{font-size:12px;color:var(--mut);text-transform:uppercase;margin:10px 0 4px}.opts{display:flex;flex-wrap:wrap;gap:6px}
.opt{border:1px solid var(--line);border-radius:8px;padding:5px 9px;cursor:pointer;font-size:12.5px}.opt.sel{background:#ddf4ff;border-color:var(--blue);font-weight:600}.opt input{margin-right:5px}
button{background:var(--blue);color:#fff;border:0;border-radius:8px;padding:8px 14px;font-weight:600;cursor:pointer}
</style></head><body>
<header><h1>RQ1 — gán nhãn lỗi compile (độc lập). Bạn là coder thứ 2.</h1>
<div><span id="prog">0/0</span> <button onclick="exportCSV()">⬇ Xuất rq1_labmate.csv</button> <span id="saved" style="color:var(--mut);font-size:12px"></span></div>
<div class="bar"><span id="pbar"></span></div></header>
<main>
<div class="note-box">⚠️ Làm <b>độc lập</b>, đừng xem nhãn của ai. Đọc <b>lỗi compiler</b>, hỏi "nguyên nhân GỐC là gì?" (bỏ qua cascade), chọn 1 loại. Xong bấm Xuất và gửi lại.</div>
<div class="legend"><b>1</b> UDL/namespace · <b>2</b> static_assert/trait · <b>3</b> Hallucinated/wrong API (hàm/member không tồn tại, sai chữ ký) · <b>4</b> Missing include/unknown type · <b>5</b> Template/overload deduction · <b>6</b> Access control (private/protected) · <b>7</b> Reference/value-category · <b>8</b> Type mismatch/conversion · <b>9</b> Malformed C++ syntax · <b>10</b> Other · <b>12</b> Test-harness/macro misuse (sai macro EXPECT_EQ/ASSERT_*)</div>
<div id="cells"></div></main>
<script>
const DATA=/*__DATA__*/;
const OPTS=[["1","UDL/namespace"],["2","static_assert/trait"],["3","Hallucinated/wrong API"],["4","Missing include/type"],["5","Template/overload deduction"],["6","Access control"],["7","Reference/value-category"],["8","Type mismatch/conversion"],["9","Malformed syntax"],["10","Other"],["12","Test-harness/macro misuse"]];
const esc=s=>s.replace(/&/g,"&amp;").replace(/</g,"&lt;").replace(/>/g,"&gt;");
let state={};
function load(){try{state=JSON.parse(localStorage.getItem("rq1_labmate")||"{}")}catch(e){state={}}}
function save(){localStorage.setItem("rq1_labmate",JSON.stringify(state));const s=document.getElementById("saved");s.textContent=" đã lưu ✓";setTimeout(()=>s.textContent="",1000)}
function prog(){const n=Object.values(state).filter(v=>v&&v.label).length;document.getElementById("prog").textContent=n+"/"+DATA.length;document.getElementById("pbar").style.width=(100*n/DATA.length)+"%"}
function pick(id,v){state[id]=Object.assign({},state[id],{label:v});save();render()}
function render(){document.getElementById("cells").innerHTML=DATA.map(c=>{
  const cur=(state[c.id]||{}).label;
  const opts=OPTS.map(([v,t])=>`<label class="opt${cur===v?' sel':''}"><input type="radio" name="c${c.id}" ${cur===v?"checked":""} onchange="pick('${c.id}','${v}')">${v}. ${t}</label>`).join("");
  return `<div class="cell${cur?' done':''}"><div><span class="cid">id ${c.id}</span> <span class="meta">${c.defect} · ${c.model}</span></div>
   <div class="lbl">lỗi compiler</div><pre>${esc(c.err)}</pre><div class="lbl">nguyên nhân gốc:</div><div class="opts">${opts}</div></div>`}).join("");prog()}
function exportCSV(){const un=DATA.filter(c=>!(state[c.id]||{}).label);if(un.length&&!confirm("Còn "+un.length+" ô chưa gán. Vẫn xuất?"))return;
  let out="id,label\n";DATA.forEach(c=>{out+=`${c.id},${(state[c.id]||{}).label||""}\n`});
  const a=document.createElement("a");a.href=URL.createObjectURL(new Blob([out],{type:"text/csv"}));a.download="rq1_labmate.csv";a.click()}
load();render();
</script></body></html>"""


if __name__ == "__main__":
    main()
