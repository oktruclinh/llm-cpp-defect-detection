#!/usr/bin/env python3
"""Non-leaky context extraction for the context-rich prompt ablation (Ablation A).

Design grounded in the related-work study (CityWalk, Yang et al., ChatUniTest):
the context elements that most improve C++ compilability are (1) the focal file's
#includes (ChatUniTest's #1 fix for "cannot find symbol"), (2) the focal CLASS
SURFACE — sibling method signatures = Yang's FC_m, the only feature with a
significant compilability effect, plus namespace/constructor context, and (3) a
build/framework line (CityWalk's Dep_c, the biggest C++ lever). We do NOT feed an
existing test (leaky + the literature is zero-shot). All extraction at the FIXED commit.
"""
import re
import subprocess
from pathlib import Path

_SIG = re.compile(r'^[A-Za-z_][\w:<>,\s\*&~]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*'
                  r'(const\b)?\s*(noexcept\b)?\s*(override\b)?\s*[;{]')
_KW = re.compile(r'^\s*(if|for|while|switch|return|else|catch|do|namespace|class|struct|'
                 r'enum|template|using|typedef|static_assert|friend|case|default)\b')


def public_api_surface(repo: Path, commit: str, focal_src: str, cap: int = 40):
    """The PUBLIC API surface of the focal file: public method + free-function SIGNATURES
    (bodies stripped). This is Yang's FC_m (the compilability-significant feature) and
    CityWalk's public-signature context — the 'more information' arm. Internal symbols
    (detail/impl/_impl/nni_) and private/protected members are excluded. Heuristic (no AST),
    sufficient to give the model a menu of callable public API. Extracted at the FIXED commit."""
    blob = _git_show(repo, commit, focal_src)
    if not blob:
        return []
    out, seen, cls, depth = [], set(), [], 0
    for raw in blob.split("\n"):
        s = raw.strip()
        am = re.match(r'(public|private|protected)\s*:', s)
        if am and cls:
            cls[-1][1] = am.group(1)
        cm = re.match(r'(?:template\s*<[^;]*?>\s*)?(class|struct)\s+([A-Za-z_]\w*)', s)
        m = _SIG.match(s)
        if m and not _KW.match(s) and "(" in s.split("{")[0]:
            access = cls[-1][1] if cls else "public"
            internal = bool(re.search(r'\bdetail\b|\binternal\b|_impl|\bnni_|::detail', s))
            if access == "public" and not internal:
                head = re.sub(r'\s+', ' ', s.split("{")[0]).strip().rstrip(";").strip()
                pref = (cls[-1][0] + "::") if cls else ""
                key = pref + head
                if key not in seen and 4 < len(head) < 130:
                    seen.add(key)
                    out.append(f"{pref}{head}" if pref else head)
        depth += raw.count("{") - raw.count("}")
        if cm and not s.rstrip().endswith(";"):
            cls.append([cm.group(2), "public" if cm.group(1) == "struct" else "private", depth])
        while cls and depth <= cls[-1][2]:
            cls.pop()
        if len(out) >= cap:
            break
    return out


def _git_show(repo: Path, commit: str, rel: str):
    try:
        return subprocess.run(["git", "-C", str(repo), "show", f"{commit}:{rel}"],
                              capture_output=True, text=True, check=True).stdout
    except subprocess.CalledProcessError:
        return None


def focal_includes(repo: Path, commit: str, focal_src: str):
    """The #include directives of the focal source file — tells the model the exact
    headers a TU of this file uses (public API surface entry points)."""
    blob = _git_show(repo, commit, focal_src)
    if not blob:
        return []
    return [ln.rstrip() for ln in blob.splitlines() if ln.strip().startswith("#include")]


def focal_region(repo: Path, commit: str, focal_src: str, func_start: int, func_end: int,
                 before: int = 60, after: int = 30, max_lines: int = 200):
    """A window of the focal file around the focal function: surfaces sibling method
    signatures (FC_m), the enclosing class/namespace, and nearby declarations — the
    'focal class surface' the model needs to call the right (public) API."""
    blob = _git_show(repo, commit, focal_src)
    if not blob:
        return None
    lines = blob.splitlines()
    fs = func_start or 1
    fe = func_end or fs
    s = max(0, fs - 1 - before)
    e = min(len(lines), fe + after)
    region = lines[s:e]
    if len(region) > max_lines:
        region = region[:max_lines]
    return "\n".join(region)
