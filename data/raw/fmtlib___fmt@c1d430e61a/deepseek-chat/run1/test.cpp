#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>
#include <sstream>

// Test fixture for chrono_formatter tests
class ChronoFormatterTest : public ::testing::Test {
protected:
    // Helper to format a duration using the public API that invokes chrono_formatter
    template <typename Rep, typename Period>
    std::string format_duration(std::chrono::duration<Rep, Period> d) {
        return fmt::format("{}", d);
    }
};

// Test zero duration
TEST_F(ChronoFormatterTest, ZeroDuration) {
    auto result = format_duration(std::chrono::seconds(0));
    EXPECT_EQ(result, "0s");
}

// Test positive seconds
TEST_F(ChronoFormatterTest, PositiveSeconds) {
    auto result = format_duration(std::chrono::seconds(42));
    EXPECT_EQ(result, "42s");
}

// Test negative seconds
TEST_F(ChronoFormatterTest, NegativeSeconds) {
    auto result = format_duration(std::chrono::seconds(-42));
    EXPECT_EQ(result, "-42s");
}

// Test positive milliseconds
TEST_F(ChronoFormatterTest, PositiveMilliseconds) {
    auto result = format_duration(std::chrono::milliseconds(1234));
    EXPECT_EQ(result, "1.234s");
}

// Test negative milliseconds
TEST_F(ChronoFormatterTest, NegativeMilliseconds) {
    auto result = format_duration(std::chrono::milliseconds(-1234));
    EXPECT_EQ(result, "-1.234s");
}

// Test sub-millisecond precision (microseconds)
TEST_F(ChronoFormatterTest, Microseconds) {
    auto result = format_duration(std::chrono::microseconds(123456));
    EXPECT_EQ(result, "0.123456s");
}

// Test negative microseconds
TEST_F(ChronoFormatterTest, NegativeMicroseconds) {
    auto result = format_duration(std::chrono::microseconds(-123456));
    EXPECT_EQ(result, "-0.123456s");
}

// Test nanoseconds
TEST_F(ChronoFormatterTest, Nanoseconds) {
    auto result = format_duration(std::chrono::nanoseconds(123456789));
    EXPECT_EQ(result, "0.123456789s");
}

// Test negative nanoseconds
TEST_F(ChronoFormatterTest, NegativeNanoseconds) {
    auto result = format_duration(std::chrono::nanoseconds(-123456789));
    EXPECT_EQ(result, "-0.123456789s");
}

// Test duration with mixed units (hours + minutes)
TEST_F(ChronoFormatterTest, HoursAndMinutes) {
    auto result = format_duration(std::chrono::hours(2) + std::chrono::minutes(30));
    EXPECT_EQ(result, "2h30m0s");
}

// Test negative mixed duration
TEST_F(ChronoFormatterTest, NegativeHoursAndMinutes) {
    auto result = format_duration(-(std::chrono::hours(1) + std::chrono::minutes(15)));
    EXPECT_EQ(result, "-1h15m0s");
}

// Test duration with fractional seconds in mixed format
TEST_F(ChronoFormatterTest, MixedWithFractionalSeconds) {
    auto result = format_duration(std::chrono::hours(1) + std::chrono::minutes(2) + 
                                  std::chrono::milliseconds(3456));
    EXPECT_EQ(result, "1h2m3.456s");
}

// Test negative mixed with fractional seconds
TEST_F(ChronoFormatterTest, NegativeMixedWithFractionalSeconds) {
    auto result = format_duration(-(std::chrono::hours(1) + std::chrono::minutes(2) + 
                                    std::chrono::milliseconds(3456)));
    EXPECT_EQ(result, "-1h2m3.456s");
}

// Test duration that is exactly one second
TEST_F(ChronoFormatterTest, ExactlyOneSecond) {
    auto result = format_duration(std::chrono::seconds(1));
    EXPECT_EQ(result, "1s");
}

// Test duration that is exactly negative one second
TEST_F(ChronoFormatterTest, ExactlyNegativeOneSecond) {
    auto result = format_duration(std::chrono::seconds(-1));
    EXPECT_EQ(result, "-1s");
}

// Test duration with only milliseconds (less than 1 second)
TEST_F(ChronoFormatterTest, OnlyMilliseconds) {
    auto result = format_duration(std::chrono::milliseconds(500));
    EXPECT_EQ(result, "0.500s");
}

// Test negative only milliseconds
TEST_F(ChronoFormatterTest, NegativeOnlyMilliseconds) {
    auto result = format_duration(std::chrono::milliseconds(-500));
    EXPECT_EQ(result, "-0.500s");
}

// Test duration with large values
TEST_F(ChronoFormatterTest, LargeDuration) {
    auto result = format_duration(std::chrono::hours(100));
    EXPECT_EQ(result, "100h0m0s");
}

// Test negative large duration
TEST_F(ChronoFormatterTest, NegativeLargeDuration) {
    auto result = format_duration(-std::chrono::hours(100));
    EXPECT_EQ(result, "-100h0m0s");
}

// Test duration with minutes and seconds only
TEST_F(ChronoFormatterTest, MinutesAndSeconds) {
    auto result = format_duration(std::chrono::minutes(5) + std::chrono::seconds(30));
    EXPECT_EQ(result, "5m30s");
}

// Test negative minutes and seconds
TEST_F(ChronoFormatterTest, NegativeMinutesAndSeconds) {
    auto result = format_duration(-(std::chrono::minutes(5) + std::chrono::seconds(30)));
    EXPECT_EQ(result, "-5m30s");
}

// Test duration with hours only
TEST_F(ChronoFormatterTest, HoursOnly) {
    auto result = format_duration(std::chrono::hours(3));
    EXPECT_EQ(result, "3h0m0s");
}

// Test negative hours only
TEST_F(ChronoFormatterTest, NegativeHoursOnly) {
    auto result = format_duration(std::chrono::hours(-3));
    EXPECT_EQ(result, "-3h0m0s");
}

// Test duration with minutes only
TEST_F(ChronoFormatterTest, MinutesOnly) {
    auto result = format_duration(std::chrono::minutes(45));
    EXPECT_EQ(result, "45m0s");
}

// Test negative minutes only
TEST_F(ChronoFormatterTest, NegativeMinutesOnly) {
    auto result = format_duration(std::chrono::minutes(-45));
    EXPECT_EQ(result, "-45m0s");
}

// Test that the output iterator is correctly advanced (multiple calls)
TEST_F(ChronoFormatterTest, MultipleFormats) {
    auto result = fmt::format("{} and {}", 
                              std::chrono::seconds(5), std::chrono::milliseconds(1234));
    EXPECT_EQ(result, "5s and 1.234s");
}

// Test with custom duration type (using std::chrono::duration directly)
TEST_F(ChronoFormatterTest, CustomDurationType) {
    using custom_dur = std::chrono::duration<int, std::ratio<1, 10000>>;
    auto result = format_duration(custom_dur(12345));
    EXPECT_EQ(result, "1.2345s");
}

// Test negative custom duration
TEST_F(ChronoFormatterTest, NegativeCustomDurationType) {
    using custom_dur = std::chrono::duration<int, std::ratio<1, 10000>>;
    auto result = format_duration(custom_dur(-12345));
    EXPECT_EQ(result, "-1.2345s");
}