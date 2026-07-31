#include <gtest/gtest.h>

#include <fmt/format.h>
#include <limits>
#include <string>

namespace {

TEST(FmtFormatRightAlignedNumericOutputTest, NegativeIntegerGetsMinusBeforePaddingZeros) {
  EXPECT_EQ(fmt::format("{:05}", -42), "-0042");
  EXPECT_EQ(fmt::format("{:06}", -1), "-00001");
  EXPECT_EQ(fmt::format("{:4}", -42), " -42");
}

TEST(FmtFormatRightAlignedNumericOutputTest, NegativeIntegerWithDynamicWidthPlacesMinusCorrectly) {
  EXPECT_EQ(fmt::format("{:0>{}}", -42, 5), "00-42");
  EXPECT_EQ(fmt::format("{:>{}}", -42, 5), "  -42");
  EXPECT_EQ(fmt::format("{:>{}}", -42, 3), "-42");
}

TEST(FmtFormatRightAlignedNumericOutputTest, NegativeFloatingPointWithZeroPaddingPlacesSignBeforeZeros) {
  EXPECT_EQ(fmt::format("{:08.2f}", -12.34), "-0012.34");
  EXPECT_EQ(fmt::format("{:07.1f}", -0.5), "-0000.5");
}

TEST(FmtFormatRightAlignedNumericOutputTest, PositiveValuesAreRightAlignedWithoutUnexpectedSign) {
  EXPECT_EQ(fmt::format("{:05}", 42), "00042");
  EXPECT_EQ(fmt::format("{:>5}", 42), "   42");
  EXPECT_EQ(fmt::format("{:08.2f}", 12.34), "00012.34");
}

TEST(FmtFormatRightAlignedNumericOutputTest, WidthEqualToContentSizeProducesNoExtraPadding) {
  EXPECT_EQ(fmt::format("{:3}", -42), "-42");
  EXPECT_EQ(fmt::format("{:4}", -999), "-999");
  EXPECT_EQ(fmt::format("{:6.2f}", -1.23), " -1.23");
}

TEST(FmtFormatRightAlignedNumericOutputTest, WidthSmallerThanContentSizeDoesNotTruncate) {
  EXPECT_EQ(fmt::format("{:2}", -42), "-42");
  EXPECT_EQ(fmt::format("{:1}", -12345), "-12345");
  EXPECT_EQ(fmt::format("{:3.2f}", -12.34), "-12.34");
}

TEST(FmtFormatRightAlignedNumericOutputTest, ZeroAndNegativeZeroFormattingAreCorrectlyPadded) {
  EXPECT_EQ(fmt::format("{:03}", 0), "000");
  EXPECT_EQ(fmt::format("{:04}", -0.0), "-0.0");
  EXPECT_EQ(fmt::format("{:08.1f}", -0.0), "-00000.0");
}

TEST(FmtFormatRightAlignedNumericOutputTest, LargeNegativeBoundaryIntegersFormatCorrectly) {
  EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::min()),
            std::to_string(std::numeric_limits<int>::min()));
  EXPECT_EQ(fmt::format("{:0>12}", std::numeric_limits<int>::min()),
            "0-2147483648");

  EXPECT_EQ(fmt::format("{}", std::numeric_limits<long long>::min()),
            std::to_string(std::numeric_limits<long long>::min()));
}

TEST(FmtFormatRightAlignedNumericOutputTest, EmptyWidthPaddingScenarioKeepsOriginalTextForNumbers) {
  EXPECT_EQ(fmt::format("{}", -42), "-42");
  EXPECT_EQ(fmt::format("{}", 0), "0");
  EXPECT_EQ(fmt::format("{}", -1.5), "-1.5");
}

TEST(FmtFormatRightAlignedNumericOutputTest, AlternateFormAndSignStillPreserveCorrectRightAlignedOutput) {
  EXPECT_EQ(fmt::format("{:#06x}", 0x2a), "0x002a");
  EXPECT_EQ(fmt::format("{:+06}", 42), "+00042");
  EXPECT_EQ(fmt::format("{:+06}", -42), "-00042");
}

TEST(FmtFormatErrorPathTest, NegativeDynamicWidthThrowsFormatError) {
  EXPECT_THROW(fmt::format("{:>{}}", 42, -1), fmt::format_error);
  EXPECT_THROW(fmt::format("{:0>{}}", -42, -5), fmt::format_error);
}

TEST(FmtFormatErrorPathTest, InvalidFormatSpecifiersThrowFormatError) {
  EXPECT_THROW(fmt::format("{:.}", 1.23), fmt::format_error);
  EXPECT_THROW(fmt::format("{:z}", 42), fmt::format_error);
}

}  // namespace