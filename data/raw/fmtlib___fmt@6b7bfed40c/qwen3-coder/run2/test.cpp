#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <string>
#include <limits>

// Test the format function with various inputs to exercise the focal function
TEST(FmtFormatTest, FormatDouble) {
    // Test normal positive numbers
    EXPECT_EQ("1.23", fmt::format("{}", 1.23));
    EXPECT_EQ("123.456", fmt::format("{}", 123.456));
    
    // Test normal negative numbers
    EXPECT_EQ("-1.23", fmt::format("{}", -1.23));
    EXPECT_EQ("-123.456", fmt::format("{}", -123.456));
    
    // Test zero
    EXPECT_EQ("0", fmt::format("{}", 0.0));
    EXPECT_EQ("0", fmt::format("{}", -0.0));
    
    // Test very small numbers (scientific notation)
    EXPECT_EQ("1e-10", fmt::format("{}", 1e-10));
    EXPECT_EQ("1e-15", fmt::format("{}", 1e-15));
    
    // Test very large numbers
    EXPECT_EQ("1e+10", fmt::format("{}", 1e10));
    EXPECT_EQ("1e+15", fmt::format("{}", 1e15));
    
    // Test numbers with many decimal places
    EXPECT_EQ("0.123456789", fmt::format("{}", 0.123456789));
    EXPECT_EQ("123456789.123456789", fmt::format("{}", 123456789.123456789));
    
    // Test edge cases around zero
    EXPECT_EQ("0", fmt::format("{}", std::numeric_limits<double>::min()));
    EXPECT_EQ("0", fmt::format("{}", -std::numeric_limits<double>::min()));
    
    // Test infinity and NaN
    EXPECT_EQ("inf", fmt::format("{}", std::numeric_limits<double>::infinity()));
    EXPECT_EQ("-inf", fmt::format("{}", -std::numeric_limits<double>::infinity()));
    EXPECT_EQ("nan", fmt::format("{}", std::numeric_limits<double>::quiet_NaN()));
}

TEST(FmtFormatTest, FormatFloat) {
    // Test float precision
    EXPECT_EQ("1.23", fmt::format("{}", 1.23f));
    EXPECT_EQ("123.456", fmt::format("{}", 123.456f));
    
    // Test float edge cases
    EXPECT_EQ("0", fmt::format("{}", 0.0f));
    EXPECT_EQ("0", fmt::format("{}", -0.0f));
    
    // Test very small floats
    EXPECT_EQ("1e-10", fmt::format("{}", 1e-10f));
    EXPECT_EQ("1e-15", fmt::format("{}", 1e-15f));
}

TEST(FmtFormatTest, FormatWithPrecision) {
    // Test with explicit precision
    EXPECT_EQ("1.23", fmt::format("{:.2f}", 1.23456));
    EXPECT_EQ("1.235", fmt::format("{:.3f}", 1.23456));
    
    // Test with zero precision
    EXPECT_EQ("1", fmt::format("{:.0f}", 1.7));
    EXPECT_EQ("2", fmt::format("{:.0f}", 1.5));
    
    // Test with large precision
    EXPECT_EQ("1.234560000000000", fmt::format("{:.15f}", 1.23456));
}

TEST(FmtFormatTest, FormatScientific) {
    // Test scientific notation
    EXPECT_EQ("1.23e+00", fmt::format("{:e}", 1.23));
    EXPECT_EQ("1.234560e+02", fmt::format("{:.6e}", 123.456));
    
    // Test scientific notation with zero
    EXPECT_EQ("0.000000e+00", fmt::format("{:.6e}", 0.0));
    
    // Test scientific notation with negative numbers
    EXPECT_EQ("-1.23e+00", fmt::format("{:e}", -1.23));
}

TEST(FmtFormatTest, FormatFixed) {
    // Test fixed notation
    EXPECT_EQ("1.23", fmt::format("{:f}", 1.23));
    EXPECT_EQ("123.456000", fmt::format("{:f}", 123.456));
    
    // Test fixed notation with zero
    EXPECT_EQ("0.000000", fmt::format("{:f}", 0.0));
    
    // Test fixed notation with negative numbers
    EXPECT_EQ("-1.23", fmt::format("{:f}", -1.23));
}

TEST(FmtFormatTest, FormatGeneral) {
    // Test general notation
    EXPECT_EQ("1.23", fmt::format("{:g}", 1.23));
    EXPECT_EQ("123.456", fmt::format("{:g}", 123.456));
    
    // Test general notation with zero
    EXPECT_EQ("0", fmt::format("{:g}", 0.0));
    
    // Test general notation with scientific notation
    EXPECT_EQ("1e-10", fmt::format("{:g}", 1e-10));
    EXPECT_EQ("1e+10", fmt::format("{:g}", 1e10));
}