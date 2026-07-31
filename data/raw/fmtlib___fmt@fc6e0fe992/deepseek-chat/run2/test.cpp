#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test that numeric alignment with sign correctly adjusts width and removes sign from output
TEST(NumericAlignmentTest, SignWithNumericAlignmentReducesWidth) {
    // Format a negative number with zero padding and numeric alignment
    // The sign should be placed before padding, and width should be reduced by 1
    std::string result = fmt::format("{:05d}", -42);
    EXPECT_EQ(result, "-0042");  // sign before padding, width 5 includes sign
}

// Test that numeric alignment with positive sign works correctly
TEST(NumericAlignmentTest, PositiveSignWithNumericAlignment) {
    std::string result = fmt::format("{:+05d}", 42);
    EXPECT_EQ(result, "+0042");
}

// Test that numeric alignment with space sign works correctly
TEST(NumericAlignmentTest, SpaceSignWithNumericAlignment) {
    std::string result = fmt::format("{: 05d}", 42);
    EXPECT_EQ(result, " 0042");
}

// Test that default alignment for non-numeric types is right-aligned
TEST(DefaultAlignmentTest, NonNumericDefaultIsRightAligned) {
    std::string result = fmt::format("{:10}", "hello");
    EXPECT_EQ(result, "     hello");
}

// Test that default alignment for numbers is right-aligned
TEST(DefaultAlignmentTest, NumericDefaultIsRightAligned) {
    std::string result = fmt::format("{:10d}", 42);
    EXPECT_EQ(result, "        42");
}

// Test that explicit right alignment works for non-numeric types
TEST(ExplicitAlignmentTest, RightAlignmentNonNumeric) {
    std::string result = fmt::format("{:>10}", "hello");
    EXPECT_EQ(result, "     hello");
}

// Test that explicit left alignment works for non-numeric types
TEST(ExplicitAlignmentTest, LeftAlignmentNonNumeric) {
    std::string result = fmt::format("{:<10}", "hello");
    EXPECT_EQ(result, "hello     ");
}

// Test that center alignment works for non-numeric types
TEST(ExplicitAlignmentTest, CenterAlignmentNonNumeric) {
    std::string result = fmt::format("{:^10}", "hello");
    EXPECT_EQ(result, "  hello   ");
}

// Test that numeric alignment with zero padding and sign works for zero value
TEST(EdgeCaseTest, ZeroWithSignAndNumericAlignment) {
    std::string result = fmt::format("{:+05d}", 0);
    EXPECT_EQ(result, "+0000");
}

// Test that numeric alignment with sign and width exactly matching number of digits
TEST(EdgeCaseTest, SignWithExactWidth) {
    std::string result = fmt::format("{:+03d}", 42);
    EXPECT_EQ(result, "+42");
}

// Test that numeric alignment with sign and width smaller than number of digits
TEST(EdgeCaseTest, SignWithInsufficientWidth) {
    std::string result = fmt::format("{:+02d}", 42);
    EXPECT_EQ(result, "+42");
}

// Test that sign is not doubled when using numeric alignment with negative numbers
TEST(EdgeCaseTest, NegativeSignNotDoubled) {
    std::string result = fmt::format("{:05d}", -42);
    EXPECT_EQ(result, "-0042");
    EXPECT_NE(result, "--042");  // ensure no double sign
}

// Test that sign is correctly placed before padding for positive numbers with explicit plus
TEST(EdgeCaseTest, PositiveSignBeforePadding) {
    std::string result = fmt::format("{:+06d}", 123);
    EXPECT_EQ(result, "+00123");
}

// Test that space sign for positive numbers works with numeric alignment
TEST(EdgeCaseTest, SpaceSignBeforePadding) {
    std::string result = fmt::format("{: 06d}", 123);
    EXPECT_EQ(result, " 00123");
}

// Test that numeric alignment with sign and zero width (minimum width) works
TEST(EdgeCaseTest, SignWithZeroWidth) {
    std::string result = fmt::format("{:+0d}", 42);
    EXPECT_EQ(result, "+42");
}

// Test that default alignment for negative numbers is right-aligned
TEST(DefaultAlignmentTest, NegativeNumberDefaultAlignment) {
    std::string result = fmt::format("{:10d}", -42);
    EXPECT_EQ(result, "       -42");
}

// Test that explicit right alignment for negative numbers works
TEST(ExplicitAlignmentTest, NegativeNumberRightAlignment) {
    std::string result = fmt::format("{:>10d}", -42);
    EXPECT_EQ(result, "       -42");
}

// Test that numeric alignment with sign and large width works correctly
TEST(EdgeCaseTest, SignWithLargeWidth) {
    std::string result = fmt::format("{:+010d}", 42);
    EXPECT_EQ(result, "+000000042");
}

// Test that sign is not affected when alignment is not numeric
TEST(NonNumericAlignmentTest, SignWithNonNumericAlignment) {
    // For non-numeric types, sign should not be present, but test that
    // the alignment logic doesn't break when sign is present in format spec
    std::string result = fmt::format("{:+<10}", "hello");
    EXPECT_EQ(result, "hello+++++");
}

// Test that numeric alignment with sign and fill character works
TEST(FillCharacterTest, SignWithFillAndNumericAlignment) {
    std::string result = fmt::format("{:*>+06d}", 42);
    EXPECT_EQ(result, "***+42");
}

// Test that numeric alignment with sign and zero fill works (zero fill implies numeric alignment)
TEST(FillCharacterTest, ZeroFillWithSign) {
    std::string result = fmt::format("{:+06d}", 42);
    EXPECT_EQ(result, "+00042");
}

// Test that width is correctly reduced when sign is present with numeric alignment
TEST(WidthReductionTest, WidthReducedByOneForSign) {
    std::string result = fmt::format("{:05d}", -7);
    EXPECT_EQ(result, "-0007");
    EXPECT_EQ(result.size(), 5);
}

// Test that width is not reduced when sign is not present
TEST(WidthReductionTest, NoSignNoWidthReduction) {
    std::string result = fmt::format("{:05d}", 7);
    EXPECT_EQ(result, "00007");
    EXPECT_EQ(result.size(), 5);
}

// Test that multiple signs are not added
TEST(EdgeCaseTest, NoDoubleSign) {
    std::string result = fmt::format("{:+05d}", -42);
    EXPECT_EQ(result, "-0042");
    // The explicit plus should be overridden by the negative sign
    EXPECT_NE(result, "+-042");
    EXPECT_NE(result, "-+042");
}