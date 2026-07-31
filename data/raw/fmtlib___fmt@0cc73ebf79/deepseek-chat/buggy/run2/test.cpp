#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <string_view>

// Test fixture for named argument ID lookup
class NamedArgIdTest : public ::testing::Test {
protected:
    // Helper to create a format string with named arguments and extract IDs
    int get_named_arg_id(const std::string& format_str, const std::string& arg_name) {
        auto args = fmt::make_format_args(
            fmt::arg("a", 1),
            fmt::arg("b", 2.5),
            fmt::arg("c", "hello"),
            fmt::arg("d", 42)
        );
        // We need to access the internal named_args through the format_args
        // The get_id function is called internally by fmt when parsing format strings
        // We'll test indirectly by formatting and checking behavior
        return -1; // Placeholder - we'll test through actual formatting
    }
};

// Test that named arguments are correctly identified by name
TEST_F(NamedArgIdTest, BasicNamedArgLookup) {
    // Test that named arguments work correctly in formatting
    std::string result = fmt::format("{a} {b} {c} {d}", 
        fmt::arg("a", 10), 
        fmt::arg("b", 20.5), 
        fmt::arg("c", "world"),
        fmt::arg("d", 100));
    EXPECT_EQ(result, "10 20.5 world 100");
}

// Test that named arguments can be used in any order
TEST_F(NamedArgIdTest, NamedArgReordering) {
    std::string result = fmt::format("{d} {c} {b} {a}",
        fmt::arg("a", 1),
        fmt::arg("b", 2.0),
        fmt::arg("c", "test"),
        fmt::arg("d", 99));
    EXPECT_EQ(result, "99 test 2 1");
}

// Test that named arguments can be mixed with positional arguments
TEST_F(NamedArgIdTest, MixedNamedAndPositional) {
    std::string result = fmt::format("{0} {name} {1}", 
        "first", 
        "second",
        fmt::arg("name", "named_value"));
    EXPECT_EQ(result, "first named_value second");
}

// Test that duplicate named arguments use the last one
TEST_F(NamedArgIdTest, DuplicateNamedArg) {
    std::string result = fmt::format("{name}", 
        fmt::arg("name", "first"),
        fmt::arg("name", "second"));
    EXPECT_EQ(result, "second");
}

// Test that named arguments work with different types
TEST_F(NamedArgIdTest, NamedArgDifferentTypes) {
    std::string result = fmt::format("{int_val} {float_val} {str_val} {char_val}",
        fmt::arg("int_val", 42),
        fmt::arg("float_val", 3.14f),
        fmt::arg("str_val", std::string("hello")),
        fmt::arg("char_val", 'X'));
    EXPECT_EQ(result, "42 3.14 hello X");
}

// Test that named arguments work with width and precision specifiers
TEST_F(NamedArgIdTest, NamedArgWithFormatSpecifiers) {
    std::string result = fmt::format("{val:>10} {val:.2f}",
        fmt::arg("val", 3.14159));
    EXPECT_EQ(result, "    3.14159 3.14");
}

// Test that named arguments work with fill and alignment
TEST_F(NamedArgIdTest, NamedArgWithFillAndAlign) {
    std::string result = fmt::format("{name:*^10}",
        fmt::arg("name", "test"));
    EXPECT_EQ(result, "***test***");
}

// Test that named arguments work with integer formatting
TEST_F(NamedArgIdTest, NamedArgIntegerFormatting) {
    std::string result = fmt::format("{val:#x} {val:#o} {val:#b}",
        fmt::arg("val", 255));
    EXPECT_EQ(result, "0xff 0377 0b11111111");
}

// Test that named arguments work with string formatting
TEST_F(NamedArgIdTest, NamedArgStringFormatting) {
    std::string result = fmt::format("{:.3s} {:.>8}",
        fmt::arg("s", "hello world"),
        fmt::arg("pad", "end"));
    EXPECT_EQ(result, "hel end.....");
}

// Test that named arguments work with dynamic width from another named arg
TEST_F(NamedArgIdTest, NamedArgDynamicWidth) {
    std::string result = fmt::format("{:{width}}",
        fmt::arg("value", 42),
        fmt::arg("width", 10));
    EXPECT_EQ(result, "        42");
}

// Test that named arguments work with dynamic precision from another named arg
TEST_F(NamedArgIdTest, NamedArgDynamicPrecision) {
    std::string result = fmt::format("{:.{prec}f}",
        fmt::arg("value", 3.14159),
        fmt::arg("prec", 2));
    EXPECT_EQ(result, "3.14");
}

// Test that named arguments work with multiple dynamic specifiers
TEST_F(NamedArgIdTest, NamedArgMultipleDynamicSpecifiers) {
    std::string result = fmt::format("{:{fill}{width}.{prec}}",
        fmt::arg("value", 3.14159),
        fmt::arg("fill", '*'),
        fmt::arg("width", 10),
        fmt::arg("prec", 3));
    EXPECT_EQ(result, "*****3.142");
}

// Test that named arguments work with chrono types
TEST_F(NamedArgIdTest, NamedArgChrono) {
    using namespace std::chrono_literals;
    auto dur = 12345ms;
    std::string result = fmt::format("{duration:%S}", 
        fmt::arg("duration", dur));
    EXPECT_EQ(result, "12.345");
}

// Test that named arguments work with custom formatting
struct Point {
    int x, y;
};

template <>
struct fmt::formatter<Point> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }
    
    template <typename FormatContext>
    auto format(const Point& p, FormatContext& ctx) -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

TEST_F(NamedArgIdTest, NamedArgCustomType) {
    Point p{3, 4};
    std::string result = fmt::format("{point}", 
        fmt::arg("point", p));
    EXPECT_EQ(result, "(3, 4)");
}

// Test that named arguments work with repeated usage
TEST_F(NamedArgIdTest, NamedArgRepeatedUsage) {
    std::string result = fmt::format("{name} {name} {name}",
        fmt::arg("name", "repeat"));
    EXPECT_EQ(result, "repeat repeat repeat");
}

// Test that named arguments work with empty string names
TEST_F(NamedArgIdTest, NamedArgEmptyName) {
    std::string result = fmt::format("{}", 
        fmt::arg("", "empty_name"));
    // Empty name should still work as a named argument
    EXPECT_EQ(result, "empty_name");
}

// Test that named arguments work with long names
TEST_F(NamedArgIdTest, NamedArgLongName) {
    std::string long_name = "a_very_long_named_argument_that_should_still_work";
    std::string result = fmt::format("{" + long_name + "}", 
        fmt::arg(long_name, "long_value"));
    EXPECT_EQ(result, "long_value");
}

// Test that named arguments work with Unicode names
TEST_F(NamedArgIdTest, NamedArgUnicodeName) {
    std::string result = fmt::format("{名前} {αβγ}",
        fmt::arg("名前", "unicode1"),
        fmt::arg("αβγ", "unicode2"));
    EXPECT_EQ(result, "unicode1 unicode2");
}

// Test that named arguments work with numeric names (as strings)
TEST_F(NamedArgIdTest, NamedArgNumericStringName) {
    std::string result = fmt::format("{123}",
        fmt::arg("123", "numeric_name"));
    EXPECT_EQ(result, "numeric_name");
}

// Test that named arguments work with special characters in names
TEST_F(NamedArgIdTest, NamedArgSpecialCharName) {
    std::string result = fmt::format("{under_score} {hyphen-name}",
        fmt::arg("under_score", "underscore"),
        fmt::arg("hyphen-name", "hyphen"));
    EXPECT_EQ(result, "underscore hyphen");
}

// Test that named arguments work with boolean values
TEST_F(NamedArgIdTest, NamedArgBoolean) {
    std::string result = fmt::format("{flag} {flag}",
        fmt::arg("flag", true));
    EXPECT_EQ(result, "true true");
}

// Test that named arguments work with pointer values
TEST_F(NamedArgIdTest, NamedArgPointer) {
    int value = 42;
    int* ptr = &value;
    std::string result = fmt::format("{ptr}",
        fmt::arg("ptr", ptr));
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "nullptr");
}

// Test that named arguments work with enum values
enum Color { Red, Green, Blue };

TEST_F(NamedArgIdTest, NamedArgEnum) {
    std::string result = fmt::format("{color}",
        fmt::arg("color", Green));
    EXPECT_EQ(result, "1");
}

// Test that named arguments work with std::optional
TEST_F(NamedArgIdTest, NamedArgOptional) {
    std::optional<int> opt_val = 42;
    std::string result = fmt::format("{opt}",
        fmt::arg("opt", opt_val));
    EXPECT_EQ(result, "42");
}

// Test that named arguments work with std::variant
TEST_F(NamedArgIdTest, NamedArgVariant) {
    std::variant<int, std::string> var = "hello";
    std::string result = fmt::format("{var}",
        fmt::arg("var", var));
    EXPECT_EQ(result, "hello");
}

// Test that named arguments work with nested format specs
TEST_F(NamedArgIdTest, NamedArgNestedSpecs) {
    std::string result = fmt::format("{0:{1}}",
        fmt::arg("value", 42),
        fmt::arg("width", 10));
    EXPECT_EQ(result, "        42");
}