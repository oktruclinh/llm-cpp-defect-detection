#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Test the dynamic_format_specs functionality through the public API
TEST(DynamicFormatSpecsTest, BasicFormatting) {
    // Test basic integer formatting
    std::string result = fmt::format("{:5d}", 42);
    EXPECT_EQ("   42", result);
    
    // Test basic string formatting
    result = fmt::format("{:>10s}", "hello");
    EXPECT_EQ("     hello", result);
}

TEST(DynamicFormatSpecsTest, PrecisionFormatting) {
    // Test floating point precision
    std::string result = fmt::format("{:.2f}", 3.14159);
    EXPECT_EQ("3.14", result);
    
    // Test string precision
    result = fmt::format("{:.3s}", "hello world");
    EXPECT_EQ("hel", result);
}

TEST(DynamicFormatSpecsTest, WidthFormatting) {
    // Test width specification
    std::string result = fmt::format("{:10d}", 123);
    EXPECT_EQ("     123", result);
    
    // Test negative width (left alignment)
    result = fmt::format("{:<10d}", 123);
    EXPECT_EQ("123       ", result);
}

TEST(DynamicFormatSpecsTest, FillCharacter) {
    // Test custom fill character
    std::string result = fmt::format("{:*<10d}", 123);
    EXPECT_EQ("123*******", result);
    
    result = fmt::format("{:*>10d}", 123);
    EXPECT_EQ("*******123", result);
}

TEST(DynamicFormatSpecsTest, ZeroPadding) {
    // Test zero padding
    std::string result = fmt::format("{:05d}", 42);
    EXPECT_EQ("00042", result);
    
    result = fmt::format("{:0>5d}", 42);
    EXPECT_EQ("00042", result);
}

TEST(DynamicFormatSpecsTest, Alignment) {
    // Test various alignment options
    std::string result = fmt::format("{:<10s}", "test");
    EXPECT_EQ("test      ", result);
    
    result = fmt::format("{:>10s}", "test");
    EXPECT_EQ("      test", result);
    
    result = fmt::format("{:^10s}", "test");
    EXPECT_EQ("   test   ", result);
    
    result = fmt::format("{:=<10d}", 42);
    EXPECT_EQ("42========", result);
}

TEST(DynamicFormatSpecsTest, ComplexFormatting) {
    // Test complex format spec combinations
    std::string result = fmt::format("{:*>10.2f}", 123.456);
    EXPECT_EQ("****123.46", result);
    
    result = fmt::format("{:<010d}", 42);
    EXPECT_EQ("0000000042", result);
    
    result = fmt::format("{:^10.1f}", 123.456);
    EXPECT_EQ("   123.5   ", result);
}

TEST(DynamicFormatSpecsTest, EdgeCases) {
    // Test empty string
    std::string result = fmt::format("{:5s}", "");
    EXPECT_EQ("     ", result);
    
    // Test zero value
    result = fmt::format("{:5d}", 0);
    EXPECT_EQ("    0", result);
    
    // Test negative value
    result = fmt::format("{:5d}", -42);
    EXPECT_EQ("   -42", result);
    
    // Test large number
    result = fmt::format("{:10d}", 1234567890);
    EXPECT_EQ("1234567890", result);
    
    // Test precision with zero
    result = fmt::format("{:.0f}", 3.14159);
    EXPECT_EQ("3", result);
}

TEST(DynamicFormatSpecsTest, StringPrecisionEdgeCases) {
    // Test precision that exceeds string length
    std::string result = fmt::format("{:.10s}", "hi");
    EXPECT_EQ("hi", result);
    
    // Test precision of zero
    result = fmt::format("{:.0s}", "hello");
    EXPECT_EQ("", result);
}

TEST(DynamicFormatSpecsTest, WidthEdgeCases) {
    // Test width of zero
    std::string result = fmt::format("{:0d}", 42);
    EXPECT_EQ("42", result);
    
    // Test very large width
    result = fmt::format("{:100d}", 42);
    EXPECT_EQ(std::string(97, ' ') + "42", result);
}

TEST(DynamicFormatSpecsTest, MixedSpecs) {
    // Test mixed alignment and width
    std::string result = fmt::format("{:<10.2f}", 123.456);
    EXPECT_EQ("123.46    ", result);
    
    result = fmt::format("{:>10.2f}", 123.456);
    EXPECT_EQ("    123.46", result);
    
    result = fmt::format("{:^10.2f}", 123.456);
    EXPECT_EQ("  123.46  ", result);
}

TEST(DynamicFormatSpecsTest, CharacterFormatting) {
    // Test character formatting
    std::string result = fmt::format("{:5c}", 'A');
    EXPECT_EQ("    A", result);
    
    result = fmt::format("{:>5c}", 'B');
    EXPECT_EQ("    B", result);
}

TEST(DynamicFormatSpecsTest, BooleanFormatting) {
    // Test boolean formatting
    std::string result = fmt::format("{:5b}", true);
    EXPECT_EQ("   true", result);
    
    result = fmt::format("{:5b}", false);
    EXPECT_EQ("  false", result);
}

TEST(DynamicFormatSpecsTest, ScientificNotation) {
    // Test scientific notation
    std::string result = fmt::format("{:.2e}", 1234.56);
    EXPECT_EQ("1.23e+03", result);
    
    result = fmt::format("{:.0e}", 1234.56);
    EXPECT_EQ("1e+03", result);
}

TEST(DynamicFormatSpecsTest, HexadecimalFormatting) {
    // Test hexadecimal formatting
    std::string result = fmt::format("{:x}", 255);
    EXPECT_EQ("ff", result);
    
    result = fmt::format("{:X}", 255);
    EXPECT_EQ("FF", result);
    
    result = fmt::format("{:#x}", 255);
    EXPECT_EQ("0xff", result);
}

TEST(DynamicFormatSpecsTest, OctalFormatting) {
    // Test octal formatting
    std::string result = fmt::format("{:o}", 64);
    EXPECT_EQ("100", result);
    
    result = fmt::format("{:#o}", 64);
    EXPECT_EQ("0100", result);
}

TEST(DynamicFormatSpecsTest, BinaryFormatting) {
    // Test binary formatting
    std::string result = fmt::format("{:b}", 64);
    EXPECT_EQ("1000000", result);
    
    result = fmt::format("{:#b}", 64);
    EXPECT_EQ("0b1000000", result);
}