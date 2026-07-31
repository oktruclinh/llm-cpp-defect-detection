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
  // Negative number (should be formatted as positive magnitude)
  EXPECT_EQ(format_octal(-10, "o"), "12");
  // Larger number
  EXPECT_EQ(format_octal(64, "o"), "100");
  EXPECT_EQ(format_octal(255, "o"), "377");
}

TEST(FmtOctalTest, AltFormPrefix) {
  // Positive number with alt form
  EXPECT_EQ(format_octal(10, "#o"), "012");
  // Zero with alt form (prefix should not be added if value is 0)
  EXPECT_EQ(format_octal(0, "#o"), "0");
  // Negative number with alt form
  EXPECT_EQ(format_octal(-10, "#o"), "012");
  // Larger number with alt form
  EXPECT_EQ(format_octal(64, "#o"), "0100");
}

TEST(FmtOctalTest, WidthSpecifier) {
  // Width greater than digits
  EXPECT_EQ(format_octal(10, "5o"), "   12");
  EXPECT_EQ(format_octal(10, "5#o"), "  012");
  // Width equal to digits
  EXPECT_EQ(format_octal(10, "2o"), "12");
  EXPECT_EQ(format_octal(10, "3#o"), "012");
  // Width less than digits (should not truncate)
  EXPECT_EQ(format_octal(10, "1o"), "12");
  EXPECT_EQ(format_octal(10, "2#o"), "012");
  // Zero with width
  EXPECT_EQ(format_octal(0, "3o"), "  0");
  EXPECT_EQ(format_octal(0, "3#o"), "  0");
}

TEST(FmtOctalTest, ZeroPadding) {
  // Zero padding with width
  EXPECT_EQ(format_octal(10, "05o"), "00012");
  EXPECT_EQ(format_octal(10, "05#o"), "00012"); // '0' prefix is handled differently
  EXPECT_EQ(format_octal(64, "05o"), "00100");
  EXPECT_EQ(format_octal(64, "05#o"), "00100"); // '0' prefix is handled differently

  // Zero padding with width and alt form, where prefix is added
  // The '0' prefix for octal is part of the number of digits for padding calculation
  EXPECT_EQ(format_octal(10, "#05o"), "00012");
  EXPECT_EQ(format_octal(64, "#05o"), "00100");
  EXPECT_EQ(format_octal(1, "#03o"), "001"); // 01, width 3 -> 001
  EXPECT_EQ(format_octal(7, "#03o"), "007"); // 07, width 3 -> 007
  EXPECT_EQ(format_octal(8, "#03o"), "010"); // 010, width 3 -> 010

  // Zero with zero padding
  EXPECT_EQ(format_octal(0, "03o"), "000");
  EXPECT_EQ(format_octal(0, "#03o"), "000");
}

TEST(FmtOctalTest, PrecisionSpecifier) {
  // Precision greater than digits
  EXPECT_EQ(format_octal(10, ".5o"), "00012");
  EXPECT_EQ(format_octal(10, ".5#o"), "00012"); // '0' prefix is part of precision
  EXPECT_EQ(format_octal(64, ".5o"), "00100");
  EXPECT_EQ(format_octal(64, ".5#o"), "00100");

  // Precision equal to digits
  EXPECT_EQ(format_octal(10, ".2o"), "12");
  EXPECT_EQ(format_octal(10, ".3#o"), "012");
  EXPECT_EQ(format_octal(64, ".3o"), "100");
  EXPECT_EQ(format_octal(64, ".4#o"), "0100");

  // Precision less than digits (should not truncate)
  EXPECT_EQ(format_octal(10, ".1o"), "12");
  EXPECT_EQ(format_octal(10, ".2#o"), "012");

  // Zero with precision
  EXPECT_EQ(format_octal(0, ".1o"), "0");
  EXPECT_EQ(format_octal(0, ".3o"), "000");
  EXPECT_EQ(format_octal(0, ".1#o"), "0"); // Alt form with 0, precision 1 -> "0"
  EXPECT_EQ(format_octal(0, ".3#o"), "000"); // Alt form with 0, precision 3 -> "000"

  // Precision 0 for non-zero value
  EXPECT_EQ(format_octal(10, ".0o"), "12");
  EXPECT_EQ(format_octal(10, ".0#o"), "012");

  // Precision 0 for zero value (should produce empty string)
  EXPECT_EQ(format_octal(0, ".0o"), "");
  EXPECT_EQ(format_octal(0, ".0#o"), ""); // Alt form with 0, precision 0 -> ""
}

TEST(FmtOctalTest, CombinedWidthAndPrecision) {
  // Width > precision > digits
  EXPECT_EQ(format_octal(10, "7.5o"), "  00012");
  EXPECT_EQ(format_octal(10, "7.5#o"), "  00012");

  // Width > digits > precision
  EXPECT_EQ(format_octal(10, "5.1o"), "   12");
  EXPECT_EQ(format_octal(10, "5.2#o"), "  012");

  // Precision > width > digits
  EXPECT_EQ(format_octal(10, "3.5o"), "00012");
  EXPECT_EQ(format_octal(10, "3.5#o"), "00012");

  // Zero with combined width and precision
  EXPECT_EQ(format_octal(0, "5.3o"), "  000");
  EXPECT_EQ(format_octal(0, "5.0o"), "     "); // Precision 0 for 0 value
  EXPECT_EQ(format_octal(0, "5.3#o"), "  000");
  EXPECT_EQ(format_octal(0, "5.0#o"), "     ");
}

TEST(FmtOctalTest, Alignment) {
  // Left alignment
  EXPECT_EQ(format_octal(10, "<5o"), "12   ");
  EXPECT_EQ(format_octal(10, "<5#o"), "012  ");
  EXPECT_EQ(format_octal(10, "<7.5o"), "00012  ");

  // Right alignment (default)
  EXPECT_EQ(format_octal(10, ">5o"), "   12");
  EXPECT_EQ(format_octal(10, ">5#o"), "  012");
  EXPECT_EQ(format_octal(10, ">7.5o"), "  00012");

  // Center alignment
  EXPECT_EQ(format_octal(10, "^5o"), " 12  ");
  EXPECT_EQ(format_octal(10, "^6o"), "  12  ");
  EXPECT_EQ(format_octal(10, "^5#o"), " 012 ");
  EXPECT_EQ(format_octal(10, "^7.5o"), " 00012 ");
}

TEST(FmtOctalTest, FillCharacter) {
  // Fill character with width
  EXPECT_EQ(format_octal(10, "*<5o"), "12***");
  EXPECT_EQ(format_octal(10, "x>5o"), "xxx12");
  EXPECT_EQ(format_octal(10, "y^6o"), "yy12yy");
  EXPECT_EQ(format_octal(10, "*<5#o"), "012**");
  EXPECT_EQ(format_octal(10, "x>5#o"), "xx012");
  EXPECT_EQ(format_octal(10, "y^6#o"), "yy012y");
}

TEST(FmtOctalTest, SignSpecifierIgnored) {
  // Sign specifiers should be ignored for octal
  EXPECT_EQ(format_octal(10, "+o"), "12");
  EXPECT_EQ(format_octal(10, " o"), "12");
  EXPECT_EQ(format_octal(-10, "+o"), "12");
  EXPECT_EQ(format_octal(-10, " o"), "12");
}

TEST(FmtOctalTest, LargeNumbers) {
  // Test with maximum int value
  EXPECT_EQ(format_octal(std::numeric_limits<int>::max(), "o"), "17777777777");
  EXPECT_EQ(format_octal(std::numeric_limits<int>::max(), "#o"), "017777777777");
  EXPECT_EQ(format_octal(std::numeric_limits<int>::max(), "015o"), "000017777777777");

  // Test with minimum int value (magnitude)
  EXPECT_EQ(format_octal(std::numeric_limits<int>::min(), "o"), "20000000000");
  EXPECT_EQ(format_octal(std::numeric_limits<int>::min(), "#o"), "020000000000");

  // Test with unsigned int
  EXPECT_EQ(fmt::format("{:o}", 4294967295U), "37777777777"); // 2^32 - 1
  EXPECT_EQ(fmt::format("{:#o}", 4294967295U), "037777777777");
}

TEST(FmtOctalTest, CustomTypeWithFormatter) {
  struct MyInt {
    int value;
  };
}

// Custom formatter for MyInt to test if the underlying formatting logic is used
template <>
struct fmt::formatter<MyInt> : fmt::formatter<int> {
  template <typename FormatContext>
  auto format(const MyInt& my_int, FormatContext& ctx) const {
    return fmt::formatter<int>::format(my_int.value, ctx);
  }
};

TEST(FmtOctalTest, CustomTypeOctalFormatting) {
  MyInt val = {10};
  EXPECT_EQ(fmt::format("{:o}", val), "12");
  EXPECT_EQ(fmt::format("{:#o}", val), "012");
  EXPECT_EQ(fmt::format("{:05o}", val), "00012");
}