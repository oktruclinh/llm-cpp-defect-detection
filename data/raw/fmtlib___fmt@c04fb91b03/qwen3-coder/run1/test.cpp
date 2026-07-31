#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Test the formatter specialization for enum types
enum TestEnum {
    VALUE_A = 1,
    VALUE_B = 2,
    VALUE_C = 3
};

// Enable formatting for our test enum
template <>
struct fmt::formatter<TestEnum> : fmt::formatter<int> {
    template <typename FormatContext>
    auto format(TestEnum e, FormatContext& ctx) {
        return formatter<int>::format(static_cast<int>(e), ctx);
    }
};

TEST(EnumFormatterTest, BasicFormatting) {
    TestEnum test_val = VALUE_B;
    std::string result = fmt::format("{}", test_val);
    EXPECT_EQ("2", result);
}

TEST(EnumFormatterTest, FormattingWithPrecision) {
    TestEnum test_val = VALUE_C;
    std::string result = fmt::format("{:.2}", test_val);
    EXPECT_EQ("02", result);
}

TEST(EnumFormatterTest, FormattingWithWidth) {
    TestEnum test_val = VALUE_A;
    std::string result = fmt::format("{:4}", test_val);
    EXPECT_EQ("   1", result);
}

TEST(EnumFormatterTest, FormattingWithZeroPadding) {
    TestEnum test_val = VALUE_B;
    std::string result = fmt::format("{:03}", test_val);
    EXPECT_EQ("002", result);
}

TEST(EnumFormatterTest, FormattingWithCustomWidthAndPrecision) {
    TestEnum test_val = VALUE_C;
    std::string result = fmt::format("{:05.2}", test_val);
    EXPECT_EQ("00003", result);
}

TEST(EnumFormatterTest, EdgeCaseZeroValue) {
    enum class EdgeEnum : int { ZERO = 0, ONE = 1 };
    EdgeEnum test_val = EdgeEnum::ZERO;
    std::string result = fmt::format("{}", test_val);
    EXPECT_EQ("0", result);
}

TEST(EnumFormatterTest, EdgeCaseMaxValue) {
    enum class MaxEnum : int { MAX_VAL = std::numeric_limits<int>::max() };
    MaxEnum test_val = MaxEnum::MAX_VAL;
    std::string result = fmt::format("{}", test_val);
    EXPECT_EQ(std::to_string(std::numeric_limits<int>::max()), result);
}

TEST(EnumFormatterTest, EdgeCaseMinValue) {
    enum class MinEnum : int { MIN_VAL = std::numeric_limits<int>::min() };
    MinEnum test_val = MinEnum::MIN_VAL;
    std::string result = fmt::format("{}", test_val);
    EXPECT_EQ(std::to_string(std::numeric_limits<int>::min()), result);
}

TEST(EnumFormatterTest, MultipleEnumsInFormat) {
    TestEnum a = VALUE_A;
    TestEnum b = VALUE_B;
    TestEnum c = VALUE_C;
    std::string result = fmt::format("{} {} {}", a, b, c);
    EXPECT_EQ("1 2 3", result);
}

TEST(EnumFormatterTest, MixedWithStrings) {
    TestEnum test_val = VALUE_B;
    std::string result = fmt::format("Value: {} is between A and C", test_val);
    EXPECT_EQ("Value: 2 is between A and C", result);
}

TEST(EnumFormatterTest, FormattingWithNegativeEnum) {
    enum class NegativeEnum : int { NEG = -5, POS = 5 };
    NegativeEnum test_val = NegativeEnum::NEG;
    std::string result = fmt::format("{}", test_val);
    EXPECT_EQ("-5", result);
}

TEST(EnumFormatterTest, FormattingWithLargeEnum) {
    enum class LargeEnum : long long { LARGE = 123456789012345LL };
    LargeEnum test_val = LargeEnum::LARGE;
    std::string result = fmt::format("{}", test_val);
    EXPECT_EQ("123456789012345", result);
}

TEST(EnumFormatterTest, PrecisionZero) {
    TestEnum test_val = VALUE_C;
    std::string result = fmt::format("{:.0}", test_val);
    EXPECT_EQ("3", result);
}

TEST(EnumFormatterTest, PrecisionExceedsActual) {
    TestEnum test_val = VALUE_A;
    std::string result = fmt::format("{:.10}", test_val);
    EXPECT_EQ("0000000001", result);
}

TEST(EnumFormatterTest, WidthZero) {
    TestEnum test_val = VALUE_B;
    std::string result = fmt::format("{:0}", test_val);
    EXPECT_EQ("2", result);
}

TEST(EnumFormatterTest, WidthNegative) {
    TestEnum test_val = VALUE_A;
    std::string result = fmt::format("{:-5}", test_val);
    EXPECT_EQ("    1", result);
}

TEST(EnumFormatterTest, ComplexFormatSpecs) {
    TestEnum test_val = VALUE_B;
    std::string result = fmt::format("{:0>5.2}", test_val);
    EXPECT_EQ("00002", result);
}