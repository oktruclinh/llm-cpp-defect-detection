#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <chrono>
#include <string>

TEST(ChronoFormatterTest, PositiveDuration) {
    std::string buffer;
    auto out = std::back_inserter(buffer);
    
    std::chrono::milliseconds ms(1500);
    fmt::format_to(out, "{}", ms);
    
    EXPECT_EQ(buffer, "1.5s");
}

TEST(ChronoFormatterTest, NegativeDuration) {
    std::string buffer;
    auto out = std::back_inserter(buffer);
    
    std::chrono::milliseconds ms(-1500);
    fmt::format_to(out, "{}", ms);
    
    EXPECT_EQ(buffer, "-1.5s");
}

TEST(ChronoFormatterTest, ZeroDuration) {
    std::string buffer;
    auto out = std::back_inserter(buffer);
    
    std::chrono::milliseconds ms(0);
    fmt::format_to(out, "{}", ms);
    
    EXPECT_EQ(buffer, "0s");
}

TEST(ChronoFormatterTest, Microseconds) {
    std::string buffer;
    auto out = std::back_inserter(buffer);
    
    std::chrono::microseconds us(1234567);
    fmt::format_to(out, "{}", us);
    
    EXPECT_EQ(buffer, "1.234567s");
}

TEST(ChronoFormatterTest, Nanoseconds) {
    std::string buffer;
    auto out = std::back_inserter(buffer);
    
    std::chrono::nanoseconds ns(1234567890);
    fmt::format_to(out, "{}", ns);
    
    EXPECT_EQ(buffer, "1.23456789s");
}

TEST(ChronoFormatterTest, Minutes) {
    std::string buffer;
    auto out = std::back_inserter(buffer);
    
    std::chrono::minutes min(5);
    fmt::format_to(out, "{}", min);
    
    EXPECT_EQ(buffer, "5m");
}

TEST(ChronoFormatterTest, Hours) {
    std::string buffer;
    auto out = std::back_inserter(buffer);
    
    std::chrono::hours h(2);
    fmt::format_to(out, "{}", h);
    
    EXPECT_EQ(buffer, "2h");
}

TEST(ChronoFormatterTest, ComplexDuration) {
    std::string buffer;
    auto out = std::back_inserter(buffer);
    
    // 2 hours, 30 minutes, 45 seconds, 123 milliseconds
    auto duration = std::chrono::hours(2) + std::chrono::minutes(30) + 
                   std::chrono::seconds(45) + std::chrono::milliseconds(123);
    fmt::format_to(out, "{}", duration);
    
    EXPECT_EQ(buffer, "2h 30m 45.123s");
}

TEST(ChronoFormatterTest, NegativeComplexDuration) {
    std::string buffer;
    auto out = std::back_inserter(buffer);
    
    // -2 hours, -30 minutes, -45 seconds, -123 milliseconds
    auto duration = -(std::chrono::hours(2) + std::chrono::minutes(30) + 
                     std::chrono::seconds(45) + std::chrono::milliseconds(123));
    fmt::format_to(out, "{}", duration);
    
    EXPECT_EQ(buffer, "-2h 30m 45.123s");
}

TEST(ChronoFormatterTest, VerySmallPositiveDuration) {
    std::string buffer;
    auto out = std::back_inserter(buffer);
    
    std::chrono::nanoseconds ns(1);
    fmt::format_to(out, "{}", ns);
    
    EXPECT_EQ(buffer, "0.000000001s");
}

TEST(ChronoFormatterTest, VeryLargeDuration) {
    std::string buffer;
    auto out = std::back_inserter(buffer);
    
    std::chrono::hours h(1000000);
    fmt::format_to(out, "{}", h);
    
    EXPECT_EQ(buffer, "1000000h");
}

TEST(ChronoFormatterTest, DurationWithFractionalSeconds) {
    std::string buffer;
    auto out = std::back_inserter(buffer);
    
    std::chrono::milliseconds ms(1234);
    fmt::format_to(out, "{}", ms);
    
    EXPECT_EQ(buffer, "1.234s");
}