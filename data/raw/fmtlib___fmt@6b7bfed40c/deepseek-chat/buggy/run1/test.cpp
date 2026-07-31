#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/format-inl.h>
#include <string>
#include <cmath>
#include <limits>

// Test the behavior of the focal function through public API calls that trigger it.
// The focal function is part of the internal formatting logic for floating-point numbers.
// We test various floating-point values that exercise the different branches.

class FocalFunctionTest : public ::testing::Test {
protected:
    // Helper to format a double and return the string
    std::string format_double(double value) {
        return fmt::format("{}", value);
    }
    
    // Helper to format with specific precision
    std::string format_double_precision(double value, int precision) {
        return fmt::format("{:.{}f}", value, precision);
    }
};

// Test normal positive values that trigger the exp10 >= 0 branch
TEST_F(FocalFunctionTest, PositiveValuesExp10NonNegative) {
    // These values have positive exponent when normalized
    EXPECT_EQ(format_double(1.0), "1");
    EXPECT_EQ(format_double(10.0), "10");
    EXPECT_EQ(format_double(100.0), "100");
    EXPECT_EQ(format_double(1e10), "10000000000");
    EXPECT_EQ(format_double(1e15), "1000000000000000");
}

// Test values that trigger the exp10 < 0 branch (fractional values)
TEST_F(FocalFunctionTest, FractionalValuesExp10Negative) {
    EXPECT_EQ(format_double(0.1), "0.1");
    EXPECT_EQ(format_double(0.01), "0.01");
    EXPECT_EQ(format_double(0.001), "0.001");
    EXPECT_EQ(format_double(0.0001), "0.0001");
    EXPECT_EQ(format_double(1e-5), "0.00001");
    EXPECT_EQ(format_double(1e-10), "0.0000000001");
}

// Test very small values that exercise the lower.assign(numerator) path
TEST_F(FocalFunctionTest, VerySmallValues) {
    EXPECT_EQ(format_double(1e-20), "0.00000000000000000001");
    EXPECT_EQ(format_double(1e-50), "0.00000000000000000000000000000000000000000000000001");
}

// Test boundary values around powers of 10
TEST_F(FocalFunctionTest, BoundaryValues) {
    // Just below and above powers of 10
    EXPECT_EQ(format_double(9.999), "9.999");
    EXPECT_EQ(format_double(10.001), "10.001");
    EXPECT_EQ(format_double(99.999), "99.999");
    EXPECT_EQ(format_double(100.001), "100.001");
    
    // Values that might cause rounding issues
    EXPECT_EQ(format_double(0.9999), "0.9999");
    EXPECT_EQ(format_double(1.0001), "1.0001");
}

// Test zero and near-zero values
TEST_F(FocalFunctionTest, ZeroAndNearZero) {
    EXPECT_EQ(format_double(0.0), "0");
    EXPECT_EQ(format_double(-0.0), "-0");
    EXPECT_EQ(format_double(1e-100), "0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");
}

// Test negative values (should still exercise the same branches)
TEST_F(FocalFunctionTest, NegativeValues) {
    EXPECT_EQ(format_double(-1.0), "-1");
    EXPECT_EQ(format_double(-10.0), "-10");
    EXPECT_EQ(format_double(-0.1), "-0.1");
    EXPECT_EQ(format_double(-0.001), "-0.001");
    EXPECT_EQ(format_double(-1e10), "-10000000000");
    EXPECT_EQ(format_double(-1e-10), "-0.0000000001");
}

// Test values that require specific precision formatting
TEST_F(FocalFunctionTest, PrecisionFormatting) {
    // These exercise the focal function through the precision path
    EXPECT_EQ(format_double_precision(1.23456789, 2), "1.23");
    EXPECT_EQ(format_double_precision(1.23456789, 5), "1.23457");
    EXPECT_EQ(format_double_precision(0.00123456, 6), "0.001235");
    EXPECT_EQ(format_double_precision(0.0000123456, 8), "0.00001235");
}

// Test values that might cause overflow in exponent calculations
TEST_F(FocalFunctionTest, ExtremeValues) {
    // Very large values
    EXPECT_EQ(format_double(1e20), "100000000000000000000");
    EXPECT_EQ(format_double(1e30), "1000000000000000000000000000000");
    
    // Very small values that are still representable
    EXPECT_EQ(format_double(1e-200), "0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");
}

// Test values that exercise the denominator <<= 1 path specifically
TEST_F(FocalFunctionTest, DenominatorShiftPath) {
    // Values that have exp10 >= 0 and require denominator shift
    EXPECT_EQ(format_double(2.0), "2");
    EXPECT_EQ(format_double(5.0), "5");
    EXPECT_EQ(format_double(20.0), "20");
    EXPECT_EQ(format_double(50.0), "50");
    EXPECT_EQ(format_double(200.0), "200");
    EXPECT_EQ(format_double(500.0), "500");
}

// Test values that exercise the numerator.assign_pow10 path
TEST_F(FocalFunctionTest, NumeratorAssignPow10Path) {
    // Values with exp10 < 0 that trigger numerator.assign_pow10
    EXPECT_EQ(format_double(0.5), "0.5");
    EXPECT_EQ(format_double(0.25), "0.25");
    EXPECT_EQ(format_double(0.125), "0.125");
    EXPECT_EQ(format_double(0.0625), "0.0625");
    EXPECT_EQ(format_double(0.03125), "0.03125");
}

// Test values that might cause precision loss or rounding errors
TEST_F(FocalFunctionTest, PrecisionLossValues) {
    // These values are known to cause issues in some implementations
    EXPECT_EQ(format_double(0.1 + 0.2), "0.30000000000000004");
    EXPECT_EQ(format_double(1.0 / 3.0), "0.3333333333333333");
    EXPECT_EQ(format_double(1.0 / 7.0), "0.14285714285714285");
}

// Test values at the boundaries of double precision
TEST_F(FocalFunctionTest, DoublePrecisionBoundaries) {
    // Smallest positive normal double
    EXPECT_EQ(format_double(std::numeric_limits<double>::min()), "2.2250738585072014e-308");
    
    // Largest finite double
    EXPECT_EQ(format_double(std::numeric_limits<double>::max()), "1.7976931348623157e+308");
    
    // Smallest positive denormalized double
    EXPECT_EQ(format_double(std::numeric_limits<double>::denorm_min()), "5e-324");
}

// Test integer values that should not have decimal points
TEST_F(FocalFunctionTest, IntegerValues) {
    EXPECT_EQ(format_double(42.0), "42");
    EXPECT_EQ(format_double(123456789.0), "123456789");
    EXPECT_EQ(format_double(-987654321.0), "-987654321");
}

// Test values that exercise both branches in sequence (mixed positive/negative exp10)
TEST_F(FocalFunctionTest, MixedExponentValues) {
    EXPECT_EQ(format_double(3.14159), "3.14159");
    EXPECT_EQ(format_double(2.71828), "2.71828");
    EXPECT_EQ(format_double(0.57721), "0.57721");  // Euler-Mascheroni constant
    EXPECT_EQ(format_double(1.61803), "1.61803");  // Golden ratio
}