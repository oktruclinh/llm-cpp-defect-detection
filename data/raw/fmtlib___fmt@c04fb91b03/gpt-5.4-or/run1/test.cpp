#include <gtest/gtest.h>

#include <fmt/format.h>

#include <limits>
#include <string>
#include <type_traits>

namespace {

enum class Color { red = 7, green = 42, blue = -3 };
enum PlainEnum { plain_zero = 0, plain_one = 1, plain_neg = -5 };

enum class BigUnsigned : unsigned long long {
  maxv = std::numeric_limits<unsigned long long>::max()
};

enum class SmallUnsigned : unsigned char { zero = 0, ten = 10, maxv = 255 };

TEST(FmtFormatEnumTest, FormatsScopedEnumAsUnderlyingIntegerByDefault) {
  EXPECT_EQ(fmt::format("{}", Color::red), "7");
  EXPECT_EQ(fmt::format("{}", Color::green), "42");
  EXPECT_EQ(fmt::format("{}", Color::blue), "-3");
}

TEST(FmtFormatEnumTest, FormatsUnscopedEnumAsUnderlyingIntegerByDefault) {
  EXPECT_EQ(fmt::format("{}", plain_zero), "0");
  EXPECT_EQ(fmt::format("{}", plain_one), "1");
  EXPECT_EQ(fmt::format("{}", plain_neg), "-5");
}

TEST(FmtFormatEnumTest, SupportsWidthAlignmentAndFillInheritedFromIntFormatter) {
  EXPECT_EQ(fmt::format("{:>5}", Color::red), "    7");
  EXPECT_EQ(fmt::format("{:<5}", Color::red), "7    ");
  EXPECT_EQ(fmt::format("{:^5}", Color::red), "  7  ");
  EXPECT_EQ(fmt::format("{:*^6}", Color::green), "**42**");
  EXPECT_EQ(fmt::format("{:0>4}", Color::red), "0007");
}

TEST(FmtFormatEnumTest, SupportsSignAlternateFormAndBasesInheritedFromIntFormatter) {
  EXPECT_EQ(fmt::format("{:+}", Color::red), "+7");
  EXPECT_EQ(fmt::format("{:d}", Color::blue), "-3");
  EXPECT_EQ(fmt::format("{:x}", Color::green), "2a");
  EXPECT_EQ(fmt::format("{:#x}", Color::green), "0x2a");
  EXPECT_EQ(fmt::format("{:#X}", Color::green), "0X2A");
  EXPECT_EQ(fmt::format("{:b}", Color::red), "111");
  EXPECT_EQ(fmt::format("{:#b}", Color::red), "0b111");
  EXPECT_EQ(fmt::format("{:o}", Color::green), "52");
  EXPECT_EQ(fmt::format("{:#o}", Color::green), "052");
}

TEST(FmtFormatEnumTest, SupportsDynamicWidthForEnumFormatting) {
  EXPECT_EQ(fmt::format("{:>{}}", Color::red, 4), "   7");
  EXPECT_EQ(fmt::format("{:*^{}}", Color::green, 7), "**42***");
  EXPECT_EQ(fmt::format("{:0>{}}", plain_one, 3), "001");
}

TEST(FmtFormatEnumTest, ZeroWidthAndNoPaddingBehaveCorrectly) {
  EXPECT_EQ(fmt::format("{:0}", Color::red), "7");
  EXPECT_EQ(fmt::format("{:>{}}", Color::red, 0), "7");
  EXPECT_EQ(fmt::format("{:<{}}", plain_zero, 0), "0");
}

TEST(FmtFormatEnumTest, SupportsFormattingOfUnsignedEnumExtremes) {
  EXPECT_EQ(fmt::format("{}", SmallUnsigned::zero), "0");
  EXPECT_EQ(fmt::format("{}", SmallUnsigned::ten), "10");
  EXPECT_EQ(fmt::format("{}", SmallUnsigned::maxv), "255");

  EXPECT_EQ(fmt::format("{}", BigUnsigned::maxv),
            "18446744073709551615");
  EXPECT_EQ(fmt::format("{:#x}", BigUnsigned::maxv),
            "0xffffffffffffffff");
}

TEST(FmtFormatEnumTest, PrecisionIsRejectedForIntegerLikeEnumFormatting) {
  EXPECT_THROW((void)fmt::format("{:.1}", Color::red), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:.0}", plain_zero), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:.3d}", Color::green), fmt::format_error);
}

TEST(FmtFormatEnumTest, DynamicPrecisionIsRejectedForIntegerLikeEnumFormatting) {
  EXPECT_THROW((void)fmt::format("{:.{}}", Color::red, 2), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:.{}d}", plain_one, 1), fmt::format_error);
}

TEST(FmtFormatEnumTest, InvalidTypeSpecifiersForIntegersAreRejected) {
  EXPECT_THROW((void)fmt::format("{:f}", Color::red), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:e}", plain_one), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:s}", Color::green), fmt::format_error);
}

TEST(FmtFormatEnumTest, DynamicWidthNegativeThrowsFormatError) {
  EXPECT_THROW((void)fmt::format("{:>{}}", Color::red, -1), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:*^{}}", plain_one, -5), fmt::format_error);
}

TEST(FmtFormatEnumTest, EnumFormatterIsAvailableViaPublicApi) {
  static_assert(std::is_base_of<fmt::formatter<int, char>,
                                fmt::formatter<Color, char>>::value,
                "Enum formatter should derive from int formatter");
  static_assert(std::is_base_of<fmt::formatter<int, char>,
                                fmt::formatter<PlainEnum, char>>::value,
                "Enum formatter should derive from int formatter");
  SUCCEED();
}

}  // namespace