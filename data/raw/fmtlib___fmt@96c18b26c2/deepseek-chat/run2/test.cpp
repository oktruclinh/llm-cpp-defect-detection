#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <string>
#include <vector>
#include <cmath>

// Test the printf formatter's behavior with various format specifications
// The focal function handles the case where a non-numeric type is formatted
// with printf-style format flags that are normally ignored for non-numeric types

class PrintfFormatterTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

// Test basic string formatting with printf
TEST_F(PrintfFormatterTest, BasicStringFormatting) {
    std::string result = fmt::sprintf("%s", "hello");
    EXPECT_EQ(result, "hello");
}

// Test that '0' flag is ignored for strings (non-numeric type)
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("%010s", "hello");
    EXPECT_EQ(result, "     hello");  // Should be right-aligned with spaces, not zero-padded
}

// Test that '0' flag is ignored for characters (non-numeric type)
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForChars) {
    std::string result = fmt::sprintf("%05c", 'A');
    EXPECT_EQ(result, "    A");  // Should be right-aligned with spaces
}

// Test that '0' flag is ignored for pointers (non-numeric type)
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForPointers) {
    int x = 42;
    std::string result = fmt::sprintf("%020p", static_cast<void*>(&x));
    // Should be right-aligned with spaces, not zero-padded
    EXPECT_NE(result.find("0x"), std::string::npos);
    EXPECT_TRUE(result.size() > 10);  // Should have leading spaces
}

// Test that '+' flag is ignored for strings
TEST_F(PrintfFormatterTest, PlusFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("%+10s", "hello");
    EXPECT_EQ(result, "     hello");  // No plus sign for strings
}

// Test that space flag is ignored for strings
TEST_F(PrintfFormatterTest, SpaceFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("% 10s", "hello");
    EXPECT_EQ(result, "     hello");  // No leading space for strings
}

// Test that '#' flag is ignored for strings
TEST_F(PrintfFormatterTest, AltFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("%#10s", "hello");
    EXPECT_EQ(result, "     hello");  // No alternate form for strings
}

// Test that '-' flag still works for strings (left alignment)
TEST_F(PrintfFormatterTest, MinusFlagWorksForStrings) {
    std::string result = fmt::sprintf("%-10s", "hello");
    EXPECT_EQ(result, "hello     ");  // Left-aligned
}

// Test that '-' overrides '0' for strings
TEST_F(PrintfFormatterTest, MinusOverridesZeroForStrings) {
    std::string result = fmt::sprintf("%-010s", "hello");
    EXPECT_EQ(result, "hello     ");  // Left-aligned, '0' ignored
}

// Test width specification for strings
TEST_F(PrintfFormatterTest, WidthSpecificationForStrings) {
    std::string result = fmt::sprintf("%10s", "hello");
    EXPECT_EQ(result, "     hello");  // Right-aligned with spaces
}

// Test precision specification for strings
TEST_F(PrintfFormatterTest, PrecisionForStrings) {
    std::string result = fmt::sprintf("%.3s", "hello");
    EXPECT_EQ(result, "hel");  // Truncated to 3 characters
}

// Test width and precision together for strings
TEST_F(PrintfFormatterTest, WidthAndPrecisionForStrings) {
    std::string result = fmt::sprintf("%10.3s", "hello");
    EXPECT_EQ(result, "       hel");  // Right-aligned, truncated to 3 chars
}

// Test that '0' flag works for integers (numeric type)
TEST_F(PrintfFormatterTest, ZeroFlagWorksForIntegers) {
    std::string result = fmt::sprintf("%010d", 42);
    EXPECT_EQ(result, "0000000042");  // Zero-padded for integers
}

// Test that '0' flag works for floats (numeric type)
TEST_F(PrintfFormatterTest, ZeroFlagWorksForFloats) {
    std::string result = fmt::sprintf("%010.2f", 3.14);
    EXPECT_EQ(result, "0000003.14");  // Zero-padded for floats
}

// Test empty string formatting
TEST_F(PrintfFormatterTest, EmptyStringFormatting) {
    std::string result = fmt::sprintf("%s", "");
    EXPECT_EQ(result, "");
}

// Test empty string with width
TEST_F(PrintfFormatterTest, EmptyStringWithWidth) {
    std::string result = fmt::sprintf("%5s", "");
    EXPECT_EQ(result, "     ");  // 5 spaces
}

// Test string with special characters
TEST_F(PrintfFormatterTest, StringWithSpecialCharacters) {
    std::string result = fmt::sprintf("%s", "hello\nworld");
    EXPECT_EQ(result, "hello\nworld");
}

// Test multiple format specifiers
TEST_F(PrintfFormatterTest, MultipleFormatSpecifiers) {
    std::string result = fmt::sprintf("%s %d %s", "hello", 42, "world");
    EXPECT_EQ(result, "hello 42 world");
}

// Test that align::numeric is properly overridden for strings
TEST_F(PrintfFormatterTest, NumericAlignOverriddenForStrings) {
    // The '=' alignment is numeric-specific and should be treated as right for strings
    std::string result = fmt::sprintf("%=10s", "hello");
    EXPECT_EQ(result, "     hello");  // Should be right-aligned
}

// Test very long string formatting
TEST_F(PrintfFormatterTest, LongStringFormatting) {
    std::string long_str(1000, 'a');
    std::string result = fmt::sprintf("%s", long_str);
    EXPECT_EQ(result, long_str);
    EXPECT_EQ(result.size(), 1000);
}

// Test string with null character in middle (should work as std::string)
TEST_F(PrintfFormatterTest, StringWithNullChar) {
    std::string with_null = std::string("hello") + '\0' + "world";
    std::string result = fmt::sprintf("%s", with_null);
    EXPECT_EQ(result.size(), 11);  // Should include the null character
    EXPECT_EQ(result[5], '\0');
}

// Test that sign flag is properly reset for non-numeric types
TEST_F(PrintfFormatterTest, SignResetForNonNumeric) {
    // After formatting a string, the sign should not affect subsequent numeric formatting
    std::string result = fmt::sprintf("%+d %s %+d", 5, "test", -3);
    EXPECT_EQ(result, "+5 test -3");
}

// Test that alt flag is properly reset for non-numeric types
TEST_F(PrintfFormatterTest, AltResetForNonNumeric) {
    // After formatting a string, the alt flag should not affect subsequent numeric formatting
    std::string result = fmt::sprintf("%#x %s %#x", 255, "test", 15);
    EXPECT_EQ(result, "0xff test 0xf");
}

// Test formatting with std::string argument
TEST_F(PrintfFormatterTest, StdStringArgument) {
    std::string arg = "hello";
    std::string result = fmt::sprintf("%s", arg);
    EXPECT_EQ(result, "hello");
}

// Test formatting with const char* argument
TEST_F(PrintfFormatterTest, ConstCharPointerArgument) {
    const char* arg = "hello";
    std::string result = fmt::sprintf("%s", arg);
    EXPECT_EQ(result, "hello");
}

// Test that width with '0' flag for string doesn't zero-pad
TEST_F(PrintfFormatterTest, ZeroFlagWithWidthForString) {
    std::string result = fmt::sprintf("%010s", "hi");
    EXPECT_EQ(result, "        hi");  // Spaces, not zeros
    EXPECT_NE(result, "00000000hi");  // Explicitly not zero-padded
}

// Test that multiple flags are properly ignored for strings
TEST_F(PrintfFormatterTest, MultipleFlagsIgnoredForStrings) {
    std::string result = fmt::sprintf("%+ #010s", "test");
    EXPECT_EQ(result, "      test");  // All flags ignored, just width and right-align
}