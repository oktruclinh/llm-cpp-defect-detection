#include <gtest/gtest.h>

#include <fmt/format.h>

#include <limits>
#include <string>

namespace {

enum UnscopedColor { red = 7, green = 42, blue = -3 };

enum class ScopedCode : int {
  zero = 0,
  positive = 123,
  negative = -9,
  maxv = std::numeric_limits<int>::max(),
  minv = std::numeric_limits<int>::min()
};

enum class SmallUnsigned : unsigned char {
  zero = 0,
  one = 1,
  maxv = 255
};

TEST(FormatEnumTest, FormatsUnscopedEnumAsUnderlyingIntegerByDefault) {
  EXPECT_EQ("7", fmt::format("{}", red));
  EXPECT_EQ("42", fmt::format("{}", green));
  EXPECT_EQ("-3", fmt::format("{}", blue));
}

TEST(FormatEnumTest, FormatsScopedEnumAsUnderlyingIntegerByDefault) {
  EXPECT_EQ("0", fmt::format("{}", ScopedCode::zero));
  EXPECT_EQ("123", fmt::format("{}", ScopedCode::positive));
  EXPECT_EQ("-9", fmt::format("{}", ScopedCode::negative));
}

TEST(FormatEnumTest, SupportsIntegerPresentationTypesForEnums) {
  EXPECT_EQ("7", fmt::format("{:d}", red));
  EXPECT_EQ("2a", fmt::format("{:x}", green));
  EXPECT_EQ("2A", fmt::format("{:X}", green));
  EXPECT_EQ("52", fmt::format("{:o}", green));
  EXPECT_EQ("101010", fmt::format("{:b}", green));
}

TEST(FormatEnumTest, SupportsWidthAlignmentAndFillThroughInheritedIntFormatter) {
  EXPECT_EQ("   42", fmt::format("{:>5}", green));
  EXPECT_EQ("42   ", fmt::format("{:<5}", green));
  EXPECT_EQ(" 42  ", fmt::format("{:^5}", green));
  EXPECT_EQ("00042", fmt::format("{:0>5}", green));
  EXPECT_EQ("***42", fmt::format("{:*>5}", green));
}

TEST(FormatEnumTest, SupportsSignAndNumericFormattingFlags) {
  EXPECT_EQ("+42", fmt::format("{:+}", green));
  EXPECT_EQ(" 42", fmt::format("{: }", green));
  EXPECT_EQ("-3", fmt::format("{:+}", blue));
  EXPECT_EQ("-0003", fmt::format("{:05}", blue));
}

TEST(FormatEnumTest, SupportsDynamicWidthForEnums) {
  EXPECT_EQ("   42", fmt::format("{:>{}}", green, 5));
  EXPECT_EQ("42   ", fmt::format("{:<{}}", green, 5));
  EXPECT_EQ("00042", fmt::format("{:0>{}}", green, 5));
}

TEST(FormatEnumTest, PrecisionIsRejectedForIntegerLikeEnumFormatting) {
  EXPECT_THROW((void)fmt::format("{:.1}", green), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:.0}", ScopedCode::zero), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:.3d}", red), fmt::format_error);
}

TEST(FormatEnumTest, DynamicPrecisionIsRejectedForEnums) {
  EXPECT_THROW((void)fmt::format("{:.{}}", green, 1), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:.{}d}", red, 2), fmt::format_error);
}

TEST(FormatEnumTest, InvalidDynamicWidthValuesPropagateAsErrorsForEnums) {
  EXPECT_THROW((void)fmt::format("{:>{}}", green, -1), fmt::format_error);
}

TEST(FormatEnumTest, HandlesIntegerBoundaryValuesForScopedEnum) {
  EXPECT_EQ(std::to_string(std::numeric_limits<int>::max()),
            fmt::format("{}", ScopedCode::maxv));
  EXPECT_EQ(std::to_string(std::numeric_limits<int>::min()),
            fmt::format("{}", ScopedCode::minv));
}

TEST(FormatEnumTest, FormatsSmallUnsignedEnumUsingUnderlyingNumericValue) {
  EXPECT_EQ("0", fmt::format("{}", SmallUnsigned::zero));
  EXPECT_EQ("1", fmt::format("{}", SmallUnsigned::one));
  EXPECT_EQ("255", fmt::format("{}", SmallUnsigned::maxv));
  EXPECT_EQ("ff", fmt::format("{:x}", SmallUnsigned::maxv));
}

TEST(FormatEnumTest, SupportsManualArgumentIndexingWithEnums) {
  EXPECT_EQ("42 7 42", fmt::format("{1} {0} {1}", red, green));
}

TEST(FormatEnumTest, SupportsFormattingEnumInsideCompositeText) {
  EXPECT_EQ("value=42;", fmt::format("value={};", green));
  EXPECT_EQ("[  7]", fmt::format("[{:>3}]", red));
}

}  // namespace