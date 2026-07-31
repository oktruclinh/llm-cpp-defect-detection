#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/core.h> // For fmt::format_to and other core functionalities

// Helper function to format a value into a string for testing
template <typename T>
std::string format_value(const char* format_str, T value) {
    return fmt::format(format_str, value);
}

// Test fixture for common setup if needed, though not strictly necessary for these tests
class FmtWritePaddedTest : public ::testing::Test {
protected:
    // You can set up common resources here if multiple tests need them
};

// Test case for basic right alignment with padding
TEST_F(FmtWritePaddedTest, BasicRightAlignment) {
    // Test with a positive integer
    EXPECT_EQ(format_value("{:>5}", 123), "  123");
    // Test with a string
    EXPECT_EQ(format_value("{:>7}", "hello"), "  hello");
    // Test with a character
    EXPECT_EQ(format_value("{:>3}", 'A'), "  A");
}

// Test case for right alignment with fill character
TEST_F(FmtWritePaddedTest, RightAlignmentWithFill) {
    // Test with a positive integer and custom fill
    EXPECT_EQ(format_value("{:*>5}", 123), "**123");
    // Test with a string and custom fill
    EXPECT_EQ(format_value("{:->7}", "hello"), "--hello");
}

// Test case for right alignment with no padding needed (width <= value size)
TEST_F(FmtWritePaddedTest, RightAlignmentNoPaddingNeeded) {
    // Width equals value size
    EXPECT_EQ(format_value("{:>3}", 123), "123");
    // Width less than value size (should not truncate, just output value)
    EXPECT_EQ(format_value("{:>2}", 123), "123");
    EXPECT_EQ(format_value("{:>4}", "hello"), "hello"); // Width less than string size
}

// Test case for right alignment with zero width (should output value without padding)
TEST_F(FmtWritePaddedTest, RightAlignmentZeroWidth) {
    EXPECT_EQ(format_value("{:>0}", 123), "123");
    EXPECT_EQ(format_value("{:>0}", "test"), "test");
}

// Test case for right alignment with negative numbers (sign handling)
TEST_F(FmtWritePaddedTest, RightAlignmentNegativeNumbers) {
    // Default sign handling for integers
    EXPECT_EQ(format_value("{:>5}", -123), " -123");
    // With custom fill
    EXPECT_EQ(format_value("{:*>6}", -123), "**-123");
    // With explicit sign
    EXPECT_EQ(format_value("{: >5}", -123), " -123"); // Space for sign
    EXPECT_EQ(format_value("{:+>5}", -123), "-123"); // + sign specifier doesn't force + for negative
    EXPECT_EQ(format_value("{: >5}", 123), "  123"); // Space for sign for positive
    EXPECT_EQ(format_value("{:+>5}", 123), " +123"); // Explicit + for positive
}

// Test case for right alignment with different data types
TEST_F(FmtWritePaddedTest, RightAlignmentDifferentTypes) {
    // Floating point
    EXPECT_EQ(format_value("{:>10.2f}", 3.14159), "      3.14");
    // Boolean
    EXPECT_EQ(format_value("{:>7}", true), "   true");
    EXPECT_EQ(format_value("{:>8}", false), "   false");
    // Pointer (address will vary, so just check format)
    int x = 42;
    // fmtlib disallows formatting of non-void pointers directly.
    // To format a pointer, it should be cast to `const void*`.
    std::string ptr_str = fmt::format("{:>10p}", static_cast<const void*>(&x));
    ASSERT_EQ(ptr_str.length(), 10);
    ASSERT_EQ(ptr_str[0], ' '); // Check for padding
    ASSERT_EQ(ptr_str[1], ' ');
    ASSERT_EQ(ptr_str[2], '0'); // Check for 0x prefix
    ASSERT_EQ(ptr_str[3], 'x');
}

// Test case for empty string/zero value
TEST_F(FmtWritePaddedTest, RightAlignmentEmptyAndZero) {
    EXPECT_EQ(format_value("{:>5}", ""), "     ");
    EXPECT_EQ(format_value("{:*>5}", ""), "*****");
    EXPECT_EQ(format_value("{:>5}", 0), "    0");
    EXPECT_EQ(format_value("{:*>5}", 0), "****0");
}

// Test case for wide characters (if supported by fmt::format)
TEST_F(FmtWritePaddedTest, WideCharacters) {
    // fmt::format typically works with char strings.
    // To test wide characters, we'd need to use wstring and wformat.
    // For this test, we'll stick to char, assuming the underlying write_padded
    // handles Char correctly based on the output iterator type.
    // The focal function uses `Char` template parameter, which is typically `char` or `wchar_t`.
    // The `fmt::format` public API usually defaults to `char`.
    // We can simulate a wide char scenario by using `fmt::format_to` with a `std::wstring` buffer.

    std::wstring wbuf;
    fmt::format_to(std::back_inserter(wbuf), L"{:>5}", L"hi");
    EXPECT_EQ(wbuf, L"   hi");

    wbuf.clear();
    fmt::format_to(std::back_inserter(wbuf), L"{:*>7}", L"world");
    EXPECT_EQ(wbuf, L"**world");

    wbuf.clear();
    fmt::format_to(std::back_inserter(wbuf), L"{:>5}", 123); // Integer with wide char output
    EXPECT_EQ(wbuf, L"  123");
}

// Test case for maximum width (within reasonable limits)
TEST_F(FmtWritePaddedTest, RightAlignmentLargeWidth) {
    EXPECT_EQ(format_value("{:>100}", "short"), std::string(95, ' ') + "short");
    EXPECT_EQ(format_value("{:->100}", 12345), std::string(95, '-') + "12345");
}

// Test case for padding with different character types (e.g., unicode fill char)
// This depends on the compiler's support for wide characters in format strings
TEST_F(FmtWritePaddedTest, UnicodeFillCharacter) {
    // This might not work directly with `char` format strings if the fill char is multi-byte.
    // However, if the fill character is a single-byte char (like ' '), it's fine.
    // For actual unicode fill, `wchar_t` or `char8_t`/`char16_t`/`char32_t` would be needed.
    // Assuming ' ' is the default fill for char strings.
    EXPECT_EQ(format_value("{: >5}", 1), "    1");
    // If we wanted to test a multi-byte fill character, we'd need to use wide strings.
    // Example (conceptual, might not compile directly depending on fmt version/config):
    // std::wstring wbuf;
    // fmt::format_to(std::back_inserter(wbuf), L"{:\u2022>5}", L"hi"); // Bullet point fill
    // EXPECT_EQ(wbuf, L"\u2022\u2022\u2022hi");
}

// Test case for combined formatting options (e.g., precision, width, alignment)
TEST_F(FmtWritePaddedTest, CombinedFormatting) {
    EXPECT_EQ(format_value("{:*>10.2f}", 123.456), "***123.46"); // Right align, fill, precision
    EXPECT_EQ(format_value("{:->8s}", "abc"), "-----abc"); // Right align, fill, string type
    EXPECT_EQ(format_value("{:0>5d}", 12), "00012"); // Right align, zero fill, decimal int
}

// Test case for zero padding with negative numbers (special handling for sign)
TEST_F(FmtWritePaddedTest, ZeroPaddingNegativeNumbers) {
    // Zero padding for negative numbers typically places the sign before the padding.
    EXPECT_EQ(format_value("{:0>5}", -12), "-0012");
    EXPECT_EQ(format_value("{:0>7}", -1234), "-01234");
    // If width is less than value size, no padding
    EXPECT_EQ(format_value("{:0>3}", -1234), "-1234");
}

// Test case for padding with different base numbers (e.g., hex, octal)
TEST_F(FmtWritePaddedTest, RightAlignmentDifferentBases) {
    EXPECT_EQ(format_value("{:>5x}", 255), "   ff");
    EXPECT_EQ(format_value("{:0>5x}", 255), "000ff");
    EXPECT_EQ(format_value("{:>7o}", 63), "     77");
    EXPECT_EQ(format_value("{:0>7o}", 63), "0000077");
}