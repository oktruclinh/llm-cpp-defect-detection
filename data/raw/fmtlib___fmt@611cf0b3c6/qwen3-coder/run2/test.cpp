#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>

// Test fixture to provide common setup
class OctalFormatTest : public ::testing::Test {
protected:
    void SetUp() override {
    }
};

TEST_F(OctalFormatTest, BasicOctalFormatting) {
    // Test normal octal formatting without alt flag
    std::string result = fmt::format("{:o}", 64);
    EXPECT_EQ(result, "100");
}

TEST_F(OctalFormatTest, OctalWithAltFlag) {
    // Test octal formatting with alt flag
    std::string result = fmt::format("{:#o}", 64);
    EXPECT_EQ(result, "0100");
}

TEST_F(OctalFormatTest, ZeroValueNoAlt) {
    // Test zero value without alt flag
    std::string result = fmt::format("{:o}", 0);
    EXPECT_EQ(result, "0");
}

TEST_F(OctalFormatTest, ZeroValueWithAlt) {
    // Test zero value with alt flag
    std::string result = fmt::format("{:#o}", 0);
    EXPECT_EQ(result, "0");
}

TEST_F(OctalFormatTest, LargeValue) {
    // Test large octal value
    std::string result = fmt::format("{:o}", 1000);
    EXPECT_EQ(result, "1750");
}

TEST_F(OctalFormatTest, NegativeValue) {
    // Test negative value (should use absolute value)
    std::string result = fmt::format("{:#o}", -64);
    EXPECT_EQ(result, "0100");
}

TEST_F(OctalFormatTest, PrecisionGreaterThanDigits) {
    // Test when precision is greater than number of digits
    std::string result = fmt::format("{:#04o}", 64);
    EXPECT_EQ(result, "0100");
}

TEST_F(OctalFormatTest, PrecisionEqualToDigits) {
    // Test when precision equals number of digits
    std::string result = fmt::format("{:#03o}", 64);
    EXPECT_EQ(result, "0100");
}

TEST_F(OctalFormatTest, PrecisionLessThanDigits) {
    // Test when precision is less than number of digits
    std::string result = fmt::format("{:#02o}", 64);
    EXPECT_EQ(result, "0100");
}

TEST_F(OctalFormatTest, OctalWithLeadingZeros) {
    // Test octal with leading zeros due to precision
    std::string result = fmt::format("{:04o}", 8);
    EXPECT_EQ(result, "0010");
}

TEST_F(OctalFormatTest, EdgeCaseMaxValue) {
    // Test maximum value that fits in unsigned long long
    std::string result = fmt::format("{:o}", 18446744073709551615ULL);
    EXPECT_EQ(result, "1777777777777777777777");
}

TEST_F(OctalFormatTest, SingleDigitOctal) {
    // Test single digit octal numbers
    std::string result = fmt::format("{:#o}", 7);
    EXPECT_EQ(result, "07");
}

TEST_F(OctalFormatTest, OctalWithZeroPrecision) {
    // Test octal with zero precision
    std::string result = fmt::format("{:#o}", 123);
    EXPECT_EQ(result, "0173");
}