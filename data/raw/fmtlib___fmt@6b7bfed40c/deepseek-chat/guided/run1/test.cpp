#include <gtest/gtest.h>
#include <fmt/format.h>
#include <cmath>
#include <limits>
#include <string>

// Test formatting of floating-point numbers with various exponents and precision
// to exercise the focal function's behavior through the public API.

TEST(FloatFormatTest, NormalCases) {
    // Test basic positive exponent formatting
    EXPECT_EQ(fmt::format("{:.0f}", 1e6), "1000000");
    EXPECT_EQ(fmt::format("{:.0f}", 1e9), "1000000000");
    
    // Test negative exponent formatting
    EXPECT_EQ(fmt::format("{:.10f}", 1e-5), "0.0000100000");
    EXPECT_EQ(fmt::format("{:.10f}", 1e-10), "0.0000000001");
    
    // Test mixed exponents
    EXPECT_EQ(fmt::format("{:.2f}", 123.456), "123.46");
    EXPECT_EQ(fmt::format("{:.2f}", 0.00123), "0.00");
}

TEST(FloatFormatTest, EdgeCases) {
    // Zero
    EXPECT_EQ(fmt::format("{:.0f}", 0.0), "0");
    EXPECT_EQ(fmt::format("{:.5f}", 0.0), "0.00000");
    
    // Very small numbers (negative exponent)
    EXPECT_EQ(fmt::format("{:.0f}", 1e-10), "0");
    EXPECT_EQ(fmt::format("{:.10f}", 1e-10), "0.0000000001");
    EXPECT_EQ(fmt::format("{:.15f}", 1e-10), "0.000000000100000");
    
    // Very large numbers (positive exponent)
    EXPECT_EQ(fmt::format("{:.0f}", 1e15), "1000000000000000");
    EXPECT_EQ(fmt::format("{:.0f}", 1e20), "100000000000000000000");
    
    // Numbers near 1
    EXPECT_EQ(fmt::format("{:.0f}", 0.9999), "1");
    EXPECT_EQ(fmt::format("{:.0f}", 1.0001), "1");
    EXPECT_EQ(fmt::format("{:.4f}", 0.9999), "0.9999");
    EXPECT_EQ(fmt::format("{:.4f}", 1.0001), "1.0001");
}

TEST(FloatFormatTest, PrecisionBoundaries) {
    // Maximum precision
    double pi = 3.14159265358979323846;
    EXPECT_EQ(fmt::format("{:.15f}", pi), "3.141592653589793");
    EXPECT_EQ(fmt::format("{:.10f}", pi), "3.1415926536");
    
    // Minimum precision (0)
    EXPECT_EQ(fmt::format("{:.0f}", 3.14159), "3");
    EXPECT_EQ(fmt::format("{:.0f}", 9.999), "10");
    
    // High precision with small numbers
    EXPECT_EQ(fmt::format("{:.20f}", 1e-5), "0.00001000000000000000");
    EXPECT_EQ(fmt::format("{:.20f}", 1e-10), "0.00000000010000000000");
}

TEST(FloatFormatTest, NegativeNumbers) {
    // Negative numbers with various exponents
    EXPECT_EQ(fmt::format("{:.2f}", -123.456), "-123.46");
    EXPECT_EQ(fmt::format("{:.10f}", -1e-5), "-0.0000100000");
    EXPECT_EQ(fmt::format("{:.0f}", -1e6), "-1000000");
    EXPECT_EQ(fmt::format("{:.0f}", -1e-10), "-0");
}

TEST(FloatFormatTest, SubnormalAndSpecialValues) {
    // Test formatting of very small subnormal numbers
    double smallest_normal = std::numeric_limits<double>::min();
    EXPECT_EQ(fmt::format("{:.0f}", smallest_normal), "0");
    EXPECT_EQ(fmt::format("{:.100f}", smallest_normal), 
              fmt::format("{:.100f}", smallest_normal)); // Just verify it doesn't crash
    
    // Test denormalized numbers (very small)
    double denorm_min = std::numeric_limits<double>::denorm_min();
    EXPECT_EQ(fmt::format("{:.0f}", denorm_min), "0");
    
    // Test numbers that exercise the exponent boundary
    EXPECT_EQ(fmt::format("{:.0f}", 1e-1), "0");
    EXPECT_EQ(fmt::format("{:.1f}", 1e-1), "0.1");
    EXPECT_EQ(fmt::format("{:.0f}", 1e-2), "0");
    EXPECT_EQ(fmt::format("{:.2f}", 1e-2), "0.01");
}

TEST(FloatFormatTest, RoundingBehavior) {
    // Test rounding at various decimal places
    EXPECT_EQ(fmt::format("{:.0f}", 0.5), "0");  // Round half to even
    EXPECT_EQ(fmt::format("{:.0f}", 1.5), "2");  // Round half to even
    EXPECT_EQ(fmt::format("{:.0f}", 2.5), "2");  // Round half to even
    EXPECT_EQ(fmt::format("{:.0f}", 3.5), "4");  // Round half to even
    
    // Test rounding with negative exponents
    EXPECT_EQ(fmt::format("{:.5f}", 0.000015), "0.00002");
    EXPECT_EQ(fmt::format("{:.5f}", 0.000014), "0.00001");
    
    // Test rounding up at exponent boundary
    EXPECT_EQ(fmt::format("{:.0f}", 0.9999), "1");
    EXPECT_EQ(fmt::format("{:.0f}", 9.9999), "10");
}

TEST(FloatFormatTest, LargeExponentValues) {
    // Test numbers with very large positive exponents
    EXPECT_EQ(fmt::format("{:.0f}", 1e10), "10000000000");
    EXPECT_EQ(fmt::format("{:.0f}", 1e12), "1000000000000");
    
    // Test numbers with very large negative exponents
    EXPECT_EQ(fmt::format("{:.12f}", 1e-12), "0.000000000001");
    EXPECT_EQ(fmt::format("{:.15f}", 1e-12), "0.000000000001000");
    
    // Test that formatting doesn't lose precision for moderate exponents
    EXPECT_EQ(fmt::format("{:.1f}", 1.234e5), "123400.0");
    EXPECT_EQ(fmt::format("{:.1f}", 1.234e-5), "0.0");
    EXPECT_EQ(fmt::format("{:.10f}", 1.234e-5), "0.0000123400");
}