#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

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
    
    // Unsigned long long
    EXPECT_THROW(fmt::format("{:+llu}", 42ull), fmt::format_error);
    
    // Unsigned char (treated as unsigned integer)
    unsigned char uc = 42;
    EXPECT_THROW(fmt::format("{:+c}", uc), fmt::format_error);
}

// Test that on_sign works correctly for floating point types
TEST(FormatSignTest, FloatingPointTypes) {
    // Floating point types should accept sign specifiers
    EXPECT_EQ(fmt::format("{:+f}", 3.14), "+3.140000");
    EXPECT_EQ(fmt::format("{:+f}", -3.14), "-3.140000");
    EXPECT_EQ(fmt::format("{: f}", 3.14), " 3.140000");
    EXPECT_EQ(fmt::format("{: f}", -3.14), "-3.140000");
    EXPECT_EQ(fmt::format("{:-f}", 3.14), "3.140000");
    EXPECT_EQ(fmt::format("{:-f}", -3.14), "-3.140000");
}

// Test that on_sign works correctly for char type (signed)
TEST(FormatSignTest, CharType) {
    // char is signed, should accept sign specifiers
    EXPECT_EQ(fmt::format("{:+c}", 'A'), "+A");
    EXPECT_EQ(fmt::format("{: c}", 'A'), " A");
    EXPECT_EQ(fmt::format("{:-c}", 'A'), "A");
}

// Test that on_sign works correctly for signed long long
TEST(FormatSignTest, SignedLongLong) {
    EXPECT_EQ(fmt::format("{:+lld}", 42ll), "+42");
    EXPECT_EQ(fmt::format("{:+lld}", -42ll), "-42");
}

// Test that on_sign works correctly for zero values
TEST(FormatSignTest, ZeroValues) {
    // Signed integer zero
    EXPECT_EQ(fmt::format("{:+d}", 0), "+0");
    EXPECT_EQ(fmt::format("{: d}", 0), " 0");
    EXPECT_EQ(fmt::format("{:-d}", 0), "0");
    
    // Floating point zero
    EXPECT_EQ(fmt::format("{:+f}", 0.0), "+0.000000");
    EXPECT_EQ(fmt::format("{: f}", 0.0), " 0.000000");
    EXPECT_EQ(fmt::format("{:-f}", 0.0), "0.000000");
}

// Test that on_sign works correctly for negative zero
TEST(FormatSignTest, NegativeZero) {
    double neg_zero = -0.0;
    EXPECT_EQ(fmt::format("{:+f}", neg_zero), "-0.000000");
    EXPECT_EQ(fmt::format("{: f}", neg_zero), "-0.000000");
    EXPECT_EQ(fmt::format("{:-f}", neg_zero), "-0.000000");
}

// Test that on_sign works correctly for large values
TEST(FormatSignTest, LargeValues) {
    // Large signed integer
    EXPECT_EQ(fmt::format("{:+d}", 2147483647), "+2147483647");
    EXPECT_EQ(fmt::format("{:+d}", -2147483647), "-2147483647");
    
    // Large unsigned integer should still throw
    EXPECT_THROW(fmt::format("{:+u}", 4294967295u), fmt::format_error);
}

// Test that on_sign works correctly with width and precision
TEST(FormatSignTest, WithWidthAndPrecision) {
    EXPECT_EQ(fmt::format("{:+10d}", 42), "       +42");
    EXPECT_EQ(fmt::format("{: 10d}", 42), "        42");
    EXPECT_EQ(fmt::format("{:+10.5f}", 3.14), " +3.14000");
}

// Test that on_sign works correctly with different format specifiers
TEST(FormatSignTest, DifferentFormatSpecifiers) {
    // Binary (should not accept sign for unsigned)
    EXPECT_THROW(fmt::format("{:+b}", 42u), fmt::format_error);
    
    // Octal (should not accept sign for unsigned)
    EXPECT_THROW(fmt::format("{:+o}", 42u), fmt::format_error);
    
    // Hexadecimal (should not accept sign for unsigned)
    EXPECT_THROW(fmt::format("{:+x}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+X}", 42u), fmt::format_error);
}

// Test that on_sign works correctly for signed integer types with different bases
TEST(FormatSignTest, SignedWithDifferentBases) {
    // Signed binary
    EXPECT_EQ(fmt::format("{:+b}", 42), "+101010");
    EXPECT_EQ(fmt::format("{:+b}", -42), "-101010");
    
    // Signed octal
    EXPECT_EQ(fmt::format("{:+o}", 42), "+52");
    EXPECT_EQ(fmt::format("{:+o}", -42), "-52");
    
    // Signed hexadecimal
    EXPECT_EQ(fmt::format("{:+x}", 42), "+2a");
    EXPECT_EQ(fmt::format("{:+X}", 42), "+2A");
    EXPECT_EQ(fmt::format("{:+x}", -42), "-2a");
}

// Test that on_sign works correctly for int128 type if available
TEST(FormatSignTest, Int128Type) {
    // int128 should accept sign specifiers
    __int128 val = 42;
    EXPECT_EQ(fmt::format("{:+d}", val), "+42");
    EXPECT_EQ(fmt::format("{: d}", val), " 42");
    EXPECT_EQ(fmt::format("{:-d}", val), "42");
    
    __int128 neg_val = -42;
    EXPECT_EQ(fmt::format("{:+d}", neg_val), "-42");
    EXPECT_EQ(fmt::format("{: d}", neg_val), "-42");
    EXPECT_EQ(fmt::format("{:-d}", neg_val), "-42");
}

// Test that on_sign works correctly for unsigned int128 type if available
TEST(FormatSignTest, UnsignedInt128Type) {
    // unsigned int128 should throw when sign specifier is used
    unsigned __int128 val = 42;
    EXPECT_THROW(fmt::format("{:+d}", val), fmt::format_error);
    EXPECT_THROW(fmt::format("{: d}", val), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-d}", val), fmt::format_error);
}