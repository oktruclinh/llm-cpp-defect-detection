#include <gtest/gtest.h>

#include <fmt/printf.h>

#include <string>

namespace {

TEST(PrintfCharFormattingTest, ZeroFlagIsIgnoredForCharAndUsesRightAlignment) {
  EXPECT_EQ("    A", fmt::sprintf("%05c", 'A'));
  EXPECT_EQ("    a", fmt::sprintf("%05c", 'a'));
}

TEST(PrintfCharFormattingTest, ZeroFlagWithExplicitWidthOneDoesNotPadChar) {
  EXPECT_EQ("A", fmt::sprintf("%01c", 'A'));
  EXPECT_EQ("\0", fmt::sprintf("%01c", '\0'));
}

TEST(PrintfCharFormattingTest, LeftAlignmentOverridesZeroFlagForChar) {
  EXPECT_EQ("A    ", fmt::sprintf("%-05c", 'A'));
  EXPECT_EQ("z    ", fmt::sprintf("%-05c", 'z'));
}

TEST(PrintfCharFormattingTest, SignFlagsAreIgnoredForChar) {
  EXPECT_EQ("A", fmt::sprintf("%+c", 'A'));
  EXPECT_EQ("A", fmt::sprintf("% c", 'A'));
}

TEST(PrintfCharFormattingTest, AlternateFormFlagIsIgnoredForChar) {
  EXPECT_EQ("A", fmt::sprintf("%#c", 'A'));
  EXPECT_EQ("    A", fmt::sprintf("%#5c", 'A'));
  EXPECT_EQ("    A", fmt::sprintf("%#05c", 'A'));
}

TEST(PrintfCharFormattingTest, CombinationOfIgnoredFlagsStillFormatsAsCharacter) {
  EXPECT_EQ("    A", fmt::sprintf("%#+05c", 'A'));
  EXPECT_EQ("A    ", fmt::sprintf("%#-+05c", 'A'));
}

TEST(PrintfCharFormattingTest, NullCharacterIsHandledWithWidthAndPreservesSize) {
  std::string s = fmt::sprintf("%05c", '\0');
  ASSERT_EQ(5u, s.size());
  EXPECT_EQ('\0', s[4]);
  EXPECT_EQ(' ', s[0]);
  EXPECT_EQ(' ', s[1]);
  EXPECT_EQ(' ', s[2]);
  EXPECT_EQ(' ', s[3]);
}

TEST(PrintfCharFormattingTest, UnsignedCharUsesCharacterFormattingNotNumericFormatting) {
  unsigned char value = static_cast<unsigned char>('A');
  EXPECT_EQ("A", fmt::sprintf("%c", value));
  EXPECT_EQ("    A", fmt::sprintf("%05c", value));
  EXPECT_EQ("A    ", fmt::sprintf("%-05c", value));
}

TEST(PrintfCharFormattingTest, SignedCharUsesCharacterFormattingNotNumericFormatting) {
  signed char value = static_cast<signed char>('A');
  EXPECT_EQ("A", fmt::sprintf("%c", value));
  EXPECT_EQ("    A", fmt::sprintf("%05c", value));
  EXPECT_EQ("A    ", fmt::sprintf("%-05c", value));
}

TEST(PrintfCharFormattingTest, IntegerWithCSpecifierFormatsAsCharacter) {
  EXPECT_EQ("A", fmt::sprintf("%c", 65));
  EXPECT_EQ("    A", fmt::sprintf("%05c", 65));
  EXPECT_EQ("A    ", fmt::sprintf("%-05c", 65));
}

TEST(PrintfCharFormattingTest, WideCharacterLikeBoundaryByteValueFormatsAsSingleChar) {
  int value = 255;
  std::string s = fmt::sprintf("%5c", value);
  ASSERT_EQ(5u, s.size());
  EXPECT_EQ(' ', s[0]);
  EXPECT_EQ(' ', s[1]);
  EXPECT_EQ(' ', s[2]);
  EXPECT_EQ(' ', s[3]);
  EXPECT_EQ(static_cast<char>(static_cast<unsigned char>(255)), s[4]);
}

TEST(PrintfCharFormattingTest, NoUnexpectedZerosAppearWhenZeroFlagUsedWithChar) {
  std::string s = fmt::sprintf("%05c", '7');
  EXPECT_EQ("    7", s);
  EXPECT_NE("00007", s);
}

}  // namespace