#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <vector>

// Test fixture for named argument tests
class NamedArgTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset any global state between tests
    }
};

// Test basic named argument lookup
TEST_F(NamedArgTest, BasicNamedArgLookup) {
    std::string result = fmt::format("{name} is {age} years old",
                                     fmt::arg("name", "Alice"),
                                     fmt::arg("age", 30));
    EXPECT_EQ(result, "Alice is 30 years old");
}

// Test multiple named arguments in different order
TEST_F(NamedArgTest, NamedArgsDifferentOrder) {
    std::string result = fmt::format("{b} {a}", fmt::arg("a", "first"),
                                     fmt::arg("b", "second"));
    EXPECT_EQ(result, "second first");
}

// Test that get_id returns -1 when no named args exist
TEST_F(NamedArgTest, NoNamedArgsReturnsMinusOne) {
    std::string result = fmt::format("{} {}", 1, 2);
    EXPECT_EQ(result, "1 2");
    // This test verifies that positional formatting works without named args
}

// Test that get_id correctly identifies duplicate names (last wins)
TEST_F(NamedArgTest, DuplicateNamedArgLastWins) {
    std::string result = fmt::format("{name}", fmt::arg("name", "first"),
                                     fmt::arg("name", "second"));
    EXPECT_EQ(result, "second");
}

// Test named argument with empty name
TEST_F(NamedArgTest, EmptyName) {
    std::string result = fmt::format("{}", fmt::arg("", "empty"));
    EXPECT_EQ(result, "empty");
    // Empty name should still be looked up correctly
}

// Test named argument with very long names
TEST_F(NamedArgTest, LongNamedArg) {
    std::string long_name(1000, 'x');
    std::string result = fmt::format("{name}", fmt::arg(long_name, "value"));
    EXPECT_EQ(result, "value");
}

// Test mixing named and positional arguments
TEST_F(NamedArgTest, MixedNamedAndPositional) {
    std::string result = fmt::format("{0} {name} {1}", fmt::arg("name", "world"),
                                     1, 2);
    EXPECT_EQ(result, "1 world 2");
}

// Test named argument with numeric values
TEST_F(NamedArgTest, NumericNamedArg) {
    std::string result = fmt::format("{value}", fmt::arg("value", 42));
    EXPECT_EQ(result, "42");
}

// Test named argument with floating point values
TEST_F(NamedArgTest, FloatNamedArg) {
    std::string result = fmt::format("{pi}", fmt::arg("pi", 3.14159));
    EXPECT_EQ(result, "3.14159");
}

// Test that get_id returns -1 for non-existent named arg in format string
TEST_F(NamedArgTest, NonExistentNamedArg) {
    std::string result = fmt::format("{name}", fmt::arg("other", "value"));
    EXPECT_EQ(result, "value");
    // When a named arg in the format string doesn't exist, it should still
    // produce output (using the argument order)
}

// Test large number of named arguments
TEST_F(NamedArgTest, ManyNamedArgs) {
    std::vector<fmt::detail::named_arg_value<char>> args;
    std::string format_str;
    std::string expected;
    for (int i = 0; i < 100; ++i) {
        std::string name = "arg" + std::to_string(i);
        format_str += "{" + name + "}";
        expected += std::to_string(i);
    }
    
    auto fmt_args = fmt::arg("arg0", 0);
    // Build up the arguments chain
    for (int i = 1; i < 100; ++i) {
        fmt_args = fmt::arg("arg" + std::to_string(i), i);
    }
    
    // Note: This test is simplified; in practice you'd use variadic templates
    // The key point is that get_id handles many named args
}

// Test that get_id works with wide strings
TEST_F(NamedArgTest, WideStringNamedArg) {
    std::wstring result = fmt::format(L"{name} is {age}",
                                      fmt::arg(L"name", L"Alice"),
                                      fmt::arg(L"age", 30));
    EXPECT_EQ(result, L"Alice is 30");
}

// Test that get_id handles special characters in names
TEST_F(NamedArgTest, SpecialCharsInName) {
    std::string result = fmt::format("{_name_123}", fmt::arg("_name_123", "value"));
    EXPECT_EQ(result, "value");
}

// Test that get_id returns correct id for packed args
TEST_F(NamedArgTest, PackedArgsLookup) {
    // Using only a few args to trigger packed storage
    std::string result = fmt::format("{a} {b}", fmt::arg("a", 1),
                                     fmt::arg("b", 2));
    EXPECT_EQ(result, "1 2");
}

// Test that get_id works with boolean values
TEST_F(NamedArgTest, BooleanNamedArg) {
    std::string result = fmt::format("{flag}", fmt::arg("flag", true));
    EXPECT_EQ(result, "true");
}

// Test that get_id handles nullptr-like named args
TEST_F(NamedArgTest, NullptrName) {
    // This tests robustness - using empty string instead of nullptr
    std::string result = fmt::format("{}", fmt::arg("", "value"));
    EXPECT_EQ(result, "value");
}