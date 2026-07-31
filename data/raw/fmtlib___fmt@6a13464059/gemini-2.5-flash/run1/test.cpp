#include <gtest/gtest.h>
#include "fmt/core.h"
#include "fmt/format.h" // Required for fmt::format and related functionalities

// Helper function to format a value with a given format string and capture output or error
template <typename T>
std::string format_value(const std::string& format_str, T value) {
  try {
    return fmt::format(fmt::runtime(format_str), value);
  } catch (const fmt::format_error& e) {
    return e.what();
  }
}

// Test fixture for common setup if needed, but for this function, direct tests are sufficient.
// We are testing the behavior of on_sign within the context of fmt::formatter.
// The on_sign method is called internally when a sign specifier ('+', '-', ' ') is used.

TEST(FormatterOnSignTest, IntegralTypesWithSignSpecifier) {
  // Test signed integral types
  EXPECT_EQ(format_value("{:+d}", 123), "+123");
  EXPECT_EQ(format_value("{:-d}", 123), "123"); // '-' only affects negative numbers
  EXPECT_EQ(format_value("{: d}", 123), " 123");

  EXPECT_EQ(format_value("{:+d}", -123), "-123");
  EXPECT_EQ(format_value("{:-d}", -123), "-123");
  EXPECT_EQ(format_value("{: d}", -123), "-123");

  EXPECT_EQ(format_value("{:+d}", 0), "+0");
  EXPECT_EQ(format_value("{:-d}", 0), "0");
  EXPECT_EQ(format_value("{: d}", 0), " 0");

  // Test char type (which is integral but can be treated specially)
  EXPECT_EQ(format_value("{:+c}", 'A'), "+65"); // char is formatted as its integer value with sign
  EXPECT_EQ(format_value("{:-c}", 'A'), "65");
  EXPECT_EQ(format_value("{: c}", 'A'), " 65");

  EXPECT_EQ(format_value("{:+c}", (char)-1), "-1");
  EXPECT_EQ(format_value("{:-c}", (char)-1), "-1");
  EXPECT_EQ(format_value("{: c}", (char)-1), "-1");
}

TEST(FormatterOnSignTest, UnsignedIntegralTypesWithSignSpecifier) {
  // Unsigned types with sign specifiers should result in an error
  EXPECT_EQ(format_value("{:+d}", 123U), "format specifier requires signed argument");
  EXPECT_EQ(format_value("{:-d}", 123U), "format specifier requires signed argument");
  EXPECT_EQ(format_value("{: d}", 123U), "format specifier requires signed argument");

  EXPECT_EQ(format_value("{:+d}", 0U), "format specifier requires signed argument");
  EXPECT_EQ(format_value("{:-d}", 0U), "format specifier requires signed argument");
  EXPECT_EQ(format_value("{: d}", 0U), "format specifier requires signed argument");

  EXPECT_EQ(format_value("{:+d}", (unsigned char)255), "format specifier requires signed argument");
  EXPECT_EQ(format_value("{:-d}", (unsigned short)65535), "format specifier requires signed argument");
  EXPECT_EQ(format_value("{: d}", (unsigned long)123456789UL), "format specifier requires signed argument");
}

TEST(FormatterOnSignTest, FloatingPointTypesWithSignSpecifier) {
  // Floating point types should handle sign specifiers correctly
  EXPECT_EQ(format_value("{:+f}", 123.45), "+123.450000");
  EXPECT_EQ(format_value("{:-f}", 123.45), "123.450000");
  EXPECT_EQ(format_value("{: f}", 123.45), " 123.450000");

  EXPECT_EQ(format_value("{:+f}", -123.45), "-123.450000");
  EXPECT_EQ(format_value("{:-f}", -123.45), "-123.450000");
  EXPECT_EQ(format_value("{: f}", -123.45), "-123.450000");

  EXPECT_EQ(format_value("{:+f}", 0.0), "+0.000000");
  EXPECT_EQ(format_value("{:-f}", 0.0), "0.000000");
  EXPECT_EQ(format_value("{: f}", 0.0), " 0.000000");

  EXPECT_EQ(format_value("{:+f}", -0.0), "-0.000000");
  EXPECT_EQ(format_value("{:-f}", -0.0), "-0.000000");
  EXPECT_EQ(format_value("{: f}", -0.0), "-0.000000");
}

TEST(FormatterOnSignTest, OtherTypesWithSignSpecifier) {
  // Pointers should not accept sign specifiers
  int i = 0;
  EXPECT_EQ(format_value("{:+p}", &i), "format specifier requires numeric argument");
  EXPECT_EQ(format_value("{:-p}", &i), "format specifier requires numeric argument");
  EXPECT_EQ(format_value("{: p}", &i), "format specifier requires numeric argument");

  // Strings should not accept sign specifiers
  EXPECT_EQ(format_value("{:+s}", "hello"), "format specifier requires numeric argument");
  EXPECT_EQ(format_value("{:-s}", "hello"), "format specifier requires numeric argument");
  EXPECT_EQ(format_value("{: s}", "hello"), "format specifier requires numeric argument");

  // Booleans should not accept sign specifiers (unless formatted as integers, which is not default)
  EXPECT_EQ(format_value("{:+}", true), "format specifier requires numeric argument");
  EXPECT_EQ(format_value("{:-}", true), "format specifier requires numeric argument");
  EXPECT_EQ(format_value("{: }", true), "format specifier requires numeric argument");
}

// Test with custom types that might implicitly convert or have specific formatters
struct CustomSigned {
  int value;
  operator int() const { return value; }
};

struct CustomUnsigned {
  unsigned int value;
  operator unsigned int() const { return value; }
};

// Provide a formatter for CustomSigned (if not implicitly convertible to int)
// For this test, we rely on implicit conversion to int.
// If a custom formatter was defined, it would need to handle on_sign.

TEST(FormatterOnSignTest, CustomTypes) {
  CustomSigned cs_pos = {10};
  CustomSigned cs_neg = {-10};
  CustomSigned cs_zero = {0};

  EXPECT_EQ(format_value("{:+d}", cs_pos), "+10");
  EXPECT_EQ(format_value("{:-d}", cs_neg), "-10");
  EXPECT_EQ(format_value("{: d}", cs_zero), " 0");

  CustomUnsigned cu_pos = {10U};
  EXPECT_EQ(format_value("{:+d}", cu_pos), "format specifier requires signed argument");
}

// Test with different integer sizes
TEST(FormatterOnSignTest, DifferentIntegerSizes) {
  long long ll_val = 123456789012345LL;
  EXPECT_EQ(format_value("{:+d}", ll_val), "+123456789012345");
  EXPECT_EQ(format_value("{:-d}", -ll_val), "-123456789012345");

  short s_val = 123;
  EXPECT_EQ(format_value("{:+d}", s_val), "+123");

  unsigned long long ull_val = 123456789012345ULL;
  EXPECT_EQ(format_value("{:+d}", ull_val), "format specifier requires signed argument");
}

// Test with `int128_type` if available and enabled in fmtlib
#if FMT_USE_INT128
TEST(FormatterOnSignTest, Int128Type) {
  __int128_t i128_pos = 123456789012345678901234567890123456789LL;
  __int128_t i128_neg = -123456789012345678901234567890123456789LL;
  __int128_t i128_zero = 0;

  EXPECT_EQ(format_value("{:+d}", i128_pos), "+123456789012345678901234567890123456789");
  EXPECT_EQ(format_value("{:-d}", i128_neg), "-123456789012345678901234567890123456789");
  EXPECT_EQ(format_value("{: d}", i128_zero), " 0");

  unsigned __int128_t ui128_val = 123456789012345678901234567890123456789ULL;
  EXPECT_EQ(format_value("{:+d}", ui128_val), "format specifier requires signed argument");
}
#endif // FMT_USE_INT128