#include <gtest/gtest.h>
#include <fmt/format.h>

#include <limits>
#include <string>

namespace {

template <typename T>
std::string fmt_oct_default(T value) {
  return fmt::format("{:o}", value);
}

template <typename T>
std::string fmt_oct_alt(T value) {
  return fmt::format("{:#o}", value);
}

template <typename T>
std::string fmt_oct_prec(T value, int precision) {
  return fmt::format("{:.{}o}", value, precision);
}

template <typename T>
std::string fmt_oct_alt_prec(T value, int precision) {
  return fmt::format("{:#.{}o}", value, precision);
}

TEST(FormatOctalOnOctTest, DefaultFormattingWithoutAlternateForm) {
  EXPECT_EQ("0", fmt_oct_default(0));
  EXPECT_EQ("7", fmt_oct_default(7));
  EXPECT_EQ("10", fmt_oct_default(8));
  EXPECT_EQ("123", fmt_oct_default(83));
}

TEST(FormatOctalOnOctTest, AlternateFormAddsLeadingZeroForNonZeroValues) {
  EXPECT_EQ("01", fmt_oct_alt(1));
  EXPECT_EQ("07", fmt_oct_alt(7));
  EXPECT_EQ("010", fmt_oct_alt(8));
  EXPECT_EQ("0123", fmt_oct_alt(83));
}

TEST(FormatOctalOnOctTest, AlternateFormDoesNotAddExtraPrefixForZeroWithoutPrecision) {
  EXPECT_EQ("0", fmt_oct_alt(0));
}

TEST(FormatOctalOnOctTest, AlternateFormWithPrecisionLessThanDigitsAddsPrefix) {
  // 8 in octal is "10" (2 digits). Since precision 0/1 <= num_digits,
  // alternate form should add a leading '0'.
  EXPECT_EQ("010", fmt_oct_alt_prec(8, 0));
  EXPECT_EQ("010", fmt_oct_alt_prec(8, 1));

  // 83 in octal is "123" (3 digits).
  EXPECT_EQ("0123", fmt_oct_alt_prec(83, 2));
  EXPECT_EQ("0123", fmt_oct_alt_prec(83, 3));
}

TEST(FormatOctalOnOctTest, AlternateFormWithPrecisionGreaterThanDigitsDoesNotAddSeparatePrefix) {
  // For octal alternate form, the leading zero is counted as a digit.
  // Therefore when precision already exceeds the digit count, no extra prefix
  // should be added beyond zero-padding to the requested precision.
  EXPECT_EQ("000", fmt_oct_alt_prec(8, 3));    // not "0010"
  EXPECT_EQ("00010", fmt_oct_alt_prec(8, 5));  // not "000010"

  EXPECT_EQ("00123", fmt_oct_alt_prec(83, 5)); // not "000123"
}

TEST(FormatOctalOnOctTest, PrecisionBoundaryEqualToDigitCountStillAddsPrefix) {
  EXPECT_EQ("010", fmt_oct_alt_prec(8, 2));     // equal to digits in "10"
  EXPECT_EQ("0123", fmt_oct_alt_prec(83, 3));   // equal to digits in "123"
}

TEST(FormatOctalOnOctTest, ZeroWithAlternateFormAndPrecisionFollowsOctalRules) {
  // Even though on_oct only adds the alternate prefix for non-zero values,
  // the overall formatting result for zero in octal alternate form should still
  // be a single leading zero, with precision controlling minimum digits.
  EXPECT_EQ("0", fmt_oct_alt_prec(0, 0));
  EXPECT_EQ("0", fmt_oct_alt_prec(0, 1));
  EXPECT_EQ("00", fmt_oct_alt_prec(0, 2));
  EXPECT_EQ("000", fmt_oct_alt_prec(0, 3));
}

TEST(FormatOctalOnOctTest, NonAlternatePrecisionFormattingForComparison) {
  EXPECT_EQ("10", fmt_oct_prec(8, 0));
  EXPECT_EQ("10", fmt_oct_prec(8, 1));
  EXPECT_EQ("10", fmt_oct_prec(8, 2));
  EXPECT_EQ("010", fmt_oct_prec(8, 3));
  EXPECT_EQ("00123", fmt_oct_prec(83, 5));
}

TEST(FormatOctalOnOctTest, WidthAndAlternateFormInteractCorrectlyWithPrefixDecision) {
  // Prefix included when precision <= digits; width applies to whole field.
  EXPECT_EQ("  010", fmt::format("{:#5.2o}", 8));
  EXPECT_EQ("   000", fmt::format("{:#6.3o}", 8)); // no separate prefix, just precision zeros

  // Zero fill with alternate form should preserve octal content correctly.
  EXPECT_EQ("00010", fmt::format("{:#05o}", 8));
}

TEST(FormatOctalOnOctTest, LargeUnsignedValuesFormatCorrectlyInAlternateOctal) {
  using U = unsigned int;
  U value = std::numeric_limits<U>::max();
  std::string plain = fmt::format("{:o}", value);
  std::string alt = fmt::format("{:#o}", value);

  ASSERT_FALSE(plain.empty());
  EXPECT_EQ('0', alt.front());
  EXPECT_EQ(plain, alt.substr(1));

  // Precision equal to current digits should still include alternate prefix.
  EXPECT_EQ("0" + plain, fmt::format("{:#.{}o}", value, static_cast<int>(plain.size())));

  // Precision greater than digits should not add an extra separate prefix.
  EXPECT_EQ(std::string(2, '0') + plain, fmt::format("{:#.{}o}", value, static_cast<int>(plain.size() + 2)));
}

TEST(FormatOctalOnOctTest, SignedValuesUseAbsoluteMagnitudeWithSignAndOctalPrefix) {
  EXPECT_EQ("-01", fmt::format("{:#o}", -1));
  EXPECT_EQ("-010", fmt::format("{:#o}", -8));
  EXPECT_EQ("-0123", fmt::format("{:#o}", -83));

  // Precision > digits should suppress separate octal prefix, but keep sign.
  EXPECT_EQ("-00010", fmt::format("{:#.5o}", -8));
  EXPECT_EQ("-00123", fmt::format("{:#.5o}", -83));

  // Precision == digits should include the octal prefix in addition to sign.
  EXPECT_EQ("-010", fmt::format("{:#.2o}", -8));
  EXPECT_EQ("-0123", fmt::format("{:#.3o}", -83));
}

TEST(FormatOctalOnOctTest, AlternateFormForZeroAcrossIntegerTypes) {
  EXPECT_EQ("0", fmt::format("{:#o}", static_cast<unsigned>(0)));
  EXPECT_EQ("0", fmt::format("{:#o}", static_cast<unsigned long>(0)));
  EXPECT_EQ("0", fmt::format("{:#o}", static_cast<unsigned long long>(0)));
  EXPECT_EQ("0", fmt::format("{:#o}", static_cast<int>(0)));
  EXPECT_EQ("0", fmt::format("{:#o}", static_cast<long>(0)));
  EXPECT_EQ("0", fmt::format("{:#o}", static_cast<long long>(0)));
}

}  // namespace