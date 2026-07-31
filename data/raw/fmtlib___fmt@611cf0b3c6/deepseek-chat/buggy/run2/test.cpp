#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <climits>

// Test octal formatting with default settings (no prefix, no precision)
TEST(OctFormatTest, DefaultNoPrefix) {
    EXPECT_EQ(fmt::format("{:o}", 0), "0");
    EXPECT_EQ(fmt::format("{:o}", 1), "1");
    EXPECT_EQ(fmt::format("{:o}", 7), "7");
    EXPECT_EQ(fmt::format("{:o}", 8), "10");
    EXPECT_EQ(fmt::format("{:o}", 64), "100");
    EXPECT_EQ(fmt::format("{:o}", 123456), "361100");
}

// Test octal formatting with alternate form ('#' prefix)
TEST(OctFormatTest, AlternateForm) {
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");  // zero is special: no prefix
    EXPECT_EQ(fmt::format("{:#o}", 1), "01");
    EXPECT_EQ(fmt::format("{:#o}", 7), "07");
    EXPECT_EQ(fmt::format("{:#o}", 8), "010");
    EXPECT_EQ(fmt::format("{:#o}", 64), "0100");
    EXPECT_EQ(fmt::format("{:#o}", 123456), "0361100");
}

// Test octal formatting with precision
TEST(OctFormatTest, Precision) {
    EXPECT_EQ(fmt::format("{:.0o}", 0), "");     // zero with zero precision -> empty
    EXPECT_EQ(fmt::format("{:.1o}", 0), "0");
    EXPECT_EQ(fmt::format("{:.3o}", 5), "005");
    EXPECT_EQ(fmt::format("{:.5o}", 42), "00052");
    EXPECT_EQ(fmt::format("{:.10o}", 123), "0000000173");
}

// Test octal formatting with alternate form AND precision
TEST(OctFormatTest, AlternateFormWithPrecision) {
    // When precision > number of digits, prefix is suppressed (per standard)
    EXPECT_EQ(fmt::format("{:#.3o}", 1), "001");   // prefix suppressed because precision > digits
    EXPECT_EQ(fmt::format("{:#.4o}", 1), "0001");
    EXPECT_EQ(fmt::format("{:#.2o}", 7), "07");    // precision == digits, prefix added
    EXPECT_EQ(fmt::format("{:#.3o}", 7), "007");   // precision > digits, prefix suppressed
    EXPECT_EQ(fmt::format("{:#.4o}", 8), "0010");  // 8 -> "10" (2 digits), precision 4 > 2, no prefix
    EXPECT_EQ(fmt::format("{:#.2o}", 8), "010");   // precision == digits (2), prefix added
}

// Test zero value edge cases
TEST(OctFormatTest, ZeroValue) {
    EXPECT_EQ(fmt::format("{:o}", 0), "0");
    EXPECT_EQ(fmt::format("{:#o}", 0), "0");       // zero with alternate: no prefix
    EXPECT_EQ(fmt::format("{:.0o}", 0), "");       // zero precision -> empty
    EXPECT_EQ(fmt::format("{:#.0o}", 0), "");      // alternate + zero precision -> empty
    EXPECT_EQ(fmt::format("{:.1o}", 0), "0");
    EXPECT_EQ(fmt::format("{:#.1o}", 0), "0");     // prefix suppressed because precision >= digits
}

// Test maximum values
TEST(OctFormatTest, MaxValues) {
    EXPECT_EQ(fmt::format("{:o}", INT_MAX), fmt::format("{:o}", static_cast<unsigned>(INT_MAX)));
    EXPECT_EQ(fmt::format("{:#o}", INT_MAX), fmt::format("{:#o}", static_cast<unsigned>(INT_MAX)));
    EXPECT_EQ(fmt::format("{:o}", UINT_MAX), "37777777777");  // 32-bit octal max
    EXPECT_EQ(fmt::format("{:#o}", UINT_MAX), "037777777777");
}

// Test negative numbers (should be treated as unsigned via format spec)
TEST(OctFormatTest, NegativeNumbers) {
    EXPECT_EQ(fmt::format("{:o}", -1), fmt::format("{:o}", UINT_MAX));
    EXPECT_EQ(fmt::format("{:#o}", -1), fmt::format("{:#o}", UINT_MAX));
    EXPECT_EQ(fmt::format("{:o}", -42), fmt::format("{:o}", static_cast<unsigned>(-42)));
}

// Test width and alignment with octal
TEST(OctFormatTest, WidthAndAlignment) {
    EXPECT_EQ(fmt::format("{:8o}", 42), "      52");
    EXPECT_EQ(fmt::format("{:<8o}", 42), "52      ");
    EXPECT_EQ(fmt::format("{:^8o}", 42), "   52   ");
    EXPECT_EQ(fmt::format("{:#10o}", 42), "      052");
    EXPECT_EQ(fmt::format("{:#<10o}", 42), "052#######");
}

// Test fill character with octal
TEST(OctFormatTest, FillCharacter) {
    EXPECT_EQ(fmt::format("{:*>8o}", 42), "******52");
    EXPECT_EQ(fmt::format("{:*<8o}", 42), "52******");
    EXPECT_EQ(fmt::format("{:*^8o}", 42), "***52***");
    EXPECT_EQ(fmt::format("{:*>10#o}", 42), "*******052");
}

// Test that precision with alternate form correctly handles digit count boundary
TEST(OctFormatTest, PrecisionBoundaryAlternate) {
    // For value 8 -> octal "10" (2 digits)
    // precision == 2: prefix added (since precision <= num_digits)
    EXPECT_EQ(fmt::format("{:#.2o}", 8), "010");
    // precision == 3: prefix suppressed (since precision > num_digits)
    EXPECT_EQ(fmt::format("{:#.3o}", 8), "010");
    
    // For value 64 -> octal "100" (3 digits)
    EXPECT_EQ(fmt::format("{:#.3o}", 64), "0100");
    EXPECT_EQ(fmt::format("{:#.4o}", 64), "0100");
}