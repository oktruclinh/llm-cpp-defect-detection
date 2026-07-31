#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test that the transcoding function handles empty strings correctly
TEST(FormatTranscodeTest, EmptyString) {
    fmt::memory_buffer buf;
    fmt::format_to(std::back_inserter(buf), "{}", "");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "");
}

// Test that ASCII strings pass through unchanged
TEST(FormatTranscodeTest, AsciiString) {
    fmt::memory_buffer buf;
    fmt::format_to(std::back_inserter(buf), "{}", "Hello, World!");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "Hello, World!");
}

// Test that the function handles strings with multibyte UTF-8 characters
TEST(FormatTranscodeTest, MultibyteUtf8) {
    fmt::memory_buffer buf;
    fmt::format_to(std::back_inserter(buf), "{}", "Café résumé ñoño");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "Café résumé ñoño");
}

// Test that the function handles strings with 4-byte UTF-8 characters (emoji)
TEST(FormatTranscodeTest, FourByteUtf8) {
    fmt::memory_buffer buf;
    fmt::format_to(std::back_inserter(buf), "{}", "😀🎉🔥");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "😀🎉🔥");
}

// Test that the function handles strings with mixed ASCII and multibyte characters
TEST(FormatTranscodeTest, MixedAsciiAndMultibyte) {
    fmt::memory_buffer buf;
    fmt::format_to(std::back_inserter(buf), "{}", "Hello 世界! Привет!");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "Hello 世界! Привет!");
}

// Test that the function handles strings that are exactly the block size
TEST(FormatTranscodeTest, ExactBlockSize) {
    // The block size is 4 bytes in the implementation
    fmt::memory_buffer buf;
    fmt::format_to(std::back_inserter(buf), "{}", "abcd");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "abcd");
}

// Test that the function handles strings that are one byte less than block size
TEST(FormatTranscodeTest, OneLessThanBlockSize) {
    fmt::memory_buffer buf;
    fmt::format_to(std::back_inserter(buf), "{}", "abc");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "abc");
}

// Test that the function handles strings that are one byte more than block size
TEST(FormatTranscodeTest, OneMoreThanBlockSize) {
    fmt::memory_buffer buf;
    fmt::format_to(std::back_inserter(buf), "{}", "abcde");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "abcde");
}

// Test that the function handles strings with trailing incomplete UTF-8 sequences
TEST(FormatTranscodeTest, TrailingIncompleteSequence) {
    fmt::memory_buffer buf;
    // This is an incomplete UTF-8 sequence (0xC3 is start of 2-byte sequence, but missing continuation byte)
    std::string incomplete = "abc\xc3";
    fmt::format_to(std::back_inserter(buf), "{}", incomplete);
    // The implementation should handle this gracefully, likely by copying the incomplete bytes
    EXPECT_EQ(std::string(buf.data(), buf.size()), incomplete);
}

// Test that the function handles strings with leading incomplete UTF-8 sequences
TEST(FormatTranscodeTest, LeadingIncompleteSequence) {
    fmt::memory_buffer buf;
    // This is an incomplete UTF-8 sequence at the start
    std::string incomplete = "\xc3" "abc";
    fmt::format_to(std::back_inserter(buf), "{}", incomplete);
    EXPECT_EQ(std::string(buf.data(), buf.size()), incomplete);
}

// Test that the function handles very long strings (stress test)
TEST(FormatTranscodeTest, VeryLongString) {
    fmt::memory_buffer buf;
    std::string long_str(10000, 'a');
    fmt::format_to(std::back_inserter(buf), "{}", long_str);
    EXPECT_EQ(std::string(buf.data(), buf.size()), long_str);
}

// Test that the function handles strings with only multibyte characters
TEST(FormatTranscodeTest, OnlyMultibyte) {
    fmt::memory_buffer buf;
    fmt::format_to(std::back_inserter(buf), "{}", "日本語");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "日本語");
}

// Test that the function handles strings with special characters (null, newline, etc.)
TEST(FormatTranscodeTest, SpecialCharacters) {
    fmt::memory_buffer buf;
    std::string special = "line1\nline2\tindented\0embedded null";
    special.resize(special.size() + 1); // Include the null terminator
    fmt::format_to(std::back_inserter(buf), "{}", special);
    EXPECT_EQ(std::string(buf.data(), buf.size()), special);
}

// Test that the function handles strings with only one character
TEST(FormatTranscodeTest, SingleCharacter) {
    fmt::memory_buffer buf;
    fmt::format_to(std::back_inserter(buf), "{}", "X");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "X");
}

// Test that the function handles strings with only one multibyte character
TEST(FormatTranscodeTest, SingleMultibyteCharacter) {
    fmt::memory_buffer buf;
    fmt::format_to(std::back_inserter(buf), "{}", "é");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "é");
}

// Test that the function handles strings with only one 4-byte character
TEST(FormatTranscodeTest, SingleFourByteCharacter) {
    fmt::memory_buffer buf;
    fmt::format_to(std::back_inserter(buf), "{}", "😀");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "😀");
}

// Test that the function handles strings with multiple blocks of multibyte characters
TEST(FormatTranscodeTest, MultipleBlocksOfMultibyte) {
    fmt::memory_buffer buf;
    // Create a string with many multibyte characters that span multiple blocks
    std::string many_emojis;
    for (int i = 0; i < 100; ++i) {
        many_emojis += "😀";
    }
    fmt::format_to(std::back_inserter(buf), "{}", many_emojis);
    EXPECT_EQ(std::string(buf.data(), buf.size()), many_emojis);
}

// Test that the function handles strings with mixed block-aligned and non-aligned multibyte characters
TEST(FormatTranscodeTest, MixedAlignmentMultibyte) {
    fmt::memory_buffer buf;
    // 3 ASCII chars + 1 multibyte char = 4 bytes (exactly one block)
    fmt::format_to(std::back_inserter(buf), "{}", "abcé");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "abcé");
    
    buf.clear();
    // 2 ASCII chars + 1 multibyte char = 3 bytes (less than one block)
    fmt::format_to(std::back_inserter(buf), "{}", "abé");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "abé");
    
    buf.clear();
    // 5 ASCII chars + 1 multibyte char = 6 bytes (more than one block)
    fmt::format_to(std::back_inserter(buf), "{}", "abcdeé");
    EXPECT_EQ(std::string(buf.data(), buf.size()), "abcdeé");
}