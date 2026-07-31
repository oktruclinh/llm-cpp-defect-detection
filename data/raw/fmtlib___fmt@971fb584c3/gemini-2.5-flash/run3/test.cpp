#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/core.h> // For fmt::format_error

// Helper function to get a format_arg from a format string and arguments
template <typename Char>
fmt::basic_format_arg<fmt::basic_format_context<Char>> get_named_arg_from_format_string(
    fmt::basic_string_view<Char> format_str, fmt::basic_string_view<Char> arg_name,
    fmt::format_args args) {
    // We need a context to call get_arg. The easiest way is to create a dummy formatter
    // and extract the context from it.
    fmt::basic_memory_buffer<Char> buffer;
    fmt::basic_format_context<Char> ctx(std::back_inserter(buffer), args);

    // This is a bit of a hack to get access to the protected get_arg method.
    // In a real scenario, you'd likely be testing the public API that uses get_arg.
    // However, since the request is to test the focal function directly,
    // we need to simulate its usage within a context.
    // We can't directly call basic_context<Char>::get_arg without a context object.
    // The focal function is a member of basic_context.
    // We'll create a derived class to expose it for testing.
    class TestContext : public fmt::basic_format_context<Char> {
    public:
        TestContext(std::back_insert_iterator<fmt::basic_memory_buffer<Char>> out, fmt::format_args args)
            : fmt::basic_format_context<Char>(out, args) {}

        typename fmt::basic_context<Char>::format_arg
        public_get_arg(fmt::basic_string_view<Char> name) {
            return this->get_arg(name);
        }
    };

    TestContext test_ctx(std::back_inserter(buffer), args);
    return test_ctx.public_get_arg(arg_name);
}

TEST(BasicContextGetArgTest, GetExistingNamedArg) {
    fmt::format_args args = fmt::make_format_args(fmt::arg("name", "Fido"), fmt::arg("age", 3));

    // Test getting a string argument
    fmt::format_arg arg_name = get_named_arg_from_format_string<char>("", "name", args);
    ASSERT_TRUE(arg_name.is_string());
    EXPECT_EQ(arg_name.as_string_view(), "Fido");

    // Test getting an integer argument
    fmt::format_arg arg_age = get_named_arg_from_format_string<char>("", "age", args);
    ASSERT_TRUE(arg_age.is_int());
    EXPECT_EQ(arg_age.as_int(), 3);
}

TEST(BasicContextGetArgTest, GetNonExistingNamedArg) {
    fmt::format_args args = fmt::make_format_args(fmt::arg("name", "Fido"));

    // Expect an error when a non-existent argument is requested
    // The focal function calls on_error, which by default throws fmt::format_error.
    EXPECT_THROW(
        get_named_arg_from_format_string<char>("", "non_existent", args),
        fmt::format_error
    );
}

TEST(BasicContextGetArgTest, GetArgWithEmptyName) {
    fmt::format_args args = fmt::make_format_args(fmt::arg("name", "Fido"));

    // An empty name should not match any argument and should result in an error.
    EXPECT_THROW(
        get_named_arg_from_format_string<char>("", "", args),
        fmt::format_error
    );
}

TEST(BasicContextGetArgTest, GetArgWithSimilarNames) {
    fmt::format_args args = fmt::make_format_args(fmt::arg("name", "Fido"), fmt::arg("name1", "Buddy"));

    fmt::format_arg arg_name = get_named_arg_from_format_string<char>("", "name", args);
    ASSERT_TRUE(arg_name.is_string());
    EXPECT_EQ(arg_name.as_string_view(), "Fido");

    fmt::format_arg arg_name1 = get_named_arg_from_format_string<char>("", "name1", args);
    ASSERT_TRUE(arg_name1.is_string());
    EXPECT_EQ(arg_name1.as_string_view(), "Buddy");

    // Ensure it doesn't mistakenly match "name" for "name1" or vice-versa
    EXPECT_THROW(
        get_named_arg_from_format_string<char>("", "nam", args),
        fmt::format_error
    );
}

TEST(BasicContextGetArgTest, GetArgWithWideCharacters) {
    fmt::wformat_args args = fmt::make_format_args(fmt::arg(L"name", L"Fido"), fmt::arg(L"age", 3));

    // Test getting a wide string argument
    fmt::wformat_arg arg_name = get_named_arg_from_format_string<wchar_t>(L"", L"name", args);
    ASSERT_TRUE(arg_name.is_string());
    EXPECT_EQ(arg_name.as_string_view(), L"Fido");

    // Test getting an integer argument (same for wide/char)
    fmt::wformat_arg arg_age = get_named_arg_from_format_string<wchar_t>(L"", L"age", args);
    ASSERT_TRUE(arg_age.is_int());
    EXPECT_EQ(arg_age.as_int(), 3);

    // Test non-existent wide argument
    EXPECT_THROW(
        get_named_arg_from_format_string<wchar_t>(L"", L"non_existent", args),
        fmt::format_error
    );
}

TEST(BasicContextGetArgTest, GetArgWithMixedArgs) {
    // This test ensures that even if there are positional arguments,
    // named arguments are correctly retrieved.
    fmt::format_args args = fmt::make_format_args(10, "hello", fmt::arg("named_val", 42));

    fmt::format_arg named_arg = get_named_arg_from_format_string<char>("", "named_val", args);
    ASSERT_TRUE(named_arg.is_int());
    EXPECT_EQ(named_arg.as_int(), 42);

    EXPECT_THROW(
        get_named_arg_from_format_string<char>("", "non_existent", args),
        fmt::format_error
    );
}

TEST(BasicContextGetArgTest, GetArgFromEmptyArgs) {
    fmt::format_args args = fmt::make_format_args(); // No arguments

    EXPECT_THROW(
        get_named_arg_from_format_string<char>("", "any_name", args),
        fmt::format_error
    );
}