#include <gtest/gtest.h>

#include <cmath>
#include <limits>
#include <string>

#include <fmt/format.h>

// These tests exercise fmt's floating-point formatting through the public API.
// The focal code is part of the internal decimal conversion path that handles
// powers of 10, especially for negative decimal exponents. We validate behavior
// using observable formatting results.

namespace {

template <typename T>
void expect_same_as_sprintf_g(T value, int precision) {
  char buf[128];
  std::snprintf(buf, sizeof(buf), "%.*g", precision, static_cast<double>(value));
  EXPECT_EQ(fmt::format("{:.{}g}", value, precision), std::string(buf));
}

template <typename T>
void expect_same_as_sprintf_e(T value, int precision) {
  char buf[128];
  std::snprintf(buf, sizeof(buf), "%.*e", precision, static_cast<double>(value));
  EXPECT_EQ(fmt::format("{:.{}e}", value, precision), std::string(buf));
}

template <typename T>
void expect_same_as_sprintf_f(T value, int precision) {
  char buf[256];
  std::snprintf(buf, sizeof(buf), "%.*f", precision, static_cast<double>(value));
  EXPECT_EQ(fmt::format("{:.{}f}", value, precision), std::string(buf));
}

}  // namespace

TEST(FmtFormatInlFloatingPointPublicApiTest, ZeroFormatsCorrectlyAcrossSpecs) {
  EXPECT_EQ(fmt::format("{}", 0.0), "0");
  EXPECT_EQ(fmt::format("{:g}", 0.0), "0");
  EXPECT_EQ(fmt::format("{:.0f}", 0.0), "0");
  EXPECT_EQ(fmt::format("{:.1f}", 0.0), "0.0");
  EXPECT_EQ(fmt::format("{:.6f}", 0.0), "0.000000");

  EXPECT_EQ(fmt::format("{}", -0.0), "-0");
  EXPECT_EQ(fmt::format("{:g}", -0.0), "-0");
  EXPECT_EQ(fmt::format("{:.0f}", -0.0), "-0");
  EXPECT_EQ(fmt::format("{:.1f}", -0.0), "-0.0");
}

TEST(FmtFormatInlFloatingPointPublicApiTest, SimpleNegativeExponentDecimalValues) {
  EXPECT_EQ(fmt::format("{}", 0.1), "0.1");
  EXPECT_EQ(fmt::format("{}", 0.01), "0.01");
  EXPECT_EQ(fmt::format("{}", 0.001), "0.001");
  EXPECT_EQ(fmt::format("{}", 1e-6), "1e-06");
  EXPECT_EQ(fmt::format("{}", 1e-7), "1e-07");

  EXPECT_EQ(fmt::format("{:.1f}", 0.1), "0.1");
  EXPECT_EQ(fmt::format("{:.2f}", 0.01), "0.01");
  EXPECT_EQ(fmt::format("{:.3f}", 0.001), "0.001");
  EXPECT_EQ(fmt::format("{:.6f}", 1e-6), "0.000001");
}

TEST(FmtFormatInlFloatingPointPublicApiTest, RoundingAtHalfwayCasesAroundNegativePowersOfTen) {
  EXPECT_EQ(fmt::format("{:.0f}", 0.5), "0");
  EXPECT_EQ(fmt::format("{:.0f}", 1.5), "2");
  EXPECT_EQ(fmt::format("{:.0f}", 2.5), "2");
  EXPECT_EQ(fmt::format("{:.0f}", 3.5), "4");

  EXPECT_EQ(fmt::format("{:.3f}", 0.0005), "0.000");
  EXPECT_EQ(fmt::format("{:.3f}", 0.0015), "0.002");
  EXPECT_EQ(fmt::format("{:.4f}", 0.00015), "0.0001");
  EXPECT_EQ(fmt::format("{:.4f}", 0.00025), "0.0003");
}

TEST(FmtFormatInlFloatingPointPublicApiTest, CarriesAcrossDecimalBoundaryForSmallNumbers) {
  EXPECT_EQ(fmt::format("{:.3f}", 0.9995), "1.000");
  EXPECT_EQ(fmt::format("{:.2f}", 9.995), "10.00");
  EXPECT_EQ(fmt::format("{:.4f}", 0.09995), "0.1000");
  EXPECT_EQ(fmt::format("{:.5f}", 0.0099995), "0.01000");
}

TEST(FmtFormatInlFloatingPointPublicApiTest, GeneralFormatChoosesExpectedRepresentationNearExponentThresholds) {
  EXPECT_EQ(fmt::format("{:g}", 0.0001), "0.0001");
  EXPECT_EQ(fmt::format("{:g}", 0.00001), "1e-05");
  EXPECT_EQ(fmt::format("{:g}", 99999.0), "99999");
  EXPECT_EQ(fmt::format("{:g}", 100000.0), "100000");
  EXPECT_EQ(fmt::format("{:.5g}", 100000.0), "1e+05");

  EXPECT_EQ(fmt::format("{:.6g}", 1.23456e-5), "1.23456e-05");
  EXPECT_EQ(fmt::format("{:.6g}", 1.23456e-4), "0.000123456");
}

TEST(FmtFormatInlFloatingPointPublicApiTest, ExactPowersOfTenWithNegativeExponent) {
  EXPECT_EQ(fmt::format("{:.0e}", 1e-1), "1e-01");
  EXPECT_EQ(fmt::format("{:.0e}", 1e-2), "1e-02");
  EXPECT_EQ(fmt::format("{:.0e}", 1e-10), "1e-10");
  EXPECT_EQ(fmt::format("{:.0e}", 1e-99), "1e-99");

  EXPECT_EQ(fmt::format("{:.1g}", 1e-1), "0.1");
  EXPECT_EQ(fmt::format("{:.1g}", 1e-4), "0.0001");
  EXPECT_EQ(fmt::format("{:.1g}", 1e-5), "1e-05");
}

TEST(FmtFormatInlFloatingPointPublicApiTest, SubnormalAndMinNormalValuesFormatReasonably) {
  const double min_normal = std::numeric_limits<double>::min();
  const double denorm_min = std::numeric_limits<double>::denorm_min();

  EXPECT_EQ(fmt::format("{:.0e}", min_normal), "2e-308");
  EXPECT_EQ(fmt::format("{:.3e}", min_normal), "2.225e-308");

  EXPECT_EQ(fmt::format("{:.0e}", denorm_min), "5e-324");
  EXPECT_EQ(fmt::format("{:.3e}", denorm_min), "4.941e-324");
}

TEST(FmtFormatInlFloatingPointPublicApiTest, NegativeValuesMirrorPositiveFormatting) {
  EXPECT_EQ(fmt::format("{}", -0.1), "-0.1");
  EXPECT_EQ(fmt::format("{:.3f}", -0.0015), "-0.002");
  EXPECT_EQ(fmt::format("{:.3f}", -0.9995), "-1.000");
  EXPECT_EQ(fmt::format("{:g}", -0.00001), "-1e-05");
}

TEST(FmtFormatInlFloatingPointPublicApiTest, MatchesPrintfForRepresentativeNegativeExponentCases) {
  const double values[] = {
      1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6,
      1.2345e-1, 1.2345e-2, 1.2345e-3, 1.2345e-10,
      9.9995e-1, 9.9995e-2, 9.9995e-3};

  for (double v : values) {
    expect_same_as_sprintf_g(v, 6);
    expect_same_as_sprintf_e(v, 6);
    expect_same_as_sprintf_f(v, 6);
  }
}

TEST(FmtFormatInlFloatingPointPublicApiTest, PrecisionZeroAndAlternateFormEdgeCases) {
  EXPECT_EQ(fmt::format("{:.0g}", 0.000123), "0.0001");
  EXPECT_EQ(fmt::format("{:.0g}", 0.0000123), "1e-05");

  EXPECT_EQ(fmt::format("{:#.0f}", 0.0), "0.");
  EXPECT_EQ(fmt::format("{:#.0f}", 1.0), "1.");
  EXPECT_EQ(fmt::format("{:#.0e}", 1e-5), "1.e-05");
}

TEST(FmtFormatInlFloatingPointPublicApiTest, InfinityAndNanAreHandledViaPublicApi) {
  const double inf = std::numeric_limits<double>::infinity();
  const double nan = std::numeric_limits<double>::quiet_NaN();

  EXPECT_EQ(fmt::format("{}", inf), "inf");
  EXPECT_EQ(fmt::format("{}", -inf), "-inf");

  const std::string nan_text = fmt::format("{}", nan);
  EXPECT_FALSE(nan_text.empty());
  EXPECT_NE(nan_text.find("nan"), std::string::npos);

  const std::string neg_nan_text = fmt::format("{}", -nan);
  EXPECT_FALSE(neg_nan_text.empty());
  EXPECT_NE(neg_nan_text.find("nan"), std::string::npos);
}