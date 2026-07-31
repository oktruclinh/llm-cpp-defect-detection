#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>

// Test write(long double) via fmt::format
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
  EXPECT_TRUE(fmt::format("{}", std::numeric_limits<long double>::quiet_NaN()) == "nan" ||
              fmt::format("{}", std::numeric_limits<long double>::quiet_NaN()) == "-nan");
}

TEST(WriteLongDoubleTest, ExtremeValues) {
  // Very large number
  EXPECT_EQ(fmt::format("{}", 1e30L), "1e+30");
  // Very small number
  EXPECT_EQ(fmt::format("{}", 1e-30L), "1e-30");
  // Largest finite value
  long double max_val = std::numeric_limits<long double>::max();
  std::string result = fmt::format("{}", max_val);
  EXPECT_FALSE(result.empty());
  EXPECT_NE(result, "inf");
  EXPECT_NE(result, "-inf");
  // Smallest positive normalized value
  long double min_val = std::numeric_limits<long double>::min();
  result = fmt::format("{}", min_val);
  EXPECT_FALSE(result.empty());
  EXPECT_NE(result, "0.0");
}

TEST(WriteLongDoubleTest, FormatSpecs) {
  // With precision specifier
  EXPECT_EQ(fmt::format("{:.5f}", 1.23456789L), "1.23457");
  // With width
  EXPECT_EQ(fmt::format("{:10.3f}", 3.14159L), "    3.142");
  // Scientific notation
  EXPECT_EQ(fmt::format("{:.3e}", 12345.6789L), "1.235e+04");
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

TEST(WriteCharTest, NonPrintableChars) {
  // Control characters
  EXPECT_EQ(fmt::format("{}", '\x01'), std::string("\x01", 1));
  EXPECT_EQ(fmt::format("{}", '\x7F'), std::string("\x7F", 1));
}

TEST(WriteCharTest, CharWithFormatSpecs) {
  // Width specifier
  EXPECT_EQ(fmt::format("{:5}", 'X'), "X    ");
  EXPECT_EQ(fmt::format("{:<5}", 'X'), "X    ");
  EXPECT_EQ(fmt::format("{:>5}", 'X'), "    X");
  EXPECT_EQ(fmt::format("{:^5}", 'X'), "  X  ");
  // Fill character
  EXPECT_EQ(fmt::format("{:*<5}", 'X'), "X****");
  EXPECT_EQ(fmt::format("{:*>5}", 'X'), "****X");
}

// Test write(wchar_t) via fmt::format (only if wchar_t is supported)
TEST(WriteWcharTest, BasicWchar) {
  // This test only runs if wchar_t formatting is enabled
  // We test via fmt::format(L"{}", ...) which uses wchar_t overloads
  EXPECT_EQ(fmt::format(L"{}", L'A'), L"A");
  EXPECT_EQ(fmt::format(L"{}", L'z'), L"z");
  EXPECT_EQ(fmt::format(L"{}", L'0'), L"0");
}

TEST(WriteWcharTest, SpecialWchars) {
  EXPECT_EQ(fmt::format(L"{}", L'\n'), L"\n");
  EXPECT_EQ(fmt::format(L"{}", L'\t'), L"\t");
  EXPECT_EQ(fmt::format(L"{}", L'\0'), std::wstring(L"\0", 1));
}

TEST(WriteWcharTest, WcharWithFormatSpecs) {
  EXPECT_EQ(fmt::format(L"{:5}", L'X'), L"X    ");
  EXPECT_EQ(fmt::format(L"{:*<5}", L'X'), L"X****");
  EXPECT_EQ(fmt::format(L"{:*>5}", L'X'), L"****X");
  EXPECT_EQ(fmt::format(L"{:*^5}", L'X'), L"**X**");
}

// Edge case: empty string context (write called with no additional formatting)
TEST(WriteEdgeCaseTest, SingleCharString) {
  // Formatting a single character as a string
  EXPECT_EQ(fmt::format("{}", 'a'), "a");
  // Formatting a single wchar_t as a string
  EXPECT_EQ(fmt::format(L"{}", L'a'), L"a");
}

// Test that write works correctly with different buffer types
TEST(WriteBufferTest, MemoryBuffer) {
  fmt::memory_buffer buf;
  fmt::format_to(std::back_inserter(buf), "{}", 3.14L);
  EXPECT_EQ(fmt::to_string(buf), "3.14");
  
  buf.clear();
  fmt::format_to(std::back_inserter(buf), "{}", 'X');
  EXPECT_EQ(fmt::to_string(buf), "X");
}