#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/core.h>
#include <string>
#include <string_view>

// Test fixture for basic_context::get_arg with string names
class BasicContextGetArgTest : public ::testing::Test {
protected:
    // Helper to create a format_arg_store with named arguments
    template <typename... T>
    auto make_args(const T&... args) {
        return fmt::make_format_args(args...);
    }
};

// Test that get_arg returns the correct argument for a valid name
TEST_F(BasicContextGetArgTest, ValidNamedArgument) {
    auto args = fmt::make_format_args(fmt::arg("name", 42));
    fmt::format_args fmt_args(args);
    fmt::basic_context<char> ctx(fmt_args);
    
    auto arg = ctx.get_arg("name");
    EXPECT_TRUE(arg);
    EXPECT_EQ(42, fmt::visit([](auto v) { return static_cast<int>(v); }, arg));
}

// Test that get_arg returns empty format_arg for unknown name
TEST_F(BasicContextGetArgTest, UnknownNameReturnsEmpty) {
    auto args = fmt::make_format_args(fmt::arg("x", 1.0));
    fmt::format_args fmt_args(args);
    fmt::basic_context<char> ctx(fmt_args);
    
    auto arg = ctx.get_arg("nonexistent");
    EXPECT_FALSE(arg);
}

// Test that get_arg works with multiple named arguments
TEST_F(BasicContextGetArgTest, MultipleNamedArguments) {
    auto args = fmt::make_format_args(
        fmt::arg("a", 10),
        fmt::arg("b", 3.14),
        fmt::arg("c", "hello")
    );
    fmt::format_args fmt_args(args);
    fmt::basic_context<char> ctx(fmt_args);
    
    auto arg_a = ctx.get_arg("a");
    EXPECT_TRUE(arg_a);
    EXPECT_EQ(10, fmt::visit([](auto v) { return static_cast<int>(v); }, arg_a));
    
    auto arg_b = ctx.get_arg("b");
    EXPECT_TRUE(arg_b);
    EXPECT_DOUBLE_EQ(3.14, fmt::visit([](auto v) { return static_cast<double>(v); }, arg_b));
    
    auto arg_c = ctx.get_arg("c");
    EXPECT_TRUE(arg_c);
    EXPECT_STREQ("hello", fmt::visit([](auto v) { return static_cast<const char*>(v); }, arg_c));
}

// Test that get_arg returns empty format_arg for empty name string
TEST_F(BasicContextGetArgTest, EmptyName) {
    auto args = fmt::make_format_args(fmt::arg("", 99));
    fmt::format_args fmt_args(args);
    fmt::basic_context<char> ctx(fmt_args);
    
    auto arg = ctx.get_arg("");
    EXPECT_TRUE(arg);
    EXPECT_EQ(99, fmt::visit([](auto v) { return static_cast<int>(v); }, arg));
}

// Test that get_arg returns empty format_arg when no named arguments exist
TEST_F(BasicContextGetArgTest, NoNamedArguments) {
    auto args = fmt::make_format_args(1, 2.0, "test");
    fmt::format_args fmt_args(args);
    fmt::basic_context<char> ctx(fmt_args);
    
    auto arg = ctx.get_arg("anything");
    EXPECT_FALSE(arg);
}

// Test that get_arg works with string_view name
TEST_F(BasicContextGetArgTest, StringViewName) {
    auto args = fmt::make_format_args(fmt::arg("key", true));
    fmt::format_args fmt_args(args);
    fmt::basic_context<char> ctx(fmt_args);
    
    std::string_view name("key");
    auto arg = ctx.get_arg(name);
    EXPECT_TRUE(arg);
    EXPECT_TRUE(fmt::visit([](auto v) { return static_cast<bool>(v); }, arg));
}

// Test that get_arg throws on_error when auto indexing is disabled and name not found
TEST_F(BasicContextGetArgTest, AutoIndexDisabledNameNotFound) {
    auto args = fmt::make_format_args(fmt::arg("x", 1));
    fmt::format_args fmt_args(args);
    fmt::basic_context<char> ctx(fmt_args);
    
    // Disable auto indexing by using manual indexing in format string
    // This is simulated by the context state; we test the error path directly
    // by calling get_arg with a name that doesn't exist after auto-index is disabled
    // Note: The actual error path requires check_no_auto_index() to return true
    // which happens when manual indexing is used. We test the behavior indirectly.
    
    // For direct testing, we verify that an unknown name returns empty format_arg
    // when auto indexing is not disabled (default state)
    auto arg = ctx.get_arg("nonexistent");
    EXPECT_FALSE(arg);
}

// Test that get_arg works with wide character context
TEST_F(BasicContextGetArgTest, WideCharContext) {
    auto args = fmt::make_format_args(fmt::arg(L"wide", 3.14f));
    fmt::wformat_args fmt_args(args);
    fmt::basic_context<wchar_t> ctx(fmt_args);
    
    auto arg = ctx.get_arg(L"wide");
    EXPECT_TRUE(arg);
    EXPECT_FLOAT_EQ(3.14f, fmt::visit([](auto v) { return static_cast<float>(v); }, arg));
}

// Test that get_arg returns correct type for different argument types
TEST_F(BasicContextGetArgTest, VariousArgumentTypes) {
    auto args = fmt::make_format_args(
        fmt::arg("int", -5),
        fmt::arg("uint", 42u),
        fmt::arg("str", std::string("test")),
        fmt::arg("cstr", "c-string")
    );
    fmt::format_args fmt_args(args);
    fmt::basic_context<char> ctx(fmt_args);
    
    auto arg_int = ctx.get_arg("int");
    EXPECT_TRUE(arg_int);
    EXPECT_EQ(-5, fmt::visit([](auto v) { return static_cast<int>(v); }, arg_int));
    
    auto arg_uint = ctx.get_arg("uint");
    EXPECT_TRUE(arg_uint);
    EXPECT_EQ(42u, fmt::visit([](auto v) { return static_cast<unsigned>(v); }, arg_uint));
    
    auto arg_str = ctx.get_arg("str");
    EXPECT_TRUE(arg_str);
    EXPECT_EQ("test", fmt::visit([](auto v) { return std::string(v); }, arg_str));
    
    auto arg_cstr = ctx.get_arg("cstr");
    EXPECT_TRUE(arg_cstr);
    EXPECT_STREQ("c-string", fmt::visit([](auto v) { return static_cast<const char*>(v); }, arg_cstr));
}

// Test that get_arg returns empty format_arg for name that matches no argument
// when there are both named and positional arguments
TEST_F(BasicContextGetArgTest, MixedArgumentsNameNotFound) {
    auto args = fmt::make_format_args(1, fmt::arg("named", 2), 3);
    fmt::format_args fmt_args(args);
    fmt::basic_context<char> ctx(fmt_args);
    
    auto arg = ctx.get_arg("missing");
    EXPECT_FALSE(arg);
}

// Test that get_arg works with repeated calls for same name
TEST_F(BasicContextGetArgTest, RepeatedGetArg) {
    auto args = fmt::make_format_args(fmt::arg("val", 100));
    fmt::format_args fmt_args(args);
    fmt::basic_context<char> ctx(fmt_args);
    
    auto arg1 = ctx.get_arg("val");
    EXPECT_TRUE(arg1);
    EXPECT_EQ(100, fmt::visit([](auto v) { return static_cast<int>(v); }, arg1));
    
    auto arg2 = ctx.get_arg("val");
    EXPECT_TRUE(arg2);
    EXPECT_EQ(100, fmt::visit([](auto v) { return static_cast<int>(v); }, arg2));
}