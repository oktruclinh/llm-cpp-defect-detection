#!/usr/bin/env python3
"""[#23] Build the RQ3 (oracle-weakness) annotator, v2 per RQ3_Codebook.md:
 - shows the BUGGY<->FIXED focal DIFF per cell (mandatory for O2/O4 -- codebook Sec.2),
 - 4 categories O2/O3/O4/O5 in decision-tree order (O1 echo dropped: near-empty under
   generate-on-fixed, codebook Sec.1.2/5).
Two coders open it, pick "Coder 1/2", label independently, export rq3_coderN.csv -> kappa_merge.

  python3 analysis/make_rq3_html.py    ->    data/taxonomy/rq3_annotator.html
"""
import csv
import difflib
import json
import os
import sys
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
sys.path[:0] = [str(STUDY / d) for d in ("generate", "harness", "benchmarks", "measure")]
from defects4c_adapter import load_all, extract_focal     # noqa: E402

TAX = STUDY / "data/taxonomy"
D4C = Path(os.environ.get("D4C_ROOT", "/home/truclinh/Thesis/kse2026/defects4c/defectsc_tpl"))
REPOS = json.loads((STUDY / "config/repos.json").read_text())


def resolve_focal(p):
    fp = Path(p)
    if fp.is_file():
        return fp
    for alt in sorted(fp.parent.parent.glob("run*/focal.txt")):
        if alt.is_file():
            return alt
    return fp


def capped(text, cap):
    ls = text.splitlines()
    return "\n".join(ls[:cap]) + (f"\n... (+{len(ls) - cap} dong)" if len(ls) > cap else "")


def main():
    defs = {d["defect_id"]: d for d in load_all(D4C)}
    rows = list(csv.DictReader((TAX / "rq3_coder1.csv").open()))
    diff_cache = {}
    for did in {r["defect_id"] for r in rows}:                 # one diff per unique defect
        d = defs.get(did)
        try:
            repo = Path(REPOS[d["project"]])
            buggy = extract_focal(repo, d["commit_before"], d["focal_src"], d["func_start"], d["func_end"]) or ""
            fixed = extract_focal(repo, d["commit_after"], d["focal_src"], d["func_start"], d["func_end"]) or ""
            diff = list(difflib.unified_diff(buggy.splitlines(), fixed.splitlines(),
                                             "buggy", "fixed", lineterm=""))
            diff_cache[did] = capped("\n".join(diff), 45) if diff else "(khong lay duoc diff)"
        except Exception as e:
            diff_cache[did] = f"(loi lay diff: {e})"

    data = []
    for r in rows:
        data.append({"id": r["id"], "defect": r["defect_id"], "model": r["model"],
                     "diff": diff_cache.get(r["defect_id"], "(n/a)"),
                     "focal": capped(resolve_focal(r["focal_path"]).read_text(errors="replace"), 60),
                     "test": capped(Path(r["test_path"]).read_text(errors="replace")
                                    if Path(r["test_path"]).is_file() else "(no test)", 130)})
    blob = json.dumps(data, ensure_ascii=False).replace("</", "<\\/")
    (TAX / "rq3_annotator.html").write_text(_TPL.replace("/*__DATA__*/", blob))
    print(f"wrote data/taxonomy/rq3_annotator.html  ({len(data)} cells, {len(diff_cache)} diffs, "
          f"{len(_TPL.replace('/*__DATA__*/', blob))//1024} KB)")


_TPL = r"""<!DOCTYPE html><html lang="vi"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1">
<title>RQ3 — oracle yếu (có diff)</title><style>
:root{--bg:#f6f8fa;--card:#fff;--ink:#1f2328;--mut:#59636e;--line:#d1d9e0;--blue:#0969da;--green:#1a7f37;--red:#cf222e}
*{box-sizing:border-box}body{margin:0;background:var(--bg);color:var(--ink);font:15px/1.55 -apple-system,Segoe UI,Roboto,Arial,sans-serif}
header{position:sticky;top:0;z-index:9;background:#fffffff2;backdrop-filter:blur(6px);border-bottom:1px solid var(--line);padding:12px 18px}
header h1{margin:0 0 6px;font-size:17px}.bar{height:8px;background:#eaeef2;border-radius:6px;overflow:hidden;margin:8px 0}.bar>span{display:block;height:100%;background:var(--green);width:0}
main{max-width:1000px;margin:0 auto;padding:18px}select{padding:6px 8px;border:1px solid var(--line);border-radius:6px;font:inherit}
.note-box{background:#fff8c5;border:1px solid #e7c200;border-radius:8px;padding:10px 14px;margin:12px 0;font-size:13px}
.legend{background:#ddf4ff;border:1px solid #54aeff;border-radius:8px;padding:10px 14px;margin:12px 0;font-size:12.5px}.legend b{color:var(--blue)}
.cell{background:var(--card);border:1px solid var(--line);border-radius:12px;padding:14px 16px;margin:14px 0}.cell.done{border-color:#1a7f37;box-shadow:0 0 0 1px #1a7f3733}
.cid{font-weight:700}.meta{color:var(--mut);font-size:12.5px;font-family:monospace}
.lbl{font-size:12px;color:var(--mut);text-transform:uppercase;letter-spacing:.04em;margin:10px 0 4px}
pre{background:#0d1117;color:#e6edf3;border-radius:8px;padding:10px 12px;overflow:auto;font-size:12px;line-height:1.45;max-height:280px;margin:0}
pre.diff .del{color:#ff9492;background:#331} pre.diff .add{color:#7ee787;background:#031} pre.diff .hd{color:#6cb6ff}
.opts{display:flex;flex-wrap:wrap;gap:6px}.opt{border:1px solid var(--line);border-radius:8px;padding:6px 10px;cursor:pointer;font-size:12.5px}
.opt:hover{background:#eaeef2}.opt.sel{background:#ddf4ff;border-color:var(--blue);font-weight:600}.opt input{margin-right:6px}
.nin{width:100%;margin-top:8px;padding:6px 8px;border:1px solid var(--line);border-radius:6px;font:inherit}
button{background:var(--blue);color:#fff;border:0;border-radius:8px;padding:8px 14px;font-weight:600;cursor:pointer}
</style></head><body>
<header><h1>RQ3 — vì sao test biên dịch được, chạy qua dòng lỗi, nhưng KHÔNG bắt được bug?</h1>
<div>Bạn là: <select id="coder"><option value="1">Coder 1</option><option value="2">Coder 2</option></select>
 · <span id="prog">0/0</span> <button onclick="exportCSV()">⬇ Xuất CSV</button> <span id="saved" style="color:var(--mut);font-size:12px"></span></div>
<div class="bar"><span id="pbar"></span></div></header>
<main>
<div class="note-box">⚠️ Làm <b>độc lập</b>. Với mỗi ô: xem <b>DIFF</b> (bug nằm đâu) → xem <b>TEST</b> → theo cây quyết định, hỏi lần lượt:
 <b>(1)</b> assertion có kiểm GIÁ TRỊ ở đoạn sửa không? nếu chỉ no-throw/!empty/kiểu/size → <b>O3</b>.
 <b>(2)</b> test có quan sát đúng bề mặt chứa bug không (theo diff)? nếu kiểm nhầm hàm/bề mặt → <b>O2</b>.
 <b>(3)</b> input có kích hoạt nhánh lỗi không? nếu lỗi==sửa cho input này → <b>O4</b>. Còn lại → <b>O5</b>.</div>
<div class="legend"><b>O3</b> Too-weak assertion (đứt OBSERVE — chỉ no-throw/!empty/kiểu/size, không xét giá trị) &nbsp;|&nbsp;
 <b>O2</b> Wrong API surface (đứt REACH/quan sát — test kiểm nhầm hàm/bề mặt, bug không lộ ra) &nbsp;|&nbsp;
 <b>O4</b> Insufficient input (đứt TRIGGER — assertion đủ mạnh & đúng chỗ, nhưng input không kích hoạt lỗi) &nbsp;|&nbsp;
 <b>O5</b> Khác/không rõ. &nbsp;<i>(O1 echo đã bỏ: trong thiết kế sinh-trên-bản-sửa, echo = giá trị đúng, không gây trượt.)</i></div>
<div id="cells"></div></main>
<script>
const DATA=/*__DATA__*/;
const OPTS=[["3","O3 Too-weak assertion"],["2","O2 Wrong API surface"],["4","O4 Insufficient input"],["5","O5 Other/unclear"]];
const esc=s=>s.replace(/&/g,"&amp;").replace(/</g,"&lt;").replace(/>/g,"&gt;");
function diffHtml(t){return t.split("\n").map(l=>{const c=l[0];const k=c==="-"?"del":c==="+"?"add":(c==="@"?"hd":"");return k?`<span class="${k}">${esc(l)}</span>`:esc(l)}).join("\n")}
let coder="1",state={};
const key=()=>"rq3v2_coder"+coder;
function load(){try{state=JSON.parse(localStorage.getItem(key())||"{}")}catch(e){state={}}}
function save(){localStorage.setItem(key(),JSON.stringify(state));const s=document.getElementById("saved");s.textContent=" đã lưu ✓";setTimeout(()=>s.textContent="",1000)}
function prog(){const n=Object.values(state).filter(v=>v&&v.label).length;document.getElementById("prog").textContent=n+"/"+DATA.length;document.getElementById("pbar").style.width=(100*n/DATA.length)+"%"}
function pick(id,v){state[id]=Object.assign({},state[id],{label:v});save();render()}
function note(id,v){state[id]=Object.assign({},state[id],{note:v});save()}
function render(){document.getElementById("cells").innerHTML=DATA.map(c=>{
 const st=state[c.id]||{},cur=st.label,nt=st.note||"";
 const opts=OPTS.map(([v,t])=>`<label class="opt${cur===v?' sel':''}"><input type="radio" name="c${c.id}" ${cur===v?"checked":""} onchange="pick('${c.id}','${v}')">${t}</label>`).join("");
 return `<div class="cell${cur?' done':''}"><div><span class="cid">id ${c.id}</span> <span class="meta">${c.defect} · ${c.model}</span></div>
  <div class="lbl">① DIFF bug (buggy → fixed)</div><pre class="diff">${diffHtml(c.diff)}</pre>
  <div class="lbl">② Hàm focal (bản sửa, đầy đủ)</div><pre>${esc(c.focal)}</pre>
  <div class="lbl">③ Bài test (miss bug)</div><pre>${esc(c.test)}</pre>
  <div class="lbl">Vì sao miss? chọn 1:</div><div class="opts">${opts}</div>
  <input class="nin" placeholder="ghi chú (tuỳ chọn)" value="${esc(nt)}" oninput="note('${c.id}',this.value)"></div>`}).join("");prog()}
function exportCSV(){const miss=DATA.filter(c=>!(state[c.id]||{}).label);if(miss.length&&!confirm("Còn "+miss.length+" ô chưa gán. Vẫn xuất?"))return;
 let out="id,defect_id,model,label,notes\n";DATA.forEach(c=>{const s=state[c.id]||{};const nt=(s.note||"").replace(/"/g,'""');out+=`${c.id},${c.defect},${c.model},${s.label||""},"${nt}"\n`});
 const a=document.createElement("a");a.href=URL.createObjectURL(new Blob([out],{type:"text/csv"}));a.download="rq3_coder"+coder+".csv";a.click()}
document.getElementById("coder").onchange=e=>{coder=e.target.value;load();render()};
load();render();
</script></body></html>"""


if __name__ == "__main__":
    main()
