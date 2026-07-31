#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test formatting of seconds with fractional parts for various durations
TEST(ChronoWriteFractionalSecondsTest, SecondsWithFractional) {
    using namespace std::chrono_literals;

    // Test 1.5 seconds
    auto dur = std::chrono::duration<double>(1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");

    // Test 0.25 seconds
    dur = std::chrono::duration<double>(0.25);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.250");

    // Test 59.999 seconds
    dur = std::chrono::duration<double>(59.999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999");
}

// Test formatting of seconds with no fractional part (integer seconds)
TEST(ChronoWriteFractionalSecondsTest, IntegerSeconds) {
    using namespace std::chrono_literals;

    // Test exactly 0 seconds
    auto dur = std::chrono::seconds(0);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00");

    // Test exactly 30 seconds
    dur = std::chrono::seconds(30);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "30");

    // Test exactly 59 seconds
    dur = std::chrono::seconds(59);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59");
}

// Test formatting with milliseconds precision
TEST(ChronoWriteFractionalSecondsTest, Milliseconds) {
    using namespace std::chrono_literals;

    // Test 1 second and 500 milliseconds
    auto dur = std::chrono::milliseconds(1500);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");

    // Test 0 milliseconds
    dur = std::chrono::milliseconds(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000");

    // Test 999 milliseconds
    dur = std::chrono::milliseconds(999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.999");
}

// Test formatting with microseconds precision
TEST(ChronoWriteFractionalSecondsTest, Microseconds) {
    using namespace std::chrono_literals;

    // Test 1 second and 500 microseconds
    auto dur = std::chrono::microseconds(1000500);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.000500");

    // Test 0 microseconds
    dur = std::chrono::microseconds(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000000");

    // Test 999999 microseconds
    dur = std::chrono::microseconds(999999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.999999");
}

// Test formatting with nanoseconds precision
TEST(ChronoWriteFractionalSecondsTest, Nanoseconds) {
    using namespace std::chrono_literals;

    // Test 1 second and 500 nanoseconds
    auto dur = std::chrono::nanoseconds(1000000500);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.000000500");

    // Test 0 nanoseconds
    dur = std::chrono::nanoseconds(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000000000");

    // Test 999999999 nanoseconds
    dur = std::chrono::nanoseconds(999999999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.999999999");
}

// Test formatting with floating-point duration types
TEST(ChronoWriteFractionalSecondsTest, FloatingPointDuration) {
    // Test with float
    auto dur = std::chrono::duration<float>(2.75f);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "02.750");

    // Test with double
    dur = std::chrono::duration<double>(0.001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.001");

    // Test with long double
    dur = std::chrono::duration<long double>(1.0L);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.000");
}

// Test edge case: very small fractional seconds
TEST(ChronoWriteFractionalSecondsTest, VerySmallFraction) {
    using namespace std::chrono_literals;

    // Test 1 nanosecond
    auto dur = std::chrono::nanoseconds(1);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000000001");

    // Test 1 microsecond
    dur = std::chrono::microseconds(1);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000001");
}

// Test edge case: duration with negative values (should not happen in normal time but test behavior)
TEST(ChronoWriteFractionalSecondsTest, NegativeDuration) {
    using namespace std::chrono_literals;

    // Test -1.5 seconds
    auto dur = std::chrono::duration<double>(-1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-01.500");

    // Test -0.25 seconds
    dur = std::chrono::duration<double>(-0.25);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-00.250");
}

// Test formatting with custom duration types
TEST(ChronoWriteFractionalSecondsTest, CustomDuration) {
    // Use a duration with a different tick period (e.g., 1/3 second)
    using third_seconds = std::chrono::duration<int, std::ratio<1, 3>>;
    auto dur = third_seconds(5); // 5/3 = 1.666... seconds
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.666");

    // Test with 1/7 second ticks
    using seventh_seconds = std::chrono::duration<int, std::ratio<1, 7>>;
    auto dur2 = seventh_seconds(10); // 10/7 ≈ 1.428571 seconds
    result = fmt::format("{:%S}", dur2);
    EXPECT_EQ(result, "01.428571");
}

// Test that the function handles the case where seconds are part of a larger time format
TEST(ChronoWriteFractionalSecondsTest, InLargerFormat) {
    using namespace std::chrono_literals;

    // Test with hours and minutes
    auto dur = std::chrono::hours(1) + std::chrono::minutes(30) + std::chrono::seconds(15) + std::chrono::milliseconds(500);
    std::string result = fmt::format("{:%H:%M:%S}", dur);
    EXPECT_EQ(result, "01:30:15.500");

    // Test with just minutes and seconds
    auto dur2 = std::chrono::minutes(5) + std::chrono::seconds(45) + std::chrono::microseconds(123456);
    result = fmt::format("{:%M:%S}", dur2);
    EXPECT_EQ(result, "05:45.123456");
}