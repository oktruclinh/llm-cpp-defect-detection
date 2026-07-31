#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Test that on_sign works correctly for signed integer types
TEST(OnSignTest, SignedIntegerTypes) {
    // Signed integers should accept sign specifiers without error
    EXPECT_EQ(fmt::format("{:+d}", 42), "+42");
    EXPECT_EQ(fmt::format("{: d}", -42), "-42");
    EXPECT_EQ(fmt::format("{:-d}", 42), "42");
    EXPECT_EQ(fmt::format("{:+d}", -42), "-42");
}

// Test that on_sign works correctly for unsigned integer types
TEST(OnSignTest, UnsignedIntegerTypes) {
    // Unsigned integers should reject sign specifiers
    EXPECT_THROW(fmt::format("{:+u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{: u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-u}", 42u), fmt::format_error);
}

// Test that on_sign works correctly for floating-point types
TEST(OnSignTest, FloatingPointTypes) {
    // Floating-point types should accept sign specifiers
    EXPECT_EQ(fmt::format("{:+f}", 3.14), "+3.140000");
    EXPECT_EQ(fmt::format("{: f}", -3.14), "-3.140000");
    EXPECT_EQ(fmt::format("{:-f}", 3.14), "3.140000");
    EXPECT_EQ(fmt::format("{:+f}", -3.14), "-3.140000");
}

// Test that on_sign works correctly for char type
TEST(OnSignTest, CharType) {
    // Char type should accept sign specifiers (it's signed)
    EXPECT_EQ(fmt::format("{:+c}", 'A'), "+A");
    EXPECT_EQ(fmt::format("{: c}", 'A'), " A");
    EXPECT_EQ(fmt::format("{:-c}", 'A'), "A");
}

// Test that on_sign works correctly for long long type
TEST(OnSignTest, LongLongType) {
    // Long long should accept sign specifiers
    EXPECT_EQ(fmt::format("{:+lld}", 42LL), "+42");
    EXPECT_EQ(fmt::format("{: lld}", -42LL), "-42");
}

// Test that on_sign works correctly for unsigned long long type
TEST(OnSignTest, UnsignedLongLongType) {
    // Unsigned long long should reject sign specifiers
    EXPECT_THROW(fmt::format("{:+llu}", 42ULL), fmt::format_error);
    EXPECT_THROW(fmt::format("{: llu}", 42ULL), fmt::format_error);
}

// Test that on_sign works correctly for short type
TEST(OnSignTest, ShortType) {
    // Short should accept sign specifiers
    short s = 42;
    EXPECT_EQ(fmt::format("{:+hd}", s), "+42");
    EXPECT_EQ(fmt::format("{: hd}", s), " 42");
}

// Test that on_sign works correctly for unsigned short type
TEST(OnSignTest, UnsignedShortType) {
    // Unsigned short should reject sign specifiers
    unsigned short us = 42;
    EXPECT_THROW(fmt::format("{:+hu}", us), fmt::format_error);
    EXPECT_THROW(fmt::format("{: hu}", us), fmt::format_error);
}

// Test that on_sign works correctly for int type
TEST(OnSignTest, IntType) {
    // Int should accept sign specifiers
    EXPECT_EQ(fmt::format("{:+d}", 0), "+0");
    EXPECT_EQ(fmt::format("{: d}", 0), " 0");
    EXPECT_EQ(fmt::format("{:-d}", 0), "0");
}

// Test that on_sign works correctly for unsigned int type
TEST(OnSignTest, UnsignedIntType) {
    // Unsigned int should reject sign specifiers
    EXPECT_THROW(fmt::format("{:+u}", 0u), fmt::format_error);
    EXPECT_THROW(fmt::format("{: u}", 0u), fmt::format_error);
}

// Test that on_sign works correctly for long type
TEST(OnSignTest, LongType) {
    // Long should accept sign specifiers
    EXPECT_EQ(fmt::format("{:+ld}", 42L), "+42");
    EXPECT_EQ(fmt::format("{: ld}", -42L), "-42");
}

// Test that on_sign works correctly for unsigned long type
TEST(OnSignTest, UnsignedLongType) {
    // Unsigned long should reject sign specifiers
    EXPECT_THROW(fmt::format("{:+lu}", 42UL), fmt::format_error);
    EXPECT_THROW(fmt::format("{: lu}", 42UL), fmt::format_error);
}

// Test that on_sign works correctly for size_t type
TEST(OnSignTest, SizeTType) {
    // size_t is unsigned, should reject sign specifiers
    size_t st = 42;
    EXPECT_THROW(fmt::format("{:+zu}", st), fmt::format_error);
    EXPECT_THROW(fmt::format("{: zu}", st), fmt::format_error);
}

// Test that on_sign works correctly for ptrdiff_t type
TEST(OnSignTest, PtrdiffTType) {
    // ptrdiff_t is signed, should accept sign specifiers
    ptrdiff_t pd = 42;
    EXPECT_EQ(fmt::format("{:+td}", pd), "+42");
    EXPECT_EQ(fmt::format("{: td}", pd), " 42");
}

// Test that on_sign works correctly for int8_t type
TEST(OnSignTest, Int8Type) {
    // int8_t is signed, should accept sign specifiers
    int8_t i8 = 42;
    EXPECT_EQ(fmt::format("{:+hhd}", i8), "+42");
    EXPECT_EQ(fmt::format("{: hhd}", i8), " 42");
}

// Test that on_sign works correctly for uint8_t type
TEST(OnSignTest, Uint8Type) {
    // uint8_t is unsigned, should reject sign specifiers
    uint8_t ui8 = 42;
    EXPECT_THROW(fmt::format("{:+hhu}", ui8), fmt::format_error);
    EXPECT_THROW(fmt::format("{: hhu}", ui8), fmt::format_error);
}

// Test that on_sign works correctly for int16_t type
TEST(OnSignTest, Int16Type) {
    // int16_t is signed, should accept sign specifiers
    int16_t i16 = 42;
    EXPECT_EQ(fmt::format("{:+hd}", i16), "+42");
}

// Test that on_sign works correctly for uint16_t type
TEST(OnSignTest, Uint16Type) {
    // uint16_t is unsigned, should reject sign specifiers
    uint16_t ui16 = 42;
    EXPECT_THROW(fmt::format("{:+hu}", ui16), fmt::format_error);
}

// Test that on_sign works correctly for int32_t type
TEST(OnSignTest, Int32Type) {
    // int32_t is signed, should accept sign specifiers
    int32_t i32 = 42;
    EXPECT_EQ(fmt::format("{:+d}", i32), "+42");
}

// Test that on_sign works correctly for uint32_t type
TEST(OnSignTest, Uint32Type) {
    // uint32_t is unsigned, should reject sign specifiers
    uint32_t ui32 = 42;
    EXPECT_THROW(fmt::format("{:+u}", ui32), fmt::format_error);
}

// Test that on_sign works correctly for int64_t type
TEST(OnSignTest, Int64Type) {
    // int64_t is signed, should accept sign specifiers
    int64_t i64 = 42;
    EXPECT_EQ(fmt::format("{:+ld}", i64), "+42");
}

// Test that on_sign works correctly for uint64_t type
TEST(OnSignTest, Uint64Type) {
    // uint64_t is unsigned, should reject sign specifiers
    uint64_t ui64 = 42;
    EXPECT_THROW(fmt::format("{:+lu}", ui64), fmt::format_error);
}

// Test that on_sign works correctly for bool type
TEST(OnSignTest, BoolType) {
    // Bool is integral but not signed, should reject sign specifiers
    EXPECT_THROW(fmt::format("{:+d}", true), fmt::format_error);
    EXPECT_THROW(fmt::format("{: d}", false), fmt::format_error);
}

// Test that on_sign works correctly for double type
TEST(OnSignTest, DoubleType) {
    // Double should accept sign specifiers
    EXPECT_EQ(fmt::format("{:+e}", 1.0), "+1.000000e+00");
    EXPECT_EQ(fmt::format("{: e}", -1.0), "-1.000000e+00");
    EXPECT_EQ(fmt::format("{:+g}", 1.0), "+1");
}

// Test that on_sign works correctly for long double type
TEST(OnSignTest, LongDoubleType) {
    // Long double should accept sign specifiers
    long double ld = 1.0L;
    EXPECT_EQ(fmt::format("{:+Lf}", ld), "+1.000000");
}

// Test that on_sign works correctly for negative zero
TEST(OnSignTest, NegativeZero) {
    // Negative zero should be handled correctly with sign specifiers
    EXPECT_EQ(fmt::format("{:+f}", -0.0), "+0.000000");
    EXPECT_EQ(fmt::format("{: f}", -0.0), "-0.000000");
}

// Test that on_sign works correctly for NaN
TEST(OnSignTest, NaN) {
    // NaN should be handled correctly with sign specifiers
    double nan = std::numeric_limits<double>::quiet_NaN();
    EXPECT_EQ(fmt::format("{:+f}", nan), "+nan");
    EXPECT_EQ(fmt::format("{: f}", nan), " nan");
}

// Test that on_sign works correctly for infinity
TEST(OnSignTest, Infinity) {
    // Infinity should be handled correctly with sign specifiers
    double inf = std::numeric_limits<double>::infinity();
    EXPECT_EQ(fmt::format("{:+f}", inf), "+inf");
    EXPECT_EQ(fmt::format("{: f}", -inf), "-inf");
}

// Test that on_sign works correctly with format_to
TEST(OnSignTest, FormatTo) {
    // Test through format_to API
    std::string out;
    fmt::format_to(std::back_inserter(out), "{:+d}", 42);
    EXPECT_EQ(out, "+42");
}

// Test that on_sign works correctly with format_to_n
TEST(OnSignTest, FormatToN) {
    // Test through format_to_n API
    char buf[10];
    auto result = fmt::format_to_n(buf, 10, "{:+d}", 42);
    *result.out = '\0';
    EXPECT_EQ(std::string(buf), "+42");
}

// Test that on_sign works correctly with formatted_size
TEST(OnSignTest, FormattedSize) {
    // Test through formatted_size API
    EXPECT_EQ(fmt::formatted_size("{:+d}", 42), 3);
    EXPECT_EQ(fmt::formatted_size("{: d}", -42), 3);
}