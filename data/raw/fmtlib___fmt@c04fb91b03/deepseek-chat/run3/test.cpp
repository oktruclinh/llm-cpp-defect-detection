#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Test formatter specialization for format_enum types
enum class Color { Red, Green, Blue };
enum class Status { OK = 0, Warning = 1, Error = 2 };

// Test basic formatting of enum values
TEST(FormatEnumFormatterTest, BasicEnumFormatting) {
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Color::Green)), "1");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Color::Blue)), "2");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Status::OK)), "0");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Status::Warning)), "1");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Status::Error)), "2");
}

// Test formatting with width specification
TEST(FormatEnumFormatterTest, WidthSpecification) {
    EXPECT_EQ(fmt::format("{:4}", static_cast<int>(Color::Red)), "   0");
    EXPECT_EQ(fmt::format("{:4}", static_cast<int>(Color::Green)), "   1");
    EXPECT_EQ(fmt::format("{:<4}", static_cast<int>(Color::Blue)), "2   ");
    EXPECT_EQ(fmt::format("{:>4}", static_cast<int>(Color::Red)), "   0");
    EXPECT_EQ(fmt::format("{:^4}", static_cast<int>(Color::Green)), " 1  ");
}

// Test formatting with fill character
TEST(FormatEnumFormatterTest, FillCharacter) {
    EXPECT_EQ(fmt::format("{:*>4}", static_cast<int>(Color::Red)), "***0");
    EXPECT_EQ(fmt::format("{:*<4}", static_cast<int>(Color::Green)), "1***");
    EXPECT_EQ(fmt::format("{:*^4}", static_cast<int>(Color::Blue)), "*2**");
}

// Test formatting with precision (should be ignored for integers)
TEST(FormatEnumFormatterTest, PrecisionIgnored) {
    EXPECT_EQ(fmt::format("{:.5}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:.0}", static_cast<int>(Color::Green)), "1");
    EXPECT_EQ(fmt::format("{:.10}", static_cast<int>(Color::Blue)), "2");
}

// Test formatting with sign specification
TEST(FormatEnumFormatterTest, SignSpecification) {
    EXPECT_EQ(fmt::format("{:+}", static_cast<int>(Color::Red)), "+0");
    EXPECT_EQ(fmt::format("{:+}", static_cast<int>(Color::Green)), "+1");
    EXPECT_EQ(fmt::format("{: }", static_cast<int>(Color::Red)), " 0");
    EXPECT_EQ(fmt::format("{:-}", static_cast<int>(Color::Red)), "0");
}

// Test formatting with alternative form (#)
TEST(FormatEnumFormatterTest, AlternativeForm) {
    // For integers, # doesn't change decimal representation
    EXPECT_EQ(fmt::format("{:#}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:#}", static_cast<int>(Color::Green)), "1");
}

// Test formatting with zero padding
TEST(FormatEnumFormatterTest, ZeroPadding) {
    EXPECT_EQ(fmt::format("{:04}", static_cast<int>(Color::Red)), "0000");
    EXPECT_EQ(fmt::format("{:04}", static_cast<int>(Color::Green)), "0001");
    EXPECT_EQ(fmt::format("{:04}", static_cast<int>(Color::Blue)), "0002");
}

// Test formatting with dynamic width
TEST(FormatEnumFormatterTest, DynamicWidth) {
    EXPECT_EQ(fmt::format("{:{}}", static_cast<int>(Color::Red), 5), "    0");
    EXPECT_EQ(fmt::format("{:{}}", static_cast<int>(Color::Green), 3), "  1");
    EXPECT_EQ(fmt::format("{:*>{}}", static_cast<int>(Color::Blue), 4), "***2");
}

// Test formatting with dynamic precision (should be ignored)
TEST(FormatEnumFormatterTest, DynamicPrecision) {
    EXPECT_EQ(fmt::format("{:.{}}", static_cast<int>(Color::Red), 5), "0");
    EXPECT_EQ(fmt::format("{:.{}}", static_cast<int>(Color::Green), 0), "1");
}

// Test formatting with both width and precision
TEST(FormatEnumFormatterTest, WidthAndPrecision) {
    EXPECT_EQ(fmt::format("{:5.3}", static_cast<int>(Color::Red)), "    0");
    EXPECT_EQ(fmt::format("{:5.3}", static_cast<int>(Color::Green)), "    1");
}

// Test formatting with locale-specific formatting
TEST(FormatEnumFormatterTest, LocaleSpecific) {
    // For integers, locale doesn't change basic formatting
    EXPECT_EQ(fmt::format("{:L}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:L}", static_cast<int>(Color::Green)), "1");
}

// Test formatting with type specifier 'd' (decimal)
TEST(FormatEnumFormatterTest, DecimalTypeSpecifier) {
    EXPECT_EQ(fmt::format("{:d}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:d}", static_cast<int>(Color::Green)), "1");
    EXPECT_EQ(fmt::format("{:d}", static_cast<int>(Color::Blue)), "2");
}

// Test formatting with type specifier 'x' (hex)
TEST(FormatEnumFormatterTest, HexTypeSpecifier) {
    EXPECT_EQ(fmt::format("{:x}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:x}", static_cast<int>(Color::Green)), "1");
    EXPECT_EQ(fmt::format("{:x}", static_cast<int>(Color::Blue)), "2");
}

// Test formatting with type specifier 'X' (upper hex)
TEST(FormatEnumFormatterTest, UpperHexTypeSpecifier) {
    EXPECT_EQ(fmt::format("{:X}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:X}", static_cast<int>(Color::Green)), "1");
    EXPECT_EQ(fmt::format("{:X}", static_cast<int>(Color::Blue)), "2");
}

// Test formatting with type specifier 'o' (octal)
TEST(FormatEnumFormatterTest, OctalTypeSpecifier) {
    EXPECT_EQ(fmt::format("{:o}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:o}", static_cast<int>(Color::Green)), "1");
    EXPECT_EQ(fmt::format("{:o}", static_cast<int>(Color::Blue)), "2");
}

// Test formatting with type specifier 'b' (binary)
TEST(FormatEnumFormatterTest, BinaryTypeSpecifier) {
    EXPECT_EQ(fmt::format("{:b}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:b}", static_cast<int>(Color::Green)), "1");
    EXPECT_EQ(fmt::format("{:b}", static_cast<int>(Color::Blue)), "10");
}

// Test formatting with type specifier 'B' (upper binary)
TEST(FormatEnumFormatterTest, UpperBinaryTypeSpecifier) {
    EXPECT_EQ(fmt::format("{:B}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:B}", static_cast<int>(Color::Green)), "1");
    EXPECT_EQ(fmt::format("{:B}", static_cast<int>(Color::Blue)), "10");
}

// Test formatting with multiple arguments
TEST(FormatEnumFormatterTest, MultipleArguments) {
    EXPECT_EQ(fmt::format("{} {} {}", static_cast<int>(Color::Red), static_cast<int>(Color::Green), static_cast<int>(Color::Blue)), "0 1 2");
}

// Test formatting with positional arguments
TEST(FormatEnumFormatterTest, PositionalArguments) {
    EXPECT_EQ(fmt::format("{2} {0} {1}", static_cast<int>(Color::Red), static_cast<int>(Color::Green), static_cast<int>(Color::Blue)), "2 0 1");
}

// Test formatting with negative enum values (if applicable)
enum class SignedEnum { Min = -10, Zero = 0, Max = 10 };
TEST(FormatEnumFormatterTest, NegativeEnumValues) {
    EXPECT_EQ(fmt::format("{}", static_cast<int>(SignedEnum::Min)), "-10");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(SignedEnum::Zero)), "0");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(SignedEnum::Max)), "10");
    EXPECT_EQ(fmt::format("{:+}", static_cast<int>(SignedEnum::Min)), "-10");
    EXPECT_EQ(fmt::format("{:+}", static_cast<int>(SignedEnum::Max)), "+10");
}

// Test formatting with large enum values
enum class LargeEnum : unsigned long long { 
    Small = 0, 
    Large = std::numeric_limits<unsigned long long>::max() 
};
TEST(FormatEnumFormatterTest, LargeEnumValues) {
    EXPECT_EQ(fmt::format("{}", static_cast<unsigned long long>(LargeEnum::Small)), "0");
    EXPECT_EQ(fmt::format("{}", static_cast<unsigned long long>(LargeEnum::Large)), 
              std::to_string(std::numeric_limits<unsigned long long>::max()));
}

// Test formatting with zero-width
TEST(FormatEnumFormatterTest, ZeroWidth) {
    EXPECT_EQ(fmt::format("{:0}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:0}", static_cast<int>(Color::Green)), "1");
}

// Test formatting with very large width
TEST(FormatEnumFormatterTest, LargeWidth) {
    EXPECT_EQ(fmt::format("{:10}", static_cast<int>(Color::Red)), "         0");
    EXPECT_EQ(fmt::format("{:10}", static_cast<int>(Color::Green)), "         1");
}

// Test formatting with empty format string
TEST(FormatEnumFormatterTest, EmptyFormatString) {
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Color::Green)), "1");
}

// Test formatting with format string containing only width
TEST(FormatEnumFormatterTest, WidthOnly) {
    EXPECT_EQ(fmt::format("{:5}", static_cast<int>(Color::Red)), "    0");
    EXPECT_EQ(fmt::format("{:5}", static_cast<int>(Color::Green)), "    1");
}

// Test formatting with format string containing only fill and width
TEST(FormatEnumFormatterTest, FillAndWidthOnly) {
    EXPECT_EQ(fmt::format("{:*<5}", static_cast<int>(Color::Red)), "0****");
    EXPECT_EQ(fmt::format("{:*<5}", static_cast<int>(Color::Green)), "1****");
}

// Test formatting with format string containing only alignment
TEST(FormatEnumFormatterTest, AlignmentOnly) {
    EXPECT_EQ(fmt::format("{:<}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:>}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:^}", static_cast<int>(Color::Red)), "0");
}