#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>
#include <cmath>

// Test formatting of enums via the formatter specialization that delegates to formatter<int>
// The focal function is the formatter::format method that handles enum types.

enum class Color { Red = 0, Green = 1, Blue = 2 };
enum class Flags : unsigned { None = 0, Read = 1, Write = 2, Execute = 4 };
enum OldStyle { A = 10, B = 20, C = 30 };

// Test basic enum formatting with default format spec
TEST(EnumFormatterTest, DefaultFormat) {
    EXPECT_EQ(fmt::format("{}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{}", Color::Blue), "2");
    EXPECT_EQ(fmt::format("{}", Flags::None), "0");
    EXPECT_EQ(fmt::format("{}", Flags::Read), "1");
    EXPECT_EQ(fmt::format("{}", Flags::Write), "2");
    EXPECT_EQ(fmt::format("{}", Flags::Execute), "4");
    EXPECT_EQ(fmt::format("{}", OldStyle::A), "10");
    EXPECT_EQ(fmt::format("{}", OldStyle::B), "20");
    EXPECT_EQ(fmt::format("{}", OldStyle::C), "30");
}

// Test enum formatting with width and alignment
TEST(EnumFormatterTest, WidthAndAlignment) {
    EXPECT_EQ(fmt::format("{:5}", Color::Red), "    0");
    EXPECT_EQ(fmt::format("{:<5}", Color::Red), "0    ");
    EXPECT_EQ(fmt::format("{:>5}", Color::Red), "    0");
    EXPECT_EQ(fmt::format("{:^5}", Color::Red), "  0  ");
    EXPECT_EQ(fmt::format("{:10}", Flags::Execute), "         4");
    EXPECT_EQ(fmt::format("{:<10}", Flags::Execute), "4         ");
}

// Test enum formatting with fill character
TEST(EnumFormatterTest, FillCharacter) {
    EXPECT_EQ(fmt::format("{:*<5}", Color::Green), "1****");
    EXPECT_EQ(fmt::format("{:*>5}", Color::Green), "****1");
    EXPECT_EQ(fmt::format("{:*^5}", Color::Green), "**1**");
    EXPECT_EQ(fmt::format("{:0>5}", Color::Blue), "00002");
}

// Test enum formatting with base prefixes (hex, octal, binary)
TEST(EnumFormatterTest, BaseFormatting) {
    EXPECT_EQ(fmt::format("{:x}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{:x}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{:x}", Color::Blue), "2");
    EXPECT_EQ(fmt::format("{:X}", Color::Blue), "2");
    EXPECT_EQ(fmt::format("{:o}", Color::Blue), "2");
    EXPECT_EQ(fmt::format("{:b}", Color::Blue), "10");
    EXPECT_EQ(fmt::format("{:#x}", Color::Blue), "0x2");
    EXPECT_EQ(fmt::format("{:#X}", Color::Blue), "0X2");
    EXPECT_EQ(fmt::format("{:#o}", Color::Blue), "02");
    EXPECT_EQ(fmt::format("{:#b}", Color::Blue), "0b10");
    EXPECT_EQ(fmt::format("{:#x}", Flags::Execute), "0x4");
    EXPECT_EQ(fmt::format("{:#b}", Flags::Execute), "0b100");
}

// Test enum formatting with sign specifiers
TEST(EnumFormatterTest, SignSpecifiers) {
    EXPECT_EQ(fmt::format("{:+}", Color::Red), "+0");
    EXPECT_EQ(fmt::format("{:+}", Color::Green), "+1");
    EXPECT_EQ(fmt::format("{: }", Color::Red), " 0");
    EXPECT_EQ(fmt::format("{: }", Color::Green), " 1");
    EXPECT_EQ(fmt::format("{:-}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{:-}", Color::Green), "1");
}

// Test enum formatting with zero padding
TEST(EnumFormatterTest, ZeroPadding) {
    EXPECT_EQ(fmt::format("{:05}", Color::Red), "00000");
    EXPECT_EQ(fmt::format("{:05}", Color::Green), "00001");
    EXPECT_EQ(fmt::format("{:05}", Color::Blue), "00002");
    EXPECT_EQ(fmt::format("{:010}", Flags::Execute), "0000000004");
}

// Test enum formatting with precision (integer precision)
TEST(EnumFormatterTest, Precision) {
    EXPECT_EQ(fmt::format("{:.5}", Color::Red), "00000");
    EXPECT_EQ(fmt::format("{:.5}", Color::Green), "00001");
    EXPECT_EQ(fmt::format("{:.5}", Color::Blue), "00002");
    EXPECT_EQ(fmt::format("{:.3}", Flags::Execute), "004");
}

// Test enum formatting with combined specifiers
TEST(EnumFormatterTest, CombinedSpecifiers) {
    EXPECT_EQ(fmt::format("{:*>+10}", Color::Green), "*******+1");
    EXPECT_EQ(fmt::format("{:*>+#10x}", Color::Blue), "*****0x2");
    EXPECT_EQ(fmt::format("{:0=+#10x}", Color::Blue), "0x0000002");
    EXPECT_EQ(fmt::format("{:*>+#10b}", Flags::Execute), "***0b100");
}

// Test formatting of enum with large values (if underlying type allows)
TEST(EnumFormatterTest, LargeEnumValues) {
    enum class LargeEnum : long long { Max = std::numeric_limits<long long>::max() };
    EXPECT_EQ(fmt::format("{}", LargeEnum::Max), std::to_string(std::numeric_limits<long long>::max()));
    EXPECT_EQ(fmt::format("{:#x}", LargeEnum::Max), "0x7fffffffffffffff");
}

// Test formatting of enum with negative values (if underlying type is signed)
TEST(EnumFormatterTest, NegativeEnumValues) {
    enum class SignedEnum : int { Neg = -42, Zero = 0, Pos = 42 };
    EXPECT_EQ(fmt::format("{}", SignedEnum::Neg), "-42");
    EXPECT_EQ(fmt::format("{:+}", SignedEnum::Neg), "-42");
    EXPECT_EQ(fmt::format("{: }", SignedEnum::Neg), "-42");
    EXPECT_EQ(fmt::format("{:05}", SignedEnum::Neg), "-0042");
    EXPECT_EQ(fmt::format("{:#x}", SignedEnum::Neg), "-0x2a");
}

// Test formatting of enum with zero value
TEST(EnumFormatterTest, ZeroValue) {
    enum class ZeroEnum { Zero = 0 };
    EXPECT_EQ(fmt::format("{}", ZeroEnum::Zero), "0");
    EXPECT_EQ(fmt::format("{:05}", ZeroEnum::Zero), "00000");
    EXPECT_EQ(fmt::format("{:#x}", ZeroEnum::Zero), "0x0");
    EXPECT_EQ(fmt::format("{:#b}", ZeroEnum::Zero), "0b0");
}

// Test formatting of enum with maximum unsigned value
TEST(EnumFormatterTest, MaxUnsignedValue) {
    enum class UnsignedEnum : unsigned { Max = std::numeric_limits<unsigned>::max() };
    EXPECT_EQ(fmt::format("{}", UnsignedEnum::Max), std::to_string(std::numeric_limits<unsigned>::max()));
    EXPECT_EQ(fmt::format("{:#x}", UnsignedEnum::Max), "0xffffffff");
}

// Test that formatting works with fmt::format_to and iterators
TEST(EnumFormatterTest, FormatToIterator) {
    std::string out;
    auto it = std::back_inserter(out);
    fmt::format_to(it, "{}", Color::Blue);
    EXPECT_EQ(out, "2");
    
    out.clear();
    fmt::format_to(it, "{:05}", Color::Red);
    EXPECT_EQ(out, "00000");
}

// Test that formatting works with fmt::sprintf style (if applicable)
TEST(EnumFormatterTest, SprintfStyle) {
    EXPECT_EQ(fmt::sprintf("%d", Color::Red), "0");
    EXPECT_EQ(fmt::sprintf("%05d", Color::Green), "00001");
    EXPECT_EQ(fmt::sprintf("%#x", Color::Blue), "0x2");
}

// Test formatting of enum in a wider context (e.g., within a string)
TEST(EnumFormatterTest, InContext) {
    EXPECT_EQ(fmt::format("Color is {}", Color::Red), "Color is 0");
    EXPECT_EQ(fmt::format("Flag value: {:#010b}", Flags::Execute), "Flag value: 0b00000100");
}

// Test that the formatter works with compile-time format string checking
TEST(EnumFormatterTest, CompileTimeCheck) {
    // This should compile and run without errors
    auto result = fmt::format(FMT_STRING("{}"), Color::Green);
    EXPECT_EQ(result, "1");
}

// Test edge case: enum with value that requires all digits
TEST(EnumFormatterTest, AllDigits) {
    enum class AllDigits : int { Value = 1234567890 };
    EXPECT_EQ(fmt::format("{}", AllDigits::Value), "1234567890");
    EXPECT_EQ(fmt::format("{:#x}", AllDigits::Value), "0x499602d2");
}

// Test that the formatter handles the case where precision is larger than width
TEST(EnumFormatterTest, PrecisionLargerThanWidth) {
    EXPECT_EQ(fmt::format("{:3.5}", Color::Blue), "00002");
    EXPECT_EQ(fmt::format("{:5.3}", Color::Blue), "  002");
}