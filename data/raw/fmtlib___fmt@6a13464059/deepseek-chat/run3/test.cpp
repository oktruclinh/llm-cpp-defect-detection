#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>
#include <limits>

// Test that on_sign works correctly for signed integer types
TEST(FormatSignTest, SignedIntegerTypes) {
    // Positive signed integers with sign specifier
    EXPECT_EQ(fmt::format("{:+d}", 42), "+42");
    EXPECT_EQ(fmt::format("{:+d}", -42), "-42");
    EXPECT_EQ(fmt::format("{: d}", 42), " 42");
    EXPECT_EQ(fmt::format("{: d}", -42), "-42");
    EXPECT_EQ(fmt::format("{:-d}", 42), "42");
    EXPECT_EQ(fmt::format("{:-d}", -42), "-42");
}

// Test that on_sign works correctly for unsigned integer types (should error)
TEST(FormatSignTest, UnsignedIntegerTypes) {
    // Unsigned types should throw when sign specifier is used
    EXPECT_THROW(fmt::format("{:+u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{: u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-u}", 42u), fmt::format_error);
    
    // Also test with larger unsigned types
    EXPECT_THROW(fmt::format("{:+lu}", 42ul), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+llu}", 42ull), fmt::format_error);
}

// Test that on_sign works correctly for floating point types
TEST(FormatSignTest, FloatingPointTypes) {
    // Floating point should accept sign specifiers
    EXPECT_EQ(fmt::format("{:+f}", 3.14), "+3.140000");
    EXPECT_EQ(fmt::format("{:+f}", -3.14), "-3.140000");
    EXPECT_EQ(fmt::format("{: f}", 3.14), " 3.140000");
    EXPECT_EQ(fmt::format("{: f}", -3.14), "-3.140000");
    EXPECT_EQ(fmt::format("{:-f}", 3.14), "3.140000");
    EXPECT_EQ(fmt::format("{:-f}", -3.14), "-3.140000");
}

// Test that on_sign works correctly for char type (should error)
TEST(FormatSignTest, CharType) {
    // Char type should throw when sign specifier is used
    EXPECT_THROW(fmt::format("{:+c}", 'a'), fmt::format_error);
    EXPECT_THROW(fmt::format("{: c}", 'a'), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-c}", 'a'), fmt::format_error);
}

// Test that on_sign works correctly for string types (should error)
TEST(FormatSignTest, StringType) {
    // String type should throw when sign specifier is used
    EXPECT_THROW(fmt::format("{:+s}", std::string("hello")), fmt::format_error);
    EXPECT_THROW(fmt::format("{: s}", std::string("hello")), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-s}", std::string("hello")), fmt::format_error);
}

// Test that on_sign works correctly for boolean type (should error)
TEST(FormatSignTest, BooleanType) {
    // Boolean type should throw when sign specifier is used
    EXPECT_THROW(fmt::format("{:+b}", true), fmt::format_error);
    EXPECT_THROW(fmt::format("{: b}", false), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-b}", true), fmt::format_error);
}

// Test that on_sign works correctly for custom types that are integral but not signed
TEST(FormatSignTest, CustomIntegralType) {
    // Test with unsigned char (integral but not signed)
    unsigned char uc = 65;
    EXPECT_THROW(fmt::format("{:+c}", uc), fmt::format_error);
    
    // Test with unsigned short
    unsigned short us = 42;
    EXPECT_THROW(fmt::format("{:+d}", us), fmt::format_error);
}

// Test that on_sign works correctly for signed char (should work)
TEST(FormatSignTest, SignedCharType) {
    // Signed char is a signed integer type
    signed char sc = 65;
    EXPECT_EQ(fmt::format("{:+d}", static_cast<int>(sc)), "+65");
}

// Test that on_sign works correctly for long long types
TEST(FormatSignTest, LongLongTypes) {
    // long long is a signed integer type
    EXPECT_EQ(fmt::format("{:+lld}", 42ll), "+42");
    EXPECT_EQ(fmt::format("{:+lld}", -42ll), "-42");
    
    // unsigned long long should throw
    EXPECT_THROW(fmt::format("{:+llu}", 42ull), fmt::format_error);
}

// Test that on_sign works correctly for zero values
TEST(FormatSignTest, ZeroValues) {
    // Zero with sign specifier
    EXPECT_EQ(fmt::format("{:+d}", 0), "+0");
    EXPECT_EQ(fmt::format("{: d}", 0), " 0");
    EXPECT_EQ(fmt::format("{:-d}", 0), "0");
    
    // Zero with unsigned type should still throw
    EXPECT_THROW(fmt::format("{:+u}", 0u), fmt::format_error);
}

// Test that on_sign works correctly for negative values
TEST(FormatSignTest, NegativeValues) {
    // Negative values with sign specifier
    EXPECT_EQ(fmt::format("{:+d}", -1), "-1");
    EXPECT_EQ(fmt::format("{: d}", -1), "-1");
    EXPECT_EQ(fmt::format("{:-d}", -1), "-1");
}

// Test that on_sign works correctly for maximum values
TEST(FormatSignTest, MaximumValues) {
    // Maximum signed integer
    EXPECT_EQ(fmt::format("{:+d}", std::numeric_limits<int>::max()), "+2147483647");
    EXPECT_EQ(fmt::format("{:+d}", std::numeric_limits<int>::min()), "-2147483648");
    
    // Maximum unsigned integer should throw
    EXPECT_THROW(fmt::format("{:+u}", std::numeric_limits<unsigned int>::max()), fmt::format_error);
}

// Test that on_sign works correctly for mixed format specifiers
TEST(FormatSignTest, MixedSpecifiers) {
    // Sign with width and precision
    EXPECT_EQ(fmt::format("{:+10d}", 42), "       +42");
    EXPECT_EQ(fmt::format("{:+010d}", 42), "+000000042");
    
    // Sign with alignment
    EXPECT_EQ(fmt::format("{:<+10d}", 42), "+42       ");
    EXPECT_EQ(fmt::format("{:>+10d}", 42), "       +42");
    EXPECT_EQ(fmt::format("{:^+10d}", 42), "   +42    ");
}