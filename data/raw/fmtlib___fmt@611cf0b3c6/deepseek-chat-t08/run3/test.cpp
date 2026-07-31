#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <climits>

// Test the on_oct behavior through fmt::format with octal format specifier 'o'
// The on_oct function is called internally when formatting with 'o' specifier

TEST(OctFormatTest, BasicOctal) {
    // Basic positive integer
    EXPECT_EQ(fmt::format("{:o}", 255), "377");
    EXPECT_EQ(fmt::format("{:o}", 64), "100");
    EXPECT_EQ(fmt::format("{:o}", 7), "7");
    EXPECT_EQ(fmt::format("{:o}", 8), "10");
}

TEST(OctFormatTest, ZeroValue) {
    // Zero is an edge case - no prefix should be added
    EXPECT_EQ(fmt::format("{:o}", 0), "0");
    // With alternate form, zero should not get prefix
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");
}

TEST(OctFormatTest, AlternateFormPrefix) {
    // Alternate form '#'' should add '0' prefix for non-zero values
    EXPECT_EQ(fmt::format("{:#o}", 255), "0377");
    EXPECT_EQ(fmt::format("{:#o}", 64), "0100");
    EXPECT_EQ(fmt::format("{:#o}", 1), "01");
    EXPECT_EQ(fmt::format("{:#o}", 7), "07");
}

TEST(OctFormatTest, PrecisionWithAlternateForm) {
    // When precision > number of digits, prefix should NOT be added
    // because the prefix '0' would be counted as a digit
    EXPECT_EQ(fmt::format("{:#.5o}", 255), "00377");
    EXPECT_EQ(fmt::format("{:#.4o}", 255), "0377");
    EXPECT_EQ(fmt::format("{:#.3o}", 255), "0377");
    EXPECT_EQ(fmt::format("{:#.2o}", 255), "0377");
    EXPECT_EQ(fmt::format("{:#.1o}", 255), "0377");
}

TEST(OctFormatTest, PrecisionLessThanNumDigits) {
    // When precision < number of digits, prefix should be added
    // (assuming value is non-zero and alt is set)
    EXPECT_EQ(fmt::format("{:#.2o}", 64), "0100");  // 64 in octal is 100, 3 digits
    EXPECT_EQ(fmt::format("{:#.1o}", 64), "0100");
    EXPECT_EQ(fmt::format("{:#.0o}", 64), "0100");
}

TEST(OctFormatTest, PrecisionEqualToNumDigits) {
    // When precision == number of digits, prefix should NOT be added
    // because prefix counts as a digit and would exceed precision
    EXPECT_EQ(fmt::format("{:#.3o}", 64), "100");   // 64 has 3 octal digits, precision=3
    EXPECT_EQ(fmt::format("{:#.2o}", 7), "07");     // 7 has 1 octal digit, precision=2
    EXPECT_EQ(fmt::format("{:#.1o}", 1), "01");     // 1 has 1 octal digit, precision=1
}

TEST(OctFormatTest, PrecisionGreaterThanNumDigits) {
    // When precision > number of digits, prefix should NOT be added
    EXPECT_EQ(fmt::format("{:#.4o}", 64), "0100");   // 3 digits, precision=4
    EXPECT_EQ(fmt::format("{:#.5o}", 64), "00100");  // 3 digits, precision=5
    EXPECT_EQ(fmt::format("{:#.6o}", 1), "000001");  // 1 digit, precision=6
}

TEST(OctFormatTest, WidthAndAlignment) {
    // Test interaction with width and alignment
    EXPECT_EQ(fmt::format("{:10o}", 255), "       377");
    EXPECT_EQ(fmt::format("{:#10o}", 255), "      0377");
    EXPECT_EQ(fmt::format("{:-<10o}", 255), "377-------");
    EXPECT_EQ(fmt::format("{:#>-10o}", 255), "------0377");
}

TEST(OctFormatTest, LargeValues) {
    // Test with large values to ensure digit counting works
    EXPECT_EQ(fmt::format("{:o}", 1048575), "3777777");  // 2^20 - 1
    EXPECT_EQ(fmt::format("{:o}", 2097151), "7777777");  // 2^21 - 1
    EXPECT_EQ(fmt::format("{:#o}", 1048575), "03777777");
}

TEST(OctFormatTest, NegativeValues) {
    // Negative values should be handled (though oct format typically unsigned)
    // fmt treats negative as unsigned for 'o'
    EXPECT_EQ(fmt::format("{:o}", -1), "37777777777");  // on 32-bit
    EXPECT_EQ(fmt::format("{:#o}", -1), "037777777777");
}

TEST(OctFormatTest, FillCharacter) {
    // Test custom fill characters
    EXPECT_EQ(fmt::format("{:*>10o}", 255), "*******377");
    EXPECT_EQ(fmt::format("{:*<10o}", 255), "377*******");
    EXPECT_EQ(fmt::format("{:*^10o}", 255), "***377****");
}

TEST(OctFormatTest, CombinedPrecisionAndWidth) {
    // Test combined precision and width with alternate form
    EXPECT_EQ(fmt::format("{:#8.5o}", 255), "  00377");
    EXPECT_EQ(fmt::format("{:#8.3o}", 255), "    0377");
    EXPECT_EQ(fmt::format("{:#8.4o}", 64), "   0100");
}

TEST(OctFormatTest, MaximumValue) {
    // Test with maximum unsigned value for 32-bit
    EXPECT_EQ(fmt::format("{:o}", 4294967295u), "37777777777");
    EXPECT_EQ(fmt::format("{:#o}", 4294967295u), "037777777777");
}

TEST(OctFormatTest, SingleDigitValues) {
    // Test single octal digit values with alternate form
    EXPECT_EQ(fmt::format("{:#o}", 1), "01");
    EXPECT_EQ(fmt::format("{:#o}", 2), "02");
    EXPECT_EQ(fmt::format("{:#o}", 3), "03");
    EXPECT_EQ(fmt::format("{:#o}", 4), "04");
    EXPECT_EQ(fmt::format("{:#o}", 5), "05");
    EXPECT_EQ(fmt::format("{:#o}", 6), "06");
    EXPECT_EQ(fmt::format("{:#o}", 7), "07");
}

TEST(OctFormatTest, PrecisionZeroWithAlternate) {
    // Precision = 0 with non-zero value and alternate form
    EXPECT_EQ(fmt::format("{:#.0o}", 255), "0377");  // precision=0 < num_digits=3, so prefix added
    EXPECT_EQ(fmt::format("{:#.0o}", 1), "01");      // precision=0 < num_digits=1, so prefix added
}