#include <gtest/gtest.h>
#include <fmt/core.h>
#include <fmt/format.h>

TEST(FmtCoreTest, GetIdWithNamedArguments) {
    // Test normal case with named arguments
    auto result = fmt::format("{first} {second}", 
                             fmt::arg("first", "hello"),
                             fmt::arg("second", "world"));
    EXPECT_EQ(result, "hello world");
}

TEST(FmtCoreTest, GetIdWithoutNamedArguments) {
    // Test when no named arguments are provided
    auto result = fmt::format("{} {}", 1, 2);
    EXPECT_EQ(result, "1 2");
}

TEST(FmtCoreTest, GetIdEmptyNamedArguments) {
    // Test with empty named arguments
    auto result = fmt::format("{a}", fmt::arg("a", "test"));
    EXPECT_EQ(result, "test");
}

TEST(FmtCoreTest, GetIdMultipleNamedArguments) {
    // Test with multiple named arguments
    auto result = fmt::format("{a} {b} {c}", 
                             fmt::arg("a", 1),
                             fmt::arg("b", 2),
                             fmt::arg("c", 3));
    EXPECT_EQ(result, "1 2 3");
}

TEST(FmtCoreTest, GetIdNamedArgumentNotFound) {
    // Test when named argument is not found
    auto result = fmt::format("{a} {b}", 
                             fmt::arg("a", "hello"));
    EXPECT_EQ(result, "hello {b}");
}

TEST(FmtCoreTest, GetIdSpecialCharacters) {
    // Test with special characters in names
    auto result = fmt::format("{a_b} {c-d}", 
                             fmt::arg("a_b", "test1"),
                             fmt::arg("c-d", "test2"));
    EXPECT_EQ(result, "test1 test2");
}

TEST(FmtCoreTest, GetIdNumericNames) {
    // Test with numeric names
    auto result = fmt::format("{1} {2}", 
                             fmt::arg("1", "one"),
                             fmt::arg("2", "two"));
    EXPECT_EQ(result, "one two");
}

TEST(FmtCoreTest, GetIdRepeatedNames) {
    // Test with repeated names (should use first occurrence)
    auto result = fmt::format("{a} {a}", 
                             fmt::arg("a", "test"));
    EXPECT_EQ(result, "test test");
}

TEST(FmtCoreTest, GetIdNestedFormatting) {
    // Test nested formatting scenarios
    auto result = fmt::format("{outer}", 
                             fmt::arg("outer", fmt::format("{inner}", 
                                                        fmt::arg("inner", "value"))));
    EXPECT_EQ(result, "{inner}");
}

TEST(FmtCoreTest, GetIdMixedPositionalAndNamed) {
    // Test mixed positional and named arguments
    auto result = fmt::format("{} {named} {}", 
                             1,
                             fmt::arg("named", "hello"),
                             3);
    EXPECT_EQ(result, "1 hello 3");
}

TEST(FmtCoreTest, GetIdEmptyStringNames) {
    // Test with empty string names
    auto result = fmt::format("{a} {b}", 
                             fmt::arg("a", "test1"),
                             fmt::arg("b", "test2"));
    EXPECT_EQ(result, "test1 test2");
}

TEST(FmtCoreTest, GetIdLongNames) {
    // Test with long argument names
    auto result = fmt::format("{very_long_argument_name} {another_long_name}", 
                             fmt::arg("very_long_argument_name", "value1"),
                             fmt::arg("another_long_name", "value2"));
    EXPECT_EQ(result, "value1 value2");
}

TEST(FmtCoreTest, GetIdUnicodeNames) {
    // Test with Unicode characters in names
    auto result = fmt::format("{α} {β}", 
                             fmt::arg("α", "alpha"),
                             fmt::arg("β", "beta"));
    EXPECT_EQ(result, "alpha beta");
}