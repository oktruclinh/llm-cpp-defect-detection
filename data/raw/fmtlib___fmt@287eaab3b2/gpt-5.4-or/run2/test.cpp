#include <gtest/gtest.h>

#include <fmt/format.h>
#include <string>
#include <limits>

TEST(FmtFormatWriteCharTest, FormatsSingleCharWithDefaultFormatter) {
  EXPECT_EQ(fmt::format("{}", 'A'), "A");
  EXPECT_EQ(fmt::format("{}", 'z'), "z");
  EXPECT_EQ(fmt::format("{}", '0'), "0");
}

TEST(FmtFormatWriteCharTest, FormatsNullCharAndPreservesLength) {
  std::string result = fmt::format("{}", '\0');
  ASSERT_EQ(result.size(), 1u);
  EXPECT_EQ(result[0], '\0');
}

TEST(FmtFormatWriteCharTest, FormatsControlCharactersAsRawCharacters) {
  std::string newline = fmt::format("{}", '\n');
  ASSERT_EQ(newline.size(), 1u);
  EXPECT_EQ(newline[0], '\n');

  std::string tab = fmt::format("{}", '\t');
  ASSERT_EQ(tab.size(), 1u);
  EXPECT_EQ(tab[0], '\t');
}

TEST(FmtFormatWriteCharTest, SupportsWidthAlignmentAndFill) {
  EXPECT_EQ(fmt::format("{:>3}", 'X'), "  X");
  EXPECT_EQ(fmt::format("{:<3}", 'X'), "X  ");
  EXPECT_EQ(fmt::format("{:^5}", 'X'), "  X  ");
  EXPECT_EQ(fmt::format("{:*^5}", 'X'), "**X**");
}

TEST(FmtFormatWriteCharTest, RepeatedFormattingProducesIndependentCharacters) {
  EXPECT_EQ(fmt::format("{}{}{}", 'a', 'b', 'c'), "abc");
  EXPECT_EQ(fmt::format("[{}][{}]", 'L', 'R'), "[L][R]");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsZeroCorrectly) {
  EXPECT_EQ(fmt::format("{}", static_cast<long double>(0.0L)), "0");
  EXPECT_EQ(fmt::format("{:.1f}", static_cast<long double>(0.0L)), "0.0");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsNegativeZeroWithSign) {
  long double neg_zero = -0.0L;
  EXPECT_EQ(fmt::format("{:+f}", neg_zero), "-0.000000");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsSimpleFiniteValues) {
  EXPECT_EQ(fmt::format("{:.2f}", static_cast<long double>(1.25L)), "1.25");
  EXPECT_EQ(fmt::format("{:.3f}", static_cast<long double>(-2.5L)), "-2.500");
  EXPECT_EQ(fmt::format("{:.0f}", static_cast<long double>(42.0L)), "42");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsLargeLongDoubleWithLongDoublePrecision) {
  long double value = 1.0L / 3.0L;
  std::string result = fmt::format("{:.20Lf}", value);
  EXPECT_EQ(result, "0.33333333333333333334");
}

TEST(FmtFormatWriteLongDoubleTest, SupportsScientificAndGeneralFormats) {
  long double value = 1234.5L;
  EXPECT_EQ(fmt::format("{:.2Le}", value), "1.23e+03");
  EXPECT_EQ(fmt::format("{:.5Lg}", value), "1234.5");
}

TEST(FmtFormatWriteLongDoubleTest, SupportsWidthAlignmentAndSign) {
  EXPECT_EQ(fmt::format("{:>10.1f}", static_cast<long double>(12.0L)), "      12.0");
  EXPECT_EQ(fmt::format("{:<10.1f}", static_cast<long double>(12.0L)), "12.0      ");
  EXPECT_EQ(fmt::format("{:+.1f}", static_cast<long double>(12.0L)), "+12.0");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsInfinityAndNaN) {
  long double inf = std::numeric_limits<long double>::infinity();
  long double nan = std::numeric_limits<long double>::quiet_NaN();

  EXPECT_EQ(fmt::format("{}", inf), "inf");
  EXPECT_EQ(fmt::format("{:+}", inf), "+inf");

  std::string nan_result = fmt::format("{}", nan);
  EXPECT_EQ(nan_result, "nan");
}

TEST(FmtFormatWriteLongDoubleTest, ConcatenatesWithOtherArgumentsCorrectly) {
  std::string result = fmt::format("x={} y={}", static_cast<long double>(1.5L), 'Q');
  EXPECT_EQ(result, "x=1.5 y=Q");
}

TEST(FmtFormatWriteWCharTest, FormatsWideCharacterWithWideStringAPI) {
  std::wstring result = fmt::format(L"{}", L'A');
  EXPECT_EQ(result, L"A");

  std::wstring digit = fmt::format(L"{}", L'7');
  EXPECT_EQ(digit, L"7");
}

TEST(FmtFormatWriteWCharTest, FormatsWideNullCharacterAndPreservesLength) {
  std::wstring result = fmt::format(L"{}", L'\0');
  ASSERT_EQ(result.size(), 1u);
  EXPECT_EQ(result[0], L'\0');
}

TEST(FmtFormatWriteWCharTest, SupportsWidthAlignmentForWideCharacters) {
  EXPECT_EQ(fmt::format(L"{:>3}", L'X'), std::wstring(L"  X"));
  EXPECT_EQ(fmt::format(L"{:<3}", L'X'), std::wstring(L"X  "));
  EXPECT_EQ(fmt::format(L"{:*^5}", L'X'), std::wstring(L"**X**"));
}