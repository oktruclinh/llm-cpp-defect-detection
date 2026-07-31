#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <chrono>
#include <string>

TEST(ChronoFormatterTest, PositiveDuration) {
    auto start = std::chrono::system_clock::now();
    auto end = start + std::chrono::seconds(42) + std::chrono::milliseconds(123);
    auto duration = end - start;
    
    std::string result = fmt::format("{}", duration);
    EXPECT_NE(result, "");
    EXPECT_NE(result.find("42"), std::string::npos);
    EXPECT_NE(result.find("123"), std::string::npos);
}

TEST(ChronoFormatterTest, ZeroDuration) {
    std::chrono::milliseconds zero(0);
    std::string result = fmt::format("{}", zero);
    EXPECT_EQ(result, "0ms");
}

TEST(ChronoFormatterTest, NegativeDuration) {
    std::chrono::milliseconds negative(-1500);
    std::string result = fmt::format("{}", negative);
    EXPECT_TRUE(result[0] == '-');
    EXPECT_NE(result.find("1500"), std::string::npos);
}

TEST(ChronoFormatterTest, LargeDuration) {
    std::chrono::seconds large(999999);
    std::string result = fmt::format("{}", large);
    EXPECT_NE(result.find("999999"), std::string::npos);
}

TEST(ChronoFormatterTest, FractionalMilliseconds) {
    std::chrono::microseconds micro(123456);
    std::string result = fmt::format("{}", micro);
    EXPECT_NE(result.find("123.456"), std::string::npos);
}

TEST(ChronoFormatterTest, MixedUnits) {
    auto duration = std::chrono::hours(2) + std::chrono::minutes(30) + 
                   std::chrono::seconds(45) + std::chrono::milliseconds(123);
    std::string result = fmt::format("{}", duration);
    EXPECT_NE(result.find("2"), std::string::npos);
    EXPECT_NE(result.find("30"), std::string::npos);
    EXPECT_NE(result.find("45"), std::string::npos);
    EXPECT_NE(result.find("123"), std::string::npos);
}

TEST(ChronoFormatterTest, SubsecondPrecision) {
    std::chrono::nanoseconds nano(123456789);
    std::string result = fmt::format("{}", nano);
    EXPECT_NE(result.find("123.456789"), std::string::npos);
}

TEST(ChronoFormatterTest, ExactSecond) {
    std::chrono::seconds exact(60);
    std::string result = fmt::format("{}", exact);
    EXPECT_NE(result.find("60"), std::string::npos);
    EXPECT_EQ(result.find("ms"), std::string::npos) << "Should not contain milliseconds for exact seconds";
}

TEST(ChronoFormatterTest, MillisecondBoundary) {
    std::chrono::milliseconds boundary(999);
    std::string result = fmt::format("{}", boundary);
    EXPECT_NE(result.find("999"), std::string::npos);
    
    std::chrono::milliseconds boundary2(1000);
    result = fmt::format("{}", boundary2);
    EXPECT_NE(result.find("1000"), std::string::npos);
}