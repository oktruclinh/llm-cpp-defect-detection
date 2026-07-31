#include <gtest/gtest.h>
#include "fmt/format.h"

// Helper function to format an integer as octal using fmtlib
template <typename T>
std::string format_octal(T value, fmt::string_view format_str = "") {
  // The original code used fmt::runtime which is for dynamic format strings.
  // For a format string constructed at compile time (or effectively constant
  // within this helper), fmt::format directly accepts it.
  // The issue was trying to use fmt::runtime on the result of fmt::format,
  // which is a std::string, not a format string literal.
  // The correct way to pass a dynamically constructed format string to fmt::format
  // is to use fmt::runtime() on the *format string itself*.
  // However, in this specific case, the format string is constructed as
  // "{{:{}}}", format_str. This results in a string like "{:o}" or "{:#o}".
  // This string can then be passed to fmt::format directly.
  return fmt::format(fmt::format("{{:{}}}", format_str), value);
}

// Test fixture for common setup if needed, though for simple formatting, direct tests are often fine.
class OctalFormattingTest : public ::testing::Test {
protected:
  // You can set up common objects here if multiple tests need them.
};

// Test case for basic positive integer octal formatting
TEST_F(OctalFormattingTest, BasicPositiveInteger) {
  EXPECT_EQ(format_octal(10, "o"), "12");
  EXPECT_EQ(format_octal(15, "o"), "17");
  EXPECT_EQ(format_octal(64, "o"), "100");
  EXPECT_EQ(format_octal(255, "o"), "377");
}

// Test case for basic negative integer octal formatting (should be treated as unsigned)
TEST_F(OctalFormattingTest, BasicNegativeInteger) {
  // fmtlib's default behavior for negative integers with 'o' is to treat them as unsigned.
  // For signed types, this means printing the two's complement representation.
  EXPECT_EQ(format_octal(-1, "o"), "37777777777"); // For 32-bit int
  EXPECT_EQ(format_octal(-8, "o"), "37777777770"); // For 32-bit int
  EXPECT_EQ(format_octal(-10, "o"), "37777777766"); // For 32-bit int
}

// Test case for zero
TEST_F(OctalFormattingTest, Zero) {
  EXPECT_EQ(format_octal(0, "o"), "0");
}

// Test case for alternative form (#)
TEST_F(OctalFormattingTest, AlternativeForm) {
  EXPECT_EQ(format_octal(10, "#o"), "012");
  EXPECT_EQ(format_octal(0, "#o"), "0"); // '0' prefix for zero
  EXPECT_EQ(format_octal(64, "#o"), "0100");
}

// Test case for alternative form with zero and precision
TEST_F(OctalFormattingTest, AlternativeFormZeroWithPrecision) {
  // When precision is greater than num_digits, the '0' prefix is not added
  // because precision already implies leading zeros.
  EXPECT_EQ(format_octal(0, "#0o"), "0"); // Precision 0, no effect
  EXPECT_EQ(format_octal(0, "#.1o"), "0"); // Precision 1, still '0'
  EXPECT_EQ(format_octal(0, "#.5o"), "00000"); // Precision 5, no '0' prefix
}

// Test case for alternative form with non-zero and precision
TEST_F(OctalFormattingTest, AlternativeFormNonZeroWithPrecision) {
  EXPECT_EQ(format_octal(10, "#.1o"), "012"); // Precision 1, num_digits 2, prefix added
  EXPECT_EQ(format_octal(10, "#.2o"), "012"); // Precision 2, num_digits 2, prefix added
  EXPECT_EQ(format_octal(10, "#.3o"), "0012"); // Precision 3, num_digits 2, prefix NOT added (precision handles leading zero)
  EXPECT_EQ(format_octal(10, "#.5o"), "00012"); // Precision 5, num_digits 2, prefix NOT added
  EXPECT_EQ(format_octal(64, "#.1o"), "0100");
  EXPECT_EQ(format_octal(64, "#.3o"), "0100");
  EXPECT_EQ(format_octal(64, "#.5o"), "00100");
}

// Test case for width
TEST_F(OctalFormattingTest, Width) {
  EXPECT_EQ(format_octal(10, "5o"), "   12");
  EXPECT_EQ(format_octal(10, "05o"), "00012"); // Zero-padding
  EXPECT_EQ(format_octal(10, "#5o"), "  012");
  EXPECT_EQ(format_octal(10, "#05o"), "00012"); // Zero-padding with alt form, '0' is part of padding
}

// Test case for width with negative numbers (treated as unsigned)
TEST_F(OctalFormattingTest, WidthNegative) {
  EXPECT_EQ(format_octal(-1, "12o"), " 37777777777"); // For 32-bit int
  EXPECT_EQ(format_octal(-1, "012o"), "037777777777"); // For 32-bit int
}

// Test case for precision
TEST_F(OctalFormattingTest, Precision) {
  EXPECT_EQ(format_octal(10, ".1o"), "12"); // Precision < num_digits, no effect
  EXPECT_EQ(format_octal(10, ".2o"), "12"); // Precision == num_digits, no effect
  EXPECT_EQ(format_octal(10, ".3o"), "012"); // Precision > num_digits, adds leading zeros
  EXPECT_EQ(format_octal(10, ".5o"), "00012");
  EXPECT_EQ(format_octal(0, ".3o"), "000"); // Zero with precision
}

// Test case for precision with alternative form
TEST_F(OctalFormattingTest, PrecisionWithAlternativeForm) {
  EXPECT_EQ(format_octal(10, "#.1o"), "012"); // Precision 1, num_digits 2, prefix added
  EXPECT_EQ(format_octal(10, "#.2o"), "012"); // Precision 2, num_digits 2, prefix added
  EXPECT_EQ(format_octal(10, "#.3o"), "0012"); // Precision 3, num_digits 2, prefix NOT added (precision handles leading zero)
  EXPECT_EQ(format_octal(10, "#.5o"), "00012"); // Precision 5, num_digits 2, prefix NOT added
  EXPECT_EQ(format_octal(0, "#.3o"), "000"); // Zero with precision, no extra '0' prefix
}

// Test case for fill and alignment
TEST_F(OctalFormattingTest, FillAndAlignment) {
  EXPECT_EQ(format_octal(10, "*<5o"), "12***");
  EXPECT_EQ(format_octal(10, "*^5o"), "*12**");
  EXPECT_EQ(format_octal(10, "*>5o"), "***12");
  EXPECT_EQ(format_octal(10, "*>#5o"), "**012");
}

// Test case for large numbers (unsigned long long)
TEST_F(OctalFormattingTest, LargeUnsignedLongLong) {
  unsigned long long large_num = 0xFFFFFFFFFFFFFFFFULL; // Max ULL
  EXPECT_EQ(format_octal(large_num, "o"), "1777777777777777777777");
  EXPECT_EQ(format_octal(large_num, "#o"), "01777777777777777777777");

  unsigned long long another_large = 123456789012345ULL;
  EXPECT_EQ(format_octal(another_large, "o"), "4323456705060505");
  EXPECT_EQ(format_octal(another_large, "#o"), "04323456705060505");
}

// Test case for different integer types
TEST_F(OctalFormattingTest, DifferentIntegerTypes) {
  short s_val = 10;
  EXPECT_EQ(format_octal(s_val, "o"), "12");
  unsigned short us_val = 10;
  EXPECT_EQ(format_octal(us_val, "o"), "12");
  long l_val = 1000;
  EXPECT_EQ(format_octal(l_val, "o"), "1750");
  unsigned long ul_val = 1000;
  EXPECT_EQ(format_octal(ul_val, "o"), "1750");
}

// Test case for combination of flags
TEST_F(OctalFormattingTest, CombinationOfFlags) {
  EXPECT_EQ(format_octal(10, "#05o"), "00012");
  EXPECT_EQ(format_octal(10, "#.5o"), "00012");
  EXPECT_EQ(format_octal(10, "*^#7.5o"), "*00012*"); // Center align, alt, precision, width
  EXPECT_EQ(format_octal(0, "#05o"), "00000"); // Zero, alt, zero-pad, width
}

// Test case for large width and precision
TEST_F(OctalFormattingTest, LargeWidthAndPrecision) {
  EXPECT_EQ(format_octal(10, ".10o"), "0000000012");
  EXPECT_EQ(format_octal(10, "15.10o"), "     0000000012");
  EXPECT_EQ(format_octal(10, "#15.10o"), "    0000000012"); // '0' prefix is absorbed by precision
}

// Test case for minimum value of signed int
TEST_F(OctalFormattingTest, MinSignedInt) {
  // For 32-bit int, INT_MIN is -2147483648.
  // As octal, it's treated as unsigned: 2^31 in octal is 20000000000.
  // So, -2147483648 is 20000000000 (octal) for 32-bit.
  // For 64-bit, it would be 177777777777777777777760 (octal)
  // Let's use a specific value that works for 32-bit for consistency.
  int min_int_val = -2147483648; // INT_MIN for 32-bit
  EXPECT_EQ(format_octal(min_int_val, "o"), "20000000000");
  EXPECT_EQ(format_octal(min_int_val, "#o"), "020000000000");
}

// Test case for maximum value of signed int
TEST_F(OctalFormattingTest, MaxSignedInt) {
  int max_int_val = 2147483647; // INT_MAX for 32-bit
  EXPECT_EQ(format_octal(max_int_val, "o"), "17777777777");
  EXPECT_EQ(format_octal(max_int_val, "#o"), "017777777777");
}