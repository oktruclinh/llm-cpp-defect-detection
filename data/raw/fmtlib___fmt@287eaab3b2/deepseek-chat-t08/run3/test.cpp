#include <gtest/gtest.h>
#include <fmt/format.h>
#include <cmath>
#include <limits>
#include <string>

// Tests for write(long double) through fmt::format (which uses write_double internally)
TEST(WriteLongDoubleTest, BasicPositive) {
    EXPECT_EQ(fmt::format("{}", 3.14L), "3.14");
    EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
}

TEST(WriteLongDoubleTest, Negative) {
    EXPECT_EQ(fmt::format("{}", -2.5L), "-2.5");
}

TEST(WriteLongDoubleTest, LargeValue) {
    long double large = 1e30L;
    std::string result = fmt::format("{}", large);
    EXPECT_FALSE(result.empty());
    // Check it contains digits and optional exponent
    EXPECT_NE(result.find_first_of("0123456789"), std::string::npos);
}

TEST(WriteLongDoubleTest, SmallValue) {
    long double small = 1e-30L;
    std::string result = fmt::format("{}", small);
    EXPECT_FALSE(result.empty());
    // Should be scientific notation
    EXPECT_NE(result.find('e'), std::string::npos);
}

TEST(WriteLongDoubleTest, Infinity) {
    long double inf = std::numeric_limits<long double>::infinity();
    EXPECT_EQ(fmt::format("{}", inf), "inf");
    EXPECT_EQ(fmt::format("{}", -inf), "-inf");
}

TEST(WriteLongDoubleTest, NaN) {
    long double nan = std::numeric_limits<long double>::quiet_NaN();
    std::string result = fmt::format("{}", nan);
    EXPECT_TRUE(result == "nan" || result == "-nan" || result == "NaN" || result == "-NaN");
}

TEST(WriteLongDoubleTest, MaxFinite) {
    long double max_val = std::numeric_limits<long double>::max();
    std::string result = fmt::format("{}", max_val);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result.find_first_of("0123456789"), std::string::npos);
}

TEST(WriteLongDoubleTest, MinPositive) {
    long double min_val = std::numeric_limits<long double>::min();
    std::string result = fmt::format("{}", min_val);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result.find_first_of("0123456789"), std::string::npos);
}

// Tests for write(char) through fmt::format (single character formatting)
TEST(WriteCharTest, BasicChar) {
    EXPECT_EQ(fmt::format("{}", 'A'), "A");
    EXPECT_EQ(fmt::format("{}", 'z'), "z");
}

TEST(WriteCharTest, DigitChar) {
    EXPECT_EQ(fmt::format("{}", '0'), "0");
    EXPECT_EQ(fmt::format("{}", '9'), "9");
}

TEST(WriteCharTest, SpecialChar) {
    EXPECT_EQ(fmt::format("{}", '\n'), "\n");
    EXPECT_EQ(fmt::format("{}", '\t'), "\t");
    EXPECT_EQ(fmt::format("{}", '\0'), std::string(1, '\0'));
}

TEST(WriteCharTest, NullChar) {
    EXPECT_EQ(fmt::format("{}", '\0'), std::string(1, '\0'));
}

TEST(WriteCharTest, CharInStringContext) {
    // Verify that char is not treated as a number
    EXPECT_EQ(fmt::format("{:c}", 65), "A");
}

// Tests for write(wchar_t) through fmt::format with wide strings
TEST(WriteWcharTest, BasicWchar) {
    EXPECT_EQ(fmt::format(L"{}", L'A'), L"A");
    EXPECT_EQ(fmt::format(L"{}", L'z'), L"z");
}

TEST(WriteWcharTest, DigitWchar) {
    EXPECT_EQ(fmt::format(L"{}", L'0'), L"0");
    EXPECT_EQ(fmt::format(L"{}", L'9'), L"9");
}

TEST(WriteWcharTest, SpecialWchar) {
    EXPECT_EQ(fmt::format(L"{}", L'\n'), L"\n");
    EXPECT_EQ(fmt::format(L"{}", L'\t'), L"\t");
    EXPECT_EQ(fmt::format(L"{}", L'\0'), std::wstring(1, L'\0'));
}

TEST(WriteWcharTest, NullWchar) {
    EXPECT_EQ(fmt::format(L"{}", L'\0'), std::wstring(1, L'\0'));
}

// Edge cases for write(char) - format specifiers that affect output
TEST(WriteCharTest, FillAndAlign) {
    EXPECT_EQ(fmt::format("{:>5}", 'X'), "    X");
    EXPECT_EQ(fmt::format("{:<5}", 'X'), "X    ");
    EXPECT_EQ(fmt::format("{:^5}", 'X'), "  X  ");
}

TEST(WriteCharTest, WidthOnly) {
    EXPECT_EQ(fmt::format("{:3}", 'A'), "A  ");
}

// Edge cases for write(wchar_t) - wide format specifiers
TEST(WriteWcharTest, FillAndAlign) {
    EXPECT_EQ(fmt::format(L"{:>5}", L'X'), L"    X");
    EXPECT_EQ(fmt::format(L"{:<5}", L'X'), L"X    ");
    EXPECT_EQ(fmt::format(L"{:^5}", L'X'), L"  X  ");
}

TEST(WriteWcharTest, WidthOnly) {
    EXPECT_EQ(fmt::format(L"{:3}", L'A'), L"A  ");
}