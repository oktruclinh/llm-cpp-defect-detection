#include <gtest/gtest.h>

#include <fmt/core.h>
#include <string>

namespace {

TEST(FmtCoreNamedArgLookupTest, FindsSingleNamedArgumentByName) {
  auto args = fmt::make_format_args(fmt::arg("answer", 42));

  EXPECT_EQ(args.get_id("answer"), 0);
  EXPECT_EQ(args.get_id("missing"), -1);
}

TEST(FmtCoreNamedArgLookupTest, NoNamedArgumentsReturnsMinusOne) {
  auto args = fmt::make_format_args(1, "text", 3.14);

  EXPECT_EQ(args.get_id(""), -1);
  EXPECT_EQ(args.get_id("anything"), -1);
}

TEST(FmtCoreNamedArgLookupTest, FindsEachNamedArgumentAtItsCorrectIndex) {
  auto args = fmt::make_format_args(
      fmt::arg("first", 10), fmt::arg("second", 20), fmt::arg("third", 30));

  EXPECT_EQ(args.get_id("first"), 0);
  EXPECT_EQ(args.get_id("second"), 1);
  EXPECT_EQ(args.get_id("third"), 2);
  EXPECT_EQ(args.get_id("fourth"), -1);
}

TEST(FmtCoreNamedArgLookupTest, MixedPositionalAndNamedArgumentsUseOverallArgumentIndex) {
  auto args = fmt::make_format_args(
      7, fmt::arg("named", 11), 'x', fmt::arg("other", 13));

  EXPECT_EQ(args.get_id("named"), 1);
  EXPECT_EQ(args.get_id("other"), 3);
  EXPECT_EQ(args.get_id("7"), -1);
}

TEST(FmtCoreNamedArgLookupTest, EmptyNamedArgumentCanBeFoundAndMissingNamesAreNotMatched) {
  auto args = fmt::make_format_args(fmt::arg("", 1), fmt::arg("nonempty", 2));

  EXPECT_EQ(args.get_id(""), 0);
  EXPECT_EQ(args.get_id("nonempty"), 1);
  EXPECT_EQ(args.get_id(" "), -1);
}

TEST(FmtCoreNamedArgLookupTest, LookupIsExactAndCaseSensitive) {
  auto args = fmt::make_format_args(
      fmt::arg("name", 1), fmt::arg("Name", 2), fmt::arg("name ", 3));

  EXPECT_EQ(args.get_id("name"), 0);
  EXPECT_EQ(args.get_id("Name"), 1);
  EXPECT_EQ(args.get_id("name "), 2);
  EXPECT_EQ(args.get_id("NAME"), -1);
}

TEST(FmtCoreNamedArgLookupTest, WorksWhenArgumentsArePacked) {
  // A small number of arguments is typically stored in the packed representation.
  auto args = fmt::make_format_args(
      fmt::arg("a", 1), fmt::arg("b", 2), fmt::arg("c", 3));

  EXPECT_EQ(args.get_id("a"), 0);
  EXPECT_EQ(args.get_id("b"), 1);
  EXPECT_EQ(args.get_id("c"), 2);
}

TEST(FmtCoreNamedArgLookupTest, WorksWhenArgumentsAreUnpackedManyArguments) {
  // Use enough arguments to exercise the non-packed path.
  auto args = fmt::make_format_args(
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
      fmt::arg("ten", 10), fmt::arg("eleven", 11), fmt::arg("twelve", 12));

  EXPECT_EQ(args.get_id("ten"), 10);
  EXPECT_EQ(args.get_id("eleven"), 11);
  EXPECT_EQ(args.get_id("twelve"), 12);
  EXPECT_EQ(args.get_id("nine"), -1);
}

TEST(FmtCoreNamedArgLookupTest, SupportsWideCharacterNamedArgumentLookup) {
  auto args = fmt::make_wformat_args(
      fmt::arg(L"alpha", 1), fmt::arg(L"beta", 2), fmt::arg(L"", 3));

  EXPECT_EQ(args.get_id(L"alpha"), 0);
  EXPECT_EQ(args.get_id(L"beta"), 1);
  EXPECT_EQ(args.get_id(L""), 2);
  EXPECT_EQ(args.get_id(L"missing"), -1);
}

TEST(FmtCoreNamedArgLookupTest, PrefixesDoNotMatchLongerNames) {
  auto args = fmt::make_format_args(
      fmt::arg("abc", 1), fmt::arg("abcd", 2), fmt::arg("xabc", 3));

  EXPECT_EQ(args.get_id("abc"), 0);
  EXPECT_EQ(args.get_id("abcd"), 1);
  EXPECT_EQ(args.get_id("ab"), -1);
  EXPECT_EQ(args.get_id("xabc"), 2);
}

}  // namespace