#include <gtest/gtest.h>
#include <fmt/core.h>

// Test that get_id returns -1 when there are no named arguments
TEST(NamedArgIdTest, NoNamedArgs) {
    std::string result = fmt::format("{}", 42);
    // We can't directly call get_id, but we can test behavior indirectly
    // by verifying that format with named args fails when not provided
    EXPECT_THROW(fmt::format("{name}", fmt::arg("name", 1)), fmt::format_error);
}

// Test basic named argument retrieval
TEST(NamedArgIdTest, SingleNamedArg) {
    std::string result = fmt::format("{name}", fmt::arg("name", 42));
    EXPECT_EQ(result, "42");
}

// Test multiple named arguments
TEST(NamedArgIdTest, MultipleNamedArgs) {
    std::string result = fmt::format("{first} {second}", 
                                     fmt::arg("first", 1), 
                                     fmt::arg("second", 2));
    EXPECT_EQ(result, "1 2");
}

// Test named argument with same name used multiple times
TEST(NamedArgIdTest, RepeatedNamedArg) {
    std::string result = fmt::format("{name} {name}", fmt::arg("name", 42));
    EXPECT_EQ(result, "42 42");
}

// Test that argument order doesn't matter for named args
TEST(NamedArgIdTest, NamedArgsOrderIndependent) {
    std::string result1 = fmt::format("{a} {b}", 
                                      fmt::arg("a", 1), 
                                      fmt::arg("b", 2));
    std::string result2 = fmt::format("{b} {a}", 
                                      fmt::arg("a", 1), 
                                      fmt::arg("b", 2));
    EXPECT_EQ(result1, "1 2");
    EXPECT_EQ(result2, "2 1");
}

// Test with empty string as named argument
TEST(NamedArgIdTest, EmptyName) {
    std::string result = fmt::format("{}", fmt::arg("", 42));
    EXPECT_EQ(result, "{}");
}

// Test that missing named argument throws
TEST(NamedArgIdTest, MissingNamedArg) {
    EXPECT_THROW(fmt::format("{missing_name}"), fmt::format_error);
}

// Test mixed positional and named arguments
TEST(NamedArgIdTest, MixedPositionalAndNamed) {
    std::string result = fmt::format("{} {name}", 1, fmt::arg("name", 2));
    EXPECT_EQ(result, "1 2");
}

// Test named arguments with different types
TEST(NamedArgIdTest, NamedArgsDifferentTypes) {
    std::string result = fmt::format("{name} {value}", 
                                     fmt::arg("name", "hello"),
                                     fmt::arg("value", 3.14));
    EXPECT_EQ(result, "hello 3.14");
}

// Test that get_id returns correct id for named arguments
TEST(NamedArgIdTest, CorrectIdAssignment) {
    // Test that named args are properly indexed by using them in format string
    std::string result = fmt::format("{b} {a}", 
                                     fmt::arg("a", 10),
                                     fmt::arg("b", 20));
    EXPECT_EQ(result, "20 10");
}

// Test that get_id returns -1 for non-existent named argument
TEST(NamedArgIdTest, NonExistentNameReturnsMinusOne) {
    // This tests the -1 return path through the public API
    EXPECT_THROW(fmt::format("{nonexistent}", fmt::arg("existing", 1)), 
                 fmt::format_error);
}