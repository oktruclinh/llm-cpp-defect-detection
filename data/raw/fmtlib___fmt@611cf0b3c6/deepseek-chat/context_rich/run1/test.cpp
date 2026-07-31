#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <limits>
#include <cstdint>

// Test formatting of integers with various format specifications
TEST(IntWriterTest, BasicDecimal) {
    EXPECT_EQ(fmt::format("{}", 42), "42");
    EXPECT_EQ(fmt::format("{}", 0), "0");
    EXPECT_EQ(fmt::format("{}", -42), "-42");
}

TEST(IntWriterTest, SignSpecifiers) {
    EXPECT_EQ(fmt::format("{:+}", 42), "+42");
    EXPECT_EQ(fmt::format("{:+}", -42), "-42");
    EXPECT_EQ(fmt::format("{: }", 42), " 42");
    EXPECT_EQ(fmt::format("{: }", -42), "-42");
    EXPECT_EQ(fmt::format("{:-}", 42), "42");
    EXPECT_EQ(fmt::format("{:-}", -42), "-42");
}

TEST(IntWriterTest, HexFormat) {
    EXPECT_EQ(fmt::format("{:x}", 255), "ff");
    EXPECT_EQ(fmt::format("{:X}", 255), "FF");
    EXPECT_EQ(fmt::format("{:#x}", 255), "0xff");
    EXPECT_EQ(fmt::format("{:#X}", 255), "0XFF");
    EXPECT_EQ(fmt::format("{:x}", 0), "0");
    EXPECT_EQ(fmt::format("{:#x}", 0), "0");
}

TEST(IntWriterTest, OctalFormat) {
    EXPECT_EQ(fmt::format("{:o}", 8), "10");
    EXPECT_EQ(fmt::format("{:o}", 0), "0");
    EXPECT_EQ(fmt::format("{:#o}", 8), "010");
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");
}

TEST(IntWriterTest, BinaryFormat) {
    EXPECT_EQ(fmt::format("{:b}", 5), "101");
    EXPECT_EQ(fmt::format("{:#b}", 5), "0b101");
    EXPECT_EQ(fmt::format("{:#B}", 5), "0B101");
    EXPECT_EQ(fmt::format("{:b}", 0), "0");
}

TEST(IntWriterTest, WidthAndFill) {
    EXPECT_EQ(fmt::format("{:5}", 42), "   42");
    EXPECT_EQ(fmt::format("{:5}", -42), "  -42");
    EXPECT_EQ(fmt::format("{:<5}", 42), "42   ");
    EXPECT_EQ(fmt::format("{:>5}", 42), "   42");
    EXPECT_EQ(fmt::format("{:^5}", 42), " 42  ");
    EXPECT_EQ(fmt::format("{:*^5}", 42), "**42*");
}

TEST(IntWriterTest, PrecisionForIntegers) {
    EXPECT_EQ(fmt::format("{:.5}", 42), "00042");
    EXPECT_EQ(fmt::format("{:.5}", -42), "-00042");
    EXPECT_EQ(fmt::format("{:.0}", 0), "");
    EXPECT_EQ(fmt::format("{:.3}", 0), "000");
}

TEST(IntWriterTest, ZeroFlag) {
    EXPECT_EQ(fmt::format("{:05}", 42), "00042");
    EXPECT_EQ(fmt::format("{:05}", -42), "-0042");
    EXPECT_EQ(fmt::format("{:+05}", 42), "+0042");
}

TEST(IntWriterTest, ThousandsSeparator) {
    // Note: Locale-dependent, but default should be no separator
    EXPECT_EQ(fmt::format("{:n}", 1234567), "1,234,567");
    EXPECT_EQ(fmt::format("{:n}", 42), "42");
    EXPECT_EQ(fmt::format("{:n}", -1234567), "-1,234,567");
}

TEST(IntWriterTest, EdgeCases) {
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::max()), "2147483647");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::min()), "-2147483648");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<unsigned>::max()), "4294967295");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<long long>::max()), "9223372036854775807");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<long long>::min()), "-9223372036854775808");
}

TEST(IntWriterTest, CombinedFlags) {
    EXPECT_EQ(fmt::format("{:#010x}", 255), "0x000000ff");
    EXPECT_EQ(fmt::format("{:#010X}", 255), "0X000000FF");
    EXPECT_EQ(fmt::format("{:+010d}", 42), "+000000042");
    EXPECT_EQ(fmt::format("{:+#010x}", 255), "+0x00000ff");
}

TEST(IntWriterTest, CharType) {
    EXPECT_EQ(fmt::format("{:c}", 65), "A");
    EXPECT_EQ(fmt::format("{:c}", 0x20), " ");
    EXPECT_EQ(fmt::format("{:c}", 0x7F), "\x7F");
}

TEST(IntWriterTest, BoolFormat) {
    EXPECT_EQ(fmt::format("{}", true), "true");
    EXPECT_EQ(fmt::format("{}", false), "false");
    EXPECT_EQ(fmt::format("{:d}", true), "1");
    EXPECT_EQ(fmt::format("{:d}", false), "0");
    EXPECT_EQ(fmt::format("{:s}", true), "true");
    EXPECT_EQ(fmt::format("{:s}", false), "false");
}

TEST(IntWriterTest, NegativeZero) {
    EXPECT_EQ(fmt::format("{:+}", -0), "+0");
    EXPECT_EQ(fmt::format("{: }", -0), " 0");
    EXPECT_EQ(fmt::format("{:-}", -0), "0");
}

TEST(IntWriterTest, LargeWidth) {
    std::string expected(100, ' ');
    expected += "42";
    EXPECT_EQ(fmt::format("{:>102}", 42), expected);
}

TEST(IntWriterTest, PrecisionWithHex) {
    EXPECT_EQ(fmt::format("{:#010x}", 0), "0x00000000");
    EXPECT_EQ(fmt::format("{:#.8x}", 255), "0x000000ff");
}

TEST(IntWriterTest, AlternateFormWithZero) {
    EXPECT_EQ(fmt::format("{:#x}", 0), "0");
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");
    EXPECT_EQ(fmt::format("{:#b}", 0), "0");
}