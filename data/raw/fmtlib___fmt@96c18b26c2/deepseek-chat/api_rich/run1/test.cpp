#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <string>
#include <vector>

// Test the focal function's behavior through fmt::sprintf and related public API
// The focal function handles format specs with '0' flag for non-numeric types,
// setting sign to none, alt to false, and align to right (overriding align::numeric)

class PrintfFocalFunctionTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

// Test basic '0' flag behavior with strings (non-numeric type)
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithString) {
    // The '0' flag should be ignored for strings, align should be right
    EXPECT_EQ(fmt::sprintf("%010s", "hello"), "     hello");
    // Without '0' flag, default alignment for strings is left
    EXPECT_EQ(fmt::sprintf("%10s", "hello"), "     hello");
}

// Test '0' flag with characters (non-numeric type)
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithChar) {
    EXPECT_EQ(fmt::sprintf("%05c", 'A'), "    A");
    EXPECT_EQ(fmt::sprintf("%5c", 'A'), "    A");
}

// Test '0' flag with integers (numeric type - should still work)
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithInteger) {
    EXPECT_EQ(fmt::sprintf("%05d", 42), "00042");
    EXPECT_EQ(fmt::sprintf("%5d", 42), "   42");
}

// Test '0' flag with negative width (should be treated as left-align)
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithNegativeWidth) {
    EXPECT_EQ(fmt::sprintf("%-010s", "hello"), "hello     ");
    EXPECT_EQ(fmt::sprintf("%-10s", "hello"), "hello     ");
}

// Test '0' flag with explicit left alignment override
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithLeftAlign) {
    EXPECT_EQ(fmt::sprintf("%-010s", "test"), "test      ");
    EXPECT_EQ(fmt::sprintf("%-10s", "test"), "test      ");
}

// Test '0' flag with empty string
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithEmptyString) {
    EXPECT_EQ(fmt::sprintf("%05s", ""), "     ");
    EXPECT_EQ(fmt::sprintf("%5s", ""), "     ");
}

// Test '0' flag with very long string (no truncation)
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithLongString) {
    std::string long_str = "this is a very long string that exceeds the width";
    EXPECT_EQ(fmt::sprintf("%010s", long_str), long_str);
    EXPECT_EQ(fmt::sprintf("%10s", long_str), long_str);
}

// Test '0' flag with width smaller than content
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithSmallWidth) {
    EXPECT_EQ(fmt::sprintf("%02s", "hello"), "hello");
    EXPECT_EQ(fmt::sprintf("%2s", "hello"), "hello");
}

// Test '0' flag with width exactly equal to content
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithExactWidth) {
    EXPECT_EQ(fmt::sprintf("%05s", "hello"), "hello");
    EXPECT_EQ(fmt::sprintf("%5s", "hello"), "hello");
}

// Test '0' flag with width of 0
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithZeroWidth) {
    EXPECT_EQ(fmt::sprintf("%0s", "test"), "test");
    EXPECT_EQ(fmt::sprintf("%s", "test"), "test");
}

// Test '0' flag with pointer (non-numeric type)
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithPointer) {
    int x = 42;
    void* ptr = &x;
    std::string result = fmt::sprintf("%020p", ptr);
    // Pointer should not be zero-padded, just right-aligned
    EXPECT_NE(result.find("0x"), std::string::npos);
    EXPECT_EQ(result.size(), 20);
    // The first character should be space (right-aligned), not '0'
    EXPECT_EQ(result[0], ' ');
}

// Test '0' flag with bool (non-numeric type in printf context)
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithBool) {
    EXPECT_EQ(fmt::sprintf("%05s", true ? "true" : "false"), " true");
    EXPECT_EQ(fmt::sprintf("%5s", "true"), " true");
}

// Test '0' flag with multiple arguments
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithMultipleArgs) {
    EXPECT_EQ(fmt::sprintf("%05s %05d", "hi", 42), "   hi 00042");
    EXPECT_EQ(fmt::sprintf("%5s %5d", "hi", 42), "   hi    42");
}

// Test '0' flag with string containing spaces
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithSpacesInString) {
    EXPECT_EQ(fmt::sprintf("%010s", "a b"), "       a b");
    EXPECT_EQ(fmt::sprintf("%10s", "a b"), "       a b");
}

// Test '0' flag with special characters
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithSpecialChars) {
    EXPECT_EQ(fmt::sprintf("%05s", "\n\t"), " \n\t");
    EXPECT_EQ(fmt::sprintf("%5s", "\n\t"), " \n\t");
}

// Test that '0' flag does not affect numeric types' zero-padding behavior
TEST_F(PrintfFocalFunctionTest, ZeroFlagNumericVsNonNumeric) {
    // For integers, '0' flag causes zero-padding
    EXPECT_EQ(fmt::sprintf("%05d", 7), "00007");
    // For strings, '0' flag is ignored (right-aligned with spaces)
    EXPECT_EQ(fmt::sprintf("%05s", "7"), "    7");
}

// Test '0' flag with width specified via asterisk
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithAsteriskWidth) {
    EXPECT_EQ(fmt::sprintf("%0*s", 5, "hi"), "   hi");
    EXPECT_EQ(fmt::sprintf("%*s", 5, "hi"), "   hi");
}

// Test '0' flag with precision (should not affect alignment)
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithPrecision) {
    EXPECT_EQ(fmt::sprintf("%010.5s", "hello world"), "     hello");
    EXPECT_EQ(fmt::sprintf("%10.5s", "hello world"), "     hello");
}

// Test '0' flag with very large width
TEST_F(PrintfFocalFunctionTest, ZeroFlagWithLargeWidth) {
    std::string result = fmt::sprintf("%0100s", "test");
    EXPECT_EQ(result.size(), 100);
    EXPECT_EQ(result.substr(96), "test");
    EXPECT_EQ(result.substr(0, 96), std::string(96, ' '));
}

// Test that '0' flag with align::numeric is properly overridden for strings
TEST_F(PrintfFocalFunctionTest, ZeroFlagOverridesNumericAlign) {
    // This tests the specific case where align::numeric would be set
    // but should be overridden to align::right for non-numeric types
    EXPECT_EQ(fmt::sprintf("%010s", "x"), "         x");
    // Compare with numeric type where '0' flag works differently
    EXPECT_EQ(fmt::sprintf("%010d", 1), "0000000001");
}