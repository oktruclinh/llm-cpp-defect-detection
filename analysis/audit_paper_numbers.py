#!/usr/bin/env python3
"""Recompute every numeric claim in the paper from the shipped data and compare.

One row per claim: what the paper says, what the artifact says, and whether they agree.
Intended as the last gate before a camera-ready tag, so that no number in the PDF is
taken on trust. Claims are transcribed from the paper by hand (the CLAIMS table below);
values are recomputed from data/ every run.

Run with cwd = artifact/ :  python3 analysis/audit_paper_numbers.py
Exit status is 1 if any claim mismatches.
"""
import csv
import json
import math
import sys
from collections import Counter, defaultdict
from pathlib import Path

ART = Path(__file__).resolve().parent.parent
RES = ART / "data" / "results"
TAX = ART / "data" / "taxonomy"

MODEL_FILES = {
    "Gemini-2.5-Flash": "rows_tierA_gemini-2.5-flash_fixed.jsonl",
    "Qwen3-Coder-30B": "rows_tierA_qwen3-coder_fixed.jsonl",
    "DeepSeek-V4-flash": "rows_tierA_deepseek-chat_fixed.jsonl",
    "GPT-5.4": "rows_tierA_gpt-5.4-or_fixed.jsonl",
}
RQ1_GROUP = {3: "api", 12: "gtest", 4: "include",
             1: "cxx", 2: "cxx", 5: "cxx", 7: "cxx", 9: "cxx", 6: "other", 10: "other"}


def load(name):
    return [json.loads(l) for l in (RES / name).read_text().splitlines() if l.strip()]


def r0(x):
    return math.floor(x + 0.5)


def main():
    tierA = {m: load(f) for m, f in MODEL_FILES.items()}
    allA = [r for rs in tierA.values() for r in rs]
    eligible = [r for r in allA if r["real_bug_detected"] is not None]
    dets = [r for r in eligible if r["real_bug_detected"]]

    internal = load("campaign_internal.jsonl") + load("campaign_internal_v2.jsonl") + \
        load("campaign_spirv.jsonl")
    RECLASS = {"nanomsg___nng@6a59b15fba", "nanomsg___nng@e3b8f31b04"}
    aud = [r for r in internal if r["defect_id"] not in RECLASS]

    peg = load("campaign_cpp_peglib.jsonl")
    arrow_pub = load("arrow_public.jsonl")
    arrow_cc = load("campaign_arrow_cc.jsonl")
    api = load("campaign_api_rich.jsonl")
    temp = load("sensitivity_temp.jsonl")
    fixed = load("rows_tierA_deepseek-chat_fixed.jsonl")
    buggy = load("rows_tierA_deepseek-chat_buggy.jsonl")

    rq1 = list(csv.DictReader((TAX / "rq1_final_adjudicated.csv").open()))
    g = Counter(RQ1_GROUP[int(r["final_code"])] for r in rq1)
    n1 = len(rq1)

    e4 = list(csv.DictReader((ART / "rq3" / "e4_suite_classification.csv").open()))
    A = [s for s in e4 if s["group"] == "A"]

    nc = {m: sum(1 for r in rs if r["outcome"] == "non_compilable") for m, rs in tierA.items()}
    cells = sum(r.get("n_tests") or 0 for r in allA)
    inval = sum(r.get("n_invalid_on_fixed") or 0 for r in allA)
    clean = [r for r in allA if r["compiled_fixed"] and (r.get("n_invalid_on_fixed") or 0) == 0
             and (r.get("n_flaky_fixed") or 0) == 0 and (r.get("n_tests") or 0) > 0]

    # (section, claim, paper value, recomputed value)
    C = [
        ("Table I", "Tier A cells", 228, len(allA)),
        ("Table I", "Tier A defects", 19, len({r["defect_id"] for r in allA})),
        ("Table I", "internal pre-audit cells", 138, len(internal)),
        ("Table I", "internal pre-audit defects", 23, len({r["defect_id"] for r in internal})),
        ("Table I", "BugsC++ cells", 80, len(peg)),
        ("Table I", "BugsC++ defects", 10, len({r["defect_id"] for r in peg})),
        ("Table I", "Arrow public cells", 6, len(arrow_pub)),
        ("Table I", "Arrow Flush cells", 6, len(arrow_cc)),

        ("RQ1", "non-compile Qwen %", 16, r0(100 * nc["Qwen3-Coder-30B"] / 57)),
        ("RQ1", "non-compile DeepSeek %", 26, r0(100 * nc["DeepSeek-V4-flash"] / 57)),
        ("RQ1", "non-compile GPT-5.4 %", 30, r0(100 * nc["GPT-5.4"] / 57)),
        ("RQ1", "non-compile Gemini %", 54, r0(100 * nc["Gemini-2.5-Flash"] / 57)),
        ("RQ1", "failed suites in Table II", 72, n1),
        ("Table II", "wrong/unavailable API %", 44, r0(100 * g["api"] / n1)),
        ("Table II", "GoogleTest misuse %", 17, r0(100 * g["gtest"] / n1)),
        ("Table II", "missing include %", 15, r0(100 * g["include"] / n1)),
        ("Table II", "C++ constraints %", 18, r0(100 * g["cxx"] / n1)),
        ("Table II", "other %", 6, r0(100 * g["other"] / n1)),
        ("RQ1", "audited internal cells", 126, len(aud)),
        ("RQ1", "audited internal defects", 21, len({r["defect_id"] for r in aud})),
        ("RQ1", "audited internal compiling", 0, sum(1 for r in aud if r.get("compiled"))),
        ("RQ1", "reclassified nng cells", 12, len(internal) - len(aud)),
        ("RQ1", "reclassified nng non-compiling", 10,
         sum(1 for r in internal if r["defect_id"] in RECLASS and not r.get("compiled"))),

        ("RQ2", "suites compiling", 156, sum(1 for r in allA if r["compiled_fixed"])),
        ("RQ2", "eligible suites", 141, len(eligible)),
        ("RQ2", "detections", 16, len(dets)),
        ("RQ2", "pooled conditional %", 11, r0(100 * len(dets) / len(eligible))),
        ("RQ2", "pooled unconditional %", 7, r0(100 * len(dets) / len(allA))),
        ("RQ2", "defects ever detected", 4, len({r["defect_id"] for r in dets})),
        ("RQ2", "invalid cases", 674, inval),
        ("RQ2", "total cases", 1810, cells),
        ("RQ2", "invalid %", 37, r0(100 * inval / cells)),
        ("RQ2", "clean suites", 14, len(clean)),
        ("RQ2", "strict detections", 0, sum(1 for r in clean if r["real_bug_detected"])),
        ("RQ2", "reached-not-caught /141", 41, len(A)),
        ("RQ2", "reached-not-caught %", 29, r0(100 * len(A) / len(eligible))),
        ("RQ2", "never reached /141", 84, len(eligible) - len(dets) - len(A)),
        ("RQ2", "never reached %", 60, r0(100 * (len(eligible) - len(dets) - len(A)) / len(eligible))),
        ("Table IV", "BugsC++ detections", 0, sum(1 for r in peg if r.get("detected"))),

        ("RQ3", "population", 41, len(A)),
        ("RQ3", "infection", 29, sum(1 for s in A if s["final_after_adjudication"] == "O4")),
        ("RQ3", "propagation", 12, sum(1 for s in A if s["final_after_adjudication"] == "O2")),
        ("RQ3", "assertion failures", 0, sum(1 for s in A if s["final_after_adjudication"] == "O3")),
        ("Threats", "fmt share of population", 39,
         sum(1 for s in A if s["defect_id"].startswith("fmtlib"))),
        ("Threats", "top infection defect", 8,
         Counter(s["defect_id"] for s in A if s["final_after_adjudication"] == "O4").most_common(1)[0][1]),
        ("Threats", "top propagation defect", 8,
         Counter(s["defect_id"] for s in A if s["final_after_adjudication"] == "O2").most_common(1)[0][1]),

        ("IV-D", "buggy-arm detections", 3, sum(1 for r in buggy if r["real_bug_detected"])),
        ("IV-D", "fixed-arm detections", 5, sum(1 for r in fixed if r["real_bug_detected"])),
        ("IV-D", "fixed-arm compile %", 74, r0(100 * sum(1 for r in fixed if r["compiled_fixed"]) / 57)),
        ("IV-D", "buggy-arm compile %", 67, r0(100 * sum(1 for r in buggy if r["compiled_fixed"]) / 57)),
        ("IV-D", "api_rich cells", 24, sum(1 for r in api if r["cond"] == "api_rich")),
        ("IV-D", "api_rich detections", 2, sum(1 for r in api if r["cond"] == "api_rich" and r["detected"])),
        ("IV-D", "guided detections", 2, sum(1 for r in api if r["cond"] == "guided" and r["detected"])),
        ("IV-D", "temp 0.0 detections", 4, sum(1 for r in temp if r["temp"] == 0.0 and r["det"])),
        ("IV-D", "temp 0.2 detections", 5, sum(1 for r in temp if r["temp"] == 0.2 and r["det"])),
        ("IV-D", "temp 0.8 detections", 2, sum(1 for r in temp if r["temp"] == 0.8 and r["det"])),
        ("IV-D", "temp cells per arm", 12, sum(1 for r in temp if r["temp"] == 0.2)),
    ]

    # per-model detection counts and conditional rates
    for m, paper_det, paper_cond, paper_elig in (
            ("Gemini-2.5-Flash", 1, 4, 24), ("Qwen3-Coder-30B", 2, 5, 38),
            ("DeepSeek-V4-flash", 5, 13, 39), ("GPT-5.4", 8, 20, 40)):
        rs = tierA[m]
        el = [r for r in rs if r["real_bug_detected"] is not None]
        d = sum(1 for r in el if r["real_bug_detected"])
        C.append(("Table IV", f"{m} detections", paper_det, d))
        C.append(("Table IV", f"{m} eligible", paper_elig, len(el)))
        C.append(("Table IV", f"{m} conditional %", paper_cond, r0(100 * d / len(el))))


    # ---- subject pool, accessibility, reliability, Arrow, cross-configuration ----
    sys.path.insert(0, str(ART / "benchmarks"))
    from defects4c_adapter import load_all                      # noqa: E402
    ds = load_all(Path("/home/truclinh/Thesis/kse2026/defects4c/defectsc_tpl"))
    tier = Counter(d["tier"] for d in ds)
    abc = [d for d in ds if d["tier"] in "ABC"]
    hdr = [d for d in abc if d["focal_src"].endswith((".h", ".hpp"))]

    rq1f = list(csv.DictReader((TAX / "rq1_final.csv").open()))
    fit = sum(1 for r in rq1f if r["clf_code"] == r["final_code"])

    # coder 1's RQ1 labels live in rq1_final.csv (field final_code), keyed by id --
    # rq1_coder1.csv is the blank labelling sheet, so joining against it yields nothing.
    c1 = {r["id"]: r["final_code"].strip() for r in csv.DictReader((TAX / "rq1_final.csv").open())}
    lm = {r["id"]: r["label"].strip() for r in csv.DictReader((TAX / "rq1_labmate.csv").open())
          if r["label"].strip()}
    ids = [i for i in lm if i in c1]
    a = [c1[i] for i in ids]
    b = [lm[i] for i in ids]
    po = sum(1 for x, y in zip(a, b) if x == y) / len(ids)
    ca, cb = Counter(a), Counter(b)
    pe = sum((ca[k] / len(ids)) * (cb[k] / len(ids)) for k in set(a) | set(b))
    kap = (po - pe) / (1 - pe)

    e5 = json.loads((RES / "e5_cross_config.json").read_text())
    e5d = [json.loads(l) for l in (RES / "e5d_probe.jsonl").read_text().splitlines() if l.strip()]

    C.extend([
        ("Sec. III", "Defects4C records in artifact", 241, len(ds)),
        ("Sec. III", "candidate pool", 93, len(abc)),
        ("Table I", "Tier A defects in pool", 19, tier["A"]),
        ("Table I", "Tier B defects in pool", 26, tier["B"]),
        ("Table I", "Tier C defects in pool", 48, tier["C"]),
        ("Sec. III", "header-defined focal (A+B)", 21, len(hdr)),
        ("Sec. III", "directly callable after audit", 24, len(hdr) + 3),
        ("Sec. III", "classifier fit %", 94, r0(100 * fit / len(rq1f))),
        ("Sec. III", "RQ1 kappa cells", 25, len(ids)),
        ("Sec. III", "RQ1 kappa x100", 56, r0(100 * kap)),
        ("RQ2", "Arrow public compiling", 2, sum(1 for r in arrow_pub if r.get("compiled"))),
        ("RQ2", "Arrow public detecting", 1, sum(1 for r in arrow_pub if r.get("detected"))),
        ("RQ2", "Arrow public reached-not-caught", 1,
         sum(1 for r in arrow_pub if r["outcome"] == "reached_not_detected")),
        ("RQ2", "Arrow Flush compiling", 0, sum(1 for r in arrow_cc if r.get("compiled"))),
        ("IV-D", "zero-repair compile % (Tier A)", 45,
         r0(100 * int(e5["A_repair_budget"]["pooled_zero_repair"].split("/")[0]) / 228)),
        ("IV-D", "clang compile % (Tier A)", 68,
         r0(100 * int(e5["B_compiler"]["pooled_clang"].split("/")[0]) / 228)),
        ("IV-D", "probe cells compiling", 4,
         sum(1 for r in e5d if r.get("zero_repair_compiles_gpp"))),
        ("IV-D", "probe cells", 12, len(e5d)),
    ])

    bad = [c for c in C if c[2] != c[3]]
    w = max(len(c[1]) for c in C)
    print(f"{'SECTION':<10} {'CLAIM':<{w}} {'PAPER':>7} {'ARTIFACT':>9}  ")
    print("-" * (10 + w + 22))
    for sec, name, pv, av in C:
        mark = "" if pv == av else "   <<< MISMATCH"
        print(f"{sec:<10} {name:<{w}} {pv:>7} {av:>9}{mark}")
    print()
    print(f"checked {len(C)} claims, {len(bad)} mismatch(es)")
    if bad:
        for sec, name, pv, av in bad:
            print(f"  MISMATCH  {sec} / {name}: paper {pv}, artifact {av}")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
