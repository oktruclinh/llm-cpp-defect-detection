#include <gtest/gtest.h>

#include <fmt/format.h>

#include <limits>
#include <string>

namespace {

TEST(FmtDigitGroupingTest, NoGroupingWithoutLocaleSpecifier) {
  EXPECT_EQ(fmt::format("{}", 0), "0");
  EXPECT_EQ(fmt::format("{}", 7), "7");
  EXPECT_EQ(fmt::format("{}", 12), "12");
  EXPECT_EQ(fmt::format("{}", 1234), "1234");
  EXPECT_EQ(fmt::format("{}", 1234567), "1234567");
  EXPECT_EQ(fmt::format("{}", -1234567), "-1234567");
}

TEST(FmtDigitGroupingTest, NumericSeparatorsWithUnderscoreForDecimalIntegers) {
  EXPECT_EQ(fmt::format("{:_}", 0), "0");
  EXPECT_EQ(fmt::format("{:_}", 1), "1");
  EXPECT_EQ(fmt::format("{:_}", 12), "12");
  EXPECT_EQ(fmt::format("{:_}", 123), "123");
  EXPECT_EQ(fmt::format("{:_}", 1234), "1_234");
  EXPECT_EQ(fmt::format("{:_}", 12345), "12_345");
  EXPECT_EQ(fmt::format("{:_}", 123456), "123_456");
  EXPECT_EQ(fmt::format("{:_}", 1234567), "1_234_567");
  EXPECT_EQ(fmt::format("{:_}", 12345678), "12_345_678");
  EXPECT_EQ(fmt::format("{:_}", 123456789), "123_456_789");
}

TEST(FmtDigitGroupingTest, NumericSeparatorsWithUnderscoreForNegativeIntegers) {
  EXPECT_EQ(fmt::format("{:_}", -1), "-1");
  EXPECT_EQ(fmt::format("{:_}", -12), "-12");
  EXPECT_EQ(fmt::format("{:_}", -123), "-123");
  EXPECT_EQ(fmt::format("{:_}", -1234), "-1_234");
  EXPECT_EQ(fmt::format("{:_}", -1234567), "-1_234_567");
}

TEST(FmtDigitGroupingTest, DigitGroupingBoundariesAroundThousands) {
  EXPECT_EQ(fmt::format("{:_}", 999), "999");
  EXPECT_EQ(fmt::format("{:_}", 1000), "1_000");
  EXPECT_EQ(fmt::format("{:_}", 1001), "1_001");

  EXPECT_EQ(fmt::format("{:_}", 999999), "999_999");
  EXPECT_EQ(fmt::format("{:_}", 1000000), "1_000_000");
  EXPECT_EQ(fmt::format("{:_}", 1000001), "1_000_001");
}

TEST(FmtDigitGroupingTest, WidthAccountsForInsertedGroupingSeparators) {
  EXPECT_EQ(fmt::format("{:>6_}", 999), "   999");
  EXPECT_EQ(fmt::format("{:>6_}", 1000), " 1_000");
  EXPECT_EQ(fmt::format("{:>10_}", 1234567), " 1_234_567");

  EXPECT_EQ(fmt::format("{:<10_}", 1234567), "1_234_567 ");
  EXPECT_EQ(fmt::format("{:^11_}", 1234567), " 1_234_567 ");
}

TEST(FmtDigitGroupingTest, ZeroPaddingAccountsForGroupingSeparators) {
  EXPECT_EQ(fmt::format("{:08_}", 1234), "0,001,234");
  EXPECT_EQ(fmt::format("{:09_}", 1234), "0,001,234");
  EXPECT_EQ(fmt::format("{:010_}", 1234), "00,001,234");

  EXPECT_EQ(fmt::format("{:09_}", -1234), "-0,001,234");
  EXPECT_EQ(fmt::format("{:010_}", -1234), "-00,001,234");
}

TEST(FmtDigitGroupingTest, WorksForLargeSignedValues) {
  EXPECT_EQ(fmt::format("{:_}", std::numeric_limits<int>::max()),
            "2_147_483_647");
  EXPECT_EQ(fmt::format("{:_}", std::numeric_limits<int>::min()),
            "-2_147_483_648");

  EXPECT_EQ(fmt::format("{:_}", std::numeric_limits<long long>::max()),
            "9_223_372_036_854_775_807");
  EXPECT_EQ(fmt::format("{:_}", std::numeric_limits<long long>::min()),
            "-9_223_372_036_854_775_808");
}

TEST(FmtDigitGroupingTest, WorksForLargeUnsignedValues) {
  EXPECT_EQ(fmt::format("{:_}", std::numeric_limits<unsigned>::max()),
            "4_294_967_295");
  EXPECT_EQ(fmt::format("{:_}", std::numeric_limits<unsigned long long>::max()),
            "18_446_744_073_709_551_615");
}

TEST(FmtDigitGroupingTest, BinaryAndHexAlsoGroupDigitsWithUnderscore) {
  EXPECT_EQ(fmt::format("{:_b}", 0), "0");
  EXPECT_EQ(fmt::format("{:_b}", 0b1010), "1010");
  EXPECT_EQ(fmt::format("{:_b}", 0b10101010), "1010'1010");
  EXPECT_EQ(fmt::format("{:_b}", 0xFFFF), "1111'1111'1111'1111");

  EXPECT_EQ(fmt::format("{:_x}", 0xABCD), "abcd");
  EXPECT_EQ(fmt::format("{:_x}", 0xABCDEF), "ab'cdef");
  EXPECT_EQ(fmt::format("{:_X}", 0xABCDEF), "AB'CDEF");
}

TEST(FmtDigitGroupingTest, InvalidGroupingSpecifierForStringsThrows) {
  EXPECT_THROW((void)fmt::format("{:_}", std::string("abc")), fmt::format_error);
}

}  // namespace