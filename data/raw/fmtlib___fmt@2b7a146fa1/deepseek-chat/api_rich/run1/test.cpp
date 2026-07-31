#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <cstdint>

// Test that the digit grouping logic works correctly for various locales and formats
class DigitGroupingTest : public ::testing::Test {
protected:
    void SetUp() override {
        // No setup needed
    }
};

// Test basic grouping with thousands separator
TEST_F(DigitGroupingTest, BasicGrouping) {
    // Test with default locale (no grouping)
    EXPECT_EQ(fmt::format("{:d}", 1234567), "1234567");
    
    // Test with grouping using apostrophe flag
    EXPECT_EQ(fmt::format("{:'d}", 1234567), "1,234,567");
}

// Test grouping with different group sizes
TEST_F(DigitGroupingTest, DifferentGroupSizes) {
    // Test with 3-digit groups (default)
    EXPECT_EQ(fmt::format("{:'d}", 1000), "1,000");
    EXPECT_EQ(fmt::format("{:'d}", 1000000), "1,000,000");
    
    // Test with 2-digit groups (Indian numbering system)
    EXPECT_EQ(fmt::format("{:'d}", 123456), "1,23,456");
}

// Test edge cases with small numbers
TEST_F(DigitGroupingTest, SmallNumbers) {
    // Single digit - no grouping needed
    EXPECT_EQ(fmt::format("{:'d}", 0), "0");
    EXPECT_EQ(fmt::format("{:'d}", 1), "1");
    EXPECT_EQ(fmt::format("{:'d}", 9), "9");
    
    // Two digits - no grouping needed
    EXPECT_EQ(fmt::format("{:'d}", 10), "10");
    EXPECT_EQ(fmt::format("{:'d}", 99), "99");
    
    // Three digits - no grouping needed
    EXPECT_EQ(fmt::format("{:'d}", 100), "100");
    EXPECT_EQ(fmt::format("{:'d}", 999), "999");
}

// Test edge cases at grouping boundaries
TEST_F(DigitGroupingTest, GroupingBoundaries) {
    // Exactly one group
    EXPECT_EQ(fmt::format("{:'d}", 1000), "1,000");
    EXPECT_EQ(fmt::format("{:'d}", 9999), "9,999");
    
    // Two groups
    EXPECT_EQ(fmt::format("{:'d}", 1000000), "1,000,000");
    EXPECT_EQ(fmt::format("{:'d}", 9999999), "9,999,999");
}

// Test with negative numbers
TEST_F(DigitGroupingTest, NegativeNumbers) {
    EXPECT_EQ(fmt::format("{:'d}", -1000), "-1,000");
    EXPECT_EQ(fmt::format("{:'d}", -1234567), "-1,234,567");
    EXPECT_EQ(fmt::format("{:'d}", -1), "-1");
    EXPECT_EQ(fmt::format("{:'d}", -0), "0");
}

// Test with large numbers
TEST_F(DigitGroupingTest, LargeNumbers) {
    EXPECT_EQ(fmt::format("{:'d}", 1234567890), "1,234,567,890");
    EXPECT_EQ(fmt::format("{:'d}", 999999999999LL), "999,999,999,999");
    EXPECT_EQ(fmt::format("{:'d}", 1000000000000LL), "1,000,000,000,000");
}

// Test with maximum values
TEST_F(DigitGroupingTest, MaximumValues) {
    EXPECT_EQ(fmt::format("{:'d}", std::numeric_limits<int>::max()), 
              "2,147,483,647");
    EXPECT_EQ(fmt::format("{:'d}", std::numeric_limits<int>::min()), 
              "-2,147,483,648");
}

// Test with different integer types
TEST_F(DigitGroupingTest, DifferentIntegerTypes) {
    // Short
    EXPECT_EQ(fmt::format("{:'d}", static_cast<short>(12345)), "12,345");
    
    // Long
    EXPECT_EQ(fmt::format("{:'d}", 123456789L), "123,456,789");
    
    // Long long
    EXPECT_EQ(fmt::format("{:'d}", 1234567890123LL), "1,234,567,890,123");
    
    // Unsigned
    EXPECT_EQ(fmt::format("{:'d}", 1234567u), "1,234,567");
}

// Test that grouping works with format specifiers
TEST_F(DigitGroupingTest, FormatSpecifiers) {
    // Width and alignment
    EXPECT_EQ(fmt::format("{:>'d}", 1234), "1,234");
    EXPECT_EQ(fmt::format("{:12'd}", 1234), "       1,234");
    
    // Fill character
    EXPECT_EQ(fmt::format("{:*>12'd}", 1234), "*******1,234");
    
    // Zero padding (should not interfere with grouping)
    EXPECT_EQ(fmt::format("{:012'd}", 1234), "0000001,234");
}

// Test that grouping works with different bases (should not apply)
TEST_F(DigitGroupingTest, DifferentBases) {
    // Grouping flag should be ignored for non-decimal formats
    EXPECT_EQ(fmt::format("{:'x}", 255), "ff");
    EXPECT_EQ(fmt::format("{:'o}", 255), "377");
    EXPECT_EQ(fmt::format("{:'b}", 255), "11111111");
}

// Test with custom group sizes using locale
TEST_F(DigitGroupingTest, CustomGroupSizes) {
    // Test with 4-digit groups (Japanese/Chinese style)
    EXPECT_EQ(fmt::format("{:'d}", 12345678), "1,23,45,678");
    
    // Test with 2-digit groups
    EXPECT_EQ(fmt::format("{:'d}", 123456), "1,23,456");
}

// Test empty string and zero-length groups
TEST_F(DigitGroupingTest, EmptyAndZeroGroups) {
    // Single digit
    EXPECT_EQ(fmt::format("{:'d}", 0), "0");
    
    // Very large number with many groups
    EXPECT_EQ(fmt::format("{:'d}", 1000000000000000000ULL), 
              "1,000,000,000,000,000,000");
}

// Test that grouping works correctly with format_to
TEST_F(DigitGroupingTest, FormatTo) {
    std::string result;
    fmt::format_to(std::back_inserter(result), "{:'d}", 1234567);
    EXPECT_EQ(result, "1,234,567");
}

// Test that grouping works correctly with format_to_n
TEST_F(DigitGroupingTest, FormatToN) {
    std::string result(20, '\0');
    auto res = fmt::format_to_n(result.begin(), 10, "{:'d}", 1234567);
    *res.out = '\0';
    EXPECT_EQ(result.substr(0, 10), "1,234,567");
}

// Test that grouping works correctly with formatted_size
TEST_F(DigitGroupingTest, FormattedSize) {
    EXPECT_EQ(fmt::formatted_size("{:'d}", 1234567), 9);  // "1,234,567" is 9 chars
    EXPECT_EQ(fmt::formatted_size("{:'d}", 1000), 5);     // "1,000" is 5 chars
    EXPECT_EQ(fmt::formatted_size("{:'d}", 0), 1);        // "0" is 1 char
}

// Test edge case: number with exactly the group size
TEST_F(DigitGroupingTest, ExactGroupSize) {
    EXPECT_EQ(fmt::format("{:'d}", 1000), "1,000");
    EXPECT_EQ(fmt::format("{:'d}", 1000000), "1,000,000");
    EXPECT_EQ(fmt::format("{:'d}", 1000000000), "1,000,000,000");
}

// Test edge case: number with one more digit than group size
TEST_F(DigitGroupingTest, OneMoreThanGroupSize) {
    EXPECT_EQ(fmt::format("{:'d}", 1001), "1,001");
    EXPECT_EQ(fmt::format("{:'d}", 1000001), "1,000,001");
}

// Test that grouping works with padding and alignment
TEST_F(DigitGroupingTest, PaddingAndAlignment) {
    EXPECT_EQ(fmt::format("{:>10'd}", 1234), "     1,234");
    EXPECT_EQ(fmt::format("{:^10'd}", 1234), "  1,234   ");
    EXPECT_EQ(fmt::format("{:<10'd}", 1234), "1,234     ");
}

// Test that grouping works with sign specifiers
TEST_F(DigitGroupingTest, SignSpecifiers) {
    EXPECT_EQ(fmt::format("{:+'d}", 1234), "+1,234");
    EXPECT_EQ(fmt::format("{: 'd}", 1234), " 1,234");
    EXPECT_EQ(fmt::format("{:-'d}", 1234), "1,234");
    EXPECT_EQ(fmt::format("{:+'d}", -1234), "-1,234");
}

// Test that grouping works with alternate form
TEST_F(DigitGroupingTest, AlternateForm) {
    // Alternate form with grouping should work for integers
    EXPECT_EQ(fmt::format("{:#'x}", 255), "0xff");
    EXPECT_EQ(fmt::format("{:#'o}", 255), "0377");
    EXPECT_EQ(fmt::format("{:#'b}", 255), "0b11111111");
}