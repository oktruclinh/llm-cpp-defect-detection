#include <gtest/gtest.h>
#include "arrow/util/basic_decimal.h"

// Test fixture for BasicDecimal128 to avoid repeating common setup
class BasicDecimal128Test : public ::testing::Test {
 protected:
  // Helper function to create BasicDecimal128 from parts
  arrow::BasicDecimal128 FromParts(int64_t high_bits, uint64_t low_bits) {
    return arrow::BasicDecimal128(high_bits, low_bits);
  }
};

// Test case for constructing BasicDecimal128 from various integer types
TEST_F(BasicDecimal128Test, ConstructorFromIntegerTypes) {
  // Test with int8_t
  int8_t i8_pos = 123;
  arrow::BasicDecimal128 dec_i8_pos(i8_pos);
  ASSERT_EQ(dec_i8_pos, FromParts(0, 123));

  int8_t i8_neg = -123;
  arrow::BasicDecimal128 dec_i8_neg(i8_neg);
  ASSERT_EQ(dec_i8_neg, FromParts(-1, static_cast<uint64_t>(-123)));

  int8_t i8_zero = 0;
  arrow::BasicDecimal128 dec_i8_zero(i8_zero);
  ASSERT_EQ(dec_i8_zero, FromParts(0, 0));

  // Test with uint8_t
  uint8_t ui8_pos = 255;
  arrow::BasicDecimal128 dec_ui8_pos(ui8_pos);
  ASSERT_EQ(dec_ui8_pos, FromParts(0, 255));

  uint8_t ui8_zero = 0;
  arrow::BasicDecimal128 dec_ui8_zero(ui8_zero);
  ASSERT_EQ(dec_ui8_zero, FromParts(0, 0));

  // Test with int16_t
  int16_t i16_pos = 32767;
  arrow::BasicDecimal128 dec_i16_pos(i16_pos);
  ASSERT_EQ(dec_i16_pos, FromParts(0, 32767));

  int16_t i16_neg = -32768;
  arrow::BasicDecimal128 dec_i16_neg(i16_neg);
  ASSERT_EQ(dec_i16_neg, FromParts(-1, static_cast<uint64_t>(-32768)));

  // Test with uint16_t
  uint16_t ui16_pos = 65535;
  arrow::BasicDecimal128 dec_ui16_pos(ui16_pos);
  ASSERT_EQ(dec_ui16_pos, FromParts(0, 65535));

  // Test with int32_t
  int32_t i32_pos = 2147483647;
  arrow::BasicDecimal128 dec_i32_pos(i32_pos);
  ASSERT_EQ(dec_i32_pos, FromParts(0, 2147483647));

  int32_t i32_neg = -2147483647 - 1; // std::numeric_limits<int32_t>::min()
  arrow::BasicDecimal128 dec_i32_neg(i32_neg);
  ASSERT_EQ(dec_i32_neg, FromParts(-1, static_cast<uint64_t>(i32_neg)));

  // Test with uint32_t
  uint32_t ui32_pos = 4294967295U;
  arrow::BasicDecimal128 dec_ui32_pos(ui32_pos);
  ASSERT_EQ(dec_ui32_pos, FromParts(0, 4294967295U));

  // Test with int64_t
  int64_t i64_pos = 9223372036854775807LL; // std::numeric_limits<int64_t>::max()
  arrow::BasicDecimal128 dec_i64_pos(i64_pos);
  ASSERT_EQ(dec_i64_pos, FromParts(0, 9223372036854775807ULL));

  int64_t i64_neg = -9223372036854775807LL - 1; // std::numeric_limits<int64_t>::min()
  arrow::BasicDecimal128 dec_i64_neg(i64_neg);
  ASSERT_EQ(dec_i64_neg, FromParts(-1, static_cast<uint64_t>(i64_neg)));

  // Test with uint64_t
  uint64_t ui64_pos = 18446744073709551615ULL; // std::numeric_limits<uint64_t>::max()
  arrow::BasicDecimal128 dec_ui64_pos(ui64_pos);
  ASSERT_EQ(dec_ui64_pos, FromParts(0, 18446744073709551615ULL));

  uint64_t ui64_zero = 0ULL;
  arrow::BasicDecimal128 dec_ui64_zero(ui64_zero);
  ASSERT_EQ(dec_ui64_zero, FromParts(0, 0ULL));
}

// Test edge cases for zero and one
TEST_F(BasicDecimal128Test, ConstructorFromZeroAndOne) {
  arrow::BasicDecimal128 dec_zero(0);
  ASSERT_EQ(dec_zero, FromParts(0, 0));

  arrow::BasicDecimal128 dec_one(1);
  ASSERT_EQ(dec_one, FromParts(0, 1));

  arrow::BasicDecimal128 dec_neg_one(-1);
  ASSERT_EQ(dec_neg_one, FromParts(-1, static_cast<uint64_t>(-1)));
}

// Test with specific values that might cross boundaries for high_bits/low_bits
TEST_F(BasicDecimal128Test, ConstructorSpecificValues) {
  // A value that fits entirely in low_bits and is positive
  int64_t val1 = 123456789012345LL;
  arrow::BasicDecimal128 dec1(val1);
  ASSERT_EQ(dec1, FromParts(0, static_cast<uint64_t>(val1)));

  // A value that fits entirely in low_bits and is negative
  int64_t val2 = -123456789012345LL;
  arrow::BasicDecimal128 dec2(val2);
  ASSERT_EQ(dec2, FromParts(-1, static_cast<uint64_t>(val2)));

  // Test with a value that is just below 2^63 - 1 (max positive int64)
  int64_t max_int64 = std::numeric_limits<int64_t>::max();
  arrow::BasicDecimal128 dec_max_int64(max_int64);
  ASSERT_EQ(dec_max_int64, FromParts(0, static_cast<uint64_t>(max_int64)));

  // Test with a value that is just above -2^63 (min negative int64)
  int64_t min_int64 = std::numeric_limits<int64_t>::min();
  arrow::BasicDecimal128 dec_min_int64(min_int64);
  ASSERT_EQ(dec_min_int64, FromParts(-1, static_cast<uint64_t>(min_int64)));
}