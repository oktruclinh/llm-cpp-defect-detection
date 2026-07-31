#include <gtest/gtest.h>
#include <fmt/format.h> // Public API header for fmtlib

// This test suite focuses on the behavior of string transcoding,
// particularly how `format_to` handles strings that might require
// transcoding (e.g., UTF-8 to internal representation or vice-versa,
// though fmtlib generally works with UTF-8 internally).
// The focal function snippet is from an internal implementation detail
// of how `format_to` efficiently processes blocks of characters.
// We will test this by observing the output of `format_to` with various
// string inputs, including those that might trigger the block processing
// and the final `num_chars_left` handling.

TEST(FormatToTranscodeTest, EmptyString) {
  std::string s = "";
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result, "");
}

TEST(FormatToTranscodeTest, SimpleAsciiString) {
  std::string s = "Hello, World!";
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result, "Hello, World!");
}

TEST(FormatToTranscodeTest, StringWithSpecialAsciiChars) {
  std::string s = "Tabs\tNewlines\nBackslashes\\Quotes\"";
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result, "Tabs\tNewlines\nBackslashes\\Quotes\"");
}

TEST(FormatToTranscodeTest, LongAsciiString) {
  std::string s(1000, 'A'); // A long string to ensure block processing
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result, s);
}

TEST(FormatToTranscodeTest, Utf8StringSingleByteChars) {
  std::string s = "Grüße"; // German word with ü (2 bytes in UTF-8)
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result, "Grüße");
  EXPECT_EQ(result.length(), 6); // 5 characters, but ü is 2 bytes
  EXPECT_EQ(result.size(), 7);   // 5 ASCII + 2 for ü
}

TEST(FormatToTranscodeTest, Utf8StringMultiByteChars) {
  std::string s = "你好世界"; // Chinese "Hello World" (3 bytes per char)
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result, "你好世界");
  EXPECT_EQ(result.length(), 4); // 4 characters
  EXPECT_EQ(result.size(), 12);  // 4 * 3 bytes
}

TEST(FormatToTranscodeTest, Utf8StringMixedChars) {
  std::string s = "Hello, 世界!"; // Mixed ASCII and UTF-8
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result, "Hello, 世界!");
  EXPECT_EQ(result.length(), 10); // 7 ASCII + 2 Chinese + 1 ASCII
  EXPECT_EQ(result.size(), 13);   // 7 ASCII + 2*3 Chinese + 1 ASCII
}

TEST(FormatToTranscodeTest, Utf8StringWithEmoji) {
  std::string s = "Smile 😊"; // Emoji (4 bytes in UTF-8)
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result, "Smile 😊");
  EXPECT_EQ(result.length(), 7); // 6 ASCII + 1 Emoji
  EXPECT_EQ(result.size(), 10);  // 6 ASCII + 4 Emoji
}

TEST(FormatToTranscodeTest, StringExactlyBlockSize) {
  // Assuming a typical block_size (e.g., 16 or 32).
  // We can't know the exact block_size, but a string of a certain length
  // should hit the block processing logic.
  // Let's pick a length that is likely to be a multiple or near a multiple
  // of a common block size.
  std::string s(32, 'X');
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result, s);
}

TEST(FormatToTranscodeTest, StringSlightlyMoreThanBlockSize) {
  // This case is designed to test the `num_chars_left` logic.
  // If block_size is 16, a string of length 17 would process one block
  // and then have 1 character left.
  std::string s(33, 'Y');
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result, s);
}

TEST(FormatToTranscodeTest, StringSlightlyLessThanBlockSize) {
  // This case might entirely fall into the `num_chars_left` logic
  // if `s.size() - block_size + 1` becomes non-positive, skipping the loop.
  // Or it might process one block and then have a remainder.
  std::string s(15, 'Z');
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result, s);
}

TEST(FormatToTranscodeTest, StringWithNullCharacter) {
  // fmtlib generally handles null characters correctly within strings,
  // treating them as regular characters rather than terminators.
  std::string s = "Hello\0World";
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result.size(), 11); // Ensure the null character is included in size
  EXPECT_EQ(result[5], '\0');
  // Note: Printing `result` might truncate in some environments,
  // but the string object itself should contain the null.
  // We can't directly compare `result` with `s` using `EXPECT_EQ`
  // if `s` is a C-style string literal, but `std::string` comparison works.
  EXPECT_EQ(result, s);
}

TEST(FormatToTranscodeTest, StringWithEmbeddedNullAndUtf8) {
  std::string s = "UTF8\0你好";
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result.size(), 4 + 1 + 6); // 4 ASCII + 1 null + 2*3 UTF-8
  EXPECT_EQ(result[4], '\0');
  EXPECT_EQ(result, s);
}

// Test with a string that might cause alignment issues or specific block
// boundary conditions for multi-byte characters.
TEST(FormatToTranscodeTest, Utf8StringCrossingBlockBoundary) {
  // Create a string where a multi-byte character spans a potential block boundary.
  // We'll use a character like 'é' (2 bytes) or '€' (3 bytes).
  // Let's assume a block size of 16 for illustration.
  // If we have 15 'A's and then '€', the '€' would cross the boundary.
  std::string base_ascii(15, 'A');
  std::string s = base_ascii + "€" + base_ascii; // '€' is 3 bytes
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result, s);
  EXPECT_EQ(result.size(), 15 + 3 + 15);
}

TEST(FormatToTranscodeTest, VeryLongUtf8String) {
  std::string utf8_char = "你好"; // 6 bytes
  std::string s;
  for (int i = 0; i < 100; ++i) { // 100 * 6 = 600 bytes
    s += utf8_char;
  }
  std::string result = fmt::format("{}", s);
  EXPECT_EQ(result, s);
  EXPECT_EQ(result.size(), 600);
}