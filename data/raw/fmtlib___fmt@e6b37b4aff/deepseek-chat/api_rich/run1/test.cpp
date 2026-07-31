#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <cstddef>

// Test that format handles various Unicode sequences correctly, exercising the transcode logic
TEST(FormatTranscodeTest, BasicAscii) {
    std::string result = fmt::format("Hello {}", "world");
    EXPECT_EQ(result, "Hello world");
}

TEST(FormatTranscodeTest, UnicodeCharacters) {
    std::string result = fmt::format("{}", "café");
    EXPECT_EQ(result, "café");
}

TEST(FormatTranscodeTest, MixedAsciiAndUnicode) {
    std::string result = fmt::format("{} {}", "héllo", "wörld");
    EXPECT_EQ(result, "héllo wörld");
}

TEST(FormatTranscodeTest, LongUnicodeString) {
    std::string input = "a" + std::string(1000, 'é') + "b";
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(result, input);
}

TEST(FormatTranscodeTest, EmptyString) {
    std::string result = fmt::format("{}", "");
    EXPECT_EQ(result, "");
}

TEST(FormatTranscodeTest, SingleCharacter) {
    std::string result = fmt::format("{}", "a");
    EXPECT_EQ(result, "a");
}

TEST(FormatTranscodeTest, SingleUnicodeCharacter) {
    std::string result = fmt::format("{}", "é");
    EXPECT_EQ(result, "é");
}

TEST(FormatTranscodeTest, FormatToWithUnicode) {
    std::string out;
    auto it = fmt::format_to(std::back_inserter(out), "{}", "héllo");
    EXPECT_EQ(out, "héllo");
    EXPECT_EQ(it - out.begin(), static_cast<std::ptrdiff_t>(out.size()));
}

TEST(FormatTranscodeTest, FormatToNWithUnicode) {
    std::string out(10, '\0');
    auto result = fmt::format_to_n(out.begin(), 5, "{}", "héllo world");
    EXPECT_EQ(result.size, 5);
    EXPECT_EQ(std::string(out.begin(), result.out), "héllo");
}

TEST(FormatTranscodeTest, FormattedSizeWithUnicode) {
    size_t size = fmt::formatted_size("{}", "café");
    EXPECT_EQ(size, 5); // 5 characters: c, a, f, é
}

TEST(FormatTranscodeTest, MultipleUnicodeArguments) {
    std::string result = fmt::format("{} {} {}", "héllo", "wörld", "café");
    EXPECT_EQ(result, "héllo wörld café");
}

TEST(FormatTranscodeTest, UnicodeInFormatString) {
    std::string result = fmt::format("héllo {}", "world");
    EXPECT_EQ(result, "héllo world");
}

TEST(FormatTranscodeTest, VeryLongUnicodeString) {
    std::string input;
    for (int i = 0; i < 10000; ++i) {
        input += "é";
    }
    std::string result = fmt::format("{}", input);
    EXPECT_EQ(result, input);
}

TEST(FormatTranscodeTest, UnicodeWithSpecialCharacters) {
    std::string result = fmt::format("{}", "héllo\nwörld\tcafé");
    EXPECT_EQ(result, "héllo\nwörld\tcafé");
}

TEST(FormatTranscodeTest, UnicodePadding) {
    std::string result = fmt::format("{:10}", "café");
    EXPECT_EQ(result, "café      ");
}

TEST(FormatTranscodeTest, UnicodeAlignment) {
    std::string result = fmt::format("{:>10}", "café");
    EXPECT_EQ(result, "      café");
}

TEST(FormatTranscodeTest, UnicodeFillAndAlign) {
    std::string result = fmt::format("{:*^10}", "café");
    EXPECT_EQ(result, "***café***");
}

TEST(FormatTranscodeTest, UnicodePrecision) {
    std::string result = fmt::format("{:.3}", "café");
    EXPECT_EQ(result, "caf");
}

TEST(FormatTranscodeTest, UnicodeWidth) {
    std::string result = fmt::format("{:5}", "café");
    EXPECT_EQ(result, "café ");
}

TEST(FormatTranscodeTest, UnicodeWithNumbers) {
    std::string result = fmt::format("{} {}", 42, "café");
    EXPECT_EQ(result, "42 café");
}

TEST(FormatTranscodeTest, UnicodeWithFloats) {
    std::string result = fmt::format("{} {}", 3.14, "café");
    EXPECT_EQ(result, "3.14 café");
}

TEST(FormatTranscodeTest, UnicodeWithMultipleFormats) {
    std::string result = fmt::format("{:>10} {:10} {:*^10}", "héllo", "wörld", "café");
    EXPECT_EQ(result, "    héllo wörld    ***café***");
}

TEST(FormatTranscodeTest, UnicodeInVector) {
    std::vector<std::string> vec = {"héllo", "wörld", "café"};
    std::string result;
    for (const auto& s : vec) {
        result += fmt::format("{} ", s);
    }
    EXPECT_EQ(result, "héllo wörld café ");
}

TEST(FormatTranscodeTest, UnicodeWithEscapedBraces) {
    std::string result = fmt::format("{{}} {}", "café");
    EXPECT_EQ(result, "{} café");
}

TEST(FormatTranscodeTest, UnicodeWithNestedFormat) {
    std::string inner = fmt::format("{}", "héllo");
    std::string result = fmt::format("{} {}", inner, "wörld");
    EXPECT_EQ(result, "héllo wörld");
}