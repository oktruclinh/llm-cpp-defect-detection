#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <climits>

// Test the grouping logic indirectly through format specifiers that use grouping
// The focal function is invoked when formatting integers with the 'n' locale-specific format
// or when using the thousands separator in the 'L' format specifier

TEST(CountDigitsGroupingTest, NoGrouping) {
    // When no grouping is needed (small numbers), the function should not add separators
    EXPECT_EQ(fmt::format("{:L}", 123), "123");
    EXPECT_EQ(fmt::format("{:L}", 0), "0");
    EXPECT_EQ(fmt::format("{:L}", 1), "1");
    EXPECT_EQ(fmt::format("{:L}", 999), "999");
}

TEST(CountDigitsGroupingTest, SingleGroup) {
    // Numbers that require exactly one grouping separator
    EXPECT_EQ(fmt::format("{:L}", 1000), "1,000");
    EXPECT_EQ(fmt::format("{:L}", 1234), "1,234");
    EXPECT_EQ(fmt::format("{:L}", 9999), "9,999");
}

TEST(CountDigitsGroupingTest, MultipleGroups) {
    // Numbers that require multiple grouping separators
    EXPECT_EQ(fmt::format("{:L}", 1000000), "1,000,000");
    EXPECT_EQ(fmt::format("{:L}", 1234567), "1,234,567");
    EXPECT_EQ(fmt::format("{:L}", 999999999), "999,999,999");
}

TEST(CountDigitsGroupingTest, LargeNumber) {
    // Very large numbers with many groups
    EXPECT_EQ(fmt::format("{:L}", 1234567890123), "1,234,567,890,123");
    EXPECT_EQ(fmt::format("{:L}", 1000000000000), "1,000,000,000,000");
}

TEST(CountDigitsGroupingTest, NegativeNumbers) {
    // Negative numbers should still be grouped correctly
    EXPECT_EQ(fmt::format("{:L}", -1000), "-1,000");
    EXPECT_EQ(fmt::format("{:L}", -1234567), "-1,234,567");
    EXPECT_EQ(fmt::format("{:L}", -1), "-1");
}

TEST(CountDigitsGroupingTest, MaxInt) {
    // Test with maximum int value
    EXPECT_EQ(fmt::format("{:L}", INT_MAX), "2,147,483,647");
}

TEST(CountDigitsGroupingTest, MinInt) {
    // Test with minimum int value (edge case for absolute value)
    EXPECT_EQ(fmt::format("{:L}", INT_MIN), "-2,147,483,648");
}

TEST(CountDigitsGroupingTest, ZeroPaddingWithGrouping) {
    // When zero padding is combined with grouping, the grouping should still work
    EXPECT_EQ(fmt::format("{:015L}", 1234), "0,000,000,001,234");
    EXPECT_EQ(fmt::format("{:010L}", 1234567), "0,001,234,567");
}

TEST(CountDigitsGroupingTest, WidthAndGrouping) {
    // Width specifier with grouping
    EXPECT_EQ(fmt::format("{:15L}", 1234), "          1,234");
    EXPECT_EQ(fmt::format("{:15L}", -1234), "         -1,234");
}

TEST(CountDigitsGroupingTest, AlignmentWithGrouping) {
    // Left and right alignment with grouping
    EXPECT_EQ(fmt::format("{:<15L}", 1234), "1,234          ");
    EXPECT_EQ(fmt::format("{:>15L}", 1234), "          1,234");
    EXPECT_EQ(fmt::format("{:^15L}", 1234), "    1,234      ");
}

TEST(CountDigitsGroupingTest, AlternativeFormWithGrouping) {
    // Alternative form (#) with grouping should not affect grouping
    EXPECT_EQ(fmt::format("{:#L}", 1234), "1,234");
    EXPECT_EQ(fmt::format("{:#015L}", 1234), "0,000,000,001,234");
}

TEST(CountDigitsGroupingTest, PlusSignWithGrouping) {
    // Plus sign with grouping
    EXPECT_EQ(fmt::format("{:+L}", 1234), "+1,234");
    EXPECT_EQ(fmt::format("{:+L}", -1234), "-1,234");
}

TEST(CountDigitsGroupingTest, SpaceFlagWithGrouping) {
    // Space flag with grouping
    EXPECT_EQ(fmt::format("{: L}", 1234), " 1,234");
    EXPECT_EQ(fmt::format("{: L}", -1234), "-1,234");
}

TEST(CountDigitsGroupingTest, LongLongType) {
    // Test with long long type
    EXPECT_EQ(fmt::format("{:L}", 1234567890123LL), "1,234,567,890,123");
    EXPECT_EQ(fmt::format("{:L}", -1234567890123LL), "-1,234,567,890,123");
}

TEST(CountDigitsGroupingTest, UnsignedType) {
    // Test with unsigned types
    EXPECT_EQ(fmt::format("{:L}", 1234567u), "1,234,567");
    EXPECT_EQ(fmt::format("{:L}", 0u), "0");
}

TEST(CountDigitsGroupingTest, CharType) {
    // Test with char type (small integer)
    EXPECT_EQ(fmt::format("{:L}", static_cast<char>(100)), "100");
    EXPECT_EQ(fmt::format("{:L}", static_cast<char>(127)), "127");
}

TEST(CountDigitsGroupingTest, ShortType) {
    // Test with short type
    EXPECT_EQ(fmt::format("{:L}", static_cast<short>(12345)), "12,345");
    EXPECT_EQ(fmt::format("{:L}", static_cast<short>(-12345)), "-12,345");
}

TEST(CountDigitsGroupingTest, BinaryFormatNoGrouping) {
    // Binary format should not be affected by grouping
    EXPECT_EQ(fmt::format("{:b}", 1234), "10011010010");
    EXPECT_EQ(fmt::format("{:#b}", 1234), "0b10011010010");
}

TEST(CountDigitsGroupingTest, HexFormatNoGrouping) {
    // Hex format should not be affected by grouping
    EXPECT_EQ(fmt::format("{:x}", 1234567), "12d687");
    EXPECT_EQ(fmt::format("{:#X}", 1234567), "0X12D687");
}

TEST(CountDigitsGroupingTest, OctalFormatNoGrouping) {
    // Octal format should not be affected by grouping
    EXPECT_EQ(fmt::format("{:o}", 1234567), "4553207");
    EXPECT_EQ(fmt::format("{:#o}", 1234567), "04553207");
}