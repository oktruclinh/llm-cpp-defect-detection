#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <fmt/core.h>

#include <chrono>
#include <string>
#include <vector>

// Helper function to format a duration using fmt::format and capture the output.
template <typename Rep, typename Period>
std::string format_duration(std::chrono::duration<Rep, Period> d) {
  return fmt::format("{}", d);
}

TEST(ChronoFormatterTest, PositiveDuration) {
  // Test with a simple positive duration
  std::chrono::seconds s(123);
  EXPECT_EQ(format_duration(s), "123s");

  // Test with milliseconds
  std::chrono::milliseconds ms(456);
  EXPECT_EQ(format_duration(ms), "456ms");

  // Test with microseconds
  std::chrono::microseconds us(789);
  EXPECT_EQ(format_duration(us), "789µs");

  // Test with nanoseconds
  std::chrono::nanoseconds ns(1011);
  EXPECT_EQ(format_duration(ns), "1011ns");

  // Test with minutes
  std::chrono::minutes m(5);
  EXPECT_EQ(format_duration(m), "5min");

  // Test with hours
  std::chrono::hours h(2);
  EXPECT_EQ(format_duration(h), "2h");
}

TEST(ChronoFormatterTest, ZeroDuration) {
  // Test with zero duration
  std::chrono::seconds s(0);
  EXPECT_EQ(format_duration(s), "0s");

  std::chrono::milliseconds ms(0);
  EXPECT_EQ(format_duration(ms), "0ms");
}

TEST(ChronoFormatterTest, NegativeDuration) {
  // Test with a simple negative duration
  std::chrono::seconds s(-123);
  EXPECT_EQ(format_duration(s), "-123s");

  // Test with negative milliseconds
  std::chrono::milliseconds ms(-456);
  EXPECT_EQ(format_duration(ms), "-456ms");

  // Test with negative microseconds
  std::chrono::microseconds us(-789);
  EXPECT_EQ(format_duration(us), "-789µs");

  // Test with negative nanoseconds
  std::chrono::nanoseconds ns(-1011);
  EXPECT_EQ(format_duration(ns), "-1011ns");

  // Test with negative minutes
  std::chrono::minutes m(-5);
  EXPECT_EQ(format_duration(m), "-5min");

  // Test with negative hours
  std::chrono::hours h(-2);
  EXPECT_EQ(format_duration(h), "-2h");
}

TEST(ChronoFormatterTest, MixedUnitsDuration) {
  // The chrono_formatter constructor specifically handles seconds and milliseconds
  // when the duration is not directly representable by a single unit.
  // This test verifies the internal logic of the constructor by observing the output.

  // A duration that is 1 second and 500 milliseconds
  std::chrono::milliseconds d1(1500);
  EXPECT_EQ(format_duration(d1), "1s500ms");

  // A duration that is 0 seconds and 500 milliseconds
  std::chrono::milliseconds d2(500);
  EXPECT_EQ(format_duration(d2), "500ms");

  // A duration that is 1 second and 0 milliseconds
  std::chrono::milliseconds d3(1000);
  EXPECT_EQ(format_duration(d3), "1s");

  // A duration that is 1 second and 1 millisecond
  std::chrono::milliseconds d4(1001);
  EXPECT_EQ(format_duration(d4), "1s1ms");

  // Negative mixed duration
  std::chrono::milliseconds d5(-1500);
  EXPECT_EQ(format_duration(d5), "-1s500ms");

  // Negative mixed duration with small millisecond part
  std::chrono::milliseconds d6(-1001);
  EXPECT_EQ(format_duration(d6), "-1s1ms");

  // Duration with a large number of milliseconds that should be converted to seconds
  std::chrono::milliseconds d7(1234567); // 1234 seconds and 567 milliseconds
  EXPECT_EQ(format_duration(d7), "1234s567ms");

  // Duration with a large negative number of milliseconds
  std::chrono::milliseconds d8(-1234567);
  EXPECT_EQ(format_duration(d8), "-1234s567ms");
}

TEST(ChronoFormatterTest, LargeDurationValues) {
  // Test with large positive values
  std::chrono::seconds s_large(std::numeric_limits<long long>::max());
  // The exact string depends on the max value, but it should not crash and be positive.
  // We can't predict the exact string without knowing the max value, but we can check for the sign.
  std::string output_s_large = format_duration(s_large);
  EXPECT_FALSE(output_s_large.empty());
  EXPECT_EQ(output_s_large[0], std::to_string(std::numeric_limits<long long>::max())[0]); // Should start with the first digit of the number

  // Test with large negative values
  std::chrono::seconds s_min(std::numeric_limits<long long>::min());
  std::string output_s_min = format_duration(s_min);
  EXPECT_FALSE(output_s_min.empty());
  EXPECT_EQ(output_s_min[0], '-');
}

TEST(ChronoFormatterTest, CustomRatioDuration) {
  // Define a custom duration unit (e.g., 1/100th of a second)
  using centiseconds = std::chrono::duration<long long, std::ratio<1, 100>>;

  centiseconds cs(123); // 1.23 seconds
  // fmtlib's default formatting for custom ratios might not be as specific as s/ms
  // unless explicitly handled. It usually prints the count and the ratio.
  // The chrono_formatter constructor's logic for s/ms is specific to std::chrono units.
  // For custom units, it will likely just print the count and the unit.
  EXPECT_EQ(format_duration(cs), "123[1/100]s");

  centiseconds neg_cs(-456);
  EXPECT_EQ(format_duration(neg_cs), "-456[1/100]s");
}

// Test with different underlying representation types
TEST(ChronoFormatterTest, DifferentRepTypes) {
  std::chrono::duration<int, std::ratio<1, 1>> s_int(10);
  EXPECT_EQ(format_duration(s_int), "10s");

  std::chrono::duration<double, std::ratio<1, 1>> s_double(10.5);
  // fmtlib truncates floating point durations to integer for seconds/milliseconds
  // in the chrono_formatter constructor's logic.
  // The `val` member stores `d.count()`, which for double will be 10.5.
  // However, `s = std::chrono::duration_cast<seconds>(d)` will truncate.
  // The output should reflect the truncation.
  EXPECT_EQ(format_duration(s_double), "10s500ms"); // 10.5 seconds = 10s 500ms

  std::chrono::duration<double, std::milli> ms_double(10.5);
  EXPECT_EQ(format_duration(ms_double), "10ms"); // Truncates to 10ms
}

// Test to ensure the negative sign is handled correctly when the duration is very small
// and would otherwise be formatted as 0s.
TEST(ChronoFormatterTest, NegativeSmallDuration) {
  std::chrono::nanoseconds ns(-1);
  EXPECT_EQ(format_duration(ns), "-1ns");

  std::chrono::microseconds us(-1);
  EXPECT_EQ(format_duration(us), "-1µs");

  std::chrono::milliseconds ms(-1);
  EXPECT_EQ(format_duration(ms), "-1ms");
}

// Test to ensure the negative sign is applied only once.
TEST(ChronoFormatterTest, DoubleNegativeSignPrevention) {
  // This test implicitly checks the `*out++ = '-';` line.
  // If `d.count()` is negative, it negates `d` and prints '-'.
  // The subsequent formatting should then treat `d` as positive.
  std::chrono::milliseconds d(-1234);
  EXPECT_EQ(format_duration(d), "-1s234ms");

  // If the internal logic were to re-evaluate the sign after `d = -d;`,
  // it might incorrectly produce "--1s234ms" or similar.
  // The current implementation correctly handles this by only adding '-' once.
}