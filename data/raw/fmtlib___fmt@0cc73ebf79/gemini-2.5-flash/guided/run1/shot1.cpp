#include <fmt/core.h>
#include <gtest/gtest.h>

// Helper function to create a format string with named arguments
template <typename... Args>
std::string format_with_named_args(fmt::string_view format_str, Args&&... args) {
  return fmt::format(format_str, std::forward<Args>(args)...);
}

TEST(FormatArgStoreTest, GetIdNormalCases) {
  // Test with a single named argument
  auto f1 = fmt::format("Hello {name}", fmt::arg("name", "World"));
  // The actual value of id is implementation-dependent, but it should be non-negative
  // and consistent for the same named argument within the same format call.
  // We can't directly call get_id, so we'll test its effect through formatting.
  // This test primarily ensures that named arguments are correctly processed
  // and can be retrieved.

  // To test get_id, we need to access the internal `format_arg_store` or similar
  // structure. Since the prompt explicitly states to "Do NOT call internal/implementation
  // symbols", and `get_id` is a method of `format_arg_store`, which is an internal
  // detail, we cannot directly test `get_id` as a standalone function.

  // The `get_id` function is used internally by `fmt::format` to resolve named arguments.
  // Therefore, the best way to test its *behavior* through the public API is to
  // ensure that named arguments are correctly substituted in the output.
  // If `get_id` were buggy, named arguments would likely not be resolved correctly.

  EXPECT_EQ(fmt::format("Hello {name}!", fmt::arg("name", "World")), "Hello World!");
  EXPECT_EQ(fmt::format("{greeting}, {name}!", fmt::arg("name", "Alice"), fmt::arg("greeting", "Hi")), "Hi, Alice!");
  EXPECT_EQ(fmt::format("{0}, {name}!", 123, fmt::arg("name", "Bob")), "123, Bob!");
}

TEST(FormatArgStoreTest, GetIdMultipleNamedArgs) {
  EXPECT_EQ(fmt::format("First: {first}, Second: {second}", fmt::arg("first", 1), fmt::arg("second", 2)), "First: 1, Second: 2");
  EXPECT_EQ(fmt::format("{a} {b} {c}", fmt::arg("a", 'A'), fmt::arg("b", 'B'), fmt::arg("c", 'C')), "A B C");
}

TEST(FormatArgStoreTest, GetIdMixedPositionalAndNamedArgs) {
  EXPECT_EQ(fmt::format("{0} {name} {1}", 10, 20, fmt::arg("name", "Middle")), "10 Middle 20");
  EXPECT_EQ(fmt::format("{name} {0}", fmt::arg("name", "Start"), 30), "Start 30");
  EXPECT_EQ(fmt::format("{0} {name}", 40, fmt::arg("name", "End")), "40 End");
}

TEST(FormatArgStoreTest, GetIdDuplicateNamedArgs) {
  // When duplicate named arguments are provided, the first one should be used.
  // The behavior of `get_id` for duplicates is to return the ID of the first match.
  // This is implicitly tested by the formatting output.
  EXPECT_EQ(fmt::format("{name}", fmt::arg("name", "First"), fmt::arg("name", "Second")), "First");
  EXPECT_EQ(fmt::format("{name} {name}", fmt::arg("name", "First"), fmt::arg("name", "Second")), "First First");
}

TEST(FormatArgStoreTest, GetIdNonExistentNamedArg) {
  // If a named argument is requested in the format string but not provided,
  // fmt::format will throw an exception. This indirectly tests that `get_id`
  // would not find the argument and lead to this error path.
  EXPECT_THROW(fmt::format("Hello {nonexistent_name}"), fmt::format_error);
  EXPECT_THROW(fmt::format("{0} {nonexistent_name}", 123), fmt::format_error);
}

TEST(FormatArgStoreTest, GetIdNoNamedArgsProvided) {
  // If no named arguments are provided, `has_named_args()` would be false,
  // and `get_id` would return -1. This is implicitly tested by ensuring
  // that formatting without named args works as expected.
  EXPECT_EQ(fmt::format("Hello World"), "Hello World");
  EXPECT_EQ(fmt::format("Value: {0}", 123), "Value: 123");
}

TEST(FormatArgStoreTest, GetIdEmptyNamedArgName) {
  // An empty string as a named argument name is not typically supported
  // and would likely lead to a format_error if used in the format string.
  // If `get_id` were called with an empty name, it should not match anything.
  // We test the public API behavior.
  EXPECT_THROW(fmt::format("Hello {}", fmt::arg("", "World")), fmt::format_error);
  EXPECT_THROW(fmt::format("Hello {name}", fmt::arg("", "World")), fmt::format_error); // The empty name arg is ignored
}

TEST(FormatArgStoreTest, GetIdLongNamedArgName) {
  std::string long_name(200, 'a');
  std::string format_str = fmt::format("Hello {{{}}}", long_name);
  EXPECT_EQ(fmt::format(format_str, fmt::arg(long_name.c_str(), "World")), "Hello World");
}

TEST(FormatArgStoreTest, GetIdSpecialCharactersInNamedArgName) {
  // Named argument names can contain alphanumeric characters and underscores.
  // Other special characters might lead to parsing errors in the format string.
  // We test valid cases.
  EXPECT_EQ(fmt::format("Value: {my_arg_1}", fmt::arg("my_arg_1", 123)), "Value: 123");
  EXPECT_EQ(fmt::format("Value: {__arg__}", fmt::arg("__arg__", "test")), "Value: test");
}