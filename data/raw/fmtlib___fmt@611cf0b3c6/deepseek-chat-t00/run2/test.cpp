#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cstdint>

// Test on_oct behavior through the public format API
// The function is called internally when formatting with 'o' specifier

TEST(OctFormatTest, BasicOctal) {
    // Normal positive integer
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
    // because the prefix '0' would be counted as a digit
    EXPECT_EQ(fmt::format("{:#.5o}", 42), "00042");  // no prefix, precision pads
    EXPECT_EQ(fmt::format("{:#.3o}", 42), "042");    // precision == digits, prefix added
    EXPECT_EQ(fmt::format("{:#.2o}", 42), "052");    // precision < digits, prefix added
    EXPECT_EQ(fmt::format("{:#.4o}", 8), "0010");    // no prefix, precision pads
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
    EXPECT_EQ(fmt::format("{:o}", 0xFFFFFFFF), "37777777777");
}

TEST(OctFormatTest, NegativeValues) {
    // Negative values should include sign
    EXPECT_EQ(fmt::format("{:o}", -42), "-52");
    EXPECT_EQ(fmt::format("{:#o}", -42), "-052");
    EXPECT_EQ(fmt::format("{:o}", -1), "-1");
}

TEST(OctFormatTest, WidthAndAlignment) {
    // Width and alignment should work with octal
    EXPECT_EQ(fmt::format("{:8o}", 42), "      52");
    EXPECT_EQ(fmt::format("{:<8o}", 42), "52      ");
    EXPECT_EQ(fmt::format("{:^8o}", 42), "   52   ");
    EXPECT_EQ(fmt::format("{:#8o}", 42), "     052");
    EXPECT_EQ(fmt::format("{:#<8o}", 42), "052#####");
}

TEST(OctFormatTest, FillAndAlignWithAlternate) {
    // Fill character should work with alternate form
    EXPECT_EQ(fmt::format("{:*>8o}", 42), "******52");
    EXPECT_EQ(fmt::format("{:*>8#o}", 42), "*****052");
    EXPECT_EQ(fmt::format("{:*^8#o}", 42), "**052***");
}

TEST(OctFormatTest, ZeroFlag) {
    // Zero flag pads with zeros
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

TEST(OctFormatTest, EdgeCasePrecisionZero) {
    // Precision of 0 with non-zero value should still show digits
    EXPECT_EQ(fmt::format("{:.0o}", 42), "52");
    // Precision of 0 with zero value should produce empty string
    EXPECT_EQ(fmt::format("{:.0o}", 0), "");
    // With alternate form and precision 0, zero should be empty
    EXPECT_EQ(fmt::format("{:#.0o}", 0), "");
}

TEST(OctFormatTest, EdgeCaseMaxPrecision) {
    // Very large precision should pad with zeros
    std::string result = fmt::format("{:.10o}", 42);
    EXPECT_EQ(result.size(), 10);
    EXPECT_EQ(result, "0000000052");
    // With alternate form, prefix should not be added when precision > digits
    result = fmt::format("{:#.10o}", 42);
    EXPECT_EQ(result.size(), 10);
    EXPECT_EQ(result, "0000000052");  // no prefix
}

TEST(OctFormatTest, UnsignedTypes) {
    // Unsigned types should work correctly
    EXPECT_EQ(fmt::format("{:o}", static_cast<unsigned>(42)), "52");
    EXPECT_EQ(fmt::format("{:#o}", static_cast<unsigned>(42)), "052");
    EXPECT_EQ(fmt::format("{:o}", static_cast<unsigned long>(255)), "377");
}

TEST(OctFormatTest, CharType) {
    // char should be formatted as integer
    EXPECT_EQ(fmt::format("{:o}", static_cast<char>(64)), "100");
    EXPECT_EQ(fmt::format("{:#o}", static_cast<char>(64)), "0100");
}

TEST(OctFormatTest, ShortType) {
    // short should be formatted correctly
    EXPECT_EQ(fmt::format("{:o}", static_cast<short>(8)), "10");
    EXPECT_EQ(fmt::format("{:#o}", static_cast<short>(8)), "010");
}

TEST(OctFormatTest, LongLongType) {
    // long long should work
    EXPECT_EQ(fmt::format("{:o}", 1LL << 40), "200000000000");
    EXPECT_EQ(fmt::format("{:#o}", 1LL << 40), "0200000000000");
}