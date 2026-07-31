#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test the chrono_formatter's hour() function through the public API
// by formatting durations and checking the hour component.

// Normal cases: various hour values within 0-23
TEST(ChronoFormatterHourTest, NormalHours) {
    // 1 hour
    auto d1 = std::chrono::hours(1);
    std::string result1 = fmt::format("{:%H}", d1);
    EXPECT_EQ(result1, "01");

    // 12 hours
    auto d12 = std::chrono::hours(12);
    std::string result12 = fmt::format("{:%H}", d12);
    EXPECT_EQ(result12, "12");

    // 23 hours
    auto d23 = std::chrono::hours(23);
    std::string result23 = fmt::format("{:%H}", d23);
    EXPECT_EQ(result23, "23");
}

// Edge case: zero hours
TEST(ChronoFormatterHourTest, ZeroHours) {
    auto d0 = std::chrono::hours(0);
    std::string result0 = fmt::format("{:%H}", d0);
    EXPECT_EQ(result0, "00");
}

// Edge case: exactly 24 hours (should wrap to 0)
TEST(ChronoFormatterHourTest, TwentyFourHoursWrapsToZero) {
    auto d24 = std::chrono::hours(24);
    std::string result24 = fmt::format("{:%H}", d24);
    EXPECT_EQ(result24, "00");
}

// Edge case: 25 hours (should wrap to 1)
TEST(ChronoFormatterHourTest, TwentyFiveHoursWrapsToOne) {
    auto d25 = std::chrono::hours(25);
    std::string result25 = fmt::format("{:%H}", d25);
    EXPECT_EQ(result25, "01");
}

// Edge case: large number of hours (multiple wraps)
TEST(ChronoFormatterHourTest, LargeHoursWrapCorrectly) {
    // 100 hours = 4 days + 4 hours -> hour = 4
    auto d100 = std::chrono::hours(100);
    std::string result100 = fmt::format("{:%H}", d100);
    EXPECT_EQ(result100, "04");

    // 1000 hours = 41 days + 16 hours -> hour = 16
    auto d1000 = std::chrono::hours(1000);
    std::string result1000 = fmt::format("{:%H}", d1000);
    EXPECT_EQ(result1000, "16");
}

// Edge case: negative hours (should wrap correctly)
TEST(ChronoFormatterHourTest, NegativeHoursWrapCorrectly) {
    // -1 hour = 23 hours (since -1 mod 24 = 23)
    auto d_neg1 = std::chrono::hours(-1);
    std::string result_neg1 = fmt::format("{:%H}", d_neg1);
    EXPECT_EQ(result_neg1, "23");

    // -25 hours = -1 day - 1 hour -> hour = 23
    auto d_neg25 = std::chrono::hours(-25);
    std::string result_neg25 = fmt::format("{:%H}", d_neg25);
    EXPECT_EQ(result_neg25, "23");
}

// Edge case: duration with minutes and seconds (hour should still be correct)
TEST(ChronoFormatterHourTest, DurationWithMinutesAndSeconds) {
    // 1 hour, 30 minutes, 15 seconds
    auto d = std::chrono::hours(1) + std::chrono::minutes(30) + std::chrono::seconds(15);
    std::string result = fmt::format("{:%H}", d);
    EXPECT_EQ(result, "01");

    // 25 hours, 45 minutes -> hour should be 1 (25 mod 24)
    auto d2 = std::chrono::hours(25) + std::chrono::minutes(45);
    std::string result2 = fmt::format("{:%H}", d2);
    EXPECT_EQ(result2, "01");
}

// Edge case: duration with milliseconds (hour should still be correct)
TEST(ChronoFormatterHourTest, DurationWithMilliseconds) {
    // 12 hours, 500 milliseconds
    auto d = std::chrono::hours(12) + std::chrono::milliseconds(500);
    std::string result = fmt::format("{:%H}", d);
    EXPECT_EQ(result, "12");
}

// Edge case: very large duration (many days)
TEST(ChronoFormatterHourTest, VeryLargeDuration) {
    // 1000000 hours = 41666 days + 16 hours -> hour = 16
    auto d = std::chrono::hours(1000000);
    std::string result = fmt::format("{:%H}", d);
    EXPECT_EQ(result, "16");
}

// Edge case: duration with fractional hours (should truncate toward zero)
TEST(ChronoFormatterHourTest, FractionalHours) {
    // 1.5 hours = 1 hour + 30 minutes -> hour = 1
    auto d = std::chrono::minutes(90);
    std::string result = fmt::format("{:%H}", d);
    EXPECT_EQ(result, "01");

    // 0.5 hours = 30 minutes -> hour = 0
    auto d2 = std::chrono::minutes(30);
    std::string result2 = fmt::format("{:%H}", d2);
    EXPECT_EQ(result2, "00");
}

// Edge case: negative fractional hours
TEST(ChronoFormatterHourTest, NegativeFractionalHours) {
    // -1.5 hours = -1 hour - 30 minutes -> hour = 23 (since -1 mod 24 = 23)
    auto d = std::chrono::minutes(-90);
    std::string result = fmt::format("{:%H}", d);
    EXPECT_EQ(result, "23");
}