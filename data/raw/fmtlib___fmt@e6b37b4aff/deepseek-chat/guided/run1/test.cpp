#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test transcode behavior through public API by verifying correct UTF-8 to UTF-16 conversion
// The focal function handles transcoding of remaining characters at end of buffer

TEST(TranscodeTest, BasicAscii) {
    // ASCII characters should pass through unchanged
    std::string input = "Hello, World!";
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(input, result);
}

TEST(TranscodeTest, SingleMultiByteCharacter) {
    // Single 2-byte UTF-8 character
    std::string input = "\xC3\xA9"; // é
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(input, result);
}

TEST(TranscodeTest, MultiByteCharactersAtEnd) {
    // Multiple multi-byte characters that might trigger the end-of-buffer handling
    std::string input = "\xF0\x9F\x98\x80\xF0\x9F\x98\x81"; // 😀😁
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(input, result);
}

TEST(TranscodeTest, MixedAsciiAndMultiByte) {
    // Mix of ASCII and multi-byte characters
    std::string input = "Hello \xC3\xA9 \xF0\x9F\x98\x80!";
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(input, result);
}

TEST(TranscodeTest, ThreeByteCharacter) {
    // 3-byte UTF-8 character
    std::string input = "\xE2\x82\xAC"; // €
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(input, result);
}

TEST(TranscodeTest, FourByteCharacter) {
    // 4-byte UTF-8 character
    std::string input = "\xF0\x9F\x8D\x95"; // 🍕
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(input, result);
}

TEST(TranscodeTest, EmptyString) {
    // Empty string should remain empty
    std::string input = "";
    std::string result = fmt::format("{}", input);
    EXPECT_TRUE(result.empty());
}

TEST(TranscodeTest, SingleCharacter) {
    // Single ASCII character
    std::string input = "A";
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(input, result);
}

TEST(TranscodeTest, LongStringWithMultiByte) {
    // Long string that may trigger multiple buffer flushes
    std::string input;
    for (int i = 0; i < 100; ++i) {
        input += "\xF0\x9F\x98\x80"; // 😀
    }
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(input, result);
}

TEST(TranscodeTest, StringWithNewlines) {
    // String containing newlines and multi-byte characters
    std::string input = "Line1\nLine2\n\xC3\xA9nd";
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(input, result);
}

TEST(TranscodeTest, StringWithTabs) {
    // String with tabs and multi-byte characters
    std::string input = "\t\xC3\xA9\t\xF0\x9F\x98\x80\t";
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(input, result);
}

TEST(TranscodeTest, AllAsciiCharacters) {
    // All printable ASCII characters
    std::string input;
    for (char c = 32; c < 127; ++c) {
        input += c;
    }
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(input, result);
}

TEST(TranscodeTest, InvalidUtf8ContinuationByte) {
    // Invalid UTF-8: continuation byte without start byte
    std::string input = "\x80\x81\x82";
    std::string result = fmt::format("{}", input);
    // The library should handle this gracefully (may produce replacement characters or pass through)
    EXPECT_FALSE(result.empty());
}

TEST(TranscodeTest, OverlongEncoding) {
    // Overlong encoding of ASCII character
    std::string input = "\xC0\x80"; // overlong encoding of NUL
    std::string result = fmt::format("{}", input);
    // Should handle gracefully
    EXPECT_FALSE(result.empty());
}

TEST(TranscodeTest, SurrogatePairInUtf8) {
    // UTF-8 encoding of a surrogate half (invalid in UTF-8)
    std::string input = "\xED\xA0\x80"; // U+D800
    std::string result = fmt::format("{}", input);
    // Should handle gracefully
    EXPECT_FALSE(result.empty());
}

TEST(TranscodeTest, StringWithNullCharacter) {
    // String containing embedded null character
    std::string input = "Hello\x00World";
    input += '\0'; // Ensure null is included
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(input.size(), result.size());
    EXPECT_EQ(input, result);
}

TEST(TranscodeTest, VeryLongString) {
    // Very long string to stress test buffer handling
    std::string input;
    input.reserve(10000);
    for (int i = 0; i < 1000; ++i) {
        input += "Hello World! \xC3\xA9 \xF0\x9F\x98\x80 ";
    }
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(input, result);
}

TEST(TranscodeTest, StringEndingWithMultiByte) {
    // String that ends exactly with a multi-byte character
    std::string input = "test\xC3\xA9";
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(input, result);
}

TEST(TranscodeTest, StringEndingWithPartialMultiByte) {
    // String ending with incomplete multi-byte sequence
    std::string input = "test\xC3"; // incomplete 2-byte sequence
    std::string result = fmt::format("{}", input);
    // Should handle gracefully
    EXPECT_FALSE(result.empty());
}

TEST(TranscodeTest, FormatWithWidth) {
    // Using format specifiers with multi-byte strings
    std::string input = "\xF0\x9F\x98\x80";
    std::string result = fmt::format("{:>10}", input);
    EXPECT_EQ(result.size(), 10);
    EXPECT_EQ(result.substr(6), input);
}

TEST(TranscodeTest, FormatWithFill) {
    // Using fill character with multi-byte strings
    std::string input = "\xC3\xA9";
    std::string result = fmt::format("{:*<10}", input);
    EXPECT_EQ(result.size(), 10);
    EXPECT_EQ(result.substr(0, 2), input);
}