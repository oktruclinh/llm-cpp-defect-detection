#include <gtest/gtest.h>
#include <fmt/format.h>

#include <string>

namespace {

TEST(FmtFormatSignAlignmentTest, NumericAlignmentPlacesSignBeforeZeroPadding) {
  EXPECT_EQ(fmt::format("{:05}", -42), "-0042");
  EXPECT_EQ(fmt::format("{:05}", 42), "00042");
  EXPECT_EQ(fmt::format("{:+05}", 42), "+0042");
  EXPECT_EQ(fmt::format("{: 05}", 42), " 0042");
}

TEST(FmtFormatSignAlignmentTest, NumericAlignmentWithExplicitWidthOneAndSign) {
  EXPECT_EQ(fmt::format("{:01}", -7), "-7");
  EXPECT_EQ(fmt::format("{:+01}", 7), "+7");
  EXPECT_EQ(fmt::format("{: 01}", 7), " 7");
}

TEST(FmtFormatSignAlignmentTest, NumericAlignmentReducesPaddingWidthBySign) {
  EXPECT_EQ(fmt::format("{:06}", -12), "-00012");
  EXPECT_EQ(fmt::format("{:+06}", 12), "+00012");
  EXPECT_EQ(fmt::format("{: 06}", 12), " 00012");
}

TEST(FmtFormatSignAlignmentTest, DefaultAlignmentKeepsSignWithinRightAlignedField) {
  EXPECT_EQ(fmt::format("{:5}", -42), "  -42");
  EXPECT_EQ(fmt::format("{:5}", 42), "   42");
  EXPECT_EQ(fmt::format("{:+5}", 42), "  +42");
  EXPECT_EQ(fmt::format("{: 5}", 42), "   42");
}

TEST(FmtFormatSignAlignmentTest, ExplicitRightAlignmentMatchesDefaultForSignedNumbers) {
  EXPECT_EQ(fmt::format("{:>5}", -42), "  -42");
  EXPECT_EQ(fmt::format("{:>5}", 42), "   42");
  EXPECT_EQ(fmt::format("{:+>5}", 42), "++++42");
  EXPECT_EQ(fmt::format("{:>+5}", 42), "  +42");
}

TEST(FmtFormatSignAlignmentTest, LeftAndCenterAlignmentCountSignAsPartOfContent) {
  EXPECT_EQ(fmt::format("{:<5}", -42), "-42  ");
  EXPECT_EQ(fmt::format("{:^5}", -42), " -42 ");
  EXPECT_EQ(fmt::format("{:<+5}", 42), "+42  ");
  EXPECT_EQ(fmt::format("{:^+5}", 42), " +42 ");
}

TEST(FmtFormatSignAlignmentTest, ZeroFlagBehavesAsNumericAlignmentEvenWithSignOptions) {
  EXPECT_EQ(fmt::format("{:+08}", 123), "+0000123");
  EXPECT_EQ(fmt::format("{: 08}", 123), " 0000123");
  EXPECT_EQ(fmt::format("{:08}", -123), "-0000123");
}

TEST(FmtFormatSignAlignmentTest, CustomFillWithRightAlignmentPadsBeforeSign) {
  EXPECT_EQ(fmt::format("{:_>6}", -12), "___-12");
  EXPECT_EQ(fmt::format("{:_>+6}", 12), "___+12");
  EXPECT_EQ(fmt::format("{:_> 6}", 12), "___ 12");
}

TEST(FmtFormatSignAlignmentTest, CustomFillWithNumericAlignmentPadsAfterSign) {
  EXPECT_EQ(fmt::format("{:0>6}", -12), "000-12");
  EXPECT_EQ(fmt::format("{:0=6}", -12), "-00012");
  EXPECT_EQ(fmt::format("{:0=+6}", 12), "+00012");
  EXPECT_EQ(fmt::format("{:0= 6}", 12), " 00012");
}

TEST(FmtFormatSignAlignmentTest, WidthZeroAndNoExtraPaddingEdgeCases) {
  EXPECT_EQ(fmt::format("{}", -1), "-1");
  EXPECT_EQ(fmt::format("{:0}", -1), "-1");
  EXPECT_EQ(fmt::format("{:+0}", 1), "+1");
  EXPECT_EQ(fmt::format("{: 0}", 1), " 1");
}

TEST(FmtFormatSignAlignmentTest, ZeroValueStillHandlesSignAndAlignmentCorrectly) {
  EXPECT_EQ(fmt::format("{:05}", 0), "00000");
  EXPECT_EQ(fmt::format("{:+05}", 0), "+0000");
  EXPECT_EQ(fmt::format("{: 05}", 0), " 0000");
  EXPECT_EQ(fmt::format("{:5}", 0), "    0");
  EXPECT_EQ(fmt::format("{:+5}", 0), "   +0");
}

TEST(FmtFormatSignAlignmentTest, NegativeZeroFormattingWithFloatingPointUsesSameSignPlacementRules) {
  EXPECT_EQ(fmt::format("{:08.1f}", -0.0), "-00000.0");
  EXPECT_EQ(fmt::format("{:8.1f}", -0.0), "    -0.0");
}

TEST(FmtFormatSignAlignmentTest, ExplicitNumericAlignmentForFloatingPointPlacesSignBeforePadding) {
  EXPECT_EQ(fmt::format("{:0=8.2f}", -1.5), "-0001.50");
  EXPECT_EQ(fmt::format("{:0=+8.2f}", 1.5), "+0001.50");
}

TEST(FmtFormatSignAlignmentTest, SignAwarePaddingWithNonNumericAlignmentCountsSignInWidth) {
  EXPECT_EQ(fmt::format("{:_<6}", -12), "-12___");
  EXPECT_EQ(fmt::format("{:_^6}", -12), "_-12__");
  EXPECT_EQ(fmt::format("{:_<+6}", 12), "+12___");
  EXPECT_EQ(fmt::format("{:_^+6}", 12), "_+12__");
}

TEST(FmtFormatSignAlignmentTest, InvalidFormatSpecifiersReportErrors) {
  EXPECT_THROW((void)fmt::format("{:0=}", "x"), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:+}", "x"), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{: }", "x"), fmt::format_error);
}

}  // namespace