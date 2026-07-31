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

// Test fixture for common setup if needed, though not strictly necessary for these tests
class OnSignTest : public ::testing::Test {
protected:
  // You can add common setup here if multiple tests need it
};

// Test case for signed integral types with '+' sign specifier
TEST_F(OnSignTest, SignedIntegralPlusSign) {
  EXPECT_EQ("+123", format_value("{:+}", 123));
  EXPECT_EQ("-123", format_value("{:+}", -123));
  EXPECT_EQ("+0", format_value("{:+}", 0));
  EXPECT_EQ("+123", format_value("{:+}", (short)123));
  EXPECT_EQ("+123", format_value("{:+}", (long)123));
  EXPECT_EQ("+123", format_value("{:+}", (long long)123));
}

// Test case for signed integral types with ' ' (space) sign specifier
TEST_F(OnSignTest, SignedIntegralSpaceSign) {
  EXPECT_EQ(" 123", format_value("{: }", 123));
  EXPECT_EQ("-123", format_value("{: }", -123));
  EXPECT_EQ("  0", format_value("{: }", 0)); // Space for positive zero
}

// Test case for signed integral types with '-' (default) sign specifier
TEST_F(OnSignTest, SignedIntegralDefaultSign) {
  EXPECT_EQ("123", format_value("{:-}", 123)); // '-' is default, so no explicit sign for positive
  EXPECT_EQ("-123", format_value("{:-}", -123));
  EXPECT_EQ("0", format_value("{:-}", 0));
  EXPECT_EQ("123", format_value("{}", 123)); // No sign specifier, default behavior
  EXPECT_EQ("-123", format_value("{}", -123));
}

// Test case for unsigned integral types with '+' sign specifier (should error)
TEST_F(OnSignTest, UnsignedIntegralPlusSignError) {
  std::string error_msg = format_value("{:+}", 123U);
  EXPECT_NE(std::string::npos, error_msg.find("format specifier requires signed argument"))
      << "Expected an error for unsigned int with '+' sign, but got: " << error_msg;

  error_msg = format_value("{:+}", (unsigned short)123);
  EXPECT_NE(std::string::npos, error_msg.find("format specifier requires signed argument"))
      << "Expected an error for unsigned short with '+' sign, but got: " << error_msg;

  error_msg = format_value("{:+}", (unsigned long)123);
  EXPECT_NE(std::string::npos, error_msg.find("format specifier requires signed argument"))
      << "Expected an error for unsigned long with '+' sign, but got: " << error_msg;

  error_msg = format_value("{:+}", (unsigned long long)123);
  EXPECT_NE(std::string::npos, error_msg.find("format specifier requires signed argument"))
      << "Expected an error for unsigned long long with '+' sign, but got: " << error_msg;
}

// Test case for unsigned integral types with ' ' (space) sign specifier (should error)
TEST_F(OnSignTest, UnsignedIntegralSpaceSignError) {
  std::string error_msg = format_value("{: }", 123U);
  EXPECT_NE(std::string::npos, error_msg.find("format specifier requires signed argument"))
      << "Expected an error for unsigned int with ' ' sign, but got: " << error_msg;
}

// Test case for floating-point types with '+' sign specifier
TEST_F(OnSignTest, FloatingPointPlusSign) {
  EXPECT_EQ("+3.14", format_value("{:+}", 3.14));
  EXPECT_EQ("-3.14", format_value("{:+}", -3.14));
  EXPECT_EQ("+0.0", format_value("{:+.1f}", 0.0));
  EXPECT_EQ("+inf", format_value("{:+}", std::numeric_limits<double>::infinity()));
  EXPECT_EQ("-inf", format_value("{:+}", -std::numeric_limits<double>::infinity()));
  // NaN behavior can vary, but generally doesn't get a sign prefix
  // EXPECT_EQ("nan", format_value("{:+}", std::numeric_limits<double>::quiet_NaN()));
}

// Test case for floating-point types with ' ' (space) sign specifier
TEST_F(OnSignTest, FloatingPointSpaceSign) {
  EXPECT_EQ(" 3.14", format_value("{: }", 3.14));
  EXPECT_EQ("-3.14", format_value("{: }", -3.14));
  EXPECT_EQ(" 0.0", format_value("{: .1f}", 0.0));
}

// Test case for character types with sign specifiers (should error as they are not numeric)
TEST_F(OnSignTest, CharTypeSignSpecifierError) {
  std::string error_msg = format_value("{:+}", 'A');
  EXPECT_NE(std::string::npos, error_msg.find("format specifier requires numeric argument"))
      << "Expected an error for char with '+' sign, but got: " << error_msg;

  error_msg = format_value("{: }", 'B');
  EXPECT_NE(std::string::npos, error_msg.find("format specifier requires numeric argument"))
      << "Expected an error for char with ' ' sign, but got: " << error_msg;
}

// Test case for string types with sign specifiers (should error as they are not numeric)
TEST_F(OnSignTest, StringTypeSignSpecifierError) {
  std::string error_msg = format_value("{:+}", "hello");
  EXPECT_NE(std::string::npos, error_msg.find("format specifier requires numeric argument"))
      << "Expected an error for string with '+' sign, but got: " << error_msg;
}

// Test case for boolean types with sign specifiers (should error as they are not numeric)
TEST_F(OnSignTest, BoolTypeSignSpecifierError) {
  std::string error_msg = format_value("{:+}", true);
  EXPECT_NE(std::string::npos, error_msg.find("format specifier requires numeric argument"))
      << "Expected an error for bool with '+' sign, but got: " << error_msg;
}

// Test case for pointer types with sign specifiers (should error as they are not numeric)
TEST_F(OnSignTest, PointerTypeSignSpecifierError) {
  int x = 10;
  std::string error_msg = format_value("{:+}", &x);
  EXPECT_NE(std::string::npos, error_msg.find("format specifier requires numeric argument"))
      << "Expected an error for pointer with '+' sign, but got: " << error_msg;
}

// Test with custom signed integer type (if fmtlib supports it, e.g., __int128)
#if defined(__SIZEOF_INT128__)
TEST_F(OnSignTest, Int128TypePlusSign) {
  __int128_t val = 1234567890123456789LL;
  EXPECT_EQ("+1234567890123456789", format_value("{:+}", val));
  EXPECT_EQ("-1234567890123456789", format_value("{:+}", -val));
}

TEST_F(OnSignTest, UInt128TypePlusSignError) {
  unsigned __int128_t val = 1234567890123456789ULL;
  std::string error_msg = format_value("{:+}", val);
  EXPECT_NE(std::string::npos, error_msg.find("format specifier requires signed argument"))
      << "Expected an error for unsigned __int128 with '+' sign, but got: " << error_msg;
}
#endif