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
    // The function is internal, so we test through public API behavior
    EXPECT_EQ(result, "42");
}

// Test that get_id correctly identifies named arguments
TEST_F(NamedArgIdTest, NamedArgFound) {
    std::string result = fmt::format("{name}", fmt::arg("name", "Alice"));
    EXPECT_EQ(result, "Alice");
}

// Test that get_id returns -1 for non-existent named argument
TEST_F(NamedArgIdTest, NamedArgNotFound) {
    // This should throw because "unknown" doesn't exist
    EXPECT_THROW(fmt::format("{unknown}", fmt::arg("name", "Bob")),
                 fmt::format_error);
}

// Test multiple named arguments with different names
TEST_F(NamedArgIdTest, MultipleNamedArgs) {
    std::string result = fmt::format("{first} {last}",
                                     fmt::arg("first", "John"),
                                     fmt::arg("last", "Doe"));
    EXPECT_EQ(result, "John Doe");
}

// Test that get_id works with empty string name
TEST_F(NamedArgIdTest, EmptyStringName) {
    std::string result = fmt::format("{}", fmt::arg("", "empty"));
    EXPECT_EQ(result, "empty");
}

// Test that get_id works with long names
TEST_F(NamedArgIdTest, LongName) {
    std::string long_name = "a_very_long_named_argument_that_exceeds_typical_buffer_sizes_1234567890";
    std::string result = fmt::format("{" + long_name + "}",
                                     fmt::arg(long_name, "long_value"));
    EXPECT_EQ(result, "long_value");
}

// Test that get_id works with special characters in names
TEST_F(NamedArgIdTest, SpecialCharactersInName) {
    std::string result = fmt::format("{_name_123}",
                                     fmt::arg("_name_123", "special"));
    EXPECT_EQ(result, "special");
}

// Test that get_id returns correct ID when multiple named args exist
TEST_F(NamedArgIdTest, CorrectIdAssignment) {
    // The order of named args should determine their IDs
    std::string result = fmt::format("{b} {a}",
                                     fmt::arg("a", "first"),
                                     fmt::arg("b", "second"));
    EXPECT_EQ(result, "second first");
}

// Test that get_id works with mixed positional and named arguments
TEST_F(NamedArgIdTest, MixedPositionalAndNamed) {
    std::string result = fmt::format("{} {name} {}",
                                     1,
                                     fmt::arg("name", "two"),
                                     3);
    EXPECT_EQ(result, "1 two 3");
}

// Test that get_id handles duplicate named arguments (last one wins)
TEST_F(NamedArgIdTest, DuplicateNamedArgs) {
    std::string result = fmt::format("{name}",
                                     fmt::arg("name", "first"),
                                     fmt::arg("name", "second"));
    EXPECT_EQ(result, "second");
}

// Test that get_id works with wide strings
TEST_F(NamedArgIdTest, WideStringNamedArg) {
    std::wstring result = fmt::format(L"{name}",
                                      fmt::arg(L"name", L"wide"));
    EXPECT_EQ(result, L"wide");
}

// Test that get_id works with string_view arguments
TEST_F(NamedArgIdTest, StringViewNamedArg) {
    std::string_view sv = "view";
    std::string result = fmt::format("{name}",
                                     fmt::arg("name", sv));
    EXPECT_EQ(result, "view");
}

// Test that get_id handles many named arguments correctly
TEST_F(NamedArgIdTest, ManyNamedArgs) {
    std::string result = fmt::format("{a}{b}{c}{d}{e}",
                                     fmt::arg("a", "1"),
                                     fmt::arg("b", "2"),
                                     fmt::arg("c", "3"),
                                     fmt::arg("d", "4"),
                                     fmt::arg("e", "5"));
    EXPECT_EQ(result, "12345");
}

// Test that get_id returns -1 when format string has no named placeholders
TEST_F(NamedArgIdTest, NoNamedPlaceholders) {
    std::string result = fmt::format("{} {}",
                                     fmt::arg("unused", "value"),
                                     1, 2);
    EXPECT_EQ(result, "1 2");
}

// Test that get_id works with Unicode characters in names
TEST_F(NamedArgIdTest, UnicodeNamedArg) {
    std::string result = fmt::format("{名前}",
                                     fmt::arg("名前", "unicode"));
    EXPECT_EQ(result, "unicode");
}