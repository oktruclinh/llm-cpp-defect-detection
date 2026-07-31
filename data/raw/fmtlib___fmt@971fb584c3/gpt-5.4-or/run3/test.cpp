#include <gtest/gtest.h>
#include <fmt/format.h>

#include <string>
#include <stdexcept>

namespace {

TEST(FmtNamedArgumentsGetArgTest, RetrievesNamedStringArgument) {
  std::string result = fmt::format("Hello, {name}!", fmt::arg("name", "world"));
  EXPECT_EQ(result, "Hello, world!");
}

TEST(FmtNamedArgumentsGetArgTest, RetrievesNamedIntegerArgumentMultipleTimes) {
  std::string result = fmt::format("{value}-{value}-{value}", fmt::arg("value", 42));
  EXPECT_EQ(result, "42-42-42");
}

TEST(FmtNamedArgumentsGetArgTest, SupportsDifferentNamedArguments) {
  std::string result =
      fmt::format("{greeting}, {name}. You have {count} messages.",
                  fmt::arg("greeting", "Hi"),
                  fmt::arg("name", "Alice"),
                  fmt::arg("count", 3));
  EXPECT_EQ(result, "Hi, Alice. You have 3 messages.");
}

TEST(FmtNamedArgumentsGetArgTest, SupportsEmptyArgumentName) {
  std::string result = fmt::format("Value:[{}]", fmt::arg("", 7));
  EXPECT_EQ(result, "Value:[7]");
}

TEST(FmtNamedArgumentsGetArgTest, SupportsLongArgumentName) {
  const std::string long_name(256, 'x');
  std::string format_str = "{" + long_name + "}";
  std::string result = fmt::format(fmt::runtime(format_str), fmt::arg(long_name.c_str(), 99));
  EXPECT_EQ(result, "99");
}

TEST(FmtNamedArgumentsGetArgTest, NamedLookupIsCaseSensitive) {
  EXPECT_THROW(
      (void)fmt::format("{Name}", fmt::arg("name", 1)),
      fmt::format_error);
}

TEST(FmtNamedArgumentsGetArgTest, MissingNamedArgumentThrowsFormatError) {
  EXPECT_THROW(
      (void)fmt::format("before {missing} after", fmt::arg("present", 123)),
      fmt::format_error);
}

TEST(FmtNamedArgumentsGetArgTest, EmptyNameMissingArgumentThrowsFormatError) {
  EXPECT_THROW(
      (void)fmt::format("{}", fmt::arg("not_empty", 1)),
      fmt::format_error);
}

TEST(FmtNamedArgumentsGetArgTest, SimilarNamesDoNotAliasPrefixes) {
  std::string result = fmt::format("{id} {id2}",
                                   fmt::arg("id", 10),
                                   fmt::arg("id2", 20));
  EXPECT_EQ(result, "10 20");
}

TEST(FmtNamedArgumentsGetArgTest, SimilarNamesDoNotAliasSuffixes) {
  std::string result = fmt::format("{abc} {zabc}",
                                   fmt::arg("abc", 1),
                                   fmt::arg("zabc", 2));
  EXPECT_EQ(result, "1 2");
}

TEST(FmtNamedArgumentsGetArgTest, SupportsNamedArgumentsInWideFormatStrings) {
  std::wstring result = fmt::format(L"{name}:{count}",
                                    fmt::arg(L"name", L"item"),
                                    fmt::arg(L"count", 5));
  EXPECT_EQ(result, L"item:5");
}

TEST(FmtNamedArgumentsGetArgTest, MissingWideNamedArgumentThrowsFormatError) {
  EXPECT_THROW(
      (void)fmt::format(L"{missing}", fmt::arg(L"present", 1)),
      fmt::format_error);
}

TEST(FmtNamedArgumentsGetArgTest, RepeatedNamedLookupAmidPositionalTextProducesCorrectOutput) {
  std::string result = fmt::format("[{name}] and again [{name}]",
                                   fmt::arg("name", std::string("X")));
  EXPECT_EQ(result, "[X] and again [X]");
}

TEST(FmtNamedArgumentsGetArgTest, MissingArgumentInComplexFormatStillThrows) {
  EXPECT_THROW(
      (void)fmt::format("{present} {missing} {present}",
                        fmt::arg("present", "ok")),
      fmt::format_error);
}

}  // namespace