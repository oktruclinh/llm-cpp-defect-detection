#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <string_view>

// Test fixture for named argument ID lookup tests
class NamedArgIdTest : public ::testing::Test {
protected:
    // Helper to create a format string with named arguments and extract IDs
    int get_named_arg_id(const std::string& format_str, const std::string& arg_name) {
        auto args = fmt::make_format_args(
            fmt::arg("a", 1),
            fmt::arg("b", 2.0),
            fmt::arg("c", "hello"),
            fmt::arg("d", 42)
        );
        auto format_store = fmt::dynamic_format_arg_store<fmt::format_context>();
        format_store.push_back(fmt::arg("a", 1));
        format_store.push_back(fmt::arg("b", 2.0));
        format_store.push_back(fmt::arg("c", "hello"));
        format_store.push_back(fmt::arg("d", 42));
        
        // Use the format string to trigger named argument processing
        auto result = fmt::vformat(format_str, format_store);
        
        // Access the internal named_args through the format_args
        // This is done by creating a format_args from the store
        auto args_view = fmt::basic_format_args<fmt::format_context>(format_store);
        
        // We need to get the named_args from the internal representation
        // This is accessed through the format_args' internal data
        // For testing, we'll use the format string with named args
        return 0; // Placeholder - actual test below
    }
};

// Test basic named argument ID lookup
TEST(NamedArgIdTest, BasicLookup) {
    auto args = fmt::make_format_args(
        fmt::arg("x", 10),
        fmt::arg("y", 20),
        fmt::arg("z", 30)
    );
    
    // Format with named arguments to trigger ID resolution
    std::string result = fmt::vformat("{x} {y} {z}", args);
    EXPECT_EQ(result, "10 20 30");
}

// Test that named arguments are correctly identified by name
TEST(NamedArgIdTest, NamedArgIdentification) {
    auto args = fmt::make_format_args(
        fmt::arg("first", 100),
        fmt::arg("second", 200)
    );
    
    // Verify that using correct names works
    std::string result = fmt::vformat("{first} {second}", args);
    EXPECT_EQ(result, "100 200");
    
    // Verify that swapping names would produce different output
    std::string swapped = fmt::vformat("{second} {first}", args);
    EXPECT_EQ(swapped, "200 100");
    EXPECT_NE(result, swapped);
}

// Test that empty named argument list returns empty/default
TEST(NamedArgIdTest, EmptyNamedArgs) {
    auto args = fmt::make_format_args();
    
    // Format string with no named arguments should work
    std::string result = fmt::vformat("{}", args);
    EXPECT_EQ(result, "");
}

// Test that non-existent named argument name returns empty/default
TEST(NamedArgIdTest, NonExistentName) {
    auto args = fmt::make_format_args(
        fmt::arg("existing", 42)
    );
    
    // Using a non-existent name should throw or produce empty
    EXPECT_THROW(fmt::vformat("{nonexistent}", args), fmt::format_error);
}

// Test multiple named arguments with same value but different names
TEST(NamedArgIdTest, MultipleNamedArgs) {
    auto args = fmt::make_format_args(
        fmt::arg("a", 1),
        fmt::arg("b", 2),
        fmt::arg("c", 3),
        fmt::arg("d", 4)
    );
    
    // Test all named arguments are accessible
    std::string result = fmt::vformat("{a} {b} {c} {d}", args);
    EXPECT_EQ(result, "1 2 3 4");
    
    // Test partial usage
    std::string partial = fmt::vformat("{b} {d}", args);
    EXPECT_EQ(partial, "2 4");
}

// Test that named argument IDs are consistent across multiple uses
TEST(NamedArgIdTest, ConsistentIds) {
    auto args = fmt::make_format_args(
        fmt::arg("name", "test"),
        fmt::arg("value", 123)
    );
    
    // First use
    std::string first = fmt::vformat("{name}={value}", args);
    EXPECT_EQ(first, "test=123");
    
    // Second use with same args should produce same result
    std::string second = fmt::vformat("{name}={value}", args);
    EXPECT_EQ(first, second);
}

// Test with string_view named arguments
TEST(NamedArgIdTest, StringViewNames) {
    std::string_view name1 = "key1";
    std::string_view name2 = "key2";
    
    auto args = fmt::make_format_args(
        fmt::arg(name1, 100),
        fmt::arg(name2, 200)
    );
    
    std::string result = fmt::vformat("{key1} {key2}", args);
    EXPECT_EQ(result, "100 200");
}

// Test that named argument IDs are correctly mapped to positions
TEST(NamedArgIdTest, IdPositionMapping) {
    auto args = fmt::make_format_args(
        fmt::arg("first", 10),
        fmt::arg("second", 20),
        fmt::arg("third", 30)
    );
    
    // Verify positional and named access produce same results
    std::string named = fmt::vformat("{first} {second} {third}", args);
    std::string positional = fmt::vformat("{0} {1} {2}", args);
    EXPECT_EQ(named, positional);
}

// Test edge case: single named argument
TEST(NamedArgIdTest, SingleNamedArg) {
    auto args = fmt::make_format_args(
        fmt::arg("only", 999)
    );
    
    std::string result = fmt::vformat("{only}", args);
    EXPECT_EQ(result, "999");
}

// Test edge case: named arguments with special characters in names
TEST(NamedArgIdTest, SpecialCharNames) {
    auto args = fmt::make_format_args(
        fmt::arg("_underscore", 1),
        fmt::arg("with123", 2)
    );
    
    std::string result = fmt::vformat("{_underscore} {with123}", args);
    EXPECT_EQ(result, "1 2");
}

// Test that duplicate named argument names are handled (last one wins)
TEST(NamedArgIdTest, DuplicateNames) {
    auto args = fmt::make_format_args(
        fmt::arg("dup", 1),
        fmt::arg("dup", 2)
    );
    
    // The last named argument with the same name should be used
    std::string result = fmt::vformat("{dup}", args);
    EXPECT_EQ(result, "2");
}

// Test with mixed positional and named arguments
TEST(NamedArgIdTest, MixedPositionalAndNamed) {
    auto args = fmt::make_format_args(
        100,
        fmt::arg("named", 200),
        300
    );
    
    // This should work with the named argument
    std::string result = fmt::vformat("{named}", args);
    EXPECT_EQ(result, "200");
}