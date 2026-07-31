#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>

TEST(FormatOctalTest, BasicOctalFormatting) {
    // Test normal octal formatting
    std::string result = fmt::format("{:o}", 123);
    EXPECT_EQ(result, "173");
}

TEST(FormatOctalTest, OctalWithAltFlag) {
    // Test octal with alt flag (should add leading 0)
    std::string result = fmt::format("{:#o}", 123);
    EXPECT_EQ(result, "0173");
}

TEST(FormatOctalTest, OctalWithAltFlagZero) {
    // Test octal with alt flag and zero value
    std::string result = fmt::format("{:#o}", 0);
    EXPECT_EQ(result, "0");
}

TEST(FormatOctalTest, OctalWithPrecision) {
    // Test octal with precision
    std::string result = fmt::format("{:05o}", 123);
    EXPECT_EQ(result, "00173");
}

TEST(FormatOctalTest, OctalWithAltFlagAndPrecision) {
    // Test octal with alt flag and precision
    std::string result = fmt::format("{:#05o}", 123);
    EXPECT_EQ(result, "00173");
}

TEST(FormatOctalTest, OctalWithAltFlagAndPrecisionLessThanDigits) {
    // Test octal with alt flag and precision less than number of digits
    std::string result = fmt::format("{:#.2o}", 123);
    EXPECT_EQ(result, "173");  // Should not add leading 0 since precision < digits
}

TEST(FormatOctalTest, OctalWithAltFlagAndPrecisionGreaterThanDigits) {
    // Test octal with alt flag and precision greater than number of digits
    std::string result = fmt::format("{:#.5o}", 123);
    EXPECT_EQ(result, "00173");  // Should add leading 0 and pad with zeros
}

TEST(FormatOctalTest, OctalNegativeValue) {
    // Test negative octal value
    std::string result = fmt::format("{:o}", -123);
    EXPECT_EQ(result, "-173");
}

TEST(FormatOctalTest, OctalNegativeWithAltFlag) {
    // Test negative octal with alt flag
    std::string result = fmt::format("{:#o}", -123);
    EXPECT_EQ(result, "-0173");
}

TEST(FormatOctalTest, OctalZero) {
    // Test zero octal formatting
    std::string result = fmt::format("{:o}", 0);
    EXPECT_EQ(result, "0");
}

TEST(FormatOctalTest, OctalZeroWithAltFlag) {
    // Test zero octal with alt flag
    std::string result = fmt::format("{:#o}", 0);
    EXPECT_EQ(result, "0");
}

TEST(FormatOctalTest, OctalLargeValue) {
    // Test large octal value
    std::string result = fmt::format("{:o}", 1000000);
    EXPECT_EQ(result, "3640000");
}

TEST(FormatOctalTest, OctalLargeValueWithAltFlag) {
    // Test large octal value with alt flag
    std::string result = fmt::format("{:#o}", 1000000);
    EXPECT_EQ(result, "03640000");
}

TEST(FormatOctalTest, OctalMaxValue) {
    // Test maximum value that fits in int
    std::string result = fmt::format("{:o}", 2147483647);
    EXPECT_EQ(result, "17777777777");
}

TEST(FormatOctalTest, OctalMaxValueWithAltFlag) {
    // Test maximum value with alt flag
    std::string result = fmt::format("{:#o}", 2147483647);
    EXPECT_EQ(result, "017777777777");
}