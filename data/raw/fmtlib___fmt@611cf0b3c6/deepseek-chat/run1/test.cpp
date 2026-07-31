#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cstdint>

// Test on_oct behavior through the public fmt::format API with 'o' format specifier
// The function is called internally when formatting integers with 'o' type

TEST(OctFormatTest, BasicOctal) {
    // Normal positive integer
    EXPECT_EQ(fmt::format("{:o}", 42), "52");
    // Zero
    EXPECT_EQ(fmt::format("{:o}", 0), "0");
    // Large number
    EXPECT_EQ(fmt::format("{:o}", 255), "377");
    // Maximum 32-bit unsigned
    EXPECT_EQ(fmt::format("{:o}", 4294967295u), "37777777777");
}

TEST(OctFormatTest, AlternativeFormPrefix) {
    // alt flag adds '0' prefix for non-zero values
    EXPECT_EQ(fmt::format("{:#o}", 42), "052");
    // Zero with alt flag - no prefix (prefix only added if abs_value != 0)
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");
    // Large number with alt
    EXPECT_EQ(fmt::format("{:#o}", 255), "0377");
    // Single digit with alt
    EXPECT_EQ(fmt::format("{:#o}", 7), "07");
}

TEST(OctFormatTest, PrecisionInteraction) {
    // Precision greater than number of digits - no prefix (per on_oct logic)
    EXPECT_EQ(fmt::format("{:#.5o}", 42), "00052");
    // Precision equal to number of digits - prefix added
    EXPECT_EQ(fmt::format("{:#.2o}", 42), "052");
    // Precision less than number of digits - prefix added
    EXPECT_EQ(fmt::format("{:#.1o}", 42), "052");
    // Zero with precision and alt - no prefix (abs_value == 0)
    EXPECT_EQ(fmt::format("{:#.3o}", 0), "000");
    // Precision zero with alt and non-zero
    EXPECT_EQ(fmt::format("{:#.0o}", 42), "052");
}

TEST(OctFormatTest, WidthAndAlignment) {
    // Width with left alignment
    EXPECT_EQ(fmt::format("{:6o}", 42), "52    ");
    // Width with right alignment
    EXPECT_EQ(fmt::format("{:>6o}", 42), "    52");
    // Width with center alignment
    EXPECT_EQ(fmt::format("{:^6o}", 42), "  52  ");
    // Width with alt and precision
    EXPECT_EQ(fmt::format("{:#8.4o}", 42), "   0052");
}

TEST(OctFormatTest, NegativeNumbers) {
    // Negative numbers - abs_value used for digit count, sign handled separately
    EXPECT_EQ(fmt::format("{:o}", -42), "-52");
    // Negative with alt flag
    EXPECT_EQ(fmt::format("{:#o}", -42), "-052");
    // Negative with precision
    EXPECT_EQ(fmt::format("{:#.5o}", -42), "-00052");
}

TEST(OctFormatTest, EdgeCases) {
    // Value 1 (single digit)
    EXPECT_EQ(fmt::format("{:#o}", 1), "01");
    // Value 7 (max single octal digit)
    EXPECT_EQ(fmt::format("{:#o}", 7), "07");
    // Value 8 (two octal digits)
    EXPECT_EQ(fmt::format("{:#o}", 8), "010");
    // Large precision with alt - prefix suppressed because precision > num_digits
    EXPECT_EQ(fmt::format("{:#.10o}", 42), "0000000052");
    // Very large precision with zero
    EXPECT_EQ(fmt::format("{:#.10o}", 0), "0000000000");
}

TEST(OctFormatTest, UnsignedTypes) {
    // unsigned int
    EXPECT_EQ(fmt::format("{:o}", 42u), "52");
    // unsigned long
    EXPECT_EQ(fmt::format("{:o}", 42ul), "52");
    // unsigned long long
    EXPECT_EQ(fmt::format("{:o}", 42ull), "52");
    // uint8_t
    EXPECT_EQ(fmt::format("{:o}", static_cast<uint8_t>(42)), "52");
}

TEST(OctFormatTest, FillCharacter) {
    // Custom fill character
    EXPECT_EQ(fmt::format("{:*>6o}", 42), "****52");
    // Custom fill with alt
    EXPECT_EQ(fmt::format("{:*<#6o}", 42), "052***");
    // Custom fill with precision
    EXPECT_EQ(fmt::format("{:*^#8.4o}", 42), "**0052**");
}

TEST(OctFormatTest, ZeroFlag) {
    // Zero padding
    EXPECT_EQ(fmt::format("{:06o}", 42), "000052");
    // Zero padding with alt
    EXPECT_EQ(fmt::format("{:#06o}", 42), "000052");  // '0' prefix counts as digit
    // Zero padding with negative
    EXPECT_EQ(fmt::format("{:06o}", -42), "-00052");
}

TEST(OctFormatTest, LargeValues) {
    // 64-bit values
    EXPECT_EQ(fmt::format("{:o}", 1ull << 60), "1000000000000000000000");
    // Maximum 64-bit unsigned
    EXPECT_EQ(fmt::format("{:o}", ~0ull), "1777777777777777777777");
    // Large value with alt
    EXPECT_EQ(fmt::format("{:#o}", 1ull << 60), "01000000000000000000000");
}