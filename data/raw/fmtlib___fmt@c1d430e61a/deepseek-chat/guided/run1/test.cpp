#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test the chrono_formatter constructor behavior through fmt::format
// The constructor handles negative durations by outputting a '-' prefix
// and converting to positive values for further formatting.

TEST(ChronoFormatterTest, PositiveDuration) {
    auto result = fmt::format("{:%S}", std::chrono::seconds(45));
    EXPECT_EQ(result, "45");
}

TEST(ChronoFormatterTest, NegativeDurationSeconds) {
    auto result = fmt::format("{:%S}", std::chrono::seconds(-30));
    EXPECT_EQ(result, "-30");
}

TEST(ChronoFormatterTest, NegativeDurationWithMinutes) {
    auto result = fmt::format("{:%M:%S}", std::chrono::minutes(-2));
    EXPECT_EQ(result, "-02:00");
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
    auto result = fmt::format("{:%S}", std::chrono::milliseconds(-500));
    EXPECT_EQ(result, "-00");
}

TEST(ChronoFormatterTest, DurationWithFractionalSeconds) {
    auto result = fmt::format("{:%S}", std::chrono::duration<double>(1.5));
    EXPECT_EQ(result, "01");
}

TEST(ChronoFormatterTest, NegativeDurationWithFractionalSeconds) {
    auto result = fmt::format("{:%S}", std::chrono::duration<double>(-2.75));
    EXPECT_EQ(result, "-02");
}

TEST(ChronoFormatterTest, LargePositiveDuration) {
    auto result = fmt::format("{:%S}", std::chrono::hours(2));
    EXPECT_EQ(result, "00");
}

TEST(ChronoFormatterTest, LargeNegativeDuration) {
    auto result = fmt::format("{:%S}", std::chrono::hours(-3));
    EXPECT_EQ(result, "-00");
}

TEST(ChronoFormatterTest, DurationWithMinutesAndSeconds) {
    auto result = fmt::format("{:%M:%S}", std::chrono::seconds(125));
    EXPECT_EQ(result, "02:05");
}

TEST(ChronoFormatterTest, NegativeDurationWithMinutesAndSeconds) {
    auto result = fmt::format("{:%M:%S}", std::chrono::seconds(-125));
    EXPECT_EQ(result, "-02:05");
}

TEST(ChronoFormatterTest, DurationWithHours) {
    auto result = fmt::format("{:%H:%M:%S}", std::chrono::seconds(3661));
    EXPECT_EQ(result, "01:01:01");
}

TEST(ChronoFormatterTest, NegativeDurationWithHours) {
    auto result = fmt::format("{:%H:%M:%S}", std::chrono::seconds(-3661));
    EXPECT_EQ(result, "-01:01:01");
}

TEST(ChronoFormatterTest, DurationWithMillisecondsPrecision) {
    auto result = fmt::format("{:%S}", std::chrono::milliseconds(1500));
    EXPECT_EQ(result, "01");
}

TEST(ChronoFormatterTest, NegativeDurationWithMillisecondsPrecision) {
    auto result = fmt::format("{:%S}", std::chrono::milliseconds(-2500));
    EXPECT_EQ(result, "-02");
}

TEST(ChronoFormatterTest, DurationWithMicroseconds) {
    auto result = fmt::format("{:%S}", std::chrono::microseconds(1000000));
    EXPECT_EQ(result, "01");
}

TEST(ChronoFormatterTest, NegativeDurationWithMicroseconds) {
    auto result = fmt::format("{:%S}", std::chrono::microseconds(-2000000));
    EXPECT_EQ(result, "-02");
}

TEST(ChronoFormatterTest, DurationWithNanoseconds) {
    auto result = fmt::format("{:%S}", std::chrono::nanoseconds(1000000000));
    EXPECT_EQ(result, "01");
}

TEST(ChronoFormatterTest, NegativeDurationWithNanoseconds) {
    auto result = fmt::format("{:%S}", std::chrono::nanoseconds(-3000000000));
    EXPECT_EQ(result, "-03");
}

TEST(ChronoFormatterTest, DurationWithMixedUnits) {
    auto result = fmt::format("{:%M:%S}", std::chrono::duration<int, std::ratio<60>>(5));
    EXPECT_EQ(result, "05:00");
}

TEST(ChronoFormatterTest, NegativeDurationWithMixedUnits) {
    auto result = fmt::format("{:%M:%S}", std::chrono::duration<int, std::ratio<60>>(-3));
    EXPECT_EQ(result, "-03:00");
}

TEST(ChronoFormatterTest, DurationWithCustomRep) {
    auto result = fmt::format("{:%S}", std::chrono::duration<long long>(100));
    EXPECT_EQ(result, "01");
}

TEST(ChronoFormatterTest, NegativeDurationWithCustomRep) {
    auto result = fmt::format("{:%S}", std::chrono::duration<long long>(-200));
    EXPECT_EQ(result, "-02");
}

TEST(ChronoFormatterTest, DurationWithFloatRep) {
    auto result = fmt::format("{:%S}", std::chrono::duration<float>(3.14f));
    EXPECT_EQ(result, "03");
}

TEST(ChronoFormatterTest, NegativeDurationWithFloatRep) {
    auto result = fmt::format("{:%S}", std::chrono::duration<float>(-1.5f));
    EXPECT_EQ(result, "-01");
}

TEST(ChronoFormatterTest, DurationWithDoubleRep) {
    auto result = fmt::format("{:%S}", std::chrono::duration<double>(0.999));
    EXPECT_EQ(result, "00");
}

TEST(ChronoFormatterTest, NegativeDurationWithDoubleRep) {
    auto result = fmt::format("{:%S}", std::chrono::duration<double>(-0.001));
    EXPECT_EQ(result, "-00");
}

TEST(ChronoFormatterTest, DurationWithLongDoubleRep) {
    auto result = fmt::format("{:%S}", std::chrono::duration<long double>(2.5L));
    EXPECT_EQ(result, "02");
}

TEST(ChronoFormatterTest, NegativeDurationWithLongDoubleRep) {
    auto result = fmt::format("{:%S}", std::chrono::duration<long double>(-4.2L));
    EXPECT_EQ(result, "-04");
}