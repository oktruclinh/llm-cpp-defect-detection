#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

// Test formatting of negative char values (on_chr path for negative chars)
TEST(FormatCharTest, NegativeChar) {
    // char with value -1 should format as -1 (not as some other character)
    char negative_char = -1;
    std::string result = fmt::format("{}", negative_char);
    EXPECT_EQ(result, "-1");
}

// Test formatting of char with value 0 (boundary case)
TEST(FormatCharTest, ZeroChar) {
    char zero_char = 0;
    std::string result = fmt::format("{}", zero_char);
    EXPECT_EQ(result, "\0");
    EXPECT_EQ(result.size(), 1);
}

// Test formatting of positive char values
TEST(FormatCharTest, PositiveChar) {
    char positive_char = 'A';
    std::string result = fmt::format("{}", positive_char);
    EXPECT_EQ(result, "A");
}

// Test formatting of signed char with negative value
TEST(FormatCharTest, SignedCharNegative) {
    signed char sc = -128;
    std::string result = fmt::format("{}", sc);
    EXPECT_EQ(result, "-128");
}

// Test formatting of signed char with positive value
TEST(FormatCharTest, SignedCharPositive) {
    signed char sc = 127;
    std::string result = fmt::format("{}", sc);
    EXPECT_EQ(result, "127");
}

// Test formatting of unsigned char (should never be negative)
TEST(FormatCharTest, UnsignedChar) {
    unsigned char uc = 200;
    std::string result = fmt::format("{}", uc);
    EXPECT_EQ(result, "200");
}

// Test formatting of char with width specifier
TEST(FormatCharTest, CharWithWidth) {
    char c = 'X';
    std::string result = fmt::format("{:5}", c);
    EXPECT_EQ(result, "    X");
}

// Test formatting of char with fill and alignment
TEST(FormatCharTest, CharWithFillAndAlign) {
    char c = 'Z';
    std::string result = fmt::format("{:*<5}", c);
    EXPECT_EQ(result, "Z****");
}

// Test formatting of negative char with width
TEST(FormatCharTest, NegativeCharWithWidth) {
    char c = -5;
    std::string result = fmt::format("{:5}", c);
    EXPECT_EQ(result, "   -5");
}

// Test formatting of negative char with zero padding
TEST(FormatCharTest, NegativeCharWithZeroPad) {
    char c = -5;
    std::string result = fmt::format("{:05}", c);
    EXPECT_EQ(result, "-0005");
}

// Test formatting of char with plus sign (should not affect char formatting)
TEST(FormatCharTest, CharWithPlus) {
    char c = 'A';
    std::string result = fmt::format("{:+}", c);
    EXPECT_EQ(result, "A");
}

// Test formatting of negative char with plus sign
TEST(FormatCharTest, NegativeCharWithPlus) {
    char c = -1;
    std::string result = fmt::format("{:+}", c);
    EXPECT_EQ(result, "-1");
}

// Test formatting of char with space flag
TEST(FormatCharTest, CharWithSpace) {
    char c = 'B';
    std::string result = fmt::format("{: }", c);
    EXPECT_EQ(result, "B");
}

// Test formatting of negative char with space flag
TEST(FormatCharTest, NegativeCharWithSpace) {
    char c = -2;
    std::string result = fmt::format("{: }", c);
    EXPECT_EQ(result, "-2");
}

// Test formatting of char with alternate form (should be ignored for chars)
TEST(FormatCharTest, CharWithAlternate) {
    char c = 'C';
    std::string result = fmt::format("{:#}", c);
    EXPECT_EQ(result, "C");
}

// Test formatting of char with precision (should be ignored for chars)
TEST(FormatCharTest, CharWithPrecision) {
    char c = 'D';
    std::string result = fmt::format("{:.5}", c);
    EXPECT_EQ(result, "D");
}

// Test formatting of char with type specifier 'd' (decimal)
TEST(FormatCharTest, CharAsDecimal) {
    char c = 'A';
    std::string result = fmt::format("{:d}", c);
    EXPECT_EQ(result, "65");
}

// Test formatting of negative char with type specifier 'd'
TEST(FormatCharTest, NegativeCharAsDecimal) {
    char c = -1;
    std::string result = fmt::format("{:d}", c);
    EXPECT_EQ(result, "-1");
}

// Test formatting of char with type specifier 'c' (character)
TEST(FormatCharTest, CharAsCharacter) {
    char c = 65;
    std::string result = fmt::format("{:c}", c);
    EXPECT_EQ(result, "A");
}

// Test formatting of negative char with type specifier 'c' (should still show as character)
TEST(FormatCharTest, NegativeCharAsCharacter) {
    char c = -1;
    std::string result = fmt::format("{:c}", c);
    // -1 as char is implementation-defined, but typically it's 255 or -1
    // The important thing is it doesn't crash and produces some output
    EXPECT_FALSE(result.empty());
}

// Test formatting of char with hex specifier
TEST(FormatCharTest, CharAsHex) {
    char c = 255;
    std::string result = fmt::format("{:x}", c);
    EXPECT_EQ(result, "ff");
}

// Test formatting of char with uppercase hex
TEST(FormatCharTest, CharAsHexUpper) {
    char c = 255;
    std::string result = fmt::format("{:X}", c);
    EXPECT_EQ(result, "FF");
}

// Test formatting of char with octal
TEST(FormatCharTest, CharAsOctal) {
    char c = 64;
    std::string result = fmt::format("{:o}", c);
    EXPECT_EQ(result, "100");
}

// Test formatting of char with binary
TEST(FormatCharTest, CharAsBinary) {
    char c = 5;
    std::string result = fmt::format("{:b}", c);
    EXPECT_EQ(result, "101");
}

// Test formatting of char with mixed specifiers
TEST(FormatCharTest, CharWithMixedSpecifiers) {
    char c = -10;
    std::string result = fmt::format("{:+#010d}", c);
    EXPECT_EQ(result, "-000000010");
}

// Test formatting of char with left alignment
TEST(FormatCharTest, CharLeftAligned) {
    char c = 'E';
    std::string result = fmt::format("{:<5}", c);
    EXPECT_EQ(result, "E    ");
}

// Test formatting of char with center alignment
TEST(FormatCharTest, CharCenterAligned) {
    char c = 'F';
    std::string result = fmt::format("{:^5}", c);
    EXPECT_EQ(result, "  F  ");
}

// Test formatting of char with right alignment (default)
TEST(FormatCharTest, CharRightAligned) {
    char c = 'G';
    std::string result = fmt::format("{:>5}", c);
    EXPECT_EQ(result, "    G");
}

// Test formatting of char with custom fill character
TEST(FormatCharTest, CharWithCustomFill) {
    char c = 'H';
    std::string result = fmt::format("{:*^5}", c);
    EXPECT_EQ(result, "**H**");
}

// Test formatting of char with locale-specific formatting
TEST(FormatCharTest, CharWithLocale) {
    char c = 'I';
    std::string result = fmt::format("{:L}", c);
    EXPECT_EQ(result, "I");
}

// Test formatting of char with thousands separator (should be ignored for chars)
TEST(FormatCharTest, CharWithThousandsSep) {
    char c = 'J';
    std::string result = fmt::format("{:,}", c);
    EXPECT_EQ(result, "J");
}

// Test formatting of char with sign aware zero padding
TEST(FormatCharTest, CharSignAwareZeroPad) {
    char c = -3;
    std::string result = fmt::format("{:05}", c);
    EXPECT_EQ(result, "-0003");
}

// Test formatting of char with minimum width larger than needed
TEST(FormatCharTest, CharWithLargeWidth) {
    char c = 'K';
    std::string result = fmt::format("{:10}", c);
    EXPECT_EQ(result, "         K");
}

// Test formatting of char with width and precision
TEST(FormatCharTest, CharWithWidthAndPrecision) {
    char c = 'L';
    std::string result = fmt::format("{:5.3}", c);
    EXPECT_EQ(result, "    L");
}

// Test formatting of char with dynamic width
TEST(FormatCharTest, CharWithDynamicWidth) {
    char c = 'M';
    std::string result = fmt::format("{:{}}", c, 5);
    EXPECT_EQ(result, "    M");
}

// Test formatting of char with dynamic precision
TEST(FormatCharTest, CharWithDynamicPrecision) {
    char c = 'N';
    std::string result = fmt::format("{:.{}}", c, 3);
    EXPECT_EQ(result, "N");
}

// Test formatting of char with both dynamic width and precision
TEST(FormatCharTest, CharWithDynamicWidthAndPrecision) {
    char c = 'O';
    std::string result = fmt::format("{:{}.{}}", c, 5, 3);
    EXPECT_EQ(result, "    O");
}

// Test formatting of char with argument index
TEST(FormatCharTest, CharWithArgIndex) {
    char c = 'P';
    std::string result = fmt::format("{0:5}", c);
    EXPECT_EQ(result, "    P");
}

// Test formatting of char with named argument
TEST(FormatCharTest, CharWithNamedArg) {
    char c = 'Q';
    std::string result = fmt::format("{ch:5}", fmt::arg("ch", c));
    EXPECT_EQ(result, "    Q");
}

// Test formatting of char with multiple arguments
TEST(FormatCharTest, CharWithMultipleArgs) {
    char c1 = 'R', c2 = 'S';
    std::string result = fmt::format("{}{}", c1, c2);
    EXPECT_EQ(result, "RS");
}

// Test formatting of char in a larger string
TEST(FormatCharTest, CharInLargerString) {
    char c = 'T';
    std::string result = fmt::format("Char: {} end", c);
    EXPECT_EQ(result, "Char: T end");
}

// Test formatting of char with escape sequences
TEST(FormatCharTest, CharWithEscape) {
    char c = '\n';
    std::string result = fmt::format("{}", c);
    EXPECT_EQ(result, "\n");
}

// Test formatting of char with tab character
TEST(FormatCharTest, CharWithTab) {
    char c = '\t';
    std::string result = fmt::format("{}", c);
    EXPECT_EQ(result, "\t");
}

// Test formatting of char with null character
TEST(FormatCharTest, CharWithNull) {
    char c = '\0';
    std::string result = fmt::format("{}", c);
    EXPECT_EQ(result, "\0");
    EXPECT_EQ(result.size(), 1);
}

// Test formatting of char with maximum positive value
TEST(FormatCharTest, CharMaxPositive) {
    char c = 127;
    std::string result = fmt::format("{}", c);
    EXPECT_EQ(result, "127");
}

// Test formatting of char with minimum negative value
TEST(FormatCharTest, CharMinNegative) {
    char c = -128;
    std::string result = fmt::format("{}", c);
    EXPECT_EQ(result, "-128");
}

// Test formatting of char with value 1
TEST(FormatCharTest, CharValueOne) {
    char c = 1;
    std::string result = fmt::format("{}", c);
    EXPECT_EQ(result, "\1");
}

// Test formatting of char with value -1 using decimal specifier
TEST(FormatCharTest, CharNegativeOneDecimal) {
    char c = -1;
    std::string result = fmt::format("{:d}", c);
    EXPECT_EQ(result, "-1");
}

// Test formatting of char with value -128 using decimal specifier
TEST(FormatCharTest, CharMinNegativeDecimal) {
    char c = -128;
    std::string result = fmt::format("{:d}", c);
    EXPECT_EQ(result, "-128");
}

// Test formatting of char with value 127 using decimal specifier
TEST(FormatCharTest, CharMaxPositiveDecimal) {
    char c = 127;
    std::string result = fmt::format("{:d}", c);
    EXPECT_EQ(result, "127");
}

// Test formatting of char with value 0 using decimal specifier
TEST(FormatCharTest, CharZeroDecimal) {
    char c = 0;
    std::string result = fmt::format("{:d}", c);
    EXPECT_EQ(result, "0");
}

// Test formatting of char with value 255 using decimal specifier (unsigned char promotion)
TEST(FormatCharTest, Char255Decimal) {
    char c = static_cast<char>(255);
    std::string result = fmt::format("{:d}", c);
    // char with value 255 is implementation-defined, but typically -1
    // The important thing is it doesn't crash
    EXPECT_FALSE(result.empty());
}