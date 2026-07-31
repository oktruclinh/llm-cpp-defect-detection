#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Test formatting of enums via the formatter specialization that inherits from formatter<int>
// The focal function is the format() method of the formatter specialization for enums with
// internal::format_enum<T>::value == true. This is exercised through fmt::format().

enum class Color { Red = 0, Green = 1, Blue = 2 };
enum class Status { Ok = 200, NotFound = 404, Error = 500 };
enum class SmallEnum : char { A = -128, B = 0, C = 127 };
enum class UnsignedEnum : unsigned { Min = 0, Max = std::numeric_limits<unsigned>::max() };

// Test basic formatting of enum values
TEST(EnumFormatterTest, BasicFormatting) {
    EXPECT_EQ(fmt::format("{}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{}", Color::Blue), "2");
    EXPECT_EQ(fmt::format("{}", Status::Ok), "200");
    EXPECT_EQ(fmt::format("{}", Status::NotFound), "404");
    EXPECT_EQ(fmt::format("{}", Status::Error), "500");
}

// Test formatting with width and fill
TEST(EnumFormatterTest, WidthAndFill) {
    EXPECT_EQ(fmt::format("{:4}", Color::Red), "   0");
    EXPECT_EQ(fmt::format("{:4}", Color::Green), "   1");
    EXPECT_EQ(fmt::format("{:<4}", Color::Blue), "2   ");
    EXPECT_EQ(fmt::format("{:>4}", Color::Red), "   0");
    EXPECT_EQ(fmt::format("{:^4}", Color::Green), " 1  ");
    EXPECT_EQ(fmt::format("{:*<4}", Color::Blue), "2***");
    EXPECT_EQ(fmt::format("{:*>4}", Color::Red), "***0");
    EXPECT_EQ(fmt::format("{:*^4}", Color::Green), "*1**");
}

// Test formatting with precision (should be ignored for integers/enums)
TEST(EnumFormatterTest, PrecisionIgnored) {
    EXPECT_EQ(fmt::format("{:.5}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{:.0}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{:.10}", Color::Blue), "2");
}

// Test formatting with sign options
TEST(EnumFormatterTest, SignOptions) {
    EXPECT_EQ(fmt::format("{:+}", Color::Red), "+0");
    EXPECT_EQ(fmt::format("{:+}", Color::Green), "+1");
    EXPECT_EQ(fmt::format("{: }", Color::Red), " 0");
    EXPECT_EQ(fmt::format("{:-}", Color::Green), "1");
}

// Test formatting with base prefixes (for enums that are integers)
TEST(EnumFormatterTest, BasePrefix) {
    EXPECT_EQ(fmt::format("{:#x}", Color::Red), "0x0");
    EXPECT_EQ(fmt::format("{:#x}", Color::Green), "0x1");
    EXPECT_EQ(fmt::format("{:#X}", Color::Blue), "0X2");
    EXPECT_EQ(fmt::format("{:#o}", Color::Red), "00");
    EXPECT_EQ(fmt::format("{:#b}", Color::Green), "0b1");
    EXPECT_EQ(fmt::format("{:#B}", Color::Blue), "0B10");
}

// Test formatting with different bases
TEST(EnumFormatterTest, DifferentBases) {
    EXPECT_EQ(fmt::format("{:d}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{:x}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{:X}", Color::Blue), "2");
    EXPECT_EQ(fmt::format("{:o}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{:b}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{:B}", Color::Blue), "10");
}

// Test edge cases: negative enum values (if underlying type is signed)
TEST(EnumFormatterTest, NegativeValues) {
    EXPECT_EQ(fmt::format("{}", SmallEnum::A), "-128");
    EXPECT_EQ(fmt::format("{}", SmallEnum::B), "0");
    EXPECT_EQ(fmt::format("{}", SmallEnum::C), "127");
    EXPECT_EQ(fmt::format("{:+}", SmallEnum::A), "-128");
    EXPECT_EQ(fmt::format("{: }", SmallEnum::A), "-128");
}

// Test edge cases: maximum unsigned values
TEST(EnumFormatterTest, MaxUnsignedValues) {
    EXPECT_EQ(fmt::format("{}", UnsignedEnum::Min), "0");
    EXPECT_EQ(fmt::format("{}", UnsignedEnum::Max), std::to_string(std::numeric_limits<unsigned>::max()));
    EXPECT_EQ(fmt::format("{:x}", UnsignedEnum::Max), "ffffffff");
    EXPECT_EQ(fmt::format("{:X}", UnsignedEnum::Max), "FFFFFFFF");
}

// Test formatting with zero width (should produce minimal output)
TEST(EnumFormatterTest, ZeroWidth) {
    EXPECT_EQ(fmt::format("{:0}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{:0}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{:0}", Status::Ok), "200");
}

// Test formatting with large width
TEST(EnumFormatterTest, LargeWidth) {
    EXPECT_EQ(fmt::format("{:10}", Color::Red), "         0");
    EXPECT_EQ(fmt::format("{:<10}", Color::Green), "1         ");
    EXPECT_EQ(fmt::format("{:>10}", Color::Blue), "         2");
    EXPECT_EQ(fmt::format("{:^10}", Color::Red), "    0     ");
}

// Test formatting with zero padding
TEST(EnumFormatterTest, ZeroPadding) {
    EXPECT_EQ(fmt::format("{:04}", Color::Red), "0000");
    EXPECT_EQ(fmt::format("{:04}", Color::Green), "0001");
    EXPECT_EQ(fmt::format("{:04}", Color::Blue), "0002");
    EXPECT_EQ(fmt::format("{:010}", Status::Ok), "0000000200");
}

// Test formatting with alternate form for zero value
TEST(EnumFormatterTest, AlternateFormZero) {
    EXPECT_EQ(fmt::format("{:#x}", Color::Red), "0x0");
    EXPECT_EQ(fmt::format("{:#X}", Color::Red), "0X0");
    EXPECT_EQ(fmt::format("{:#o}", Color::Red), "00");
    EXPECT_EQ(fmt::format("{:#b}", Color::Red), "0b0");
}

// Test formatting with multiple enums in one format string
TEST(EnumFormatterTest, MultipleEnums) {
    EXPECT_EQ(fmt::format("{}, {}, {}", Color::Red, Color::Green, Color::Blue), "0, 1, 2");
    EXPECT_EQ(fmt::format("{:04}-{:04}-{:04}", Color::Red, Color::Green, Color::Blue), "0000-0001-0002");
}

// Test that the formatter works with fmt::format_to
TEST(EnumFormatterTest, FormatTo) {
    std::string out;
    auto it = std::back_inserter(out);
    fmt::format_to(it, "{}", Color::Red);
    EXPECT_EQ(out, "0");
    
    out.clear();
    fmt::format_to(it, "{:04}", Color::Green);
    EXPECT_EQ(out, "0001");
}

// Test that the formatter works with fmt::sprintf-style formatting
TEST(EnumFormatterTest, PrintfStyle) {
    EXPECT_EQ(fmt::sprintf("%d", Color::Red), "0");
    EXPECT_EQ(fmt::sprintf("%04d", Color::Green), "0001");
    EXPECT_EQ(fmt::sprintf("%x", Color::Blue), "2");
}

// Test that the formatter handles all enum values in a range
TEST(EnumFormatterTest, AllValues) {
    for (int i = 0; i <= 2; ++i) {
        Color c = static_cast<Color>(i);
        EXPECT_EQ(fmt::format("{}", c), std::to_string(i));
        EXPECT_EQ(fmt::format("{:04}", c), fmt::format("{:04}", i));
    }
}