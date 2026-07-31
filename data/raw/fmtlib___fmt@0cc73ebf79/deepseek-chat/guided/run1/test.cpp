#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <vector>

// Test fixture for named argument ID lookup tests
class NamedArgIdTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

// Test that get_id returns -1 when no named arguments are present
TEST_F(NamedArgIdTest, NoNamedArgsReturnsMinusOne) {
    // Format string without named arguments
    auto result = fmt::format("{} {}", 1, 2);
    // We can't directly test get_id, but we can verify that named args don't work
    // This tests the underlying behavior indirectly
    EXPECT_EQ(result, "1 2");
}

// Test that named arguments can be accessed by name
TEST_F(NamedArgIdTest, NamedArgAccessByString) {
    auto result = fmt::format("{name} is {age} years old", 
                              fmt::arg("name", "Alice"), 
                              fmt::arg("age", 30));
    EXPECT_EQ(result, "Alice is 30 years old");
}

// Test that named arguments work with different string types
TEST_F(NamedArgIdTest, NamedArgWithStringView) {
    std::string name = "Bob";
    auto result = fmt::format("{name}", fmt::arg("name", name));
    EXPECT_EQ(result, "Bob");
}

// Test that named arguments work with const char*
TEST_F(NamedArgIdTest, NamedArgWithConstCharPtr) {
    auto result = fmt::format("{greeting}", fmt::arg("greeting", "Hello"));
    EXPECT_EQ(result, "Hello");
}

// Test that named arguments with empty name string
TEST_F(NamedArgIdTest, NamedArgWithEmptyName) {
    auto result = fmt::format("{}", fmt::arg("", "value"));
    // Empty name should not match any placeholder
    EXPECT_EQ(result, "value");
}

// Test that named arguments with special characters in name
TEST_F(NamedArgIdTest, NamedArgWithSpecialChars) {
    auto result = fmt::format("{my_var}", fmt::arg("my_var", 42));
    EXPECT_EQ(result, "42");
}

// Test that named arguments work with multiple arguments
TEST_F(NamedArgIdTest, MultipleNamedArgs) {
    auto result = fmt::format("{a}{b}{c}", 
                              fmt::arg("a", 1), 
                              fmt::arg("b", 2), 
                              fmt::arg("c", 3));
    EXPECT_EQ(result, "123");
}

// Test that named arguments can be mixed with positional arguments
TEST_F(NamedArgIdTest, MixedNamedAndPositional) {
    auto result = fmt::format("{} {name}", fmt::arg("name", "World"), "Hello");
    EXPECT_EQ(result, "Hello World");
}

// Test that named argument lookup is case-sensitive
TEST_F(NamedArgIdTest, CaseSensitiveNameLookup) {
    auto result = fmt::format("{Name}", fmt::arg("name", "value"));
    // "Name" != "name", so the placeholder should remain
    EXPECT_EQ(result, "{Name}");
}

// Test that named arguments work with repeated names (last one wins)
TEST_F(NamedArgIdTest, RepeatedNamedArgLastWins) {
    auto result = fmt::format("{x}", fmt::arg("x", 1), fmt::arg("x", 2));
    EXPECT_EQ(result, "2");
}

// Test that named arguments work with integer names
TEST_F(NamedArgIdTest, NamedArgWithNumericName) {
    auto result = fmt::format("{123}", fmt::arg("123", "test"));
    EXPECT_EQ(result, "test");
}

// Test that named arguments work with long names
TEST_F(NamedArgIdTest, NamedArgWithLongName) {
    std::string long_name(1000, 'a');
    auto result = fmt::format("{name}", fmt::arg(long_name, "value"));
    EXPECT_EQ(result, "{name}");  // Long name doesn't match short placeholder
}

// Test that named arguments work with Unicode names
TEST_F(NamedArgIdTest, NamedArgWithUnicodeName) {
    auto result = fmt::format("{名前}", fmt::arg("名前", "value"));
    EXPECT_EQ(result, "value");
}

// Test that get_id returns correct ID for multiple named args
TEST_F(NamedArgIdTest, MultipleNamedArgsCorrectOrder) {
    auto result = fmt::format("{second}{first}", 
                              fmt::arg("first", "1"), 
                              fmt::arg("second", "2"));
    EXPECT_EQ(result, "21");
}

// Test that named arguments work with format specifiers
TEST_F(NamedArgIdTest, NamedArgWithFormatSpec) {
    auto result = fmt::format("{value:05d}", fmt::arg("value", 42));
    EXPECT_EQ(result, "00042");
}

// Test that named arguments work with dynamic width
TEST_F(NamedArgIdTest, NamedArgWithDynamicWidth) {
    auto result = fmt::format("{:{width}}", fmt::arg("width", 10), "test");
    EXPECT_EQ(result, "test      ");
}

// Test that named arguments work with dynamic precision
TEST_F(NamedArgIdTest, NamedArgWithDynamicPrecision) {
    auto result = fmt::format("{:.{prec}f}", fmt::arg("prec", 3), 3.14159);
    EXPECT_EQ(result, "3.142");
}

// Test that named arguments work in nested format specs
TEST_F(NamedArgIdTest, NamedArgInNestedSpec) {
    auto result = fmt::format("{:>{width}}", fmt::arg("width", 10), "test");
    EXPECT_EQ(result, "      test");
}

// Test that named arguments work with custom types
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

TEST_F(NamedArgIdTest, NamedArgWithCustomType) {
    Point p{10, 20};
    auto result = fmt::format("{point}", fmt::arg("point", p));
    EXPECT_EQ(result, "(10, 20)");
}