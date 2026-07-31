#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Test that on_sign is called correctly for signed integer types
TEST(OnSignTest, SignedIntegerTypes) {
    // Test with signed int
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+d}", 42);
        EXPECT_EQ(result, "+42");
    });
    
    // Test with signed long long
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+lld}", 42LL);
        EXPECT_EQ(result, "+42");
    });
    
    // Test with signed char
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+c}", static_cast<signed char>('A'));
        EXPECT_EQ(result, "+A");
    });
}

// Test that on_sign throws for unsigned integer types (requires signed argument)
TEST(OnSignTest, UnsignedIntegerTypes) {
    // Test with unsigned int
    EXPECT_THROW({
        fmt::format("{:+u}", 42u);
    }, fmt::format_error);
    
    // Test with unsigned long long
    EXPECT_THROW({
        fmt::format("{:+llu}", 42ULL);
    }, fmt::format_error);
    
    // Test with unsigned char
    EXPECT_THROW({
        fmt::format("{:+c}", static_cast<unsigned char>('A'));
    }, fmt::format_error);
    
    // Test with size_t (unsigned)
    EXPECT_THROW({
        fmt::format("{:+zu}", static_cast<size_t>(42));
    }, fmt::format_error);
}

// Test that on_sign works with floating-point types (not affected by the signed check)
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
    
    // Test with negative float
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+f}", -1.5);
        EXPECT_EQ(result, "-1.500000");
    });
}

// Test that on_sign works with character types (signed char is allowed)
TEST(OnSignTest, CharType) {
    // Signed char should work (it's explicitly excluded from the error check)
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+c}", static_cast<signed char>('X'));
        EXPECT_EQ(result, "+X");
    });
}

// Test that on_sign with no sign specifier works normally
TEST(OnSignTest, NoSignSpecifier) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:d}", 42);
        EXPECT_EQ(result, "42");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:u}", 42u);
        EXPECT_EQ(result, "42");
    });
}

// Test that on_sign with space flag works for signed types
TEST(OnSignTest, SpaceFlag) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{: d}", 42);
        EXPECT_EQ(result, " 42");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{: d}", -42);
        EXPECT_EQ(result, "-42");
    });
}

// Test that on_sign with minus flag works
TEST(OnSignTest, MinusFlag) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:-d}", 42);
        EXPECT_EQ(result, "42");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:-d}", -42);
        EXPECT_EQ(result, "-42");
    });
}

// Test edge case: zero value with sign
TEST(OnSignTest, ZeroValue) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+d}", 0);
        EXPECT_EQ(result, "+0");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+f}", 0.0);
        EXPECT_EQ(result, "+0.000000");
    });
}

// Test edge case: maximum/minimum signed values
TEST(OnSignTest, ExtremeSignedValues) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+d}", std::numeric_limits<int>::max());
        EXPECT_EQ(result, "+2147483647");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+d}", std::numeric_limits<int>::min());
        EXPECT_EQ(result, "-2147483648");
    });
}

// Test that on_sign error message is correct
TEST(OnSignTest, ErrorMessage) {
    try {
        fmt::format("{:+u}", 42u);
        FAIL() << "Expected fmt::format_error";
    } catch (const fmt::format_error& e) {
        EXPECT_STREQ(e.what(), "format specifier requires signed argument");
    }
}

// Test with multiple format specifiers including sign
TEST(OnSignTest, MultipleSpecifiers) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+10d}", 42);
        EXPECT_EQ(result, "       +42");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+010d}", 42);
        EXPECT_EQ(result, "+000000042");
    });
}