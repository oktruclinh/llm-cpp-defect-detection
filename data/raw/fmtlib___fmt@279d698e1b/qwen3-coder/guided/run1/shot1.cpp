#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <stdexcept>

TEST(FmtFormatTest, TestFormatRightAligned) {
    // Test normal right-aligned formatting
    std::string result = fmt::format("{:>10}", "hello");
    EXPECT_EQ(result, "     hello");
    
    // Test with width larger than string
    result = fmt::format("{:>5}", "hi");
    EXPECT_EQ(result, "   hi");
    
    // Test with width equal to string
    result = fmt::format("{:>5}", "hello");
    EXPECT_EQ(result, "hello");
    
    // Test with zero width
    result = fmt::format("{:>0}", "test");
    EXPECT_EQ(result, "test");
    
    // Test with negative width (should be treated as positive)
    result = fmt::format("{:>-5}", "test");
    EXPECT_EQ(result, "  test");
    
    // Test with empty string
    result = fmt::format("{:>5}", "");
    EXPECT_EQ(result, "    ");
    
    // Test with single character
    result = fmt::format("{:>3}", "a");
    EXPECT_EQ(result, "  a");
    
    // Test with unicode characters
    result = fmt::format("{:>5}", "αβ");
    EXPECT_EQ(result, "  αβ");
    
    // Test with numbers
    result = fmt::format("{:>5}", 42);
    EXPECT_EQ(result, "   42");
    
    // Test with negative numbers
    result = fmt::format("{:>5}", -42);
    EXPECT_EQ(result, "  -42");
    
    // Test with floating point
    result = fmt::format("{:>8.2f}", 3.14159);
    EXPECT_EQ(result, "    3.14");
    
    // Test with precision and width
    result = fmt::format("{:>10.3f}", 123.456789);
    EXPECT_EQ(result, "   123.457");
}

TEST(FmtFormatTest, TestFormatRightAlignedEdgeCases) {
    // Test with very large width
    std::string result = fmt::format("{:>100}", "test");
    EXPECT_EQ(result.length(), 100);
    EXPECT_TRUE(result.substr(96) == "test");
    
    // Test with width that causes padding
    result = fmt::format("{:>1000}", "x");
    EXPECT_EQ(result.length(), 1000);
    EXPECT_TRUE(result.substr(999) == "x");
    
    // Test with zero padding
    result = fmt::format("{:>0}", "");
    EXPECT_EQ(result, "");
    
    // Test with empty format string
    result = fmt::format("{:>", "test");
    EXPECT_EQ(result, "test");
    
    // Test with width of 1
    result = fmt::format("{:>1}", "test");
    EXPECT_EQ(result, "test");
    
    // Test with width of 2
    result = fmt::format("{:>2}", "test");
    EXPECT_EQ(result, "test");
    
    // Test with width of 3
    result = fmt::format("{:>3}", "test");
    EXPECT_EQ(result, "test");
    
    // Test with width of 4
    result = fmt::format("{:>4}", "test");
    EXPECT_EQ(result, "test");
    
    // Test with width of 5
    result = fmt::format("{:>5}", "test");
    EXPECT_EQ(result, " test");
}

TEST(FmtFormatTest, TestFormatRightAlignedSpecialCases) {
    // Test with special characters
    result = fmt::format("{:>5}", "hello\nworld");
    EXPECT_EQ(result, "hello\nworld");
    
    // Test with tab characters
    result = fmt::format("{:>5}", "hello\tworld");
    EXPECT_EQ(result, "hello\tworld");
    
    // Test with multiple spaces
    result = fmt::format("{:>5}", "hello world");
    EXPECT_EQ(result, "hello world");
    
    // Test with leading spaces in original
    result = fmt::format("{:>5}", "  hello");
    EXPECT_EQ(result, "  hello");
    
    // Test with trailing spaces in original
    result = fmt::format("{:>5}", "hello  ");
    EXPECT_EQ(result, "hello  ");
    
    // Test with mixed whitespace
    result = fmt::format("{:>5}", " \thello ");
    EXPECT_EQ(result, " \thello ");
}