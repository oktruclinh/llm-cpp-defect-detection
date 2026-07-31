#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <fmt/format.h>
#include <string>
#include <stdexcept>

TEST(PrintfTest, FormatIntegerWithSign) {
    // Test normal integer formatting with sign
    std::string result = fmt::sprintf("%d", 42);
    EXPECT_EQ(result, "42");
    
    result = fmt::sprintf("%d", -42);
    EXPECT_EQ(result, "-42");
}

TEST(PrintfTest, FormatIntegerWithFlags) {
    // Test formatting with various flags
    std::string result = fmt::sprintf("%05d", 42);
    EXPECT_EQ(result, "00042");
    
    result = fmt::sprintf("%+d", 42);
    EXPECT_EQ(result, "+42");
    
    result = fmt::sprintf("% d", 42);
    EXPECT_EQ(result, " 42");
    
    result = fmt::sprintf("%-5d", 42);
    EXPECT_EQ(result, "42   ");
}

TEST(PrintfTest, FormatIntegerWithAltForm) {
    // Test alternative form with #
    std::string result = fmt::sprintf("%#x", 255);
    EXPECT_EQ(result, "0xff");
    
    result = fmt::sprintf("%#o", 64);
    EXPECT_EQ(result, "0100");
}

TEST(PrintfTest, FormatIntegerEdgeCases) {
    // Test edge cases
    std::string result = fmt::sprintf("%d", 0);
    EXPECT_EQ(result, "0");
    
    result = fmt::sprintf("%d", INT_MAX);
    EXPECT_EQ(result, std::to_string(INT_MAX));
    
    result = fmt::sprintf("%d", INT_MIN);
    EXPECT_EQ(result, std::to_string(INT_MIN));
}

TEST(PrintfTest, FormatIntegerAlignment) {
    // Test alignment behavior
    std::string result = fmt::sprintf("%10d", 42);
    EXPECT_EQ(result, "        42");
    
    result = fmt::sprintf("%-10d", 42);
    EXPECT_EQ(result, "42        ");
    
    // Test numeric alignment (should behave like right alignment)
    result = fmt::sprintf("%10d", 42);
    EXPECT_EQ(result, "        42");
}

TEST(PrintfTest, FormatIntegerZeroPadding) {
    // Test zero padding behavior
    std::string result = fmt::sprintf("%05d", 42);
    EXPECT_EQ(result, "00042");
    
    result = fmt::sprintf("%010d", -42);
    EXPECT_EQ(result, "-000000042");
}

TEST(PrintfTest, FormatIntegerWidthAndPrecision) {
    // Test width specification
    std::string result = fmt::sprintf("%10d", 42);
    EXPECT_EQ(result, "        42");
    
    // Test precision (should be ignored for integers)
    result = fmt::sprintf("%.5d", 42);
    EXPECT_EQ(result, "00042");
}

TEST(PrintfTest, FormatIntegerVariousTypes) {
    // Test different integer types
    std::string result = fmt::sprintf("%d", static_cast<int>(42));
    EXPECT_EQ(result, "42");
    
    result = fmt::sprintf("%d", static_cast<long>(42));
    EXPECT_EQ(result, "42");
    
    result = fmt::sprintf("%d", static_cast<long long>(42));
    EXPECT_EQ(result, "42");
}

TEST(PrintfTest, FormatIntegerNegativeValues) {
    // Test negative values with various flags
    std::string result = fmt::sprintf("%d", -42);
    EXPECT_EQ(result, "-42");
    
    result = fmt::sprintf("%+d", -42);
    EXPECT_EQ(result, "-42");
    
    result = fmt::sprintf("% d", -42);
    EXPECT_EQ(result, "-42");
    
    result = fmt::sprintf("%05d", -42);
    EXPECT_EQ(result, "-0042");
}

TEST(PrintfTest, FormatIntegerSpecialCases) {
    // Test special cases
    std::string result = fmt::sprintf("%d", 0);
    EXPECT_EQ(result, "0");
    
    result = fmt::sprintf("%+d", 0);
    EXPECT_EQ(result, "+0");
    
    result = fmt::sprintf("% d", 0);
    EXPECT_EQ(result, " 0");
}