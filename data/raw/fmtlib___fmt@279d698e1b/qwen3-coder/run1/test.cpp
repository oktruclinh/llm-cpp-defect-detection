#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <limits>

TEST(FmtFormatTest, FormatIntWithPrefix) {
    // Test normal case with positive number
    std::string result = fmt::format("{}", 42);
    EXPECT_EQ(result, "42");
    
    // Test normal case with negative number
    result = fmt::format("{}", -42);
    EXPECT_EQ(result, "-42");
    
    // Test zero
    result = fmt::format("{}", 0);
    EXPECT_EQ(result, "0");
    
    // Test large number
    result = fmt::format("{}", 123456789);
    EXPECT_EQ(result, "123456789");
    
    // Test with width specification
    result = fmt::format("{:10}", 42);
    EXPECT_EQ(result, "        42");
    
    // Test with zero padding
    result = fmt::format("{:010}", 42);
    EXPECT_EQ(result, "0000000042");
    
    // Test with sign
    result = fmt::format("{:+}", 42);
    EXPECT_EQ(result, "+42");
    
    // Test with sign and width
    result = fmt::format("{:+10}", 42);
    EXPECT_EQ(result, "       +42");
    
    // Test with negative sign
    result = fmt::format("{:+}", -42);
    EXPECT_EQ(result, "-42");
    
    // Test with space
    result = fmt::format("{: }", 42);
    EXPECT_EQ(result, " 42");
    
    // Test with space and width
    result = fmt::format("{: 10}", 42);
    EXPECT_EQ(result, "         42");
    
    // Test with space and negative
    result = fmt::format("{: }", -42);
    EXPECT_EQ(result, "-42");
}

TEST(FmtFormatTest, FormatIntEdgeCases) {
    // Test minimum value
    std::string result = fmt::format("{}", std::numeric_limits<int>::min());
    EXPECT_EQ(result, "-2147483648");
    
    // Test maximum value
    result = fmt::format("{}", std::numeric_limits<int>::max());
    EXPECT_EQ(result, "2147483647");
    
    // Test with very large number
    result = fmt::format("{}", 999999999999LL);
    EXPECT_EQ(result, "999999999999");
    
    // Test with very small number
    result = fmt::format("{}", -999999999999LL);
    EXPECT_EQ(result, "-999999999999");
    
    // Test with width larger than number
    result = fmt::format("{:20}", 123);
    EXPECT_EQ(result, "                   123");
    
    // Test with width equal to number
    result = fmt::format("{:3}", 123);
    EXPECT_EQ(result, "123");
    
    // Test with width smaller than number
    result = fmt::format("{:2}", 123);
    EXPECT_EQ(result, "123");
}

TEST(FmtFormatTest, FormatIntAlignment) {
    // Test left alignment
    std::string result = fmt::format("{:<10}", 42);
    EXPECT_EQ(result, "42        ");
    
    // Test right alignment (default)
    result = fmt::format("{:>10}", 42);
    EXPECT_EQ(result, "        42");
    
    // Test center alignment
    result = fmt::format("{:^10}", 42);
    EXPECT_EQ(result, "    42    ");
    
    // Test zero padding with alignment
    result = fmt::format("{:0>10}", 42);
    EXPECT_EQ(result, "0000000042");
    
    // Test zero padding with left alignment
    result = fmt::format("{:0<10}", 42);
    EXPECT_EQ(result, "4200000000");
    
    // Test zero padding with center alignment
    result = fmt::format("{:0^10}", 42);
    EXPECT_EQ(result, "0000420000");
}

TEST(FmtFormatTest, FormatIntSpecialCases) {
    // Test empty string (should not happen with integers, but test robustness)
    std::string result = fmt::format("{:}", 42);
    EXPECT_EQ(result, "42");
    
    // Test with various format specifiers
    result = fmt::format("{:d}", 42);
    EXPECT_EQ(result, "42");
    
    result = fmt::format("{:i}", 42);
    EXPECT_EQ(result, "42");
    
    // Test with hex format
    result = fmt::format("{:x}", 255);
    EXPECT_EQ(result, "ff");
    
    // Test with uppercase hex
    result = fmt::format("{:X}", 255);
    EXPECT_EQ(result, "FF");
    
    // Test with octal
    result = fmt::format("{:o}", 64);
    EXPECT_EQ(result, "100");
    
    // Test with binary
    result = fmt::format("{:b}", 8);
    EXPECT_EQ(result, "1000");
}

TEST(FmtFormatTest, FormatIntErrorConditions) {
    // These tests verify that the function handles edge cases properly
    // The actual error conditions are handled by the format library itself
    
    // Test that negative zero is handled correctly (if applicable)
    std::string result = fmt::format("{}", -0);
    EXPECT_EQ(result, "0");  // Negative zero should display as positive zero
    
    // Test with maximum width
    result = fmt::format("{:999999999}", 42);
    // Should not crash and should produce a long string of spaces followed by 42
    EXPECT_TRUE(result.length() > 100);
    EXPECT_TRUE(result.substr(result.length() - 2) == "42");
}