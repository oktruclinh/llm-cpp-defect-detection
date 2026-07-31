#include <gtest/gtest.h>
#include <fmt/core.h>

// Test that get_id returns -1 when there are no named arguments
TEST(NamedArgIdTest, NoNamedArgs) {
    auto result = fmt::format("{} {}", 1, 2);
    // We can't directly call get_id, but we can verify behavior through format
    // The function is internal, so we test through the public API
    EXPECT_EQ(result, "1 2");
}

// Test that get_id returns correct id for named arguments
TEST(NamedArgIdTest, SingleNamedArg) {
    auto result = fmt::format("{name}", fmt::arg("name", "Alice"));
    EXPECT_EQ(result, "Alice");
}

// Test multiple named arguments with different names
TEST(NamedArgIdTest, MultipleNamedArgs) {
    auto result = fmt::format("{first} {last}", 
                              fmt::arg("first", "John"),
                              fmt::arg("last", "Doe"));
    EXPECT_EQ(result, "John Doe");
}

// Test that get_id returns correct id for named argument used multiple times
TEST(NamedArgIdTest, RepeatedNamedArg) {
    auto result = fmt::format("{name} {name}", fmt::arg("name", "Bob"));
    EXPECT_EQ(result, "Bob Bob");
}

// Test that get_id returns -1 for non-existent named argument
TEST(NamedArgIdTest, NonExistentNamedArg) {
    // This should throw or produce unexpected output since the named arg doesn't exist
    EXPECT_THROW(fmt::format("{missing}", fmt::arg("present", "value")), 
                 fmt::format_error);
}

// Test empty string as named argument name
TEST(NamedArgIdTest, EmptyName) {
    auto result = fmt::format("{}", fmt::arg("", "empty"));
    EXPECT_EQ(result, "empty");
}

// Test named argument with special characters in name
TEST(NamedArgIdTest, SpecialCharsInName) {
    auto result = fmt::format("{a_b}", fmt::arg("a_b", "underscore"));
    EXPECT_EQ(result, "underscore");
}

// Test mixing positional and named arguments
TEST(NamedArgIdTest, MixedPositionalAndNamed) {
    auto result = fmt::format("{} {name} {}", 1, fmt::arg("name", "two"), 3);
    EXPECT_EQ(result, "1 two 3");
}

// Test that get_id works correctly with multiple named arguments in different order
TEST(NamedArgIdTest, NamedArgsDifferentOrder) {
    auto result = fmt::format("{b} {a}", 
                              fmt::arg("a", "first"),
                              fmt::arg("b", "second"));
    EXPECT_EQ(result, "second first");
}

// Test that get_id handles many named arguments correctly
TEST(NamedArgIdTest, ManyNamedArgs) {
    auto result = fmt::format("{a}{b}{c}{d}{e}",
                              fmt::arg("a", "1"),
                              fmt::arg("b", "2"),
                              fmt::arg("c", "3"),
                              fmt::arg("d", "4"),
                              fmt::arg("e", "5"));
    EXPECT_EQ(result, "12345");
}

// Test that get_id returns -1 when format string has no named placeholders
TEST(NamedArgIdTest, NoNamedPlaceholders) {
    auto result = fmt::format("{}", fmt::arg("unused", "value"));
    EXPECT_EQ(result, "value");
}

// Test that get_id works with wide strings
TEST(NamedArgIdTest, WideStringNamedArg) {
    auto result = fmt::format(L"{name}", fmt::arg(L"name", L"wide"));
    EXPECT_EQ(result, L"wide");
}

// Test that get_id works with mixed char types (should fail or work depending on implementation)
TEST(NamedArgIdTest, MixedCharTypes) {
    // This should fail because char types don't match
    EXPECT_THROW(fmt::format("{name}", fmt::arg(L"name", L"wide")), 
                 fmt::format_error);
}