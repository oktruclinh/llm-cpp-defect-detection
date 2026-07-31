#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test formatting of seconds with fractional parts
TEST(ChronoFormatTest, FormatSecondsWithFractional) {
    using namespace std::chrono_literals;
    
    // Test whole seconds
    auto result = fmt::format("{:%S}", 42s);
    EXPECT_EQ(result, "42");
    
    // Test seconds with milliseconds
    result = fmt::format("{:%S}", std::chrono::duration<double>(42.123));
    EXPECT_EQ(result, "42.123");
    
    // Test seconds with microseconds
    result = fmt::format("{:%S}", std::chrono::duration<double>(42.123456));
    EXPECT_EQ(result, "42.123456");
    
    // Test seconds with nanoseconds
    result = fmt::format("{:%S}", std::chrono::duration<double>(42.123456789));
    EXPECT_EQ(result, "42.123456789");
}

// Test formatting of seconds with zero fractional part
TEST(ChronoFormatTest, FormatSecondsWithZeroFractional) {
    using namespace std::chrono_literals;
    
    // Test exactly zero fractional seconds
    auto result = fmt::format("{:%S}", std::chrono::duration<double>(0.0));
    EXPECT_EQ(result, "00");
    
    // Test whole seconds with no fractional part
    result = fmt::format("{:%S}", std::chrono::duration<double>(5.0));
    EXPECT_EQ(result, "05");
}

// Test formatting of seconds with very small fractional parts
TEST(ChronoFormatTest, FormatSecondsWithTinyFractional) {
    // Test very small fractional seconds
    auto result = fmt::format("{:%S}", std::chrono::duration<double>(0.000001));
    EXPECT_EQ(result, "00.000001");
    
    // Test extremely small value
    result = fmt::format("{:%S}", std::chrono::duration<double>(0.000000001));
    EXPECT_EQ(result, "00.000000001");
}

// Test formatting of seconds with large fractional parts
TEST(ChronoFormatTest, FormatSecondsWithLargeFractional) {
    // Test fractional seconds approaching 1
    auto result = fmt::format("{:%S}", std::chrono::duration<double>(59.999999999));
    EXPECT_EQ(result, "59.999999999");
    
    // Test exactly 59.999
    result = fmt::format("{:%S}", std::chrono::duration<double>(59.999));
    EXPECT_EQ(result, "59.999");
}

// Test formatting of seconds with different duration types
TEST(ChronoFormatTest, FormatSecondsWithDifferentDurationTypes) {
    // Test with milliseconds duration
    auto result = fmt::format("{:%S}", std::chrono::milliseconds(42123));
    EXPECT_EQ(result, "42.123");
    
    // Test with microseconds duration
    result = fmt::format("{:%S}", std::chrono::microseconds(42123456));
    EXPECT_EQ(result, "42.123456");
    
    // Test with nanoseconds duration
    result = fmt::format("{:%S}", std::chrono::nanoseconds(42123456789));
    EXPECT_EQ(result, "42.123456789");
}

// Test formatting of seconds with negative values
TEST(ChronoFormatTest, FormatSecondsWithNegativeValues) {
    // Test negative seconds with fractional part
    auto result = fmt::format("{:%S}", std::chrono::duration<double>(-42.123));
    EXPECT_EQ(result, "-42.123");
    
    // Test negative whole seconds
    result = fmt::format("{:%S}", std::chrono::duration<double>(-5.0));
    EXPECT_EQ(result, "-05");
}

// Test formatting of seconds with boundary values
TEST(ChronoFormatTest, FormatSecondsBoundaryValues) {
    // Test exactly 0 seconds
    auto result = fmt::format("{:%S}", std::chrono::seconds(0));
    EXPECT_EQ(result, "00");
    
    // Test exactly 59 seconds
    result = fmt::format("{:%S}", std::chrono::seconds(59));
    EXPECT_EQ(result, "59");
    
    // Test exactly 60 seconds (should wrap to 00 in minute context)
    result = fmt::format("{:%S}", std::chrono::seconds(60));
    EXPECT_EQ(result, "00");
}

// Test formatting of seconds with very large values
TEST(ChronoFormatTest, FormatSecondsWithLargeValues) {
    // Test large number of seconds
    auto result = fmt::format("{:%S}", std::chrono::seconds(123456));
    EXPECT_EQ(result, "36"); // 123456 % 60 = 36
    
    // Test large value with fractional part
    result = fmt::format("{:%S}", std::chrono::duration<double>(123456.789));
    EXPECT_EQ(result, "36.789");
}

// Test formatting of seconds with precision specifier
TEST(ChronoFormatTest, FormatSecondsWithPrecision) {
    // Test with precision specifier
    auto result = fmt::format("{:.3%S}", std::chrono::duration<double>(42.123456));
    EXPECT_EQ(result, "42.123");
    
    // Test with higher precision
    result = fmt::format("{:.6%S}", std::chrono::duration<double>(42.123456789));
    EXPECT_EQ(result, "42.123456");
}

// Test formatting of seconds in combined format strings
TEST(ChronoFormatTest, FormatSecondsInCombinedFormat) {
    using namespace std::chrono_literals;
    
    // Test in full timestamp format
    auto tp = std::chrono::system_clock::from_time_t(0) + 42s + std::chrono::milliseconds(123);
    auto result = fmt::format("{:%H:%M:%S}", tp);
    // The exact result depends on timezone, but should contain the seconds part
    EXPECT_NE(result.find("42.123"), std::string::npos);
}