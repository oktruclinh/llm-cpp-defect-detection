#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Test formatting of enum types through the public API
// The focal function handles format_enum types by converting them to int
// and applying format specs

enum class Color { Red = 0, Green = 1, Blue = 2 };
enum OldStyle { A = 10, B = 20, C = 30 };

// Test basic enum formatting (default format)
TEST(EnumFormatterTest, BasicEnumFormatting) {
    EXPECT_EQ(fmt::format("{}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{}", Color::Blue), "2");
    EXPECT_EQ(fmt::format("{}", OldStyle::A), "10");
    EXPECT_EQ(fmt::format("{}", OldStyle::B), "20");
    EXPECT_EQ(fmt::format("{}", OldStyle::C), "30");
}

// Test enum formatting with width specifier
TEST(EnumFormatterTest, EnumFormattingWithWidth) {
    EXPECT_EQ(fmt::format("{:4}", Color::Red), "   0");
    EXPECT_EQ(fmt::format("{:4}", Color::Green), "   1");
    EXPECT_EQ(fmt::format("{:4}", OldStyle::A), "  10");
    EXPECT_EQ(fmt::format("{:4}", OldStyle::B), "  20");
}

// Test enum formatting with fill and alignment
TEST(EnumFormatterTest, EnumFormattingWithFillAndAlign) {
    EXPECT_EQ(fmt::format("{:*<4}", Color::Red), "0***");
    EXPECT_EQ(fmt::format("{:*>4}", Color::Green), "***1");
    EXPECT_EQ(fmt::format("{:*^4}", Color::Blue), "*2**");
    EXPECT_EQ(fmt::format("{:*<5}", OldStyle::A), "10***");
    EXPECT_EQ(fmt::format("{:*>5}", OldStyle::B), "***20");
    EXPECT_EQ(fmt::format("{:*^5}", OldStyle::C), "*30**");
}

// Test enum formatting with sign specifier (should work like int)
TEST(EnumFormatterTest, EnumFormattingWithSign) {
    EXPECT_EQ(fmt::format("{:+}", Color::Red), "+0");
    EXPECT_EQ(fmt::format("{:+}", Color::Green), "+1");
    EXPECT_EQ(fmt::format("{: }", Color::Blue), " 2");
    EXPECT_EQ(fmt::format("{:-}", Color::Red), "0");
}

// Test enum formatting with base specifiers
TEST(EnumFormatterTest, EnumFormattingWithBase) {
    EXPECT_EQ(fmt::format("{:x}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{:x}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{:x}", Color::Blue), "2");
    EXPECT_EQ(fmt::format("{:X}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{:o}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{:b}", Color::Blue), "10");
    EXPECT_EQ(fmt::format("{:#x}", Color::Red), "0x0");
    EXPECT_EQ(fmt::format("{:#x}", Color::Green), "0x1");
}

// Test enum formatting with precision (should work like int)
TEST(EnumFormatterTest, EnumFormattingWithPrecision) {
    EXPECT_EQ(fmt::format("{:.5}", Color::Red), "00000");
    EXPECT_EQ(fmt::format("{:.5}", Color::Green), "00001");
    EXPECT_EQ(fmt::format("{:.3}", OldStyle::A), "010");
    EXPECT_EQ(fmt::format("{:.3}", OldStyle::B), "020");
}

// Test enum formatting with zero padding
TEST(EnumFormatterTest, EnumFormattingWithZeroPadding) {
    EXPECT_EQ(fmt::format("{:05}", Color::Red), "00000");
    EXPECT_EQ(fmt::format("{:05}", Color::Green), "00001");
    EXPECT_EQ(fmt::format("{:05}", OldStyle::A), "00010");
    EXPECT_EQ(fmt::format("{:05}", OldStyle::B), "00020");
}

// Test enum formatting with large enum values
enum LargeEnum : int { LargeMax = std::numeric_limits<int>::max(), LargeMin = std::numeric_limits<int>::min() };

TEST(EnumFormatterTest, EnumFormattingLargeValues) {
    EXPECT_EQ(fmt::format("{}", LargeMax), std::to_string(std::numeric_limits<int>::max()));
    EXPECT_EQ(fmt::format("{}", LargeMin), std::to_string(std::numeric_limits<int>::min()));
    EXPECT_EQ(fmt::format("{:x}", LargeMax), "7fffffff");
    EXPECT_EQ(fmt::format("{:x}", LargeMin), "80000000");
}

// Test enum formatting with negative enum values
enum SignedEnum : int { NegOne = -1, NegTwo = -2, Zero = 0 };

TEST(EnumFormatterTest, EnumFormattingNegativeValues) {
    EXPECT_EQ(fmt::format("{}", NegOne), "-1");
    EXPECT_EQ(fmt::format("{}", NegTwo), "-2");
    EXPECT_EQ(fmt::format("{}", Zero), "0");
    EXPECT_EQ(fmt::format("{:+}", NegOne), "-1");
    EXPECT_EQ(fmt::format("{: }", NegOne), "-1");
    EXPECT_EQ(fmt::format("{:05}", NegOne), "-0001");
    EXPECT_EQ(fmt::format("{:05}", NegTwo), "-0002");
}

// Test enum formatting with format_to
TEST(EnumFormatterTest, EnumFormattingToOutput) {
    std::string out;
    fmt::format_to(std::back_inserter(out), "{}", Color::Red);
    EXPECT_EQ(out, "0");
    
    out.clear();
    fmt::format_to(std::back_inserter(out), "{:04}", Color::Green);
    EXPECT_EQ(out, "0001");
}

// Test enum formatting with format_to_n
TEST(EnumFormatterTest, EnumFormattingToN) {
    std::string out(10, '\0');
    auto result = fmt::format_to_n(out.data(), 3, "{}", Color::Blue);
    *result.out = '\0';
    EXPECT_EQ(std::string(out.data()), "2");
    
    std::fill(out.begin(), out.end(), '\0');
    result = fmt::format_to_n(out.data(), 5, "{:04}", Color::Red);
    *result.out = '\0';
    EXPECT_EQ(std::string(out.data()), "0000");
}

// Test enum formatting with formatted_size
TEST(EnumFormatterTest, EnumFormattedSize) {
    EXPECT_EQ(fmt::formatted_size("{}", Color::Red), 1);
    EXPECT_EQ(fmt::formatted_size("{:04}", Color::Green), 4);
    EXPECT_EQ(fmt::formatted_size("{:x}", Color::Blue), 1);
}

// Test enum formatting with print (just verify it compiles and doesn't crash)
TEST(EnumFormatterTest, EnumPrint) {
    EXPECT_NO_THROW(fmt::print("{}", Color::Red));
    EXPECT_NO_THROW(fmt::print("{:04}", Color::Green));
}

// Test enum formatting with multiple arguments
TEST(EnumFormatterTest, MultipleEnumArguments) {
    EXPECT_EQ(fmt::format("{} {} {}", Color::Red, Color::Green, Color::Blue), "0 1 2");
    EXPECT_EQ(fmt::format("{:2} {:2} {:2}", Color::Red, Color::Green, Color::Blue), " 0  1  2");
}

// Test enum formatting with mixed types
TEST(EnumFormatterTest, MixedEnumAndIntFormatting) {
    EXPECT_EQ(fmt::format("{} {} {}", Color::Red, 42, Color::Blue), "0 42 2");
    EXPECT_EQ(fmt::format("{:04} {:04} {:04}", Color::Red, 42, Color::Blue), "0000 0042 0002");
}

// Test enum formatting with zero value
TEST(EnumFormatterTest, EnumZeroValue) {
    enum ZeroEnum : int { ZeroVal = 0 };
    EXPECT_EQ(fmt::format("{}", ZeroVal), "0");
    EXPECT_EQ(fmt::format("{:04}", ZeroVal), "0000");
    EXPECT_EQ(fmt::format("{:#x}", ZeroVal), "0x0");
}

// Test enum formatting with all bits set
TEST(EnumFormatterTest, EnumAllBitsSet) {
    enum AllBitsEnum : unsigned int { AllBits = 0xFFFFFFFF };
    EXPECT_EQ(fmt::format("{}", static_cast<int>(AllBits)), "-1");
    EXPECT_EQ(fmt::format("{:x}", static_cast<int>(AllBits)), "ffffffff");
}