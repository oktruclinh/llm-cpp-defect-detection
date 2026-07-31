#include <gtest/gtest.h>

#include <fmt/format.h>

#include <limits>
#include <string>

namespace {

TEST(FmtFormatPaddingAndSignTest, NegativeIntegerRightAlignedGetsSignBeforePadding) {
  EXPECT_EQ(fmt::format("{:>5}", -42), "  -42");
  EXPECT_EQ(fmt::format("{:>6}", -42), "   -42");
}

TEST(FmtFormatPaddingAndSignTest, NegativeIntegerWidthEqualToContentHasNoExtraPadding) {
  EXPECT_EQ(fmt::format("{:>3}", -42), "-42");
  EXPECT_EQ(fmt::format("{:>2}", -42), "-42");
  EXPECT_EQ(fmt::format("{}", -42), "-42");
}

TEST(FmtFormatPaddingAndSignTest, NegativeIntegerCustomFillAndRightAlignment) {
  EXPECT_EQ(fmt::format("{:*>6}", -42), "***-42");
  EXPECT_EQ(fmt::format("{:_>7}", -42), "____-42");
}

TEST(FmtFormatPaddingAndSignTest, NegativeIntegerDynamicWidthRightAlignment) {
  EXPECT_EQ(fmt::format("{:>{}}", -42, 6), "   -42");
  EXPECT_EQ(fmt::format("{:>{}}", -42, 3), "-42");
  EXPECT_EQ(fmt::format("{:>{}}", -42, 0), "-42");
}

TEST(FmtFormatPaddingAndSignTest, NegativeStringRightAlignmentPreservesLeadingMinus) {
  std::string s = "-x";
  EXPECT_EQ(fmt::format("{:>4}", s), "  -x");
  EXPECT_EQ(fmt::format("{:*>5}", s), "***-x");
}

TEST(FmtFormatPaddingAndSignTest, NegativeStringPrecisionAndRightAlignmentKeepMinusOutsideTruncationBuffer) {
  std::string s = "-abcdef";
  EXPECT_EQ(fmt::format("{:>6.3}", s), "  -abc");
  EXPECT_EQ(fmt::format("{:*>7.3}", s), "***-abc");
}

TEST(FmtFormatPaddingAndSignTest, NegativeStringPrecisionZeroProducesOnlyMinusWhenPresent) {
  std::string s = "-abcdef";
  EXPECT_EQ(fmt::format("{:>.0}", s), "-");
  EXPECT_EQ(fmt::format("{:>4.0}", s), "   -");
  EXPECT_EQ(fmt::format("{:*>4.0}", s), "***-");
}

TEST(FmtFormatPaddingAndSignTest, NonNegativeStringPrecisionZeroProducesOnlyPadding) {
  std::string s = "abcdef";
  EXPECT_EQ(fmt::format("{:>.0}", s), "");
  EXPECT_EQ(fmt::format("{:>4.0}", s), "    ");
  EXPECT_EQ(fmt::format("{:*>4.0}", s), "****");
}

TEST(FmtFormatPaddingAndSignTest, EmptyStringWithWidthIsPaddedCorrectly) {
  EXPECT_EQ(fmt::format("{:>4}", ""), "    ");
  EXPECT_EQ(fmt::format("{:*>4}", ""), "****");
  EXPECT_EQ(fmt::format("{:>0}", ""), "");
}

TEST(FmtFormatPaddingAndSignTest, SingleNegativeSignStringBoundaryCases) {
  std::string s = "-";
  EXPECT_EQ(fmt::format("{}", s), "-");
  EXPECT_EQ(fmt::format("{:>1}", s), "-");
  EXPECT_EQ(fmt::format("{:>3}", s), "  -");
  EXPECT_EQ(fmt::format("{:*>3}", s), "**-");
  EXPECT_EQ(fmt::format("{:.0}", s), "-");
  EXPECT_EQ(fmt::format("{:>3.0}", s), "  -");
}

TEST(FmtFormatPaddingAndSignTest, NegativeFloatingPointRightAlignmentPlacesSignBeforeDigits) {
  EXPECT_EQ(fmt::format("{:>8.2f}", -1.5), "   -1.50");
  EXPECT_EQ(fmt::format("{:*>8.2f}", -1.5), "***-1.50");
}

TEST(FmtFormatPaddingAndSignTest, LargeNegativeValueRightAlignment) {
  auto v = std::numeric_limits<long long>::min();
  auto expected = std::string(" ") + fmt::format("{}", v);
  EXPECT_EQ(fmt::format("{:>20}", v), expected);
}

TEST(FmtFormatPaddingAndSignTest, WidthSmallerThanFormattedNegativeFloatDoesNotTruncate) {
  EXPECT_EQ(fmt::format("{:>3.2f}", -1.5), "-1.50");
}

TEST(FmtFormatPaddingAndSignTest, DynamicWidthWithNegativeStringPrecisionZero) {
  std::string s = "-xyz";
  EXPECT_EQ(fmt::format("{:>{}.0}", s, 5), "    -");
}

TEST(FmtFormatPaddingAndSignTest, InvalidDynamicWidthThrowsFormatError) {
  EXPECT_THROW(fmt::format("{:>{}}", 42, -1), fmt::format_error);
}

TEST(FmtFormatPaddingAndSignTest, InvalidPrecisionThrowsFormatError) {
  EXPECT_THROW(fmt::format("{:.{}}", "abc", -1), fmt::format_error);
}

}  // namespace