#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>
#include <cmath>

// Test the focal function's behavior through public API calls that exercise
// the write_padded with align::right and negative number formatting

class FocalFunctionTest : public ::testing::Test {
protected:
    // Helper to test negative number formatting with various widths and precisions
    void testNegativeNumberFormatting(int value, const std::string& format_str, 
                                      const std::string& expected) {
        EXPECT_EQ(fmt::format(format_str, value), expected);
    }
};

// Test basic negative integer formatting (exercises the focal function path)
TEST_F(FocalFunctionTest, BasicNegativeInteger) {
    testNegativeNumberFormatting(-42, "{}", "-42");
    testNegativeNumberFormatting(-1, "{}", "-1");
    testNegativeNumberFormatting(-1000, "{}", "-1000");
}

// Test negative numbers with width specification (triggers write_padded with align::right)
TEST_F(FocalFunctionTest, NegativeIntegerWithWidth) {
    testNegativeNumberFormatting(-42, "{:10}", "       -42");
    testNegativeNumberFormatting(-42, "{:<10}", "-42       ");
    testNegativeNumberFormatting(-42, "{:>10}", "       -42");
    testNegativeNumberFormatting(-42, "{:^10}", "   -42    ");
}

// Test negative numbers with zero padding (exercises internal padding logic)
TEST_F(FocalFunctionTest, NegativeIntegerWithZeroPadding) {
    testNegativeNumberFormatting(-42, "{:010}", "-000000042");
    testNegativeNumberFormatting(-1, "{:05}", "-0001");
    testNegativeNumberFormatting(-100, "{:08}", "-0000100");
}

// Test negative numbers with precision (floating point path may differ)
TEST_F(FocalFunctionTest, NegativeFloatWithPrecision) {
    EXPECT_EQ(fmt::format("{:.2f}", -3.14159), "-3.14");
    EXPECT_EQ(fmt::format("{:.0f}", -3.9), "-4");
    EXPECT_EQ(fmt::format("{:.10f}", -0.0), "-0.0000000000");
}

// Test edge case: negative zero
TEST_F(FocalFunctionTest, NegativeZero) {
    EXPECT_EQ(fmt::format("{}", -0.0), "-0");
    EXPECT_EQ(fmt::format("{:.1f}", -0.0), "-0.0");
}

// Test edge case: minimum integer value
TEST_F(FocalFunctionTest, MinimumInteger) {
    int min_int = std::numeric_limits<int>::min();
    std::string expected = std::to_string(min_int);
    EXPECT_EQ(fmt::format("{}", min_int), expected);
}

// Test edge case: maximum negative integer
TEST_F(FocalFunctionTest, MaximumNegativeInteger) {
    EXPECT_EQ(fmt::format("{}", -1), "-1");
    EXPECT_EQ(fmt::format("{}", -2147483647), "-2147483647");
}

// Test negative numbers with fill character
TEST_F(FocalFunctionTest, NegativeWithFillCharacter) {
    testNegativeNumberFormatting(-42, "{:*>10}", "*******-42");
    testNegativeNumberFormatting(-42, "{:*<10}", "-42*******");
    testNegativeNumberFormatting(-42, "{:*^10}", "***-42****");
}

// Test negative numbers with sign specification
TEST_F(FocalFunctionTest, NegativeWithSignSpec) {
    EXPECT_EQ(fmt::format("{:+}", 42), "+42");
    EXPECT_EQ(fmt::format("{:+}", -42), "-42");
    EXPECT_EQ(fmt::format("{: }", 42), " 42");
    EXPECT_EQ(fmt::format("{: }", -42), "-42");
    EXPECT_EQ(fmt::format("{:-}", 42), "42");
    EXPECT_EQ(fmt::format("{:-}", -42), "-42");
}

// Test negative numbers with combined width, precision, and fill
TEST_F(FocalFunctionTest, NegativeCombinedFormatting) {
    EXPECT_EQ(fmt::format("{:*>+10.2f}", -3.14), "*****-3.14");
    EXPECT_EQ(fmt::format("{:*>+10.2f}", 3.14), "*****+3.14");
    EXPECT_EQ(fmt::format("{:0=+10}", -42), "-000000042");
    EXPECT_EQ(fmt::format("{:0=+10}", 42), "+000000042");
}

// Test negative numbers with different integer types
TEST_F(FocalFunctionTest, NegativeDifferentTypes) {
    EXPECT_EQ(fmt::format("{}", static_cast<short>(-42)), "-42");
    EXPECT_EQ(fmt::format("{}", static_cast<long>(-42)), "-42");
    EXPECT_EQ(fmt::format("{}", static_cast<long long>(-42)), "-42");
}

// Test negative numbers with hex/octal formatting (these don't typically show sign)
TEST_F(FocalFunctionTest, NegativeHexOctal) {
    EXPECT_EQ(fmt::format("{:x}", -255), "-ff");
    EXPECT_EQ(fmt::format("{:o}", -8), "-10");
    EXPECT_EQ(fmt::format("{:X}", -255), "-FF");
}

// Test edge case: empty format string with negative number
TEST_F(FocalFunctionTest, EmptyFormatString) {
    EXPECT_EQ(fmt::format("{}", -1), "-1");
    EXPECT_EQ(fmt::format("{:}", -1), "-1");
}

// Test edge case: very large width with negative number
TEST_F(FocalFunctionTest, LargeWidth) {
    std::string expected(100, ' ');
    expected += "-42";
    EXPECT_EQ(fmt::format("{:>100}", -42), expected);
}

// Test edge case: width smaller than number
TEST_F(FocalFunctionTest, WidthSmallerThanNumber) {
    EXPECT_EQ(fmt::format("{:2}", -1000), "-1000");
    EXPECT_EQ(fmt::format("{:1}", -42), "-42");
}

// Test negative numbers with format_to (exercises output iterator path)
TEST_F(FocalFunctionTest, FormatToNegative) {
    std::string out;
    auto it = std::back_inserter(out);
    fmt::format_to(it, "{}", -42);
    EXPECT_EQ(out, "-42");
}

// Test negative numbers with format_to_n
TEST_F(FocalFunctionTest, FormatToNNegative) {
    std::string out(10, '\0');
    auto result = fmt::format_to_n(out.data(), 5, "{}", -12345);
    *result.out = '\0';
    EXPECT_EQ(std::string(out.data()), "-1234");
}

// Test formatted_size with negative numbers
TEST_F(FocalFunctionTest, FormattedSizeNegative) {
    EXPECT_EQ(fmt::formatted_size("{}", -42), 3);
    EXPECT_EQ(fmt::formatted_size("{:10}", -42), 10);
}

// Test negative numbers with locale-specific formatting (if available)
TEST_F(FocalFunctionTest, NegativeWithLocale) {
    // Basic test without locale to ensure no crash
    EXPECT_EQ(fmt::format("{:L}", -1234567), "-1,234,567");
}

// Test edge case: negative infinity
TEST_F(FocalFunctionTest, NegativeInfinity) {
    double neg_inf = -std::numeric_limits<double>::infinity();
    EXPECT_EQ(fmt::format("{}", neg_inf), "-inf");
    EXPECT_EQ(fmt::format("{:10}", neg_inf), "      -inf");
}

// Test edge case: negative NaN (should preserve sign)
TEST_F(FocalFunctionTest, NegativeNaN) {
    double neg_nan = -std::numeric_limits<double>::quiet_NaN();
    std::string result = fmt::format("{}", neg_nan);
    EXPECT_TRUE(result == "-nan" || result == "-nan(ind)" || result == "-nan(snan)");
}

// Test negative numbers with custom width and alignment that triggers padding
TEST_F(FocalFunctionTest, NegativePaddingEdgeCases) {
    // Width exactly matching number length
    EXPECT_EQ(fmt::format("{:3}", -42), "-42");
    // Width one more than number length
    EXPECT_EQ(fmt::format("{:4}", -42), " -42");
    // Width with zero padding and sign
    EXPECT_EQ(fmt::format("{:0=5}", -42), "-0042");
    // Width with zero padding and positive number
    EXPECT_EQ(fmt::format("{:0=5}", 42), "00042");
}