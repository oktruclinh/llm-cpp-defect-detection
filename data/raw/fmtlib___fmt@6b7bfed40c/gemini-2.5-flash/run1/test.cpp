#include <gtest/gtest.h>
#include "fmt/format.h"
#include "fmt/format-inl.h" // For access to internal functions if needed, though public API is preferred.

// This test suite focuses on the internal `format_dragonbox` function,
// specifically the logic related to `exp10` handling and `denominator`
// and `numerator` assignments. Since `format_dragonbox` is an internal
// implementation detail, we'll test its behavior indirectly through
// `fmt::format` with floating-point numbers, ensuring that the
// underlying logic for `exp10` and power-of-10 handling is correct.

// We'll use a custom formatter for `double` to ensure we hit the
// `format_dragonbox` path.

namespace fmt {
namespace detail {

// Helper function to expose the internal logic for testing purposes,
// if direct testing of `format_dragonbox` is desired.
// However, the prompt asks to exercise through public API, so this is
// more for understanding the internal structure.
// For this specific snippet, it's hard to isolate without the full context
// of `format_dragonbox`. We'll focus on the public API.

// The snippet shows logic for `denominator.assign_pow10(exp10)` and
// `numerator.assign_pow10(-exp10)`. This implies that `exp10`
// determines whether we're multiplying or dividing by a power of 10.
// We'll test this by formatting numbers that require such adjustments.

} // namespace detail
} // namespace fmt

TEST(FormatDragonboxExp10Test, PositiveExp10Handling) {
  // Test cases where exp10 would likely be positive, meaning the number
  // is large and needs a large denominator (or small numerator).
  // Example: 1.23e5 -> 123000.0. The internal representation might
  // involve a denominator of 10^0 and a large numerator, or a small
  // numerator and a negative exp10.
  // The snippet shows:
  // if (exp10 >= 0) {
  //   denominator.assign_pow10(exp10);
  //   denominator <<= shift;
  // }
  // This implies that if exp10 is positive, the denominator is a power of 10.

  // Test a number that should result in a positive exp10 in the internal
  // Dragonbox algorithm, leading to `denominator.assign_pow10(exp10)`.
  // For example, formatting 123.0 should internally have an exp10 that
  // makes the denominator 1.
  EXPECT_EQ(fmt::format("{}", 123.0), "123");
  EXPECT_EQ(fmt::format("{}", 12345.0), "12345");
  EXPECT_EQ(fmt::format("{}", 1.0), "1");
  EXPECT_EQ(fmt::format("{}", 10.0), "10");
  EXPECT_EQ(fmt::format("{}", 100.0), "100");

  // Test numbers that are exact powers of 10.
  EXPECT_EQ(fmt::format("{}", 1e5), "100000");
  EXPECT_EQ(fmt::format("{}", 1e10), "10000000000");
}

TEST(FormatDragonboxExp10Test, NegativeExp10Handling) {
  // Test cases where exp10 would likely be negative, meaning the number
  // is small and needs a large numerator (or small denominator).
  // Example: 0.00123 -> 123 / 100000.
  // The snippet shows:
  // } else if (exp10 < 0) {
  //   numerator.assign_pow10(-exp10);
  //   lower.assign(numerator);
  // }
  // This implies that if exp10 is negative, the numerator is a power of 10.

  // Test a number that should result in a negative exp10 in the internal
  // Dragonbox algorithm, leading to `numerator.assign_pow10(-exp10)`.
  EXPECT_EQ(fmt::format("{}", 0.1), "0.1");
  EXPECT_EQ(fmt::format("{}", 0.01), "0.01");
  EXPECT_EQ(fmt::format("{}", 0.001), "0.001");
  EXPECT_EQ(fmt::format("{}", 0.123), "0.123");
  EXPECT_EQ(fmt::format("{}", 0.00001), "0.00001");

  // Test numbers that are exact inverse powers of 10.
  EXPECT_EQ(fmt::format("{}", 1e-1), "0.1");
  EXPECT_EQ(fmt::format("{}", 1e-5), "0.00001");
  EXPECT_EQ(fmt::format("{}", 1e-10), "0.0000000001");
}

TEST(FormatDragonboxExp10Test, ZeroExp10Handling) {
  // Test cases where exp10 might be zero.
  // This would fall under the `exp10 >= 0` branch, with `denominator.assign_pow10(0)`.
  // A denominator of 1.
  EXPECT_EQ(fmt::format("{}", 1.0), "1");
  EXPECT_EQ(fmt::format("{}", 2.0), "2");
  EXPECT_EQ(fmt::format("{}", 123.0), "123"); // Already covered, but emphasizes zero exp10.
}

TEST(FormatDragonboxExp10Test, EdgeCases) {
  // Test zero
  EXPECT_EQ(fmt::format("{}", 0.0), "0");
  EXPECT_EQ(fmt::format("{}", -0.0), "0"); // -0.0 is formatted as 0

  // Test very small numbers (denormalized or near zero)
  EXPECT_EQ(fmt::format("{}", 1e-30), "1e-30");
  EXPECT_EQ(fmt::format("{}", 1e-300), "1e-300"); // Should still use Dragonbox

  // Test very large numbers
  EXPECT_EQ(fmt::format("{}", 1e30), "1e+30");
  EXPECT_EQ(fmt::format("{}", 1e300), "1e+300"); // Should still use Dragonbox

  // Test numbers with many decimal places that require precise power-of-10 handling
  EXPECT_EQ(fmt::format("{}", 0.12345678901234567), "0.12345678901234567");
  EXPECT_EQ(fmt::format("{}", 123456789012345.67), "123456789012345.67");
}

TEST(FormatDragonboxExp10Test, SpecificValuesRequiringPrecisePowerOf10) {
  // These values are known to sometimes cause issues with floating-point
  // to string conversion if power-of-10 handling is not exact.
  EXPECT_EQ(fmt::format("{}", 0.3), "0.3");
  EXPECT_EQ(fmt::format("{}", 0.6), "0.6");
  EXPECT_EQ(fmt::format("{}", 0.9), "0.9");
  EXPECT_EQ(fmt::format("{}", 0.125), "0.125"); // Exact binary fraction
  EXPECT_EQ(fmt::format("{}", 0.0625), "0.0625"); // Exact binary fraction

  // A number that is slightly off an exact power of 10
  EXPECT_EQ(fmt::format("{}", 9.999999999999999), "9.999999999999999");
  EXPECT_EQ(fmt::format("{}", 10.000000000000001), "10.000000000000001");
}