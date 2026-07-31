#include <gtest/gtest.h>

#include <chrono>
#include <string>

#include <fmt/chrono.h>
#include <fmt/format.h>

namespace {

using namespace std::chrono;

TEST(FmtChronoFormatterDurationTest, ZeroDurationFormatsWithoutSign) {
  EXPECT_EQ(fmt::format("{}", milliseconds(0)), "0ms");
  EXPECT_EQ(fmt::format("{}", seconds(0)), "0s");
}

TEST(FmtChronoFormatterDurationTest, PositiveMillisecondDurationFormatsNormally) {
  EXPECT_EQ(fmt::format("{}", milliseconds(1234)), "1234ms");
  EXPECT_EQ(fmt::format("{}", milliseconds(1)), "1ms");
}

TEST(FmtChronoFormatterDurationTest, NegativeMillisecondDurationHasSingleLeadingMinus) {
  EXPECT_EQ(fmt::format("{}", milliseconds(-1)), "-1ms");
  EXPECT_EQ(fmt::format("{}", milliseconds(-1234)), "-1234ms");
}

TEST(FmtChronoFormatterDurationTest, NegativeSecondDurationHasSingleLeadingMinus) {
  EXPECT_EQ(fmt::format("{}", seconds(-1)), "-1s");
  EXPECT_EQ(fmt::format("{}", seconds(-42)), "-42s");
}

TEST(FmtChronoFormatterDurationTest, NegativeDurationEmbeddedInTextPreservesOutputOrder) {
  EXPECT_EQ(fmt::format("value={}", milliseconds(-7)), "value=-7ms");
  EXPECT_EQ(fmt::format("[{}]", seconds(-9)), "[-9s]");
}

TEST(FmtChronoFormatterDurationTest, HmsStyleFormattingSplitsAbsoluteValueAfterWritingMinus) {
  EXPECT_EQ(fmt::format("{:%H:%M:%S}", seconds(0)), "00:00:00");
  EXPECT_EQ(fmt::format("{:%H:%M:%S}", seconds(1)), "00:00:01");
  EXPECT_EQ(fmt::format("{:%H:%M:%S}", seconds(-1)), "-00:00:01");
  EXPECT_EQ(fmt::format("{:%H:%M:%S}", seconds(-61)), "-00:01:01");
  EXPECT_EQ(fmt::format("{:%H:%M:%S}", seconds(-3661)), "-01:01:01");
}

TEST(FmtChronoFormatterDurationTest, HmsStyleFormattingWithSubsecondsUsesAbsoluteRemainderForNegativeValues) {
  EXPECT_EQ(fmt::format("{:%H:%M:%S}", milliseconds(-1234)), "-00:00:01");
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(-1234)), "-01.234");
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(1234)), "01.234");
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(-1)), "-00.001");
}

TEST(FmtChronoFormatterDurationTest, NegativeSubsecondDurationDoesNotProduceDoubleMinusOrNegativeFraction) {
  const std::string s1 = fmt::format("{:%S}", milliseconds(-234));
  const std::string s2 = fmt::format("{:%S}", milliseconds(-1234));

  EXPECT_EQ(s1, "-00.234");
  EXPECT_EQ(s2, "-01.234");

  EXPECT_EQ(s1.find("--"), std::string::npos);
  EXPECT_EQ(s2.find("--"), std::string::npos);
  EXPECT_EQ(s1.find(".-"), std::string::npos);
  EXPECT_EQ(s2.find(".-"), std::string::npos);
}

TEST(FmtChronoFormatterDurationTest, MinuteAndHourBoundaryFormattingForNegativeDurations) {
  EXPECT_EQ(fmt::format("{:%H:%M:%S}", milliseconds(-999)), "-00:00:00");
  EXPECT_EQ(fmt::format("{:%H:%M:%S}", seconds(-59)), "-00:00:59");
  EXPECT_EQ(fmt::format("{:%H:%M:%S}", seconds(-60)), "-00:01:00");
  EXPECT_EQ(fmt::format("{:%H:%M:%S}", seconds(-3599)), "-00:59:59");
  EXPECT_EQ(fmt::format("{:%H:%M:%S}", seconds(-3600)), "-01:00:00");
}

TEST(FmtChronoFormatterDurationTest, FractionalSecondsFormattingForPositiveAndNegativeMicroseconds) {
  EXPECT_EQ(fmt::format("{:%S}", microseconds(1)), "00.000001");
  EXPECT_EQ(fmt::format("{:%S}", microseconds(-1)), "-00.000001");
  EXPECT_EQ(fmt::format("{:%S}", microseconds(1000001)), "01.000001");
  EXPECT_EQ(fmt::format("{:%S}", microseconds(-1000001)), "-01.000001");
}

TEST(FmtChronoFormatterDurationTest, LargeNegativeDurationFormatsWithLeadingMinusAndAbsoluteComponents) {
  const auto d = hours(-25) - minutes(2) - seconds(3);
  EXPECT_EQ(fmt::format("{:%H:%M:%S}", d), "-25:02:03");
}

}  // namespace