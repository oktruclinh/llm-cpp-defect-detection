#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>

// Test write(long double) via fmt::format
TEST(WriteLongDoubleTest, BasicValues) {
  EXPECT_EQ(fmt::format("{}", 3.14159265358979323846L), "3.141592653589793");
  EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
  EXPECT_EQ(fmt::format("{}", -2.5L), "-2.5");
}

TEST(WriteLongDoubleTest, InfinityAndNaN) {
  long double inf = std::numeric_limits<long double>::infinity();
  long double nan = std::numeric_limits<long double>::quiet_NaN();
  EXPECT_EQ(fmt::format("{}", inf), "inf");
  EXPECT_EQ(fmt::format("{}", -inf), "-inf");
  EXPECT_EQ(fmt::format("{}", nan), "nan");
}

TEST(WriteLongDoubleTest, VeryLargeAndSmall) {
  long double large = 1e308L;
  long double small = 1e-308L;
  EXPECT_EQ(fmt::format("{}", large), "1e+308");
  EXPECT_EQ(fmt::format("{}", small), "1e-308");
}

TEST(WriteLongDoubleTest, ZeroAndNegativeZero) {
  EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
  EXPECT_EQ(fmt::format("{}", -0.0L), "-0.0");
}

// Test write(char) via fmt::format
TEST(WriteCharTest, BasicCharacters) {
  EXPECT_EQ(fmt::format("{}", 'a'), "a");
  EXPECT_EQ(fmt::format("{}", 'Z'), "Z");
  EXPECT_EQ(fmt::format("{}", '0'), "0");
}

TEST(WriteCharTest, SpecialCharacters) {
  EXPECT_EQ(fmt::format("{}", '\n'), "\n");
  EXPECT_EQ(fmt::format("{}", '\t'), "\t");
  EXPECT_EQ(fmt::format("{}", '\0'), std::string(1, '\0'));
}

TEST(WriteCharTest, NonPrintableCharacters) {
  EXPECT_EQ(fmt::format("{}", '\x01'), std::string(1, '\x01'));
  EXPECT_EQ(fmt::format("{}", '\x7F'), std::string(1, '\x7F'));
}

// Test write(wchar_t) via fmt::format (only if wchar_t is supported)
TEST(WriteWcharTest, BasicWideCharacters) {
  EXPECT_EQ(fmt::format(L"{}", L'a'), L"a");
  EXPECT_EQ(fmt::format(L"{}", L'Z'), L"Z");
  EXPECT_EQ(fmt::format(L"{}", L'0'), L"0");
}

TEST(WriteWcharTest, WideSpecialCharacters) {
  EXPECT_EQ(fmt::format(L"{}", L'\n'), L"\n");
  EXPECT_EQ(fmt::format(L"{}", L'\t'), L"\t");
  EXPECT_EQ(fmt::format(L"{}", L'\0'), std::wstring(1, L'\0'));
}

TEST(WriteWcharTest, WideNonPrintableCharacters) {
  EXPECT_EQ(fmt::format(L"{}", L'\x01'), std::wstring(1, L'\x01'));
  EXPECT_EQ(fmt::format(L"{}", L'\x7F'), std::wstring(1, L'\x7F'));
}

// Edge cases for write(char) - format specifiers
TEST(WriteCharTest, WithFormatSpecifiers) {
  EXPECT_EQ(fmt::format("{:c}", 'x'), "x");
  EXPECT_EQ(fmt::format("{:d}", 'A'), "65");
  EXPECT_EQ(fmt::format("{:x}", 'B'), "42");
  EXPECT_EQ(fmt::format("{:o}", 'C'), "103");
}

// Edge cases for write(long double) - format specifiers
TEST(WriteLongDoubleTest, WithFormatSpecifiers) {
  EXPECT_EQ(fmt::format("{:.2f}", 1.23456789L), "1.23");
  EXPECT_EQ(fmt::format("{:.0f}", 9.9L), "10");
  EXPECT_EQ(fmt::format("{:+g}", 5.0L), "+5");
  EXPECT_EQ(fmt::format("{:e}", 1000.0L), "1.000000e+03");
}

// Test that write(char) works in a larger formatted string
TEST(WriteCharTest, InLargerString) {
  EXPECT_EQ(fmt::format("char: {} int: {}", 'x', 42), "char: x int: 42");
}

// Test that write(wchar_t) works in a larger formatted string
TEST(WriteWcharTest, InLargerWideString) {
  EXPECT_EQ(fmt::format(L"char: {} int: {}", L'x', 42), L"char: x int: 42");
}

// Test that write(long double) works in a larger formatted string
TEST(WriteLongDoubleTest, InLargerString) {
  EXPECT_EQ(fmt::format("pi = {}, e = {}", 3.14159265358979323846L, 2.71828182845904523536L),
            "pi = 3.141592653589793, e = 2.718281828459045");
}