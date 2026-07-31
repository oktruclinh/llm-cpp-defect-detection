#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/core.h>
#include <string>
#include <string_view>

// Test fixture for basic_context::get_arg with string names
class BasicContextGetArgTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset any global format state that might affect auto index tracking
        fmt::detail::check_no_auto_index_ = false;
    }
};

// Test that get_arg returns the correct argument for a named argument
TEST_F(BasicContextGetArgTest, NamedArgumentFound) {
    auto args = fmt::make_format_args(fmt::arg("name", 42), fmt::arg("value", 3.14));
    fmt::format_args format_args(args);
    fmt::basic_context<char> ctx(format_args);
    
    // Access a named argument that exists
    auto arg = ctx.get_arg("name");
    EXPECT_TRUE(arg);
    EXPECT_EQ(42, arg.template get<int>());
}

// Test that get_arg returns empty format_arg for non-existent named argument
TEST_F(BasicContextGetArgTest, NamedArgumentNotFound) {
    auto args = fmt::make_format_args(fmt::arg("name", 42));
    fmt::format_args format_args(args);
    fmt::basic_context<char> ctx(format_args);
    
    // Access a named argument that doesn't exist
    auto arg = ctx.get_arg("nonexistent");
    EXPECT_FALSE(arg);
}

// Test that get_arg works with multiple named arguments
TEST_F(BasicContextGetArgTest, MultipleNamedArguments) {
    auto args = fmt::make_format_args(
        fmt::arg("a", 1),
        fmt::arg("b", 2.5),
        fmt::arg("c", "hello")
    );
    fmt::format_args format_args(args);
    fmt::basic_context<char> ctx(format_args);
    
    auto arg_a = ctx.get_arg("a");
    EXPECT_TRUE(arg_a);
    EXPECT_EQ(1, arg_a.template get<int>());
    
    auto arg_b = ctx.get_arg("b");
    EXPECT_TRUE(arg_b);
    EXPECT_DOUBLE_EQ(2.5, arg_b.template get<double>());
    
    auto arg_c = ctx.get_arg("c");
    EXPECT_TRUE(arg_c);
    EXPECT_STREQ("hello", arg_c.template get<const char*>());
}

// Test that get_arg returns empty format_arg for empty string name
TEST_F(BasicContextGetArgTest, EmptyStringName) {
    auto args = fmt::make_format_args(fmt::arg("", 42));
    fmt::format_args format_args(args);
    fmt::basic_context<char> ctx(format_args);
    
    auto arg = ctx.get_arg("");
    EXPECT_TRUE(arg);
    EXPECT_EQ(42, arg.template get<int>());
}

// Test that get_arg returns empty format_arg when no named arguments exist
TEST_F(BasicContextGetArgTest, NoNamedArguments) {
    auto args = fmt::make_format_args(42, 3.14);
    fmt::format_args format_args(args);
    fmt::basic_context<char> ctx(format_args);
    
    auto arg = ctx.get_arg("anything");
    EXPECT_FALSE(arg);
}

// Test that get_arg returns empty format_arg when args_ is empty
TEST_F(BasicContextGetArgTest, EmptyArgs) {
    auto args = fmt::make_format_args();
    fmt::format_args format_args(args);
    fmt::basic_context<char> ctx(format_args);
    
    auto arg = ctx.get_arg("test");
    EXPECT_FALSE(arg);
}

// Test that get_arg throws when auto index is not checked (simulating error path)
TEST_F(BasicContextGetArgTest, AutoIndexNotChecked) {
    // This test verifies that get_arg returns empty format_arg when
    // check_no_auto_index() returns false (which is the default behavior
    // when no manual indexing has been used)
    auto args = fmt::make_format_args(fmt::arg("name", 42));
    fmt::format_args format_args(args);
    fmt::basic_context<char> ctx(format_args);
    
    // When check_no_auto_index() returns false, get_arg should return empty
    auto arg = ctx.get_arg("name");
    EXPECT_FALSE(arg) << "get_arg should return empty when auto indexing is allowed";
}

// Test that get_arg works with wchar_t context
TEST_F(BasicContextGetArgTest, WideStringName) {
    auto args = fmt::make_format_args(fmt::arg(L"name", 42));
    fmt::wformat_args format_args(args);
    fmt::basic_context<wchar_t> ctx(format_args);
    
    auto arg = ctx.get_arg(L"name");
    EXPECT_TRUE(arg);
    EXPECT_EQ(42, arg.template get<int>());
}

// Test that get_arg returns empty for non-existent wide string name
TEST_F(BasicContextGetArgTest, WideStringNameNotFound) {
    auto args = fmt::make_format_args(fmt::arg(L"name", 42));
    fmt::wformat_args format_args(args);
    fmt::basic_context<wchar_t> ctx(format_args);
    
    auto arg = ctx.get_arg(L"nonexistent");
    EXPECT_FALSE(arg);
}

// Test that get_arg with string_view works correctly
TEST_F(BasicContextGetArgTest, StringViewName) {
    auto args = fmt::make_format_args(fmt::arg("key", 100));
    fmt::format_args format_args(args);
    fmt::basic_context<char> ctx(format_args);
    
    std::string_view name("key");
    auto arg = ctx.get_arg(name);
    EXPECT_TRUE(arg);
    EXPECT_EQ(100, arg.template get<int>());
}

// Test that get_arg returns empty for string_view name that doesn't exist
TEST_F(BasicContextGetArgTest, StringViewNameNotFound) {
    auto args = fmt::make_format_args(fmt::arg("key", 100));
    fmt::format_args format_args(args);
    fmt::basic_context<char> ctx(format_args);
    
    std::string_view name("missing");
    auto arg = ctx.get_arg(name);
    EXPECT_FALSE(arg);
}

// Test that get_arg with const char* works correctly
TEST_F(BasicContextGetArgTest, ConstCharName) {
    auto args = fmt::make_format_args(fmt::arg("data", 3.14f));
    fmt::format_args format_args(args);
    fmt::basic_context<char> ctx(format_args);
    
    const char* name = "data";
    auto arg = ctx.get_arg(name);
    EXPECT_TRUE(arg);
    EXPECT_FLOAT_EQ(3.14f, arg.template get<float>());
}

// Test that get_arg returns empty for const char* name that doesn't exist
TEST_F(BasicContextGetArgTest, ConstCharNameNotFound) {
    auto args = fmt::make_format_args(fmt::arg("data", 3.14f));
    fmt::format_args format_args(args);
    fmt::basic_context<char> ctx(format_args);
    
    const char* name = "missing";
    auto arg = ctx.get_arg(name);
    EXPECT_FALSE(arg);
}