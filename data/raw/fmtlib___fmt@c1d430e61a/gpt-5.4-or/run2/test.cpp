#include <gtest/gtest.h>

#include <chrono>
#include <string>

#include <fmt/chrono.h>
#include <fmt/format.h>

namespace {

using namespace std::chrono;

TEST(FmtChronoFormatterDurationTest, ZeroDurationHasNoNegativeSignAndZeroFraction) {
  EXPECT_EQ(fmt::format("{}", milliseconds(0)), "0ms");
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(0)), "00");
  EXPECT_EQ(fmt::format("{:%S}", duration<int>(0)), "00");
}

TEST(FmtChronoFormatterDurationTest, PositiveSubsecondDurationFormatsNormally) {
  EXPECT_EQ(fmt::format("{}", milliseconds(123)), "123ms");
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(123)), "00.123");
  EXPECT_EQ(fmt::format("{:%S}", microseconds(123000)), "00.123");
}

TEST(FmtChronoFormatterDurationTest, NegativeSubsecondDurationPlacesMinusBeforeWholeValue) {
  EXPECT_EQ(fmt::format("{}", milliseconds(-123)), "-123ms");
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(-123)), "-00.123");
  EXPECT_EQ(fmt::format("{:%S}", microseconds(-123000)), "-00.123");
}

TEST(FmtChronoFormatterDurationTest, PositiveMultiSecondDurationSplitsSecondsAndMillisecondsCorrectly) {
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(12345)), "12.345");
  EXPECT_EQ(fmt::format("{:%M:%S}", milliseconds(62345)), "01:02.345");
}

TEST(FmtChronoFormatterDurationTest, NegativeMultiSecondDurationPlacesSingleLeadingMinusAndUsesAbsoluteComponents) {
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(-12345)), "-12.345");
  EXPECT_EQ(fmt::format("{:%M:%S}", milliseconds(-62345)), "-01:02.345");
}

TEST(FmtChronoFormatterDurationTest, ExactSecondNegativeDurationDoesNotShowNegativeFraction) {
  EXPECT_EQ(fmt::format("{:%S}", seconds(-1)), "-01");
  EXPECT_EQ(fmt::format("{:%M:%S}", seconds(-61)), "-01:01");
}

TEST(FmtChronoFormatterDurationTest, NegativeDurationNearBoundaryUsesAbsoluteRemainder) {
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(-1001)), "-01.001");
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(-1999)), "-01.999");
  EXPECT_EQ(fmt::format("{:%M:%S}", milliseconds(-60001)), "-01:00.001");
}

TEST(FmtChronoFormatterDurationTest, PositiveDurationNearBoundaryUsesExpectedRemainder) {
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(999)), "00.999");
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(1000)), "01");
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(1001)), "01.001");
}

TEST(FmtChronoFormatterDurationTest, NegativeZeroLikeFloatingDurationDoesNotPrintMinus) {
  EXPECT_EQ(fmt::format("{:%S}", duration<double>(-0.0)), "00");
}

TEST(FmtChronoFormatterDurationTest, NegativeFloatingSubsecondDurationPrintsMinusAndAbsoluteFraction) {
  EXPECT_EQ(fmt::format("{:%S}", duration<double>(-0.25)), "-00.250");
  EXPECT_EQ(fmt::format("{:%S}", duration<double>(-1.25)), "-01.250");
}

TEST(FmtChronoFormatterDurationTest, NegativeMinutesAndSecondsFromDurationUseSingleLeadingMinus) {
  EXPECT_EQ(fmt::format("{:%H:%M:%S}", seconds(-3661)), "-01:01:01");
  EXPECT_EQ(fmt::format("{:%H:%M:%S}", milliseconds(-3661123)), "-01:01:01.123");
}

TEST(FmtChronoFormatterDurationTest, DifferentPeriodsStillRespectSignNormalization) {
  EXPECT_EQ(fmt::format("{:%S}", duration<int, std::ratio<60>>(-1)), "-00");
  EXPECT_EQ(fmt::format("{:%M:%S}", duration<int, std::ratio<60>>(-1)), "-01:00");
  EXPECT_EQ(fmt::format("{:%S}", duration<int, std::ratio<1, 10>>(-15)), "-01.500");
}

}  // namespace