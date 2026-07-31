#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/format-inl.h>
#include <string>
#include <cmath>
#include <cstdint>

// Test that format correctly handles various floating-point formatting scenarios
// that exercise the focal function's logic for exponent adjustment and power-of-10
// assignment in the big integer formatting path.

class FormatFocalTest : public ::testing::Test {
protected:
    // Helper to format a double with specific precision and format spec
    std::string format_double(double value, const std::string& format_spec) {
        return fmt::format(fmt::format("{{:{}}}", format_spec), value);
    }
};

// Test normal positive exponent case (exp10 > 0)
TEST_F(FormatFocalTest, PositiveExponent) {
    // 1e10 should trigger exp10 > 0 path with denominator power-of-10
    std::string result = format_double(1e10, ".0f");
    EXPECT_EQ(result, "10000000000");
}

// Test negative exponent case (exp10 < 0)
TEST_F(FormatFocalTest, NegativeExponent) {
    // 1e-10 should trigger exp10 < 0 path with numerator power-of-10
    std::string result = format_double(1e-10, ".10f");
    EXPECT_EQ(result, "0.0000000001");
}

// Test zero exponent case (exp10 == 0)
TEST_F(FormatFocalTest, ZeroExponent) {
    // 1.0 should have exp10 == 0
    std::string result = format_double(1.0, ".0f");
    EXPECT_EQ(result, "1");
}

// Test very large positive exponent
TEST_F(FormatFocalTest, LargePositiveExponent) {
    // 1e20 should exercise large positive exponent path
    std::string result = format_double(1e20, ".0f");
    EXPECT_EQ(result, "100000000000000000000");
}

// Test very large negative exponent
TEST_F(FormatFocalTest, LargeNegativeExponent) {
    // 1e-20 should exercise large negative exponent path
    std::string result = format_double(1e-20, ".20f");
    // Should have 20 zeros after decimal point then 1
    EXPECT_EQ(result, "0.00000000000000000001");
}

// Test exponent boundary near 0
TEST_F(FormatFocalTest, ExponentBoundaryNearZero) {
    // Test values that might have exp10 = -1, 0, or 1
    std::string result1 = format_double(0.1, ".1f");
    EXPECT_EQ(result1, "0.1");
    
    std::string result2 = format_double(0.5, ".1f");
    EXPECT_EQ(result2, "0.5");
    
    std::string result3 = format_double(1.5, ".0f");
    EXPECT_EQ(result3, "2");  // Rounding
}

// Test with precision that forces specific exponent paths
TEST_F(FormatFocalTest, PrecisionForcesExponentPath) {
    // High precision formatting of small number
    std::string result = format_double(1.23456789e-5, ".10f");
    EXPECT_EQ(result, "0.0000123457");
    
    // High precision formatting of large number
    result = format_double(1.23456789e5, ".1f");
    EXPECT_EQ(result, "123456.8");
}

// Test with format spec that uses 'e' type (scientific notation)
TEST_F(FormatFocalTest, ScientificNotation) {
    // Scientific notation should still exercise the big integer path for some values
    std::string result = format_double(1e10, ".0e");
    EXPECT_EQ(result, "1e+10");
    
    result = format_double(1e-10, ".0e");
    EXPECT_EQ(result, "1e-10");
}

// Test with format spec that uses 'g' type (general format)
TEST_F(FormatFocalTest, GeneralFormat) {
    // General format may switch between fixed and scientific
    std::string result = format_double(1e10, ".0g");
    EXPECT_EQ(result, "1e+10");
    
    result = format_double(1e-5, ".0g");
    EXPECT_EQ(result, "1e-05");
}

// Test with format spec that uses 'a' type (hex float)
TEST_F(FormatFocalTest, HexFloatFormat) {
    // Hex float format should not use the big integer path
    std::string result = format_double(1.0, ".0a");
    EXPECT_EQ(result, "0x1p+0");
    
    result = format_double(1e10, ".0a");
    EXPECT_EQ(result, "0x1.2a05f2p+33");
}

// Test edge case: denormalized numbers
TEST_F(FormatFocalTest, DenormalizedNumbers) {
    // Smallest positive double (denormalized)
    double smallest = std::numeric_limits<double>::denorm_min();
    std::string result = format_double(smallest, ".0e");
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "nan");
    
    // Very small number that might trigger edge cases in exponent handling
    result = format_double(5e-324, ".0e");
    EXPECT_FALSE(result.empty());
}

// Test edge case: infinity
TEST_F(FormatFocalTest, Infinity) {
    std::string result = format_double(std::numeric_limits<double>::infinity(), ".0f");
    EXPECT_EQ(result, "inf");
    
    result = format_double(-std::numeric_limits<double>::infinity(), ".0f");
    EXPECT_EQ(result, "-inf");
}

// Test edge case: NaN
TEST_F(FormatFocalTest, NaN) {
    std::string result = format_double(std::numeric_limits<double>::quiet_NaN(), ".0f");
    EXPECT_EQ(result, "nan");
    
    result = format_double(-std::numeric_limits<double>::quiet_NaN(), ".0f");
    EXPECT_EQ(result, "-nan");
}

// Test edge case: zero
TEST_F(FormatFocalTest, Zero) {
    std::string result = format_double(0.0, ".0f");
    EXPECT_EQ(result, "0");
    
    result = format_double(-0.0, ".0f");
    EXPECT_EQ(result, "-0");
    
    result = format_double(0.0, ".10f");
    EXPECT_EQ(result, "0.0000000000");
}

// Test edge case: very large precision
TEST_F(FormatFocalTest, LargePrecision) {
    // Large precision should still work correctly
    std::string result = format_double(1.0, ".50f");
    EXPECT_EQ(result.substr(0, 2), "1.");
    EXPECT_EQ(result.length(), 52);  // "1." + 50 digits
    
    // Small number with large precision
    result = format_double(1e-10, ".50f");
    EXPECT_EQ(result.substr(0, 12), "0.0000000001");
}

// Test edge case: values that cause exponent to be exactly at boundary
TEST_F(FormatFocalTest, ExponentBoundaryValues) {
    // Values where exp10 might be exactly 0, 1, or -1
    std::string result = format_double(9.999999999999, ".0f");
    EXPECT_EQ(result, "10");  // Rounding up
    
    result = format_double(0.999999999999, ".0f");
    EXPECT_EQ(result, "1");  // Rounding up
    
    result = format_double(1.000000000001, ".0f");
    EXPECT_EQ(result, "1");
}

// Test that format correctly handles values that require shift operations
TEST_F(FormatFocalTest, ShiftOperations) {
    // Values that might trigger the denominator <<= shift path
    std::string result = format_double(1e15, ".0f");
    EXPECT_EQ(result, "1000000000000000");
    
    result = format_double(1e-15, ".15f");
    EXPECT_EQ(result, "0.000000000000001");
}

// Test with format spec that includes fill and alignment
TEST_F(FormatFocalTest, FillAndAlignment) {
    std::string result = format_double(1e10, ">20.0f");
    EXPECT_EQ(result, "       10000000000");
    
    result = format_double(1e-10, "<20.10f");
    EXPECT_EQ(result, "0.0000000001       ");
}

// Test with format spec that includes sign
TEST_F(FormatFocalTest, SignFormatting) {
    std::string result = format_double(1e10, "+.0f");
    EXPECT_EQ(result, "+10000000000");
    
    result = format_double(-1e10, "+.0f");
    EXPECT_EQ(result, "-10000000000");
    
    result = format_double(1e-10, "+.10f");
    EXPECT_EQ(result, "+0.0000000001");
}

// Test with format spec that includes thousands separator
TEST_F(FormatFocalTest, ThousandsSeparator) {
    std::string result = format_double(1e10, "L.0f");
    // The exact format depends on locale, but should not be empty
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "nan");
}

// Test that the focal function's logic works for various decimal exponents
TEST_F(FormatFocalTest, VariousDecimalExponents) {
    // Test a range of decimal exponents
    for (int exp = -5; exp <= 5; ++exp) {
        double value = std::pow(10.0, exp);
        std::string result = format_double(value, ".0f");
        EXPECT_FALSE(result.empty());
        // The result should represent the correct order of magnitude
        if (exp >= 0) {
            EXPECT_EQ(result.length(), static_cast<size_t>(exp + 1));
        }
    }
}

// Test that the focal function handles rounding correctly
TEST_F(FormatFocalTest, RoundingBehavior) {
    // Test rounding at various positions
    std::string result = format_double(1.5, ".0f");
    EXPECT_EQ(result, "2");
    
    result = format_double(2.5, ".0f");
    EXPECT_EQ(result, "2");  // Banker's rounding (round to even)
    
    result = format_double(1.999999999999, ".0f");
    EXPECT_EQ(result, "2");
    
    result = format_double(1.499999999999, ".0f");
    EXPECT_EQ(result, "1");
}