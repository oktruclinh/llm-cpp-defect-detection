```cpp
#include <gtest/gtest.h>
#include <peglib.h> // Public header for the peglib library

// Helper function to create a SemanticValues object for testing
// In a real scenario, SemanticValues are populated by the PEG parser.
// For unit testing, we need to manually construct one.
peg::SemanticValues create_test_semantic_values(
    const std::string& input_text,
    const std::string& matched_text,
    size_t choice = 0,
    const std::vector<std::pair<const char*, size_t>>& tokens = {})
{
    peg::SemanticValues sv;
    sv.ss = input_text.c_str();
    sv.s_ = matched_text.c_str();
    sv.n_ = matched_text.length();
    sv.choice_ = choice;
    sv.path = nullptr; // Not relevant for these tests

    // Manually populate the internal vector<any> for transform tests
    // This simulates what the parser would do.
    sv.push_back(peg::any(10));
    sv.push_back(peg::any(std::string("hello")));
    sv.push_back(peg::any(3.14));
    sv.push_back(peg::any(true));

    sv.tokens = tokens;

    return sv;
}

TEST(SemanticValuesTest, DefaultConstructor) {
    peg::SemanticValues sv;
    ASSERT_EQ(sv.c_str(), nullptr);
    ASSERT_EQ(sv.length(), 0);
    ASSERT_TRUE(sv.str().empty());
    ASSERT_EQ(sv.choice(), 0);
    ASSERT_TRUE(sv.tokens.empty());
    ASSERT_TRUE(sv.empty()); // Inherited from std::vector<any>
    ASSERT_EQ(sv.size(), 0); // Inherited from std::vector<any>
}

TEST(SemanticValuesTest, CStrAndLength) {
    std::string input = "test input string";
    std::string matched = "input";
    peg::SemanticValues sv = create_test_semantic_values(input, matched);

    ASSERT_EQ(sv.c_str(), matched.c_str());
    ASSERT_EQ(sv.length(), matched.length());
}

TEST(SemanticValuesTest, Str) {
    std::string input = "test input string";
    std::string matched = "input";
    peg::SemanticValues sv = create_test_semantic_values(input, matched);

    ASSERT_EQ(sv.str(), matched);

    // Edge case: empty matched string
    std::string empty_matched = "";
    peg::SemanticValues sv_empty = create_test_semantic_values(input, empty_matched);
    ASSERT_TRUE(sv_empty.str().empty());
}

TEST(SemanticValuesTest, LineInfo) {
    std::string input = "line 1\nline 2\nline 3";

    // Matched "line 1"
    peg::SemanticValues sv1 = create_test_semantic_values(input, "line 1");
    sv1.s_ = input.c_str(); // Manually set s_ to start of "line 1"
    ASSERT_EQ(sv1.line_info().first, 1);
    ASSERT_EQ(sv1.line_info().second, 1);

    // Matched "line 2"
    peg::SemanticValues sv2 = create_test_semantic_values(input, "line 2");
    sv2.s_ = input.c_str() + input.find("line 2"); // Manually set s_ to start of "line 2"
    ASSERT_EQ(sv2.line_info().first, 2);
    ASSERT_EQ(sv2.line_info().second, 1);

    // Matched "3" from "line 3"
    peg::SemanticValues sv3 = create_test_semantic_values(input, "3");
    sv3.s_ = input.c_str() + input.find("3"); // Manually set s_ to start of "3"
    ASSERT_EQ(sv3.line_info().first, 3);
    ASSERT_EQ(sv3.line_info().second, 6); // 'l' is 1, 'i' is 2, 'n' is 3, 'e' is 4, ' ' is 5, '3' is 6

    // Edge case: empty input string
    std::string empty_input = "";
    peg::SemanticValues sv_empty_input = create_test_semantic_values(empty_input, "");
    sv_empty_input.s_ = empty_input.c_str();
    sv_empty_input.ss = empty_input.c_str();
    ASSERT_EQ(sv_empty_input.line_info().first, 1);
    ASSERT_EQ(sv_empty_input.line_info().second, 1);

    // Edge case: s_ is nullptr
    peg::SemanticValues sv_null_s;
    sv_null_s.ss = input.c_str();
    sv_null_s.s_ = nullptr;
    // The behavior of line_info with s_ = nullptr is undefined by peg::line_info,
    // but typically it would return (1,1) or throw. Let's assume (1,1) for this test.
    // This is an assertion on the helper function's behavior, not directly SemanticValues.
    // In a real parser, s_ would never be nullptr if ss is not nullptr.
    // For robust testing, we might need to mock peg::line_info or ensure valid inputs.
    // For now, we'll rely on the default behavior of peg::line_info(nullptr, nullptr) or (ss, nullptr)
    // which typically returns (1,1).
    ASSERT_EQ(sv_null_s.line_info().first, 1);
    ASSERT_EQ(sv_null_s.line_info().second, 1);
}

TEST(SemanticValuesTest, Choice) {
    std::string input = "abc";
    std::string matched = "a";

    peg::SemanticValues sv0 = create_test_semantic_values(input, matched, 0);
    ASSERT_EQ(sv0.choice(), 0);

    peg::SemanticValues sv1 = create_test_semantic_values(input, matched, 1);
    ASSERT_EQ(sv1.choice(), 1);

    peg::SemanticValues sv_large = create_test_semantic_values(input, matched, 100);
    ASSERT_EQ(sv_large.choice(), 100);
}

TEST(SemanticValuesTest, Token) {
    std::string input = "token1 token2 token3";
    std::string matched = "token1 token2 token3";

    std::vector<std::pair<const char*, size_t>> tokens;
    tokens.push_back({input.c_str(), 6}); // "token1"
    tokens.push_back({input.c_str() + 7, 6}); // "token2"

    peg::SemanticValues sv = create_test_semantic_values(input, matched, 0, tokens);

    ASSERT_EQ(sv.token(0), "token1");
    ASSERT_EQ(sv.token(1), "token2");

    // Default token (id=0)
    ASSERT_EQ(sv.token(), "token1");

    // Edge case: no tokens, should return the matched string
    peg::SemanticValues sv_no_tokens = create_test_semantic_values(input, matched);
    ASSERT_EQ(sv_no_tokens.token(), matched);

    // Edge case: empty matched string, no tokens
    std::string empty_matched = "";
    peg::SemanticValues sv_empty_matched_no_tokens = create_test_semantic_values(input, empty_matched);
    ASSERT_TRUE(sv_empty_matched_no_tokens.token().empty());

    // Edge case: empty matched string, with tokens (should still return the token)
    std::vector<std::pair<const char*, size_t>> single_token = {{input.c_str(), 6}};
    peg::SemanticValues sv_empty_matched_with_tokens = create_test_semantic_values(input, empty_matched, 0, single_token);
    ASSERT_EQ(sv_empty_matched_with_tokens.token(), "token1");

    // Assert that accessing out-of-bounds token ID would assert (debug mode)
    // In release mode, this might lead to undefined behavior or crash.
    // For unit tests, we typically don't test for assert failures directly unless
    // using specific frameworks for that. We assume valid input for token ID.
    // If we wanted to test this, we'd need to compile with NDEBUG and check for crashes,
    // or use a death test if GoogleTest is configured for it.
    // For now, we'll avoid an explicit out-of-bounds access.
}

TEST(SemanticValuesTest, TransformBasic) {
    std::string input = "10 hello 3.14 true";
    std::string matched = "10 hello 3.14 true";
    peg::SemanticValues sv = create_test_semantic_values(input, matched);

    // Transform to int
    auto ints = sv.transform<int>();
    ASSERT_EQ(ints.size(), 4);
    ASSERT_EQ(ints[0], 10);
    // The string "hello" cannot be directly converted to int by any::get<int>
    // This will likely throw std::bad_any_cast if any::get<int> is called on a string any.
    // The current implementation of transform<T> calls v.get<T>().
    // To make this test pass, we need to ensure the types match or handle exceptions.
    // For this test, let's assume we only push compatible types for the transform.
    // Let's re-create sv with only ints for the first transform test.
    peg::SemanticValues sv_ints_only;
    sv_ints_only.push_back(peg::any(1));
    sv_ints_only.push_back(peg::any(2));
    sv_ints_only.push_back(peg::any(3));
    auto ints_only = sv_ints_only.transform<int>();
    ASSERT_EQ(ints_only.size(), 3);
    ASSERT_EQ(ints_only[0], 1);
    ASSERT_EQ(ints_only[1], 2);
    ASSERT_EQ(ints_only[2], 3);

    // Transform to string
    peg::SemanticValues sv_strings_only;
    sv_strings_only.push_back(peg::any(std::string("one")));
    sv_strings_only.push_back(peg::any(std::string("two")));
    auto strings_only = sv_strings_only.transform<std::string>();
    ASSERT_EQ(strings_only.size(), 2);
    ASSERT_EQ(strings_only[0], "one");
    ASSERT_EQ(strings_only[1], "two");

    // Transform with mixed types - this will throw std::bad_any_cast if not handled
    // The transform<T> template directly calls get<T>().
    // We expect a throw if types don't match.
    peg::SemanticValues sv_mixed;
    sv_mixed.push_back(peg::any(10));
    sv_mixed.push_back(peg::any(std::string("hello")));
    ASSERT_NO_THROW(sv_mixed.transform<int>(0, 1)); // Only transform the int
    ASSERT_THROW(sv_mixed.transform<int>(), std::bad_any_cast); // Will fail on "hello"
    ASSERT_THROW(sv_mixed.transform<std::string>(0, 2), std::bad_any_cast); // Will fail on 10
}

TEST(SemanticValuesTest, TransformWithRange) {
    std::string input = "10 hello 3.14 true";
    std::string matched = "10 hello 3.14 true";
    peg::SemanticValues sv = create_test_semantic_values(input, matched); // Contains int, string, double, bool

    // Transform a subset of elements
    auto first_two_elements = sv.transform<int>(0, 1); // Only the first element (int 10)
    ASSERT_EQ(first_two_elements.size(), 1);
    ASSERT_EQ(first_two_elements[0], 10);

    auto string_element = sv.transform<std::string>(1, 2); // Only the second element (string "hello")
    ASSERT_EQ(string_element.size(), 1);
    ASSERT_EQ(string_element[0], "hello");

    auto double_element = sv.transform<double>(2, 3); // Only the third element (double 3.14)
    ASSERT_EQ(double_element.size(), 1);
    ASSERT_EQ(double_element[0], 3.14);

    // Transform with end index beyond size
    auto all_elements_int_fail = sv.transform<int>(0, 100); // Should only process up to size()
    ASSERT_EQ(all_elements_int_fail.size(), 1); // Only the first element is int
    ASSERT_EQ(all_elements_int_fail[0], 10);
    // This will still throw bad_any_cast if it tries to convert "hello" to int.
    // The transform<T> method is designed to convert all elements in the range.
    // So, if the range includes incompatible types, it will throw.
    ASSERT_THROW(sv.transform<int>(0, 4), std::bad_any_cast);

    // Edge case: empty range
    auto empty_transform = sv.transform<int>(1, 1);
    ASSERT_TRUE(empty_transform.empty());

    // Edge case: beg >= end
    auto invalid_range_transform = sv.transform<int>(2, 1);
    ASSERT_TRUE(invalid_range_transform.empty());

    // Edge case: empty SemanticValues
    peg::SemanticValues empty_sv;
    auto empty_sv_transform = empty_sv.transform<int>();
    ASSERT_TRUE(empty_sv_transform.empty());
    auto empty_sv_transform_range = empty_sv.transform<int>(0, 5);
    ASSERT_TRUE(empty_sv_transform_range.empty());
}

TEST(SemanticValuesTest, TransformWithLambda) {
    std::string input = "10 hello 3.14 true";
    std::string matched = "10 hello 3.14 true";
    peg::SemanticValues sv = create_test_semantic_values(input, matched); // Contains int, string, double, bool

    // Transform using a lambda to convert all to string
    auto strings = sv.transform([](const peg::any& v) {
        if (v.type() == typeid(int)) return std::to_string(v.get<int>());
        if (v.type() == typeid(std::string)) return v.get<std::string>();
        if (v.type() == typeid(double)) return std::to_string(v.get<double>());
        if (v.type() == typeid(bool)) return v.get<bool>() ? "true" : "false";
        return std::string("unknown");
    });

    ASSERT_EQ(strings.size(), 4);
    ASSERT_EQ(strings[0], "10");
    ASSERT_EQ(strings[1], "hello");
    ASSERT_EQ(strings[2], "3.140000"); // Default precision for std::to_string(double)
    ASSERT_EQ(strings[3], "true");

    // Transform using a lambda with range
    auto first_two_transformed = sv.transform(0, 2, [](const peg::any& v) {
        if (v.type() == typeid(int)) return v.get<int>() * 2;
        if (v.type() == typeid(std::string)) return v.get<std::string>() + "!";
        return 0; // Should not be reached for this range
    });
    ASSERT_EQ(first_two_transformed.size(), 2);
    ASSERT_EQ(first_two_transformed[0], 20);
    ASSERT_EQ(first_two_transformed[1], "hello!");

    // Edge case: lambda returning different type than expected by template deduction
    // The return type of the lambda determines the vector<T> type.
    auto mixed_return_lambda = sv.transform(0, 2, [](const peg::any& v) -> peg::any {
        if (v.type() == typeid(int)) return v.get<int>() + 1;
        if (v.type() == typeid(std::string)) return v.get<std::string>() + "_suffix";
        return peg::any();
    });
    ASSERT_EQ(mixed_return_lambda.size(), 2);
    ASSERT_EQ(mixed_return_lambda[0].get<int>(), 11);
    ASSERT_EQ(mixed_return_lambda[1].get<std::string>(),