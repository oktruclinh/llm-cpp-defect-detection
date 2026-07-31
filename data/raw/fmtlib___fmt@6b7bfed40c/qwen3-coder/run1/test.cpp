#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/core.h>
#include <string>
#include <limits>

// Test the format function with various numeric inputs to exercise the focal function
TEST(FmtFormatTest, FormatDouble) {
    // Test normal positive double
    std::string result = fmt::format("{}", 123.456);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result.find("123.456"), std::string::npos);
    
    // Test normal negative double
    result = fmt::format("{}", -123.456);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result.find("-123.456"), std::string::npos);
    
    // Test zero
    result = fmt::format("{}", 0.0);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result.find("0"), std::string::npos);
    
    // Test very small positive number
    result = fmt::format("{}", 1e-10);
    EXPECT_FALSE(result.empty());
    
    // Test very large positive number
    result = fmt::format("{}", 1e10);
    EXPECT_FALSE(result.empty());
    
    // Test scientific notation
    result = fmt::format("{:e}", 123.456);
    EXPECT_FALSE(result.empty());
    
    // Test with precision
    result = fmt::format("{:.2f}", 123.456);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result.find("123.46"), std::string::npos);
}

TEST(FmtFormatTest, FormatFloat) {
    // Test normal positive float
    std::string result = fmt::format("{}", 123.456f);
    EXPECT_FALSE(result.empty());
    
    // Test normal negative float
    result = fmt::format("{}", -123.456f);
    EXPECT_FALSE(result.empty());
    
    // Test zero float
    result = fmt::format("{}", 0.0f);
    EXPECT_FALSE(result.empty());
    
    // Test very small float
    result = fmt::format("{}", 1e-10f);
    EXPECT_FALSE(result.empty());
    
    // Test very large float
    result = fmt::format("{}", 1e10f);
    EXPECT_FALSE(result.empty());
}

TEST(FmtFormatTest, FormatSpecialValues) {
    // Test infinity
    double inf = std::numeric_limits<double>::infinity();
    std::string result = fmt::format("{}", inf);
    EXPECT_FALSE(result.empty());
    
    // Test negative infinity
    double ninf = -std::numeric_limits<double>::infinity();
    result = fmt::format("{}", ninf);
    EXPECT_FALSE(result.empty());
    
    // Test NaN
    double nan = std::numeric_limits<double>::quiet_NaN();
    result = fmt::format("{}", nan);
    EXPECT_FALSE(result.empty());
}

TEST(FmtFormatTest, FormatEdgeCases) {
    // Test minimum double
    double min_double = std::numeric_limits<double>::min();
    std::string result = fmt::format("{}", min_double);
    EXPECT_FALSE(result.empty());
    
    // Test maximum double
    double max_double = std::numeric_limits<double>::max();
    result = fmt::format("{}", max_double);
    EXPECT_FALSE(result.empty());
    
    // Test epsilon
    double epsilon = std::numeric_limits<double>::epsilon();
    result = fmt::format("{}", epsilon);
    EXPECT_FALSE(result.empty());
    
    // Test very small negative number
    result = fmt::format("{}", -1e-300);
    EXPECT_FALSE(result.empty());
    
    // Test very large negative number
    result = fmt::format("{}", -1e300);
    EXPECT_FALSE(result.empty());
}

TEST(FmtFormatTest, FormatIntegers) {
    // Test normal integers
    std::string result = fmt::format("{}", 42);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result.find("42"), std::string::npos);
    
    // Test negative integers
    result = fmt::format("{}", -42);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result.find("-42"), std::string::npos);
    
    // Test zero
    result = fmt::format("{}", 0);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result.find("0"), std::string::npos);
    
    // Test large integer
    result = fmt::format("{}", 1234567890123456789LL);
    EXPECT_FALSE(result.empty());
    
    // Test small integer
    result = fmt::format("{}", -1234567890123456789LL);
    EXPECT_FALSE(result.empty());
}

TEST(FmtFormatTest, FormatString) {
    // Test normal string
    std::string result = fmt::format("{}", "hello");
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result.find("hello"), std::string::npos);
    
    // Test empty string
    result = fmt::format("{}", "");
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result, "");
    
    // Test string with special characters
    result = fmt::format("{}", "hello\nworld\t!");
    EXPECT_FALSE(result.empty());
}