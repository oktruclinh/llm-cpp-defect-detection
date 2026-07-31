#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>

// Tests for the write() overloads in fmt::detail::buffer and related machinery.
// These are exercised through fmt::format_to and fmt::format.

// ===================== long double write =====================

TEST(LongDoubleWriteTest, Basic) {
  std::string result = fmt::format("{}", 3.14159265358979323846L);
  // The default format for long double should produce a reasonable string.
  EXPECT_FALSE(result.empty());
  EXPECT_TRUE(result.find("3.14") != std::string::npos ||
              result.find("3,14") != std::string::npos); // locale complicates
}

TEST(LongDoubleWriteTest, Zero) {
  std::string result = fmt::format("{}", 0.0L);
  EXPECT_EQ("0", result);
}

TEST(LongDoubleWriteTest, NegativeZero) {
  std::string result = fmt::format("{}", -0.0L);
  EXPECT_EQ("-0", result);
}

TEST(LongDoubleWriteTest, Infinity) {
  std::string result = fmt::format("{}", std::numeric_limits<long double>::infinity());
  EXPECT_EQ("inf", result);
}

TEST(LongDoubleWriteTest, NegativeInfinity) {
  std::string result = fmt::format("{}", -std::numeric_limits<long double>::infinity());
  EXPECT_EQ("-inf", result);
}

TEST(LongDoubleWriteTest, NaN) {
  std::string result = fmt::format("{}", std::numeric_limits<long double>::quiet_NaN());
  EXPECT_EQ("nan", result);
}

TEST(LongDoubleWriteTest, VeryLarge) {
  long double large = 1e308L;
  std::string result = fmt::format("{}", large);
  EXPECT_TRUE(result.find('e') != std::string::npos || result.find('E') != std::string::npos);
}

TEST(LongDoubleWriteTest, VerySmall) {
  long double small = 1e-308L;
  std::string result = fmt::format("{}", small);
  EXPECT_FALSE(result.empty());
  EXPECT_TRUE(result.find('e') != std::string::npos || result.find('E') != std::string::npos);
}

TEST(LongDoubleWriteTest, MaxPrecision) {
  long double val = 1.23456789012345678901234567890L;
  std::string result = fmt::format("{:.20Lf}", val);
  // Just check it starts correctly and has enough digits.
  EXPECT_TRUE(result.find("1.234567890123456789") != std::string::npos ||
              result.find("1,234567890123456789") != std::string::npos);
}

TEST(LongDoubleWriteTest, FormatSpec) {
  std::string result = fmt::format("{:.2e}", 123.456L);
  EXPECT_EQ("1.23e+02", result);
}

TEST(LongDoubleWriteTest, HexFloat) {
  std::string result = fmt::format("{:.4a}", 1.0L);
  EXPECT_EQ("0x1.0000p+0", result);
}

// ===================== char write =====================

TEST(CharWriteTest, Basic) {
  std::string result = fmt::format("{}", 'A');
  EXPECT_EQ("A", result);
}

TEST(CharWriteTest, NullChar) {
  std::string result = fmt::format("{}", '\0');
  EXPECT_EQ(1, result.size());
  EXPECT_EQ('\0', result[0]);
}

TEST(CharWriteTest, Newline) {
  std::string result = fmt::format("{}", '\n');
  EXPECT_EQ("\n", result);
}

TEST(CharWriteTest, Space) {
  std::string result = fmt::format("{}", ' ');
  EXPECT_EQ(" ", result);
}

TEST(CharWriteTest, Digit) {
  std::string result = fmt::format("{}", '9');
  EXPECT_EQ("9", result);
}

TEST(CharWriteTest, Bracket) {
  std::string result = fmt::format("{}", '[');
  EXPECT_EQ("[", result);
}

TEST(CharWriteTest, WithFormatSpecInt) {
  std::string result = fmt::format("{:d}", 'A');
  EXPECT_EQ("65", result);
}

TEST(CharWriteTest, WithFormatSpecHex) {
  std::string result = fmt::format("{:x}", 255);
  EXPECT_EQ("ff", result);
}

TEST(CharWriteTest, WithFormatSpecChar) {
  std::string result = fmt::format("{:c}", 65);
  EXPECT_EQ("A", result);
}

// ===================== wchar_t write =====================

TEST(WcharWriteTest, Basic) {
  std::wstring result = fmt::format(L"{}", L'A');
  EXPECT_EQ(L"A", result);
}

TEST(WcharWriteTest, NullChar) {
  std::wstring result = fmt::format(L"{}", L'\0');
  EXPECT_EQ(1, result.size());
  EXPECT_EQ(L'\0', result[0]);
}

TEST(WcharWriteTest, WideChar) {
  std::wstring result = fmt::format(L"{}", L'\u00E9'); // é
  EXPECT_EQ(L"\u00E9", result);
}

TEST(WcharWriteTest, WithFormatSpecInt) {
  std::wstring result = fmt::format(L"{:d}", L'A');
  EXPECT_EQ(L"65", result);
}

TEST(WcharWriteTest, WithFormatSpecChar) {
  std::wstring result = fmt::format(L"{:c}", 65);
  EXPECT_EQ(L"A", result);
}

// ===================== Edge cases =====================

TEST(CharWriteEdgeTest, WriteToBackInserter) {
  std::string out;
  auto it = std::back_inserter(out);
  // This exercises the write(char) path via a different output iterator.
  fmt::format_to(it, "{}", 'X');
  EXPECT_EQ("X", out);
}

TEST(LongDoubleWriteEdgeTest, WriteToBackInserter) {
  std::string out;
  auto it = std::back_inserter(out);
  fmt::format_to(it, "{}", 1.5L);
  EXPECT_EQ("1.5", out);
}

TEST(LongDoubleWriteEdgeTest, Negative) {
  std::string result = fmt::format("{}", -42.5L);
  EXPECT_EQ("-42.5", result);
}

TEST(CharWriteEdgeTest, NegativeIntAsChar) {
  std::string result = fmt::format("{:c}", -1);
  // -1 cast to char is implementation-defined, but typically 0xFF or similar.
  // We just check it doesn't crash and produces something.
  EXPECT_EQ(1, result.size());
}

// ===================== Format string with embedded chars =====================

TEST(CharWriteCombinedTest, MultipleChars) {
  std::string result = fmt::format("{}{}{}", 'a', 'b', 'c');
  EXPECT_EQ("abc", result);
}

TEST(LongDoubleWriteCombinedTest, MultipleValues) {
  std::string result = fmt::format("{} and {}", 1.5L, 2.5L);
  EXPECT_EQ("1.5 and 2.5", result);
}