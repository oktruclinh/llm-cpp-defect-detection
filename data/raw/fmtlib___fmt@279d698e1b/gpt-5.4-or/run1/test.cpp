#include <gtest/gtest.h>

#include <fmt/format.h>

#include <limits>
#include <string>

TEST(FmtFormatPaddingAndSignTest, RightAlignmentAddsPaddingBeforeNegativeSignAndDigits) {
  EXPECT_EQ(fmt::format("{:>6}", -42), "   -42");
  EXPECT_EQ(fmt::format("{:>8}", -1), "      -1");
}

TEST(FmtFormatPaddingAndSignTest, RightAlignmentWithZeroWidthDoesNotPad) {
  EXPECT_EQ(fmt::format("{:>0}", -42), "-42");
  EXPECT_EQ(fmt::format("{:>0}", 42), "42");
  EXPECT_EQ(fmt::format("{:>0}", 0), "0");
}

TEST(FmtFormatPaddingAndSignTest, WidthEqualToFormattedSizeProducesNoExtraPadding) {
  EXPECT_EQ(fmt::format("{:>3}", -42), "-42");
  EXPECT_EQ(fmt::format("{:>2}", 42), "42");
  EXPECT_EQ(fmt::format("{:>1}", 0), "0");
}

TEST(FmtFormatPaddingAndSignTest, WidthSmallerThanFormattedSizeProducesNoTruncation) {
  EXPECT_EQ(fmt::format("{:>2}", -999), "-999");
  EXPECT_EQ(fmt::format("{:>1}", 12345), "12345");
}

TEST(FmtFormatPaddingAndSignTest, EmptyFillBeforeNegativeNumberIsNotPossibleAndDefaultSpaceIsUsed) {
  EXPECT_EQ(fmt::format("{:>5}", -7), "   -7");
}

TEST(FmtFormatPaddingAndSignTest, DynamicWidthRightAlignmentWorksForNegativeValues) {
  EXPECT_EQ(fmt::format("{:>{}}", -42, 6), "   -42");
  EXPECT_EQ(fmt::format("{:>{}}", -42, 3), "-42");
  EXPECT_EQ(fmt::format("{:>{}}", -42, 2), "-42");
  EXPECT_EQ(fmt::format("{:>{}}", -42, 0), "-42");
}

TEST(FmtFormatPaddingAndSignTest, SignOptionForPositiveValuesCountsTowardWidthAndPadsOnLeft) {
  EXPECT_EQ(fmt::format("{:>5}", 42), "   42");
  EXPECT_EQ(fmt::format("{:+>5}", 42), "+++42"); // custom fill, right align
  EXPECT_EQ(fmt::format("{:+5}", 42), "  +42");
  EXPECT_EQ(fmt::format("{: 5}", 42), "   42");
}

TEST(FmtFormatPaddingAndSignTest, MinusSignRemainsAdjacentToDigitsWhenSpacePadded) {
  std::string s = fmt::format("{:>10}", -123);
  ASSERT_EQ(s.size(), 10u);
  EXPECT_EQ(s, "      -123");
  EXPECT_EQ(s[6], '-');
  EXPECT_EQ(s.substr(7), "123");
}

TEST(FmtFormatPaddingAndSignTest, AlternateFormHexNegativeStillPadsOnLeftWithoutMovingMinus) {
  EXPECT_EQ(fmt::format("{:#8x}", -42), "   -0x2a");
  EXPECT_EQ(fmt::format("{:#10X}", -42), "     -0X2A");
}

TEST(FmtFormatPaddingAndSignTest, ZeroValueRightAlignmentPadsCorrectly) {
  EXPECT_EQ(fmt::format("{:>4}", 0), "   0");
  EXPECT_EQ(fmt::format("{:+4}", 0), "  +0");
  EXPECT_EQ(fmt::format("{: 4}", 0), "   0");
}

TEST(FmtFormatPaddingAndSignTest, StringRightAlignmentUsesSamePaddingPathForNonNumericData) {
  EXPECT_EQ(fmt::format("{:>5}", std::string{}), "     ");
  EXPECT_EQ(fmt::format("{:>5}", "x"), "    x");
  EXPECT_EQ(fmt::format("{:>3}", "abcd"), "abcd");
}

TEST(FmtFormatPaddingAndSignTest, CharacterRightAlignmentPadsCorrectly) {
  EXPECT_EQ(fmt::format("{:>4}", 'A'), "   A");
  EXPECT_EQ(fmt::format("{:>1}", 'A'), "A");
}

TEST(FmtFormatPaddingAndSignTest, LargeNegativeIntegerFormatsWithoutCorruptingLeadingSign) {
  int v = std::numeric_limits<int>::min();
  std::string expected = std::to_string(v);
  std::string padded = fmt::format("{:>15}", v);

  ASSERT_EQ(padded.size(), 15u);
  EXPECT_EQ(padded.substr(15 - expected.size()), expected);
  EXPECT_EQ(padded, std::string(15 - expected.size(), ' ') + expected);
}

TEST(FmtFormatPaddingAndSignTest, LongLongMinFormatsCorrectlyWithPadding) {
  long long v = std::numeric_limits<long long>::min();
  std::string expected = std::to_string(v);
  std::string padded = fmt::format("{:>25}", v);

  ASSERT_EQ(padded.size(), 25u);
  EXPECT_EQ(padded, std::string(25 - expected.size(), ' ') + expected);
}

TEST(FmtFormatPaddingAndSignTest, InvalidDynamicWidthThrowsFormatError) {
  EXPECT_THROW((void)fmt::format("{:>{}}", 42, -1), fmt::format_error);
}

TEST(FmtFormatPaddingAndSignTest, InvalidFormatSpecifiersThrowFormatError) {
  EXPECT_THROW((void)fmt::format("{:>}", 1), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:>>5}", 1), fmt::format_error);
}