#!/usr/bin/env python3
"""Tier B/C harness: build the project, then compile+link the generated gtest against
it using flags extracted from the project's compile_commands.json. Returns the SAME
HarnessOutcome shape as the Tier-A path, so measure/detection.py is unchanged.

⚠️ UNVERIFIED ON A BUILD MACHINE — the build/link specifics live in recipes_tierbc.py
and must be confirmed on the B0 box (spike one project end-to-end first).

Flow per version (caller = detection.py, sequential, same persistent repo):
  fixed: checkout commit_after  -> full build (build_cmd)        -> compile test -> run
  buggy: checkout focal@before  -> incremental rebuild (rebuild) -> recompile/relink -> run
"""
import json
import re
import shlex
import subprocess
from pathlib import Path

from cpp_harness import (HarnessOutcome, run_test, prepare_version,
                         GTEST_INC, GTEST_LIBS, STUDY_ROOT)
from recipes_tierbc import RECIPES_BC

# Tier B/C uses Defects4C's toolchain: clang-16 + libc++ (apt-installed, no LLVM build).
# ABI consistency is mandatory: the project lib, gtest, and the test must all use libc++.
CXX_BC = "clang++-16"
STDLIB_BC = "-stdlib=libc++"
# libc++-built gtest (rebuilt on the box; falls back to the default libstdc++ gtest).
_g1 = STUDY_ROOT / "third_party/libgtest_cxx.a"
_g2 = STUDY_ROOT / "third_party/libgtest_main_cxx.a"
GTEST_LIBS_BC = [_g1.as_posix(), _g2.as_posix()] if _g1.exists() else GTEST_LIBS


def _parse_focal_flags(compile_db: Path, focal_src: str):
    """Return the -I/-isystem/-D/-std flags of the focal translation unit (or, if the
    focal file is a header, the closest TU in its directory)."""
    try:
        db = json.loads(compile_db.read_text())
    except Exception:
        return None
    base = Path(focal_src).name
    focal_dir = str(Path(focal_src).parent)
    entry = None
    for e in db:
        if e.get("file", "").endswith(base):
            entry = e
            break
    if entry is None:  # focal is a header: pick any TU in the same directory
        for e in db:
            if focal_dir and focal_dir in e.get("file", ""):
                entry = e
                break
    if entry is None:
        return None
    args = entry.get("arguments") or shlex.split(entry.get("command", ""))
    # keep include/define context; DROP the project's -std (we force c++17 for gtest).
    flags, i = [], 0
    while i < len(args):
        a = args[i]
        if a.startswith(("-I", "-D", "-isystem")):
            flags.append(a)
            if a in ("-I", "-isystem") and i + 1 < len(args):  # separated form
                flags.append(args[i + 1]); i += 1
        i += 1
    return flags


def _build(repo: Path, cmd: str):
    if not cmd:
        return True, ""
    p = subprocess.run(["bash", "-lc", cmd], cwd=str(repo), capture_output=True, text=True)
    return p.returncode == 0, (p.stderr or "")[-1500:]


def _ensure_built(repo: Path, defect: dict, version: str, rec):
    """Build the project at (defect, version) ONCE and cache via build/.d4c_state, so all
    model×run test compilations of the same defect reuse it. Switching DEFECT -> clean
    full build (avoid stale objects across commits); same defect fixed->buggy -> incremental."""
    import shutil
    bdir = repo / "build"
    marker = bdir / ".d4c_state"
    cur = marker.read_text().strip() if marker.exists() else ""
    want = f"{defect['defect_id']}:{version}"
    if cur == want:
        return True, ""                                  # already built at this exact state
    same_defect = cur.startswith(defect["defect_id"] + ":")
    prepare_version(repo, defect, version)               # checkout fixed, or focal@buggy
    if not same_defect and bdir.exists():
        shutil.rmtree(bdir, ignore_errors=True)          # different defect -> clean build
    cmd = rec.build_cmd if (version == "fixed" or not same_defect) else (rec.rebuild_cmd or rec.build_cmd)
    ok, err = _build(repo, cmd)
    if ok:
        bdir.mkdir(parents=True, exist_ok=True)
        marker.write_text(want)
    return ok, err


def _compile_test(test_cpp: Path, defect: dict, repo: Path, rec, out_bin: Path,
                  coverage: bool):
    db = repo / rec.compile_db
    focal_flags = _parse_focal_flags(db, defect["focal_src"]) or []
    cmd = [CXX_BC, STDLIB_BC, f"-std={rec.std}", *focal_flags]
    cmd += [f"-I{(repo / inc).as_posix()}" for inc in rec.extra_includes]
    cmd += ["-I", GTEST_INC]
    if coverage:
        cmd += ["--coverage", "-O0", "-g"]
    cmd += [test_cpp.as_posix(), *GTEST_LIBS_BC]
    cmd += [(repo / lib).as_posix() if not lib.startswith("-") else lib for lib in rec.link_libs]
    cmd += ["-pthread", "-o", out_bin.as_posix()]
    p = subprocess.run(cmd, capture_output=True, text=True)
    return p.returncode == 0, p.stderr


def build_and_run_tierbc(test_cpp: Path, defect: dict, repo: Path, version: str,
                         workdir: Path, coverage: bool = False,
                         gtest_filter: str = None, repeat: int = 1) -> HarnessOutcome:
    rec = RECIPES_BC.get(defect["project"])
    if rec is None:
        return HarnessOutcome(compiled=False, compile_err=f"no Tier-B/C recipe for {defect['project']}")
    built, berr = _ensure_built(repo, defect, version, rec)   # cached per (defect, version)
    if not built:
        return HarnessOutcome(compiled=False, compile_err="PROJECT BUILD FAILED:\n" + berr)
    out_bin = workdir / f"test_{version}"
    ok, err = _compile_test(test_cpp, defect, repo, rec, out_bin, coverage)
    if not ok:
        return HarnessOutcome(compiled=False, compile_err=err)
    pass_counts, n_eff, last = {}, 0, None
    for _ in range(max(1, repeat)):
        r = run_test(out_bin, gtest_filter=gtest_filter)
        if not r.ran:
            continue
        n_eff += 1; last = r
        for t, okk in r.tests.items():
            pass_counts[t] = pass_counts.get(t, 0) + (1 if okk else 0)
    if n_eff == 0:
        return HarnessOutcome(compiled=True, ran=False, extra={"timed_out": True})
    stable = {t: (c == n_eff) for t, c in pass_counts.items()}
    return HarnessOutcome(compiled=True, ran=True,
                          passed=(all(stable.values()) if stable else last.returncode == 0),
                          returncode=last.returncode, stdout=last.stdout,
                          extra={"tests": stable, "pass_counts": pass_counts, "n_runs": n_eff})
