# Bang ma so danh nhan (dien SO vao cot `label`)

## RQ1 — loi compile (chon 1 so tu cot `error`)
1. UDL/namespace (e.g. _hs without `using namespace ...::literals`)
2. static_assert / unsatisfied trait (type not formattable, etc.)
3. Hallucinated/wrong API (no such member / function / overload)
4. Missing include / unknown type
5. Template / overload deduction failure
6. Access control (private / protected member)
7. Reference / value-category binding (lvalue/rvalue, const)
8. Type mismatch / bad conversion / ambiguous overload
9. Malformed C++ syntax (template/generic syntax errors)
10. Other / uncategorized (add a note)

## RQ3 — oracle yeu (mo focal_path + test_path roi chon 1 so)
1. O1 Echo / self-confirming (assert encodes CURRENT output -> buggy still passes)
2. O2 Wrong API surface (bug only observable via an API the test never calls)
3. O3 Too-weak assertion (only non-empty / no-crash / type, not the value)
4. O4 Insufficient input (inputs never trigger the buggy branch/condition)
5. O5 Other / unclear
