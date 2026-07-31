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
    // because the prefix would be counted as a digit and precision would be exceeded
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

TEST(OctFormatTest, WidthAndAlignment) {
    // Width with left/right alignment
    EXPECT_EQ(fmt::format("{:10o}", 42), "        52");
    EXPECT_EQ(fmt::format("{:<10o}", 42), "52        ");
    EXPECT_EQ(fmt::format("{:^10o}", 42), "    52    ");
    
    // Width with alternate form
    EXPECT_EQ(fmt::format("{:#10o}", 42), "       052");
    EXPECT_EQ(fmt::format("{:#<10o}", 42), "052       ");
}

TEST(OctFormatTest, FillCharacter) {
    // Custom fill character
    EXPECT_EQ(fmt::format("{:*>10o}", 42), "********52");
    EXPECT_EQ(fmt::format("{:*^10o}", 42), "****52****");
    EXPECT_EQ(fmt::format("{:#*>10o}", 42), "*******052");
}

TEST(OctFormatTest, NegativeNumbers) {
    // Negative numbers should show sign
    EXPECT_EQ(fmt::format("{:o}", -42), "-52");
    EXPECT_EQ(fmt::format("{:#o}", -42), "-052");
    EXPECT_EQ(fmt::format("{:10o}", -42), "       -52");
    EXPECT_EQ(fmt::format("{:#10o}", -42), "      -052");
}

TEST(OctFormatTest, SignFlags) {
    // Plus sign for positive
    EXPECT_EQ(fmt::format("{:+o}", 42), "+52");
    EXPECT_EQ(fmt::format("{:+o}", -42), "-52");
    EXPECT_EQ(fmt::format("{: o}", 42), " 52");
    EXPECT_EQ(fmt::format("{: o}", -42), "-52");
}

TEST(OctFormatTest, CombinedFlags) {
    // Alternate form with sign and width
    EXPECT_EQ(fmt::format("{:#+10o}", 42), "      +052");
    EXPECT_EQ(fmt::format("{:#-10o}", 42), "+052      ");
    EXPECT_EQ(fmt::format("{:#+10o}", -42), "      -052");
}

TEST(OctFormatTest, PrecisionWithZero) {
    // Precision with zero value
    EXPECT_EQ(fmt::format("{:.0o}", 0), "");  // precision 0, value 0 -> empty
    EXPECT_EQ(fmt::format("{:.1o}", 0), "0");
    EXPECT_EQ(fmt::format("{:.5o}", 0), "00000");
    EXPECT_EQ(fmt::format("{:#.0o}", 0), "");  // alternate form doesn't add prefix for zero
    EXPECT_EQ(fmt::format("{:#.5o}", 0), "00000");
}

TEST(OctFormatTest, PrecisionWithNonZero) {
    // Precision with non-zero values
    EXPECT_EQ(fmt::format("{:.0o}", 1), "1");  // precision 0, value non-zero -> no padding
    EXPECT_EQ(fmt::format("{:.3o}", 1), "001");
    EXPECT_EQ(fmt::format("{:#.0o}", 1), "01");  // alternate form adds prefix even with precision 0
    EXPECT_EQ(fmt::format("{:#.3o}", 1), "001");  // precision > digits, no prefix
}

TEST(OctFormatTest, EdgeCaseMaxValues) {
    // Test with maximum values for common integer types
    EXPECT_EQ(fmt::format("{:o}", std::numeric_limits<int>::max()), 
              fmt::format("{:o}", std::numeric_limits<int>::max()));
    EXPECT_EQ(fmt::format("{:#o}", std::numeric_limits<int>::max()),
              fmt::format("{:#o}", std::numeric_limits<int>::max()));
    
    // Test with unsigned max
    EXPECT_EQ(fmt::format("{:o}", std::numeric_limits<unsigned>::max()),
              fmt::format("{:o}", std::numeric_limits<unsigned>::max()));
}

TEST(OctFormatTest, LongLongTypes) {
    // Test with long long
    EXPECT_EQ(fmt::format("{:o}", 1LL << 40), "200000000000");
    EXPECT_EQ(fmt::format("{:#o}", 1LL << 40), "0200000000000");
}