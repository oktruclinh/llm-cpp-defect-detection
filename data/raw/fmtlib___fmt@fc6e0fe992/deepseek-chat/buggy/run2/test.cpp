#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test the alignment and sign handling logic that is exercised through fmt::format
// The focal function processes align_spec with sign and alignment adjustments

class FormatAlignSignTest : public ::testing::Test {
protected:
    // Helper to test the internal behavior indirectly through format strings
    std::string format_with_spec(const std::string& fmt_str, int value) {
        return fmt::format(fmt_str, value);
    }
};

// Test default alignment (ALIGN_DEFAULT) with positive numbers
TEST_F(FormatAlignSignTest, DefaultAlignmentPositive) {
    // Default alignment for integers is right-aligned
    EXPECT_EQ(format_with_spec("{:10}", 42), "        42");
    EXPECT_EQ(format_with_spec("{:10}", 0), "         0");
}

// Test default alignment with negative numbers (sign should be included in width)
TEST_F(FormatAlignSignTest, DefaultAlignmentNegative) {
    EXPECT_EQ(format_with_spec("{:10}", -42), "       -42");
    EXPECT_EQ(format_with_spec("{:10}", -1), "        -1");
}

// Test explicit right alignment with sign
TEST_F(FormatAlignSignTest, RightAlignmentWithSign) {
    EXPECT_EQ(format_with_spec("{:>10}", 42), "        42");
    EXPECT_EQ(format_with_spec("{:>+10}", 42), "       +42");
    EXPECT_EQ(format_with_spec("{:>10}", -42), "       -42");
}

// Test left alignment with sign (sign should be before the number, not at edge)
TEST_F(FormatAlignSignTest, LeftAlignmentWithSign) {
    EXPECT_EQ(format_with_spec("{:<10}", 42), "42        ");
    EXPECT_EQ(format_with_spec("{:<+10}", 42), "+42       ");
    EXPECT_EQ(format_with_spec("{:<10}", -42), "-42       ");
}

// Test center alignment with sign
TEST_F(FormatAlignSignTest, CenterAlignmentWithSign) {
    EXPECT_EQ(format_with_spec("{:^10}", 42), "    42    ");
    EXPECT_EQ(format_with_spec("{:^+10}", 42), "   +42    ");
    EXPECT_EQ(format_with_spec("{:^10}", -42), "   -42    ");
}

// Test numeric alignment (ALIGN_NUMERIC) - sign before padding
TEST_F(FormatAlignSignTest, NumericAlignmentPositive) {
    // Numeric alignment for integers with zero padding
    EXPECT_EQ(format_with_spec("{:010}", 42), "0000000042");
    EXPECT_EQ(format_with_spec("{:+010}", 42), "+000000042");
    EXPECT_EQ(format_with_spec("{:010}", -42), "-000000042");
}

// Test numeric alignment with sign and width reduction
TEST_F(FormatAlignSignTest, NumericAlignmentSignWidthReduction) {
    // Sign character should be placed before padding, reducing effective width
    EXPECT_EQ(format_with_spec("{:+05}", 42), "+0042");
    EXPECT_EQ(format_with_spec("{:+05}", -42), "-0042");
    EXPECT_EQ(format_with_spec("{:05}", 42), "00042");
    EXPECT_EQ(format_with_spec("{:05}", -42), "-0042");
}

// Test numeric alignment with zero width (edge case)
TEST_F(FormatAlignSignTest, NumericAlignmentZeroWidth) {
    EXPECT_EQ(format_with_spec("{:0}", 42), "42");
    EXPECT_EQ(format_with_spec("{:+0}", 42), "+42");
    EXPECT_EQ(format_with_spec("{:0}", -42), "-42");
}

// Test numeric alignment with width equal to number of digits
TEST_F(FormatAlignSignTest, NumericAlignmentExactWidth) {
    EXPECT_EQ(format_with_spec("{:02}", 42), "42");
    EXPECT_EQ(format_with_spec("{:+03}", 42), "+42");
    EXPECT_EQ(format_with_spec("{:03}", -42), "-42");
}

// Test numeric alignment with width less than number of digits
TEST_F(FormatAlignSignTest, NumericAlignmentWidthLessThanDigits) {
    EXPECT_EQ(format_with_spec("{:01}", 42), "42");
    EXPECT_EQ(format_with_spec("{:+01}", 42), "+42");
    EXPECT_EQ(format_with_spec("{:01}", -42), "-42");
}

// Test that ALIGN_NUMERIC with sign correctly handles the sign before padding
TEST_F(FormatAlignSignTest, NumericAlignmentSignBeforePadding) {
    // The sign should appear before the zero padding, not at the right
    EXPECT_EQ(format_with_spec("{:+010}", 123), "+000000123");
    EXPECT_EQ(format_with_spec("{:-010}", 123), "0000000123");  // no sign for positive with '-'
    EXPECT_EQ(format_with_spec("{: 010}", 123), " 000000123");  // space for positive
}

// Test that ALIGN_NUMERIC with negative numbers works correctly
TEST_F(FormatAlignSignTest, NumericAlignmentNegativeNumbers) {
    EXPECT_EQ(format_with_spec("{:010}", -123), "-000000123");
    EXPECT_EQ(format_with_spec("{:+010}", -123), "-000000123");
    EXPECT_EQ(format_with_spec("{: 010}", -123), "-000000123");
}

// Test that ALIGN_DEFAULT with sign increases width (sign counted in width)
TEST_F(FormatAlignSignTest, DefaultAlignmentSignIncreasesWidth) {
    // For default alignment, sign is part of the content and increases width
    EXPECT_EQ(format_with_spec("{:5}", 42), "   42");
    EXPECT_EQ(format_with_spec("{:5}", -42), "  -42");
    EXPECT_EQ(format_with_spec("{:+5}", 42), "  +42");
    EXPECT_EQ(format_with_spec("{: 5}", 42), "   42");  // space sign
}

// Test that ALIGN_DEFAULT with sign and width equal to content
TEST_F(FormatAlignSignTest, DefaultAlignmentSignExactWidth) {
    EXPECT_EQ(format_with_spec("{:3}", 42), " 42");
    EXPECT_EQ(format_with_spec("{:3}", -42), "-42");
    EXPECT_EQ(format_with_spec("{:+3}", 42), "+42");
}

// Test that ALIGN_NUMERIC with sign and width equal to content
TEST_F(FormatAlignSignTest, NumericAlignmentSignExactWidth) {
    EXPECT_EQ(format_with_spec("{:03}", 42), "042");
    EXPECT_EQ(format_with_spec("{:+04}", 42), "+042");
    EXPECT_EQ(format_with_spec("{:04}", -42), "-042");
}

// Test edge case: zero value with various alignments and signs
TEST_F(FormatAlignSignTest, ZeroValueEdgeCases) {
    EXPECT_EQ(format_with_spec("{:5}", 0), "    0");
    EXPECT_EQ(format_with_spec("{:05}", 0), "00000");
    EXPECT_EQ(format_with_spec("{:+05}", 0), "+0000");
    EXPECT_EQ(format_with_spec("{:<5}", 0), "0    ");
    EXPECT_EQ(format_with_spec("{:>5}", 0), "    0");
    EXPECT_EQ(format_with_spec("{:^5}", 0), "  0  ");
}

// Test that ALIGN_NUMERIC with sign correctly handles width reduction
TEST_F(FormatAlignSignTest, NumericAlignmentWidthReduction) {
    // When sign is present, width_ is decremented by 1
    EXPECT_EQ(format_with_spec("{:+06}", 123), "+00123");
    EXPECT_EQ(format_with_spec("{:+06}", -123), "-00123");
    // Without sign, no reduction
    EXPECT_EQ(format_with_spec("{:06}", 123), "000123");
    EXPECT_EQ(format_with_spec("{:06}", -123), "-00123");
}

// Test that ALIGN_DEFAULT with sign increases n (content width)
TEST_F(FormatAlignSignTest, DefaultAlignmentSignIncreasesN) {
    // For default alignment, sign adds to the content width
    EXPECT_EQ(format_with_spec("{:4}", 42), "  42");
    EXPECT_EQ(format_with_spec("{:+4}", 42), " +42");
    EXPECT_EQ(format_with_spec("{:4}", -42), " -42");
}

// Test that ALIGN_NUMERIC with sign and space sign works
TEST_F(FormatAlignSignTest, NumericAlignmentSpaceSign) {
    EXPECT_EQ(format_with_spec("{: 06}", 42), " 00042");
    EXPECT_EQ(format_with_spec("{: 06}", -42), "-00042");
    EXPECT_EQ(format_with_spec("{: 06}", 0), " 00000");
}

// Test that ALIGN_NUMERIC with sign and explicit plus sign works
TEST_F(FormatAlignSignTest, NumericAlignmentExplicitPlus) {
    EXPECT_EQ(format_with_spec("{:+06}", 42), "+00042");
    EXPECT_EQ(format_with_spec("{:+06}", -42), "-00042");
}

// Test that ALIGN_NUMERIC with sign and minus sign (only negative shown) works
TEST_F(FormatAlignSignTest, NumericAlignmentMinusSign) {
    EXPECT_EQ(format_with_spec("{:-06}", 42), "000042");
    EXPECT_EQ(format_with_spec("{:-06}", -42), "-00042");
}

// Test that ALIGN_NUMERIC with sign and width of 1 (minimum)
TEST_F(FormatAlignSignTest, NumericAlignmentMinimumWidth) {
    EXPECT_EQ(format_with_spec("{:01}", 5), "5");
    EXPECT_EQ(format_with_spec("{:+02}", 5), "+5");
    EXPECT_EQ(format_with_spec("{:02}", -5), "-5");
}

// Test that ALIGN_NUMERIC with sign and width of 0 (edge case)
TEST_F(FormatAlignSignTest, NumericAlignmentZeroWidthWithSign) {
    EXPECT_EQ(format_with_spec("{:0}", 5), "5");
    EXPECT_EQ(format_with_spec("{:+0}", 5), "+5");
    EXPECT_EQ(format_with_spec("{:0}", -5), "-5");
}

// Test that ALIGN_DEFAULT with sign and width of 0 (edge case)
TEST_F(FormatAlignSignTest, DefaultAlignmentZeroWidthWithSign) {
    EXPECT_EQ(format_with_spec("{:}", 5), "5");
    EXPECT_EQ(format_with_spec("{:+}", 5), "+5");
    EXPECT_EQ(format_with_spec("{:}", -5), "-5");
}

// Test that ALIGN_NUMERIC with sign correctly handles large numbers
TEST_F(FormatAlignSignTest, NumericAlignmentLargeNumbers) {
    EXPECT_EQ(format_with_spec("{:+015}", 123456789), "+0000123456789");
    EXPECT_EQ(format_with_spec("{:015}", -123456789), "-0000123456789");
}

// Test that ALIGN_DEFAULT with sign correctly handles large numbers
TEST_F(FormatAlignSignTest, DefaultAlignmentLargeNumbers) {
    EXPECT_EQ(format_with_spec("{:15}", 123456789), "     123456789");
    EXPECT_EQ(format_with_spec("{:+15}", 123456789), "     +123456789");
    EXPECT_EQ(format_with_spec("{:15}", -123456789), "     -123456789");
}