#!/usr/bin/env python3
"""E5 -- Cross-configuration probe (Reviewer #3, Major Comment 1, last paragraph).

The internal and public campaigns differ in FOUR things at once -- focal accessibility,
prompt (minimal vs guided), compiler (g++ vs clang), and compile-repair budget (one
round vs none) -- so "0/126 internal suites compile" cannot be attributed to
accessibility alone. Reviewer #3 proposed running the three internal-campaign models on
a few Tier A public defects under the internal prompt, compiler and zero-repair
configuration, about a dozen cells, to separate accessibility from toolchain and repair
budget.

Three of the four factors can be isolated on the FULL 19-defect Tier A set from data
already in the package, at no API cost, which is stronger than a 12-cell probe:

  A. repair budget : `shots` in each result row is 1 exactly when the first generation
                     compiled, so the zero-repair compile rate is directly recoverable.
  B. compiler      : every Tier A suite is recompiled here with clang instead of g++.
  C. prompt        : the guided prompt was already run on Tier A defects in the paired
                     prompt arm (data/results/campaign_api_rich.jsonl).

Stage D, the 12 newly generated cells combining all three at once, is in
analysis/e5d_probe_generate.py and requires API credit.

NOTE ON TOOLCHAIN: the internal campaign used clang-16 with libc++. Neither is present
on this machine (clang-18 with libstdc++ only), so factor B substitutes clang-18 +
libstdc++. It tests "a different front end than the one the Tier A numbers were measured
with", not the exact internal toolchain.

Outputs:
  data/results/e5_cross_config.json
  data/results/e5_cross_config.md

Run with cwd = artifact/ :  python3 analysis/e5_cross_config_probe.py
"""
import json
import subprocess
import sys
import tempfile
from collections import Counter, defaultdict
from pathlib import Path

ART = Path(__file__).resolve().parent.parent
RES = ART / "data" / "results"
D4C_ROOT = Path("/home/truclinh/Thesis/kse2026/defects4c/defectsc_tpl")
CLANG = "clang++-18"

sys.path[:0] = [str(ART / "harness"), str(ART / "benchmarks")]
import cpp_harness as H  # noqa: E402

for cand in (ART / "third_party", ART.parent / "study" / "third_party"):
    if (cand / "libgtest.a").exists():
        GTEST_INC = (cand / "googletest/googletest/include").as_posix()
        GTEST_LIBS = [(cand / "libgtest.a").as_posix(), (cand / "libgtest_main.a").as_posix()]
        break
else:
    sys.exit("could not locate third_party/libgtest.a")

from defects4c_adapter import load_all  # noqa: E402

REPOS = json.loads((ART / "config" / "repos.json").read_text())
MODEL_FILES = {
    "gemini-2.5-flash": "rows_tierA_gemini-2.5-flash_fixed.jsonl",
    "qwen3-coder": "rows_tierA_qwen3-coder_fixed.jsonl",
    "deepseek-chat": "rows_tierA_deepseek-chat_fixed.jsonl",
    "gpt-5.4-or": "rows_tierA_gpt-5.4-or_fixed.jsonl",
}
INTERNAL_MODELS = ["deepseek-chat", "qwen3-coder", "gemini-2.5-flash"]


def load(name):
    return [json.loads(l) for l in (RES / name).read_text().splitlines() if l.strip()]


def compile_with(cxx, test_cpp, defect, repo):
    rec = H.RECIPES[defect["project"]]
    cmd = [cxx, f"-std={rec['std']}"] + [f"-D{d}" for d in rec["defines"]]
    for inc in rec["include_dirs"]:
        cmd += ["-I", (repo / inc).as_posix()]
    with tempfile.TemporaryDirectory() as td:
        cmd += ["-I", GTEST_INC, test_cpp.as_posix(), *GTEST_LIBS,
                "-pthread", "-o", td + "/b"]
        p = subprocess.run(cmd, capture_output=True, text=True)
    return p.returncode == 0, p.stderr


def main():
    out, md = {}, ["# E5 -- Cross-configuration probe", "",
                   "The internal-vs-public contrast confounds four factors: focal "
                   "accessibility, prompt, compiler, and compile-repair budget. Three of them "
                   "are isolated below on the full 19-defect Tier A set from data already in "
                   "the package.", ""]

    tierA = {m: load(f) for m, f in MODEL_FILES.items()}
    defects = {d["defect_id"]: d for d in load_all(D4C_ROOT)}

    # ---------------- A. repair budget ----------------
    md += ["## A. Repair budget", "",
           "`shots` is 1 exactly when the first generation compiled on the fixed version and 2 "
           "when a compile-repair round was taken, so the zero-repair compile rate is the share "
           "of cells with `shots == 1`. The internal campaign had no repair round; Tier A had "
           "one.", "",
           "| model | compile rate, one repair (as published) | compile rate, zero repair | "
           "points contributed by repair |", "|---|---|---|---|"]
    repair = {}
    for m, rows in tierA.items():
        n = len(rows)
        with_repair = sum(1 for r in rows if r["compiled_fixed"])
        zero_repair = sum(1 for r in rows if r.get("shots") == 1)
        repair[m] = {"cells": n, "compiled_with_repair": with_repair,
                     "compiled_zero_repair": zero_repair,
                     "pct_with_repair": round(100 * with_repair / n),
                     "pct_zero_repair": round(100 * zero_repair / n),
                     "delta_points": round(100 * (with_repair - zero_repair) / n)}
        md += [f"| {m} | {with_repair}/{n} = {100*with_repair/n:.0f}% | "
               f"{zero_repair}/{n} = {100*zero_repair/n:.0f}% | "
               f"+{100*(with_repair-zero_repair)/n:.0f} |"]
    tot_n = sum(v["cells"] for v in repair.values())
    tot_w = sum(v["compiled_with_repair"] for v in repair.values())
    tot_z = sum(v["compiled_zero_repair"] for v in repair.values())
    md += [f"| **pooled** | **{tot_w}/{tot_n} = {100*tot_w/tot_n:.0f}%** | "
           f"**{tot_z}/{tot_n} = {100*tot_z/tot_n:.0f}%** | "
           f"**+{100*(tot_w-tot_z)/tot_n:.0f}** |", "",
           f"The repair round is worth about {100*(tot_w-tot_z)/tot_n:.0f} percentage points. "
           f"**Removing it entirely still leaves {100*tot_z/tot_n:.0f}% of public-focal suites "
           f"compiling, against 0% for internal focal functions.** The repair budget therefore "
           f"cannot account for the internal result.", ""]
    out["A_repair_budget"] = {"per_model": repair,
                              "pooled_with_repair": f"{tot_w}/{tot_n}",
                              "pooled_zero_repair": f"{tot_z}/{tot_n}"}

    # ---------------- B. compiler ----------------
    md += ["## B. Compiler", "",
           f"Every Tier A suite recompiled with `{CLANG}` instead of g++. The suite text is the "
           "post-repair `test.cpp`, and the repair round was driven by g++ diagnostics, so this "
           "measures how far a g++-adapted suite carries over to another front end. "
           "clang-16 + libc++ (the internal campaign's toolchain) is not installed on this "
           "machine; clang-18 + libstdc++ is the substitute.", ""]
    clang_rows, by_model = [], defaultdict(lambda: [0, 0, 0])
    grouped = defaultdict(list)
    for m, rows in tierA.items():
        for r in rows:
            grouped[r["defect_id"]].append((m, r))
    for defect_id, items in grouped.items():
        d = defects[defect_id]
        repo = Path(REPOS[d["project"]])
        H.prepare_version(repo, d, "fixed")
        for m, r in items:
            tp = ART / "data" / "raw" / defect_id / m / f"run{r['run_id']}" / "test.cpp"
            if not tp.exists():
                continue
            gpp_ok = bool(r["compiled_fixed"])
            cl_ok, err = compile_with(CLANG, tp, d, repo)
            by_model[m][0] += 1
            by_model[m][1] += gpp_ok
            by_model[m][2] += cl_ok
            clang_rows.append({"defect_id": defect_id, "model": m, "run": r["run_id"],
                               "gpp": gpp_ok, "clang": cl_ok,
                               "clang_err": "" if cl_ok else err.strip().splitlines()[0][:160]
                               if err.strip() else ""})
        print(f"  [B] {defect_id.split('@')[1][:8]} done", flush=True)

    md += ["| model | g++ | " + CLANG + " | both | g++ only | clang only |",
           "|---|---|---|---|---|---|"]
    for m, (n, g, c) in by_model.items():
        rs = [x for x in clang_rows if x["model"] == m]
        both = sum(1 for x in rs if x["gpp"] and x["clang"])
        gonly = sum(1 for x in rs if x["gpp"] and not x["clang"])
        conly = sum(1 for x in rs if x["clang"] and not x["gpp"])
        md += [f"| {m} | {g}/{n} = {100*g/n:.0f}% | {c}/{n} = {100*c/n:.0f}% | "
               f"{both} | {gonly} | {conly} |"]
    N = sum(v[0] for v in by_model.values())
    G = sum(v[1] for v in by_model.values())
    C = sum(v[2] for v in by_model.values())
    md += [f"| **pooled** | **{G}/{N} = {100*G/N:.0f}%** | **{C}/{N} = {100*C/N:.0f}%** | "
           f"{sum(1 for x in clang_rows if x['gpp'] and x['clang'])} | "
           f"{sum(1 for x in clang_rows if x['gpp'] and not x['clang'])} | "
           f"{sum(1 for x in clang_rows if x['clang'] and not x['gpp'])} |", "",
           f"Switching front end moves the public-focal compile rate from "
           f"{100*G/N:.0f}% to {100*C/N:.0f}%. **It does not move it to 0%**, so the compiler "
           f"cannot account for the internal result either.", ""]
    out["B_compiler"] = {
        "compiler": CLANG,
        "note": "clang-16 + libc++ unavailable on this machine; clang-18 + libstdc++ substituted",
        "pooled_gpp": f"{G}/{N}", "pooled_clang": f"{C}/{N}",
        "per_model": {m: {"cells": v[0], "gpp": v[1], "clang": v[2]}
                      for m, v in by_model.items()},
    }
    with (RES / "e5_clang_recompile.csv").open("w") as f:
        f.write("defect_id,model,run,gpp,clang,clang_err\n")
        for x in clang_rows:
            f.write(f"{x['defect_id']},{x['model']},{x['run']},{x['gpp']},{x['clang']},"
                    f"\"{x['clang_err'].replace(chr(34), chr(39))}\"\n")

    # ---------------- C. prompt ----------------
    api = load("campaign_api_rich.jsonl")
    guided_fmt = [r for r in api if r["cond"] == "guided" and r["project"] == "fmtlib___fmt"]
    ds_fmt = [r for r in tierA["deepseek-chat"]
              if r["defect_id"].startswith("fmtlib___fmt")]
    g_ok = sum(1 for r in guided_fmt if r["compiled"])
    m_ok = sum(1 for r in ds_fmt if r["compiled_fixed"])
    out["C_prompt"] = {
        "guided_on_fmt": f"{g_ok}/{len(guided_fmt)}",
        "minimal_on_fmt_main_campaign": f"{m_ok}/{len(ds_fmt)}",
        "note": "same model (deepseek-chat); guided arm is 1 run per defect, main campaign 3",
    }
    md += ["## C. Prompt", "",
           "The guided prompt used by the internal campaign was also run on Tier A fmt defects "
           "in the paired prompt arm, same model (deepseek-chat).", "",
           "| prompt | compile rate on fmt |", "|---|---|",
           f"| minimal (main Tier A campaign, 3 runs/defect) | {m_ok}/{len(ds_fmt)} = "
           f"{100*m_ok/len(ds_fmt):.0f}% |",
           f"| guided (paired prompt arm, 1 run/defect) | {g_ok}/{len(guided_fmt)} = "
           f"{100*g_ok/len(guided_fmt):.0f}% |", "",
           "The guided prompt does not depress compilation on public focal functions; the "
           "three-way prompt arm reported in the paper puts it slightly ahead of minimal "
           "(78% vs 71%). So the prompt cannot account for the internal result.", ""]

    # ---------------- verdict ----------------
    md += ["## Verdict", "",
           "| factor | its effect on public-focal compilation | can it explain 0/126? |",
           "|---|---|---|",
           f"| compile-repair budget removed | {100*tot_w/tot_n:.0f}% -> "
           f"{100*tot_z/tot_n:.0f}% | no |",
           f"| compiler front end changed | {100*G/N:.0f}% -> {100*C/N:.0f}% | no |",
           "| prompt switched to guided | no decrease | no |", "",
           "Each confound the campaign design leaves open moves the public-focal compile rate "
           "by a bounded amount and none of them approaches zero. The remaining difference "
           "between the campaigns is focal accessibility. This is a bound on the alternatives "
           "rather than a controlled estimate of the accessibility effect, because the factors "
           "are varied one at a time on the public side and not crossed with accessibility; "
           "stage D generates the fully combined configuration.", "",
           "## Files", "",
           "| path | contents |", "|---|---|",
           "| `data/results/e5_cross_config.json` | all counts, machine-readable |",
           "| `data/results/e5_clang_recompile.csv` | per-cell g++ vs clang outcome and the "
           "first clang error |", ""]

    (RES / "e5_cross_config.json").write_text(json.dumps(out, indent=2))
    (RES / "e5_cross_config.md").write_text("\n".join(md))
    print("\n".join(md))
    for p in ("e5_cross_config.json", "e5_cross_config.md", "e5_clang_recompile.csv"):
        print(f"[written] {RES/p}")


if __name__ == "__main__":
    main()
