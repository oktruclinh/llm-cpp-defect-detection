#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>

// Test write(long double) via fmt::format and fmt::print to string
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

    // Very small number
    result = fmt::format("{}", 1e-10L);
    EXPECT_EQ(result, "0.0000000001");

    // Very large number
    result = fmt::format("{}", 1e20L);
    EXPECT_EQ(result, "100000000000000000000.0");
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

TEST(WriteLongDoubleTest, MinAndMaxValues) {
    // Minimum positive normalized value
    long double min_val = std::numeric_limits<long double>::min();
    std::string result = fmt::format("{}", min_val);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0.0");

    // Maximum finite value
    long double max_val = std::numeric_limits<long double>::max();
    result = fmt::format("{}", max_val);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
}

TEST(WriteLongDoubleTest, DenormalizedValue) {
    // Denormalized number (smallest positive)
    long double denorm = std::numeric_limits<long double>::denorm_min();
    std::string result = fmt::format("{}", denorm);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0.0");
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

TEST(WriteCharTest, SpecialCharacters) {
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

TEST(WriteCharTest, NonPrintableCharacters) {
    // Character with value 1 (SOH)
    std::string result = fmt::format("{}", static_cast<char>(1));
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], static_cast<char>(1));

    // Character with value 127 (DEL)
    result = fmt::format("{}", static_cast<char>(127));
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], static_cast<char>(127));
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

TEST(WriteWcharTest, WideSpecialCharacters) {
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

TEST(WriteWcharTest, UnicodeCharacters) {
    // Unicode character (Latin capital letter A with acute)
    std::wstring result = fmt::format(L"{}", L'\u00C1');
    EXPECT_EQ(result, L"\u00C1");

    // Unicode character (CJK ideograph)
    result = fmt::format(L"{}", L'\u4E2D');
    EXPECT_EQ(result, L"\u4E2D");
}

// Test that write(char) and write(long double) work together in format string
TEST(WriteMixedTest, CharAndDoubleInSameFormat) {
    std::string result = fmt::format("{}{}", 'x', 3.14L);
    EXPECT_EQ(result, "x3.14");
}

// Test that write(char) works with format specifiers
TEST(WriteCharWithFormatSpecifiers, FillAndAlign) {
    std::string result = fmt::format("{:*>5}", 'A');
    EXPECT_EQ(result, "****A");

    result = fmt::format("{:*<5}", 'A');
    EXPECT_EQ(result, "A****");

    result = fmt::format("{:*^5}", 'A');
    EXPECT_EQ(result, "**A**");
}

// Test that write(long double) works with format specifiers
TEST(WriteLongDoubleWithFormatSpecifiers, PrecisionAndWidth) {
    std::string result = fmt::format("{:.2f}", 3.14159L);
    EXPECT_EQ(result, "3.14");

    result = fmt::format("{:10.3f}", 2.71828L);
    EXPECT_EQ(result, "     2.718");

    result = fmt::format("{:+06.2f}", -1.5L);
    EXPECT_EQ(result, "-001.50");
}