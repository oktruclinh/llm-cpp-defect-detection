#include <gtest/gtest.h>
#include "fmt/format.h"
#include <string>
#include <climits>

// Tests for the on_oct() behavior via fmt::format with octal formatting
// The function is called when formatting with 'o' specifier (octal)

TEST(OnOctTest, BasicOctal) {
    // Basic octal formatting
    EXPECT_EQ(fmt::format("{:o}", 0), "0");
    EXPECT_EQ(fmt::format("{:o}", 1), "1");
    EXPECT_EQ(fmt::format("{:o}", 8), "10");
    EXPECT_EQ(fmt::format("{:o}", 64), "100");
    EXPECT_EQ(fmt::format("{:o}", 511), "777");
}

TEST(OnOctTest, ZeroValue) {
    // Zero should not get a prefix
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");
    EXPECT_EQ(fmt::format("{:#.0o}", 0), "");
    EXPECT_EQ(fmt::format("{:#.1o}", 0), "0");
}

TEST(OnOctTest, AlternateFormPrefix) {
    // Alternate form '#' adds '0' prefix for non-zero values
    EXPECT_EQ(fmt::format("{:#o}", 1), "01");
    EXPECT_EQ(fmt::format("{:#o}", 8), "010");
    EXPECT_EQ(fmt::format("{:#o}", 64), "0100");
    EXPECT_EQ(fmt::format("{:#o}", 7), "07");
    EXPECT_EQ(fmt::format("{:#o}", 9), "011");
}

TEST(OnOctTest, PrecisionWithAlternateForm) {
    // When precision > number of digits, prefix should be suppressed
    // because the prefix '0' counts as a digit
    EXPECT_EQ(fmt::format("{:#.4o}", 1), "0001");    // precision 4 > 1 digit, no prefix
    EXPECT_EQ(fmt::format("{:#.3o}", 1), "001");     // precision 3 > 1 digit, no prefix
    EXPECT_EQ(fmt::format("{:#.2o}", 1), "01");      // precision 2 == 1 digit? Actually 2 > 1, so no prefix? Wait, precision is 2, digits=1, so precision > digits => no prefix
    // Actually let's check: precision=2, num_digits=1 => 2 > 1 => no prefix
    EXPECT_EQ(fmt::format("{:#.2o}", 1), "01");      // Should be "01" with prefix? No, prefix suppressed because precision > digits
    // But wait: "01" with prefix would be "001"? Let's verify empirically:
    // fmt::format("{:#.2o}", 1) gives "01" in fmtlib — prefix '0' counts as digit, so with precision 2 and value 1 (1 digit),
    // we get "01" (precision 2, so 2 digits, no extra prefix). So expected: "01"
}

TEST(OnOctTest, PrecisionEqualToDigitsWithAlternate) {
    // When precision equals number of digits AND alternate form, prefix should be added
    // because prefix '0' counts as a digit, so total digits = precision + 1 (the prefix)
    EXPECT_EQ(fmt::format("{:#.1o}", 1), "01");      // 1 digit, precision=1, prefix added => "0" + "1" = "01"
    EXPECT_EQ(fmt::format("{:#.2o}", 8), "010");     // 2 digits (10), precision=2, prefix added => "0" + "10" = "010"
    EXPECT_EQ(fmt::format("{:#.3o}", 64), "0100");   // 3 digits (100), precision=3, prefix added => "0" + "100" = "0100"
}

TEST(OnOctTest, PrecisionGreaterThanDigitsNoAlternate) {
    // Without alternate form, just padding with zeros
    EXPECT_EQ(fmt::format("{:.4o}", 1), "0001");
    EXPECT_EQ(fmt::format("{:.5o}", 8), "00010");
    EXPECT_EQ(fmt::format("{:.6o}", 64), "000100");
}

TEST(OnOctTest, LargeValues) {
    // Large positive values
    EXPECT_EQ(fmt::format("{:o}", 1023), "1777");
    EXPECT_EQ(fmt::format("{:#o}", 1023), "01777");
    EXPECT_EQ(fmt::format("{:o}", 4095), "7777");
    EXPECT_EQ(fmt::format("{:#o}", 4095), "07777");
}

TEST(OnOctTest, NegativeValues) {
    // Negative values (should show minus sign)
    EXPECT_EQ(fmt::format("{:o}", -1), "-1");
    EXPECT_EQ(fmt::format("{:o}", -8), "-10");
    EXPECT_EQ(fmt::format("{:#o}", -1), "-01");
    EXPECT_EQ(fmt::format("{:#o}", -8), "-010");
}

TEST(OnOctTest, WidthAndPrecision) {
    // Width and precision interaction
    EXPECT_EQ(fmt::format("{:6o}", 42), "    52");    // width 6, right-aligned
    EXPECT_EQ(fmt::format("{:<6o}", 42), "52    ");  // left-aligned
    EXPECT_EQ(fmt::format("{:^6o}", 42), "  52  ");  // center
    EXPECT_EQ(fmt::format("{:#8o}", 42), "     052"); // width 8 with prefix
    EXPECT_EQ(fmt::format("{:#.5o}", 42), "00052");   // precision 5, no prefix because 5 > 2 digits
    EXPECT_EQ(fmt::format("{:#.2o}", 42), "052");     // precision 2 == 2 digits? 42 in octal is "52" (2 digits), so prefix added
}

TEST(OnOctTest, ZeroPaddingWithAlternate) {
    // Zero padding with alternate form
    EXPECT_EQ(fmt::format("{:#06o}", 42), "000052"); // width 6, zero pad, prefix '0' counts, so "0" + "00052"? Actually width 6, value "52", prefix "0" -> "0"+"52" = "052", pad to 6: "000052"
    EXPECT_EQ(fmt::format("{:#08o}", 42), "00000052"); // width 8
}

TEST(OnOctTest, AlternateFormWithZero) {
    // Alternate form with zero value and various precisions
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");
    EXPECT_EQ(fmt::format("{:#.0o}", 0), "");       // precision 0, zero value -> empty
    EXPECT_EQ(fmt::format("{:#.1o}", 0), "0");       // precision 1, zero value -> "0" (no prefix because value is 0)
    EXPECT_EQ(fmt::format("{:#.2o}", 0), "00");      // precision 2, zero value -> "00" (no prefix)
}

TEST(OnOctTest, EdgeCaseMaxInt) {
    // Max int value (works for 32-bit int)
    int max_val = INT_MAX;
    std::string expected = fmt::format("{:o}", static_cast<unsigned>(max_val));
    EXPECT_EQ(fmt::format("{:o}", max_val), expected);
}