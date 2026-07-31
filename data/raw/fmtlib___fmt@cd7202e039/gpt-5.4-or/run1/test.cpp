#include <gtest/gtest.h>

#include <chrono>
#include <string>

#include <fmt/chrono.h>
#include <fmt/format.h>

namespace {

using namespace std::chrono;

TEST(FmtChronoDurationFormattingTest, SecondsWithoutFractionDoNotPrintDecimalPart) {
  EXPECT_EQ(fmt::format("{}", seconds(0)), "00");
  EXPECT_EQ(fmt::format("{}", seconds(5)), "05");
  EXPECT_EQ(fmt::format("{}", seconds(59)), "59");
}

TEST(FmtChronoDurationFormattingTest, MillisecondsPrintFractionalSeconds) {
  EXPECT_EQ(fmt::format("{}", milliseconds(0)), "00.000");
  EXPECT_EQ(fmt::format("{}", milliseconds(1)), "00.001");
  EXPECT_EQ(fmt::format("{}", milliseconds(12)), "00.012");
  EXPECT_EQ(fmt::format("{}", milliseconds(999)), "00.999");

  EXPECT_EQ(fmt::format("{}", milliseconds(1000)), "01.000");
  EXPECT_EQ(fmt::format("{}", milliseconds(1234)), "01.234");
  EXPECT_EQ(fmt::format("{}", milliseconds(59001)), "59.001");
}

TEST(FmtChronoDurationFormattingTest, MicrosecondsPrintSixFractionalDigits) {
  EXPECT_EQ(fmt::format("{}", microseconds(0)), "00.000000");
  EXPECT_EQ(fmt::format("{}", microseconds(1)), "00.000001");
  EXPECT_EQ(fmt::format("{}", microseconds(42)), "00.000042");
  EXPECT_EQ(fmt::format("{}", microseconds(123456)), "00.123456");
  EXPECT_EQ(fmt::format("{}", microseconds(1000000)), "01.000000");
}

TEST(FmtChronoDurationFormattingTest, NanosecondsPrintNineFractionalDigits) {
  EXPECT_EQ(fmt::format("{}", nanoseconds(0)), "00.000000000");
  EXPECT_EQ(fmt::format("{}", nanoseconds(1)), "00.000000001");
  EXPECT_EQ(fmt::format("{}", nanoseconds(999999999)), "00.999999999");
  EXPECT_EQ(fmt::format("{}", nanoseconds(1000000000)), "01.000000000");
  EXPECT_EQ(fmt::format("{}", nanoseconds(1234567890)), "01.234567890");
}

TEST(FmtChronoDurationFormattingTest, FractionalFormattingWorksWithCustomChronoSpec) {
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(1)), "00.001");
  EXPECT_EQ(fmt::format("{:%S}", microseconds(123456)), "00.123456");
  EXPECT_EQ(fmt::format("{:%S}", nanoseconds(123456789)), "00.123456789");
  EXPECT_EQ(fmt::format("{:%S}", seconds(7)), "07");
}

TEST(FmtChronoDurationFormattingTest, MinutesAndHoursUseSecondFieldModuloSixtyWithFraction) {
  EXPECT_EQ(fmt::format("{:%S}", minutes(1)), "00");
  EXPECT_EQ(fmt::format("{:%S}", hours(1)), "00");

  EXPECT_EQ(fmt::format("{:%S}", duration_cast<milliseconds>(minutes(1)) + milliseconds(234)),
            "00.234");
  EXPECT_EQ(fmt::format("{:%S}", duration_cast<microseconds>(hours(1)) + microseconds(5)),
            "00.000005");
}

TEST(FmtChronoDurationFormattingTest, CustomRatioDurationPrintsExactFractionalPrecision) {
  using tenth_seconds = duration<int, std::ratio<1, 10>>;
  using quarter_seconds = duration<int, std::ratio<1, 4>>;

  EXPECT_EQ(fmt::format("{}", tenth_seconds(0)), "00.1");
  EXPECT_EQ(fmt::format("{}", tenth_seconds(1)), "00.1");
  EXPECT_EQ(fmt::format("{}", tenth_seconds(12)), "01.2");

  EXPECT_EQ(fmt::format("{}", quarter_seconds(1)), "00.25");
  EXPECT_EQ(fmt::format("{}", quarter_seconds(3)), "00.75");
  EXPECT_EQ(fmt::format("{}", quarter_seconds(5)), "01.25");
}

TEST(FmtChronoDurationFormattingTest, NegativeFractionalDurationsKeepSecondFieldAndFractionConsistent) {
  EXPECT_EQ(fmt::format("{}", milliseconds(-1)), "-00.001");
  EXPECT_EQ(fmt::format("{}", milliseconds(-1234)), "-01.234");
  EXPECT_EQ(fmt::format("{}", microseconds(-42)), "-00.000042");
  EXPECT_EQ(fmt::format("{}", nanoseconds(-1000000001)), "-01.000000001");
}

TEST(FmtChronoDurationFormattingTest, BoundaryAroundMinuteStillFormatsSecondsFieldCorrectly) {
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(59999)), "59.999");
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(60000)), "00.000");
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(61001)), "01.001");
}

TEST(FmtChronoDurationFormattingTest, StringFormattingIsConsistent) {
  EXPECT_EQ(fmt::format("{}", milliseconds(2345)), "02.345");
  EXPECT_EQ(fmt::format("{:%S}", microseconds(7)), "00.000007");
}

}  // namespace