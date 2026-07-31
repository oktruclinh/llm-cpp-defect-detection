#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/core.h> // For fmt::format_error

// Helper function to get a format_arg from a format string and arguments
// This simulates the internal context behavior for named arguments.
template <typename Char>
fmt::basic_format_arg<fmt::basic_format_context<Char>> get_named_arg_from_format_string(
    fmt::basic_string_view<Char> format_str, fmt::basic_string_view<Char> arg_name,
    fmt::format_args args) {
    // We need to create a context to simulate the behavior of get_arg
    // This is a bit of a hack as get_arg is usually called internally during formatting.
    // We'll use a dummy buffer and a custom error handler.
    struct TestContext : fmt::basic_format_context<Char> {
        fmt::basic_string_view<Char> format_string;
        fmt::basic_format_args<TestContext> args_;
        fmt::basic_format_parse_context<Char> parse_ctx;
        fmt::basic_format_arg_map<TestContext> map_;
        std::string error_message;

        TestContext(fmt::basic_string_view<Char> fs, fmt::format_args a)
            : fmt::basic_format_context<Char>(
                  fmt::detail::buffer_range<Char>(nullptr, nullptr), a),
              format_string(fs),
              args_(a),
              parse_ctx(fs) {}

        void on_error(const char* message) override {
            error_message = message;
        }

        fmt::basic_format_arg<TestContext> get_arg(fmt::basic_string_view<Char> name) {
            map_.init(this->args()); // Initialize map_ with the arguments
            if (const auto* arg = map_.find(name)) {
                return *arg;
            }
            this->on_error("argument not found");
            return fmt::basic_format_arg<TestContext>();
        }

        fmt::basic_format_parse_context<Char>& parse_context() { return parse_ctx; }
        fmt::basic_format_args<TestContext> args() const { return args_; }
    };

    TestContext ctx(format_str, args);
    return ctx.get_arg(arg_name);
}

TEST(BasicContextGetArgTest, NamedArgumentFound) {
    auto args = fmt::make_format_args(fmt::arg("name", "FmtLib"), fmt::arg("age", 10));
    auto arg = get_named_arg_from_format_string<char>("", "name", args);

    ASSERT_TRUE(arg.is_string());
    EXPECT_EQ(arg.as_string(), "FmtLib");

    arg = get_named_arg_from_format_string<char>("", "age", args);
    ASSERT_TRUE(arg.is_int());
    EXPECT_EQ(arg.as_int(), 10);
}

TEST(BasicContextGetArgTest, NamedArgumentNotFound) {
    auto args = fmt::make_format_args(fmt::arg("name", "FmtLib"));
    // The actual error handling for "argument not found" is typically
    // caught by fmt::format, which throws fmt::format_error.
    // We simulate this by checking if the returned arg is invalid.
    auto arg = get_named_arg_from_format_string<char>("", "nonexistent", args);

    // An argument not found should return an invalid/empty format_arg
    EXPECT_FALSE(arg.is_int());
    EXPECT_FALSE(arg.is_string());
    EXPECT_FALSE(arg.is_double());
    EXPECT_FALSE(arg.is_bool());
    EXPECT_FALSE(arg.is_pointer());
    EXPECT_FALSE(arg.is_custom_type());
}

TEST(BasicContextGetArgTest, EmptyNamedArgumentList) {
    auto args = fmt::make_format_args(); // No named arguments
    auto arg = get_named_arg_from_format_string<char>("", "name", args);

    EXPECT_FALSE(arg.is_int());
    EXPECT_FALSE(arg.is_string());
}

TEST(BasicContextGetArgTest, MultipleNamedArguments) {
    auto args = fmt::make_format_args(
        fmt::arg("first", 1),
        fmt::arg("second", 2.0),
        fmt::arg("third", "three")
    );

    auto arg1 = get_named_arg_from_format_string<char>("", "first", args);
    ASSERT_TRUE(arg1.is_int());
    EXPECT_EQ(arg1.as_int(), 1);

    auto arg2 = get_named_arg_from_format_string<char>("", "second", args);
    ASSERT_TRUE(arg2.is_double());
    EXPECT_EQ(arg2.as_double(), 2.0);

    auto arg3 = get_named_arg_from_format_string<char>("", "third", args);
    ASSERT_TRUE(arg3.is_string());
    EXPECT_EQ(arg3.as_string(), "three");
}

TEST(BasicContextGetArgTest, NamedArgumentShadowingPositional) {
    // This test ensures that named arguments are correctly looked up
    // even if there are positional arguments present.
    // The `get_arg` function specifically looks for named arguments.
    auto args = fmt::make_format_args(10, "hello", fmt::arg("name", "FmtLib"));
    auto arg = get_named_arg_from_format_string<char>("", "name", args);

    ASSERT_TRUE(arg.is_string());
    EXPECT_EQ(arg.as_string(), "FmtLib");

    // Trying to get a positional arg by name should fail
    arg = get_named_arg_from_format_string<char>("", "0", args); // "0" is not a named arg
    EXPECT_FALSE(arg.is_int());
}

TEST(BasicContextGetArgTest, WideCharNamedArgument) {
    auto args = fmt::make_format_args(fmt::arg("name", L"FmtLib"));
    auto arg = get_named_arg_from_format_string<wchar_t>(L"", L"name", args);

    ASSERT_TRUE(arg.is_string());
    EXPECT_EQ(arg.as_string(), L"FmtLib");
}

TEST(BasicContextGetArgTest, WideCharNamedArgumentNotFound) {
    auto args = fmt::make_format_args(fmt::arg("name", L"FmtLib"));
    auto arg = get_named_arg_from_format_string<wchar_t>(L"", L"nonexistent", args);

    EXPECT_FALSE(arg.is_string());
}

// Test for a scenario where fmt::format would throw an error
TEST(BasicContextGetArgTest, FormatThrowsOnError) {
    // When a named argument is not found during an actual format call,
    // fmt::format is expected to throw fmt::format_error.
    EXPECT_THROW(
        fmt::format("Hello {nonexistent}!", fmt::arg("name", "World")),
        fmt::format_error
    );

    // Ensure it works correctly when found
    EXPECT_EQ(fmt::format("Hello {name}!", fmt::arg("name", "World")), "Hello World!");
}

// Test with an empty name for the argument lookup
TEST(BasicContextGetArgTest, EmptyArgumentNameLookup) {
    auto args = fmt::make_format_args(fmt::arg("name", "Value"));
    auto arg = get_named_arg_from_format_string<char>("", "", args); // Look up empty name

    // An empty name should not match any valid argument
    EXPECT_FALSE(arg.is_string());
}

// Test with a named argument that has an empty string as its name
TEST(BasicContextGetArgTest, ArgumentWithEmptyName) {
    // This is an edge case, typically named arguments have non-empty names.
    // However, the API allows it.
    auto args = fmt::make_format_args(fmt::arg("", "EmptyNameValue"));
    auto arg = get_named_arg_from_format_string<char>("", "", args);

    ASSERT_TRUE(arg.is_string());
    EXPECT_EQ(arg.as_string(), "EmptyNameValue");

    // Looking for a non-empty name should fail
    arg = get_named_arg_from_format_string<char>("", "name", args);
    EXPECT_FALSE(arg.is_string());
}