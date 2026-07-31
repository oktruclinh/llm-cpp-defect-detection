#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <string>
#include <stdexcept>

TEST(PrintfTest, FormatIntegerWithSignAndFlags) {
    // Test normal integer formatting with sign
    std::string result = fmt::sprintf("%d", 42);
    EXPECT_EQ(result, "42");
    
    // Test negative integer
    result = fmt::sprintf("%d", -42);
    EXPECT_EQ(result, "-42");
    
    // Test with plus sign flag
    result = fmt::sprintf("%+d", 42);
    EXPECT_EQ(result, "+42");
    
    // Test with space flag
    result = fmt::sprintf("% d", 42);
    EXPECT_EQ(result, " 42");
    
    // Test with zero padding
    result = fmt::sprintf("%05d", 42);
    EXPECT_EQ(result, "00042");
    
    // Test with alt flag (should not affect integers)
    result = fmt::sprintf("%#d", 42);
    EXPECT_EQ(result, "42");
}

TEST(PrintfTest, FormatIntegerEdgeCases) {
    // Test zero
    std::string result = fmt::sprintf("%d", 0);
    EXPECT_EQ(result, "0");
    
    // Test maximum positive integer
    result = fmt::sprintf("%d", 2147483647);
    EXPECT_EQ(result, "2147483647");
    
    // Test minimum negative integer
    result = fmt::sprintf("%d", -2147483648);
    EXPECT_EQ(result, "-2147483648");
    
    // Test with width specification
    result = fmt::sprintf("%5d", 42);
    EXPECT_EQ(result, "   42");
    
    // Test with negative width (should be left-aligned)
    result = fmt::sprintf("%-5d", 42);
    EXPECT_EQ(result, "42   ");
}

TEST(PrintfTest, FormatUnsignedInteger) {
    // Test unsigned integer formatting
    std::string result = fmt::sprintf("%u", 42u);
    EXPECT_EQ(result, "42");
    
    // Test unsigned with zero padding
    result = fmt::sprintf("%05u", 42u);
    EXPECT_EQ(result, "00042");
    
    // Test unsigned with alt flag
    result = fmt::sprintf("%#u", 42u);
    EXPECT_EQ(result, "42");
}

TEST(PrintfTest, FormatHexadecimal) {
    // Test hexadecimal formatting
    std::string result = fmt::sprintf("%x", 255);
    EXPECT_EQ(result, "ff");
    
    // Test uppercase hexadecimal
    result = fmt::sprintf("%X", 255);
    EXPECT_EQ(result, "FF");
    
    // Test with alt flag
    result = fmt::sprintf("%#x", 255);
    EXPECT_EQ(result, "0xff");
    
    // Test with zero padding
    result = fmt::sprintf("%08x", 255);
    EXPECT_EQ(result, "000000ff");
}

TEST(PrintfTest, FormatOctal) {
    // Test octal formatting
    std::string result = fmt::sprintf("%o", 64);
    EXPECT_EQ(result, "100");
    
    // Test with alt flag
    result = fmt::sprintf("%#o", 64);
    EXPECT_EQ(result, "0100");
    
    // Test with zero padding
    result = fmt::sprintf("%05o", 64);
    EXPECT_EQ(result, "00100");
}

TEST(PrintfTest, FormatCharacter) {
    // Test character formatting
    std::string result = fmt::sprintf("%c", 'A');
    EXPECT_EQ(result, "A");
    
    // Test with zero padding
    result = fmt::sprintf("%05c", 'A');
    EXPECT_EQ(result, "    A");
}

TEST(PrintfTest, FormatString) {
    // Test string formatting
    std::string result = fmt::sprintf("%s", "hello");
    EXPECT_EQ(result, "hello");
    
    // Test with width specification
    result = fmt::sprintf("%10s", "hello");
    EXPECT_EQ(result, "     hello");
    
    // Test with precision
    result = fmt::sprintf("%.3s", "hello");
    EXPECT_EQ(result, "hel");
}

TEST(PrintfTest, FormatFloat) {
    // Test float formatting
    std::string result = fmt::sprintf("%f", 3.14159);
    EXPECT_EQ(result, "3.141590");
    
    // Test with precision
    result = fmt::sprintf("%.2f", 3.14159);
    EXPECT_EQ(result, "3.14");
    
    // Test scientific notation
    result = fmt::sprintf("%e", 123.456);
    EXPECT_EQ(result, "1.234560e+02");
    
    // Test with width
    result = fmt::sprintf("%10f", 3.14159);
    EXPECT_EQ(result, "  3.141590");
}

TEST(PrintfTest, FormatPointer) {
    // Test pointer formatting
    int x = 42;
    std::string result = fmt::sprintf("%p", static_cast<void*>(&x));
    EXPECT_NE(result, "");
    EXPECT_TRUE(result.find("0x") != std::string::npos || result.find("0X") != std::string::npos);
}

TEST(PrintfTest, FormatMixed) {
    // Test mixed format specifiers
    std::string result = fmt::sprintf("Number: %d, String: %s, Float: %.2f", 42, "test", 3.14159);
    EXPECT_EQ(result, "Number: 42, String: test, Float: 3.14");
    
    // Test with multiple flags
    result = fmt::sprintf("%+05d", 42);
    EXPECT_EQ(result, "+0042");
}