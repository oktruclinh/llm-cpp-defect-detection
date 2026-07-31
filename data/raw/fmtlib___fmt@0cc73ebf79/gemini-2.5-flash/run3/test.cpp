#include <gtest/gtest.h>
#include "fmt/core.h" // Public API header

// Helper struct to expose internal details for testing purposes
// This is generally not recommended for production code but is useful for white-box testing
namespace fmt {
namespace detail {

// Forward declaration of internal types if necessary, or include internal headers
// For this specific function, we need to understand how named_args are stored.
// The focal function is part of `format_arg_store` or a similar internal type.
// We'll simulate its behavior through the public API.

} // namespace detail
} // namespace fmt

// Test fixture for fmt::format_arg_store related tests
class FormatArgStoreTest : public ::testing::Test {
protected:
    // No common setup needed for these tests, as we'll create format_arg_store
    // instances directly within each test case using fmt::make_format_args.
};

// Test case for a normal scenario with named arguments
TEST_F(FormatArgStoreTest, GetIdWithNamedArgs) {
    auto args = fmt::make_format_args(fmt::arg("name1", 10), fmt::arg("name2", "hello"));

    // We need to access the internal get_id method.
    // The public API for fmt::format_args does not directly expose get_id.
    // The focal function is part of an internal `format_arg_store` or similar.
    // To test it, we must simulate how fmt uses it, or cast to an internal type.
    // Given the prompt, we'll assume `args` itself has a `get_id` method,
    // or we're testing a component that holds these args and calls `get_id`.
    // For `fmt::make_format_args`, the `get_id` is typically called by the formatter.

    // Let's create a dummy formatter context to call get_id,
    // as `fmt::format_args` itself doesn't have `get_id`.
    // The `get_id` function is a member of `basic_format_args<Context>`.
    // We need to instantiate a `basic_format_args` object.
    fmt::format_args format_args = fmt::make_format_args(fmt::arg("name1", 10), fmt::arg("name2", "hello"));

    // The focal function is `basic_format_args::get_id`.
    // We can call it directly on the `format_args` object.
    EXPECT_EQ(format_args.get_id("name1"), 0); // First named arg typically gets id 0
    EXPECT_EQ(format_args.get_id("name2"), 1); // Second named arg typically gets id 1
}

// Test case for a scenario where the named argument does not exist
TEST_F(FormatArgStoreTest, GetIdNonExistentNamedArg) {
    auto args = fmt::make_format_args(fmt::arg("name1", 10), fmt::arg("name2", "hello"));
    fmt::format_args format_args = args;

    EXPECT_EQ(format_args.get_id("non_existent_name"), -1);
}

// Test case for an empty string as a named argument name
TEST_F(FormatArgStoreTest, GetIdEmptyName) {
    auto args = fmt::make_format_args(fmt::arg("", 10)); // An empty string as a name is valid
    fmt::format_args format_args = args;

    EXPECT_EQ(format_args.get_id(""), 0);
    EXPECT_EQ(format_args.get_id("other"), -1);
}

// Test case with no named arguments at all
TEST_F(FormatArgStoreTest, GetIdNoNamedArgs) {
    auto args = fmt::make_format_args(1, "test", 3.14); // Only positional arguments
    fmt::format_args format_args = args;

    // When there are no named arguments, has_named_args() should return false,
    // and get_id should immediately return -1.
    EXPECT_EQ(format_args.get_id("any_name"), -1);
}

// Test case with multiple named arguments, including duplicates (though fmt usually handles this by taking the first)
TEST_F(FormatArgStoreTest, GetIdWithDuplicateNamedArgs) {
    // fmt::make_format_args will typically store them in order.
    // The focal function iterates and returns the first match.
    auto args = fmt::make_format_args(fmt::arg("name", 1), fmt::arg("other", 2), fmt::arg("name", 3));
    fmt::format_args format_args = args;

    // Expect the ID of the first occurrence of "name"
    EXPECT_EQ(format_args.get_id("name"), 0);
    EXPECT_EQ(format_args.get_id("other"), 1);
}

// Test case with a large number of named arguments to check performance/boundary
TEST_F(FormatArgStoreTest, GetIdManyNamedArgs) {
    // Create a vector of named arguments
    std::vector<fmt::arg_data> arg_data_vec;
    for (int i = 0; i < 100; ++i) {
        arg_data_vec.push_back(fmt::arg(fmt::format("name{}", i), i));
    }

    // fmt::make_format_args doesn't directly take a vector of arg_data.
    // We need to use a helper to construct fmt::format_args with many named args.
    // This is a bit tricky as fmt::make_format_args is variadic.
    // For simplicity, let's test with a smaller, but still significant, number.
    auto args = fmt::make_format_args(
        fmt::arg("n0", 0), fmt::arg("n1", 1), fmt::arg("n2", 2), fmt::arg("n3", 3),
        fmt::arg("n4", 4), fmt::arg("n5", 5), fmt::arg("n6", 6), fmt::arg("n7", 7),
        fmt::arg("n8", 8), fmt::arg("n9", 9), fmt::arg("n10", 10), fmt::arg("n11", 11),
        fmt::arg("n12", 12), fmt::arg("n13", 13), fmt::arg("n14", 14), fmt::arg("n15", 15)
    );
    fmt::format_args format_args = args;

    EXPECT_EQ(format_args.get_id("n0"), 0);
    EXPECT_EQ(format_args.get_id("n7"), 7);
    EXPECT_EQ(format_args.get_id("n15"), 15);
    EXPECT_EQ(format_args.get_id("non_existent"), -1);
}

// Test case with mixed positional and named arguments
TEST_F(FormatArgStoreTest, GetIdMixedArgs) {
    // Positional arguments are stored separately from named arguments.
    // The `get_id` function only searches among named arguments.
    auto args = fmt::make_format_args(10, fmt::arg("first", "hello"), 20, fmt::arg("second", 3.14));
    fmt::format_args format_args = args;

    EXPECT_EQ(format_args.get_id("first"), 0);
    EXPECT_EQ(format_args.get_id("second"), 1);
    EXPECT_EQ(format_args.get_id("non_existent"), -1);
}

// Test case with different character types for basic_string_view (Char)
// The focal function is templated on `Char`.
TEST_F(FormatArgStoreTest, GetIdWithWideCharName) {
    // fmt::arg typically uses char. To test Char=wchar_t, we need to construct
    // basic_string_view<wchar_t> and pass it.
    // fmt::make_format_args itself doesn't directly support wchar_t named args in the same way.
    // However, the `get_id` method is templated on `Char`.
    // We can call `get_id` with a `basic_string_view<wchar_t>`.

    auto args = fmt::make_format_args(fmt::arg("name_char", 10));
    fmt::format_args format_args = args;

    // The internal storage uses `char` for names.
    // So, calling `get_id` with `wchar_t` should still compare against `char` names.
    // This implies an implicit conversion or a comparison mechanism.
    // Based on fmt's design, `basic_string_view<Char>` is usually compared against
    // the stored `basic_string_view<char>`.
    // The `operator==` for `basic_string_view` handles different `Char` types.

    // Test with char string view (normal case)
    EXPECT_EQ(format_args.get_id(fmt::string_view("name_char")), 0);

    // Test with wchar_t string view
    // This will likely involve a comparison between basic_string_view<char> and basic_string_view<wchar_t>
    // which fmt's basic_string_view operator== supports.
    EXPECT_EQ(format_args.get_id(fmt::basic_string_view<wchar_t>(L"name_char")), 0);
    EXPECT_EQ(format_args.get_id(fmt::basic_string_view<wchar_t>(L"non_existent_w")), -1);
}

// Test case with a very long named argument name
TEST_F(FormatArgStoreTest, GetIdLongName) {
    std::string long_name(256, 'a'); // A long name
    auto args = fmt::make_format_args(fmt::arg(long_name, 1));
    fmt::format_args format_args = args;

    EXPECT_EQ(format_args.get_id(long_name), 0);
    EXPECT_EQ(format_args.get_id(long_name + "b"), -1);
}