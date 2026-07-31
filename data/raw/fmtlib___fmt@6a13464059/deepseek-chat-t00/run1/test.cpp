#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Test that on_sign works correctly for signed integer types
TEST(OnSignTest, SignedIntegralTypes) {
    // int_type should accept sign specifier
    EXPECT_NO_THROW(fmt::format("{:+d}", 42));
    EXPECT_EQ(fmt::format("{:+d}", 42), "+42");
    EXPECT_EQ(fmt::format("{:-d}", 42), "42");
    EXPECT_EQ(fmt::format("{: d}", 42), " 42");

    // long_long_type should accept sign specifier
    EXPECT_NO_THROW(fmt::format("{:+lld}", 42LL));
    EXPECT_EQ(fmt::format("{:+lld}", 42LL), "+42");
}

// Test that on_sign correctly rejects unsigned integral types
TEST(OnSignTest, UnsignedIntegralTypes) {
    // unsigned int should throw when sign specifier is used
    EXPECT_THROW(fmt::format("{:+u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{: u}", 42u), fmt::format_error);

    // unsigned long long should throw
    EXPECT_THROW(fmt::format("{:+llu}", 42ULL), fmt::format_error);
}

// Test that on_sign works correctly for floating point types
TEST(OnSignTest, FloatingPointTypes) {
    // Floating point types should accept sign specifier
    EXPECT_NO_THROW(fmt::format("{:+f}", 3.14));
    EXPECT_EQ(fmt::format("{:+f}", 3.14), "+3.140000");
    EXPECT_EQ(fmt::format("{:-f}", 3.14), "3.140000");
    EXPECT_EQ(fmt::format("{: f}", 3.14), " 3.140000");
}

// Test that on_sign works correctly for char_type
TEST(OnSignTest, CharType) {
    // char_type should accept sign specifier
    EXPECT_NO_THROW(fmt::format("{:+c}", 'A'));
    EXPECT_EQ(fmt::format("{:+c}", 'A'), "+A");
}

// Test that on_sign works correctly for signed char
TEST(OnSignTest, SignedCharType) {
    // signed char should accept sign specifier
    EXPECT_NO_THROW(fmt::format("{:+d}", static_cast<signed char>(65)));
}

// Test that on_sign works correctly for short int
TEST(OnSignTest, ShortIntType) {
    // short int should accept sign specifier
    EXPECT_NO_THROW(fmt::format("{:+hd}", static_cast<short>(42)));
    EXPECT_EQ(fmt::format("{:+hd}", static_cast<short>(42)), "+42");
}

// Test that on_sign rejects unsigned short
TEST(OnSignTest, UnsignedShortType) {
    // unsigned short should throw when sign specifier is used
    EXPECT_THROW(fmt::format("{:+hu}", static_cast<unsigned short>(42)), fmt::format_error);
}

// Test that on_sign rejects unsigned char
TEST(OnSignTest, UnsignedCharType) {
    // unsigned char should throw when sign specifier is used
    EXPECT_THROW(fmt::format("{:+d}", static_cast<unsigned char>(65)), fmt::format_error);
}

// Test that on_sign rejects size_t (unsigned)
TEST(OnSignTest, SizeTType) {
    // size_t should throw when sign specifier is used
    EXPECT_THROW(fmt::format("{:+d}", static_cast<size_t>(42)), fmt::format_error);
}

// Test that on_sign works correctly for negative signed integers
TEST(OnSignTest, NegativeSignedIntegers) {
    EXPECT_EQ(fmt::format("{:+d}", -42), "-42");
    EXPECT_EQ(fmt::format("{:-d}", -42), "-42");
    EXPECT_EQ(fmt::format("{: d}", -42), "-42");
}

// Test that on_sign works correctly for zero
TEST(OnSignTest, ZeroValue) {
    EXPECT_EQ(fmt::format("{:+d}", 0), "+0");
    EXPECT_EQ(fmt::format("{:-d}", 0), "0");
    EXPECT_EQ(fmt::format("{: d}", 0), " 0");
}

// Test that on_sign works with width and precision
TEST(OnSignTest, WithWidthAndPrecision) {
    EXPECT_EQ(fmt::format("{:+5d}", 42), "  +42");
    EXPECT_EQ(fmt::format("{:+05d}", 42), "+0042");
    EXPECT_EQ(fmt::format("{:+8.3f}", 3.14), "  +3.140");
}

// Test that on_sign works with different format specifiers
TEST(OnSignTest, DifferentFormatSpecifiers) {
    EXPECT_EQ(fmt::format("{:+x}", 255), "+ff");
    EXPECT_EQ(fmt::format("{:+o}", 255), "+377");
    EXPECT_EQ(fmt::format("{:+b}", 255), "+11111111");
}

// Test that on_sign correctly handles the case where sign is not specified
TEST(OnSignTest, NoSignSpecifier) {
    // Without sign specifier, no error should occur for unsigned types
    EXPECT_NO_THROW(fmt::format("{:d}", 42u));
    EXPECT_EQ(fmt::format("{:d}", 42u), "42");
}