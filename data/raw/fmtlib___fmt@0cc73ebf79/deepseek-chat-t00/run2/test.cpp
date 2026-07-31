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
    std::string result = fmt::format("{name}", fmt::arg("name", "Alice"));
    EXPECT_EQ(result, "Alice");
}

// Test that get_id returns -1 for non-existent named argument
TEST_F(NamedArgIdTest, NamedArgNotFound) {
    // This should throw because "unknown" doesn't exist
    EXPECT_THROW(fmt::format("{unknown}", fmt::arg("name", "Bob")), fmt::format_error);
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
    std::string long_name = "a" + std::string(100, 'b') + "c";
    std::string result = fmt::format("{long_name}", fmt::arg(long_name, "long_value"));
    EXPECT_EQ(result, "long_value");
}

// Test that get_id works with special characters in names
TEST_F(NamedArgIdTest, SpecialCharactersInName) {
    std::string result = fmt::format("{_name_123}", fmt::arg("_name_123", "special"));
    EXPECT_EQ(result, "special");
}

// Test that get_id returns correct ID when multiple named args exist
TEST_F(NamedArgIdTest, CorrectIdForMultipleArgs) {
    std::string result = fmt::format("{b} {a}", 
                                     fmt::arg("a", "first"),
                                     fmt::arg("b", "second"));
    EXPECT_EQ(result, "second first");
}

// Test that get_id works with mixed positional and named arguments
TEST_F(NamedArgIdTest, MixedPositionalAndNamed) {
    std::string result = fmt::format("{0} {name}", 42, fmt::arg("name", "test"));
    EXPECT_EQ(result, "42 test");
}

// Test that get_id handles duplicate named arguments (last one wins)
TEST_F(NamedArgIdTest, DuplicateNamedArgs) {
    std::string result = fmt::format("{name}", 
                                     fmt::arg("name", "first"),
                                     fmt::arg("name", "second"));
    EXPECT_EQ(result, "second");
}

// Test that get_id works with Unicode characters in names
TEST_F(NamedArgIdTest, UnicodeName) {
    std::string result = fmt::format("{名前}", fmt::arg("名前", "日本語"));
    EXPECT_EQ(result, "日本語");
}

// Test that get_id works with numeric names
TEST_F(NamedArgIdTest, NumericName) {
    std::string result = fmt::format("{123}", fmt::arg("123", "numeric"));
    EXPECT_EQ(result, "numeric");
}

// Test that get_id returns -1 when no named args but format string has named placeholders
TEST_F(NamedArgIdTest, NamedPlaceholderWithoutNamedArg) {
    EXPECT_THROW(fmt::format("{name}", 42), fmt::format_error);
}

// Test that get_id works with many named arguments
TEST_F(NamedArgIdTest, ManyNamedArgs) {
    std::string result = fmt::format("{a}{b}{c}{d}{e}",
                                     fmt::arg("a", "1"),
                                     fmt::arg("b", "2"),
                                     fmt::arg("c", "3"),
                                     fmt::arg("d", "4"),
                                     fmt::arg("e", "5"));
    EXPECT_EQ(result, "12345");
}

// Test that get_id works with names that are substrings of each other
TEST_F(NamedArgIdTest, SubstringNames) {
    std::string result = fmt::format("{ab} {a}",
                                     fmt::arg("a", "short"),
                                     fmt::arg("ab", "long"));
    EXPECT_EQ(result, "long short");
}

// Test that get_id works with names containing underscores
TEST_F(NamedArgIdTest, UnderscoreNames) {
    std::string result = fmt::format("{my_var} {my_var_2}",
                                     fmt::arg("my_var", "first"),
                                     fmt::arg("my_var_2", "second"));
    EXPECT_EQ(result, "first second");
}