#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>
#include <cstddef>

// Test write(long double) through fmt::format
TEST(WriteLongDoubleTest, FormatsLongDoubleCorrectly) {
    // Normal case: positive long double
    std::string result = fmt::format("{}", 3.14159265358979323846L);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "3.14159265358979323846"); // Not exact due to default precision
    EXPECT_TRUE(result.find("3.14") != std::string::npos || result.find("3.14") != std::string::npos);

    // Negative long double
    result = fmt::format("{}", -2.71828182845904523536L);
    EXPECT_TRUE(result[0] == '-');
    EXPECT_TRUE(result.find("2.718") != std::string::npos || result.find("2.718") != std::string::npos);

    // Zero
    result = fmt::format("{}", 0.0L);
    EXPECT_EQ(result, "0");

    // Very small number (denormalized)
    result = fmt::format("{}", std::numeric_limits<long double>::denorm_min());
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0");

    // Very large number
    result = fmt::format("{}", std::numeric_limits<long double>::max());
    EXPECT_FALSE(result.empty());
    EXPECT_TRUE(result.find("e") != std::string::npos || result.find("E") != std::string::npos);

    // Infinity
    result = fmt::format("{}", std::numeric_limits<long double>::infinity());
    EXPECT_EQ(result, "inf");

    // NaN
    result = fmt::format("{}", std::numeric_limits<long double>::quiet_NaN());
    EXPECT_EQ(result, "nan");
}

// Test write(char) through fmt::format
TEST(WriteCharTest, FormatsCharCorrectly) {
    // Normal ASCII character
    std::string result = fmt::format("{}", 'A');
    EXPECT_EQ(result, "A");

    // Lowercase letter
    result = fmt::format("{}", 'z');
    EXPECT_EQ(result, "z");

    // Digit character
    result = fmt::format("{}", '5');
    EXPECT_EQ(result, "5");

    // Space character
    result = fmt::format("{}", ' ');
    EXPECT_EQ(result, " ");

    // Null character
    result = fmt::format("{}", '\0');
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], '\0');

    // Newline character
    result = fmt::format("{}", '\n');
    EXPECT_EQ(result, "\n");

    // Tab character
    result = fmt::format("{}", '\t');
    EXPECT_EQ(result, "\t");

    // Non-printable character
    result = fmt::format("{}", '\x01');
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], '\x01');

    // Maximum char value
    result = fmt::format("{}", static_cast<char>(127));
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 127);
}

// Test write(wchar_t) through fmt::format (wide string context)
TEST(WriteWcharTest, FormatsWcharCorrectly) {
    // Normal wide character
    std::wstring result = fmt::format(L"{}", L'A');
    EXPECT_EQ(result, L"A");

    // Lowercase letter
    result = fmt::format(L"{}", L'z');
    EXPECT_EQ(result, L"z");

    // Digit character
    result = fmt::format(L"{}", L'5');
    EXPECT_EQ(result, L"5");

    // Space character
    result = fmt::format(L"{}", L' ');
    EXPECT_EQ(result, L" ");

    // Null wide character
    result = fmt::format(L"{}", L'\0');
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], L'\0');

    // Newline wide character
    result = fmt::format(L"{}", L'\n');
    EXPECT_EQ(result, L"\n");

    // Tab wide character
    result = fmt::format(L"{}", L'\t');
    EXPECT_EQ(result, L"\t");

    // Non-printable wide character
    result = fmt::format(L"{}", L'\x01');
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], L'\x01');

    // Maximum wchar_t value (basic)
    result = fmt::format(L"{}", static_cast<wchar_t>(0xFFFF));
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], static_cast<wchar_t>(0xFFFF));
}

// Test write(char) with format specifiers
TEST(WriteCharWithFormatTest, FormatsCharWithSpecifiers) {
    // Width specifier
    std::string result = fmt::format("{:5}", 'X');
    EXPECT_EQ(result, "X    ");

    // Left alignment
    result = fmt::format("{:<5}", 'Y');
    EXPECT_EQ(result, "Y    ");

    // Right alignment
    result = fmt::format("{:>5}", 'Z');
    EXPECT_EQ(result, "    Z");

    // Center alignment
    result = fmt::format("{:^5}", 'W');
    EXPECT_EQ(result, "  W  ");

    // Fill character
    result = fmt::format("{:*<5}", 'Q');
    EXPECT_EQ(result, "Q****");
}

// Test write(long double) with format specifiers
TEST(WriteLongDoubleWithFormatTest, FormatsLongDoubleWithSpecifiers) {
    // Fixed precision
    std::string result = fmt::format("{:.2f}", 3.14159265358979323846L);
    EXPECT_EQ(result, "3.14");

    // Scientific notation
    result = fmt::format("{:.2e}", 123456.789L);
    EXPECT_EQ(result, "1.23e+05");

    // General format
    result = fmt::format("{:.3g}", 123456.789L);
    EXPECT_EQ(result, "1.23e+05");

    // Width and precision
    result = fmt::format("{:10.4f}", 3.14159265358979323846L);
    EXPECT_EQ(result, "    3.1416");

    // Zero padding
    result = fmt::format("{:010.4f}", 3.14159265358979323846L);
    EXPECT_EQ(result, "00003.1416");

    // Plus sign
    result = fmt::format("{:+.2f}", 3.14L);
    EXPECT_EQ(result, "+3.14");

    // Space for positive
    result = fmt::format("{: .2f}", 3.14L);
    EXPECT_EQ(result, " 3.14");
}

// Test write(char) through format_to
TEST(WriteCharFormatToTest, FormatsCharToOutput) {
    std::string out;
    fmt::format_to(std::back_inserter(out), "{}", 'B');
    EXPECT_EQ(out, "B");

    out.clear();
    fmt::format_to(std::back_inserter(out), "{:>5}", 'C');
    EXPECT_EQ(out, "    C");
}

// Test write(long double) through format_to
TEST(WriteLongDoubleFormatToTest, FormatsLongDoubleToOutput) {
    std::string out;
    fmt::format_to(std::back_inserter(out), "{}", 2.71828L);
    EXPECT_FALSE(out.empty());
    EXPECT_TRUE(out.find("2.718") != std::string::npos || out.find("2.718") != std::string::npos);
}

// Test write(char) through format_to_n
TEST(WriteCharFormatToNTest, FormatsCharToN) {
    std::string out(5, '\0');
    auto result = fmt::format_to_n(out.begin(), 3, "{}", 'D');
    EXPECT_EQ(result.size, 1);
    EXPECT_EQ(out.substr(0, 1), "D");
}

// Test write(long double) through format_to_n
TEST(WriteLongDoubleFormatToNTest, FormatsLongDoubleToN) {
    std::string out(10, '\0');
    auto result = fmt::format_to_n(out.begin(), 5, "{:.3f}", 3.14159L);
    EXPECT_EQ(result.size, 5);
    EXPECT_EQ(out.substr(0, 5), "3.142");
}

// Test write(char) through formatted_size
TEST(WriteCharFormattedSizeTest, CharFormattedSize) {
    size_t size = fmt::formatted_size("{}", 'E');
    EXPECT_EQ(size, 1);

    size = fmt::formatted_size("{:>10}", 'F');
    EXPECT_EQ(size, 10);
}

// Test write(long double) through formatted_size
TEST(WriteLongDoubleFormattedSizeTest, LongDoubleFormattedSize) {
    size_t size = fmt::formatted_size("{:.2f}", 1.23L);
    EXPECT_EQ(size, 4); // "1.23"

    size = fmt::formatted_size("{:10.2f}", 1.23L);
    EXPECT_EQ(size, 10);
}

// Edge case: char with empty format string
TEST(WriteCharEdgeCaseTest, CharWithEmptyFormat) {
    std::string result = fmt::format("{}", '\0');
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], '\0');
}

// Edge case: long double with very high precision
TEST(WriteLongDoubleEdgeCaseTest, LongDoubleHighPrecision) {
    std::string result = fmt::format("{:.50f}", 1.0L / 3.0L);
    EXPECT_TRUE(result.find("333") != std::string::npos);
    EXPECT_NE(result, "0.33333333333333333333333333333333333333333333333333"); // Not exact
}

// Edge case: long double negative zero
TEST(WriteLongDoubleEdgeCaseTest, NegativeZero) {
    std::string result = fmt::format("{}", -0.0L);
    EXPECT_EQ(result, "-0");
}

// Edge case: char with fill and alignment
TEST(WriteCharEdgeCaseTest, CharFillAndAlignment) {
    std::string result = fmt::format("{:*^10}", 'G');
    EXPECT_EQ(result, "****G*****");
}