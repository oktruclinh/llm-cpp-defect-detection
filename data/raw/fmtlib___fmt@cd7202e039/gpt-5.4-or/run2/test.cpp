#include <gtest/gtest.h>

#include <chrono>
#include <string>

#include <fmt/chrono.h>
#include <fmt/format.h>

namespace {

using namespace std::chrono;

TEST(FmtChronoDurationFormattingTest, FormatsWholeSecondsWithoutFraction) {
  EXPECT_EQ(fmt::format("{}", 0s), "0s");
  EXPECT_EQ(fmt::format("{}", 1s), "1s");
  EXPECT_EQ(fmt::format("{}", 42s), "42s");
}

TEST(FmtChronoDurationFormattingTest, FormatsMillisecondsAsFractionalSeconds) {
  EXPECT_EQ(fmt::format("{}", 1ms), "0.001s");
  EXPECT_EQ(fmt::format("{}", 12ms), "0.012s");
  EXPECT_EQ(fmt::format("{}", 345ms), "0.345s");
  EXPECT_EQ(fmt::format("{}", 999ms), "0.999s");
}

TEST(FmtChronoDurationFormattingTest, FormatsMicrosecondsAsFractionalSecondsWithZeroPadding) {
  EXPECT_EQ(fmt::format("{}", 1us), "0.000001s");
  EXPECT_EQ(fmt::format("{}", 12us), "0.000012s");
  EXPECT_EQ(fmt::format("{}", 123us), "0.000123s");
  EXPECT_EQ(fmt::format("{}", 999999us), "0.999999s");
}

TEST(FmtChronoDurationFormattingTest, FormatsNanosecondsAsFractionalSecondsWithZeroPadding) {
  EXPECT_EQ(fmt::format("{}", 1ns), "0.000000001s");
  EXPECT_EQ(fmt::format("{}", 12ns), "0.000000012s");
  EXPECT_EQ(fmt::format("{}", 123456789ns), "0.123456789s");
  EXPECT_EQ(fmt::format("{}", 999999999ns), "0.999999999s");
}

TEST(FmtChronoDurationFormattingTest, FormatsSubsecondValuesWithIntegralSecondPart) {
  EXPECT_EQ(fmt::format("{}", 1001ms), "1.001s");
  EXPECT_EQ(fmt::format("{}", 1010ms), "1.010s");
  EXPECT_EQ(fmt::format("{}", 1100ms), "1.100s");
  EXPECT_EQ(fmt::format("{}", 1234ms), "1.234s");

  EXPECT_EQ(fmt::format("{}", 1000001us), "1.000001s");
  EXPECT_EQ(fmt::format("{}", 1200000us), "1.200000s");
  EXPECT_EQ(fmt::format("{}", 1234567us), "1.234567s");

  EXPECT_EQ(fmt::format("{}", 1000000001ns), "1.000000001s");
  EXPECT_EQ(fmt::format("{}", 1234567890ns), "1.234567890s");
}

TEST(FmtChronoDurationFormattingTest, FormatsNegativeFractionalSeconds) {
  EXPECT_EQ(fmt::format("{}", -1ms), "-0.001s");
  EXPECT_EQ(fmt::format("{}", -12us), "-0.000012s");
  EXPECT_EQ(fmt::format("{}", -123456789ns), "-0.123456789s");
}

TEST(FmtChronoDurationFormattingTest, FormatsNegativeValuesWithSecondAndFractionalParts) {
  EXPECT_EQ(fmt::format("{}", -1001ms), "-1.001s");
  EXPECT_EQ(fmt::format("{}", -1234ms), "-1.234s");
  EXPECT_EQ(fmt::format("{}", -1000001us), "-1.000001s");
  EXPECT_EQ(fmt::format("{}", -1234567890ns), "-1.234567890s");
}

TEST(FmtChronoDurationFormattingTest, ZeroSubsecondDurationsKeepExpectedPrecision) {
  EXPECT_EQ(fmt::format("{}", milliseconds{0}), "0ms");
  EXPECT_EQ(fmt::format("{}", microseconds{0}), "0us");
  EXPECT_EQ(fmt::format("{}", nanoseconds{0}), "0ns");
}

TEST(FmtChronoDurationFormattingTest, DurationSpecUsesChronoFormatterAndEmitsFractionalSeconds) {
  EXPECT_EQ(fmt::format("{:%S}", 1ms), "00.001");
  EXPECT_EQ(fmt::format("{:%S}", 12ms), "00.012");
  EXPECT_EQ(fmt::format("{:%S}", 1us), "00.000001");
  EXPECT_EQ(fmt::format("{:%S}", 123456789ns), "00.123456789");
}

TEST(FmtChronoDurationFormattingTest, DurationSpecPadsIntegralSecondsToTwoDigitsBeforeFraction) {
  EXPECT_EQ(fmt::format("{:%S}", 1s + 1ms), "01.001");
  EXPECT_EQ(fmt::format("{:%S}", 9s + 12us), "09.000012");
  EXPECT_EQ(fmt::format("{:%S}", 10s + 345ms), "10.345");
  EXPECT_EQ(fmt::format("{:%S}", 59s + 999999999ns), "59.999999999");
}

TEST(FmtChronoDurationFormattingTest, DurationSpecWithOnlyWholeSecondsHasNoFraction) {
  EXPECT_EQ(fmt::format("{:%S}", 0s), "00");
  EXPECT_EQ(fmt::format("{:%S}", 7s), "07");
  EXPECT_EQ(fmt::format("{:%S}", 42s), "42");
}

TEST(FmtChronoDurationFormattingTest, DurationSpecHandlesZeroFractionalPartPrecisely) {
  EXPECT_EQ(fmt::format("{:%S}", 0ms), "00.000");
  EXPECT_EQ(fmt::format("{:%S}", 0us), "00.000000");
  EXPECT_EQ(fmt::format("{:%S}", 0ns), "00.000000000");
}

TEST(FmtChronoDurationFormattingTest, DurationSpecRejectsNegativeDurations) {
  EXPECT_THROW((void)fmt::format("{:%S}", -1ms), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:%S}", -1s), fmt::format_error);
}

TEST(FmtChronoDurationFormattingTest, DurationSpecRejectsSecondFieldOutsideValidRange) {
  EXPECT_THROW((void)fmt::format("{:%S}", 60s), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:%S}", 61s + 1ms), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:%S}", 120s), fmt::format_error);
}

}  // namespace