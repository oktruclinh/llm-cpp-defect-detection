#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

// Test the on_chr() behavior through format_to with char type
TEST(FormatHandlerTest, OnChrChar) {
    // Test basic character output
    auto it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", 65);
    std::string result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "A");

    // Test with zero value
    it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", 0);
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, std::string(1, '\0'));

    // Test with negative value (should produce character from absolute value)
    it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", -65);
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "A");

    // Test with max char value
    it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", 127);
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, std::string(1, 127));
}

// Test on_chr() with wchar_t type
TEST(FormatHandlerTest, OnChrWideChar) {
    // Test basic wide character output
    auto it = fmt::format_to(std::back_inserter(std::wstring{}), L"{:c}", 65);
    std::wstring result = std::wstring(std::back_inserter(std::wstring{}).base(), it);
    EXPECT_EQ(result, L"A");

    // Test with zero value
    it = fmt::format_to(std::back_inserter(std::wstring{}), L"{:c}", 0);
    result = std::wstring(std::back_inserter(std::wstring{}).base(), it);
    EXPECT_EQ(result, std::wstring(1, L'\0'));

    // Test with negative value
    it = fmt::format_to(std::back_inserter(std::wstring{}), L"{:c}", -65);
    result = std::wstring(std::back_inserter(std::wstring{}).base(), it);
    EXPECT_EQ(result, L"A");

    // Test with Unicode code point
    it = fmt::format_to(std::back_inserter(std::wstring{}), L"{:c}", 0x00E9);
    result = std::wstring(std::back_inserter(std::wstring{}).base(), it);
    EXPECT_EQ(result, L"\u00E9");
}

// Test on_chr() with signed char type through format_to
TEST(FormatHandlerTest, OnChrSignedChar) {
    // Test with negative char value
    auto it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", static_cast<signed char>(-65));
    std::string result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "A");

    // Test with signed char max
    it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", static_cast<signed char>(127));
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, std::string(1, 127));
}

// Test on_chr() with unsigned char type
TEST(FormatHandlerTest, OnChrUnsignedChar) {
    // Test with unsigned char value
    auto it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", static_cast<unsigned char>(65));
    std::string result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "A");

    // Test with unsigned char max
    it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", static_cast<unsigned char>(255));
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, std::string(1, static_cast<char>(255)));
}

// Test on_chr() with integer types that produce characters
TEST(FormatHandlerTest, OnChrIntegerTypes) {
    // Test with short
    auto it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", static_cast<short>(65));
    std::string result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "A");

    // Test with unsigned short
    it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", static_cast<unsigned short>(65));
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "A");

    // Test with int
    it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", 65);
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "A");

    // Test with long
    it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", 65L);
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "A");

    // Test with long long
    it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", 65LL);
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "A");
}

// Test on_chr() with floating point types (should truncate to char)
TEST(FormatHandlerTest, OnChrFloatingPoint) {
    // Test with float
    auto it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", 65.0f);
    std::string result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "A");

    // Test with double
    it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", 65.0);
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "A");

    // Test with negative double
    it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", -65.0);
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "A");
}

// Test on_chr() with edge case values
TEST(FormatHandlerTest, OnChrEdgeCases) {
    // Test with minimum negative value (should use absolute value)
    auto it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", -128);
    std::string result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, std::string(1, static_cast<char>(128)));

    // Test with value that wraps around
    it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", 256);
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, std::string(1, static_cast<char>(0)));

    // Test with large positive value
    it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", 1000);
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, std::string(1, static_cast<char>(1000 % 256)));
}

// Test on_chr() with format specifiers that affect output
TEST(FormatHandlerTest, OnChrWithFormatSpecs) {
    // Test with width specifier
    auto it = fmt::format_to(std::back_inserter(std::string{}), "{:5c}", 65);
    std::string result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "    A");

    // Test with fill and align
    it = fmt::format_to(std::back_inserter(std::string{}), "{:*<5c}", 65);
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "A****");

    // Test with center alignment
    it = fmt::format_to(std::back_inserter(std::string{}), "{:*^5c}", 65);
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, "**A**");
}

// Test on_chr() with boolean type
TEST(FormatHandlerTest, OnChrBool) {
    // Test with true
    auto it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", true);
    std::string result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, std::string(1, static_cast<char>(1)));

    // Test with false
    it = fmt::format_to(std::back_inserter(std::string{}), "{:c}", false);
    result = std::string(std::back_inserter(std::string{}).base(), it);
    EXPECT_EQ(result, std::string(1, static_cast<char>(0)));
}