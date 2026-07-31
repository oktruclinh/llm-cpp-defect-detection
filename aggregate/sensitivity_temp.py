#!/usr/bin/env python3
"""Temperature sensitivity sub-study (pre-empts 'low detection is a temp artifact').
Runs a small defect sample across temperatures {0.0, 0.2, 0.8} × 3 runs, condition=fixed,
and reports whether outcome distribution / detection / non-compile / wrong-oracle move.
deepseek-chat = the 0.2 arm (reuses existing cache, $0). t00/t08 = new arms.
"""
import json
import sys
from collections import Counter
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
sys.path[:0] = [str(STUDY / "generate"), str(STUDY / "harness"),
                str(STUDY / "benchmarks"), str(STUDY / "measure")]
from defects4c_adapter import load_all          # noqa: E402
from generate_tests import generate, repo_for   # noqa: E402
from detection import evaluate                   # noqa: E402

# temperature -> model config name
ARMS = {0.0: "deepseek-chat-t00", 0.2: "deepseek-chat", 0.8: "deepseek-chat-t08"}
# sample spanning outcomes seen at 0.2 (2 detected, 1 reached-not-detected, 1 non-compilable)
SAMPLE = ["fmtlib___fmt@6a13464059", "fmtlib___fmt@611cf0b3c6",
          "fmtlib___fmt@287eaab3b2", "fmtlib___fmt@0cc73ebf79"]
RUNS = 3
DEFECTS4C_ROOT = "/home/truclinh/Thesis/kse2026/defects4c/defectsc_tpl"


def main():
    by_id = {d["defect_id"]: d for d in load_all(Path(DEFECTS4C_ROOT))}
    out_rows = []
    for temp, model in ARMS.items():
        for did in SAMPLE:
            d = by_id[did]
            for run in range(1, RUNS + 1):
                meta = generate(d, model, run, condition="fixed")
                ev = evaluate(Path(meta["test_path"]), d, repo_for(d), repeat=5)
                row = {"temp": temp, "defect": did.split("@")[1][:7], "run": run,
                       "outcome": ev["outcome"], "det": ev["real_bug_detected"],
                       "strict": ev["real_bug_detected_strict"],
                       "n_invalid": ev.get("n_invalid_on_fixed"), "n_tests": ev.get("n_tests")}
                out_rows.append(row)
                print(f"  t={temp}  {row['defect']}  run{run}  {row['outcome']:20s} "
                      f"det={row['det']}", flush=True)
    (STUDY / "data/results/sensitivity_temp.jsonl").write_text(
        "\n".join(json.dumps(r) for r in out_rows))

    print("\n===== TEMPERATURE SENSITIVITY SUMMARY (4 fmt defects × 3 runs) =====")
    for temp in ARMS:
        rs = [r for r in out_rows if r["temp"] == temp]
        oc = Counter(r["outcome"] for r in rs)
        det = sum(1 for r in rs if r["det"])
        inv = sum(r["n_invalid"] or 0 for r in rs); tot = sum(r["n_tests"] or 0 for r in rs)
        print(f"temp={temp}: det(test-level)={det}/{len(rs)} | wrong-oracle={inv}/{tot}"
              f" ({100*inv//max(tot,1)}%) | outcomes={dict(oc)}")


if __name__ == "__main__":
    main()
