#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>
#include <limits>

// Test fixture for chrono formatter tests
class ChronoFormatterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set default locale for consistent formatting
        std::locale::global(std::locale("C"));
    }
};

// Test chrono_formatter constructor and basic properties
TEST_F(ChronoFormatterTest, ChronoFormatterBasic) {
    auto dur = std::chrono::seconds(3661); // 1 hour, 1 minute, 1 second
    
    // Test through format_to which uses chrono_formatter internally
    std::string result = fmt::format("{:%H:%M:%S}", dur);
    EXPECT_EQ("01:01:01", result);
}

// Test chrono_formatter with negative durations
TEST_F(ChronoFormatterTest, ChronoFormatterNegative) {
    auto dur = std::chrono::seconds(-3661);
    std::string result = fmt::format("{:%H:%M:%S}", dur);
    EXPECT_EQ("-01:01:01", result);
}

// Test chrono_formatter hour() method
TEST_F(ChronoFormatterTest, ChronoFormatterHour) {
    EXPECT_EQ("00", fmt::format("{:%H}", std::chrono::seconds(0)));
    EXPECT_EQ("01", fmt::format("{:%H}", std::chrono::seconds(3600)));
    EXPECT_EQ("12", fmt::format("{:%H}", std::chrono::seconds(12 * 3600)));
    EXPECT_EQ("23", fmt::format("{:%H}", std::chrono::seconds(23 * 3600)));
    EXPECT_EQ("00", fmt::format("{:%H}", std::chrono::seconds(24 * 3600))); // Wrap around
    EXPECT_EQ("01", fmt::format("{:%H}", std::chrono::seconds(25 * 3600)));
}

// Test chrono_formatter hour12() method
TEST_F(ChronoFormatterTest, ChronoFormatterHour12) {
    EXPECT_EQ("12", fmt::format("{:%I}", std::chrono::seconds(0)));     // 12 AM
    EXPECT_EQ("01", fmt::format("{:%I}", std::chrono::seconds(3600)));  // 1 AM
    EXPECT_EQ("12", fmt::format("{:%I}", std::chrono::seconds(12 * 3600))); // 12 PM
    EXPECT_EQ("11", fmt::format("{:%I}", std::chrono::seconds(23 * 3600))); // 11 PM
    EXPECT_EQ("12", fmt::format("{:%I}", std::chrono::seconds(24 * 3600))); // Wrap to 12 AM
}

// Test chrono_formatter minute() method
TEST_F(ChronoFormatterTest, ChronoFormatterMinute) {
    EXPECT_EQ("00", fmt::format("{:%M}", std::chrono::seconds(0)));
    EXPECT_EQ("30", fmt::format("{:%M}", std::chrono::seconds(1800)));
    EXPECT_EQ("59", fmt::format("{:%M}", std::chrono::seconds(3599)));
    EXPECT_EQ("00", fmt::format("{:%M}", std::chrono::seconds(3600))); // Roll over to hour
    EXPECT_EQ("01", fmt::format("{:%M}", std::chrono::seconds(3660)));
}

// Test chrono_formatter second() method
TEST_F(ChronoFormatterTest, ChronoFormatterSecond) {
    EXPECT_EQ("00", fmt::format("{:%S}", std::chrono::seconds(0)));
    EXPECT_EQ("30", fmt::format("{:%S}", std::chrono::seconds(30)));
    EXPECT_EQ("59", fmt::format("{:%S}", std::chrono::seconds(59)));
    EXPECT_EQ("00", fmt::format("{:%S}", std::chrono::seconds(60))); // Roll over to minute
    EXPECT_EQ("01", fmt::format("{:%S}", std::chrono::seconds(61)));
}

// Test chrono_formatter with milliseconds
TEST_F(ChronoFormatterTest, ChronoFormatterMilliseconds) {
    auto dur = std::chrono::milliseconds(1500); // 1.5 seconds
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ("01", result); // Seconds part only
}

// Test chrono_formatter write() method (zero-padded output)
TEST_F(ChronoFormatterTest, ChronoFormatterWrite) {
    EXPECT_EQ("00", fmt::format("{:%H}", std::chrono::seconds(0)));
    EXPECT_EQ("05", fmt::format("{:%M}", std::chrono::seconds(300)));
    EXPECT_EQ("09", fmt::format("{:%S}", std::chrono::seconds(9)));
    EXPECT_EQ("10", fmt::format("{:%S}", std::chrono::seconds(10)));
}

// Test format_localized (basic functionality)
TEST_F(ChronoFormatterTest, FormatLocalized) {
    auto dur = std::chrono::seconds(3661);
    std::string result = fmt::format("{:L%H:%M:%S}", dur);
    EXPECT_EQ("01:01:01", result);
}

// Test edge cases: zero duration
TEST_F(ChronoFormatterTest, ZeroDuration) {
    EXPECT_EQ("00:00:00", fmt::format("{:%H:%M:%S}", std::chrono::seconds(0)));
    EXPECT_EQ("12:00:00", fmt::format("{:%I:%M:%S}", std::chrono::seconds(0)));
}

// Test edge cases: very large duration
TEST_F(ChronoFormatterTest, LargeDuration) {
    auto large_dur = std::chrono::hours(100) + std::chrono::minutes(30) + std::chrono::seconds(15);
    std::string result = fmt::format("{:%H:%M:%S}", large_dur);
    EXPECT_EQ("04:30:15", result); // 100 hours mod 24 = 4
}

// Test edge cases: duration with fractional seconds
TEST_F(ChronoFormatterTest, FractionalSeconds) {
    auto dur = std::chrono::duration<double>(1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ("01", result); // Only integer part
}

// Test with different rep types
TEST_F(ChronoFormatterTest, DifferentRepTypes) {
    // int rep
    EXPECT_EQ("00:00:05", fmt::format("{:%H:%M:%S}", std::chrono::seconds(5)));
    
    // long long rep
    EXPECT_EQ("00:00:10", fmt::format("{:%H:%M:%S}", std::chrono::duration<long long>(10)));
    
    // double rep
    EXPECT_EQ("00:00:03", fmt::format("{:%H:%M:%S}", std::chrono::duration<double>(3.14)));
}

// Test on_24_hour_time behavior (24-hour format)
TEST_F(ChronoFormatterTest, On24HourTime) {
    EXPECT_EQ("00", fmt::format("{:%H}", std::chrono::hours(0)));
    EXPECT_EQ("13", fmt::format("{:%H}", std::chrono::hours(13)));
    EXPECT_EQ("23", fmt::format("{:%H}", std::chrono::hours(23)));
}

// Test on_am_pm behavior (12-hour format)
TEST_F(ChronoFormatterTest, OnAmPm) {
    EXPECT_EQ("AM", fmt::format("{:%p}", std::chrono::hours(0)));
    EXPECT_EQ("AM", fmt::format("{:%p}", std::chrono::hours(11)));
    EXPECT_EQ("PM", fmt::format("{:%p}", std::chrono::hours(12)));
    EXPECT_EQ("PM", fmt::format("{:%p}", std::chrono::hours(23)));
}

// Test on_iso_time behavior
TEST_F(ChronoFormatterTest, OnIsoTime) {
    auto dur = std::chrono::seconds(3661);
    std::string result = fmt::format("{:%T}", dur); // %T is equivalent to %H:%M:%S
    EXPECT_EQ("01:01:01", result);
}

// Test on_duration_value behavior
TEST_F(ChronoFormatterTest, OnDurationValue) {
    auto dur = std::chrono::seconds(42);
    std::string result = fmt::format("{:%Q}", dur); // %Q outputs the numeric value
    EXPECT_EQ("42", result);
}

// Test on_duration_unit behavior
TEST_F(ChronoFormatterTest, OnDurationUnit) {
    auto dur = std::chrono::seconds(5);
    std::string result = fmt::format("{:%q}", dur); // %q outputs the unit
    EXPECT_EQ("s", result);
    
    // With milliseconds
    auto ms_dur = std::chrono::milliseconds(500);
    result = fmt::format("{:%q}", ms_dur);
    EXPECT_EQ("ms", result);
}

// Test combined duration value and unit
TEST_F(ChronoFormatterTest, DurationValueAndUnit) {
    auto dur = std::chrono::seconds(42);
    std::string result = fmt::format("{:%Q %q}", dur);
    EXPECT_EQ("42 s", result);
}

// Test with custom duration periods
TEST_F(ChronoFormatterTest, CustomPeriodDuration) {
    using custom_dur = std::chrono::duration<int, std::ratio<5>>; // 5-second ticks
    auto dur = custom_dur(10); // 50 seconds
    std::string result = fmt::format("{:%Q %q}", dur);
    EXPECT_EQ("10 [5]s", result);
}

// Test edge case: minimum negative duration
TEST_F(ChronoFormatterTest, MinimumNegativeDuration) {
    auto dur = std::chrono::seconds(std::numeric_limits<int>::min());
    std::string result = fmt::format("{:%H:%M:%S}", dur);
    // Should handle negative values correctly
    EXPECT_FALSE(result.empty());
    EXPECT_EQ('-', result[0]);
}

// Test edge case: maximum duration
TEST_F(ChronoFormatterTest, MaximumDuration) {
    auto dur = std::chrono::seconds(std::numeric_limits<int>::max());
    std::string result = fmt::format("{:%H:%M:%S}", dur);
    EXPECT_FALSE(result.empty());
}