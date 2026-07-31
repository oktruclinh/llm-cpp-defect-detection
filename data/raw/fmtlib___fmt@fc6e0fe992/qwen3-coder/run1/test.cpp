#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>

TEST(AlignSpecTest, NumericAlignmentWithSign) {
    // Test case where align is numeric and sign is present
    std::string result = fmt::format("{:+d}", 42);
    EXPECT_EQ(result, "+42");
    
    // Test case where align is numeric and sign is negative
    result = fmt::format("{:+d}", -42);
    EXPECT_EQ(result, "-42");
}

TEST(AlignSpecTest, DefaultAlignmentWithSign) {
    // Test case where align is default and sign is present
    std::string result = fmt::format("{:d}", 42);
    EXPECT_EQ(result, "42");
    
    result = fmt::format("{:d}", -42);
    EXPECT_EQ(result, "-42");
}

TEST(AlignSpecTest, RightAlignmentWithSign) {
    // Test case where align is right and sign is present
    std::string result = fmt::format("{:>d}", 42);
    EXPECT_EQ(result, "42");
    
    result = fmt::format("{:>d}", -42);
    EXPECT_EQ(result, "-42");
}

TEST(AlignSpecTest, WidthAdjustmentWithNumericAlign) {
    // Test numeric alignment with width specification
    std::string result = fmt::format("{:+>5d}", 42);
    EXPECT_EQ(result, "+0042");
    
    result = fmt::format("{:+>5d}", -42);
    EXPECT_EQ(result, "-0042");
}

TEST(AlignSpecTest, WidthAdjustmentWithDefaultAlign) {
    // Test default alignment with width specification
    std::string result = fmt::format("{:>5d}", 42);
    EXPECT_EQ(result, "   42");
    
    result = fmt::format("{:>5d}", -42);
    EXPECT_EQ(result, "  -42");
}

TEST(AlignSpecTest, ZeroPaddingWithSign) {
    // Test zero padding with sign
    std::string result = fmt::format("{:+05d}", 42);
    EXPECT_EQ(result, "+0042");
    
    result = fmt::format("{:+05d}", -42);
    EXPECT_EQ(result, "-0042");
}

TEST(AlignSpecTest, EdgeCaseZero) {
    // Test zero value with different alignments
    std::string result = fmt::format("{:+d}", 0);
    EXPECT_EQ(result, "+0");
    
    result = fmt::format("{:d}", 0);
    EXPECT_EQ(result, "0");
    
    result = fmt::format("{:>5d}", 0);
    EXPECT_EQ(result, "    0");
}

TEST(AlignSpecTest, EdgeCaseNegativeZero) {
    // Test negative zero (though this is not a standard case)
    std::string result = fmt::format("{:+d}", -0);
    EXPECT_EQ(result, "+0");  // Should behave like positive zero
}

TEST(AlignSpecTest, LargeNumberWithAlignment) {
    // Test large number with alignment
    std::string result = fmt::format("{:+>10d}", 123456789);
    EXPECT_EQ(result, "+123456789");
    
    result = fmt::format("{:>10d}", 123456789);
    EXPECT_EQ(result, "123456789");
}

TEST(AlignSpecTest, StringWithSign) {
    // Test string formatting with sign (should not affect strings)
    std::string result = fmt::format("{:+s}", "hello");
    EXPECT_EQ(result, "hello");  // Strings don't have signs in this context
}