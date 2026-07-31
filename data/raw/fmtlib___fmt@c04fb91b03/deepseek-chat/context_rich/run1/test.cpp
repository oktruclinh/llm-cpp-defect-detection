#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <cmath>
#include <limits>
#include <cstdint>

// Test formatter for basic integer types
TEST(FormatterTest, BasicIntFormat) {
    EXPECT_EQ(fmt::format("{}", 42), "42");
    EXPECT_EQ(fmt::format("{:d}", 42), "42");
    EXPECT_EQ(fmt::format("{:x}", 255), "ff");
    EXPECT_EQ(fmt::format("{:X}", 255), "FF");
    EXPECT_EQ(fmt::format("{:o}", 8), "10");
    EXPECT_EQ(fmt::format("{:b}", 6), "110");
    EXPECT_EQ(fmt::format("{:+}", 42), "+42");
    EXPECT_EQ(fmt::format("{: }", 42), " 42");
    EXPECT_EQ(fmt::format("{:-}", 42), "42");
    EXPECT_EQ(fmt::format("{:0>5}", 42), "00042");
    EXPECT_EQ(fmt::format("{:*<5}", 42), "42***");
    EXPECT_EQ(fmt::format("{:^5}", 42), " 42  ");
}

// Test formatter for unsigned integers
TEST(FormatterTest, UnsignedIntFormat) {
    EXPECT_EQ(fmt::format("{}", 42u), "42");
    EXPECT_EQ(fmt::format("{:d}", 42u), "42");
    EXPECT_EQ(fmt::format("{:x}", 255u), "ff");
    EXPECT_EQ(fmt::format("{:X}", 255u), "FF");
}

// Test formatter for long long types
TEST(FormatterTest, LongLongFormat) {
    EXPECT_EQ(fmt::format("{}", 1234567890123LL), "1234567890123");
    EXPECT_EQ(fmt::format("{}", 1234567890123ULL), "1234567890123");
}

// Test formatter for bool
TEST(FormatterTest, BoolFormat) {
    EXPECT_EQ(fmt::format("{}", true), "true");
    EXPECT_EQ(fmt::format("{}", false), "false");
    EXPECT_EQ(fmt::format("{:d}", true), "1");
    EXPECT_EQ(fmt::format("{:d}", false), "0");
}

// Test formatter for char
TEST(FormatterTest, CharFormat) {
    EXPECT_EQ(fmt::format("{}", 'A'), "A");
    EXPECT_EQ(fmt::format("{:c}", 'A'), "A");
    EXPECT_EQ(fmt::format("{:d}", 'A'), "65");
    EXPECT_EQ(fmt::format("{:x}", 'A'), "41");
    EXPECT_EQ(fmt::format("{:b}", 'A'), "1000001");
}

// Test formatter for floating point types
TEST(FormatterTest, FloatFormat) {
    EXPECT_EQ(fmt::format("{}", 3.14), "3.14");
    EXPECT_EQ(fmt::format("{:f}", 3.14), "3.140000");
    EXPECT_EQ(fmt::format("{:e}", 3.14), "3.140000e+00");
    EXPECT_EQ(fmt::format("{:E}", 3.14), "3.140000E+00");
    EXPECT_EQ(fmt::format("{:g}", 3.14), "3.14");
    EXPECT_EQ(fmt::format("{:G}", 3.14), "3.14");
    EXPECT_EQ(fmt::format("{:.2f}", 3.14159), "3.14");
    EXPECT_EQ(fmt::format("{:+.2f}", 3.14), "+3.14");
    EXPECT_EQ(fmt::format("{: .2f}", 3.14), " 3.14");
    EXPECT_EQ(fmt::format("{:0>8.2f}", 3.14), "00003.14");
}

// Test formatter for long double
TEST(FormatterTest, LongDoubleFormat) {
    long double ld = 3.14159265358979323846L;
    EXPECT_EQ(fmt::format("{}", ld), "3.141592653589793");
}

// Test formatter for C strings
TEST(FormatterTest, CStringFormat) {
    EXPECT_EQ(fmt::format("{}", "hello"), "hello");
    EXPECT_EQ(fmt::format("{:s}", "hello"), "hello");
    EXPECT_EQ(fmt::format("{:>10}", "hello"), "     hello");
    EXPECT_EQ(fmt::format("{:*<10}", "hello"), "hello*****");
}

// Test formatter for std::string
TEST(FormatterTest, StringFormat) {
    std::string s = "world";
    EXPECT_EQ(fmt::format("{}", s), "world");
    EXPECT_EQ(fmt::format("{:s}", s), "world");
    EXPECT_EQ(fmt::format("{:>10}", s), "     world");
    EXPECT_EQ(fmt::format("{:*<10}", s), "world*****");
}

// Test formatter for pointers
TEST(FormatterTest, PointerFormat) {
    int x = 42;
    EXPECT_EQ(fmt::format("{}", fmt::ptr(&x)), fmt::format("0x{}", fmt::ptr(&x)));
    EXPECT_EQ(fmt::format("{:p}", fmt::ptr(&x)), fmt::format("0x{}", fmt::ptr(&x)));
}

// Test dynamic format specs
TEST(FormatterTest, DynamicWidthAndPrecision) {
    EXPECT_EQ(fmt::format("{:{}}", 42, 5), "   42");
    EXPECT_EQ(fmt::format("{:.{}}", 3.14159, 2), "3.14");
    EXPECT_EQ(fmt::format("{:>{}}", "hello", 10), "     hello");
}

// Test error handling for invalid format specs
TEST(FormatterTest, InvalidFormatSpecs) {
    EXPECT_THROW(fmt::format("{:}", 42), fmt::format_error);
    EXPECT_THROW(fmt::format("{:.}", 42), fmt::format_error);
    EXPECT_THROW(fmt::format("{:a}", 42), fmt::format_error);
    EXPECT_THROW(fmt::format("{:L}", 42), fmt::format_error);
}

// Test edge cases with numeric limits
TEST(FormatterTest, NumericLimits) {
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::max()), "2147483647");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::min()), "-2147483648");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<unsigned>::max()), "4294967295");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<double>::max()), "1.7976931348623157e+308");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<double>::min()), "2.2250738585072014e-308");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<double>::infinity()), "inf");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<double>::quiet_NaN()), "nan");
}

// Test zero and negative values
TEST(FormatterTest, ZeroAndNegative) {
    EXPECT_EQ(fmt::format("{}", 0), "0");
    EXPECT_EQ(fmt::format("{}", -0.0), "-0.000000");
    EXPECT_EQ(fmt::format("{}", -42), "-42");
    EXPECT_EQ(fmt::format("{:+}", 0), "+0");
    EXPECT_EQ(fmt::format("{: }", 0), " 0");
}

// Test alignment with different types
TEST(FormatterTest, Alignment) {
    EXPECT_EQ(fmt::format("{:>10}", 42), "        42");
    EXPECT_EQ(fmt::format("{:<10}", 42), "42        ");
    EXPECT_EQ(fmt::format("{:^10}", 42), "    42    ");
    EXPECT_EQ(fmt::format("{:=>10}", 42), "========42");
    EXPECT_EQ(fmt::format("{:=<10}", 42), "42========");
    EXPECT_EQ(fmt::format("{:=^10}", 42), "====42====");
}

// Test fill character
TEST(FormatterTest, FillCharacter) {
    EXPECT_EQ(fmt::format("{:*>5}", 42), "***42");
    EXPECT_EQ(fmt::format("{:*<5}", 42), "42***");
    EXPECT_EQ(fmt::format("{:*^5}", 42), "*42**");
}

// Test sign options
TEST(FormatterTest, SignOptions) {
    EXPECT_EQ(fmt::format("{:+}", 42), "+42");
    EXPECT_EQ(fmt::format("{:+}", -42), "-42");
    EXPECT_EQ(fmt::format("{: }", 42), " 42");
    EXPECT_EQ(fmt::format("{: }", -42), "-42");
    EXPECT_EQ(fmt::format("{:-}", 42), "42");
    EXPECT_EQ(fmt::format("{:-}", -42), "-42");
}

// Test alternate form
TEST(FormatterTest, AlternateForm) {
    EXPECT_EQ(fmt::format("{:#x}", 255), "0xff");
    EXPECT_EQ(fmt::format("{:#X}", 255), "0XFF");
    EXPECT_EQ(fmt::format("{:#o}", 8), "010");
    EXPECT_EQ(fmt::format("{:#b}", 6), "0b110");
    EXPECT_EQ(fmt::format("{:#.0f}", 3.0), "3.");
}

// Test width and precision with strings
TEST(FormatterTest, StringWidthPrecision) {
    EXPECT_EQ(fmt::format("{:10}", "hello"), "hello     ");
    EXPECT_EQ(fmt::format("{:.3}", "hello"), "hel");
    EXPECT_EQ(fmt::format("{:10.3}", "hello"), "hel       ");
}

// Test format with named arguments
TEST(FormatterTest, NamedArguments) {
    EXPECT_EQ(fmt::format("{name}", fmt::arg("name", "world")), "world");
    EXPECT_EQ(fmt::format("{name:>10}", fmt::arg("name", "hello")), "     hello");
}

// Test format with multiple arguments
TEST(FormatterTest, MultipleArguments) {
    EXPECT_EQ(fmt::format("{} {}", 1, 2), "1 2");
    EXPECT_EQ(fmt::format("{1} {0}", 1, 2), "2 1");
    EXPECT_EQ(fmt::format("{0} {0}", 42), "42 42");
}

// Test format with positional arguments
TEST(FormatterTest, PositionalArguments) {
    EXPECT_EQ(fmt::format("{0} {1} {0}", "a", "b"), "a b a");
    EXPECT_EQ(fmt::format("{2} {1} {0}", 1, 2, 3), "3 2 1");
}

// Test format with empty format string
TEST(FormatterTest, EmptyFormatString) {
    EXPECT_EQ(fmt::format(""), "");
    EXPECT_EQ(fmt::format("{}", 42), "42");
}

// Test format with special characters
TEST(FormatterTest, SpecialCharacters) {
    EXPECT_EQ(fmt::format("{}", '\n'), "\n");
    EXPECT_EQ(fmt::format("{}", '\t'), "\t");
    EXPECT_EQ(fmt::format("{}", '\0'), "\0");
}

// Test format with enum types (if applicable)
enum class Color { Red, Green, Blue };
template <>
struct fmt::formatter<Color> : fmt::formatter<int> {
    auto format(Color c, fmt::format_context& ctx) -> decltype(ctx.out()) {
        return fmt::formatter<int>::format(static_cast<int>(c), ctx);
    }
};

TEST(FormatterTest, EnumFormat) {
    EXPECT_EQ(fmt::format("{}", Color::Red), "0");
    EXPECT_EQ(fmt::format("{}", Color::Green), "1");
    EXPECT_EQ(fmt::format("{}", Color::Blue), "2");
}

// Test dynamic_formatter (if available)
TEST(DynamicFormatterTest, BasicUsage) {
    fmt::dynamic_formatter<> df;
    // Note: dynamic_formatter requires a context to be created properly
    // This test verifies it can be instantiated
    (void)df;
}

// Test format with large numbers
TEST(FormatterTest, LargeNumbers) {
    EXPECT_EQ(fmt::format("{}", 1000000), "1000000");
    EXPECT_EQ(fmt::format("{:,}", 1000000), "1,000,000");
    EXPECT_EQ(fmt::format("{:f}", 1e20), "100000000000000000000.000000");
}

// Test format with negative zero
TEST(FormatterTest, NegativeZero) {
    double neg_zero = -0.0;
    EXPECT_EQ(fmt::format("{:f}", neg_zero), "-0.000000");
    EXPECT_EQ(fmt::format("{:g}", neg_zero), "-0");
}

// Test format with infinity and NaN
TEST(FormatterTest, InfinityAndNaN) {
    EXPECT_EQ(fmt::format("{:f}", std::numeric_limits<double>::infinity()), "inf");
    EXPECT_EQ(fmt::format("{:F}", std::numeric_limits<double>::infinity()), "INF");
    EXPECT_EQ(fmt::format("{:f}", std::numeric_limits<double>::quiet_NaN()), "nan");
    EXPECT_EQ(fmt::format("{:F}", std::numeric_limits<double>::quiet_NaN()), "NAN");
}