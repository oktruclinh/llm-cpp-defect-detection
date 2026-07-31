#!/usr/bin/env python3
"""C++ test harness for Tier-A (header-only) Defects4C projects.

Given an LLM-generated GoogleTest file + a defect record, this can:
  - put the project repo into the FIXED or BUGGY state (mirrors the benchmark:
    buggy = fixed repo with ONLY the focal file checked out at commit_before),
  - compile the test against that state (capturing compile errors -> RQ1 data),
  - run it (gtest exit code: 0 = all pass, non-0 = a test failed).

No Docker. Build = plain g++ -I<include>. gtest is the locally-vendored static lib.
"""
import subprocess
from dataclasses import dataclass, field
from pathlib import Path

STUDY_ROOT = Path(__file__).resolve().parent.parent
GTEST_INC = (STUDY_ROOT / "third_party/googletest/googletest/include").as_posix()
GTEST_LIBS = [(STUDY_ROOT / "third_party/libgtest.a").as_posix(),
              (STUDY_ROOT / "third_party/libgtest_main.a").as_posix()]

# Per-project build recipe (Tier A header-only). include_dirs are relative to repo root.
RECIPES = {
    "fmtlib___fmt":    {"std": "c++17", "defines": ["FMT_HEADER_ONLY"], "include_dirs": ["include"]},
    "CLIUtils___CLI11": {"std": "c++17", "defines": [],                  "include_dirs": ["include"]},
    "skypjack___entt": {"std": "c++17", "defines": [],                  "include_dirs": ["src", "single_include"]},
    # arrow's 2 PUBLIC-header focal defects compile header-only (light util/compute headers,
    # no arrow build) — treated as the header-only path to test public-focal Tier-B cleanly.
    "apache___arrow":  {"std": "c++17", "defines": [],                  "include_dirs": ["cpp/src"]},
    # BugsCpp cross-benchmark: cpp-peglib is single-file header-only (peglib.h at repo root).
    "yhirose___cpp_peglib": {"std": "c++17", "defines": [],             "include_dirs": ["."]},
}


@dataclass
class BuildResult:
    compiled: bool
    stderr: str = ""
    bin_path: str = ""


@dataclass
class RunResult:
    ran: bool
    returncode: int = -1
    stdout: str = ""
    timed_out: bool = False
    tests: dict = field(default_factory=dict)   # {"Suite.Test": passed_bool}


@dataclass
class HarnessOutcome:
    compiled: bool = False
    compile_err: str = ""
    ran: bool = False
    passed: bool = False          # all tests passed (exit 0)
    returncode: int = -1
    stdout: str = ""
    extra: dict = field(default_factory=dict)


def _git(repo: Path, *args, check=True):
    return subprocess.run(["git", "-C", str(repo), *args],
                          capture_output=True, text=True, check=check)


def prepare_version(repo: Path, defect: dict, version: str):
    """version='fixed' -> whole repo at commit_after.
       version='buggy' -> repo at commit_after, focal file at commit_before."""
    _git(repo, "checkout", "-f", "-q", defect["commit_after"])
    if version == "buggy":
        _git(repo, "checkout", "-f", "-q", defect["commit_before"], "--", defect["focal_src"])
    elif version != "fixed":
        raise ValueError(version)


def compile_test(test_cpp: Path, defect: dict, repo: Path, out_bin: Path,
                 coverage: bool = False) -> BuildResult:
    rec = RECIPES.get(defect["project"])
    if rec is None:
        return BuildResult(False, f"no build recipe for {defect['project']}")
    cmd = ["g++", f"-std={rec['std']}"]
    cmd += [f"-D{d}" for d in rec["defines"]]
    for inc in rec["include_dirs"]:
        cmd += ["-I", (repo / inc).as_posix()]
    cmd += ["-I", GTEST_INC]
    if coverage:
        cmd += ["--coverage", "-O0", "-g"]
    cmd += [test_cpp.as_posix(), *GTEST_LIBS, "-pthread", "-o", out_bin.as_posix()]
    p = subprocess.run(cmd, capture_output=True, text=True)
    return BuildResult(p.returncode == 0, p.stderr, out_bin.as_posix() if p.returncode == 0 else "")


def _parse_gtest_json(jpath: Path) -> dict:
    """Map 'Suite.Test' -> passed(bool) from a gtest --gtest_output=json file."""
    import json as _json
    out = {}
    try:
        data = _json.loads(jpath.read_text())
    except Exception:
        return out
    for suite in data.get("testsuites", []):
        sname = suite.get("name", "")
        for t in suite.get("testsuite", []):
            tid = f"{sname}.{t.get('name')}"
            out[tid] = "failures" not in t   # gtest omits 'failures' when the test passed
    return out


def run_test(bin_path: Path, timeout: int = 60, gtest_filter: str = None) -> RunResult:
    import tempfile as _tf
    cmd = [bin_path.as_posix()]
    with _tf.TemporaryDirectory() as td:
        jpath = Path(td) / "gtest.json"
        cmd.append(f"--gtest_output=json:{jpath.as_posix()}")
        if gtest_filter:
            cmd.append(f"--gtest_filter={gtest_filter}")
        try:
            p = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)
        except subprocess.TimeoutExpired:
            return RunResult(False, -1, "", timed_out=True)
        return RunResult(True, p.returncode, p.stdout, tests=_parse_gtest_json(jpath))


def build_and_run(test_cpp: Path, defect: dict, repo: Path, version: str,
                  workdir: Path, coverage: bool = False,
                  gtest_filter: str = None, repeat: int = 1) -> HarnessOutcome:
    """Full cycle for ONE version. Runs the compiled binary `repeat` times to expose
    flakiness (C++ UB / uninit / ordering). extra['pass_counts'] = {test: #passes},
    extra['n_runs'] = effective runs; extra['tests'][t] = stable-pass (passed ALL runs)."""
    # Tier B/C (project-build) projects are not in the header-only RECIPES -> dispatch
    # to the project-build harness (build project + link test). Lazy import = no cycle.
    if defect["project"] not in RECIPES:
        from tierbc_harness import build_and_run_tierbc
        return build_and_run_tierbc(test_cpp, defect, repo, version, workdir,
                                    coverage=coverage, gtest_filter=gtest_filter, repeat=repeat)
    prepare_version(repo, defect, version)
    out_bin = workdir / f"test_{version}"
    b = compile_test(test_cpp, defect, repo, out_bin, coverage=coverage)
    if not b.compiled:
        return HarnessOutcome(compiled=False, compile_err=b.stderr)
    pass_counts, n_eff, last = {}, 0, None
    for _ in range(max(1, repeat)):
        r = run_test(out_bin, gtest_filter=gtest_filter)
        if not r.ran:
            continue
        n_eff += 1
        last = r
        for t, ok in r.tests.items():
            pass_counts[t] = pass_counts.get(t, 0) + (1 if ok else 0)
    if n_eff == 0:
        return HarnessOutcome(compiled=True, ran=False, extra={"timed_out": True})
    stable = {t: (c == n_eff) for t, c in pass_counts.items()}
    return HarnessOutcome(compiled=True, ran=True,
                          passed=(all(stable.values()) if stable else last.returncode == 0),
                          returncode=last.returncode, stdout=last.stdout,
                          extra={"tests": stable, "pass_counts": pass_counts, "n_runs": n_eff})
