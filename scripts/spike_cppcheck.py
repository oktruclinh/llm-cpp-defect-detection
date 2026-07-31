#!/usr/bin/env python3
"""Quick box-side validation: compile+link+run a hand test against cppcheck's libc++ lib
(no full rebuild; uses the existing build/). Run with cwd=study/."""
import sys
import tempfile
from pathlib import Path

sys.path[:0] = ["generate", "harness", "benchmarks", "measure"]
from tierbc_harness import _compile_test, run_test          # noqa: E402
from recipes_tierbc import RECIPES_BC                         # noqa: E402
from defects4c_adapter import load_all                        # noqa: E402

d = [x for x in load_all(Path("/home/azureuser/d4c")) if x["project"] == "danmar___cppcheck"][0]
rec = RECIPES_BC["danmar___cppcheck"]
repo = Path("/home/azureuser/cppcheck")
test = '''#include <gtest/gtest.h>
#include "settings.h"
TEST(Lnk, S) { Settings s; (void)s; EXPECT_TRUE(true); }
'''
with tempfile.TemporaryDirectory() as td:
    tf = Path(td) / "t.cpp"
    tf.write_text(test)
    ok, err = _compile_test(tf, d, repo, rec, Path(td) / "b", False)
    print("compiled+linked:", ok)
    if not ok:
        print("ERR:", err[:600])
    else:
        r = run_test(Path(td) / "b")
        print("ran:", r.ran, "| tests:", r.tests)
