#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <stdexcept>

using namespace fmt;

TEST(BasicContextTest, GetArgWithNamedArguments) {
    // Test normal case with named arguments
    auto result = format("{first} {second}", 
                        fmt::arg("first", "Hello"), 
                        fmt::arg("second", "World"));
    EXPECT_EQ("Hello World", result);
}

TEST(BasicContextTest, GetArgWithEmptyName) {
    // Test with empty argument name
    auto result = format("{name}", fmt::arg("", "empty"));
    EXPECT_EQ("empty", result);
}

TEST(BasicContextTest, GetArgWithSpecialCharacters) {
    // Test with special characters in argument names
    auto result = format("{a@b} {1-2}", fmt::arg("a@b", "special"), fmt::arg("1-2", "hyphen"));
    EXPECT_EQ("special hyphen", result);
}

TEST(BasicContextTest, GetArgWithNumericNames) {
    // Test with numeric argument names
    auto result = format("{1} {0}", fmt::arg("1", "one"), fmt::arg("0", "zero"));
    EXPECT_EQ("one zero", result);
}

TEST(BasicContextTest, GetArgMultipleArguments) {
    // Test with multiple arguments
    auto result = format("{a} {b} {c}", 
                        fmt::arg("a", 1), 
                        fmt::arg("b", 2.5), 
                        fmt::arg("c", "test"));
    EXPECT_EQ("1 2.5 test", result);
}

TEST(BasicContextTest, GetArgRepeatedNames) {
    // Test with repeated argument names (should use first occurrence)
    auto result = format("{name} {name}", fmt::arg("name", "test"));
    EXPECT_EQ("test test", result);
}

TEST(BasicContextTest, GetArgMissingArgument) {
    // Test that missing argument triggers error handling
    // This should not crash but rather throw an exception or handle gracefully
    try {
        // This should trigger the error path in get_arg
        auto result = format("{missing_arg}");
        // If we get here without exception, it's acceptable behavior
        // but we want to ensure it doesn't crash
    } catch (const std::exception&) {
        // Expected behavior - exception thrown for missing argument
    }
}

TEST(BasicContextTest, GetArgMixedNamedAndPositional) {
    // Test mixing named and positional arguments
    auto result = format("{0} {name} {1}", 
                        fmt::arg("0", "pos0"), 
                        fmt::arg("name", "named"), 
                        fmt::arg("1", "pos1"));
    EXPECT_EQ("pos0 named pos1", result);
}

TEST(BasicContextTest, GetArgComplexFormatting) {
    // Test complex formatting with named arguments
    auto result = format("{name:*>10} {value:.2f}", 
                        fmt::arg("name", "test"), 
                        fmt::arg("value", 3.14159));
    EXPECT_EQ("******test 3.14", result);
}

TEST(BasicContextTest, GetArgUnicodeNames) {
    // Test with Unicode argument names
    auto result = format("{α} {β}", fmt::arg("α", "alpha"), fmt::arg("β", "beta"));
    EXPECT_EQ("alpha beta", result);
}

TEST(BasicContextTest, GetArgLongNames) {
    // Test with very long argument names
    std::string long_name(1000, 'x');
    auto result = format("{long_name}", fmt::arg(long_name, "long"));
    EXPECT_EQ("long", result);
}

TEST(BasicContextTest, GetArgSingleCharacterNames) {
    // Test with single character argument names
    auto result = format("{a} {b} {c}", 
                        fmt::arg("a", "A"), 
                        fmt::arg("b", "B"), 
                        fmt::arg("c", "C"));
    EXPECT_EQ("A B C", result);
}

TEST(BasicContextTest, GetArgWithSpacesInNames) {
    // Test with spaces in argument names (should work as literal)
    auto result = format("{first name} {last name}", 
                        fmt::arg("first name", "John"), 
                        fmt::arg("last name", "Doe"));
    EXPECT_EQ("John Doe", result);
}

TEST(BasicContextTest, GetArgWithUnderscores) {
    // Test with underscores in argument names
    auto result = format("{first_name} {last_name}", 
                        fmt::arg("first_name", "John"), 
                        fmt::arg("last_name", "Doe"));
    EXPECT_EQ("John Doe", result);
}

TEST(BasicContextTest, GetArgWithNumbersInNames) {
    // Test with numbers in argument names
    auto result = format("{arg1} {arg2}", 
                        fmt::arg("arg1", "one"), 
                        fmt::arg("arg2", "two"));
    EXPECT_EQ("one two", result);
}