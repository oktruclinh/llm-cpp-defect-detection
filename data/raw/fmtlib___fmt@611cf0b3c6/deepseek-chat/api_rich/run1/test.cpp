#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <climits>

// Test the on_oct() behavior through the public fmt::format API with octal format specifiers
class OctalFormatTest : public ::testing::Test {
protected:
    // Helper to format with octal specifier
    std::string format_octal(int value, const std::string& spec = "") {
        return fmt::format("{:" + spec + "o}", value);
    }
    
    std::string format_octal(unsigned int value, const std::string& spec = "") {
        return fmt::format("{:" + spec + "o}", value);
    }
    
    std::string format_octal(long value, const std::string& spec = "") {
        return fmt::format("{:" + spec + "o}", value);
    }
    
    std::string format_octal(unsigned long value, const std::string& spec = "") {
        return fmt::format("{:" + spec + "o}", value);
    }
    
    std::string format_octal(long long value, const std::string& spec = "") {
        return fmt::format("{:" + spec + "o}", value);
    }
    
    std::string format_octal(unsigned long long value, const std::string& spec = "") {
        return fmt::format("{:" + spec + "o}", value);
    }
};

// Basic octal formatting without any flags
TEST_F(OctalFormatTest, BasicOctal) {
    EXPECT_EQ(format_octal(0), "0");
    EXPECT_EQ(format_octal(1), "1");
    EXPECT_EQ(format_octal(7), "7");
    EXPECT_EQ(format_octal(8), "10");
    EXPECT_EQ(format_octal(64), "100");
    EXPECT_EQ(format_octal(511), "777");
    EXPECT_EQ(format_octal(512), "1000");
    EXPECT_EQ(format_octal(123456), "361100");
}

// Octal with alternate form (# flag) - this exercises on_oct()
TEST_F(OctalFormatTest, AlternateForm) {
    // Zero should not get prefix
    EXPECT_EQ(format_octal(0, "#"), "0");
    
    // Non-zero values should get '0' prefix
    EXPECT_EQ(format_octal(1, "#"), "01");
    EXPECT_EQ(format_octal(7, "#"), "07");
    EXPECT_EQ(format_octal(8, "#"), "010");
    EXPECT_EQ(format_octal(64, "#"), "0100");
    EXPECT_EQ(format_octal(511, "#"), "0777");
    EXPECT_EQ(format_octal(512, "#"), "01000");
    EXPECT_EQ(format_octal(123456, "#"), "0361100");
}

// Alternate form with precision - tests the precision comparison logic in on_oct()
TEST_F(OctalFormatTest, AlternateFormWithPrecision) {
    // Precision greater than number of digits: no prefix added
    EXPECT_EQ(format_octal(1, "#.2"), " 01");  // width 2, precision 2, no prefix
    EXPECT_EQ(format_octal(7, "#.3"), " 007"); // width 3, precision 3, no prefix
    
    // Precision equal to number of digits: prefix should be added
    EXPECT_EQ(format_octal(1, "#.1"), "01");   // 1 digit, precision 1 -> prefix
    EXPECT_EQ(format_octal(8, "#.2"), "010");  // 2 digits, precision 2 -> prefix
    
    // Precision less than number of digits: prefix should be added
    EXPECT_EQ(format_octal(64, "#.1"), "0100"); // 3 digits, precision 1 -> prefix
    EXPECT_EQ(format_octal(511, "#.2"), "0777"); // 3 digits, precision 2 -> prefix
    
    // Zero with alternate and precision
    EXPECT_EQ(format_octal(0, "#.0"), "0");    // zero, no prefix
    EXPECT_EQ(format_octal(0, "#.5"), "00000"); // zero, precision 5, no prefix
}

// Width and alternate form
TEST_F(OctalFormatTest, WidthAndAlternate) {
    EXPECT_EQ(format_octal(1, "#4"), "  01");
    EXPECT_EQ(format_octal(8, "#6"), "  010");
    EXPECT_EQ(format_octal(64, "#5"), "0100");
    EXPECT_EQ(format_octal(0, "#4"), "   0");
}

// Negative numbers (should not happen with octal, but test behavior)
TEST_F(OctalFormatTest, NegativeNumbers) {
    // Negative numbers in octal format - implementation defined, but should not crash
    EXPECT_EQ(format_octal(-1), fmt::format("{:o}", -1));
    EXPECT_EQ(format_octal(-8), fmt::format("{:o}", -8));
}

// Large values to test count_digits logic
TEST_F(OctalFormatTest, LargeValues) {
    EXPECT_EQ(format_octal(INT_MAX), fmt::format("{:o}", INT_MAX));
    EXPECT_EQ(format_octal(INT_MIN), fmt::format("{:o}", INT_MIN));
    EXPECT_EQ(format_octal(UINT_MAX), fmt::format("{:o}", UINT_MAX));
    EXPECT_EQ(format_octal(LLONG_MAX), fmt::format("{:o}", LLONG_MAX));
    EXPECT_EQ(format_octal(ULLONG_MAX), fmt::format("{:o}", ULLONG_MAX));
}

// Large values with alternate form
TEST_F(OctalFormatTest, LargeValuesAlternate) {
    EXPECT_EQ(format_octal(INT_MAX, "#"), fmt::format("{:#o}", INT_MAX));
    EXPECT_EQ(format_octal(UINT_MAX, "#"), fmt::format("{:#o}", UINT_MAX));
    EXPECT_EQ(format_octal(ULLONG_MAX, "#"), fmt::format("{:#o}", ULLONG_MAX));
}

// Precision without alternate form
TEST_F(OctalFormatTest, PrecisionOnly) {
    EXPECT_EQ(format_octal(0, ".5"), "00000");
    EXPECT_EQ(format_octal(1, ".5"), "00001");
    EXPECT_EQ(format_octal(8, ".5"), "00010");
    EXPECT_EQ(format_octal(64, ".5"), "00100");
    EXPECT_EQ(format_octal(511, ".5"), "00777");
}

// Width and precision combined
TEST_F(OctalFormatTest, WidthAndPrecision) {
    EXPECT_EQ(format_octal(1, "5.3"), "  001");
    EXPECT_EQ(format_octal(8, "5.3"), "  010");
    EXPECT_EQ(format_octal(64, "5.3"), "  100");
    EXPECT_EQ(format_octal(0, "5.3"), "  000");
}

// Alternate form with width and precision - comprehensive test
TEST_F(OctalFormatTest, AlternateWidthPrecision) {
    // When precision > digits: no prefix, width fills with spaces
    EXPECT_EQ(format_octal(1, "#5.3"), "  001"); // 1 digit, precision 3 > 1, no prefix
    EXPECT_EQ(format_octal(8, "#6.4"), "  0010"); // 2 digits, precision 4 > 2, no prefix
    
    // When precision <= digits: prefix added
    EXPECT_EQ(format_octal(1, "#5.1"), "   01"); // 1 digit, precision 1 <= 1, prefix
    EXPECT_EQ(format_octal(8, "#6.2"), "  010"); // 2 digits, precision 2 <= 2, prefix
    EXPECT_EQ(format_octal(64, "#6.2"), " 0100"); // 3 digits, precision 2 <= 3, prefix
    
    // Zero with alternate, width, precision
    EXPECT_EQ(format_octal(0, "#5.3"), "  000"); // zero, no prefix
    EXPECT_EQ(format_octal(0, "#5.0"), "    0"); // zero, precision 0, no prefix
}

// Edge case: value with exactly one digit
TEST_F(OctalFormatTest, SingleDigit) {
    EXPECT_EQ(format_octal(0, "#"), "0");
    EXPECT_EQ(format_octal(1, "#"), "01");
    EXPECT_EQ(format_octal(7, "#"), "07");
}

// Edge case: value with many digits
TEST_F(OctalFormatTest, ManyDigits) {
    // 2^30 in octal is 40000000000 (11 digits)
    EXPECT_EQ(format_octal(1 << 30, "#"), "040000000000");
    EXPECT_EQ(format_octal(1 << 30), "40000000000");
}

// Test with different integer types
TEST_F(OctalFormatTest, DifferentTypes) {
    EXPECT_EQ(format_octal(static_cast<short>(8)), "10");
    EXPECT_EQ(format_octal(static_cast<unsigned short>(8)), "10");
    EXPECT_EQ(format_octal(static_cast<long>(64)), "100");
    EXPECT_EQ(format_octal(static_cast<unsigned long>(64)), "100");
    EXPECT_EQ(format_octal(static_cast<long long>(512)), "1000");
    EXPECT_EQ(format_octal(static_cast<unsigned long long>(512)), "1000");
}

// Test that alternate form doesn't add prefix when precision is greater
TEST_F(OctalFormatTest, AlternateNoPrefixWhenPrecisionGreater) {
    // These should NOT have the '0' prefix because precision > num_digits
    EXPECT_EQ(format_octal(1, "#.2"), " 01");  // width 2, precision 2 > 1
    EXPECT_EQ(format_octal(7, "#.3"), " 007"); // width 3, precision 3 > 1
    EXPECT_EQ(format_octal(8, "#.4"), " 0010"); // width 4, precision 4 > 2
    EXPECT_EQ(format_octal(64, "#.5"), " 00100"); // width 5, precision 5 > 3
}

// Verify that prefix is added when precision equals number of digits
TEST_F(OctalFormatTest, AlternatePrefixWhenPrecisionEquals) {
    EXPECT_EQ(format_octal(1, "#.1"), "01");   // 1 digit, precision 1 -> prefix
    EXPECT_EQ(format_octal(8, "#.2"), "010");  // 2 digits, precision 2 -> prefix
    EXPECT_EQ(format_octal(64, "#.3"), "0100"); // 3 digits, precision 3 -> prefix
}

// Verify that prefix is added when precision is less than number of digits
TEST_F(OctalFormatTest, AlternatePrefixWhenPrecisionLess) {
    EXPECT_EQ(format_octal(8, "#.1"), "010");  // 2 digits, precision 1 < 2 -> prefix
    EXPECT_EQ(format_octal(64, "#.2"), "0100"); // 3 digits, precision 2 < 3 -> prefix
    EXPECT_EQ(format_octal(512, "#.2"), "01000"); // 4 digits, precision 2 < 4 -> prefix
}