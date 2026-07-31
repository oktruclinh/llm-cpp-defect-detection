#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <climits>

// Test suite for the on_oct() behavior through fmt::format with 'o' specifier
class OctFormatTest : public ::testing::Test {
protected:
    // Helper to format with octal specifier
    std::string format_oct(const std::string& fmt_str, auto&&... args) {
        return fmt::format(fmt_str, std::forward<decltype(args)>(args)...);
    }
};

// Basic octal formatting without any flags
TEST_F(OctFormatTest, BasicOctal) {
    EXPECT_EQ(fmt::format("{:o}", 0), "0");
    EXPECT_EQ(fmt::format("{:o}", 1), "1");
    EXPECT_EQ(fmt::format("{:o}", 7), "7");
    EXPECT_EQ(fmt::format("{:o}", 8), "10");
    EXPECT_EQ(fmt::format("{:o}", 64), "100");
    EXPECT_EQ(fmt::format("{:o}", 511), "777");
    EXPECT_EQ(fmt::format("{:o}", 512), "1000");
}

// Octal with alternate form (# flag) - this exercises on_oct() prefix logic
TEST_F(OctFormatTest, AlternateForm) {
    // Zero should not get prefix
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");
    
    // Non-zero values get '0' prefix
    EXPECT_EQ(fmt::format("{:#o}", 1), "01");
    EXPECT_EQ(fmt::format("{:#o}", 7), "07");
    EXPECT_EQ(fmt::format("{:#o}", 8), "010");
    EXPECT_EQ(fmt::format("{:#o}", 64), "0100");
    EXPECT_EQ(fmt::format("{:#o}", 511), "0777");
    EXPECT_EQ(fmt::format("{:#o}", 512), "01000");
}

// Alternate form with precision - exercises the precision <= num_digits check
TEST_F(OctFormatTest, AlternateFormWithPrecision) {
    // Precision less than number of digits: prefix should be added
    EXPECT_EQ(fmt::format("{:#.2o}", 8), "010");   // 8 = 10 octal, 2 digits, precision 2 <= 2
    EXPECT_EQ(fmt::format("{:#.1o}", 8), "010");   // precision 1 < 2 digits
    
    // Precision greater than number of digits: prefix should NOT be added
    EXPECT_EQ(fmt::format("{:#.3o}", 8), "010");   // precision 3 > 2 digits, no prefix
    EXPECT_EQ(fmt::format("{:#.4o}", 8), "0010");  // precision 4 > 2 digits, no prefix
    
    // Edge case: precision equals number of digits
    EXPECT_EQ(fmt::format("{:#.2o}", 8), "010");   // precision 2 == 2 digits, prefix added
    EXPECT_EQ(fmt::format("{:#.3o}", 64), "0100"); // 64 = 100 octal, 3 digits, precision 3 == 3
}

// Alternate form with zero value and precision
TEST_F(OctFormatTest, AlternateFormZeroWithPrecision) {
    // Zero with alternate form: no prefix, precision controls padding
    EXPECT_EQ(fmt::format("{:#.0o}", 0), "0");
    EXPECT_EQ(fmt::format("{:#.1o}", 0), "0");
    EXPECT_EQ(fmt::format("{:#.2o}", 0), "00");
    EXPECT_EQ(fmt::format("{:#.3o}", 0), "000");
}

// Width and alignment with octal
TEST_F(OctFormatTest, WidthAndAlignment) {
    EXPECT_EQ(fmt::format("{:5o}", 42), "   52");  // 42 = 52 octal
    EXPECT_EQ(fmt::format("{:<5o}", 42), "52   ");
    EXPECT_EQ(fmt::format("{:^5o}", 42), " 52  ");
    EXPECT_EQ(fmt::format("{:#5o}", 42), "  052");
    EXPECT_EQ(fmt::format("{:#<5o}", 42), "052##");
}

// Negative numbers (should not happen with unsigned, but signed ints)
TEST_F(OctFormatTest, NegativeNumbers) {
    // Negative numbers are formatted as two's complement for large types
    // This tests the abs_value handling
    EXPECT_EQ(fmt::format("{:o}", -1), "37777777777");  // Assuming 32-bit int
    EXPECT_EQ(fmt::format("{:#o}", -1), "037777777777");
}

// Large numbers to test digit counting
TEST_F(OctFormatTest, LargeNumbers) {
    EXPECT_EQ(fmt::format("{:o}", INT_MAX), "17777777777");
    EXPECT_EQ(fmt::format("{:#o}", INT_MAX), "017777777777");
    EXPECT_EQ(fmt::format("{:o}", UINT_MAX), "37777777777");
    EXPECT_EQ(fmt::format("{:#o}", UINT_MAX), "037777777777");
}

// Precision without alternate form
TEST_F(OctFormatTest, PrecisionOnly) {
    EXPECT_EQ(fmt::format("{:.0o}", 0), "");
    EXPECT_EQ(fmt::format("{:.1o}", 0), "0");
    EXPECT_EQ(fmt::format("{:.2o}", 0), "00");
    EXPECT_EQ(fmt::format("{:.3o}", 42), "052");  // 42 = 52 octal, padded to 3
    EXPECT_EQ(fmt::format("{:.5o}", 42), "00052");
}

// Combined width, precision, and alternate form
TEST_F(OctFormatTest, CombinedFlags) {
    EXPECT_EQ(fmt::format("{:#8.5o}", 42), "   00052");
    EXPECT_EQ(fmt::format("{:#<8.5o}", 42), "00052###");
    EXPECT_EQ(fmt::format("{:#>8.5o}", 42), "###00052");
    EXPECT_EQ(fmt::format("{:#^8.5o}", 42), "#00052##");
    
    // Precision > num_digits: no prefix
    EXPECT_EQ(fmt::format("{:#8.6o}", 42), "  000052");  // 6 > 2 digits, no prefix
    EXPECT_EQ(fmt::format("{:#8.2o}", 42), "     052");  // 2 == 2 digits, prefix added
}

// Edge case: value exactly at power of 8 boundaries
TEST_F(OctFormatTest, PowerOfEightBoundaries) {
    EXPECT_EQ(fmt::format("{:o}", 7), "7");
    EXPECT_EQ(fmt::format("{:o}", 8), "10");
    EXPECT_EQ(fmt::format("{:o}", 63), "77");
    EXPECT_EQ(fmt::format("{:o}", 64), "100");
    EXPECT_EQ(fmt::format("{:o}", 511), "777");
    EXPECT_EQ(fmt::format("{:o}", 512), "1000");
    
    // With alternate form
    EXPECT_EQ(fmt::format("{:#o}", 7), "07");
    EXPECT_EQ(fmt::format("{:#o}", 8), "010");
    EXPECT_EQ(fmt::format("{:#o}", 63), "077");
    EXPECT_EQ(fmt::format("{:#o}", 64), "0100");
    EXPECT_EQ(fmt::format("{:#o}", 511), "0777");
    EXPECT_EQ(fmt::format("{:#o}", 512), "01000");
}

// Test with unsigned long long for maximum range
TEST_F(OctFormatTest, UnsignedLongLong) {
    unsigned long long val = 0xFFFFFFFFFFFFFFFFULL;
    std::string result = fmt::format("{:o}", val);
    // 64-bit max in octal is 1777777777777777777777 (22 digits)
    EXPECT_EQ(result.size(), 22);
    EXPECT_EQ(result[0], '1');
    EXPECT_EQ(result, "1777777777777777777777");
    
    // With alternate form
    result = fmt::format("{:#o}", val);
    EXPECT_EQ(result.size(), 23);
    EXPECT_EQ(result[0], '0');
    EXPECT_EQ(result, "01777777777777777777777");
}

// Test that zero with alternate form and various precisions works correctly
TEST_F(OctFormatTest, ZeroEdgeCases) {
    // Zero with alternate form: no prefix regardless of precision
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");
    EXPECT_EQ(fmt::format("{:#.0o}", 0), "");
    EXPECT_EQ(fmt::format("{:#.1o}", 0), "0");
    EXPECT_EQ(fmt::format("{:#.2o}", 0), "00");
    EXPECT_EQ(fmt::format("{:#.3o}", 0), "000");
    
    // Zero with width
    EXPECT_EQ(fmt::format("{:#5o}", 0), "    0");
    EXPECT_EQ(fmt::format("{:#05o}", 0), "00000");
}