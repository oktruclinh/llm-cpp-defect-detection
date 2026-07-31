#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <fmt/core.h> // For fmt::format_context and fmt::buffer_context

// Helper to create a dummy format context for testing
struct TestFormatContext {
  using char_type = char;
  using iterator = fmt::appender;
  using format_arg = fmt::basic_format_arg<fmt::format_context>;

  fmt::format_context ctx;
  fmt::detail::buffer<char> buffer;

  TestFormatContext() : ctx(fmt::detail::buffer_context<char>(buffer)) {}

  iterator out() { return fmt::appender(buffer); }
};

// Define a custom duration type for testing
template <typename Rep, typename Period = std::ratio<1>>
using CustomDuration = std::chrono::duration<Rep, Period>;

TEST(ChronoFormatterTest, PositiveDuration) {
  TestFormatContext tfc;
  std::string s;
  auto out = std::back_inserter(s);

  // Test with a positive duration
  std::chrono::seconds d(123);
  fmt::detail::chrono_formatter<std::chrono::seconds> formatter(tfc.ctx, out, d);

  // The constructor itself doesn't format, but extracts components.
  // We can inspect the internal state if necessary, but the primary goal
  // is to ensure it handles the sign and component extraction correctly.
  // For this specific constructor, the main effect is the potential '-' prefix.
  // Since it's positive, 's' should be 123 and 'ms' should be 0.
  // The output iterator 'out' should not have been written to yet for positive durations.
  EXPECT_EQ(s, ""); // No '-' should be written for positive duration
  EXPECT_EQ(formatter.s, std::chrono::seconds(123));
  EXPECT_EQ(formatter.ms, std::chrono::milliseconds(0));
}

TEST(ChronoFormatterTest, NegativeDuration) {
  TestFormatContext tfc;
  std::string s;
  auto out = std::back_inserter(s);

  // Test with a negative duration
  std::chrono::milliseconds d(-4567);
  fmt::detail::chrono_formatter<std::chrono::milliseconds> formatter(tfc.ctx, out, d);

  // For negative durations, a '-' should be written to the output.
  // The internal 'val' should store the absolute value.
  // 's' should be 4 seconds, 'ms' should be 567 milliseconds (from the absolute value).
  EXPECT_EQ(s, "-");
  EXPECT_EQ(formatter.s, std::chrono::seconds(4));
  EXPECT_EQ(formatter.ms, std::chrono::milliseconds(567));
}

TEST(ChronoFormatterTest, ZeroDuration) {
  TestFormatContext tfc;
  std::string s;
  auto out = std::back_inserter(s);

  // Test with a zero duration
  std::chrono::minutes d(0);
  fmt::detail::chrono_formatter<std::chrono::minutes> formatter(tfc.ctx, out, d);

  // For zero duration, no '-' should be written.
  // 's' and 'ms' should both be zero.
  EXPECT_EQ(s, "");
  EXPECT_EQ(formatter.s, std::chrono::seconds(0));
  EXPECT_EQ(formatter.ms, std::chrono::milliseconds(0));
}

TEST(ChronoFormatterTest, DurationWithFractionalSeconds) {
  TestFormatContext tfc;
  std::string s;
  auto out = std::back_inserter(s);

  // Test with a duration that has fractional seconds
  std::chrono::duration<double, std::ratio<1>> d(12.345); // 12 seconds, 345 milliseconds
  fmt::detail::chrono_formatter<std::chrono::duration<double, std::ratio<1>>> formatter(tfc.ctx, out, d);

  EXPECT_EQ(s, "");
  EXPECT_EQ(formatter.s, std::chrono::seconds(12));
  EXPECT_EQ(formatter.ms, std::chrono::milliseconds(345));
}

TEST(ChronoFormatterTest, NegativeDurationWithFractionalSeconds) {
  TestFormatContext tfc;
  std::string s;
  auto out = std::back_inserter(s);

  // Test with a negative duration that has fractional seconds
  std::chrono::duration<double, std::ratio<1>> d(-5.678); // -5 seconds, -678 milliseconds
  fmt::detail::chrono_formatter<std::chrono::duration<double, std::ratio<1>>> formatter(tfc.ctx, out, d);

  EXPECT_EQ(s, "-");
  EXPECT_EQ(formatter.s, std::chrono::seconds(5));
  EXPECT_EQ(formatter.ms, std::chrono::milliseconds(678));
}

TEST(ChronoFormatterTest, LargeDuration) {
  TestFormatContext tfc;
  std::string s;
  auto out = std::back_inserter(s);

  // Test with a large duration (e.g., many hours)
  std::chrono::hours d(1234); // 1234 hours = 4442400 seconds
  fmt::detail::chrono_formatter<std::chrono::hours> formatter(tfc.ctx, out, d);

  EXPECT_EQ(s, "");
  EXPECT_EQ(formatter.s, std::chrono::seconds(4442400));
  EXPECT_EQ(formatter.ms, std::chrono::milliseconds(0));
}

TEST(ChronoFormatterTest, SmallestRepresentableDuration) {
  TestFormatContext tfc;
  std::string s;
  auto out = std::back_inserter(s);

  // Test with a very small positive duration (e.g., nanoseconds)
  std::chrono::nanoseconds d(1234567); // 1 millisecond, 234 microseconds, 567 nanoseconds
  fmt::detail::chrono_formatter<std::chrono::nanoseconds> formatter(tfc.ctx, out, d);

  EXPECT_EQ(s, "");
  EXPECT_EQ(formatter.s, std::chrono::seconds(0));
  EXPECT_EQ(formatter.ms, std::chrono::milliseconds(1)); // Only milliseconds are extracted
}

TEST(ChronoFormatterTest, SmallestRepresentableNegativeDuration) {
  TestFormatContext tfc;
  std::string s;
  auto out = std::back_inserter(s);

  // Test with a very small negative duration (e.g., nanoseconds)
  std::chrono::nanoseconds d(-9876543); // -9 milliseconds, -876 microseconds, -543 nanoseconds
  fmt::detail::chrono_formatter<std::chrono::nanoseconds> formatter(tfc.ctx, out, d);

  EXPECT_EQ(s, "-");
  EXPECT_EQ(formatter.s, std::chrono::seconds(0));
  EXPECT_EQ(formatter.ms, std::chrono::milliseconds(9)); // Only milliseconds are extracted from absolute value
}

TEST(ChronoFormatterTest, CustomDurationType) {
  TestFormatContext tfc;
  std::string s;
  auto out = std::back_inserter(s);

  // Test with a custom duration type (e.g., 1/10th of a second)
  CustomDuration<long long, std::ratio<1, 10>> d(25); // 2.5 seconds
  fmt::detail::chrono_formatter<CustomDuration<long long, std::ratio<1, 10>>> formatter(tfc.ctx, out, d);

  EXPECT_EQ(s, "");
  EXPECT_EQ(formatter.s, std::chrono::seconds(2));
  EXPECT_EQ(formatter.ms, std::chrono::milliseconds(500));
}

TEST(ChronoFormatterTest, NegativeCustomDurationType) {
  TestFormatContext tfc;
  std::string s;
  auto out = std::back_inserter(s);

  // Test with a negative custom duration type
  CustomDuration<long long, std::ratio<1, 10>> d(-73); // -7.3 seconds
  fmt::detail::chrono_formatter<CustomDuration<long long, std::ratio<1, 10>>> formatter(tfc.ctx, out, d);

  EXPECT_EQ(s, "-");
  EXPECT_EQ(formatter.s, std::chrono::seconds(7));
  EXPECT_EQ(formatter.ms, std::chrono::milliseconds(300));
}

TEST(ChronoFormatterTest, DurationWithOnlyMilliseconds) {
  TestFormatContext tfc;
  std::string s;
  auto out = std::back_inserter(s);

  std::chrono::milliseconds d(789);
  fmt::detail::chrono_formatter<std::chrono::milliseconds> formatter(tfc.ctx, out, d);

  EXPECT_EQ(s, "");
  EXPECT_EQ(formatter.s, std::chrono::seconds(0));
  EXPECT_EQ(formatter.ms, std::chrono::milliseconds(789));
}

TEST(ChronoFormatterTest, NegativeDurationWithOnlyMilliseconds) {
  TestFormatContext tfc;
  std::string s;
  auto out = std::back_inserter(s);

  std::chrono::milliseconds d(-123);
  fmt::detail::chrono_formatter<std::chrono::milliseconds> formatter(tfc.ctx, out, d);

  EXPECT_EQ(s, "-");
  EXPECT_EQ(formatter.s, std::chrono::seconds(0));
  EXPECT_EQ(formatter.ms, std::chrono::milliseconds(123));
}