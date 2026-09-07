#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Tests for the formatter's handling of negative numbers with custom width/precision
// This exercises the focal function's path where a negative number is formatted
// with padding, and the '-' prefix is placed correctly

TEST(NegativeNumberFormatTest, DefaultFormatNegativeInt) {
    // Basic negative integer formatting
    EXPECT_EQ(fmt::format("{}", -42), "-42");
    EXPECT_EQ(fmt::format("{}", -1), "-1");
    EXPECT_EQ(fmt::format("{}", -1000), "-1000");
}

TEST(NegativeNumberFormatTest, NegativeIntWithWidth) {
    // Negative number with width - should be right-aligned with '-' prefix before padding
    EXPECT_EQ(fmt::format("{:5}", -42),  "  -42");
    EXPECT_EQ(fmt::format("{:6}", -42), "   -42");
    EXPECT_EQ(fmt::format("{:3}", -42), "-42");  // width equals size
    EXPECT_EQ(fmt::format("{:2}", -42), "-42");  // width less than size
}

TEST(NegativeNumberFormatTest, NegativeIntZeroWidth) {
    // Zero width should not affect output
    EXPECT_EQ(fmt::format("{:0}", -42), "-42");
}

TEST(NegativeNumberFormatTest, NegativeIntWithPrecision) {
    // Precision can increase number width including sign
    EXPECT_EQ(fmt::format("{:.5}", -42),   "-00042");
    EXPECT_EQ(fmt::format("{:.3}", -42),   "-042");
    EXPECT_EQ(fmt::format("{:.2}", -42),   "-42");
    EXPECT_EQ(fmt::format("{:.1}", -42),   "-42");  // precision less than digits
}

TEST(NegativeNumberFormatTest, NegativeIntWithWidthAndPrecision) {
    // Both width and precision - width includes sign
    EXPECT_EQ(fmt::format("{:8.5}", -42),  "  -00042");
    EXPECT_EQ(fmt::format("{:8.3}", -42),  "    -042");
    EXPECT_EQ(fmt::format("{:5.3}", -42),  " -042");
    EXPECT_EQ(fmt::format("{:4.3}", -42),  "-042");  // width equals padded size
}

TEST(NegativeNumberFormatTest, NegativeIntLeftAligned) {
    // Left alignment - '-' prefix stays at leftmost, padding on right
    EXPECT_EQ(fmt::format("{:<5}", -42), "-42  ");
    EXPECT_EQ(fmt::format("{:<6}", -42), "-42   ");
    EXPECT_EQ(fmt::format("{:<3}", -42), "-42");
}

TEST(NegativeNumberFormatTest, NegativeIntCenterAligned) {
    // Center alignment with sign
    EXPECT_EQ(fmt::format("{:^5}", -42), " -42 ");
    EXPECT_EQ(fmt::format("{:^6}", -42), " -42  ");
    EXPECT_EQ(fmt::format("{:^7}", -42), "  -42  ");
}

TEST(NegativeNumberFormatTest, NegativeLongLong) {
    // Larger integer types
    EXPECT_EQ(fmt::format("{}", -100000000000LL), "-100000000000");
    EXPECT_EQ(fmt::format("{:20}", -1234567890123LL), "    -1234567890123");
}

TEST(NegativeNumberFormatTest, NegativeDouble) {
    // Floating point negative - width includes sign
    EXPECT_EQ(fmt::format("{}", -3.14), "-3.14");
    EXPECT_EQ(fmt::format("{:10}", -3.14), "     -3.14");
    EXPECT_EQ(fmt::format("{:10.2}", -3.14), "     -3.14");
}

TEST(NegativeNumberFormatTest, NegativeDoubleWithPrecision) {
    // Precision controlled double
    EXPECT_EQ(fmt::format("{:.1}", -3.14), "-3.1");
    EXPECT_EQ(fmt::format("{:.3}", -3.14), "-3.140");
    EXPECT_EQ(fmt::format("{:8.1}", -3.14), "    -3.1");
}

TEST(NegativeNumberFormatTest, NegativeDoubleZeroPrecision) {
    // Zero precision rounding
    EXPECT_EQ(fmt::format("{:.0f}", -3.9), "-4");
    EXPECT_EQ(fmt::format("{:.0f}", -3.1), "-3");
}

TEST(NegativeNumberFormatTest, MinimumIntNegative) {
    // Minimum integer value (special case in some representations)
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::min()), "-2147483648");
    EXPECT_EQ(fmt::format("{:15}", std::numeric_limits<int>::min()), "    -2147483648");
}

TEST(NegativeNumberFormatTest, NegativeZeroFloat) {
    // Negative zero formatting
    EXPECT_EQ(fmt::format("{}", -0.0), "-0.0");
    EXPECT_EQ(fmt::format("{:5}", -0.0), " -0.0");
}

TEST(NegativeNumberFormatTest, NegativeIntWithFillChar) {
    // Custom fill character
    EXPECT_EQ(fmt::format("{:*>5}", -42), "**-42");
    EXPECT_EQ(fmt::format("{:*<5}", -42), "-42**");
    EXPECT_EQ(fmt::format("{:*^5}", -42), "-42**");  // center with odd width
    EXPECT_EQ(fmt::format("{:*^6}", -42), "*-42**");
}

TEST(NegativeNumberFormatTest, NegativeIntWithSignOption) {
    // Sign options - '-' is default for negatives
    EXPECT_EQ(fmt::format("{:+}", 42),  "+42");
    EXPECT_EQ(fmt::format("{:+}", -42), "-42");
    EXPECT_EQ(fmt::format("{: }", 42),  " 42");
    EXPECT_EQ(fmt::format("{: }", -42), "-42");
}

TEST(NegativeNumberFormatTest, NegativeBinaryFormat) {
    // Alternate formats - binary
    // Correct behavior is a bit tricky: sign should prefix the representation
    EXPECT_EQ(fmt::format("{:b}", -1),  "-1");
    EXPECT_EQ(fmt::format("{:#b}", -1), "-0b1");
}

TEST(NegativeNumberFormatTest, NegativeHexFormat) {
    EXPECT_EQ(fmt::format("{:x}", -255),  "-ff");
    EXPECT_EQ(fmt::format("{:#x}", -255), "-0xff");
}

TEST(NegativeNumberFormatTest, NegativeOctalFormat) {
    EXPECT_EQ(fmt::format("{:o}", -8),  "-10");
    EXPECT_EQ(fmt::format("{:#o}", -8), "-010");
}

TEST(NegativeNumberFormatTest, VeryLargeWidth) {
    // Large width should still work (just produces large string)
    // This tests that the implementation doesn't have overflow issues with sizes
    std::string result = fmt::format("{:100}", -42);
    EXPECT_EQ(result.size(), 100);
    EXPECT_EQ(result.substr(97), " -42");  // last 4 chars should be right-aligned negative
    EXPECT_EQ(result[0], ' ');  // first char should be padding
}

TEST(NegativeNumberFormatTest, NegativeNumberInTable) {
    // Formatting multiple values (pathological combination)
    std::vector<int> values = {1, -2, 3, -4, 5};
    std::string result;
    for (auto v : values) {
        result += fmt::format("{:5}", v);
    }
    EXPECT_EQ(result, "    1   -2    3   -4    5");
}