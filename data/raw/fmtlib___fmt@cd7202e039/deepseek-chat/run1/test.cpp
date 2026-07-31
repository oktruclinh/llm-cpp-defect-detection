#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test formatting of seconds with fractional parts for various durations
TEST(ChronoWriteFractionalSecondsTest, SecondsWithFraction) {
    // Test 1.5 seconds
    auto dur = std::chrono::duration<double>(1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");

    // Test 0.001 seconds (1 millisecond)
    dur = std::chrono::duration<double>(0.001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.001");

    // Test 59.999 seconds (near max)
    dur = std::chrono::duration<double>(59.999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999");
}

// Test formatting of seconds with no fractional part (integer seconds)
TEST(ChronoWriteFractionalSecondsTest, IntegerSeconds) {
    auto dur = std::chrono::seconds(5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "05");

    dur = std::chrono::seconds(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00");

    dur = std::chrono::seconds(59);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59");
}

// Test formatting with milliseconds precision
TEST(ChronoWriteFractionalSecondsTest, Milliseconds) {
    auto dur = std::chrono::milliseconds(1500);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");

    dur = std::chrono::milliseconds(1);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.001");

    dur = std::chrono::milliseconds(59999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999");
}

// Test formatting with microseconds precision
TEST(ChronoWriteFractionalSecondsTest, Microseconds) {
    auto dur = std::chrono::microseconds(1500000);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500000");

    dur = std::chrono::microseconds(1);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000001");

    dur = std::chrono::microseconds(59999999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999999");
}

// Test formatting with nanoseconds precision
TEST(ChronoWriteFractionalSecondsTest, Nanoseconds) {
    auto dur = std::chrono::nanoseconds(1500000000);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500000000");

    dur = std::chrono::nanoseconds(1);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000000001");

    dur = std::chrono::nanoseconds(59999999999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999999999");
}

// Test edge case: zero duration
TEST(ChronoWriteFractionalSecondsTest, ZeroDuration) {
    auto dur = std::chrono::duration<double>(0.0);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000");

    dur = std::chrono::seconds(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00");

    dur = std::chrono::milliseconds(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000");
}

// Test edge case: very small fractional seconds
TEST(ChronoWriteFractionalSecondsTest, VerySmallFraction) {
    auto dur = std::chrono::duration<double>(0.000001);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000001");

    dur = std::chrono::duration<double>(0.000000001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000000001");
}

// Test edge case: seconds near boundaries
TEST(ChronoWriteFractionalSecondsTest, BoundarySeconds) {
    // Just above 0
    auto dur = std::chrono::duration<double>(0.0005);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.001");  // rounds to 3 decimal places

    // Just below 60
    dur = std::chrono::duration<double>(59.9995);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "60.000");  // rounds up to 60 (but format may show 60)
}

// Test formatting with custom precision specifier
TEST(ChronoWriteFractionalSecondsTest, CustomPrecision) {
    auto dur = std::chrono::duration<double>(1.23456789);
    std::string result = fmt::format("{:.3%S}", dur);
    EXPECT_EQ(result, "01.235");  // rounded to 3 decimal places

    result = fmt::format("{:.6%S}", dur);
    EXPECT_EQ(result, "01.234568");  // rounded to 6 decimal places

    result = fmt::format("{:.0%S}", dur);
    EXPECT_EQ(result, "01");  // no fractional part
}

// Test that the function handles negative durations (if applicable)
TEST(ChronoWriteFractionalSecondsTest, NegativeDuration) {
    auto dur = std::chrono::duration<double>(-1.5);
    std::string result = fmt::format("{:%S}", dur);
    // Negative durations may show negative seconds or wrap; check behavior
    // Typically fmtlib shows negative sign before seconds
    EXPECT_EQ(result, "-01.500");
}

// Test that the function works with duration_cast truncation
TEST(ChronoWriteFractionalSecondsTest, DurationCastTruncation) {
    auto dur = std::chrono::duration<double>(1.9999);
    // Using integer seconds should truncate
    std::string result = fmt::format("{:%S}", std::chrono::duration_cast<std::chrono::seconds>(dur));
    EXPECT_EQ(result, "01");  // truncated, not rounded

    dur = std::chrono::duration<double>(1.0001);
    result = fmt::format("{:%S}", std::chrono::duration_cast<std::chrono::seconds>(dur));
    EXPECT_EQ(result, "01");
}