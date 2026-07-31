#include <gtest/gtest.h>
#include <fmt/core.h>
#include <fmt/format.h>

TEST(FmtCoreTest, GetIdWithNamedArguments) {
    // Test normal case with named arguments
    auto result = fmt::format("Hello {name}!", fmt::arg("name", "World"));
    EXPECT_EQ("Hello World!", result);
    
    // Test with multiple named arguments
    result = fmt::format("Hello {name}, you are {age} years old!", 
                        fmt::arg("name", "Alice"), 
                        fmt::arg("age", 30));
    EXPECT_EQ("Hello Alice, you are 30 years old!", result);
}

TEST(FmtCoreTest, GetIdWithoutNamedArguments) {
    // Test with no named arguments - should not crash
    auto result = fmt::format("Hello World!");
    EXPECT_EQ("Hello World!", result);
    
    // Test with positional arguments only
    result = fmt::format("Hello {}!", "World");
    EXPECT_EQ("Hello World!", result);
}

TEST(FmtCoreTest, GetIdEdgeCases) {
    // Test empty string name
    auto result = fmt::format("Hello {name}!", fmt::arg("name", "World"));
    EXPECT_EQ("Hello World!", result);
    
    // Test with numeric names (should work as strings)
    result = fmt::format("Value {0} is {1}", fmt::arg("0", 42), fmt::arg("1", 3.14));
    EXPECT_EQ("Value 42 is 3.14", result);
    
    // Test with special characters in names
    result = fmt::format("Hello {name-with-dashes}!", fmt::arg("name-with-dashes", "World"));
    EXPECT_EQ("Hello World!", result);
}

TEST(FmtCoreTest, GetIdComplexScenarios) {
    // Test nested formatting
    auto result = fmt::format("Result: {result}", fmt::arg("result", fmt::format("Inner: {value}", fmt::arg("value", 123))));
    EXPECT_EQ("Result: Inner: 123", result);
    
    // Test with mixed positional and named arguments
    result = fmt::format("First: {}, Second: {second}, Third: {}", 
                        "pos1", 
                        fmt::arg("second", "named"), 
                        "pos3");
    EXPECT_EQ("First: pos1, Second: named, Third: pos3", result);
}

TEST(FmtCoreTest, GetIdPerformance) {
    // Test that formatting with named arguments doesn't cause performance issues
    std::string result;
    for (int i = 0; i < 1000; ++i) {
        result = fmt::format("Test {value}!", fmt::arg("value", i));
    }
    EXPECT_EQ("Test 999!", result);
}

TEST(FmtCoreTest, GetIdStringViewCompatibility) {
    // Test with different string view types
    std::string name = "test";
    auto result = fmt::format("Hello {name}!", fmt::arg(name.c_str(), "World"));
    EXPECT_EQ("Hello World!", result);
    
    // Test with std::string_view (if available) - use const char* instead
    const char* cstr_name = "name";
    result = fmt::format("Hello {name}!", fmt::arg(cstr_name, "World"));
    EXPECT_EQ("Hello World!", result);
}

TEST(FmtCoreTest, GetIdLargeArguments) {
    // Test with large argument values
    std::string large_string(1000, 'A');
    auto result = fmt::format("String length: {len}", fmt::arg("len", large_string.length()));
    EXPECT_EQ("String length: 1000", result);
    
    // Test with large numeric values
    auto large_num = static_cast<long long>(1000000000000LL);
    result = fmt::format("Large number: {num}", fmt::arg("num", large_num));
    EXPECT_EQ("Large number: 1000000000000", result);
}

TEST(FmtCoreTest, GetIdSpecialCharacters) {
    // Test with unicode characters
    auto result = fmt::format("Hello {name}!", fmt::arg("name", "世界"));
    EXPECT_EQ("Hello 世界!", result);
    
    // Test with special formatting characters
    result = fmt::format("Hello {name}!", fmt::arg("name", "{braces}"));
    EXPECT_EQ("Hello {braces}!", result);
}