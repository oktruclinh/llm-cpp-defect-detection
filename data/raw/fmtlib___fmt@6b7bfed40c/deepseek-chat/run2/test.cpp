#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/format-inl.h>
#include <string>
#include <cmath>
#include <limits>

// Test formatting with various decimal exponents to exercise the
// numerator/denominator assignment logic in format-inl.h
class DecimalExponentTest : public ::testing::Test {
protected:
    // Helper to format a double and verify it produces expected output
    void testFormat(double value, const std::string& expected) {
        std::string result = fmt::format("{}", value);
        EXPECT_EQ(expected, result);
    }
};

// Normal cases: positive exponents
TEST_F(DecimalExponentTest, PositiveExponentSmall) {
    testFormat(1.0e5, "100000");
}

TEST_F(DecimalExponentTest, PositiveExponentMedium) {
    testFormat(1.234e10, "12340000000");
}

TEST_F(DecimalExponentTest, PositiveExponentLarge) {
    testFormat(1.0e20, "100000000000000000000");
}

// Normal cases: negative exponents
TEST_F(DecimalExponentTest, NegativeExponentSmall) {
    testFormat(1.0e-5, "0.00001");
}

TEST_F(DecimalExponentTest, NegativeExponentMedium) {
    testFormat(1.234e-10, "0.0000000001234");
}

TEST_F(DecimalExponentTest, NegativeExponentLarge) {
    testFormat(1.0e-20, "0.00000000000000000001");
}

// Edge cases: zero exponent
TEST_F(DecimalExponentTest, ZeroExponent) {
    testFormat(1.0, "1");
    testFormat(0.0, "0");
}

// Edge cases: very small positive numbers (denormalized)
TEST_F(DecimalExponentTest, DenormalizedPositive) {
    double denorm_min = std::numeric_limits<double>::denorm_min();
    std::string result = fmt::format("{}", denorm_min);
    EXPECT_FALSE(result.empty());
    // Should start with "0." or "5e-" (depending on implementation)
    EXPECT_TRUE(result[0] == '0' || result[0] == '5');
}

// Edge cases: very large numbers
TEST_F(DecimalExponentTest, VeryLargePositive) {
    double large = 1.0e100;
    std::string result = fmt::format("{}", large);
    EXPECT_TRUE(result.find('e') != std::string::npos || 
                result.find('E') != std::string::npos);
}

// Edge cases: very small negative numbers
TEST_F(DecimalExponentTest, VerySmallNegative) {
    double tiny = -1.0e-100;
    std::string result = fmt::format("{}", tiny);
    EXPECT_EQ(result[0], '-');
    EXPECT_TRUE(result.find('e') != std::string::npos || 
                result.find('E') != std::string::npos);
}

// Edge cases: infinity and NaN
TEST_F(DecimalExponentTest, Infinity) {
    double inf = std::numeric_limits<double>::infinity();
    EXPECT_EQ(fmt::format("{}", inf), "inf");
    EXPECT_EQ(fmt::format("{}", -inf), "-inf");
}

TEST_F(DecimalExponentTest, NaN) {
    double nan = std::numeric_limits<double>::quiet_NaN();
    std::string result = fmt::format("{}", nan);
    EXPECT_TRUE(result == "nan" || result == "-nan" || result == "NaN" || result == "-NaN");
}

// Edge cases: max and min finite values
TEST_F(DecimalExponentTest, MaxDouble) {
    double max_val = std::numeric_limits<double>::max();
    std::string result = fmt::format("{}", max_val);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "-inf");
}

TEST_F(DecimalExponentTest, MinDouble) {
    double min_val = std::numeric_limits<double>::min();
    std::string result = fmt::format("{}", min_val);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0");
}

// Test with specific precision to exercise different code paths
TEST_F(DecimalExponentTest, PrecisionPositiveExponent) {
    std::string result = fmt::format("{:.10}", 1.23456789e5);
    EXPECT_EQ(result, "123456.7890000000");
}

TEST_F(DecimalExponentTest, PrecisionNegativeExponent) {
    std::string result = fmt::format("{:.15}", 1.23456789e-5);
    EXPECT_EQ(result, "0.000012345678900");
}

// Test with scientific notation to exercise exponent handling
TEST_F(DecimalExponentTest, ScientificNotationPositive) {
    std::string result = fmt::format("{:.5e}", 12345.6789);
    EXPECT_EQ(result, "1.23457e+04");
}

TEST_F(DecimalExponentTest, ScientificNotationNegative) {
    std::string result = fmt::format("{:.5e}", 0.000123456789);
    EXPECT_EQ(result, "1.23457e-04");
}

// Test with fixed notation to exercise denominator assignment
TEST_F(DecimalExponentTest, FixedNotationPositive) {
    std::string result = fmt::format("{:.10f}", 1.23456789e10);
    EXPECT_EQ(result, "12345678900.0000000000");
}

TEST_F(DecimalExponentTest, FixedNotationNegative) {
    std::string result = fmt::format("{:.10f}", 1.23456789e-10);
    EXPECT_EQ(result, "0.0000000001");
}

// Test with general format to exercise both paths
TEST_F(DecimalExponentTest, GeneralFormatPositive) {
    std::string result = fmt::format("{:.10g}", 1.23456789e10);
    EXPECT_EQ(result, "12345678900");
}

TEST_F(DecimalExponentTest, GeneralFormatNegative) {
    std::string result = fmt::format("{:.10g}", 1.23456789e-10);
    EXPECT_EQ(result, "1.23456789e-10");
}

// Test with zero value and various formats
TEST_F(DecimalExponentTest, ZeroWithPrecision) {
    EXPECT_EQ(fmt::format("{:.5f}", 0.0), "0.00000");
    EXPECT_EQ(fmt::format("{:.5e}", 0.0), "0.00000e+00");
    EXPECT_EQ(fmt::format("{:.5g}", 0.0), "0");
}

// Test with values that cause exponent to cross boundaries
TEST_F(DecimalExponentTest, ExponentBoundaryPositive) {
    // Values near 10^0 boundary
    testFormat(9.999, "9.999");
    testFormat(10.0, "10");
    testFormat(999.999, "999.999");
    testFormat(1000.0, "1000");
}

TEST_F(DecimalExponentTest, ExponentBoundaryNegative) {
    // Values near 10^-0 boundary
    testFormat(0.9999, "0.9999");
    testFormat(0.1, "0.1");
    testFormat(0.01, "0.01");
    testFormat(0.001, "0.001");
}

// Test with values that exercise the shift operation in denominator assignment
TEST_F(DecimalExponentTest, ShiftBoundary) {
    // Values that require significant shifting
    testFormat(1.0e-15, "0.000000000000001");
    testFormat(1.0e-16, "0.0000000000000001");
    testFormat(1.0e-17, "0.00000000000000001");
}

// Test with values that exercise the numerator assignment path
TEST_F(DecimalExponentTest, NumeratorAssignment) {
    // Values with negative exponents that trigger numerator.assign_pow10
    testFormat(1.0e-5, "0.00001");
    testFormat(2.5e-5, "0.000025");
    testFormat(3.14159e-5, "0.0000314159");
}

// Test with values that exercise the denominator assignment path
TEST_F(DecimalExponentTest, DenominatorAssignment) {
    // Values with positive exponents that trigger denominator.assign_pow10
    testFormat(1.0e5, "100000");
    testFormat(2.5e5, "250000");
    testFormat(3.14159e5, "314159");
}

// Test with values that exercise both numerator and denominator paths
TEST_F(DecimalExponentTest, BothPaths) {
    // Values with both positive and negative exponents
    testFormat(1.0e-3, "0.001");
    testFormat(1.0e3, "1000");
    testFormat(1.0e-6, "0.000001");
    testFormat(1.0e6, "1000000");
}