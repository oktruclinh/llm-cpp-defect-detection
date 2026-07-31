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

TEST(WriteLongDoubleTest, MaxAndMin) {
    long double max_val = std::numeric_limits<long double>::max();
    long double min_val = std::numeric_limits<long double>::min();
    // Just ensure no crash and output is non-empty
    EXPECT_FALSE(fmt::format("{}", max_val).empty());
    EXPECT_FALSE(fmt::format("{}", min_val).empty());
}

// Test write(char) via fmt::format
TEST(WriteCharTest, BasicChar) {
    EXPECT_EQ(fmt::format("{}", 'A'), "A");
    EXPECT_EQ(fmt::format("{}", 'z'), "z");
    EXPECT_EQ(fmt::format("{}", '0'), "0");
}

TEST(WriteCharTest, SpecialChars) {
    EXPECT_EQ(fmt::format("{}", '\n'), "\n");
    EXPECT_EQ(fmt::format("{}", '\t'), "\t");
    EXPECT_EQ(fmt::format("{}", '\0'), std::string("\0", 1));
}

TEST(WriteCharTest, PrintableAsciiRange) {
    for (char c = 32; c < 127; ++c) {
        EXPECT_EQ(fmt::format("{}", c), std::string(1, c));
    }
}

// Test write(wchar_t) via fmt::format (only if wchar_t is supported)
TEST(WriteWcharTest, BasicWchar) {
    // This test only makes sense if fmt supports wchar_t output
    // We test via fmt::format(L"{}", ...) which uses wchar_t version
    EXPECT_EQ(fmt::format(L"{}", L'A'), L"A");
    EXPECT_EQ(fmt::format(L"{}", L'z'), L"z");
    EXPECT_EQ(fmt::format(L"{}", L'0'), L"0");
}

TEST(WriteWcharTest, SpecialWchars) {
    EXPECT_EQ(fmt::format(L"{}", L'\n'), L"\n");
    EXPECT_EQ(fmt::format(L"{}", L'\t'), L"\t");
    EXPECT_EQ(fmt::format(L"{}", L'\0'), std::wstring(L"\0", 1));
}

TEST(WriteWcharTest, WideAsciiRange) {
    for (wchar_t c = 32; c < 127; ++c) {
        EXPECT_EQ(fmt::format(L"{}", c), std::wstring(1, c));
    }
}

// Edge cases: empty format string, but still exercises write
TEST(WriteEdgeCasesTest, EmptyFormatString) {
    EXPECT_EQ(fmt::format(""), "");
    EXPECT_EQ(fmt::format(L""), L"");
}

// Test that write(char) works with format specifiers (though they don't affect char)
TEST(WriteCharFormatTest, WithFormatSpecifiers) {
    EXPECT_EQ(fmt::format("{:c}", 'A'), "A");
    EXPECT_EQ(fmt::format("{:3}", 'A'), "A  ");
    EXPECT_EQ(fmt::format("{:<3}", 'A'), "A  ");
    EXPECT_EQ(fmt::format("{:>3}", 'A'), "  A");
    EXPECT_EQ(fmt::format("{:^3}", 'A'), " A ");
}

// Test that write(long double) works with format specifiers
TEST(WriteLongDoubleFormatTest, WithPrecision) {
    EXPECT_EQ(fmt::format("{:.1f}", 3.14159L), "3.1");
    EXPECT_EQ(fmt::format("{:.5f}", 2.0L), "2.00000");
    EXPECT_EQ(fmt::format("{:.0f}", 9.9L), "10");
}

TEST(WriteLongDoubleFormatTest, WithWidthAndAlignment) {
    EXPECT_EQ(fmt::format("{:10.2f}", 1.23L), "      1.23");
    EXPECT_EQ(fmt::format("{:<10.2f}", 1.23L), "1.23      ");
    EXPECT_EQ(fmt::format("{:>10.2f}", 1.23L), "      1.23");
    EXPECT_EQ(fmt::format("{:^10.2f}", 1.23L), "   1.23   ");
}

// Test that write(char) handles negative char values (signed char)
TEST(WriteCharSignedTest, NegativeChar) {
    char negative = static_cast<char>(-1);
    // Should output the character with value -1 (typically 0xFF)
    EXPECT_EQ(fmt::format("{}", negative), std::string(1, negative));
}

// Test that write(wchar_t) handles negative values
TEST(WriteWcharSignedTest, NegativeWchar) {
    wchar_t negative = static_cast<wchar_t>(-1);
    EXPECT_EQ(fmt::format(L"{}", negative), std::wstring(1, negative));
}