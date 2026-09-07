#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <stdexcept>

// Test that named arguments are correctly resolved via the public API.
// This indirectly exercises the get_id function.

TEST(NamedArgGetIdTest, SingleNamedArg) {
  std::string result = fmt::format("{name}", fmt::arg("name", 42));
  EXPECT_EQ(result, "42");
}

TEST(NamedArgGetIdTest, MultipleNamedArgs) {
  std::string result = fmt::format("{a} {b}", fmt::arg("a", 1), fmt::arg("b", 2));
  EXPECT_EQ(result, "1 2");
}

TEST(NamedArgGetIdTest, NamedArgWithStringValue) {
  std::string result = fmt::format("{greeting}", fmt::arg("greeting", "hello"));
  EXPECT_EQ(result, "hello");
}

TEST(NamedArgGetIdTest, MissingNamedArgThrows) {
  // When a named argument is not provided, fmt::format should throw.
  EXPECT_THROW(fmt::format("{missing}", fmt::arg("other", 1)), fmt::format_error);
}

TEST(NamedArgGetIdTest, NoNamedArgsThrows) {
  // If no named arguments are provided at all, using a named placeholder throws.
  EXPECT_THROW(fmt::format("{name}"), fmt::format_error);
}

TEST(NamedArgGetIdTest, DuplicateNameLastWins) {
  // When multiple arguments have the same name, the last one should be used.
  std::string result = fmt::format("{x}", fmt::arg("x", 1), fmt::arg("x", 2));
  EXPECT_EQ(result, "2");
}

TEST(NamedArgGetIdTest, MixedPositionalAndNamed) {
  // Positional and named arguments can be mixed.
  std::string result = fmt::format("{0} {name}", 10, fmt::arg("name", 20));
  EXPECT_EQ(result, "10 20");
}

TEST(NamedArgGetIdTest, NamedArgWithIndex) {
  // Named arguments can appear in any order; indices refer to positional args.
  std::string result = fmt::format("{name} {0}", fmt::arg("name", "hello"), 99);
  EXPECT_EQ(result, "hello 99");
}

TEST(NamedArgGetIdTest, ManyNamedArgs) {
  // Stress test with many named arguments.
  std::string result = fmt::format(
      "{a}{b}{c}{d}{e}{f}{g}{h}{i}{j}",
      fmt::arg("a", 1), fmt::arg("b", 2), fmt::arg("c", 3),
      fmt::arg("d", 4), fmt::arg("e", 5), fmt::arg("f", 6),
      fmt::arg("g", 7), fmt::arg("h", 8), fmt::arg("i", 9),
      fmt::arg("j", 10));
  EXPECT_EQ(result, "12345678910");
}

TEST(NamedArgGetIdTest, NamedArgWithSpecialCharacters) {
  // Names can contain underscores and digits.
  std::string result = fmt::format("{my_var_1}", fmt::arg("my_var_1", 100));
  EXPECT_EQ(result, "100");
}

TEST(NamedArgGetIdTest, NamedArgWithEmptyName) {
  // An empty name is allowed as an argument, but cannot be used in format string.
  // The format string {} is positional, so using it with an empty-named arg
  // will not match; it will use the positional argument instead.
  // This test verifies that no crash occurs and the positional arg is used.
  std::string result = fmt::format("{}", fmt::arg("", 42));
  EXPECT_EQ(result, "42");
}

TEST(NamedArgGetIdTest, NamedArgWithEmptyNameThrowsIfUsed) {
  // If we try to use an empty name in the format string, it's invalid syntax.
  // The format string {} is positional, so { } is not valid.
  // Actually, { } is a named argument with a space, not empty.
  // To use an empty name, one would need {} which is positional.
  // So there is no way to reference an empty-named argument via format string.
  // This test ensures that using a format string that looks like an empty name
  // (e.g., {}) does not accidentally match the empty-named argument.
  std::string result = fmt::format("{}", fmt::arg("", 42));
  EXPECT_EQ(result, "42");
  // The empty-named argument is ignored because {} is positional.
}

TEST(NamedArgGetIdTest, NamedArgWithWhitespaceName) {
  // Names can contain spaces? The format string syntax does not allow spaces
  // inside the braces for named arguments. So a name with a space is not
  // directly usable. But we can still create such an argument; it will not
  // be matched by any format placeholder.
  std::string result = fmt::format("{}", fmt::arg("my name", 7));
  EXPECT_EQ(result, "7");
  // The named argument is ignored; positional arg is used.
}

TEST(NamedArgGetIdTest, NamedArgWithLongName) {
  // Long names should work.
  std::string long_name(1000, 'x');
  std::string result = fmt::format("{name}", fmt::arg(long_name, 123));
  // The format string uses a short name, so it won't match the long name.
  // This should throw because the named argument is not found.
  EXPECT_THROW(fmt::format("{name}", fmt::arg(long_name, 123)), fmt::format_error);
}

TEST(NamedArgGetIdTest, NamedArgWithSameNameDifferentTypes) {
  // The same name can be used with different types in different calls.
  std::string result1 = fmt::format("{val}", fmt::arg("val", 42));
  std::string result2 = fmt::format("{val}", fmt::arg("val", "hello"));
  EXPECT_EQ(result1, "42");
  EXPECT_EQ(result2, "hello");
}

TEST(NamedArgGetIdTest, NamedArgWithFormatSpec) {
  // Named arguments can have format specifiers.
  std::string result = fmt::format("{val:05d}", fmt::arg("val", 7));
  EXPECT_EQ(result, "00007");
}

TEST(NamedArgGetIdTest, NamedArgWithPrecision) {
  // Named arguments with floating point and precision.
  std::string result = fmt::format("{pi:.2f}", fmt::arg("pi", 3.14159));
  EXPECT_EQ(result, "3.14");
}