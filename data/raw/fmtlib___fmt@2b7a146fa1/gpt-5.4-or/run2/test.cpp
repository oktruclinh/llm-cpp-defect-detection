#include <gtest/gtest.h>

#include <fmt/format.h>
#include <limits>
#include <string>

namespace {

template <typename T>
std::string FormatWithGrouping(T value) {
  return fmt::format("{:L}", value);
}

TEST(FmtFormatLocalizedGroupingTest, NoGroupingForSingleAndSmallNumbers) {
  EXPECT_EQ(FormatWithGrouping(0), "0");
  EXPECT_EQ(FormatWithGrouping(1), "1");
  EXPECT_EQ(FormatWithGrouping(9), "9");
  EXPECT_EQ(FormatWithGrouping(10), "10");
  EXPECT_EQ(FormatWithGrouping(999), "999");
}

TEST(FmtFormatLocalizedGroupingTest, InsertsThousandsSeparatorsAtBoundaries) {
  EXPECT_EQ(FormatWithGrouping(1000), "1,000");
  EXPECT_EQ(FormatWithGrouping(1001), "1,001");
  EXPECT_EQ(FormatWithGrouping(9999), "9,999");
  EXPECT_EQ(FormatWithGrouping(10000), "10,000");
  EXPECT_EQ(FormatWithGrouping(999999), "999,999");
  EXPECT_EQ(FormatWithGrouping(1000000), "1,000,000");
}

TEST(FmtFormatLocalizedGroupingTest, HandlesMultipleGroupingSteps) {
  EXPECT_EQ(FormatWithGrouping(1234567), "1,234,567");
  EXPECT_EQ(FormatWithGrouping(123456789), "123,456,789");
  EXPECT_EQ(FormatWithGrouping(1234567890), "1,234,567,890");
  EXPECT_EQ(FormatWithGrouping(1234567890123LL), "1,234,567,890,123");
}

TEST(FmtFormatLocalizedGroupingTest, NegativeNumbersUseAbsoluteValueForDigitCounting) {
  EXPECT_EQ(FormatWithGrouping(-1), "-1");
  EXPECT_EQ(FormatWithGrouping(-999), "-999");
  EXPECT_EQ(FormatWithGrouping(-1000), "-1,000");
  EXPECT_EQ(FormatWithGrouping(-1001), "-1,001");
  EXPECT_EQ(FormatWithGrouping(-1234567), "-1,234,567");
}

TEST(FmtFormatLocalizedGroupingTest, SignedExtremaAreFormattedWithCorrectGrouping) {
  EXPECT_EQ(FormatWithGrouping(std::numeric_limits<int>::max()), "2,147,483,647");
  EXPECT_EQ(FormatWithGrouping(std::numeric_limits<int>::min()), "-2,147,483,648");

  EXPECT_EQ(FormatWithGrouping(std::numeric_limits<long long>::max()),
            "9,223,372,036,854,775,807");
  EXPECT_EQ(FormatWithGrouping(std::numeric_limits<long long>::min()),
            "-9,223,372,036,854,775,808");
}

TEST(FmtFormatLocalizedGroupingTest, UnsignedExtremaAreFormattedWithCorrectGrouping) {
  EXPECT_EQ(FormatWithGrouping(std::numeric_limits<unsigned>::max()),
            "4,294,967,295");
  EXPECT_EQ(FormatWithGrouping(std::numeric_limits<unsigned long long>::max()),
            "18,446,744,073,709,551,615");
}

TEST(FmtFormatLocalizedGroupingTest, LocaleSpecifierDoesNotChangeNonDecimalPresentation) {
  EXPECT_EQ(fmt::format("{:Lx}", 0x1234abcd), "1234abcd");
  EXPECT_EQ(fmt::format("{:Lb}", 42), "101010");
}

TEST(FmtFormatLocalizedGroupingTest, LocaleSpecifierWorksWithDynamicWidthAndAlignment) {
  EXPECT_EQ(fmt::format("{:>10L}", 1000), "     1,000");
  EXPECT_EQ(fmt::format("{:<10L}", 1000), "1,000     ");
  EXPECT_EQ(fmt::format("{:^11L}", 1000), "   1,000   ");
}

TEST(FmtFormatLocalizedGroupingTest, GroupingAffectsComputedWidthByAddingSeparators) {
  EXPECT_EQ(fmt::format("{:7L}", 1000), "  1,000");
  EXPECT_EQ(fmt::format("{:6L}", 1000), " 1,000");
  EXPECT_EQ(fmt::format("{:5L}", 1000), "1,000");

  EXPECT_EQ(fmt::format("{:13L}", 1000000000), "1,000,000,000");
  EXPECT_EQ(fmt::format("{:14L}", 1000000000), " 1,000,000,000");
}

TEST(FmtFormatLocalizedGroupingTest, SignAndGroupingInteractCorrectlyWithWidth) {
  EXPECT_EQ(fmt::format("{:8L}", -1000), "  -1,000");
  EXPECT_EQ(fmt::format("{:7L}", -1000), " -1,000");
  EXPECT_EQ(fmt::format("{:6L}", -1000), "-1,000");
}

TEST(FmtFormatLocalizedGroupingTest, PrecisionIsRejectedForIntegersEvenWithLocaleSpecifier) {
  EXPECT_THROW(fmt::format("{:.2L}", 42), fmt::format_error);
  EXPECT_THROW(fmt::format("{:.0L}", 1000), fmt::format_error);
}

}  // namespace