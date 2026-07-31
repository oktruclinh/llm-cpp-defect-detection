#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <string>
#include <vector>
#include <cmath>

// Test the printf-like formatting through the public API
// The focal function is part of the printf format spec handling in fmt::printf

class PrintfFormatTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

// Test basic integer formatting with printf-style
TEST_F(PrintfFormatTest, BasicIntegerFormatting) {
    EXPECT_EQ(fmt::sprintf("%d", 42), "42");
    EXPECT_EQ(fmt::sprintf("%i", -123), "-123");
    EXPECT_EQ(fmt::sprintf("%u", 0), "0");
}

// Test width and precision specifiers
TEST_F(PrintfFormatTest, WidthAndPrecision) {
    EXPECT_EQ(fmt::sprintf("%5d", 42), "   42");
    EXPECT_EQ(fmt::sprintf("%-5d", 42), "42   ");
    EXPECT_EQ(fmt::sprintf("%05d", 42), "00042");
    EXPECT_EQ(fmt::sprintf("%.5d", 42), "00042");
    EXPECT_EQ(fmt::sprintf("%8.5d", 42), "   00042");
}

// Test string formatting
TEST_F(PrintfFormatTest, StringFormatting) {
    EXPECT_EQ(fmt::sprintf("%s", "hello"), "hello");
    EXPECT_EQ(fmt::sprintf("%10s", "hello"), "     hello");
    EXPECT_EQ(fmt::sprintf("%-10s", "hello"), "hello     ");
    EXPECT_EQ(fmt::sprintf("%.3s", "hello"), "hel");
    EXPECT_EQ(fmt::sprintf("%10.3s", "hello"), "       hel");
}

// Test character formatting
TEST_F(PrintfFormatTest, CharFormatting) {
    EXPECT_EQ(fmt::sprintf("%c", 'A'), "A");
    EXPECT_EQ(fmt::sprintf("%c", 65), "A");
    EXPECT_EQ(fmt::sprintf("%5c", 'Z'), "    Z");
}

// Test floating-point formatting
TEST_F(PrintfFormatTest, FloatFormatting) {
    EXPECT_EQ(fmt::sprintf("%f", 3.14), "3.140000");
    EXPECT_EQ(fmt::sprintf("%.2f", 3.14159), "3.14");
    EXPECT_EQ(fmt::sprintf("%8.2f", 3.14), "    3.14");
    EXPECT_EQ(fmt::sprintf("%-8.2f", 3.14), "3.14    ");
    EXPECT_EQ(fmt::sprintf("%e", 3.14), "3.140000e+00");
    EXPECT_EQ(fmt::sprintf("%g", 3.14), "3.14");
}

// Test hex and octal formatting
TEST_F(PrintfFormatTest, HexOctalFormatting) {
    EXPECT_EQ(fmt::sprintf("%x", 255), "ff");
    EXPECT_EQ(fmt::sprintf("%X", 255), "FF");
    EXPECT_EQ(fmt::sprintf("%#x", 255), "0xff");
    EXPECT_EQ(fmt::sprintf("%#X", 255), "0XFF");
    EXPECT_EQ(fmt::sprintf("%o", 8), "10");
    EXPECT_EQ(fmt::sprintf("%#o", 8), "010");
}

// Test zero flag behavior (the focal function handles this)
TEST_F(PrintfFormatTest, ZeroFlagBehavior) {
    // Zero flag should work for numeric types
    EXPECT_EQ(fmt::sprintf("%05d", 42), "00042");
    EXPECT_EQ(fmt::sprintf("%05d", -42), "-0042");
    EXPECT_EQ(fmt::sprintf("%08.2f", 3.14), "00003.14");
    
    // Zero flag should be ignored for non-numeric types (align::numeric -> align::right)
    EXPECT_EQ(fmt::sprintf("%05s", "hi"), "    hi");  // Not "000hi"
    EXPECT_EQ(fmt::sprintf("%05c", 'A'), "    A");    // Not "0000A"
}

// Test sign handling
TEST_F(PrintfFormatTest, SignHandling) {
    EXPECT_EQ(fmt::sprintf("%+d", 42), "+42");
    EXPECT_EQ(fmt::sprintf("%+d", -42), "-42");
    EXPECT_EQ(fmt::sprintf("% d", 42), " 42");
    EXPECT_EQ(fmt::sprintf("% d", -42), "-42");
    EXPECT_EQ(fmt::sprintf("%+05d", 42), "+0042");
    EXPECT_EQ(fmt::sprintf("% 05d", 42), " 0042");
}

// Test alignment behavior (the focal function handles align::numeric)
TEST_F(PrintfFormatTest, AlignmentBehavior) {
    // Default alignment for numbers is right
    EXPECT_EQ(fmt::sprintf("%10d", 42), "        42");
    EXPECT_EQ(fmt::sprintf("%-10d", 42), "42        ");
    
    // Default alignment for strings is left
    EXPECT_EQ(fmt::sprintf("%10s", "hi"), "        hi");
    EXPECT_EQ(fmt::sprintf("%-10s", "hi"), "hi        ");
}

// Test edge cases with empty/zero values
TEST_F(PrintfFormatTest, EdgeCases) {
    EXPECT_EQ(fmt::sprintf("%d", 0), "0");
    EXPECT_EQ(fmt::sprintf("%s", ""), "");
    EXPECT_EQ(fmt::sprintf("%.0f", 0.0), "0");
    EXPECT_EQ(fmt::sprintf("%.0f", 1.5), "2");  // Rounding
    EXPECT_EQ(fmt::sprintf("%.0f", 1.4), "1");
}

// Test multiple arguments
TEST_F(PrintfFormatTest, MultipleArguments) {
    EXPECT_EQ(fmt::sprintf("%d %s %f", 42, "hello", 3.14), "42 hello 3.140000");
    EXPECT_EQ(fmt::sprintf("%d %d %d", 1, 2, 3), "1 2 3");
}

// Test positional arguments
TEST_F(PrintfFormatTest, PositionalArguments) {
    EXPECT_EQ(fmt::sprintf("%1$d %1$d", 42), "42 42");
    EXPECT_EQ(fmt::sprintf("%2$d %1$d", 10, 20), "20 10");
}

// Test large numbers and boundaries
TEST_F(PrintfFormatTest, BoundaryValues) {
    EXPECT_EQ(fmt::sprintf("%d", std::numeric_limits<int>::max()), "2147483647");
    EXPECT_EQ(fmt::sprintf("%d", std::numeric_limits<int>::min()), "-2147483648");
    EXPECT_EQ(fmt::sprintf("%u", std::numeric_limits<unsigned>::max()), "4294967295");
    EXPECT_EQ(fmt::sprintf("%ld", std::numeric_limits<long>::max()), "9223372036854775807");
}

// Test that the focal function's behavior for non-numeric types with zero flag is correct
TEST_F(PrintfFormatTest, NonNumericZeroFlag) {
    // The focal function changes align::numeric to align::right for non-numeric types
    // This means zero flag should be ignored for strings and chars
    EXPECT_EQ(fmt::sprintf("%010s", "test"), "      test");  // Not "000000test"
    EXPECT_EQ(fmt::sprintf("%010c", 'x'), "         x");    // Not "00000000x"
    
    // But zero flag should still work for numeric types
    EXPECT_EQ(fmt::sprintf("%010d", 42), "0000000042");
    EXPECT_EQ(fmt::sprintf("%010.3f", 3.14), "000003.140");
}

// Test alt flag behavior
TEST_F(PrintfFormatTest, AltFlag) {
    EXPECT_EQ(fmt::sprintf("%#x", 0), "0");
    EXPECT_EQ(fmt::sprintf("%#X", 0), "0");
    EXPECT_EQ(fmt::sprintf("%#o", 0), "0");
    EXPECT_EQ(fmt::sprintf("%#.0f", 3.0), "3.");
    EXPECT_EQ(fmt::sprintf("%#g", 3.0), "3.00000");
}

// Test that the focal function properly handles the sign reset for non-numeric types
TEST_F(PrintfFormatTest, SignResetForNonNumeric) {
    // Sign flags should be ignored for strings
    EXPECT_EQ(fmt::sprintf("%+s", "hello"), "hello");
    EXPECT_EQ(fmt::sprintf("% s", "hello"), "hello");
    
    // Sign flags should work for numbers
    EXPECT_EQ(fmt::sprintf("%+d", 42), "+42");
    EXPECT_EQ(fmt::sprintf("% d", 42), " 42");
}