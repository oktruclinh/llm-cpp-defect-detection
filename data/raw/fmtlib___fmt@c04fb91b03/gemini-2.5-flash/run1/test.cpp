#include <gtest/gtest.h>
#include "fmt/format.h"

// Helper function to format a value and return the result as a string.
template <typename T>
std::string format_value(const T& value, fmt::string_view format_str = "") {
  return fmt::format(fmt::runtime(format_str), value);
}

// Test fixture for enum formatting.
// This allows us to define a custom enum for testing purposes.
enum class MyEnum {
  Value1 = 0,
  Value2 = 1,
  Value3 = 42,
  NegativeValue = -1,
  LargeValue = 1000000,
};

// Specialize fmt::formatter for MyEnum to enable formatting.
// This is required for the focal function to be exercised.
// We inherit from formatter<int> as per the focal function's template specialization.
template <>
struct fmt::formatter<MyEnum> : fmt::formatter<int> {
  // Parse is inherited from formatter<int>, so we don't need to implement it
  // unless we want custom enum-specific parsing. For these tests, default is fine.

  // The format method will be called by the focal function.
  // We delegate to the base class (formatter<int>) to format the underlying integer value.
  template <typename FormatContext>
  auto format(MyEnum e, FormatContext& ctx) const -> decltype(ctx.out()) {
    return fmt::formatter<int>::format(static_cast<int>(e), ctx);
  }
};

TEST(EnumFormatterTest, DefaultFormatting) {
  EXPECT_EQ("0", format_value(MyEnum::Value1));
  EXPECT_EQ("1", format_value(MyEnum::Value2));
  EXPECT_EQ("42", format_value(MyEnum::Value3));
  EXPECT_EQ("-1", format_value(MyEnum::NegativeValue));
  EXPECT_EQ("1000000", format_value(MyEnum::LargeValue));
}

TEST(EnumFormatterTest, WidthFormatting) {
  EXPECT_EQ("   0", format_value(MyEnum::Value1, "{:4}"));
  EXPECT_EQ("0001", format_value(MyEnum::Value2, "{:04}"));
  EXPECT_EQ("  42", format_value(MyEnum::Value3, "{:4}"));
  EXPECT_EQ("  -1", format_value(MyEnum::NegativeValue, "{:4}"));
  EXPECT_EQ("1000000", format_value(MyEnum::LargeValue, "{:4}")); // Width less than value length
  EXPECT_EQ("001000000", format_value(MyEnum::LargeValue, "{:09}"));
}

TEST(EnumFormatterTest, AlignmentFormatting) {
  EXPECT_EQ("0   ", format_value(MyEnum::Value1, "{:<4}"));
  EXPECT_EQ(" 1  ", format_value(MyEnum::Value2, "{:^4}"));
  EXPECT_EQ("  42", format_value(MyEnum::Value3, "{:>4}")); // Default alignment for numbers
  EXPECT_EQ("-1  ", format_value(MyEnum::NegativeValue, "{:<4}"));
}

TEST(EnumFormatterTest, SignFormatting) {
  EXPECT_EQ("+0", format_value(MyEnum::Value1, "{:+}"));
  EXPECT_EQ("+1", format_value(MyEnum::Value2, "{:+}"));
  EXPECT_EQ(" 42", format_value(MyEnum::Value3, "{: }"));
  EXPECT_EQ("-1", format_value(MyEnum::NegativeValue, "{:+}"));
  EXPECT_EQ("-1", format_value(MyEnum::NegativeValue, "{: }"));
}

TEST(EnumFormatterTest, BaseFormatting) {
  EXPECT_EQ("0", format_value(MyEnum::Value1, "{:d}"));
  EXPECT_EQ("1", format_value(MyEnum::Value2, "{:d}"));
  EXPECT_EQ("2a", format_value(MyEnum::Value3, "{:x}"));
  EXPECT_EQ("2A", format_value(MyEnum::Value3, "{:X}"));
  EXPECT_EQ("52", format_value(MyEnum::Value3, "{:o}"));
  EXPECT_EQ("-1", format_value(MyEnum::NegativeValue, "{:d}"));
  EXPECT_EQ("ffffffff", format_value(MyEnum::NegativeValue, "{:x}")); // Assuming 32-bit int for -1
}

TEST(EnumFormatterTest, CombinedFormatting) {
  EXPECT_EQ(" +00042", format_value(MyEnum::Value3, "{:0=+7d}"));
  EXPECT_EQ("0x2a", format_value(MyEnum::Value3, "{:#x}"));
  EXPECT_EQ("0X2A", format_value(MyEnum::Value3, "{:#X}"));
  EXPECT_EQ("052", format_value(MyEnum::Value3, "{:#o}"));
  EXPECT_EQ("  -1", format_value(MyEnum::NegativeValue, "{:4d}"));
  EXPECT_EQ(" -1", format_value(MyEnum::NegativeValue, "{: d}"));
}

TEST(EnumFormatterTest, EmptyFormatString) {
  EXPECT_EQ("0", format_value(MyEnum::Value1, "{}"));
  EXPECT_EQ("42", format_value(MyEnum::Value3, "{}"));
}

TEST(EnumFormatterTest, InvalidFormatSpecifier) {
  // The focal function delegates to formatter<int>::format.
  // Invalid format specifiers for int should result in a format_error.
  EXPECT_THROW(format_value(MyEnum::Value1, "{:f}"), fmt::format_error);
  EXPECT_THROW(format_value(MyEnum::Value3, "{:p}"), fmt::format_error);
}

// Test with a different enum type to ensure generic behavior
enum class AnotherEnum : short {
  Alpha = 10,
  Beta = 20,
};

template <>
struct fmt::formatter<AnotherEnum> : fmt::formatter<int> {
  template <typename FormatContext>
  auto format(AnotherEnum e, FormatContext& ctx) const -> decltype(ctx.out()) {
    return fmt::formatter<int>::format(static_cast<int>(e), ctx);
  }
};

TEST(EnumFormatterTest, AnotherEnumType) {
  EXPECT_EQ("10", format_value(AnotherEnum::Alpha));
  EXPECT_EQ("20", format_value(AnotherEnum::Beta));
  EXPECT_EQ("0x14", format_value(AnotherEnum::Beta, "{:#x}"));
}

// Test with an enum that has a large underlying value type
enum class LargeEnum : long long {
  MaxLongLong = std::numeric_limits<long long>::max(),
  MinLongLong = std::numeric_limits<long long>::min(),
};

template <>
struct fmt::formatter<LargeEnum> : fmt::formatter<long long> {
  template <typename FormatContext>
  auto format(LargeEnum e, FormatContext& ctx) const -> decltype(ctx.out()) {
    return fmt::formatter<long long>::format(static_cast<long long>(e), ctx);
  }
};

TEST(EnumFormatterTest, LargeEnumValues) {
  EXPECT_EQ(std::to_string(std::numeric_limits<long long>::max()), format_value(LargeEnum::MaxLongLong));
  EXPECT_EQ(std::to_string(std::numeric_limits<long long>::min()), format_value(LargeEnum::MinLongLong));
  EXPECT_EQ(fmt::format("{:x}", std::numeric_limits<long long>::max()), format_value(LargeEnum::MaxLongLong, "{:x}"));
}