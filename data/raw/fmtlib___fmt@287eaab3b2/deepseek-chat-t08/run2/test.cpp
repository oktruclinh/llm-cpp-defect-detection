#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>

// Test write(long double) via fmt::format
TEST(WriteLongDoubleTest, NormalValues) {
  EXPECT_EQ(fmt::format("{}", 3.141592653589793238L), "3.141592653589793");
  EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
  EXPECT_EQ(fmt::format("{}", -2.5L), "-2.5");
}

TEST(WriteLongDoubleTest, LargeAndSmall) {
  EXPECT_EQ(fmt::format("{}", 1e20L), "100000000000000000000");
  EXPECT_EQ(fmt::format("{}", 1e-10L), "0.0000000001");
}

TEST(WriteLongDoubleTest, InfinityAndNaN) {
  long double inf = std::numeric_limits<long double>::infinity();
  long double nan = std::numeric_limits<long double>::quiet_NaN();
  EXPECT_EQ(fmt::format("{}", inf), "inf");
  EXPECT_EQ(fmt::format("{}", -inf), "-inf");
  std::string nan_str = fmt::format("{}", nan);
  EXPECT_TRUE(nan_str == "nan" || nan_str == "-nan" || nan_str == "NaN" || nan_str == "-NaN");
}

TEST(WriteLongDoubleTest, Zero) {
  EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
  EXPECT_EQ(fmt::format("{}", -0.0L), "-0.0");
}

TEST(WriteLongDoubleTest, Subnormal) {
  long double smallest = std::numeric_limits<long double>::denorm_min();
  std::string result = fmt::format("{}", smallest);
  EXPECT_FALSE(result.empty());
  EXPECT_NE(result, "0.0");
}

// Test write(char) via fmt::format
TEST(WriteCharTest, NormalChar) {
  EXPECT_EQ(fmt::format("{}", 'A'), "A");
  EXPECT_EQ(fmt::format("{}", 'z'), "z");
  EXPECT_EQ(fmt::format("{}", '0'), "0");
}

TEST(WriteCharTest, ControlCharacters) {
  EXPECT_EQ(fmt::format("{}", '\n'), "\n");
  EXPECT_EQ(fmt::format("{}", '\t'), "\t");
  EXPECT_EQ(fmt::format("{}", '\0'), std::string(1, '\0'));
}

TEST(WriteCharTest, NonAsciiChar) {
  char c = static_cast<char>(200);
  std::string expected(1, c);
  EXPECT_EQ(fmt::format("{}", c), expected);
}

TEST(WriteCharTest, MinAndMaxChar) {
  char min_char = std::numeric_limits<char>::min();
  char max_char = std::numeric_limits<char>::max();
  std::string min_expected(1, min_char);
  std::string max_expected(1, max_char);
  EXPECT_EQ(fmt::format("{}", min_char), min_expected);
  EXPECT_EQ(fmt::format("{}", max_char), max_expected);
}

// Test write(wchar_t) via public API using fmt::format
TEST(WriteWcharTest, BasicWideChar) {
  EXPECT_EQ(fmt::format(L"{}", L'A'), L"A");
}

TEST(WriteWcharTest, WideCharValues) {
  EXPECT_EQ(fmt::format(L"{}", L'\n'), L"\n");
  EXPECT_EQ(fmt::format(L"{}", L'\0'), std::wstring(1, L'\0'));
}

TEST(WriteWcharTest, WideCharBoundaries) {
  wchar_t min_w = std::numeric_limits<wchar_t>::min();
  wchar_t max_w = std::numeric_limits<wchar_t>::max();

  std::wstring min_result = fmt::format(L"{}", min_w);
  EXPECT_EQ(min_result, std::wstring(1, min_w));

  std::wstring max_result = fmt::format(L"{}", max_w);
  EXPECT_FALSE(max_result.empty());
}

// Additional edge cases for long double: very large exponents
TEST(WriteLongDoubleTest, ExtremeExponents) {
  long double huge = std::numeric_limits<long double>::max();
  std::string huge_str = fmt::format("{}", huge);
  EXPECT_FALSE(huge_str.empty());
  EXPECT_NE(huge_str, "inf");

  long double tiny = std::numeric_limits<long double>::min();
  std::string tiny_str = fmt::format("{}", tiny);
  EXPECT_FALSE(tiny_str.empty());
  EXPECT_NE(tiny_str, "0.0");
}

// Test that write(char) works inside format strings with other content
TEST(WriteCharTest, WithFormatSpecifiers) {
  EXPECT_EQ(fmt::format("{:c}", 65), "A");
  EXPECT_EQ(fmt::format("{:4}", 'x'), "x   ");
  EXPECT_EQ(fmt::format("{:*<4}", 'y'), "y***");
}

// Test that write(long double) with format specifiers works
TEST(WriteLongDoubleTest, WithFormatSpecifiers) {
  EXPECT_EQ(fmt::format("{:.2f}", 3.14159L), "3.14");
  EXPECT_EQ(fmt::format("{:+g}", 1.0L), "+1");
  EXPECT_EQ(fmt::format("{:10.4e}", 12345.6789L), "1.2346e+04");
}