#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <climits>

// Test octal formatting through the public fmt::format API
// The focal function on_oct() is called internally when formatting with 'o' specifier

TEST(OctFormatTest, BasicOctal) {
    // Basic octal conversion
    EXPECT_EQ(fmt::format("{:o}", 0), "0");
    EXPECT_EQ(fmt::format("{:o}", 1), "1");
    EXPECT_EQ(fmt::format("{:o}", 7), "7");
    EXPECT_EQ(fmt::format("{:o}", 8), "10");
    EXPECT_EQ(fmt::format("{:o}", 64), "100");
    EXPECT_EQ(fmt::format("{:o}", 255), "377");
}

TEST(OctFormatTest, AltFlagPrefix) {
    // Alternate form adds '0' prefix for non-zero values
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");  // No prefix for zero
    EXPECT_EQ(fmt::format("{:#o}", 1), "01");
    EXPECT_EQ(fmt::format("{:#o}", 7), "07");
    EXPECT_EQ(fmt::format("{:#o}", 8), "010");
    EXPECT_EQ(fmt::format("{:#o}", 64), "0100");
    EXPECT_EQ(fmt::format("{:#o}", 255), "0377");
}

TEST(OctFormatTest, PrecisionWithAlt) {
    // Precision greater than number of digits should NOT add prefix
    EXPECT_EQ(fmt::format("{:#.5o}", 1), "00001");  // No prefix, precision > digits
    EXPECT_EQ(fmt::format("{:#.5o}", 8), "00010");  // No prefix
    EXPECT_EQ(fmt::format("{:#.3o}", 1), "001");    // Precision == digits+1, no prefix
    EXPECT_EQ(fmt::format("{:#.2o}", 1), "01");     // Precision == digits, prefix added
    EXPECT_EQ(fmt::format("{:#.1o}", 1), "01");     // Precision < digits, prefix added
    EXPECT_EQ(fmt::format("{:#.4o}", 64), "0100");  // Precision == digits, prefix added
    EXPECT_EQ(fmt::format("{:#.5o}", 64), "00100"); // Precision > digits, no prefix
}

TEST(OctFormatTest, PrecisionWithoutAlt) {
    // Precision without alt flag
    EXPECT_EQ(fmt::format("{:.5o}", 1), "00001");
    EXPECT_EQ(fmt::format("{:.3o}", 8), "010");
    EXPECT_EQ(fmt::format("{:.0o}", 0), "");  // Zero with zero precision
    EXPECT_EQ(fmt::format("{:.0o}", 1), "1"); // Non-zero with zero precision
}

TEST(OctFormatTest, ZeroValue) {
    // Zero is a special case for alt flag
    EXPECT_EQ(fmt::format("{:o}", 0), "0");
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");  // No prefix for zero
    EXPECT_EQ(fmt::format("{:.5o}", 0), "00000");
    EXPECT_EQ(fmt::format("{:#.5o}", 0), "00000");  // No prefix even with alt
    EXPECT_EQ(fmt::format("{:.0o}", 0), "");  // Zero precision with zero
}

TEST(OctFormatTest, LargeValues) {
    // Test with larger values
    EXPECT_EQ(fmt::format("{:o}", 1024), "2000");
    EXPECT_EQ(fmt::format("{:#o}", 1024), "02000");
    EXPECT_EQ(fmt::format("{:o}", 65535), "177777");
    EXPECT_EQ(fmt::format("{:#o}", 65535), "0177777");
}

TEST(OctFormatTest, WidthAndAlignment) {
    // Width and alignment interact with octal formatting
    EXPECT_EQ(fmt::format("{:10o}", 255), "       377");
    EXPECT_EQ(fmt::format("{:<10o}", 255), "377       ");
    EXPECT_EQ(fmt::format("{:^10o}", 255), "   377    ");
    EXPECT_EQ(fmt::format("{:#10o}", 255), "      0377");
    EXPECT_EQ(fmt::format("{:#<10o}", 255), "0377######");
}

TEST(OctFormatTest, FillAndAlignWithAlt) {
    // Fill character and alignment with alt flag
    EXPECT_EQ(fmt::format("{:*>10o}", 255), "*******377");
    EXPECT_EQ(fmt::format("{:*<10o}", 255), "377*******");
    EXPECT_EQ(fmt::format("{:*^10o}", 255), "***377****");
    EXPECT_EQ(fmt::format("{:*>10#o}", 255), "******0377");
    EXPECT_EQ(fmt::format("{:*<10#o}", 255), "0377******");
}

TEST(OctFormatTest, NegativeValues) {
    // Negative values should work (though octal of negative is implementation-defined)
    // fmtlib handles negative by formatting the absolute value and adding sign
    EXPECT_EQ(fmt::format("{:o}", -1), "37777777777");  // Assuming 32-bit two's complement
    EXPECT_EQ(fmt::format("{:#o}", -1), "037777777777");
    EXPECT_EQ(fmt::format("{:o}", -8), "37777777770");
}

TEST(OctFormatTest, CombinedSpecifiers) {
    // Test various combinations of flags, width, precision
    EXPECT_EQ(fmt::format("{:#010o}", 255), "0000000377");
    EXPECT_EQ(fmt::format("{:#010.5o}", 255), "0000000377");  // Precision < width
    EXPECT_EQ(fmt::format("{:#010.8o}", 255), "00000000377"); // Precision > width
    EXPECT_EQ(fmt::format("{:+o}", 255), "+377");
    EXPECT_EQ(fmt::format("{: o}", 255), " 377");
    EXPECT_EQ(fmt::format("{:-o}", 255), "377");
}

TEST(OctFormatTest, EdgeCasePrecisionEqualsDigits) {
    // When precision equals number of digits, alt flag should add prefix
    EXPECT_EQ(fmt::format("{:#.3o}", 64), "0100");   // 3 digits, precision=3
    EXPECT_EQ(fmt::format("{:#.2o}", 7), "07");      // 1 digit, precision=2 (no prefix)
    EXPECT_EQ(fmt::format("{:#.1o}", 7), "07");      // 1 digit, precision=1 (prefix)
}

TEST(OctFormatTest, ZeroWithAltAndPrecision) {
    // Zero with various precision and alt combinations
    EXPECT_EQ(fmt::format("{:#.0o}", 0), "");
    EXPECT_EQ(fmt::format("{:#.1o}", 0), "0");
    EXPECT_EQ(fmt::format("{:#.5o}", 0), "00000");
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");
}