#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <string>
#include <climits>

// Test the printf formatter's handling of integer formatting with various specifiers
class PrintfIntegerFormattingTest : public ::testing::Test {
protected:
    // Helper to format using printf-style syntax
    std::string format(const std::string& fmt, int value) {
        return fmt::sprintf(fmt, value);
    }
};

// Test basic integer formatting with default specifiers
TEST_F(PrintfIntegerFormattingTest, BasicIntegerFormatting) {
    EXPECT_EQ(format("%d", 42), "42");
    EXPECT_EQ(format("%i", 42), "42");
    EXPECT_EQ(format("%u", 42), "42");
}

// Test integer formatting with width specifier
TEST_F(PrintfIntegerFormattingTest, WidthSpecifier) {
    EXPECT_EQ(format("%5d", 42), "   42");
    EXPECT_EQ(format("%-5d", 42), "42   ");
    EXPECT_EQ(format("%05d", 42), "00042");
}

// Test integer formatting with precision specifier
TEST_F(PrintfIntegerFormattingTest, PrecisionSpecifier) {
    EXPECT_EQ(format("%.5d", 42), "00042");
    EXPECT_EQ(format("%.0d", 0), "");
    EXPECT_EQ(format("%.5d", -42), "-00042");
}

// Test integer formatting with sign specifiers
TEST_F(PrintfIntegerFormattingTest, SignSpecifiers) {
    EXPECT_EQ(format("%+d", 42), "+42");
    EXPECT_EQ(format("%+d", -42), "-42");
    EXPECT_EQ(format("% d", 42), " 42");
    EXPECT_EQ(format("% d", -42), "-42");
}

// Test integer formatting with alternate form (#)
TEST_F(PrintfIntegerFormattingTest, AlternateForm) {
    EXPECT_EQ(format("%#x", 255), "0xff");
    EXPECT_EQ(format("%#X", 255), "0XFF");
    EXPECT_EQ(format("%#o", 255), "0377");
    EXPECT_EQ(format("%#x", 0), "0");
}

// Test integer formatting with different bases
TEST_F(PrintfIntegerFormattingTest, DifferentBases) {
    EXPECT_EQ(format("%x", 255), "ff");
    EXPECT_EQ(format("%X", 255), "FF");
    EXPECT_EQ(format("%o", 255), "377");
    EXPECT_EQ(format("%b", 255), "11111111");
}

// Test edge cases: zero
TEST_F(PrintfIntegerFormattingTest, ZeroValue) {
    EXPECT_EQ(format("%d", 0), "0");
    EXPECT_EQ(format("%+d", 0), "+0");
    EXPECT_EQ(format("% d", 0), " 0");
    EXPECT_EQ(format("%05d", 0), "00000");
    EXPECT_EQ(format("%.0d", 0), "");
}

// Test edge cases: maximum and minimum integer values
TEST_F(PrintfIntegerFormattingTest, ExtremeValues) {
    EXPECT_EQ(format("%d", INT_MAX), "2147483647");
    EXPECT_EQ(format("%d", INT_MIN), "-2147483648");
    EXPECT_EQ(format("%u", UINT_MAX), "4294967295");
}

// Test edge cases: negative numbers
TEST_F(PrintfIntegerFormattingTest, NegativeNumbers) {
    EXPECT_EQ(format("%d", -1), "-1");
    EXPECT_EQ(format("%+d", -1), "-1");
    EXPECT_EQ(format("% d", -1), "-1");
    EXPECT_EQ(format("%05d", -1), "-0001");
    EXPECT_EQ(format("%-5d", -1), "-1   ");
}

// Test combination of multiple specifiers
TEST_F(PrintfIntegerFormattingTest, CombinedSpecifiers) {
    EXPECT_EQ(format("%+05d", 42), "+0042");
    EXPECT_EQ(format("%-+5d", 42), "+42  ");
    EXPECT_EQ(format("%#010x", 255), "0x000000ff");
    EXPECT_EQ(format("% 05d", -42), "-0042");
}

// Test that alt flag is properly reset for non-octal/hex formats
TEST_F(PrintfIntegerFormattingTest, AltFlagReset) {
    EXPECT_EQ(format("%#d", 42), "42");  // # has no effect on %d
    EXPECT_EQ(format("%#u", 42), "42");  // # has no effect on %u
}

// Test that sign is properly reset for unsigned formats
TEST_F(PrintfIntegerFormattingTest, SignResetForUnsigned) {
    EXPECT_EQ(format("%+u", 42), "42");  // + has no effect on %u
    EXPECT_EQ(format("% u", 42), "42");  // space has no effect on %u
}

// Test alignment behavior
TEST_F(PrintfIntegerFormattingTest, Alignment) {
    EXPECT_EQ(format("%-10d", 42), "42        ");
    EXPECT_EQ(format("%10d", 42), "        42");
    EXPECT_EQ(format("%-+10d", 42), "+42       ");
    EXPECT_EQ(format("%+10d", 42), "       +42");
}

// Test with long long integers (if supported)
TEST_F(PrintfIntegerFormattingTest, LongLongIntegers) {
    EXPECT_EQ(fmt::sprintf("%lld", 1234567890123LL), "1234567890123");
    EXPECT_EQ(fmt::sprintf("%llu", 1234567890123ULL), "1234567890123");
}

// Test that the function handles the conversion from char to int properly
TEST_F(PrintfIntegerFormattingTest, CharToIntConversion) {
    EXPECT_EQ(fmt::sprintf("%d", 'A'), "65");
    EXPECT_EQ(fmt::sprintf("%c", 65), "A");
}

// Test error handling: invalid format string should not crash
TEST_F(PrintfIntegerFormattingTest, InvalidFormatString) {
    EXPECT_THROW(fmt::sprintf("%", 42), fmt::format_error);
    EXPECT_THROW(fmt::sprintf("%*d", 42), fmt::format_error);
}