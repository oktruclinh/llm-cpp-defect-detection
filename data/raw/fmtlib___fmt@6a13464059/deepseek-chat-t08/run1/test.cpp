#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Test that on_sign works correctly for signed integral types
TEST(OnSignTest, SignedIntegralTypes) {
    // int_type should accept sign specifier
    EXPECT_NO_THROW(fmt::format("{:+d}", 42));
    EXPECT_EQ(fmt::format("{:+d}", 42), "+42");
    EXPECT_EQ(fmt::format("{:-d}", 42), "42");
    EXPECT_EQ(fmt::format("{: d}", 42), " 42");
    
    // long_long_type should accept sign specifier
    EXPECT_NO_THROW(fmt::format("{:+lld}", 42LL));
    EXPECT_EQ(fmt::format("{:+lld}", 42LL), "+42");
    
    // char_type should accept sign specifier
    EXPECT_NO_THROW(fmt::format("{:+c}", 'A'));
    EXPECT_EQ(fmt::format("{:+c}", 'A'), "+A");
}

// Test that on_sign rejects unsigned integral types
TEST(OnSignTest, UnsignedIntegralTypes) {
    // unsigned int should throw
    EXPECT_THROW(fmt::format("{:+u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{: u}", 42u), fmt::format_error);
    
    // unsigned long long should throw
    EXPECT_THROW(fmt::format("{:+llu}", 42ULL), fmt::format_error);
}

// Test that on_sign works correctly for floating-point types
TEST(OnSignTest, FloatingPointTypes) {
    // Floating point should accept sign specifier
    EXPECT_NO_THROW(fmt::format("{:+f}", 3.14));
    EXPECT_EQ(fmt::format("{:+f}", 3.14), "+3.140000");
    EXPECT_EQ(fmt::format("{:-f}", 3.14), "3.140000");
    EXPECT_EQ(fmt::format("{: f}", 3.14), " 3.140000");
    
    // Negative floating point
    EXPECT_EQ(fmt::format("{:+f}", -3.14), "-3.140000");
}

// Test that on_sign works with zero values
TEST(OnSignTest, ZeroValues) {
    // Zero signed int
    EXPECT_EQ(fmt::format("{:+d}", 0), "+0");
    EXPECT_EQ(fmt::format("{:-d}", 0), "0");
    EXPECT_EQ(fmt::format("{: d}", 0), " 0");
    
    // Zero unsigned int should still throw
    EXPECT_THROW(fmt::format("{:+u}", 0u), fmt::format_error);
    
    // Zero float
    EXPECT_EQ(fmt::format("{:+f}", 0.0), "+0.000000");
}

// Test that on_sign works with negative values
TEST(OnSignTest, NegativeValues) {
    // Negative signed int with plus sign (sign is overridden by minus)
    EXPECT_EQ(fmt::format("{:+d}", -42), "-42");
    EXPECT_EQ(fmt::format("{:-d}", -42), "-42");
    EXPECT_EQ(fmt::format("{: d}", -42), "-42");
}

// Test that on_sign works with various signed integer types
TEST(OnSignTest, VariousSignedTypes) {
    // short
    EXPECT_EQ(fmt::format("{:+hd}", static_cast<short>(42)), "+42");
    
    // signed char
    EXPECT_EQ(fmt::format("{:+hhd}", static_cast<signed char>(42)), "+42");
    
    // long
    EXPECT_EQ(fmt::format("{:+ld}", 42L), "+42");
    
    // long long
    EXPECT_EQ(fmt::format("{:+lld}", 42LL), "+42");
}

// Test that on_sign rejects unsigned types that could be confused with signed
TEST(OnSignTest, UnsignedTypeRejection) {
    // These should all throw because they are unsigned integral types
    EXPECT_THROW(fmt::format("{:+o}", 42), fmt::format_error);   // unsigned octal
    EXPECT_THROW(fmt::format("{:+x}", 42), fmt::format_error);   // unsigned hex
    EXPECT_THROW(fmt::format("{:+X}", 42), fmt::format_error);   // unsigned hex uppercase
    EXPECT_THROW(fmt::format("{:+b}", 42), fmt::format_error);   // unsigned binary
}

// Test that on_sign works with size_t (which is unsigned)
TEST(OnSignTest, SizeT) {
    // size_t is unsigned, should throw
    EXPECT_THROW(fmt::format("{:+zu}", static_cast<size_t>(42)), fmt::format_error);
}

// Test that on_sign handles the case where format specifier requires signed argument error message
TEST(OnSignTest, ErrorMessage) {
    try {
        fmt::format("{:+u}", 42u);
        FAIL() << "Expected fmt::format_error";
    } catch (const fmt::format_error& e) {
        EXPECT_STREQ(e.what(), "format specifier requires signed argument");
    }
}

// Test that on_sign works with string types (should not be affected)
TEST(OnSignTest, StringTypes) {
    // Strings don't use sign specifier in the same way
    EXPECT_NO_THROW(fmt::format("{}", "hello"));
    // Sign specifier with string should throw
    EXPECT_THROW(fmt::format("{:+s}", "hello"), fmt::format_error);
}