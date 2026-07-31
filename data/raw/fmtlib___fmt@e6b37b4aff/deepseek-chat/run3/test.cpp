#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/format-inl.h>
#include <string>
#include <vector>

// Test that the transcode loop correctly handles empty strings
TEST(TranscodeLoopTest, EmptyString) {
    std::string empty;
    EXPECT_NO_THROW(fmt::detail::write_escaped_string<char>(fmt::memory_buffer(), empty));
    fmt::memory_buffer buf;
    fmt::detail::write_escaped_string<char>(buf, empty);
    EXPECT_EQ(buf.size(), 0);
}

// Test that the transcode loop correctly handles ASCII strings (no multi-byte characters)
TEST(TranscodeLoopTest, AsciiString) {
    std::string ascii = "Hello, World!";
    fmt::memory_buffer buf;
    fmt::detail::write_escaped_string<char>(buf, ascii);
    std::string result(buf.data(), buf.size());
    // For ASCII, the output should be the same as input (no escaping needed)
    EXPECT_EQ(result, ascii);
}

// Test that the transcode loop correctly handles strings with multi-byte UTF-8 characters
TEST(TranscodeLoopTest, MultiByteUtf8) {
    // String with 2-byte, 3-byte, and 4-byte UTF-8 characters
    std::string utf8 = "A\u00E9\u20AC\U0001F600"; // A, é, €, 😀
    fmt::memory_buffer buf;
    fmt::detail::write_escaped_string<char>(buf, utf8);
    std::string result(buf.data(), buf.size());
    // The output should be the same as input (no escaping needed for valid UTF-8)
    EXPECT_EQ(result, utf8);
}

// Test that the transcode loop correctly handles strings that need escaping
TEST(TranscodeLoopTest, EscapedCharacters) {
    std::string input = "Hello\nWorld\tTab";
    fmt::memory_buffer buf;
    fmt::detail::write_escaped_string<char>(buf, input);
    std::string result(buf.data(), buf.size());
    // The output should contain escape sequences
    EXPECT_NE(result, input);
    EXPECT_TRUE(result.find("\\n") != std::string::npos);
    EXPECT_TRUE(result.find("\\t") != std::string::npos);
}

// Test that the transcode loop correctly handles strings at the boundary of block_size
TEST(TranscodeLoopTest, BoundaryBlockSize) {
    // Create a string of exactly block_size (16) bytes
    std::string boundary(16, 'A');
    fmt::memory_buffer buf;
    fmt::detail::write_escaped_string<char>(buf, boundary);
    std::string result(buf.data(), buf.size());
    EXPECT_EQ(result, boundary);
}

// Test that the transcode loop correctly handles strings just below block_size
TEST(TranscodeLoopTest, JustBelowBlockSize) {
    std::string below(15, 'B');
    fmt::memory_buffer buf;
    fmt::detail::write_escaped_string<char>(buf, below);
    std::string result(buf.data(), buf.size());
    EXPECT_EQ(result, below);
}

// Test that the transcode loop correctly handles strings just above block_size
TEST(TranscodeLoopTest, JustAboveBlockSize) {
    std::string above(17, 'C');
    fmt::memory_buffer buf;
    fmt::detail::write_escaped_string<char>(buf, above);
    std::string result(buf.data(), buf.size());
    EXPECT_EQ(result, above);
}

// Test that the transcode loop correctly handles strings with multi-byte characters at block boundaries
TEST(TranscodeLoopTest, MultiByteAtBoundary) {
    // Create a string where a multi-byte character spans across block_size boundary
    std::string prefix(14, 'X');
    std::string multi_byte = "\U0001F600"; // 4-byte character
    std::string input = prefix + multi_byte;
    fmt::memory_buffer buf;
    fmt::detail::write_escaped_string<char>(buf, input);
    std::string result(buf.data(), buf.size());
    EXPECT_EQ(result, input);
}

// Test that the transcode loop correctly handles strings with multiple block_size multiples
TEST(TranscodeLoopTest, MultipleBlockSize) {
    std::string multiple(32, 'D'); // Exactly 2 * block_size
    fmt::memory_buffer buf;
    fmt::detail::write_escaped_string<char>(buf, multiple);
    std::string result(buf.data(), buf.size());
    EXPECT_EQ(result, multiple);
}

// Test that the transcode loop correctly handles strings with mixed ASCII and multi-byte characters
TEST(TranscodeLoopTest, MixedAsciiAndMultiByte) {
    std::string mixed = "Hello \u00E9\u20AC World \U0001F600!";
    fmt::memory_buffer buf;
    fmt::detail::write_escaped_string<char>(buf, mixed);
    std::string result(buf.data(), buf.size());
    EXPECT_EQ(result, mixed);
}

// Test that the transcode loop correctly handles strings with only multi-byte characters
TEST(TranscodeLoopTest, OnlyMultiByte) {
    std::string only_multi = "\u00E9\u20AC\U0001F600\u00E9";
    fmt::memory_buffer buf;
    fmt::detail::write_escaped_string<char>(buf, only_multi);
    std::string result(buf.data(), buf.size());
    EXPECT_EQ(result, only_multi);
}

// Test that the transcode loop correctly handles strings with special characters that need escaping
// and multi-byte characters
TEST(TranscodeLoopTest, EscapedAndMultiByte) {
    std::string input = "Hello\n\u00E9\tWorld\U0001F600";
    fmt::memory_buffer buf;
    fmt::detail::write_escaped_string<char>(buf, input);
    std::string result(buf.data(), buf.size());
    EXPECT_NE(result, input);
    EXPECT_TRUE(result.find("\\n") != std::string::npos);
    EXPECT_TRUE(result.find("\\t") != std::string::npos);
    EXPECT_TRUE(result.find("\u00E9") != std::string::npos);
    EXPECT_TRUE(result.find("\U0001F600") != std::string::npos);
}

// Test that the transcode loop correctly handles very long strings
TEST(TranscodeLoopTest, VeryLongString) {
    std::string long_str(1000, 'E');
    fmt::memory_buffer buf;
    fmt::detail::write_escaped_string<char>(buf, long_str);
    std::string result(buf.data(), buf.size());
    EXPECT_EQ(result, long_str);
}

// Test that the transcode loop correctly handles strings with null characters
TEST(TranscodeLoopTest, NullCharacter) {
    std::string with_null = "Hello\0World";
    with_null.resize(11); // Include the null character
    fmt::memory_buffer buf;
    fmt::detail::write_escaped_string<char>(buf, with_null);
    std::string result(buf.data(), buf.size());
    EXPECT_NE(result, "HelloWorld");
    EXPECT_TRUE(result.find("\\x00") != std::string::npos || result.find("\\0") != std::string::npos);
}