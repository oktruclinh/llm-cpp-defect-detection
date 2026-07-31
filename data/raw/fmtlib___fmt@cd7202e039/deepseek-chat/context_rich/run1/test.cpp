```cpp
#include <gtest/gtest.h>
#include "format.h"
#include <chrono>
#include <cmath>
#include <limits>
#include <string>
#include <sstream>

// Test fixture for tm_writer tests
class TmWriterTest : public ::testing::Test {
protected:
    // Helper to create a time_point from a specific date/time
    static std::chrono::system_clock::time_point make_time_point(int year, int month, int day,
                                                                  int hour, int minute, int second) {
        std::tm tm = {};
        tm.tm_year = year - 1900;
        tm.tm_mon = month - 1;
        tm.tm_mday = day;
        tm.tm_hour = hour;
        tm.tm_min = minute;
        tm.tm_sec = second;
        tm.tm_isdst = -1;
        std::time_t t = std::mktime(&tm);
        return std::chrono::system_clock::from_time_t(t);
    }

    // Helper to format a time_point using fmt
    static std::string format_time(const std::chrono::system_clock::time_point& tp,
                                    const std::string& format_str) {
        return fmt::format(format_str, tp);
    }
};

// Test on_24_hour with standard numeric system
TEST_F(TmWriterTest, On24HourStandard) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%H}");
    EXPECT_EQ(result, "14");
}

// Test on_24_hour with locale-specific numeric system (midnight)
TEST_F(TmWriterTest, On24HourMidnight) {
    auto tp = make_time_point(2023, 6, 15, 0, 0, 0);
    std::string result = format_time(tp, "{:%H}");
    EXPECT_EQ(result, "00");
}

// Test on_24_hour with single digit hour
TEST_F(TmWriterTest, On24HourSingleDigit) {
    auto tp = make_time_point(2023, 6, 15, 3, 0, 0);
    std::string result = format_time(tp, "{:%H}");
    EXPECT_EQ(result, "03");
}

// Test on_12_hour with standard numeric system
TEST_F(TmWriterTest, On12HourStandard) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%I}");
    EXPECT_EQ(result, "02");
}

// Test on_12_hour with midnight (12 AM)
TEST_F(TmWriterTest, On12HourMidnight) {
    auto tp = make_time_point(2023, 6, 15, 0, 0, 0);
    std::string result = format_time(tp, "{:%I}");
    EXPECT_EQ(result, "12");
}

// Test on_12_hour with noon (12 PM)
TEST_F(TmWriterTest, On12HourNoon) {
    auto tp = make_time_point(2023, 6, 15, 12, 0, 0);
    std::string result = format_time(tp, "{:%I}");
    EXPECT_EQ(result, "12");
}

// Test on_minute with standard numeric system
TEST_F(TmWriterTest, OnMinuteStandard) {
    auto tp = make_time_point(2023, 6, 15, 14, 5, 45);
    std::string result = format_time(tp, "{:%M}");
    EXPECT_EQ(result, "05");
}

// Test on_minute with zero padding
TEST_F(TmWriterTest, OnMinuteZero) {
    auto tp = make_time_point(2023, 6, 15, 14, 0, 45);
    std::string result = format_time(tp, "{:%M}");
    EXPECT_EQ(result, "00");
}

// Test on_second with integer seconds
TEST_F(TmWriterTest, OnSecondInteger) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 7);
    std::string result = format_time(tp, "{:%S}");
    EXPECT_EQ(result, "07");
}

// Test on_second with floating point seconds
TEST_F(TmWriterTest, OnSecondFloatingPoint) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    // Use duration with fractional seconds
    auto dur = std::chrono::duration<double>(45.123);
    auto tp_frac = tp + dur - std::chrono::seconds(45);
    std::string result = format_time(tp_frac, "{:%S}");
    // Should contain the fractional part
    EXPECT_NE(result.find('.'), std::string::npos);
}

// Test on_12_hour_time
TEST_F(TmWriterTest, On12HourTime) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%r}");
    EXPECT_EQ(result, "02:30:45 PM");
}

// Test on_12_hour_time at midnight
TEST_F(TmWriterTest, On12HourTimeMidnight) {
    auto tp = make_time_point(2023, 6, 15, 0, 0, 0);
    std::string result = format_time(tp, "{:%r}");
    EXPECT_EQ(result, "12:00:00 AM");
}

// Test on_24_hour_time
TEST_F(TmWriterTest, On24HourTime) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%R}");
    EXPECT_EQ(result, "14:30");
}

// Test on_24_hour_time at midnight
TEST_F(TmWriterTest, On24HourTimeMidnight) {
    auto tp = make_time_point(2023, 6, 15, 0, 0, 0);
    std::string result = format_time(tp, "{:%R}");
    EXPECT_EQ(result, "00:00");
}

// Test on_iso_time
TEST_F(TmWriterTest, OnIsoTime) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%T}");
    EXPECT_EQ(result, "14:30:45");
}

// Test on_iso_time with single digit components
TEST_F(TmWriterTest, OnIsoTimeSingleDigits) {
    auto tp = make_time_point(2023, 6, 15, 3, 5, 7);
    std::string result = format_time(tp, "{:%T}");
    EXPECT_EQ(result, "03:05:07");
}

// Test on_am_pm for AM
TEST_F(TmWriterTest, OnAmPmAM) {
    auto tp = make_time_point(2023, 6, 15, 8, 0, 0);
    std::string result = format_time(tp, "{:%p}");
    EXPECT_EQ(result, "AM");
}

// Test on_am_pm for PM
TEST_F(TmWriterTest, OnAmPmPM) {
    auto tp = make_time_point(2023, 6, 15, 20, 0, 0);
    std::string result = format_time(tp, "{:%p}");
    EXPECT_EQ(result, "PM");
}

// Test on_am_pm at noon
TEST_F(TmWriterTest, OnAmPmNoon) {
    auto tp = make_time_point(2023, 6, 15, 12, 0, 0);
    std::string result = format_time(tp, "{:%p}");
    EXPECT_EQ(result, "PM");
}

// Test on_am_pm at midnight
TEST_F(TmWriterTest, OnAmPmMidnight) {
    auto tp = make_time_point(2023, 6, 15, 0, 0, 0);
    std::string result = format_time(tp, "{:%p}");
    EXPECT_EQ(result, "AM");
}

// Test on_year
TEST_F(TmWriterTest, OnYear) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%Y}");
    EXPECT_EQ(result, "2023");
}

// Test on_short_year
TEST_F(TmWriterTest, OnShortYear) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%y}");
    EXPECT_EQ(result, "23");
}

// Test on_short_year for year 2000
TEST_F(TmWriterTest, OnShortYear2000) {
    auto tp = make_time_point(2000, 1, 1, 0, 0, 0);
    std::string result = format_time(tp, "{:%y}");
    EXPECT_EQ(result, "00");
}

// Test on_dec_month
TEST_F(TmWriterTest, OnDecMonth) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%m}");
    EXPECT_EQ(result, "06");
}

// Test on_dec_month for December
TEST_F(TmWriterTest, OnDecMonthDecember) {
    auto tp = make_time_point(2023, 12, 25, 0, 0, 0);
    std::string result = format_time(tp, "{:%m}");
    EXPECT_EQ(result, "12");
}

// Test on_day_of_month
TEST_F(TmWriterTest, OnDayOfMonth) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%d}");
    EXPECT_EQ(result, "15");
}

// Test on_day_of_month with single digit
TEST_F(TmWriterTest, OnDayOfMonthSingleDigit) {
    auto tp = make_time_point(2023, 6, 5, 14, 30, 45);
    std::string result = format_time(tp, "{:%d}");
    EXPECT_EQ(result, "05");
}

// Test on_day_of_month_space
TEST_F(TmWriterTest, OnDayOfMonthSpace) {
    auto tp = make_time_point(2023, 6, 5, 14, 30, 45);
    std::string result = format_time(tp, "{:%e}");
    EXPECT_EQ(result, " 5");
}

// Test on_day_of_month_space for two digit day
TEST_F(TmWriterTest, OnDayOfMonthSpaceTwoDigit) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%e}");
    EXPECT_EQ(result, "15");
}

// Test on_abbr_month
TEST_F(TmWriterTest, OnAbbrMonth) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%b}");
    EXPECT_EQ(result, "Jun");
}

// Test on_full_month
TEST_F(TmWriterTest, OnFullMonth) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%B}");
    EXPECT_EQ(result, "June");
}

// Test on_day_of_year
TEST_F(TmWriterTest, OnDayOfYear) {
    auto tp = make_time_point(2023, 1, 1, 0, 0, 0);
    std::string result = format_time(tp, "{:%j}");
    EXPECT_EQ(result, "001");
}

// Test on_day_of_year for December 31
TEST_F(TmWriterTest, OnDayOfYearDec31) {
    auto tp = make_time_point(2023, 12, 31, 0, 0, 0);
    std::string result = format_time(tp, "{:%j}");
    EXPECT_EQ(result, "365");
}

// Test on_weekday
TEST_F(TmWriterTest, OnWeekday) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45); // Thursday
    std::string result = format_time(tp, "{:%a}");
    EXPECT_EQ(result, "Thu");
}

// Test on_full_weekday
TEST_F(TmWriterTest, OnFullWeekday) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45); // Thursday
    std::string result = format_time(tp, "{:%A}");
    EXPECT_EQ(result, "Thursday");
}

// Test on_iso_week_of_year
TEST_F(TmWriterTest, OnIsoWeekOfYear) {
    auto tp = make_time_point(2023, 1, 2, 0, 0, 0); // Monday of week 1
    std::string result = format_time(tp, "{:%V}");
    EXPECT_EQ(result, "01");
}

// Test on_iso_week_based_year
TEST_F(TmWriterTest, OnIsoWeekBasedYear) {
    auto tp = make_time_point(2023, 1, 2, 0, 0, 0);
    std::string result = format_time(tp, "{:%G}");
    EXPECT_EQ(result, "2023");
}

// Test on_utc_offset
TEST_F(TmWriterTest, OnUtcOffset) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%z}");
    // Should contain + or - followed by 4 digits
    EXPECT_TRUE(result.size() == 5);
    EXPECT_TRUE(result[0] == '+' || result[0] == '-');
}

// Test on_tz_name
TEST_F(TmWriterTest, OnTzName) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%Z}");
    // Should not be empty
    EXPECT_FALSE(result.empty());
}

// Test on_datetime
TEST_F(TmWriterTest, OnDatetime) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%c}");
    // Should contain date and time information
    EXPECT_TRUE(result.find("2023") != std::string::npos);
    EXPECT_TRUE(result.find("Jun") != std::string::npos || 
                result.find("June") != std::string::npos);
}

// Test on_loc_date
TEST_F(TmWriterTest, OnLocDate) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%x}");
    // Should contain date information
    EXPECT_TRUE(result.find("2023") != std::string::npos);
}

// Test on_loc_time
TEST_F(TmWriterTest, OnLocTime) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%X}");
    // Should contain time information
    EXPECT_TRUE(result.find("14") != std::string::npos || 
                result.find("02") != std::string::npos);
}

// Test on_us_date
TEST_F(TmWriterTest, OnUsDate) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%D}");
    EXPECT_EQ(result, "06/15/23");
}

// Test on_iso_date
TEST_F(TmWriterTest, OnIsoDate) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%F}");
    EXPECT_EQ(result, "2023-06-15");
}

// Test on_century
TEST_F(TmWriterTest, OnCentury) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%C}");
    EXPECT_EQ(result, "20");
}

// Test on_century for year 2000
TEST_F(TmWriterTest, OnCentury2000) {
    auto tp = make_time_point(2000, 1, 1, 0, 0, 0);
    std::string result = format_time(tp, "{:%C}");
    EXPECT_EQ(result, "20");
}

// Test on_offset_year
TEST_F(TmWriterTest, OnOffsetYear) {
    auto tp = make_time_point(2023, 6, 15, 14, 30, 45);
    std::string result = format_time(tp, "{:%EY}");
    // Should contain year information
    EXPECT_TRUE(result.find("2023") != std::string::npos);
}

// Test on_iso_week_based_short_year
TEST_F(TmWriterTest, OnIsoWeekBasedShortYear) {
    auto tp = make_time_point(2023, 1, 2, 0, 0, 0);
    std::string result = format_time(tp, "{:%g}");
    EXPECT_EQ(result, "23");
}

// Test on_dec0_week_of_year
TEST_F(TmWriterTest, OnDec0WeekOfYear) {
    auto tp = make_time_point(2023, 1, 1, 0, 0, 0); // Sunday
    std::string result = format_time(tp, "{:%U}");
    EXPECT_EQ(result, "00