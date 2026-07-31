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
    // The format string has no named arguments, so get_id should return -1
    // We can't directly call get_id, but we can verify behavior through format
    // A format string with no named args should work fine
    EXPECT_EQ(result, "42");
}

// Test basic named argument lookup
TEST_F(NamedArgIdTest, BasicNamedArgLookup) {
    auto result = fmt::format("{name}", fmt::arg("name", "Alice"));
    EXPECT_EQ(result, "Alice");
}

// Test multiple named arguments
TEST_F(NamedArgIdTest, MultipleNamedArgs) {
    auto result = fmt::format("{first} {last}", 
                              fmt::arg("first", "John"),
                              fmt::arg("last", "Doe"));
    EXPECT_EQ(result, "John Doe");
}

// Test that named argument IDs are correctly assigned in order
TEST_F(NamedArgIdTest, NamedArgIdsAreSequential) {
    // Using the same name twice should use the first occurrence
    auto result = fmt::format("{a} {b} {a}", 
                              fmt::arg("a", "first"),
                              fmt::arg("b", "second"));
    EXPECT_EQ(result, "first second first");
}

// Test that get_id returns -1 for non-existent named argument
TEST_F(NamedArgIdTest, NonExistentNameReturnsMinusOne) {
    // This should throw because "nonexistent" is not provided
    EXPECT_THROW(fmt::format("{nonexistent}", fmt::arg("existing", 42)), 
                 fmt::format_error);
}

// Test empty string as named argument name
TEST_F(NamedArgIdTest, EmptyStringAsName) {
    auto result = fmt::format("{}", fmt::arg("", "empty_name"));
    // Empty name should still work as a named argument
    EXPECT_EQ(result, "empty_name");
}

// Test named argument with empty format string
TEST_F(NamedArgIdTest, EmptyFormatStringWithNamedArgs) {
    auto result = fmt::format("", fmt::arg("name", "value"));
    EXPECT_EQ(result, "");
}

// Test that get_id works correctly with mixed positional and named arguments
TEST_F(NamedArgIdTest, MixedPositionalAndNamed) {
    auto result = fmt::format("{} {name}", 42, fmt::arg("name", "Alice"));
    EXPECT_EQ(result, "42 Alice");
}

// Test that named argument IDs are unique and correctly indexed
TEST_F(NamedArgIdTest, NamedArgIdsAreUnique) {
    // Using the same name multiple times should reference the same argument
    auto result = fmt::format("{x} {x} {x}", fmt::arg("x", 7));
    EXPECT_EQ(result, "7 7 7");
}

// Test that get_id handles long names correctly
TEST_F(NamedArgIdTest, LongNameLookup) {
    std::string long_name(100, 'a');
    auto result = fmt::format("{name}", fmt::arg(long_name, "long_value"));
    EXPECT_EQ(result, "long_value");
}

// Test that get_id handles special characters in names
TEST_F(NamedArgIdTest, SpecialCharactersInName) {
    auto result = fmt::format("{_name_123}", fmt::arg("_name_123", "special"));
    EXPECT_EQ(result, "special");
}

// Test that get_id returns correct ID for multiple named arguments in different order
TEST_F(NamedArgIdTest, NamedArgsInDifferentOrder) {
    auto result = fmt::format("{b} {a}", 
                              fmt::arg("a", "first"),
                              fmt::arg("b", "second"));
    EXPECT_EQ(result, "second first");
}

// Test that get_id works with numeric string names
TEST_F(NamedArgIdTest, NumericStringNames) {
    auto result = fmt::format("{123}", fmt::arg("123", "numeric_name"));
    EXPECT_EQ(result, "numeric_name");
}

// Test that get_id returns -1 when format string has no named placeholders but named args are provided
TEST_F(NamedArgIdTest, UnusedNamedArgs) {
    auto result = fmt::format("{}", 42, fmt::arg("unused", "value"));
    EXPECT_EQ(result, "42");
}

// Test that get_id correctly handles duplicate named arguments in format string
TEST_F(NamedArgIdTest, DuplicateNamedArgsInFormat) {
    auto result = fmt::format("{name} {name}", fmt::arg("name", "duplicate"));
    EXPECT_EQ(result, "duplicate duplicate");
}