#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/format-inl.h>
#include <string>
#include <vector>

// Test that transcoding works correctly for ASCII strings (no actual transcoding needed)
TEST(TranscodeTest, AsciiString) {
    std::string input = "Hello, World!";
    std::string result = fmt::detail::transcode(input);
    EXPECT_EQ(result, input);
}

// Test that transcoding handles empty strings
TEST(TranscodeTest, EmptyString) {
    std::string input = "";
    std::string result = fmt::detail::transcode(input);
    EXPECT_TRUE(result.empty());
}

// Test that transcoding correctly handles UTF-8 to UTF-16 conversion
TEST(TranscodeTest, Utf8ToUtf16) {
    // U+0041 (A) in UTF-8: 0x41
    // U+00E9 (é) in UTF-8: 0xC3 0xA9
    // U+4E2D (中) in UTF-8: 0xE4 0xB8 0xAD
    std::string input = "A\xC3\xA9\xE4\xB8\xAD";
    std::string result = fmt::detail::transcode(input);
    // Expected UTF-16LE: 0x0041, 0x00E9, 0x4E2D
    std::string expected = "\x41\x00\xE9\x00\x2D\x4E";
    EXPECT_EQ(result, expected);
}

// Test that transcoding handles surrogate pairs correctly
TEST(TranscodeTest, SurrogatePair) {
    // U+1F600 (😀) in UTF-8: 0xF0 0x9F 0x98 0x80
    std::string input = "\xF0\x9F\x98\x80";
    std::string result = fmt::detail::transcode(input);
    // Expected UTF-16LE surrogate pair: 0xD83D 0xDE00
    std::string expected = "\x3D\xD8\x00\xDE";
    EXPECT_EQ(result, expected);
}

// Test that transcoding handles multiple code points including surrogate pairs
TEST(TranscodeTest, MixedUtf8) {
    // "A😀é中" in UTF-8
    std::string input = "A\xF0\x9F\x98\x80\xC3\xA9\xE4\xB8\xAD";
    std::string result = fmt::detail::transcode(input);
    // Expected UTF-16LE: 0x0041, 0xD83D 0xDE00, 0x00E9, 0x4E2D
    std::string expected = "\x41\x00\x3D\xD8\x00\xDE\xE9\x00\x2D\x4E";
    EXPECT_EQ(result, expected);
}

// Test that transcoding handles the boundary case of exactly block_size characters
TEST(TranscodeTest, ExactBlockSize) {
    // Create a string that will produce exactly block_size (typically 4) UTF-16 code units
    // 4 ASCII characters = 4 UTF-16 code units
    std::string input = "ABCD";
    std::string result = fmt::detail::transcode(input);
    std::string expected = "\x41\x00\x42\x00\x43\x00\x44\x00";
    EXPECT_EQ(result, expected);
}

// Test that transcoding handles strings that are one character less than block_size
TEST(TranscodeTest, OneLessThanBlockSize) {
    std::string input = "ABC";
    std::string result = fmt::detail::transcode(input);
    std::string expected = "\x41\x00\x42\x00\x43\x00";
    EXPECT_EQ(result, expected);
}

// Test that transcoding handles strings that are one character more than block_size
TEST(TranscodeTest, OneMoreThanBlockSize) {
    std::string input = "ABCDE";
    std::string result = fmt::detail::transcode(input);
    std::string expected = "\x41\x00\x42\x00\x43\x00\x44\x00\x45\x00";
    EXPECT_EQ(result, expected);
}

// Test that transcoding handles invalid UTF-8 gracefully (replacement character)
TEST(TranscodeTest, InvalidUtf8) {
    // Invalid UTF-8 sequence: 0x80 (continuation byte without leading byte)
    std::string input = "\x80";
    std::string result = fmt::detail::transcode(input);
    // Should produce replacement character U+FFFD in UTF-16LE
    std::string expected = "\xFD\xFF";
    EXPECT_EQ(result, expected);
}

// Test that transcoding handles overlong UTF-8 sequences
TEST(TranscodeTest, OverlongUtf8) {
    // Overlong encoding of '/' (0x2F) in UTF-8: 0xC0 0xAF
    std::string input = "\xC0\xAF";
    std::string result = fmt::detail::transcode(input);
    // Should produce replacement character U+FFFD in UTF-16LE
    std::string expected = "\xFD\xFF";
    EXPECT_EQ(result, expected);
}

// Test that transcoding handles lone surrogates in UTF-8 (invalid)
TEST(TranscodeTest, LoneSurrogate) {
    // UTF-8 encoding of U+D800 (lone surrogate) is invalid
    std::string input = "\xED\xA0\x80";
    std::string result = fmt::detail::transcode(input);
    // Should produce replacement character U+FFFD in UTF-16LE
    std::string expected = "\xFD\xFF";
    EXPECT_EQ(result, expected);
}

// Test that transcoding handles very long strings (stress test)
TEST(TranscodeTest, LongString) {
    std::string input;
    input.reserve(10000);
    for (int i = 0; i < 2500; ++i) {
        input += "A\xC3\xA9";  // 2 code points, 4 bytes
    }
    std::string result = fmt::detail::transcode(input);
    // Each "Aé" becomes 4 bytes in UTF-16LE (0x0041, 0x00E9)
    EXPECT_EQ(result.size(), 2500 * 4);
    // Verify first and last characters
    EXPECT_EQ(result[0], '\x41');
    EXPECT_EQ(result[1], '\x00');
    EXPECT_EQ(result[result.size() - 2], '\xE9');
    EXPECT_EQ(result[result.size() - 1], '\x00');
}

// Test that transcoding handles strings with only multi-byte characters
TEST(TranscodeTest, OnlyMultiByte) {
    // Three 3-byte UTF-8 characters: 中文字
    std::string input = "\xE4\xB8\xAD\xE6\x96\x87\xE5\xAD\x97";
    std::string result = fmt::detail::transcode(input);
    // Expected UTF-16LE: 0x4E2D, 0x6587, 0x5B57
    std::string expected = "\x2D\x4E\x87\x65\x57\x5B";
    EXPECT_EQ(result, expected);
}

// Test that transcoding handles strings with BOM (Byte Order Mark)
TEST(TranscodeTest, WithBom) {
    // UTF-8 BOM: 0xEF 0xBB 0xBF followed by "A"
    std::string input = "\xEF\xBB\xBF\x41";
    std::string result = fmt::detail::transcode(input);
    // BOM becomes U+FEFF in UTF-16LE, then 'A' becomes U+0041
    std::string expected = "\xFF\xFE\x41\x00";
    EXPECT_EQ(result, expected);
}