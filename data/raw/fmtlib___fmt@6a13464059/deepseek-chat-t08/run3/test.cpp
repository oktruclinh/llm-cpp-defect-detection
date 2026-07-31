#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Test that formatting signed integral types does not trigger the error
TEST(OnSignTest, SignedIntegralTypesSucceed) {
    // int
    EXPECT_NO_THROW(fmt::format("{:+}", 42));
    EXPECT_EQ(fmt::format("{:+}", 42), "+42");
    // long long
    EXPECT_NO_THROW(fmt::format("{:+}", -5LL));
    EXPECT_EQ(fmt::format("{:+}", -5LL), "-5");
    // char (signed by default on most platforms)
    EXPECT_NO_THROW(fmt::format("{:+}", 'A'));
    EXPECT_EQ(fmt::format("{:+}", 'A'), "+65");
}

// Test that unsigned integral types with sign specifier throw format_error
TEST(OnSignTest, UnsignedIntegralTypesThrow) {
    EXPECT_THROW(fmt::format("{:+}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+}", 0u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+}", 1u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+}", 12345678u), fmt::format_error);
}

// Test that unsigned long long throws
TEST(OnSignTest, UnsignedLongLongThrows) {
    EXPECT_THROW(fmt::format("{:+}", 42ULL), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+}", 0ULL), fmt::format_error);
}

// Test that unsigned char (which is unsigned char type, not char) throws
TEST(OnSignTest, UnsignedCharThrows) {
    // unsigned char is a distinct type from char
    unsigned char uc = 'A';
    EXPECT_THROW(fmt::format("{:+}", uc), fmt::format_error);
}

// Test that size_t (unsigned) throws
TEST(OnSignTest, SizeTThrows) {
    EXPECT_THROW(fmt::format("{:+}", size_t{0}), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+}", size_t{42}), fmt::format_error);
}

// Test that bool (integral but unsigned) throws
TEST(OnSignTest, BoolThrows) {
    EXPECT_THROW(fmt::format("{:+}", true), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+}", false), fmt::format_error);
}

// Test that floating-point types accept sign specifier
TEST(OnSignTest, FloatingPointSucceed) {
    EXPECT_NO_THROW(fmt::format("{:+}", 3.14));
    EXPECT_EQ(fmt::format("{:+}", 3.14), "+3.14");
    EXPECT_NO_THROW(fmt::format("{:+}", -2.5));
    EXPECT_EQ(fmt::format("{:+}", -2.5), "-2.5");
    EXPECT_NO_THROW(fmt::format("{:+}", 0.0));
    // The sign for +0.0 might be "+0" or "+0.0" depending on precision, just check no throw
}

// Test that strings (non-numeric) throw because require_numeric_argument fails first
TEST(OnSignTest, StringThrows) {
    EXPECT_THROW(fmt::format("{:+}", std::string("hello")), fmt::format_error);
}

// Test that custom unsigned types (e.g., uint8_t) throw
TEST(OnSignTest, Uint8TThrows) {
    uint8_t val = 42;
    EXPECT_THROW(fmt::format("{:+}", val), fmt::format_error);
}

// Test that uint16_t throws
TEST(OnSignTest, Uint16TThrows) {
    uint16_t val = 1000;
    EXPECT_THROW(fmt::format("{:+}", val), fmt::format_error);
}

// Test that uint32_t throws
TEST(OnSignTest, Uint32TThrows) {
    uint32_t val = 123456;
    EXPECT_THROW(fmt::format("{:+}", val), fmt::format_error);
}

// Test that uint64_t throws
TEST(OnSignTest, Uint64TThrows) {
    uint64_t val = 1234567890123ULL;
    EXPECT_THROW(fmt::format("{:+}", val), fmt::format_error);
}

// Test that negative sign specifier ("-") works for all types (no error)
TEST(OnSignTest, NegativeSignNoThrow) {
    EXPECT_NO_THROW(fmt::format("{:-}", 42));
    EXPECT_NO_THROW(fmt::format("{:-}", 42u));
    EXPECT_NO_THROW(fmt::format("{:-}", 3.14));
    EXPECT_NO_THROW(fmt::format("{:-}", 'A'));
}

// Test that space sign specifier (" ") works for unsigned types (no error)
TEST(OnSignTest, SpaceSignNoThrow) {
    EXPECT_NO_THROW(fmt::format("{: }", 42u));
    EXPECT_NO_THROW(fmt::format("{: }", 42));
    EXPECT_NO_THROW(fmt::format("{: }", 3.14));
}

// Test that sign specifier with unsigned and explicit type specifier (like 'd') still throws
TEST(OnSignTest, UnsignedWithExplicitTypeThrows) {
    EXPECT_THROW(fmt::format("{:+d}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+u}", 42u), fmt::format_error);
}

// Test zero value edge case for unsigned types
TEST(OnSignTest, UnsignedZeroThrows) {
    EXPECT_THROW(fmt::format("{:+}", 0u), fmt::format_error);
    unsigned int zero = 0;
    EXPECT_THROW(fmt::format("{:+}", zero), fmt::format_error);
}

// Test signed char (which is signed integral) does not throw
TEST(OnSignTest, SignedCharSucceeds) {
    signed char sc = -1;
    EXPECT_NO_THROW(fmt::format("{:+}", sc));
    EXPECT_EQ(fmt::format("{:+}", sc), "-1");
}