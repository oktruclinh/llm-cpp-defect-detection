#!/usr/bin/env python3
"""Adapter: turn the 10 BugsCpp cpp_peglib defects into Defects4C-style records the
existing pipeline (generate + cpp_harness Tier-A) consumes.

Per defect: (1) create a BUGGY commit = fixed-commit + buggy patch, tag it; (2) find the
focal function enclosing the change (brace-peeling). Emits records with commit_after=fixed,
commit_before=buggy, focal_src=peglib.h, func_start/func_end.

Dry-run (default): print the 10 records + focal first-line for sanity.  cwd=study/.
"""
import json, re, subprocess, sys
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
REPO = Path("/home/truclinh/Thesis/kse2026/cpp_peglib_spike")
TAX = STUDY / "benchmarks" / "bugscpp_cpp_peglib"
PROJECT = "yhirose___cpp_peglib"
CTRL = re.compile(r'^\s*(if|for|while|switch|do|else|catch|try)\b')


def git(*a, check=True):
    return subprocess.run(["git", "-C", str(REPO), *a], capture_output=True, text=True, check=check)


def setup_buggy(defect_id, fixed_hash):
    """Idempotent: checkout fixed, apply NNNN-buggy.patch, commit, tag buggy-<id>. Return buggy hash."""
    tag = f"buggy-{defect_id}"
    r = git("rev-parse", "-q", "--verify", f"refs/tags/{tag}", check=False)
    if r.returncode == 0:
        return r.stdout.strip()
    patch = TAX / "patch" / f"{int(defect_id):04d}-buggy.patch"
    git("checkout", "-f", "-q", fixed_hash)
    ap = git("apply", str(patch), check=False)
    if ap.returncode != 0:
        subprocess.run(["patch", "-p1", "-i", str(patch)], cwd=str(REPO), capture_output=True, text=True)
    git("-c", "user.email=s@s", "-c", "user.name=s", "commit", "-q", "-am", f"buggy-{defect_id}")
    h = git("rev-parse", "HEAD").stdout.strip()
    git("tag", "-f", tag, h)
    return h


def change_anchor(defect_id):
    """First changed line in the FIXED file from the buggy patch hunk (@@ -A,B +C,D @@)."""
    txt = (TAX / "patch" / f"{int(defect_id):04d}-buggy.patch").read_text(errors="ignore")
    m = re.search(r'@@ -(\d+),?\d* \+\d+', txt)
    base = int(m.group(1)) if m else 1
    # advance to the first +/- line inside the hunk body
    body = txt[txt.index(m.group(0)) + len(m.group(0)):].splitlines()[1:]
    off = 0
    for ln in body:
        if ln.startswith(('+', '-')) and not ln.startswith(('+++', '---')):
            break
        if ln.startswith(' '):
            off += 1
    return base + off


def enclosing_function(lines, anchor):
    """Brace-peel up from 1-indexed `anchor` through control/bare blocks to the function sig."""
    pos = anchor - 1
    for _ in range(60):
        d, opener = 0, None
        for j in range(pos, -1, -1):
            d += lines[j].count('}') - lines[j].count('{')
            if d < 0:
                opener = j
                break
        if opener is None:
            return (max(1, anchor - 25), anchor + 8)          # fallback window
        s = opener
        while s > 0 and lines[s - 1].strip() and not re.search(r'[;{}]\s*$', lines[s - 1]):
            s -= 1
        sig = " ".join(l.strip() for l in lines[s:opener + 1])
        is_struct = re.search(r'\b(namespace|struct|class|enum|union)\b', sig.split("{")[0])
        if "(" in sig and not CTRL.match(lines[opener]) and not is_struct:
            d2, end = 0, opener
            for k in range(opener, min(len(lines), opener + 500)):
                d2 += lines[k].count("{") - lines[k].count("}")
                if d2 <= 0:
                    end = k
                    break
            return (s + 1, end + 1)
        pos = opener - 1
    return (max(1, anchor - 25), anchor + 8)


def build_defects(setup=True):
    meta = json.loads((TAX / "meta.json").read_text())
    out = []
    for d in meta["defects"]:
        did, fixed = d["id"], d["hash"]
        buggy = setup_buggy(did, fixed) if setup else None
        blob = git("show", f"{fixed}:peglib.h").stdout.splitlines()
        anchor = change_anchor(did)
        fs, fe = enclosing_function(blob, anchor)
        sigl = blob[fs - 1].strip() if 0 <= fs - 1 < len(blob) else ""
        if "(" not in sigl or sigl.startswith(("//", "/*", "*", ")", ">")):   # bad extraction -> window
            fs, fe = max(1, anchor - 20), min(len(blob), anchor + 10)
        out.append({
            "defect_id": f"{PROJECT}@{fixed[:10]}", "project": PROJECT, "focal_src": "peglib.h",
            "commit_after": fixed, "commit_before": buggy, "func_start": fs, "func_end": fe,
            "bug_type": d["description"], "_id": did, "_anchor": anchor,
            "_sig": blob[fs - 1].strip()[:80] if fs - 1 < len(blob) else "?",
        })
    return out


if __name__ == "__main__":
    defs = build_defects(setup="--no-setup" not in sys.argv)
    print(f"=== {len(defs)} cpp_peglib defects ===")
    for d in defs:
        print(f"  d{d['_id']:>2} fixed={d['commit_after'][:8]} buggy={(d['commit_before'] or '?')[:8]} "
              f"focal L{d['func_start']}-{d['func_end']} (anchor {d['_anchor']})  ::  {d['_sig']}")
    (STUDY / "data").mkdir(exist_ok=True)
    (STUDY / "data" / "cpp_peglib_defects.jsonl").write_text(
        "\n".join(json.dumps({k: v for k, v in d.items() if not k.startswith("_")}) for d in defs))
    print("wrote data/cpp_peglib_defects.jsonl")
