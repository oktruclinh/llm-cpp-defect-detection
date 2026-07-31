#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <climits>

// Test write(long double) via fmt::format
TEST(WriteLongDoubleTest, BasicValues) {
  EXPECT_EQ(fmt::format("{}", 3.14L), "3.14");
  EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
  EXPECT_EQ(fmt::format("{}", -2.5L), "-2.5");
}

TEST(WriteLongDoubleTest, LargeAndSmallValues) {
  EXPECT_EQ(fmt::format("{}", 1e100L), "1e+100");
  EXPECT_EQ(fmt::format("{}", 1e-100L), "1e-100");
  EXPECT_EQ(fmt::format("{}", 1.0L / 3.0L), "0.3333333333333333");
}

TEST(WriteLongDoubleTest, InfinityAndNaN) {
  EXPECT_EQ(fmt::format("{}", std::numeric_limits<long double>::infinity()), "inf");
  EXPECT_EQ(fmt::format("{}", -std::numeric_limits<long double>::infinity()), "-inf");
  EXPECT_TRUE(fmt::format("{}", std::numeric_limits<long double>::quiet_NaN()) == "nan" ||
              fmt::format("{}", std::numeric_limits<long double>::quiet_NaN()) == "-nan");
}

TEST(WriteLongDoubleTest, ZeroAndNegativeZero) {
  EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
  EXPECT_EQ(fmt::format("{}", -0.0L), "-0.0");
}

TEST(WriteLongDoubleTest, FormatSpecs) {
  EXPECT_EQ(fmt::format("{:.2f}", 3.14159L), "3.14");
  EXPECT_EQ(fmt::format("{:+.3e}", 1.23456L), "+1.235e+00");
  EXPECT_EQ(fmt::format("{:10.4g}", 12345.6789L), "    1.235e+04");
}

// Test write(char) via fmt::format
TEST(WriteCharTest, BasicChar) {
  EXPECT_EQ(fmt::format("{}", 'a'), "a");
  EXPECT_EQ(fmt::format("{}", 'Z'), "Z");
  EXPECT_EQ(fmt::format("{}", '0'), "0");
}

TEST(WriteCharTest, SpecialChars) {
  EXPECT_EQ(fmt::format("{}", '\n'), "\n");
  EXPECT_EQ(fmt::format("{}", '\t'), "\t");
  EXPECT_EQ(fmt::format("{}", '\0'), std::string(1, '\0'));
}

TEST(WriteCharTest, CharInFormatString) {
  EXPECT_EQ(fmt::format("{} {}", 'x', 'y'), "x y");
  EXPECT_EQ(fmt::format("{:c}", 65), "A");
}

TEST(WriteCharTest, CharWithWidth) {
  EXPECT_EQ(fmt::format("{:5}", 'a'), "a    ");
  EXPECT_EQ(fmt::format("{:<5}", 'b'), "b    ");
  EXPECT_EQ(fmt::format("{:>5}", 'c'), "    c");
  EXPECT_EQ(fmt::format("{:^5}", 'd'), "  d  ");
}

// Test write(wchar_t) via fmt::format (only if wchar_t is supported)
TEST(WriteWcharTest, BasicWchar) {
  EXPECT_EQ(fmt::format(L"{}", L'a'), L"a");
  EXPECT_EQ(fmt::format(L"{}", L'Z'), L"Z");
  EXPECT_EQ(fmt::format(L"{}", L'0'), L"0");
}

TEST(WriteWcharTest, SpecialWchars) {
  EXPECT_EQ(fmt::format(L"{}", L'\n'), L"\n");
  EXPECT_EQ(fmt::format(L"{}", L'\t'), L"\t");
  EXPECT_EQ(fmt::format(L"{}", L'\0'), std::wstring(1, L'\0'));
}

TEST(WriteWcharTest, WcharInFormatString) {
  EXPECT_EQ(fmt::format(L"{} {}", L'x', L'y'), L"x y");
  EXPECT_EQ(fmt::format(L"{:c}", static_cast<wchar_t>(65)), L"A");
}

TEST(WriteWcharTest, WcharWithWidth) {
  EXPECT_EQ(fmt::format(L"{:5}", L'a'), L"a    ");
  EXPECT_EQ(fmt::format(L"{:<5}", L'b'), L"b    ");
  EXPECT_EQ(fmt::format(L"{:>5}", L'c'), L"    c");
  EXPECT_EQ(fmt::format(L"{:^5}", L'd'), L"  d  ");
}

// Edge cases: empty string via char/wchar_t (though write(char) writes a single char)
TEST(WriteCharEdgeTest, CharBoundaryValues) {
  EXPECT_EQ(fmt::format("{}", static_cast<char>(0)), std::string(1, '\0'));
  EXPECT_EQ(fmt::format("{}", static_cast<char>(127)), std::string(1, static_cast<char>(127)));
  EXPECT_EQ(fmt::format("{}", static_cast<char>(-128)), std::string(1, static_cast<char>(-128)));
}

TEST(WriteWcharEdgeTest, WcharBoundaryValues) {
  EXPECT_EQ(fmt::format(L"{}", static_cast<wchar_t>(0)), std::wstring(1, L'\0'));
  EXPECT_EQ(fmt::format(L"{}", static_cast<wchar_t>(65535)), std::wstring(1, static_cast<wchar_t>(65535)));
}

// Test that write(long double) handles extreme values correctly
TEST(WriteLongDoubleEdgeTest, MinMaxValues) {
  long double max_val = std::numeric_limits<long double>::max();
  long double min_val = std::numeric_limits<long double>::min();
  long double denorm_min = std::numeric_limits<long double>::denorm_min();

  EXPECT_FALSE(fmt::format("{}", max_val).empty());
  EXPECT_FALSE(fmt::format("{}", min_val).empty());
  EXPECT_FALSE(fmt::format("{}", denorm_min).empty());
}

// Test that write(char) and write(wchar_t) work with format specifiers that affect char output
TEST(WriteCharFormatTest, CharWithFillAndAlign) {
  EXPECT_EQ(fmt::format("{:*>5}", 'a'), "****a");
  EXPECT_EQ(fmt::format("{:*<5}", 'b'), "b****");
  EXPECT_EQ(fmt::format("{:*^5}", 'c'), "**c**");
}

TEST(WriteWcharFormatTest, WcharWithFillAndAlign) {
  EXPECT_EQ(fmt::format(L"{:*>5}", L'a'), L"****a");
  EXPECT_EQ(fmt::format(L"{:*<5}", L'b'), L"b****");
  EXPECT_EQ(fmt::format(L"{:*^5}", L'c'), L"**c**");
}