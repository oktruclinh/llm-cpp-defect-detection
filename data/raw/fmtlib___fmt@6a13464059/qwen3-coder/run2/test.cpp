#include <gtest/gtest.h>
#include <fmt/core.h>
#include <fmt/format.h>

// Test fixture to exercise the on_sign function through public API
class FmtSignTest : public ::testing::Test {
protected:
    // We cannot directly call on_sign since it's protected, so we use fmt::format
    // to trigger the formatting path that invokes on_sign internally.
    // We test by attempting to format integers with sign specifiers and observing
    // whether errors are thrown for unsigned types, or succeed for signed types.
};

// Test that signed integers (int, long, long long, int128) work with sign specifier
TEST_F(FmtSignTest, SignedIntegerTypesAcceptSignSpecifier) {
    // int (type::int_type)
    EXPECT_NO_THROW(fmt::format("{:+}", 42));
    EXPECT_EQ(fmt::format("{:+}", 42), "+42");

    // long (type::long_long_type on many platforms, but at least integral)
    EXPECT_NO_THROW(fmt::format("{:+}", static_cast<long>(-123)));
    EXPECT_EQ(fmt::format("{:+}", static_cast<long>(-123)), "-123");

    // long long (type::long_long_type)
    EXPECT_NO_THROW(fmt::format("{:+}", static_cast<long long>(0)));
    EXPECT_EQ(fmt::format("{:+}", static_cast<long long>(0)), "+0");

    // int128 (if available, fmt supports it via compiler extensions)
    // We test with a known int128 value if available, but fmt's public API
    // accepts __int128 or __uint128_t if supported by the compiler.
#ifdef FMT_HAS_INT128
    __int128 val = 123;
    EXPECT_NO_THROW(fmt::format("{:+}", val));
#endif
}

// Test that unsigned integer types (unsigned int, unsigned long, etc.) reject sign specifier
TEST_F(FmtSignTest, UnsignedIntegerTypesRejectSignSpecifier) {
    // unsigned int
    EXPECT_THROW(fmt::format("{:+}", static_cast<unsigned int>(42)), fmt::format_error);
    
    // unsigned long
    EXPECT_THROW(fmt::format("{:+}", static_cast<unsigned long>(42)), fmt::format_error);
    
    // unsigned long long
    EXPECT_THROW(fmt::format("{:+}", static_cast<unsigned long long>(42)), fmt::format_error);
    
    // char (unsigned char if char is unsigned)
    EXPECT_THROW(fmt::format("{:+}", static_cast<unsigned char>(42)), fmt::format_error);
    
    // uint16_t, uint32_t, uint64_t
    EXPECT_THROW(fmt::format("{:+}", static_cast<uint16_t>(42)), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+}", static_cast<uint32_t>(42)), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+}", static_cast<uint64_t>(42)), fmt::format_error);
    
    // size_t (typically unsigned)
    EXPECT_THROW(fmt::format("{:+}", static_cast<size_t>(42)), fmt::format_error);
}

// Test that char (signed) works with sign specifier
TEST_F(FmtSignTest, SignedCharAcceptsSignSpecifier) {
    // char (if signed)
    EXPECT_NO_THROW(fmt::format("{:+}", static_cast<char>(42)));
    // Note: char formatting may not show '+' for positive values in some implementations
    // but the key is that it does NOT throw. The error is only thrown for unsigned integral types.
    // The actual output may vary, but the function should not throw.
}

// Test that zero values (boundary) for signed types work
TEST_F(FmtSignTest, ZeroSignedValuesAcceptSignSpecifier) {
    EXPECT_NO_THROW(fmt::format("{:+}", 0));
    EXPECT_EQ(fmt::format("{:+}", 0), "+0");
    
    EXPECT_NO_THROW(fmt::format("{:+}", 0LL));
    EXPECT_EQ(fmt::format("{:+}", 0LL), "+0");
    
    EXPECT_NO_THROW(fmt::format("{:+}", static_cast<short>(0)));
    EXPECT_EQ(fmt::format("{:+}", static_cast<short>(0)), "+0");
}

// Test that negative values work (edge case)
TEST_F(FmtSignTest, NegativeSignedValuesAcceptSignSpecifier) {
    EXPECT_NO_THROW(fmt::format("{:+}", -42));
    EXPECT_EQ(fmt::format("{:+}", -42), "-42");
    
    EXPECT_NO_THROW(fmt::format("{:+}", -42LL));
    EXPECT_EQ(fmt::format("{:+}", -42LL), "-42");
}

// Test that non-integral types (float, double) are handled differently
// Note: on_sign is only called for integral types. For floating point, the behavior is different.
// But the function requires_numeric_argument() is called for all numeric types.
// However, the error condition in on_sign only triggers for unsigned integral types.
// So floating point should NOT throw here.
TEST_F(FmtSignTest, FloatingPointTypesAcceptSignSpecifier) {
    EXPECT_NO_THROW(fmt::format("{:+}", 3.14));
    EXPECT_EQ(fmt::format("{:+}", 3.14), "+3.14");
    
    EXPECT_NO_THROW(fmt::format("{:+}", -2.71));
    EXPECT_EQ(fmt::format("{:+}", -2.71), "-2.71");
}

// Test that bool (integral but not signed in the sense of the condition) is rejected
TEST_F(FmtSignTest, BoolRejectsSignSpecifier) {
    EXPECT_THROW(fmt::format("{:+}", true), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+}", false), fmt::format_error);
}

// Test that enum class (integral type) is rejected if unsigned
enum class TestEnum : unsigned int {
    VALUE = 42
};

TEST_F(FmtSignTest, UnsignedEnumClassRejectsSignSpecifier) {
    EXPECT_THROW(fmt::format("{:+}", TestEnum::VALUE), fmt::format_error);
}

// Test that enum (default int, signed) is accepted
enum TestSignedEnum {
    VALUE2 = 42
};

TEST_F(FmtSignTest, SignedEnumAcceptsSignSpecifier) {
    EXPECT_NO_THROW(fmt::format("{:+}", TestSignedEnum::VALUE2));
    // Output may be "+42" or just "42" depending on formatter, but must not throw
}