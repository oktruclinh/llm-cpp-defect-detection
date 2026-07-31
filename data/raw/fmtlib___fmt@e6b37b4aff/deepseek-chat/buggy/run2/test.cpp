#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test that the transcode function (called via format_to with Unicode strings)
// correctly handles various input sizes and edge cases.
// The focal function is the loop that processes remaining characters after
// block-sized chunks, including the final null terminator push.

TEST(TranscodeTest, EmptyString) {
    std::string result = fmt::format("{}", "");
    EXPECT_EQ(result, "");
    EXPECT_EQ(result.size(), 0);
}

TEST(TranscodeTest, SingleCharacter) {
    std::string result = fmt::format("{}", "a");
    EXPECT_EQ(result, "a");
    EXPECT_EQ(result.size(), 1);
}

TEST(TranscodeTest, ShortString) {
    std::string result = fmt::format("{}", "hello");
    EXPECT_EQ(result, "hello");
    EXPECT_EQ(result.size(), 5);
}

TEST(TranscodeTest, ExactlyBlockSize) {
    // Block size is typically 4 (for UTF-8 to UTF-32 or similar), but the
    // internal block size is implementation-defined. We test with a string
    // of length 4 to exercise the boundary.
    std::string result = fmt::format("{}", "abcd");
    EXPECT_EQ(result, "abcd");
    EXPECT_EQ(result.size(), 4);
}

TEST(TranscodeTest, OneLessThanBlockSize) {
    std::string result = fmt::format("{}", "abc");
    EXPECT_EQ(result, "abc");
    EXPECT_EQ(result.size(), 3);
}

TEST(TranscodeTest, OneMoreThanBlockSize) {
    std::string result = fmt::format("{}", "abcde");
    EXPECT_EQ(result, "abcde");
    EXPECT_EQ(result.size(), 5);
}

TEST(TranscodeTest, MultipleBlocks) {
    std::string result = fmt::format("{}", "abcdefghijklmnop");
    EXPECT_EQ(result, "abcdefghijklmnop");
    EXPECT_EQ(result.size(), 16);
}

TEST(TranscodeTest, UnicodeMultiByteCharacters) {
    // UTF-8 encoded string with multi-byte characters
    std::string input = u8"Hello, 世界!";
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(result, input);
    // The string contains 9 characters but 13 bytes (including null)
    EXPECT_EQ(result.size(), 13);
}

TEST(TranscodeTest, UnicodeWithFourByteCharacter) {
    // U+1F600 (😀) is 4 bytes in UTF-8
    std::string input = u8"a😀b";
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(result, input);
    EXPECT_EQ(result.size(), 6); // 1 + 4 + 1 bytes
}

TEST(TranscodeTest, LongStringWithUnicode) {
    std::string input = u8"αβγδεζηθικλμνξοπρστυφχψω";
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(result, input);
    // 24 Greek letters, each 2 bytes in UTF-8 = 48 bytes
    EXPECT_EQ(result.size(), 48);
}

TEST(TranscodeTest, StringWithNullCharacter) {
    // Test that embedded null is handled (though format stops at null)
    std::string input("ab\0cd", 5);
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(result.size(), 2); // format stops at null
    EXPECT_EQ(result, "ab");
}

TEST(TranscodeTest, VeryLongString) {
    // Create a string longer than typical internal buffer sizes
    std::string input(1000, 'x');
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(result, input);
    EXPECT_EQ(result.size(), 1000);
}

TEST(TranscodeTest, MixedAsciiAndUnicode) {
    std::string input = u8"The quick brown fox jumps over the lazy dog. 敏捷的棕色狐狸跳过了懒狗。";
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(result, input);
    // Verify the string is properly terminated (no garbage after null)
    EXPECT_EQ(result[result.size()], '\0');
}

TEST(TranscodeTest, StringWithOnlyUnicode) {
    std::string input = u8"日本語";
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(result, input);
    EXPECT_EQ(result.size(), 9); // 3 characters * 3 bytes each
}

TEST(TranscodeTest, FormatWithArguments) {
    // Test that transcode works correctly when formatting with arguments
    std::string result = fmt::format("Hello, {}!", "world");
    EXPECT_EQ(result, "Hello, world!");
    EXPECT_EQ(result.size(), 13);
}

TEST(TranscodeTest, FormatWithMultipleArguments) {
    std::string result = fmt::format("{0} {1} {0}", "a", "b");
    EXPECT_EQ(result, "a b a");
    EXPECT_EQ(result.size(), 5);
}