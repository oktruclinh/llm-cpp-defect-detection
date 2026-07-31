#include <gtest/gtest.h>
#include "fmt/chrono.h"
#include "fmt/format.h"

#include <chrono>
#include <string>
#include <vector>

// Helper function to format a duration and return the string
template <typename Duration>
std::string format_duration(const Duration& d, fmt::string_view format_str = "{:%S.%F}") {
  return fmt::format(format_str, d);
}

TEST(ChronoFormatTest, SecondAndFractionalSecondsNormal) {
  // Test with milliseconds
  std::chrono::milliseconds ms(12345); // 12 seconds, 345 milliseconds
  EXPECT_EQ(format_duration(ms), "12.345");

  // Test with microseconds
  std::chrono::microseconds us(6789123); // 6 seconds, 789123 microseconds
  EXPECT_EQ(format_duration(us), "06.789123");

  // Test with nanoseconds
  std::chrono::nanoseconds ns(123456789); // 0 seconds, 123456789 nanoseconds
  EXPECT_EQ(format_duration(ns), "00.123456789");

  // Test with a custom duration type (e.g., 1/100th of a second)
  using centiseconds = std::chrono::duration<long long, std::ratio<1, 100>>;
  centiseconds cs(54321); // 543 seconds, 21 centiseconds
  EXPECT_EQ(format_duration(cs), "03.21"); // 543 % 60 = 3 seconds, 21 centiseconds

  // Test with a duration that has a large number of fractional digits
  using picoseconds = std::chrono::duration<long long, std::ratio<1, 1'000'000'000'000>>;
  picoseconds ps(1234567890123); // 1 second, 234567890123 picoseconds
  EXPECT_EQ(format_duration(ps), "01.234567890123");
}

TEST(ChronoFormatTest, SecondAndFractionalSecondsEdgeCases) {
  // Zero duration
  std::chrono::seconds s_zero(0);
  EXPECT_EQ(format_duration(s_zero), "00.000"); // Default precision for %F

  std::chrono::milliseconds ms_zero(0);
  EXPECT_EQ(format_duration(ms_zero), "00.000");

  // Duration less than a second
  std::chrono::milliseconds ms_less_than_sec(500);
  EXPECT_EQ(format_duration(ms_less_than_sec), "00.500");

  std::chrono::microseconds us_less_than_sec(123);
  EXPECT_EQ(format_duration(us_less_than_sec), "00.000123");

  // Duration exactly one second
  std::chrono::seconds s_one(1);
  EXPECT_EQ(format_duration(s_one), "01.000");

  // Duration with no fractional part
  std::chrono::seconds s_full(10);
  EXPECT_EQ(format_duration(s_full), "10.000");

  // Duration with fractional part that is a multiple of the base unit
  std::chrono::milliseconds ms_exact_sec(2000); // 2 seconds
  EXPECT_EQ(format_duration(ms_exact_sec), "02.000");

  // Duration with fractional part that ends in zero
  std::chrono::milliseconds ms_trailing_zero(1200); // 1 second, 200 milliseconds
  EXPECT_EQ(format_duration(ms_trailing_zero), "01.200");

  std::chrono::microseconds us_trailing_zero(1230000); // 1 second, 230000 microseconds
  EXPECT_EQ(format_duration(us_trailing_zero), "01.230000");

  // Large duration (seconds part wraps around 60)
  std::chrono::seconds s_large(125); // 2 minutes, 5 seconds
  EXPECT_EQ(format_duration(s_large), "05.000"); // %S only prints seconds part

  // Negative duration (should still show positive seconds and fractional part)
  std::chrono::milliseconds ms_negative(-12345); // -12 seconds, -345 milliseconds
  // fmtlib's chrono formatting for durations typically treats them as absolute for %S and %F
  // The sign is usually handled by %H, %M, etc. or by the overall duration formatting.
  // For %S.%F, it's the absolute value of the second and fractional part.
  EXPECT_EQ(format_duration(ms_negative), "12.345");

  std::chrono::microseconds us_negative(-500);
  EXPECT_EQ(format_duration(us_negative), "00.000500");
}

TEST(ChronoFormatTest, SecondAndFractionalSecondsPrecision) {
  std::chrono::microseconds us(1234567); // 1 second, 234567 microseconds

  // Default precision for %F (microseconds)
  EXPECT_EQ(format_duration(us, "{:%S.%F}"), "01.234567");

  // Precision 0 (no fractional part)
  EXPECT_EQ(format_duration(us, "{:%S.%F0}"), "01."); // Note: fmtlib behavior for F0 is to print '.'

  // Precision 1
  EXPECT_EQ(format_duration(us, "{:%S.%F1}"), "01.2");

  // Precision 3
  EXPECT_EQ(format_duration(us, "{:%S.%F3}"), "01.234");

  // Precision 6
  EXPECT_EQ(format_duration(us, "{:%S.%F6}"), "01.234567");

  // Precision greater than available (should pad with zeros)
  EXPECT_EQ(format_duration(us, "{:%S.%F9}"), "01.234567000");

  // Precision with milliseconds
  std::chrono::milliseconds ms(123);
  EXPECT_EQ(format_duration(ms, "{:%S.%F1}"), "00.1");
  EXPECT_EQ(format_duration(ms, "{:%S.%F3}"), "00.123");
  EXPECT_EQ(format_duration(ms, "{:%S.%F5}"), "00.12300");

  // Precision with nanoseconds
  std::chrono::nanoseconds ns(123456789);
  EXPECT_EQ(format_duration(ns, "{:%S.%F3}"), "00.123");
  EXPECT_EQ(format_duration(ns, "{:%S.%F9}"), "00.123456789");
  EXPECT_EQ(format_duration(ns, "{:%S.%F12}"), "00.123456789000");
}

TEST(ChronoFormatTest, SecondAndFractionalSecondsCustomPeriod) {
  // Custom duration with a period of 1/10th of a second
  using deciseconds = std::chrono::duration<long long, std::ratio<1, 10>>;
  deciseconds ds(123); // 12 seconds, 3 deciseconds
  EXPECT_EQ(format_duration(ds), "12.3");
  EXPECT_EQ(format_duration(ds, "{:%S.%F2}"), "12.30");

  // Custom duration with a period of 1/10000th of a second
  using ten_thousandths_of_a_second = std::chrono::duration<long long, std::ratio<1, 10000>>;
  ten_thousandths_of_a_second tts(45678); // 4 seconds, 5678 ten_thousandths
  EXPECT_EQ(format_duration(tts), "04.5678");
  EXPECT_EQ(format_duration(tts, "{:%S.%F2}"), "04.56");
  EXPECT_EQ(format_duration(tts, "{:%S.%F6}"), "04.567800");

  // Custom duration with a period of 1/2 of a second (not a power of 10)
  using half_seconds = std::chrono::duration<long long, std::ratio<1, 2>>;
  half_seconds hs(5); // 2.5 seconds
  // fmtlib converts to common_type for fractional seconds, which for half_seconds
  // and std::chrono::seconds would be std::chrono::seconds.
  // The fractional part is then calculated based on the common_type.
  // 5 half_seconds = 2.5 seconds.
  // The fractional part is 0.5 seconds.
  EXPECT_EQ(format_duration(hs), "02.500"); // Default precision for %F is milliseconds

  half_seconds hs_fractional(1); // 0.5 seconds
  EXPECT_EQ(format_duration(hs_fractional), "00.500");

  // Custom duration with a period of 1/3 of a second (non-terminating decimal)
  using third_seconds = std::chrono::duration<long long, std::ratio<1, 3>>;
  third_seconds ts(7); // 2 and 1/3 seconds
  // The fractional part will be approximated.
  // 7/3 seconds = 2.333... seconds
  EXPECT_EQ(format_duration(ts, "{:%S.%F3}"), "02.333");
  EXPECT_EQ(format_duration(ts, "{:%S.%F6}"), "02.333333");
}

TEST(ChronoFormatTest, SecondAndFractionalSecondsMixedFormat) {
  std::chrono::milliseconds ms(12345); // 12 seconds, 345 milliseconds

  // Combined with other format specifiers
  EXPECT_EQ(fmt::format("{:%M:%S.%F}", ms), "00:12.345");

  std::chrono::microseconds us(6789123); // 6 seconds, 789123 microseconds
  EXPECT_EQ(fmt::format("{:%H:%M:%S.%F}", us), "00:00:06.789123");

  // With padding/width
  EXPECT_EQ(fmt::format("{:%S.%F8}", ms), "12.34500000");
  EXPECT_EQ(fmt::format("{:%S.%F}", std::chrono::seconds(5)), "05.000");
}