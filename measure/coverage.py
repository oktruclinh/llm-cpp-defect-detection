#!/usr/bin/env python3
"""Coverage of the focal change — to separate 'reached-but-not-detected' (weak
oracle) from 'never reached' (coverage gap), the key RQ2/RQ3 distinction.

We compile the test with gcov instrumentation, run it on a given version, then read
the .gcov for the focal source and report execution counts on the changed-hunk lines
(hunk_start..hunk_end). On the BUGGY version: hunk executed + test still passes
=> weak oracle; hunk not executed => the test never even ran the bug.
"""
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "harness"))
from cpp_harness import RECIPES, GTEST_INC, GTEST_LIBS, prepare_version, run_test  # noqa: E402

_GCOV_LINE = re.compile(r"^\s*([#\-0-9]+)\s*:\s*(\d+)\s*:")


def _compile_with_cov(test_cpp: Path, defect: dict, repo: Path, workdir: Path):
    rec = RECIPES[defect["project"]]
    cmd = ["g++", f"-std={rec['std']}", *[f"-D{d}" for d in rec["defines"]]]
    for inc in rec["include_dirs"]:
        cmd += ["-I", (repo / inc).resolve().as_posix()]
    cmd += ["-I", GTEST_INC, "--coverage", "-O0", "-g",
            test_cpp.resolve().as_posix(), *GTEST_LIBS, "-pthread",
            "-o", (workdir / "covbin").as_posix()]
    # run in workdir so .gcno/.gcda land here
    p = subprocess.run(cmd, cwd=workdir, capture_output=True, text=True)
    return p.returncode == 0, p.stderr


def _parse_gcov_for(focal_basename: str, workdir: Path):
    """Return {line_no: exec_count or None}. None = not instrumented ('-')."""
    gcov_file = None
    for g in workdir.glob("*.gcov"):
        # gcov names it like 'format.h.gcov'; match by basename of the focal file
        if g.name.endswith(focal_basename + ".gcov") or g.name == focal_basename + ".gcov":
            gcov_file = g
            break
    if gcov_file is None:
        return None
    cov = {}
    for ln in gcov_file.read_text(errors="replace").splitlines():
        m = _GCOV_LINE.match(ln)
        if not m:
            continue
        cnt, lineno = m.group(1), int(m.group(2))
        cov[lineno] = None if cnt.strip() == "-" else (0 if cnt.strip() == "#####" else int(cnt))
    return cov


def hunk_coverage(test_cpp: Path, defect: dict, repo: Path, version: str, workdir: Path):
    # gcov hunk coverage is implemented for the header-only (Tier A) build only.
    # Tier B/C (project-build) coverage is a B0 box TODO -> report unsupported so the
    # caller falls through to 'missed_unknown_reach' instead of crashing.
    if defect["project"] not in RECIPES:
        return {"compiled": False, "unsupported_tierbc": True}
    prepare_version(repo, defect, version)
    ok, err = _compile_with_cov(test_cpp, defect, repo, workdir)
    if not ok:
        return {"compiled": False, "err": err[-400:]}
    run_test(workdir / "covbin")
    gcda = [g.name for g in workdir.glob("*.gcda")]
    if gcda:
        subprocess.run(["gcov", *gcda], cwd=workdir, capture_output=True, text=True)
    focal_base = Path(defect["focal_src"]).name
    cov = _parse_gcov_for(focal_base, workdir)
    if cov is None:
        return {"compiled": True, "gcov_found": False}
    hs, he = defect.get("hunk_start"), defect.get("hunk_end")
    hunk_lines = {ln: c for ln, c in cov.items() if hs and he and hs <= ln <= he}
    executed = [ln for ln, c in hunk_lines.items() if c and c > 0]
    instrumented = [ln for ln, c in hunk_lines.items() if c is not None]
    return {
        "compiled": True, "gcov_found": True,
        "hunk_range": [hs, he],
        "hunk_lines_executed": executed,
        "hunk_executed": bool(executed),
        "hunk_instrumented_lines": instrumented,
        "focal_lines_total": len(cov),
    }


if __name__ == "__main__":
    import json, tempfile
    jsonl, defect_id, repo, test_cpp, version = sys.argv[1:6]
    recs = [json.loads(l) for l in Path(jsonl).read_text().splitlines() if l.strip()]
    defect = next(r for r in recs if r["defect_id"] == defect_id)
    with tempfile.TemporaryDirectory() as td:
        print(json.dumps(hunk_coverage(Path(test_cpp), defect, Path(repo), version, Path(td)), indent=2))
