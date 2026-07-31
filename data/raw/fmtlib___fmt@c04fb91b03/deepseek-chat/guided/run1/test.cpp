#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Test formatter specialization for format_enum types through public API
// The focal function handles formatting of enum types with format specifications

enum class Color { Red, Green, Blue };
enum class Status { Ok = 0, Warning = 1, Error = 2 };

// Test basic enum formatting
TEST(FormatEnumTest, BasicEnumFormatting) {
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Color::Green)), "1");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Color::Blue)), "2");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Status::Ok)), "0");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Status::Warning)), "1");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Status::Error)), "2");
}

// Test enum formatting with width specification
TEST(FormatEnumTest, EnumWithWidth) {
    EXPECT_EQ(fmt::format("{:4}", static_cast<int>(Color::Red)), "   0");
    EXPECT_EQ(fmt::format("{:4}", static_cast<int>(Color::Green)), "   1");
    EXPECT_EQ(fmt::format("{:<4}", static_cast<int>(Color::Blue)), "2   ");
    EXPECT_EQ(fmt::format("{:>4}", static_cast<int>(Status::Ok)), "   0");
    EXPECT_EQ(fmt::format("{:^4}", static_cast<int>(Status::Warning)), " 1  ");
}

// Test enum formatting with fill and alignment
TEST(FormatEnumTest, EnumWithFillAndAlign) {
    EXPECT_EQ(fmt::format("{:*<5}", static_cast<int>(Color::Red)), "0****");
    EXPECT_EQ(fmt::format("{:*>5}", static_cast<int>(Color::Green)), "****1");
    EXPECT_EQ(fmt::format("{:*^5}", static_cast<int>(Color::Blue)), "**2**");
}

// Test enum formatting with zero padding
TEST(FormatEnumTest, EnumWithZeroPadding) {
    EXPECT_EQ(fmt::format("{:04}", static_cast<int>(Color::Red)), "0000");
    EXPECT_EQ(fmt::format("{:04}", static_cast<int>(Color::Green)), "0001");
    EXPECT_EQ(fmt::format("{:04}", static_cast<int>(Color::Blue)), "0002");
}

// Test enum formatting with sign (should not affect unsigned enums)
TEST(FormatEnumTest, EnumWithSign) {
    EXPECT_EQ(fmt::format("{:+}", static_cast<int>(Color::Red)), "+0");
    EXPECT_EQ(fmt::format("{:+}", static_cast<int>(Color::Green)), "+1");
    EXPECT_EQ(fmt::format("{:-}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{: }", static_cast<int>(Color::Red)), " 0");
}

// Test enum formatting with alternate form (should not affect integers)
TEST(FormatEnumTest, EnumWithAlternateForm) {
    EXPECT_EQ(fmt::format("{:#x}", static_cast<int>(Color::Red)), "0x0");
    EXPECT_EQ(fmt::format("{:#x}", static_cast<int>(Color::Green)), "0x1");
    EXPECT_EQ(fmt::format("{:#o}", static_cast<int>(Color::Red)), "00");
}

// Test enum formatting with different bases
TEST(FormatEnumTest, EnumWithDifferentBases) {
    EXPECT_EQ(fmt::format("{:d}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:x}", static_cast<int>(Color::Green)), "1");
    EXPECT_EQ(fmt::format("{:X}", static_cast<int>(Color::Blue)), "2");
    EXPECT_EQ(fmt::format("{:o}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:b}", static_cast<int>(Color::Green)), "1");
}

// Test enum formatting with precision (should be ignored for integers)
TEST(FormatEnumTest, EnumWithPrecision) {
    EXPECT_EQ(fmt::format("{:.5}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:.0}", static_cast<int>(Color::Green)), "1");
}

// Test enum formatting with large width
TEST(FormatEnumTest, EnumWithLargeWidth) {
    EXPECT_EQ(fmt::format("{:10}", static_cast<int>(Color::Red)), "         0");
    EXPECT_EQ(fmt::format("{:10}", static_cast<int>(Color::Blue)), "         2");
}

// Test enum formatting with zero width
TEST(FormatEnumTest, EnumWithZeroWidth) {
    EXPECT_EQ(fmt::format("{:0}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:0}", static_cast<int>(Color::Green)), "1");
}

// Test enum formatting with negative width (should be treated as left-aligned)
TEST(FormatEnumTest, EnumWithNegativeWidth) {
    EXPECT_EQ(fmt::format("{:-4}", static_cast<int>(Color::Red)), "0   ");
    EXPECT_EQ(fmt::format("{:-4}", static_cast<int>(Color::Green)), "1   ");
}

// Test enum formatting with maximum enum value
TEST(FormatEnumTest, EnumWithMaxValue) {
    enum class LargeEnum : int { Max = std::numeric_limits<int>::max() };
    EXPECT_EQ(fmt::format("{}", static_cast<int>(LargeEnum::Max)), std::to_string(std::numeric_limits<int>::max()));
}

// Test enum formatting with minimum enum value
TEST(FormatEnumTest, EnumWithMinValue) {
    enum class NegativeEnum : int { Min = std::numeric_limits<int>::min() };
    EXPECT_EQ(fmt::format("{}", static_cast<int>(NegativeEnum::Min)), std::to_string(std::numeric_limits<int>::min()));
}

// Test enum formatting with zero enum value
TEST(FormatEnumTest, EnumWithZeroValue) {
    enum class ZeroEnum : int { Zero = 0 };
    EXPECT_EQ(fmt::format("{}", static_cast<int>(ZeroEnum::Zero)), "0");
}

// Test enum formatting with multiple arguments
TEST(FormatEnumTest, MultipleEnumArguments) {
    EXPECT_EQ(fmt::format("{} {} {}", static_cast<int>(Color::Red), static_cast<int>(Color::Green), static_cast<int>(Color::Blue)), "0 1 2");
    EXPECT_EQ(fmt::format("{:3} {:3} {:3}", static_cast<int>(Color::Red), static_cast<int>(Color::Green), static_cast<int>(Color::Blue)), "  0   1   2");
}

// Test enum formatting with mixed types
TEST(FormatEnumTest, EnumWithMixedTypes) {
    EXPECT_EQ(fmt::format("{} {} {}", static_cast<int>(Color::Red), 42, "hello"), "0 42 hello");
}

// Test enum formatting with format string errors
TEST(FormatEnumTest, EnumWithInvalidFormatSpec) {
    // Invalid format spec should throw
    EXPECT_THROW(fmt::format("{:invalid}", static_cast<int>(Color::Red)), fmt::format_error);
}

// Test enum formatting with empty format string
TEST(FormatEnumTest, EnumWithEmptyFormatString) {
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Color::Red)), "0");
}

// Test enum formatting with positional arguments
TEST(FormatEnumTest, EnumWithPositionalArgs) {
    EXPECT_EQ(fmt::format("{1} {0}", static_cast<int>(Color::Green), static_cast<int>(Color::Red)), "1 0");
    EXPECT_EQ(fmt::format("{0:4} {1:4}", static_cast<int>(Color::Red), static_cast<int>(Color::Blue)), "   0    2");
}