#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test formatting of seconds with fractional parts for various durations
TEST(ChronoWriteFractionalSecondsTest, SecondsWithFractional) {
    using namespace std::chrono_literals;
    
    // Test 1.5 seconds
    auto dur = std::chrono::duration<double>(1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");
    
    // Test 0.25 seconds
    dur = std::chrono::duration<double>(0.25);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.250");
    
    // Test 59.999 seconds (near boundary)
    dur = std::chrono::duration<double>(59.999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999");
}

// Test formatting of seconds with no fractional part (integer seconds)
TEST(ChronoWriteFractionalSecondsTest, IntegerSeconds) {
    using namespace std::chrono_literals;
    
    // Test exactly 0 seconds
    auto dur = std::chrono::seconds(0);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00");
    
    // Test exactly 30 seconds
    dur = std::chrono::seconds(30);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "30");
    
    // Test exactly 59 seconds
    dur = std::chrono::seconds(59);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59");
}

// Test formatting of seconds with very small fractional parts
TEST(ChronoWriteFractionalSecondsTest, SmallFractional) {
    using namespace std::chrono_literals;
    
    // Test 0.001 seconds (1 millisecond)
    auto dur = std::chrono::duration<double>(0.001);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.001");
    
    // Test 0.000001 seconds (1 microsecond)
    dur = std::chrono::duration<double>(0.000001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000001");
}

// Test formatting of seconds with very large fractional parts
TEST(ChronoWriteFractionalSecondsTest, LargeFractional) {
    using namespace std::chrono_literals;
    
    // Test 0.999999 seconds
    auto dur = std::chrono::duration<double>(0.999999);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.999999");
}

// Test formatting of seconds with negative values
TEST(ChronoWriteFractionalSecondsTest, NegativeSeconds) {
    using namespace std::chrono_literals;
    
    // Test -1.5 seconds
    auto dur = std::chrono::duration<double>(-1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-01.500");
    
    // Test -0.25 seconds
    dur = std::chrono::duration<double>(-0.25);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-00.250");
}

// Test formatting of seconds with different precision specifications
TEST(ChronoWriteFractionalSecondsTest, PrecisionSpecifier) {
    using namespace std::chrono_literals;
    
    // Test with precision specifier .3
    auto dur = std::chrono::duration<double>(1.23456);
    std::string result = fmt::format("{:.3%S}", dur);
    EXPECT_EQ(result, "01.235");  // Rounded to 3 decimal places
    
    // Test with precision specifier .0 (no fractional)
    result = fmt::format("{:.0%S}", dur);
    EXPECT_EQ(result, "01");
    
    // Test with precision specifier .6
    result = fmt::format("{:.6%S}", dur);
    EXPECT_EQ(result, "01.234560");
}

// Test formatting of seconds with chrono::duration using different rep types
TEST(ChronoWriteFractionalSecondsTest, DifferentRepTypes) {
    // Test with float rep
    auto dur = std::chrono::duration<float>(2.5f);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "02.500");
    
    // Test with long double rep
    dur = std::chrono::duration<long double>(3.14159L);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "03.14159");
}

// Test formatting of seconds with different period types
TEST(ChronoWriteFractionalSecondsTest, DifferentPeriodTypes) {
    // Test with milliseconds period
    auto dur = std::chrono::milliseconds(1500);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");
    
    // Test with microseconds period
    auto dur_us = std::chrono::microseconds(2500000);
    result = fmt::format("{:%S}", dur_us);
    EXPECT_EQ(result, "02.500000");
    
    // Test with nanoseconds period
    auto dur_ns = std::chrono::nanoseconds(1234567890);
    result = fmt::format("{:%S}", dur_ns);
    EXPECT_EQ(result, "01.234567890");
}

// Test formatting of seconds with zero duration
TEST(ChronoWriteFractionalSecondsTest, ZeroDuration) {
    using namespace std::chrono_literals;
    
    // Test zero seconds with double rep
    auto dur = std::chrono::duration<double>(0.0);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.0");
    
    // Test zero seconds with integer rep
    dur = std::chrono::seconds(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00");
}

// Test formatting of seconds with very large values
TEST(ChronoWriteFractionalSecondsTest, LargeValues) {
    using namespace std::chrono_literals;
    
    // Test 1000 seconds (should wrap to minutes, but we only format seconds part)
    auto dur = std::chrono::duration<double>(1000.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "40.500");  // 1000.5 mod 60 = 40.5
    
    // Test 3600 seconds (1 hour)
    dur = std::chrono::duration<double>(3600.25);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.250");  // 3600.25 mod 60 = 0.25
}

// Test formatting of seconds with very small negative values near zero
TEST(ChronoWriteFractionalSecondsTest, NegativeNearZero) {
    using namespace std::chrono_literals;
    
    // Test -0.001 seconds
    auto dur = std::chrono::duration<double>(-0.001);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-00.001");
    
    // Test -0.000001 seconds
    dur = std::chrono::duration<double>(-0.000001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-00.000001");
}