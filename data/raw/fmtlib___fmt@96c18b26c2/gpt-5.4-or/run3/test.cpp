#include <gtest/gtest.h>

#include <fmt/printf.h>

#include <string>

namespace {

TEST(PrintfCharFormattingTest, CharWithoutFlagsFormatsAsCharacter) {
  EXPECT_EQ(fmt::sprintf("%c", 'A'), "A");
  EXPECT_EQ(fmt::sprintf("%c", 65), "A");
}

TEST(PrintfCharFormattingTest, SignFlagsAreIgnoredForCharConversion) {
  EXPECT_EQ(fmt::sprintf("%+c", 'A'), "A");
  EXPECT_EQ(fmt::sprintf("% c", 'A'), "A");
  EXPECT_EQ(fmt::sprintf("%-+5c", 'A'), "A    ");
  EXPECT_EQ(fmt::sprintf("% +5c", 'A'), "    A");
}

TEST(PrintfCharFormattingTest, AlternateFormIsIgnoredForCharConversion) {
  EXPECT_EQ(fmt::sprintf("%#c", 'A'), "A");
  EXPECT_EQ(fmt::sprintf("%#5c", 'A'), "    A");
  EXPECT_EQ(fmt::sprintf("%-#5c", 'A'), "A    ");
}

TEST(PrintfCharFormattingTest, ZeroFlagDoesNotTriggerNumericAlignmentForChar) {
  EXPECT_EQ(fmt::sprintf("%05c", 'A'), "    A");
  EXPECT_EQ(fmt::sprintf("%0+5c", 'A'), "    A");
  EXPECT_EQ(fmt::sprintf("%0#5c", 'A'), "    A");
  EXPECT_EQ(fmt::sprintf("%-05c", 'A'), "A    ");
}

TEST(PrintfCharFormattingTest, WidthAndAlignmentBehaveLikeNonNumericType) {
  EXPECT_EQ(fmt::sprintf("%5c", 'A'), "    A");
  EXPECT_EQ(fmt::sprintf("%-5c", 'A'), "A    ");
  EXPECT_EQ(fmt::sprintf("%1c", 'A'), "A");
  EXPECT_EQ(fmt::sprintf("%0c", 'A'), "A");
}

TEST(PrintfCharFormattingTest, NullCharacterIsFormattedAndPaddedCorrectly) {
  std::string s = fmt::sprintf("%3c", '\0');
  ASSERT_EQ(s.size(), 3u);
  EXPECT_EQ(s[0], ' ');
  EXPECT_EQ(s[1], ' ');
  EXPECT_EQ(s[2], '\0');

  std::string left = fmt::sprintf("%-3c", '\0');
  ASSERT_EQ(left.size(), 3u);
  EXPECT_EQ(left[0], '\0');
  EXPECT_EQ(left[1], ' ');
  EXPECT_EQ(left[2], ' ');

  std::string zero_flag = fmt::sprintf("%03c", '\0');
  ASSERT_EQ(zero_flag.size(), 3u);
  EXPECT_EQ(zero_flag[0], ' ');
  EXPECT_EQ(zero_flag[1], ' ');
  EXPECT_EQ(zero_flag[2], '\0');
}

TEST(PrintfCharFormattingTest, UnsignedCharUsesCharacterSemanticsNotIntegerSemantics) {
  unsigned char ch = static_cast<unsigned char>('A');
  EXPECT_EQ(fmt::sprintf("%c", ch), "A");
  EXPECT_EQ(fmt::sprintf("%05c", ch), "    A");
  EXPECT_EQ(fmt::sprintf("%+c", ch), "A");
}

TEST(PrintfCharFormattingTest, SignedCharUsesCharacterSemanticsNotIntegerSemantics) {
  signed char ch = static_cast<signed char>('A');
  EXPECT_EQ(fmt::sprintf("%c", ch), "A");
  EXPECT_EQ(fmt::sprintf("%05c", ch), "    A");
  EXPECT_EQ(fmt::sprintf("%#c", ch), "A");
}

}  // namespace