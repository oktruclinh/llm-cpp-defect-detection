#include <gtest/gtest.h>
#include <fmt/format.h>

#include <string>

namespace {

TEST(FmtFormatSignAlignmentTest, NumericAlignmentPlacesSignBeforeZeroPadding) {
  EXPECT_EQ(fmt::format("{:0=5d}", -42), "-0042");
  EXPECT_EQ(fmt::format("{:0=5d}", 42), "00042");
  EXPECT_EQ(fmt::format("{:+0=5d}", 42), "+0042");
  EXPECT_EQ(fmt::format("{: 0=5d}", 42), " 0042");
}

TEST(FmtFormatSignAlignmentTest, NumericAlignmentReducesPaddingWidthForExplicitSign) {
  EXPECT_EQ(fmt::format("{:+0=3d}", 7), "+07");
  EXPECT_EQ(fmt::format("{:+0=2d}", 7), "+7");
  EXPECT_EQ(fmt::format("{:+0=1d}", 7), "+7");
  EXPECT_EQ(fmt::format("{:0=1d}", -7), "-7");
}

TEST(FmtFormatSignAlignmentTest, DefaultAlignmentForNumbersIsRightAndSignCountsTowardWidth) {
  EXPECT_EQ(fmt::format("{:5d}", -42), "  -42");
  EXPECT_EQ(fmt::format("{:5d}", 42), "   42");
  EXPECT_EQ(fmt::format("{:+5d}", 42), "  +42");
  EXPECT_EQ(fmt::format("{: 5d}", 42), "   42");
}

TEST(FmtFormatSignAlignmentTest, ExplicitRightAlignmentWithZeroFillDoesNotBehaveLikeNumericAlignment) {
  EXPECT_EQ(fmt::format("{:0>5d}", -42), "00-42");
  EXPECT_EQ(fmt::format("{:0>5d}", 42), "00042");
  EXPECT_EQ(fmt::format("{:+0>5d}", 42), "00+42");
  EXPECT_EQ(fmt::format("{:0>3d}", -7), "0-7");
}

TEST(FmtFormatSignAlignmentTest, NumericAlignmentDiffersFromRightAlignmentForSignedValues) {
  EXPECT_EQ(fmt::format("{:0=5d}", -42), "-0042");
  EXPECT_EQ(fmt::format("{:0>5d}", -42), "00-42");

  EXPECT_EQ(fmt::format("{:+0=5d}", 42), "+0042");
  EXPECT_EQ(fmt::format("{:+0>5d}", 42), "00+42");
}

TEST(FmtFormatSignAlignmentTest, LeftAndCenterAlignmentKeepSignWithinContentWidth) {
  EXPECT_EQ(fmt::format("{:<5d}", -42), "-42  ");
  EXPECT_EQ(fmt::format("{:^5d}", -42), " -42 ");
  EXPECT_EQ(fmt::format("{:<5d}", 42), "42   ");
  EXPECT_EQ(fmt::format("{:^5d}", 42), " 42  ");
}

TEST(FmtFormatSignAlignmentTest, ZeroWidthAndMinimalWidthsHandleSignsCorrectly) {
  EXPECT_EQ(fmt::format("{:d}", -1), "-1");
  EXPECT_EQ(fmt::format("{:+d}", 1), "+1");
  EXPECT_EQ(fmt::format("{: d}", 1), " 1");

  EXPECT_EQ(fmt::format("{:0=0d}", -1), "-1");
  EXPECT_EQ(fmt::format("{:+0=0d}", 1), "+1");
  EXPECT_EQ(fmt::format("{:0>0d}", -1), "-1");
}

TEST(FmtFormatSignAlignmentTest, StringFormattingWithSignIsRejected) {
  EXPECT_THROW((void)fmt::format("{:+}", std::string("x")), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{: }", std::string("x")), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:0=5}", std::string("x")), fmt::format_error);
}

TEST(FmtFormatSignAlignmentTest, PositiveZeroAndNegativeZeroFormattingRespectAlignmentRules) {
  EXPECT_EQ(fmt::format("{:+0=3d}", 0), "+00");
  EXPECT_EQ(fmt::format("{: 0=3d}", 0), " 00");
  EXPECT_EQ(fmt::format("{:+3d}", 0), " +0");
  EXPECT_EQ(fmt::format("{:03d}", -0), "000");
}

TEST(FmtFormatSignAlignmentTest, FloatingPointNumericAlignmentPlacesSignBeforeZeros) {
  EXPECT_EQ(fmt::format("{:0=8.2f}", -1.5), "-0001.50");
  EXPECT_EQ(fmt::format("{:+0=8.2f}", 1.5), "+0001.50");
  EXPECT_EQ(fmt::format("{:0>8.2f}", -1.5), "000-1.50");
  EXPECT_EQ(fmt::format("{:8.2f}", -1.5), "   -1.50");
}

}  // namespace