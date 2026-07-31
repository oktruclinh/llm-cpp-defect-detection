#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Test that on_sign works correctly for signed integer types
TEST(OnSignTest, SignedIntegralTypes) {
    // int_type should accept sign specifiers
    EXPECT_NO_THROW(fmt::format("{:+d}", 42));
    EXPECT_EQ(fmt::format("{:+d}", 42), "+42");
    EXPECT_EQ(fmt::format("{: d}", 42), " 42");
    EXPECT_EQ(fmt::format("{:-d}", 42), "42");
    
    // long_long_type should accept sign specifiers
    EXPECT_NO_THROW(fmt::format("{:+lld}", 42LL));
    EXPECT_EQ(fmt::format("{:+lld}", 42LL), "+42");
    
    // char_type should accept sign specifiers
    EXPECT_NO_THROW(fmt::format("{:+c}", 'A'));
}

// Test that on_sign correctly rejects unsigned integral types
TEST(OnSignTest, UnsignedIntegralTypes) {
    // unsigned int should throw
    EXPECT_THROW(fmt::format("{:+u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{: u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-u}", 42u), fmt::format_error);
    
    // unsigned long long should throw
    EXPECT_THROW(fmt::format("{:+llu}", 42ULL), fmt::format_error);
    
    // unsigned char should throw
    EXPECT_THROW(fmt::format("{:+hhu}", static_cast<unsigned char>(42)), fmt::format_error);
}

// Test that on_sign correctly rejects other unsigned types
TEST(OnSignTest, OtherUnsignedTypes) {
    // size_t (unsigned) should throw
    EXPECT_THROW(fmt::format("{:+zu}", static_cast<size_t>(42)), fmt::format_error);
    
    // uint64_t should throw
    EXPECT_THROW(fmt::format("{:+lu}", static_cast<uint64_t>(42)), fmt::format_error);
}

// Test that on_sign works correctly for floating-point types
TEST(OnSignTest, FloatingPointTypes) {
    // Floating point should accept sign specifiers
    EXPECT_NO_THROW(fmt::format("{:+f}", 3.14));
    EXPECT_EQ(fmt::format("{:+f}", 3.14), "+3.140000");
    EXPECT_EQ(fmt::format("{: f}", 3.14), " 3.140000");
    EXPECT_EQ(fmt::format("{:-f}", 3.14), "3.140000");
    
    // Negative floating point
    EXPECT_EQ(fmt::format("{:+f}", -3.14), "-3.140000");
}

// Test that on_sign works correctly for string types (should accept sign)
TEST(OnSignTest, StringTypes) {
    // Strings should accept sign specifiers (they are not integral)
    EXPECT_NO_THROW(fmt::format("{:+s}", std::string("hello")));
    EXPECT_EQ(fmt::format("{:+s}", std::string("hello")), "+hello");
}

// Test that on_sign works correctly for bool type
TEST(OnSignTest, BoolType) {
    // Bool is integral but not int_type, long_long_type, int128_type, or char_type
    // According to the function, it should throw
    EXPECT_THROW(fmt::format("{:+d}", true), fmt::format_error);
}

// Test that on_sign works correctly for negative numbers with sign
TEST(OnSignTest, NegativeNumbers) {
    EXPECT_EQ(fmt::format("{:+d}", -42), "-42");
    EXPECT_EQ(fmt::format("{: d}", -42), "-42");
    EXPECT_EQ(fmt::format("{:-d}", -42), "-42");
}

// Test that on_sign works correctly for zero
TEST(OnSignTest, ZeroValue) {
    EXPECT_EQ(fmt::format("{:+d}", 0), "+0");
    EXPECT_EQ(fmt::format("{: d}", 0), " 0");
    EXPECT_EQ(fmt::format("{:-d}", 0), "0");
}

// Test that on_sign works correctly with width and precision
TEST(OnSignTest, WithWidthAndPrecision) {
    EXPECT_EQ(fmt::format("{:+5d}", 42), "  +42");
    EXPECT_EQ(fmt::format("{:+05d}", 42), "+0042");
    EXPECT_EQ(fmt::format("{:+8.3f}", 3.14), "  +3.140");
}

// Test that on_sign correctly identifies int128_type (if available)
TEST(OnSignTest, Int128Type) {
    // __int128_t should accept sign specifiers
    // Note: This test may not compile on all platforms
    #ifdef __SIZEOF_INT128__
    __int128_t val = 42;
    EXPECT_NO_THROW(fmt::format("{:+d}", val));
    EXPECT_EQ(fmt::format("{:+d}", val), "+42");
    #endif
}

// Test that on_sign correctly rejects unsigned __int128 (if available)
TEST(OnSignTest, UnsignedInt128Type) {
    #ifdef __SIZEOF_INT128__
    __uint128_t val = 42;
    EXPECT_THROW(fmt::format("{:+d}", val), fmt::format_error);
    #endif
}

// Test that on_sign works correctly for short int (signed)
TEST(OnSignTest, ShortIntType) {
    short s = 42;
    EXPECT_NO_THROW(fmt::format("{:+hd}", s));
    EXPECT_EQ(fmt::format("{:+hd}", s), "+42");
}

// Test that on_sign correctly rejects unsigned short
TEST(OnSignTest, UnsignedShortType) {
    unsigned short us = 42;
    EXPECT_THROW(fmt::format("{:+hu}", us), fmt::format_error);
}

// Test that on_sign works correctly for long int (signed)
TEST(OnSignTest, LongIntType) {
    long l = 42L;
    EXPECT_NO_THROW(fmt::format("{:+ld}", l));
    EXPECT_EQ(fmt::format("{:+ld}", l), "+42");
}

// Test that on_sign correctly rejects unsigned long
TEST(OnSignTest, UnsignedLongType) {
    unsigned long ul = 42UL;
    EXPECT_THROW(fmt::format("{:+lu}", ul), fmt::format_error);
}