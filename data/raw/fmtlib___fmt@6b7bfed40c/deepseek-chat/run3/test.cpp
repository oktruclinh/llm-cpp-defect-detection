#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/format-inl.h>
#include <string>
#include <cmath>
#include <cstdint>

// Test that format correctly handles large exponents that trigger the
// denominator.assign_pow10 and denominator <<= shift path (exp10 > 0)
TEST(FormatInlTest, LargePositiveExponent) {
    // This value has a large positive exponent that should exercise the
    // denominator.assign_pow10(exp10) and denominator <<= shift path
    double value = 1e50;
    std::string result = fmt::format("{}", value);
    EXPECT_FALSE(result.empty());
    // The result should be a valid representation of a large number
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "nan");
}

// Test that format correctly handles large negative exponents that trigger the
// numerator.assign_pow10(-exp10) and lower.assign(numerator) path
TEST(FormatInlTest, LargeNegativeExponent) {
    // This value has a large negative exponent that should exercise the
    // numerator.assign_pow10(-exp10) and lower.assign(numerator) path
    double value = 1e-50;
    std::string result = fmt::format("{}", value);
    EXPECT_FALSE(result.empty());
    // The result should be a valid representation of a very small number
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "nan");
    EXPECT_NE(result, "0");
}

// Test with a value that has a moderate positive exponent
TEST(FormatInlTest, ModeratePositiveExponent) {
    double value = 1e10;
    std::string result = fmt::format("{}", value);
    EXPECT_EQ(result, "10000000000");
}

// Test with a value that has a moderate negative exponent
TEST(FormatInlTest, ModerateNegativeExponent) {
    double value = 1e-5;
    std::string result = fmt::format("{}", value);
    EXPECT_EQ(result, "0.00001");
}

// Test with a value that has exponent exactly at the boundary
TEST(FormatInlTest, ExponentBoundaryPositive) {
    // exp10 = 0 should not trigger either branch
    double value = 1.0;
    std::string result = fmt::format("{}", value);
    EXPECT_EQ(result, "1");
}

// Test with a value that has exponent exactly at the boundary (negative)
TEST(FormatInlTest, ExponentBoundaryNegative) {
    // exp10 = -1 should trigger the negative branch
    double value = 0.1;
    std::string result = fmt::format("{}", value);
    EXPECT_EQ(result, "0.1");
}

// Test with a value that has a very large positive exponent (edge case)
TEST(FormatInlTest, VeryLargePositiveExponent) {
    double value = 1e100;
    std::string result = fmt::format("{}", value);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "nan");
    // Should contain 'e' for scientific notation
    EXPECT_TRUE(result.find('e') != std::string::npos || 
                result.find('E') != std::string::npos);
}

// Test with a value that has a very large negative exponent (edge case)
TEST(FormatInlTest, VeryLargeNegativeExponent) {
    double value = 1e-100;
    std::string result = fmt::format("{}", value);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "nan");
    EXPECT_NE(result, "0");
    // Should contain 'e' for scientific notation
    EXPECT_TRUE(result.find('e') != std::string::npos || 
                result.find('E') != std::string::npos);
}

// Test with a value that has exponent causing overflow in shift
TEST(FormatInlTest, ExponentCausingLargeShift) {
    // This value should trigger a large shift in the denominator
    double value = 1e20;
    std::string result = fmt::format("{}", value);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "nan");
}

// Test with a value that has exponent causing underflow in numerator
TEST(FormatInlTest, ExponentCausingLargeNumerator) {
    // This value should trigger a large numerator assignment
    double value = 1e-20;
    std::string result = fmt::format("{}", value);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "nan");
    EXPECT_NE(result, "0");
}

// Test with a value that has exponent exactly at the threshold
TEST(FormatInlTest, ExponentAtThreshold) {
    // exp10 = 1 should trigger the positive branch
    double value = 10.0;
    std::string result = fmt::format("{}", value);
    EXPECT_EQ(result, "10");
}

// Test with a value that has exponent exactly at the negative threshold
TEST(FormatInlTest, ExponentAtNegativeThreshold) {
    // exp10 = -2 should trigger the negative branch
    double value = 0.01;
    std::string result = fmt::format("{}", value);
    EXPECT_EQ(result, "0.01");
}

// Test with multiple values to ensure consistent behavior
TEST(FormatInlTest, MultipleExponentValues) {
    struct TestCase {
        double value;
        std::string expected;
    };
    
    TestCase cases[] = {
        {1e15, "1000000000000000"},
        {1e-15, "0.000000000000001"},
        {1e5, "100000"},
        {1e-5, "0.00001"},
        {1e3, "1000"},
        {1e-3, "0.001"},
    };
    
    for (const auto& tc : cases) {
        std::string result = fmt::format("{}", tc.value);
        EXPECT_EQ(result, tc.expected);
    }
}

// Test with values that have non-integer mantissa and large exponent
TEST(FormatInlTest, NonIntegerMantissaLargeExponent) {
    double value = 3.14159e20;
    std::string result = fmt::format("{}", value);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "nan");
}

// Test with values that have non-integer mantissa and large negative exponent
TEST(FormatInlTest, NonIntegerMantissaLargeNegativeExponent) {
    double value = 3.14159e-20;
    std::string result = fmt::format("{}", value);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "nan");
    EXPECT_NE(result, "0");
}