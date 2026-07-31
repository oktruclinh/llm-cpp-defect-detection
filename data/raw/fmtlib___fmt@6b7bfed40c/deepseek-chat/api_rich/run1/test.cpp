#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>
#include <cstdint>

// Test formatting of very large numbers that exercise the denominator shift path
TEST(FmtFormatInlTest, LargeNumberDenominatorShift) {
    // Test with a large double that requires denominator shift
    double large_val = 1e15;
    std::string result = fmt::format("{}", large_val);
    EXPECT_EQ(result, "1000000000000000");
    
    // Test with very large double that requires significant shift
    double very_large = 1e20;
    result = fmt::format("{}", very_large);
    EXPECT_EQ(result, "100000000000000000000");
}

// Test formatting of very small numbers that exercise the numerator path
TEST(FmtFormatInlTest, SmallNumberNumeratorPath) {
    // Test with a small double that requires numerator assignment
    double small_val = 1e-10;
    std::string result = fmt::format("{}", small_val);
    EXPECT_EQ(result, "0.0000000001");
    
    // Test with extremely small double
    double very_small = 1e-15;
    result = fmt::format("{}", very_small);
    EXPECT_EQ(result, "0.000000000000001");
}

// Test formatting of numbers that trigger both numerator and denominator paths
TEST(FmtFormatInlTest, MixedExponentPaths) {
    // Test with positive exponent that requires denominator shift
    double pos_exp = 1.234e5;
    std::string result = fmt::format("{}", pos_exp);
    EXPECT_EQ(result, "123400");
    
    // Test with negative exponent that requires numerator assignment
    double neg_exp = 1.234e-5;
    result = fmt::format("{}", neg_exp);
    EXPECT_EQ(result, "0.00001234");
}

// Test edge cases around exponent boundaries
TEST(FmtFormatInlTest, ExponentBoundaryCases) {
    // Test with exponent exactly at boundary (exp10 = 0)
    double boundary_val = 1.0;
    std::string result = fmt::format("{}", boundary_val);
    EXPECT_EQ(result, "1");
    
    // Test with exponent just above zero
    double just_above = 1.0001;
    result = fmt::format("{}", just_above);
    EXPECT_EQ(result, "1.0001");
    
    // Test with exponent just below zero
    double just_below = 0.9999;
    result = fmt::format("{}", just_below);
    EXPECT_EQ(result, "0.9999");
}

// Test with numbers that require significant precision
TEST(FmtFormatInlTest, HighPrecisionNumbers) {
    // Test with a number that has many decimal places
    double precise = 3.141592653589793;
    std::string result = fmt::format("{:.15f}", precise);
    EXPECT_EQ(result, "3.141592653589793");
    
    // Test with a number that requires both numerator and denominator adjustments
    double complex = 1.234567890123456e-10;
    result = fmt::format("{:.15f}", complex);
    EXPECT_EQ(result, "0.0000000001234567890123456");
}

// Test with extreme values that stress the algorithm
TEST(FmtFormatInlTest, ExtremeValueCases) {
    // Test with maximum finite double
    double max_double = std::numeric_limits<double>::max();
    std::string result = fmt::format("{}", max_double);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
    EXPECT_NE(result, "nan");
    
    // Test with minimum positive double
    double min_double = std::numeric_limits<double>::min();
    result = fmt::format("{}", min_double);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0");
    EXPECT_NE(result, "inf");
    
    // Test with denormalized number
    double denorm = std::numeric_limits<double>::denorm_min();
    result = fmt::format("{}", denorm);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0");
}

// Test with numbers that have specific exponent patterns
TEST(FmtFormatInlTest, SpecificExponentPatterns) {
    // Test with exponent that is a power of 10
    double pow10_pos = 1e10;
    std::string result = fmt::format("{}", pow10_pos);
    EXPECT_EQ(result, "10000000000");
    
    double pow10_neg = 1e-10;
    result = fmt::format("{}", pow10_neg);
    EXPECT_EQ(result, "0.0000000001");
    
    // Test with exponent that is not a power of 10
    double non_pow10 = 2.5e5;
    result = fmt::format("{}", non_pow10);
    EXPECT_EQ(result, "250000");
    
    double non_pow10_neg = 2.5e-5;
    result = fmt::format("{}", non_pow10_neg);
    EXPECT_EQ(result, "0.000025");
}

// Test with numbers that require large shifts
TEST(FmtFormatInlTest, LargeShiftCases) {
    // Test with very large positive exponent requiring large denominator shift
    double huge_pos = 1e100;
    std::string result = fmt::format("{}", huge_pos);
    EXPECT_EQ(result, "1e+100");
    
    // Test with very large negative exponent requiring large numerator assignment
    double huge_neg = 1e-100;
    result = fmt::format("{}", huge_neg);
    EXPECT_EQ(result, "1e-100");
}

// Test with numbers that have trailing zeros
TEST(FmtFormatInlTest, TrailingZeroCases) {
    // Test with number that has trailing zeros after decimal
    double trailing = 1.5000;
    std::string result = fmt::format("{}", trailing);
    EXPECT_EQ(result, "1.5");
    
    // Test with number that has many trailing zeros
    double many_trailing = 2.00000;
    result = fmt::format("{}", many_trailing);
    EXPECT_EQ(result, "2");
}

// Test with numbers that require rounding
TEST(FmtFormatInlTest, RoundingCases) {
    // Test with number that rounds up
    double round_up = 1.999999999999999;
    std::string result = fmt::format("{:.15f}", round_up);
    EXPECT_EQ(result, "2.000000000000000");
    
    // Test with number that rounds down
    double round_down = 1.000000000000001;
    result = fmt::format("{:.15f}", round_down);
    EXPECT_EQ(result, "1.000000000000001");
}

// Test with numbers that have specific digit patterns
TEST(FmtFormatInlTest, DigitPatternCases) {
    // Test with repeating decimal pattern
    double repeating = 1.0/3.0;
    std::string result = fmt::format("{:.10f}", repeating);
    EXPECT_EQ(result, "0.3333333333");
    
    // Test with number that has all same digits
    double all_same = 111111.111111;
    result = fmt::format("{:.6f}", all_same);
    EXPECT_EQ(result, "111111.111111");
}

// Test with numbers that exercise the boundary between fixed and scientific notation
TEST(FmtFormatInlTest, NotationBoundaryCases) {
    // Test with number just above the threshold for scientific notation
    double just_above_sci = 1e15;
    std::string result = fmt::format("{}", just_above_sci);
    EXPECT_EQ(result, "1000000000000000");
    
    // Test with number just below the threshold for scientific notation
    double just_below_sci = 1e-4;
    result = fmt::format("{}", just_below_sci);
    EXPECT_EQ(result, "0.0001");
}

// Test with numbers that have specific mantissa patterns
TEST(FmtFormatInlTest, MantissaPatternCases) {
    // Test with mantissa that is a power of 2
    double pow2_mantissa = 1.0;
    std::string result = fmt::format("{}", pow2_mantissa);
    EXPECT_EQ(result, "1");
    
    // Test with mantissa that is close to 1
    double close_to_one = 1.0000000000000002;
    result = fmt::format("{:.16f}", close_to_one);
    EXPECT_EQ(result, "1.0000000000000002");
    
    // Test with mantissa that is close to 10
    double close_to_ten = 9.999999999999999;
    result = fmt::format("{:.15f}", close_to_ten);
    EXPECT_EQ(result, "10.000000000000000");
}