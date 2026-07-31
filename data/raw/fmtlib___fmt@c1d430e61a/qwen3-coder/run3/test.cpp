#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <chrono>

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
    auto duration = std::chrono::seconds(0);
    std::string result = fmt::format("{}", duration);
    EXPECT_EQ(result, "0s");
}

TEST(ChronoFormatterTest, NegativeDuration) {
    auto duration = -std::chrono::seconds(5);
    std::string result = fmt::format("{}", duration);
    EXPECT_TRUE(result[0] == '-');
    EXPECT_NE(result.find("5"), std::string::npos);
}

TEST(ChronoFormatterTest, SubSecondDuration) {
    auto duration = std::chrono::milliseconds(750);
    std::string result = fmt::format("{}", duration);
    EXPECT_NE(result.find("750"), std::string::npos);
    EXPECT_NE(result.find("ms"), std::string::npos);
}

TEST(ChronoFormatterTest, MixedUnitsDuration) {
    auto duration = std::chrono::seconds(120) + std::chrono::milliseconds(500);
    std::string result = fmt::format("{}", duration);
    EXPECT_NE(result.find("120"), std::string::npos);
    EXPECT_NE(result.find("500"), std::string::npos);
}

TEST(ChronoFormatterTest, LargeDuration) {
    auto duration = std::chrono::hours(25) + std::chrono::minutes(30) + std::chrono::seconds(45);
    std::string result = fmt::format("{}", duration);
    EXPECT_NE(result.find("25"), std::string::npos);
    EXPECT_NE(result.find("30"), std::string::npos);
    EXPECT_NE(result.find("45"), std::string::npos);
}

TEST(ChronoFormatterTest, MicrosecondPrecision) {
    auto duration = std::chrono::microseconds(123456);
    std::string result = fmt::format("{}", duration);
    EXPECT_NE(result.find("123456"), std::string::npos);
    EXPECT_NE(result.find("us"), std::string::npos);
}

TEST(ChronoFormatterTest, NanosecondPrecision) {
    auto duration = std::chrono::nanoseconds(123456789);
    std::string result = fmt::format("{}", duration);
    EXPECT_NE(result.find("123456789"), std::string::npos);
    EXPECT_NE(result.find("ns"), std::string::npos);
}

TEST(ChronoFormatterTest, DurationWithFractionalSeconds) {
    auto duration = std::chrono::milliseconds(1500);
    std::string result = fmt::format("{}", duration);
    EXPECT_NE(result.find("1"), std::string::npos);
    EXPECT_NE(result.find("500"), std::string::npos);
    EXPECT_NE(result.find("s"), std::string::npos);
}

TEST(ChronoFormatterTest, DurationEdgeCaseZero) {
    auto duration = std::chrono::duration<int, std::ratio<1, 1000>>(0);
    std::string result = fmt::format("{}", duration);
    EXPECT_EQ(result, "0ms");
}

TEST(ChronoFormatterTest, DurationEdgeCaseNegativeZero) {
    auto duration = std::chrono::duration<int, std::ratio<1, 1000>>(-0);
    std::string result = fmt::format("{}", duration);
    EXPECT_EQ(result, "0ms");
}

TEST(ChronoFormatterTest, DurationEdgeCaseMinMax) {
    auto max_duration = std::chrono::duration<long long, std::ratio<1, 1000>>(9223372036854775807LL);
    std::string result = fmt::format("{}", max_duration);
    EXPECT_NE(result, "");
    
    auto min_duration = std::chrono::duration<long long, std::ratio<1, 1000>>(-9223372036854775807LL);
    std::string result2 = fmt::format("{}", min_duration);
    EXPECT_TRUE(result2[0] == '-');
}