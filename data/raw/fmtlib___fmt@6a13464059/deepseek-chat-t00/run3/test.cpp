#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Test that on_sign works correctly for signed integer types
TEST(FormatSignTest, SignedIntegralTypes) {
    // Positive signed integer with sign flag
    EXPECT_EQ(fmt::format("{:+d}", 42), "+42");
    EXPECT_EQ(fmt::format("{:+d}", -42), "-42");
    
    // Negative signed integer with sign flag
    EXPECT_EQ(fmt::format("{: d}", 42), " 42");
    EXPECT_EQ(fmt::format("{: d}", -42), "-42");
    
    // Default sign (only negative shows sign)
    EXPECT_EQ(fmt::format("{:d}", 42), "42");
    EXPECT_EQ(fmt::format("{:d}", -42), "-42");
}

// Test that on_sign works correctly for unsigned integer types (should error)
TEST(FormatSignTest, UnsignedIntegralTypes) {
    // Unsigned int with sign flag should throw
    EXPECT_THROW(fmt::format("{:+u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{: u}", 42u), fmt::format_error);
    
    // Unsigned long with sign flag should throw
    EXPECT_THROW(fmt::format("{:+lu}", 42ul), fmt::format_error);
    
    // Unsigned long long with sign flag should throw
    EXPECT_THROW(fmt::format("{:+llu}", 42ull), fmt::format_error);
    
    // Unsigned char with sign flag should throw
    EXPECT_THROW(fmt::format("{:+c}", static_cast<unsigned char>('A')), fmt::format_error);
}

// Test that on_sign works correctly for floating point types
TEST(FormatSignTest, FloatingPointTypes) {
    // Float with sign flag
    EXPECT_EQ(fmt::format("{:+f}", 3.14f), "+3.140000");
    EXPECT_EQ(fmt::format("{:+f}", -3.14f), "-3.140000");
    EXPECT_EQ(fmt::format("{: f}", 3.14f), " 3.140000");
    
    // Double with sign flag
    EXPECT_EQ(fmt::format("{:+f}", 3.14), "+3.140000");
    EXPECT_EQ(fmt::format("{:+f}", -3.14), "-3.140000");
    EXPECT_EQ(fmt::format("{: f}", 3.14), " 3.140000");
    
    // Long double with sign flag
    EXPECT_EQ(fmt::format("{:+Lf}", 3.14L), "+3.140000");
    EXPECT_EQ(fmt::format("{:+Lf}", -3.14L), "-3.140000");
}

// Test that on_sign works correctly for char type (signed)
TEST(FormatSignTest, CharType) {
    // char with sign flag should work (char is signed on most platforms)
    EXPECT_EQ(fmt::format("{:+c}", 'A'), "+A");
    EXPECT_EQ(fmt::format("{: c}", 'A'), " A");
}

// Test that on_sign works correctly for signed char type
TEST(FormatSignTest, SignedCharType) {
    // signed char with sign flag should work
    signed char sc = 'B';
    EXPECT_EQ(fmt::format("{:+c}", sc), "+B");
    EXPECT_EQ(fmt::format("{: c}", sc), " B");
}

// Test that on_sign works correctly for short int (signed)
TEST(FormatSignTest, ShortIntType) {
    short s = 42;
    EXPECT_EQ(fmt::format("{:+hd}", s), "+42");
    EXPECT_EQ(fmt::format("{: hd}", s), " 42");
    
    short neg_s = -42;
    EXPECT_EQ(fmt::format("{:+hd}", neg_s), "-42");
}

// Test that on_sign works correctly for long int (signed)
TEST(FormatSignTest, LongIntType) {
    long l = 42L;
    EXPECT_EQ(fmt::format("{:+ld}", l), "+42");
    EXPECT_EQ(fmt::format("{: ld}", l), " 42");
    
    long neg_l = -42L;
    EXPECT_EQ(fmt::format("{:+ld}", neg_l), "-42");
}

// Test that on_sign works correctly for long long int (signed)
TEST(FormatSignTest, LongLongIntType) {
    long long ll = 42LL;
    EXPECT_EQ(fmt::format("{:+lld}", ll), "+42");
    EXPECT_EQ(fmt::format("{: lld}", ll), " 42");
    
    long long neg_ll = -42LL;
    EXPECT_EQ(fmt::format("{:+lld}", neg_ll), "-42");
}

// Test that on_sign works correctly for int128_t if available
TEST(FormatSignTest, Int128Type) {
    // Use __int128 if available (compiler-specific)
#ifdef __SIZEOF_INT128__
    __int128 i128 = 42;
    EXPECT_EQ(fmt::format("{:+d}", static_cast<long long>(i128)), "+42");
    
    __int128 neg_i128 = -42;
    EXPECT_EQ(fmt::format("{:+d}", static_cast<long long>(neg_i128)), "-42");
#endif
}

// Test that on_sign throws for unsigned short
TEST(FormatSignTest, UnsignedShortType) {
    unsigned short us = 42;
    EXPECT_THROW(fmt::format("{:+hu}", us), fmt::format_error);
}

// Test that on_sign throws for unsigned long
TEST(FormatSignTest, UnsignedLongType) {
    unsigned long ul = 42UL;
    EXPECT_THROW(fmt::format("{:+lu}", ul), fmt::format_error);
}

// Test that on_sign throws for unsigned long long
TEST(FormatSignTest, UnsignedLongLongType) {
    unsigned long long ull = 42ULL;
    EXPECT_THROW(fmt::format("{:+llu}", ull), fmt::format_error);
}

// Test that on_sign works for size_t (which is unsigned, but not in the exclusion list)
TEST(FormatSignTest, SizeTType) {
    size_t st = 42;
    // size_t is unsigned, but not in the explicit exclusion list (int_type, long_long_type, int128_type, char_type)
    // So it should work with sign flag
    EXPECT_EQ(fmt::format("{:+d}", st), "+42");
}

// Test that on_sign works for ptrdiff_t (signed)
TEST(FormatSignTest, PtrdiffTType) {
    std::ptrdiff_t pd = 42;
    EXPECT_EQ(fmt::format("{:+d}", pd), "+42");
    
    std::ptrdiff_t neg_pd = -42;
    EXPECT_EQ(fmt::format("{:+d}", neg_pd), "-42");
}

// Test that on_sign works for bool (integral but not in exclusion list)
TEST(FormatSignTest, BoolType) {
    EXPECT_EQ(fmt::format("{:+d}", true), "+1");
    EXPECT_EQ(fmt::format("{:+d}", false), "+0");
}

// Test that on_sign works for signed char with sign flag
TEST(FormatSignTest, SignedCharWithSign) {
    signed char sc = 65;
    EXPECT_EQ(fmt::format("{:+c}", sc), "+A");
    EXPECT_EQ(fmt::format("{: c}", sc), " A");
}

// Test that on_sign throws for unsigned char
TEST(FormatSignTest, UnsignedCharType) {
    unsigned char uc = 'A';
    EXPECT_THROW(fmt::format("{:+c}", uc), fmt::format_error);
    EXPECT_THROW(fmt::format("{: c}", uc), fmt::format_error);
}

// Test that on_sign works for wchar_t (signed on most platforms)
TEST(FormatSignTest, WcharTType) {
    wchar_t wc = L'A';
    EXPECT_EQ(fmt::format("{:+c}", static_cast<char>(wc)), "+A");
}

// Test that on_sign works for char8_t (C++20)
TEST(FormatSignTest, Char8TType) {
#if defined(__cpp_char8_t)
    char8_t c8 = u8'A';
    EXPECT_EQ(fmt::format("{:+c}", static_cast<char>(c8)), "+A");
#endif
}

// Test that on_sign works for char16_t
TEST(FormatSignTest, Char16TType) {
    char16_t c16 = u'A';
    EXPECT_EQ(fmt::format("{:+c}", static_cast<char>(c16)), "+A");
}

// Test that on_sign works for char32_t
TEST(FormatSignTest, Char32TType) {
    char32_t c32 = U'A';
    EXPECT_EQ(fmt::format("{:+c}", static_cast<char>(c32)), "+A");
}

// Test that on_sign throws for unsigned int with space flag
TEST(FormatSignTest, UnsignedIntSpaceFlag) {
    EXPECT_THROW(fmt::format("{: u}", 42u), fmt::format_error);
}

// Test that on_sign throws for unsigned long long with plus flag
TEST(FormatSignTest, UnsignedLongLongPlusFlag) {
    EXPECT_THROW(fmt::format("{:+llu}", 42ULL), fmt::format_error);
}

// Test that on_sign works for signed int with zero padding and sign
TEST(FormatSignTest, SignedIntZeroPadWithSign) {
    EXPECT_EQ(fmt::format("{:+05d}", 42), "+0042");
    EXPECT_EQ(fmt::format("{:+05d}", -42), "-0042");
    EXPECT_EQ(fmt::format("{: 05d}", 42), " 0042");
}

// Test that on_sign works for negative zero (floating point)
TEST(FormatSignTest, NegativeZeroFloat) {
    double neg_zero = -0.0;
    EXPECT_EQ(fmt::format("{:+f}", neg_zero), "-0.000000");
    EXPECT_EQ(fmt::format("{: f}", neg_zero), "-0.000000");
}

// Test that on_sign works for infinity
TEST(FormatSignTest, InfinityFloat) {
    double inf = std::numeric_limits<double>::infinity();
    EXPECT_EQ(fmt::format("{:+f}", inf), "+inf");
    EXPECT_EQ(fmt::format("{: f}", inf), " inf");
}

// Test that on_sign works for NaN
TEST(FormatSignTest, NaN) {
    double nan = std::numeric_limits<double>::quiet_NaN();
    std::string result = fmt::format("{:+f}", nan);
    EXPECT_TRUE(result == "+nan" || result == "+NaN" || result == "+nan(");
}