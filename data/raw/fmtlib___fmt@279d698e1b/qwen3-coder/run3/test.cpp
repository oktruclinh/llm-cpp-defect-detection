#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <limits>
#include <stdexcept>

TEST(FmtFormatTest, FormatIntegers) {
    // Test normal positive integer formatting
    std::string result = fmt::format("{}", 42);
    EXPECT_EQ(result, "42");
    
    // Test negative integer formatting
    result = fmt::format("{}", -42);
    EXPECT_EQ(result, "-42");
    
    // Test zero
    result = fmt::format("{}", 0);
    EXPECT_EQ(result, "0");
    
    // Test large positive integer
    result = fmt::format("{}", 1234567890);
    EXPECT_EQ(result, "1234567890");
    
    // Test large negative integer
    result = fmt::format("{}", -1234567890);
    EXPECT_EQ(result, "-1234567890");
    
    // Test with width specification
    result = fmt::format("{:10}", 42);
    EXPECT_EQ(result, "        42");
    
    // Test with zero padding
    result = fmt::format("{:010}", 42);
    EXPECT_EQ(result, "0000000042");
    
    // Test with sign specification
    result = fmt::format("{:+}", 42);
    EXPECT_EQ(result, "+42");
    
    // Test with sign specification for negative
    result = fmt::format("{:+}", -42);
    EXPECT_EQ(result, "-42");
    
    // Test with space for positive
    result = fmt::format("{: }", 42);
    EXPECT_EQ(result, " 42");
    
    // Test with space for negative
    result = fmt::format("{: }", -42);
    EXPECT_EQ(result, "-42");
}

TEST(FmtFormatTest, FormatFloatingPoint) {
    // Test normal float formatting
    std::string result = fmt::format("{}", 3.14159);
    EXPECT_EQ(result, "3.14159");
    
    // Test negative float
    result = fmt::format("{}", -3.14159);
    EXPECT_EQ(result, "-3.14159");
    
    // Test zero float
    result = fmt::format("{}", 0.0);
    EXPECT_EQ(result, "0");
    
    // Test with precision
    result = fmt::format("{:.2f}", 3.14159);
    EXPECT_EQ(result, "3.14");
    
    // Test with width
    result = fmt::format("{:10.2f}", 3.14159);
    EXPECT_EQ(result, "      3.14");
    
    // Test scientific notation
    result = fmt::format("{:.2e}", 1234.56);
    EXPECT_EQ(result, "1.23e+03");
    
    // Test with zero padding
    result = fmt::format("{:010.2f}", 3.14159);
    EXPECT_EQ(result, "000003.14");
}

TEST(FmtFormatTest, FormatStrings) {
    // Test normal string
    std::string result = fmt::format("{}", "hello");
    EXPECT_EQ(result, "hello");
    
    // Test empty string
    result = fmt::format("{}", "");
    EXPECT_EQ(result, "");
    
    // Test string with width
    result = fmt::format("{:10}", "hello");
    EXPECT_EQ(result, "hello     ");
    
    // Test string with width (shorter than width)
    result = fmt::format("{:10}", "hi");
    EXPECT_EQ(result, "hi       ");
    
    // Test string with width (longer than width)
    result = fmt::format("{:3}", "hello");
    EXPECT_EQ(result, "hel");
}

TEST(FmtFormatTest, FormatMixed) {
    // Test mixed format specifiers
    std::string result = fmt::format("Hello {}! You are {} years old.", "World", 25);
    EXPECT_EQ(result, "Hello World! You are 25 years old.");
    
    // Test with multiple same specifier
    result = fmt::format("{} + {} = {}", 2, 3, 5);
    EXPECT_EQ(result, "2 + 3 = 5");
    
    // Test with complex formatting
    result = fmt::format("Price: ${:.2f}, Tax: {:.1%}", 19.99, 0.08);
    EXPECT_EQ(result, "Price: $19.99, Tax: 8.0%");
}

TEST(FmtFormatTest, FormatSpecialCases) {
    // Test with null pointer (should not crash)
    const char* null_ptr = nullptr;
    std::string result = fmt::format("{}", null_ptr);
    EXPECT_EQ(result, "(null)");
    
    // Test with boolean
    result = fmt::format("{}", true);
    EXPECT_EQ(result, "true");
    
    result = fmt::format("{}", false);
    EXPECT_EQ(result, "false");
    
    // Test with character
    result = fmt::format("{}", 'A');
    EXPECT_EQ(result, "A");
    
    // Test with unsigned integer
    result = fmt::format("{}", static_cast<unsigned int>(42));
    EXPECT_EQ(result, "42");
    
    // Test with long long
    result = fmt::format("{}", static_cast<long long>(9223372036854775807LL));
    EXPECT_EQ(result, "9223372036854775807");
    
    // Test with unsigned long long
    result = fmt::format("{}", static_cast<unsigned long long>(18446744073709551615ULL));
    EXPECT_EQ(result, "18446744073709551615");
}

TEST(FmtFormatTest, FormatEdgeCases) {
    // Test with maximum integer values
    std::string result = fmt::format("{}", std::numeric_limits<int>::max());
    EXPECT_EQ(result, "2147483647");
    
    result = fmt::format("{}", std::numeric_limits<int>::min());
    EXPECT_EQ(result, "-2147483648");
    
    // Test with maximum double values
    result = fmt::format("{}", std::numeric_limits<double>::max());
    EXPECT_NE(result, "inf");
    
    // Test with minimum double values
    result = fmt::format("{}", std::numeric_limits<double>::min());
    EXPECT_NE(result, "0");
    
    // Test with very small double
    result = fmt::format("{}", 1e-308);
    EXPECT_NE(result, "0");
    
    // Test with very large double
    result = fmt::format("{}", 1e308);
    EXPECT_NE(result, "inf");
    
    // Test with NaN
    double nan_val = std::numeric_limits<double>::quiet_NaN();
    result = fmt::format("{}", nan_val);
    EXPECT_EQ(result, "nan");
    
    // Test with infinity
    double inf_val = std::numeric_limits<double>::infinity();
    result = fmt::format("{}", inf_val);
    EXPECT_EQ(result, "inf");
    
    // Test with negative infinity
    result = fmt::format("{}", -inf_val);
    EXPECT_EQ(result, "-inf");
}