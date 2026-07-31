#include <gtest/gtest.h>

#include <chrono>
#include <string>

#include <fmt/chrono.h>
#include <fmt/format.h>

namespace {

using namespace std::chrono;

TEST(FmtChronoFormatterDurationTest, FormatsZeroDurationWithoutNegativeSign) {
  EXPECT_EQ(fmt::format("{}", milliseconds(0)), "0ms");
  EXPECT_EQ(fmt::format("{}", seconds(0)), "0s");
}

TEST(FmtChronoFormatterDurationTest, FormatsPositiveMilliseconds) {
  EXPECT_EQ(fmt::format("{}", milliseconds(123)), "123ms");
  EXPECT_EQ(fmt::format("{}", milliseconds(1)), "1ms");
}

TEST(FmtChronoFormatterDurationTest, FormatsNegativeMillisecondsWithLeadingMinus) {
  EXPECT_EQ(fmt::format("{}", milliseconds(-123)), "-123ms");
  EXPECT_EQ(fmt::format("{}", milliseconds(-1)), "-1ms");
}

TEST(FmtChronoFormatterDurationTest, FormatsPositiveSecondsAndSubseconds) {
  EXPECT_EQ(fmt::format("{}", milliseconds(1234)), "1s 234ms");
  EXPECT_EQ(fmt::format("{}", milliseconds(1001)), "1s 1ms");
  EXPECT_EQ(fmt::format("{}", milliseconds(1999)), "1s 999ms");
}

TEST(FmtChronoFormatterDurationTest, FormatsNegativeSecondsAndSubsecondsWithSingleLeadingMinus) {
  EXPECT_EQ(fmt::format("{}", milliseconds(-1234)), "-1s 234ms");
  EXPECT_EQ(fmt::format("{}", milliseconds(-1001)), "-1s 1ms");
  EXPECT_EQ(fmt::format("{}", milliseconds(-1999)), "-1s 999ms");
}

TEST(FmtChronoFormatterDurationTest, NegativeSubsecondDurationDoesNotProduceNegativeRemainder) {
  std::string formatted = fmt::format("{}", milliseconds(-234));
  EXPECT_EQ(formatted, "-234ms");
  EXPECT_EQ(formatted.find("--"), std::string::npos);
}

TEST(FmtChronoFormatterDurationTest, BoundaryAroundOneSecond) {
  EXPECT_EQ(fmt::format("{}", milliseconds(999)), "999ms");
  EXPECT_EQ(fmt::format("{}", milliseconds(1000)), "1s");
  EXPECT_EQ(fmt::format("{}", milliseconds(1001)), "1s 1ms");

  EXPECT_EQ(fmt::format("{}", milliseconds(-999)), "-999ms");
  EXPECT_EQ(fmt::format("{}", milliseconds(-1000)), "-1s");
  EXPECT_EQ(fmt::format("{}", milliseconds(-1001)), "-1s 1ms");
}

TEST(FmtChronoFormatterDurationTest, FormatsMinutesSecondsAndMillisecondsForNegativeValue) {
  EXPECT_EQ(fmt::format("{}", milliseconds(-62123)), "-1m 2s 123ms");
}

TEST(FmtChronoFormatterDurationTest, FormatsHoursMinutesSecondsAndMillisecondsForNegativeValue) {
  const auto d = hours(-1) - minutes(2) - seconds(3) - milliseconds(4);
  EXPECT_EQ(fmt::format("{}", d), "-1h 2m 3s 4ms");
}

TEST(FmtChronoFormatterDurationTest, NegativeMicrosecondsPreserveMagnitudeAndSingleSign) {
  EXPECT_EQ(fmt::format("{}", microseconds(-1)), "-1us");
  EXPECT_EQ(fmt::format("{}", microseconds(-1001)), "-1001us");
  EXPECT_EQ(fmt::format("{}", microseconds(-1234567)), "-1234567us");
}

TEST(FmtChronoFormatterDurationTest, NegativeNanosecondsPreserveMagnitudeAndSingleSign) {
  EXPECT_EQ(fmt::format("{}", nanoseconds(-1)), "-1ns");
  EXPECT_EQ(fmt::format("{}", nanoseconds(-42)), "-42ns");
}

TEST(FmtChronoFormatterDurationTest, CustomFormatSpecStillUsesCorrectNegativeNormalization) {
  EXPECT_EQ(fmt::format("{:%T}", milliseconds(-1234)), "-00:00:01.234");
  EXPECT_EQ(fmt::format("{:%T}", milliseconds(1234)), "00:00:01.234");
  EXPECT_EQ(fmt::format("{:%T}", milliseconds(-1)), "-00:00:00.001");
}

TEST(FmtChronoFormatterDurationTest, LargeNegativeDurationIsFormattedWithLeadingMinusAndNormalizedFields) {
  const auto d = hours(-25) - minutes(1) - seconds(2) - milliseconds(3);
  EXPECT_EQ(fmt::format("{}", d), "-25h 1m 2s 3ms");
}

}  // namespace