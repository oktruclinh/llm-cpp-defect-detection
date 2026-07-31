#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>

// Test fixture to provide common setup for format tests
class FormatOctalTest : public ::testing::Test {
protected:
    std::string format_with_specs(const char* format_str, int value, bool alt = false, int precision = -1) {
        fmt::format_specs specs;
        specs.alt = alt;
        if (precision >= 0) {
            specs.precision = precision;
        }
        specs.type = 'o';
        
        // We'll test the internal behavior by examining the formatted output
        // since the on_oct() method is internal to the formatter
        if (alt && precision == -1) {
            return fmt::format(format_str, value);
        } else if (alt && precision >= 0) {
            return fmt::format(format_str, value);
        } else {
            return fmt::format(format_str, value);
        }
    }
};

// Test normal octal formatting without alt flag
TEST_F(FormatOctalTest, NormalOctalFormatting) {
    // Test basic octal formatting
    std::string result = fmt::format("{:o}", 10);
    EXPECT_EQ(result, "12");
    
    result = fmt::format("{:o}", 64);
    EXPECT_EQ(result, "100");
    
    result = fmt::format("{:o}", 255);
    EXPECT_EQ(result, "377");
}

// Test octal formatting with alt flag (should add leading 0)
TEST_F(FormatOctalTest, OctalWithAltFlag) {
    // Test with alt flag and value != 0
    std::string result = fmt::format("{:#o}", 10);
    EXPECT_EQ(result, "012");
    
    result = fmt::format("{:#o}", 64);
    EXPECT_EQ(result, "0100");
    
    result = fmt::format("{:#o}", 255);
    EXPECT_EQ(result, "0377");
}

// Test octal formatting with alt flag and precision
TEST_F(FormatOctalTest, OctalWithAltFlagAndPrecision) {
    // Test with alt flag and precision greater than number of digits
    std::string result = fmt::format("{:#.5o}", 10);
    EXPECT_EQ(result, "00012");
    
    // Test with alt flag and precision equal to number of digits
    result = fmt::format("{:#.3o}", 10);
    EXPECT_EQ(result, "012");
    
    // Test with alt flag and precision less than number of digits
    result = fmt::format("{:#.2o}", 10);
    EXPECT_EQ(result, "012");  // Should not add leading 0 when precision <= num_digits
}

// Test edge cases: zero value
TEST_F(FormatOctalTest, OctalZeroValue) {
    std::string result = fmt::format("{:o}", 0);
    EXPECT_EQ(result, "0");
    
    result = fmt::format("{:#o}", 0);
    EXPECT_EQ(result, "0");  // Should not add leading 0 for zero
    
    result = fmt::format("{:#.5o}", 0);
    EXPECT_EQ(result, "00000");
}

// Test edge cases: negative values
TEST_F(FormatOctalTest, OctalNegativeValues) {
    std::string result = fmt::format("{:o}", -10);
    EXPECT_EQ(result, "-12");
    
    result = fmt::format("{:#o}", -10);
    EXPECT_EQ(result, "-012");
}

// Test octal formatting with various precision values
TEST_F(FormatOctalTest, OctalPrecisionVariations) {
    // Test precision without alt flag
    std::string result = fmt::format("{:.5o}", 10);
    EXPECT_EQ(result, "00012");
    
    result = fmt::format("{:.2o}", 10);
    EXPECT_EQ(result, "12");
    
    result = fmt::format("{:.1o}", 10);
    EXPECT_EQ(result, "12");
    
    result = fmt::format("{:.0o}", 10);
    EXPECT_EQ(result, "12");
}

// Test that precision is handled correctly with alt flag
TEST_F(FormatOctalTest, OctalPrecisionWithAltFlag) {
    // When precision is greater than number of digits, leading zeros should be added
    std::string result = fmt::format("{:#.5o}", 7);
    EXPECT_EQ(result, "00007");
    
    // When precision equals number of digits, no extra zeros should be added
    result = fmt::format("{:#.3o}", 7);
    EXPECT_EQ(result, "007");
    
    // When precision is less than number of digits, no extra zeros should be added
    result = fmt::format("{:#.2o}", 7);
    EXPECT_EQ(result, "07");
}

// Test large values
TEST_F(FormatOctalTest, OctalLargeValues) {
    std::string result = fmt::format("{:o}", 1000);
    EXPECT_EQ(result, "1750");
    
    result = fmt::format("{:#o}", 1000);
    EXPECT_EQ(result, "01750");
    
    result = fmt::format("{:#.10o}", 1000);
    EXPECT_EQ(result, "0000001750");
}

// Test boundary values
TEST_F(FormatOctalTest, OctalBoundaryValues) {
    // Test maximum values that fit in standard types
    std::string result = fmt::format("{:o}", 2147483647);  // 2^31 - 1
    EXPECT_EQ(result, "17777777777");
    
    result = fmt::format("{:#o}", 2147483647);
    EXPECT_EQ(result, "017777777777");
    
    // Test minimum negative value
    result = fmt::format("{:o}", -2147483648);  // -2^31
    EXPECT_EQ(result, "-20000000000");
}