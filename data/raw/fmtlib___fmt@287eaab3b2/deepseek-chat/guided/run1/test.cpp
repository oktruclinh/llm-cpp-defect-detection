#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>

// Test write(long double) through public API
TEST(WriteLongDoubleTest, BasicValues) {
  // Normal positive value
  EXPECT_EQ(fmt::format("{}", 3.14159265358979323846L), "3.141592653589793");
  
  // Negative value
  EXPECT_EQ(fmt::format("{}", -2.718281828459045L), "-2.718281828459045");
  
  // Zero
  EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
  
  // Negative zero
  EXPECT_EQ(fmt::format("{}", -0.0L), "-0.0");
}

TEST(WriteLongDoubleTest, SpecialValues) {
  // Infinity
  EXPECT_EQ(fmt::format("{}", std::numeric_limits<long double>::infinity()), "inf");
  
  // Negative infinity
  EXPECT_EQ(fmt::format("{}", -std::numeric_limits<long double>::infinity()), "-inf");
  
  // NaN
  auto nan_str = fmt::format("{}", std::numeric_limits<long double>::quiet_NaN());
  EXPECT_TRUE(nan_str == "nan" || nan_str == "-nan" || nan_str == "NaN" || nan_str == "-NaN");
}

TEST(WriteLongDoubleTest, ExtremeValues) {
  // Very small positive number
  EXPECT_EQ(fmt::format("{}", 1e-100L), "1e-100");
  
  // Very large number
  EXPECT_EQ(fmt::format("{}", 1e+100L), "1e+100");
  
  // Minimum positive normal
  auto min_val = std::numeric_limits<long double>::min();
  EXPECT_FALSE(fmt::format("{}", min_val).empty());
  
  // Maximum finite value
  auto max_val = std::numeric_limits<long double>::max();
  EXPECT_FALSE(fmt::format("{}", max_val).empty());
}

TEST(WriteLongDoubleTest, FormatSpecs) {
  // Fixed precision
  EXPECT_EQ(fmt::format("{:.2f}", 1.23456789L), "1.23");
  
  // Scientific notation
  EXPECT_EQ(fmt::format("{:.2e}", 12345.6789L), "1.23e+04");
  
  // General format
  EXPECT_EQ(fmt::format("{:.3g}", 12345.6789L), "1.23e+04");
  
  // Width and alignment
  EXPECT_EQ(fmt::format("{:10.2f}", 3.14L), "      3.14");
  EXPECT_EQ(fmt::format("{:<10.2f}", 3.14L), "3.14      ");
  EXPECT_EQ(fmt::format("{:>10.2f}", 3.14L), "      3.14");
  EXPECT_EQ(fmt::format("{:^10.2f}", 3.14L), "   3.14   ");
  
  // Sign
  EXPECT_EQ(fmt::format("{:+g}", 1.0L), "+1");
  EXPECT_EQ(fmt::format("{:+g}", -1.0L), "-1");
  EXPECT_EQ(fmt::format("{: g}", 1.0L), " 1");
  EXPECT_EQ(fmt::format("{: g}", -1.0L), "-1");
}

// Test write(char) through public API
TEST(WriteCharTest, BasicCharacters) {
  EXPECT_EQ(fmt::format("{}", 'a'), "a");
  EXPECT_EQ(fmt::format("{}", 'Z'), "Z");
  EXPECT_EQ(fmt::format("{}", '0'), "0");
  EXPECT_EQ(fmt::format("{}", ' '), " ");
}

TEST(WriteCharTest, SpecialCharacters) {
  EXPECT_EQ(fmt::format("{}", '\n'), "\n");
  EXPECT_EQ(fmt::format("{}", '\t'), "\t");
  EXPECT_EQ(fmt::format("{}", '\0'), std::string(1, '\0'));
  EXPECT_EQ(fmt::format("{}", '\x7F'), "\x7F");
}

TEST(WriteCharTest, FormatSpecs) {
  // Width
  EXPECT_EQ(fmt::format("{:5}", 'x'), "x    ");
  EXPECT_EQ(fmt::format("{:<5}", 'x'), "x    ");
  EXPECT_EQ(fmt::format("{:>5}", 'x'), "    x");
  EXPECT_EQ(fmt::format("{:^5}", 'x'), "  x  ");
  
  // Fill character
  EXPECT_EQ(fmt::format("{:*<5}", 'x'), "x****");
  EXPECT_EQ(fmt::format("{:*>5}", 'x'), "****x");
  EXPECT_EQ(fmt::format("{:*^5}", 'x'), "**x**");
}

TEST(WriteCharTest, EdgeCases) {
  // Minimum and maximum char values
  EXPECT_EQ(fmt::format("{}", static_cast<char>(std::numeric_limits<char>::min())),
            std::string(1, std::numeric_limits<char>::min()));
  EXPECT_EQ(fmt::format("{}", static_cast<char>(std::numeric_limits<char>::max())),
            std::string(1, std::numeric_limits<char>::max()));
}

// Test write(wchar_t) through public API (only if wchar_t is supported)
TEST(WriteWCharTest, BasicWideCharacters) {
  // Note: wchar_t support depends on platform and fmt configuration
  // These tests verify the function exists and works for basic cases
  EXPECT_EQ(fmt::format(L"{}", L'a'), L"a");
  EXPECT_EQ(fmt::format(L"{}", L'Z'), L"Z");
  EXPECT_EQ(fmt::format(L"{}", L'0'), L"0");
  EXPECT_EQ(fmt::format(L"{}", L' '), L" ");
}

TEST(WriteWCharTest, WideSpecialCharacters) {
  EXPECT_EQ(fmt::format(L"{}", L'\n'), L"\n");
  EXPECT_EQ(fmt::format(L"{}", L'\t'), L"\t");
  EXPECT_EQ(fmt::format(L"{}", L'\0'), std::wstring(1, L'\0'));
}

TEST(WriteWCharTest, WideFormatSpecs) {
  EXPECT_EQ(fmt::format(L"{:5}", L'x'), L"x    ");
  EXPECT_EQ(fmt::format(L"{:<5}", L'x'), L"x    ");
  EXPECT_EQ(fmt::format(L"{:>5}", L'x'), L"    x");
  EXPECT_EQ(fmt::format(L"{:^5}", L'x'), L"  x  ");
  EXPECT_EQ(fmt::format(L"{:*<5}", L'x'), L"x****");
  EXPECT_EQ(fmt::format(L"{:*>5}", L'x'), L"****x");
  EXPECT_EQ(fmt::format(L"{:*^5}", L'x'), L"**x**");
}

TEST(WriteWCharTest, WideEdgeCases) {
  EXPECT_EQ(fmt::format(L"{}", static_cast<wchar_t>(std::numeric_limits<wchar_t>::min())),
            std::wstring(1, std::numeric_limits<wchar_t>::min()));
  EXPECT_EQ(fmt::format(L"{}", static_cast<wchar_t>(std::numeric_limits<wchar_t>::max())),
            std::wstring(1, std::numeric_limits<wchar_t>::max()));
}