#include <gtest/gtest.h>

#include <chrono>
#include <string>
#include <type_traits>

#include <fmt/chrono.h>
#include <fmt/format.h>

namespace {

template <typename Duration>
std::string format_with_default_spec(Duration d) {
  return fmt::format("{}", d);
}

template <typename Duration>
std::string format_with_chrono_spec(Duration d) {
  return fmt::format("{:%S}", d);
}

TEST(FmtChronoDurationSecondsFormattingTest, WholeSecondsDoNotShowFraction) {
  using namespace std::chrono;

  EXPECT_EQ(format_with_chrono_spec(seconds(0)), "00");
  EXPECT_EQ(format_with_chrono_spec(seconds(5)), "05");
  EXPECT_EQ(format_with_chrono_spec(seconds(59)), "59");
}

TEST(FmtChronoDurationSecondsFormattingTest, MillisecondsAppendFractionalPart) {
  using namespace std::chrono;

  EXPECT_EQ(format_with_chrono_spec(milliseconds(0)), "00.000");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(1)), "00.001");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(12)), "00.012");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(999)), "00.999");

  EXPECT_EQ(format_with_chrono_spec(milliseconds(1000)), "01.000");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(1234)), "01.234");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(59001)), "59.001");
}

TEST(FmtChronoDurationSecondsFormattingTest, MicrosecondsAppendFractionalPartWithSixDigits) {
  using namespace std::chrono;

  EXPECT_EQ(format_with_chrono_spec(microseconds(1)), "00.000001");
  EXPECT_EQ(format_with_chrono_spec(microseconds(12)), "00.000012");
  EXPECT_EQ(format_with_chrono_spec(microseconds(123456)), "00.123456");
  EXPECT_EQ(format_with_chrono_spec(microseconds(1000001)), "01.000001");
}

TEST(FmtChronoDurationSecondsFormattingTest, NanosecondsAppendFractionalPartWithNineDigits) {
  using namespace std::chrono;

  EXPECT_EQ(format_with_chrono_spec(nanoseconds(1)), "00.000000001");
  EXPECT_EQ(format_with_chrono_spec(nanoseconds(42)), "00.000000042");
  EXPECT_EQ(format_with_chrono_spec(nanoseconds(123456789)), "00.123456789");
  EXPECT_EQ(format_with_chrono_spec(nanoseconds(1000000001LL)), "01.000000001");
}

TEST(FmtChronoDurationSecondsFormattingTest, FractionalFormattingUsesSecondFieldModuloMinute) {
  using namespace std::chrono;

  EXPECT_EQ(format_with_chrono_spec(minutes(1) + milliseconds(1)), "00.001");
  EXPECT_EQ(format_with_chrono_spec(minutes(1) + seconds(2) + milliseconds(3)), "02.003");
  EXPECT_EQ(format_with_chrono_spec(hours(1) + minutes(2) + seconds(3) + microseconds(4)),
            "03.000004");
}

TEST(FmtChronoDurationSecondsFormattingTest, NegativeFractionalDurationsKeepSignAndFraction) {
  using namespace std::chrono;

  EXPECT_EQ(format_with_chrono_spec(milliseconds(-1)), "-00.001");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(-1234)), "-01.234");
  EXPECT_EQ(format_with_chrono_spec(microseconds(-42)), "-00.000042");
  EXPECT_EQ(format_with_chrono_spec(nanoseconds(-1000000001LL)), "-01.000000001");
}

TEST(FmtChronoDurationSecondsFormattingTest, FormattingDistinguishesWholeAndFractionalDurations) {
  using namespace std::chrono;

  EXPECT_NE(format_with_chrono_spec(seconds(7)), "07.000");
  EXPECT_EQ(format_with_chrono_spec(seconds(7)), "07");

  EXPECT_NE(format_with_chrono_spec(milliseconds(7000)), "07");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(7000)), "07.000");
}

TEST(FmtChronoDurationDefaultFormattingTest, PublicApiStillRepresentsFractionalDurations) {
  using namespace std::chrono;

  const auto s = format_with_default_spec(milliseconds(1234));

  EXPECT_FALSE(s.empty());
  EXPECT_NE(s.find("1.234"), std::string::npos)
      << "Default public formatting should preserve the fractional value";
}

TEST(FmtChronoDurationSecondsFormattingTest, CustomRatioDurationFormatsFractionalSecondsCorrectly) {
  using custom_ms = std::chrono::duration<int, std::milli>;
  using custom_us = std::chrono::duration<long long, std::micro>;

  EXPECT_EQ(format_with_chrono_spec(custom_ms(2345)), "02.345");
  EXPECT_EQ(format_with_chrono_spec(custom_us(1000001)), "01.000001");
}

TEST(FmtChronoDurationSecondsFormattingTest, BoundaryAroundSecondTransitionIsCorrect) {
  using namespace std::chrono;

  EXPECT_EQ(format_with_chrono_spec(milliseconds(999)), "00.999");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(1000)), "01.000");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(1001)), "01.001");

  EXPECT_EQ(format_with_chrono_spec(nanoseconds(999999999LL)), "00.999999999");
  EXPECT_EQ(format_with_chrono_spec(nanoseconds(1000000000LL)), "01.000000000");
  EXPECT_EQ(format_with_chrono_spec(nanoseconds(1000000001LL)), "01.000000001");
}

}  // namespace