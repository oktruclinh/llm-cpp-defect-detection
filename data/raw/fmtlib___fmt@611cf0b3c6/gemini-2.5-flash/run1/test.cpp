```cpp
#include <gtest/gtest.h>
#include "fmt/format.h"
#include "fmt/core.h" // For fmt::format

// Helper function to format an integer as octal with specific format specs
std::string format_octal(int value, const std::string& format_str) {
  return fmt::format(fmt::runtime("{:" + format_str + "}"), value);
}

TEST(FmtOctalTest, BasicOctalFormatting) {
  // Positive number
  EXPECT_EQ(format_octal(10, "o"), "12");
  // Zero
  EXPECT_EQ(format_octal(0, "o"), "0");
  // Negative number (should be formatted as positive magnitude with sign)
  EXPECT_EQ(format_octal(-10, "o"), "-12");
  // Large number
  EXPECT_EQ(format_octal(12345, "o"), "30071");
}

TEST(FmtOctalTest, AltFormPrefix) {
  // Positive number with alt form
  EXPECT_EQ(format_octal(10, "#o"), "012");
  // Zero with alt form (prefix should not be added if value is 0)
  EXPECT_EQ(format_octal(0, "#o"), "0");
  // Negative number with alt form
  EXPECT_EQ(format_octal(-10, "#o"), "-012");
  // Large number with alt form
  EXPECT_EQ(format_octal(12345, "#o"), "030071");
}

TEST(FmtOctalTest, WidthPadding) {
  // Right padding
  EXPECT_EQ(format_octal(10, "5o"), "   12");
  // Left padding
  EXPECT_EQ(format_octal(10, "<5o"), "12   ");
  // Center padding
  EXPECT_EQ(format_octal(10, "^5o"), " 12  ");
  // Zero padding
  EXPECT_EQ(format_octal(10, "05o"), "00012");
  // Zero padding with alt form
  EXPECT_EQ(format_octal(10, "#05o"), "00012"); // Prefix '0' is part of the width
  EXPECT_EQ(format_octal(123, "#05o"), "00173");
  EXPECT_EQ(format_octal(123, "#06o"), "000173");
  EXPECT_EQ(format_octal(0, "#05o"), "00000"); // Zero with alt form and zero padding
}

TEST(FmtOctalTest, PrecisionSpecifier) {
  // Precision less than digits, no alt form
  EXPECT_EQ(format_octal(10, ".1o"), "12");
  // Precision equal to digits, no alt form
  EXPECT_EQ(format_octal(10, ".2o"), "12");
  // Precision greater than digits, no alt form
  EXPECT_EQ(format_octal(10, ".5o"), "00012");

  // Precision less than digits, with alt form
  EXPECT_EQ(format_octal(10, "#.1o"), "012"); // Prefix '0' is added, then padded
  EXPECT_EQ(format_octal(10, "#.2o"), "012"); // Prefix '0' is added, then padded
  // Precision greater than digits, with alt form
  EXPECT_EQ(format_octal(10, "#.5o"), "000012"); // Prefix '0' is added, then padded to 5 digits (excluding prefix)

  // Zero with precision
  EXPECT_EQ(format_octal(0, ".5o"), "00000");
  EXPECT_EQ(format_octal(0, "#.5o"), "00000"); // Alt form does not add prefix for 0
  EXPECT_EQ(format_octal(0, ".0o"), ""); // Precision 0 for 0 value
  EXPECT_EQ(format_octal(0, "#.0o"), ""); // Precision 0 for 0 value with alt form
}

TEST(FmtOctalTest, SignSpecifiers) {
  // Positive sign
  EXPECT_EQ(format_octal(10, "+o"), "+12");
  EXPECT_EQ(format_octal(-10, "+o"), "-12");
  // Space sign
  EXPECT_EQ(format_octal(10, " o"), " 12");
  EXPECT_EQ(format_octal(-10, " o"), "-12");
  // No sign (default)
  EXPECT_EQ(format_octal(10, "o"), "12");
  EXPECT_EQ(format_octal(-10, "o"), "-12");

  // Sign with alt form
  EXPECT_EQ(format_octal(10, "+#o"), "+012");
  EXPECT_EQ(format_octal(-10, "+#o"), "-012");
  EXPECT_EQ(format_octal(10, " #o"), " 012");
  EXPECT_EQ(format_octal(-10, " #o"), "-012");
}

TEST(FmtOctalTest, CombinedSpecs) {
  // Width, alt, zero padding
  EXPECT_EQ(format_octal(10, "#07o"), "0000012");
  EXPECT_EQ(format_octal(12345, "#07o"), "0030071");

  // Width, alt, precision
  EXPECT_EQ(format_octal(10, "#.5o"), "000012");
  EXPECT_EQ(format_octal(10, "#7.5o"), " 000012"); // Width applies to the whole string including prefix and padding
  EXPECT_EQ(format_octal(10, "#07.5o"), "0000012"); // Zero padding with precision, precision takes precedence for digits

  // Sign, width, alt, precision
  EXPECT_EQ(format_octal(10, "+#08.5o"), "+0000012");
  EXPECT_EQ(format_octal(-10, "+#08.5o"), "-0000012");
}

TEST(FmtOctalTest, MaxIntValue) {
  // Max int value
  EXPECT_EQ(format_octal(std::numeric_limits<int>::max(), "o"), "17777777777");
  EXPECT_EQ(format_octal(std::numeric_limits<int>::max(), "#o"), "017777777777");
  EXPECT_EQ(format_octal(std::numeric_limits<int>::max(), "#015o"), "000017777777777");
}

TEST(FmtOctalTest, MinIntValue) {
  // Min int value
  EXPECT_EQ(format_octal(std::numeric_limits<int>::min(), "o"), "-20000000000");
  EXPECT_EQ(format_octal(std::numeric_limits<int>::min(), "#o"), "-020000000000");
  EXPECT_EQ(format_octal(std::numeric_limits<int>::min(), "#015o"), "-000020000000000");
}

TEST(FmtOctalTest, UnsignedInt) {
  // Unsigned int
  EXPECT_EQ(format_octal(static_cast<unsigned int>(10), "o"), "12");
  EXPECT_EQ(format_octal(static_cast<unsigned int>(0), "o"), "0");
  EXPECT_EQ(format_octal(static_cast<unsigned int>(10), "#o"), "012");
  EXPECT_EQ(format_octal(std::numeric_limits<unsigned int>::max(), "o"), "37777777777");
  EXPECT_EQ(format_octal(std::numeric_limits<unsigned int>::max(), "#o"), "037777777777");
}

TEST(FmtOctalTest, LongLong) {
  // Long long
  EXPECT_EQ(format_octal(123456789012345LL, "o"), "1373204713200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000