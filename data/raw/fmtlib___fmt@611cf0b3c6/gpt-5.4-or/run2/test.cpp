#include <gtest/gtest.h>
#include <fmt/format.h>

#include <limits>
#include <string>

namespace {

TEST(FmtFormatOctAlternateTest, AlternateFormAddsLeadingZeroWhenNoPrecisionAndNonZero) {
  EXPECT_EQ(fmt::format("{:#o}", 1), "01");
  EXPECT_EQ(fmt::format("{:#o}", 8), "010");
  EXPECT_EQ(fmt::format("{:#o}", 64), "0100");
}

TEST(FmtFormatOctAlternateTest, AlternateFormDoesNotAddExtraZeroForZeroValue) {
  EXPECT_EQ(fmt::format("{:#o}", 0), "0");
  EXPECT_EQ(fmt::format("{:#5o}", 0), "    0");
  EXPECT_EQ(fmt::format("{:#05o}", 0), "00000");
}

TEST(FmtFormatOctAlternateTest, PrecisionGreaterThanDigitsSuppressesAlternatePrefix) {
  EXPECT_EQ(fmt::format("{:#.2o}", 1), "01");
  EXPECT_EQ(fmt::format("{:#.3o}", 1), "001");
  EXPECT_EQ(fmt::format("{:#.4o}", 7), "0007");
  EXPECT_EQ(fmt::format("{:#.5o}", 8), "00010");
}

TEST(FmtFormatOctAlternateTest, PrecisionEqualToDigitsCountsPrefixAsDigitAndAddsLeadingZero) {
  EXPECT_EQ(fmt::format("{:#.1o}", 1), "01");
  EXPECT_EQ(fmt::format("{:#.2o}", 8), "010");
  EXPECT_EQ(fmt::format("{:#.3o}", 64), "0100");
}

TEST(FmtFormatOctAlternateTest, PrecisionLessThanDigitsAddsAlternatePrefix) {
  EXPECT_EQ(fmt::format("{:#.1o}", 8), "010");
  EXPECT_EQ(fmt::format("{:#.2o}", 64), "0100");
  EXPECT_EQ(fmt::format("{:#.3o}", 512), "01000");
}

TEST(FmtFormatOctAlternateTest, WithoutAlternateFormPrecisionBehavesNormally) {
  EXPECT_EQ(fmt::format("{:.3o}", 1), "001");
  EXPECT_EQ(fmt::format("{:.3o}", 8), "010");
  EXPECT_EQ(fmt::format("{:.1o}", 8), "10");
}

TEST(FmtFormatOctAlternateTest, WidthAndAlternateFormWithPrecisionInteractCorrectly) {
  EXPECT_EQ(fmt::format("{:#5.3o}", 1), "  001");
  EXPECT_EQ(fmt::format("{:#5.2o}", 1), "  01");
  EXPECT_EQ(fmt::format("{:#5.1o}", 1), "   01");
  EXPECT_EQ(fmt::format("{:#6.3o}", 8), "  0010");
}

TEST(FmtFormatOctAlternateTest, ZeroPaddingUsesPrefixBeforeZeros) {
  EXPECT_EQ(fmt::format("{:#05o}", 1), "00001");
  EXPECT_EQ(fmt::format("{:#06o}", 8), "000010");
  EXPECT_EQ(fmt::format("{:#06o}", 64), "000100");
}

TEST(FmtFormatOctAlternateTest, SignedValuesUseSignAndOctalPrefixCorrectly) {
  EXPECT_EQ(fmt::format("{:#o}", -1), "-01");
  EXPECT_EQ(fmt::format("{:#.3o}", -1), "-001");
  EXPECT_EQ(fmt::format("{:#05o}", -1), "-0001");
  EXPECT_EQ(fmt::format("{:#o}", -8), "-010");
}

TEST(FmtFormatOctAlternateTest, UnsignedMaximumStillHasSingleAlternateLeadingZero) {
  const auto value = std::numeric_limits<unsigned int>::max();
  const std::string plain = fmt::format("{:o}", value);
  const std::string alt = fmt::format("{:#o}", value);

  ASSERT_FALSE(plain.empty());
  EXPECT_EQ(alt.size(), plain.size() + 1);
  EXPECT_EQ(alt[0], '0');
  EXPECT_EQ(alt.substr(1), plain);
}

TEST(FmtFormatOctAlternateTest, PrecisionZeroWithAlternateFormNonZeroStillAddsPrefix) {
  EXPECT_EQ(fmt::format("{:#.0o}", 1), "01");
  EXPECT_EQ(fmt::format("{:#.0o}", 8), "010");
}

TEST(FmtFormatOctAlternateTest, PrecisionZeroWithAlternateFormZeroDoesNotAddExtraPrefix) {
  EXPECT_EQ(fmt::format("{:#.0o}", 0), "0");
}

TEST(FmtFormatOctAlternateTest, DynamicPrecisionExercisesSameBoundaryRules) {
  EXPECT_EQ(fmt::format("{:#.{}o}", 1, 1), "01");
  EXPECT_EQ(fmt::format("{:#.{}o}", 1, 2), "01");
  EXPECT_EQ(fmt::format("{:#.{}o}", 1, 3), "001");

  EXPECT_EQ(fmt::format("{:#.{}o}", 8, 1), "010");
  EXPECT_EQ(fmt::format("{:#.{}o}", 8, 2), "010");
  EXPECT_EQ(fmt::format("{:#.{}o}", 8, 3), "0010");
}

TEST(FmtFormatOctAlternateTest, DynamicWidthAndPrecisionWithAlternateForm) {
  EXPECT_EQ(fmt::format("{:#{}.{ }o}", 1, 5, 1), "   01");
  EXPECT_EQ(fmt::format("{:#{}.{ }o}", 1, 5, 3), "  001");
}

TEST(FmtFormatOctAlternateTest, InvalidPrecisionTypeThrowsFormatError) {
  EXPECT_THROW(fmt::format("{:#.{}o}", 8, "x"), fmt::format_error);
}

}  // namespace