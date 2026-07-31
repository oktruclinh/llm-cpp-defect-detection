#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test formatting of seconds with fractional part for various durations
TEST(ChronoWriteFractionalSecondsTest, SecondsWithFractional) {
    using namespace std::chrono_literals;
    
    // Test 1.5 seconds
    auto dur = 1500ms;
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");
    
    // Test 0.5 seconds
    dur = 500ms;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.500");
    
    // Test 1.25 seconds
    dur = 1250ms;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.250");
}

// Test formatting of seconds with no fractional part (integer seconds)
TEST(ChronoWriteFractionalSecondsTest, IntegerSeconds) {
    using namespace std::chrono_literals;
    
    // Exactly 1 second
    auto dur = 1s;
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01");
    
    // Exactly 0 seconds
    dur = 0s;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00");
    
    // Exactly 59 seconds
    dur = 59s;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59");
}

// Test formatting with sub-second precision using microseconds
TEST(ChronoWriteFractionalSecondsTest, MicrosecondPrecision) {
    using namespace std::chrono_literals;
    
    // 1.000001 seconds
    auto dur = 1s + 1us;
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.000001");
    
    // 0.000001 seconds
    dur = 1us;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000001");
}

// Test formatting with nanoseconds
TEST(ChronoWriteFractionalSecondsTest, NanosecondPrecision) {
    using namespace std::chrono_literals;
    
    // 1.000000001 seconds
    auto dur = 1s + 1ns;
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.000000001");
    
    // 0.000000001 seconds
    dur = 1ns;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000000001");
}

// Test edge case: very small fractional seconds
TEST(ChronoWriteFractionalSecondsTest, VerySmallFraction) {
    using namespace std::chrono_literals;
    
    // 0.0001 seconds (100 microseconds)
    auto dur = 100us;
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000100");
    
    // 0.00001 seconds (10 microseconds)
    dur = 10us;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000010");
}

// Test edge case: duration with Rep type that is not integer (floating point)
TEST(ChronoWriteFractionalSecondsTest, FloatingPointDuration) {
    using Fsec = std::chrono::duration<double>;
    
    // 1.5 seconds using double
    Fsec dur(1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");
    
    // 0.333 seconds using double
    dur = Fsec(0.333);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.333");
}

// Test edge case: duration with custom period (e.g., tenths of seconds)
TEST(ChronoWriteFractionalSecondsTest, CustomPeriodDuration) {
    using Tenths = std::chrono::duration<int, std::ratio<1, 10>>;
    
    // 1.5 seconds = 15 tenths
    Tenths dur(15);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.5");
    
    // 0.3 seconds = 3 tenths
    dur = Tenths(3);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.3");
}

// Test edge case: duration with period that doesn't evenly divide into seconds
TEST(ChronoWriteFractionalSecondsTest, NonStandardPeriod) {
    using Thirds = std::chrono::duration<int, std::ratio<1, 3>>;
    
    // 1 second = 3 thirds
    Thirds dur(3);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.0");
    
    // 2/3 second = 2 thirds
    dur = Thirds(2);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.666666667");
}

// Test that negative durations are handled (if supported)
TEST(ChronoWriteFractionalSecondsTest, NegativeDuration) {
    using namespace std::chrono_literals;
    
    // Negative duration
    auto dur = -1500ms;
    std::string result = fmt::format("{:%S}", dur);
    // Note: fmt typically handles negative durations by formatting the absolute value
    // and prepending a minus sign in the overall format, but for %S alone it may
    // just format the absolute seconds part
    EXPECT_TRUE(result == "01.500" || result == "-01.500");
}

// Test that zero duration with fractional part works
TEST(ChronoWriteFractionalSecondsTest, ZeroDurationWithFraction) {
    using namespace std::chrono_literals;
    
    // Zero duration
    auto dur = 0ms;
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000");
    
    // Zero duration with higher precision
    auto dur_ns = 0ns;
    result = fmt::format("{:%S}", dur_ns);
    EXPECT_EQ(result, "00.000000000");
}

// Test that the function correctly handles durations that would overflow if not careful
TEST(ChronoWriteFractionalSecondsTest, LargeDuration) {
    using namespace std::chrono_literals;
    
    // Large number of seconds (but still within reasonable range)
    auto dur = 3600s + 500ms;  // 1 hour + 0.5 seconds
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.500");  // %S only shows seconds part (0-59)
    
    // 59.999 seconds
    dur = 59s + 999ms;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999");
}