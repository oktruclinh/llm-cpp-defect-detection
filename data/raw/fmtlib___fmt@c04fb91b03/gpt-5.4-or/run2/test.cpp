#include <gtest/gtest.h>

#include <fmt/format.h>

#include <cstdint>
#include <limits>
#include <string>

namespace {

enum Color {
  Red = 0,
  Green = 7,
  Blue = 42,
  Negative = -5
};

enum class ScopedStatus : int {
  Idle = 0,
  Busy = 3,
  Error = -9
};

enum LargeUnsigned : unsigned long long {
  Huge = 18446744073709551615ull
};

enum class TinySigned : signed char {
  Min = static_cast<signed char>(-128),
  Max = static_cast<signed char>(127)
};

TEST(FormatEnumTest, FormatsUnscopedEnumAsUnderlyingInteger) {
  EXPECT_EQ("0", fmt::format("{}", Red));
  EXPECT_EQ("7", fmt::format("{}", Green));
  EXPECT_EQ("42", fmt::format("{}", Blue));
  EXPECT_EQ("-5", fmt::format("{}", Negative));
}

TEST(FormatEnumTest, FormatsScopedEnumAsUnderlyingInteger) {
  EXPECT_EQ("0", fmt::format("{}", ScopedStatus::Idle));
  EXPECT_EQ("3", fmt::format("{}", ScopedStatus::Busy));
  EXPECT_EQ("-9", fmt::format("{}", ScopedStatus::Error));
}

TEST(FormatEnumTest, SupportsIntegerPresentationTypes) {
  EXPECT_EQ("2a", fmt::format("{:x}", Blue));
  EXPECT_EQ("0x2a", fmt::format("{:#x}", Blue));
  EXPECT_EQ("052", fmt::format("{:o}", Blue));
  EXPECT_EQ("101010", fmt::format("{:b}", Blue));
}

TEST(FormatEnumTest, SupportsWidthAlignmentAndFill) {
  EXPECT_EQ("   42", fmt::format("{:>5}", Blue));
  EXPECT_EQ("42   ", fmt::format("{:<5}", Blue));
  EXPECT_EQ("**42*", fmt::format("{:*^5}", Blue));
  EXPECT_EQ("-0005", fmt::format("{:0>5}", Negative));
}

TEST(FormatEnumTest, SupportsSignFlags) {
  EXPECT_EQ("+7", fmt::format("{:+}", Green));
  EXPECT_EQ(" 7", fmt::format("{: }", Green));
  EXPECT_EQ("-5", fmt::format("{:+}", Negative));
}

TEST(FormatEnumTest, SupportsDynamicWidthForEnumFormatting) {
  EXPECT_EQ("   42", fmt::format("{:>{}}", Blue, 5));
  EXPECT_EQ("42   ", fmt::format("{:<{}}", Blue, 5));
  EXPECT_EQ("0042", fmt::format("{:0>{}}", Blue, 4));
}

TEST(FormatEnumTest, SupportsDynamicPrecisionWhereAcceptedByIntegerFormatterAsErrorPath) {
  EXPECT_THROW(fmt::format("{:.{}}", Blue, 3), fmt::format_error);
  EXPECT_THROW(fmt::format("{:{}.{}d}", Blue, 4, 2), fmt::format_error);
}

TEST(FormatEnumTest, RejectsPrecisionForIntegerLikeEnumFormatting) {
  EXPECT_THROW(fmt::format("{:.3}", Blue), fmt::format_error);
  EXPECT_THROW(fmt::format("{:.0d}", Green), fmt::format_error);
}

TEST(FormatEnumTest, RejectsInvalidTypeSpecifiersForEnumFormatter) {
  EXPECT_THROW(fmt::format("{:f}", Blue), fmt::format_error);
  EXPECT_THROW(fmt::format("{:s}", Blue), fmt::format_error);
}

TEST(FormatEnumTest, LargeUnsignedEnumFormatsWithoutSignLoss) {
  EXPECT_EQ("18446744073709551615", fmt::format("{}", Huge));
  EXPECT_EQ("ffffffffffffffff", fmt::format("{:x}", Huge));
}

TEST(FormatEnumTest, SmallSignedUnderlyingEnumPreservesValueRange) {
  EXPECT_EQ("-128", fmt::format("{}", TinySigned::Min));
  EXPECT_EQ("127", fmt::format("{}", TinySigned::Max));
}

TEST(FormatEnumTest, CanBeUsedMultipleTimesInSingleFormatString) {
  EXPECT_EQ("7 42 -5", fmt::format("{} {} {}", Green, Blue, Negative));
}

TEST(FormatEnumTest, WorksWithNamedArgumentsAndEnums) {
  EXPECT_EQ("value=42", fmt::format("value={v}", fmt::arg("v", Blue)));
  EXPECT_EQ("status=-9", fmt::format("status={s:+}", fmt::arg("s", ScopedStatus::Error)));
}

TEST(FormatEnumTest, AlternateFormAndZeroValueBehaveLikeIntegers) {
  EXPECT_EQ("0", fmt::format("{:#x}", Red));
  EXPECT_EQ("0", fmt::format("{:#o}", Red));
  EXPECT_EQ("0", fmt::format("{:#b}", Red));
}

}  // namespace