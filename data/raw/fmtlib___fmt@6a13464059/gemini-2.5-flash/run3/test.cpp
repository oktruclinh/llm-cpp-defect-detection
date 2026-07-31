#include <gtest/gtest.h>
#include "fmt/core.h"
#include "fmt/format.h" // Required for fmt::format and its internal machinery

// A custom error handler to capture errors for testing
class TestErrorHandler : public fmt::detail::error_handler {
 public:
  std::string error_message;

  void on_error(const char* message) override { error_message = message; }
};

// Helper function to format with a custom error handler
template <typename... Args>
std::string format_with_error_handler(TestErrorHandler& eh, fmt::string_view format_str,
                                      const Args&... args) {
  fmt::format_arg_store<fmt::format_context, Args...> as{args...};
  fmt::detail::arg_formatter<fmt::format_context> af(
      fmt::format_context(std::back_inserter(eh.error_message), format_str, as), &eh);
  // The actual on_sign is called internally during formatting.
  // We just need to trigger the formatting process.
  // This is a bit indirect, but it's how fmt's public API works.
  try {
    return fmt::vformat(format_str, fmt::make_format_args(args...));
  } catch (const fmt::format_error& e) {
    // Catch fmt::format_error if it's thrown, as our handler might not always prevent it.
    eh.on_error(e.what());
    return ""; // Return empty string on error
  }
}

// Test fixture for common setup
class OnSignTest : public ::testing::Test {
 protected:
  TestErrorHandler eh_;
};

// Test case for signed integer types with '+' sign
TEST_F(OnSignTest, SignedIntWithPlusSign) {
  EXPECT_EQ(fmt::format("{:+d}", 123), "+123");
  EXPECT_EQ(eh_.error_message, ""); // No error expected
}

// Test case for signed integer types with '-' sign (negative numbers)
TEST_F(OnSignTest, SignedIntWithMinusSign) {
  EXPECT_EQ(fmt::format("{:d}", -123), "-123"); // '-' is default for negative
  EXPECT_EQ(fmt::format("{:-d}", -123), "-123"); // Explicit '-' is same as default
  EXPECT_EQ(eh_.error_message, ""); // No error expected
}

// Test case for signed integer types with ' ' sign (space for positive)
TEST_F(OnSignTest, SignedIntWithSpaceSign) {
  EXPECT_EQ(fmt::format("{: d}", 123), " 123");
  EXPECT_EQ(fmt::format("{: d}", -123), "-123");
  EXPECT_EQ(eh_.error_message, ""); // No error expected
}

// Test case for unsigned integer types with '+' sign (should error)
TEST_F(OnSignTest, UnsignedIntWithPlusSignErrors) {
  // Using format_with_error_handler to capture the error message
  format_with_error_handler(eh_, "{:+d}", 123U);
  EXPECT_EQ(eh_.error_message, "format specifier requires signed argument");
  eh_.error_message.clear(); // Clear for next assertion

  format_with_error_handler(eh_, "{:+d}", (unsigned long)123);
  EXPECT_EQ(eh_.error_message, "format specifier requires signed argument");
  eh_.error_message.clear();

  format_with_error_handler(eh_, "{:+d}", (unsigned short)123);
  EXPECT_EQ(eh_.error_message, "format specifier requires signed argument");
  eh_.error_message.clear();

  format_with_error_handler(eh_, "{:+d}", (unsigned char)123);
  EXPECT_EQ(eh_.error_message, "format specifier requires signed argument");
  eh_.error_message.clear();
}

// Test case for unsigned integer types with ' ' sign (should error)
TEST_F(OnSignTest, UnsignedIntWithSpaceSignErrors) {
  format_with_error_handler(eh_, "{: d}", 123U);
  EXPECT_EQ(eh_.error_message, "format specifier requires signed argument");
  eh_.error_message.clear();

  format_with_error_handler(eh_, "{: d}", (unsigned long)123);
  EXPECT_EQ(eh_.error_message, "format specifier requires signed argument");
  eh_.error_message.clear();
}

// Test case for character types with '+' sign (should error)
TEST_F(OnSignTest, CharWithPlusSignErrors) {
  format_with_error_handler(eh_, "{:+d}", 'a');
  EXPECT_EQ(eh_.error_message, "format specifier requires signed argument");
  eh_.error_message.clear();
}

// Test case for character types with ' ' sign (should error)
TEST_F(OnSignTest, CharWithSpaceSignErrors) {
  format_with_error_handler(eh_, "{: d}", 'a');
  EXPECT_EQ(eh_.error_message, "format specifier requires signed argument");
  eh_.error_message.clear();
}

// Test case for floating-point types with '+' sign (no error)
TEST_F(OnSignTest, FloatWithPlusSign) {
  EXPECT_EQ(fmt::format("{:+f}", 123.45), "+123.450000");
  EXPECT_EQ(eh_.error_message, ""); // No error expected
}

// Test case for floating-point types with ' ' sign (no error)
TEST_F(OnSignTest, FloatWithSpaceSign) {
  EXPECT_EQ(fmt::format("{: f}", 123.45), " 123.450000");
  EXPECT_EQ(fmt::format("{: f}", -123.45), "-123.450000");
  EXPECT_EQ(eh_.error_message, ""); // No error expected
}

// Test case for boolean types with '+' sign (should error)
TEST_F(OnSignTest, BoolWithPlusSignErrors) {
  format_with_error_handler(eh_, "{:+d}", true);
  EXPECT_EQ(eh_.error_message, "format specifier requires signed argument");
  eh_.error_message.clear();
}

// Test case for boolean types with ' ' sign (should error)
TEST_F(OnSignTest, BoolWithSpaceSignErrors) {
  format_with_error_handler(eh_, "{: d}", false);
  EXPECT_EQ(eh_.error_message, "format specifier requires signed argument");
  eh_.error_message.clear();
}

// Test case for pointer types with '+' sign (should error)
TEST_F(OnSignTest, PointerWithPlusSignErrors) {
  int x = 0;
  format_with_error_handler(eh_, "{:+p}", &x);
  EXPECT_EQ(eh_.error_message, "format specifier requires numeric argument");
  eh_.error_message.clear();
}

// Test case for string types with '+' sign (should error)
TEST_F(OnSignTest, StringWithPlusSignErrors) {
  format_with_error_handler(eh_, "{:+s}", "hello");
  EXPECT_EQ(eh_.error_message, "format specifier requires numeric argument");
  eh_.error_message.clear();
}

// Test case for custom types (should error if not numeric)
struct CustomType {};
template <> struct fmt::is_numeric<CustomType> : std::false_type {};

TEST_F(OnSignTest, CustomTypeWithPlusSignErrors) {
  CustomType c;
  format_with_error_handler(eh_, "{:+d}", c);
  EXPECT_EQ(eh_.error_message, "format specifier requires numeric argument");
  eh_.error_message.clear();
}

// Test case for signed char with sign specifiers (should not error)
TEST_F(OnSignTest, SignedCharWithSign) {
  signed char sc = 10;
  EXPECT_EQ(fmt::format("{:+d}", sc), "+10");
  EXPECT_EQ(fmt::format("{: d}", sc), " 10");
  EXPECT_EQ(eh_.error_message, "");
}

// Test case for int128_t with sign specifiers (if available and signed)
#if FMT_USE_INT128
TEST_F(OnSignTest, Int128WithSign) {
  __int128_t i128 = 12345678901234567890LL;
  EXPECT_EQ(fmt::format("{:+d}", i128), "+12345678901234567890");
  EXPECT_EQ(fmt::format("{: d}", i128), " 12345678901234567890");
  EXPECT_EQ(eh_.error_message, "");

  unsigned __int128_t ui128 = 12345678901234567890ULL;
  format_with_error_handler(eh_, "{:+d}", ui128);
  EXPECT_EQ(eh_.error_message, "format specifier requires signed argument");
  eh_.error_message.clear();
}
#endif