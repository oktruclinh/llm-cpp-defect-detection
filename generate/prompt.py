#!/usr/bin/env python3
"""Standard test-generation prompt + compile-repair prompt.
Style mirrors Yuan et al. / Schäfer et al. (expert role, focal method, GoogleTest,
high coverage + meaningful assertions). Kept in one place — no scattered strings."""

SYSTEM = ("You are an expert C++ developer who writes high-quality unit tests with "
          "the GoogleTest framework. You write tests that both maximize coverage and "
          "contain meaningful assertions that would FAIL if the code under test is wrong.")

# condition -> how the focal code is presented (bracket the oracle-leakage confound K3)
_FOCAL_BLOCK = {
    "fixed":  "Focal function (taken from the FIXED, correct version of the code):\n```cpp\n{focal_code}\n```",
    "buggy":  "Focal function (the implementation under test):\n```cpp\n{focal_code}\n```",
    "no_focal": "Focal function signature (implementation withheld):\n```cpp\n{focal_code}\n```",
}

_GEN_TMPL = """\
Write a complete, self-contained GoogleTest source file (.cpp) that unit-tests the \
following focal function from the project `{project}`.

Focal file: `{focal_src}`
{focal_block}

Requirements:
- Exercise the focal function's behavior through the library's PUBLIC API; include the \
appropriate public header(s) of `{project}`.
- Include `<gtest/gtest.h>`. Do NOT write a `main()` (a gtest main is linked separately).
- Cover normal cases AND edge cases (boundaries, empty/zero, error paths) relevant to \
this function.
- Assertions must encode the CORRECT expected behavior so that a buggy implementation \
would make a test fail. Avoid assertions that merely echo whatever the code returns.
- Output ONLY the .cpp file inside a single ```cpp code block. No prose."""

_REPAIR_TMPL = """\
The GoogleTest file you wrote does not compile. Fix ONLY the compilation errors and \
return the full corrected file. Do not change the test intent or weaken assertions.

Compiler errors:
```
{errors}
```

Return ONLY the corrected .cpp inside a single ```cpp code block."""


_CTX_TMPL = """\
Write a complete, compilable GoogleTest source file (.cpp) that unit-tests the focal \
function from the project `{project}`.

## Build / framework context
- Language standard: C++17. GoogleTest IS available and linked — use `<gtest/gtest.h>`, do NOT write `main()`.
- The code under test is compiled into the project's library and linked for you.

## Includes the focal source file uses (include the appropriate PUBLIC header(s) from these)
```cpp
{includes}
```

## Focal class surface (focal function + its enclosing class/namespace + sibling declarations)
```cpp
{region}
```

## Rules (important for compilation)
- Use ONLY the project's PUBLIC API and the headers shown above; include the correct public header(s).
- Do NOT call internal/private functions or access private fields. Avoid internal helpers \
(e.g. names with internal prefixes such as `nni_`, `detail::`, `impl_`); call the PUBLIC API instead.
- Use correct namespaces; access static members via `ClassName::`. Construct objects via their public constructors.
- Cover normal AND edge cases; assertions must encode the CORRECT expected behavior (a buggy \
implementation should make a test FAIL), not merely echo current output.

Output ONLY the .cpp inside a single ```cpp code block. No prose."""


def gen_messages_context_rich(defect: dict, focal_code: str, includes, region):
    inc = "\n".join(includes) if includes else "(none found — use the project's public header)"
    return [
        {"role": "system", "content": SYSTEM},
        {"role": "user", "content": _CTX_TMPL.format(
            project=defect["project"], includes=inc, region=region or focal_code)},
    ]


_GUIDED_RULE = """

IMPORTANT for compilation: test the focal function's behavior ONLY through the project's \
PUBLIC, top-level API (e.g. `fmt::format`, public library functions in the project's public \
headers). Do NOT call internal/implementation symbols — anything in a `detail`/`internal` \
namespace, or with prefixes like `nni_`, `*_impl` — these are not part of the public API and \
will not link. Include the correct PUBLIC header of the project."""


def gen_messages_guided(defect: dict, focal_code: str):
    """Naive focal prompt + a public-API rule (no internal-implementation context dump)."""
    m = gen_messages(defect, focal_code, condition="fixed")
    m[1]["content"] += _GUIDED_RULE
    return m


# Curated PUBLIC API surface per project (signatures only — what CityWalk extracts via AST;
# we curate it since the eval laptop has no clang tooling). NOT leaky: documentation-level
# public symbols, no bug/test info. Used by the `api_rich` robustness arm.
PUBLIC_API = {
    "fmtlib___fmt": """// fmt public API (namespace fmt; headers <fmt/core.h>, <fmt/format.h>):
std::string fmt::format(format_string<Args...> fmt, Args&&... args);
OutputIt    fmt::format_to(OutputIt out, format_string<Args...> fmt, Args&&... args);
format_to_n_result<OutputIt> fmt::format_to_n(OutputIt out, size_t n, format_string<Args...> fmt, Args&&...);
size_t      fmt::formatted_size(format_string<Args...> fmt, Args&&... args);
void        fmt::print(format_string<Args...> fmt, Args&&... args);
template <typename T, typename Char> struct fmt::formatter;  // user customization point""",
    "yhirose___cpp_peglib": """// cpp-peglib public API (namespace peg; header peglib.h):
peg::parser pg;                                  // default constructor
peg::parser pg(std::string_view grammar);        // construct from a PEG grammar string
bool        pg.load_grammar(const char* s, size_t n);
explicit    operator bool() const;               // (bool)pg is true IFF the grammar is valid
bool        pg.parse(std::string_view sv) const; // parse input text
template <typename T> bool pg.parse(std::string_view sv, T& out) const;  // parse + semantic value
peg::parser& pg[const char* rule];               // attach action: pg["RULE"] = [](const peg::SemanticValues& vs){...};
template <typename T = peg::Ast> peg::parser& pg.enable_ast();
void        pg.enable_packrat_parsing();
pg.log = [](size_t line, size_t col, const std::string& msg){...};  // error callback
// peg::SemanticValues (in actions): vs.token(), vs.sv(), vs[i], vs.choice(); peg::any_cast<T>(vs[i])""",
}

_API_RICH_BLOCK = """

## Public API you may call (signatures only — drive the focal behavior through THESE)
```cpp
{api}
```
Use only the public symbols above (and standard headers). Do NOT call internal/`detail`/`*_impl` \
symbols. Still: assertions must encode the CORRECT expected behavior so a buggy implementation FAILS."""


def gen_messages_api_rich(defect: dict, focal_code: str, api_text: str):
    """guided prompt + the project's PUBLIC API surface (Yang's FC_m / CityWalk context).
    The 'more information' robustness arm: tests whether richer public-API context lifts
    detection (not just compilation). No few-shot example (leaky + conflates variables)."""
    m = gen_messages_guided(defect, focal_code)
    if api_text:
        m[1]["content"] += _API_RICH_BLOCK.format(api=api_text)
    return m


def gen_messages(defect: dict, focal_code: str, condition: str = "fixed"):
    block = _FOCAL_BLOCK.get(condition, _FOCAL_BLOCK["fixed"]).format(focal_code=focal_code)
    return [
        {"role": "system", "content": SYSTEM},
        {"role": "user", "content": _GEN_TMPL.format(
            project=defect["project"], focal_src=defect["focal_src"], focal_block=block)},
    ]


def repair_messages(defect, focal_code, prior_assistant_text, compiler_errors):
    msgs = gen_messages(defect, focal_code)
    msgs.append({"role": "assistant", "content": prior_assistant_text})
    msgs.append({"role": "user", "content": _REPAIR_TMPL.format(errors=compiler_errors[-2500:])})
    return msgs


def extract_code(text: str) -> str:
    """Pull the first ```cpp ...``` block. Robust to UNCLOSED / malformed fences: if no closed
    block matches but a fence marker is present, strip the pure fence-marker lines so a stray
    ``` / ```cpp does not leak into the .cpp (that leak caused 'stray backtick' compile failures
    that masked the test's real error)."""
    import re
    m = re.search(r"```(?:cpp|c\+\+|cxx)?\s*\n(.*?)```", text, re.DOTALL | re.IGNORECASE)
    if m:
        return m.group(1).strip()
    if "```" in text:
        return "\n".join(l for l in text.splitlines()
                         if not re.match(r"^\s*```+\s*[a-zA-Z+]*\s*$", l)).strip()
    return text.strip()
