#!/usr/bin/env python3
"""[#23 task2] Diagnostic: the 6 "fence" non-compiles (category 11) start with a stray ```cpp
line that extract_code() failed to strip. Question: if we HAD stripped the markdown fence,
would the test compile (=> extraction artifact, not a C++-competence failure)? Strip pure
fence-marker lines from each test.cpp and re-compile against the fixed version.

  python3 analysis/rq1_fence_diag.py    (cwd = study/)
"""
import csv
import json
import os
import re
import sys
import tempfile
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
sys.path[:0] = [str(STUDY / d) for d in ("generate", "harness", "benchmarks", "measure")]
from defects4c_adapter import load_all                    # noqa: E402
from cpp_harness import compile_test, prepare_version     # noqa: E402

D4C = Path(os.environ.get("D4C_ROOT", "/home/truclinh/Thesis/kse2026/defects4c/defectsc_tpl"))
REPOS = json.loads((STUDY / "config/repos.json").read_text())
FENCE = re.compile(r"^\s*```+\s*[a-zA-Z+]*\s*$")  # a line that is ONLY a markdown fence


def first_error(s):
    for ln in (s or "").splitlines():
        if " error:" in ln:
            return ln.split(" error:", 1)[1].strip()[:160]
    return ""


def main():
    defs = {d["defect_id"]: d for d in load_all(D4C)}
    idx = list(csv.DictReader((STUDY / "data/taxonomy/rq1_index.csv").open()))
    fence = [r for r in idx if "stray" in r["first_error"].lower() and "`" in r["first_error"]]
    print(f"{len(fence)} fence cells\n")
    now_compile = 0
    for r in fence:
        d = defs[r["defect_id"]]
        repo = Path(REPOS[r["project"]])
        tp = STUDY / "data/raw" / r["defect_id"] / r["model"] / f"run{r['run_id']}" / "test.cpp"
        stripped = "\n".join(l for l in tp.read_text(errors="replace").splitlines()
                             if not FENCE.match(l))
        prepare_version(repo, d, "fixed")
        with tempfile.TemporaryDirectory() as td:
            tf = Path(td) / "t.cpp"
            tf.write_text(stripped)
            b = compile_test(tf, d, repo, Path(td) / "b")
        now_compile += b.compiled
        print(f"  id {r['id']:>2} {r['model'][:8]:8s} after strip -> compiled={b.compiled}"
              + ("" if b.compiled else f" | real error: {first_error(b.stderr)}"))
    print(f"\n{now_compile}/{len(fence)} compile after fence-stripping "
          f"(=> extraction artifacts); {len(fence)-now_compile} fail for a real C++ reason")


if __name__ == "__main__":
    main()
