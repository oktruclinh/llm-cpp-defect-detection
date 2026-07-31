#include <gtest/gtest.h>

#include <fmt/format.h>
#include <fmt/xchar.h>

#include <string>
#include <vector>

namespace {

TEST(FmtUtf8ToUtf16BoundaryTest, EmptyInputProducesEmptyOutput) {
  std::wstring out = fmt::detail::utf8_to_utf16("");
  EXPECT_TRUE(out.empty());
}

TEST(FmtUtf8ToUtf16BoundaryTest, AsciiSingleCharacter) {
  std::wstring out = fmt::detail::utf8_to_utf16("A");
  ASSERT_EQ(out.size(), 1u);
  EXPECT_EQ(out[0], L'A');
}

TEST(FmtUtf8ToUtf16BoundaryTest, AsciiAtInternalBlockBoundary) {
  // The focal code processes full blocks and then a leftover tail. These sizes
  // exercise exact-boundary behavior for common SIMD/transcoding block sizes.
  for (size_t n : {15u, 16u, 31u, 32u, 63u, 64u, 127u, 128u}) {
    std::string input(n, 'x');
    std::wstring out = fmt::detail::utf8_to_utf16(input);

    ASSERT_EQ(out.size(), n) << "input size=" << n;
    for (size_t i = 0; i < n; ++i) EXPECT_EQ(out[i], L'x') << "at i=" << i;
  }
}

TEST(FmtUtf8ToUtf16BoundaryTest, AsciiAroundInternalBlockBoundary) {
  // Exercise sizes just below and above likely block boundaries to validate
  // leftover-buffer logic.
  for (size_t n : {1u, 2u, 3u, 14u, 15u, 16u, 17u, 18u, 30u, 31u, 32u, 33u,
                   62u, 63u, 64u, 65u, 126u, 127u, 128u, 129u}) {
    std::string input;
    input.reserve(n);
    for (size_t i = 0; i < n; ++i) input.push_back(static_cast<char>('a' + (i % 26)));

    std::wstring out = fmt::detail::utf8_to_utf16(input);

    ASSERT_EQ(out.size(), n) << "input size=" << n;
    for (size_t i = 0; i < n; ++i) {
      EXPECT_EQ(out[i], static_cast<wchar_t>('a' + (i % 26))) << "size=" << n << " i=" << i;
    }
  }
}

TEST(FmtUtf8ToUtf16BoundaryTest, MultiByteCharactersInLeftoverTail) {
  // Construct input with a long ASCII prefix plus a multi-byte UTF-8 suffix.
  // This specifically stresses the path that copies the tail into a temporary
  // zero-initialized buffer and continues transcoding there.
  const std::string input =
      std::string(31, 'a') +         // likely leaves a small tail after block processing
      "\xC2\xA2"                     // U+00A2 CENT SIGN
      "\xE2\x82\xAC"                 // U+20AC EURO SIGN
      "\xF0\x9F\x98\x80";            // U+1F600 GRINNING FACE

  std::wstring out = fmt::detail::utf8_to_utf16(input);

  ASSERT_EQ(out.size(), 34u);
  for (size_t i = 0; i < 31; ++i) EXPECT_EQ(out[i], L'a');
  EXPECT_EQ(out[31], static_cast<wchar_t>(0x00A2));
  EXPECT_EQ(out[32], static_cast<wchar_t>(0x20AC));
  if (sizeof(wchar_t) == 2) {
    ASSERT_EQ(out.size(), 35u) << "UTF-16 should use a surrogate pair on 16-bit wchar_t";
    EXPECT_EQ(static_cast<unsigned int>(out[33]), 0xD83D);
    EXPECT_EQ(static_cast<unsigned int>(out[34]), 0xDE00);
  } else {
    EXPECT_EQ(static_cast<unsigned int>(out[33]), 0x1F600);
  }
}

TEST(FmtUtf8ToUtf16BoundaryTest, ValidCodePointSplitNearTailBoundary) {
  // Prefix lengths around common block sizes ensure the final code point lands
  // in the leftover region. The result must still decode correctly.
  const std::vector<size_t> prefix_lengths = {14u, 15u, 16u, 17u, 30u, 31u, 32u, 33u};
  for (size_t prefix_len : prefix_lengths) {
    std::string input(prefix_len, 'b');
    input += "\xE2\x82\xAC";  // U+20AC

    std::wstring out = fmt::detail::utf8_to_utf16(input);

    ASSERT_EQ(out.size(), prefix_len + 1) << "prefix_len=" << prefix_len;
    for (size_t i = 0; i < prefix_len; ++i) {
      EXPECT_EQ(out[i], L'b') << "prefix_len=" << prefix_len << " i=" << i;
    }
    EXPECT_EQ(out[prefix_len], static_cast<wchar_t>(0x20AC)) << "prefix_len=" << prefix_len;
  }
}

TEST(FmtUtf8ToUtf16BoundaryTest, EmbeddedNullByteIsPreserved) {
  std::string input = std::string("ab", 2) + '\0' + std::string("cd", 2);

  std::wstring out = fmt::detail::utf8_to_utf16(input);

  ASSERT_EQ(out.size(), 5u);
  EXPECT_EQ(out[0], L'a');
  EXPECT_EQ(out[1], L'b');
  EXPECT_EQ(out[2], L'\0');
  EXPECT_EQ(out[3], L'c');
  EXPECT_EQ(out[4], L'd');
}

TEST(FmtUtf8ToUtf16BoundaryTest, LongMixedStringDecodesCorrectly) {
  std::string input;
  for (int i = 0; i < 50; ++i) input += 'A';
  input += "\xC2\xA9";       // U+00A9
  for (int i = 0; i < 50; ++i) input += 'B';
  input += "\xE2\x98\x83";   // U+2603
  for (int i = 0; i < 50; ++i) input += 'C';

  std::wstring out = fmt::detail::utf8_to_utf16(input);

  ASSERT_EQ(out.size(), 152u);
  for (size_t i = 0; i < 50; ++i) EXPECT_EQ(out[i], L'A');
  EXPECT_EQ(out[50], static_cast<wchar_t>(0x00A9));
  for (size_t i = 51; i < 101; ++i) EXPECT_EQ(out[i], L'B');
  EXPECT_EQ(out[101], static_cast<wchar_t>(0x2603));
  for (size_t i = 102; i < 152; ++i) EXPECT_EQ(out[i], L'C');
}

TEST(FmtUtf8ToUtf16BoundaryTest, InvalidUtf8ThrowsFormatError) {
  // Lone continuation byte is invalid UTF-8.
  EXPECT_THROW(fmt::detail::utf8_to_utf16(std::string("\x80", 1)), fmt::format_error);

  // Incomplete multi-byte sequence at end; this is directly relevant to the
  // leftover-tail handling in the focal code.
  EXPECT_THROW(fmt::detail::utf8_to_utf16(std::string("abc\xE2\x82", 5)), fmt::format_error);

  // Leading byte for a 4-byte sequence without sufficient continuation bytes.
  EXPECT_THROW(fmt::detail::utf8_to_utf16(std::string("\xF0\x9F\x98", 3)), fmt::format_error);
}

TEST(FmtUtf8ToUtf16BoundaryTest, InvalidSequenceAfterLongPrefixThrows) {
  for (size_t prefix_len : {15u, 16u, 31u, 32u, 63u, 64u}) {
    std::string input(prefix_len, 'x');
    input += "\xE2\x82";  // truncated U+20AC

    EXPECT_THROW(
        {
          auto out = fmt::detail::utf8_to_utf16(input);
          (void)out;
        },
        fmt::format_error)
        << "prefix_len=" << prefix_len;
  }
}

}  // namespace