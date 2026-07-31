#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test fixture for chrono_formatter hour() function
class ChronoFormatterHourTest : public ::testing::Test {
protected:
    // Helper to format a duration and extract the hour component
    // We use the public API: fmt::format with chrono duration
    template <typename Rep, typename Period>
    std::string format_hours(std::chrono::duration<Rep, Period> d) {
        return fmt::format("{:%H}", d);
    }
};

// Normal cases
TEST_F(ChronoFormatterHourTest, ZeroHours) {
    EXPECT_EQ(format_hours(std::chrono::hours(0)), "00");
}

TEST_F(ChronoFormatterHourTest, OneHour) {
    EXPECT_EQ(format_hours(std::chrono::hours(1)), "01");
}

TEST_F(ChronoFormatterHourTest, TwelveHours) {
    EXPECT_EQ(format_hours(std::chrono::hours(12)), "12");
}

TEST_F(ChronoFormatterHourTest, TwentyThreeHours) {
    EXPECT_EQ(format_hours(std::chrono::hours(23)), "23");
}

TEST_F(ChronoFormatterHourTest, TwentyFourHours) {
    // 24 hours should wrap to 00 (mod 24)
    EXPECT_EQ(format_hours(std::chrono::hours(24)), "00");
}

TEST_F(ChronoFormatterHourTest, TwentyFiveHours) {
    EXPECT_EQ(format_hours(std::chrono::hours(25)), "01");
}

TEST_F(ChronoFormatterHourTest, FortySevenHours) {
    EXPECT_EQ(format_hours(std::chrono::hours(47)), "23");
}

TEST_F(ChronoFormatterHourTest, FortyEightHours) {
    EXPECT_EQ(format_hours(std::chrono::hours(48)), "00");
}

// Edge cases with large values
TEST_F(ChronoFormatterHourTest, LargeHoursMod24) {
    // 1000 hours = 41 days + 16 hours -> 16
    EXPECT_EQ(format_hours(std::chrono::hours(1000)), "16");
}

TEST_F(ChronoFormatterHourTest, VeryLargeHours) {
    // 100000 hours mod 24 = 16 (100000 / 24 = 4166 remainder 16)
    EXPECT_EQ(format_hours(std::chrono::hours(100000)), "16");
}

// Negative durations (C++20 allows negative durations)
TEST_F(ChronoFormatterHourTest, NegativeOneHour) {
    EXPECT_EQ(format_hours(std::chrono::hours(-1)), "23");
}

TEST_F(ChronoFormatterHourTest, NegativeTwentyFourHours) {
    EXPECT_EQ(format_hours(std::chrono::hours(-24)), "00");
}

TEST_F(ChronoFormatterHourTest, NegativeTwentyFiveHours) {
    EXPECT_EQ(format_hours(std::chrono::hours(-25)), "23");
}

// Test with non-hour durations (minutes, seconds, etc.)
TEST_F(ChronoFormatterHourTest, MinutesDuration) {
    // 90 minutes = 1 hour 30 minutes -> hour = 01
    EXPECT_EQ(format_hours(std::chrono::minutes(90)), "01");
}

TEST_F(ChronoFormatterHourTest, SecondsDuration) {
    // 3600 seconds = 1 hour -> hour = 01
    EXPECT_EQ(format_hours(std::chrono::seconds(3600)), "01");
}

TEST_F(ChronoFormatterHourTest, MillisecondsDuration) {
    // 3600000 ms = 1 hour -> hour = 01
    EXPECT_EQ(format_hours(std::chrono::milliseconds(3600000)), "01");
}

TEST_F(ChronoFormatterHourTest, MixedUnitsDuration) {
    // 1 hour + 30 minutes = 90 minutes -> hour = 01
    auto dur = std::chrono::hours(1) + std::chrono::minutes(30);
    EXPECT_EQ(format_hours(dur), "01");
}

// Test with different Rep types
TEST_F(ChronoFormatterHourTest, FloatDuration) {
    // 1.5 hours should truncate to 1 hour
    EXPECT_EQ(format_hours(std::chrono::duration<double>(1.5)), "01");
}

TEST_F(ChronoFormatterHourTest, FloatDurationWrap) {
    // 25.7 hours -> 1 hour (mod 24, truncate)
    EXPECT_EQ(format_hours(std::chrono::duration<double>(25.7)), "01");
}

// Test that hour() returns correct value for durations with milliseconds
TEST_F(ChronoFormatterHourTest, DurationWithMs) {
    // 1 hour + 500 ms -> hour = 01
    auto dur = std::chrono::hours(1) + std::chrono::milliseconds(500);
    EXPECT_EQ(format_hours(dur), "01");
}

// Test boundary: exactly 24 hours from negative side
TEST_F(ChronoFormatterHourTest, NegativeTwentyThreeHours) {
    EXPECT_EQ(format_hours(std::chrono::hours(-23)), "01");
}

// Test with zero duration from different unit types
TEST_F(ChronoFormatterHourTest, ZeroMinutes) {
    EXPECT_EQ(format_hours(std::chrono::minutes(0)), "00");
}

TEST_F(ChronoFormatterHourTest, ZeroSeconds) {
    EXPECT_EQ(format_hours(std::chrono::seconds(0)), "00");
}

// Test that hour() handles durations that are not whole hours
TEST_F(ChronoFormatterHourTest, PartialHourTruncation) {
    // 1 hour 59 minutes -> hour = 01 (truncated, not rounded)
    EXPECT_EQ(format_hours(std::chrono::minutes(119)), "01");
}

TEST_F(ChronoFormatterHourTest, PartialHourTruncation2) {
    // 2 hours 1 minute -> hour = 02
    EXPECT_EQ(format_hours(std::chrono::minutes(121)), "02");
}