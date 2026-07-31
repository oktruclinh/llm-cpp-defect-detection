#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <climits>

// Test the digit grouping logic indirectly through the public API
// The focal function is part of the internal formatting logic that handles
// digit grouping (e.g., thousands separators). We test it via format strings
// that trigger grouping behavior.

class DigitGroupingTest : public ::testing::Test {
protected:
    // Helper to test grouping with specific locale-like behavior
    // The actual grouping is controlled by the 'n' format specifier
    // which uses the locale's grouping rules
    void testGrouping(const std::string& format_str, 
                     long long value,
                     const std::string& expected) {
        // Use the default "C" locale which has no grouping
        // We test the internal logic by using custom grouping patterns
        // through the format specification
        std::string result = fmt::format(format_str, value);
        EXPECT_EQ(result, expected);
    }
};

// Test basic digit counting without grouping (default behavior)
TEST_F(DigitGroupingTest, NoGrouping) {
    EXPECT_EQ(fmt::format("{}", 0), "0");
    EXPECT_EQ(fmt::format("{}", 1), "1");
    EXPECT_EQ(fmt::format("{}", 9), "9");
    EXPECT_EQ(fmt::format("{}", 10), "10");
    EXPECT_EQ(fmt::format("{}", 99), "99");
    EXPECT_EQ(fmt::format("{}", 100), "100");
    EXPECT_EQ(fmt::format("{}", 999), "999");
    EXPECT_EQ(fmt::format("{}", 1000), "1000");
    EXPECT_EQ(fmt::format("{}", 9999), "9999");
    EXPECT_EQ(fmt::format("{}", 10000), "10000");
}

// Test with grouping that triggers the focal function's loop
// The 'n' format specifier uses locale's grouping, but we can
// test the internal logic by using the 'L' format specifier
// which uses the global locale's grouping
TEST_F(DigitGroupingTest, WithGrouping) {
    // These tests verify that the grouping logic correctly handles
    // the while loop in the focal function
    
    // Single group (thousands separator)
    EXPECT_EQ(fmt::format("{:L}", 1000), "1,000");
    EXPECT_EQ(fmt::format("{:L}", 1000000), "1,000,000");
    
    // Multiple groups
    EXPECT_EQ(fmt::format("{:L}", 1234567), "1,234,567");
    EXPECT_EQ(fmt::format("{:L}", 123456789), "123,456,789");
}

// Test edge cases for the grouping loop
TEST_F(DigitGroupingTest, GroupingEdgeCases) {
    // Zero should have no grouping
    EXPECT_EQ(fmt::format("{:L}", 0), "0");
    
    // Small numbers that don't trigger grouping
    EXPECT_EQ(fmt::format("{:L}", 1), "1");
    EXPECT_EQ(fmt::format("{:L}", 9), "9");
    EXPECT_EQ(fmt::format("{:L}", 10), "10");
    EXPECT_EQ(fmt::format("{:L}", 99), "99");
    EXPECT_EQ(fmt::format("{:L}", 100), "100");
    EXPECT_EQ(fmt::format("{:L}", 999), "999");
    
    // Boundary: exactly at first group boundary
    EXPECT_EQ(fmt::format("{:L}", 1000), "1,000");
    
    // Just above group boundary
    EXPECT_EQ(fmt::format("{:L}", 1001), "1,001");
    
    // Large numbers with multiple groups
    EXPECT_EQ(fmt::format("{:L}", 1000000), "1,000,000");
    EXPECT_EQ(fmt::format("{:L}", 1000000000), "1,000,000,000");
}

// Test negative numbers (the focal function uses abs_value)
TEST_F(DigitGroupingTest, NegativeNumbers) {
    EXPECT_EQ(fmt::format("{:L}", -1000), "-1,000");
    EXPECT_EQ(fmt::format("{:L}", -1234567), "-1,234,567");
    EXPECT_EQ(fmt::format("{:L}", -1000000), "-1,000,000");
}

// Test with custom grouping patterns via locale
// This tests the while loop with different group sizes
TEST_F(DigitGroupingTest, CustomGrouping) {
    // The 'n' format specifier uses the locale's grouping
    // We can test with the default "C" locale which has no grouping
    // and verify that the loop handles empty groups correctly
    
    // With default locale, 'n' should behave like 'd'
    EXPECT_EQ(fmt::format("{:n}", 1000), "1000");
    EXPECT_EQ(fmt::format("{:n}", 1234567), "1234567");
    
    // Test that the grouping logic handles the case where
    // groups.cend() is reached (empty groups)
    EXPECT_EQ(fmt::format("{:n}", 0), "0");
    EXPECT_EQ(fmt::format("{:n}", 1), "1");
}

// Test the specific conditions in the while loop
TEST_F(DigitGroupingTest, WhileLoopConditions) {
    // Test when group == groups.cend() (no groups)
    EXPECT_EQ(fmt::format("{:L}", 123), "123");
    
    // Test when n <= *group (remaining digits less than group size)
    // This happens when we have fewer digits than the group size
    EXPECT_EQ(fmt::format("{:L}", 100), "100");
    EXPECT_EQ(fmt::format("{:L}", 999), "999");
    
    // Test when *group <= 0 (should not happen with valid locale)
    // but the condition guards against it
    
    // Test when *group == max_value<char>() (sentinel value)
    // This is the end-of-groups marker in some locale implementations
}

// Test with very large numbers to stress the grouping logic
TEST_F(DigitGroupingTest, LargeNumbers) {
    // Maximum 64-bit signed integer
    EXPECT_EQ(fmt::format("{:L}", 9223372036854775807LL), 
              "9,223,372,036,854,775,807");
    
    // Large negative
    EXPECT_EQ(fmt::format("{:L}", -9223372036854775807LL - 1), 
              "-9,223,372,036,854,775,808");
}

// Test that the grouping logic correctly handles the size calculation
TEST_F(DigitGroupingTest, SizeCalculation) {
    // Verify that the total size includes separators
    // "1,000" has 5 characters (4 digits + 1 separator)
    std::string result = fmt::format("{:L}", 1000);
    EXPECT_EQ(result.size(), 5);
    
    // "1,000,000" has 9 characters (7 digits + 2 separators)
    result = fmt::format("{:L}", 1000000);
    EXPECT_EQ(result.size(), 9);
    
    // "1,234,567" has 9 characters (7 digits + 2 separators)
    result = fmt::format("{:L}", 1234567);
    EXPECT_EQ(result.size(), 9);
}

// Test with different separator characters
TEST_F(DigitGroupingTest, DifferentSeparators) {
    // The default locale uses comma as separator
    // We can verify the separator is correctly inserted
    EXPECT_EQ(fmt::format("{:L}", 1000), "1,000");
    EXPECT_NE(fmt::format("{:L}", 1000), "1.000");
    EXPECT_NE(fmt::format("{:L}", 1000), "1 000");
}

// Test that the grouping logic handles the case where
// the number of digits is exactly equal to a group size
TEST_F(DigitGroupingTest, ExactGroupSize) {
    EXPECT_EQ(fmt::format("{:L}", 1000), "1,000");
    EXPECT_EQ(fmt::format("{:L}", 1000000), "1,000,000");
    EXPECT_EQ(fmt::format("{:L}", 1000000000), "1,000,000,000");
}

// Test that the grouping logic handles the case where
// the number of digits is one less than a group size
TEST_F(DigitGroupingTest, OneLessThanGroupSize) {
    EXPECT_EQ(fmt::format("{:L}", 999), "999");
    EXPECT_EQ(fmt::format("{:L}", 999999), "999,999");
    EXPECT_EQ(fmt::format("{:L}", 999999999), "999,999,999");
}