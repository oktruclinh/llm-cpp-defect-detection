#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/ostream.h> // For fmt::format to ostream, if needed for complex types

// Helper to get the output of fmt::format to a string
template <typename T>
std::string format_value(T value) {
    return fmt::format("{}", value);
}

// Test fixture for common setup if needed, though not strictly necessary for these simple functions
class FmtWriteTest : public ::testing::Test {
protected:
    // You can set up common resources here if multiple tests need them
};

// Test case for write(long double value)
TEST_F(FmtWriteTest, WriteLongDouble) {
    // Normal positive value
    EXPECT_EQ(format_value(123.456L), "123.456");

    // Normal negative value
    EXPECT_EQ(format_value(-123.456L), "-123.456");

    // Zero
    EXPECT_EQ(format_value(0.0L), "0");

    // Smallest positive normal long double (approx)
    // Note: fmt::format might round or use scientific notation for very small/large numbers
    // We test a value that should be representable without scientific notation by default
    EXPECT_EQ(format_value(0.000001L), "0.000001");

    // Large positive value
    EXPECT_EQ(format_value(123456789012345.0L), "123456789012345");

    // Edge case: NaN (Not a Number)
    // The exact string representation of NaN can vary slightly across platforms/compilers (e.g., "nan", "-nan", "NaN")
    // fmtlib typically produces "nan" or "NaN"
    EXPECT_TRUE(format_value(std::numeric_limits<long double>::quiet_NaN()).find("nan") != std::string::npos);

    // Edge case: Positive Infinity
    EXPECT_EQ(format_value(std::numeric_limits<long double>::infinity()), "inf");

    // Edge case: Negative Infinity
    EXPECT_EQ(format_value(-std::numeric_limits<long double>::infinity()), "-inf");

    // Value with many decimal places, testing precision handling (default precision)
    EXPECT_EQ(format_value(1.234567890123456789L), "1.2345678901234567"); // Default precision for long double might vary, but typically around 17-18 digits

    // Test with specific precision formatting
    EXPECT_EQ(fmt::format("{:.2Lf}", 123.456L), "123.46");
    EXPECT_EQ(fmt::format("{:.0Lf}", 123.5L), "124");
    EXPECT_EQ(fmt::format("{:.0Lf}", 123.4L), "123");
}

// Test case for write(char value)
TEST_F(FmtWriteTest, WriteChar) {
    // Normal ASCII character
    EXPECT_EQ(format_value('A'), "A");

    // Space character
    EXPECT_EQ(format_value(' '), " ");

    // Digit character
    EXPECT_EQ(format_value('5'), "5");

    // Special character
    EXPECT_EQ(format_value('$'), "$");

    // Null character (should be printed as a character, not string terminator)
    EXPECT_EQ(format_value('\0'), "\0");

    // Newline character
    EXPECT_EQ(format_value('\n'), "\n");

    // Max char value (assuming signed char, 127)
    EXPECT_EQ(format_value(static_cast<char>(127)), "\x7F");

    // Min char value (assuming signed char, -128)
    // This might be tricky as it's a non-printable character.
    // fmtlib treats char as a character, so it should output the byte value.
    EXPECT_EQ(format_value(static_cast<char>(-128)), "\x80"); // Assuming ASCII/UTF-8 compatible output
}

// Test case for write(wchar_t value)
// This test is conditional on char_type being wchar_t, as per the static_assert.
// We can simulate this by using fmt::wformat.
TEST_F(FmtWriteTest, WriteWChar) {
    // Normal wide character
    EXPECT_EQ(fmt::format(L"{}", L'A'), L"A");

    // Unicode character (e.g., Greek letter Pi)
    EXPECT_EQ(fmt::format(L"{}", L'\x03A0'), L"\x03A0"); // Capital Pi

    // Space character
    EXPECT_EQ(fmt::format(L"{}", L' '), L" ");

    // Null wide character
    EXPECT_EQ(fmt::format(L"{}", L'\0'), L"\0");

    // Max wchar_t value (platform dependent, but a large value)
    // We'll pick a common non-ASCII character that fits in 16-bit or 32-bit wchar_t
    EXPECT_EQ(fmt::format(L"{}", L'\x20AC'), L"\x20AC"); // Euro sign

    // Another common non-ASCII character
    EXPECT_EQ(fmt::format(L"{}", L'\x00E9'), L"\x00E9"); // é
}