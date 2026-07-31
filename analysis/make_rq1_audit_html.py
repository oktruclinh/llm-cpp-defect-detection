#!/usr/bin/env python3
"""[#23 step 1] Build a self-contained HTML AUDIT tool for RQ1. Each cell shows the full
compiler error + the classifier's proposed category (pre-selected); the human confirms or
corrects, ticks "da duyet", and exports rq1_audit.csv. This yields classifier ACCURACY
(final vs auto), the honest validity evidence for a mechanical task -- NOT kappa.

  python3 analysis/make_rq1_audit_html.py   ->   data/taxonomy/rq1_audit.html
"""
import csv
import json
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
TAX = STUDY / "data/taxonomy"


def main():
    rows = list(csv.DictReader((TAX / "rq1_auto.csv").open()))
    data = []
    for r in rows:
        ef = TAX / "rq1_errors" / f"{r['id']}.txt"
        err = ef.read_text(errors="replace") if ef.is_file() else "(no error file)"
        lines = err.splitlines()
        data.append({"id": r["id"], "defect": r["defect_id"], "model": r["model"],
                     "auto": int(r["auto_code"]), "fe": r["first_error"],
                     "err": "\n".join(lines[:110]) + (f"\n... (+{len(lines)-110})" if len(lines) > 110 else "")})
    blob = json.dumps(data, ensure_ascii=False).replace("</", "<\\/")
    (TAX / "rq1_audit.html").write_text(_TEMPLATE.replace("/*__DATA__*/", blob))
    print(f"wrote data/taxonomy/rq1_audit.html  ({len(data)} cells)")
    print("Mo: xdg-open data/taxonomy/rq1_audit.html")


_TEMPLATE = r"""<!DOCTYPE html>
<html lang="vi"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1">
<title>RQ1 — Audit phân loại lỗi compile</title>
<style>
:root{--bg:#f6f8fa;--card:#fff;--ink:#1f2328;--mut:#59636e;--line:#d1d9e0;--blue:#0969da;--green:#1a7f37;--red:#cf222e}
*{box-sizing:border-box}body{margin:0;background:var(--bg);color:var(--ink);font:15px/1.55 -apple-system,Segoe UI,Roboto,Arial,sans-serif}
header{position:sticky;top:0;z-index:9;background:#fffffff2;backdrop-filter:blur(6px);border-bottom:1px solid var(--line);padding:12px 18px}
header h1{margin:0 0 6px;font-size:18px}
.bar{height:8px;background:#eaeef2;border-radius:6px;overflow:hidden;margin:8px 0}.bar>span{display:block;height:100%;background:var(--green);width:0}
main{max-width:1000px;margin:0 auto;padding:18px}
.note-box{background:#fff8c5;border:1px solid #e7c200;border-radius:8px;padding:10px 14px;margin:12px 0;font-size:13.5px}
.cell{background:var(--card);border:1px solid var(--line);border-radius:12px;padding:14px 16px;margin:14px 0}
.cell.seen{border-color:#1a7f37;box-shadow:0 0 0 1px #1a7f3733}
.cid{font-weight:700}.meta{color:var(--mut);font-size:12.5px;font-family:monospace}
.fe{background:#ffebe9;border:1px solid #ffced0;border-radius:6px;padding:6px 9px;margin:8px 0;font-family:monospace;font-size:12.5px}
pre{background:#0d1117;color:#e6edf3;border-radius:8px;padding:10px 12px;overflow:auto;font-size:12px;line-height:1.4;max-height:240px}
.lbl{font-size:12px;color:var(--mut);text-transform:uppercase;letter-spacing:.04em;margin:10px 0 4px}
.opts{display:flex;flex-wrap:wrap;gap:6px}.opt{border:1px solid var(--line);border-radius:8px;padding:5px 9px;cursor:pointer;font-size:12.5px}
.opt:hover{background:#eaeef2}.opt.sel{background:#ddf4ff;border-color:var(--blue);font-weight:600}.opt input{margin-right:5px}
.nin{width:100%;margin-top:8px;padding:6px 8px;border:1px solid var(--line);border-radius:6px;font:inherit}
button{background:var(--blue);color:#fff;border:0;border-radius:8px;padding:8px 14px;font-weight:600;cursor:pointer}
.chk{margin-top:8px;font-size:13px;color:var(--green);font-weight:600;cursor:pointer;user-select:none}
</style></head><body>
<header>
  <h1>RQ1 — Audit: kiểm nhãn phân loại lỗi compile (máy đề xuất sẵn, bạn duyệt)</h1>
  <div><span id="prog">0/0 đã duyệt</span> · <span id="chg" style="color:var(--red)">0 sửa</span>
   &nbsp; <button onclick="exportCSV()">⬇ Xuất rq1_audit.csv</button>
   <span id="saved" style="color:var(--mut);font-size:12px"></span></div>
  <div class="bar"><span id="pbar"></span></div>
</header>
<main>
  <div class="note-box">📋 Máy đã chọn sẵn 1 nhãn dựa trên dòng lỗi (ô đỏ). Bạn <b>đọc lỗi</b>, nếu đúng thì bấm <b>"✓ đã duyệt"</b>; nếu sai thì chọn lại số khác (rồi nó tự tính là đã duyệt). Mục tiêu là ra <b>độ chính xác</b> của máy, không phải κ. Xong bấm <b>Xuất</b> và gửi file.</div>
  <div id="cells"></div>
</main>
<script>
const DATA = /*__DATA__*/;
const CATS = ["UDL/namespace","static_assert / trait","Hallucinated/wrong API","Missing include / unknown type",
  "Template/overload deduction","Access control","Reference/value-category","Type mismatch/conversion","Malformed C++ syntax","Other"];
const esc=s=>s.replace(/&/g,"&amp;").replace(/</g,"&lt;").replace(/>/g,"&gt;");
let state={};
function load(){try{state=JSON.parse(localStorage.getItem("rq1_audit")||"{}")}catch(e){state={}}}
function save(){localStorage.setItem("rq1_audit",JSON.stringify(state));const s=document.getElementById("saved");s.textContent=" đã lưu ✓";setTimeout(()=>s.textContent="",1000)}
function prog(){const seen=Object.values(state).filter(v=>v&&v.seen).length;
  const chg=DATA.filter(c=>(state[c.id]||{}).final && (state[c.id]).final!=c.auto).length;
  document.getElementById("prog").textContent=seen+"/"+DATA.length+" đã duyệt";
  document.getElementById("chg").textContent=chg+" sửa";
  document.getElementById("pbar").style.width=(100*seen/DATA.length)+"%";}
function pick(id,v){state[id]=Object.assign({},state[id],{final:v,seen:true});save();render();}
function seen(id){const c=state[id]||{};state[id]=Object.assign({},c,{seen:!c.seen});save();render();}
function note(id,v){state[id]=Object.assign({},state[id],{note:v});save();}
function render(){
  document.getElementById("cells").innerHTML=DATA.map(c=>{
    const st=state[c.id]||{}, cur=st.final||String(c.auto), nt=st.note||"";
    const opts=CATS.map((t,i)=>{const v=String(i+1),s=cur===v?" sel":"";
      return `<label class="opt${s}"><input type="radio" name="c${c.id}" ${cur===v?"checked":""} onchange="pick('${c.id}','${v}')">${v}. ${t}</label>`}).join("");
    return `<div class="cell${st.seen?' seen':''}"><div><span class="cid">id ${c.id}</span> <span class="meta">${c.defect} · ${c.model}</span></div>
      <div class="fe">lỗi đầu tiên: ${esc(c.fe||"(không có dòng error:)")}</div>
      <div class="lbl">toàn bộ stderr</div><pre>${esc(c.err)}</pre>
      <div class="lbl">Loại (máy chọn: ${c.auto}. ${CATS[c.auto-1]}):</div><div class="opts">${opts}</div>
      <div class="chk" onclick="seen('${c.id}')">${st.seen?"☑":"☐"} ✓ đã duyệt</div>
      <input class="nin" placeholder="ghi chú (tuỳ chọn)" value="${esc(nt)}" oninput="note('${c.id}',this.value)"></div>`;
  }).join("");
  prog();
}
function exportCSV(){
  const un=DATA.filter(c=>!(state[c.id]||{}).seen);
  if(un.length && !confirm("Còn "+un.length+" ô chưa duyệt. Vẫn xuất?")) return;
  let out="id,defect_id,model,auto_code,final_code,notes\n";
  DATA.forEach(c=>{const s=state[c.id]||{};const nt=(s.note||"").replace(/"/g,'""');
    out+=`${c.id},${c.defect},${c.model},${c.auto},${s.final||c.auto},"${nt}"\n`;});
  const a=document.createElement("a");a.href=URL.createObjectURL(new Blob([out],{type:"text/csv"}));
  a.download="rq1_audit.csv";a.click();
}
load();render();
</script></body></html>"""


if __name__ == "__main__":
    main()
