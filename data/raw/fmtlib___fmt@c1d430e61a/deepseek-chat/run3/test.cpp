#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test fixture for chrono_formatter behavior via fmt::format
class ChronoFormatterTest : public ::testing::Test {
protected:
    // Helper to format a duration using the public API
    template <typename Rep, typename Period>
    std::string format_duration(std::chrono::duration<Rep, Period> d) {
        return fmt::format("{:%H:%M:%S}", d);
    }
};

// Test zero duration
TEST_F(ChronoFormatterTest, ZeroDuration) {
    auto result = format_duration(std::chrono::seconds(0));
    EXPECT_EQ(result, "00:00:00");
}

// Test positive seconds
TEST_F(ChronoFormatterTest, PositiveSeconds) {
    auto result = format_duration(std::chrono::seconds(3661));
    EXPECT_EQ(result, "01:01:01");
}

// Test negative duration (should produce negative sign)
TEST_F(ChronoFormatterTest, NegativeDuration) {
    auto result = format_duration(std::chrono::seconds(-3661));
    EXPECT_EQ(result, "-01:01:01");
}

// Test with milliseconds
TEST_F(ChronoFormatterTest, WithMilliseconds) {
    auto result = format_duration(std::chrono::milliseconds(1500));
    EXPECT_EQ(result, "00:00:01.500");
}

// Test negative milliseconds
TEST_F(ChronoFormatterTest, NegativeMilliseconds) {
    auto result = format_duration(std::chrono::milliseconds(-1500));
    EXPECT_EQ(result, "-00:00:01.500");
}

// Test with microseconds (should truncate to milliseconds)
TEST_F(ChronoFormatterTest, MicrosecondsTruncation) {
    auto result = format_duration(std::chrono::microseconds(1234567));
    EXPECT_EQ(result, "00:00:01.234");
}

// Test with nanoseconds (should truncate to milliseconds)
TEST_F(ChronoFormatterTest, NanosecondsTruncation) {
    auto result = format_duration(std::chrono::nanoseconds(1234567890));
    EXPECT_EQ(result, "00:00:01.234");
}

// Test boundary: exactly one second
TEST_F(ChronoFormatterTest, ExactlyOneSecond) {
    auto result = format_duration(std::chrono::seconds(1));
    EXPECT_EQ(result, "00:00:01");
}

// Test boundary: exactly one minute
TEST_F(ChronoFormatterTest, ExactlyOneMinute) {
    auto result = format_duration(std::chrono::seconds(60));
    EXPECT_EQ(result, "00:01:00");
}

// Test boundary: exactly one hour
TEST_F(ChronoFormatterTest, ExactlyOneHour) {
    auto result = format_duration(std::chrono::seconds(3600));
    EXPECT_EQ(result, "01:00:00");
}

// Test large positive duration
TEST_F(ChronoFormatterTest, LargePositiveDuration) {
    auto result = format_duration(std::chrono::hours(100));
    EXPECT_EQ(result, "100:00:00");
}

// Test large negative duration
TEST_F(ChronoFormatterTest, LargeNegativeDuration) {
    auto result = format_duration(std::chrono::hours(-100));
    EXPECT_EQ(result, "-100:00:00");
}

// Test with fractional seconds (milliseconds only)
TEST_F(ChronoFormatterTest, FractionalSeconds) {
    auto result = format_duration(std::chrono::milliseconds(123));
    EXPECT_EQ(result, "00:00:00.123");
}

// Test negative fractional seconds
TEST_F(ChronoFormatterTest, NegativeFractionalSeconds) {
    auto result = format_duration(std::chrono::milliseconds(-123));
    EXPECT_EQ(result, "-00:00:00.123");
}

// Test that the sign is correctly applied to the whole duration, not just seconds
TEST_F(ChronoFormatterTest, SignAppliedToWholeDuration) {
    // -1.5 seconds should be "-00:00:01.500", not "00:00:-01.500"
    auto result = format_duration(std::chrono::milliseconds(-1500));
    EXPECT_EQ(result, "-00:00:01.500");
}

// Test with duration that has both seconds and milliseconds
TEST_F(ChronoFormatterTest, SecondsAndMilliseconds) {
    auto result = format_duration(std::chrono::milliseconds(123456));
    EXPECT_EQ(result, "00:02:03.456");
}

// Test negative duration with seconds and milliseconds
TEST_F(ChronoFormatterTest, NegativeSecondsAndMilliseconds) {
    auto result = format_duration(std::chrono::milliseconds(-123456));
    EXPECT_EQ(result, "-00:02:03.456");
}

// Test with duration that is exactly at a boundary where milliseconds wrap
TEST_F(ChronoFormatterTest, MillisecondWrap) {
    auto result = format_duration(std::chrono::milliseconds(1000));
    EXPECT_EQ(result, "00:00:01.000");
}

// Test with duration that is exactly at a negative boundary where milliseconds wrap
TEST_F(ChronoFormatterTest, NegativeMillisecondWrap) {
    auto result = format_duration(std::chrono::milliseconds(-1000));
    EXPECT_EQ(result, "-00:00:01.000");
}