#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <string>
#include <limits>

TEST(FmtFormatTest, FormatDoublePrecision) {
    // Test normal cases with various precisions
    EXPECT_EQ("1.2345678901234567", fmt::format("{:.16f}", 1.2345678901234567));
    EXPECT_EQ("0.0001234567890123", fmt::format("{:.16f}", 0.0001234567890123));
    EXPECT_EQ("1234567890.1234567", fmt::format("{:.7f}", 1234567890.1234567));
    
    // Test zero cases
    EXPECT_EQ("0.0000000000000000", fmt::format("{:.16f}", 0.0));
    EXPECT_EQ("0.0", fmt::format("{:.1f}", 0.0));
    
    // Test negative numbers
    EXPECT_EQ("-1.2345678901234567", fmt::format("{:.16f}", -1.2345678901234567));
    EXPECT_EQ("-0.0001234567890123", fmt::format("{:.16f}", -0.0001234567890123));
    
    // Test boundary cases around powers of 10
    EXPECT_EQ("1.0000000000000000", fmt::format("{:.16f}", 1.0));
    EXPECT_EQ("10.0000000000000000", fmt::format("{:.16f}", 10.0));
    EXPECT_EQ("100.0000000000000000", fmt::format("{:.16f}", 100.0));
    EXPECT_EQ("0.1000000000000000", fmt::format("{:.16f}", 0.1));
    EXPECT_EQ("0.0100000000000000", fmt::format("{:.16f}", 0.01));
    EXPECT_EQ("0.0010000000000000", fmt::format("{:.16f}", 0.001));
    
    // Test very small numbers
    EXPECT_EQ("0.0000000000000001", fmt::format("{:.16f}", 1e-16));
    EXPECT_EQ("1e-16", fmt::format("{:.1f}", 1e-16));
    
    // Test very large numbers
    EXPECT_EQ("10000000000000000.0000000000000000", fmt::format("{:.16f}", 1e16));
    EXPECT_EQ("100000000000000000.0000000000000000", fmt::format("{:.16f}", 1e17));
    
    // Test scientific notation
    EXPECT_EQ("1.2345678901234567e+00", fmt::format("{:.16e}", 1.2345678901234567));
    EXPECT_EQ("1.2345678901234567e-16", fmt::format("{:.16e}", 1e-16));
    EXPECT_EQ("1.2345678901234567e+16", fmt::format("{:.16e}", 1e16));
    
    // Test edge case with maximum double precision
    double max_double = std::numeric_limits<double>::max();
    std::string formatted_max = fmt::format("{:.16f}", max_double);
    EXPECT_FALSE(formatted_max.empty());
    
    // Test edge case with minimum double precision
    double min_double = std::numeric_limits<double>::min();
    std::string formatted_min = fmt::format("{:.16f}", min_double);
    EXPECT_FALSE(formatted_min.empty());
    
    // Test special values
    EXPECT_EQ("inf", fmt::format("{:f}", std::numeric_limits<double>::infinity()));
    EXPECT_EQ("-inf", fmt::format("{:f}", -std::numeric_limits<double>::infinity()));
    EXPECT_EQ("nan", fmt::format("{:f}", std::numeric_limits<double>::quiet_NaN()));
}

TEST(FmtFormatTest, FormatDoubleEdgeCases) {
    // Test exactly representable values
    EXPECT_EQ("0.1", fmt::format("{:.1f}", 0.1));
    EXPECT_EQ("0.2", fmt::format("{:.1f}", 0.2));
    EXPECT_EQ("0.3", fmt::format("{:.1f}", 0.3));
    
    // Test values that might trigger precision issues
    EXPECT_EQ("0.1000000000000000", fmt::format("{:.16f}", 0.1));
    EXPECT_EQ("0.2000000000000000", fmt::format("{:.16f}", 0.2));
    
    // Test rounding behavior
    EXPECT_EQ("0.1234567890123457", fmt::format("{:.16f}", 0.1234567890123456789));
    EXPECT_EQ("0.1234567890123457", fmt::format("{:.16f}", 0.12345678901234565));
    
    // Test precision limits
    EXPECT_EQ("0.0000000000000000", fmt::format("{:.16f}", 0.00000000000000001));
    EXPECT_EQ("0.0000000000000001", fmt::format("{:.16f}", 0.0000000000000001));
    
    // Test zero with various precisions
    EXPECT_EQ("0.0000000000000000", fmt::format("{:.16f}", 0.0));
    EXPECT_EQ("0.0000000000000000", fmt::format("{:.16f}", -0.0));
    EXPECT_EQ("0.0000000000000000", fmt::format("{:.16f}", 0.0));
    
    // Test very small positive values
    EXPECT_EQ("0.0000000000000000", fmt::format("{:.16f}", 1e-17));
    EXPECT_EQ("0.0000000000000000", fmt::format("{:.16f}", 1e-18));
    
    // Test very large positive values
    EXPECT_EQ("10000000000000000.0000000000000000", fmt::format("{:.16f}", 1e16));
    EXPECT_EQ("100000000000000000.0000000000000000", fmt::format("{:.16f}", 1e17));
}

TEST(FmtFormatTest, FormatDoubleSpecialValues) {
    // Test infinity and NaN
    double inf = std::numeric_limits<double>::infinity();
    double ninf = -std::numeric_limits<double>::infinity();
    double nan = std::numeric_limits<double>::quiet_NaN();
    
    EXPECT_EQ("inf", fmt::format("{:f}", inf));
    EXPECT_EQ("-inf", fmt::format("{:f}", ninf));
    EXPECT_EQ("nan", fmt::format("{:f}", nan));
    
    // Test with different precisions
    EXPECT_EQ("inf", fmt::format("{:.1f}", inf));
    EXPECT_EQ("-inf", fmt::format("{:.1f}", ninf));
    EXPECT_EQ("nan", fmt::format("{:.1f}", nan));
    
    // Test with scientific notation
    EXPECT_EQ("inf", fmt::format("{:e}", inf));
    EXPECT_EQ("-inf", fmt::format("{:e}", ninf));
    EXPECT_EQ("nan", fmt::format("{:e}", nan));
}