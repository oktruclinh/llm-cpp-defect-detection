#include <gtest/gtest.h>
#include <fmt/format.h>

#include <string>

namespace {

TEST(FmtFormatSignAlignmentTest, NumericAlignmentPlacesSignBeforeZeroPadding) {
  EXPECT_EQ(fmt::format("{:05}", -42), "-0042");
  EXPECT_EQ(fmt::format("{:06}", -42), "-00042");
  EXPECT_EQ(fmt::format("{:05}", 42), "00042");
}

TEST(FmtFormatSignAlignmentTest, NumericAlignmentWithExplicitPlusPlacesPlusBeforeZeroPadding) {
  EXPECT_EQ(fmt::format("{:+05}", 42), "+0042");
  EXPECT_EQ(fmt::format("{:+06}", 42), "+00042");
}

TEST(FmtFormatSignAlignmentTest, NumericAlignmentWithSpaceSignPlacesSpaceBeforeZeroPadding) {
  EXPECT_EQ(fmt::format("{: 05}", 42), " 0042");
  EXPECT_EQ(fmt::format("{: 06}", 42), " 00042");
}

TEST(FmtFormatSignAlignmentTest, NumericAlignmentWidthCountsSignCharacter) {
  EXPECT_EQ(fmt::format("{:+3}", 42), "+42");
  EXPECT_EQ(fmt::format("{:+4}", 42), " +42");
  EXPECT_EQ(fmt::format("{:+5}", 42), "  +42");

  EXPECT_EQ(fmt::format("{:03}", -1), "-01");
  EXPECT_EQ(fmt::format("{:02}", -1), "-1");
  EXPECT_EQ(fmt::format("{:01}", -1), "-1");
  EXPECT_EQ(fmt::format("{:00}", -1), "-1");
}

TEST(FmtFormatSignAlignmentTest, DefaultAlignmentForSignedNumbersIsRightAndIncludesSignInWidth) {
  EXPECT_EQ(fmt::format("{:5}", -42), "  -42");
  EXPECT_EQ(fmt::format("{:4}", -42), " -42");
  EXPECT_EQ(fmt::format("{:3}", -42), "-42");

  EXPECT_EQ(fmt::format("{:+5}", 42), "  +42");
  EXPECT_EQ(fmt::format("{: 5}", 42), "   42");
}

TEST(FmtFormatSignAlignmentTest, ExplicitRightAlignmentIncludesSignWithinFieldWidth) {
  EXPECT_EQ(fmt::format("{:>5}", -42), "  -42");
  EXPECT_EQ(fmt::format("{:>5}", 42), "   42");
  EXPECT_EQ(fmt::format("{:+>5}", -42), "++-42");
  EXPECT_EQ(fmt::format("{:*>5}", -42), "**-42");
  EXPECT_EQ(fmt::format("{:*>5}", 42), "***42");
}

TEST(FmtFormatSignAlignmentTest, ExplicitLeftAlignmentIncludesSignWithinFieldWidth) {
  EXPECT_EQ(fmt::format("{:<5}", -42), "-42  ");
  EXPECT_EQ(fmt::format("{:<5}", 42), "42   ");
  EXPECT_EQ(fmt::format("{:*<5}", -42), "-42**");
}

TEST(FmtFormatSignAlignmentTest, ExplicitCenterAlignmentIncludesSignWithinFieldWidth) {
  EXPECT_EQ(fmt::format("{:^5}", -42), " -42 ");
  EXPECT_EQ(fmt::format("{:^6}", -42), " -42  ");
  EXPECT_EQ(fmt::format("{:*^6}", -42), "*-42**");
}

TEST(FmtFormatSignAlignmentTest, ZeroFlagBehavesAsNumericAlignmentAndOverridesDefaultRightPaddingBehavior) {
  EXPECT_EQ(fmt::format("{:5}", -42), "  -42");
  EXPECT_EQ(fmt::format("{:05}", -42), "-0042");

  EXPECT_EQ(fmt::format("{:+5}", 42), "  +42");
  EXPECT_EQ(fmt::format("{:+05}", 42), "+0042");
}

TEST(FmtFormatSignAlignmentTest, WidthZeroOrLessThanContentDoesNotCorruptSignHandling) {
  EXPECT_EQ(fmt::format("{:0}", -42), "-42");
  EXPECT_EQ(fmt::format("{:+0}", 42), "+42");
  EXPECT_EQ(fmt::format("{:1}", -42), "-42");
  EXPECT_EQ(fmt::format("{:+1}", 42), "+42");
}

TEST(FmtFormatSignAlignmentTest, DynamicWidthWithNumericAlignmentStillCountsSignInsideWidth) {
  EXPECT_EQ(fmt::format("{:0{}}", -42, 5), "-0042");
  EXPECT_EQ(fmt::format("{:+0{}}", 42, 5), "+0042");
  EXPECT_EQ(fmt::format("{:>{}}", -42, 5), "  -42");
  EXPECT_EQ(fmt::format("{:<{}}", -42, 5), "-42  ");
}

TEST(FmtFormatSignAlignmentTest, NegativeZeroFormattingPreservesSignAndAlignmentRules) {
  const double neg_zero = -0.0;
  EXPECT_EQ(fmt::format("{:05.0f}", neg_zero), "-0000");
  EXPECT_EQ(fmt::format("{:>5.0f}", neg_zero), "   -0");
  EXPECT_EQ(fmt::format("{:<5.0f}", neg_zero), "-0   ");
}

TEST(FmtFormatSignAlignmentTest, CombinationOfAlternateFormAndNumericAlignmentKeepsSignFirst) {
  EXPECT_EQ(fmt::format("{:#06x}", 0x2a), "0x002a");
  EXPECT_EQ(fmt::format("{:#06X}", 0x2a), "0X002A");
  EXPECT_EQ(fmt::format("{:#08o}", 0123), "00000123");
}

TEST(FmtFormatSignAlignmentTest, SignAwarePaddingForFloatingPoint) {
  EXPECT_EQ(fmt::format("{:+08.2f}", 12.5), "+0012.50");
  EXPECT_EQ(fmt::format("{:08.2f}", -12.5), "-0012.50");
  EXPECT_EQ(fmt::format("{:8.2f}", -12.5), "  -12.50");
}

TEST(FmtFormatSignAlignmentTest, ErrorOnInvalidDynamicWidthStillOccursThroughPublicApi) {
  EXPECT_THROW((void)fmt::format("{:0{}}", 42, -1), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:>{}}", 42, -1), fmt::format_error);
}

}  // namespace