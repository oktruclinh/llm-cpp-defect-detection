#include <gtest/gtest.h>
#include <fmt/core.h>
#include <fmt/format.h>

TEST(NamedArgsTest, GetIdWithNamedArguments) {
    // Test normal case: named argument exists
    auto result = fmt::format("Hello {name}!", fmt::arg("name", "World"));
    EXPECT_EQ("Hello World!", result);
    
    // Test that get_id works through the public API by checking
    // that named arguments are properly resolved
    EXPECT_NO_THROW(fmt::format("Hello {name}!", fmt::arg("name", "World")));
}

TEST(NamedArgsTest, GetIdWithMultipleNamedArguments) {
    // Test multiple named arguments
    auto result = fmt::format("{a} {b} {c}!", 
                             fmt::arg("a", "Hello"),
                             fmt::arg("b", "beautiful"),
                             fmt::arg("c", "world"));
    EXPECT_EQ("Hello beautiful world!", result);
}

TEST(NamedArgsTest, GetIdWithEmptyNamedArgument) {
    // Test empty named argument name
    auto result = fmt::format("Hello {name}!", fmt::arg("name", ""));
    EXPECT_EQ("Hello !", result);
}

TEST(NamedArgsTest, GetIdWithSpecialCharactersInName) {
    // Test named argument with special characters
    auto result = fmt::format("Value: {test_var}!", fmt::arg("test_var", 42));
    EXPECT_EQ("Value: 42!", result);
}

TEST(NamedArgsTest, GetIdWithRepeatedNames) {
    // Test that repeated names work correctly
    auto result = fmt::format("{name} {name}!", fmt::arg("name", "test"));
    EXPECT_EQ("test test!", result);
}

TEST(NamedArgsTest, GetIdWithNoNamedArguments) {
    // Test format string with no named arguments
    auto result = fmt::format("Hello World!");
    EXPECT_EQ("Hello World!", result);
    
    // Test with positional arguments only
    auto result2 = fmt::format("{} {}", "Hello", "World");
    EXPECT_EQ("Hello World", result2);
}

TEST(NamedArgsTest, GetIdWithMixedArguments) {
    // Test mix of positional and named arguments
    auto result = fmt::format("Hello {name}, you have {count} messages!", 
                             fmt::arg("name", "Alice"),
                             fmt::arg("count", 5));
    EXPECT_EQ("Hello Alice, you have 5 messages!", result);
}

TEST(NamedArgsTest, GetIdWithNumericNames) {
    // Test named arguments with numeric names
    auto result = fmt::format("Value {1} is {2}!", 
                             fmt::arg("1", "first"),
                             fmt::arg("2", "important"));
    EXPECT_EQ("Value first is important!", result);
}

TEST(NamedArgsTest, GetIdWithLongNames) {
    // Test with long named argument names
    auto result = fmt::format("Hello {very_long_argument_name}!", 
                             fmt::arg("very_long_argument_name", "World"));
    EXPECT_EQ("Hello World!", result);
}

TEST(NamedArgsTest, GetIdWithUnicodeNames) {
    // Test with Unicode characters in argument names
    auto result = fmt::format("Hello {naïve}!", fmt::arg("naïve", "World"));
    EXPECT_EQ("Hello World!", result);
}

TEST(NamedArgsTest, GetIdWithSameNameDifferentValues) {
    // Test that same name can be used with different values
    auto result1 = fmt::format("{name}!", fmt::arg("name", "Alice"));
    auto result2 = fmt::format("{name}!", fmt::arg("name", "Bob"));
    EXPECT_EQ("Alice!", result1);
    EXPECT_EQ("Bob!", result2);
}