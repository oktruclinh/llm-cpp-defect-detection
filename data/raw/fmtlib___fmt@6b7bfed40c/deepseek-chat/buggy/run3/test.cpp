#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/format-inl.h>
#include <string>
#include <cmath>
#include <limits>

// Test formatting with various precision and exponent combinations
TEST(FormatInlTest, FormatFloatPrecision) {
    // Test basic float formatting with precision
    EXPECT_EQ(fmt::format("{:.2f}", 3.14159), "3.14");
    EXPECT_EQ(fmt::format("{:.0f}", 3.14159), "3");
    EXPECT_EQ(fmt::format("{:.10f}", 1.0), "1.0000000000");
    
    // Test with large exponent (positive)
    EXPECT_EQ(fmt::format("{:.2e}", 123456.0), "1.23e+05");
    EXPECT_EQ(fmt::format("{:.2e}", 0.00123456), "1.23e-03");
    
    // Test with very small numbers (negative exponent)
    EXPECT_EQ(fmt::format("{:.5f}", 0.00001), "0.00001");
    EXPECT_EQ(fmt::format("{:.10f}", 1e-10), "0.0000000001");
}

// Test formatting with zero and near-zero values
TEST(FormatInlTest, FormatZeroAndNearZero) {
    // Zero
    EXPECT_EQ(fmt::format("{:.2f}", 0.0), "0.00");
    EXPECT_EQ(fmt::format("{:.0f}", 0.0), "0");
    
    // Very small positive number (should not underflow)
    EXPECT_EQ(fmt::format("{:.5f}", 1e-15), "0.00000");
    
    // Negative zero
    EXPECT_EQ(fmt::format("{:.1f}", -0.0), "0.0");
}

// Test formatting with large numbers (large positive exponent)
TEST(FormatInlTest, FormatLargeNumbers) {
    // Large integer-like float
    EXPECT_EQ(fmt::format("{:.0f}", 1e15), "1000000000000000");
    EXPECT_EQ(fmt::format("{:.0f}", 1e20), "100000000000000000000");
    
    // Large number with decimal
    EXPECT_EQ(fmt::format("{:.1f}", 1.5e10), "15000000000.0");
}

// Test formatting with negative exponents (small numbers)
TEST(FormatInlTest, FormatSmallNumbers) {
    // Very small positive numbers
    EXPECT_EQ(fmt::format("{:.5e}", 1.23e-10), "1.23000e-10");
    EXPECT_EQ(fmt::format("{:.3e}", 9.876e-15), "9.876e-15");
    
    // Small number that requires exponent notation
    EXPECT_EQ(fmt::format("{:.2e}", 0.0000123456), "1.23e-05");
}

// Test formatting with different precision values (boundary cases)
TEST(FormatInlTest, FormatPrecisionBoundaries) {
    // Precision = 0
    EXPECT_EQ(fmt::format("{:.0f}", 1.5), "2");  // rounding
    EXPECT_EQ(fmt::format("{:.0f}", 1.4), "1");
    
    // Very high precision
    EXPECT_EQ(fmt::format("{:.50f}", 0.5), "0.50000000000000000000000000000000000000000000000000");
    
    // Precision that triggers exponent boundary
    EXPECT_EQ(fmt::format("{:.6g}", 1234567.0), "1.23457e+06");
    EXPECT_EQ(fmt::format("{:.6g}", 0.000001234567), "1.23457e-06");
}

// Test formatting with special float values
TEST(FormatInlTest, FormatSpecialValues) {
    // Infinity
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<double>::infinity()), "inf");
    EXPECT_EQ(fmt::format("{}", -std::numeric_limits<double>::infinity()), "-inf");
    
    // NaN
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<double>::quiet_NaN()), "nan");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<double>::signaling_NaN()), "nan");
    
    // Denormalized numbers
    double denorm = std::numeric_limits<double>::denorm_min();
    EXPECT_FALSE(fmt::format("{}", denorm).empty());
}

// Test formatting with different format specifiers that affect exponent handling
TEST(FormatInlTest, FormatExponentSpecifiers) {
    // 'e' specifier forces exponent notation
    EXPECT_EQ(fmt::format("{:.2e}", 100.0), "1.00e+02");
    EXPECT_EQ(fmt::format("{:.2e}", 0.01), "1.00e-02");
    
    // 'g' specifier uses shortest representation
    EXPECT_EQ(fmt::format("{:.4g}", 12345.0), "1.234e+04");
    EXPECT_EQ(fmt::format("{:.4g}", 0.00012345), "1.234e-04");
    
    // 'f' specifier fixed notation
    EXPECT_EQ(fmt::format("{:.2f}", 100.0), "100.00");
    EXPECT_EQ(fmt::format("{:.2f}", 0.01), "0.01");
}

// Test formatting with numbers that exercise the denominator shift logic
TEST(FormatInlTest, FormatDenominatorShift) {
    // Numbers that require shifting denominator (exp10 < 0 case)
    EXPECT_EQ(fmt::format("{:.10f}", 0.5), "0.5000000000");
    EXPECT_EQ(fmt::format("{:.10f}", 0.25), "0.2500000000");
    
    // Numbers with negative exponent that trigger numerator.assign_pow10
    EXPECT_EQ(fmt::format("{:.5e}", 0.0000123456789), "1.23457e-05");
    EXPECT_EQ(fmt::format("{:.5e}", 0.000000001), "1.00000e-09");
}

// Test formatting with numbers that exercise the numerator/lower assignment
TEST(FormatInlTest, FormatNumeratorLowerAssignment) {
    // Numbers where exp10 < 0 triggers numerator.assign_pow10 and lower.assign(numerator)
    EXPECT_EQ(fmt::format("{:.15f}", 0.001), "0.001000000000000");
    EXPECT_EQ(fmt::format("{:.15f}", 0.0001), "0.000100000000000");
    
    // Very small numbers that require precise handling
    EXPECT_EQ(fmt::format("{:.20f}", 1e-5), "0.00001000000000000000");
    EXPECT_EQ(fmt::format("{:.20f}", 1e-8), "0.00000001000000000000");
}

// Test formatting with numbers that have exact binary representations
TEST(FormatInlTest, FormatExactBinary) {
    // Powers of 2 (exact in binary)
    EXPECT_EQ(fmt::format("{:.1f}", 0.5), "0.5");
    EXPECT_EQ(fmt::format("{:.1f}", 0.25), "0.3");  // 0.25 rounds to 0.3 with 1 decimal
    EXPECT_EQ(fmt::format("{:.2f}", 0.125), "0.12");  // 0.125 rounds to 0.12 with 2 decimals
    
    // Numbers that are sums of powers of 2
    EXPECT_EQ(fmt::format("{:.2f}", 0.75), "0.75");
    EXPECT_EQ(fmt::format("{:.2f}", 0.375), "0.38");  // 0.375 rounds to 0.38
}

// Test formatting with numbers that cause rounding at exponent boundaries
TEST(FormatInlTest, FormatRoundingAtBoundaries) {
    // Rounding up at exponent boundary
    EXPECT_EQ(fmt::format("{:.0e}", 9.5), "1e+01");
    EXPECT_EQ(fmt::format("{:.0e}", 9.4), "9e+00");
    
    // Rounding down at exponent boundary
    EXPECT_EQ(fmt::format("{:.0e}", 0.095), "1e-01");
    EXPECT_EQ(fmt::format("{:.0e}", 0.094), "9e-02");
}

// Test formatting with maximum and minimum double values
TEST(FormatInlTest, FormatExtremeValues) {
    // Maximum finite double
    double max_double = std::numeric_limits<double>::max();
    std::string max_str = fmt::format("{:.5e}", max_double);
    EXPECT_FALSE(max_str.empty());
    EXPECT_NE(max_str, "inf");
    
    // Minimum positive normalized double
    double min_normal = std::numeric_limits<double>::min();
    std::string min_str = fmt::format("{:.5e}", min_normal);
    EXPECT_FALSE(min_str.empty());
    EXPECT_NE(min_str, "0.00000e+00");
}