#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test that numeric alignment with sign correctly adjusts width and removes sign from output
TEST(NumericAlignmentTest, SignWithNumericAlignment) {
    // Format a negative number with '0' fill and width 5, which triggers numeric alignment
    std::string result = fmt::format("{:05d}", -42);
    // Expected: "-0042" (sign at front, then zero-padded to width 5)
    EXPECT_EQ(result, "-0042");
}

// Test that numeric alignment with positive sign also works
TEST(NumericAlignmentTest, PositiveSignWithNumericAlignment) {
    std::string result = fmt::format("{:+05d}", 42);
    // Expected: "+0042"
    EXPECT_EQ(result, "+0042");
}

// Test that numeric alignment with space sign works
TEST(NumericAlignmentTest, SpaceSignWithNumericAlignment) {
    std::string result = fmt::format("{: 05d}", 42);
    // Expected: " 0042" (space before zero-padded number)
    EXPECT_EQ(result, " 0042");
}

// Test that numeric alignment without sign does not modify width
TEST(NumericAlignmentTest, NoSignNumericAlignment) {
    std::string result = fmt::format("{:05d}", 42);
    // Expected: "00042" (no sign, zero-padded to width 5)
    EXPECT_EQ(result, "00042");
}

// Test that default alignment (right) works correctly with sign
TEST(NumericAlignmentTest, DefaultAlignmentWithSign) {
    std::string result = fmt::format("{:+5d}", 42);
    // Expected: "  +42" (right-aligned, sign before number, width 5)
    EXPECT_EQ(result, "  +42");
}

// Test that default alignment without sign works
TEST(NumericAlignmentTest, DefaultAlignmentNoSign) {
    std::string result = fmt::format("{:5d}", 42);
    // Expected: "   42" (right-aligned, width 5)
    EXPECT_EQ(result, "   42");
}

// Test that left alignment with sign does not trigger numeric alignment path
TEST(NumericAlignmentTest, LeftAlignmentWithSign) {
    std::string result = fmt::format("{:<+5d}", 42);
    // Expected: "+42  " (left-aligned, sign at front, padded with spaces)
    EXPECT_EQ(result, "+42  ");
}

// Test that center alignment with sign works
TEST(NumericAlignmentTest, CenterAlignmentWithSign) {
    std::string result = fmt::format("{:^+5d}", 42);
    // Expected: " +42 " (centered, sign at front)
    EXPECT_EQ(result, " +42 ");
}

// Test that numeric alignment with zero width (width_ = 0) works
TEST(NumericAlignmentTest, NumericAlignmentZeroWidth) {
    std::string result = fmt::format("{:0d}", 42);
    // Expected: "42" (no width specified, no padding)
    EXPECT_EQ(result, "42");
}

// Test that numeric alignment with sign and zero width works
TEST(NumericAlignmentTest, NumericAlignmentSignZeroWidth) {
    std::string result = fmt::format("{:+0d}", 42);
    // Expected: "+42" (sign, no padding)
    EXPECT_EQ(result, "+42");
}

// Test that numeric alignment with width less than number length works
TEST(NumericAlignmentTest, NumericAlignmentWidthLessThanNumber) {
    std::string result = fmt::format("{:02d}", 12345);
    // Expected: "12345" (width 2 is less than number length, no padding)
    EXPECT_EQ(result, "12345");
}

// Test that numeric alignment with sign and width less than number length works
TEST(NumericAlignmentTest, NumericAlignmentSignWidthLessThanNumber) {
    std::string result = fmt::format("{:+02d}", 12345);
    // Expected: "+12345" (sign, no padding because width < number length + sign)
    EXPECT_EQ(result, "+12345");
}

// Test that numeric alignment with large width works
TEST(NumericAlignmentTest, NumericAlignmentLargeWidth) {
    std::string result = fmt::format("{:010d}", -42);
    // Expected: "-000000042" (sign at front, zero-padded to width 10)
    EXPECT_EQ(result, "-000000042");
}

// Test that numeric alignment with sign and large width works
TEST(NumericAlignmentTest, NumericAlignmentSignLargeWidth) {
    std::string result = fmt::format("{:+010d}", 42);
    // Expected: "+000000042" (sign at front, zero-padded to width 10)
    EXPECT_EQ(result, "+000000042");
}

// Test that numeric alignment with space sign and large width works
TEST(NumericAlignmentTest, NumericAlignmentSpaceSignLargeWidth) {
    std::string result = fmt::format("{: 010d}", 42);
    // Expected: " 000000042" (space at front, zero-padded to width 10)
    EXPECT_EQ(result, " 000000042");
}

// Test that default alignment with sign and width less than number length works
TEST(NumericAlignmentTest, DefaultAlignmentSignWidthLessThanNumber) {
    std::string result = fmt::format("{:+3d}", 12345);
    // Expected: "+12345" (width 3 is less than number length + sign, no padding)
    EXPECT_EQ(result, "+12345");
}

// Test that default alignment with sign and large width works
TEST(NumericAlignmentTest, DefaultAlignmentSignLargeWidth) {
    std::string result = fmt::format("{:+10d}", 42);
    // Expected: "       +42" (right-aligned, sign before number, padded with spaces)
    EXPECT_EQ(result, "       +42");
}

// Test that numeric alignment with negative number and zero width works
TEST(NumericAlignmentTest, NegativeNumericAlignmentZeroWidth) {
    std::string result = fmt::format("{:0d}", -42);
    // Expected: "-42" (sign, no padding)
    EXPECT_EQ(result, "-42");
}

// Test that numeric alignment with negative number and width works
TEST(NumericAlignmentTest, NegativeNumericAlignmentWithWidth) {
    std::string result = fmt::format("{:05d}", -42);
    // Expected: "-0042" (sign at front, zero-padded to width 5)
    EXPECT_EQ(result, "-0042");
}

// Test that numeric alignment with zero value works
TEST(NumericAlignmentTest, NumericAlignmentZeroValue) {
    std::string result = fmt::format("{:05d}", 0);
    // Expected: "00000" (zero-padded to width 5)
    EXPECT_EQ(result, "00000");
}

// Test that numeric alignment with sign and zero value works
TEST(NumericAlignmentTest, NumericAlignmentSignZeroValue) {
    std::string result = fmt::format("{:+05d}", 0);
    // Expected: "+0000" (sign at front, zero-padded to width 5)
    EXPECT_EQ(result, "+0000");
}

// Test that numeric alignment with space sign and zero value works
TEST(NumericAlignmentTest, NumericAlignmentSpaceSignZeroValue) {
    std::string result = fmt::format("{: 05d}", 0);
    // Expected: " 0000" (space at front, zero-padded to width 5)
    EXPECT_EQ(result, " 0000");
}

// Test that default alignment with zero value works
TEST(NumericAlignmentTest, DefaultAlignmentZeroValue) {
    std::string result = fmt::format("{:5d}", 0);
    // Expected: "    0" (right-aligned, width 5)
    EXPECT_EQ(result, "    0");
}

// Test that default alignment with sign and zero value works
TEST(NumericAlignmentTest, DefaultAlignmentSignZeroValue) {
    std::string result = fmt::format("{:+5d}", 0);
    // Expected: "   +0" (right-aligned, sign before zero, width 5)
    EXPECT_EQ(result, "   +0");
}

// Test that numeric alignment with fill character other than '0' does not trigger numeric alignment
TEST(NumericAlignmentTest, NonZeroFillNumericAlignment) {
    std::string result = fmt::format("{:*>5d}", 42);
    // Expected: "***42" (right-aligned with '*' fill, not numeric alignment)
    EXPECT_EQ(result, "***42");
}

// Test that numeric alignment with sign and non-zero fill works
TEST(NumericAlignmentTest, NonZeroFillSignNumericAlignment) {
    std::string result = fmt::format("{:*>+5d}", 42);
    // Expected: "**+42" (right-aligned with '*' fill, sign at front)
    EXPECT_EQ(result, "**+42");
}

// Test that numeric alignment with hex format works
TEST(NumericAlignmentTest, NumericAlignmentHex) {
    std::string result = fmt::format("{:08x}", 255);
    // Expected: "000000ff" (zero-padded hex, width 8)
    EXPECT_EQ(result, "000000ff");
}

// Test that numeric alignment with sign and hex format works
TEST(NumericAlignmentTest, NumericAlignmentSignHex) {
    std::string result = fmt::format("{:+08x}", 255);
    // Expected: "+00000ff" (sign at front, zero-padded hex, width 8)
    EXPECT_EQ(result, "+00000ff");
}

// Test that numeric alignment with octal format works
TEST(NumericAlignmentTest, NumericAlignmentOctal) {
    std::string result = fmt::format("{:08o}", 255);
    // Expected: "00000377" (zero-padded octal, width 8)
    EXPECT_EQ(result, "00000377");
}

// Test that numeric alignment with binary format works
TEST(NumericAlignmentTest, NumericAlignmentBinary) {
    std::string result = fmt::format("{:08b}", 255);
    // Expected: "11111111" (zero-padded binary, width 8, but note: binary doesn't pad with zeros by default in some implementations)
    // Actually fmt::format pads with zeros for binary as well
    EXPECT_EQ(result, "11111111");
}

// Test that numeric alignment with sign and binary format works
TEST(NumericAlignmentTest, NumericAlignmentSignBinary) {
    std::string result = fmt::format("{:+08b}", 255);
    // Expected: "+1111111" (sign at front, zero-padded binary, width 8)
    EXPECT_EQ(result, "+1111111");
}

// Test that default alignment with sign and hex format works
TEST(NumericAlignmentTest, DefaultAlignmentSignHex) {
    std::string result = fmt::format("{:+8x}", 255);
    // Expected: "     +ff" (right-aligned, sign before hex, width 8)
    EXPECT_EQ(result, "     +ff");
}

// Test that numeric alignment with negative hex works
TEST(NumericAlignmentTest, NumericAlignmentNegativeHex) {
    std::string result = fmt::format("{:08x}", -1);
    // Expected: "ffffffff" (negative hex, zero-padded to width 8)
    EXPECT_EQ(result, "ffffffff");
}

// Test that numeric alignment with sign and negative hex works
TEST(NumericAlignmentTest, NumericAlignmentSignNegativeHex) {
    std::string result = fmt::format("{:+08x}", -1);
    // Expected: "-fffffff" (sign at front, zero-padded hex, width 8)
    EXPECT_EQ(result, "-fffffff");
}