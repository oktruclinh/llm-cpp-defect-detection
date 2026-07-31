#include <gtest/gtest.h>
#include "fmt/format.h"

// Helper function to format an enum value using fmt::format
template <typename EnumType>
std::string format_enum(EnumType value) {
  return fmt::format("{}", value);
}

// Define a simple enum for testing
enum class MyEnum {
  Value1,
  Value2,
  Value3 = 100,
  NegativeValue = -1,
  ZeroValue = 0
};

// Make MyEnum format-able by fmtlib.
// This is the mechanism that the focal function relies on.
template <>
struct fmt::formatter<MyEnum> : fmt::formatter<int> {
  template <typename FormatContext>
  auto format(MyEnum e, FormatContext& ctx) {
    return fmt::formatter<int>::format(static_cast<int>(e), ctx);
  }
};

TEST(EnumFormatterTest, BasicEnumFormatting) {
  // Test basic formatting of enum values
  EXPECT_EQ(format_enum(MyEnum::Value1), "0");
  EXPECT_EQ(format_enum(MyEnum::Value2), "1");
  EXPECT_EQ(format_enum(MyEnum::Value3), "100");
}

TEST(EnumFormatterTest, NegativeEnumValueFormatting) {
  // Test formatting of a negative enum value
  EXPECT_EQ(format_enum(MyEnum::NegativeValue), "-1");
}

TEST(EnumFormatterTest, ZeroEnumValueFormatting) {
  // Test formatting of a zero enum value
  EXPECT_EQ(format_enum(MyEnum::ZeroValue), "0");
}

TEST(EnumFormatterTest, EnumWithFormatSpecifiers) {
  // Test enum formatting with various format specifiers
  EXPECT_EQ(fmt::format("{:d}", MyEnum::Value1), "0");
  EXPECT_EQ(fmt::format("{:x}", MyEnum::Value3), "64"); // 100 in hex
  EXPECT_EQ(fmt::format("{:X}", MyEnum::Value3), "64");
  EXPECT_EQ(fmt::format("{:o}", MyEnum::Value3), "144"); // 100 in octal
  EXPECT_EQ(fmt::format("{:b}", MyEnum::Value3), "1100100"); // 100 in binary
  EXPECT_EQ(fmt::format("{:#x}", MyEnum::Value3), "0x64");
  EXPECT_EQ(fmt::format("{:5d}", MyEnum::Value2), "    1");
  EXPECT_EQ(fmt::format("{:05d}", MyEnum::Value2), "00001");
  EXPECT_EQ(fmt::format("{: d}", MyEnum::Value2), " 1");
  EXPECT_EQ(fmt::format("{:+d}", MyEnum::Value2), "+1");
  EXPECT_EQ(fmt::format("{:+d}", MyEnum::NegativeValue), "-1");
}

// Define another enum to ensure type-specific behavior
enum class AnotherEnum {
  A = 10,
  B = 20
};

template <>
struct fmt::formatter<AnotherEnum> : fmt::formatter<int> {
  template <typename FormatContext>
  auto format(AnotherEnum e, FormatContext& ctx) {
    return fmt::formatter<int>::format(static_cast<int>(e), ctx);
  }
};

TEST(EnumFormatterTest, MultipleEnumTypes) {
  // Test that different enum types are handled correctly
  EXPECT_EQ(format_enum(AnotherEnum::A), "10");
  EXPECT_EQ(format_enum(AnotherEnum::B), "20");
  EXPECT_EQ(format_enum(MyEnum::Value1), "0"); // Ensure MyEnum still works
}

// Test with a scoped enum that has explicit underlying type
enum class LongEnum : long {
  LargeValue = 2147483647L, // Max int
  VeryLargeValue = 2147483648L // Exceeds max int
};

template <>
struct fmt::formatter<LongEnum> : fmt::formatter<long> {
  template <typename FormatContext>
  auto format(LongEnum e, FormatContext& ctx) {
    return fmt::formatter<long>::format(static_cast<long>(e), ctx);
  }
};

TEST(EnumFormatterTest, LongEnumFormatting) {
  EXPECT_EQ(format_enum(LongEnum::LargeValue), "2147483647");
  EXPECT_EQ(format_enum(LongEnum::VeryLargeValue), "2147483648");
  EXPECT_EQ(fmt::format("{:L}", LongEnum::VeryLargeValue), "2,147,483,648"); // Locale-specific formatting
}

// Test with an unscoped enum
enum UnscopedEnum {
  UnscopedVal1 = 5,
  UnscopedVal2 = 6
};

template <>
struct fmt::formatter<UnscopedEnum> : fmt::formatter<int> {
  template <typename FormatContext>
  auto format(UnscopedEnum e, FormatContext& ctx) {
    return fmt::formatter<int>::format(static_cast<int>(e), ctx);
  }
};

TEST(EnumFormatterTest, UnscopedEnumFormatting) {
  EXPECT_EQ(format_enum(UnscopedEnum::UnscopedVal1), "5");
  EXPECT_EQ(format_enum(UnscopedEnum::UnscopedVal2), "6");
}

// Test case for an enum that might not have a direct integer mapping
// (though C++ enums always do, this tests the general case of the formatter
// delegating to another formatter type).
enum class CharEnum : char {
  CharA = 'A',
  CharB = 'B'
};

template <>
struct fmt::formatter<CharEnum> : fmt::formatter<char> {
  template <typename FormatContext>
  auto format(CharEnum e, FormatContext& ctx) {
    return fmt::formatter<char>::format(static_cast<char>(e), ctx);
  }
};

TEST(EnumFormatterTest, CharEnumFormatting) {
  // When formatted as char, it should print the character
  EXPECT_EQ(fmt::format("{}", CharEnum::CharA), "A");
  EXPECT_EQ(fmt::format("{}", CharEnum::CharB), "B");
  // When formatted as int, it should print the integer value
  EXPECT_EQ(fmt::format("{:d}", CharEnum::CharA), "65");
}

// Test with a custom underlying type for the enum
enum class CustomUnderlyingEnum : unsigned short {
    UShortVal = 65535
};

template <>
struct fmt::formatter<CustomUnderlyingEnum> : fmt::formatter<unsigned short> {
    template <typename FormatContext>
    auto format(CustomUnderlyingEnum e, FormatContext& ctx) {
        return fmt::formatter<unsigned short>::format(static_cast<unsigned short>(e), ctx);
    }
};

TEST(EnumFormatterTest, CustomUnderlyingTypeEnumFormatting) {
    EXPECT_EQ(format_enum(CustomUnderlyingEnum::UShortVal), "65535");
    EXPECT_EQ(fmt::format("{:x}", CustomUnderlyingEnum::UShortVal), "ffff");
}