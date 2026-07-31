#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <string>
#include <climits>

// Test the printf formatter's handling of integer formatting with various specifiers
// This tests the focal function's behavior through the public printf API

class PrintfIntegerTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Test basic integer formatting
TEST_F(PrintfIntegerTest, BasicInteger) {
    EXPECT_EQ(fmt::sprintf("%d", 42), "42");
    EXPECT_EQ(fmt::sprintf("%i", 42), "42");
    EXPECT_EQ(fmt::sprintf("%u", 42), "42");
}

// Test zero value
TEST_F(PrintfIntegerTest, ZeroValue) {
    EXPECT_EQ(fmt::sprintf("%d", 0), "0");
    EXPECT_EQ(fmt::sprintf("%u", 0), "0");
    EXPECT_EQ(fmt::sprintf("%x", 0), "0");
    EXPECT_EQ(fmt::sprintf("%X", 0), "0");
}

// Test negative numbers
TEST_F(PrintfIntegerTest, NegativeNumbers) {
    EXPECT_EQ(fmt::sprintf("%d", -42), "-42");
    EXPECT_EQ(fmt::sprintf("%i", -42), "-42");
    EXPECT_EQ(fmt::sprintf("%d", INT_MIN), "-2147483648");
}

// Test unsigned formatting of negative values (should wrap)
TEST_F(PrintfIntegerTest, NegativeAsUnsigned) {
    EXPECT_EQ(fmt::sprintf("%u", -1), "4294967295");
    EXPECT_EQ(fmt::sprintf("%u", -42), "4294967254");
}

// Test hexadecimal formatting
TEST_F(PrintfIntegerTest, Hexadecimal) {
    EXPECT_EQ(fmt::sprintf("%x", 255), "ff");
    EXPECT_EQ(fmt::sprintf("%X", 255), "FF");
    EXPECT_EQ(fmt::sprintf("%x", 0xDEAD), "dead");
    EXPECT_EQ(fmt::sprintf("%X", 0xDEAD), "DEAD");
}

// Test octal formatting
TEST_F(PrintfIntegerTest, Octal) {
    EXPECT_EQ(fmt::sprintf("%o", 8), "10");
    EXPECT_EQ(fmt::sprintf("%o", 64), "100");
    EXPECT_EQ(fmt::sprintf("%o", 0), "0");
}

// Test width specifier
TEST_F(PrintfIntegerTest, WidthSpecifier) {
    EXPECT_EQ(fmt::sprintf("%5d", 42), "   42");
    EXPECT_EQ(fmt::sprintf("%5d", -42), "  -42");
    EXPECT_EQ(fmt::sprintf("%5u", 42), "   42");
    EXPECT_EQ(fmt::sprintf("%5x", 255), "   ff");
}

// Test precision specifier
TEST_F(PrintfIntegerTest, PrecisionSpecifier) {
    EXPECT_EQ(fmt::sprintf("%.5d", 42), "00042");
    EXPECT_EQ(fmt::sprintf("%.5d", -42), "-00042");
    EXPECT_EQ(fmt::sprintf("%.5u", 42), "00042");
    EXPECT_EQ(fmt::sprintf("%.5x", 255), "000ff");
}

// Test width and precision together
TEST_F(PrintfIntegerTest, WidthAndPrecision) {
    EXPECT_EQ(fmt::sprintf("%8.5d", 42), "   00042");
    EXPECT_EQ(fmt::sprintf("%8.5d", -42), "  -00042");
    EXPECT_EQ(fmt::sprintf("%-8.5d", 42), "00042   ");
}

// Test left alignment
TEST_F(PrintfIntegerTest, LeftAlignment) {
    EXPECT_EQ(fmt::sprintf("%-5d", 42), "42   ");
    EXPECT_EQ(fmt::sprintf("%-5d", -42), "-42  ");
    EXPECT_EQ(fmt::sprintf("%-10x", 255), "ff        ");
}

// Test zero padding
TEST_F(PrintfIntegerTest, ZeroPadding) {
    EXPECT_EQ(fmt::sprintf("%05d", 42), "00042");
    EXPECT_EQ(fmt::sprintf("%05d", -42), "-0042");
    EXPECT_EQ(fmt::sprintf("%05u", 42), "00042");
    EXPECT_EQ(fmt::sprintf("%05x", 255), "000ff");
}

// Test sign specifiers
TEST_F(PrintfIntegerTest, SignSpecifiers) {
    EXPECT_EQ(fmt::sprintf("%+d", 42), "+42");
    EXPECT_EQ(fmt::sprintf("%+d", -42), "-42");
    EXPECT_EQ(fmt::sprintf("% d", 42), " 42");
    EXPECT_EQ(fmt::sprintf("% d", -42), "-42");
}

// Test alternate form (#)
TEST_F(PrintfIntegerTest, AlternateForm) {
    EXPECT_EQ(fmt::sprintf("%#x", 255), "0xff");
    EXPECT_EQ(fmt::sprintf("%#X", 255), "0XFF");
    EXPECT_EQ(fmt::sprintf("%#o", 8), "010");
    EXPECT_EQ(fmt::sprintf("%#o", 0), "0");
}

// Test combination of flags
TEST_F(PrintfIntegerTest, CombinedFlags) {
    EXPECT_EQ(fmt::sprintf("%+05d", 42), "+0042");
    EXPECT_EQ(fmt::sprintf("%-+5d", 42), "+42  ");
    EXPECT_EQ(fmt::sprintf("%#08x", 255), "0x0000ff");
    EXPECT_EQ(fmt::sprintf("%#010X", 255), "0X000000FF");
}

// Test large numbers
TEST_F(PrintfIntegerTest, LargeNumbers) {
    EXPECT_EQ(fmt::sprintf("%d", INT_MAX), "2147483647");
    EXPECT_EQ(fmt::sprintf("%u", UINT_MAX), "4294967295");
    EXPECT_EQ(fmt::sprintf("%x", UINT_MAX), "ffffffff");
}

// Test with variable arguments
TEST_F(PrintfIntegerTest, VariableArguments) {
    EXPECT_EQ(fmt::sprintf("%d %d %d", 1, 2, 3), "1 2 3");
    EXPECT_EQ(fmt::sprintf("%d %x %o", 42, 255, 8), "42 ff 10");
}

// Test with string and integer mix
TEST_F(PrintfIntegerTest, MixedFormatting) {
    EXPECT_EQ(fmt::sprintf("Value: %d", 42), "Value: 42");
    EXPECT_EQ(fmt::sprintf("Hex: %#x, Dec: %d", 255, 42), "Hex: 0xff, Dec: 42");
}

// Test edge case: minimum width larger than number
TEST_F(PrintfIntegerTest, LargeWidth) {
    EXPECT_EQ(fmt::sprintf("%20d", 42), "                  42");
    EXPECT_EQ(fmt::sprintf("%-20d", 42), "42                  ");
}

// Test edge case: precision larger than number
TEST_F(PrintfIntegerTest, LargePrecision) {
    EXPECT_EQ(fmt::sprintf("%.20d", 42), "00000000000000000042");
    EXPECT_EQ(fmt::sprintf("%.20d", -42), "-00000000000000000042");
}

// Test edge case: zero width and precision
TEST_F(PrintfIntegerTest, ZeroWidthAndPrecision) {
    EXPECT_EQ(fmt::sprintf("%0d", 42), "42");
    EXPECT_EQ(fmt::sprintf("%.0d", 0), "");
    EXPECT_EQ(fmt::sprintf("%.0d", 42), "42");
}

// Test that sign::none is properly applied when not specified
TEST_F(PrintfIntegerTest, DefaultSignNone) {
    EXPECT_EQ(fmt::sprintf("%d", 42), "42");
    EXPECT_EQ(fmt::sprintf("%d", -42), "-42");
    // Positive numbers should not have a sign by default
    EXPECT_NE(fmt::sprintf("%d", 42), "+42");
}

// Test that alt=false is default
TEST_F(PrintfIntegerTest, DefaultAltFalse) {
    EXPECT_EQ(fmt::sprintf("%x", 255), "ff");
    EXPECT_NE(fmt::sprintf("%x", 255), "0xff");
}

// Test that align::right is default
TEST_F(PrintfIntegerTest, DefaultAlignRight) {
    EXPECT_EQ(fmt::sprintf("%5d", 42), "   42");
    EXPECT_NE(fmt::sprintf("%5d", 42), "42   ");
}