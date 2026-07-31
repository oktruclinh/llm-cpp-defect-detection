#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/core.h>
#include <string>
#include <string_view>

// Test fixture for basic_context::get_arg with string names
class BasicContextGetArgTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a format string with named arguments for testing
        fmt::dynamic_format_arg_store<fmt::format_context> store;
        store.push_back(fmt::arg("name", "Alice"));
        store.push_back(fmt::arg("age", 30));
        store.push_back(fmt::arg("pi", 3.14));
        args_ = fmt::basic_format_args<fmt::format_context>(store);
    }

    fmt::basic_format_args<fmt::format_context> args_;
};

// Test that get_arg returns the correct argument for a valid name
TEST_F(BasicContextGetArgTest, ValidNameReturnsCorrectArg) {
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args_);
    auto arg = ctx.get_arg("name");
    ASSERT_TRUE(arg);
    EXPECT_EQ(fmt::format("{}", arg), "Alice");
}

// Test that get_arg returns the correct argument for another valid name
TEST_F(BasicContextGetArgTest, ValidNameReturnsCorrectArgAge) {
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args_);
    auto arg = ctx.get_arg("age");
    ASSERT_TRUE(arg);
    EXPECT_EQ(fmt::format("{}", arg), "30");
}

// Test that get_arg returns the correct argument for a third valid name
TEST_F(BasicContextGetArgTest, ValidNameReturnsCorrectArgPi) {
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args_);
    auto arg = ctx.get_arg("pi");
    ASSERT_TRUE(arg);
    EXPECT_EQ(fmt::format("{}", arg), "3.14");
}

// Test that get_arg returns a valid argument for a name that exists
TEST_F(BasicContextGetArgTest, ExistingNameReturnsValidArg) {
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args_);
    auto arg = ctx.get_arg("name");
    EXPECT_TRUE(arg);
}

// Test that get_arg throws an error for a non-existent name
TEST_F(BasicContextGetArgTest, NonExistentNameThrowsError) {
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args_);
    EXPECT_THROW(ctx.get_arg("nonexistent"), fmt::format_error);
}

// Test that get_arg throws an error for an empty string name
TEST_F(BasicContextGetArgTest, EmptyNameThrowsError) {
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args_);
    EXPECT_THROW(ctx.get_arg(""), fmt::format_error);
}

// Test that get_arg throws an error for a name with special characters
TEST_F(BasicContextGetArgTest, SpecialCharacterNameThrowsError) {
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args_);
    EXPECT_THROW(ctx.get_arg("!@#$%"), fmt::format_error);
}

// Test that get_arg works with wchar_t context
TEST_F(BasicContextGetArgTest, WideCharContextWorks) {
    fmt::dynamic_format_arg_store<fmt::wformat_context> wstore;
    wstore.push_back(fmt::arg(L"name", L"Alice"));
    wstore.push_back(fmt::arg(L"value", 42));
    auto wargs = fmt::basic_format_args<fmt::wformat_context>(wstore);
    fmt::wformat_context wctx(fmt::detail::buffer_context<wchar_t>::get_buffer(), wargs);
    auto arg = wctx.get_arg(L"name");
    ASSERT_TRUE(arg);
    EXPECT_EQ(fmt::format(L"{}", arg), L"Alice");
}

// Test that get_arg throws for non-existent name in wide context
TEST_F(BasicContextGetArgTest, WideCharNonExistentNameThrowsError) {
    fmt::dynamic_format_arg_store<fmt::wformat_context> wstore;
    wstore.push_back(fmt::arg(L"name", L"Alice"));
    auto wargs = fmt::basic_format_args<fmt::wformat_context>(wstore);
    fmt::wformat_context wctx(fmt::detail::buffer_context<wchar_t>::get_buffer(), wargs);
    EXPECT_THROW(wctx.get_arg(L"missing"), fmt::format_error);
}

// Test that get_arg works with multiple named arguments
TEST_F(BasicContextGetArgTest, MultipleNamedArgsWork) {
    fmt::dynamic_format_arg_store<fmt::format_context> store;
    store.push_back(fmt::arg("a", 1));
    store.push_back(fmt::arg("b", 2));
    store.push_back(fmt::arg("c", 3));
    auto args = fmt::basic_format_args<fmt::format_context>(store);
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args);
    
    auto arg_a = ctx.get_arg("a");
    auto arg_b = ctx.get_arg("b");
    auto arg_c = ctx.get_arg("c");
    
    ASSERT_TRUE(arg_a);
    ASSERT_TRUE(arg_b);
    ASSERT_TRUE(arg_c);
    EXPECT_EQ(fmt::format("{}", arg_a), "1");
    EXPECT_EQ(fmt::format("{}", arg_b), "2");
    EXPECT_EQ(fmt::format("{}", arg_c), "3");
}

// Test that get_arg returns different arguments for different names
TEST_F(BasicContextGetArgTest, DifferentNamesReturnDifferentArgs) {
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args_);
    auto arg_name = ctx.get_arg("name");
    auto arg_age = ctx.get_arg("age");
    ASSERT_TRUE(arg_name);
    ASSERT_TRUE(arg_age);
    EXPECT_NE(fmt::format("{}", arg_name), fmt::format("{}", arg_age));
}

// Test that get_arg works with integer arguments
TEST_F(BasicContextGetArgTest, IntegerArgumentWorks) {
    fmt::dynamic_format_arg_store<fmt::format_context> store;
    store.push_back(fmt::arg("num", 42));
    auto args = fmt::basic_format_args<fmt::format_context>(store);
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args);
    auto arg = ctx.get_arg("num");
    ASSERT_TRUE(arg);
    EXPECT_EQ(fmt::format("{}", arg), "42");
}

// Test that get_arg works with floating-point arguments
TEST_F(BasicContextGetArgTest, FloatArgumentWorks) {
    fmt::dynamic_format_arg_store<fmt::format_context> store;
    store.push_back(fmt::arg("val", 3.14159));
    auto args = fmt::basic_format_args<fmt::format_context>(store);
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args);
    auto arg = ctx.get_arg("val");
    ASSERT_TRUE(arg);
    EXPECT_EQ(fmt::format("{:.5f}", arg), "3.14159");
}

// Test that get_arg works with string arguments
TEST_F(BasicContextGetArgTest, StringArgumentWorks) {
    fmt::dynamic_format_arg_store<fmt::format_context> store;
    store.push_back(fmt::arg("str", std::string("hello")));
    auto args = fmt::basic_format_args<fmt::format_context>(store);
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args);
    auto arg = ctx.get_arg("str");
    ASSERT_TRUE(arg);
    EXPECT_EQ(fmt::format("{}", arg), "hello");
}

// Test that get_arg works with const char* arguments
TEST_F(BasicContextGetArgTest, ConstCharPtrArgumentWorks) {
    fmt::dynamic_format_arg_store<fmt::format_context> store;
    store.push_back(fmt::arg("cstr", "world"));
    auto args = fmt::basic_format_args<fmt::format_context>(store);
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args);
    auto arg = ctx.get_arg("cstr");
    ASSERT_TRUE(arg);
    EXPECT_EQ(fmt::format("{}", arg), "world");
}

// Test that get_arg works with boolean arguments
TEST_F(BasicContextGetArgTest, BoolArgumentWorks) {
    fmt::dynamic_format_arg_store<fmt::format_context> store;
    store.push_back(fmt::arg("flag", true));
    auto args = fmt::basic_format_args<fmt::format_context>(store);
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args);
    auto arg = ctx.get_arg("flag");
    ASSERT_TRUE(arg);
    EXPECT_EQ(fmt::format("{}", arg), "true");
}

// Test that get_arg works with char arguments
TEST_F(BasicContextGetArgTest, CharArgumentWorks) {
    fmt::dynamic_format_arg_store<fmt::format_context> store;
    store.push_back(fmt::arg("ch", 'A'));
    auto args = fmt::basic_format_args<fmt::format_context>(store);
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args);
    auto arg = ctx.get_arg("ch");
    ASSERT_TRUE(arg);
    EXPECT_EQ(fmt::format("{}", arg), "A");
}

// Test that get_arg throws when called on a context with no named arguments
TEST_F(BasicContextGetArgTest, NoNamedArgsThrowsError) {
    fmt::dynamic_format_arg_store<fmt::format_context> store;
    store.push_back(42);  // positional argument only
    auto args = fmt::basic_format_args<fmt::format_context>(store);
    fmt::format_context ctx(fmt::detail::buffer_context<char>::get_buffer(), args);
    EXPECT_THROW(ctx.get_arg("anything"), fmt::format_error);
}