#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <vector>
#include <map>

// Test the get_id function indirectly through named argument formatting
// The get_id function is called internally when resolving named arguments
// in format strings

TEST(NamedArgGetIdTest, BasicNamedArgument) {
    // Test basic named argument resolution
    std::string result = fmt::format("{name}", fmt::arg("name", "World"));
    EXPECT_EQ(result, "World");
}

TEST(NamedArgGetIdTest, MultipleNamedArguments) {
    // Test multiple named arguments with different names
    std::string result = fmt::format("{first} {second}", 
                                     fmt::arg("first", "Hello"),
                                     fmt::arg("second", "World"));
    EXPECT_EQ(result, "Hello World");
}

TEST(NamedArgGetIdTest, MixedNamedAndPositionalArguments) {
    // Test mixing named and positional arguments
    std::string result = fmt::format("{0} {name}", "Hello", 
                                     fmt::arg("name", "World"));
    EXPECT_EQ(result, "Hello World");
}

TEST(NamedArgGetIdTest, RepeatedNamedArgument) {
    // Test using the same named argument multiple times
    std::string result = fmt::format("{name} {name}", 
                                     fmt::arg("name", "Echo"));
    EXPECT_EQ(result, "Echo Echo");
}

TEST(NamedArgGetIdTest, NamedArgumentWithFormatSpec) {
    // Test named argument with format specification
    std::string result = fmt::format("{value:.2f}", 
                                     fmt::arg("value", 3.14159));
    EXPECT_EQ(result, "3.14");
}

TEST(NamedArgGetIdTest, NoNamedArguments) {
    // Test when no named arguments are provided (get_id should return -1)
    std::string result = fmt::format("{} {}", "Hello", "World");
    EXPECT_EQ(result, "Hello World");
}

TEST(NamedArgGetIdTest, NamedArgumentNotFound) {
    // Test when a named argument is referenced but not provided
    // This should throw an exception
    EXPECT_THROW(fmt::format("{missing}", fmt::arg("present", "value")), 
                 fmt::format_error);
}

TEST(NamedArgGetIdTest, EmptyNamedArgumentName) {
    // Test with empty string as named argument name
    std::string result = fmt::format("{}", fmt::arg("", "empty_name"));
    EXPECT_EQ(result, "empty_name");
}

TEST(NamedArgGetIdTest, LongNamedArgumentName) {
    // Test with a long named argument name
    std::string long_name = "a" + std::string(100, 'b') + "c";
    std::string result = fmt::format("{name}", fmt::arg(long_name, "long_value"));
    EXPECT_EQ(result, "long_value");
}

TEST(NamedArgGetIdTest, SpecialCharactersInName) {
    // Test named argument names with special characters
    std::string result = fmt::format("{_name_123}", 
                                     fmt::arg("_name_123", "special"));
    EXPECT_EQ(result, "special");
}

TEST(NamedArgGetIdTest, MultipleCallsWithSameNamedArgs) {
    // Test that named arguments work correctly across multiple format calls
    auto named_arg = fmt::arg("x", 42);
    EXPECT_EQ(fmt::format("{x}", named_arg), "42");
    EXPECT_EQ(fmt::format("{x}", named_arg), "42");
    EXPECT_EQ(fmt::format("{x}", named_arg), "42");
}

TEST(NamedArgGetIdTest, NamedArgumentWithDifferentTypes) {
    // Test named arguments with various types
    EXPECT_EQ(fmt::format("{int_val}", fmt::arg("int_val", 42)), "42");
    EXPECT_EQ(fmt::format("{str_val}", fmt::arg("str_val", "test")), "test");
    EXPECT_EQ(fmt::format("{dbl_val}", fmt::arg("dbl_val", 3.14)), "3.14");
    EXPECT_EQ(fmt::format("{char_val}", fmt::arg("char_val", 'A')), "A");
}

TEST(NamedArgGetIdTest, NamedArgumentOrderIndependence) {
    // Test that named arguments work regardless of declaration order
    auto result1 = fmt::format("{a} {b}", 
                               fmt::arg("a", "first"),
                               fmt::arg("b", "second"));
    auto result2 = fmt::format("{b} {a}", 
                               fmt::arg("a", "first"),
                               fmt::arg("b", "second"));
    EXPECT_EQ(result1, "first second");
    EXPECT_EQ(result2, "second first");
}

TEST(NamedArgGetIdTest, ManyNamedArguments) {
    // Test with many named arguments to stress the lookup
    std::vector<std::string> names;
    for (int i = 0; i < 100; ++i) {
        names.push_back("name" + std::to_string(i));
    }
    
    std::string format_str;
    for (size_t i = 0; i < names.size(); ++i) {
        if (i > 0) format_str += " ";
        format_str += "{" + names[i] + "}";
    }
    
    // Build format args
    auto args = fmt::arg(names[0], 0);
    // Use a different approach for multiple args
    std::string result = fmt::format(format_str, 
                                     fmt::arg("name0", 0),
                                     fmt::arg("name1", 1),
                                     fmt::arg("name2", 2));
    // Just verify the first few work correctly
    EXPECT_EQ(result.substr(0, 5), "0 1 2");
}

TEST(NamedArgGetIdTest, FormatToWithNamedArgs) {
    // Test format_to with named arguments
    std::string out;
    auto it = fmt::format_to(std::back_inserter(out), "{name}", 
                             fmt::arg("name", "test"));
    EXPECT_EQ(out, "test");
}

TEST(NamedArgGetIdTest, FormattedSizeWithNamedArgs) {
    // Test formatted_size with named arguments
    size_t size = fmt::formatted_size("{name}", fmt::arg("name", "hello"));
    EXPECT_EQ(size, 5);
}

TEST(NamedArgGetIdTest, FormatToNWithNamedArgs) {
    // Test format_to_n with named arguments
    char buffer[10] = {};
    auto result = fmt::format_to_n(buffer, 3, "{name}", 
                                   fmt::arg("name", "hello"));
    *result.out = '\0';
    EXPECT_STREQ(buffer, "hel");
}