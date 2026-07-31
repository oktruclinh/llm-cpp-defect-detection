#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>

// Test write(long double) via fmt::format and fmt::print to string
TEST(WriteLongDoubleTest, BasicValues) {
    // Normal positive value
    std::string result = fmt::format("{:Lf}", 3.14159265358979323846L);
    EXPECT_EQ(result, "3.141593");

    // Negative value
    result = fmt::format("{:Lf}", -2.718281828459045L);
    EXPECT_EQ(result, "-2.718282");

    // Zero
    result = fmt::format("{:Lf}", 0.0L);
    EXPECT_EQ(result, "0.000000");

    // Very large value
    result = fmt::format("{:Le}", 1.0e100L);
    EXPECT_EQ(result, "1.000000e+100");

    // Very small value (subnormal)
    result = fmt::format("{:Le}", 1.0e-100L);
    EXPECT_EQ(result, "1.000000e-100");
}

TEST(WriteLongDoubleTest, SpecialValues) {
    // Infinity
    std::string result = fmt::format("{}", std::numeric_limits<long double>::infinity());
    EXPECT_EQ(result, "inf");

    // Negative infinity
    result = fmt::format("{}", -std::numeric_limits<long double>::infinity());
    EXPECT_EQ(result, "-inf");

    // NaN
    result = fmt::format("{}", std::numeric_limits<long double>::quiet_NaN());
    EXPECT_EQ(result, "nan");
}

TEST(WriteLongDoubleTest, FormatSpecs) {
    // Fixed precision
    std::string result = fmt::format("{:.5f}", 1.23456789L);
    EXPECT_EQ(result, "1.23457");

    // Scientific notation
    result = fmt::format("{:.3e}", 123456.789L);
    EXPECT_EQ(result, "1.235e+05");

    // General format
    result = fmt::format("{:.4g}", 123456.789L);
    EXPECT_EQ(result, "1.235e+05");

    // Width and alignment
    result = fmt::format("{:10.3f}", 3.14L);
    EXPECT_EQ(result, "    3.140");
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

TEST(WriteCharTest, FormatSpecs) {
    // Width and fill
    std::string result = fmt::format("{:5}", 'X');
    EXPECT_EQ(result, "X    ");

    result = fmt::format("{:*>5}", 'Y');
    EXPECT_EQ(result, "****Y");

    result = fmt::format("{:*^5}", 'Z');
    EXPECT_EQ(result, "**Z**");
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

TEST(WriteWcharTest, FormatSpecs) {
    // Width and fill
    std::wstring result = fmt::format(L"{:5}", L'X');
    EXPECT_EQ(result, L"X    ");

    result = fmt::format(L"{:*>5}", L'Y');
    EXPECT_EQ(result, L"****Y");

    result = fmt::format(L"{:*^5}", L'Z');
    EXPECT_EQ(result, L"**Z**");
}

// Edge cases for write(long double)
TEST(WriteLongDoubleEdgeTest, MinMaxValues) {
    // Minimum positive normal value
    long double min_val = std::numeric_limits<long double>::min();
    std::string result = fmt::format("{:Le}", min_val);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0.000000e+00");

    // Maximum finite value
    long double max_val = std::numeric_limits<long double>::max();
    result = fmt::format("{:Le}", max_val);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");

    // Denormalized minimum
    long double denorm_min = std::numeric_limits<long double>::denorm_min();
    result = fmt::format("{:Le}", denorm_min);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0.000000e+00");
}

TEST(WriteLongDoubleEdgeTest, PrecisionBoundaries) {
    // Very high precision
    std::string result = fmt::format("{:.50f}", 1.0L / 3.0L);
    EXPECT_EQ(result.substr(0, 2), "0.");
    EXPECT_EQ(result.size(), 53); // "0." + 50 digits + possible rounding

    // Zero precision
    result = fmt::format("{:.0f}", 123.456L);
    EXPECT_EQ(result, "123");

    // Negative precision (should be treated as zero)
    result = fmt::format("{:.-1f}", 123.456L);
    EXPECT_EQ(result, "123");
}

// Edge cases for write(char)
TEST(WriteCharEdgeTest, AllPrintableAscii) {
    for (char c = 32; c < 127; ++c) {
        std::string result = fmt::format("{}", c);
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result[0], c);
    }
}

TEST(WriteCharEdgeTest, FormatWithEmptyString) {
    std::string result = fmt::format("{:1}", ' ');
    EXPECT_EQ(result, " ");
    EXPECT_EQ(result.size(), 1);
}

// Edge cases for write(wchar_t)
TEST(WriteWcharEdgeTest, AllBasicLatin) {
    for (wchar_t c = 32; c < 127; ++c) {
        std::wstring result = fmt::format(L"{}", c);
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result[0], c);
    }
}

TEST(WriteWcharEdgeTest, WideCharBoundaries) {
    // Maximum wchar_t value (implementation-defined, but test typical)
    wchar_t max_wchar = static_cast<wchar_t>(-1);
    std::wstring result = fmt::format(L"{}", max_wchar);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], max_wchar);
}