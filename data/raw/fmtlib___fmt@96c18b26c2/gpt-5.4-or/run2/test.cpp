#include <gtest/gtest.h>

#include <fmt/printf.h>

#include <limits>
#include <string>

namespace {

TEST(PrintfCharFormattingTest, CharUsesCharacterFormattingNotIntegerByDefault) {
  EXPECT_EQ(fmt::sprintf("%c", 'A'), "A");
  EXPECT_EQ(fmt::sprintf("%c", 65), "A");
  EXPECT_EQ(fmt::sprintf("%c", 0), std::string(1, '\0'));
}

TEST(PrintfCharFormattingTest, SignFlagsAreIgnoredForCharConversion) {
  EXPECT_EQ(fmt::sprintf("%+c", 'A'), "A");
  EXPECT_EQ(fmt::sprintf("% c", 'A'), "A");
  EXPECT_EQ(fmt::sprintf("%-+3c", 'A'), "A  ");
}

TEST(PrintfCharFormattingTest, AlternateFormIsIgnoredForCharConversion) {
  EXPECT_EQ(fmt::sprintf("%#c", 'A'), "A");
  EXPECT_EQ(fmt::sprintf("%#5c", 'A'), "    A");
  EXPECT_EQ(fmt::sprintf("%-#5c", 'A'), "A    ");
}

TEST(PrintfCharFormattingTest, ZeroFlagDoesNotTriggerNumericAlignmentForCharConversion) {
  EXPECT_EQ(fmt::sprintf("%05c", 'A'), "    A");
  EXPECT_EQ(fmt::sprintf("%0+5c", 'A'), "    A");
  EXPECT_EQ(fmt::sprintf("%0#5c", 'A'), "    A");
}

TEST(PrintfCharFormattingTest, WidthAndAlignmentBehaveAsNonNumericFormatting) {
  EXPECT_EQ(fmt::sprintf("%5c", 'A'), "    A");
  EXPECT_EQ(fmt::sprintf("%-5c", 'A'), "A    ");
  EXPECT_EQ(fmt::sprintf("%1c", 'A'), "A");
  EXPECT_EQ(fmt::sprintf("%0c", 'A'), "A");
}

TEST(PrintfCharFormattingTest, PrecisionDoesNotAffectCharConversion) {
  EXPECT_EQ(fmt::sprintf("%.0c", 'A'), "A");
  EXPECT_EQ(fmt::sprintf("%.3c", 'A'), "A");
  EXPECT_EQ(fmt::sprintf("%5.3c", 'A'), "    A");
  EXPECT_EQ(fmt::sprintf("%-5.3c", 'A'), "A    ");
}

TEST(PrintfCharFormattingTest, NullCharacterRespectsWidthAndAlignment) {
  EXPECT_EQ(fmt::sprintf("%3c", '\0'), std::string("  ", 2) + std::string(1, '\0'));
  EXPECT_EQ(fmt::sprintf("%-3c", '\0'), std::string(1, '\0') + "  ");
  EXPECT_EQ(fmt::sprintf("%03c", '\0'), std::string("  ", 2) + std::string(1, '\0'));
}

TEST(PrintfCharFormattingTest, LargeIntegerArgumentsToPercentCAreConvertedToIntThenChar) {
  EXPECT_EQ(fmt::sprintf("%c", static_cast<unsigned int>('Z')), "Z");
  EXPECT_EQ(fmt::sprintf("%5c", static_cast<unsigned short>('B')), "    B");
}

TEST(PrintfCharFormattingTest, ZeroPaddingStillIgnoredWhenCombinedWithLeftAlignment) {
  EXPECT_EQ(fmt::sprintf("%-05c", 'A'), "A    ");
}

TEST(PrintfCharFormattingTest, WideRangeOfCharValuesFormatAsSingleCharacter) {
  EXPECT_EQ(fmt::sprintf("%c", static_cast<unsigned char>(255)), std::string(1, static_cast<char>(255)));
  EXPECT_EQ(fmt::sprintf("%c", static_cast<signed char>(-1)), std::string(1, static_cast<char>(-1)));
}

}  // namespace