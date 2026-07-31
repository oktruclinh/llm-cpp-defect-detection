#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <limits>
#include <cmath>
#include <locale>
#include <clocale>

// Test fixture for write_nonfinite function
class WriteNonfiniteTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Save current locale
        saved_locale_ = std::setlocale(LC_ALL, nullptr);
    }

    void TearDown() override {
        // Restore locale
        if (saved_locale_) {
            std::setlocale(LC_ALL, saved_locale_);
        }
    }

private:
    const char* saved_locale_ = nullptr;
};

// Test write_nonfinite with infinity
TEST_F(WriteNonfiniteTest, Infinity) {
    fmt::format_specs specs;
    specs.type = 'f';
    
    // Test positive infinity
    std::string result = fmt::format("{:f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "inf");
    
    // Test negative infinity
    result = fmt::format("{:f}", -std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "-inf");
}

// Test write_nonfinite with NaN
TEST_F(WriteNonfiniteTest, NaN) {
    fmt::format_specs specs;
    specs.type = 'f';
    
    // Test NaN
    std::string result = fmt::format("{:f}", std::numeric_limits<double>::quiet_NaN());
    EXPECT_EQ(result, "nan");
    
    // Test negative NaN
    result = fmt::format("{:f}", -std::numeric_limits<double>::quiet_NaN());
    EXPECT_EQ(result, "-nan");
}

// Test write_nonfinite with uppercase specifier
TEST_F(WriteNonfiniteTest, Uppercase) {
    // Test uppercase infinity
    std::string result = fmt::format("{:F}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "INF");
    
    // Test uppercase NaN
    result = fmt::format("{:F}", std::numeric_limits<double>::quiet_NaN());
    EXPECT_EQ(result, "NAN");
}

// Test write_nonfinite with sign specifiers
TEST_F(WriteNonfiniteTest, SignSpecifiers) {
    // Test with plus sign
    std::string result = fmt::format("{:+f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "+inf");
    
    // Test with space sign
    result = fmt::format("{: f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, " inf");
    
    // Test with minus sign (should still show minus for negative)
    result = fmt::format("{:-f}", -std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "-inf");
}

// Test write_nonfinite with width and alignment
TEST_F(WriteNonfiniteTest, WidthAndAlignment) {
    // Test right alignment with width
    std::string result = fmt::format("{:>8f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "     inf");
    
    // Test left alignment with width
    result = fmt::format("{:<8f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "inf     ");
    
    // Test center alignment with width
    result = fmt::format("{:^8f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "  inf   ");
}

// Test write_nonfinite with fill character
TEST_F(WriteNonfiniteTest, FillCharacter) {
    std::string result = fmt::format("{:*>8f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "*****inf");
}

// Test write_nonfinite with precision (should be ignored for nonfinite)
TEST_F(WriteNonfiniteTest, PrecisionIgnored) {
    std::string result = fmt::format("{:.10f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "inf");
    
    result = fmt::format("{:.0f}", std::numeric_limits<double>::quiet_NaN());
    EXPECT_EQ(result, "nan");
}

// Test write_nonfinite with locale-specific grouping (should not affect nonfinite)
TEST_F(WriteNonfiniteTest, LocaleGrouping) {
    // Set locale with grouping
    std::setlocale(LC_ALL, "en_US.UTF-8");
    
    std::string result = fmt::format("{:f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "inf");
    
    std::setlocale(LC_ALL, "C");
}

// Test write_nonfinite with different floating point types
TEST_F(WriteNonfiniteTest, FloatTypes) {
    // Test with float
    std::string result = fmt::format("{:f}", std::numeric_limits<float>::infinity());
    EXPECT_EQ(result, "inf");
    
    // Test with long double
    result = fmt::format("{:f}", std::numeric_limits<long double>::infinity());
    EXPECT_EQ(result, "inf");
}

// Test write_nonfinite with custom format specifiers
TEST_F(WriteNonfiniteTest, CustomSpecifiers) {
    // Test with 'g' type
    std::string result = fmt::format("{:g}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "inf");
    
    // Test with 'e' type
    result = fmt::format("{:e}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "inf");
    
    // Test with 'a' type
    result = fmt::format("{:a}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "inf");
}

// Test write_nonfinite with zero padding
TEST_F(WriteNonfiniteTest, ZeroPadding) {
    std::string result = fmt::format("{:08f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "00000inf");
}

// Test write_nonfinite with alternative form (#)
TEST_F(WriteNonfiniteTest, AlternativeForm) {
    // Alternative form should not affect nonfinite values
    std::string result = fmt::format("{:#f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "inf");
}

// Test write_nonfinite with both sign and width
TEST_F(WriteNonfiniteTest, SignAndWidth) {
    std::string result = fmt::format("{:+10f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "      +inf");
    
    result = fmt::format("{:-10f}", -std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "      -inf");
}

// Test write_nonfinite with fill, alignment, and sign
TEST_F(WriteNonfiniteTest, FillAlignmentAndSign) {
    std::string result = fmt::format("{:*>+10f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "*****+inf");
    
    result = fmt::format("{:*<+10f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "+inf*****");
}

// Test write_nonfinite with NaN payload (should still output "nan")
TEST_F(WriteNonfiniteTest, NaNPayload) {
    // Create NaN with payload (implementation-defined, but should still output "nan")
    double nan_with_payload;
    std::memcpy(&nan_with_payload, "\x7f\xf8\x00\x00\x00\x00\x00\x01", 8);
    
    std::string result = fmt::format("{:f}", nan_with_payload);
    EXPECT_EQ(result, "nan");
}

// Test write_nonfinite with signaling NaN
TEST_F(WriteNonfiniteTest, SignalingNaN) {
    double signaling_nan = std::numeric_limits<double>::signaling_NaN();
    std::string result = fmt::format("{:f}", signaling_nan);
    EXPECT_EQ(result, "nan");
}

// Test write_nonfinite with negative zero (should not affect nonfinite)
TEST_F(WriteNonfiniteTest, NegativeZero) {
    double neg_zero = -0.0;
    std::string result = fmt::format("{:f}", neg_zero);
    EXPECT_EQ(result, "-0.000000");
}

// Test write_nonfinite with denormalized numbers (should not affect nonfinite)
TEST_F(WriteNonfiniteTest, DenormalizedNumbers) {
    double denorm = std::numeric_limits<double>::denorm_min();
    std::string result = fmt::format("{:f}", denorm);
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "nan");
}

// Test write_nonfinite with very large width
TEST_F(WriteNonfiniteTest, LargeWidth) {
    std::string result = fmt::format("{:100f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result.size(), 100);
    EXPECT_EQ(result.substr(97), "inf");
}

// Test write_nonfinite with zero width
TEST_F(WriteNonfiniteTest, ZeroWidth) {
    std::string result = fmt::format("{:0f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "inf");
}

// Test write_nonfinite with negative width (should be treated as left alignment)
TEST_F(WriteNonfiniteTest, NegativeWidth) {
    std::string result = fmt::format("{:-10f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "inf       ");
}

// Test write_nonfinite with multiple format specifiers
TEST_F(WriteNonfiniteTest, MultipleSpecifiers) {
    std::string result = fmt::format("{:*>+10.5f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "*****+inf");
}

// Test write_nonfinite with empty format string
TEST_F(WriteNonfiniteTest, EmptyFormat) {
    std::string result = fmt::format("{}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "inf");
}

// Test write_nonfinite with format string containing only type
TEST_F(WriteNonfiniteTest, TypeOnly) {
    std::string result = fmt::format("{:f}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "inf");
}

// Test write_nonfinite with format string containing only width
TEST_F(WriteNonfiniteTest, WidthOnly) {
    std::string result = fmt::format("{:10}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "       inf");
}

// Test write_nonfinite with format string containing only precision
TEST_F(WriteNonfiniteTest, PrecisionOnly) {
    std::string result = fmt::format("{:.5}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(result, "inf");
}