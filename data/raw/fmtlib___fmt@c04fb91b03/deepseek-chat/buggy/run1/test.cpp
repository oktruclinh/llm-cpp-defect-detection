```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <map>
#include <set>

// Test formatting of enums via the formatter specialization that inherits from formatter<int>
// The focal function is the format() method in the formatter specialization for enums
// that uses internal::format_enum to detect enum types and delegates to formatter<int>

enum class Color { Red = 0, Green = 1, Blue = 2 };
enum class Status { OK = 200, NotFound = 404, Error = 500 };
enum OldStyle { A = 1, B = 2, C = 4 };

TEST(EnumFormatterTest, BasicEnumFormatting) {
    EXPECT_EQ(fmt::format("{}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{}", Color::Blue), "2");
}

TEST(EnumFormatterTest, EnumWithNonZeroValues) {
    EXPECT_EQ(fmt::format("{}", Status::OK), "200");
    EXPECT_EQ(fmt::format("{}", Status::NotFound), "404");
    EXPECT_EQ(fmt::format("{}", Status::Error), "500");
}

TEST(EnumFormatterTest, OldStyleEnum) {
    EXPECT_EQ(fmt::format("{}", A), "1");
    EXPECT_EQ(fmt::format("{}", B), "2");
    EXPECT_EQ(fmt::format("{}", C), "4");
}

TEST(EnumFormatterTest, EnumWithWidthAndFill) {
    EXPECT_EQ(fmt::format("{:>4}", Color::Red), "   0");
    EXPECT_EQ(fmt::format("{:<4}", Color::Green), "1   ");
    EXPECT_EQ(fmt::format("{:*^5}", Color::Blue), "**2**");
}

TEST(EnumFormatterTest, EnumWithPrecision) {
    // Precision on integer types should be ignored or handled as per int formatting
    EXPECT_EQ(fmt::format("{:.5}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{:.0}", Color::Green), "1");
}

TEST(EnumFormatterTest, EnumWithSign) {
    EXPECT_EQ(fmt::format("{:+}", Color::Red), "+0");
    EXPECT_EQ(fmt::format("{:-}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{: }", Color::Blue), " 2");
}

TEST(EnumFormatterTest, EnumWithBasePrefix) {
    EXPECT_EQ(fmt::format("{:#x}", Color::Red), "0x0");
    EXPECT_EQ(fmt::format("{:#X}", Color::Green), "0X1");
    EXPECT_EQ(fmt::format("{:#o}", Color::Blue), "02");
    EXPECT_EQ(fmt::format("{:#b}", Color::Red), "0b0");
    EXPECT_EQ(fmt::format("{:#B}", Color::Green), "0B1");
}

TEST(EnumFormatterTest, EnumWithDifferentBases) {
    EXPECT_EQ(fmt::format("{:d}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{:x}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{:X}", Color::Blue), "2");
    EXPECT_EQ(fmt::format("{:o}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{:b}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{:B}", Color::Blue), "10");
}

TEST(EnumFormatterTest, EnumWithZeroFlag) {
    EXPECT_EQ(fmt::format("{:04}", Color::Red), "0000");
    EXPECT_EQ(fmt::format("{:04}", Color::Green), "0001");
    EXPECT_EQ(fmt::format("{:04}", Color::Blue), "0002");
}

TEST(EnumFormatterTest, EnumWithAlternateForm) {
    EXPECT_EQ(fmt::format("{:#06x}", Color::Red), "0x0000");
    EXPECT_EQ(fmt::format("{:#06x}", Color::Green), "0x0001");
}

TEST(EnumFormatterTest, EnumInContainer) {
    std::vector<Color> colors = {Color::Red, Color::Green, Color::Blue};
    EXPECT_EQ(fmt::format("{}", fmt::join(colors, ",")), "0,1,2");
    
    std::set<Status> statuses = {Status::OK, Status::NotFound};
    EXPECT_EQ(fmt::format("{}", fmt::join(statuses, "|")), "200|404");
}

TEST(EnumFormatterTest, EnumInMap) {
    std::map<Color, std::string> colorMap = {{Color::Red, "red"}, {Color::Blue, "blue"}};
    std::string result = fmt::format("{}", fmt::join(colorMap, ", "));
    EXPECT_TRUE(result.find("0") != std::string::npos);
    EXPECT_TRUE(result.find("2") != std::string::npos);
}

TEST(EnumFormatterTest, EnumWithFormatArgs) {
    EXPECT_EQ(fmt::format("{0} {0:#x} {0:04}", Color::Green), "1 0x1 0001");
}

TEST(EnumFormatterTest, EnumWithNegativeValues) {
    enum class SignedEnum : int { Neg = -5, Zero = 0, Pos = 10 };
    EXPECT_EQ(fmt::format("{}", SignedEnum::Neg), "-5");
    EXPECT_EQ(fmt::format("{:+}", SignedEnum::Neg), "-5");
    EXPECT_EQ(fmt::format("{: }", SignedEnum::Neg), "-5");
    EXPECT_EQ(fmt::format("{:05}", SignedEnum::Neg), "-0005");
}

TEST(EnumFormatterTest, EnumWithLargeValues) {
    enum class LargeEnum : unsigned long long { Max = 0xFFFFFFFFFFFFFFFFull };
    EXPECT_EQ(fmt::format("{}", LargeEnum::Max), "18446744073709551615");
    EXPECT_EQ(fmt::format("{:#x}", LargeEnum::Max), "0xffffffffffffffff");
}

TEST(EnumFormatterTest, EnumWithCharUnderlyingType) {
    enum class CharEnum : unsigned char { A = 'A', B = 'B' };
    EXPECT_EQ(fmt::format("{}", CharEnum::A), "65");
    EXPECT_EQ(fmt::format("{}", CharEnum::B), "66");
}

TEST(EnumFormatterTest, EnumWithBoolUnderlyingType) {
    enum class BoolEnum : bool { False = false, True = true };
    EXPECT_EQ(fmt::format("{}", BoolEnum::False), "0");
    EXPECT_EQ(fmt::format("{}", BoolEnum::True), "1");
}

TEST(EnumFormatterTest, EnumWithShortUnderlyingType) {
    enum class ShortEnum : short { Min = -32768, Max = 32767 };
    EXPECT_EQ(fmt::format("{}", ShortEnum::Min), "-32768");
    EXPECT_EQ(fmt::format("{}", ShortEnum::Max), "32767");
}

TEST(EnumFormatterTest, EnumWithUnsignedShortUnderlyingType) {
    enum class UShortEnum : unsigned short { Max = 65535 };
    EXPECT_EQ(fmt::format("{}", UShortEnum::Max), "65535");
}

TEST(EnumFormatterTest, EnumWithLongUnderlyingType) {
    enum class LongEnum : long { Value = 123456789L };
    EXPECT_EQ(fmt::format("{}", LongEnum::Value), "123456789");
}

TEST(EnumFormatterTest, EnumWithUnsignedLongUnderlyingType) {
    enum class ULongEnum : unsigned long { Value = 987654321UL };
    EXPECT_EQ(fmt::format("{}", ULongEnum::Value), "987654321");
}

TEST(EnumFormatterTest, EnumWithLongLongUnderlyingType) {
    enum class LongLongEnum : long long { Value = 123456789012345LL };
    EXPECT_EQ(fmt::format("{}", LongLongEnum::Value), "123456789012345");
}

TEST(EnumFormatterTest, EnumWithUnsignedLongLongUnderlyingType) {
    enum class ULongLongEnum : unsigned long long { Value = 987654321098765ULL };
    EXPECT_EQ(fmt::format("{}", ULongLongEnum::Value), "987654321098765");
}

TEST(EnumFormatterTest, EnumWithInt8UnderlyingType) {
    enum class Int8Enum : int8_t { Min = -128, Max = 127 };
    EXPECT_EQ(fmt::format("{}", Int8Enum::Min), "-128");
    EXPECT_EQ(fmt::format("{}", Int8Enum::Max), "127");
}

TEST(EnumFormatterTest, EnumWithUint8UnderlyingType) {
    enum class Uint8Enum : uint8_t { Max = 255 };
    EXPECT_EQ(fmt::format("{}", Uint8Enum::Max), "255");
}

TEST(EnumFormatterTest, EnumWithInt16UnderlyingType) {
    enum class Int16Enum : int16_t { Min = -32768, Max = 32767 };
    EXPECT_EQ(fmt::format("{}", Int16Enum::Min), "-32768");
    EXPECT_EQ(fmt::format("{}", Int16Enum::Max), "32767");
}

TEST(EnumFormatterTest, EnumWithUint16UnderlyingType) {
    enum class Uint16Enum : uint16_t { Max = 65535 };
    EXPECT_EQ(fmt::format("{}", Uint16Enum::Max), "65535");
}

TEST(EnumFormatterTest, EnumWithInt32UnderlyingType) {
    enum class Int32Enum : int32_t { Min = -2147483647 - 1, Max = 2147483647 };
    EXPECT_EQ(fmt::format("{}", Int32Enum::Min), "-2147483648");
    EXPECT_EQ(fmt::format("{}", Int32Enum::Max), "2147483647");
}

TEST(EnumFormatterTest, EnumWithUint32UnderlyingType) {
    enum class Uint32Enum : uint32_t { Max = 4294967295U };
    EXPECT_EQ(fmt::format("{}", Uint32Enum::Max), "4294967295");
}

TEST(EnumFormatterTest, EnumWithInt64UnderlyingType) {
    enum class Int64Enum : int64_t { Min = -9223372036854775807LL - 1, Max = 9223372036854775807LL };
    EXPECT_EQ(fmt::format("{}", Int64Enum::Min), "-9223372036854775808");
    EXPECT_EQ(fmt::format("{}", Int64Enum::Max), "9223372036854775807");
}

TEST(EnumFormatterTest, EnumWithUint64UnderlyingType) {
    enum class Uint64Enum : uint64_t { Max = 18446744073709551615ULL };
    EXPECT_EQ(fmt::format("{}", Uint64Enum::Max), "18446744073709551615");
}

TEST(EnumFormatterTest, EnumWithMixedFormatSpecifiers) {
    EXPECT_EQ(fmt::format("{:+#010x}", Color::Red), "+0x0000000");
    EXPECT_EQ(fmt::format("{: 08b}", Color::Green), " 0000001");
    EXPECT_EQ(fmt::format("{:#08o}", Color::Blue), "0000002");
}

TEST(EnumFormatterTest, EnumWithDynamicWidth) {
    int width = 5;
    EXPECT_EQ(fmt::format("{:{}}", Color::Red, width), "    0");
    EXPECT_EQ(fmt::format("{:*>{}}", Color::Green, width), "****1");
}

TEST(EnumFormatterTest, EnumWithDynamicPrecision) {
    int prec = 3;
    EXPECT_EQ(fmt::format("{:.{}}", Color::Red, prec), "0");
}

TEST(EnumFormatterTest, EnumWithDynamicFormatSpec) {
    std::string spec = "04";
    EXPECT_EQ(fmt::format("{:{}}", Color::Blue, spec), "0002");
}

TEST(EnumFormatterTest, EnumWithNamedArgs) {
    EXPECT_EQ(fmt::format("{color:#06x}", fmt::arg("color", Color::Green)), "0x0001");
}

TEST(EnumFormatterTest, EnumWithMultipleArgs) {
    EXPECT_EQ(fmt::format("{} {} {}", Color::Red, Color::Green, Color::Blue), "0 1 2");
}

TEST(EnumFormatterTest, EnumWithText) {
    EXPECT_EQ(fmt::format("Color: {}, Status: {}", Color::Blue, Status::OK), "Color: 2, Status: 200");
}

TEST(EnumFormatterTest, EnumWithEscapedBraces) {
    EXPECT_EQ(fmt::format("{{{}}}", Color::Red), "{0}");
}

TEST(EnumFormatterTest, EnumWithReorderedArgs) {
    EXPECT_EQ(fmt::format("{2} {0} {1}", Color::Red, Color::Green, Color::Blue), "2 0 1");
}

TEST(EnumFormatterTest, EnumWithRepeatedArgs) {
    EXPECT_EQ(fmt::format("{0} {0:#x} {0:04}", Color::Green), "1 0x1 0001");
}

TEST(EnumFormatterTest, EnumWithConditionalFormatting) {
    auto format_color = [](Color c) -> std::string {
        if (c == Color::Red) return fmt::format("{:#x}", c);
        return fmt::format("{}", c);
    };
    EXPECT_EQ(format_color(Color::Red), "0x0");
    EXPECT_EQ(format_color(Color::Green), "1");
}

TEST(EnumFormatterTest, EnumWithUserDefinedLiteral) {
    using namespace fmt::literals;
    EXPECT_EQ("{}"_format(Color::Red), "0");
    EXPECT_EQ("{:#x}"_format(Color::Green), "0x1");
}

TEST(EnumFormatterTest, EnumWithFormatTo) {
    std::string result;
    fmt::format_to(std::back_inserter(result), "{}", Color::Blue);
    EXPECT_EQ(result, "2");
}

TEST(EnumFormatterTest, EnumWithFormatToN) {
    std::string result(5, '\0');
    auto it = fmt::format_to_n(result.begin(), 3, "{}", Color::Red);
    *it = '\0';
    EXPECT_EQ(result, "0\0\0\0\0");
}

TEST(EnumFormatterTest, EnumWithFormattedSize) {
    auto size = fmt::formatted_size("{}", Color::Green);
    EXPECT_EQ(size, 1);
    size = fmt::formatted_size("{:#06x}", Color::Blue);
    EXPECT_EQ(size, 6);
}

TEST(EnumFormatterTest, EnumWithPrintfStyle) {
    EXPECT_EQ(fmt::sprintf("%d", Color::Red), "0");
    EXPECT_EQ(fmt::sprintf("%#x", Color::Green), "0x1");
    EXPECT_EQ(fmt::sprintf("%04d", Color::Blue), "0002");
}

TEST(EnumFormatterTest, EnumWithPrintfStyleWidth) {
    EXPECT_EQ(fmt::sprintf("%4d", Color::Red), "   0");
    EXPECT_EQ(fmt::sprintf("%-4d", Color::Green), "1   ");
}

TEST(EnumFormatterTest, EnumWithPrintfStylePrecision) {
    EXPECT_EQ(fmt::sprintf("%.5d", Color::Red), "00000");
    EXPECT_EQ(fmt::sprintf("%.0d", Color::Green), "1");
}

TEST(EnumFormatterTest, EnumWithPrintfStyleFlags) {
    EXPECT_EQ(fmt::sprintf("%+d", Color::Red), "+0");
    EXPECT_EQ(fmt::sprintf("% d", Color::Green), " 1");
    EXPECT_EQ(fmt::sprintf("%04d", Color::Blue), "0002");
}

TEST(EnumFormatterTest, EnumWithPrintfStyleStarWidth) {
    EXPECT_EQ(fmt::sprintf("%*d", 5, Color::Red), "    0");
    EXPECT_EQ(fmt::sprintf("%*d", -5, Color::Green), "1    ");
}

TEST(EnumFormatterTest, EnumWithPrintfStyleStarPrecision) {
    EXPECT_EQ(fmt::sprintf("%.*d", 3, Color::Red), "000");
    EXPECT_EQ(fmt::sprintf("%.*d", 0, Color::Green), "1");
}

TEST(EnumFormatterTest, EnumWithPrintfStyleLongLong) {
    enum class LLEnum : long long { Value = 1234567890123LL };
    EXPECT_EQ(fmt::sprintf("%lld", LLEnum::Value), "1234567890123");
}

TEST(EnumFormatterTest, EnumWithPrintfStyleUnsigned) {
    enum class UEnum : unsigned { Value = 4294967295U };
    EXPECT_EQ(fmt::sprintf("%u", UEnum::Value), "4294967295");
}

TEST(EnumFormatterTest, EnumWithPrintfStyleHex) {
    EXPECT_EQ(fmt::sprintf("%x", Color::Red), "0");
    EXPECT_EQ(fmt::sprintf("%X", Color::Green), "1");
    EXPECT_EQ(fmt::sprintf("%#x", Color::Blue), "0x2");
}

TEST(EnumFormatterTest, EnumWithPrintfStyleOctal) {
    EXPECT_EQ(fmt::sprintf("%o", Color::Red), "0");
    EXPECT_EQ(fmt::sprintf("%#o", Color::Green), "01");
}

TEST(EnumFormatterTest, EnumWithPrintfStyleBinary) {
    EXPECT_EQ(fmt::sprintf("%b", Color::Red), "0");
    EXPECT_EQ(fmt::sprintf("%B", Color::Green), "1");
    EXPECT_EQ(fmt::sprintf("%#b", Color::Blue), "0b10");
}

TEST(EnumFormatterTest, EnumWithPrintfStyleMixed) {
    EXPECT_EQ(fmt::sprintf("%+-#10x", Color::Red), "+0x0      ");
    EXPECT_EQ(fmt::sprintf("% 08d", Color::Green), " 0000001");
}

TEST(EnumFormatterTest, EnumWithPrintfStylePositional) {
    EXPECT_EQ(fmt::sprintf("%2$d %1$#x", Color::Red, Color::Green), "1 0x0");
}

TEST(EnumFormatterTest, EnumWithPrintfStyleRepeated) {
    EXPECT_EQ(fmt::sprintf("%1$d %1$#x %1$04d", Color::Green), "1 0x1 0001");
}

TEST(EnumFormatterTest, EnumWithPrint