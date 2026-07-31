#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <stdexcept>

TEST(BasicContextTest, GetArgWithNamedArgument) {
    // Test normal case with named argument
    auto result = fmt::format("{foo}", fmt::arg("foo", 42));
    EXPECT_EQ(result, "42");
}

TEST(BasicContextTest, GetArgWithMultipleNamedArguments) {
    // Test with multiple named arguments
    auto result = fmt::format("{foo} {bar}", fmt::arg("foo", 1), fmt::arg("bar", 2));
    EXPECT_EQ(result, "1 2");
}

TEST(BasicContextTest, GetArgWithEmptyName) {
    // Test with empty argument name
    auto result = fmt::format("{foo}", fmt::arg("", 42));
    EXPECT_EQ(result, "42");
}

TEST(BasicContextTest, GetArgWithSpecialCharactersInName) {
    // Test with special characters in argument name
    auto result = fmt::format("{foo-bar}", fmt::arg("foo-bar", 42));
    EXPECT_EQ(result, "42");
}

TEST(BasicContextTest, GetArgWithNonExistentArgument) {
    // Test accessing non-existent argument should trigger error
    // We expect this to throw or handle error appropriately
    try {
        auto result = fmt::format("{missing}", fmt::arg("foo", 42));
        // If we reach here, it means the error wasn't properly handled
        FAIL() << "Expected error when accessing non-existent argument";
    } catch (const std::runtime_error& e) {
        // This is expected behavior
    }
}

TEST(BasicContextTest, GetArgWithNumericArgument) {
    // Test with numeric argument names
    auto result = fmt::format("{123}", fmt::arg("123", "test"));
    EXPECT_EQ(result, "test");
}

TEST(BasicContextTest, GetArgWithComplexFormat) {
    // Test complex formatting with named arguments
    auto result = fmt::format("{name} is {age} years old", 
                             fmt::arg("name", "Alice"), 
                             fmt::arg("age", 30));
    EXPECT_EQ(result, "Alice is 30 years old");
}

TEST(BasicContextTest, GetArgWithMixedArguments) {
    // Test mixing positional and named arguments
    auto result = fmt::format("Hello {0}, {name}!", 
                             fmt::arg("0", "World"), 
                             fmt::arg("name", "Alice"));
    EXPECT_EQ(result, "Hello World, Alice!");
}

TEST(BasicContextTest, GetArgWithRepeatedNames) {
    // Test with repeated argument names (should use first occurrence)
    auto result = fmt::format("{foo} {foo}", fmt::arg("foo", "test"));
    EXPECT_EQ(result, "test test");
}

TEST(BasicContextTest, GetArgWithEmptyFormatString) {
    // Test with empty format string
    auto result = fmt::format("", fmt::arg("foo", 42));
    EXPECT_EQ(result, "");
}

TEST(BasicContextTest, GetArgWithOnlyNamedArguments) {
    // Test with only named arguments and no positional
    auto result = fmt::format("{foo} {bar} {baz}", 
                             fmt::arg("foo", "a"), 
                             fmt::arg("bar", "b"), 
                             fmt::arg("baz", "c"));
    EXPECT_EQ(result, "a b c");
}

TEST(BasicContextTest, GetArgWithNestedFormatting) {
    // Test nested formatting scenarios
    auto result = fmt::format("{outer}", fmt::arg("outer", fmt::format("{inner}", fmt::arg("inner", "value"))));
    EXPECT_EQ(result, "value");
}

TEST(BasicContextTest, GetArgWithLargeStringArgument) {
    // Test with large string argument
    std::string large_str(1000, 'x');
    auto result = fmt::format("{test}", fmt::arg("test", large_str));
    EXPECT_EQ(result, large_str);
}

TEST(BasicContextTest, GetArgWithZeroValue) {
    // Test with zero value
    auto result = fmt::format("{zero}", fmt::arg("zero", 0));
    EXPECT_EQ(result, "0");
}

TEST(BasicContextTest, GetArgWithNegativeValue) {
    // Test with negative value
    auto result = fmt::format("{neg}", fmt::arg("neg", -42));
    EXPECT_EQ(result, "-42");
}

TEST(BasicContextTest, GetArgWithFloatValue) {
    // Test with floating point value
    auto result = fmt::format("{pi}", fmt::arg("pi", 3.14159));
    EXPECT_EQ(result, "3.14159");
}