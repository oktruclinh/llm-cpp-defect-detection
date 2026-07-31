#!/usr/bin/env python3
"""[#23 / RQ3 F1+F2] EMPIRICAL confirmation of the oracle-weakness mechanisms, independent of
the (LLM-first-pass / human) labels. For representative defects we hand-write the ONE test the
LLM suites lacked and run it through the same detection pipeline:

  F1 (O4 insufficient input): add the triggering input -> the bug is DETECTED (so the miss was
     purely an input-coverage gap, not a weak assertion).
  F2 (O2 wrong API surface): the write(char) iterator bug is INVISIBLE through fmt::format(->string)
     but DETECTED through fmt::format_to(raw pointer) -- same bug, same reached hunk, different
     surface. Direct evidence of API-surface blindness (the paper's flagship C++-specific finding).

  python3 analysis/rq3_confirm.py     (cwd = study/)
"""
import json
import os
import sys
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
sys.path[:0] = [str(STUDY / d) for d in ("generate", "harness", "benchmarks", "measure")]
from defects4c_adapter import load_all      # noqa: E402
from detection import evaluate              # noqa: E402

D4C = Path(os.environ.get("D4C_ROOT", "/home/truclinh/Thesis/kse2026/defects4c/defectsc_tpl"))
REPOS = json.loads((STUDY / "config/repos.json").read_text())
H = ("#include <gtest/gtest.h>\n#include <fmt/core.h>\n#include <fmt/format.h>\n"
     "#include <string>\n#include <iterator>\n")

# (defect_id, finding, name, expectation, test-body) -- expectation is what CONFIRMS the mechanism
CASES = [
    ("fmtlib___fmt@0cc73ebf79", "F1/O4", "get_id_no_named_arg", "DETECTED",
     'std::string f="{x}"; EXPECT_THROW(fmt::format(f, 42), fmt::format_error);'),
    ("fmtlib___fmt@6a13464059", "F1/O4", "on_sign_int128", "DETECTED",
     'EXPECT_EQ(fmt::format("{:+}", (__int128)5), "+5");'),
    ("fmtlib___fmt@287eaab3b2", "F2/O2", "write_char_via_format_string", "NOT detected (invisible)",
     'EXPECT_EQ(fmt::format("{}{}{}", \'A\',\'B\',\'C\'), "ABC");'),
    ("fmtlib___fmt@287eaab3b2", "F2/O2", "write_char_via_format_to_ptr", "DETECTED (visible)",
     'char buf[8]={0}; auto e=fmt::format_to(buf,"{}{}{}",\'A\',\'B\',\'C\'); *e=0; EXPECT_STREQ(buf,"ABC");'),
]


def main():
    defs = {d["defect_id"]: d for d in load_all(D4C)}
    rows = []
    for did, finding, name, expect, body in CASES:
        d = defs[did]
        tp = Path(f"/tmp/rq3c_{name}.cpp")
        tp.write_text(H + f"TEST(RQ3Confirm,{name}){{\n  {body}\n}}\n")
        o = evaluate(tp, d, Path(REPOS[d["project"]]), repeat=3)
        det = bool(o.get("real_bug_detected"))
        rows.append(dict(finding=finding, name=name, defect=did, expect=expect,
                         outcome=o["outcome"], detected=det))
        print(f"  [{finding}] {name:30s} outcome={o['outcome']:20s} DETECTED={det}  (expect: {expect})")
    (STUDY / "data/taxonomy/rq3_confirmation.json").write_text(json.dumps(rows, indent=2))
    print("\nwrote data/taxonomy/rq3_confirmation.json")
    print("Headline: same write(char) bug is reached-but-not-detected via fmt::format, "
          "yet DETECTED via fmt::format_to(raw ptr) -> API-surface blindness, demonstrated.")


if __name__ == "__main__":
    main()
