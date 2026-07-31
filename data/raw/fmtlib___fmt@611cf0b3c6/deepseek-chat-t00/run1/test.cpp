#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cstdint>

// Test on_oct behavior through the public format API
// The function is called when formatting integers with 'o' type specifier

TEST(OctFormatTest, BasicOctal) {
    // Normal case: positive integer
    EXPECT_EQ(fmt::format("{:o}", 42), "52");
    EXPECT_EQ(fmt::format("{:o}", 255), "377");
    EXPECT_EQ(fmt::format("{:o}", 8), "10");
}

TEST(OctFormatTest, ZeroValue) {
    // Zero should produce "0" without prefix
    EXPECT_EQ(fmt::format("{:o}", 0), "0");
    // With alternate form, zero should still be "0" (no prefix)
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");
}

TEST(OctFormatTest, AlternateFormPrefix) {
    // Alternate form adds '0' prefix for non-zero values
    EXPECT_EQ(fmt::format("{:#o}", 42), "052");
    EXPECT_EQ(fmt::format("{:#o}", 1), "01");
    EXPECT_EQ(fmt::format("{:#o}", 7), "07");
    EXPECT_EQ(fmt::format("{:#o}", 8), "010");
}

TEST(OctFormatTest, PrecisionWithAlternateForm) {
    // When precision > number of digits, prefix should NOT be added
    // because the prefix would be counted as a digit
    EXPECT_EQ(fmt::format("{:#.5o}", 42), "00042");  // precision 5 > 2 digits, no prefix
    EXPECT_EQ(fmt::format("{:#.3o}", 42), "042");    // precision 3 > 2 digits, no prefix
    EXPECT_EQ(fmt::format("{:#.2o}", 42), "052");    // precision 2 == 2 digits, prefix added
    EXPECT_EQ(fmt::format("{:#.1o}", 42), "052");    // precision 1 < 2 digits, prefix added
}

TEST(OctFormatTest, PrecisionWithoutAlternateForm) {
    // Without alternate form, precision just pads with zeros
    EXPECT_EQ(fmt::format("{:.5o}", 42), "00052");
    EXPECT_EQ(fmt::format("{:.3o}", 42), "052");
    EXPECT_EQ(fmt::format("{:.2o}", 42), "52");
    EXPECT_EQ(fmt::format("{:.1o}", 42), "52");
}

TEST(OctFormatTest, LargeValues) {
    // Test with larger integers
    EXPECT_EQ(fmt::format("{:o}", 1024), "2000");
    EXPECT_EQ(fmt::format("{:#o}", 1024), "02000");
    EXPECT_EQ(fmt::format("{:o}", 123456789), "726746425");
    EXPECT_EQ(fmt::format("{:#o}", 123456789), "0726746425");
}

TEST(OctFormatTest, NegativeValues) {
    // Negative values should include sign
    EXPECT_EQ(fmt::format("{:o}", -42), "-52");
    EXPECT_EQ(fmt::format("{:#o}", -42), "-052");
    EXPECT_EQ(fmt::format("{:o}", -1), "-1");
    EXPECT_EQ(fmt::format("{:#o}", -1), "-01");
}

TEST(OctFormatTest, WidthAndAlignment) {
    // Width and alignment should work with octal
    EXPECT_EQ(fmt::format("{:8o}", 42), "      52");
    EXPECT_EQ(fmt::format("{:<8o}", 42), "52      ");
    EXPECT_EQ(fmt::format("{:^8o}", 42), "   52   ");
    EXPECT_EQ(fmt::format("{:#8o}", 42), "     052");
    EXPECT_EQ(fmt::format("{:#<8o}", 42), "052#####");
}

TEST(OctFormatTest, FillAndAlignWithAlternateForm) {
    // Fill character should not interfere with prefix logic
    EXPECT_EQ(fmt::format("{:*>8o}", 42), "******52");
    EXPECT_EQ(fmt::format("{:*>8#o}", 42), "*****052");
    EXPECT_EQ(fmt::format("{:*<8#o}", 42), "052*****");
}

TEST(OctFormatTest, ZeroFlag) {
    // Zero padding flag
    EXPECT_EQ(fmt::format("{:08o}", 42), "00000052");
    EXPECT_EQ(fmt::format("{:#08o}", 42), "00000052");  // prefix counted in width
}

TEST(OctFormatTest, PlusAndSpaceFlags) {
    // Sign flags should work with octal
    EXPECT_EQ(fmt::format("{:+o}", 42), "+52");
    EXPECT_EQ(fmt::format("{:+o}", -42), "-52");
    EXPECT_EQ(fmt::format("{: o}", 42), " 52");
    EXPECT_EQ(fmt::format("{: o}", -42), "-52");
    EXPECT_EQ(fmt::format("{:#+o}", 42), "+052");
    EXPECT_EQ(fmt::format("{:# o}", 42), " 052");
}

TEST(OctFormatTest, EdgeCaseMaxValues) {
    // Test with maximum values for common integer types
    EXPECT_EQ(fmt::format("{:o}", std::numeric_limits<int>::max()), "17777777777");
    EXPECT_EQ(fmt::format("{:#o}", std::numeric_limits<int>::max()), "017777777777");
    EXPECT_EQ(fmt::format("{:o}", std::numeric_limits<unsigned int>::max()), "37777777777");
    EXPECT_EQ(fmt::format("{:#o}", std::numeric_limits<unsigned int>::max()), "037777777777");
}

TEST(OctFormatTest, PrecisionZero) {
    // Precision of 0 with non-zero value should still show digits
    EXPECT_EQ(fmt::format("{:.0o}", 42), "52");
    // Precision of 0 with zero value should produce empty string
    EXPECT_EQ(fmt::format("{:.0o}", 0), "");
    // With alternate form and precision 0, zero should still be empty
    EXPECT_EQ(fmt::format("{:#.0o}", 0), "");
}

TEST(OctFormatTest, AlternateFormPrecisionEdgeCases) {
    // Edge case: value with exactly 1 digit
    EXPECT_EQ(fmt::format("{:#o}", 1), "01");
    EXPECT_EQ(fmt::format("{:#.1o}", 1), "01");   // precision == digits, prefix added
    EXPECT_EQ(fmt::format("{:#.2o}", 1), "001");  // precision > digits, no prefix
    
    // Edge case: value with many digits
    EXPECT_EQ(fmt::format("{:#.10o}", 42), "0000000052");  // precision > digits, no prefix
    EXPECT_EQ(fmt::format("{:#.2o}", 42), "052");          // precision == digits, prefix added
}