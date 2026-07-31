#include <gtest/gtest.h>

#include <fmt/format.h>

#include <string>

namespace {

TEST(FmtNamedArgumentLookupTest, RetrievesNamedArgumentByName) {
  std::string result = fmt::format("Hello, {name}!", fmt::arg("name", "world"));
  EXPECT_EQ(result, "Hello, world!");
}

TEST(FmtNamedArgumentLookupTest, RetrievesMultipleNamedArgumentsAndPreservesMapping) {
  std::string result =
      fmt::format("{greeting}, {name}! You have {count} messages.",
                  fmt::arg("greeting", "Hello"),
                  fmt::arg("name", "Alice"),
                  fmt::arg("count", 3));
  EXPECT_EQ(result, "Hello, Alice! You have 3 messages.");
}

TEST(FmtNamedArgumentLookupTest, RepeatedLookupOfSameNamedArgumentWorks) {
  std::string result = fmt::format("{item}-{item}-{item}", fmt::arg("item", 42));
  EXPECT_EQ(result, "42-42-42");
}

TEST(FmtNamedArgumentLookupTest, EmptyArgumentNameCanBeResolved) {
  std::string result = fmt::format("value=[{}]", fmt::arg("", 7));
  EXPECT_EQ(result, "value=[7]");
}

TEST(FmtNamedArgumentLookupTest, MissingNamedArgumentThrowsFormatError) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("Value: {missing}", fmt::arg("present", 1));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ(e.what(), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, MissingNamedArgumentAmongExistingNamedArgumentsThrowsFormatError) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{a} {missing} {b}",
                            fmt::arg("a", "left"),
                            fmt::arg("b", "right"));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ(e.what(), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, NameLookupIsCaseSensitive) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{Name}", fmt::arg("name", 123));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ(e.what(), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, SimilarPrefixesDoNotMatchWrongArgument) {
  std::string result = fmt::format("{id}:{id2}:{id}",
                                   fmt::arg("id", 10),
                                   fmt::arg("id2", 20));
  EXPECT_EQ(result, "10:20:10");
}

TEST(FmtNamedArgumentLookupTest, UnicodeLikeBytesInArgumentNameAreMatchedExactly) {
  const char* key = u8"naïve";
  std::string result = fmt::format("{naïve}", fmt::arg(key, "ok"));
  EXPECT_EQ(result, "ok");

  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{naive}", fmt::arg(key, "ok"));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ(e.what(), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, NamedArgumentWorksWithFormatToBufferAPI) {
  fmt::memory_buffer buffer;
  fmt::format_to(buffer,
                 "[{first}|{second}|{first}]",
                 fmt::arg("first", "A"),
                 fmt::arg("second", "B"));
  EXPECT_EQ(fmt::to_string(buffer), "[A|B|A]");
}

}  // namespace