#include <gtest/gtest.h>
#include "fmt/format.h"
#include "fmt/compile.h" // For compiled format strings, if needed

// Helper to capture errors from fmt::format
struct ErrorHandler {
    std::string message;
    void on_error(const char* msg) {
        message = msg;
    }
};

TEST(FmtGetArgTest, BasicNamedArgument) {
    std::string result = fmt::format("{arg}", fmt::arg("arg", 42));
    ASSERT_EQ(result, "42");
}

TEST(FmtGetArgTest, MultipleNamedArguments) {
    std::string result = fmt::format("{first} {second}", fmt::arg("first", "hello"), fmt::arg("second", 123));
    ASSERT_EQ(result, "hello 123");
}

TEST(FmtGetArgTest, NamedArgumentWithDifferentTypes) {
    std::string result = fmt::format("{s} {i} {f}", fmt::arg("s", "test"), fmt::arg("i", 10), fmt::arg("f", 3.14f));
    ASSERT_EQ(result, "test 10 3.14");
}

TEST(FmtGetArgTest, NamedArgumentNotFound) {
    // We need to use a custom error handler to catch the "argument not found" error
    // as fmt::format by default throws an exception.
    // This test specifically targets the `on_error` call within `get_arg`.

    // Create a custom context that uses our error handler
    struct CustomContext : fmt::basic_format_context<char, ErrorHandler> {
        CustomContext(fmt::basic_format_args<fmt::basic_format_context<char, ErrorHandler>> args, ErrorHandler& eh)
            : fmt::basic_format_context<char, ErrorHandler>(fmt::detail::buffer_range<char>(nullptr, nullptr), args, eh) {}
    };

    ErrorHandler eh;
    fmt::format_args args = fmt::make_format_args(fmt::arg("present", 1));
    CustomContext ctx(args, eh);

    // Simulate calling get_arg directly. This is tricky as get_arg is protected
    // and usually called internally by the format string parser.
    // The most direct way to test the error path of get_arg via public API
    // is to provide a format string that references a non-existent argument.
    // This will cause fmt::format to throw, which we can catch.

    try {
        fmt::format("{missing}", fmt::arg("present", 1));
        FAIL() << "Expected fmt::format to throw an exception for missing argument.";
    } catch (const fmt::format_error& e) {
        ASSERT_STREQ(e.what(), "argument not found");
    }
}

TEST(FmtGetArgTest, NamedArgumentNotFoundWithEmptyArgs) {
    try {
        fmt::format("{missing}");
        FAIL() << "Expected fmt::format to throw an exception for missing argument.";
    } catch (const fmt::format_error& e) {
        ASSERT_STREQ(e.what(), "argument not found");
    }
}

TEST(FmtGetArgTest, NamedArgumentWithPositionalFallback) {
    // This test ensures that named arguments are prioritized over positional ones
    // if a name is provided.
    std::string result = fmt::format("{0} {name}", 10, fmt::arg("name", "test"));
    ASSERT_EQ(result, "10 test");
}

TEST(FmtGetArgTest, NamedArgumentUsedMultipleTimes) {
    std::string result = fmt::format("{arg} {arg} {arg}", fmt::arg("arg", "value"));
    ASSERT_EQ(result, "value value value");
}

TEST(FmtGetArgTest, NamedArgumentWithUnicodeName) {
    // fmt::string_view supports non-ASCII characters, so argument names should too.
    // However, typical usage sticks to ASCII for argument names.
    // This tests the underlying string_view comparison.
    std::string result = fmt::format("{你好}", fmt::arg("你好", "world"));
    ASSERT_EQ(result, "world");
}

TEST(FmtGetArgTest, NamedArgumentWithEmptyName) {
    // An empty string as an argument name is technically possible but unusual.
    // It should behave like any other name.
    std::string result = fmt::format("{}", fmt::arg("", "empty_name_arg"));
    ASSERT_EQ(result, "empty_name_arg"); // The empty name refers to the first positional arg, which is the named arg.
                                         // This behavior is a bit ambiguous in fmt, but it works.
                                         // More robustly, we'd expect an error if it's not found.
                                         // Let's re-evaluate this test.
                                         // The format string "{}" without a name refers to the 0-th positional argument.
                                         // If we have a named argument with an empty string name, it will be treated as a positional argument.
                                         // The `get_arg` function is called with `basic_string_view<Char> name`.
                                         // If `name` is empty, it's treated as a positional argument.
                                         // Let's try to explicitly use an empty name in the format string.
    try {
        fmt::format("{name}", fmt::arg("", "value")); // This will look for "name", not ""
        FAIL() << "Expected fmt::format to throw for missing 'name' argument.";
    } catch (const fmt::format_error& e) {
        ASSERT_STREQ(e.what(), "argument not found");
    }

    // To test an empty name, we'd need to explicitly refer to it, which is not possible
    // with the current format string syntax (e.g., "{}").
    // The `get_arg` function itself is called with a `name` string_view.
    // The most direct way to test `get_arg` with an empty name is if the parser
    // somehow generates an empty name for it. This is not a typical use case
    // for named arguments.
    // The current `fmt::format` syntax doesn't allow referencing an empty named argument directly.
    // The test above for `"{}"` with `fmt::arg("", "value")` actually tests positional argument 0.
    // The `get_arg` function is called with the actual name from the format string.
    // If the format string has `{}`, it's a positional argument.
    // If it has `{name}`, it's a named argument.
    // An empty string as a *name* in `fmt::arg` is treated as a positional argument.
    // So, `fmt::arg("", "value")` makes "value" the 0-th positional argument.
    std::string result_empty_name_as_positional = fmt::format("{}", fmt::arg("", "value"));
    ASSERT_EQ(result_empty_name_as_positional, "value");
}

TEST(FmtGetArgTest, NamedArgumentWithLongName) {
    std::string long_name(1000, 'a');
    std::string format_string = fmt::format("{{{}}}", long_name); // e.g., "{aaaa...a}"
    std::string result = fmt::format(format_string, fmt::arg(long_name.c_str(), "long_name_value"));
    ASSERT_EQ(result, "long_name_value");
}

TEST(FmtGetArgTest, NamedArgumentCaseSensitivity) {
    try {
        fmt::format("{Arg}", fmt::arg("arg", 1));
        FAIL() << "Expected fmt::format to throw for case-sensitive mismatch.";
    } catch (const fmt::format_error& e) {
        ASSERT_STREQ(e.what(), "argument not found");
    }
    std::string result = fmt::format("{arg}", fmt::arg("arg", 1));
    ASSERT_EQ(result, "1");
}

// Test with wide characters
TEST(FmtGetArgTest, BasicNamedArgumentWideChar) {
    std::wstring result = fmt::format(L"{arg}", fmt::arg("arg", 42));
    ASSERT_EQ(result, L"42");
}

TEST(FmtGetArgTest, NamedArgumentNotFoundWideChar) {
    try {
        fmt::format(L"{missing}", fmt::arg("present", 1));
        FAIL() << "Expected fmt::format to throw an exception for missing argument.";
    } catch (const fmt::format_error& e) {
        ASSERT_STREQ(e.what(), "argument not found");
    }
}

// Test with compiled format strings (if applicable, though get_arg is runtime)
TEST(FmtGetArgTest, CompiledFormatStringNamedArgument) {
    std::string result = fmt::format(FMT_COMPILE("{arg}"), fmt::arg("arg", 123));
    ASSERT_EQ(result, "123");
}

TEST(FmtGetArgTest, CompiledFormatStringNamedArgumentNotFound) {
    try {
        fmt::format(FMT_COMPILE("{missing}"), fmt::arg("present", 1));
        FAIL() << "Expected fmt::format to throw an exception for missing argument.";
    } catch (const fmt::format_error& e) {
        ASSERT_STREQ(e.what(), "argument not found");
    }
}