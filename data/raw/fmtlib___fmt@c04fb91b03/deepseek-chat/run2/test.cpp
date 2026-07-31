#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Test formatter for format_enum types (e.g., enums with format specifiers)
enum class Color { Red, Green, Blue };
enum class Status { Ok = 0, Warning = 1, Error = 2 };

// Test basic formatting of enums
TEST(FormatEnumFormatterTest, BasicEnumFormatting) {
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Color::Green)), "1");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Color::Blue)), "2");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Status::Ok)), "0");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Status::Warning)), "1");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(Status::Error)), "2");
}

// Test formatting with width specifier
TEST(FormatEnumFormatterTest, WidthSpecifier) {
    EXPECT_EQ(fmt::format("{:4}", static_cast<int>(Color::Red)), "   0");
    EXPECT_EQ(fmt::format("{:4}", static_cast<int>(Color::Green)), "   1");
    EXPECT_EQ(fmt::format("{:4}", static_cast<int>(Status::Error)), "   2");
}

// Test formatting with fill and alignment
TEST(FormatEnumFormatterTest, FillAndAlignment) {
    EXPECT_EQ(fmt::format("{:*<4}", static_cast<int>(Color::Red)), "0***");
    EXPECT_EQ(fmt::format("{:*>4}", static_cast<int>(Color::Red)), "***0");
    EXPECT_EQ(fmt::format("{:*^4}", static_cast<int>(Color::Red)), "*0**");
}

// Test formatting with zero padding
TEST(FormatEnumFormatterTest, ZeroPadding) {
    EXPECT_EQ(fmt::format("{:04}", static_cast<int>(Color::Red)), "0000");
    EXPECT_EQ(fmt::format("{:04}", static_cast<int>(Color::Green)), "0001");
    EXPECT_EQ(fmt::format("{:04}", static_cast<int>(Status::Error)), "0002");
}

// Test formatting with sign specifier (should be ignored for unsigned enums)
TEST(FormatEnumFormatterTest, SignSpecifier) {
    EXPECT_EQ(fmt::format("{:+}", static_cast<int>(Color::Red)), "+0");
    EXPECT_EQ(fmt::format("{:-}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{: }", static_cast<int>(Color::Red)), " 0");
}

// Test formatting with alternative form (#)
TEST(FormatEnumFormatterTest, AlternativeForm) {
    // For enums, # should not add prefix (unlike integers)
    EXPECT_EQ(fmt::format("{:#}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:#x}", static_cast<int>(Color::Red)), "0");
}

// Test formatting with different bases
TEST(FormatEnumFormatterTest, DifferentBases) {
    EXPECT_EQ(fmt::format("{:d}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:x}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:X}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:o}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:b}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:B}", static_cast<int>(Color::Red)), "0");
}

// Test formatting with precision specifier (should be ignored for integers/enums)
TEST(FormatEnumFormatterTest, PrecisionSpecifier) {
    EXPECT_EQ(fmt::format("{:.5}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:.0}", static_cast<int>(Color::Green)), "1");
}

// Test formatting with locale-specific formatting
TEST(FormatEnumFormatterTest, LocaleSpecific) {
    EXPECT_EQ(fmt::format("{:L}", static_cast<int>(Color::Red)), "0");
    EXPECT_EQ(fmt::format("{:L}", static_cast<int>(Color::Green)), "1");
}

// Test formatting with dynamic width
TEST(FormatEnumFormatterTest, DynamicWidth) {
    EXPECT_EQ(fmt::format("{:{}}", static_cast<int>(Color::Red), 5), "    0");
    EXPECT_EQ(fmt::format("{:*>{}}", static_cast<int>(Color::Green), 4), "***1");
}

// Test formatting with dynamic precision
TEST(FormatEnumFormatterTest, DynamicPrecision) {
    EXPECT_EQ(fmt::format("{:.{}}", static_cast<int>(Color::Red), 3), "0");
    EXPECT_EQ(fmt::format("{:.{}}", static_cast<int>(Color::Blue), 0), "2");
}

// Test formatting with combined specifiers
TEST(FormatEnumFormatterTest, CombinedSpecifiers) {
    EXPECT_EQ(fmt::format("{:*>+6}", static_cast<int>(Color::Red)), "****+0");
    EXPECT_EQ(fmt::format("{:*>+6}", static_cast<int>(Color::Green)), "****+1");
    EXPECT_EQ(fmt::format("{:*>+6}", static_cast<int>(Color::Blue)), "****+2");
}

// Test formatting with zero value enum
TEST(FormatEnumFormatterTest, ZeroValueEnum) {
    EXPECT_EQ(fmt::format("{}", static_cast<int>(static_cast<Color>(0))), "0");
    EXPECT_EQ(fmt::format("{:04}", static_cast<int>(static_cast<Color>(0))), "0000");
}

// Test formatting with large enum values
TEST(FormatEnumFormatterTest, LargeEnumValues) {
    enum class LargeEnum : int { Value = 1234567890 };
    EXPECT_EQ(fmt::format("{}", static_cast<int>(LargeEnum::Value)), "1234567890");
    EXPECT_EQ(fmt::format("{:x}", static_cast<int>(LargeEnum::Value)), "499602d2");
}

// Test formatting with negative enum values (if underlying type is signed)
TEST(FormatEnumFormatterTest, NegativeEnumValues) {
    enum class SignedEnum : int { Negative = -42, Positive = 42 };
    EXPECT_EQ(fmt::format("{}", static_cast<int>(SignedEnum::Negative)), "-42");
    EXPECT_EQ(fmt::format("{:+}", static_cast<int>(SignedEnum::Negative)), "-42");
    EXPECT_EQ(fmt::format("{: }", static_cast<int>(SignedEnum::Negative)), "-42");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(SignedEnum::Positive)), "42");
}

// Test formatting with maximum and minimum values
TEST(FormatEnumFormatterTest, ExtremeValues) {
    enum class ExtremeEnum : unsigned int { 
        Min = 0, 
        Max = std::numeric_limits<unsigned int>::max() 
    };
    EXPECT_EQ(fmt::format("{}", static_cast<unsigned int>(ExtremeEnum::Min)), "0");
    EXPECT_EQ(fmt::format("{}", static_cast<unsigned int>(ExtremeEnum::Max)), 
              std::to_string(std::numeric_limits<unsigned int>::max()));
}

// Test formatting with char underlying type
TEST(FormatEnumFormatterTest, CharUnderlyingType) {
    enum class CharEnum : char { A = 'A', B = 'B' };
    EXPECT_EQ(fmt::format("{}", static_cast<int>(CharEnum::A)), "65");
    EXPECT_EQ(fmt::format("{}", static_cast<int>(CharEnum::B)), "66");
}

// Test formatting with short underlying type
TEST(FormatEnumFormatterTest, ShortUnderlyingType) {
    enum class ShortEnum : short { Value = 32767 };
    EXPECT_EQ(fmt::format("{}", static_cast<int>(ShortEnum::Value)), "32767");
}

// Test formatting with long long underlying type
TEST(FormatEnumFormatterTest, LongLongUnderlyingType) {
    enum class LongLongEnum : long long { Value = 123456789012345LL };
    EXPECT_EQ(fmt::format("{}", static_cast<long long>(LongLongEnum::Value)), "123456789012345");
}

// Test that format errors are properly reported
TEST(FormatEnumFormatterDeathTest, InvalidFormatString) {
    EXPECT_THROW(fmt::format("{:}", static_cast<int>(Color::Red)), fmt::format_error);
    EXPECT_THROW(fmt::format("{:{}", static_cast<int>(Color::Red)), fmt::format_error);
}

// Test formatting with multiple arguments
TEST(FormatEnumFormatterTest, MultipleArguments) {
    EXPECT_EQ(fmt::format("{} {} {}", static_cast<int>(Color::Red), static_cast<int>(Color::Green), static_cast<int>(Color::Blue)), "0 1 2");
    EXPECT_EQ(fmt::format("{:04} {:04} {:04}", static_cast<int>(Color::Red), static_cast<int>(Color::Green), static_cast<int>(Color::Blue)), 
              "0000 0001 0002");
}

// Test formatting with named arguments
TEST(FormatEnumFormatterTest, NamedArguments) {
    EXPECT_EQ(fmt::format("{color}", fmt::arg("color", static_cast<int>(Color::Red))), "0");
    EXPECT_EQ(fmt::format("{color:04}", fmt::arg("color", static_cast<int>(Color::Green))), "0001");
}