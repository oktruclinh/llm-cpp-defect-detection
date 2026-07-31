#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <fmt/core.h>

#include <chrono>
#include <string>
#include <vector>

// Helper function to format a duration and return the string.
// This simulates how fmt::chrono_formatter would be used internally
// by fmt::format.
template <typename Rep, typename Period>
std::string format_duration_internal(std::chrono::duration<Rep, Period> d) {
  fmt::memory_buffer buf;
  fmt::format_context ctx(buf);
  fmt::detail::chrono_formatter<decltype(ctx), decltype(buf.data()), Rep, Period> formatter(ctx, buf.data(), d);
  // The chrono_formatter constructor performs the initial formatting for negative sign and duration decomposition.
  // We don't need to call format() explicitly for this test, as we are testing the constructor's side effects.
  // However, to make it a complete test, we'll just return the buffer content.
  // In a real scenario, format() would be called later to handle specifiers.
  return fmt::to_string(buf);
}

TEST(ChronoFormatterTest, PositiveDuration) {
  // Test with a positive duration
  std::chrono::seconds d(123);
  std::string result = format_duration_internal(d);
  // The constructor should not add a '-' for positive durations.
  // The actual number formatting happens later, so we expect an empty string
  // or just the initial buffer content without the sign.
  // Since the constructor only handles the negative sign, and then stores the absolute value,
  // the buffer should be empty for positive values.
  EXPECT_EQ("", result);
}

TEST(ChronoFormatterTest, ZeroDuration) {
  // Test with a zero duration
  std::chrono::seconds d(0);
  std::string result = format_duration_internal(d);
  // Zero duration should not have a negative sign.
  EXPECT_EQ("", result);
}

TEST(ChronoFormatterTest, NegativeDuration) {
  // Test with a negative duration
  std::chrono::seconds d(-456);
  std::string result = format_duration_internal(d);
  // The constructor should add a '-' to the output buffer.
  EXPECT_EQ("-", result);
}

TEST(ChronoFormatterTest, NegativeDurationWithMilliseconds) {
  // Test with a negative duration that has fractional seconds
  std::chrono::milliseconds d(-12345); // -12 seconds, -345 milliseconds
  std::string result = format_duration_internal(d);
  // The constructor should add a '-' to the output buffer.
  EXPECT_EQ("-", result);
}

TEST(ChronoFormatterTest, LargeNegativeDuration) {
  // Test with a large negative duration
  std::chrono::hours d(-100);
  std::string result = format_duration_internal(d);
  EXPECT_EQ("-", result);
}

TEST(ChronoFormatterTest, SmallNegativeDuration) {
  // Test with a very small negative duration (e.g., nanoseconds)
  std::chrono::nanoseconds d(-1);
  std::string result = format_duration_internal(d);
  EXPECT_EQ("-", result);
}

// Test the internal decomposition of duration into seconds and milliseconds
// This requires accessing the private members of chrono_formatter,
// which is generally discouraged for public API testing.
// However, since the prompt specifically asks to test the constructor's logic
// which includes `s = ...` and `ms = ...`, we'll use a friend class or
// a modified helper to expose these for testing.
// For this example, we'll create a test-specific wrapper that exposes these.

template <typename Rep, typename Period>
struct TestChronoFormatter {
  fmt::detail::chrono_formatter<fmt::format_context, char*, Rep, Period> formatter;
  fmt::memory_buffer buf;

  TestChronoFormatter(std::chrono::duration<Rep, Period> d)
      : formatter(fmt::format_context(buf), buf.data(), d) {}

  std::chrono::seconds get_s() const { return formatter.s; }
  std::chrono::milliseconds get_ms() const { return formatter.ms; }
  std::string get_output_sign() const { return fmt::to_string(buf); }
};

TEST(ChronoFormatterDecompositionTest, PositiveDurationDecomposition) {
  std::chrono::milliseconds d(12345); // 12 seconds, 345 milliseconds
  TestChronoFormatter<long long, std::milli> test_formatter(d);

  EXPECT_EQ(std::chrono::seconds(12), test_formatter.get_s());
  EXPECT_EQ(std::chrono::milliseconds(345), test_formatter.get_ms());
  EXPECT_EQ("", test_formatter.get_output_sign());
}

TEST(ChronoFormatterDecompositionTest, NegativeDurationDecomposition) {
  std::chrono::milliseconds d(-12345); // -12 seconds, -345 milliseconds
  TestChronoFormatter<long long, std::milli> test_formatter(d);

  // The constructor takes the absolute value for s and ms.
  EXPECT_EQ(std::chrono::seconds(12), test_formatter.get_s());
  EXPECT_EQ(std::chrono::milliseconds(345), test_formatter.get_ms());
  EXPECT_EQ("-", test_formatter.get_output_sign());
}

TEST(ChronoFormatterDecompositionTest, ZeroDurationDecomposition) {
  std::chrono::milliseconds d(0);
  TestChronoFormatter<long long, std::milli> test_formatter(d);

  EXPECT_EQ(std::chrono::seconds(0), test_formatter.get_s());
  EXPECT_EQ(std::chrono::milliseconds(0), test_formatter.get_ms());
  EXPECT_EQ("", test_formatter.get_output_sign());
}

TEST(ChronoFormatterDecompositionTest, DurationLessThanSecond) {
  std::chrono::milliseconds d(500);
  TestChronoFormatter<long long, std::milli> test_formatter(d);

  EXPECT_EQ(std::chrono::seconds(0), test_formatter.get_s());
  EXPECT_EQ(std::chrono::milliseconds(500), test_formatter.get_ms());
  EXPECT_EQ("", test_formatter.get_output_sign());
}

TEST(ChronoFormatterDecompositionTest, NegativeDurationLessThanSecond) {
  std::chrono::milliseconds d(-500);
  TestChronoFormatter<long long, std::milli> test_formatter(d);

  EXPECT_EQ(std::chrono::seconds(0), test_formatter.get_s());
  EXPECT_EQ(std::chrono::milliseconds(500), test_formatter.get_ms());
  EXPECT_EQ("-", test_formatter.get_output_sign());
}

TEST(ChronoFormatterDecompositionTest, DurationWithOnlySeconds) {
  std::chrono::seconds d(10);
  TestChronoFormatter<long long, std::ratio<1>> test_formatter(d);

  EXPECT_EQ(std::chrono::seconds(10), test_formatter.get_s());
  EXPECT_EQ(std::chrono::milliseconds(0), test_formatter.get_ms());
  EXPECT_EQ("", test_formatter.get_output_sign());
}

TEST(ChronoFormatterDecompositionTest, NegativeDurationWithOnlySeconds) {
  std::chrono::seconds d(-10);
  TestChronoFormatter<long long, std::ratio<1>> test_formatter(d);

  EXPECT_EQ(std::chrono::seconds(10), test_formatter.get_s());
  EXPECT_EQ(std::chrono::milliseconds(0), test_formatter.get_ms());
  EXPECT_EQ("-", test_formatter.get_output_sign());
}

TEST(ChronoFormatterDecompositionTest, DurationWithDifferentPeriod) {
  std::chrono::microseconds d(1234567); // 1 second, 234 milliseconds, 567 microseconds
  TestChronoFormatter<long long, std::micro> test_formatter(d);

  EXPECT_EQ(std::chrono::seconds(1), test_formatter.get_s());
  EXPECT_EQ(std::chrono::milliseconds(234), test_formatter.get_ms());
  EXPECT_EQ("", test_formatter.get_output_sign());
}

TEST(ChronoFormatterDecompositionTest, NegativeDurationWithDifferentPeriod) {
  std::chrono::microseconds d(-1234567); // -1 second, -234 milliseconds, -567 microseconds
  TestChronoFormatter<long long, std::micro> test_formatter(d);

  EXPECT_EQ(std::chrono::seconds(1), test_formatter.get_s());
  EXPECT_EQ(std::chrono::milliseconds(234), test_formatter.get_ms());
  EXPECT_EQ("-", test_formatter.get_output_sign());
}

TEST(ChronoFormatterDecompositionTest, LargeDuration) {
  std::chrono::hours d(25); // 25 hours = 90000 seconds
  TestChronoFormatter<long long, std::ratio<3600>> test_formatter(d);

  EXPECT_EQ(std::chrono::seconds(90000), test_formatter.get_s());
  EXPECT_EQ(std::chrono::milliseconds(0), test_formatter.get_ms());
  EXPECT_EQ("", test_formatter.get_output_sign());
}

TEST(ChronoFormatterDecompositionTest, LargeNegativeDuration) {
  std::chrono::hours d(-25); // -25 hours = -90000 seconds
  TestChronoFormatter<long long, std::ratio<3600>> test_formatter(d);

  EXPECT_EQ(std::chrono::seconds(90000), test_formatter.get_s());
  EXPECT_EQ(std::chrono::milliseconds(0), test_formatter.get_ms());
  EXPECT_EQ("-", test_formatter.get_output_sign());
}