#include <gtest/gtest.h>
#include "format.h"
#include <cmath>
#include <limits>
#include <string>
#include <cstring>

// Test fallback_format with various floating point values
TEST(FallbackFormatTest, BasicDouble) {
    double d = 3.14159;
    int num_digits = -1;  // shortest representation
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    // This should not crash and produce some output
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    // Buffer should have some content
    EXPECT_GT(buf.size(), 0);
}

TEST(FallbackFormatTest, Zero) {
    double d = 0.0;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    // Zero should produce "0"
    EXPECT_EQ(std::string(buf.data(), buf.size()), "0");
}

TEST(FallbackFormatTest, NegativeZero) {
    double d = -0.0;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    // Negative zero should produce "-0" or "0" depending on implementation
    std::string result(buf.data(), buf.size());
    EXPECT_TRUE(result == "0" || result == "-0");
}

TEST(FallbackFormatTest, One) {
    double d = 1.0;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    EXPECT_EQ(std::string(buf.data(), buf.size()), "1");
}

TEST(FallbackFormatTest, Ten) {
    double d = 10.0;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    EXPECT_EQ(std::string(buf.data(), buf.size()), "10");
}

TEST(FallbackFormatTest, Hundred) {
    double d = 100.0;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    EXPECT_EQ(std::string(buf.data(), buf.size()), "100");
}

TEST(FallbackFormatTest, SmallFraction) {
    double d = 0.5;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    EXPECT_EQ(std::string(buf.data(), buf.size()), "0.5");
}

TEST(FallbackFormatTest, LargeNumber) {
    double d = 1e10;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    // Should produce something like "10000000000" or "1e+10"
    std::string result(buf.data(), buf.size());
    EXPECT_FALSE(result.empty());
}

TEST(FallbackFormatTest, VerySmallNumber) {
    double d = 1e-10;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    // Should produce something like "0.0000000001" or "1e-10"
    std::string result(buf.data(), buf.size());
    EXPECT_FALSE(result.empty());
}

TEST(FallbackFormatTest, Infinity) {
    double d = std::numeric_limits<double>::infinity();
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    EXPECT_EQ(std::string(buf.data(), buf.size()), "inf");
}

TEST(FallbackFormatTest, NegativeInfinity) {
    double d = -std::numeric_limits<double>::infinity();
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    EXPECT_EQ(std::string(buf.data(), buf.size()), "-inf");
}

TEST(FallbackFormatTest, NaN) {
    double d = std::numeric_limits<double>::quiet_NaN();
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    EXPECT_EQ(std::string(buf.data(), buf.size()), "nan");
}

TEST(FallbackFormatTest, NegativeNaN) {
    double d = -std::numeric_limits<double>::quiet_NaN();
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    EXPECT_EQ(std::string(buf.data(), buf.size()), "-nan");
}

TEST(FallbackFormatTest, MaxDouble) {
    double d = std::numeric_limits<double>::max();
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    std::string result(buf.data(), buf.size());
    EXPECT_FALSE(result.empty());
    // Should not be infinity
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "-inf");
}

TEST(FallbackFormatTest, MinDouble) {
    double d = std::numeric_limits<double>::min();
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    std::string result(buf.data(), buf.size());
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "-inf");
}

TEST(FallbackFormatTest, Denormalized) {
    double d = std::numeric_limits<double>::denorm_min();
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    std::string result(buf.data(), buf.size());
    EXPECT_FALSE(result.empty());
}

TEST(FallbackFormatTest, Binary32Mode) {
    float f = 3.14f;
    double d = static_cast<double>(f);
    int num_digits = -1;
    bool binary32 = true;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    std::string result(buf.data(), buf.size());
    EXPECT_FALSE(result.empty());
}

TEST(FallbackFormatTest, FixedNumDigits) {
    double d = 3.141592653589793;
    int num_digits = 5;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    std::string result(buf.data(), buf.size());
    EXPECT_FALSE(result.empty());
    // Should have at most 5 significant digits
    // (plus possible decimal point and leading zeros)
}

TEST(FallbackFormatTest, NegativeNumber) {
    double d = -3.14;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    std::string result(buf.data(), buf.size());
    EXPECT_EQ(result[0], '-');
}

TEST(FallbackFormatTest, IntegerValue) {
    double d = 42.0;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    EXPECT_EQ(std::string(buf.data(), buf.size()), "42");
}

TEST(FallbackFormatTest, PowerOfTwo) {
    double d = 8.0;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    EXPECT_EQ(std::string(buf.data(), buf.size()), "8");
}

TEST(FallbackFormatTest, RepeatingDecimal) {
    double d = 1.0/3.0;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    std::string result(buf.data(), buf.size());
    EXPECT_FALSE(result.empty());
    // Should produce a reasonable representation
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "nan");
}

TEST(FallbackFormatTest, LargeExponent) {
    double d = 1e20;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    std::string result(buf.data(), buf.size());
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
}

TEST(FallbackFormatTest, SmallExponent) {
    double d = 1e-20;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf, exp10);
    
    std::string result(buf.data(), buf.size());
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "nan");
}

TEST(FallbackFormatTest, BufferReuse) {
    double d1 = 3.14;
    double d2 = 2.718;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf;
    int exp10 = 0;
    
    fmt::detail::fallback_format(d1, num_digits, binary32, buf, exp10);
    std::string first(buf.data(), buf.size());
    
    fmt::detail::fallback_format(d2, num_digits, binary32, buf, exp10);
    std::string second(buf.data(), buf.size());
    
    EXPECT_NE(first, second);
}

TEST(FallbackFormatTest, Consistency) {
    // Test that same value produces same output
    double d = 123.456;
    int num_digits = -1;
    bool binary32 = false;
    fmt::detail::buffer<char> buf1, buf2;
    int exp10_1 = 0, exp10_2 = 0;
    
    fmt::detail::fallback_format(d, num_digits, binary32, buf1, exp10_1);
    fmt::detail::fallback_format(d, num_digits, binary32, buf2, exp10_2);
    
    std::string result1(buf1.data(), buf1.size());
    std::string result2(buf2.data(), buf2.size());
    
    EXPECT_EQ(result1, result2);
    EXPECT_EQ(exp10_1, exp10_2);
}