#include <gtest/gtest.h>
#include "fmt/format.h"
#include "fmt/core.h"
#include <string>
#include <string_view>

// Test fixture for basic_context::get_arg with string names
class BasicContextGetArgTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a format argument store with some named arguments
        args_ = fmt::make_format_args(
            fmt::arg("name", std::string("Alice")),
            fmt::arg("age", 30),
            fmt::arg("pi", 3.14159),
            fmt::arg("empty_str", std::string("")),
            fmt::arg("zero", 0)
        );
    }

    fmt::format_args args_;
};

// Test that a valid named argument can be retrieved
TEST_F(BasicContextGetArgTest, GetExistingNamedArg) {
    fmt::basic_context<char> ctx(args_);
    auto arg = ctx.get_arg("name");
    EXPECT_TRUE(arg);
    EXPECT_EQ(fmt::format("{}", arg), "Alice");
}

// Test that multiple named arguments can be retrieved
TEST_F(BasicContextGetArgTest, GetMultipleExistingNamedArgs) {
    fmt::basic_context<char> ctx(args_);
    auto name_arg = ctx.get_arg("name");
    auto age_arg = ctx.get_arg("age");
    auto pi_arg = ctx.get_arg("pi");
    
    EXPECT_TRUE(name_arg);
    EXPECT_TRUE(age_arg);
    EXPECT_TRUE(pi_arg);
    
    EXPECT_EQ(fmt::format("{}", name_arg), "Alice");
    EXPECT_EQ(fmt::format("{}", age_arg), "30");
    EXPECT_EQ(fmt::format("{}", pi_arg), "3.14159");
}

// Test that retrieving an argument with an empty string name throws an error
TEST_F(BasicContextGetArgTest, GetArgWithEmptyName) {
    fmt::basic_context<char> ctx(args_);
    EXPECT_THROW(ctx.get_arg(""), fmt::format_error);
}

// Test that retrieving a non-existent named argument throws an error
TEST_F(BasicContextGetArgTest, GetNonExistentNamedArg) {
    fmt::basic_context<char> ctx(args_);
    EXPECT_THROW(ctx.get_arg("nonexistent"), fmt::format_error);
}

// Test that retrieving an argument with a name that is a substring of an existing one throws
TEST_F(BasicContextGetArgTest, GetPartialNameMatch) {
    fmt::basic_context<char> ctx(args_);
    EXPECT_THROW(ctx.get_arg("nam"), fmt::format_error);
    EXPECT_THROW(ctx.get_arg("name_extra"), fmt::format_error);
}

// Test that retrieving an argument with a name that is a different case throws
TEST_F(BasicContextGetArgTest, GetCaseSensitiveName) {
    fmt::basic_context<char> ctx(args_);
    EXPECT_THROW(ctx.get_arg("Name"), fmt::format_error);
    EXPECT_THROW(ctx.get_arg("NAME"), fmt::format_error);
}

// Test that retrieving an argument with special characters in name throws
TEST_F(BasicContextGetArgTest, GetArgWithSpecialChars) {
    fmt::basic_context<char> ctx(args_);
    EXPECT_THROW(ctx.get_arg("name@123"), fmt::format_error);
    EXPECT_THROW(ctx.get_arg("name with spaces"), fmt::format_error);
}

// Test that retrieving an argument with a very long name throws
TEST_F(BasicContextGetArgTest, GetArgWithLongName) {
    fmt::basic_context<char> ctx(args_);
    std::string long_name(1000, 'x');
    EXPECT_THROW(ctx.get_arg(long_name), fmt::format_error);
}

// Test that retrieving an argument with a name containing null characters throws
TEST_F(BasicContextGetArgTest, GetArgWithNullInName) {
    fmt::basic_context<char> ctx(args_);
    std::string name_with_null = "name\0test";
    EXPECT_THROW(ctx.get_arg(name_with_null), fmt::format_error);
}

// Test that retrieving an empty string value argument works
TEST_F(BasicContextGetArgTest, GetEmptyStringArg) {
    fmt::basic_context<char> ctx(args_);
    auto arg = ctx.get_arg("empty_str");
    EXPECT_TRUE(arg);
    EXPECT_EQ(fmt::format("{}", arg), "");
}

// Test that retrieving a zero value argument works
TEST_F(BasicContextGetArgTest, GetZeroValueArg) {
    fmt::basic_context<char> ctx(args_);
    auto arg = ctx.get_arg("zero");
    EXPECT_TRUE(arg);
    EXPECT_EQ(fmt::format("{}", arg), "0");
}

// Test that get_arg works with wchar_t context
TEST_F(BasicContextGetArgTest, GetArgWithWideString) {
    fmt::basic_context<wchar_t> ctx(fmt::make_format_args<fmt::basic_format_context<wchar_t>>(
        fmt::arg(L"name", std::wstring(L"Alice")),
        fmt::arg(L"age", 30)
    ));
    
    auto name_arg = ctx.get_arg(L"name");
    EXPECT_TRUE(name_arg);
    EXPECT_EQ(fmt::format(L"{}", name_arg), L"Alice");
    
    auto age_arg = ctx.get_arg(L"age");
    EXPECT_TRUE(age_arg);
    EXPECT_EQ(fmt::format(L"{}", age_arg), L"30");
    
    EXPECT_THROW(ctx.get_arg(L"nonexistent"), fmt::format_error);
}

// Test that get_arg works correctly in a formatting context
TEST_F(BasicContextGetArgTest, GetArgInFormattingContext) {
    auto result = fmt::format("{name} is {age} years old", 
                              fmt::arg("name", std::string("Bob")),
                              fmt::arg("age", 25));
    EXPECT_EQ(result, "Bob is 25 years old");
}

// Test that get_arg throws when called on a context with no named arguments
TEST_F(BasicContextGetArgTest, GetArgFromContextWithNoNamedArgs) {
    auto args = fmt::make_format_args(42, 3.14, std::string("hello"));
    fmt::basic_context<char> ctx(args);
    EXPECT_THROW(ctx.get_arg("anything"), fmt::format_error);
}

// Test that get_arg works with multiple calls to the same context
TEST_F(BasicContextGetArgTest, MultipleGetArgCallsOnSameContext) {
    fmt::basic_context<char> ctx(args_);
    
    auto arg1 = ctx.get_arg("name");
    EXPECT_TRUE(arg1);
    EXPECT_EQ(fmt::format("{}", arg1), "Alice");
    
    auto arg2 = ctx.get_arg("age");
    EXPECT_TRUE(arg2);
    EXPECT_EQ(fmt::format("{}", arg2), "30");
    
    // Verify that the context still works after multiple calls
    EXPECT_THROW(ctx.get_arg("nonexistent"), fmt::format_error);
}

// Test that get_arg works with integer arguments
TEST_F(BasicContextGetArgTest, GetIntegerArg) {
    auto args = fmt::make_format_args(
        fmt::arg("int_val", 42),
        fmt::arg("neg_val", -10),
        fmt::arg("large_val", 1000000)
    );
    fmt::basic_context<char> ctx(args);
    
    EXPECT_EQ(fmt::format("{}", ctx.get_arg("int_val")), "42");
    EXPECT_EQ(fmt::format("{}", ctx.get_arg("neg_val")), "-10");
    EXPECT_EQ(fmt::format("{}", ctx.get_arg("large_val")), "1000000");
}

// Test that get_arg works with floating point arguments
TEST_F(BasicContextGetArgTest, GetFloatArg) {
    auto args = fmt::make_format_args(
        fmt::arg("float_val", 3.14f),
        fmt::arg("double_val", 2.71828),
        fmt::arg("neg_float", -1.5f)
    );
    fmt::basic_context<char> ctx(args);
    
    EXPECT_EQ(fmt::format("{:.2f}", ctx.get_arg("float_val")), "3.14");
    EXPECT_EQ(fmt::format("{:.5f}", ctx.get_arg("double_val")), "2.71828");
    EXPECT_EQ(fmt::format("{:.1f}", ctx.get_arg("neg_float")), "-1.5");
}

// Test that get_arg works with boolean arguments
TEST_F(BasicContextGetArgTest, GetBoolArg) {
    auto args = fmt::make_format_args(
        fmt::arg("true_val", true),
        fmt::arg("false_val", false)
    );
    fmt::basic_context<char> ctx(args);
    
    EXPECT_EQ(fmt::format("{}", ctx.get_arg("true_val")), "true");
    EXPECT_EQ(fmt::format("{}", ctx.get_arg("false_val")), "false");
}

// Test that get_arg works with character arguments
TEST_F(BasicContextGetArgTest, GetCharArg) {
    auto args = fmt::make_format_args(
        fmt::arg("char_val", 'A'),
        fmt::arg("digit_char", '5')
    );
    fmt::basic_context<char> ctx(args);
    
    EXPECT_EQ(fmt::format("{}", ctx.get_arg("char_val")), "A");
    EXPECT_EQ(fmt::format("{}", ctx.get_arg("digit_char")), "5");
}

// Test that get_arg works with pointer arguments
TEST_F(BasicContextGetArgTest, GetPointerArg) {
    int value = 42;
    auto args = fmt::make_format_args(
        fmt::arg("ptr", &value)
    );
    fmt::basic_context<char> ctx(args);
    
    auto arg = ctx.get_arg("ptr");
    EXPECT_TRUE(arg);
    // Just verify it's not empty, pointer formatting is implementation-defined
    EXPECT_NE(fmt::format("{}", arg), "");
}

// Test that get_arg works with string_view arguments
TEST_F(BasicContextGetArgTest, GetStringViewArg) {
    std::string_view sv = "test string view";
    auto args = fmt::make_format_args(
        fmt::arg("sv", sv)
    );
    fmt::basic_context<char> ctx(args);
    
    EXPECT_EQ(fmt::format("{}", ctx.get_arg("sv")), "test string view");
}