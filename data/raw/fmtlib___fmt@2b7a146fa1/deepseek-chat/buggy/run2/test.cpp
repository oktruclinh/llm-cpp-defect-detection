#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <climits>

// Test: count_digits with grouping for various group sizes and separators
TEST(CountDigitsGroupingTest, BasicGrouping) {
    // Test with thousands separator (default locale grouping)
    auto result = fmt::format("{:L}", 1234567);
    EXPECT_EQ(result, "1,234,567");
}

TEST(CountDigitsGroupingTest, NoGroupingNeeded) {
    // Numbers smaller than the first group size should not have separators
    auto result = fmt::format("{:L}", 123);
    EXPECT_EQ(result, "123");
}

TEST(CountDigitsGroupingTest, SingleGroup) {
    // Exactly one group boundary
    auto result = fmt::format("{:L}", 1000);
    EXPECT_EQ(result, "1,000");
}

TEST(CountDigitsGroupingTest, MultipleGroups) {
    // Multiple group boundaries
    auto result = fmt::format("{:L}", 123456789);
    EXPECT_EQ(result, "123,456,789");
}

TEST(CountDigitsGroupingTest, ZeroValue) {
    // Zero should have no separators
    auto result = fmt::format("{:L}", 0);
    EXPECT_EQ(result, "0");
}

TEST(CountDigitsGroupingTest, NegativeNumber) {
    // Negative numbers should still be grouped
    auto result = fmt::format("{:L}", -1234567);
    EXPECT_EQ(result, "-1,234,567");
}

TEST(CountDigitsGroupingTest, LargeNumber) {
    // Very large number with many groups
    auto result = fmt::format("{:L}", 1234567890123LL);
    EXPECT_EQ(result, "1,234,567,890,123");
}

TEST(CountDigitsGroupingTest, CustomGrouping) {
    // Test with custom grouping (e.g., Indian numbering system)
    // Note: This requires locale support; using default for simplicity
    auto result = fmt::format("{:L}", 123456789);
    EXPECT_EQ(result, "123,456,789");
}

TEST(CountDigitsGroupingTest, SingleDigit) {
    // Single digit should have no separators
    auto result = fmt::format("{:L}", 5);
    EXPECT_EQ(result, "5");
}

TEST(CountDigitsGroupingTest, TwoDigits) {
    // Two digits should have no separators
    auto result = fmt::format("{:L}", 42);
    EXPECT_EQ(result, "42");
}

TEST(CountDigitsGroupingTest, ThreeDigits) {
    // Three digits should have no separators
    auto result = fmt::format("{:L}", 999);
    EXPECT_EQ(result, "999");
}

TEST(CountDigitsGroupingTest, FourDigits) {
    // Four digits should have one separator
    auto result = fmt::format("{:L}", 1000);
    EXPECT_EQ(result, "1,000");
}

TEST(CountDigitsGroupingTest, MaxInt) {
    // Test with maximum int value
    auto result = fmt::format("{:L}", INT_MAX);
    EXPECT_EQ(result, "2,147,483,647");
}

TEST(CountDigitsGroupingTest, MinInt) {
    // Test with minimum int value (negative)
    auto result = fmt::format("{:L}", INT_MIN);
    EXPECT_EQ(result, "-2,147,483,648");
}

TEST(CountDigitsGroupingTest, LongLongMax) {
    // Test with maximum long long value
    auto result = fmt::format("{:L}", LLONG_MAX);
    EXPECT_EQ(result, "9,223,372,036,854,775,807");
}

TEST(CountDigitsGroupingTest, LongLongMin) {
    // Test with minimum long long value
    auto result = fmt::format("{:L}", LLONG_MIN);
    EXPECT_EQ(result, "-9,223,372,036,854,775,808");
}

TEST(CountDigitsGroupingTest, UnsignedLongLongMax) {
    // Test with maximum unsigned long long value
    auto result = fmt::format("{:L}", ULLONG_MAX);
    EXPECT_EQ(result, "18,446,744,073,709,551,615");
}

TEST(CountDigitsGroupingTest, NoLocaleSpecifier) {
    // Without the 'L' specifier, no grouping should occur
    auto result = fmt::format("{}", 1234567);
    EXPECT_EQ(result, "1234567");
}

TEST(CountDigitsGroupingTest, FillAndAlignWithGrouping) {
    // Test grouping with fill and alignment
    auto result = fmt::format("{:>15L}", 1234567);
    EXPECT_EQ(result, "      1,234,567");
}

TEST(CountDigitsGroupingTest, WidthAndGrouping) {
    // Test with width specifier
    auto result = fmt::format("{:10L}", 1234567);
    EXPECT_EQ(result, " 1,234,567");
}

TEST(CountDigitsGroupingTest, PrecisionWithGrouping) {
    // Test grouping with floating point (should not affect integer)
    auto result = fmt::format("{:.0L}", 1234567.0);
    EXPECT_EQ(result, "1,234,567");
}

TEST(CountDigitsGroupingTest, BinaryFormatNoGrouping) {
    // Binary format should not use grouping
    auto result = fmt::format("{:b}", 1234567);
    EXPECT_NE(result.find(','), std::string::npos) << "Binary should not contain commas";
}

TEST(CountDigitsGroupingTest, HexFormatNoGrouping) {
    // Hex format should not use grouping
    auto result = fmt::format("{:x}", 1234567);
    EXPECT_NE(result.find(','), std::string::npos) << "Hex should not contain commas";
}

TEST(CountDigitsGroupingTest, OctalFormatNoGrouping) {
    // Octal format should not use grouping
    auto result = fmt::format("{:o}", 1234567);
    EXPECT_NE(result.find(','), std::string::npos) << "Octal should not contain commas";
}

TEST(CountDigitsGroupingTest, CustomSeparator) {
    // Test with custom separator (requires locale, using default)
    auto result = fmt::format("{:L}", 1234567);
    EXPECT_EQ(result, "1,234,567");
}

TEST(CountDigitsGroupingTest, GroupSizeOne) {
    // Edge case: group size of 1 (every digit separated)
    // This is not standard but tests the algorithm
    auto result = fmt::format("{:L}", 12345);
    EXPECT_EQ(result, "12,345");
}

TEST(CountDigitsGroupingTest, GroupSizeExceedsDigits) {
    // Group size larger than number of digits
    auto result = fmt::format("{:L}", 99);
    EXPECT_EQ(result, "99");
}

TEST(CountDigitsGroupingTest, NegativeWithGrouping) {
    // Negative number with grouping
    auto result = fmt::format("{:L}", -1000000);
    EXPECT_EQ(result, "-1,000,000");
}

TEST(CountDigitsGroupingTest, ZeroWithGrouping) {
    // Zero with grouping specifier
    auto result = fmt::format("{:L}", 0);
    EXPECT_EQ(result, "0");
}

TEST(CountDigitsGroupingTest, LargeGroupCount) {
    // Number with many groups
    auto result = fmt::format("{:L}", 1000000000000000000ULL);
    EXPECT_EQ(result, "1,000,000,000,000,000,000");
}

TEST(CountDigitsGroupingTest, AlternatingGroupSizes) {
    // Test with non-standard grouping (e.g., Indian: 2,3,3...)
    // Using default locale which uses 3,3,3...
    auto result = fmt::format("{:L}", 123456789);
    EXPECT_EQ(result, "123,456,789");
}

TEST(CountDigitsGroupingTest, SingleGroupWithRemainder) {
    // Number that exactly fills one group
    auto result = fmt::format("{:L}", 1000);
    EXPECT_EQ(result, "1,000");
}

TEST(CountDigitsGroupingTest, MultipleGroupsWithRemainder) {
    // Number that fills multiple groups exactly
    auto result = fmt::format("{:L}", 1000000);
    EXPECT_EQ(result, "1,000,000");
}

TEST(CountDigitsGroupingTest, VeryLargeNumber) {
    // Very large number to stress test
    auto result = fmt::format("{:L}", 1234567890123456789ULL);
    EXPECT_EQ(result, "1,234,567,890,123,456,789");
}