#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test that numeric alignment with sign is handled correctly for integers
TEST(NumericAlignmentTest, IntegerWithSign) {
    // Positive number with '+' sign and numeric alignment
    std::string result = fmt::format("{:+06d}", 42);
    EXPECT_EQ(result, "+00042");
    
    // Negative number with numeric alignment
    result = fmt::format("{:06d}", -42);
    EXPECT_EQ(result, "-00042");
    
    // Positive number with space sign and numeric alignment
    result = fmt::format("{: 06d}", 42);
    EXPECT_EQ(result, " 00042");
}

// Test that numeric alignment with sign and width adjustment works
TEST(NumericAlignmentTest, SignWithWidthAdjustment) {
    // Sign should be placed before padding, width should be reduced by 1
    std::string result = fmt::format("{:+06d}", 7);
    EXPECT_EQ(result, "+00007");
    EXPECT_EQ(result.size(), 6u);
    
    // Zero width with sign
    result = fmt::format("{:+0d}", 7);
    EXPECT_EQ(result, "+7");
}

// Test default alignment (ALIGN_DEFAULT) behavior for non-numeric types
TEST(NumericAlignmentTest, DefaultAlignmentNonNumeric) {
    // Default alignment for strings should be left-aligned
    std::string result = fmt::format("{:10}", "hello");
    EXPECT_EQ(result, "hello     ");
    
    // Default alignment for integers should be right-aligned
    result = fmt::format("{:10}", 42);
    EXPECT_EQ(result, "        42");
}

// Test that ALIGN_NUMERIC is converted to ALIGN_RIGHT when no sign is present
TEST(NumericAlignmentTest, NumericToRightWithoutSign) {
    // Zero-padded integer without sign should be right-aligned
    std::string result = fmt::format("{:010d}", 123);
    EXPECT_EQ(result, "0000000123");
    
    // Zero-padded float without sign
    result = fmt::format("{:010.2f}", 3.14);
    EXPECT_EQ(result, "0000003.14");
}

// Test sign handling with width and numeric alignment
TEST(NumericAlignmentTest, SignWithWidthAndNumeric) {
    // Sign should be placed before the number, not before padding
    std::string result = fmt::format("{:+08d}", 42);
    EXPECT_EQ(result, "+0000042");
    EXPECT_EQ(result[0], '+');
    
    // Negative number with width
    result = fmt::format("{:08d}", -42);
    EXPECT_EQ(result, "-0000042");
    EXPECT_EQ(result[0], '-');
}

// Test edge case: zero value with sign and numeric alignment
TEST(NumericAlignmentTest, ZeroWithSign) {
    std::string result = fmt::format("{:+05d}", 0);
    EXPECT_EQ(result, "+0000");
    
    result = fmt::format("{:05d}", 0);
    EXPECT_EQ(result, "00000");
}

// Test edge case: maximum width with sign
TEST(NumericAlignmentTest, LargeWidthWithSign) {
    std::string result = fmt::format("{:+020d}", 42);
    EXPECT_EQ(result, "+00000000000000000042");
    EXPECT_EQ(result.size(), 20u);
}

// Test that ALIGN_DEFAULT for numeric types becomes ALIGN_RIGHT
TEST(NumericAlignmentTest, DefaultNumericBecomesRight) {
    // Default alignment for integers should be right-aligned
    std::string result = fmt::format("{:10d}", 42);
    EXPECT_EQ(result, "        42");
    
    // Default alignment for floats should be right-aligned
    result = fmt::format("{:10.2f}", 3.14);
    EXPECT_EQ(result, "      3.14");
}

// Test sign handling with no width specified
TEST(NumericAlignmentTest, SignWithoutWidth) {
    std::string result = fmt::format("{:+d}", 42);
    EXPECT_EQ(result, "+42");
    
    result = fmt::format("{: d}", 42);
    EXPECT_EQ(result, " 42");
    
    result = fmt::format("{:+d}", -42);
    EXPECT_EQ(result, "-42");
}

// Test that sign is not duplicated when width is exactly the number length
TEST(NumericAlignmentTest, SignExactWidth) {
    std::string result = fmt::format("{:+3d}", 42);
    EXPECT_EQ(result, "+42");
    EXPECT_EQ(result.size(), 3u);
    
    result = fmt::format("{:+4d}", 42);
    EXPECT_EQ(result, " +42");
    EXPECT_EQ(result.size(), 4u);
}

// Test multiple signs in format string (should use last one)
TEST(NumericAlignmentTest, MultipleSigns) {
    std::string result = fmt::format("{:+-06d}", 42);
    EXPECT_EQ(result, "+00042");
    
    result = fmt::format("{:-+06d}", 42);
    EXPECT_EQ(result, "+00042");
}

// Test that ALIGN_NUMERIC with sign works for different numeric types
TEST(NumericAlignmentTest, NumericAlignmentDifferentTypes) {
    // Integer
    std::string result = fmt::format("{:+08d}", 123);
    EXPECT_EQ(result, "+0000123");
    
    // Float
    result = fmt::format("{:+08.2f}", 3.14);
    EXPECT_EQ(result, "+0003.14");
    
    // Double
    result = fmt::format("{:+08.2f}", 3.14159);
    EXPECT_EQ(result, "+0003.14");
}

// Test edge case: minimum width with sign
TEST(NumericAlignmentTest, MinimumWidthWithSign) {
    std::string result = fmt::format("{:+1d}", 42);
    EXPECT_EQ(result, "+42");
    EXPECT_EQ(result.size(), 3u);
    
    result = fmt::format("{:+2d}", 42);
    EXPECT_EQ(result, "+42");
    EXPECT_EQ(result.size(), 3u);
}

// Test that sign is correctly placed when using fill character
TEST(NumericAlignmentTest, SignWithFillCharacter) {
    std::string result = fmt::format("{:*>+06d}", 42);
    EXPECT_EQ(result, "**+042");
    
    result = fmt::format("{:*<+06d}", 42);
    EXPECT_EQ(result, "+042**");
}