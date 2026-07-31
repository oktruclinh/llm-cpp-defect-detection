#include <gtest/gtest.h>

#include <cmath>
#include <limits>
#include <string>

#include <fmt/format.h>

// These tests exercise floating-point formatting through fmt's public API.
// The focal code path is part of the internal decimal formatting machinery
// handling powers of ten, especially for negative decimal exponents.

namespace {

template <typename T>
std::string G(T value, int precision = -1) {
  if (precision < 0) return fmt::format("{}", value);
  return fmt::format("{:.{}g}", value, precision);
}

template <typename T>
std::string F(T value, int precision) {
  return fmt::format("{:.{}f}", value, precision);
}

template <typename T>
std::string E(T value, int precision) {
  return fmt::format("{:.{}e}", value, precision);
}

TEST(FmtFormatInl_PublicAPI_FloatingPoint, ZeroFormatsCorrectly) {
  EXPECT_EQ(fmt::format("{}", 0.0), "0");
  EXPECT_EQ(fmt::format("{}", -0.0), "-0");

  EXPECT_EQ(F(0.0, 0), "0");
  EXPECT_EQ(F(0.0, 3), "0.000");
  EXPECT_EQ(E(0.0, 2), "0.00e+00");
}

TEST(FmtFormatInl_PublicAPI_FloatingPoint, SimpleNegativeExponentFiniteDecimals) {
  EXPECT_EQ(fmt::format("{}", 0.1), "0.1");
  EXPECT_EQ(fmt::format("{}", 0.01), "0.01");
  EXPECT_EQ(fmt::format("{}", 0.001), "0.001");

  EXPECT_EQ(F(0.1, 1), "0.1");
  EXPECT_EQ(F(0.01, 2), "0.01");
  EXPECT_EQ(F(0.001, 3), "0.001");

  EXPECT_EQ(E(0.1, 1), "1.0e-01");
  EXPECT_EQ(E(0.01, 1), "1.0e-02");
  EXPECT_EQ(E(0.001, 1), "1.0e-03");
}

TEST(FmtFormatInl_PublicAPI_FloatingPoint, NegativeExponentRoundingBoundaries) {
  // Values around half-way boundaries where exact scaling by powers of ten matters.
  EXPECT_EQ(F(0.15, 1), "0.1");
  EXPECT_EQ(F(0.25, 1), "0.2");
  EXPECT_EQ(F(0.35, 1), "0.3");

  EXPECT_EQ(F(0.14999999999999999, 1), "0.1");
  EXPECT_EQ(F(0.15000000000000002, 1), "0.2");

  EXPECT_EQ(F(0.00999, 2), "0.01");
  EXPECT_EQ(F(0.00499, 2), "0.00");
  EXPECT_EQ(F(0.00501, 2), "0.01");
}

TEST(FmtFormatInl_PublicAPI_FloatingPoint, GeneralFormatUsesShortestRoundTrippableForSmallFractions) {
  EXPECT_EQ(G(1e-1), "0.1");
  EXPECT_EQ(G(1e-2), "0.01");
  EXPECT_EQ(G(1e-3), "0.001");
  EXPECT_EQ(G(1e-4), "0.0001");

  EXPECT_EQ(G(1.25e-3), "0.00125");
  EXPECT_EQ(G(1.2345e-5), "1.2345e-05");
}

TEST(FmtFormatInl_PublicAPI_FloatingPoint, PrecisionGForSmallNegativePowersOfTen) {
  EXPECT_EQ(G(0.000123456, 1), "0.0001");
  EXPECT_EQ(G(0.000123456, 2), "0.00012");
  EXPECT_EQ(G(0.000123456, 3), "0.000123");
  EXPECT_EQ(G(0.000123456, 4), "0.0001235");

  EXPECT_EQ(G(9.999e-5, 3), "0.0001");
  EXPECT_EQ(G(9.994e-5, 3), "9.99e-05");
}

TEST(FmtFormatInl_PublicAPI_FloatingPoint, VerySmallNormalNumbers) {
  const double v = 1e-308;
  EXPECT_EQ(fmt::format("{:.0e}", v), "1e-308");
  EXPECT_EQ(fmt::format("{:.3e}", v), "1.000e-308");

  const double w = 2.2250738585072014e-308;  // DBL_MIN
  EXPECT_EQ(fmt::format("{:.16e}", w), "2.2250738585072014e-308");
}

TEST(FmtFormatInl_PublicAPI_FloatingPoint, SmallSubnormalNumbers) {
  const double denorm_min = std::numeric_limits<double>::denorm_min();
  ASSERT_GT(denorm_min, 0.0);

  // Smallest positive subnormal double should format consistently.
  EXPECT_EQ(fmt::format("{:.0e}", denorm_min), "5e-324");
  EXPECT_EQ(fmt::format("{:.1e}", denorm_min), "4.9e-324");
  EXPECT_EQ(fmt::format("{}", denorm_min), "5e-324");
}

TEST(FmtFormatInl_PublicAPI_FloatingPoint, FloatSubnormalAndMin) {
  const float denorm_min = std::numeric_limits<float>::denorm_min();
  const float min_normal = std::numeric_limits<float>::min();

  EXPECT_EQ(fmt::format("{:.0e}", denorm_min), "1e-45");
  EXPECT_EQ(fmt::format("{:.1e}", denorm_min), "1.4e-45");

  EXPECT_EQ(fmt::format("{:.0e}", min_normal), "1e-38");
  EXPECT_EQ(fmt::format("{:.6e}", min_normal), "1.175494e-38");
}

TEST(FmtFormatInl_PublicAPI_FloatingPoint, NegativeSmallNumbersPreserveSignAndMagnitude) {
  EXPECT_EQ(fmt::format("{}", -0.1), "-0.1");
  EXPECT_EQ(F(-0.01, 2), "-0.01");
  EXPECT_EQ(E(-0.001, 1), "-1.0e-03");

  EXPECT_EQ(F(-0.00501, 2), "-0.01");
  EXPECT_EQ(F(-0.00499, 2), "-0.00");
}

TEST(FmtFormatInl_PublicAPI_FloatingPoint, LargeNegativeDecimalExponentFormatting) {
  EXPECT_EQ(fmt::format("{:.2e}", 1e-100), "1.00e-100");
  EXPECT_EQ(fmt::format("{:.2e}", 1e-200), "1.00e-200");
  EXPECT_EQ(fmt::format("{:.2e}", 1e-300), "1.00e-300");

  EXPECT_EQ(fmt::format("{:.0e}", 9.99e-100), "1e-99");
  EXPECT_EQ(fmt::format("{:.1e}", 9.94e-100), "9.9e-100");
  EXPECT_EQ(fmt::format("{:.1e}", 9.95e-100), "1.0e-99");
}

TEST(FmtFormatInl_PublicAPI_FloatingPoint, InfinityAndNaNErrorRelevantSpecialValues) {
  const double inf = std::numeric_limits<double>::infinity();
  const double nan = std::numeric_limits<double>::quiet_NaN();

  EXPECT_EQ(fmt::format("{}", inf), "inf");
  EXPECT_EQ(fmt::format("{}", -inf), "-inf");

  std::string nan_text = fmt::format("{}", nan);
  EXPECT_FALSE(nan_text.empty());
  EXPECT_NE(nan_text.find("nan"), std::string::npos);

  std::string neg_nan_text = fmt::format("{}", -nan);
  EXPECT_FALSE(neg_nan_text.empty());
  EXPECT_NE(neg_nan_text.find("nan"), std::string::npos);
}

TEST(FmtFormatInl_PublicAPI_FloatingPoint, ExactPowersOfTenRemainStableAcrossFormats) {
  EXPECT_EQ(fmt::format("{}", 1e-6), "1e-06");
  EXPECT_EQ(fmt::format("{}", 1e-5), "1e-05");
  EXPECT_EQ(fmt::format("{}", 1e-4), "0.0001");

  EXPECT_EQ(F(1e-6, 6), "0.000001");
  EXPECT_EQ(F(1e-5, 5), "0.00001");
  EXPECT_EQ(F(1e-4, 4), "0.0001");
}

TEST(FmtFormatInl_PublicAPI_FloatingPoint, RoundTripSensitiveFractions) {
  // These values are sensitive to correct internal scaling by powers of ten.
  EXPECT_EQ(fmt::format("{}", 0.84551240822557006), "0.8455124082255701");
  EXPECT_EQ(fmt::format("{}", 0.29999999999999999), "0.3");
  EXPECT_EQ(fmt::format("{}", 0.0999999999999999917), "0.1");
  EXPECT_EQ(fmt::format("{}", 0.0009999999999999998), "0.0009999999999999998");
}

}  // namespace