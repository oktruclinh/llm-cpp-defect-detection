#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>

// Test write(long double) via fmt::format and fmt::print-like functions
TEST(WriteLongDoubleTest, BasicValues) {
    // Normal positive value
    std::string result = fmt::format("{}", 3.14159265358979323846L);
    EXPECT_EQ(result, "3.141592653589793");

    // Negative value
    result = fmt::format("{}", -2.71828182845904523536L);
    EXPECT_EQ(result, "-2.718281828459045");

    // Zero
    result = fmt::format("{}", 0.0L);
    EXPECT_EQ(result, "0.0");

    // Negative zero
    result = fmt::format("{}", -0.0L);
    EXPECT_EQ(result, "-0.0");
}

TEST(WriteLongDoubleTest, InfinityAndNaN) {
    // Positive infinity
    std::string result = fmt::format("{}", std::numeric_limits<long double>::infinity());
    EXPECT_EQ(result, "inf");

    // Negative infinity
    result = fmt::format("{}", -std::numeric_limits<long double>::infinity());
    EXPECT_EQ(result, "-inf");

    // NaN
    result = fmt::format("{}", std::numeric_limits<long double>::quiet_NaN());
    EXPECT_EQ(result, "nan");
}

TEST(WriteLongDoubleTest, Extremes) {
    // Very small positive number (denormal)
    long double tiny = std::numeric_limits<long double>::denorm_min();
    std::string result = fmt::format("{}", tiny);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0.0"); // denorm min is not zero

    // Very large number
    long double huge = std::numeric_limits<long double>::max();
    result = fmt::format("{}", huge);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
}

TEST(WriteLongDoubleTest, FormatSpecs) {
    // With precision
    std::string result = fmt::format("{:.5f}", 1.23456789L);
    EXPECT_EQ(result, "1.23457");

    // With width and alignment
    result = fmt::format("{:>10.3f}", 42.0L);
    EXPECT_EQ(result, "    42.000");

    // Scientific notation
    result = fmt::format("{:.3e}", 123456.789L);
    EXPECT_EQ(result, "1.235e+05");
}

// Test write(char) via fmt::format
TEST(WriteCharTest, BasicChar) {
    std::string result = fmt::format("{}", 'A');
    EXPECT_EQ(result, "A");

    result = fmt::format("{}", 'z');
    EXPECT_EQ(result, "z");

    result = fmt::format("{}", '0');
    EXPECT_EQ(result, "0");
}

TEST(WriteCharTest, SpecialChars) {
    // Null character
    std::string result = fmt::format("{}", '\0');
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], '\0');

    // Newline
    result = fmt::format("{}", '\n');
    EXPECT_EQ(result, "\n");

    // Tab
    result = fmt::format("{}", '\t');
    EXPECT_EQ(result, "\t");
}

TEST(WriteCharTest, CharWithFormatSpecs) {
    // Width
    std::string result = fmt::format("{:5}", 'X');
    EXPECT_EQ(result, "X    ");

    // Left align
    result = fmt::format("{:<5}", 'Y');
    EXPECT_EQ(result, "Y    ");

    // Right align
    result = fmt::format("{:>5}", 'Z');
    EXPECT_EQ(result, "    Z");
}

// Test write(wchar_t) via fmt::format with wide strings
TEST(WriteWcharTest, BasicWchar) {
    std::wstring result = fmt::format(L"{}", L'A');
    EXPECT_EQ(result, L"A");

    result = fmt::format(L"{}", L'z');
    EXPECT_EQ(result, L"z");

    result = fmt::format(L"{}", L'0');
    EXPECT_EQ(result, L"0");
}

TEST(WriteWcharTest, SpecialWchars) {
    // Null wide character
    std::wstring result = fmt::format(L"{}", L'\0');
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], L'\0');

    // Newline
    result = fmt::format(L"{}", L'\n');
    EXPECT_EQ(result, L"\n");

    // Tab
    result = fmt::format(L"{}", L'\t');
    EXPECT_EQ(result, L"\t");
}

TEST(WriteWcharTest, WcharWithFormatSpecs) {
    // Width
    std::wstring result = fmt::format(L"{:5}", L'X');
    EXPECT_EQ(result, L"X    ");

    // Left align
    result = fmt::format(L"{:<5}", L'Y');
    EXPECT_EQ(result, L"Y    ");

    // Right align
    result = fmt::format(L"{:>5}", L'Z');
    EXPECT_EQ(result, L"    Z");
}

// Edge case: empty format string with char
TEST(WriteCharTest, EmptyFormatString) {
    std::string result = fmt::format("{}", ' ');
    EXPECT_EQ(result, " ");
}

// Edge case: combining char and long double in same format
TEST(WriteCombinedTest, CharAndDouble) {
    std::string result = fmt::format("char={}, double={}", 'c', 3.14L);
    EXPECT_EQ(result, "char=c, double=3.14");
}