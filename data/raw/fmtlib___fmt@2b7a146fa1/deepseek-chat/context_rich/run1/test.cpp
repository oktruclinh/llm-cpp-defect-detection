#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <limits>
#include <cstdint>

// Test formatting integers with various format specifications
TEST(FormatIntTest, DecimalFormatting) {
    // Basic decimal formatting
    EXPECT_EQ(fmt::format("{}", 42), "42");
    EXPECT_EQ(fmt::format("{}", -42), "-42");
    EXPECT_EQ(fmt::format("{}", 0), "0");
    
    // Width and fill
    EXPECT_EQ(fmt::format("{:5}", 42), "   42");
    EXPECT_EQ(fmt::format("{:5}", -42), "  -42");
    EXPECT_EQ(fmt::format("{:<5}", 42), "42   ");
    EXPECT_EQ(fmt::format("{:>5}", 42), "   42");
    EXPECT_EQ(fmt::format("{:^5}", 42), " 42  ");
    
    // Sign handling
    EXPECT_EQ(fmt::format("{:+}", 42), "+42");
    EXPECT_EQ(fmt::format("{:+}", -42), "-42");
    EXPECT_EQ(fmt::format("{: }", 42), " 42");
    EXPECT_EQ(fmt::format("{: }", -42), "-42");
    EXPECT_EQ(fmt::format("{:-}", 42), "42");
    EXPECT_EQ(fmt::format("{:-}", -42), "-42");
}

TEST(FormatIntTest, HexFormatting) {
    // Basic hex
    EXPECT_EQ(fmt::format("{:x}", 255), "ff");
    EXPECT_EQ(fmt::format("{:X}", 255), "FF");
    EXPECT_EQ(fmt::format("{:x}", 0), "0");
    
    // Alternative form with 0x prefix
    EXPECT_EQ(fmt::format("{:#x}", 255), "0xff");
    EXPECT_EQ(fmt::format("{:#X}", 255), "0XFF");
    
    // Width and fill with hex
    EXPECT_EQ(fmt::format("{:8x}", 255), "      ff");
    EXPECT_EQ(fmt::format("{:#8x}", 255), "    0xff");
    
    // Negative numbers in hex
    EXPECT_EQ(fmt::format("{:x}", -1), "ffffffff");
}

TEST(FormatIntTest, OctalFormatting) {
    // Basic octal
    EXPECT_EQ(fmt::format("{:o}", 8), "10");
    EXPECT_EQ(fmt::format("{:o}", 0), "0");
    
    // Alternative form with 0 prefix
    EXPECT_EQ(fmt::format("{:#o}", 8), "010");
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");
    
    // Width
    EXPECT_EQ(fmt::format("{:6o}", 8), "    10");
    EXPECT_EQ(fmt::format("{:#6o}", 8), "   010");
}

TEST(FormatIntTest, BinaryFormatting) {
    // Basic binary
    EXPECT_EQ(fmt::format("{:b}", 5), "101");
    EXPECT_EQ(fmt::format("{:b}", 0), "0");
    
    // Alternative form with 0b prefix
    EXPECT_EQ(fmt::format("{:#b}", 5), "0b101");
    EXPECT_EQ(fmt::format("{:#B}", 5), "0B101");
    
    // Width
    EXPECT_EQ(fmt::format("{:8b}", 5), "     101");
    EXPECT_EQ(fmt::format("{:#8b}", 5), "   0b101");
}

TEST(FormatIntTest, PrecisionAndZeroPadding) {
    // Zero padding
    EXPECT_EQ(fmt::format("{:05}", 42), "00042");
    EXPECT_EQ(fmt::format("{:05}", -42), "-0042");
    
    // Precision for integers (treated as minimum digits)
    EXPECT_EQ(fmt::format("{:.5}", 42), "00042");
    EXPECT_EQ(fmt::format("{:.5}", -42), "-00042");
    
    // Combined width and precision
    EXPECT_EQ(fmt::format("{:8.5}", 42), "   00042");
    EXPECT_EQ(fmt::format("{:08.5}", 42), "   00042");
}

TEST(FormatIntTest, LargeIntegers) {
    // Maximum and minimum values
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::max()), "2147483647");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::min()), "-2147483648");
    
    // 64-bit integers
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int64_t>::max()), "9223372036854775807");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int64_t>::min()), "-9223372036854775808");
    
    // Unsigned integers
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<uint32_t>::max()), "4294967295");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<uint64_t>::max()), "18446744073709551615");
}

TEST(FormatIntTest, LocaleGrouping) {
    // With 'L' option (locale-specific grouping)
    // Note: This test assumes default locale uses no grouping or comma
    // The actual behavior depends on the locale
    EXPECT_EQ(fmt::format("{:L}", 1234567), "1,234,567");
    EXPECT_EQ(fmt::format("{:L}", -1234567), "-1,234,567");
    EXPECT_EQ(fmt::format("{:L}", 0), "0");
}

TEST(FormatIntTest, MixedFormatSpecifiers) {
    // Combining multiple specifiers
    EXPECT_EQ(fmt::format("{:+08x}", 255), "+0000ff");
    EXPECT_EQ(fmt::format("{:#010x}", 255), "0x000000ff");
    EXPECT_EQ(fmt::format("{:+#10x}", 255), "    +0xff");
    
    // Fill character
    EXPECT_EQ(fmt::format("{:*<10}", 42), "42********");
    EXPECT_EQ(fmt::format("{:*>10}", 42), "********42");
    EXPECT_EQ(fmt::format("{:*^10}", 42), "****42****");
}

TEST(FormatIntTest, CharTypeFormatting) {
    // Formatting as character
    EXPECT_EQ(fmt::format("{:c}", 65), "A");
    EXPECT_EQ(fmt::format("{:c}", 0x41), "A");
    
    // Width with character
    EXPECT_EQ(fmt::format("{:5c}", 65), "    A");
}

TEST(FormatIntTest, EdgeCases) {
    // Zero with various formats
    EXPECT_EQ(fmt::format("{:#x}", 0), "0");
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");
    EXPECT_EQ(fmt::format("{:#b}", 0), "0");
    
    // One
    EXPECT_EQ(fmt::format("{:#x}", 1), "0x1");
    EXPECT_EQ(fmt::format("{:#o}", 1), "01");
    EXPECT_EQ(fmt::format("{:#b}", 1), "0b1");
    
    // Negative zero (should be treated as zero)
    EXPECT_EQ(fmt::format("{:+}", -0), "+0");
    EXPECT_EQ(fmt::format("{: }", -0), " 0");
}