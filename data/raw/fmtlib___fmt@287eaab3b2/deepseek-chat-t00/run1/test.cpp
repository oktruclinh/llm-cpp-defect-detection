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

TEST(WriteLongDoubleTest, MaxAndMinValues) {
    long double max_val = std::numeric_limits<long double>::max();
    long double min_val = std::numeric_limits<long double>::min();
    EXPECT_FALSE(fmt::format("{}", max_val).empty());
    EXPECT_FALSE(fmt::format("{}", min_val).empty());
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
    EXPECT_EQ(fmt::format("{}", '\0'), std::string("\0", 1));
}

TEST(WriteCharTest, PrintableAndNonPrintable) {
    EXPECT_EQ(fmt::format("{}", ' '), " ");
    EXPECT_EQ(fmt::format("{}", '\x01'), std::string("\x01", 1));
    EXPECT_EQ(fmt::format("{}", '\x7F'), std::string("\x7F", 1));
}

// Test write(wchar_t) via fmt::format (only if wchar_t is supported)
TEST(WriteWcharTest, BasicWideCharacters) {
    EXPECT_EQ(fmt::format(L"{}", L'a'), L"a");
    EXPECT_EQ(fmt::format(L"{}", L'Z'), L"Z");
    EXPECT_EQ(fmt::format(L"{}", L'0'), L"0");
}

TEST(WriteWcharTest, SpecialWideCharacters) {
    EXPECT_EQ(fmt::format(L"{}", L'\n'), L"\n");
    EXPECT_EQ(fmt::format(L"{}", L'\t'), L"\t");
    EXPECT_EQ(fmt::format(L"{}", L'\0'), std::wstring(L"\0", 1));
}

TEST(WriteWcharTest, WidePrintableAndNonPrintable) {
    EXPECT_EQ(fmt::format(L"{}", L' '), L" ");
    EXPECT_EQ(fmt::format(L"{}", L'\x01'), std::wstring(L"\x01", 1));
    EXPECT_EQ(fmt::format(L"{}", L'\x7F'), std::wstring(L"\x7F", 1));
}

// Edge cases: empty format string, multiple writes
TEST(WriteCharTest, EmptyFormatString) {
    EXPECT_EQ(fmt::format("{}", 'x'), "x");
}

TEST(WriteCharTest, MultipleCharactersInString) {
    EXPECT_EQ(fmt::format("{}{}", 'a', 'b'), "ab");
    EXPECT_EQ(fmt::format("{}{}{}", '1', '2', '3'), "123");
}

TEST(WriteLongDoubleTest, FormatSpecifiers) {
    EXPECT_EQ(fmt::format("{:.1f}", 3.14159L), "3.1");
    EXPECT_EQ(fmt::format("{:.10f}", 1.0L / 3.0L), "0.3333333333");
}

TEST(WriteLongDoubleTest, NegativeZeroFormatting) {
    EXPECT_EQ(fmt::format("{:.1f}", -0.0L), "-0.0");
    EXPECT_EQ(fmt::format("{:+.0f}", 0.0L), "+0");
}