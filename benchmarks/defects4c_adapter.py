#!/usr/bin/env python3
"""Defects4C adapter — normalize the benchmark's per-project bug lists into flat
defect records for the KSE2026 LLM-C++-test study.

Reads <defects4c_root>/projects_v1/<proj>/bugs_list_new.json (+ project.json) and
emits one record per defect. Optionally slices the focal-function source out of a
checked-out project repo (func_start..func_end at commit_after).

No Defects4C runtime / Docker needed for listing — this only reads the shipped JSON.
"""
import argparse
import json
import sys
from pathlib import Path

# Feasibility tiers (see KSE2026_brief_v3_reframe.md §4). Build-cost driven.
TIERS = {
    # A: header-only, compile with g++ -I include, NO Docker, runs on a 15GB laptop.
    "fmtlib___fmt": "A", "CLIUtils___CLI11": "A", "skypjack___entt": "A",
    # B: real library build, needs >15GB or rented box.
    "apache___arrow": "B", "KhronosGroup___SPIRV-Tools": "B",
    "facebook___rocksdb": "B", "nanomsg___nng": "B",
    # C: monolithic app / C project, moderate build (Docker, rented box).
    "danmar___cppcheck": "C", "CESNET___libyang": "C", "uncrustify___uncrustify": "C",
    # excluded: llvm = 143 defects, hours to build, OOM; singletons amortize poorly.
    "llvm___llvm-project": "X", "awslabs___aws-c-common": "X",
    "DynamoRIO___dynamorio": "X", "libevent___libevent": "X",
    "SOCI___soci": "X", "zeromq___libzmq": "X",
}


def normalize(project: str, raw: dict, idx: int) -> dict:
    files = raw.get("files", {}) or {}
    loc = files.get("src0_location", {}) or {}
    typ = raw.get("type", {}) or {}
    cc = raw.get("c_compile", {}) or {}
    srcs = files.get("src") or []
    return {
        "defect_id": f"{project}@{raw['commit_after'][:10]}",
        "project": project,
        "tier": TIERS.get(project, "?"),
        "commit_before": raw.get("commit_before"),   # buggy
        "commit_after": raw.get("commit_after"),     # fixed (code-under-test)
        "commit_date": raw.get("commit_date"),
        "focal_src": srcs[0] if srcs else None,
        "all_src": srcs,
        "trigger_test_file": (files.get("test") or [None])[0],
        "func_start": loc.get("func_start"),
        "func_end": loc.get("func_end"),
        "hunk_start": loc.get("hunk_start"),
        "hunk_end": loc.get("hunk_end"),
        "func_is_single": loc.get("func_is_single"),
        "ctest_filter": (cc.get("test_flags") or raw.get("unittest", {}).get("name") or [None])[0],
        "bug_type": typ.get("name"),
        "bug_type_id": typ.get("id"),
        "kind": typ.get("type", "bug"),
        "status_manual": raw.get("status_manual"),
        "_index": idx,
    }


def load_all(root: Path, subdir: str = "projects_v1"):
    base = root / subdir
    if not base.is_dir():
        sys.exit(f"not found: {base}")
    out = []
    for proj_dir in sorted(base.iterdir()):
        bl = proj_dir / "bugs_list_new.json"
        if not bl.is_file():
            continue
        data = json.loads(bl.read_text())
        for i, raw in enumerate(data):
            if not raw.get("commit_after") or not raw.get("commit_before"):
                continue
            out.append(normalize(proj_dir.name, raw, i))
    return out


def extract_focal(repo: Path, commit: str, rel_src: str, start: int, end: int,
                  max_extend: int = 80):
    """Slice the focal function(s) from rel_src at `commit` (reads the blob via git,
    no checkout needed). func_start/func_end in the metadata can land mid-function, so
    we EXTEND the end forward (brace-balanced) until every brace opened since func_start
    is closed — guaranteeing complete function bodies. Returns source text or None."""
    import subprocess
    try:
        blob = subprocess.run(
            ["git", "-C", str(repo), "show", f"{commit}:{rel_src}"],
            capture_output=True, text=True, check=True,
        ).stdout
    except subprocess.CalledProcessError:
        return None
    lines = blob.splitlines()
    if not start:
        return None
    end = end or start
    depth, seen_open, last = 0, False, start - 1
    for i in range(start - 1, min(len(lines), end - 1 + max_extend)):
        depth += lines[i].count("{") - lines[i].count("}")
        if "{" in lines[i]:
            seen_open = True
        last = i
        if i >= end - 1 and seen_open and depth <= 0:
            break  # past the metadata end AND all opened braces closed
    return "\n".join(lines[start - 1:last + 1])


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", default="/home/truclinh/Thesis/kse2026/defects4c/defectsc_tpl")
    ap.add_argument("--tier", help="filter: A | B | C (comma-separated)")
    ap.add_argument("--subdir", default="projects_v1", help="projects_v1 (functional) | projects (vuln/CVE)")
    ap.add_argument("--summary", action="store_true", help="print per-tier/per-project counts")
    ap.add_argument("--jsonl", help="write records to this .jsonl path")
    ap.add_argument("--extract-demo", metavar="REPO", help="extract focal fn of first match from this repo path")
    args = ap.parse_args()

    recs = load_all(Path(args.root), args.subdir)
    if args.tier:
        keep = set(args.tier.upper().split(","))
        recs = [r for r in recs if r["tier"] in keep]

    if args.summary:
        from collections import Counter
        bytier, byproj = Counter(), Counter()
        for r in recs:
            bytier[r["tier"]] += 1
            byproj[(r["tier"], r["project"])] += 1
        print(f"total records: {len(recs)}")
        print("by tier:", dict(sorted(bytier.items())))
        for (t, p), n in sorted(byproj.items()):
            print(f"  [{t}] {p:30s} {n}")

    if args.jsonl:
        Path(args.jsonl).write_text("\n".join(json.dumps(r) for r in recs))
        print(f"wrote {len(recs)} records -> {args.jsonl}")

    if args.extract_demo:
        r = recs[0]
        src = extract_focal(Path(args.extract_demo), r["commit_after"],
                            r["focal_src"], r["func_start"], r["func_end"])
        print(f"\n--- focal fn of {r['defect_id']} ({r['focal_src']} L{r['func_start']}-{r['func_end']}) ---")
        print(src if src else "(extraction failed — repo not checked out / wrong path)")


if __name__ == "__main__":
    main()
