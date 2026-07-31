#include <gtest/gtest.h>
#include "fmt/format.h"
#include "fmt/core.h"

// Test fixture for basic_context::get_arg with string names
class BasicContextGetArgTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Ensure format string is valid for the context
        fmt::memory_buffer buf;
        fmt::format_to(std::back_inserter(buf), "{} {}", 42, "hello");
    }
};

// Test that a named argument can be retrieved successfully
TEST_F(BasicContextGetArgTest, NamedArgumentFound) {
    auto args = fmt::make_format_args(fmt::arg("key", 123));
    fmt::format_args fa(args);
    fmt::basic_context<char> ctx(fa, fmt::string_view(""));
    auto arg = ctx.get_arg("key");
    EXPECT_TRUE(arg);
    EXPECT_EQ(123, arg.template get<int>());
}

// Test that get_arg returns a valid argument for multiple named args
TEST_F(BasicContextGetArgTest, MultipleNamedArgs) {
    auto args = fmt::make_format_args(
        fmt::arg("a", 1), fmt::arg("b", 2.5), fmt::arg("c", "test"));
    fmt::format_args fa(args);
    fmt::basic_context<char> ctx(fa, fmt::string_view(""));
    auto arg_a = ctx.get_arg("a");
    auto arg_b = ctx.get_arg("b");
    auto arg_c = ctx.get_arg("c");
    EXPECT_TRUE(arg_a);
    EXPECT_TRUE(arg_b);
    EXPECT_TRUE(arg_c);
    EXPECT_EQ(1, arg_a.template get<int>());
    EXPECT_DOUBLE_EQ(2.5, arg_b.template get<double>());
    EXPECT_STREQ("test", arg_c.template get<const char*>());
}

// Test that get_arg returns an invalid argument for a non-existent name
TEST_F(BasicContextGetArgTest, NamedArgumentNotFound) {
    auto args = fmt::make_format_args(fmt::arg("existing", 99));
    fmt::format_args fa(args);
    fmt::basic_context<char> ctx(fa, fmt::string_view(""));
    auto arg = ctx.get_arg("nonexistent");
    EXPECT_FALSE(arg);
}

// Test that get_arg returns an invalid argument when no named args exist
TEST_F(BasicContextGetArgTest, NoNamedArgs) {
    auto args = fmt::make_format_args(42, "hello");
    fmt::format_args fa(args);
    fmt::basic_context<char> ctx(fa, fmt::string_view(""));
    auto arg = ctx.get_arg("anything");
    EXPECT_FALSE(arg);
}

// Test that get_arg works with empty string name (edge case)
TEST_F(BasicContextGetArgTest, EmptyName) {
    auto args = fmt::make_format_args(fmt::arg("", 777));
    fmt::format_args fa(args);
    fmt::basic_context<char> ctx(fa, fmt::string_view(""));
    auto arg = ctx.get_arg("");
    EXPECT_TRUE(arg);
    EXPECT_EQ(777, arg.template get<int>());
}

// Test that get_arg returns invalid for empty name when not present
TEST_F(BasicContextGetArgTest, EmptyNameNotFound) {
    auto args = fmt::make_format_args(fmt::arg("nonempty", 1));
    fmt::format_args fa(args);
    fmt::basic_context<char> ctx(fa, fmt::string_view(""));
    auto arg = ctx.get_arg("");
    EXPECT_FALSE(arg);
}

// Test that get_arg works with long names
TEST_F(BasicContextGetArgTest, LongName) {
    std::string long_name(1000, 'x');
    auto args = fmt::make_format_args(fmt::arg(long_name.c_str(), 42));
    fmt::format_args fa(args);
    fmt::basic_context<char> ctx(fa, fmt::string_view(""));
    auto arg = ctx.get_arg(long_name.c_str());
    EXPECT_TRUE(arg);
    EXPECT_EQ(42, arg.template get<int>());
}

// Test that get_arg with wchar_t context works (if supported)
TEST_F(BasicContextGetArgTest, WideCharContext) {
    auto args = fmt::make_format_args(fmt::arg(L"wide", 3.14));
    fmt::wformat_args fa(args);
    fmt::basic_context<wchar_t> ctx(fa, fmt::wstring_view(L""));
    auto arg = ctx.get_arg(L"wide");
    EXPECT_TRUE(arg);
    EXPECT_DOUBLE_EQ(3.14, arg.template get<double>());
}

// Test that get_arg returns invalid for non-existent wide char name
TEST_F(BasicContextGetArgTest, WideCharNotFound) {
    auto args = fmt::make_format_args(fmt::arg(L"exists", 1));
    fmt::wformat_args fa(args);
    fmt::basic_context<wchar_t> ctx(fa, fmt::wstring_view(L""));
    auto arg = ctx.get_arg(L"missing");
    EXPECT_FALSE(arg);
}

// Test that get_arg works with mixed positional and named args (named only)
TEST_F(BasicContextGetArgTest, MixedArgsNamedOnly) {
    auto args = fmt::make_format_args(fmt::arg("n", 10), 20);
    fmt::format_args fa(args);
    fmt::basic_context<char> ctx(fa, fmt::string_view(""));
    auto arg = ctx.get_arg("n");
    EXPECT_TRUE(arg);
    EXPECT_EQ(10, arg.template get<int>());
}

// Test that get_arg returns invalid for a name that is a positional index string
TEST_F(BasicContextGetArgTest, NameIsNumberString) {
    auto args = fmt::make_format_args(fmt::arg("0", 100));
    fmt::format_args fa(args);
    fmt::basic_context<char> ctx(fa, fmt::string_view(""));
    auto arg = ctx.get_arg("0");
    EXPECT_TRUE(arg);
    EXPECT_EQ(100, arg.template get<int>());
}

// Test that get_arg with duplicate names returns the first occurrence
TEST_F(BasicContextGetArgTest, DuplicateNames) {
    auto args = fmt::make_format_args(
        fmt::arg("dup", 1), fmt::arg("dup", 2));
    fmt::format_args fa(args);
    fmt::basic_context<char> ctx(fa, fmt::string_view(""));
    auto arg = ctx.get_arg("dup");
    EXPECT_TRUE(arg);
    EXPECT_EQ(1, arg.template get<int>());
}

// Test that get_arg works with string_view argument
TEST_F(BasicContextGetArgTest, StringViewArg) {
    fmt::string_view sv("sv_arg");
    auto args = fmt::make_format_args(fmt::arg(sv.data(), 42));
    fmt::format_args fa(args);
    fmt::basic_context<char> ctx(fa, fmt::string_view(""));
    auto arg = ctx.get_arg(sv);
    EXPECT_TRUE(arg);
    EXPECT_EQ(42, arg.template get<int>());
}

// Test that get_arg returns invalid for a name that is a substring of an existing name
TEST_F(BasicContextGetArgTest, SubstringName) {
    auto args = fmt::make_format_args(fmt::arg("longname", 1));
    fmt::format_args fa(args);
    fmt::basic_context<char> ctx(fa, fmt::string_view(""));
    auto arg = ctx.get_arg("long");
    EXPECT_FALSE(arg);
}

// Test that get_arg with special characters in name works
TEST_F(BasicContextGetArgTest, SpecialCharactersInName) {
    auto args = fmt::make_format_args(fmt::arg("a_b-c.d", 99));
    fmt::format_args fa(args);
    fmt::basic_context<char> ctx(fa, fmt::string_view(""));
    auto arg = ctx.get_arg("a_b-c.d");
    EXPECT_TRUE(arg);
    EXPECT_EQ(99, arg.template get<int>());
}