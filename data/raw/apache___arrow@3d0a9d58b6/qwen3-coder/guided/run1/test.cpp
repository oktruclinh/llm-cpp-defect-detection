#include <gtest/gtest.h>
#include <arrow/util/basic_decimal.h>
#include <cstdint>

using arrow::BasicDecimal128;

TEST(BasicDecimal128ConstructorTest, ConstructFromPositiveInt) {
  int32_t value = 42;
  BasicDecimal128 decimal(value);
  
  // Check that the value is correctly represented
  ASSERT_EQ(decimal.low_bits(), static_cast<uint64_t>(value));
  ASSERT_EQ(decimal.high_bits(), 0);
}

TEST(BasicDecimal128ConstructorTest, ConstructFromNegativeInt) {
  int32_t value = -42;
  BasicDecimal128 decimal(value);
  
  // Check that the value is correctly represented with sign extension
  ASSERT_EQ(decimal.low_bits(), static_cast<uint64_t>(value));
  ASSERT_EQ(decimal.high_bits(), static_cast<uint64_t>(-1));
}

TEST(BasicDecimal128ConstructorTest, ConstructFromZero) {
  int64_t value = 0;
  BasicDecimal128 decimal(value);
  
  // Check that zero is correctly represented
  ASSERT_EQ(decimal.low_bits(), 0);
  ASSERT_EQ(decimal.high_bits(), 0);
}

TEST(BasicDecimal128ConstructorTest, ConstructFromMaxInt32) {
  int32_t value = std::numeric_limits<int32_t>::max();
  BasicDecimal128 decimal(value);
  
  // Check that max int32 is correctly represented
  ASSERT_EQ(decimal.low_bits(), static_cast<uint64_t>(value));
  ASSERT_EQ(decimal.high_bits(), 0);
}

TEST(BasicDecimal128ConstructorTest, ConstructFromMinInt32) {
  int32_t value = std::numeric_limits<int32_t>::min();
  BasicDecimal128 decimal(value);
  
  // Check that min int32 is correctly represented with sign extension
  ASSERT_EQ(decimal.low_bits(), static_cast<uint64_t>(value));
  ASSERT_EQ(decimal.high_bits(), static_cast<uint64_t>(-1));
}

TEST(BasicDecimal128ConstructorTest, ConstructFromMaxInt64) {
  int64_t value = std::numeric_limits<int64_t>::max();
  BasicDecimal128 decimal(value);
  
  // Check that max int64 is correctly represented
  ASSERT_EQ(decimal.low_bits(), static_cast<uint64_t>(value));
  ASSERT_EQ(decimal.high_bits(), 0);
}

TEST(BasicDecimal128ConstructorTest, ConstructFromMinInt64) {
  int64_t value = std::numeric_limits<int64_t>::min();
  BasicDecimal128 decimal(value);
  
  // Check that min int64 is correctly represented with sign extension
  ASSERT_EQ(decimal.low_bits(), static_cast<uint64_t>(value));
  ASSERT_EQ(decimal.high_bits(), static_cast<uint64_t>(-1));
}

TEST(BasicDecimal128ConstructorTest, ConstructFromUint32) {
  uint32_t value = 123456;
  BasicDecimal128 decimal(value);
  
  // Check that uint32 is correctly represented
  ASSERT_EQ(decimal.low_bits(), static_cast<uint64_t>(value));
  ASSERT_EQ(decimal.high_bits(), 0);
}

TEST(BasicDecimal128ConstructorTest, ConstructFromUint64) {
  uint64_t value = 9876543210ULL;
  BasicDecimal128 decimal(value);
  
  // Check that uint64 is correctly represented
  ASSERT_EQ(decimal.low_bits(), value);
  ASSERT_EQ(decimal.high_bits(), 0);
}

TEST(BasicDecimal128ConstructorTest, ConstructFromSmallNegative) {
  int16_t value = -1;
  BasicDecimal128 decimal(value);
  
  // Check that small negative is correctly represented
  ASSERT_EQ(decimal.low_bits(), static_cast<uint64_t>(value));
  ASSERT_EQ(decimal.high_bits(), static_cast<uint64_t>(-1));
}

TEST(BasicDecimal128ConstructorTest, ConstructFromLargeNegative) {
  int64_t value = -9223372036854775807LL - 1LL;  // INT64_MIN
  BasicDecimal128 decimal(value);
  
  // Check that large negative is correctly represented
  ASSERT_EQ(decimal.low_bits(), static_cast<uint64_t>(value));
  ASSERT_EQ(decimal.high_bits(), static_cast<uint64_t>(-1));
}