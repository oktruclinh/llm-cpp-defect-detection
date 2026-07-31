#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <stdexcept>

TEST(BasicContextTest, GetArgWithNamedArgument) {
    // Test normal case with named argument
    auto result = fmt::format("{first} {second}", 
                             fmt::arg("first", "Hello"), 
                             fmt::arg("second", "World"));
    EXPECT_EQ(result, "Hello World");
}

TEST(BasicContextTest, GetArgWithMissingNamedArgument) {
    // Test error case when argument is not found
    EXPECT_THROW(
        fmt::format("{missing}", fmt::arg("present", "value")),
        std::runtime_error
    );
}

TEST(BasicContextTest, GetArgWithEmptyName) {
    // Test with empty argument name
    auto result = fmt::format("{empty}", fmt::arg("", "value"));
    EXPECT_EQ(result, "value");
}

TEST(BasicContextTest, GetArgWithMultipleArguments) {
    // Test multiple named arguments
    auto result = fmt::format("{a} {b} {c}", 
                             fmt::arg("a", 1), 
                             fmt::arg("b", 2.5), 
                             fmt::arg("c", "test"));
    EXPECT_EQ(result, "1 2.5 test");
}

TEST(BasicContextTest, GetArgWithNumericArguments) {
    // Test numeric argument names (should work as strings)
    auto result = fmt::format("{0} {1}", fmt::arg("0", "zero"), fmt::arg("1", "one"));
    EXPECT_EQ(result, "zero one");
}

TEST(BasicContextTest, GetArgWithSpecialCharacters) {
    // Test argument names with special characters
    auto result = fmt::format("{hello-world} {foo.bar}", 
                             fmt::arg("hello-world", "test1"), 
                             fmt::arg("foo.bar", "test2"));
    EXPECT_EQ(result, "test1 test2");
}

TEST(BasicContextTest, GetArgWithRepeatedNames) {
    // Test that repeated argument names work correctly
    auto result = fmt::format("{name} {name}", fmt::arg("name", "test"));
    EXPECT_EQ(result, "test test");
}

TEST(BasicContextTest, GetArgWithComplexTypes) {
    // Test with complex types
    std::string str = "hello";
    auto result = fmt::format("{str} {num}", 
                             fmt::arg("str", str), 
                             fmt::arg("num", 42));
    EXPECT_EQ(result, "hello 42");
}

TEST(BasicContextTest, GetArgWithZeroValues) {
    // Test with zero values
    auto result = fmt::format("{zero} {empty}", 
                             fmt::arg("zero", 0), 
                             fmt::arg("empty", ""));
    EXPECT_EQ(result, "0 ");
}

TEST(BasicContextTest, GetArgWithOnlyNamedArguments) {
    // Test format string with only named arguments
    auto result = fmt::format("{greeting} {subject}!", 
                             fmt::arg("greeting", "Hello"), 
                             fmt::arg("subject", "World"));
    EXPECT_EQ(result, "Hello World!");
}

TEST(BasicContextTest, GetArgWithMixedArguments) {
    // Test mixed positional and named arguments
    auto result = fmt::format("{} {named} {}", 
                             "pos1", 
                             fmt::arg("named", "named_value"), 
                             "pos2");
    EXPECT_EQ(result, "pos1 named_value pos2");
}