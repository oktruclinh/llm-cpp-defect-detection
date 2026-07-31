#include <gtest/gtest.h>

#include <cmath>
#include <limits>
#include <string>

#include <fmt/format.h>

// These tests exercise floating-point formatting through fmt's public API.
// The focal code in format-inl.h is part of the internal decimal conversion
// logic used when formatting floating-point values, especially for values
// whose decimal exponent is negative. The tests below target behavior that
// depends on correct handling of powers of 10, boundaries, and rounding.

namespace {

template <typename T>
std::string FormatGeneral(T value) {
  return fmt::format("{}", value);
}

template <typename T>
std::string FormatFixed(T value, int precision) {
  return fmt::format("{:.{}f}", value, precision);
}

template <typename T>
std::string FormatScientific(T value, int precision) {
  return fmt::format("{:.{}e}", value, precision);
}

template <typename T>
std::string FormatGeneralPrec(T value, int precision) {
  return fmt::format("{:.{}g}", value, precision);
}

}  // namespace

TEST(FmtFormatFloatNegativeExponentTest, FormatsSimpleFractionalValuesExactly) {
  EXPECT_EQ(FormatGeneral(0.1), "0.1");
  EXPECT_EQ(FormatGeneral(0.01), "0.01");
  EXPECT_EQ(FormatGeneral(0.001), "0.001");

  EXPECT_EQ(FormatGeneral(0.125), "0.125");
  EXPECT_EQ(FormatGeneral(0.5), "0.5");
  EXPECT_EQ(FormatGeneral(0.25), "0.25");
}

TEST(FmtFormatFloatNegativeExponentTest, FormatsNegativeFractionalValuesExactly) {
  EXPECT_EQ(FormatGeneral(-0.1), "-0.1");
  EXPECT_EQ(FormatGeneral(-0.01), "-0.01");
  EXPECT_EQ(FormatGeneral(-0.125), "-0.125");
}

TEST(FmtFormatFloatNegativeExponentTest, PreservesLeadingZerosAfterDecimalPoint) {
  EXPECT_EQ(FormatGeneral(1e-5), "1e-05");
  EXPECT_EQ(FormatGeneral(1e-6), "1e-06");
  EXPECT_EQ(FormatGeneral(1.2e-5), "1.2e-05");

  EXPECT_EQ(FormatFixed(1e-5, 6), "0.000010");
  EXPECT_EQ(FormatFixed(1e-6, 7), "0.0000010");
  EXPECT_EQ(FormatFixed(1.2e-5, 7), "0.0000120");
}

TEST(FmtFormatFloatNegativeExponentTest, ZeroAndNegativeZeroAreHandledCorrectly) {
  EXPECT_EQ(FormatGeneral(0.0), "0");
  EXPECT_EQ(FormatFixed(0.0, 3), "0.000");

  double neg_zero = -0.0;
  EXPECT_EQ(FormatGeneral(neg_zero), "-0");
  EXPECT_EQ(FormatFixed(neg_zero, 3), "-0.000");
  EXPECT_EQ(FormatScientific(neg_zero, 2), "-0.00e+00");
}

TEST(FmtFormatFloatNegativeExponentTest, RoundsCorrectlyAtFractionalHalfwayBoundariesFixed) {
  EXPECT_EQ(FormatFixed(0.05, 1), "0.1");
  EXPECT_EQ(FormatFixed(0.15, 1), "0.1");
  EXPECT_EQ(FormatFixed(0.25, 1), "0.2");
  EXPECT_EQ(FormatFixed(0.35, 1), "0.3");
  EXPECT_EQ(FormatFixed(0.45, 1), "0.5");

  EXPECT_EQ(FormatFixed(-0.05, 1), "-0.1");
  EXPECT_EQ(FormatFixed(-0.15, 1), "-0.1");
}

TEST(FmtFormatFloatNegativeExponentTest, RoundsAcrossDecimalCarryBoundary) {
  EXPECT_EQ(FormatFixed(0.9999, 3), "1.000");
  EXPECT_EQ(FormatFixed(9.9999, 3), "10.000");
  EXPECT_EQ(FormatFixed(0.009999, 2), "0.01");
  EXPECT_EQ(FormatFixed(0.0009999, 3), "0.001");
}

TEST(FmtFormatFloatNegativeExponentTest, HandlesVerySmallNormalValues) {
  EXPECT_EQ(FormatScientific(std::numeric_limits<double>::min(), 6),
            "2.225074e-308");
  EXPECT_EQ(FormatScientific(std::numeric_limits<float>::min(), 6),
            "1.175494e-38");
}

TEST(FmtFormatFloatNegativeExponentTest, HandlesSubnormalValues) {
  EXPECT_EQ(FormatScientific(std::numeric_limits<double>::denorm_min(), 6),
            "4.940656e-324");
  EXPECT_EQ(FormatScientific(std::numeric_limits<float>::denorm_min(), 6),
            "1.401298e-45");
}

TEST(FmtFormatFloatNegativeExponentTest, GeneralFormatChoosesScientificForTinyValues) {
  EXPECT_EQ(FormatGeneral(1e-7), "1e-07");
  EXPECT_EQ(FormatGeneral(1e-20), "1e-20");
  EXPECT_EQ(FormatGeneral(-1e-20), "-1e-20");
}

TEST(FmtFormatFloatNegativeExponentTest, FixedFormatRepresentsTinyValuesWithExpectedZeros) {
  EXPECT_EQ(FormatFixed(1e-7, 7), "0.0000001");
  EXPECT_EQ(FormatFixed(1e-7, 8), "0.00000010");
  EXPECT_EQ(FormatFixed(5e-4, 4), "0.0005");
  EXPECT_EQ(FormatFixed(5e-4, 3), "0.001");
}

TEST(FmtFormatFloatNegativeExponentTest, PrecisionGeneralProducesExpectedShortestRoundedForms) {
  EXPECT_EQ(FormatGeneralPrec(0.000123456, 3), "0.000123");
  EXPECT_EQ(FormatGeneralPrec(0.000123956, 3), "0.000124");
  EXPECT_EQ(FormatGeneralPrec(0.0009999, 3), "0.001");
  EXPECT_EQ(FormatGeneralPrec(9.999e-5, 3), "0.0001");
}

TEST(FmtFormatFloatNegativeExponentTest, MixedMagnitudeFractionalCases) {
  EXPECT_EQ(FormatGeneral(123.456), "123.456");
  EXPECT_EQ(FormatFixed(123.456, 2), "123.46");
  EXPECT_EQ(FormatScientific(123.456, 2), "1.23e+02");

  EXPECT_EQ(FormatGeneral(0.000244140625), "0.000244140625");
  EXPECT_EQ(FormatFixed(0.000244140625, 12), "0.000244140625");
}

TEST(FmtFormatFloatNegativeExponentTest, InfinityAndNaNAreFormattedThroughPublicApi) {
  EXPECT_EQ(FormatGeneral(std::numeric_limits<double>::infinity()), "inf");
  EXPECT_EQ(FormatGeneral(-std::numeric_limits<double>::infinity()), "-inf");

  std::string nan_str = FormatGeneral(std::numeric_limits<double>::quiet_NaN());
  EXPECT_EQ(nan_str, "nan");
}

TEST(FmtFormatFloatNegativeExponentTest, LongDoubleTinyValuesAreHandled) {
  long double v = 1e-6L;
  EXPECT_EQ(fmt::format("{:.8f}", v), "0.00000100");
  EXPECT_EQ(fmt::format("{:.2e}", v), "1.00e-06");
}