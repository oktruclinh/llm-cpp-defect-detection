#include <gtest/gtest.h>
#include "fmt/core.h"

// Test that on_sign works correctly for signed integral types
TEST(FormatSignTest, SignedIntegralTypes) {
    // int type with sign specification
    EXPECT_EQ(fmt::format("{:+d}", 42), "+42");
    EXPECT_EQ(fmt::format("{: d}", 42), " 42");
    EXPECT_EQ(fmt::format("{:-d}", 42), "42");
    EXPECT_EQ(fmt::format("{:+d}", -42), "-42");
    
    // long long type with sign specification
    EXPECT_EQ(fmt::format("{:+lld}", 42LL), "+42");
    EXPECT_EQ(fmt::format("{: lld}", 42LL), " 42");
    
    // char type (signed) with sign specification
    EXPECT_EQ(fmt::format("{:+c}", 'A'), "+A");
    EXPECT_EQ(fmt::format("{: c}", 'A'), " A");
}

// Test that on_sign works correctly for floating-point types
TEST(FormatSignTest, FloatingPointTypes) {
    EXPECT_EQ(fmt::format("{:+f}", 3.14), "+3.140000");
    EXPECT_EQ(fmt::format("{: f}", 3.14), " 3.140000");
    EXPECT_EQ(fmt::format("{:-f}", 3.14), "3.140000");
    EXPECT_EQ(fmt::format("{:+f}", -3.14), "-3.140000");
    
    // double
    EXPECT_EQ(fmt::format("{:+e}", 1.0e10), "+1.000000e+10");
    EXPECT_EQ(fmt::format("{: e}", 1.0e10), " 1.000000e+10");
    
    // long double
    EXPECT_EQ(fmt::format("{:+Lf}", 2.718L), "+2.718000");
}

// Test that on_sign throws error for unsigned integral types
TEST(FormatSignTest, UnsignedIntegralTypes) {
    // unsigned int should throw
    EXPECT_THROW(fmt::format("{:+u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{: u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-u}", 42u), fmt::format_error);
    
    // unsigned long should throw
    EXPECT_THROW(fmt::format("{:+lu}", 42UL), fmt::format_error);
    
    // unsigned long long should throw
    EXPECT_THROW(fmt::format("{:+llu}", 42ULL), fmt::format_error);
    
    // size_t (unsigned) should throw
    EXPECT_THROW(fmt::format("{:+zu}", static_cast<size_t>(42)), fmt::format_error);
}

// Test that on_sign works for bool type (integral but not signed)
TEST(FormatSignTest, BoolType) {
    EXPECT_EQ(fmt::format("{:d}", true), "1");
    EXPECT_EQ(fmt::format("{:d}", false), "0");
}

// Test edge cases with zero values
TEST(FormatSignTest, ZeroValues) {
    EXPECT_EQ(fmt::format("{:+d}", 0), "+0");
    EXPECT_EQ(fmt::format("{: d}", 0), " 0");
    EXPECT_EQ(fmt::format("{:-d}", 0), "0");
    
    EXPECT_EQ(fmt::format("{:+f}", 0.0), "+0.000000");
    EXPECT_EQ(fmt::format("{: f}", 0.0), " 0.000000");
}

// Test edge case: negative zero (floating point)
TEST(FormatSignTest, NegativeZero) {
    EXPECT_EQ(fmt::format("{:+f}", -0.0), "-0.000000");
    EXPECT_EQ(fmt::format("{: f}", -0.0), "-0.000000");
}

// Test edge case: very large and very small numbers
TEST(FormatSignTest, ExtremeValues) {
    EXPECT_EQ(fmt::format("{:+d}", INT_MAX), "+2147483647");
    EXPECT_EQ(fmt::format("{:+d}", INT_MIN), "-2147483648");
    
    EXPECT_THROW(fmt::format("{:+u}", UINT_MAX), fmt::format_error);
}

// Test that on_sign works with different format specifiers
TEST(FormatSignTest, DifferentFormatSpecifiers) {
    // Binary
    EXPECT_EQ(fmt::format("{:+b}", 42), "+101010");
    EXPECT_THROW(fmt::format("{:+b}", 42u), fmt::format_error); // unsigned throws
    
    // Octal
    EXPECT_EQ(fmt::format("{:+o}", 42), "+52");
    EXPECT_THROW(fmt::format("{:+o}", 42u), fmt::format_error);
    
    // Hexadecimal
    EXPECT_EQ(fmt::format("{:+x}", 255), "+ff");
    EXPECT_EQ(fmt::format("{:+X}", 255), "+FF");
    EXPECT_THROW(fmt::format("{:+x}", 255u), fmt::format_error);
}

// Test that on_sign works with width and precision
TEST(FormatSignTest, WidthAndPrecision) {
    EXPECT_EQ(fmt::format("{:+10d}", 42), "       +42");
    EXPECT_EQ(fmt::format("{: 10d}", 42), "        42");
    
    EXPECT_EQ(fmt::format("{:+10.3f}", 3.14159), "    +3.142");
    EXPECT_EQ(fmt::format("{: 10.3f}", 3.14159), "     3.142");
}

// Test that on_sign works correctly for char_type (signed character)
TEST(FormatSignTest, CharType) {
    EXPECT_EQ(fmt::format("{:+c}", static_cast<char>(65)), "+A");
    EXPECT_EQ(fmt::format("{: c}", static_cast<char>(65)), " A");
    EXPECT_EQ(fmt::format("{:-c}", static_cast<char>(65)), "A");
    
    // Negative char values
    EXPECT_EQ(fmt::format("{:+c}", static_cast<char>(-1)), "+" + std::string(1, static_cast<char>(-1)));
}

// Test that on_sign correctly handles signed char
TEST(FormatSignTest, SignedCharExplicit) {
    signed char c = 'B';
    EXPECT_EQ(fmt::format("{:+d}", c), "+66"); // 'B' = 66
    EXPECT_EQ(fmt::format("{: d}", c), " 66");
    EXPECT_EQ(fmt::format("{:-d}", c), "66");
}