#!/usr/bin/env python3
"""[#23 task1+2] Finalize RQ1:
 (2) for the 6 fence cells, strip the markdown fence + re-compile -> overwrite their error file
     with the REAL underlying error (what robust extract_code() would have surfaced).
 (1) FINAL taxonomy = the human audit (rq1_audit_review.csv), migrated to the v2 codebook:
     macro cells -> 12, fence cells -> their underlying category, else the audited proposed_num.
     Then measure how well the deterministic v2 classifier reproduces that audit.
Writes data/taxonomy/rq1_final.csv. cwd = study/.
"""
import csv
import json
import os
import re
import sys
import tempfile
from collections import Counter
from pathlib import Path

STUDY = Path(__file__).resolve().parent.parent
sys.path[:0] = [str(STUDY / d) for d in ("generate", "harness", "benchmarks", "measure", "analysis")]
from defects4c_adapter import load_all                    # noqa: E402
from cpp_harness import compile_test, prepare_version     # noqa: E402
from taxonomy_rq1 import categorize, first_error          # noqa: E402
from rq1_classify import code_of                          # noqa: E402

TAX = STUDY / "data/taxonomy"
D4C = Path(os.environ.get("D4C_ROOT", "/home/truclinh/Thesis/kse2026/defects4c/defectsc_tpl"))
REPOS = json.loads((STUDY / "config/repos.json").read_text())
FENCE_LINE = re.compile(r"^\s*```+\s*[a-zA-Z+]*\s*$")
CATS = {1: "UDL/namespace", 2: "static_assert/trait", 3: "Hallucinated/wrong API",
        4: "Missing include/type", 5: "Template/overload deduction", 6: "Access control",
        7: "Reference/value-category", 8: "Type mismatch/conversion", 9: "Malformed syntax",
        10: "Other", 12: "Test-harness/macro misuse"}


def is_macro(fe):
    return bool(re.search(r'macro ".*" passed \d+ argument|unterminated argument list invoking macro', fe))


def is_fence(fe):
    return "stray" in fe.lower() and "`" in fe


def main():
    defs = {d["defect_id"]: d for d in load_all(D4C)}
    idx = {r["id"]: r for r in csv.DictReader((TAX / "rq1_index.csv").open())}
    audit = list(csv.DictReader((TAX / "rq1_audit_review.csv").open(encoding="utf-8-sig")))

    # (2) resolve the 6 fence cells: strip fence, re-compile, overwrite error file, classify real error
    fence_real = {}
    for r in audit:
        if not is_fence(r["first_error"]):
            continue
        m = idx[r["id"]]
        d = defs[m["defect_id"]]
        repo = Path(REPOS[m["project"]])
        tp = STUDY / "data/raw" / m["defect_id"] / m["model"] / f"run{m['run_id']}" / "test.cpp"
        stripped = "\n".join(l for l in tp.read_text(errors="replace").splitlines()
                             if not FENCE_LINE.match(l))
        prepare_version(repo, d, "fixed")
        with tempfile.TemporaryDirectory() as td:
            tf = Path(td) / "t.cpp"
            tf.write_text(stripped)
            b = compile_test(tf, d, repo, Path(td) / "b")
        (TAX / "rq1_errors" / f"{r['id']}.txt").write_text(b.stderr or "")   # overwrite w/ real error
        fence_real[r["id"]] = code_of(categorize(b.stderr or ""))

    # (1) FINAL audited labels (v2) + classifier labels
    out = open(TAX / "rq1_final.csv", "w", newline="")
    w = csv.DictWriter(out, fieldnames=["id", "defect_id", "model", "final_code", "final_label",
                                        "clf_code", "source"])
    w.writeheader()
    final_dist, repro_hit, repro_tot, mismatches = Counter(), 0, 0, []
    for r in audit:
        cid, fe = r["id"], r["first_error"]
        if is_fence(fe):
            fc, src = fence_real[cid], "fence->underlying"
        elif is_macro(fe):
            fc, src = 12, "audit(macro->12)"
        else:
            fc, src = int(r["proposed_num"]), "audit"
        err = (TAX / "rq1_errors" / f"{cid}.txt").read_text() if (TAX / "rq1_errors" / f"{cid}.txt").is_file() else ""
        clf = code_of(categorize(err))
        final_dist[fc] += 1
        if not is_fence(fe):                      # reproducibility on cells with a direct human label
            repro_tot += 1
            if clf == fc:
                repro_hit += 1
            else:
                mismatches.append((cid, clf, fc, first_error(err)[:70]))
        w.writerow({"id": cid, "defect_id": r["defect"], "model": r["model"], "final_code": fc,
                    "final_label": CATS[fc], "clf_code": clf, "source": src})
    out.close()

    print("=== FINAL RQ1 taxonomy (human audit, v2 codebook; n=72) ===")
    for code, c in sorted(final_dist.items(), key=lambda kv: -kv[1]):
        print(f"  {c:3d}  {code:2d}. {CATS[code]}  ({100*c/72:.0f}%)")
    print(f"\n=== v2 classifier reproduces the audit on {repro_hit}/{repro_tot} non-fence cells "
          f"({100*repro_hit/repro_tot:.0f}%) ===")
    print("residual (classifier != audit) -- the judgment/cascade cells:")
    for cid, clf, fc, fe in mismatches:
        print(f"  id {cid:>2}: clf={clf}.{CATS[clf]} vs audit={fc}.{CATS[fc]}  | {fe}")
    print(f"\nfence cells resolved to: " +
          ", ".join(f"id{c}->{fence_real[c]}" for c in sorted(fence_real, key=int)))
    print("wrote data/taxonomy/rq1_final.csv")


if __name__ == "__main__":
    main()
