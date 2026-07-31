#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test formatting of seconds with fractional parts using default format
TEST(ChronoFractionalSecondsTest, DefaultFormatSeconds) {
    auto dur = std::chrono::duration<double>(1.5);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "01.500" (seconds with 3 fractional digits by default)
    EXPECT_EQ(result, "01.500");
}

// Test formatting of seconds with fractional parts using custom precision
TEST(ChronoFractionalSecondsTest, CustomPrecisionSeconds) {
    auto dur = std::chrono::duration<double>(2.25);
    std::string result = fmt::format("{:%S.%6}", dur);
    // Expected: "02.250000" (seconds with 6 fractional digits)
    EXPECT_EQ(result, "02.250000");
}

// Test formatting of seconds with zero fractional part
TEST(ChronoFractionalSecondsTest, ZeroFractionalSeconds) {
    auto dur = std::chrono::duration<double>(3.0);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "03.000" (seconds with 3 fractional digits, all zero)
    EXPECT_EQ(result, "03.000");
}

// Test formatting of seconds with very small fractional part
TEST(ChronoFractionalSecondsTest, SmallFractionalSeconds) {
    auto dur = std::chrono::duration<double>(0.001);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "00.001" (seconds with 3 fractional digits)
    EXPECT_EQ(result, "00.001");
}

// Test formatting of seconds with large fractional part
TEST(ChronoFractionalSecondsTest, LargeFractionalSeconds) {
    auto dur = std::chrono::duration<double>(59.999);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "59.999" (seconds with 3 fractional digits)
    EXPECT_EQ(result, "59.999");
}

// Test formatting of seconds with rounding behavior
TEST(ChronoFractionalSecondsTest, RoundingFractionalSeconds) {
    auto dur = std::chrono::duration<double>(1.9999);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "01.999" (3 fractional digits, truncated/rounded to 3 places)
    EXPECT_EQ(result, "01.999");
}

// Test formatting of seconds with negative duration (should still format seconds)
TEST(ChronoFractionalSecondsTest, NegativeDurationSeconds) {
    auto dur = std::chrono::duration<double>(-1.5);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "01.500" (seconds are absolute value, sign handled elsewhere)
    EXPECT_EQ(result, "01.500");
}

// Test formatting of seconds with integer duration type
TEST(ChronoFractionalSecondsTest, IntegerDurationSeconds) {
    auto dur = std::chrono::seconds(5);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "05" (no fractional part for integer duration)
    EXPECT_EQ(result, "05");
}

// Test formatting of seconds with milliseconds duration
TEST(ChronoFractionalSecondsTest, MillisecondsDurationSeconds) {
    auto dur = std::chrono::milliseconds(1500);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "01.500" (1.5 seconds with 3 fractional digits)
    EXPECT_EQ(result, "01.500");
}

// Test formatting of seconds with microseconds duration
TEST(ChronoFractionalSecondsTest, MicrosecondsDurationSeconds) {
    auto dur = std::chrono::microseconds(1234567);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "01.234567" (1.234567 seconds with 6 fractional digits)
    EXPECT_EQ(result, "01.234567");
}

// Test formatting of seconds with nanoseconds duration
TEST(ChronoFractionalSecondsTest, NanosecondsDurationSeconds) {
    auto dur = std::chrono::nanoseconds(1234567890);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "01.234567890" (1.234567890 seconds with 9 fractional digits)
    EXPECT_EQ(result, "01.234567890");
}

// Test formatting of seconds with zero duration
TEST(ChronoFractionalSecondsTest, ZeroDurationSeconds) {
    auto dur = std::chrono::duration<double>(0.0);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "00.000" (zero seconds with 3 fractional digits)
    EXPECT_EQ(result, "00.000");
}

// Test formatting of seconds with max precision (9 digits)
TEST(ChronoFractionalSecondsTest, MaxPrecisionSeconds) {
    auto dur = std::chrono::duration<double>(1.123456789);
    std::string result = fmt::format("{:%S.%9}", dur);
    // Expected: "01.123456789" (9 fractional digits)
    EXPECT_EQ(result, "01.123456789");
}

// Test formatting of seconds with min precision (0 digits)
TEST(ChronoFractionalSecondsTest, MinPrecisionSeconds) {
    auto dur = std::chrono::duration<double>(1.5);
    std::string result = fmt::format("{:%S.%0}", dur);
    // Expected: "01" (no fractional digits)
    EXPECT_EQ(result, "01");
}

// Test formatting of seconds with custom precision that truncates
TEST(ChronoFractionalSecondsTest, TruncatingPrecisionSeconds) {
    auto dur = std::chrono::duration<double>(1.123456789);
    std::string result = fmt::format("{:%S.%3}", dur);
    // Expected: "01.123" (truncated to 3 fractional digits)
    EXPECT_EQ(result, "01.123");
}