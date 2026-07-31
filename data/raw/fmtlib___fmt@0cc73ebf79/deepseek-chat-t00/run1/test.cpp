#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <string_view>

// Test fixture for named argument ID lookup
class NamedArgIdTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Test that get_id returns -1 when there are no named arguments
TEST_F(NamedArgIdTest, NoNamedArgsReturnsMinusOne) {
    auto result = fmt::format("{}", 42);
    // We can't directly call get_id, but we can verify behavior through format
    // The function is internal, so we test through the public API
    EXPECT_EQ(result, "42");
}

// Test that get_id correctly identifies named arguments
TEST_F(NamedArgIdTest, NamedArgFound) {
    auto result = fmt::format("{name}", fmt::arg("name", "Alice"));
    EXPECT_EQ(result, "Alice");
}

// Test that get_id returns -1 for non-existent named argument
TEST_F(NamedArgIdTest, NamedArgNotFound) {
    // This should throw because "unknown" doesn't exist
    EXPECT_THROW(fmt::format("{unknown}", fmt::arg("name", "Bob")),
                 fmt::format_error);
}

// Test with multiple named arguments
TEST_F(NamedArgIdTest, MultipleNamedArgs) {
    auto result = fmt::format("{first} {last}",
                              fmt::arg("first", "John"),
                              fmt::arg("last", "Doe"));
    EXPECT_EQ(result, "John Doe");
}

// Test with empty string name
TEST_F(NamedArgIdTest, EmptyName) {
    auto result = fmt::format("{}", fmt::arg("", "empty"));
    EXPECT_EQ(result, "empty");
}

// Test with special characters in name
TEST_F(NamedArgIdTest, SpecialCharsInName) {
    auto result = fmt::format("{_name_123}", fmt::arg("_name_123", "special"));
    EXPECT_EQ(result, "special");
}

// Test that get_id works with mixed positional and named arguments
TEST_F(NamedArgIdTest, MixedPositionalAndNamed) {
    auto result = fmt::format("{} {name}", 1, fmt::arg("name", "two"));
    EXPECT_EQ(result, "1 two");
}

// Test that get_id works with repeated named arguments (last one wins)
TEST_F(NamedArgIdTest, RepeatedNamedArg) {
    auto result = fmt::format("{name}", fmt::arg("name", "first"),
                              fmt::arg("name", "second"));
    EXPECT_EQ(result, "second");
}

// Test with wide string names
TEST_F(NamedArgIdTest, WideStringName) {
    auto result = fmt::format(L"{name}", fmt::arg(L"name", L"wide"));
    EXPECT_EQ(result, L"wide");
}

// Test with very long name
TEST_F(NamedArgIdTest, LongName) {
    std::string long_name(1000, 'a');
    auto result = fmt::format("{long}", fmt::arg(long_name, "long_value"));
    EXPECT_EQ(result, "long_value");
}

// Test that get_id handles names that are substrings of other names
TEST_F(NamedArgIdTest, SubstringNames) {
    auto result = fmt::format("{ab} {abc}",
                              fmt::arg("ab", "short"),
                              fmt::arg("abc", "long"));
    EXPECT_EQ(result, "short long");
}

// Test with numeric string names
TEST_F(NamedArgIdTest, NumericStringName) {
    auto result = fmt::format("{123}", fmt::arg("123", "numeric"));
    EXPECT_EQ(result, "numeric");
}

// Test that get_id works with format specifiers
TEST_F(NamedArgIdTest, NamedArgWithFormatSpec) {
    auto result = fmt::format("{value:.2f}", fmt::arg("value", 3.14159));
    EXPECT_EQ(result, "3.14");
}

// Test that get_id works with alignment
TEST_F(NamedArgIdTest, NamedArgWithAlignment) {
    auto result = fmt::format("{name:>10}", fmt::arg("name", "right"));
    EXPECT_EQ(result, "     right");
}

// Test that get_id works with fill and alignment
TEST_F(NamedArgIdTest, NamedArgWithFillAndAlign) {
    auto result = fmt::format("{name:*^10}", fmt::arg("name", "center"));
    EXPECT_EQ(result, "**center**");
}

// Test with multiple format specifiers and named args
TEST_F(NamedArgIdTest, MultipleNamedArgsWithSpecs) {
    auto result = fmt::format("{a:>5} {b:.1f}",
                              fmt::arg("a", "left"),
                              fmt::arg("b", 3.14159));
    EXPECT_EQ(result, " left 3.1");
}

// Test that get_id handles names with underscores and digits
TEST_F(NamedArgIdTest, ComplexNamePattern) {
    auto result = fmt::format("{_a1_b2_c3_}", fmt::arg("_a1_b2_c3_", "complex"));
    EXPECT_EQ(result, "complex");
}

// Test that get_id returns correct ID for first named argument
TEST_F(NamedArgIdTest, FirstNamedArgId) {
    auto result = fmt::format("{a} {b} {c}",
                              fmt::arg("a", 1),
                              fmt::arg("b", 2),
                              fmt::arg("c", 3));
    EXPECT_EQ(result, "1 2 3");
}

// Test that get_id handles names that are prefixes of other names
TEST_F(NamedArgIdTest, PrefixNames) {
    auto result = fmt::format("{pre} {prefix}",
                              fmt::arg("pre", "short"),
                              fmt::arg("prefix", "long"));
    EXPECT_EQ(result, "short long");
}

// Test with Unicode characters in name (if Char supports it)
TEST_F(NamedArgIdTest, UnicodeName) {
    auto result = fmt::format("{café}", fmt::arg("café", "coffee"));
    EXPECT_EQ(result, "coffee");
}

// Test that get_id works with const char* names
TEST_F(NamedArgIdTest, ConstCharName) {
    auto result = fmt::format("{name}", fmt::arg("name", "value"));
    EXPECT_EQ(result, "value");
}

// Test that get_id works with std::string names
TEST_F(NamedArgIdTest, StdStringName) {
    std::string name = "dynamic_name";
    auto result = fmt::format("{dynamic}", fmt::arg(name, "dynamic_value"));
    EXPECT_EQ(result, "dynamic_value");
}

// Test that get_id works with std::string_view names
TEST_F(NamedArgIdTest, StringViewName) {
    std::string_view sv = "sv_name";
    auto result = fmt::format("{sv}", fmt::arg(sv, "sv_value"));
    EXPECT_EQ(result, "sv_value");
}