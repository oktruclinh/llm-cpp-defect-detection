```cpp
#include <gtest/gtest.h>
#include "fmt/chrono.h"
#include "fmt/format.h"

#include <chrono>
#include <string>
#include <vector>

// Helper function to format a duration and return the string
template <typename Duration>
std::string format_duration(const Duration& d, fmt::string_view format_str) {
  return fmt::format(fmt::runtime(format_str), d);
}

// Test fixture for chrono durations
template <typename Duration>
struct ChronoDurationTest : public ::testing::Test {};

using DurationTypes = ::testing::Types<
    std::chrono::nanoseconds,
    std::chrono::microseconds,
    std::chrono::milliseconds,
    std::chrono::seconds,
    std::chrono::minutes,
    std::chrono::hours>;

TYPED_TEST_SUITE(ChronoDurationTest, DurationTypes);

// Test case for formatting durations with fractional seconds
// This specifically targets the `write_fractional_seconds` part of the focal function
TYPED_TEST(ChronoDurationTest, FractionalSecondsFormatting) {
  using Duration = TypeParam;

  // Test with various fractional second values
  // The focal function is called when the format string contains fractional seconds
  // and the duration type is not just seconds.

  // Test case 1: Milliseconds
  if constexpr (std::is_same_v<Duration, std::chrono::milliseconds>) {
    EXPECT_EQ(format_duration(Duration(123), "{:%S.%L}"), "00.123");
    EXPECT_EQ(format_duration(Duration(1234), "{:%S.%L}"), "01.234");
    EXPECT_EQ(format_duration(Duration(0), "{:%S.%L}"), "00.000");
    EXPECT_EQ(format_duration(Duration(999), "{:%S.%L}"), "00.999");
    EXPECT_EQ(format_duration(Duration(12345), "{:%S.%L}"), "12.345");
    EXPECT_EQ(format_duration(Duration(-123), "{:%S.%L}"), "-00.123"); // Negative duration
  }

  // Test case 2: Microseconds
  if constexpr (std::is_same_v<Duration, std::chrono::microseconds>) {
    EXPECT_EQ(format_duration(Duration(123456), "{:%S.%f}"), "00.123456");
    EXPECT_EQ(format_duration(Duration(1234567), "{:%S.%f}"), "01.234567");
    EXPECT_EQ(format_duration(Duration(0), "{:%S.%f}"), "00.000000");
    EXPECT_EQ(format_duration(Duration(999999), "{:%S.%f}"), "00.999999");
    EXPECT_EQ(format_duration(Duration(12345678), "{:%S.%f}"), "12.345678");
    EXPECT_EQ(format_duration(Duration(-123456), "{:%S.%f}"), "-00.123456"); // Negative duration
  }

  // Test case 3: Nanoseconds
  if constexpr (std::is_same_v<Duration, std::chrono::nanoseconds>) {
    EXPECT_EQ(format_duration(Duration(123456789), "{:%S.%N}"), "00.123456789");
    EXPECT_EQ(format_duration(Duration(1234567890), "{:%S.%N}"), "01.234567890");
    EXPECT_EQ(format_duration(Duration(0), "{:%S.%N}"), "00.000000000");
    EXPECT_EQ(format_duration(Duration(999999999), "{:%S.%N}"), "00.999999999");
    EXPECT_EQ(format_duration(Duration(12345678901), "{:%S.%N}"), "12.345678901");
    EXPECT_EQ(format_duration(Duration(-123456789), "{:%S.%N}"), "-00.123456789"); // Negative duration
  }

  // Test case 4: Seconds with fractional part (should still show fractional)
  if constexpr (std::is_same_v<Duration, std::chrono::seconds>) {
    // Even though it's seconds, if the format string asks for fractional,
    // the fractional part should be zero-padded.
    EXPECT_EQ(format_duration(Duration(1), "{:%S.%L}"), "01.000");
    EXPECT_EQ(format_duration(Duration(1), "{:%S.%f}"), "01.000000");
    EXPECT_EQ(format_duration(Duration(1), "{:%S.%N}"), "01.000000000");
    EXPECT_EQ(format_duration(Duration(0), "{:%S.%L}"), "00.000");
    EXPECT_EQ(format_duration(Duration(0), "{:%S.%f}"), "00.000000");
    EXPECT_EQ(format_duration(Duration(0), "{:%S.%N}"), "00.000000000");
    EXPECT_EQ(format_duration(Duration(-1), "{:%S.%L}"), "-01.000");
  }

  // Test case 5: Minutes/Hours with fractional part (should still show fractional for seconds)
  if constexpr (std::is_same_v<Duration, std::chrono::minutes>) {
    EXPECT_EQ(format_duration(Duration(1), "{:%M:%S.%L}"), "01:00.000");
    EXPECT_EQ(format_duration(Duration(1) + std::chrono::milliseconds(500), "{:%M:%S.%L}"), "01:00.500");
    EXPECT_EQ(format_duration(Duration(0) + std::chrono::milliseconds(500), "{:%M:%S.%L}"), "00:00.500");
    EXPECT_EQ(format_duration(Duration(-1) - std::chrono::milliseconds(500), "{:%M:%S.%L}"), "-01:00.500");
  }
  if constexpr (std::is_same_v<Duration, std::chrono::hours>) {
    EXPECT_EQ(format_duration(Duration(1), "{:%H:%M:%S.%f}"), "01:00:00.000000");
    EXPECT_EQ(format_duration(Duration(1) + std::chrono::microseconds(123456), "{:%H:%M:%S.%f}"), "01:00:00.123456");
    EXPECT_EQ(format_duration(Duration(0) + std::chrono::microseconds(123456), "{:%H:%M:%S.%f}"), "00:00:00.123456");
    EXPECT_EQ(format_duration(Duration(-1) - std::chrono::microseconds(123456), "{:%H:%M:%S.%f}"), "-01:00:00.123456");
  }
}

// Test case for formatting durations without fractional seconds
// This ensures the `write(second(), 2)` part works correctly when no fractional part is requested.
TYPED_TEST(ChronoDurationTest, NoFractionalSecondsFormatting) {
  using Duration = TypeParam;

  // Test case 1: Milliseconds, but only seconds requested
  if constexpr (std::is_same_v<Duration, std::chrono::milliseconds>) {
    EXPECT_EQ(format_duration(Duration(123), "{:%S}"), "00");
    EXPECT_EQ(format_duration(Duration(1234), "{:%S}"), "01");
    EXPECT_EQ(format_duration(Duration(0), "{:%S}"), "00");
    EXPECT_EQ(format_duration(Duration(999), "{:%S}"), "00");
    EXPECT_EQ(format_duration(Duration(12345), "{:%S}"), "12");
    EXPECT_EQ(format_duration(Duration(-123), "{:%S}"), "-00");
    EXPECT_EQ(format_duration(Duration(-1234), "{:%S}"), "-01");
  }

  // Test case 2: Microseconds, but only seconds requested
  if constexpr (std::is_same_v<Duration, std::chrono::microseconds>) {
    EXPECT_EQ(format_duration(Duration(123456), "{:%S}"), "00");
    EXPECT_EQ(format_duration(Duration(1234567), "{:%S}"), "01");
    EXPECT_EQ(format_duration(Duration(0), "{:%S}"), "00");
    EXPECT_EQ(format_duration(Duration(999999), "{:%S}"), "00");
    EXPECT_EQ(format_duration(Duration(12345678), "{:%S}"), "12");
    EXPECT_EQ(format_duration(Duration(-123456), "{:%S}"), "-00");
    EXPECT_EQ(format_duration(Duration(-1234567), "{:%S}"), "-01");
  }

  // Test case 3: Nanoseconds, but only seconds requested
  if constexpr (std::is_same_v<Duration, std::chrono::nanoseconds>) {
    EXPECT_EQ(format_duration(Duration(123456789), "{:%S}"), "00");
    EXPECT_EQ(format_duration(Duration(1234567890), "{:%S}"), "01");
    EXPECT_EQ(format_duration(Duration(0), "{:%S}"), "00");
    EXPECT_EQ(format_duration(Duration(999999999), "{:%S}"), "00");
    EXPECT_EQ(format_duration(Duration(12345678901), "{:%S}"), "12");
    EXPECT_EQ(format_duration(Duration(-123456789), "{:%S}"), "-00");
    EXPECT_EQ(format_duration(Duration(-1234567890), "{:%S}"), "-01");
  }

  // Test case 4: Seconds
  if constexpr (std::is_same_v<Duration, std::chrono::seconds>) {
    EXPECT_EQ(format_duration(Duration(1), "{:%S}"), "01");
    EXPECT_EQ(format_duration(Duration(59), "{:%S}"), "59");
    EXPECT_EQ(format_duration(Duration(60), "{:%S}"), "00"); // Seconds wrap around for %S
    EXPECT_EQ(format_duration(Duration(0), "{:%S}"), "00");
    EXPECT_EQ(format_duration(Duration(-1), "{:%S}"), "-01");
    EXPECT_EQ(format_duration(Duration(-60), "{:%S}"), "-00");
  }

  // Test case 5: Minutes
  if constexpr (std::is_same_v<Duration, std::chrono::minutes>) {
    EXPECT_EQ(format_duration(Duration(1), "{:%M:%S}"), "01:00");
    EXPECT_EQ(format_duration(Duration(1) + std::chrono::seconds(30), "{:%M:%S}"), "01:30");
    EXPECT_EQ(format_duration(Duration(0), "{:%M:%S}"), "00:00");
    EXPECT_EQ(format_duration(Duration(-1), "{:%M:%S}"), "-01:00");
  }

  // Test case 6: Hours
  if constexpr (std::is_same_v<Duration, std::chrono::hours>) {
    EXPECT_EQ(format_duration(Duration(1), "{:%H:%M:%S}"), "01:00:00");
    EXPECT_EQ(format_duration(Duration(1) + std::chrono::minutes(30) + std::chrono::seconds(15), "{:%H:%M:%S}"), "01:30:15");
    EXPECT_EQ(format_duration(Duration(0), "{:%H:%M:%S}"), "00:00:00");
    EXPECT_EQ(format_duration(Duration(-1), "{:%H:%M:%S}"), "-01:00:00");
  }
}

// Test with different precisions for fractional seconds
TEST(ChronoDurationTestSpecific, FractionalSecondsPrecision) {
  std::chrono::nanoseconds ns(123456789); // 0.123456789 seconds

  EXPECT_EQ(format_duration(ns, "{:%S.%L}"), "00.123"); // Milliseconds
  EXPECT_EQ(format_duration(ns, "{:%S.%f}"), "00.123456"); // Microseconds
  EXPECT_EQ(format_duration(ns, "{:%S.%N}"), "00.123456789"); // Nanoseconds

  // Test with less precision than available
  EXPECT_EQ(format_duration(ns, "{:%S.%3N}"), "00.123"); // 3 nanoseconds (effectively milliseconds)
  EXPECT_EQ(format_duration(ns, "{:%S.%6N}"), "00.123456"); // 6 nanoseconds (effectively microseconds)
  EXPECT_EQ(format_duration(ns, "{:%S.%9N}"), "00.123456789"); // 9 nanoseconds (full nanoseconds)

  // Test with more precision than available (should pad with zeros)
  EXPECT_EQ(format_duration(std::chrono::milliseconds(123), "{:%S.%f}"), "00.123000");
  EXPECT_EQ(format_duration(std::chrono::milliseconds(123), "{:%S.%N}"), "00.123000000");
  EXPECT_EQ(format_duration(std::chrono::microseconds(123456), "{:%S.%N}"), "00.123456000");

  // Test with custom precision specifiers
  EXPECT_EQ(format_duration(ns, "{:%S.%1N}"), "00.1");
  EXPECT_EQ(format_duration(ns, "{:%S.%2N}"), "00.12");
  EXPECT_EQ(format_duration(ns, "{:%S.%12N}"), "00.123456789000"); // More than 9, should pad
}

// Test with zero duration
TEST(ChronoDurationTestSpecific, ZeroDuration) {
  EXPECT_EQ(format_duration(std::chrono::seconds(0), "{:%S.%L}"), "00.000");
  EXPECT_EQ(format_duration(std::chrono::milliseconds(0), "{:%S.%f}"), "00.000000");
  EXPECT_EQ(format_duration(std::chrono::nanoseconds(0), "{:%S.%N}"), "00.000000000");
  EXPECT_EQ(format_duration(std::chrono::seconds(0), "{:%S}"), "00");
}

// Test with negative duration
TEST(ChronoDurationTestSpecific, NegativeDuration) {
  EXPECT_EQ(format_duration(std::chrono::seconds(-1), "{:%S.%L}"), "-01.000");
  EXPECT_EQ(format_duration(std::chrono::milliseconds(-123), "{:%S.%L}"), "-00.123");
  EXPECT_EQ(format_duration(std::chrono::microseconds(-123456), "{:%S.%f}"), "-00.123456");
  EXPECT_EQ(format_duration(std::chrono::nanoseconds(-123456789), "{:%S.%N}"), "-00.123456789");

  // Ensure the sign is only applied once at the beginning
  EXPECT_EQ(format_duration(std::chrono::seconds(-61), "{:%M:%S.%L}"), "-01:01.000");
  EXPECT_EQ(format_duration(std::chrono::seconds(-60), "{:%M:%S.%L}"), "-01:00.000");
  EXPECT_EQ(format_duration(std::chrono::seconds(-59), "{:%M:%S.%L}"), "-00:59.000");
}

// Test with large durations to ensure correct second calculation
TEST(ChronoDurationTestSpecific, LargeDurations) {
  // Large number of milliseconds
  std::chrono::milliseconds large_ms(1234567890123); // ~14 days
  EXPECT_EQ(format_duration