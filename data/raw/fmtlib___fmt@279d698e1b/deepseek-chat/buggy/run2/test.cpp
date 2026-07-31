#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <cmath>
#include <climits>

// Test the on_chr() behavior through format API for char types
TEST(FormatHandlerTest, OnChrChar) {
    // Test with positive char
    std::string result = fmt::format("{:c}", 'A');
    EXPECT_EQ(result, "A");
    
    // Test with negative char (signed char)
    result = fmt::format("{:c}", static_cast<char>(-1));
    EXPECT_EQ(result, "\xff");
    
    // Test with zero char
    result = fmt::format("{:c}", '\0');
    EXPECT_EQ(result, std::string(1, '\0'));
}

// Test the on_chr() behavior through format API for wchar_t types
TEST(FormatHandlerTest, OnChrWchar) {
    // Test with positive wchar_t
    std::wstring result = fmt::format(L"{:c}", L'A');
    EXPECT_EQ(result, L"A");
    
    // Test with zero wchar_t
    result = fmt::format(L"{:c}", L'\0');
    EXPECT_EQ(result, std::wstring(1, L'\0'));
}

// Test the on_chr() behavior through format API for integer types
TEST(FormatHandlerTest, OnChrInt) {
    // Test with ASCII value
    std::string result = fmt::format("{:c}", 65);
    EXPECT_EQ(result, "A");
    
    // Test with zero
    result = fmt::format("{:c}", 0);
    EXPECT_EQ(result, std::string(1, '\0'));
    
    // Test with max ASCII value
    result = fmt::format("{:c}", 127);
    EXPECT_EQ(result, "\x7f");
}

// Test the on_chr() behavior through format API for unsigned char
TEST(FormatHandlerTest, OnChrUnsignedChar) {
    // Test with unsigned char
    std::string result = fmt::format("{:c}", static_cast<unsigned char>(65));
    EXPECT_EQ(result, "A");
    
    // Test with max unsigned char
    result = fmt::format("{:c}", static_cast<unsigned char>(255));
    EXPECT_EQ(result, "\xff");
}

// Test the on_chr() behavior through format API with width and fill
TEST(FormatHandlerTest, OnChrWithWidth) {
    // Test with width specification
    std::string result = fmt::format("{:5c}", 'A');
    EXPECT_EQ(result, "    A");
    
    // Test with width and fill
    result = fmt::format("{:*<5c}", 'A');
    EXPECT_EQ(result, "A****");
    
    // Test with width and center alignment
    result = fmt::format("{:*^5c}", 'A');
    EXPECT_EQ(result, "**A**");
}

// Test the on_chr() behavior through format API with precision (should be ignored for char)
TEST(FormatHandlerTest, OnChrWithPrecision) {
    // Precision should be ignored for char type
    std::string result = fmt::format("{:.5c}", 'A');
    EXPECT_EQ(result, "A");
}

// Test the on_chr() behavior through format API with sign specifiers
TEST(FormatHandlerTest, OnChrWithSign) {
    // Sign specifiers should be ignored for char type
    std::string result = fmt::format("{:+c}", 'A');
    EXPECT_EQ(result, "A");
    
    result = fmt::format("{:-c}", 'A');
    EXPECT_EQ(result, "A");
    
    result = fmt::format("{: c}", 'A');
    EXPECT_EQ(result, "A");
}

// Test the on_chr() behavior through format API with alternate form
TEST(FormatHandlerTest, OnChrWithAlternate) {
    // Alternate form should be ignored for char type
    std::string result = fmt::format("{:#c}", 'A');
    EXPECT_EQ(result, "A");
}

// Test the on_chr() behavior through format API with locale
TEST(FormatHandlerTest, OnChrWithLocale) {
    // Locale should not affect char formatting
    std::string result = fmt::format("{:Lc}", 'A');
    EXPECT_EQ(result, "A");
}

// Test the on_chr() behavior through format API with multiple arguments
TEST(FormatHandlerTest, OnChrMultiple) {
    std::string result = fmt::format("{:c}{:c}{:c}", 'A', 'B', 'C');
    EXPECT_EQ(result, "ABC");
    
    result = fmt::format("{:c} {:c} {:c}", 'X', 'Y', 'Z');
    EXPECT_EQ(result, "X Y Z");
}

// Test the on_chr() behavior through format API with mixed types
TEST(FormatHandlerTest, OnChrMixedTypes) {
    std::string result = fmt::format("{:c} {} {:c}", 'A', 42, 'B');
    EXPECT_EQ(result, "A 42 B");
}

// Test the on_chr() behavior through format API with escape sequences
TEST(FormatHandlerTest, OnChrEscape) {
    // Test with newline
    std::string result = fmt::format("{:c}", '\n');
    EXPECT_EQ(result, "\n");
    
    // Test with tab
    result = fmt::format("{:c}", '\t');
    EXPECT_EQ(result, "\t");
    
    // Test with carriage return
    result = fmt::format("{:c}", '\r');
    EXPECT_EQ(result, "\r");
}

// Test the on_chr() behavior through format API with extended ASCII
TEST(FormatHandlerTest, OnChrExtendedAscii) {
    // Test with values 128-255 (extended ASCII)
    for (int i = 128; i <= 255; ++i) {
        std::string result = fmt::format("{:c}", i);
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(static_cast<unsigned char>(result[0]), static_cast<unsigned char>(i));
    }
}

// Test the on_chr() behavior through format API with negative values (should wrap)
TEST(FormatHandlerTest, OnChrNegativeValues) {
    // Negative values should wrap around for char formatting
    std::string result = fmt::format("{:c}", -1);
    EXPECT_EQ(result, "\xff");
    
    result = fmt::format("{:c}", -128);
    EXPECT_EQ(result, "\x80");
}

// Test the on_chr() behavior through format API with very large values
TEST(FormatHandlerTest, OnChrLargeValues) {
    // Large values should be truncated to char
    std::string result = fmt::format("{:c}", 256);
    EXPECT_EQ(result, "\x00");
    
    result = fmt::format("{:c}", 257);
    EXPECT_EQ(result, "\x01");
    
    result = fmt::format("{:c}", 511);
    EXPECT_EQ(result, "\xff");
}

// Test the on_chr() behavior through format API with format string errors
TEST(FormatHandlerTest, OnChrFormatError) {
    // Invalid format specifier should throw
    EXPECT_THROW(fmt::format("{:x}", 'A'), fmt::format_error);
}