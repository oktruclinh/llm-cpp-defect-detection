#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <chrono>
#include <string>

TEST(ChronoTest, WriteFractionalSeconds) {
    // Test normal case with fractional seconds
    auto time_point = std::chrono::system_clock::from_time_t(1609459200); // 2021-01-01 00:00:00 UTC
    time_point += std::chrono::milliseconds(123);
    
    std::string result = fmt::format("{:%S}", time_point);
    // Should contain seconds with fractional part
    EXPECT_NE(result.find('.'), std::string::npos);
    EXPECT_TRUE(result.length() > 2);
    
    // Test with zero fractional seconds
    auto time_point_zero = std::chrono::system_clock::from_time_t(1609459200);
    std::string result_zero = fmt::format("{:%S}", time_point_zero);
    // Should be exactly "00" or similar without decimal point
    EXPECT_NE(result_zero.find('.'), std::string::npos) << "Zero fractional seconds should still show decimal point";
    
    // Test with exactly 1 second
    auto time_point_one = std::chrono::system_clock::from_time_t(1609459201);
    time_point_one += std::chrono::milliseconds(0);
    std::string result_one = fmt::format("{:%S}", time_point_one);
    EXPECT_NE(result_one.find('.'), std::string::npos);
    
    // Test with 999 milliseconds (boundary case)
    auto time_point_999 = std::chrono::system_clock::from_time_t(1609459200);
    time_point_999 += std::chrono::milliseconds(999);
    std::string result_999 = fmt::format("{:%S}", time_point_999);
    EXPECT_NE(result_999.find('.'), std::string::npos);
    EXPECT_TRUE(result_999.length() > 2);
    
    // Test with 500 milliseconds
    auto time_point_500 = std::chrono::system_clock::from_time_t(1609459200);
    time_point_500 += std::chrono::milliseconds(500);
    std::string result_500 = fmt::format("{:%S}", time_point_500);
    EXPECT_NE(result_500.find('.'), std::string::npos);
    EXPECT_TRUE(result_500.length() > 2);
    
    // Test with microsecond precision
    auto time_point_micro = std::chrono::system_clock::from_time_t(1609459200);
    time_point_micro += std::chrono::microseconds(123456);
    std::string result_micro = fmt::format("{:%S}", time_point_micro);
    EXPECT_NE(result_micro.find('.'), std::string::npos);
    EXPECT_TRUE(result_micro.length() > 2);
    
    // Test with nanosecond precision
    auto time_point_nano = std::chrono::system_clock::from_time_t(1609459200);
    time_point_nano += std::chrono::nanoseconds(123456789);
    std::string result_nano = fmt::format("{:%S}", time_point_nano);
    EXPECT_NE(result_nano.find('.'), std::string::npos);
    EXPECT_TRUE(result_nano.length() > 2);
    
    // Test edge case: exactly 60 seconds (should wrap to next minute)
    auto time_point_60 = std::chrono::system_clock::from_time_t(1609459259); // 59 seconds
    time_point_60 += std::chrono::milliseconds(999); // 59.999 seconds
    std::string result_60 = fmt::format("{:%S}", time_point_60);
    EXPECT_NE(result_60.find('.'), std::string::npos);
    
    // Test that the format string is properly handled
    auto time_point_normal = std::chrono::system_clock::from_time_t(1609459200);
    time_point_normal += std::chrono::milliseconds(456);
    std::string result_format = fmt::format("{:.3S}", time_point_normal);
    EXPECT_NE(result_format.find('.'), std::string::npos);
    EXPECT_TRUE(result_format.length() > 2);
}