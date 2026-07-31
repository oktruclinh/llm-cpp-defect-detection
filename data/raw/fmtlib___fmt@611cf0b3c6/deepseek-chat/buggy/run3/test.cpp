#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <climits>

// Test fixture for octal formatting tests
class OctalFormatTest : public ::testing::Test {
protected:
    // Helper to format a value as octal with given format spec
    std::string format_octal(int value, const std::string& format_spec) {
        return fmt::format(fmt::format("{{:{}}}", format_spec), value);
    }
};

// Basic octal formatting without any flags
TEST_F(OctalFormatTest, BasicOctal) {
    EXPECT_EQ(format_octal(0, "o"), "0");
    EXPECT_EQ(format_octal(1, "o"), "1");
    EXPECT_EQ(format_octal(7, "o"), "7");
    EXPECT_EQ(format_octal(8, "o"), "10");
    EXPECT_EQ(format_octal(64, "o"), "100");
    EXPECT_EQ(format_octal(255, "o"), "377");
    EXPECT_EQ(format_octal(INT_MAX, "o"), "17777777777");
}

// Octal with alternate form ('#' flag) - this exercises on_oct()
TEST_F(OctalFormatTest, AlternateForm) {
    // Zero with alternate form
    EXPECT_EQ(format_octal(0, "#o"), "0");
    
    // Positive values with alternate form
    EXPECT_EQ(format_octal(1, "#o"), "01");
    EXPECT_EQ(format_octal(7, "#o"), "07");
    EXPECT_EQ(format_octal(8, "#o"), "010");
    EXPECT_EQ(format_octal(64, "#o"), "0100");
    EXPECT_EQ(format_octal(255, "#o"), "0377");
}

// Octal with precision - precision should pad with zeros
TEST_F(OctalFormatTest, WithPrecision) {
    // Precision greater than number of digits
    EXPECT_EQ(format_octal(0, "#.5o"), "00000");
    EXPECT_EQ(format_octal(1, "#.5o"), "00001");
    EXPECT_EQ(format_octal(7, "#.5o"), "00007");
    EXPECT_EQ(format_octal(8, "#.5o"), "00010");
    
    // Precision equal to number of digits
    EXPECT_EQ(format_octal(0, "#.1o"), "0");
    EXPECT_EQ(format_octal(7, "#.1o"), "07");
    EXPECT_EQ(format_octal(8, "#.2o"), "010");
    
    // Precision less than number of digits - no prefix added
    EXPECT_EQ(format_octal(64, "#.2o"), "100");  // 64 = 100 octal, 3 digits, precision 2 < 3
    EXPECT_EQ(format_octal(255, "#.2o"), "377"); // 255 = 377 octal, 3 digits, precision 2 < 3
}

// Octal with precision and alternate form - edge cases
TEST_F(OctalFormatTest, AlternateFormWithPrecision) {
    // When precision > num_digits, prefix should NOT be added (per on_oct logic)
    EXPECT_EQ(format_octal(1, "#.10o"), "0000000001");  // 10 > 1 digit, no prefix
    EXPECT_EQ(format_octal(8, "#.10o"), "0000000010");  // 10 > 2 digits, no prefix
    
    // When precision == num_digits, prefix should be added
    EXPECT_EQ(format_octal(1, "#.1o"), "01");   // 1 digit, precision 1
    EXPECT_EQ(format_octal(8, "#.2o"), "010");  // 2 digits, precision 2
    
    // When precision < num_digits, prefix should be added
    EXPECT_EQ(format_octal(8, "#.1o"), "010");  // 2 digits, precision 1 < 2
}

// Negative numbers - should be formatted as unsigned
TEST_F(OctalFormatTest, NegativeNumbers) {
    EXPECT_EQ(format_octal(-1, "o"), "37777777777");
    EXPECT_EQ(format_octal(-1, "#o"), "037777777777");
    EXPECT_EQ(format_octal(-8, "o"), "37777777770");
    EXPECT_EQ(format_octal(-8, "#o"), "037777777770");
}

// Zero value edge cases
TEST_F(OctalFormatTest, ZeroEdgeCases) {
    EXPECT_EQ(format_octal(0, "o"), "0");
    EXPECT_EQ(format_octal(0, "#o"), "0");
    EXPECT_EQ(format_octal(0, "#.0o"), "0");
    EXPECT_EQ(format_octal(0, "#.5o"), "00000");
    EXPECT_EQ(format_octal(0, ".5o"), "00000");
}

// Width and alignment with octal
TEST_F(OctalFormatTest, WidthAndAlignment) {
    EXPECT_EQ(format_octal(8, "10o"), "        10");
    EXPECT_EQ(format_octal(8, "#10o"), "       010");
    EXPECT_EQ(format_octal(8, "-10o"), "10        ");
    EXPECT_EQ(format_octal(8, "#-10o"), "010       ");
    EXPECT_EQ(format_octal(8, "010o"), "0000000010");
    EXPECT_EQ(format_octal(8, "#010o"), "0000000010");
}

// Large values
TEST_F(OctalFormatTest, LargeValues) {
    EXPECT_EQ(format_octal(INT_MAX, "o"), "17777777777");
    EXPECT_EQ(format_octal(INT_MAX, "#o"), "017777777777");
    EXPECT_EQ(format_octal(INT_MAX, "#.15o"), "0000017777777777");
    EXPECT_EQ(format_octal(INT_MAX, ".15o"), "0000017777777777");
}

// Precision zero with alternate form
TEST_F(OctalFormatTest, PrecisionZero) {
    EXPECT_EQ(format_octal(0, "#.0o"), "0");
    EXPECT_EQ(format_octal(1, "#.0o"), "01");
    EXPECT_EQ(format_octal(8, "#.0o"), "010");
}

// Verify that on_oct is called for octal formatting
TEST_F(OctalFormatTest, VerifyOctalFormattingPath) {
    // These should all trigger the octal formatting path
    EXPECT_NO_THROW(format_octal(42, "o"));
    EXPECT_NO_THROW(format_octal(42, "#o"));
    EXPECT_NO_THROW(format_octal(42, "#.5o"));
    EXPECT_NO_THROW(format_octal(42, ".5o"));
}

// Edge case: very large precision with alternate form
TEST_F(OctalFormatTest, LargePrecisionAlternateForm) {
    // When precision >> num_digits, prefix should not be added
    EXPECT_EQ(format_octal(1, "#.100o"), 
              std::string(99, '0') + "1");  // 100 precision, 1 digit, no prefix
    EXPECT_EQ(format_octal(8, "#.100o"), 
              std::string(98, '0') + "10"); // 100 precision, 2 digits, no prefix
}