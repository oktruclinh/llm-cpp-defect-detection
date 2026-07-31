#include <gtest/gtest.h>
#include "fmt/format.h" // Public API header

// This test suite focuses on the internal logic related to `assign_pow10` and
// bit shifting within the `format-inl.h` context, specifically when `exp10` is
// positive or negative. Since `format-inl.h` contains implementation details
// and not a public API, we need to test its effects through the public `fmt::format`
// function, which internally uses these mechanisms for floating-point formatting.

// The provided snippet is part of a larger function, likely involved in
// converting a floating-point number to a string using a high-precision
// decimal representation (e.g., Grisu or Dragonbox).
//
// The key parts are:
// 1. `denominator.assign_pow10(exp10);` when `exp10 > 0`
// 2. `denominator <<= shift;`
// 3. `numerator.assign_pow10(-exp10);` when `exp10 < 0`
// 4. `lower.assign(numerator);`
//
// We will test how different `exp10` values (positive, negative, zero)
// and their interaction with the internal big integer arithmetic affect
// the final formatted output.

// Helper function to format a double with a specific precision to observe
// the effects of the internal power-of-10 and shifting logic.
std::string format_double_with_precision(double value, int precision) {
  return fmt::format("{:.{}f}", value, precision);
}

TEST(FmtInternalPow10Test, PositiveExp10AffectsDenominator) {
  // When exp10 is positive, `denominator` is assigned a power of 10.
  // This typically means we are dealing with a number that needs to be
  // scaled down (e.g., 123.45, exp10 might be 2 for 10^2).
  // The `shift` then further adjusts the denominator.
  //
  // Test case: A number that requires a positive exp10 for correct formatting.
  // Example: 123.45, formatted to 2 decimal places.
  // Internally, this might involve `denominator.assign_pow10(2)` and then shifting.
  // If `assign_pow10` or `shift` were buggy, the output would be wrong.
  EXPECT_EQ(format_double_with_precision(123.45, 2), "123.45");
  EXPECT_EQ(format_double_with_precision(0.01, 2), "0.01");
  EXPECT_EQ(format_double_with_precision(1.0, 0), "1");
  EXPECT_EQ(format_double_with_precision(12345.6789, 4), "12345.6789");
  EXPECT_EQ(format_double_with_precision(12345.6789, 2), "12345.68"); // Check rounding
}

TEST(FmtInternalPow10Test, NegativeExp10AffectsNumerator) {
  // When exp10 is negative, `numerator` is assigned a power of 10.
  // This typically means we are dealing with a very small number (e.g., 0.0001).
  // Internally, the number might be scaled up by 10^(-exp10) to work with integers.
  // `lower.assign(numerator)` then copies this scaled value.
  //
  // Test case: A number that requires a negative exp10 for correct formatting.
  // Example: 0.0001, formatted to 4 decimal places.
  // Internally, this might involve `numerator.assign_pow10(4)`.
  // If `assign_pow10` or `lower.assign` were buggy, the output would be wrong.
  EXPECT_EQ(format_double_with_precision(0.0001, 4), "0.0001");
  EXPECT_EQ(format_double_with_precision(0.000000123, 9), "0.000000123");
  EXPECT_EQ(format_double_with_precision(1.23e-5, 7), "0.0000123");
  EXPECT_EQ(format_double_with_precision(1.0e-9, 9), "0.000000001");
  EXPECT_EQ(format_double_with_precision(1.0e-10, 10), "0.0000000001");
}

TEST(FmtInternalPow10Test, ZeroExp10) {
  // When exp10 is zero, neither of the `if/else if` branches are taken for `assign_pow10`.
  // This implies that the number is already in a suitable range or handled differently.
  // We expect correct formatting for numbers that might result in exp10 = 0.
  EXPECT_EQ(format_double_with_precision(1.0, 2), "1.00");
  EXPECT_EQ(format_double_with_precision(0.0, 5), "0.00000");
  EXPECT_EQ(format_double_with_precision(123.0, 0), "123");
  EXPECT_EQ(format_double_with_precision(123.0, 3), "123.000");
}

TEST(FmtInternalPow10Test, EdgeCasesAndBoundaries) {
  // Test very large and very small numbers to ensure the big integer arithmetic
  // and power-of-10 scaling works correctly at extremes.
  EXPECT_EQ(format_double_with_precision(1.2345678901234567e+20, 0), "123456789012345670000"); // Large number, no decimals
  EXPECT_EQ(format_double_with_precision(1.2345678901234567e+20, 2), "123456789012345670000.00"); // Large number, with decimals
  EXPECT_EQ(format_double_with_precision(1.2345678901234567e-20, 25), "0.0000000000000000000123457"); // Small number
  EXPECT_EQ(format_double_with_precision(1.0e-30, 30), "0.000000000000000000000000000001"); // Extremely small
  EXPECT_EQ(format_double_with_precision(1.0e+30, 0), "1000000000000000000000000000000"); // Extremely large

  // Test numbers near powers of 10
  EXPECT_EQ(format_double_with_precision(0.999999, 6), "0.999999");
  EXPECT_EQ(format_double_with_precision(0.9999999, 6), "1.000000"); // Rounding up
  EXPECT_EQ(format_double_with_precision(10.0 - 1e-10, 10), "9.9999999999");
  EXPECT_EQ(format_double_with_precision(10.0 + 1e-10, 10), "10.0000000001");
}

TEST(FmtInternalPow10Test, ZeroValue) {
  // Special handling for zero should be robust.
  EXPECT_EQ(format_double_with_precision(0.0, 0), "0");
  EXPECT_EQ(format_double_with_precision(0.0, 1), "0.0");
  EXPECT_EQ(format_double_with_precision(-0.0, 2), "-0.00"); // Check negative zero
}

TEST(FmtInternalPow10Test, NaNAndInf) {
  // While not directly related to `assign_pow10` or `shift` for finite numbers,
  // these are important edge cases for floating-point formatting.
  // They should not trigger the `assign_pow10` logic for finite numbers.
  EXPECT_EQ(fmt::format("{}", std::numeric_limits<double>::quiet_NaN()), "nan");
  EXPECT_EQ(fmt::format("{}", std::numeric_limits<double>::infinity()), "inf");
  EXPECT_EQ(fmt::format("{}", -std::numeric_limits<double>::infinity()), "-inf");
}