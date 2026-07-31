#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Test that on_sign correctly handles signed integer types
TEST(OnSignTest, SignedIntegerTypes) {
    // int_type should work fine with sign specifiers
    EXPECT_NO_THROW(fmt::format("{:+d}", 42));
    EXPECT_EQ(fmt::format("{:+d}", 42), "+42");
    EXPECT_EQ(fmt::format("{:-d}", 42), "42");
    EXPECT_EQ(fmt::format("{: d}", 42), " 42");

    // long_long_type should work fine
    EXPECT_NO_THROW(fmt::format("{:+lld}", 42LL));
    EXPECT_EQ(fmt::format("{:+lld}", 42LL), "+42");

    // char_type should work fine
    EXPECT_NO_THROW(fmt::format("{:+c}", 'A'));
    EXPECT_EQ(fmt::format("{:+c}", 'A'), "+A");
}

// Test that on_sign correctly handles unsigned integer types (should error)
TEST(OnSignTest, UnsignedIntegerTypes) {
    // unsigned int should throw when sign specifier is used
    EXPECT_THROW(fmt::format("{:+u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{: u}", 42u), fmt::format_error);

    // unsigned long long should throw
    EXPECT_THROW(fmt::format("{:+llu}", 42ULL), fmt::format_error);

    // unsigned char should throw
    EXPECT_THROW(fmt::format("{:+c}", static_cast<unsigned char>('A')), fmt::format_error);
}

// Test that on_sign correctly handles floating point types (should work)
TEST(OnSignTest, FloatingPointTypes) {
    // float should work fine with sign specifiers
    EXPECT_NO_THROW(fmt::format("{:+f}", 3.14f));
    EXPECT_EQ(fmt::format("{:+f}", 3.14f), "+3.140000");
    EXPECT_EQ(fmt::format("{:-f}", 3.14f), "3.140000");
    EXPECT_EQ(fmt::format("{: f}", 3.14f), " 3.140000");

    // double should work fine
    EXPECT_NO_THROW(fmt::format("{:+f}", 3.14));
    EXPECT_EQ(fmt::format("{:+f}", 3.14), "+3.140000");

    // long double should work fine
    EXPECT_NO_THROW(fmt::format("{:+Lf}", 3.14L));
}

// Test that on_sign correctly handles negative numbers
TEST(OnSignTest, NegativeNumbers) {
    // Signed integer with negative value
    EXPECT_EQ(fmt::format("{:+d}", -42), "-42");
    EXPECT_EQ(fmt::format("{:-d}", -42), "-42");
    EXPECT_EQ(fmt::format("{: d}", -42), "-42");

    // Floating point with negative value
    EXPECT_EQ(fmt::format("{:+f}", -3.14), "-3.140000");
    EXPECT_EQ(fmt::format("{:-f}", -3.14), "-3.140000");
    EXPECT_EQ(fmt::format("{: f}", -3.14), "-3.140000");
}

// Test that on_sign correctly handles zero
TEST(OnSignTest, ZeroValues) {
    // Signed integer zero
    EXPECT_EQ(fmt::format("{:+d}", 0), "+0");
    EXPECT_EQ(fmt::format("{:-d}", 0), "0");
    EXPECT_EQ(fmt::format("{: d}", 0), " 0");

    // Floating point zero
    EXPECT_EQ(fmt::format("{:+f}", 0.0), "+0.000000");
    EXPECT_EQ(fmt::format("{:-f}", 0.0), "0.000000");
    EXPECT_EQ(fmt::format("{: f}", 0.0), " 0.000000");
}

// Test that on_sign correctly handles edge cases with different format specifiers
TEST(OnSignTest, EdgeCases) {
    // Very large positive number
    EXPECT_EQ(fmt::format("{:+d}", 2147483647), "+2147483647");
    
    // Very large negative number
    EXPECT_EQ(fmt::format("{:+d}", -2147483647), "-2147483647");
    
    // Minimum int
    EXPECT_EQ(fmt::format("{:+d}", -2147483648), "-2147483648");
    
    // Zero with width specifier
    EXPECT_EQ(fmt::format("{:+5d}", 0), "   +0");
    EXPECT_EQ(fmt::format("{:-5d}", 0), "    0");
    EXPECT_EQ(fmt::format("{: 5d}", 0), "    0");
}

// Test that on_sign correctly handles the error message for unsigned types
TEST(OnSignTest, ErrorMessage) {
    try {
        fmt::format("{:+u}", 42u);
        FAIL() << "Expected fmt::format_error";
    } catch (const fmt::format_error& e) {
        EXPECT_STREQ(e.what(), "format specifier requires signed argument");
    }
}

// Test that on_sign works with custom format strings
TEST(OnSignTest, CustomFormatStrings) {
    // Test with fill and align
    EXPECT_EQ(fmt::format("{:*>+5d}", 42), "**+42");
    EXPECT_EQ(fmt::format("{:*<+5d}", 42), "+42**");
    EXPECT_EQ(fmt::format("{:*^+5d}", 42), "+42**");
    
    // Test with precision for floating point
    EXPECT_EQ(fmt::format("{:+.2f}", 3.14159), "+3.14");
    EXPECT_EQ(fmt::format("{:-.2f}", 3.14159), "3.14");
    EXPECT_EQ(fmt::format("{: .2f}", 3.14159), " 3.14");
}