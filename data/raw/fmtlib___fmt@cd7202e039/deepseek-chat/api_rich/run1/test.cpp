#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test formatting of seconds with fractional parts
TEST(ChronoFormatTest, FormatSecondsWithFractional) {
    using namespace std::chrono_literals;
    
    // Test whole seconds
    auto dur1 = std::chrono::seconds(42);
    EXPECT_EQ(fmt::format("{:%S}", dur1), "42");
    
    // Test seconds with milliseconds
    auto dur2 = std::chrono::milliseconds(12345); // 12.345 seconds
    EXPECT_EQ(fmt::format("{:%S}", dur2), "12.345");
    
    // Test seconds with microseconds
    auto dur3 = std::chrono::microseconds(12345678); // 12.345678 seconds
    EXPECT_EQ(fmt::format("{:%S}", dur3), "12.345678");
    
    // Test seconds with nanoseconds
    auto dur4 = std::chrono::nanoseconds(1234567890); // 1.234567890 seconds
    EXPECT_EQ(fmt::format("{:%S}", dur4), "01.234567890");
}

// Test formatting of minutes and seconds combined
TEST(ChronoFormatTest, FormatMinutesAndSeconds) {
    using namespace std::chrono_literals;
    
    // Test minutes with fractional seconds
    auto dur1 = std::chrono::milliseconds(90500); // 1 minute 30.5 seconds
    EXPECT_EQ(fmt::format("{:%M:%S}", dur1), "01:30.500");
    
    // Test zero seconds
    auto dur2 = std::chrono::seconds(120); // 2 minutes exactly
    EXPECT_EQ(fmt::format("{:%M:%S}", dur2), "02:00");
}

// Test formatting of hours, minutes and seconds
TEST(ChronoFormatTest, FormatHoursMinutesSeconds) {
    using namespace std::chrono_literals;
    
    // Test hours with fractional seconds
    auto dur1 = std::chrono::milliseconds(3661500); // 1 hour 1 minute 1.5 seconds
    EXPECT_EQ(fmt::format("{:%H:%M:%S}", dur1), "01:01:01.500");
    
    // Test boundary: exactly 1 hour
    auto dur2 = std::chrono::hours(1);
    EXPECT_EQ(fmt::format("{:%H:%M:%S}", dur2), "01:00:00");
}

// Test edge cases with zero duration
TEST(ChronoFormatTest, FormatZeroDuration) {
    using namespace std::chrono_literals;
    
    auto zero_sec = std::chrono::seconds(0);
    EXPECT_EQ(fmt::format("{:%S}", zero_sec), "00");
    
    auto zero_ms = std::chrono::milliseconds(0);
    EXPECT_EQ(fmt::format("{:%S}", zero_ms), "00.000");
    
    auto zero_ns = std::chrono::nanoseconds(0);
    EXPECT_EQ(fmt::format("{:%S}", zero_ns), "00.000000000");
}

// Test formatting with different precision specifications
TEST(ChronoFormatTest, FormatWithPrecision) {
    using namespace std::chrono_literals;
    
    auto dur = std::chrono::milliseconds(12345); // 12.345 seconds
    
    // Default precision (3 decimal places for milliseconds)
    EXPECT_EQ(fmt::format("{:%S}", dur), "12.345");
    
    // Explicit precision
    EXPECT_EQ(fmt::format("{:.3%S}", dur), "12.345");
    EXPECT_EQ(fmt::format("{:.1%S}", dur), "12.3");
    EXPECT_EQ(fmt::format("{:.0%S}", dur), "12");
}

// Test formatting with very small fractional seconds
TEST(ChronoFormatTest, FormatSmallFractionalSeconds) {
    using namespace std::chrono_literals;
    
    // 1 microsecond
    auto dur1 = std::chrono::microseconds(1);
    EXPECT_EQ(fmt::format("{:%S}", dur1), "00.000001");
    
    // 1 nanosecond
    auto dur2 = std::chrono::nanoseconds(1);
    EXPECT_EQ(fmt::format("{:%S}", dur2), "00.000000001");
}

// Test formatting with large durations
TEST(ChronoFormatTest, FormatLargeDuration) {
    using namespace std::chrono_literals;
    
    // Large number of seconds
    auto dur = std::chrono::seconds(86399); // 23:59:59
    EXPECT_EQ(fmt::format("{:%H:%M:%S}", dur), "23:59:59");
    
    // Duration that exceeds 24 hours
    auto dur2 = std::chrono::hours(25);
    EXPECT_EQ(fmt::format("{:%H:%M:%S}", dur2), "01:00:00"); // Wraps around
}

// Test formatting with negative durations (if supported)
TEST(ChronoFormatTest, FormatNegativeDuration) {
    using namespace std::chrono_literals;
    
    auto neg_dur = std::chrono::seconds(-5);
    // Negative durations should still format correctly
    EXPECT_EQ(fmt::format("{:%S}", neg_dur), "-05");
}

// Test formatting with custom duration types
TEST(ChronoFormatTest, FormatCustomDuration) {
    using custom_duration = std::chrono::duration<double, std::ratio<1, 1000>>;
    
    custom_duration dur(1234.567);
    EXPECT_EQ(fmt::format("{:%S}", dur), "01.234567");
}

// Test that fractional seconds are properly truncated/padded
TEST(ChronoFormatTest, FractionalSecondsTruncation) {
    using namespace std::chrono_literals;
    
    // Duration with many decimal places
    auto dur = std::chrono::nanoseconds(123456789);
    EXPECT_EQ(fmt::format("{:%S}", dur), "00.123456789");
    
    // Duration that needs leading zeros in fractional part
    auto dur2 = std::chrono::nanoseconds(1000);
    EXPECT_EQ(fmt::format("{:%S}", dur2), "00.000001000");
}