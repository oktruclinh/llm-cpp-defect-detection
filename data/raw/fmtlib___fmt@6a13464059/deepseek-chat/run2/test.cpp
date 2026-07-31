#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Test that on_sign works correctly for signed integer types
TEST(OnSignTest, SignedIntegerTypes) {
    // int_type should accept sign specifier
    EXPECT_NO_THROW(fmt::format("{:+d}", 42));
    EXPECT_EQ(fmt::format("{:+d}", 42), "+42");
    EXPECT_EQ(fmt::format("{:-d}", 42), "42");
    EXPECT_EQ(fmt::format("{: d}", 42), " 42");

    // long_long_type should accept sign specifier
    EXPECT_NO_THROW(fmt::format("{:+lld}", 42LL));
    EXPECT_EQ(fmt::format("{:+lld}", 42LL), "+42");
}

// Test that on_sign correctly rejects unsigned integer types
TEST(OnSignTest, UnsignedIntegerTypes) {
    // unsigned int should throw
    EXPECT_THROW(fmt::format("{:+u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{: u}", 42u), fmt::format_error);

    // unsigned long long should throw
    EXPECT_THROW(fmt::format("{:+llu}", 42ULL), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-llu}", 42ULL), fmt::format_error);
    EXPECT_THROW(fmt::format("{: llu}", 42ULL), fmt::format_error);

    // unsigned char should throw
    EXPECT_THROW(fmt::format("{:+c}", static_cast<unsigned char>('A')), fmt::format_error);
}

// Test that on_sign works correctly for floating-point types
TEST(OnSignTest, FloatingPointTypes) {
    // float should accept sign specifier
    EXPECT_NO_THROW(fmt::format("{:+f}", 3.14f));
    EXPECT_EQ(fmt::format("{:+f}", 3.14f), "+3.140000");
    EXPECT_EQ(fmt::format("{:-f}", 3.14f), "3.140000");
    EXPECT_EQ(fmt::format("{: f}", 3.14f), " 3.140000");

    // double should accept sign specifier
    EXPECT_NO_THROW(fmt::format("{:+f}", 3.14));
    EXPECT_EQ(fmt::format("{:+f}", 3.14), "+3.140000");
}

// Test that on_sign works correctly for character types
TEST(OnSignTest, CharType) {
    // char should accept sign specifier (it's signed)
    EXPECT_NO_THROW(fmt::format("{:+c}", 'A'));
    EXPECT_EQ(fmt::format("{:+c}", 'A'), "+A");
}

// Test that on_sign works correctly for string types (should be rejected)
TEST(OnSignTest, StringType) {
    // string should throw because it's not numeric
    EXPECT_THROW(fmt::format("{:+s}", "hello"), fmt::format_error);
}

// Test that on_sign works correctly for bool type (should be rejected)
TEST(OnSignTest, BoolType) {
    EXPECT_THROW(fmt::format("{:+b}", true), fmt::format_error);
}

// Test edge case: zero values with sign specifier
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

// Test edge case: negative values with sign specifier
TEST(OnSignTest, NegativeValues) {
    EXPECT_EQ(fmt::format("{:+d}", -42), "-42");
    EXPECT_EQ(fmt::format("{:-d}", -42), "-42");
    EXPECT_EQ(fmt::format("{: d}", -42), "-42");
}

// Test that on_sign works with width and precision specifiers
TEST(OnSignTest, WithWidthAndPrecision) {
    EXPECT_EQ(fmt::format("{:+10d}", 42), "       +42");
    EXPECT_EQ(fmt::format("{:+10.2f}", 3.14), "     +3.14");
}

// Test that on_sign correctly handles int128_type if available
TEST(OnSignTest, Int128Type) {
    // int128 should accept sign specifier
    __int128 val = 42;
    EXPECT_NO_THROW(fmt::format("{:+d}", val));
    EXPECT_EQ(fmt::format("{:+d}", val), "+42");
}

// Test that on_sign correctly rejects unsigned int128 if available
TEST(OnSignTest, UnsignedInt128Type) {
    unsigned __int128 val = 42;
    EXPECT_THROW(fmt::format("{:+u}", val), fmt::format_error);
}