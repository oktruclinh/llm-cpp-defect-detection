#include <gtest/gtest.h>

#include <fmt/format.h>

#include <string>
#include <vector>

namespace {

// Converts a UTF-8 string to UTF-16 using fmt's public API path that exercises
// the internal UTF-8 transcoding logic in format-inl.h.
static std::u16string ToUtf16(const std::string& s) {
  return fmt::detail::utf8_to_utf16(s);
}

TEST(FmtUtf8ToUtf16Test, EmptyString) {
  std::u16string out = ToUtf16("");
  EXPECT_TRUE(out.empty());
  EXPECT_EQ(out.size(), 0u);
}

TEST(FmtUtf8ToUtf16Test, AsciiSingleCharacter) {
  std::u16string out = ToUtf16("A");
  ASSERT_EQ(out.size(), 1u);
  EXPECT_EQ(out[0], u'A');
}

TEST(FmtUtf8ToUtf16Test, AsciiBoundaryLengthsAroundBlockRemainderLogic) {
  const std::vector<size_t> lengths = {
      1, 2, 3, 4, 5, 6, 7, 8, 15, 16, 17, 31, 32, 33, 63, 64, 65};

  for (size_t n : lengths) {
    std::string input(n, 'x');
    std::u16string out = ToUtf16(input);
    ASSERT_EQ(out.size(), n) << "length=" << n;
    for (size_t i = 0; i < n; ++i) {
      EXPECT_EQ(out[i], u'x') << "length=" << n << " index=" << i;
    }
  }
}

TEST(FmtUtf8ToUtf16Test, TwoByteUtf8Character) {
  const std::string input = u8"é";
  std::u16string out = ToUtf16(input);

  ASSERT_EQ(out.size(), 1u);
  EXPECT_EQ(out[0], 0x00E9);
}

TEST(FmtUtf8ToUtf16Test, ThreeByteUtf8Character) {
  const std::string input = u8"€";
  std::u16string out = ToUtf16(input);

  ASSERT_EQ(out.size(), 1u);
  EXPECT_EQ(out[0], 0x20AC);
}

TEST(FmtUtf8ToUtf16Test, FourByteUtf8CharacterProducesSurrogatePair) {
  const std::string input = u8"😀";  // U+1F600
  std::u16string out = ToUtf16(input);

  ASSERT_EQ(out.size(), 2u);
  EXPECT_EQ(out[0], 0xD83D);
  EXPECT_EQ(out[1], 0xDE00);
}

TEST(FmtUtf8ToUtf16Test, MixedAsciiBmpAndSupplementaryCharacters) {
  const std::string input = std::string("A") + u8"é€😀" + "Z";
  std::u16string out = ToUtf16(input);

  ASSERT_EQ(out.size(), 6u);
  EXPECT_EQ(out[0], u'A');
  EXPECT_EQ(out[1], 0x00E9);
  EXPECT_EQ(out[2], 0x20AC);
  EXPECT_EQ(out[3], 0xD83D);
  EXPECT_EQ(out[4], 0xDE00);
  EXPECT_EQ(out[5], u'Z');
}

TEST(FmtUtf8ToUtf16Test, RemainderPathWithSingleTrailingByteCharacter) {
  std::string input(15, 'a');
  input += "Z";

  std::u16string out = ToUtf16(input);

  ASSERT_EQ(out.size(), 16u);
  for (size_t i = 0; i < 15; ++i) EXPECT_EQ(out[i], u'a');
  EXPECT_EQ(out[15], u'Z');
}

TEST(FmtUtf8ToUtf16Test, RemainderPathWithTrailingTwoByteCharacter) {
  std::string input(15, 'a');
  input += u8"é";

  std::u16string out = ToUtf16(input);

  ASSERT_EQ(out.size(), 16u);
  for (size_t i = 0; i < 15; ++i) EXPECT_EQ(out[i], u'a');
  EXPECT_EQ(out[15], 0x00E9);
}

TEST(FmtUtf8ToUtf16Test, RemainderPathWithTrailingThreeByteCharacter) {
  std::string input(15, 'a');
  input += u8"€";

  std::u16string out = ToUtf16(input);

  ASSERT_EQ(out.size(), 16u);
  for (size_t i = 0; i < 15; ++i) EXPECT_EQ(out[i], u'a');
  EXPECT_EQ(out[15], 0x20AC);
}

TEST(FmtUtf8ToUtf16Test, RemainderPathWithTrailingFourByteCharacter) {
  std::string input(15, 'a');
  input += u8"😀";

  std::u16string out = ToUtf16(input);

  ASSERT_EQ(out.size(), 17u);
  for (size_t i = 0; i < 15; ++i) EXPECT_EQ(out[i], u'a');
  EXPECT_EQ(out[15], 0xD83D);
  EXPECT_EQ(out[16], 0xDE00);
}

TEST(FmtUtf8ToUtf16Test, IncompleteSequenceAtEndThrows) {
  const std::string input = std::string("abc") + "\xE2\x82";  // incomplete Euro sign
  EXPECT_THROW(ToUtf16(input), fmt::system_error);
}

TEST(FmtUtf8ToUtf16Test, InvalidLeadingByteThrows) {
  const std::string input = std::string("ok") + "\xFF";
  EXPECT_THROW(ToUtf16(input), fmt::system_error);
}

TEST(FmtUtf8ToUtf16Test, InvalidContinuationByteThrows) {
  const std::string input = std::string("x") + "\xE2\x28\xA1";
  EXPECT_THROW(ToUtf16(input), fmt::system_error);
}

TEST(FmtUtf8ToUtf16Test, LargeInputCrossesMultipleBlocksAndPreservesAllCharacters) {
  std::string input;
  for (int i = 0; i < 40; ++i) {
    input += "ab";
    input += u8"é";
    input += u8"€";
    input += u8"😀";
  }

  std::u16string out = ToUtf16(input);

  // Each iteration contributes:
  // "ab" -> 2 code units
  // é    -> 1
  // €    -> 1
  // 😀   -> 2
  // total = 6
  ASSERT_EQ(out.size(), 40u * 6u);

  for (int i = 0; i < 40; ++i) {
    const size_t base = static_cast<size_t>(i) * 6u;
    EXPECT_EQ(out[base + 0], u'a');
    EXPECT_EQ(out[base + 1], u'b');
    EXPECT_EQ(out[base + 2], 0x00E9);
    EXPECT_EQ(out[base + 3], 0x20AC);
    EXPECT_EQ(out[base + 4], 0xD83D);
    EXPECT_EQ(out[base + 5], 0xDE00);
  }
}

}  // namespace