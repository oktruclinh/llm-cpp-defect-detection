#include <gtest/gtest.h>
#include <fmt/format.h>

#include <stdexcept>
#include <string>

TEST(FmtNamedArgumentLookupTest, ReturnsNamedStringArgument) {
  std::string result = fmt::format("{name}", fmt::arg("name", "value"));
  EXPECT_EQ(result, "value");
}

TEST(FmtNamedArgumentLookupTest, ReturnsNamedIntegerArgument) {
  std::string result = fmt::format("{count}", fmt::arg("count", 42));
  EXPECT_EQ(result, "42");
}

TEST(FmtNamedArgumentLookupTest, SupportsEmptyArgumentName) {
  std::string result = fmt::format("{0:s}|{1}", "", fmt::arg("", 7));
  EXPECT_EQ(result, "|7");
}

TEST(FmtNamedArgumentLookupTest, ReusesSameNamedArgumentMultipleTimes) {
  std::string result = fmt::format("{item}-{item}-{item}", fmt::arg("item", 9));
  EXPECT_EQ(result, "9-9-9");
}

TEST(FmtNamedArgumentLookupTest, FindsNamedArgumentAmongPositionalArguments) {
  std::string result = fmt::format("{} {name} {}", 1, fmt::arg("name", 2), 3);
  EXPECT_EQ(result, "1 2 3");
}

TEST(FmtNamedArgumentLookupTest, MissingNamedArgumentThrowsFormatError) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{missing}", fmt::arg("present", 1));
        } catch (const fmt::format_error& e) {
          EXPECT_EQ(std::string(e.what()), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, MissingNamedArgumentWithNoArgumentsThrowsFormatError) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{name}");
        } catch (const fmt::format_error& e) {
          EXPECT_EQ(std::string(e.what()), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, MissingEmptyNamedArgumentThrowsFormatError) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{0}", fmt::arg("other", 1));
        } catch (const fmt::format_error& e) {
          EXPECT_EQ(std::string(e.what()), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, NameLookupIsExact) {
  std::string result = fmt::format("{a} {ab}", fmt::arg("a", 1), fmt::arg("ab", 2));
  EXPECT_EQ(result, "1 2");
}

TEST(FmtNamedArgumentLookupTest, NameLookupIsCaseSensitive) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{Name}", fmt::arg("name", 5));
        } catch (const fmt::format_error& e) {
          EXPECT_EQ(std::string(e.what()), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, WorksWithWideCharacterFormatStrings) {
  std::wstring result = fmt::format(L"{0}:{1}", L"id", 11);
  EXPECT_EQ(result, L"id:11");
}

TEST(FmtNamedArgumentLookupTest, MissingWideNamedArgumentThrowsFormatError) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format(L"{0}", fmt::arg(L"present", 1));
        } catch (const fmt::format_error& e) {
          EXPECT_EQ(std::string(e.what()), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}