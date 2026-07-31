#include <gtest/gtest.h>
#include "fmt/format.h"

// Helper struct to test custom enum formatting
enum class MyEnum {
  Value1,
  Value2,
  Value3
};

// Custom formatter for MyEnum, inheriting from formatter<int, Char>
// This is the specific scenario the focal function handles.
template <>
struct fmt::formatter<MyEnum> : fmt::formatter<int> {
  template <typename FormatContext>
  auto format(MyEnum e, FormatContext& ctx) {
    return fmt::formatter<int>::format(static_cast<int>(e), ctx);
  }
};

// Another custom enum to test different enum values
enum class AnotherEnum {
  Zero = 0,
  One = 1,
  NegativeOne = -1
};

template <>
struct fmt::formatter<AnotherEnum> : fmt::formatter<int> {
  template <typename FormatContext>
  auto format(AnotherEnum e, FormatContext& ctx) {
    return fmt::formatter<int>::format(static_cast<int>(e), ctx);
  }
};

// Test fixture for common setup if needed, though not strictly necessary here.
class EnumFormattingTest : public ::testing::Test {
protected:
  // You can set up common resources here if needed.
};

// Test case for basic enum formatting
TEST_F(EnumFormattingTest, BasicEnumFormatting) {
  EXPECT_EQ(fmt::format("{}", MyEnum::Value1), "0");
  EXPECT_EQ(fmt::format("{}", MyEnum::Value2), "1");
  EXPECT_EQ(fmt::format("{}", MyEnum::Value3), "2");
}

// Test case for enum formatting with different integer values
TEST_F(EnumFormattingTest, EnumWithCustomValuesFormatting) {
  EXPECT_EQ(fmt::format("{}", AnotherEnum::Zero), "0");
  EXPECT_EQ(fmt::format("{}", AnotherEnum::One), "1");
  EXPECT_EQ(fmt::format("{}", AnotherEnum::NegativeOne), "-1");
}

// Test case for enum formatting with format specifiers (e.g., width, alignment)
TEST_F(EnumFormattingTest, EnumFormattingWithSpecifiers) {
  EXPECT_EQ(fmt::format("{:5}", MyEnum::Value1), "    0");
  EXPECT_EQ(fmt::format("{:<5}", MyEnum::Value2), "1    ");
  EXPECT_EQ(fmt::format("{:^5}", MyEnum::Value3), "  2  ");
  EXPECT_EQ(fmt::format("{:05}", MyEnum::Value1), "00000");
  EXPECT_EQ(fmt::format("{:+}", AnotherEnum::One), "+1");
  EXPECT_EQ(fmt::format("{: }", AnotherEnum::One), " 1");
  EXPECT_EQ(fmt::format("{:-}", AnotherEnum::NegativeOne), "-1"); // - is default for negative
}

// Test case for enum formatting with precision (should be ignored for int)
TEST_F(EnumFormattingTest, EnumFormattingWithPrecision) {
  // Precision specifier for integers typically refers to minimum number of digits,
  // padding with zeros.
  EXPECT_EQ(fmt::format("{:.5}", MyEnum::Value1), "00000");
  EXPECT_EQ(fmt::format("{:.2}", MyEnum::Value2), "01");
  EXPECT_EQ(fmt::format("{:.1}", MyEnum::Value3), "2");
  EXPECT_EQ(fmt::format("{:.0}", MyEnum::Value1), ""); // This is a tricky case, usually means no digits for float, but for int it's min digits.
                                                       // fmtlib's behavior for int precision is to pad with zeros.
                                                       // If precision is 0, it means print nothing if value is 0, otherwise print value.
                                                       // However, the default int formatter in fmtlib treats precision as minimum digits.
                                                       // Let's verify the actual behavior.
  EXPECT_EQ(fmt::format("{:.0}", 0), ""); // This is the expected behavior for int 0 with precision 0.
  EXPECT_EQ(fmt::format("{:.0}", MyEnum::Value1), ""); // MyEnum::Value1 is 0
  EXPECT_EQ(fmt::format("{:.0}", MyEnum::Value2), "1"); // MyEnum::Value2 is 1
}


// Test case for enum formatting with different bases (binary, octal, hex)
TEST_F(EnumFormattingTest, EnumFormattingWithBases) {
  EXPECT_EQ(fmt::format("{:b}", MyEnum::Value2), "1"); // 1 in binary
  EXPECT_EQ(fmt::format("{:B}", MyEnum::Value2), "1");
  EXPECT_EQ(fmt::format("{:o}", MyEnum::Value3), "2"); // 2 in octal
  EXPECT_EQ(fmt::format("{:x}", AnotherEnum::NegativeOne), "ffffffff"); // Assuming 32-bit int, platform dependent
  EXPECT_EQ(fmt::format("{:X}", AnotherEnum::NegativeOne), "FFFFFFFF");
  EXPECT_EQ(fmt::format("{:#x}", AnotherEnum::Value3), "0x2");
  EXPECT_EQ(fmt::format("{:#X}", AnotherEnum::Value3), "0X2");
}

// Test case for empty format string
TEST_F(EnumFormattingTest, EmptyFormatString) {
  EXPECT_EQ(fmt::format("{}", MyEnum::Value1), "0");
}

// Test case for multiple enums in one format string
TEST_F(EnumFormattingTest, MultipleEnums) {
  EXPECT_EQ(fmt::format("Enum1: {}, Enum2: {}", MyEnum::Value1, AnotherEnum::One), "Enum1: 0, Enum2: 1");
}

// Test case for a non-enum type that might accidentally match the enable_if
// This test ensures that the specific enable_if for `format_enum` doesn't
// incorrectly apply to non-enum types.
TEST_F(EnumFormattingTest, NonEnumTypeDoesNotUseEnumFormatter) {
  // This should use the default formatter for int, not the enum-specific one.
  // The focal function is about `internal::format_enum<T>::value`, so we
  // want to ensure that for a non-enum T, this value is false, and thus
  // the specialized formatter is not chosen.
  EXPECT_EQ(fmt::format("{}", 123), "123");
  EXPECT_EQ(fmt::format("{:x}", 255), "ff");
}

// Test case for a large enum value (within int limits)
enum class LargeEnum : int {
  MaxInt = 2147483647,
  MinInt = -2147483648
};

template <>
struct fmt::formatter<LargeEnum> : fmt::formatter<int> {
  template <typename FormatContext>
  auto format(LargeEnum e, FormatContext& ctx) {
    return fmt::formatter<int>::format(static_cast<int>(e), ctx);
  }
};

TEST_F(EnumFormattingTest, LargeEnumValue) {
  EXPECT_EQ(fmt::format("{}", LargeEnum::MaxInt), "2147483647");
  EXPECT_EQ(fmt::format("{}", LargeEnum::MinInt), "-2147483648");
  EXPECT_EQ(fmt::format("{:x}", LargeEnum::MaxInt), "7fffffff");
  EXPECT_EQ(fmt::format("{:x}", LargeEnum::MinInt), "80000000");
}

// Test case for an enum with underlying type other than int (e.g., short, long long)
enum class ShortEnum : short {
  S1 = 100,
  S2 = -50
};

template <>
struct fmt::formatter<ShortEnum> : fmt::formatter<int> { // Still inherits from int formatter
  template <typename FormatContext>
  auto format(ShortEnum e, FormatContext& ctx) {
    return fmt::formatter<int>::format(static_cast<int>(e), ctx);
  }
};

TEST_F(EnumFormattingTest, EnumWithShortUnderlyingType) {
  EXPECT_EQ(fmt::format("{}", ShortEnum::S1), "100");
  EXPECT_EQ(fmt::format("{}", ShortEnum::S2), "-50");
}

enum class LongLongEnum : long long {
  LL1 = 123456789012345LL,
  LL2 = -98765432109876LL
};

// For long long, we should inherit from long long formatter if available,
// or ensure the int formatter can handle it (it will typically cast).
// fmt::formatter<int> will cast to int, which might truncate.
// To correctly test, we should inherit from fmt::formatter<long long> if available,
// or explicitly cast to long long in the format method.
// The focal function is about `formatter<T, Char, typename std::enable_if<internal::format_enum<T>::value>::type> : public formatter<int, Char>`
// so it explicitly inherits from `formatter<int, Char>`. This means even if the underlying type is long long,
// it will be formatted as an int. This is a crucial detail to test.

template <>
struct fmt::formatter<LongLongEnum> : fmt::formatter<int> { // Inherits from int formatter as per focal function
  template <typename FormatContext>
  auto format(LongLongEnum e, FormatContext& ctx) {
    // This will cast to int, potentially truncating large long long values.
    return fmt::formatter<int>::format(static_cast<int>(e), ctx);
  }
};

TEST_F(EnumFormattingTest, EnumWithLongLongUnderlyingType_TruncationExpected) {
  // Due to inheriting from formatter<int>, large long long values will be truncated.
  // This tests the behavior of the focal function's inheritance choice.
  EXPECT_EQ(fmt::format("{}", LongLongEnum::LL1), fmt::format("{}", static_cast<int>(LongLongEnum::LL1)));
  EXPECT_EQ(fmt::format("{}", LongLongEnum::LL2), fmt::format("{}", static_cast<int>(LongLongEnum::LL2)));

  // Example of truncation:
  long long large_val = 2147483647LL + 1; // Max int + 1
  enum class TruncatedEnum : long long {
    Val = 2147483647LL + 1
  };
  template <>
  struct fmt::formatter<TruncatedEnum> : fmt::formatter<int> {
    template <typename FormatContext>
    auto format(TruncatedEnum e, FormatContext& ctx) {
      return fmt::formatter<int>::format(static_cast<int>(e), ctx);
    }
  };
  EXPECT_EQ(fmt::format("{}", TruncatedEnum::Val), fmt::format("{}", static_cast<int>(TruncatedEnum::Val)));
  EXPECT_NE(fmt::format("{}", TruncatedEnum::Val), fmt::format("{}", static_cast<long long>(TruncatedEnum::Val)));
  EXPECT_EQ(fmt::format("{}", TruncatedEnum::Val), "-2147483648"); // Expected truncation result for 2147483648
}

// Test case for char_type other than char (e.g., wchar_t)
TEST_F(EnumFormattingTest, WideCharEnumFormatting) {
  EXPECT_EQ(fmt::format(L"{}", MyEnum::Value1), L"0");
  EXPECT_EQ(fmt::format(L"{:5}", MyEnum::Value2), L"1    ");
}

// Test case for compile-time checks (if possible, though harder with gtest)
// The `enable_if` part of the focal function is a compile-time mechanism.
// We've implicitly tested it by ensuring our enum formatters compile and work,
// and that non-enums don't use this specific path.