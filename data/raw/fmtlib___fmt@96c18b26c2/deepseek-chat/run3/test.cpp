#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <string>
#include <vector>
#include <stdexcept>

// Test the printf formatter's behavior for various format specifications,
// particularly focusing on the handling of non-numeric types with '0' flag,
// alignment, and sign/alt flags.

class PrintfFormatterTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

// Test that '0' flag is ignored for non-numeric types (strings)
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("%010s", "hello");
    // '0' flag should be ignored, alignment should be right with spaces
    EXPECT_EQ(result, "     hello");
}

// Test that '0' flag is ignored for characters
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForChar) {
    std::string result = fmt::sprintf("%05c", 'A');
    // '0' flag should be ignored, alignment should be right with spaces
    EXPECT_EQ(result, "    A");
}

// Test that '0' flag works for numeric types (should not be affected)
TEST_F(PrintfFormatterTest, ZeroFlagWorksForIntegers) {
    std::string result = fmt::sprintf("%05d", 42);
    EXPECT_EQ(result, "00042");
}

// Test default alignment for non-numeric types (should be right)
TEST_F(PrintfFormatterTest, DefaultAlignmentForString) {
    std::string result = fmt::sprintf("%10s", "test");
    EXPECT_EQ(result, "      test");
}

// Test left alignment for strings (overrides default)
TEST_F(PrintfFormatterTest, LeftAlignmentForString) {
    std::string result = fmt::sprintf("%-10s", "test");
    EXPECT_EQ(result, "test      ");
}

// Test that numeric alignment is overwritten to right for non-numeric types
TEST_F(PrintfFormatterTest, NumericAlignmentOverwrittenForString) {
    // Using '=' alignment (numeric) should be treated as right for strings
    std::string result = fmt::sprintf("%=10s", "test");
    EXPECT_EQ(result, "      test");
}

// Test sign flag is ignored for non-numeric types
TEST_F(PrintfFormatterTest, SignFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("%+10s", "hello");
    // '+' sign should be ignored for strings
    EXPECT_EQ(result, "     hello");
}

// Test space flag is ignored for non-numeric types
TEST_F(PrintfFormatterTest, SpaceFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("% 10s", "hello");
    // space sign should be ignored for strings
    EXPECT_EQ(result, "     hello");
}

// Test alt flag is ignored for non-numeric types
TEST_F(PrintfFormatterTest, AltFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("%#10s", "hello");
    // '#' alt flag should be ignored for strings
    EXPECT_EQ(result, "     hello");
}

// Test combination of flags that should all be ignored for strings
TEST_F(PrintfFormatterTest, AllFlagsIgnoredForStrings) {
    std::string result = fmt::sprintf("%+-#010s", "test");
    // All flags should be ignored, just width and right alignment with spaces
    EXPECT_EQ(result, "      test");
}

// Test that '0' flag is ignored for pointers
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForPointers) {
    int x = 42;
    std::string result = fmt::sprintf("%010p", static_cast<void*>(&x));
    // '0' flag should be ignored for pointers, alignment should be right
    EXPECT_NE(result.find("0x"), std::string::npos);
    EXPECT_EQ(result.size(), 12); // 10 width + "0x" prefix
    EXPECT_EQ(result[0], ' '); // Should be space-padded, not zero-padded
}

// Test empty string with width
TEST_F(PrintfFormatterTest, EmptyStringWithWidth) {
    std::string result = fmt::sprintf("%5s", "");
    EXPECT_EQ(result, "     ");
}

// Test string longer than width (no truncation)
TEST_F(PrintfFormatterTest, StringLongerThanWidth) {
    std::string result = fmt::sprintf("%3s", "hello");
    EXPECT_EQ(result, "hello");
}

// Test zero width for string
TEST_F(PrintfFormatterTest, ZeroWidthForString) {
    std::string result = fmt::sprintf("%0s", "test");
    EXPECT_EQ(result, "test");
}

// Test that '0' flag with precision for strings (precision should work, '0' ignored)
TEST_F(PrintfFormatterTest, ZeroFlagWithPrecisionForString) {
    std::string result = fmt::sprintf("%010.3s", "hello");
    // Precision limits to 3 chars, '0' flag ignored, right aligned with spaces
    EXPECT_EQ(result, "       hel");
}

// Test that sign and alt flags are properly reset for subsequent numeric formatting
TEST_F(PrintfFormatterTest, FlagResetAfterStringFormatting) {
    // First format a string (should ignore flags), then format an integer
    std::string result = fmt::sprintf("%+10s %+05d", "test", 42);
    EXPECT_EQ(result, "      test +0042");
}

// Test that '0' flag is ignored for boolean (non-numeric)
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForBool) {
    std::string result = fmt::sprintf("%05s", true ? "true" : "false");
    EXPECT_EQ(result, " true");
}

// Test that '0' flag is ignored for custom string-like types
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForStringView) {
    std::string_view sv = "world";
    std::string result = fmt::sprintf("%08s", sv);
    EXPECT_EQ(result, "   world");
}

// Test that alignment is properly set to right for non-numeric types with numeric alignment
TEST_F(PrintfFormatterTest, NumericAlignmentToRightForString) {
    // Using '=' alignment specifier (numeric) should become right for strings
    std::string result = fmt::sprintf("%=10s", "hi");
    EXPECT_EQ(result, "        hi");
}

// Test that all flags are cleared for non-numeric types in the same format string
TEST_F(PrintfFormatterTest, MultipleFormatsWithMixedTypes) {
    std::string result = fmt::sprintf("%+010s %+05d %+010s", "abc", 123, "def");
    // First string: flags ignored, right aligned with spaces
    // Integer: flags work normally
    // Second string: flags ignored again
    EXPECT_EQ(result, "       abc +0123       def");
}