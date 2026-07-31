#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test the chrono_formatter constructor behavior through fmt::format
// The constructor handles negative durations by outputting a '-' prefix
// and converting to positive values for formatting.

TEST(ChronoFormatterTest, PositiveDuration) {
    auto result = fmt::format("{:%S}", std::chrono::seconds(42));
    EXPECT_EQ(result, "42");
}

TEST(ChronoFormatterTest, NegativeDuration) {
    auto result = fmt::format("{:%S}", std::chrono::seconds(-42));
    EXPECT_EQ(result, "-42");
}

TEST(ChronoFormatterTest, ZeroDuration) {
    auto result = fmt::format("{:%S}", std::chrono::seconds(0));
    EXPECT_EQ(result, "00");
}

TEST(ChronoFormatterTest, NegativeZeroDuration) {
    auto result = fmt::format("{:%S}", std::chrono::seconds(-0));
    EXPECT_EQ(result, "00");
}

TEST(ChronoFormatterTest, PositiveMilliseconds) {
    auto result = fmt::format("{:%S}", std::chrono::milliseconds(1234));
    EXPECT_EQ(result, "01");
}

TEST(ChronoFormatterTest, NegativeMilliseconds) {
    auto result = fmt::format("{:%S}", std::chrono::milliseconds(-1234));
    EXPECT_EQ(result, "-01");
}

TEST(ChronoFormatterTest, PositiveFractionalSeconds) {
    auto result = fmt::format("{:%S}", std::chrono::duration<double>(1.5));
    EXPECT_EQ(result, "01");
}

TEST(ChronoFormatterTest, NegativeFractionalSeconds) {
    auto result = fmt::format("{:%S}", std::chrono::duration<double>(-1.5));
    EXPECT_EQ(result, "-01");
}

TEST(ChronoFormatterTest, LargePositiveDuration) {
    auto result = fmt::format("{:%S}", std::chrono::hours(2) + std::chrono::minutes(30) + std::chrono::seconds(15));
    EXPECT_EQ(result, "15");
}

TEST(ChronoFormatterTest, LargeNegativeDuration) {
    auto result = fmt::format("{:%S}", -(std::chrono::hours(2) + std::chrono::minutes(30) + std::chrono::seconds(15)));
    EXPECT_EQ(result, "-15");
}

TEST(ChronoFormatterTest, DurationWithMilliseconds) {
    auto result = fmt::format("{:%S}", std::chrono::milliseconds(1500));
    EXPECT_EQ(result, "01");
}

TEST(ChronoFormatterTest, NegativeDurationWithMilliseconds) {
    auto result = fmt::format("{:%S}", std::chrono::milliseconds(-1500));
    EXPECT_EQ(result, "-01");
}

TEST(ChronoFormatterTest, DurationWithMicroseconds) {
    auto result = fmt::format("{:%S}", std::chrono::microseconds(1000000));
    EXPECT_EQ(result, "01");
}

TEST(ChronoFormatterTest, NegativeDurationWithMicroseconds) {
    auto result = fmt::format("{:%S}", std::chrono::microseconds(-1000000));
    EXPECT_EQ(result, "-01");
}

TEST(ChronoFormatterTest, DurationWithNanoseconds) {
    auto result = fmt::format("{:%S}", std::chrono::nanoseconds(1000000000));
    EXPECT_EQ(result, "01");
}

TEST(ChronoFormatterTest, NegativeDurationWithNanoseconds) {
    auto result = fmt::format("{:%S}", std::chrono::nanoseconds(-1000000000));
    EXPECT_EQ(result, "-01");
}

TEST(ChronoFormatterTest, DurationWithMinutes) {
    auto result = fmt::format("{:%M}", std::chrono::minutes(5));
    EXPECT_EQ(result, "05");
}

TEST(ChronoFormatterTest, NegativeDurationWithMinutes) {
    auto result = fmt::format("{:%M}", std::chrono::minutes(-5));
    EXPECT_EQ(result, "-05");
}

TEST(ChronoFormatterTest, DurationWithHours) {
    auto result = fmt::format("{:%H}", std::chrono::hours(3));
    EXPECT_EQ(result, "03");
}

TEST(ChronoFormatterTest, NegativeDurationWithHours) {
    auto result = fmt::format("{:%H}", std::chrono::hours(-3));
    EXPECT_EQ(result, "-03");
}

TEST(ChronoFormatterTest, FullTimestampPositive) {
    auto result = fmt::format("{:%H:%M:%S}", std::chrono::hours(1) + std::chrono::minutes(2) + std::chrono::seconds(3));
    EXPECT_EQ(result, "01:02:03");
}

TEST(ChronoFormatterTest, FullTimestampNegative) {
    auto result = fmt::format("{:%H:%M:%S}", -(std::chrono::hours(1) + std::chrono::minutes(2) + std::chrono::seconds(3)));
    EXPECT_EQ(result, "-01:02:03");
}

TEST(ChronoFormatterTest, DurationWithSubseconds) {
    auto result = fmt::format("{:%S}", std::chrono::milliseconds(1234));
    EXPECT_EQ(result, "01");
}

TEST(ChronoFormatterTest, NegativeDurationWithSubseconds) {
    auto result = fmt::format("{:%S}", std::chrono::milliseconds(-1234));
    EXPECT_EQ(result, "-01");
}

TEST(ChronoFormatterTest, DurationWithPrecision) {
    auto result = fmt::format("{:.3%S}", std::chrono::duration<double>(1.234));
    EXPECT_EQ(result, "01.234");
}

TEST(ChronoFormatterTest, NegativeDurationWithPrecision) {
    auto result = fmt::format("{:.3%S}", std::chrono::duration<double>(-1.234));
    EXPECT_EQ(result, "-01.234");
}

TEST(ChronoFormatterTest, DurationWithZeroPrecision) {
    auto result = fmt::format("{:.0%S}", std::chrono::duration<double>(1.5));
    EXPECT_EQ(result, "02");
}

TEST(ChronoFormatterTest, NegativeDurationWithZeroPrecision) {
    auto result = fmt::format("{:.0%S}", std::chrono::duration<double>(-1.5));
    EXPECT_EQ(result, "-02");
}

TEST(ChronoFormatterTest, DurationWithHighPrecision) {
    auto result = fmt::format("{:.6%S}", std::chrono::duration<double>(1.123456));
    EXPECT_EQ(result, "01.123456");
}

TEST(ChronoFormatterTest, NegativeDurationWithHighPrecision) {
    auto result = fmt::format("{:.6%S}", std::chrono::duration<double>(-1.123456));
    EXPECT_EQ(result, "-01.123456");
}