#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <climits>

// Test the focal function's behavior through public API: fmt::format with
// width, precision, and alignment specifiers that trigger write_padded and
// the associated formatting logic for negative numbers.

// Normal cases: basic negative integer formatting with various widths
TEST(FormatNegativeTest, BasicNegativeInteger) {
    EXPECT_EQ(fmt::format("{:d}", -42), "-42");
    EXPECT_EQ(fmt::format("{:5d}", -42), "  -42");
    EXPECT_EQ(fmt::format("{:05d}", -42), "-0042");
    EXPECT_EQ(fmt::format("{:10d}", -12345), "    -12345");
}

// Edge case: zero width (should not pad)
TEST(FormatNegativeTest, ZeroWidth) {
    EXPECT_EQ(fmt::format("{:d}", -1), "-1");
    EXPECT_EQ(fmt::format("{:0d}", -1), "-1");
}

// Edge case: width exactly matching the number of characters (including sign)
TEST(FormatNegativeTest, ExactWidth) {
    EXPECT_EQ(fmt::format("{:2d}", -1), "-1");
    EXPECT_EQ(fmt::format("{:3d}", -10), "-10");
    EXPECT_EQ(fmt::format("{:4d}", -100), "-100");
}

// Edge case: width smaller than number of characters (should not truncate)
TEST(FormatNegativeTest, WidthSmallerThanNumber) {
    EXPECT_EQ(fmt::format("{:1d}", -100), "-100");
    EXPECT_EQ(fmt::format("{:2d}", -1000), "-1000");
}

// Edge case: minimum integer value (INT_MIN)
TEST(FormatNegativeTest, MinInt) {
    EXPECT_EQ(fmt::format("{:d}", INT_MIN), "-2147483648");
    EXPECT_EQ(fmt::format("{:12d}", INT_MIN), " -2147483648");
    EXPECT_EQ(fmt::format("{:012d}", INT_MIN), "-02147483648");
}

// Edge case: zero (not negative, but tests sign handling boundary)
TEST(FormatNegativeTest, Zero) {
    EXPECT_EQ(fmt::format("{:d}", 0), "0");
    EXPECT_EQ(fmt::format("{:5d}", 0), "    0");
    EXPECT_EQ(fmt::format("{:05d}", 0), "00000");
}

// Edge case: negative zero (should format as zero)
TEST(FormatNegativeTest, NegativeZero) {
    EXPECT_EQ(fmt::format("{:d}", -0), "0");
    EXPECT_EQ(fmt::format("{:5d}", -0), "    0");
}

// Edge case: large negative number
TEST(FormatNegativeTest, LargeNegative) {
    EXPECT_EQ(fmt::format("{:d}", -999999999), "-999999999");
    EXPECT_EQ(fmt::format("{:15d}", -999999999), "     -999999999");
}

// Edge case: width with fill character
TEST(FormatNegativeTest, FillCharacter) {
    EXPECT_EQ(fmt::format("{:*>5d}", -42), "**-42");
    EXPECT_EQ(fmt::format("{:*<5d}", -42), "-42**");
    EXPECT_EQ(fmt::format("{:*^5d}", -42), "*-42*");
}

// Edge case: zero padding with negative number (sign should come first)
TEST(FormatNegativeTest, ZeroPaddingWithSign) {
    EXPECT_EQ(fmt::format("{:05d}", -1), "-0001");
    EXPECT_EQ(fmt::format("{:010d}", -123), "-000000123");
}

// Edge case: width of 1 with negative number
TEST(FormatNegativeTest, WidthOneNegative) {
    EXPECT_EQ(fmt::format("{:1d}", -5), "-5");
    EXPECT_EQ(fmt::format("{:1d}", -10), "-10");
}

// Edge case: very large width
TEST(FormatNegativeTest, VeryLargeWidth) {
    std::string expected(100, ' ');
    expected += "-42";
    EXPECT_EQ(fmt::format("{:>102d}", -42), expected);
}

// Edge case: negative number with plus sign flag (should be ignored for negatives)
TEST(FormatNegativeTest, PlusSignFlag) {
    EXPECT_EQ(fmt::format("{:+d}", -42), "-42");
    EXPECT_EQ(fmt::format("{:+5d}", -42), "  -42");
}

// Edge case: negative number with space flag (should be ignored for negatives)
TEST(FormatNegativeTest, SpaceFlag) {
    EXPECT_EQ(fmt::format("{: d}", -42), "-42");
    EXPECT_EQ(fmt::format("{: 5d}", -42), "  -42");
}

// Edge case: negative number with alternate form (no effect for integers)
TEST(FormatNegativeTest, AlternateForm) {
    EXPECT_EQ(fmt::format("{:#d}", -42), "-42");
    EXPECT_EQ(fmt::format("{:#5d}", -42), "  -42");
}

// Edge case: long long negative numbers
TEST(FormatNegativeTest, LongLongNegative) {
    EXPECT_EQ(fmt::format("{:d}", -123456789012345LL), "-123456789012345");
    EXPECT_EQ(fmt::format("{:20d}", -123456789012345LL), "     -123456789012345");
}

// Edge case: unsigned negative (should not happen, but test behavior)
TEST(FormatNegativeTest, UnsignedNegative) {
    EXPECT_EQ(fmt::format("{:d}", -1u), "4294967295");
    EXPECT_EQ(fmt::format("{:15d}", -1u), "     4294967295");
}

// Edge case: empty format string
TEST(FormatNegativeTest, EmptyFormat) {
    EXPECT_EQ(fmt::format("{}", -42), "-42");
    EXPECT_EQ(fmt::format("{:}", -42), "-42");
}

// Edge case: precision with negative number (precision is for floats, but test integer)
TEST(FormatNegativeTest, PrecisionWithInteger) {
    EXPECT_EQ(fmt::format("{:.5d}", -42), "-00042");
    EXPECT_EQ(fmt::format("{:10.5d}", -42), "     -00042");
}

// Edge case: negative number with left alignment
TEST(FormatNegativeTest, LeftAlignment) {
    EXPECT_EQ(fmt::format("{:<5d}", -42), "-42  ");
    EXPECT_EQ(fmt::format("{:<10d}", -123), "-123      ");
}

// Edge case: negative number with center alignment
TEST(FormatNegativeTest, CenterAlignment) {
    EXPECT_EQ(fmt::format("{:^5d}", -42), "-42  ");
    EXPECT_EQ(fmt::format("{:^6d}", -42), " -42  ");
    EXPECT_EQ(fmt::format("{:^7d}", -42), " -42   ");
}