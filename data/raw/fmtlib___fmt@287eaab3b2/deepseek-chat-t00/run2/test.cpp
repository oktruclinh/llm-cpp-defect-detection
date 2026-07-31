#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>

// Test write(long double) via fmt::format
TEST(WriteLongDoubleTest, BasicValues) {
    EXPECT_EQ(fmt::format("{}", 3.14L), "3.14");
    EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
    EXPECT_EQ(fmt::format("{}", -2.5L), "-2.5");
}

TEST(WriteLongDoubleTest, LargeAndSmallValues) {
    EXPECT_EQ(fmt::format("{}", 1e10L), "10000000000.0");
    EXPECT_EQ(fmt::format("{}", 1e-10L), "0.0000000001");
}

TEST(WriteLongDoubleTest, InfinityAndNaN) {
    long double inf = std::numeric_limits<long double>::infinity();
    long double nan = std::numeric_limits<long double>::quiet_NaN();
    EXPECT_EQ(fmt::format("{}", inf), "inf");
    EXPECT_EQ(fmt::format("{}", -inf), "-inf");
    EXPECT_EQ(fmt::format("{}", nan), "nan");
}

TEST(WriteLongDoubleTest, ZeroAndNegativeZero) {
    EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
    EXPECT_EQ(fmt::format("{}", -0.0L), "-0.0");
}

// Test write(char) via fmt::format
TEST(WriteCharTest, BasicCharacters) {
    EXPECT_EQ(fmt::format("{}", 'a'), "a");
    EXPECT_EQ(fmt::format("{}", 'Z'), "Z");
    EXPECT_EQ(fmt::format("{}", '0'), "0");
}

TEST(WriteCharTest, SpecialCharacters) {
    EXPECT_EQ(fmt::format("{}", '\n'), "\n");
    EXPECT_EQ(fmt::format("{}", '\t'), "\t");
    EXPECT_EQ(fmt::format("{}", '\0'), std::string(1, '\0'));
}

TEST(WriteCharTest, NonPrintableCharacters) {
    EXPECT_EQ(fmt::format("{}", '\x01'), std::string(1, '\x01'));
    EXPECT_EQ(fmt::format("{}", '\x7F'), std::string(1, '\x7F'));
}

// Test write(wchar_t) via fmt::format (only if wchar_t is supported)
TEST(WriteWcharTest, BasicWideCharacters) {
    EXPECT_EQ(fmt::format(L"{}", L'a'), L"a");
    EXPECT_EQ(fmt::format(L"{}", L'Z'), L"Z");
    EXPECT_EQ(fmt::format(L"{}", L'0'), L"0");
}

TEST(WriteWcharTest, WideSpecialCharacters) {
    EXPECT_EQ(fmt::format(L"{}", L'\n'), L"\n");
    EXPECT_EQ(fmt::format(L"{}", L'\t'), L"\t");
    EXPECT_EQ(fmt::format(L"{}", L'\0'), std::wstring(1, L'\0'));
}

TEST(WriteWcharTest, WideNonPrintableCharacters) {
    EXPECT_EQ(fmt::format(L"{}", L'\x01'), std::wstring(1, L'\x01'));
    EXPECT_EQ(fmt::format(L"{}", L'\x7F'), std::wstring(1, L'\x7F'));
}

// Edge cases: empty format string, but still exercises write for single char
TEST(WriteCharTest, SingleCharFormat) {
    EXPECT_EQ(fmt::format("{:c}", 65), "A");
    EXPECT_EQ(fmt::format("{:c}", 97), "a");
}

// Test that write(long double) respects format specs
TEST(WriteLongDoubleTest, FormatSpecs) {
    EXPECT_EQ(fmt::format("{:.2f}", 3.14159L), "3.14");
    EXPECT_EQ(fmt::format("{:+.1f}", 1.5L), "+1.5");
    EXPECT_EQ(fmt::format("{:010.3f}", 1.234L), "00001.234");
}

// Test that write(char) works with format specs
TEST(WriteCharTest, CharFormatSpecs) {
    EXPECT_EQ(fmt::format("{:>5}", 'x'), "    x");
    EXPECT_EQ(fmt::format("{:*<5}", 'y'), "y****");
}

// Test that write(wchar_t) works with format specs
TEST(WriteWcharTest, WideCharFormatSpecs) {
    EXPECT_EQ(fmt::format(L"{:>5}", L'x'), L"    x");
    EXPECT_EQ(fmt::format(L"{:*<5}", L'y'), L"y****");
}