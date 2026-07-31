#include <gtest/gtest.h>
#include <fmt/format.h>

#include <limits>
#include <string>

namespace {

template <typename T>
std::string format_o(T value) {
  return fmt::format("{:o}", value);
}

template <typename T>
std::string format_hash_o(T value) {
  return fmt::format("{:#o}", value);
}

template <typename T>
std::string format_prec_hash_o(T value, int precision) {
  return fmt::format("{:#{}.{}o}", value, "", precision); // unused helper form not used
}

TEST(FmtFormatOnOctTest, BasicOctalFormattingWithoutAlternateForm) {
  EXPECT_EQ(fmt::format("{:o}", 0), "0");
  EXPECT_EQ(fmt::format("{:o}", 1), "1");
  EXPECT_EQ(fmt::format("{:o}", 8), "10");
  EXPECT_EQ(fmt::format("{:o}", 64), "100");
  EXPECT_EQ(fmt::format("{:o}", 511), "777");
}

TEST(FmtFormatOnOctTest, AlternateFormAddsLeadingZeroForNonZeroWithoutPrecision) {
  EXPECT_EQ(fmt::format("{:#o}", 1), "01");
  EXPECT_EQ(fmt::format("{:#o}", 8), "010");
  EXPECT_EQ(fmt::format("{:#o}", 64), "0100");
  EXPECT_EQ(fmt::format("{:#o}", 511), "0777");
}

TEST(FmtFormatOnOctTest, AlternateFormDoesNotAddExtraPrefixForZeroWithoutPrecision) {
  EXPECT_EQ(fmt::format("{:#o}", 0), "0");
}

TEST(FmtFormatOnOctTest, AlternateFormWithPrecisionLessThanDigitsAddsPrefix) {
  // 8 -> "10" (2 octal digits). With precision 1, precision <= digits, so prefix is added.
  EXPECT_EQ(fmt::format("{:#.1o}", 8), "010");

  // 64 -> "100" (3 octal digits). With precision 2, precision <= digits, so prefix is added.
  EXPECT_EQ(fmt::format("{:#.2o}", 64), "0100");

  // 511 -> "777" (3 octal digits). With precision 3, precision <= digits, so prefix is added.
  EXPECT_EQ(fmt::format("{:#.3o}", 511), "0777");
}

TEST(FmtFormatOnOctTest, AlternateFormWithPrecisionGreaterThanDigitsDoesNotAddExtraPrefix) {
  // For octal, the alternate form prefix '0' is counted as a digit.
  // Therefore when precision exceeds the number of digits, no extra prefix should be added.
  EXPECT_EQ(fmt::format("{:#.2o}", 1), "01");
  EXPECT_EQ(fmt::format("{:#.3o}", 1), "001");
  EXPECT_EQ(fmt::format("{:#.4o}", 1), "0001");

  EXPECT_EQ(fmt::format("{:#.3o}", 8), "010");
  EXPECT_EQ(fmt::format("{:#.4o}", 8), "0010");

  EXPECT_EQ(fmt::format("{:#.4o}", 64), "0100");
  EXPECT_EQ(fmt::format("{:#.5o}", 64), "00100");
}

TEST(FmtFormatOnOctTest, AlternateFormPrecisionBoundaryAtExactDigitCount) {
  // Exact boundary: precision == digit count should add prefix for nonzero values.
  EXPECT_EQ(fmt::format("{:#.1o}", 1), "01");
  EXPECT_EQ(fmt::format("{:#.2o}", 8), "010");
  EXPECT_EQ(fmt::format("{:#.3o}", 64), "0100");
  EXPECT_EQ(fmt::format("{:#.4o}", 512), "01000");
}

TEST(FmtFormatOnOctTest, ZeroWithPrecisionAndAlternateFormDoesNotProduceExtraPrefix) {
  // Zero should not receive an alternate-form prefix via on_oct's condition.
  // Precision controls zero-padding/representation, but should not create a distinct extra prefix.
  EXPECT_EQ(fmt::format("{:#.0o}", 0), "0");
  EXPECT_EQ(fmt::format("{:#.1o}", 0), "0");
  EXPECT_EQ(fmt::format("{:#.3o}", 0), "000");
  EXPECT_EQ(fmt::format("{:#5.3o}", 0), "  000");
}

TEST(FmtFormatOnOctTest, WidthInteractsCorrectlyWithAlternateFormAndPrecision) {
  EXPECT_EQ(fmt::format("{:#5o}", 8), "  010");
  EXPECT_EQ(fmt::format("{:#5.2o}", 8), "  010");
  EXPECT_EQ(fmt::format("{:#5.3o}", 8), "  010");
  EXPECT_EQ(fmt::format("{:#6.4o}", 8), "  0010");

  EXPECT_EQ(fmt::format("{:#6o}", 64), "  0100");
  EXPECT_EQ(fmt::format("{:#6.3o}", 64), "  0100");
  EXPECT_EQ(fmt::format("{:#6.5o}", 64), " 00100");
}

TEST(FmtFormatOnOctTest, DynamicPrecisionBehavesLikeStaticPrecision) {
  EXPECT_EQ(fmt::format("{:#.*o}", 1, 1), "01");
  EXPECT_EQ(fmt::format("{:#.*o}", 2, 1), "01");
  EXPECT_EQ(fmt::format("{:#.*o}", 3, 1), "001");
  EXPECT_EQ(fmt::format("{:#.*o}", 2, 8), "010");
  EXPECT_EQ(fmt::format("{:#.*o}", 3, 8), "010");
  EXPECT_EQ(fmt::format("{:#.*o}", 4, 8), "0010");
}

TEST(FmtFormatOnOctTest, LargeUnsignedValueUsesCorrectOctalPrefixRules) {
  using U = unsigned int;
  U value = std::numeric_limits<U>::max();

  std::string plain = fmt::format("{:o}", value);
  std::string alt = fmt::format("{:#o}", value);

  ASSERT_FALSE(plain.empty());
  EXPECT_EQ(alt.size(), plain.size() + 1);
  EXPECT_EQ(alt[0], '0');
  EXPECT_EQ(alt.substr(1), plain);

  EXPECT_EQ(fmt::format("{:#.100o}", value).size(), 100u);
  EXPECT_EQ(fmt::format("{:#.100o}", value)[0], '0');
}

TEST(FmtFormatOnOctTest, SignedValuesUseSameOctalPrefixRulesForMagnitude) {
  EXPECT_EQ(fmt::format("{:#o}", 9), "011");
  EXPECT_EQ(fmt::format("{:#.2o}", 9), "011");
  EXPECT_EQ(fmt::format("{:#.3o}", 9), "011");
  EXPECT_EQ(fmt::format("{:#.4o}", 9), "0011");
}

TEST(FmtFormatOnOctTest, ZeroPaddingFlagWithAlternateFormDoesNotBreakOctalPrefixLogic) {
  EXPECT_EQ(fmt::format("{:#04o}", 1), "0001");
  EXPECT_EQ(fmt::format("{:#05o}", 8), "00010");
  EXPECT_EQ(fmt::format("{:#06o}", 64), "000100");
}

}  // namespace