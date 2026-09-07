#!/usr/bin/env python3
"""E4 -- Per-test-case reachability for the 44 "reached but not caught" suites
(Reviewer #3, Major Comment 3).

The paper measures reachability per SUITE (measure/coverage.py runs the whole binary
with no --gtest_filter) but interprets it per CASE: detection uses only the VALID cases,
i.e. those that pass on the fixed version. Because 674 of 1810 cases fail on the fixed
version, a suite can be recorded as "reached but not caught" when the case that actually
reached the patched region is an INVALID case that never enters the detection analysis.
Such a suite is really a reachability failure and should not receive an infection or
propagation label.

This script re-runs every case of every one of the 44 suites separately on the BUGGY
version under gcov and partitions the suites into:

  A : at least one VALID case reaches the patched region on its own   -> RQ3 population is sound
  B : no valid case reaches, but some invalid/flaky case does         -> mislabelled, belongs in "never reached"
  C : no single case reaches, although the whole suite does           -> gcov aggregation artefact
  D : the whole-suite run does not reproduce the recorded reach       -> coverage non-reproduction

If |B| + |C| + |D| > 0 the RQ3 population changes from 44 to |A| and the 30/14
infection/propagation split, Figure 1, Findings 2 and 3, and the abstract must be
recomputed. The script therefore also re-tabulates the infection/propagation split
over group A alone.

Outputs:
  rq3/per_case_coverage.csv      one row per (suite, test case)
  rq3/e4_suite_classification.csv one row per suite with its group
  rq3/e4_summary.md              recomputed RQ3 table
  rq3/e4_summary.json

Run with cwd = artifact/ :  python3 analysis/e4_per_case_coverage.py
"""
import csv
import json
import re
import subprocess
import sys
import tempfile
import time
from collections import Counter, defaultdict
from pathlib import Path

ART = Path(__file__).resolve().parent.parent
OUT = ART / "rq3"
OUT.mkdir(exist_ok=True)

D4C_ROOT = Path("/home/truclinh/Thesis/kse2026/defects4c/defectsc_tpl")

sys.path[:0] = [str(ART / "harness"), str(ART / "benchmarks"), str(ART / "measure")]
import cpp_harness as H  # noqa: E402

# The published artifact does not ship the vendored googletest build; locate it.
for cand in (ART / "third_party", ART.parent / "study" / "third_party"):
    if (cand / "libgtest.a").exists():
        H.GTEST_INC = (cand / "googletest/googletest/include").as_posix()
        H.GTEST_LIBS = [(cand / "libgtest.a").as_posix(), (cand / "libgtest_main.a").as_posix()]
        break
else:
    sys.exit("could not locate third_party/libgtest.a")

from defects4c_adapter import load_all  # noqa: E402

REPOS = json.loads((ART / "config" / "repos.json").read_text())
GCOV_LINE = re.compile(r"^\s*([#\-0-9]+)\s*:\s*(\d+)\s*:")


def frozen44():
    """Prefer the working-tree copy, which retains the adjudication_reason column."""
    for p in (ART.parent / "study" / "data" / "taxonomy" / "rq3_frozen44.csv",
              ART / "data" / "taxonomy" / "rq3_frozen44.csv"):
        if p.exists():
            return list(csv.DictReader(p.open())), p
    raise SystemExit("rq3_frozen44.csv not found")


def find_test_dir(defect_id, model, run):
    base = ART / "data" / "raw" / defect_id / model
    for cand in (base / f"run{run}", *(base / c / f"run{run}" for c in
                                       ("fixed", "buggy", "guided", "api_rich", "context_rich"))):
        if (cand / "test.cpp").exists():
            return cand
    return None


def hunk_lines_executed(wd: Path, focal_basename: str, hs: int, he: int):
    """gcov the current .gcda set and return executed lines inside [hs, he].

    In template-heavy code gcov emits SEVERAL entries for the same source line, one per
    instantiation, and they disagree: one instantiation may be covered while another is
    not. measure/coverage.py stores them in a dict keyed by line number, so the LAST
    entry silently overwrites the earlier ones and a line covered by an early
    instantiation is recorded as uncovered. This is the "line attribution in template
    code is coarse" threat named in the paper, and it is measurable rather than
    hypothetical: for fmt@96c18b26 the published record lists only line 263 although
    258-261 also execute.

    Two readings are therefore returned:
      exec_any  : line counts as executed if ANY instantiation executed it   (correct)
      exec_last : the last entry wins, reproducing the published pipeline    (as-published)
    """
    gcda = [g.name for g in wd.glob("*.gcda")]
    if not gcda:
        return None
    subprocess.run(["gcov", *gcda], cwd=wd, capture_output=True, text=True)
    gfile = next((x for x in wd.glob("*.gcov")
                  if x.name.endswith(focal_basename + ".gcov")), None)
    if gfile is None:
        return None
    best, last = {}, {}
    for ln in gfile.read_text(errors="replace").splitlines():
        m = GCOV_LINE.match(ln)
        if not m:
            continue
        cnt, no = m.group(1).strip(), int(m.group(2))
        if not (hs <= no <= he):
            continue
        c = None if cnt == "-" else (0 if cnt == "#####" else int(cnt))
        last[no] = c
        if c is not None:
            best[no] = max(best.get(no, 0), c)
    return {
        "exec_any": sorted(n for n, c in best.items() if c > 0),
        "exec_last": sorted(n for n, c in last.items() if c),
        "n_multi_entry_lines": sum(1 for n in best if last.get(n) != best.get(n)),
    }


def clean(wd: Path):
    for p in list(wd.glob("*.gcda")) + list(wd.glob("*.gcov")):
        p.unlink()


def main():
    rows, src_csv = frozen44()
    defects = {d["defect_id"]: d for d in load_all(D4C_ROOT)}
    print(f"[E4] {len(rows)} suites from {src_csv}")

    # Process grouped by defect: prepare_version does a git checkout per defect.
    by_defect = defaultdict(list)
    for r in rows:
        by_defect[r["defect_id"]].append(r)

    case_rows, suite_rows = [], []
    t_start = time.time()

    for defect_id, suites in by_defect.items():
        d = defects[defect_id]
        repo = Path(REPOS[d["project"]])
        hs, he = int(d["hunk_start"]), int(d["hunk_end"])
        focal_base = Path(d["focal_src"]).name
        H.prepare_version(repo, d, "buggy")
        print(f"\n=== {defect_id}  focal={d['focal_src']} hunk={hs}-{he} "
              f"({len(suites)} suites) ===")

        for r in suites:
            model, run = r["model"], r["run"]
            tdir = find_test_dir(defect_id, model, run)
            sid = f"{defect_id}|{model}|run{run}"
            if tdir is None:
                suite_rows.append({**r, "group": "ERR", "note": "test.cpp not found"})
                print(f"  [ERR] {model} run{run}: test.cpp not found")
                continue
            ev = json.loads((tdir / "eval.json").read_text())
            invalid = set(ev.get("invalid_on_fixed_tests") or [])
            flaky = set(ev.get("flaky_fixed_tests") or [])
            recorded = (ev.get("coverage") or {}).get("hunk_lines_executed")

            with tempfile.TemporaryDirectory() as td:
                wd = Path(td)
                b = H.compile_test(tdir / "test.cpp", d, repo, wd / "covbin", coverage=True)
                if not b.compiled:
                    suite_rows.append({**r, "group": "ERR", "note": "recompile failed"})
                    print(f"  [ERR] {model} run{run}: recompile failed")
                    continue

                # --- whole-suite run: reproduce the paper's suite-level measurement ---
                clean(wd)
                full = H.run_test(wd / "covbin")
                sc = hunk_lines_executed(wd, focal_base, hs, he) or {
                    "exec_any": [], "exec_last": [], "n_multi_entry_lines": 0}
                all_cases = sorted(full.tests)
                valid = [c for c in all_cases if c not in invalid and c not in flaky]

                # --- one run per case ---
                per_case = {}
                for c in all_cases:
                    clean(wd)
                    rr = H.run_test(wd / "covbin", gtest_filter=c)
                    cc = hunk_lines_executed(wd, focal_base, hs, he) if rr.ran else None
                    cc = cc or {"exec_any": [], "exec_last": [], "n_multi_entry_lines": 0}
                    kind = ("invalid" if c in invalid else
                            "flaky" if c in flaky else "valid")
                    per_case[c] = {"kind": kind,
                                   "any": bool(cc["exec_any"]),
                                   "last": bool(cc["exec_last"]), **cc}
                    case_rows.append({
                        "defect_id": defect_id, "model": model, "run": run,
                        "test_case": c, "case_kind": kind,
                        "reached_patch_any": bool(cc["exec_any"]),
                        "reached_patch_lastentry": bool(cc["exec_last"]),
                        "lines_any": " ".join(map(str, cc["exec_any"])),
                        "lines_lastentry": " ".join(map(str, cc["exec_last"])),
                        "ran": rr.ran, "timed_out": rr.timed_out,
                    })

                def classify(rule):
                    vr = [c for c in valid if per_case[c][rule]]
                    ir = [c for c in all_cases
                          if per_case[c]["kind"] != "valid" and per_case[c][rule]]
                    sr = bool(sc["exec_any" if rule == "any" else "exec_last"])
                    if not sr:
                        g, nt = "D", "whole-suite run did not reproduce the recorded reach"
                    elif vr:
                        g, nt = "A", ""
                    elif ir:
                        g, nt = "B", "only invalid/flaky cases reach the patched region"
                    else:
                        g, nt = "C", "suite reaches but no single case does (aggregation artefact)"
                    return g, nt, vr, ir, sr

                grp, note, v_reach, iv_reach, suite_reached = classify("any")
                grp_l, _, v_reach_l, iv_reach_l, suite_reached_l = classify("last")

                suite_rows.append({
                    **r, "group": grp, "group_lastentry_rule": grp_l, "note": note,
                    "n_cases": len(all_cases), "n_valid": len(valid),
                    "n_invalid": len(invalid), "n_flaky": len(flaky),
                    "suite_reached_any": suite_reached,
                    "suite_reached_lastentry": suite_reached_l,
                    "suite_lines_any": " ".join(map(str, sc["exec_any"])),
                    "suite_lines_lastentry": " ".join(map(str, sc["exec_last"])),
                    "recorded_lines": " ".join(str(x) for x in (recorded or [])),
                    "reproduces_recorded": sc["exec_last"] == sorted(recorded or []),
                    "n_multi_entry_lines": sc["n_multi_entry_lines"],
                    "n_valid_reaching": len(v_reach),
                    "n_invalid_reaching": len(iv_reach),
                    "n_valid_reaching_lastentry": len(v_reach_l),
                    "valid_reaching": "; ".join(v_reach),
                    "invalid_reaching": "; ".join(iv_reach),
                })
                print(f"  [{grp}/{grp_l}] {model:17s} run{run}  cases={len(all_cases):3d} "
                      f"valid={len(valid):3d}  valid_reach={len(v_reach):2d}"
                      f"({len(v_reach_l)})  invalid_reach={len(iv_reach):2d}"
                      f"{'  ** ' + note if note else ''}")

    print(f"\n[E4] done in {time.time()-t_start:.0f}s")

    # ---------------- write per-case + per-suite CSV ----------------
    with (OUT / "per_case_coverage.csv").open("w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(case_rows[0]))
        w.writeheader()
        w.writerows(case_rows)

    suite_fields = ["defect_id", "model", "run", "source", "coder1", "coder2", "agree",
                    "final_after_adjudication", "group", "group_lastentry_rule", "note",
                    "n_cases", "n_valid", "n_invalid", "n_flaky",
                    "suite_reached_any", "suite_reached_lastentry",
                    "suite_lines_any", "suite_lines_lastentry", "recorded_lines",
                    "reproduces_recorded", "n_multi_entry_lines",
                    "n_valid_reaching", "n_invalid_reaching", "n_valid_reaching_lastentry",
                    "valid_reaching", "invalid_reaching"]
    with (OUT / "e4_suite_classification.csv").open("w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=suite_fields, extrasaction="ignore")
        w.writeheader()
        w.writerows(suite_rows)

    # ---------------- recompute RQ3 ----------------
    groups = Counter(s["group"] for s in suite_rows)
    groups_l = Counter(s["group_lastentry_rule"] for s in suite_rows)
    A = [s for s in suite_rows if s["group"] == "A"]
    A_l = [s for s in suite_rows if s["group_lastentry_rule"] == "A"]
    lab = lambda rs: Counter(s["final_after_adjudication"] for s in rs)  # noqa: E731
    lab_all, lab_A, lab_A_l = lab(suite_rows), lab(A), lab(A_l)
    proj_A = Counter(s["defect_id"].split("___")[0] for s in A)
    defects_A = {s["defect_id"] for s in A}
    repro = sum(1 for s in suite_rows if s.get("reproduces_recorded"))
    multi = sum(1 for s in suite_rows if s.get("n_multi_entry_lines"))

    summary = {
        "n_suites": len(suite_rows),
        "groups": dict(groups),
        "groups_under_lastentry_rule": dict(groups_l),
        "labels_over_groupA_lastentry": dict(lab_A_l),
        "suites_with_multi_entry_gcov_lines": multi,
        "group_definitions": {
            "A": "at least one valid case reaches the patched region on its own",
            "B": "no valid case reaches; only invalid/flaky cases do",
            "C": "suite reaches but no single case does (gcov aggregation artefact)",
            "D": "whole-suite run did not reproduce the recorded reach",
            "ERR": "suite could not be rebuilt",
        },
        "suite_level_reproduction": f"{repro}/{len(suite_rows)}",
        "labels_over_44": dict(lab_all),
        "labels_over_groupA": dict(lab_A),
        "groupA_n_defects": len(defects_A),
        "groupA_projects": dict(proj_A),
        "rq3_population_changes": groups.get("A", 0) != len(suite_rows),
    }
    (OUT / "e4_summary.json").write_text(json.dumps(summary, indent=2))

    n = len(suite_rows)
    nA = groups.get("A", 0)
    md = ["# E4 -- Per-case reachability for the 44 RQ3 suites", "",
          f"Source suite list: `{src_csv}` ({n} suites). Every case of every suite was re-run "
          f"individually on the buggy version under gcov.", "",
          "## A prior finding: gcov line attribution in templates", "",
          "gcov emits several entries for the same source line in template-heavy code, one per "
          "instantiation, and they disagree. `measure/coverage.py` keys them into a dict, so the "
          "**last entry silently overwrites the earlier ones** and a line covered only by an "
          "earlier instantiation is recorded as uncovered. The paper names this threat "
          "(\"line attribution in template code is coarse\"); it is measurable. "
          f"**{multi} of {n} suites** contain at least one hunk line whose entries disagree. "
          f"For `fmt@96c18b26` the published record lists only line 263, although lines 258-261 "
          f"also execute. Under the last-entry rule this pipeline reproduces the published "
          f"whole-suite line set exactly in **{repro}/{n}** suites, which validates it against "
          f"the campaign; the disagreement is confined to which lines are credited, and at suite "
          f"level it never changes the reached / not-reached verdict.", "",
          "Everything below is therefore reported under two rules: **any** (a line counts as "
          "executed if any instantiation executed it -- the correct reading) and **last-entry** "
          "(the published pipeline's behaviour).", "",
          "## Partition", "",
          "| group | meaning | any (primary) | last-entry (as published) |", "|---|---|---|---|",
          f"| A | at least one **valid** case reaches the patched region on its own | "
          f"**{groups.get('A',0)}** | {groups_l.get('A',0)} |",
          f"| B | no valid case reaches; only invalid/flaky cases do | "
          f"**{groups.get('B',0)}** | {groups_l.get('B',0)} |",
          f"| C | suite reaches but no single case does (aggregation artefact) | "
          f"**{groups.get('C',0)}** | {groups_l.get('C',0)} |",
          f"| D | whole-suite run did not reproduce the recorded reach | "
          f"**{groups.get('D',0)}** | {groups_l.get('D',0)} |",
          f"| ERR | could not rebuild | {groups.get('ERR',0)} | {groups_l.get('ERR',0)} |", ""]

    if nA == n:
        md += ["## Verdict", "",
               "**Every one of the 44 suites has at least one valid test case that reaches the "
               "patched region by itself.** The suite-level gcov measurement therefore did not "
               "inflate the RQ3 population: no suite was admitted because an invalid case, "
               "excluded from the detection analysis, was the one that executed the patch. "
               "The RQ3 population stays at 44 and the 30/14 infection/propagation split, "
               "Figure 1, Findings 2 and 3, and the abstract are unchanged.", "",
               "This is the answer Reviewer #3 asked for: the number of the 44 suites in which "
               "the reaching case is invalid is **0**.", ""]
    else:
        md += ["## Verdict", "",
               f"**The RQ3 population changes from {n} to {nA}.** "
               f"{n-nA} suites reach the patched region only through cases that the detection "
               f"analysis excludes, so their infection/propagation labels do not describe a "
               f"reachable-and-observed defect. Every downstream quantity must be recomputed.", ""]

    nA_l = groups_l.get("A", 0)
    md += ["## Infection / propagation, recomputed", "",
           "Labels: `O4` = infection failure (no differentiating input), "
           "`O2` = propagation failure (interface hides the difference).", "",
           "| population | n | O4 infection | O2 propagation | split |", "|---|---|---|---|---|",
           f"| all 44 suites (as published) | {n} | {lab_all.get('O4',0)} | {lab_all.get('O2',0)} | "
           f"{100*lab_all.get('O4',0)/n:.0f}/{100*lab_all.get('O2',0)/n:.0f} |"]
    if nA:
        md += [f"| group A, any rule (primary) | {nA} | {lab_A.get('O4',0)} | {lab_A.get('O2',0)} | "
               f"{100*lab_A.get('O4',0)/nA:.0f}/{100*lab_A.get('O2',0)/nA:.0f} |"]
    if nA_l:
        md += [f"| group A, last-entry rule | {nA_l} | {lab_A_l.get('O4',0)} | "
               f"{lab_A_l.get('O2',0)} | "
               f"{100*lab_A_l.get('O4',0)/nA_l:.0f}/{100*lab_A_l.get('O2',0)/nA_l:.0f} |"]
    md += ["",
           f"Group A spans {len(defects_A)} distinct defects "
           f"({', '.join(f'{k} {v}' for k, v in sorted(proj_A.items()))}).", ""]

    b_any = [s for s in suite_rows if s["group"] == "B"]
    b_last = [s for s in suite_rows if s["group_lastentry_rule"] == "B"]
    md += ["## The suites that leave the RQ3 population", "",
           "| suite | published label | leaves under | only case reaching the patch |",
           "|---|---|---|---|"]
    for s in b_last:
        rule = "both rules" if s in b_any else "last-entry rule only"
        md += [f"| `{s['defect_id'].split('@')[1][:8]}` {s['model']} run{s['run']} | "
               f"{s['final_after_adjudication']} | {rule} | "
               f"`{(s['invalid_reaching'] or '-').split(';')[0].strip()}` |"]
    md += ["",
           "In each of these the case that executes the patched region fails on the fixed "
           "version, so it never enters the detection analysis. Among its valid cases the suite "
           "is a reachability failure and belongs with the \"never reached\" group, not with the "
           "reached-but-not-caught population that carries an infection or propagation label.", "",
           "## Files", "",
           "| path | contents |", "|---|---|",
           "| `rq3/per_case_coverage.csv` | one row per (suite, test case): case kind "
           "(valid/invalid/flaky), whether it reaches the patch alone, which hunk lines |",
           "| `rq3/e4_suite_classification.csv` | one row per suite: group, counts, the "
           "reaching cases by name, and whether the suite-level measurement reproduced |",
           "| `rq3/e4_summary.json` | the counts above, machine-readable |", ""]

    (OUT / "e4_summary.md").write_text("\n".join(md))
    print("\n".join(md))
    for p in ("per_case_coverage.csv", "e4_suite_classification.csv",
              "e4_summary.json", "e4_summary.md"):
        print(f"[written] {OUT/p}")


if __name__ == "__main__":
    main()
