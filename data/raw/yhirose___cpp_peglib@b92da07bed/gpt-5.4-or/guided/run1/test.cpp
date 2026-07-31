#include <gtest/gtest.h>

#include "peglib.h"

#include <string>
#include <vector>

namespace {

using peg::parser;

TEST(PeglibPackratBehaviorTest, RepeatedReferenceUsesMemoizationForSuccessfulParse) {
  parser pg(R"(
    Start <- A A
    A <- Counted / 'a'
    Counted <- 'a' { count++; }
  )");

  ASSERT_TRUE(pg);

  int count = 0;
  pg["Counted"] = [&count](const peg::SemanticValues &) { count++; };

  std::string input = "aa";
  auto result = pg.parse(input);

  EXPECT_TRUE(result);
  EXPECT_EQ(1, count);
}

TEST(PeglibPackratBehaviorTest, RepeatedReferenceUsesMemoizationForFailedParse) {
  parser pg(R"(
    Start <- A / A
    A <- Counted
    Counted <- 'b' { count++; }
  )");

  ASSERT_TRUE(pg);

  int count = 0;
  pg["Counted"] = [&count](const peg::SemanticValues &) { count++; };

  std::string input = "a";
  auto result = pg.parse(input);

  EXPECT_FALSE(result);
  EXPECT_EQ(1, count);
}

TEST(PeglibPackratBehaviorTest, PackratDisabledReevaluatesSuccessfulRule) {
  parser pg(R"(
    Start <- A A
    A <- Counted / 'a'
    Counted <- 'a' { count++; }
  )");

  ASSERT_TRUE(pg);

  pg.enable_packrat_parsing(false);

  int count = 0;
  pg["Counted"] = [&count](const peg::SemanticValues &) { count++; };

  std::string input = "aa";
  auto result = pg.parse(input);

  EXPECT_TRUE(result);
  EXPECT_EQ(2, count);
}

TEST(PeglibPackratBehaviorTest, PackratDisabledReevaluatesFailedRule) {
  parser pg(R"(
    Start <- A / A
    A <- Counted
    Counted <- 'b' { count++; }
  )");

  ASSERT_TRUE(pg);

  pg.enable_packrat_parsing(false);

  int count = 0;
  pg["Counted"] = [&count](const peg::SemanticValues &) { count++; };

  std::string input = "a";
  auto result = pg.parse(input);

  EXPECT_FALSE(result);
  EXPECT_EQ(2, count);
}

TEST(PeglibPackratBehaviorTest, EmptyInputCanBeParsedAndMemoizedAtEndPosition) {
  parser pg(R"(
    Start <- Empty Empty
    Empty <- Counted / ''
    Counted <- '' { count++; }
  )");

  ASSERT_TRUE(pg);

  int count = 0;
  pg["Counted"] = [&count](const peg::SemanticValues &) { count++; };

  std::string input;
  auto result = pg.parse(input);

  EXPECT_TRUE(result);
  EXPECT_EQ(1, count);
}

TEST(PeglibPackratBehaviorTest, DifferentPositionsAreCachedIndependently) {
  parser pg(R"(
    Start <- A A
    A <- Counted / 'a'
    Counted <- 'a' { count++; }
  )");

  ASSERT_TRUE(pg);

  int count = 0;
  pg["Counted"] = [&count](const peg::SemanticValues &) { count++; };

  std::string input = "aa";
  auto result = pg.parse(input);

  EXPECT_TRUE(result);
  EXPECT_EQ(1, count);

  count = 0;
  input = "aaa";
  result = pg.parse(input);

  EXPECT_FALSE(result);
  EXPECT_EQ(2, count);
}

} // namespace