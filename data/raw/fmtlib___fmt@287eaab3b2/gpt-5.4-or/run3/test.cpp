#include <gtest/gtest.h>

#include <limits>
#include <string>
#include <type_traits>

#include <fmt/format.h>

TEST(FmtFormatWriteCharTest, FormatsSingleCharWithFormat) {
  EXPECT_EQ(fmt::format("{}", 'A'), "A");
  EXPECT_EQ(fmt::format("prefix{}suffix", 'Z'), "prefixZsuffix");
}

TEST(FmtFormatWriteCharTest, FormatsNullCharPreservingLength) {
  std::string result = fmt::format("{}{}", '\0', 'X');
  ASSERT_EQ(result.size(), 2u);
  EXPECT_EQ(result[0], '\0');
  EXPECT_EQ(result[1], 'X');
}

TEST(FmtFormatWriteCharTest, FormatsBoundaryCharValues) {
  std::string zero = fmt::format("{}", static_cast<char>(0));
  ASSERT_EQ(zero.size(), 1u);
  EXPECT_EQ(static_cast<unsigned char>(zero[0]), 0u);

  std::string maxc = fmt::format("{}", std::numeric_limits<unsigned char>::max());
  EXPECT_EQ(maxc, "255");
}

TEST(FmtFormatWriteCharTest, WritesCharIntoMemoryBuffer) {
  fmt::memory_buffer buf;
  fmt::format_to(std::back_inserter(buf), "{}", 'q');

  std::string result(buf.data(), buf.size());
  EXPECT_EQ(result, "q");
}

TEST(FmtFormatWriteCharTest, WritesLiteralCharViaFormatToBuffer) {
  fmt::memory_buffer buf;
  fmt::format_to(std::back_inserter(buf), "{}{}{}", 'a', '\0', 'b');

  ASSERT_EQ(buf.size(), 3u);
  EXPECT_EQ(buf[0], 'a');
  EXPECT_EQ(buf[1], '\0');
  EXPECT_EQ(buf[2], 'b');
}

TEST(FmtFormatWriteWCharTest, FormatsSingleWideChar) {
  std::wstring result = fmt::format(L"{}", L'Ω');
  EXPECT_EQ(result, L"Ω");
}

TEST(FmtFormatWriteWCharTest, FormatsWideNullCharPreservingLength) {
  std::wstring result = fmt::format(L"{}{}", L'\0', L'Y');
  ASSERT_EQ(result.size(), 2u);
  EXPECT_EQ(result[0], L'\0');
  EXPECT_EQ(result[1], L'Y');
}

TEST(FmtFormatWriteWCharTest, WritesWideCharIntoWideMemoryBuffer) {
  fmt::basic_memory_buffer<wchar_t> buf;
  fmt::format_to(std::back_inserter(buf), L"{}{}", L'你', L'好');

  std::wstring result(buf.data(), buf.size());
  EXPECT_EQ(result, L"你好");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsZeroLongDouble) {
  long double value = 0.0L;
  EXPECT_EQ(fmt::format("{}", value), "0");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsNegativeLongDouble) {
  long double value = -42.5L;
  std::string s = fmt::format("{}", value);

  EXPECT_FALSE(s.empty());
  EXPECT_NE(s.find("-42.5"), std::string::npos);
}

TEST(FmtFormatWriteLongDoubleTest, HonorsPrecisionForLongDouble) {
  long double value = 1.0L / 3.0L;
  EXPECT_EQ(fmt::format("{:.3f}", value), "0.333");
  EXPECT_EQ(fmt::format("{:.0f}", 2.5L), "2");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsScientificNotationWhenRequested) {
  long double value = 1234.0L;
  std::string s = fmt::format("{:.2Le}", value);

  EXPECT_EQ(s, "1.23e+03");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsInfinityAndNaN) {
  long double inf = std::numeric_limits<long double>::infinity();
  long double nan = std::numeric_limits<long double>::quiet_NaN();

  std::string inf_str = fmt::format("{}", inf);
  std::string ninf_str = fmt::format("{}", -inf);
  std::string nan_str = fmt::format("{}", nan);

  EXPECT_EQ(inf_str, "inf");
  EXPECT_EQ(ninf_str, "-inf");
  EXPECT_EQ(nan_str, "nan");
}

TEST(FmtFormatWriteLongDoubleTest, WritesLongDoubleIntoMemoryBuffer) {
  fmt::memory_buffer buf;
  fmt::format_to(std::back_inserter(buf), "{:.1f}", 7.25L);

  std::string result(buf.data(), buf.size());
  EXPECT_EQ(result, "7.2");
}

TEST(FmtFormatWriteLongDoubleTest, CanAppendLongDoubleAndCharsTogether) {
  fmt::memory_buffer buf;
  fmt::format_to(std::back_inserter(buf), "{}{}{}", '[', 3.5L, ']');

  std::string result(buf.data(), buf.size());
  EXPECT_EQ(result, "[3.5]");
}