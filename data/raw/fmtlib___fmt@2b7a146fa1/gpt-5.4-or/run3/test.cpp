#include <gtest/gtest.h>

#include <limits>
#include <string>

#include <fmt/format.h>

namespace {

TEST(FmtGroupedDigitsTest, NoLocaleSpecifierProducesNoGrouping) {
  EXPECT_EQ(fmt::format("{}", 0), "0");
  EXPECT_EQ(fmt::format("{}", 7), "7");
  EXPECT_EQ(fmt::format("{}", 1234), "1234");
  EXPECT_EQ(fmt::format("{}", 1234567890), "1234567890");
  EXPECT_EQ(fmt::format("{}", -1234567890), "-1234567890");
}

TEST(FmtGroupedDigitsTest, LocaleGroupingForSingleDigitAndZeroDoesNotAddSeparator) {
  EXPECT_EQ(fmt::format("{:L}", 0), "0");
  EXPECT_EQ(fmt::format("{:L}", 5), "5");
  EXPECT_EQ(fmt::format("{:L}", -5), "-5");
}

TEST(FmtGroupedDigitsTest, LocaleGroupingForExactlyThreeDigitsDoesNotAddSeparator) {
  EXPECT_EQ(fmt::format("{:L}", 999), "999");
  EXPECT_EQ(fmt::format("{:L}", -999), "-999");
}

TEST(FmtGroupedDigitsTest, LocaleGroupingAddsSeparatorAtFirstBoundary) {
  EXPECT_EQ(fmt::format("{:L}", 1000), "1,000");
  EXPECT_EQ(fmt::format("{:L}", 1234), "1,234");
  EXPECT_EQ(fmt::format("{:L}", -1000), "-1,000");
  EXPECT_EQ(fmt::format("{:L}", -1234), "-1,234");
}

TEST(FmtGroupedDigitsTest, LocaleGroupingHandlesMultipleSeparators) {
  EXPECT_EQ(fmt::format("{:L}", 1234567), "1,234,567");
  EXPECT_EQ(fmt::format("{:L}", 123456789), "123,456,789");
  EXPECT_EQ(fmt::format("{:L}", -123456789), "-123,456,789");
}

TEST(FmtGroupedDigitsTest, LocaleGroupingHandlesValuesNearIntegerLimits) {
  EXPECT_EQ(fmt::format("{:L}", std::numeric_limits<int>::max()),
            "2,147,483,647");
  EXPECT_EQ(fmt::format("{:L}", std::numeric_limits<int>::min()),
            "-2,147,483,648");
}

TEST(FmtGroupedDigitsTest, LocaleGroupingHandlesValuesNearLongLongLimits) {
  EXPECT_EQ(fmt::format("{:L}", std::numeric_limits<long long>::max()),
            "9,223,372,036,854,775,807");
  EXPECT_EQ(fmt::format("{:L}", std::numeric_limits<long long>::min()),
            "-9,223,372,036,854,775,808");
}

TEST(FmtGroupedDigitsTest, WidthAccountsForInsertedGroupSeparators) {
  EXPECT_EQ(fmt::format("{:>6L}", 999), "   999");
  EXPECT_EQ(fmt::format("{:>6L}", 1000), " 1,000");
  EXPECT_EQ(fmt::format("{:>10L}", 1234567), " 1,234,567");
  EXPECT_EQ(fmt::format("{:<10L}", 1234567), "1,234,567 ");
}

TEST(FmtGroupedDigitsTest, ZeroFillWidthAccountsForGroupingAndSign) {
  EXPECT_EQ(fmt::format("{:08L}", 1234), "0001,234");
  EXPECT_EQ(fmt::format("{:09L}", -1234), "-001,234");
  EXPECT_EQ(fmt::format("{:013L}", 1234567), "00001,234,567");
}

TEST(FmtGroupedDigitsTest, PrecisionIsRejectedForIntegerLocalePresentation) {
  EXPECT_THROW((void)fmt::format("{:.2L}", 1234), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:.0L}", 0), fmt::format_error);
}

TEST(FmtGroupedDigitsTest, GroupingDoesNotAffectNonDecimalBases) {
  EXPECT_EQ(fmt::format("{:Lx}", 0x1234abcd), "1234abcd");
  EXPECT_EQ(fmt::format("{:Lb}", 0b101010101010), "101010101010");
}

}  // namespace