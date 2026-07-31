#include <gtest/gtest.h>

#include <limits>
#include <string>
#include <type_traits>

#include <fmt/format.h>

namespace {

TEST(FmtWriteCharTest, FormatsSingleChar) {
  EXPECT_EQ(fmt::format("{}", 'A'), "A");
  EXPECT_EQ(fmt::format("{}", 'z'), "z");
  EXPECT_EQ(fmt::format("{}", '0'), "0");
}

TEST(FmtWriteCharTest, FormatsNullCharAndPreservesLength) {
  std::string s = fmt::format("{}{}{}", 'A', '\0', 'B');

  ASSERT_EQ(s.size(), 3u);
  EXPECT_EQ(s[0], 'A');
  EXPECT_EQ(s[1], '\0');
  EXPECT_EQ(s[2], 'B');
}

TEST(FmtWriteCharTest, FormatsControlCharacter) {
  std::string s = fmt::format("{}{}", '\n', 'X');

  ASSERT_EQ(s.size(), 2u);
  EXPECT_EQ(s[0], '\n');
  EXPECT_EQ(s[1], 'X');
}

TEST(FmtWriteCharTest, AppliesAlignmentAndWidthToChar) {
  EXPECT_EQ(fmt::format("{:>3}", 'Q'), "  Q");
  EXPECT_EQ(fmt::format("{:<3}", 'Q'), "Q  ");
  EXPECT_EQ(fmt::format("{:^3}", 'Q'), " Q ");
}

TEST(FmtWriteCharTest, AppliesCustomFillToChar) {
  EXPECT_EQ(fmt::format("{:*^5}", 'X'), "**X**");
  EXPECT_EQ(fmt::format("{:#>4}", 'X'), "###X");
  EXPECT_EQ(fmt::format("{:.<4}", 'X'), "X...");
}

TEST(FmtWriteCharTest, WidthZeroOrOneDoesNotAddUnexpectedCharacters) {
  EXPECT_EQ(fmt::format("{:1}", 'M'), "M");
  EXPECT_EQ(fmt::format("{}", 'M'), "M");
}

TEST(FmtWriteLongDoubleTest, FormatsZero) {
  EXPECT_EQ(fmt::format("{}", static_cast<long double>(0.0L)), "0");
  EXPECT_EQ(fmt::format("{:.0f}", static_cast<long double>(0.0L)), "0");
}

TEST(FmtWriteLongDoubleTest, FormatsNegativeZeroWithSign) {
  long double neg_zero = -0.0L;
  EXPECT_EQ(fmt::format("{:+}", neg_zero), "-0");
}

TEST(FmtWriteLongDoubleTest, FormatsFiniteValuesWithDefaultAndFixedPrecision) {
  EXPECT_EQ(fmt::format("{:.1f}", static_cast<long double>(1.25L)), "1.2");
  EXPECT_EQ(fmt::format("{:.2f}", static_cast<long double>(1.25L)), "1.25");
  EXPECT_EQ(fmt::format("{:.3f}", static_cast<long double>(-12.5L)), "-12.500");
}

TEST(FmtWriteLongDoubleTest, FormatsSpecialValues) {
  const long double inf = std::numeric_limits<long double>::infinity();
  const long double nan = std::numeric_limits<long double>::quiet_NaN();

  EXPECT_EQ(fmt::format("{}", inf), "inf");
  EXPECT_EQ(fmt::format("{:+}", inf), "+inf");
  EXPECT_EQ(fmt::format("{}", -inf), "-inf");

  std::string nan_text = fmt::format("{}", nan);
  EXPECT_FALSE(nan_text.empty());
  EXPECT_NE(nan_text.find("nan"), std::string::npos);
}

TEST(FmtWriteLongDoubleTest, AppliesWidthAlignmentAndFill) {
  EXPECT_EQ(fmt::format("{:>8.2f}", static_cast<long double>(1.5L)), "    1.50");
  EXPECT_EQ(fmt::format("{:*<8.1f}", static_cast<long double>(2.0L)), "2.0*****");
  EXPECT_EQ(fmt::format("{:^7.0f}", static_cast<long double>(42.0L)), "  42   ");
}

TEST(FmtWriteLongDoubleTest, UsesLongDoubleOverloadInFormatArgStore) {
  long double value = 3.5L;
  EXPECT_EQ(fmt::format("value={:.1f}", value), "value=3.5");
}

#if defined(FMT_USE_WCHAR) || !defined(_WIN32) || defined(_WIN32)
TEST(FmtWriteWCharTest, FormatsSingleWideCharWithWformat) {
  std::wstring s = fmt::format(L"{}", L'Å');
  ASSERT_EQ(s.size(), 1u);
  EXPECT_EQ(s[0], L'Å');
}

TEST(FmtWriteWCharTest, FormatsNullWideCharAndPreservesLength) {
  std::wstring s = fmt::format(L"{}{}{}", L'A', L'\0', L'B');

  ASSERT_EQ(s.size(), 3u);
  EXPECT_EQ(s[0], L'A');
  EXPECT_EQ(s[1], L'\0');
  EXPECT_EQ(s[2], L'B');
}

TEST(FmtWriteWCharTest, AppliesAlignmentAndFillToWideChar) {
  EXPECT_EQ(fmt::format(L"{:>3}", L'Q'), std::wstring(L"  Q"));
  EXPECT_EQ(fmt::format(L"{:*^5}", L'X'), std::wstring(L"**X**"));
}
#endif

}  // namespace