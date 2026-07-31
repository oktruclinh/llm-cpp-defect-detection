#!/usr/bin/env python3
"""Batch runner: for each defect × run -> generate test, evaluate, append a master-table row.
Resumable: skips (defect,model,run) rows already present in the output jsonl.
Generation is cached separately by generate_tests; evaluation is re-run (cheap).

Usage: run_tier.py --tier A --model deepseek-chat --runs 3
"""
import argparse
import csv
import json
import sys
import time
import traceback
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
sys.path[:0] = [str(STUDY / "generate"), str(STUDY / "harness"),
                str(STUDY / "benchmarks"), str(STUDY / "measure")]
from defects4c_adapter import load_all          # noqa: E402
from generate_tests import generate, repo_for   # noqa: E402
from detection import evaluate                   # noqa: E402

DEFECTS4C_ROOT = "/home/truclinh/Thesis/kse2026/defects4c/defectsc_tpl"
RESULTS = STUDY / "data/results"
ROW_KEYS = ["defect_id", "project", "bug_type", "model", "model_version", "condition",
            "run_id", "seed", "repeat", "compiled_fixed", "n_tests", "n_valid",
            "n_invalid_on_fixed", "n_flaky_fixed", "n_flaky_buggy", "n_detecting",
            "real_bug_detected", "real_bug_detected_strict", "reached_buggy", "outcome",
            "shots", "tokens_in", "tokens_out"]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--tier", default="A")
    ap.add_argument("--model", default="deepseek-chat")
    ap.add_argument("--runs", type=int, default=3)
    ap.add_argument("--condition", default="fixed", choices=["fixed", "buggy", "no_focal"])
    ap.add_argument("--repeat", type=int, default=5, help="executions per version (flakiness gate)")
    ap.add_argument("--root", default=DEFECTS4C_ROOT)
    args = ap.parse_args()

    RESULTS.mkdir(parents=True, exist_ok=True)
    rows_path = RESULTS / f"rows_tier{args.tier}_{args.model}_{args.condition}.jsonl"
    done = set()
    if rows_path.is_file():
        for l in rows_path.read_text().splitlines():
            if l.strip():
                r = json.loads(l)
                done.add((r["defect_id"], r["model"], r["run_id"]))

    defects = [d for d in load_all(Path(args.root)) if d["tier"] == args.tier.upper()]
    # keep only projects with a registered repo
    defects = [d for d in defects if d["project"] in json.loads((STUDY / "config/repos.json").read_text())]
    total = len(defects) * args.runs
    print(f"tier {args.tier}: {len(defects)} defects × {args.runs} runs = {total} cells "
          f"({len(done)} already done)", flush=True)

    rows = []
    with rows_path.open("a") as fout:
        for d in defects:
            for run in range(1, args.runs + 1):
                key = (d["defect_id"], args.model, run)
                if key in done:
                    continue
                t0 = time.time()
                row = {k: None for k in ROW_KEYS}
                row.update({"defect_id": d["defect_id"], "project": d["project"],
                            "bug_type": d["bug_type"], "model": args.model,
                            "condition": args.condition, "run_id": run, "repeat": args.repeat})
                try:
                    meta = generate(d, args.model, run, condition=args.condition)
                    row.update({"model_version": meta["model_version"], "shots": meta["shots"],
                                "seed": meta.get("seed"),
                                "tokens_in": meta["tokens_in"], "tokens_out": meta["tokens_out"]})
                    ev = evaluate(Path(meta["test_path"]), d, repo_for(d), repeat=args.repeat)
                    # persist FULL eval detail (test names, coverage) for taxonomy/RQ3 — free to redo
                    Path(meta["test_path"]).parent.joinpath("eval.json").write_text(
                        json.dumps(ev, indent=1, ensure_ascii=False))
                    for k in ["compiled_fixed", "n_tests", "n_valid", "n_invalid_on_fixed",
                              "n_flaky_fixed", "n_flaky_buggy", "n_detecting", "real_bug_detected",
                              "real_bug_detected_strict", "reached_buggy", "outcome"]:
                        row[k] = ev.get(k)
                except Exception as e:
                    row["outcome"] = f"ERROR:{type(e).__name__}"
                    row["_err"] = traceback.format_exc()[-400:]
                fout.write(json.dumps(row) + "\n")
                fout.flush()
                rows.append(row)
                print(f"  {d['defect_id'][:40]:40s} {args.condition[:5]:5s} run{run}  "
                      f"{str(row['outcome']):20s} det={row['real_bug_detected']}"
                      f"/strict={row['real_bug_detected_strict']} ({time.time()-t0:.1f}s)", flush=True)

    # rebuild full CSV from all rows on disk
    all_rows = [json.loads(l) for l in rows_path.read_text().splitlines() if l.strip()]
    csv_path = RESULTS / f"master_table_tier{args.tier}_{args.model}_{args.condition}.csv"
    with csv_path.open("w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=ROW_KEYS)
        w.writeheader()
        for r in all_rows:
            w.writerow({k: r.get(k) for k in ROW_KEYS})
    print(f"\nwrote {len(all_rows)} rows -> {csv_path}", flush=True)


if __name__ == "__main__":
    main()
