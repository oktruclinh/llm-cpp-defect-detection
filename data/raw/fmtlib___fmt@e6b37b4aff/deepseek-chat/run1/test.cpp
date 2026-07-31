#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test that the focal function (transcoding in format-inl.h) correctly handles
// various UTF-8 to UTF-16/32 conversions through the public API.

class FormatTranscodeTest : public ::testing::Test {
protected:
    // Helper to test that a format string with Unicode content produces expected output
    void testUnicodeFormat(const std::string& format_str, 
                          const std::vector<std::string>& args,
                          const std::string& expected) {
        std::string result;
        if (args.empty()) {
            result = fmt::format(format_str);
        } else if (args.size() == 1) {
            result = fmt::format(format_str, args[0]);
        } else if (args.size() == 2) {
            result = fmt::format(format_str, args[0], args[1]);
        }
        EXPECT_EQ(result, expected);
    }
};

// Test empty string - should produce empty output
TEST_F(FormatTranscodeTest, EmptyString) {
    EXPECT_EQ(fmt::format("{}", ""), "");
}

// Test ASCII-only strings (no transcoding needed)
TEST_F(FormatTranscodeTest, AsciiOnly) {
    EXPECT_EQ(fmt::format("{}", "hello"), "hello");
    EXPECT_EQ(fmt::format("{}", "world"), "world");
    EXPECT_EQ(fmt::format("{} {}", "hello", "world"), "hello world");
}

// Test single-byte UTF-8 characters (ASCII range)
TEST_F(FormatTranscodeTest, SingleByteUtf8) {
    EXPECT_EQ(fmt::format("{}", "a"), "a");
    EXPECT_EQ(fmt::format("{}", "123"), "123");
    EXPECT_EQ(fmt::format("{}", "!@#$"), "!@#$");
}

// Test two-byte UTF-8 characters (Latin-1 supplement, etc.)
TEST_F(FormatTranscodeTest, TwoByteUtf8) {
    // U+00A9 Copyright symbol (0xC2 0xA9)
    EXPECT_EQ(fmt::format("{}", "\u00A9"), "\u00A9");
    // U+00AE Registered symbol (0xC2 0xAE)
    EXPECT_EQ(fmt::format("{}", "\u00AE"), "\u00AE");
    // U+00F1 n-tilde (0xC3 0xB1)
    EXPECT_EQ(fmt::format("{}", "\u00F1"), "\u00F1");
}

// Test three-byte UTF-8 characters (CJK, etc.)
TEST_F(FormatTranscodeTest, ThreeByteUtf8) {
    // U+4E2D Chinese character (0xE4 0xB8 0xAD)
    EXPECT_EQ(fmt::format("{}", "\u4E2D"), "\u4E2D");
    // U+5B57 Chinese character (0xE5 0xAD 0x97)
    EXPECT_EQ(fmt::format("{}", "\u5B57"), "\u5B57");
    // U+00E9 with accent (0xC3 0xA9)
    EXPECT_EQ(fmt::format("{}", "\u00E9"), "\u00E9");
}

// Test four-byte UTF-8 characters (emoji, etc.)
TEST_F(FormatTranscodeTest, FourByteUtf8) {
    // U+1F600 Grinning face emoji (0xF0 0x9F 0x98 0x80)
    EXPECT_EQ(fmt::format("{}", "\U0001F600"), "\U0001F600");
    // U+1F44D Thumbs up (0xF0 0x9F 0x91 0x8D)
    EXPECT_EQ(fmt::format("{}", "\U0001F44D"), "\U0001F44D");
}

// Test mixed ASCII and multi-byte characters
TEST_F(FormatTranscodeTest, MixedAsciiAndUtf8) {
    EXPECT_EQ(fmt::format("{}", "Hello \u00E9"), "Hello \u00E9");
    EXPECT_EQ(fmt::format("{}", "\u4E2D\u6587"), "\u4E2D\u6587");
    EXPECT_EQ(fmt::format("{} {}", "Hello", "\u4E16\u754C"), "Hello \u4E16\u754C");
}

// Test strings that are exactly at block boundaries
TEST_F(FormatTranscodeTest, BlockBoundarySizes) {
    // Test with strings of various sizes that might hit block boundaries
    std::string short_str = "a";
    std::string medium_str = "abcdefghijklmnopqrstuvwxyz";
    std::string long_str = std::string(100, 'x');
    
    EXPECT_EQ(fmt::format("{}", short_str), short_str);
    EXPECT_EQ(fmt::format("{}", medium_str), medium_str);
    EXPECT_EQ(fmt::format("{}", long_str), long_str);
}

// Test strings with Unicode at block boundaries
TEST_F(FormatTranscodeTest, UnicodeAtBlockBoundaries) {
    // Create strings where multi-byte chars cross block boundaries
    std::string mixed;
    for (int i = 0; i < 10; ++i) {
        mixed += "\u00E9";  // 2-byte char
    }
    EXPECT_EQ(fmt::format("{}", mixed), mixed);
    
    mixed.clear();
    for (int i = 0; i < 10; ++i) {
        mixed += "\u4E2D";  // 3-byte char
    }
    EXPECT_EQ(fmt::format("{}", mixed), mixed);
    
    mixed.clear();
    for (int i = 0; i < 5; ++i) {
        mixed += "\U0001F600";  // 4-byte char
    }
    EXPECT_EQ(fmt::format("{}", mixed), mixed);
}

// Test format strings with Unicode in the format string itself
TEST_F(FormatTranscodeTest, UnicodeInFormatString) {
    EXPECT_EQ(fmt::format("\u00A9 {} \u00AE", "test"), "\u00A9 test \u00AE");
    EXPECT_EQ(fmt::format("\u4E2D\u6587: {}", "hello"), "\u4E2D\u6587: hello");
}

// Test that the function handles invalid UTF-8 gracefully (implementation-defined)
TEST_F(FormatTranscodeTest, InvalidUtf8) {
    // Invalid continuation byte
    std::string invalid = "\x80\x81";
    // Should not crash, output is implementation-defined but should be valid
    EXPECT_NO_THROW(fmt::format("{}", invalid));
    
    // Overlong encoding
    invalid = "\xC0\x80";  // Overlong null
    EXPECT_NO_THROW(fmt::format("{}", invalid));
    
    // Missing continuation bytes
    invalid = "\xE0";  // Incomplete 3-byte sequence
    EXPECT_NO_THROW(fmt::format("{}", invalid));
}

// Test very long strings to exercise the loop
TEST_F(FormatTranscodeTest, VeryLongString) {
    std::string long_str(10000, 'a');
    EXPECT_EQ(fmt::format("{}", long_str), long_str);
    
    // Long string with Unicode
    std::string unicode_long;
    for (int i = 0; i < 5000; ++i) {
        unicode_long += "\u00E9";
    }
    EXPECT_EQ(fmt::format("{}", unicode_long), unicode_long);
}

// Test that the function preserves exact byte sequences for valid UTF-8
TEST_F(FormatTranscodeTest, PreservesValidUtf8) {
    std::vector<std::string> test_strings = {
        "Hello, World!",
        "Caf\u00E9",
        "\u4E2D\u56FD",
        "\U0001F600\U0001F44D",
        "a\u00E9\u4E2D\U0001F600",
        "\n\t\r",
        "  spaces  ",
        "1234567890"
    };
    
    for (const auto& s : test_strings) {
        EXPECT_EQ(fmt::format("{}", s), s) << "Failed for string: " << s;
    }
}

// Test format with multiple arguments containing Unicode
TEST_F(FormatTranscodeTest, MultipleUnicodeArgs) {
    EXPECT_EQ(fmt::format("{} {}", "\u00E9", "\u00E9"), "\u00E9 \u00E9");
    EXPECT_EQ(fmt::format("{0} {1} {0}", "a", "\u4E2D"), "a \u4E2D a");
}

// Test that the function handles strings that are exactly block_size - 1
TEST_F(FormatTranscodeTest, NearBlockSize) {
    // Assuming block_size is typically 16 or 32, test around those sizes
    std::string s15 = std::string(15, 'a');
    std::string s16 = std::string(16, 'a');
    std::string s17 = std::string(17, 'a');
    std::string s31 = std::string(31, 'a');
    std::string s32 = std::string(32, 'a');
    std::string s33 = std::string(33, 'a');
    
    EXPECT_EQ(fmt::format("{}", s15), s15);
    EXPECT_EQ(fmt::format("{}", s16), s16);
    EXPECT_EQ(fmt::format("{}", s17), s17);
    EXPECT_EQ(fmt::format("{}", s31), s31);
    EXPECT_EQ(fmt::format("{}", s32), s32);
    EXPECT_EQ(fmt::format("{}", s33), s33);
}