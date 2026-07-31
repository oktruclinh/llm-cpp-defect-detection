#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <stdexcept>

TEST(FmtFormatTest, TestFormatRightAligned) {
    // Test normal case with right alignment
    std::string result = fmt::format("{:>10}", "hello");
    EXPECT_EQ(result, "     hello");
    
    // Test with zero padding
    result = fmt::format("{:0>5}", "42");
    EXPECT_EQ(result, "00042");
    
    // Test with negative number
    result = fmt::format("{:>-5}", -42);
    EXPECT_EQ(result, "   -42");
    
    // Test with empty string
    result = fmt::format("{:>5}", "");
    EXPECT_EQ(result, "     ");
    
    // Test with zero width
    result = fmt::format("{:>0}", "test");
    EXPECT_EQ(result, "test");
    
    // Test with width smaller than string
    result = fmt::format("{:>3}", "hello");
    EXPECT_EQ(result, "hello");
    
    // Test with unicode string
    result = fmt::format("{:>6}", "café");
    EXPECT_EQ(result, "   café");
    
    // Test with different character type
    std::wstring wresult = fmt::format(L"{:>5}", L"hello");
    EXPECT_EQ(wresult, L"  hello");
}

TEST(FmtFormatTest, TestFormatRightAlignedEdgeCases) {
    // Test with very large width
    std::string result = fmt::format("{:>100}", "a");
    EXPECT_EQ(result.length(), 100);
    EXPECT_EQ(result.substr(99), "a");
    
    // Test with negative width (should be treated as zero)
    result = fmt::format("{:>-10}", "test");
    EXPECT_EQ(result, "test");
    
    // Test with zero width and empty string
    result = fmt::format("{:>0}", "");
    EXPECT_EQ(result, "");
    
    // Test with precision specification
    result = fmt::format("{:.3>5}", "hello");
    EXPECT_EQ(result, "  hel");
    
    // Test with mixed alignment and width
    result = fmt::format("{:*>10}", "test");
    EXPECT_EQ(result, "******test");
    
    // Test with negative number and padding
    result = fmt::format("{:*>+10}", 42);
    EXPECT_EQ(result, "*****+42");
    
    // Test with negative number and right alignment
    result = fmt::format("{:>-10}", -42);
    EXPECT_EQ(result, "       -42");
}

TEST(FmtFormatTest, TestFormatRightAlignedSpecialCases) {
    // Test with boolean values
    std::string result = fmt::format("{:>5}", true);
    EXPECT_EQ(result, "   true");
    
    result = fmt::format("{:>5}", false);
    EXPECT_EQ(result, "  false");
    
    // Test with floating point numbers
    result = fmt::format("{:>10.2f}", 123.456);
    EXPECT_EQ(result, "    123.46");
    
    // Test with scientific notation
    result = fmt::format("{:>10e}", 123.456);
    EXPECT_EQ(result.length(), 10);
    
    // Test with hex formatting
    result = fmt::format("{:>10x}", 255);
    EXPECT_EQ(result, "       ff");
    
    // Test with octal formatting
    result = fmt::format("{:>10o}", 64);
    EXPECT_EQ(result, "        100");
}

TEST(FmtFormatTest, TestFormatRightAlignedErrorConditions) {
    // These tests verify that the formatting works correctly
    // and don't test actual error conditions since the API
    // is designed to handle all inputs gracefully
    
    // Test that formatting doesn't crash with various inputs
    std::string result;
    
    // Test with very long string
    std::string long_str(1000, 'x');
    result = fmt::format("{:>1000}", long_str);
    EXPECT_EQ(result.length(), 1000);
    EXPECT_EQ(result.substr(0, 1000), long_str);
    
    // Test with special characters
    result = fmt::format("{:>10}", "hello\nworld");
    EXPECT_EQ(result.length(), 10);
    
    // Test with tab characters
    result = fmt::format("{:>10}", "hello\tworld");
    EXPECT_EQ(result.length(), 10);
    
    // Test with null characters (should be handled gracefully)
    result = fmt::format("{:>5}", std::string("hello\0world", 11));
    EXPECT_EQ(result.length(), 5);
}