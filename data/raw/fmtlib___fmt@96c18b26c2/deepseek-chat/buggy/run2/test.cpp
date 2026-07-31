#include <gtest/gtest.h>
#include <fmt/printf.h>

// Test that printf-style formatting with signed integer conversion works correctly
TEST(PrintfTest, SignedIntegerConversion) {
    // Normal positive value
    EXPECT_EQ(fmt::sprintf("%d", 42), "42");
    // Normal negative value
    EXPECT_EQ(fmt::sprintf("%d", -42), "-42");
    // Zero
    EXPECT_EQ(fmt::sprintf("%d", 0), "0");
    // Maximum int
    EXPECT_EQ(fmt::sprintf("%d", std::numeric_limits<int>::max()), "2147483647");
    // Minimum int
    EXPECT_EQ(fmt::sprintf("%d", std::numeric_limits<int>::min()), "-2147483648");
}

// Test that printf-style formatting with unsigned integer conversion works correctly
TEST(PrintfTest, UnsignedIntegerConversion) {
    // Normal positive value
    EXPECT_EQ(fmt::sprintf("%u", 42), "42");
    // Zero
    EXPECT_EQ(fmt::sprintf("%u", 0), "0");
    // Maximum unsigned int
    EXPECT_EQ(fmt::sprintf("%u", std::numeric_limits<unsigned int>::max()), "4294967295");
    // Negative value cast to unsigned
    EXPECT_EQ(fmt::sprintf("%u", -1), "4294967295");
}

// Test that printf-style formatting with hex conversion works correctly
TEST(PrintfTest, HexConversion) {
    // Normal value
    EXPECT_EQ(fmt::sprintf("%x", 255), "ff");
    // Uppercase hex
    EXPECT_EQ(fmt::sprintf("%X", 255), "FF");
    // Zero
    EXPECT_EQ(fmt::sprintf("%x", 0), "0");
    // With alternate form
    EXPECT_EQ(fmt::sprintf("%#x", 255), "0xff");
    EXPECT_EQ(fmt::sprintf("%#X", 255), "0XFF");
}

// Test that printf-style formatting with octal conversion works correctly
TEST(PrintfTest, OctalConversion) {
    // Normal value
    EXPECT_EQ(fmt::sprintf("%o", 8), "10");
    // Zero
    EXPECT_EQ(fmt::sprintf("%o", 0), "0");
    // With alternate form
    EXPECT_EQ(fmt::sprintf("%#o", 8), "010");
}

// Test that printf-style formatting with width and precision works correctly
TEST(PrintfTest, WidthAndPrecision) {
    // Width with padding
    EXPECT_EQ(fmt::sprintf("%5d", 42), "   42");
    // Width with left alignment
    EXPECT_EQ(fmt::sprintf("%-5d", 42), "42   ");
    // Width with zero padding
    EXPECT_EQ(fmt::sprintf("%05d", 42), "00042");
    // Precision for integers
    EXPECT_EQ(fmt::sprintf("%.5d", 42), "00042");
    // Width and precision combined
    EXPECT_EQ(fmt::sprintf("%8.5d", 42), "   00042");
}

// Test that printf-style formatting with sign flags works correctly
TEST(PrintfTest, SignFlags) {
    // Always show sign
    EXPECT_EQ(fmt::sprintf("%+d", 42), "+42");
    EXPECT_EQ(fmt::sprintf("%+d", -42), "-42");
    // Space for positive sign
    EXPECT_EQ(fmt::sprintf("% d", 42), " 42");
    EXPECT_EQ(fmt::sprintf("% d", -42), "-42");
}

// Test that printf-style formatting with string conversion works correctly
TEST(PrintfTest, StringConversion) {
    // Normal string
    EXPECT_EQ(fmt::sprintf("%s", "hello"), "hello");
    // Empty string
    EXPECT_EQ(fmt::sprintf("%s", ""), "");
    // String with width
    EXPECT_EQ(fmt::sprintf("%10s", "hello"), "     hello");
    // String with left alignment
    EXPECT_EQ(fmt::sprintf("%-10s", "hello"), "hello     ");
    // String with precision
    EXPECT_EQ(fmt::sprintf("%.3s", "hello"), "hel");
}

// Test that printf-style formatting with character conversion works correctly
TEST(PrintfTest, CharacterConversion) {
    // Normal character
    EXPECT_EQ(fmt::sprintf("%c", 'A'), "A");
    // Character from integer
    EXPECT_EQ(fmt::sprintf("%c", 65), "A");
}

// Test that printf-style formatting with pointer conversion works correctly
TEST(PrintfTest, PointerConversion) {
    int x = 42;
    // Pointer should be formatted as hex address
    std::string result = fmt::sprintf("%p", static_cast<const void*>(&x));
    EXPECT_TRUE(result.find("0x") == 0 || result.find("0X") == 0);
    EXPECT_GT(result.size(), 2);
}

// Test that printf-style formatting with floating-point conversion works correctly
TEST(PrintfTest, FloatingPointConversion) {
    // Normal float
    EXPECT_EQ(fmt::sprintf("%f", 3.14), "3.140000");
    // Float with precision
    EXPECT_EQ(fmt::sprintf("%.2f", 3.14159), "3.14");
    // Float with width
    EXPECT_EQ(fmt::sprintf("%8.2f", 3.14), "    3.14");
    // Scientific notation
    EXPECT_EQ(fmt::sprintf("%e", 314.0), "3.140000e+02");
    // Shortest representation
    EXPECT_EQ(fmt::sprintf("%g", 3.14), "3.14");
}

// Test that printf-style formatting with multiple arguments works correctly
TEST(PrintfTest, MultipleArguments) {
    EXPECT_EQ(fmt::sprintf("%d %s %f", 42, "hello", 3.14), "42 hello 3.140000");
}

// Test that printf-style formatting with positional arguments works correctly
TEST(PrintfTest, PositionalArguments) {
    EXPECT_EQ(fmt::sprintf("%2$d %1$d", 10, 20), "20 10");
}

// Test that printf-style formatting handles edge cases correctly
TEST(PrintfTest, EdgeCases) {
    // Very large width
    EXPECT_EQ(fmt::sprintf("%100d", 42), std::string(98, ' ') + "42");
    // Very large precision
    EXPECT_EQ(fmt::sprintf("%.100d", 42), std::string(98, '0') + "42");
    // Negative width (should be treated as left alignment)
    EXPECT_EQ(fmt::sprintf("%-5d", 42), "42   ");
    // Star width
    EXPECT_EQ(fmt::sprintf("%*d", 5, 42), "   42");
    // Star precision
    EXPECT_EQ(fmt::sprintf("%.*d", 5, 42), "00042");
}

// Test that printf-style formatting handles invalid format strings gracefully
TEST(PrintfTest, InvalidFormatStrings) {
    // Unknown conversion specifier
    EXPECT_THROW(fmt::sprintf("%q", 42), fmt::format_error);
    // Incomplete format specifier
    EXPECT_THROW(fmt::sprintf("%", 42), fmt::format_error);
}