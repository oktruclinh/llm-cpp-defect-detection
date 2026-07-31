#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/core.h> // For fmt::format_to

// This test suite focuses on the behavior of write_padded, specifically
// when used with align::right and a custom lambda for copying data.
// The focal function snippet is part of the internal implementation of
// formatting numbers, particularly handling negative signs and padding.

TEST(FmtWritePaddedRightTest, PositiveNumberNoPadding) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:}", 123);
  EXPECT_EQ(s, "123");
}

TEST(FmtWritePaddedRightTest, PositiveNumberWithPadding) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:5}", 123);
  EXPECT_EQ(s, "  123"); // Right aligned, padded with spaces
}

TEST(FmtWritePaddedRightTest, PositiveNumberWithZeroPadding) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:05}", 123);
  EXPECT_EQ(s, "00123"); // Right aligned, padded with zeros
}

TEST(FmtWritePaddedRightTest, PositiveNumberWithCustomFill) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:*5}", 123);
  EXPECT_EQ(s, "**123"); // Right aligned, padded with asterisks
}

TEST(FmtWritePaddedRightTest, NegativeNumberNoPadding) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:}", -123);
  EXPECT_EQ(s, "-123");
}

TEST(FmtWritePaddedRightTest, NegativeNumberWithPadding) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:5}", -123);
  EXPECT_EQ(s, " -123"); // Right aligned, padded with spaces, sign included in width
}

TEST(FmtWritePaddedRightTest, NegativeNumberWithZeroPadding) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:05}", -123);
  EXPECT_EQ(s, "-0123"); // Right aligned, padded with zeros, sign before zeros
}

TEST(FmtWritePaddedRightTest, NegativeNumberWithCustomFill) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:*5}", -123);
  EXPECT_EQ(s, "-*123"); // Right aligned, padded with asterisks, sign before fill
}

TEST(FmtWritePaddedRightTest, NegativeNumberWithZeroPaddingAndPlusSign) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:+05}", -123);
  EXPECT_EQ(s, "-0123"); // Plus sign has no effect on negative numbers
}

TEST(FmtWritePaddedRightTest, PositiveNumberWithZeroPaddingAndPlusSign) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:+05}", 123);
  EXPECT_EQ(s, "+0123"); // Plus sign is printed for positive numbers
}

TEST(FmtWritePaddedRightTest, ZeroValueNoPadding) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:}", 0);
  EXPECT_EQ(s, "0");
}

TEST(FmtWritePaddedRightTest, ZeroValueWithPadding) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:3}", 0);
  EXPECT_EQ(s, "  0");
}

TEST(FmtWritePaddedRightTest, ZeroValueWithZeroPadding) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:03}", 0);
  EXPECT_EQ(s, "000");
}

TEST(FmtWritePaddedRightTest, NumberLargerThanWidth) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:3}", 12345);
  EXPECT_EQ(s, "12345"); // Width is ignored if number is larger
}

TEST(FmtWritePaddedRightTest, NegativeNumberLargerThanWidth) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:3}", -12345);
  EXPECT_EQ(s, "-12345"); // Width is ignored if number is larger
}

TEST(FmtWritePaddedRightTest, MaxInt) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:15}", std::numeric_limits<int>::max());
  EXPECT_EQ(s, fmt::format("{:15}", std::numeric_limits<int>::max()));
}

TEST(FmtWritePaddedRightTest, MinInt) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:15}", std::numeric_limits<int>::min());
  EXPECT_EQ(s, fmt::format("{:15}", std::numeric_limits<int>::min()));
}

TEST(FmtWritePaddedRightTest, LongLong) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:25}", std::numeric_limits<long long>::max());
  EXPECT_EQ(s, fmt::format("{:25}", std::numeric_limits<long long>::max()));
}

TEST(FmtWritePaddedRightTest, UnsignedLongLong) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:25}", std::numeric_limits<unsigned long long>::max());
  EXPECT_EQ(s, fmt::format("{:25}", std::numeric_limits<unsigned long long>::max()));
}

TEST(FmtWritePaddedRightTest, CharType) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:5}", 'A');
  EXPECT_EQ(s, "   65"); // Char formatted as integer by default
}

TEST(FmtWritePaddedRightTest, CharTypeWithZeroPadding) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:05}", 'A');
  EXPECT_EQ(s, "00065");
}

TEST(FmtWritePaddedRightTest, WideCharType) {
  std::wstring ws;
  fmt::format_to(std::back_inserter(ws), L"{:5}", L'B');
  EXPECT_EQ(ws, L"   66");
}

TEST(FmtWritePaddedRightTest, WideCharTypeWithZeroPadding) {
  std::wstring ws;
  fmt::format_to(std::back_inserter(ws), L"{:05}", L'B');
  EXPECT_EQ(ws, L"00066");
}

TEST(FmtWritePaddedRightTest, DifferentAlignmentsIgnoredForNumbersWithZeroPadding) {
  // When zero-padding is used, alignment specifiers other than fill are often ignored
  // because the sign is handled specially and zeros fill the space.
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:<05}", -123); // Left align with zero padding
  EXPECT_EQ(s, "-0123"); // Still behaves like right align with sign handling
}

TEST(FmtWritePaddedRightTest, DifferentAlignmentsIgnoredForNumbersWithZeroPaddingPositive) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:<05}", 123); // Left align with zero padding
  EXPECT_EQ(s, "00123"); // Still behaves like right align
}

TEST(FmtWritePaddedRightTest, AlternateFormHex) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:#06x}", 0xAF);
  EXPECT_EQ(s, "0x00af"); // 0x prefix, zero-padded
}

TEST(FmtWritePaddedRightTest, AlternateFormHexUppercase) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:#06X}", 0xAF);
  EXPECT_EQ(s, "0X00AF"); // 0X prefix, zero-padded
}

TEST(FmtWritePaddedRightTest, AlternateFormOctal) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:#06o}", 077);
  EXPECT_EQ(s, "000077"); // 0 prefix, zero-padded
}

TEST(FmtWritePaddedRightTest, AlternateFormBinary) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{:#010b}", 5); // 5 is 101 in binary
  EXPECT_EQ(s, "0b00000101"); // 0b prefix, zero-padded
}

TEST(FmtWritePaddedRightTest, AlternateFormBinaryNegative) {
  std::string s;
  // Binary formatting for negative numbers is implementation-defined or not directly supported
  // for signed types in a standard way by fmtlib (it usually formats the absolute value or uses two's complement).
  // For simplicity, we'll test with unsigned or assume the absolute value is formatted.
  // The focal function deals with the sign separately.
  fmt::format_to(std::back_inserter(s), "{:#010b}", -5);
  // fmtlib formats negative numbers in binary by taking the absolute value and adding a sign.
  EXPECT_EQ(s, "-0b0000101");
}

TEST(FmtWritePaddedRightTest, NegativeNumberWithSpaceSign) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{: 5}", -123);
  EXPECT_EQ(s, " -123"); // Space sign specifier has no effect on negative numbers
}

TEST(FmtWritePaddedRightTest, PositiveNumberWithSpaceSign) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{: 5}", 123);
  EXPECT_EQ(s, "  123"); // Space is added before positive numbers
}

TEST(FmtWritePaddedRightTest, PositiveNumberWithSpaceSignAndZeroPadding) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{: 05}", 123);
  EXPECT_EQ(s, " 0123"); // Space is added, then zero padding
}

TEST(FmtWritePaddedRightTest, ZeroWithSpaceSignAndZeroPadding) {
  std::string s;
  fmt::format_to(std::back_inserter(s), "{: 03}", 0);
  EXPECT_EQ(s, " 00"); // Space is added, then zero padding
}