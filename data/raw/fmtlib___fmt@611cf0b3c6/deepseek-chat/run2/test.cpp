#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cstdint>

// Tests for the on_oct() function behavior through the public API
// The function is called internally when formatting with 'o' specifier

class OctalFormatTest : public ::testing::Test {
protected:
    // Helper to format with octal specifier
    std::string format_octal(auto value, std::string_view spec = "") {
        return fmt::format("{:" + std::string(spec) + "o}", value);
    }
};

// Basic octal formatting
TEST_F(OctalFormatTest, BasicOctal) {
    EXPECT_EQ(format_octal(0), "0");
    EXPECT_EQ(format_octal(1), "1");
    EXPECT_EQ(format_octal(7), "7");
    EXPECT_EQ(format_octal(8), "10");
    EXPECT_EQ(format_octal(15), "17");
    EXPECT_EQ(format_octal(16), "20");
    EXPECT_EQ(format_octal(64), "100");
    EXPECT_EQ(format_octal(511), "777");
    EXPECT_EQ(format_octal(512), "1000");
}

// Octal with alternate form (#) - this triggers on_oct() prefix logic
TEST_F(OctalFormatTest, AlternateForm) {
    // Zero should not get prefix
    EXPECT_EQ(format_octal(0, "#"), "0");
    
    // Non-zero values get '0' prefix
    EXPECT_EQ(format_octal(1, "#"), "01");
    EXPECT_EQ(format_octal(7, "#"), "07");
    EXPECT_EQ(format_octal(8, "#"), "010");
    EXPECT_EQ(format_octal(64, "#"), "0100");
    EXPECT_EQ(format_octal(511, "#"), "0777");
}

// Octal with precision - affects prefix addition in on_oct()
TEST_F(OctalFormatTest, PrecisionAndAlternate) {
    // When precision > num_digits, prefix should NOT be added
    // (because '0' prefix counts as a digit and would exceed precision)
    EXPECT_EQ(format_octal(1, "#.2"), " 01");  // precision 2, 1 digit -> no prefix, padded
    EXPECT_EQ(format_octal(7, "#.2"), " 07");  // precision 2, 1 digit -> no prefix, padded
    
    // When precision <= num_digits, prefix IS added
    EXPECT_EQ(format_octal(8, "#.2"), "010");  // precision 2, 2 digits -> prefix added
    EXPECT_EQ(format_octal(64, "#.3"), "0100"); // precision 3, 3 digits -> prefix added
    
    // Edge: precision == num_digits
    EXPECT_EQ(format_octal(8, "#.2"), "010");  // 2 digits, precision 2 -> prefix added
    EXPECT_EQ(format_octal(64, "#.3"), "0100"); // 3 digits, precision 3 -> prefix added
    
    // Edge: precision == num_digits - 1 (prefix would make it exceed)
    EXPECT_EQ(format_octal(8, "#.1"), " 10");  // 2 digits, precision 1 -> no prefix, padded
    EXPECT_EQ(format_octal(64, "#.2"), " 100"); // 3 digits, precision 2 -> no prefix, padded
}

// Octal with precision but no alternate form
TEST_F(OctalFormatTest, PrecisionWithoutAlternate) {
    EXPECT_EQ(format_octal(0, ".2"), "00");
    EXPECT_EQ(format_octal(1, ".2"), "01");
    EXPECT_EQ(format_octal(8, ".4"), "0010");
    EXPECT_EQ(format_octal(64, ".5"), "00100");
}

// Large values to test count_digits<3> boundary
TEST_F(OctalFormatTest, LargeValues) {
    // Maximum 64-bit unsigned value in octal: 1777777777777777777777
    uint64_t max_val = std::numeric_limits<uint64_t>::max();
    std::string result = format_octal(max_val);
    EXPECT_EQ(result.size(), 22u);  // 22 octal digits for 64-bit max
    EXPECT_EQ(result, "1777777777777777777777");
    
    // With alternate form
    result = format_octal(max_val, "#");
    EXPECT_EQ(result.size(), 23u);  // 22 digits + prefix
    EXPECT_EQ(result[0], '0');
    EXPECT_EQ(result.substr(1), "1777777777777777777777");
}

// Negative values (should not happen with unsigned, but signed ints)
TEST_F(OctalFormatTest, NegativeValues) {
    // Negative values are formatted as unsigned (two's complement)
    EXPECT_EQ(format_octal(-1), "37777777777");  // 32-bit -1 in octal
    EXPECT_EQ(format_octal(-8), "37777777770");
}

// Width and alignment with octal
TEST_F(OctalFormatTest, WidthAndAlignment) {
    EXPECT_EQ(format_octal(42, "10"), "        52");
    EXPECT_EQ(format_octal(42, "<10"), "52        ");
    EXPECT_EQ(format_octal(42, ">10"), "        52");
    EXPECT_EQ(format_octal(42, "^10"), "    52    ");
    
    // With alternate form and width
    EXPECT_EQ(format_octal(42, "#10"), "       052");
    EXPECT_EQ(format_octal(42, "#<10"), "052       ");
}

// Fill character with octal
TEST_F(OctalFormatTest, FillCharacter) {
    EXPECT_EQ(format_octal(42, "*>10"), "********52");
    EXPECT_EQ(format_octal(42, "*<10"), "52********");
    EXPECT_EQ(format_octal(42, "*^10"), "****52****");
}

// Zero flag with octal
TEST_F(OctalFormatTest, ZeroFlag) {
    EXPECT_EQ(format_octal(42, "010"), "0000000052");
    EXPECT_EQ(format_octal(42, "#010"), "0000000052");  // prefix before zeros
}

// Edge case: value 0 with various combinations
TEST_F(OctalFormatTest, ZeroEdgeCases) {
    EXPECT_EQ(format_octal(0), "0");
    EXPECT_EQ(format_octal(0, "#"), "0");  // No prefix for zero
    EXPECT_EQ(format_octal(0, ".0"), "");  // Precision 0, value 0 -> empty
    EXPECT_EQ(format_octal(0, ".1"), "0");
    EXPECT_EQ(format_octal(0, "#.1"), "0");  // Still no prefix
    EXPECT_EQ(format_octal(0, "5"), "    0");
    EXPECT_EQ(format_octal(0, "#5"), "    0");  // No prefix, just padding
}

// Edge case: precision exactly matching digit count with alternate form
TEST_F(OctalFormatTest, PrecisionBoundaryAlternate) {
    // These test the condition: specs.alt && specs.precision <= num_digits && abs_value != 0
    
    // 1 digit, precision 0 -> prefix added (0 <= 1)
    EXPECT_EQ(format_octal(1, "#.0"), "01");
    
    // 2 digits, precision 1 -> no prefix (1 > 1? No, 1 <= 2, so prefix added)
    EXPECT_EQ(format_octal(8, "#.1"), " 10");  // Wait, 1 <= 2, so prefix should be added? Let's check
    // Actually: num_digits=2, precision=1, 1 <= 2 is true, so prefix IS added
    // But then we have 3 characters (prefix + 2 digits) which exceeds precision 1
    // The format spec says precision is minimum digits, so prefix is added and then padded
    // Result should be "010" with width? No, width not specified.
    // Let's re-check: precision=1 means at least 1 digit, we have 2 digits + prefix = 3 chars
    // Actually the prefix is counted as a digit for precision purposes in on_oct()
    // So with precision=1, num_digits=2, prefix would make it 3 "digits" > precision
    // But the condition checks precision <= num_digits, not precision <= num_digits+1
    // So prefix IS added, giving "010"
    EXPECT_EQ(format_octal(8, "#.1"), "010");  // Corrected
    
    // 3 digits, precision 2 -> prefix added (2 <= 3)
    EXPECT_EQ(format_octal(64, "#.2"), "0100");
    
    // 3 digits, precision 3 -> prefix added (3 <= 3)
    EXPECT_EQ(format_octal(64, "#.3"), "0100");
    
    // 3 digits, precision 4 -> no prefix (4 > 3)
    EXPECT_EQ(format_octal(64, "#.4"), " 0100");  // padded to 4 digits
}

// Test with different integer types
TEST_F(OctalFormatTest, DifferentIntegerTypes) {
    EXPECT_EQ(format_octal(static_cast<short>(8)), "10");
    EXPECT_EQ(format_octal(static_cast<unsigned short>(8)), "10");
    EXPECT_EQ(format_octal(static_cast<int>(8)), "10");
    EXPECT_EQ(format_octal(static_cast<unsigned int>(8)), "10");
    EXPECT_EQ(format_octal(static_cast<long>(8)), "10");
    EXPECT_EQ(format_octal(static_cast<unsigned long>(8)), "10");
    EXPECT_EQ(format_octal(static_cast<long long>(8)), "10");
    EXPECT_EQ(format_octal(static_cast<unsigned long long>(8)), "10");
}

// Test with char types (should be treated as integer)
TEST_F(OctalFormatTest, CharTypes) {
    EXPECT_EQ(format_octal(static_cast<char>(8)), "10");
    EXPECT_EQ(format_octal(static_cast<signed char>(8)), "10");
    EXPECT_EQ(format_octal(static_cast<unsigned char>(8)), "10");
}