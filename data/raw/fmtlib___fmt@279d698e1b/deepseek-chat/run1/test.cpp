#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Test formatting integers with sign, width, and alignment
TEST(FormatTest, SignedIntegerWithWidthAndAlignment) {
    // Positive number with sign, right-aligned in width 10
    std::string result = fmt::format("{:+>10}", 42);
    EXPECT_EQ(result, "       +42");

    // Negative number with sign, right-aligned in width 10
    result = fmt::format("{:+>10}", -42);
    EXPECT_EQ(result, "       -42");

    // Positive number with sign, left-aligned in width 10
    result = fmt::format("{:+<10}", 42);
    EXPECT_EQ(result, "+42       ");

    // Negative number with sign, left-aligned in width 10
    result = fmt::format("{:+<10}", -42);
    EXPECT_EQ(result, "-42       ");

    // Positive number with sign, center-aligned in width 10
    result = fmt::format("{:+^10}", 42);
    EXPECT_EQ(result, "   +42    ");

    // Negative number with sign, center-aligned in width 10
    result = fmt::format("{:+^10}", -42);
    EXPECT_EQ(result, "   -42    ");
}

// Test formatting integers with zero padding and sign
TEST(FormatTest, SignedIntegerWithZeroPadding) {
    // Positive number with sign and zero padding
    std::string result = fmt::format("{:+010}", 42);
    EXPECT_EQ(result, "+000000042");

    // Negative number with sign and zero padding
    result = fmt::format("{:+010}", -42);
    EXPECT_EQ(result, "-000000042");

    // Zero with sign and zero padding
    result = fmt::format("{:+010}", 0);
    EXPECT_EQ(result, "+000000000");
}

// Test formatting integers with space for sign
TEST(FormatTest, SignedIntegerWithSpaceSign) {
    // Positive number with space sign
    std::string result = fmt::format("{: }", 42);
    EXPECT_EQ(result, " 42");

    // Negative number with space sign
    result = fmt::format("{: }", -42);
    EXPECT_EQ(result, "-42");

    // Zero with space sign
    result = fmt::format("{: }", 0);
    EXPECT_EQ(result, " 0");
}

// Test formatting integers with minus sign only (default)
TEST(FormatTest, SignedIntegerWithMinusSign) {
    // Positive number (no sign)
    std::string result = fmt::format("{:10}", 42);
    EXPECT_EQ(result, "        42");

    // Negative number (minus sign)
    result = fmt::format("{:10}", -42);
    EXPECT_EQ(result, "       -42");

    // Zero (no sign)
    result = fmt::format("{:10}", 0);
    EXPECT_EQ(result, "         0");
}

// Test edge cases: min/max integer values
TEST(FormatTest, SignedIntegerEdgeValues) {
    // Maximum int
    std::string result = fmt::format("{:+010}", std::numeric_limits<int>::max());
    EXPECT_EQ(result, "+2147483647");

    // Minimum int (note: -2147483648 is one more than max positive)
    result = fmt::format("{:+010}", std::numeric_limits<int>::min());
    EXPECT_EQ(result, "-2147483648");

    // Minimum int with width 11
    result = fmt::format("{:+>11}", std::numeric_limits<int>::min());
    EXPECT_EQ(result, "  -2147483648");
}

// Test formatting with custom width smaller than number
TEST(FormatTest, WidthSmallerThanNumber) {
    // Width smaller than number of digits
    std::string result = fmt::format("{:+>3}", 12345);
    EXPECT_EQ(result, "+12345");

    // Negative number, width smaller
    result = fmt::format("{:+>3}", -12345);
    EXPECT_EQ(result, "-12345");
}

// Test formatting with zero width
TEST(FormatTest, ZeroWidth) {
    // Zero width with sign
    std::string result = fmt::format("{:+>0}", 42);
    EXPECT_EQ(result, "+42");

    // Zero width with negative
    result = fmt::format("{:+>0}", -42);
    EXPECT_EQ(result, "-42");
}

// Test formatting with large width
TEST(FormatTest, LargeWidth) {
    // Large width with positive number
    std::string result = fmt::format("{:+>100}", 42);
    EXPECT_EQ(result, std::string(97, ' ') + "+42");

    // Large width with negative number
    result = fmt::format("{:+>100}", -42);
    EXPECT_EQ(result, std::string(97, ' ') + "-42");
}

// Test formatting with fill character
TEST(FormatTest, CustomFillCharacter) {
    // Custom fill character '#'
    std::string result = fmt::format("{:#>10}", 42);
    EXPECT_EQ(result, "########42");

    // Custom fill character with sign
    result = fmt::format("{:#>+10}", 42);
    EXPECT_EQ(result, "#######+42");

    // Custom fill character with negative
    result = fmt::format("{:#>+10}", -42);
    EXPECT_EQ(result, "#######-42");
}

// Test formatting with different integer types
TEST(FormatTest, DifferentIntegerTypes) {
    // long long
    std::string result = fmt::format("{:+>10}", 1234567890123LL);
    EXPECT_EQ(result, " 1234567890123");

    // unsigned long long (should not have sign)
    result = fmt::format("{:>10}", 1234567890123ULL);
    EXPECT_EQ(result, "1234567890123");

    // short
    result = fmt::format("{:+>10}", static_cast<short>(42));
    EXPECT_EQ(result, "       +42");
}

// Test formatting with precision (should not affect integers)
TEST(FormatTest, PrecisionWithInteger) {
    // Precision with integer (ignored)
    std::string result = fmt::format("{:+>10.5}", 42);
    EXPECT_EQ(result, "       +42");

    // Precision with negative integer
    result = fmt::format("{:+>10.5}", -42);
    EXPECT_EQ(result, "       -42");
}

// Test formatting zero with various options
TEST(FormatTest, ZeroWithVariousOptions) {
    // Zero with sign and width
    std::string result = fmt::format("{:+>10}", 0);
    EXPECT_EQ(result, "        +0");

    // Zero with space sign
    result = fmt::format("{: >10}", 0);
    EXPECT_EQ(result, "         0");

    // Zero with minus sign only
    result = fmt::format("{:>10}", 0);
    EXPECT_EQ(result, "         0");
}

// Test formatting with alignment and sign for negative zero (should not exist, but test)
TEST(FormatTest, NegativeZero) {
    // Negative zero (should format as zero)
    std::string result = fmt::format("{:+>10}", -0);
    EXPECT_EQ(result, "        +0");
}