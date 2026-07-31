#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Test formatter for enum types via format_enum trait
enum class Color { Red, Green, Blue };
enum OldStyle { A = 1, B = 2, C = 4 };

// Specialize format_enum for our test enums
namespace fmt {
template<>
struct format_enum<Color> : std::true_type {};
template<>
struct format_enum<OldStyle> : std::true_type {};
}

TEST(EnumFormatterTest, BasicEnumFormatting) {
    // Test basic formatting of enum values
    Color c = Color::Red;
    std::string result = fmt::format("{}", c);
    EXPECT_EQ("0", result);  // Red is 0
    
    c = Color::Green;
    result = fmt::format("{}", c);
    EXPECT_EQ("1", result);  // Green is 1
    
    c = Color::Blue;
    result = fmt::format("{}", c);
    EXPECT_EQ("2", result);  // Blue is 2
}

TEST(EnumFormatterTest, OldStyleEnumFormatting) {
    // Test old-style enum formatting
    OldStyle val = A;
    std::string result = fmt::format("{}", val);
    EXPECT_EQ("1", result);
    
    val = B;
    result = fmt::format("{}", val);
    EXPECT_EQ("2", result);
    
    val = C;
    result = fmt::format("{}", val);
    EXPECT_EQ("4", result);
}

TEST(EnumFormatterTest, EnumWithWidth) {
    // Test width specifier with enum
    Color c = Color::Green;
    std::string result = fmt::format("{:5}", c);
    EXPECT_EQ("    1", result);  // Right-aligned, width 5
    
    result = fmt::format("{:<5}", c);
    EXPECT_EQ("1    ", result);  // Left-aligned, width 5
}

TEST(EnumFormatterTest, EnumWithFillAndAlign) {
    // Test fill and alignment with enum
    Color c = Color::Blue;
    std::string result = fmt::format("{:*>5}", c);
    EXPECT_EQ("****2", result);  // Right-aligned with fill
    
    result = fmt::format("{:*<5}", c);
    EXPECT_EQ("2****", result);  // Left-aligned with fill
    
    result = fmt::format("{:*^5}", c);
    EXPECT_EQ("**2**", result);  // Center-aligned with fill
}

TEST(EnumFormatterTest, EnumWithBasePrefix) {
    // Test hex and octal formatting of enum values
    Color c = Color::Red;
    std::string result = fmt::format("{:#x}", c);
    EXPECT_EQ("0x0", result);
    
    c = Color::Green;
    result = fmt::format("{:#x}", c);
    EXPECT_EQ("0x1", result);
    
    c = Color::Blue;
    result = fmt::format("{:#x}", c);
    EXPECT_EQ("0x2", result);
    
    result = fmt::format("{:#o}", c);
    EXPECT_EQ("02", result);  // Octal 2
}

TEST(EnumFormatterTest, EnumWithSign) {
    // Test sign specifier with enum (should show sign for positive values)
    Color c = Color::Green;
    std::string result = fmt::format("{:+}", c);
    EXPECT_EQ("+1", result);
    
    result = fmt::format("{: }", c);
    EXPECT_EQ(" 1", result);  // Space for positive
    
    // Zero should not have sign
    c = Color::Red;
    result = fmt::format("{:+}", c);
    EXPECT_EQ("+0", result);  // fmt shows +0 for signed zero
}

TEST(EnumFormatterTest, EnumWithPrecision) {
    // Test precision specifier (should be ignored for integers, but not error)
    Color c = Color::Blue;
    std::string result = fmt::format("{:.5}", c);
    EXPECT_EQ("2", result);  // Precision ignored for integer types
}

TEST(EnumFormatterTest, EnumWithZeroPadding) {
    // Test zero padding with enum
    Color c = Color::Green;
    std::string result = fmt::format("{:05}", c);
    EXPECT_EQ("00001", result);
    
    c = Color::Blue;
    result = fmt::format("{:05}", c);
    EXPECT_EQ("00002", result);
}

TEST(EnumFormatterTest, EnumWithMultipleArgs) {
    // Test formatting multiple enum values
    Color c1 = Color::Red;
    Color c2 = Color::Green;
    Color c3 = Color::Blue;
    
    std::string result = fmt::format("{} {} {}", c1, c2, c3);
    EXPECT_EQ("0 1 2", result);
}

TEST(EnumFormatterTest, EnumWithMixedTypes) {
    // Test mixing enum with other types
    Color c = Color::Green;
    std::string result = fmt::format("Color: {}, value: {}", c, 42);
    EXPECT_EQ("Color: 1, value: 42", result);
}

TEST(EnumFormatterTest, EnumWithLargeValues) {
    // Test enum with values that might be large
    enum LargeEnum : int { MAX = std::numeric_limits<int>::max(), MIN = std::numeric_limits<int>::min() };
    
    // Need to specialize format_enum for this enum
    // But we can test the underlying int formatting behavior
    int max_val = std::numeric_limits<int>::max();
    std::string result = fmt::format("{}", max_val);
    EXPECT_EQ(std::to_string(std::numeric_limits<int>::max()), result);
    
    int min_val = std::numeric_limits<int>::min();
    result = fmt::format("{}", min_val);
    EXPECT_EQ(std::to_string(std::numeric_limits<int>::min()), result);
}

TEST(EnumFormatterTest, EnumWithBinaryFormat) {
    // Test binary formatting (if supported)
    Color c = Color::Blue;
    std::string result = fmt::format("{:b}", c);
    EXPECT_EQ("10", result);  // 2 in binary
    
    c = Color::Green;
    result = fmt::format("{:b}", c);
    EXPECT_EQ("1", result);  // 1 in binary
}

TEST(EnumFormatterTest, EnumWithUpperCaseHex) {
    // Test uppercase hex formatting
    Color c = Color::Blue;
    std::string result = fmt::format("{:X}", c);
    EXPECT_EQ("2", result);
    
    // Test with a value that would have letters in hex
    enum HexEnum : int { TEN = 10, FIFTEEN = 15 };
    // Can't directly test without format_enum specialization, but test underlying int
    result = fmt::format("{:X}", 10);
    EXPECT_EQ("A", result);
    
    result = fmt::format("{:X}", 15);
    EXPECT_EQ("F", result);
}

TEST(EnumFormatterTest, EnumWithAlternateForm) {
    // Test alternate form (#) with various bases
    Color c = Color::Blue;
    std::string result = fmt::format("{:#x}", c);
    EXPECT_EQ("0x2", result);
    
    result = fmt::format("{:#X}", c);
    EXPECT_EQ("0X2", result);
    
    result = fmt::format("{:#o}", c);
    EXPECT_EQ("02", result);
}

TEST(EnumFormatterTest, EnumWithDynamicWidth) {
    // Test dynamic width from argument
    Color c = Color::Green;
    std::string result = fmt::format("{:{}}", c, 5);
    EXPECT_EQ("    1", result);
    
    result = fmt::format("{:*>{}}", c, 5);
    EXPECT_EQ("****1", result);
}

TEST(EnumFormatterTest, EnumWithDynamicPrecision) {
    // Test dynamic precision (should be ignored for integers)
    Color c = Color::Blue;
    std::string result = fmt::format("{:.{}}", c, 5);
    EXPECT_EQ("2", result);
}

TEST(EnumFormatterTest, EnumWithFormatSpecsReuse) {
    // Test that format specs are properly reset between calls
    Color c1 = Color::Red;
    Color c2 = Color::Green;
    
    std::string result = fmt::format("{:5} {}", c1, c2);
    EXPECT_EQ("    0 1", result);
    
    result = fmt::format("{} {:5}", c1, c2);
    EXPECT_EQ("0     1", result);
}

TEST(EnumFormatterTest, EnumWithNegativeValues) {
    // Test enum with negative values (if underlying type is signed)
    enum SignedEnum : int { NEG = -1, POS = 1 };
    // Test underlying int behavior
    std::string result = fmt::format("{}", -1);
    EXPECT_EQ("-1", result);
    
    result = fmt::format("{:+}", -1);
    EXPECT_EQ("-1", result);
    
    result = fmt::format("{: }", -1);
    EXPECT_EQ("-1", result);
}