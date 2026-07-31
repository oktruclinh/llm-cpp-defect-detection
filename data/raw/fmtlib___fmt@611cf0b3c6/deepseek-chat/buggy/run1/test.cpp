#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <climits>

// Test octal formatting with default settings
TEST(OctFormatTest, DefaultFormat) {
    EXPECT_EQ(fmt::format("{:o}", 0), "0");
    EXPECT_EQ(fmt::format("{:o}", 1), "1");
    EXPECT_EQ(fmt::format("{:o}", 7), "7");
    EXPECT_EQ(fmt::format("{:o}", 8), "10");
    EXPECT_EQ(fmt::format("{:o}", 64), "100");
    EXPECT_EQ(fmt::format("{:o}", 255), "377");
    EXPECT_EQ(fmt::format("{:o}", 1024), "2000");
}

// Test octal formatting with alternate form (# flag)
TEST(OctFormatTest, AlternateForm) {
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");  // No prefix for zero
    EXPECT_EQ(fmt::format("{:#o}", 1), "01");
    EXPECT_EQ(fmt::format("{:#o}", 7), "07");
    EXPECT_EQ(fmt::format("{:#o}", 8), "010");
    EXPECT_EQ(fmt::format("{:#o}", 64), "0100");
    EXPECT_EQ(fmt::format("{:#o}", 255), "0377");
}

// Test octal formatting with precision
TEST(OctFormatTest, WithPrecision) {
    EXPECT_EQ(fmt::format("{:.0o}", 0), "");
    EXPECT_EQ(fmt::format("{:.1o}", 0), "0");
    EXPECT_EQ(fmt::format("{:.3o}", 1), "001");
    EXPECT_EQ(fmt::format("{:.5o}", 42), "00052");
    EXPECT_EQ(fmt::format("{:.10o}", 255), "0000000377");
}

// Test octal formatting with alternate form and precision
TEST(OctFormatTest, AlternateFormWithPrecision) {
    // When precision > number of digits, prefix should NOT be added
    EXPECT_EQ(fmt::format("{:#.4o}", 1), "0001");
    EXPECT_EQ(fmt::format("{:#.5o}", 8), "00010");
    EXPECT_EQ(fmt::format("{:#.6o}", 64), "000100");
    
    // When precision <= number of digits, prefix SHOULD be added
    EXPECT_EQ(fmt::format("{:#.1o}", 1), "01");
    EXPECT_EQ(fmt::format("{:#.2o}", 8), "010");
    EXPECT_EQ(fmt::format("{:#.3o}", 64), "0100");
    
    // Edge case: precision equals number of digits
    EXPECT_EQ(fmt::format("{:#.1o}", 7), "07");
    EXPECT_EQ(fmt::format("{:#.2o}", 10), "012");
}

// Test octal formatting with width
TEST(OctFormatTest, WithWidth) {
    EXPECT_EQ(fmt::format("{:5o}", 42), "   52");
    EXPECT_EQ(fmt::format("{:5o}", 0), "    0");
    EXPECT_EQ(fmt::format("{:<5o}", 42), "52   ");
    EXPECT_EQ(fmt::format("{:>5o}", 42), "   52");
    EXPECT_EQ(fmt::format("{:^5o}", 42), " 52  ");
}

// Test octal formatting with alternate form and width
TEST(OctFormatTest, AlternateFormWithWidth) {
    EXPECT_EQ(fmt::format("{:#6o}", 42), "   052");
    EXPECT_EQ(fmt::format("{:#<6o}", 42), "052   ");
    EXPECT_EQ(fmt::format("{:#>6o}", 42), "   052");
    EXPECT_EQ(fmt::format("{:#^6o}", 42), " 052  ");
}

// Test octal formatting with width and precision
TEST(OctFormatTest, WidthAndPrecision) {
    EXPECT_EQ(fmt::format("{:8.5o}", 42), "   00052");
    EXPECT_EQ(fmt::format("{:<8.5o}", 42), "00052   ");
    EXPECT_EQ(fmt::format("{:>8.5o}", 42), "   00052");
    EXPECT_EQ(fmt::format("{:^8.5o}", 42), " 00052  ");
}

// Test octal formatting with alternate form, width, and precision
TEST(OctFormatTest, AlternateFormWidthAndPrecision) {
    // Precision > num_digits: no prefix
    EXPECT_EQ(fmt::format("{:#10.6o}", 42), "    000052");
    // Precision <= num_digits: with prefix
    EXPECT_EQ(fmt::format("{:#10.2o}", 42), "      052");
}

// Test negative numbers (should be formatted as unsigned)
TEST(OctFormatTest, NegativeNumbers) {
    EXPECT_EQ(fmt::format("{:o}", -1), "37777777777");  // Assuming 32-bit
    EXPECT_EQ(fmt::format("{:o}", -8), "37777777770");
    EXPECT_EQ(fmt::format("{:#o}", -1), "037777777777");
}

// Test zero values
TEST(OctFormatTest, ZeroValues) {
    EXPECT_EQ(fmt::format("{:o}", 0), "0");
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");
    EXPECT_EQ(fmt::format("{:.0o}", 0), "");
    EXPECT_EQ(fmt::format("{:#.0o}", 0), "");
    EXPECT_EQ(fmt::format("{:5o}", 0), "    0");
    EXPECT_EQ(fmt::format("{:#5o}", 0), "    0");
}

// Test large values
TEST(OctFormatTest, LargeValues) {
    EXPECT_EQ(fmt::format("{:o}", INT_MAX), "17777777777");
    EXPECT_EQ(fmt::format("{:o}", UINT_MAX), "37777777777");
    EXPECT_EQ(fmt::format("{:#o}", INT_MAX), "017777777777");
}

// Test with fill character
TEST(OctFormatTest, WithFillCharacter) {
    EXPECT_EQ(fmt::format("{:*>8o}", 42), "*******52");
    EXPECT_EQ(fmt::format("{:*<8o}", 42), "52*******");
    EXPECT_EQ(fmt::format("{:*^8o}", 42), "***52****");
    EXPECT_EQ(fmt::format("{:#*>8o}", 42), "*****052");
}

// Test combination of all features
TEST(OctFormatTest, AllFeaturesCombined) {
    EXPECT_EQ(fmt::format("{:#010.5o}", 42), "000000052");
    EXPECT_EQ(fmt::format("{:#010.2o}", 42), "000000052");
    EXPECT_EQ(fmt::format("{:*>#12.6o}", 42), "*****000052");
    EXPECT_EQ(fmt::format("{:*<#12.6o}", 42), "000052*****");
}

// Test that the function handles the case where precision forces zero padding
TEST(OctFormatTest, PrecisionForcesZeroPadding) {
    EXPECT_EQ(fmt::format("{:.10o}", 1), "0000000001");
    EXPECT_EQ(fmt::format("{:#.10o}", 1), "0000000001");  // No prefix when precision > digits
    EXPECT_EQ(fmt::format("{:.10o}", 0), "0000000000");
}