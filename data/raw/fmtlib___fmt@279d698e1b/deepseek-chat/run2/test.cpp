#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Test formatting integers with various widths, alignments, and fill characters
TEST(FormatIntegerTest, WidthAndAlignment) {
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
    EXPECT_EQ(fmt::format("{:+}", 0), "+0");
    
    EXPECT_EQ(fmt::format("{: }", 42), " 42");
    EXPECT_EQ(fmt::format("{: }", -42), "-42");
    EXPECT_EQ(fmt::format("{: }", 0), " 0");
    
    EXPECT_EQ(fmt::format("{:-}", 42), "42");
    EXPECT_EQ(fmt::format("{:-}", -42), "-42");
    EXPECT_EQ(fmt::format("{:-}", 0), "0");
}

// Test formatting integers with different bases
TEST(FormatIntegerTest, BaseFormatting) {
    // Binary
    EXPECT_EQ(fmt::format("{:b}", 42), "101010");
    EXPECT_EQ(fmt::format("{:#b}", 42), "0b101010");
    
    // Octal
    EXPECT_EQ(fmt::format("{:o}", 42), "52");
    EXPECT_EQ(fmt::format("{:#o}", 42), "052");
    
    // Hexadecimal (lowercase)
    EXPECT_EQ(fmt::format("{:x}", 42), "2a");
    EXPECT_EQ(fmt::format("{:#x}", 42), "0x2a");
    
    // Hexadecimal (uppercase)
    EXPECT_EQ(fmt::format("{:X}", 42), "2A");
    EXPECT_EQ(fmt::format("{:#X}", 42), "0X2A");
}

// Test formatting integers with width and base together
TEST(FormatIntegerTest, WidthAndBase) {
    EXPECT_EQ(fmt::format("{:10b}", 42), "    101010");
    EXPECT_EQ(fmt::format("{:#10b}", 42), "  0b101010");
    EXPECT_EQ(fmt::format("{:10x}", 255), "        ff");
    EXPECT_EQ(fmt::format("{:#10x}", 255), "    0xff");
    EXPECT_EQ(fmt::format("{:10X}", 255), "        FF");
    EXPECT_EQ(fmt::format("{:#10X}", 255), "    0XFF");
}

// Test formatting integers with zero padding and base
TEST(FormatIntegerTest, ZeroPaddingAndBase) {
    EXPECT_EQ(fmt::format("{:010b}", 42), "0000101010");
    EXPECT_EQ(fmt::format("{:#010b}", 42), "0b00101010");
    EXPECT_EQ(fmt::format("{:010x}", 255), "00000000ff");
    EXPECT_EQ(fmt::format("{:#010x}", 255), "0x000000ff");
}

// Test formatting integers with precision (for integers, precision sets minimum digits)
TEST(FormatIntegerTest, Precision) {
    EXPECT_EQ(fmt::format("{:.5}", 42), "00042");
    EXPECT_EQ(fmt::format("{:.5}", -42), "-00042");
    EXPECT_EQ(fmt::format("{:8.5}", 42), "   00042");
    EXPECT_EQ(fmt::format("{:8.5}", -42), "  -00042");
}

// Test formatting with zero and edge cases
TEST(FormatIntegerTest, ZeroAndEdgeCases) {
    EXPECT_EQ(fmt::format("{}", 0), "0");
    EXPECT_EQ(fmt::format("{:}", 0), "0");
    EXPECT_EQ(fmt::format("{:1}", 0), "0");
    EXPECT_EQ(fmt::format("{:2}", 0), " 0");
    EXPECT_EQ(fmt::format("{:02}", 0), "00");
    
    // Maximum and minimum values
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::max()), "2147483647");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::min()), "-2147483648");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<unsigned>::max()), "4294967295");
    
    // Large width
    EXPECT_EQ(fmt::format("{:20}", 42), "                  42");
    EXPECT_EQ(fmt::format("{:20}", -42), "                 -42");
}

// Test formatting with fill, align, and sign combined
TEST(FormatIntegerTest, FillAlignSign) {
    EXPECT_EQ(fmt::format("{:*>+10}", 42), "*******+42");
    EXPECT_EQ(fmt::format("{:*<+10}", 42), "+42*******");
    EXPECT_EQ(fmt::format("{:*^+10}", 42), "***+42****");
    EXPECT_EQ(fmt::format("{:*>+10}", -42), "*******-42");
    EXPECT_EQ(fmt::format("{:*> 10}", 42), "******* 42");
    EXPECT_EQ(fmt::format("{:*> 10}", -42), "*******-42");
}

// Test formatting with width smaller than number of digits
TEST(FormatIntegerTest, WidthSmallerThanDigits) {
    EXPECT_EQ(fmt::format("{:2}", 12345), "12345");
    EXPECT_EQ(fmt::format("{:2}", -12345), "-12345");
    EXPECT_EQ(fmt::format("{:2}", 0), "0");
}

// Test formatting with char types
TEST(FormatIntegerTest, CharTypes) {
    EXPECT_EQ(fmt::format("{:c}", 65), "A");
    EXPECT_EQ(fmt::format("{:c}", 97), "a");
    EXPECT_EQ(fmt::format("{:c}", 48), "0");
}

// Test formatting with long long types
TEST(FormatIntegerTest, LongLongTypes) {
    EXPECT_EQ(fmt::format("{}", 1234567890123LL), "1234567890123");
    EXPECT_EQ(fmt::format("{}", -1234567890123LL), "-1234567890123");
    EXPECT_EQ(fmt::format("{}", 0ULL), "0");
}

// Test formatting with different integer types
TEST(FormatIntegerTest, DifferentIntegerTypes) {
    short s = 42;
    EXPECT_EQ(fmt::format("{}", s), "42");
    
    unsigned short us = 42;
    EXPECT_EQ(fmt::format("{}", us), "42");
    
    long l = 42L;
    EXPECT_EQ(fmt::format("{}", l), "42");
    
    unsigned long ul = 42UL;
    EXPECT_EQ(fmt::format("{}", ul), "42");
    
    long long ll = 42LL;
    EXPECT_EQ(fmt::format("{}", ll), "42");
    
    unsigned long long ull = 42ULL;
    EXPECT_EQ(fmt::format("{}", ull), "42");
}

// Test formatting with negative numbers and alignment
TEST(FormatIntegerTest, NegativeNumbersWithAlignment) {
    EXPECT_EQ(fmt::format("{:5}", -1), "   -1");
    EXPECT_EQ(fmt::format("{:<5}", -1), "-1   ");
    EXPECT_EQ(fmt::format("{:^5}", -1), " -1  ");
    EXPECT_EQ(fmt::format("{:*>5}", -1), "***-1");
    EXPECT_EQ(fmt::format("{:*<5}", -1), "-1***");
    EXPECT_EQ(fmt::format("{:*^5}", -1), "-1***");
}

// Test formatting with zero padding and negative numbers
TEST(FormatIntegerTest, ZeroPaddingNegative) {
    EXPECT_EQ(fmt::format("{:05}", -1), "-0001");
    EXPECT_EQ(fmt::format("{:05}", -100), "-0100");
    EXPECT_EQ(fmt::format("{:05}", -1000), "-1000");
}

// Test formatting with precision and negative numbers
TEST(FormatIntegerTest, PrecisionNegative) {
    EXPECT_EQ(fmt::format("{:.3}", -1), "-001");
    EXPECT_EQ(fmt::format("{:.3}", -10), "-010");
    EXPECT_EQ(fmt::format("{:.3}", -100), "-100");
    EXPECT_EQ(fmt::format("{:.3}", -1000), "-1000");
}

// Test formatting with width, precision, and negative numbers
TEST(FormatIntegerTest, WidthPrecisionNegative) {
    EXPECT_EQ(fmt::format("{:6.3}", -1), " -001");
    EXPECT_EQ(fmt::format("{:6.3}", -10), " -010");
    EXPECT_EQ(fmt::format("{:6.3}", -100), " -100");
    EXPECT_EQ(fmt::format("{:6.3}", -1000), "-1000");
}