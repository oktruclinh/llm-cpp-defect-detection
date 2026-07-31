#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test formatting of seconds with fractional seconds for various durations
TEST(ChronoWriteFractionalSecondsTest, SecondsWithFractional) {
    // Test 1.5 seconds
    auto dur = std::chrono::duration<double>(1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");

    // Test 0.001 seconds (1 millisecond)
    dur = std::chrono::duration<double>(0.001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.001");

    // Test 59.999 seconds (near max seconds)
    dur = std::chrono::duration<double>(59.999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999");
}

// Test formatting of seconds with zero fractional part
TEST(ChronoWriteFractionalSecondsTest, SecondsWithZeroFractional) {
    auto dur = std::chrono::duration<int>(30);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "30.000");

    dur = std::chrono::duration<int>(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000");
}

// Test formatting of seconds with very small fractional values
TEST(ChronoWriteFractionalSecondsTest, VerySmallFractional) {
    auto dur = std::chrono::duration<double>(0.000001);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000001");

    dur = std::chrono::duration<double>(0.000000001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000000001");
}

// Test formatting of seconds with very large fractional values (near 1 second)
TEST(ChronoWriteFractionalSecondsTest, LargeFractional) {
    auto dur = std::chrono::duration<double>(0.999999999);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.999999999");

    dur = std::chrono::duration<double>(59.999999999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999999999");
}

// Test formatting of seconds with negative values (if supported)
TEST(ChronoWriteFractionalSecondsTest, NegativeSeconds) {
    auto dur = std::chrono::duration<double>(-1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-01.500");

    dur = std::chrono::duration<double>(-0.001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-00.001");
}

// Test formatting with different duration types (milliseconds, microseconds, etc.)
TEST(ChronoWriteFractionalSecondsTest, DifferentDurationTypes) {
    // Milliseconds
    auto ms = std::chrono::milliseconds(1500);
    std::string result = fmt::format("{:%S}", ms);
    EXPECT_EQ(result, "01.500");

    // Microseconds
    auto us = std::chrono::microseconds(1234567);
    result = fmt::format("{:%S}", us);
    EXPECT_EQ(result, "01.234567");

    // Nanoseconds
    auto ns = std::chrono::nanoseconds(1234567890);
    result = fmt::format("{:%S}", ns);
    EXPECT_EQ(result, "01.234567890");
}

// Test formatting with custom Rep types (e.g., float)
TEST(ChronoWriteFractionalSecondsTest, FloatRep) {
    auto dur = std::chrono::duration<float>(2.5f);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "02.500");

    dur = std::chrono::duration<float>(0.1f);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.100");
}

// Test that the function correctly handles the boundary between seconds and minutes
TEST(ChronoWriteFractionalSecondsTest, BoundaryAt60Seconds) {
    auto dur = std::chrono::duration<double>(60.0);
    std::string result = fmt::format("{:%S}", dur);
    // At exactly 60 seconds, the seconds part should be 00 (since it rolls over to minutes)
    EXPECT_EQ(result, "00.000");

    dur = std::chrono::duration<double>(59.999999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999999");
}

// Test with zero duration
TEST(ChronoWriteFractionalSecondsTest, ZeroDuration) {
    auto dur = std::chrono::duration<double>(0.0);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000");

    dur = std::chrono::duration<int>(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000");
}

// Test with very large duration values
TEST(ChronoWriteFractionalSecondsTest, LargeDuration) {
    auto dur = std::chrono::duration<double>(123456.789);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "36.789");  // 123456 seconds = 34 hours, 17 minutes, 36 seconds
}