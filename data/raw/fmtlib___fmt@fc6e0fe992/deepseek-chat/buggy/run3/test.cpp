#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>

// Test the alignment and sign handling logic that is exercised through
// the public formatting API. The focal function is invoked internally
// when formatting numeric types with alignment and sign specifiers.

// Helper to test formatting with specific alignment, sign, and width
std::string format_numeric(char align, char sign, int width, int value) {
    std::string format_str = "{:";
    if (align != 0) {
        format_str += '<';  // placeholder, will be replaced
        format_str += align;
    }
    if (sign != 0) {
        format_str += sign;
    }
    if (width > 0) {
        format_str += std::to_string(width);
    }
    format_str += '}';
    // Replace the placeholder with the actual alignment character
    if (align != 0) {
        size_t pos = format_str.find('<');
        format_str[pos] = align;
    }
    return fmt::format(format_str, value);
}

// Test ALIGN_NUMERIC with sign: sign should be placed before padding
TEST(AlignNumericTest, SignBeforePadding) {
    // For ALIGN_NUMERIC ('='), sign should appear before the number
    // but after any fill characters. Width 5 with sign '+' on value 42
    // should produce "+++42" (fill ' ', align '=', sign '+')
    std::string result = fmt::format("{:=+5}", 42);
    EXPECT_EQ(result, "+  42");  // sign before number, right-aligned within field
}

// Test ALIGN_NUMERIC with negative sign
TEST(AlignNumericTest, NegativeSignNumericAlign) {
    std::string result = fmt::format("{:=+5}", -42);
    EXPECT_EQ(result, "-  42");
}

// Test ALIGN_NUMERIC with zero width (edge case)
TEST(AlignNumericTest, NumericAlignZeroWidth) {
    std::string result = fmt::format("{:=+0}", 42);
    EXPECT_EQ(result, "+42");
}

// Test ALIGN_NUMERIC with sign and width exactly matching number length
TEST(AlignNumericTest, NumericAlignExactWidth) {
    std::string result = fmt::format("{:=+3}", 42);
    EXPECT_EQ(result, "+42");
}

// Test ALIGN_DEFAULT (no explicit alignment) with sign: sign should be part of the number
TEST(AlignDefaultTest, SignIncreasesWidth) {
    // Default alignment for numbers is right-aligned. Sign counts toward width.
    // Width 5, sign '+', value 42 should produce "  +42"
    std::string result = fmt::format("{:+5}", 42);
    EXPECT_EQ(result, "  +42");
}

// Test ALIGN_DEFAULT with negative sign
TEST(AlignDefaultTest, NegativeSignDefaultAlign) {
    std::string result = fmt::format("{:+5}", -42);
    EXPECT_EQ(result, "  -42");
}

// Test ALIGN_DEFAULT with zero width
TEST(AlignDefaultTest, DefaultAlignZeroWidth) {
    std::string result = fmt::format("{:+}", 42);
    EXPECT_EQ(result, "+42");
}

// Test ALIGN_LEFT with sign: sign should be part of the number, left-aligned
TEST(AlignLeftTest, SignLeftAligned) {
    std::string result = fmt::format("{:<+5}", 42);
    EXPECT_EQ(result, "+42  ");
}

// Test ALIGN_RIGHT with sign: sign should be part of the number, right-aligned
TEST(AlignRightTest, SignRightAligned) {
    std::string result = fmt::format("{:>+5}", 42);
    EXPECT_EQ(result, "  +42");
}

// Test ALIGN_CENTER with sign: sign should be part of the number, centered
TEST(AlignCenterTest, SignCentered) {
    std::string result = fmt::format("{:^+5}", 42);
    EXPECT_EQ(result, " +42 ");
}

// Test ALIGN_NUMERIC with sign and fill character
TEST(AlignNumericTest, FillCharacterWithSign) {
    std::string result = fmt::format("{:*=+5}", 42);
    EXPECT_EQ(result, "+**42");
}

// Test ALIGN_NUMERIC with sign and width less than number length
TEST(AlignNumericTest, NumericAlignWidthLessThanNumber) {
    std::string result = fmt::format("{:=+2}", 123);
    EXPECT_EQ(result, "+123");
}

// Test no sign (sign = 0) with ALIGN_NUMERIC: should behave like ALIGN_RIGHT
TEST(AlignNumericTest, NoSignNumericAlign) {
    std::string result = fmt::format("{:=5}", 42);
    EXPECT_EQ(result, "   42");
}

// Test no sign with ALIGN_DEFAULT: should be right-aligned
TEST(AlignDefaultTest, NoSignDefaultAlign) {
    std::string result = fmt::format("{:5}", 42);
    EXPECT_EQ(result, "   42");
}

// Test sign with ALIGN_NUMERIC and width 1 (edge case: width becomes 0 after sign)
TEST(AlignNumericTest, NumericAlignWidthOneWithSign) {
    std::string result = fmt::format("{:=+1}", 42);
    EXPECT_EQ(result, "+42");
}

// Test sign with ALIGN_NUMERIC and width 0 (edge case)
TEST(AlignNumericTest, NumericAlignWidthZeroWithSign) {
    std::string result = fmt::format("{:=+0}", 42);
    EXPECT_EQ(result, "+42");
}

// Test negative zero (should produce "-0" with sign)
TEST(AlignNumericTest, NegativeZeroWithSign) {
    std::string result = fmt::format("{:=+5}", -0);
    EXPECT_EQ(result, "    0");  // -0 is 0, so no sign
}

// Test sign with ALIGN_NUMERIC and large width
TEST(AlignNumericTest, NumericAlignLargeWidth) {
    std::string result = fmt::format("{:=+10}", 42);
    EXPECT_EQ(result, "+       42");
}

// Test sign with ALIGN_NUMERIC and fill character that is a digit
TEST(AlignNumericTest, DigitFillCharacter) {
    std::string result = fmt::format("{:0=+5}", 42);
    EXPECT_EQ(result, "+0042");
}