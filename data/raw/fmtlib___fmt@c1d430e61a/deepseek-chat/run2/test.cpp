#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test fixture for chrono_formatter tests
class ChronoFormatterTest : public ::testing::Test {
protected:
    // Helper to format a duration using the public API that exercises chrono_formatter
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
    auto result = format_duration(std::chrono::seconds(3661)); // 1h 1m 1s
    EXPECT_EQ(result, "01:01:01");
}

// Test negative duration (should produce negative sign)
TEST_F(ChronoFormatterTest, NegativeDuration) {
    auto result = format_duration(std::chrono::seconds(-3661));
    EXPECT_EQ(result, "-01:01:01");
}

// Test with milliseconds
TEST_F(ChronoFormatterTest, WithMilliseconds) {
    auto result = format_duration(std::chrono::milliseconds(1500)); // 1.5s
    EXPECT_EQ(result, "00:00:01.500");
}

// Test negative milliseconds
TEST_F(ChronoFormatterTest, NegativeMilliseconds) {
    auto result = format_duration(std::chrono::milliseconds(-1500));
    EXPECT_EQ(result, "-00:00:01.500");
}

// Test with microseconds (should be truncated to milliseconds)
TEST_F(ChronoFormatterTest, MicrosecondsTruncation) {
    auto result = format_duration(std::chrono::microseconds(1234567)); // 1.234567s
    EXPECT_EQ(result, "00:00:01.234");
}

// Test with nanoseconds (should be truncated to milliseconds)
TEST_F(ChronoFormatterTest, NanosecondsTruncation) {
    auto result = format_duration(std::chrono::nanoseconds(1234567890)); // 1.234567890s
    EXPECT_EQ(result, "00:00:01.234");
}

// Test large positive duration
TEST_F(ChronoFormatterTest, LargePositiveDuration) {
    auto result = format_duration(std::chrono::hours(25) + std::chrono::minutes(30));
    EXPECT_EQ(result, "25:30:00");
}

// Test large negative duration
TEST_F(ChronoFormatterTest, LargeNegativeDuration) {
    auto result = format_duration(-(std::chrono::hours(25) + std::chrono::minutes(30)));
    EXPECT_EQ(result, "-25:30:00");
}

// Test duration with fractional seconds that have exact millisecond representation
TEST_F(ChronoFormatterTest, ExactMilliseconds) {
    auto result = format_duration(std::chrono::milliseconds(500));
    EXPECT_EQ(result, "00:00:00.500");
}

// Test duration with no milliseconds component
TEST_F(ChronoFormatterTest, NoMilliseconds) {
    auto result = format_duration(std::chrono::seconds(5));
    EXPECT_EQ(result, "00:00:05");
}

// Test negative duration with exact seconds (no fractional part)
TEST_F(ChronoFormatterTest, NegativeExactSeconds) {
    auto result = format_duration(std::chrono::seconds(-5));
    EXPECT_EQ(result, "-00:00:05");
}

// Test duration that is exactly one hour
TEST_F(ChronoFormatterTest, ExactlyOneHour) {
    auto result = format_duration(std::chrono::hours(1));
    EXPECT_EQ(result, "01:00:00");
}

// Test duration that is exactly one minute
TEST_F(ChronoFormatterTest, ExactlyOneMinute) {
    auto result = format_duration(std::chrono::minutes(1));
    EXPECT_EQ(result, "00:01:00");
}

// Test duration with all components non-zero
TEST_F(ChronoFormatterTest, AllComponentsNonZero) {
    auto result = format_duration(std::chrono::hours(12) + 
                                   std::chrono::minutes(34) + 
                                   std::chrono::seconds(56) + 
                                   std::chrono::milliseconds(789));
    EXPECT_EQ(result, "12:34:56.789");
}

// Test negative duration with all components non-zero
TEST_F(ChronoFormatterTest, NegativeAllComponentsNonZero) {
    auto result = format_duration(-(std::chrono::hours(12) + 
                                    std::chrono::minutes(34) + 
                                    std::chrono::seconds(56) + 
                                    std::chrono::milliseconds(789)));
    EXPECT_EQ(result, "-12:34:56.789");
}

// Test duration with only milliseconds (less than a second)
TEST_F(ChronoFormatterTest, OnlyMilliseconds) {
    auto result = format_duration(std::chrono::milliseconds(999));
    EXPECT_EQ(result, "00:00:00.999");
}

// Test negative duration with only milliseconds
TEST_F(ChronoFormatterTest, NegativeOnlyMilliseconds) {
    auto result = format_duration(std::chrono::milliseconds(-999));
    EXPECT_EQ(result, "-00:00:00.999");
}

// Test duration with maximum representable milliseconds (edge case for truncation)
TEST_F(ChronoFormatterTest, MaxMillisecondsTruncation) {
    auto result = format_duration(std::chrono::milliseconds(999));
    EXPECT_EQ(result, "00:00:00.999");
}

// Test that negative durations produce exactly one leading minus sign
TEST_F(ChronoFormatterTest, SingleMinusSign) {
    auto result = format_duration(std::chrono::seconds(-1));
    EXPECT_EQ(result, "-00:00:01");
    // Verify no double minus
    EXPECT_EQ(result.front(), '-');
    EXPECT_EQ(std::count(result.begin(), result.end(), '-'), 1);
}