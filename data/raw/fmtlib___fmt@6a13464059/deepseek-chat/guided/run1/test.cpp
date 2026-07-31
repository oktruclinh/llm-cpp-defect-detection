#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Test that on_sign works correctly for signed integer types
TEST(FormatSignTest, SignedIntegerTypes) {
    // Positive sign for signed integers
    EXPECT_EQ(fmt::format("{:+d}", 42), "+42");
    EXPECT_EQ(fmt::format("{:+d}", -42), "-42");
    
    // Space sign for signed integers
    EXPECT_EQ(fmt::format("{: d}", 42), " 42");
    EXPECT_EQ(fmt::format("{: d}", -42), "-42");
    
    // Minus sign (default) for signed integers
    EXPECT_EQ(fmt::format("{:-d}", 42), "42");
    EXPECT_EQ(fmt::format("{:-d}", -42), "-42");
}

// Test that on_sign works correctly for floating-point types
TEST(FormatSignTest, FloatingPointTypes) {
    // Positive sign for floating point
    EXPECT_EQ(fmt::format("{:+f}", 3.14), "+3.140000");
    EXPECT_EQ(fmt::format("{:+f}", -3.14), "-3.140000");
    
    // Space sign for floating point
    EXPECT_EQ(fmt::format("{: f}", 3.14), " 3.140000");
    EXPECT_EQ(fmt::format("{: f}", -3.14), "-3.140000");
}

// Test that on_sign throws for unsigned integer types
TEST(FormatSignDeathTest, UnsignedIntegerTypes) {
    // Unsigned int with sign should throw
    EXPECT_THROW(fmt::format("{:+u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{: u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-u}", 42u), fmt::format_error);
    
    // Unsigned long with sign should throw
    EXPECT_THROW(fmt::format("{:+lu}", 42ul), fmt::format_error);
    EXPECT_THROW(fmt::format("{: lu}", 42ul), fmt::format_error);
    
    // Unsigned long long with sign should throw
    EXPECT_THROW(fmt::format("{:+llu}", 42ull), fmt::format_error);
    EXPECT_THROW(fmt::format("{: llu}", 42ull), fmt::format_error);
}

// Test that on_sign works for char type (signed)
TEST(FormatSignTest, CharType) {
    EXPECT_EQ(fmt::format("{:+c}", 'A'), "+A");
    EXPECT_EQ(fmt::format("{: c}", 'A'), " A");
    EXPECT_EQ(fmt::format("{:-c}", 'A'), "A");
}

// Test that on_sign works for signed char
TEST(FormatSignTest, SignedChar) {
    // signed char is treated as signed integer
    EXPECT_EQ(fmt::format("{:+d}", static_cast<signed char>(65)), "+65");
    EXPECT_EQ(fmt::format("{: d}", static_cast<signed char>(65)), " 65");
}

// Test that on_sign throws for unsigned char
TEST(FormatSignDeathTest, UnsignedChar) {
    // unsigned char with sign should throw
    EXPECT_THROW(fmt::format("{:+d}", static_cast<unsigned char>(65)), fmt::format_error);
    EXPECT_THROW(fmt::format("{: d}", static_cast<unsigned char>(65)), fmt::format_error);
}

// Test that on_sign works for short (signed)
TEST(FormatSignTest, ShortType) {
    EXPECT_EQ(fmt::format("{:+hd}", static_cast<short>(42)), "+42");
    EXPECT_EQ(fmt::format("{: hd}", static_cast<short>(42)), " 42");
}

// Test that on_sign throws for unsigned short
TEST(FormatSignDeathTest, UnsignedShort) {
    EXPECT_THROW(fmt::format("{:+hu}", static_cast<unsigned short>(42)), fmt::format_error);
    EXPECT_THROW(fmt::format("{: hu}", static_cast<unsigned short>(42)), fmt::format_error);
}

// Test that on_sign works for long (signed)
TEST(FormatSignTest, LongType) {
    EXPECT_EQ(fmt::format("{:+ld}", 42l), "+42");
    EXPECT_EQ(fmt::format("{: ld}", 42l), " 42");
}

// Test that on_sign works for long long (signed)
TEST(FormatSignTest, LongLongType) {
    EXPECT_EQ(fmt::format("{:+lld}", 42ll), "+42");
    EXPECT_EQ(fmt::format("{: lld}", 42ll), " 42");
}

// Test that on_sign works for int128 if available
TEST(FormatSignTest, Int128Type) {
    // __int128 is not standard, but if available, test it
    #ifdef __SIZEOF_INT128__
    __int128 val = 42;
    EXPECT_EQ(fmt::format("{:+d}", val), "+42");
    EXPECT_EQ(fmt::format("{: d}", val), " 42");
    #endif
}

// Test that on_sign throws for unsigned int128 if available
TEST(FormatSignDeathTest, UnsignedInt128) {
    #ifdef __SIZEOF_INT128__
    unsigned __int128 val = 42;
    EXPECT_THROW(fmt::format("{:+d}", val), fmt::format_error);
    EXPECT_THROW(fmt::format("{: d}", val), fmt::format_error);
    #endif
}

// Test that on_sign works for size_t (which is unsigned, but not in the excluded list)
// Note: size_t is typically unsigned long or unsigned long long, which are excluded
TEST(FormatSignDeathTest, SizeTType) {
    // size_t is unsigned, so sign should throw
    EXPECT_THROW(fmt::format("{:+zu}", static_cast<size_t>(42)), fmt::format_error);
    EXPECT_THROW(fmt::format("{: zu}", static_cast<size_t>(42)), fmt::format_error);
}

// Test that on_sign works for ptrdiff_t (signed)
TEST(FormatSignTest, PtrdiffTType) {
    EXPECT_EQ(fmt::format("{:+td}", static_cast<ptrdiff_t>(42)), "+42");
    EXPECT_EQ(fmt::format("{: td}", static_cast<ptrdiff_t>(42)), " 42");
}

// Test that on_sign works for int (the most basic signed type)
TEST(FormatSignTest, BasicIntType) {
    EXPECT_EQ(fmt::format("{:+d}", 0), "+0");
    EXPECT_EQ(fmt::format("{: d}", 0), " 0");
    EXPECT_EQ(fmt::format("{:-d}", 0), "0");
    EXPECT_EQ(fmt::format("{:+d}", -0), "+0");
}

// Test that on_sign throws for bool (which is integral but not signed)
TEST(FormatSignDeathTest, BoolType) {
    EXPECT_THROW(fmt::format("{:+d}", true), fmt::format_error);
    EXPECT_THROW(fmt::format("{: d}", false), fmt::format_error);
}