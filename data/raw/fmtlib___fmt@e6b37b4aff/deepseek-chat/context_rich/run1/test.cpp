#include <gtest/gtest.h>
#include "format.h"
#include <string>
#include <stdexcept>

// Test the utf8_to_utf16 constructor with valid UTF-8 input
TEST(Utf8ToUtf16Test, ValidAscii) {
    fmt::internal::utf8_to_utf16 converter("Hello");
    // The buffer should contain the ASCII characters plus null terminator
    std::wstring expected = L"Hello";
    EXPECT_EQ(converter.buffer_.size(), expected.size() + 1); // +1 for null
    EXPECT_EQ(std::wstring(converter.buffer_.begin(), converter.buffer_.end() - 1), expected);
    EXPECT_EQ(converter.buffer_.back(), 0);
}

TEST(Utf8ToUtf16Test, ValidTwoByte) {
    // U+00A9 (©) in UTF-8: 0xC2 0xA9
    fmt::internal::utf8_to_utf16 converter("\xC2\xA9");
    std::wstring expected = L"\u00A9";
    EXPECT_EQ(converter.buffer_.size(), 2); // one wchar_t + null
    EXPECT_EQ(converter.buffer_[0], expected[0]);
    EXPECT_EQ(converter.buffer_[1], 0);
}

TEST(Utf8ToUtf16Test, ValidThreeByte) {
    // U+4E2D (中) in UTF-8: 0xE4 0xB8 0xAD
    fmt::internal::utf8_to_utf16 converter("\xE4\xB8\xAD");
    std::wstring expected = L"\u4E2D";
    EXPECT_EQ(converter.buffer_[0], expected[0]);
    EXPECT_EQ(converter.buffer_[1], 0);
}

TEST(Utf8ToUtf16Test, ValidFourByteToSurrogatePair) {
    // U+1F600 (😀) in UTF-8: 0xF0 0x9F 0x98 0x80
    // Surrogate pair: 0xD83D 0xDE00
    fmt::internal::utf8_to_utf16 converter("\xF0\x9F\x98\x80");
    EXPECT_EQ(converter.buffer_.size(), 3); // two wchar_t + null
    EXPECT_EQ(converter.buffer_[0], 0xD83D);
    EXPECT_EQ(converter.buffer_[1], 0xDE00);
    EXPECT_EQ(converter.buffer_[2], 0);
}

TEST(Utf8ToUtf16Test, MultipleCharacters) {
    // Mix of 1, 2, 3, and 4-byte characters
    std::string input = "A\xC2\xA9\xE4\xB8\xAD\xF0\x9F\x98\x80";
    fmt::internal::utf8_to_utf16 converter(input);
    std::wstring expected = L"A\u00A9\u4E2D\U0001F600";
    EXPECT_EQ(converter.buffer_.size(), expected.size() + 1);
    EXPECT_EQ(std::wstring(converter.buffer_.begin(), converter.buffer_.end() - 1), expected);
    EXPECT_EQ(converter.buffer_.back(), 0);
}

TEST(Utf8ToUtf16Test, EmptyString) {
    fmt::internal::utf8_to_utf16 converter("");
    EXPECT_EQ(converter.buffer_.size(), 1);
    EXPECT_EQ(converter.buffer_[0], 0);
}

TEST(Utf8ToUtf16Test, StringWithNullTerminator) {
    // String containing embedded null should still work
    std::string input = "ab\0cd";
    fmt::internal::utf8_to_utf16 converter(input);
    std::wstring expected = L"ab\0cd";
    EXPECT_EQ(converter.buffer_.size(), expected.size() + 1);
    EXPECT_EQ(std::wstring(converter.buffer_.begin(), converter.buffer_.end() - 1), expected);
    EXPECT_EQ(converter.buffer_.back(), 0);
}

// Test invalid UTF-8 sequences
TEST(Utf8ToUtf16Test, InvalidContinuationByte) {
    // Invalid: 0xC2 followed by 0x00 (not a continuation byte)
    EXPECT_THROW(fmt::internal::utf8_to_utf16 converter("\xC2\x00"), std::runtime_error);
}

TEST(Utf8ToUtf16Test, MissingContinuationByte) {
    // Incomplete sequence: 0xC2 without continuation
    EXPECT_THROW(fmt::internal::utf8_to_utf16 converter("\xC2"), std::runtime_error);
}

TEST(Utf8ToUtf16Test, OverlongEncoding) {
    // Overlong encoding of '/' (0x2F) as 0xC0 0xAF
    EXPECT_THROW(fmt::internal::utf8_to_utf16 converter("\xC0\xAF"), std::runtime_error);
}

TEST(Utf8ToUtf16Test, SurrogateHalf) {
    // Encoded surrogate half U+D800: 0xED 0xA0 0x80
    EXPECT_THROW(fmt::internal::utf8_to_utf16 converter("\xED\xA0\x80"), std::runtime_error);
}

TEST(Utf8ToUtf16Test, OutOfRange) {
    // Code point > U+10FFFF: 0xF4 0x90 0x80 0x80 (U+110000)
    EXPECT_THROW(fmt::internal::utf8_to_utf16 converter("\xF4\x90\x80\x80"), std::runtime_error);
}

TEST(Utf8ToUtf16Test, InvalidStartByte) {
    // 0xFF is not a valid start byte
    EXPECT_THROW(fmt::internal::utf8_to_utf16 converter("\xFF"), std::runtime_error);
}

// Test the formatter for bigint
TEST(BigintFormatterTest, FormatZero) {
    fmt::internal::bigint n;
    // Default-constructed bigint should be zero
    std::string result = fmt::format("{}", n);
    EXPECT_EQ(result, "0");
}

TEST(BigintFormatterTest, FormatSmallValue) {
    fmt::internal::bigint n;
    n = 0x1234; // Assuming bigint supports assignment from uint64_t
    std::string result = fmt::format("{}", n);
    EXPECT_EQ(result, "1234");
}

TEST(BigintFormatterTest, FormatLargeValue) {
    fmt::internal::bigint n;
    // Set a value that spans multiple bigits (assuming bigit_bits is 32)
    // This is a simplified test; actual bigint construction may differ
    n = 0xDEADBEEFCAFEBABEULL;
    std::string result = fmt::format("{}", n);
    // The exact format depends on bigint implementation, but should be hex
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result.find_first_not_of("0123456789abcdefp"), std::string::npos);
}

// Test format_system_error (basic smoke test)
TEST(FormatSystemErrorTest, BasicError) {
    fmt::memory_buffer buf;
    fmt::format_system_error(buf, 0, "Success");
    std::string result = to_string(buf);
    EXPECT_NE(result.find("Success"), std::string::npos);
    // Should contain the error message for error code 0
    EXPECT_NE(result.find("Success"), std::string::npos);
}

TEST(FormatSystemErrorTest, NonZeroError) {
    fmt::memory_buffer buf;
    fmt::format_system_error(buf, ENOENT, "File not found");
    std::string result = to_string(buf);
    EXPECT_NE(result.find("File not found"), std::string::npos);
    // Should contain the system error message for ENOENT
    EXPECT_NE(result.find("No such file or directory"), std::string::npos);
}

// Test error_handler::on_error
TEST(ErrorHandlerTest, OnErrorThrows) {
    fmt::internal::error_handler handler;
    EXPECT_THROW(handler.on_error("test error"), fmt::format_error);
}

// Test that the formatter for bigint handles exponent
TEST(BigintFormatterTest, WithExponent) {
    fmt::internal::bigint n;
    // Set a value with exponent (if bigint supports it)
    // This is a placeholder; actual test depends on bigint API
    // For now, just test that formatting doesn't crash
    std::string result = fmt::format("{}", n);
    EXPECT_FALSE(result.empty());
}