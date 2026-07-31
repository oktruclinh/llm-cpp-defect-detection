#include <gtest/gtest.h>
#include <chrono>
#include <fmt/chrono.h>
#include <fmt/format.h>

// Helper function to format a duration and return the string.
template <typename Rep, typename Period>
std::string format_duration(std::chrono::duration<Rep, Period> d) {
  return fmt::format("{}", d);
}

// Test fixture for chrono durations to avoid repetition
template <typename DurationType>
struct ChronoDurationTest : public ::testing::Test {};

using DurationTypes = ::testing::Types<
    std::chrono::nanoseconds,
    std::chrono::microseconds,
    std::chrono::milliseconds,
    std::chrono::seconds,
    std::chrono::minutes,
    std::chrono::hours,
    std::chrono::duration<long long, std::ratio<1, 1000000000000>>, // Picoseconds
    std::chrono::duration<long long, std::ratio<1, 1000000000000000>> // Femtoseconds
>;
TYPED_TEST_SUITE(ChronoDurationTest, DurationTypes);

// Test cases for various durations and fractional seconds.
TYPED_TEST(ChronoDurationTest, BasicFractionalSeconds) {
  using Duration = TypeParam;

  // Test with a duration that has fractional seconds
  if constexpr (std::is_same_v<Duration, std::chrono::seconds>) {
    // Seconds don't have fractional parts by definition, so we test with a sub-second duration
    // that will be formatted as fractional seconds when combined with a larger unit.
    // The focal function is called when formatting a duration that is not seconds, minutes, or hours
    // and has a fractional part.
    // We need to ensure the `write_fractional_seconds` part is exercised.
    // The easiest way to do this is to format a duration that is less than a second.
    // Or, format a duration that is a mix, e.g., 1.5 seconds.
    // fmt::format("{}", std::chrono::duration<double>(1.5)) will call the focal function.
    EXPECT_EQ(fmt::format("{}", std::chrono::duration<double>(1.5)), "1.5s");
    EXPECT_EQ(fmt::format("{}", std::chrono::duration<double>(1.0)), "1s");
    EXPECT_EQ(fmt::format("{}", std::chrono::duration<double>(0.5)), "0.5s");
    EXPECT_EQ(fmt::format("{}", std::chrono::duration<double>(0.0)), "0s");
  } else if constexpr (std::is_same_v<Duration, std::chrono::milliseconds>) {
    EXPECT_EQ(fmt::format("{}", std::chrono::milliseconds(1234)), "1.234s");
    EXPECT_EQ(fmt::format("{}", std::chrono::milliseconds(500)), "0.5s");
    EXPECT_EQ(fmt::format("{}", std::chrono::milliseconds(1)), "0.001s");
    EXPECT_EQ(fmt::format("{}", std::chrono::milliseconds(0)), "0s");
    EXPECT_EQ(fmt::format("{}", std::chrono::milliseconds(-1234)), "-1.234s");
  } else if constexpr (std::is_same_v<Duration, std::chrono::microseconds>) {
    EXPECT_EQ(fmt::format("{}", std::chrono::microseconds(1234567)), "1.234567s");
    EXPECT_EQ(fmt::format("{}", std::chrono::microseconds(500000)), "0.5s");
    EXPECT_EQ(fmt::format("{}", std::chrono::microseconds(1)), "0.000001s");
    EXPECT_EQ(fmt::format("{}", std::chrono::microseconds(0)), "0s");
    EXPECT_EQ(fmt::format("{}", std::chrono::microseconds(-1234567)), "-1.234567s");
  } else if constexpr (std::is_same_v<Duration, std::chrono::nanoseconds>) {
    EXPECT_EQ(fmt::format("{}", std::chrono::nanoseconds(1234567890)), "1.23456789s");
    EXPECT_EQ(fmt::format("{}", std::chrono::nanoseconds(500000000)), "0.5s");
    EXPECT_EQ(fmt::format("{}", std::chrono::nanoseconds(1)), "0.000000001s");
    EXPECT_EQ(fmt::format("{}", std::chrono::nanoseconds(0)), "0s");
    EXPECT_EQ(fmt::format("{}", std::chrono::nanoseconds(-1234567890)), "-1.23456789s");
  } else if constexpr (std::is_same_v<Duration, std::chrono::minutes>) {
    // Minutes are handled by a different branch, but we can test fractional minutes
    // by using a floating-point representation.
    EXPECT_EQ(fmt::format("{}", std::chrono::duration<double, std::ratio<60>>(1.5)), "1.5min");
    EXPECT_EQ(fmt::format("{}", std::chrono::duration<double, std::ratio<60>>(0.5)), "0.5min");
  } else if constexpr (std::is_same_v<Duration, std::chrono::hours>) {
    // Hours are handled by a different branch, but we can test fractional hours
    // by using a floating-point representation.
    EXPECT_EQ(fmt::format("{}", std::chrono::duration<double, std::ratio<3600>>(1.5)), "1.5h");
    EXPECT_EQ(fmt::format("{}", std::chrono::duration<double, std::ratio<3600>>(0.5)), "0.5h");
  } else {
    // For custom durations with smaller periods than nanoseconds,
    // they should also be formatted as fractional seconds.
    // The focal function's `write(second(), 2)` part is for the integer seconds,
    // and `write_fractional_seconds` handles the rest.
    // Let's test a duration that is less than a second but not a standard chrono unit.
    // Example: 1.234567890123 seconds (picoseconds)
    if constexpr (std::is_same_v<Duration, std::chrono::duration<long long, std::ratio<1, 1000000000000>>>) { // Picoseconds
      EXPECT_EQ(fmt::format("{}", Duration(1234567890123LL)), "1.234567890123s");
      EXPECT_EQ(fmt::format("{}", Duration(1LL)), "0.000000000001s");
      EXPECT_EQ(fmt::format("{}", Duration(0LL)), "0s");
      EXPECT_EQ(fmt::format("{}", Duration(-1234567890123LL)), "-1.234567890123s");
    } else if constexpr (std::is_same_v<Duration, std::chrono::duration<long long, std::ratio<1, 1000000000000000>>>) { // Femtoseconds
      EXPECT_EQ(fmt::format("{}", Duration(1234567890123456LL)), "1.234567890123456s");
      EXPECT_EQ(fmt::format("{}", Duration(1LL)), "0.000000000000001s");
      EXPECT_EQ(fmt::format("{}", Duration(0LL)), "0s");
      EXPECT_EQ(fmt::format("{}", Duration(-1234567890123456LL)), "-1.234567890123456s");
    }
  }
}

// Test with floating-point representations of durations
TEST(ChronoDurationTest, FloatingPointDurations) {
  EXPECT_EQ(fmt::format("{}", std::chrono::duration<double>(1.234)), "1.234s");
  EXPECT_EQ(fmt::format("{}", std::chrono::duration<double>(0.0)), "0s");
  EXPECT_EQ(fmt::format("{}", std::chrono::duration<double>(-1.234)), "-1.234s");
  EXPECT_EQ(fmt::format("{}", std::chrono::duration<double>(1.000000001)), "1.000000001s"); // More precision
  EXPECT_EQ(fmt::format("{}", std::chrono::duration<double>(1.0 / 3.0)), "0.3333333333333333s"); // Recurring decimal
  EXPECT_EQ(fmt::format("{}", std::chrono::duration<float>(1.234f)), "1.234s");
}

// Test edge cases for fractional seconds
TEST(ChronoDurationTest, FractionalSecondsEdgeCases) {
  // Very small positive duration
  EXPECT_EQ(fmt::format("{}", std::chrono::nanoseconds(1)), "0.000000001s");
  // Very small negative duration
  EXPECT_EQ(fmt::format("{}", std::chrono::nanoseconds(-1)), "-0.000000001s");
  // Duration exactly 0
  EXPECT_EQ(fmt::format("{}", std::chrono::nanoseconds(0)), "0s");
  EXPECT_EQ(fmt::format("{}", std::chrono::milliseconds(0)), "0s");
  EXPECT_EQ(fmt::format("{}", std::chrono::duration<double>(0.0)), "0s");

  // Duration just under a second
  EXPECT_EQ(fmt::format("{}", std::chrono::milliseconds(999)), "0.999s");
  EXPECT_EQ(fmt::format("{}", std::chrono::microseconds(999999)), "0.999999s");
  EXPECT_EQ(fmt::format("{}", std::chrono::nanoseconds(999999999)), "0.999999999s");

  // Duration just over a second
  EXPECT_EQ(fmt::format("{}", std::chrono::milliseconds(1001)), "1.001s");
  EXPECT_EQ(fmt::format("{}", std::chrono::microseconds(1000001)), "1.000001s");
  EXPECT_EQ(fmt::format("{}", std::chrono::nanoseconds(1000000001)), "1.000000001s");

  // Large number of fractional digits (beyond standard chrono units)
  // Using a custom duration type with a very small period
  using picoseconds = std::chrono::duration<long long, std::ratio<1, 1000000000000>>;
  EXPECT_EQ(fmt::format("{}", picoseconds(1234567890123LL)), "1.234567890123s");
  EXPECT_EQ(fmt::format("{}", picoseconds(123456789012LL)), "0.123456789012s");
  EXPECT_EQ(fmt::format("{}", picoseconds(1LL)), "0.000000000001s");

  // Test with a large integer second component and fractional part
  EXPECT_EQ(fmt::format("{}", std::chrono::duration<double>(12345.6789)), "12345.6789s");
  EXPECT_EQ(fmt::format("{}", std::chrono::duration<double>(-12345.6789)), "-12345.6789s");
}

// Test with different precision specifiers (though the focal function itself doesn't directly
// handle precision, the overall formatting does, and it influences how fractional seconds are written).
TEST(ChronoDurationTest, PrecisionSpecifiers) {
  // Default precision for double is usually 6 decimal places, but fmt might adjust.
  // The focal function writes all available fractional digits.
  EXPECT_EQ(fmt::format("{:.3}", std::chrono::duration<double>(1.234567)), "1.235s"); // Rounds
  EXPECT_EQ(fmt::format("{:.0}", std::chrono::duration<double>(1.234567)), "1s"); // No fractional part
  EXPECT_EQ(fmt::format("{:.9}", std::chrono::duration<double>(1.234567)), "1.234567000s"); // Pads with zeros
  EXPECT_EQ(fmt::format("{:.12}", std::chrono::nanoseconds(1234567890)), "1.234567890000s");
  EXPECT_EQ(fmt::format("{:.12}", std::chrono::duration<double>(1.234567890123456)), "1.234567890123s");
}

// Test with custom duration types that are not standard chrono units
TEST(ChronoDurationTest, CustomDurations) {
  // Duration in 1/10th of a second
  using deciseconds = std::chrono::duration<int, std::ratio<1, 10>>;
  EXPECT_EQ(fmt::format("{}", deciseconds(12)), "1.2s");
  EXPECT_EQ(fmt::format("{}", deciseconds(5)), "0.5s");
  EXPECT_EQ(fmt::format("{}", deciseconds(0)), "0s");
  EXPECT_EQ(fmt::format("{}", deciseconds(-12)), "-1.2s");

  // Duration in 1/10000th of a second
  using ten_thousandths_of_a_second = std::chrono::duration<long long, std::ratio<1, 10000>>;
  EXPECT_EQ(fmt::format("{}", ten_thousandths_of_a_second(12345)), "1.2345s");
  EXPECT_EQ(fmt::format("{}", ten_thousandths_of_a_second(5000)), "0.5s");
  EXPECT_EQ(fmt::format("{}", ten_thousandths_of_a_second(1)), "0.0001s");
  EXPECT_EQ(fmt::format("{}", ten_thousandths_of_a_second(0)), "0s");
}

// Test with different character types (char_type in the focal function)
TEST(ChronoDurationTest, WideCharacterFormatting) {
  // This tests the `write` and `write_fractional_seconds` with `wchar_t`.
  // The focal function uses `char_type` which is templated.
  EXPECT_EQ(fmt::format(L"{}", std::chrono::duration<double>(1.234)), L"1.234s");
  EXPECT_EQ(fmt::format(L"{}", std::chrono::milliseconds(500)), L"0.5s");
  EXPECT_EQ(fmt::format(L"{}", std::chrono::nanoseconds(123456789)), L"0.123456789s");
}

// Test with maximum and minimum representable values for integer durations
TEST(ChronoDurationTest, MinMaxValues) {
  // For milliseconds, max value will be a very large number of seconds
  EXPECT_EQ(fmt::format("{}", std::chrono::milliseconds(std::numeric_limits<long long>::max())),
            fmt::format("{}s", static_cast<double>(std::numeric_limits<long long>::max()) / 1000.0));
  EXPECT_EQ(fmt::format("{}", std::chrono::milliseconds(std::numeric_limits<long long>::min())),
            fmt::format("{}s", static_cast<double>(std::numeric_limits<long long>::min()) / 1000.0));

  // For nanoseconds, max value will be a very large number of seconds
  EXPECT_EQ(fmt::format("{}", std::chrono::nanoseconds(std::numeric_limits<long long>::max())),
            fmt::format("{}s", static_cast<double>(std::numeric_limits<long long>::max()) / 1000000000.0));
  EXPECT_EQ(fmt::format("{}", std::chrono::nanoseconds(std::numeric_limits<long long>::min())),
            fmt::format("{}s", static_cast<double>(std::numeric_limits<long long>::min()) / 1000000000.0));
}