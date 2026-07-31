#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <cstdint>
#include "ostream.h"

// Test the printf_arg_formatter class through the public printf API
// Since printf_arg_formatter is used internally by fmt's printf-style formatting,
// we test it indirectly through the public printf functions

TEST(PrintfArgFormatterTest, NullPointerChar) {
    // Test that null char* is formatted as "(null)" by default
    std::string result = fmt::sprintf("%s", static_cast<const char*>(nullptr));
    EXPECT_EQ(result, "(null)");
}

TEST(PrintfArgFormatterTest, NullPointerWchar) {
    // Test that null wchar_t* is formatted as "(null)" by default
    std::string result = fmt::sprintf("%ls", static_cast<const wchar_t*>(nullptr));
    EXPECT_EQ(result, "(null)");
}

TEST(PrintfArgFormatterTest, NullPointerWithPFormat) {
    // Test that null pointer with %p format writes "(nil)"
    std::string result = fmt::sprintf("%p", static_cast<const char*>(nullptr));
    EXPECT_EQ(result, "(nil)");
}

TEST(PrintfArgFormatterTest, NullWidePointerWithPFormat) {
    // Test that null wchar_t* with %p format writes "(nil)"
    std::string result = fmt::sprintf("%p", static_cast<const wchar_t*>(nullptr));
    EXPECT_EQ(result, "(nil)");
}

TEST(PrintfArgFormatterTest, BoolTrueFormatsAsOne) {
    // Test that bool true formats as 1 (not "true")
    std::string result = fmt::sprintf("%d", true);
    EXPECT_EQ(result, "1");
}

TEST(PrintfArgFormatterTest, BoolFalseFormatsAsZero) {
    // Test that bool false formats as 0 (not "false")
    std::string result = fmt::sprintf("%d", false);
    EXPECT_EQ(result, "0");
}

TEST(PrintfArgFormatterTest, BoolWithSFormat) {
    // Test that bool with %s formats as "true"/"false"
    std::string result = fmt::sprintf("%s", true);
    EXPECT_EQ(result, "true");
    
    result = fmt::sprintf("%s", false);
    EXPECT_EQ(result, "false");
}

TEST(PrintfArgFormatterTest, CharFormatsAsCharacter) {
    // Test that char formats as character by default
    std::string result = fmt::sprintf("%c", 'A');
    EXPECT_EQ(result, "A");
}

TEST(PrintfArgFormatterTest, CharWithDFormat) {
    // Test that char with %d formats as integer
    std::string result = fmt::sprintf("%d", 'A');
    EXPECT_EQ(result, "65");
}

TEST(PrintfArgFormatterTest, IntegerFormatsCorrectly) {
    // Test basic integer formatting
    std::string result = fmt::sprintf("%d", 42);
    EXPECT_EQ(result, "42");
    
    result = fmt::sprintf("%d", -42);
    EXPECT_EQ(result, "-42");
}

TEST(PrintfArgFormatterTest, IntegerWithWidth) {
    // Test integer formatting with width
    std::string result = fmt::sprintf("%5d", 42);
    EXPECT_EQ(result, "   42");
}

TEST(PrintfArgFormatterTest, IntegerWithZeroPadding) {
    // Test integer formatting with zero padding
    std::string result = fmt::sprintf("%05d", 42);
    EXPECT_EQ(result, "00042");
}

TEST(PrintfArgFormatterTest, IntegerWithLeftAlign) {
    // Test integer formatting with left alignment
    std::string result = fmt::sprintf("%-5d", 42);
    EXPECT_EQ(result, "42   ");
}

TEST(PrintfArgFormatterTest, FloatingPointFormatsCorrectly) {
    // Test basic floating point formatting
    std::string result = fmt::sprintf("%f", 3.14159);
    EXPECT_EQ(result, "3.141590");
}

TEST(PrintfArgFormatterTest, FloatingPointWithPrecision) {
    // Test floating point with precision
    std::string result = fmt::sprintf("%.2f", 3.14159);
    EXPECT_EQ(result, "3.14");
}

TEST(PrintfArgFormatterTest, StringFormatsCorrectly) {
    // Test basic string formatting
    std::string result = fmt::sprintf("%s", "hello");
    EXPECT_EQ(result, "hello");
}

TEST(PrintfArgFormatterTest, StringWithWidth) {
    // Test string formatting with width
    std::string result = fmt::sprintf("%10s", "hello");
    EXPECT_EQ(result, "     hello");
}

TEST(PrintfArgFormatterTest, StringWithLeftAlign) {
    // Test string formatting with left alignment
    std::string result = fmt::sprintf("%-10s", "hello");
    EXPECT_EQ(result, "hello     ");
}

TEST(PrintfArgFormatterTest, WideStringFormatsCorrectly) {
    // Test wide string formatting
    std::string result = fmt::sprintf("%ls", L"hello");
    EXPECT_EQ(result, "hello");
}

TEST(PrintfArgFormatterTest, MultipleArguments) {
    // Test formatting with multiple arguments
    std::string result = fmt::sprintf("%s %d %f", "test", 42, 3.14);
    EXPECT_EQ(result, "test 42 3.140000");
}

TEST(PrintfArgFormatterTest, HexFormatting) {
    // Test hexadecimal formatting
    std::string result = fmt::sprintf("%x", 255);
    EXPECT_EQ(result, "ff");
    
    result = fmt::sprintf("%X", 255);
    EXPECT_EQ(result, "FF");
}

TEST(PrintfArgFormatterTest, OctalFormatting) {
    // Test octal formatting
    std::string result = fmt::sprintf("%o", 8);
    EXPECT_EQ(result, "10");
}

TEST(PrintfArgFormatterTest, PointerFormatting) {
    // Test pointer formatting (just check it doesn't crash and produces something)
    int x = 42;
    std::string result = fmt::sprintf("%p", &x);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "(nil)");
    EXPECT_NE(result, "(null)");
}

TEST(PrintfArgFormatterTest, CharTypeWithCFormat) {
    // Test char with %c format
    std::string result = fmt::sprintf("%c", 'X');
    EXPECT_EQ(result, "X");
}

TEST(PrintfArgFormatterTest, CharTypeWithSFormat) {
    // Test char with %s format (should format as character)
    std::string result = fmt::sprintf("%s", 'X');
    EXPECT_EQ(result, "X");
}

TEST(PrintfArgFormatterTest, LargeInteger) {
    // Test large integer values
    std::string result = fmt::sprintf("%lld", static_cast<long long>(1234567890123LL));
    EXPECT_EQ(result, "1234567890123");
}

TEST(PrintfArgFormatterTest, NegativeInteger) {
    // Test negative integer formatting
    std::string result = fmt::sprintf("%d", -123);
    EXPECT_EQ(result, "-123");
}

TEST(PrintfArgFormatterTest, UnsignedInteger) {
    // Test unsigned integer formatting
    std::string result = fmt::sprintf("%u", 4294967295U);
    EXPECT_EQ(result, "4294967295");
}

TEST(PrintfArgFormatterTest, ZeroPaddingWithNegative) {
    // Test zero padding with negative numbers
    std::string result = fmt::sprintf("%05d", -42);
    EXPECT_EQ(result, "-0042");
}

TEST(PrintfArgFormatterTest, PlusFlag) {
    // Test plus flag for positive numbers
    std::string result = fmt::sprintf("%+d", 42);
    EXPECT_EQ(result, "+42");
    
    result = fmt::sprintf("%+d", -42);
    EXPECT_EQ(result, "-42");
}

TEST(PrintfArgFormatterTest, SpaceFlag) {
    // Test space flag for positive numbers
    std::string result = fmt::sprintf("% d", 42);
    EXPECT_EQ(result, " 42");
    
    result = fmt::sprintf("% d", -42);
    EXPECT_EQ(result, "-42");
}

TEST(PrintfArgFormatterTest, AlternativeFormHex) {
    // Test alternative form for hex
    std::string result = fmt::sprintf("%#x", 255);
    EXPECT_EQ(result, "0xff");
    
    result = fmt::sprintf("%#X", 255);
    EXPECT_EQ(result, "0XFF");
}

TEST(PrintfArgFormatterTest, AlternativeFormOctal) {
    // Test alternative form for octal
    std::string result = fmt::sprintf("%#o", 8);
    EXPECT_EQ(result, "010");
}

TEST(PrintfArgFormatterTest, PrecisionForString) {
    // Test precision for strings
    std::string result = fmt::sprintf("%.3s", "hello");
    EXPECT_EQ(result, "hel");
}

TEST(PrintfArgFormatterTest, PrecisionForInteger) {
    // Test precision for integers
    std::string result = fmt::sprintf("%.5d", 42);
    EXPECT_EQ(result, "00042");
}

TEST(PrintfArgFormatterTest, StarWidth) {
    // Test width from argument
    std::string result = fmt::sprintf("%*d", 5, 42);
    EXPECT_EQ(result, "   42");
}

TEST(PrintfArgFormatterTest, StarPrecision) {
    // Test precision from argument
    std::string result = fmt::sprintf("%.*f", 2, 3.14159);
    EXPECT_EQ(result, "3.14");
}