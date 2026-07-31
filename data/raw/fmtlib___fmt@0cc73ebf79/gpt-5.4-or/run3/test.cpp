#include <gtest/gtest.h>

#include <string>
#include <vector>

#include <fmt/core.h>

namespace {

using fmt::basic_format_args;
using fmt::format_arg;
using fmt::format_arg_store;
using fmt::format_context;
using fmt::make_format_args;
using fmt::string_view;

TEST(FmtCoreGetIdTest, NoNamedArgsReturnsInvalidIndex) {
  auto store = make_format_args(42, "text", 3.14);
  basic_format_args<format_context> args(store);

  EXPECT_EQ(args.get_id("name"), -1);
  EXPECT_EQ(args.get_id(""), -1);
  EXPECT_EQ(args.get_id("another"), -1);
}

TEST(FmtCoreGetIdTest, EmptyNamedArgListReturnsInvalidIndex) {
  format_arg_store<format_context> store{};
  basic_format_args<format_context> args(store);

  EXPECT_EQ(args.get_id("anything"), -1);
  EXPECT_EQ(args.get_id(""), -1);
}

TEST(FmtCoreGetIdTest, FindsSingleNamedArgumentByName) {
  auto store = make_format_args(fmt::arg("alpha", 123));
  basic_format_args<format_context> args(store);

  EXPECT_EQ(args.get_id("alpha"), 0);
  EXPECT_EQ(args.get_id("beta"), -1);
  EXPECT_EQ(args.get_id(""), -1);
}

TEST(FmtCoreGetIdTest, FindsCorrectIndicesAmongMixedPositionalAndNamedArguments) {
  auto store = make_format_args(7, fmt::arg("first", 11), 'x', fmt::arg("second", 22));
  basic_format_args<format_context> args(store);

  EXPECT_EQ(args.get_id("first"), 1);
  EXPECT_EQ(args.get_id("second"), 3);

  EXPECT_EQ(args.get_id("missing"), -1);
  EXPECT_EQ(args.get_id("First"), -1);
}

TEST(FmtCoreGetIdTest, FindsMultipleNamedArgumentsInOrder) {
  auto store = make_format_args(
      fmt::arg("n0", 0), fmt::arg("n1", 1), fmt::arg("n2", 2), fmt::arg("n3", 3));
  basic_format_args<format_context> args(store);

  EXPECT_EQ(args.get_id("n0"), 0);
  EXPECT_EQ(args.get_id("n1"), 1);
  EXPECT_EQ(args.get_id("n2"), 2);
  EXPECT_EQ(args.get_id("n3"), 3);
  EXPECT_EQ(args.get_id("n4"), -1);
}

TEST(FmtCoreGetIdTest, NameMatchingIsExactAndCaseSensitive) {
  auto store = make_format_args(
      fmt::arg("name", 1), fmt::arg("Name", 2), fmt::arg("name ", 3), fmt::arg("", 4));
  basic_format_args<format_context> args(store);

  EXPECT_EQ(args.get_id("name"), 0);
  EXPECT_EQ(args.get_id("Name"), 1);
  EXPECT_EQ(args.get_id("name "), 2);
  EXPECT_EQ(args.get_id(""), 3);

  EXPECT_EQ(args.get_id("NAME"), -1);
  EXPECT_EQ(args.get_id(" name"), -1);
}

TEST(FmtCoreGetIdTest, SupportsStringViewLookupIncludingEmbeddedNulls) {
  const std::string with_null("ab\0cd", 5);
  const std::string same_with_null("ab\0cd", 5);
  const std::string prefix("ab", 2);
  const std::string different_tail("ab\0ce", 5);

  auto store = make_format_args(fmt::arg(with_null.c_str(), 17), fmt::arg("plain", 9));
  basic_format_args<format_context> args(store);

  // fmt::arg(name, value) takes a null-terminated name, so only "ab" is stored.
  EXPECT_EQ(args.get_id(string_view(prefix.data(), prefix.size())), 0);
  EXPECT_EQ(args.get_id(string_view(same_with_null.data(), same_with_null.size())), -1);
  EXPECT_EQ(args.get_id(string_view(different_tail.data(), different_tail.size())), -1);
  EXPECT_EQ(args.get_id("plain"), 1);
}

TEST(FmtCoreGetIdTest, WorksForLargeEnoughArgumentSetThatMayUseUnpackedStorage) {
  auto store = make_format_args(
      fmt::arg("a0", 0), 1, fmt::arg("a2", 2), 3, fmt::arg("a4", 4), 5, fmt::arg("a6", 6),
      7, fmt::arg("a8", 8), 9, fmt::arg("a10", 10), 11, fmt::arg("a12", 12), 13,
      fmt::arg("a14", 14), 15, fmt::arg("a16", 16), 17, fmt::arg("a18", 18), 19);

  basic_format_args<format_context> args(store);

  EXPECT_EQ(args.get_id("a0"), 0);
  EXPECT_EQ(args.get_id("a2"), 2);
  EXPECT_EQ(args.get_id("a10"), 10);
  EXPECT_EQ(args.get_id("a18"), 18);

  EXPECT_EQ(args.get_id("a1"), -1);
  EXPECT_EQ(args.get_id("missing"), -1);
}

TEST(FmtCoreGetIdTest, ReturnedIdMatchesAccessByIndex) {
  auto store = make_format_args(100, fmt::arg("target", 200), 300);
  basic_format_args<format_context> args(store);

  const int id = args.get_id("target");
  ASSERT_EQ(id, 1);

  format_arg arg = args.get(id);
  ASSERT_TRUE(static_cast<bool>(arg));

  int visited = -1;
  fmt::visit_format_arg([&](const auto& value) { visited = static_cast<int>(value); }, arg);
  EXPECT_EQ(visited, 200);
}

TEST(FmtCoreGetIdTest, MissingNameDoesNotAliasExistingPartialMatches) {
  auto store = make_format_args(
      fmt::arg("a", 1), fmt::arg("ab", 2), fmt::arg("abc", 3), fmt::arg("abcd", 4));
  basic_format_args<format_context> args(store);

  EXPECT_EQ(args.get_id("a"), 0);
  EXPECT_EQ(args.get_id("ab"), 1);
  EXPECT_EQ(args.get_id("abc"), 2);
  EXPECT_EQ(args.get_id("abcd"), 3);

  EXPECT_EQ(args.get_id("abcde"), -1);
  EXPECT_EQ(args.get_id("b"), -1);
  EXPECT_EQ(args.get_id(""), -1);
}

}  // namespace