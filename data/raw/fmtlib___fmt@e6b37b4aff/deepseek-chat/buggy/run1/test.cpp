#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test that the function handles empty strings correctly
TEST(FormatInlTest, EmptyString) {
    std::string result = fmt::format("{}", "");
    EXPECT_EQ(result, "");
}

// Test that the function handles single character strings
TEST(FormatInlTest, SingleCharacter) {
    std::string result = fmt::format("{}", "a");
    EXPECT_EQ(result, "a");
}

// Test that the function handles short strings (less than block_size)
TEST(FormatInlTest, ShortString) {
    std::string result = fmt::format("{}", "hello");
    EXPECT_EQ(result, "hello");
}

// Test that the function handles strings exactly at block_size boundary
TEST(FormatInlTest, ExactBlockSize) {
    // block_size is typically 4, so test with 4 characters
    std::string result = fmt::format("{}", "abcd");
    EXPECT_EQ(result, "abcd");
}

// Test that the function handles strings just over block_size
TEST(FormatInlTest, JustOverBlockSize) {
    std::string result = fmt::format("{}", "abcde");
    EXPECT_EQ(result, "abcde");
}

// Test that the function handles strings that are multiples of block_size
TEST(FormatInlTest, MultipleOfBlockSize) {
    std::string result = fmt::format("{}", "abcdefgh");
    EXPECT_EQ(result, "abcdefgh");
}

// Test that the function handles long strings
TEST(FormatInlTest, LongString) {
    std::string long_str(1000, 'x');
    std::string result = fmt::format("{}", long_str);
    EXPECT_EQ(result, long_str);
}

// Test that the function handles strings with special characters
TEST(FormatInlTest, SpecialCharacters) {
    std::string result = fmt::format("{}", "hello\nworld\t!");
    EXPECT_EQ(result, "hello\nworld\t!");
}

// Test that the function handles strings with unicode characters
TEST(FormatInlTest, UnicodeCharacters) {
    std::string result = fmt::format("{}", "héllo wörld");
    EXPECT_EQ(result, "héllo wörld");
}

// Test that the function handles strings with null characters
TEST(FormatInlTest, NullCharacter) {
    std::string input = "hello";
    input.push_back('\0');
    input += "world";
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(result, input);
}

// Test that the function handles very long strings (stress test)
TEST(FormatInlTest, VeryLongString) {
    std::string long_str(10000, 'a');
    std::string result = fmt::format("{}", long_str);
    EXPECT_EQ(result, long_str);
}

// Test that the function handles strings with only whitespace
TEST(FormatInlTest, WhitespaceOnly) {
    std::string result = fmt::format("{}", "   \t\n  ");
    EXPECT_EQ(result, "   \t\n  ");
}

// Test that the function handles strings with repeated patterns
TEST(FormatInlTest, RepeatedPattern) {
    std::string pattern = "abc";
    std::string input;
    for (int i = 0; i < 100; ++i) {
        input += pattern;
    }
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(result, input);
}

// Test that the function handles strings that are exactly one character less than block_size
TEST(FormatInlTest, OneLessThanBlockSize) {
    std::string result = fmt::format("{}", "abc");
    EXPECT_EQ(result, "abc");
}

// Test that the function handles strings that are exactly one character more than block_size
TEST(FormatInlTest, OneMoreThanBlockSize) {
    std::string result = fmt::format("{}", "abcde");
    EXPECT_EQ(result, "abcde");
}

// Test that the function handles strings with varying lengths
TEST(FormatInlTest, VaryingLengths) {
    for (int len = 0; len < 20; ++len) {
        std::string input(len, 'a' + (len % 26));
        std::string result = fmt::format("{}", input);
        EXPECT_EQ(result, input) << "Failed for length " << len;
    }
}

// Test that the function handles strings with all printable ASCII characters
TEST(FormatInlTest, AllPrintableASCII) {
    std::string input;
    for (char c = 32; c < 127; ++c) {
        input += c;
    }
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(result, input);
}

// Test that the function handles strings with non-printable characters
TEST(FormatInlTest, NonPrintableCharacters) {
    std::string input;
    for (char c = 1; c < 32; ++c) {
        input += c;
    }
    input += 127;
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(result, input);
}