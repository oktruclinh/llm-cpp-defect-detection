#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <fmt/format.h>
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
    
    // Test with alternative format (octal)
    result = fmt::sprintf("%#o", 42);
    EXPECT_EQ(result, "052");
    
    // Test with alternative format (hex)
    result = fmt::sprintf("%#x", 42);
    EXPECT_EQ(result, "0x2a");
}

TEST(PrintfTest, FormatIntegerEdgeCases) {
    // Test zero
    std::string result = fmt::sprintf("%d", 0);
    EXPECT_EQ(result, "0");
    
    // Test maximum integer
    result = fmt::sprintf("%d", 2147483647);
    EXPECT_EQ(result, "2147483647");
    
    // Test minimum integer
    result = fmt::sprintf("%d", -2147483648);
    EXPECT_EQ(result, "-2147483648");
    
    // Test with width specification
    result = fmt::sprintf("%5d", 42);
    EXPECT_EQ(result, "   42");
    
    // Test with negative width (left alignment)
    result = fmt::sprintf("%-5d", 42);
    EXPECT_EQ(result, "42   ");
    
    // Test with zero width and zero value
    result = fmt::sprintf("%0d", 0);
    EXPECT_EQ(result, "0");
    
    // Test with zero padding and negative number
    result = fmt::sprintf("%05d", -42);
    EXPECT_EQ(result, "-0042");
}

TEST(PrintfTest, FormatIntegerAlignmentAndFlags) {
    // Test numeric alignment behavior
    std::string result = fmt::sprintf("%05d", 42);
    EXPECT_EQ(result, "00042");
    
    // Test right alignment with width
    result = fmt::sprintf("%5d", 42);
    EXPECT_EQ(result, "   42");
    
    // Test left alignment
    result = fmt::sprintf("%-5d", 42);
    EXPECT_EQ(result, "42   ");
    
    // Test with sign and padding
    result = fmt::sprintf("%+05d", 42);
    EXPECT_EQ(result, "+0042");
    
    // Test with space and padding
    result = fmt::sprintf("% 05d", 42);
    EXPECT_EQ(result, " 0042");
}

TEST(PrintfTest, FormatIntegerSpecialCases) {
    // Test formatting with various flags combinations
    std::string result = fmt::sprintf("%#05d", 42);
    EXPECT_EQ(result, "00042");
    
    // Test with multiple flags
    result = fmt::sprintf("% +05d", 42);
    EXPECT_EQ(result, "+0042");
    
    // Test with alternative format and zero padding
    result = fmt::sprintf("%#08x", 255);
    EXPECT_EQ(result, "0x0000ff");
    
    // Test with octal alternative format
    result = fmt::sprintf("%#08o", 64);
    EXPECT_EQ(result, "00000100");
}

TEST(PrintfTest, FormatIntegerBounds) {
    // Test boundary values
    std::string result = fmt::sprintf("%d", 0);
    EXPECT_EQ(result, "0");
    
    result = fmt::sprintf("%d", 1);
    EXPECT_EQ(result, "1");
    
    result = fmt::sprintf("%d", -1);
    EXPECT_EQ(result, "-1");
    
    // Test maximum positive value
    result = fmt::sprintf("%d", 2147483647);
    EXPECT_EQ(result, "2147483647");
    
    // Test minimum negative value
    result = fmt::sprintf("%d", -2147483648);
    EXPECT_EQ(result, "-2147483648");
}

TEST(PrintfTest, FormatIntegerVariousTypes) {
    // Test with different integer types
    int8_t i8 = 42;
    int16_t i16 = 42;
    int32_t i32 = 42;
    int64_t i64 = 42;
    
    std::string result = fmt::sprintf("%d", i8);
    EXPECT_EQ(result, "42");
    
    result = fmt::sprintf("%d", i16);
    EXPECT_EQ(result, "42");
    
    result = fmt::sprintf("%d", i32);
    EXPECT_EQ(result, "42");
    
    result = fmt::sprintf("%d", i64);
    EXPECT_EQ(result, "42");
}