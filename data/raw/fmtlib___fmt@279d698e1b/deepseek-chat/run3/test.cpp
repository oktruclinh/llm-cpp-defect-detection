#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Test formatting integers with various widths, alignments, and fill characters
TEST(FormatIntegerTest, BasicWidthAndAlignment) {
    // Right alignment (default)
    EXPECT_EQ(fmt::format("{:5}", 42), "   42");
    EXPECT_EQ(fmt::format("{:5}", -42), "  -42");
    
    // Left alignment
    EXPECT_EQ(fmt::format("{:<5}", 42), "42   ");
    EXPECT_EQ(fmt::format("{:<5}", -42), "-42  ");
    
    // Center alignment
    EXPECT_EQ(fmt::format("{:^5}", 42), " 42  ");
    EXPECT_EQ(fmt::format("{:^5}", -42), "-42  ");
    EXPECT_EQ(fmt::format("{:^6}", -42), " -42  ");
    
    // Fill character
    EXPECT_EQ(fmt::format("{:*>5}", 42), "***42");
    EXPECT_EQ(fmt::format("{:*<5}", 42), "42***");
    EXPECT_EQ(fmt::format("{:*^5}", 42), "*42**");
    EXPECT_EQ(fmt::format("{:*>5}", -42), "**-42");
    EXPECT_EQ(fmt::format("{:*<5}", -42), "-42**");
    EXPECT_EQ(fmt::format("{:*^5}", -42), "-42**");
}

// Test formatting integers with zero padding
TEST(FormatIntegerTest, ZeroPadding) {
    EXPECT_EQ(fmt::format("{:05}", 42), "00042");
    EXPECT_EQ(fmt::format("{:05}", -42), "-0042");
    EXPECT_EQ(fmt::format("{:05}", 0), "00000");
    EXPECT_EQ(fmt::format("{:05}", -0), "00000");
}

// Test formatting integers with sign options
TEST(FormatIntegerTest, SignOptions) {
    EXPECT_EQ(fmt::format("{:+}", 42), "+42");
    EXPECT_EQ(fmt::format("{:+}", -42), "-42");
    EXPECT_EQ(fmt::format("{: }", 42), " 42");
    EXPECT_EQ(fmt::format("{: }", -42), "-42");
    EXPECT_EQ(fmt::format("{:-}", 42), "42");
    EXPECT_EQ(fmt::format("{:-}", -42), "-42");
}

// Test formatting integers with different bases
TEST(FormatIntegerTest, BaseFormatting) {
    EXPECT_EQ(fmt::format("{:b}", 42), "101010");
    EXPECT_EQ(fmt::format("{:B}", 42), "101010");
    EXPECT_EQ(fmt::format("{:d}", 42), "42");
    EXPECT_EQ(fmt::format("{:o}", 42), "52");
    EXPECT_EQ(fmt::format("{:x}", 42), "2a");
    EXPECT_EQ(fmt::format("{:X}", 42), "2A");
    
    // With prefix
    EXPECT_EQ(fmt::format("{:#b}", 42), "0b101010");
    EXPECT_EQ(fmt::format("{:#B}", 42), "0B101010");
    EXPECT_EQ(fmt::format("{:#o}", 42), "052");
    EXPECT_EQ(fmt::format("{:#x}", 42), "0x2a");
    EXPECT_EQ(fmt::format("{:#X}", 42), "0X2A");
}

// Test formatting integers with width and precision
TEST(FormatIntegerTest, WidthAndPrecision) {
    EXPECT_EQ(fmt::format("{:6d}", 42), "    42");
    EXPECT_EQ(fmt::format("{:6.4d}", 42), "  0042");
    EXPECT_EQ(fmt::format("{:6.4d}", -42), " -0042");
    EXPECT_EQ(fmt::format("{:6.4d}", 0), "  0000");
}

// Test edge cases: zero, min/max values
TEST(FormatIntegerTest, EdgeCases) {
    EXPECT_EQ(fmt::format("{}", 0), "0");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::max()), "2147483647");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::min()), "-2147483648");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<unsigned>::max()), "4294967295");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<long long>::max()), "9223372036854775807");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<long long>::min()), "-9223372036854775808");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<unsigned long long>::max()), "18446744073709551615");
}

// Test formatting with width larger than number of digits
TEST(FormatIntegerTest, WidthLargerThanDigits) {
    EXPECT_EQ(fmt::format("{:10}", 42), "        42");
    EXPECT_EQ(fmt::format("{:10}", -42), "       -42");
    EXPECT_EQ(fmt::format("{:<10}", 42), "42        ");
    EXPECT_EQ(fmt::format("{:^10}", 42), "    42    ");
    EXPECT_EQ(fmt::format("{:*>10}", 42), "********42");
}

// Test formatting with width smaller than number of digits (no padding)
TEST(FormatIntegerTest, WidthSmallerThanDigits) {
    EXPECT_EQ(fmt::format("{:2}", 12345), "12345");
    EXPECT_EQ(fmt::format("{:2}", -12345), "-12345");
    EXPECT_EQ(fmt::format("{:<2}", 12345), "12345");
    EXPECT_EQ(fmt::format("{:^2}", 12345), "12345");
}

// Test formatting with fill character and alignment for negative numbers
TEST(FormatIntegerTest, NegativeNumbersWithFill) {
    EXPECT_EQ(fmt::format("{:*>6}", -42), "***-42");
    EXPECT_EQ(fmt::format("{:*<6}", -42), "-42***");
    EXPECT_EQ(fmt::format("{:*^6}", -42), "-42***");
    EXPECT_EQ(fmt::format("{:*>6}", -12345), "-12345");
    EXPECT_EQ(fmt::format("{:*<6}", -12345), "-12345");
    EXPECT_EQ(fmt::format("{:*^6}", -12345), "-12345");
}

// Test formatting with zero padding and alignment
TEST(FormatIntegerTest, ZeroPaddingWithAlignment) {
    EXPECT_EQ(fmt::format("{:06}", 42), "000042");
    EXPECT_EQ(fmt::format("{:<06}", 42), "420000");
    EXPECT_EQ(fmt::format("{:^06}", 42), "042000");
    EXPECT_EQ(fmt::format("{:>06}", 42), "000042");
}

// Test formatting with sign and zero padding
TEST(FormatIntegerTest, SignAndZeroPadding) {
    EXPECT_EQ(fmt::format("{:+06}", 42), "+00042");
    EXPECT_EQ(fmt::format("{:+06}", -42), "-00042");
    EXPECT_EQ(fmt::format("{: 06}", 42), " 00042");
    EXPECT_EQ(fmt::format("{: 06}", -42), "-00042");
}

// Test formatting with width, precision, and sign
TEST(FormatIntegerTest, WidthPrecisionAndSign) {
    EXPECT_EQ(fmt::format("{:+8.4d}", 42), "   +0042");
    EXPECT_EQ(fmt::format("{:+8.4d}", -42), "   -0042");
    EXPECT_EQ(fmt::format("{: 8.4d}", 42), "    0042");
    EXPECT_EQ(fmt::format("{: 8.4d}", -42), "   -0042");
}

// Test formatting with alternate form and width
TEST(FormatIntegerTest, AlternateFormWithWidth) {
    EXPECT_EQ(fmt::format("{:#10x}", 255), "     0xff");
    EXPECT_EQ(fmt::format("{:#10X}", 255), "     0XFF");
    EXPECT_EQ(fmt::format("{:#10o}", 255), "      0377");
    EXPECT_EQ(fmt::format("{:#10b}", 255), "0b11111111");
}

// Test formatting with empty format string
TEST(FormatIntegerTest, EmptyFormatString) {
    EXPECT_EQ(fmt::format("{}", 42), "42");
    EXPECT_EQ(fmt::format("{}", -42), "-42");
    EXPECT_EQ(fmt::format("{}", 0), "0");
}

// Test formatting with multiple arguments
TEST(FormatIntegerTest, MultipleArguments) {
    EXPECT_EQ(fmt::format("{} {} {}", 1, 2, 3), "1 2 3");
    EXPECT_EQ(fmt::format("{:4} {:4} {:4}", 1, -2, 3), "   1   -2    3");
    EXPECT_EQ(fmt::format("{:<4} {:^4} {:>4}", 1, -2, 3), "1    -2     3");
}

// Test formatting with char type
TEST(FormatIntegerTest, CharType) {
    EXPECT_EQ(fmt::format("{}", 'A'), "65");
    EXPECT_EQ(fmt::format("{:c}", 65), "A");
    EXPECT_EQ(fmt::format("{:c}", 0x41), "A");
}

// Test formatting with bool type
TEST(FormatIntegerTest, BoolType) {
    EXPECT_EQ(fmt::format("{}", true), "true");
    EXPECT_EQ(fmt::format("{}", false), "false");
    EXPECT_EQ(fmt::format("{:d}", true), "1");
    EXPECT_EQ(fmt::format("{:d}", false), "0");
}