#include <gtest/gtest.h>
#include "fmt/core.h" // Public API for fmtlib

// Helper struct to expose internal details for testing purposes if needed,
// but for get_id, we can mostly rely on public formatting functions.
// However, since get_id is an internal helper, we need to simulate its usage
// by creating a format string with named arguments.

// Test fixture for fmt::format_arg_store and related functionalities
class FormatArgStoreTest : public ::testing::Test {
protected:
    // We can't directly construct a format_arg_store with named arguments
    // from the public API. The get_id function is called internally when
    // formatting with named arguments.
    // We will use fmt::format to trigger the internal logic that uses get_id.
};

// Test case for a normal scenario with multiple named arguments
TEST_F(FormatArgStoreTest, GetIdNormalCase) {
    // This format string will cause fmt::format to internally create
    // a format_arg_store and then resolve named arguments.
    // We are testing the *effect* of get_id, not calling it directly.
    // If get_id is buggy, the formatting will be incorrect.
    std::string result = fmt::format("Hello, {name}! Your age is {age}.", fmt::arg("name", "Alice"), fmt::arg("age", 30));
    EXPECT_EQ(result, "Hello, Alice! Your age is 30.");

    // Another order to ensure name resolution is independent of declaration order
    result = fmt::format("Your age is {age}, {name}!", fmt::arg("age", 30), fmt::arg("name", "Alice"));
    EXPECT_EQ(result, "Your age is 30, Alice!");
}

// Test case for a single named argument
TEST_F(FormatArgStoreTest, GetIdSingleNamedArg) {
    std::string result = fmt::format("The value is {val}.", fmt::arg("val", 123));
    EXPECT_EQ(result, "The value is 123.");
}

// Test case for a named argument that is not present in the format string
// This should still format correctly, ignoring the unused named argument.
TEST_F(FormatArgStoreTest, GetIdUnusedNamedArg) {
    std::string result = fmt::format("Hello, {name}!", fmt::arg("name", "Bob"), fmt::arg("unused", "data"));
    EXPECT_EQ(result, "Hello, Bob!");
}

// Test case for a format string with no named arguments, but named args are provided.
// The get_id function should not be called for named arguments if the format string
// only uses positional arguments or no arguments.
TEST_F(FormatArgStoreTest, GetIdNoNamedArgsInFormatString) {
    std::string result = fmt::format("Hello, world!", fmt::arg("name", "Charlie"));
    EXPECT_EQ(result, "Hello, world!");

    result = fmt::format("Value: {}", 42, fmt::arg("name", "Charlie"));
    EXPECT_EQ(result, "Value: 42");
}

// Test case for an empty named argument string (name itself is empty)
// This is an edge case for the `name` parameter of get_id.
// fmtlib does not allow empty named argument names in the format string.
// If we try to pass an empty string as a named argument, it's generally
// not a valid use case for fmt::arg.
// However, we can test what happens if such an argument is somehow passed.
// The current implementation of fmt::arg("","value") is not allowed.
// The `get_id` function itself would compare `basic_string_view<Char> name`.
// Let's simulate a scenario where a named argument with an empty name might exist
// in the internal `named_args` structure (though unlikely via public API).
// Since we can't directly call `get_id`, we rely on the public API's behavior.
// fmt::arg requires a non-empty string literal for the name.
// So, this specific edge case for `name` being empty is hard to trigger via public API.
// We'll focus on valid named argument names.

// Test case with duplicate named arguments (same name, different values)
// The first one should be used.
TEST_F(FormatArgStoreTest, GetIdDuplicateNamedArgs) {
    std::string result = fmt::format("Value: {name}", fmt::arg("name", 1), fmt::arg("name", 2));
    EXPECT_EQ(result, "Value: 1"); // The first occurrence should be used.
}

// Test case with a named argument that is not found in the provided arguments.
// This should result in a formatting error, which is caught by fmtlib.
TEST_F(FormatArgStoreTest, GetIdNamedArgNotFound) {
    // Expect a format_error because 'missing_name' is not provided.
    EXPECT_THROW(fmt::format("Hello, {missing_name}!"), fmt::format_error);
}

// Test case with a mix of named and positional arguments
TEST_F(FormatArgStoreTest, GetIdMixedArgs) {
    std::string result = fmt::format("Hello, {name}! You are {} years old.", fmt::arg("name", "David"), 40);
    EXPECT_EQ(result, "Hello, David! You are 40 years old.");

    result = fmt::format("You are {} years old, {name}!", 40, fmt::arg("name", "David"));
    EXPECT_EQ(result, "You are 40 years old, David!");
}

// Test case with a long named argument name
TEST_F(FormatArgStoreTest, GetIdLongNamedArg) {
    std::string long_name = "this_is_a_very_long_named_argument_to_test_string_view_comparison";
    std::string format_str = fmt::format("Value: {{{}}}", long_name); // Construct format string dynamically
    std::string result = fmt::format(format_str, fmt::arg(long_name.c_str(), 999));
    EXPECT_EQ(result, "Value: 999");
}

// Test case with special characters in named argument name (if allowed by fmtlib)
// fmtlib generally restricts named argument names to identifiers.
// Let's assume valid identifier characters.
TEST_F(FormatArgStoreTest, GetIdNamedArgWithUnderscore) {
    std::string result = fmt::format("User ID: {user_id}", fmt::arg("user_id", 101));
    EXPECT_EQ(result, "User ID: 101");
}

// Test case with no arguments at all (neither named nor positional)
TEST_F(FormatArgStoreTest, GetIdNoArgs) {
    std::string result = fmt::format("Just a string.");
    EXPECT_EQ(result, "Just a string.");
}

// Test case with a large number of named arguments
TEST_F(FormatArgStoreTest, GetIdManyNamedArgs) {
    // This test is more about ensuring performance/correctness with many args,
    // rather than a specific edge case for `get_id` itself, but it exercises
    // the loop within `get_id` many times.
    std::string result = fmt::format(
        "{a}{b}{c}{d}{e}{f}{g}{h}{i}{j}{k}{l}{m}{n}{o}{p}{q}{r}{s}{t}",
        fmt::arg("a", 1), fmt::arg("b", 2), fmt::arg("c", 3), fmt::arg("d", 4),
        fmt::arg("e", 5), fmt::arg("f", 6), fmt::arg("g", 7), fmt::arg("h", 8),
        fmt::arg("i", 9), fmt::arg("j", 10), fmt::arg("k", 11), fmt::arg("l", 12),
        fmt::arg("m", 13), fmt::arg("n", 14), fmt::arg("o", 15), fmt::arg("p", 16),
        fmt::arg("q", 17), fmt::arg("r", 18), fmt::arg("s", 19), fmt::arg("t", 20)
    );
    EXPECT_EQ(result, "1234567891011121314151617181920");
}

// Test case where a named argument is provided but not used in the format string,
// and other positional arguments are used.
TEST_F(FormatArgStoreTest, GetIdUnusedNamedArgWithPositional) {
    std::string result = fmt::format("Value: {}", 100, fmt::arg("unused", "test"));
    EXPECT_EQ(result, "Value: 100");
}

// Test case with different character types (wchar_t)
TEST_F(FormatArgStoreTest, GetIdWideChar) {
    std::wstring result = fmt::format(L"Hello, {name}!", fmt::arg(L"name", L"Eve"));
    EXPECT_EQ(result, L"Hello, Eve!");

    result = fmt::format(L"Value: {val}.", fmt::arg(L"val", 456));
    EXPECT_EQ(result, L"Value: 456.");
}