#include <gtest/gtest.h>
#include "fmt/core.h"
#include <string>
#include <variant>
#include <sstream>

// Test fixture for dynamic_formatter with variant
class DynamicFormatterTest : public ::testing::Test {
protected:
    using variant = std::variant<int, std::string>;
    
    void SetUp() override {
        // Setup code if needed
    }
};

// Test basic formatting of int variant
TEST_F(DynamicFormatterTest, FormatIntVariant) {
    variant v = 42;
    std::string result = fmt::format("{}", v);
    EXPECT_EQ(result, "42");
}

// Test basic formatting of string variant
TEST_F(DynamicFormatterTest, FormatStringVariant) {
    variant v = std::string("hello");
    std::string result = fmt::format("{}", v);
    EXPECT_EQ(result, "hello");
}

// Test formatting with width specifier
TEST_F(DynamicFormatterTest, FormatWithWidth) {
    variant v = 42;
    std::string result = fmt::format("{:5}", v);
    EXPECT_EQ(result, "   42");
}

// Test formatting with alignment
TEST_F(DynamicFormatterTest, FormatWithAlignment) {
    variant v = std::string("hi");
    std::string result = fmt::format("{:<5}", v);
    EXPECT_EQ(result, "hi   ");
    
    result = fmt::format("{:>5}", v);
    EXPECT_EQ(result, "   hi");
    
    result = fmt::format("{:^5}", v);
    EXPECT_EQ(result, " hi  ");
}

// Test formatting with fill character
TEST_F(DynamicFormatterTest, FormatWithFill) {
    variant v = 7;
    std::string result = fmt::format("{:*>5}", v);
    EXPECT_EQ(result, "****7");
}

// Test formatting with sign for int
TEST_F(DynamicFormatterTest, FormatWithSign) {
    variant v = 42;
    std::string result = fmt::format("{:+}", v);
    EXPECT_EQ(result, "+42");
    
    v = -42;
    result = fmt::format("{:+}", v);
    EXPECT_EQ(result, "-42");
    
    result = fmt::format("{: }", v);
    EXPECT_EQ(result, "-42");
    
    v = 42;
    result = fmt::format("{: }", v);
    EXPECT_EQ(result, " 42");
}

// Test formatting with precision for string
TEST_F(DynamicFormatterTest, FormatWithPrecision) {
    variant v = std::string("hello world");
    std::string result = fmt::format("{:.5}", v);
    EXPECT_EQ(result, "hello");
}

// Test formatting with hash flag for int
TEST_F(DynamicFormatterTest, FormatWithHash) {
    variant v = 255;
    std::string result = fmt::format("{:#x}", v);
    EXPECT_EQ(result, "0xff");
}

// Test formatting with multiple specifiers
TEST_F(DynamicFormatterTest, FormatWithMultipleSpecifiers) {
    variant v = 42;
    std::string result = fmt::format("{:+#010}", v);
    EXPECT_EQ(result, "+000000042");
}

// Test formatting with dynamic width
TEST_F(DynamicFormatterTest, FormatWithDynamicWidth) {
    variant v = 42;
    std::string result = fmt::format("{:{}}", v, 5);
    EXPECT_EQ(result, "   42");
}

// Test formatting with dynamic precision
TEST_F(DynamicFormatterTest, FormatWithDynamicPrecision) {
    variant v = std::string("hello world");
    std::string result = fmt::format("{:.{}}", v, 5);
    EXPECT_EQ(result, "hello");
}

// Test formatting with both dynamic width and precision
TEST_F(DynamicFormatterTest, FormatWithDynamicWidthAndPrecision) {
    variant v = std::string("hello world");
    std::string result = fmt::format("{:>{}.{}}", v, 10, 5);
    EXPECT_EQ(result, "     hello");
}

// Test formatting with zero flag
TEST_F(DynamicFormatterTest, FormatWithZeroFlag) {
    variant v = 42;
    std::string result = fmt::format("{:05}", v);
    EXPECT_EQ(result, "00042");
}

// Test formatting with minus flag
TEST_F(DynamicFormatterTest, FormatWithMinusFlag) {
    variant v = 42;
    std::string result = fmt::format("{:-5}", v);
    EXPECT_EQ(result, "42   ");
}

// Test formatting with space flag
TEST_F(DynamicFormatterTest, FormatWithSpaceFlag) {
    variant v = 42;
    std::string result = fmt::format("{: }", v);
    EXPECT_EQ(result, " 42");
}

// Test formatting with plus flag
TEST_F(DynamicFormatterTest, FormatWithPlusFlag) {
    variant v = 42;
    std::string result = fmt::format("{:+}", v);
    EXPECT_EQ(result, "+42");
}

// Test formatting with hash flag for octal
TEST_F(DynamicFormatterTest, FormatWithHashOctal) {
    variant v = 255;
    std::string result = fmt::format("{:#o}", v);
    EXPECT_EQ(result, "0377");
}

// Test formatting with hash flag for hex
TEST_F(DynamicFormatterTest, FormatWithHashHex) {
    variant v = 255;
    std::string result = fmt::format("{:#X}", v);
    EXPECT_EQ(result, "0XFF");
}

// Test formatting with center alignment and fill
TEST_F(DynamicFormatterTest, FormatCenterAlignmentWithFill) {
    variant v = std::string("test");
    std::string result = fmt::format("{:*^10}", v);
    EXPECT_EQ(result, "***test***");
}

// Test formatting with left alignment and fill
TEST_F(DynamicFormatterTest, FormatLeftAlignmentWithFill) {
    variant v = std::string("test");
    std::string result = fmt::format("{:*<10}", v);
    EXPECT_EQ(result, "test******");
}

// Test formatting with right alignment and fill
TEST_F(DynamicFormatterTest, FormatRightAlignmentWithFill) {
    variant v = std::string("test");
    std::string result = fmt::format("{:*>10}", v);
    EXPECT_EQ(result, "******test");
}

// Test formatting with multiple variants in one format string
TEST_F(DynamicFormatterTest, FormatMultipleVariants) {
    variant v1 = 42;
    variant v2 = std::string("hello");
    std::string result = fmt::format("{} {}", v1, v2);
    EXPECT_EQ(result, "42 hello");
}

// Test formatting with named arguments
TEST_F(DynamicFormatterTest, FormatWithNamedArgs) {
    variant v = 42;
    std::string result = fmt::format("{val}", fmt::arg("val", v));
    EXPECT_EQ(result, "42");
}

// Test formatting with empty string variant
TEST_F(DynamicFormatterTest, FormatEmptyStringVariant) {
    variant v = std::string("");
    std::string result = fmt::format("{}", v);
    EXPECT_EQ(result, "");
}

// Test formatting with zero int variant
TEST_F(DynamicFormatterTest, FormatZeroIntVariant) {
    variant v = 0;
    std::string result = fmt::format("{}", v);
    EXPECT_EQ(result, "0");
}

// Test formatting with negative int variant
TEST_F(DynamicFormatterTest, FormatNegativeIntVariant) {
    variant v = -42;
    std::string result = fmt::format("{}", v);
    EXPECT_EQ(result, "-42");
}

// Test formatting with large int variant
TEST_F(DynamicFormatterTest, FormatLargeIntVariant) {
    variant v = 1234567890;
    std::string result = fmt::format("{}", v);
    EXPECT_EQ(result, "1234567890");
}

// Test formatting with very long string variant
TEST_F(DynamicFormatterTest, FormatLongStringVariant) {
    std::string longStr(1000, 'a');
    variant v = longStr;
    std::string result = fmt::format("{}", v);
    EXPECT_EQ(result, longStr);
}

// Test formatting with special characters in string variant
TEST_F(DynamicFormatterTest, FormatSpecialCharsStringVariant) {
    variant v = std::string("hello\nworld\t!");
    std::string result = fmt::format("{}", v);
    EXPECT_EQ(result, "hello\nworld\t!");
}

// Test formatting with Unicode string variant
TEST_F(DynamicFormatterTest, FormatUnicodeStringVariant) {
    variant v = std::string("héllo wörld");
    std::string result = fmt::format("{}", v);
    EXPECT_EQ(result, "héllo wörld");
}

// Test formatting with precision truncation
TEST_F(DynamicFormatterTest, FormatPrecisionTruncation) {
    variant v = std::string("hello world");
    std::string result = fmt::format("{:.3}", v);
    EXPECT_EQ(result, "hel");
}

// Test formatting with width larger than content
TEST_F(DynamicFormatterTest, FormatWidthLargerThanContent) {
    variant v = 42;
    std::string result = fmt::format("{:10}", v);
    EXPECT_EQ(result, "        42");
}

// Test formatting with width smaller than content
TEST_F(DynamicFormatterTest, FormatWidthSmallerThanContent) {
    variant v = std::string("hello world");
    std::string result = fmt::format("{:5}", v);
    EXPECT_EQ(result, "hello world");
}

// Test formatting with zero width
TEST_F(DynamicFormatterTest, FormatZeroWidth) {
    variant v = 42;
    std::string result = fmt::format("{:0}", v);
    EXPECT_EQ(result, "42");
}

// Test formatting with negative width (should be treated as left alignment)
TEST_F(DynamicFormatterTest, FormatNegativeWidth) {
    variant v = 42;
    std::string result = fmt::format("{:-5}", v);
    EXPECT_EQ(result, "42   ");
}

// Test formatting with precision for int (should be ignored)
TEST_F(DynamicFormatterTest, FormatPrecisionForInt) {
    variant v = 42;
    std::string result = fmt::format("{:.5}", v);
    EXPECT_EQ(result, "42");
}

// Test formatting with type specifier for int
TEST_F(DynamicFormatterTest, FormatIntWithTypeSpecifier) {
    variant v = 255;
    std::string result = fmt::format("{:x}", v);
    EXPECT_EQ(result, "ff");
    
    result = fmt::format("{:X}", v);
    EXPECT_EQ(result, "FF");
    
    result = fmt::format("{:o}", v);
    EXPECT_EQ(result, "377");
    
    result = fmt::format("{:b}", v);
    EXPECT_EQ(result, "11111111");
}

// Test formatting with type specifier for string (should be ignored)
TEST_F(DynamicFormatterTest, FormatStringWithTypeSpecifier) {
    variant v = std::string("hello");
    std::string result = fmt::format("{:s}", v);
    EXPECT_EQ(result, "hello");
}

// Test formatting with mixed types in format string
TEST_F(DynamicFormatterTest, FormatMixedTypes) {
    variant v1 = 42;
    variant v2 = std::string("hello");
    int plainInt = 100;
    std::string plainStr = "world";
    
    std::string result = fmt::format("{} {} {} {}", v1, v2, plainInt, plainStr);
    EXPECT_EQ(result, "42 hello 100 world");
}

// Test formatting with nested format specifiers
TEST_F(DynamicFormatterTest, FormatNestedSpecifiers) {
    variant v = 42;
    std::string result = fmt::format("{:0{}}", v, 5);
    EXPECT_EQ(result, "00042");
}

// Test formatting with argument index
TEST_F(DynamicFormatterTest, FormatWithArgumentIndex) {
    variant v1 = 42;
    variant v2 = std::string("hello");
    std::string result = fmt::format("{1} {0}", v1, v2);
    EXPECT_EQ(result, "hello 42");
}

// Test formatting with repeated argument index
TEST_F(DynamicFormatterTest, FormatWithRepeatedArgumentIndex) {
    variant v = 42;
    std::string result = fmt::format("{0} {0} {0}", v);
    EXPECT_EQ(result, "42 42 42");
}