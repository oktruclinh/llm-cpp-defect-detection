#include <fmt/format.h>
#include <gtest/gtest.h>

// Helper function to format an integer with specific specs and return the result as a string.
template <typename T>
std::string format_integer(T value, fmt::string_view format_str) {
  // Use fmt::format directly with the format string, as fmt::runtime is for dynamic format strings
  // that are not known at compile time. Here, format_str is known at compile time for the format() call.
  return fmt::format(format_str, value);
}

TEST(FmtFormatIntegerTest, BasicFormatting) {
  // Test basic integer formatting
  EXPECT_EQ(format_integer(123, "{}"), "123");
  EXPECT_EQ(format_integer(-456, "{}"), "-456");
  EXPECT_EQ(format_integer(0, "{}"), "0");
}

TEST(FmtFormatIntegerTest, WidthAndAlignment) {
  // Test width and alignment
  EXPECT_EQ(format_integer(123, "{:5}"), "  123");
  EXPECT_EQ(format_integer(123, "{:<5}"), "123  ");
  EXPECT_EQ(format_integer(123, "{:^5}"), " 123 ");
  EXPECT_EQ(format_integer(-123, "{:5}"), " -123");
  EXPECT_EQ(format_integer(-123, "{:<5}"), "-123 ");
  EXPECT_EQ(format_integer(-123, "{:^5}"), "-123 "); // Centered negative number
  EXPECT_EQ(format_integer(0, "{:5}"), "    0");
}

TEST(FmtFormatIntegerTest, FillCharacter) {
  // Test fill character
  EXPECT_EQ(format_integer(123, "{:*<5}"), "123**");
  EXPECT_EQ(format_integer(123, "{:*>5}"), "**123");
  EXPECT_EQ(format_integer(123, "{:*^5}"), "*123*");
  EXPECT_EQ(format_integer(-123, "{:*^6}"), "*-123*");
  EXPECT_EQ(format_integer(-123, "{:*>6}"), "**-123");
}

TEST(FmtFormatIntegerTest, SignSpecifier) {
  // Test sign specifiers
  EXPECT_EQ(format_integer(123, "{:+}"), "+123");
  EXPECT_EQ(format_integer(-123, "{:+}"), "-123");
  EXPECT_EQ(format_integer(0, "{:+}"), "+0");

  EXPECT_EQ(format_integer(123, "{: }"), " 123");
  EXPECT_EQ(format_integer(-123, "{: }"), "-123");
  EXPECT_EQ(format_integer(0, "{: }"), " 0");

  EXPECT_EQ(format_integer(123, "{:-}"), "123"); // Default for positive
  EXPECT_EQ(format_integer(-123, "{:-}"), "-123");
  EXPECT_EQ(format_integer(0, "{:-}"), "0");
}

TEST(FmtFormatIntegerTest, ZeroPadding) {
  // Test zero padding
  EXPECT_EQ(format_integer(123, "{:05}"), "00123");
  EXPECT_EQ(format_integer(-123, "{:05}"), "-0123");
  EXPECT_EQ(format_integer(0, "{:05}"), "00000");
  EXPECT_EQ(format_integer(123, "{:01}"), "123"); // Width less than number of digits
}

TEST(FmtFormatIntegerTest, CombinedSpecs) {
  // Test combined specifiers
  EXPECT_EQ(format_integer(123, "{:0=+6}"), "+00123");
  EXPECT_EQ(format_integer(-123, "{:0= 6}"), "-00123");
  EXPECT_EQ(format_integer(42, "{:*>+8}"), "***+0042");
  EXPECT_EQ(format_integer(-42, "{:*> 8}"), "***-0042");
}

TEST(FmtFormatIntegerTest, LargeNumbers) {
  // Test with large numbers (long long)
  EXPECT_EQ(format_integer(123456789012345LL, "{}"), "123456789012345");
  EXPECT_EQ(format_integer(-98765432109876LL, "{}"), "-98765432109876");
  EXPECT_EQ(format_integer(std::numeric_limits<long long>::max(), "{}"), "9223372036854775807");
  EXPECT_EQ(format_integer(std::numeric_limits<long long>::min(), "{}"), "-9223372036854775808");
}

TEST(FmtFormatIntegerTest, UnsignedNumbers) {
  // Test unsigned integers
  EXPECT_EQ(format_integer(123U, "{}"), "123");
  EXPECT_EQ(format_integer(0U, "{}"), "0");
  EXPECT_EQ(format_integer(std::numeric_limits<unsigned int>::max(), "{}"), "4294967295");
  EXPECT_EQ(format_integer(std::numeric_limits<unsigned long long>::max(), "{}"), "18446744073709551615");
}

TEST(FmtFormatIntegerTest, PrecisionSpecifierIgnoredForIntegers) {
  // Precision specifier is ignored for integers, but should not cause errors.
  EXPECT_EQ(format_integer(123, "{:.2}"), "123");
  EXPECT_EQ(format_integer(123, "{:5.2}"), "  123");
}

TEST(FmtFormatIntegerTest, AlternateFormIgnoredForDecimal) {
  // Alternate form (#) is ignored for decimal integers, but should not cause errors.
  EXPECT_EQ(format_integer(123, "{:#}"), "123");
  EXPECT_EQ(format_integer(123, "{:#5}"), "  123");
}

TEST(FmtFormatIntegerTest, NegativeWidth) {
  // Negative width is not standard and typically results in an error or is ignored.
  // fmtlib handles it by making it positive.
  EXPECT_EQ(format_integer(123, "{:-5}"), "123  "); // Equivalent to {:<5}
  EXPECT_EQ(format_integer(123, "{:--5}"), "123  "); // Equivalent to {:<5}
}

TEST(FmtFormatIntegerTest, EmptyFormatString) {
  // Empty format string should just convert to string
  EXPECT_EQ(format_integer(123, "{}"), "123");
  EXPECT_EQ(format_integer(-123, "{}"), "-123");
  EXPECT_EQ(format_integer(0, "{}"), "0");
}

TEST(FmtFormatIntegerTest, CustomFillWithZeroPadding) {
  // Custom fill character with zero padding is not allowed and should throw an error.
  // However, the focal function is about writing the padded output, not parsing the format string.
  // We test valid combinations that reach the focal function.
  // Zero padding implies '0' as fill char and right alignment, and sign handling.
  EXPECT_EQ(format_integer(123, "{:05}"), "00123");
  EXPECT_EQ(format_integer(-123, "{:05}"), "-0123");
  EXPECT_EQ(format_integer(123, "{:0=+6}"), "+00123");
  EXPECT_EQ(format_integer(-123, "{:0= 6}"), "-00123");
}

TEST(FmtFormatIntegerTest, MaxWidth) {
  // Test with a very large width to ensure buffer handling
  std::string expected_output(1000, ' ');
  expected_output += "123";
  EXPECT_EQ(format_integer(123, "{:1003}"), expected_output);

  expected_output = std::string(999, ' ');
  expected_output += "-123";
  EXPECT_EQ(format_integer(-123, "{:1003}"), expected_output);
}