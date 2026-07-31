#!/usr/bin/env python3
"""[#23 step 1] Re-compile the non_compilable fixed-condition Tier-A cells to capture the FULL
compiler stderr. The stored eval.json errors are 800-char TAILS that often miss the real
'error:' line (heavy C++ template spew), so both the rule classifier and a human auditor need
the true first error. Dumps per-cell stderr to data/taxonomy/rq1_errors/<id>.txt + an index CSV.

Run once (~10-15 min, local, $0):  python3 analysis/rq1_recompile.py    (cwd = study/)
"""
import csv
import glob
import json
import os
import sys
import tempfile
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
sys.path[:0] = [str(STUDY / d) for d in ("generate", "harness", "benchmarks", "measure", "analysis")]
from defects4c_adapter import load_all                         # noqa: E402
from cpp_harness import compile_test, prepare_version          # noqa: E402

D4C = Path(os.environ.get("D4C_ROOT", "/home/truclinh/Thesis/kse2026/defects4c/defectsc_tpl"))
REPOS = json.loads((STUDY / "config/repos.json").read_text())


def first_error(stderr: str) -> str:
    for ln in (stderr or "").splitlines():
        if " error:" in ln:
            return ln.split(" error:", 1)[1].strip()[:200]
    return ""


def main():
    defs = {d["defect_id"]: d for d in load_all(D4C)}
    cells = []
    for f in glob.glob(str(STUDY / "data/results/rows_tierA_*_fixed.jsonl")):
        for line in open(f):
            if not line.strip():
                continue
            r = json.loads(line)
            if r.get("outcome") == "non_compilable" or r.get("compiled_fixed") is False:
                cells.append(r)
    # group by defect so each fixed checkout happens once
    cells.sort(key=lambda c: (c["project"], c["defect_id"], c["model"], c["run_id"]))
    outdir = STUDY / "data/taxonomy/rq1_errors"
    outdir.mkdir(parents=True, exist_ok=True)

    idx = open(STUDY / "data/taxonomy/rq1_index.csv", "w", newline="")
    w = csv.writer(idx)
    w.writerow(["id", "defect_id", "model", "run_id", "project", "err_file", "first_error"])
    cur = None
    for i, c in enumerate(cells, 1):
        d = defs.get(c["defect_id"])
        repo = Path(REPOS[c["project"]])
        try:
            if cur != c["defect_id"]:
                prepare_version(repo, d, "fixed")
                cur = c["defect_id"]
            tp = STUDY / "data/raw" / c["defect_id"] / c["model"] / f"run{c['run_id']}" / "test.cpp"
            with tempfile.TemporaryDirectory() as td:
                b = compile_test(tp, d, repo, Path(td) / "b")
            err = b.stderr or ""
            compiled = b.compiled
        except Exception as e:
            err, compiled = f"(recompile error: {e})", None
        (outdir / f"{i}.txt").write_text(err)
        fe = first_error(err)
        w.writerow([i, c["defect_id"], c["model"], c["run_id"], c["project"], f"rq1_errors/{i}.txt", fe])
        idx.flush()
        print(f"{i:>2}/{len(cells)} {c['defect_id'][:22]:22s} {c['model'][:8]:8s} "
              f"compiled={compiled} errlen={len(err)} | {fe[:70]}", flush=True)
    idx.close()
    print("RQ1 RECOMPILE DONE")


if __name__ == "__main__":
    main()
