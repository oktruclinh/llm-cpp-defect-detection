#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Test that on_sign works correctly for signed integer types
TEST(OnSignTest, SignedIntegerTypes) {
    // Test with int (signed)
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+d}", 42);
        EXPECT_EQ(result, "+42");
    });
    
    // Test with long long (signed)
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+lld}", 42LL);
        EXPECT_EQ(result, "+42");
    });
    
    // Test with char (signed on most platforms)
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+c}", 'A');
        EXPECT_EQ(result, "+A");
    });
}

// Test that on_sign throws error for unsigned integer types
TEST(OnSignTest, UnsignedIntegerTypes) {
    // Test with unsigned int
    EXPECT_THROW({
        fmt::format("{:+u}", 42u);
    }, fmt::format_error);
    
    // Test with unsigned long long
    EXPECT_THROW({
        fmt::format("{:+llu}", 42ULL);
    }, fmt::format_error);
    
    // Test with size_t (unsigned)
    EXPECT_THROW({
        fmt::format("{:+zu}", static_cast<size_t>(42));
    }, fmt::format_error);
}

// Test that on_sign works correctly for floating point types
TEST(OnSignTest, FloatingPointTypes) {
    // Test with double
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+f}", 3.14);
        EXPECT_EQ(result, "+3.140000");
    });
    
    // Test with float
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+f}", 2.5f);
        EXPECT_EQ(result, "+2.500000");
    });
    
    // Test with negative floating point
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+f}", -3.14);
        EXPECT_EQ(result, "-3.140000");
    });
}

// Test that on_sign works correctly for signed char
TEST(OnSignTest, SignedCharType) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+c}", static_cast<signed char>('B'));
        EXPECT_EQ(result, "+B");
    });
}

// Test that on_sign works correctly for zero values
TEST(OnSignTest, ZeroValues) {
    // Signed integer zero
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+d}", 0);
        EXPECT_EQ(result, "+0");
    });
    
    // Floating point zero
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+f}", 0.0);
        EXPECT_EQ(result, "+0.000000");
    });
}

// Test that on_sign works correctly for negative signed integers
TEST(OnSignTest, NegativeSignedIntegers) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+d}", -42);
        EXPECT_EQ(result, "-42");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+lld}", -42LL);
        EXPECT_EQ(result, "-42");
    });
}

// Test that on_sign works with space flag (alternative to +)
TEST(OnSignTest, SpaceFlag) {
    // Signed integer with space
    EXPECT_NO_THROW({
        auto result = fmt::format("{: d}", 42);
        EXPECT_EQ(result, " 42");
    });
    
    // Negative signed integer with space
    EXPECT_NO_THROW({
        auto result = fmt::format("{: d}", -42);
        EXPECT_EQ(result, "-42");
    });
    
    // Unsigned integer with space should throw
    EXPECT_THROW({
        fmt::format("{: u}", 42u);
    }, fmt::format_error);
}

// Test that on_sign works with both + and - flags
TEST(OnSignTest, MinusFlagCombination) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+-d}", 42);
        EXPECT_EQ(result, "+42");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+-d}", -42);
        EXPECT_EQ(result, "-42");
    });
}

// Test that on_sign works with width and precision
TEST(OnSignTest, WidthAndPrecision) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+5d}", 42);
        EXPECT_EQ(result, "  +42");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+5d}", -42);
        EXPECT_EQ(result, "  -42");
    });
}

// Test that on_sign throws for unsigned types in various format specifiers
TEST(OnSignTest, UnsignedTypesVariousSpecifiers) {
    // Test with different unsigned types
    EXPECT_THROW(fmt::format("{:+u}", 0u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+lu}", 0UL), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+llu}", 0ULL), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+zu}", static_cast<size_t>(0)), fmt::format_error);
}

// Test that on_sign works for pointer types (should not throw)
TEST(OnSignTest, PointerTypes) {
    int x = 42;
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+p}", fmt::ptr(&x));
        // Pointer format with sign is implementation-defined but should not throw
    });
}

// Test that on_sign works for string types (should not throw)
TEST(OnSignTest, StringTypes) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+s}", std::string("hello"));
        EXPECT_EQ(result, "+hello");
    });
}