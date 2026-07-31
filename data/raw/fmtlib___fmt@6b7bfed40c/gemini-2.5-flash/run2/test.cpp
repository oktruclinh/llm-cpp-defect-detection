#include <gtest/gtest.h>
#include "fmt/format.h"
#include "fmt/format-inl.h" // For access to internal functions if necessary, though public API is preferred.

// This test suite focuses on the internal `format_dragonbox` function
// which is responsible for converting floating-point numbers to strings
// using the Dragonbox algorithm. The provided snippet is part of the
// `to_decimal` function within `format_dragonbox`.
//
// Since `format_dragonbox` is an internal implementation detail, we will
// test its behavior indirectly by calling `fmt::format` with floating-point
// numbers and verifying the output. This ensures we are testing through
// the public API as requested.

// Helper function to format a double and return the string.
template <typename T>
std::string format_double_with_precision(T value, int precision = -1) {
  if (precision >= 0) {
    return fmt::format("{:.{}g}", value, precision);
  }
  return fmt::format("{}", value);
}

TEST(FormatDragonboxTest, PositiveIntegers) {
  // These numbers should have exact representations.
  EXPECT_EQ(format_double_with_precision(0.0), "0");
  EXPECT_EQ(format_double_with_precision(1.0), "1");
  EXPECT_EQ(format_double_with_precision(123.0), "123");
  EXPECT_EQ(format_double_with_precision(123456789.0), "123456789");
}

TEST(FormatDragonboxTest, NegativeIntegers) {
  EXPECT_EQ(format_double_with_precision(-0.0), "-0"); // IEEE 754 -0.0
  EXPECT_EQ(format_double_with_precision(-1.0), "-1");
  EXPECT_EQ(format_double_with_precision(-123.0), "-123");
}

TEST(FormatDragonboxTest, SmallPositiveDecimals) {
  // Numbers that might involve `exp10 < 0` path.
  EXPECT_EQ(format_double_with_precision(0.1), "0.1");
  EXPECT_EQ(format_double_with_precision(0.01), "0.01");
  EXPECT_EQ(format_double_with_precision(0.125), "0.125"); // Exact binary fraction
  EXPECT_EQ(format_double_with_precision(0.0001), "0.0001");
  EXPECT_EQ(format_double_with_precision(0.0000000001), "0.0000000001");
}

TEST(FormatDragonboxTest, SmallNegativeDecimals) {
  EXPECT_EQ(format_double_with_precision(-0.1), "-0.1");
  EXPECT_EQ(format_double_with_precision(-0.01), "-0.01");
  EXPECT_EQ(format_double_with_precision(-0.125), "-0.125");
}

TEST(FormatDragonboxTest, LargePositiveDecimals) {
  // Numbers that might involve `exp10 > 0` path.
  EXPECT_EQ(format_double_with_precision(10.0), "10");
  EXPECT_EQ(format_double_with_precision(123.45), "123.45");
  EXPECT_EQ(format_double_with_precision(12345.6789), "12345.6789");
  EXPECT_EQ(format_double_with_precision(123456789.12345), "123456789.12345");
}

TEST(FormatDragonboxTest, LargeNegativeDecimals) {
  EXPECT_EQ(format_double_with_precision(-10.0), "-10");
  EXPECT_EQ(format_double_with_precision(-123.45), "-123.45");
}

TEST(FormatDragonboxTest, ScientificNotation) {
  // Numbers that naturally fall into scientific notation.
  EXPECT_EQ(format_double_with_precision(1.23e10), "1.23e+10");
  EXPECT_EQ(format_double_with_precision(1.23e-10), "1.23e-10");
  EXPECT_EQ(format_double_with_precision(1.0e20), "1e+20");
  EXPECT_EQ(format_double_with_precision(1.0e-20), "1e-20");
  EXPECT_EQ(format_double_with_precision(1.23456789e-5), "1.23456789e-5");
  EXPECT_EQ(format_double_with_precision(1.23456789e+5), "123456.789"); // Within default precision, not scientific
  EXPECT_EQ(format_double_with_precision(1.23456789e+6), "1.23456789e+6"); // Beyond default precision, scientific
}

TEST(FormatDragonboxTest, EdgeCases_Zero) {
  EXPECT_EQ(format_double_with_precision(0.0), "0");
  EXPECT_EQ(format_double_with_precision(-0.0), "-0");
}

TEST(FormatDragonboxTest, EdgeCases_Infinity) {
  EXPECT_EQ(format_double_with_precision(std::numeric_limits<double>::infinity()), "inf");
  EXPECT_EQ(format_double_with_precision(-std::numeric_limits<double>::infinity()), "-inf");
}

TEST(FormatDragonboxTest, EdgeCases_NaN) {
  // NaN representation can vary, but "nan" is standard.
  EXPECT_EQ(format_double_with_precision(std::numeric_limits<double>::quiet_NaN()), "nan");
}

TEST(FormatDragonboxTest, EdgeCases_MinMaxDouble) {
  // Test with smallest positive normal double
  EXPECT_EQ(format_double_with_precision(std::numeric_limits<double>::min()), "2.2250738585072014e-308");
  // Test with largest finite double
  EXPECT_EQ(format_double_with_precision(std::numeric_limits<double>::max()), "1.7976931348623157e+308");
}

TEST(FormatDragonboxTest, EdgeCases_Subnormal) {
  // Smallest positive subnormal double
  EXPECT_EQ(format_double_with_precision(std::numeric_limits<double>::denorm_min()), "4.9406564584124654e-324");
}

TEST(FormatDragonboxTest, PrecisionSpecifier) {
  // Test with precision specifiers, which can influence the output
  // and thus the internal `to_decimal` logic.
  EXPECT_EQ(format_double_with_precision(M_PI, 2), "3.1");
  EXPECT_EQ(format_double_with_precision(M_PI, 5), "3.1416");
  EXPECT_EQ(format_double_with_precision(M_PI, 10), "3.141592654");
  EXPECT_EQ(format_double_with_precision(123.456789, 3), "123");
  EXPECT_EQ(format_double_with_precision(123.456789, 5), "123.46");
  EXPECT_EQ(format_double_with_precision(0.000123456, 3), "0.000123");
  EXPECT_EQ(format_double_with_precision(0.000123456, 2), "0.00012");
  EXPECT_EQ(format_double_with_precision(1.23456789e-5, 3), "1.23e-5");
  EXPECT_EQ(format_double_with_precision(1.23456789e+5, 3), "1.23e+5"); // Precision forces scientific
}

TEST(FormatDragonboxTest, FloatType) {
  // Ensure float values are also handled correctly.
  EXPECT_EQ(format_double_with_precision(0.1f), "0.1");
  EXPECT_EQ(format_double_with_precision(1.234567f), "1.234567");
  EXPECT_EQ(format_double_with_precision(1.234567e10f), "1.234567e+10");
  EXPECT_EQ(format_double_with_precision(std::numeric_limits<float>::max()), "3.40282347e+38");
}

// Test case for a specific value that might have tricky decimal representation
TEST(FormatDragonboxTest, TrickyDecimalValue) {
  // This value is known to be tricky for some float-to-string conversions.
  // Dragonbox should handle it correctly.
  EXPECT_EQ(format_double_with_precision(0.3), "0.3");
  EXPECT_EQ(format_double_with_precision(0.6), "0.6");
  EXPECT_EQ(format_double_with_precision(0.9), "0.9");
  EXPECT_EQ(format_double_with_precision(0.1 + 0.2), "0.3"); // Floating point addition
}

// Test case for values that are very close to powers of 10
TEST(FormatDragonboxTest, NearPowersOfTen) {
  EXPECT_EQ(format_double_with_precision(9.999999999999999), "10"); // Rounds up
  EXPECT_EQ(format_double_with_precision(1.0000000000000001), "1"); // Rounds down
  EXPECT_EQ(format_double_with_precision(99.99999999999999), "100");
  EXPECT_EQ(format_double_with_precision(0.9999999999999999), "1");
  EXPECT_EQ(format_double_with_precision(0.09999999999999999), "0.1");
}