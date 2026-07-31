#include <gtest/gtest.h>
#include <fmt/format.h>
#include <cmath>
#include <limits>
#include <string>

// Test formatting of infinity and NaN values
TEST(FormatDoubleTest, InfinityAndNaN) {
    double inf = std::numeric_limits<double>::infinity();
    double nan = std::numeric_limits<double>::quiet_NaN();
    double neg_inf = -inf;
    double neg_nan = -nan;

    // Test infinity
    EXPECT_EQ(fmt::format("{}", inf), "inf");
    EXPECT_EQ(fmt::format("{:f}", inf), "inf");
    EXPECT_EQ(fmt::format("{:e}", inf), "inf");
    EXPECT_EQ(fmt::format("{:g}", inf), "inf");
    EXPECT_EQ(fmt::format("{:F}", inf), "INF");
    EXPECT_EQ(fmt::format("{:E}", inf), "INF");
    EXPECT_EQ(fmt::format("{:G}", inf), "INF");

    // Test negative infinity
    EXPECT_EQ(fmt::format("{}", neg_inf), "-inf");
    EXPECT_EQ(fmt::format("{:f}", neg_inf), "-inf");
    EXPECT_EQ(fmt::format("{:F}", neg_inf), "-INF");

    // Test NaN
    EXPECT_EQ(fmt::format("{}", nan), "nan");
    EXPECT_EQ(fmt::format("{:f}", nan), "nan");
    EXPECT_EQ(fmt::format("{:e}", nan), "nan");
    EXPECT_EQ(fmt::format("{:g}", nan), "nan");
    EXPECT_EQ(fmt::format("{:F}", nan), "NAN");
    EXPECT_EQ(fmt::format("{:E}", nan), "NAN");
    EXPECT_EQ(fmt::format("{:G}", nan), "NAN");

    // Test negative NaN (should still be nan, not -nan)
    EXPECT_EQ(fmt::format("{}", neg_nan), "nan");
    EXPECT_EQ(fmt::format("{:f}", neg_nan), "nan");
    EXPECT_EQ(fmt::format("{:F}", neg_nan), "NAN");
}

// Test sign handling with infinity and NaN
TEST(FormatDoubleTest, InfinityAndNaNWithSign) {
    double inf = std::numeric_limits<double>::infinity();
    double nan = std::numeric_limits<double>::quiet_NaN();

    // Force sign with '+' flag
    EXPECT_EQ(fmt::format("{:+}", inf), "+inf");
    EXPECT_EQ(fmt::format("{:+}", nan), "+nan");
    EXPECT_EQ(fmt::format("{:+F}", inf), "+INF");
    EXPECT_EQ(fmt::format("{:+F}", nan), "+NAN");

    // Space flag
    EXPECT_EQ(fmt::format("{: }", inf), " inf");
    EXPECT_EQ(fmt::format("{: }", nan), " nan");
    EXPECT_EQ(fmt::format("{: F}", inf), " INF");
    EXPECT_EQ(fmt::format("{: F}", nan), " NAN");
}

// Test padding with infinity and NaN
TEST(FormatDoubleTest, InfinityAndNaNWithPadding) {
    double inf = std::numeric_limits<double>::infinity();
    double nan = std::numeric_limits<double>::quiet_NaN();

    // Right alignment
    EXPECT_EQ(fmt::format("{:10}", inf), "       inf");
    EXPECT_EQ(fmt::format("{:10}", nan), "       nan");
    EXPECT_EQ(fmt::format("{:10F}", inf), "       INF");
    EXPECT_EQ(fmt::format("{:10F}", nan), "       NAN");

    // Left alignment
    EXPECT_EQ(fmt::format("{:<10}", inf), "inf       ");
    EXPECT_EQ(fmt::format("{:<10}", nan), "nan       ");
    EXPECT_EQ(fmt::format("{:<10F}", inf), "INF       ");
    EXPECT_EQ(fmt::format("{:<10F}", nan), "NAN       ");

    // Center alignment
    EXPECT_EQ(fmt::format("{:^10}", inf), "   inf    ");
    EXPECT_EQ(fmt::format("{:^10}", nan), "   nan    ");
    EXPECT_EQ(fmt::format("{:^10F}", inf), "   INF    ");
    EXPECT_EQ(fmt::format("{:^10F}", nan), "   NAN    ");
}

// Test padding with sign and infinity/NaN
TEST(FormatDoubleTest, InfinityAndNaNWithSignAndPadding) {
    double inf = std::numeric_limits<double>::infinity();
    double nan = std::numeric_limits<double>::quiet_NaN();

    // Right alignment with sign
    EXPECT_EQ(fmt::format("{:+10}", inf), "      +inf");
    EXPECT_EQ(fmt::format("{:+10}", nan), "      +nan");
    EXPECT_EQ(fmt::format("{:+10F}", inf), "      +INF");
    EXPECT_EQ(fmt::format("{:+10F}", nan), "      +NAN");

    // Left alignment with sign
    EXPECT_EQ(fmt::format("{:<+10}", inf), "+inf      ");
    EXPECT_EQ(fmt::format("{:<+10}", nan), "+nan      ");
    EXPECT_EQ(fmt::format("{:<+10F}", inf), "+INF      ");
    EXPECT_EQ(fmt::format("{:<+10F}", nan), "+NAN      ");
}

// Test numeric alignment with infinity and NaN (should behave like right alignment)
TEST(FormatDoubleTest, InfinityAndNaNWithNumericAlignment) {
    double inf = std::numeric_limits<double>::infinity();
    double nan = std::numeric_limits<double>::quiet_NaN();

    // Numeric alignment (0=) should be treated as right alignment for inf/nan
    EXPECT_EQ(fmt::format("{:010}", inf), "       inf");
    EXPECT_EQ(fmt::format("{:010}", nan), "       nan");
    EXPECT_EQ(fmt::format("{:010F}", inf), "       INF");
    EXPECT_EQ(fmt::format("{:010F}", nan), "       NAN");
}

// Test that negative infinity and NaN preserve sign in output
TEST(FormatDoubleTest, NegativeInfinityAndNaN) {
    double neg_inf = -std::numeric_limits<double>::infinity();
    double neg_nan = -std::numeric_limits<double>::quiet_NaN();

    // Negative infinity should show minus sign
    EXPECT_EQ(fmt::format("{}", neg_inf), "-inf");
    EXPECT_EQ(fmt::format("{:f}", neg_inf), "-inf");
    EXPECT_EQ(fmt::format("{:F}", neg_inf), "-INF");

    // Negative NaN should still be "nan" (NaN has no sign)
    EXPECT_EQ(fmt::format("{}", neg_nan), "nan");
    EXPECT_EQ(fmt::format("{:f}", neg_nan), "nan");
    EXPECT_EQ(fmt::format("{:F}", neg_nan), "NAN");
}

// Test that infinity and NaN work with width and precision specifiers
TEST(FormatDoubleTest, InfinityAndNaNWithWidthAndPrecision) {
    double inf = std::numeric_limits<double>::infinity();
    double nan = std::numeric_limits<double>::quiet_NaN();

    // Width only
    EXPECT_EQ(fmt::format("{:5}", inf), "  inf");
    EXPECT_EQ(fmt::format("{:5}", nan), "  nan");

    // Precision should be ignored for inf/nan
    EXPECT_EQ(fmt::format("{:.5}", inf), "inf");
    EXPECT_EQ(fmt::format("{:.5}", nan), "nan");
    EXPECT_EQ(fmt::format("{:.5f}", inf), "inf");
    EXPECT_EQ(fmt::format("{:.5f}", nan), "nan");

    // Both width and precision
    EXPECT_EQ(fmt::format("{:8.3}", inf), "     inf");
    EXPECT_EQ(fmt::format("{:8.3}", nan), "     nan");
}

// Test that infinity and NaN work with fill character
TEST(FormatDoubleTest, InfinityAndNaNWithFill) {
    double inf = std::numeric_limits<double>::infinity();
    double nan = std::numeric_limits<double>::quiet_NaN();

    EXPECT_EQ(fmt::format("{:*>10}", inf), "*******inf");
    EXPECT_EQ(fmt::format("{:*>10}", nan), "*******nan");
    EXPECT_EQ(fmt::format("{:*<10}", inf), "inf*******");
    EXPECT_EQ(fmt::format("{:*<10}", nan), "nan*******");
    EXPECT_EQ(fmt::format("{:*^10}", inf), "***inf****");
    EXPECT_EQ(fmt::format("{:*^10}", nan), "***nan****");
}

// Test that infinity and NaN work with upper case specifiers
TEST(FormatDoubleTest, InfinityAndNaNUpperCase) {
    double inf = std::numeric_limits<double>::infinity();
    double nan = std::numeric_limits<double>::quiet_NaN();

    EXPECT_EQ(fmt::format("{:F}", inf), "INF");
    EXPECT_EQ(fmt::format("{:E}", inf), "INF");
    EXPECT_EQ(fmt::format("{:G}", inf), "INF");
    EXPECT_EQ(fmt::format("{:F}", nan), "NAN");
    EXPECT_EQ(fmt::format("{:E}", nan), "NAN");
    EXPECT_EQ(fmt::format("{:G}", nan), "NAN");
}

// Test that infinity and NaN work with mixed case specifiers
TEST(FormatDoubleTest, InfinityAndNaNLowerCase) {
    double inf = std::numeric_limits<double>::infinity();
    double nan = std::numeric_limits<double>::quiet_NaN();

    EXPECT_EQ(fmt::format("{:f}", inf), "inf");
    EXPECT_EQ(fmt::format("{:e}", inf), "inf");
    EXPECT_EQ(fmt::format("{:g}", inf), "inf");
    EXPECT_EQ(fmt::format("{:f}", nan), "nan");
    EXPECT_EQ(fmt::format("{:e}", nan), "nan");
    EXPECT_EQ(fmt::format("{:g}", nan), "nan");
}