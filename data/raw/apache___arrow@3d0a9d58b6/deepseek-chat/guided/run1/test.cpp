#include <gtest/gtest.h>
#include "arrow/util/basic_decimal.h"
#include <cstdint>
#include <limits>
#include <type_traits>

namespace arrow {
namespace {

// Test fixture for BasicDecimal128 construction from integral types
class BasicDecimal128FromIntegralTest : public ::testing::Test {
 protected:
  // Helper to verify the low and high parts of a BasicDecimal128
  void VerifyParts(const BasicDecimal128& decimal, int64_t expected_high, uint64_t expected_low) {
    // Access high and low through the public API - we can use ToString() or comparison
    // but since we don't have direct access to parts, we'll use the public constructors
    // and comparison operators
    BasicDecimal128 expected(expected_high, expected_low);
    EXPECT_EQ(expected, decimal);
  }
};

TEST_F(BasicDecimal128FromIntegralTest, FromZero) {
  BasicDecimal128 decimal(0);
  // 0 should have high=0, low=0
  BasicDecimal128 expected(0, 0);
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromPositiveInt8) {
  BasicDecimal128 decimal(static_cast<int8_t>(42));
  BasicDecimal128 expected(0, 42);
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromNegativeInt8) {
  BasicDecimal128 decimal(static_cast<int8_t>(-42));
  // For negative values: high = -1, low = static_cast<uint64_t>(value)
  BasicDecimal128 expected(-1, static_cast<uint64_t>(static_cast<int8_t>(-42)));
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromMaxInt8) {
  BasicDecimal128 decimal(std::numeric_limits<int8_t>::max());
  BasicDecimal128 expected(0, static_cast<uint64_t>(std::numeric_limits<int8_t>::max()));
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromMinInt8) {
  BasicDecimal128 decimal(std::numeric_limits<int8_t>::min());
  BasicDecimal128 expected(-1, static_cast<uint64_t>(std::numeric_limits<int8_t>::min()));
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromPositiveInt16) {
  BasicDecimal128 decimal(static_cast<int16_t>(12345));
  BasicDecimal128 expected(0, 12345);
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromNegativeInt16) {
  BasicDecimal128 decimal(static_cast<int16_t>(-12345));
  BasicDecimal128 expected(-1, static_cast<uint64_t>(static_cast<int16_t>(-12345)));
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromMaxInt16) {
  BasicDecimal128 decimal(std::numeric_limits<int16_t>::max());
  BasicDecimal128 expected(0, static_cast<uint64_t>(std::numeric_limits<int16_t>::max()));
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromMinInt16) {
  BasicDecimal128 decimal(std::numeric_limits<int16_t>::min());
  BasicDecimal128 expected(-1, static_cast<uint64_t>(std::numeric_limits<int16_t>::min()));
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromPositiveInt32) {
  BasicDecimal128 decimal(static_cast<int32_t>(123456789));
  BasicDecimal128 expected(0, 123456789);
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromNegativeInt32) {
  BasicDecimal128 decimal(static_cast<int32_t>(-123456789));
  BasicDecimal128 expected(-1, static_cast<uint64_t>(static_cast<int32_t>(-123456789)));
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromMaxInt32) {
  BasicDecimal128 decimal(std::numeric_limits<int32_t>::max());
  BasicDecimal128 expected(0, static_cast<uint64_t>(std::numeric_limits<int32_t>::max()));
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromMinInt32) {
  BasicDecimal128 decimal(std::numeric_limits<int32_t>::min());
  BasicDecimal128 expected(-1, static_cast<uint64_t>(std::numeric_limits<int32_t>::min()));
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromPositiveInt64) {
  BasicDecimal128 decimal(static_cast<int64_t>(1234567890123456789LL));
  BasicDecimal128 expected(0, 1234567890123456789ULL);
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromNegativeInt64) {
  BasicDecimal128 decimal(static_cast<int64_t>(-1234567890123456789LL));
  BasicDecimal128 expected(-1, static_cast<uint64_t>(static_cast<int64_t>(-1234567890123456789LL)));
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromMaxInt64) {
  BasicDecimal128 decimal(std::numeric_limits<int64_t>::max());
  BasicDecimal128 expected(0, static_cast<uint64_t>(std::numeric_limits<int64_t>::max()));
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromMinInt64) {
  BasicDecimal128 decimal(std::numeric_limits<int64_t>::min());
  BasicDecimal128 expected(-1, static_cast<uint64_t>(std::numeric_limits<int64_t>::min()));
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromUnsignedInt8) {
  BasicDecimal128 decimal(static_cast<uint8_t>(200));
  BasicDecimal128 expected(0, 200);
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromUnsignedInt16) {
  BasicDecimal128 decimal(static_cast<uint16_t>(60000));
  BasicDecimal128 expected(0, 60000);
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromUnsignedInt32) {
  BasicDecimal128 decimal(static_cast<uint32_t>(4000000000UL));
  BasicDecimal128 expected(0, 4000000000ULL);
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromUnsignedInt64) {
  BasicDecimal128 decimal(static_cast<uint64_t>(18446744073709551615ULL));
  BasicDecimal128 expected(0, 18446744073709551615ULL);
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromMaxUnsignedInt64) {
  BasicDecimal128 decimal(std::numeric_limits<uint64_t>::max());
  BasicDecimal128 expected(0, std::numeric_limits<uint64_t>::max());
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromMinUnsignedInt64) {
  BasicDecimal128 decimal(std::numeric_limits<uint64_t>::min());
  BasicDecimal128 expected(0, std::numeric_limits<uint64_t>::min());
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromChar) {
  // char is integral but typically signed or unsigned; test both possibilities
  BasicDecimal128 decimal(static_cast<char>('A'));
  BasicDecimal128 expected(0, static_cast<uint64_t>(static_cast<char>('A')));
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromBoolTrue) {
  BasicDecimal128 decimal(true);
  BasicDecimal128 expected(0, 1);
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromBoolFalse) {
  BasicDecimal128 decimal(false);
  BasicDecimal128 expected(0, 0);
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromNegativeOne) {
  BasicDecimal128 decimal(static_cast<int32_t>(-1));
  BasicDecimal128 expected(-1, static_cast<uint64_t>(static_cast<int32_t>(-1)));
  EXPECT_EQ(expected, decimal);
}

TEST_F(BasicDecimal128FromIntegralTest, FromOne) {
  BasicDecimal128 decimal(static_cast<int32_t>(1));
  BasicDecimal128 expected(0, 1);
  EXPECT_EQ(expected, decimal);
}

// Test that the constructor is constexpr (compile-time check)
static_assert(BasicDecimal128(42) == BasicDecimal128(0, 42), "constexpr test failed");
static_assert(BasicDecimal128(-42) == BasicDecimal128(-1, static_cast<uint64_t>(-42)), "constexpr test failed");

}  // namespace
}  // namespace arrow