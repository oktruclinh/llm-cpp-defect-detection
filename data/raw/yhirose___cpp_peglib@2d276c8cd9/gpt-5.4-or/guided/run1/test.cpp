#include <gtest/gtest.h>
#include "peglib.h"

#include <string>

namespace {

using peg::parser;

TEST(PeglibLeftRecursionDetectionTest, DirectLeftRecursionIsRejectedWithRuleName) {
  parser pg;

  const auto grammar = R"(
    Expr <- Expr 'a' / 'b'
  )";

  EXPECT_FALSE(pg.load_grammar(grammar));

  const auto msg = pg.log;
  EXPECT_NE(msg.find("Expr"), std::string::npos);
  EXPECT_NE(msg.find("left"), std::string::npos);
}

TEST(PeglibLeftRecursionDetectionTest, IndirectLeftRecursionAcrossMultipleRulesIsRejected) {
  parser pg;

  const auto grammar = R"(
    A <- B
    B <- C
    C <- A / 'c'
  )";

  EXPECT_FALSE(pg.load_grammar(grammar));

  const auto msg = pg.log;
  EXPECT_NE(msg.find("A"), std::string::npos);
  EXPECT_NE(msg.find("left"), std::string::npos);
}

TEST(PeglibLeftRecursionDetectionTest, NonLeftRecursiveCycleThroughRightConsumptionIsAccepted) {
  parser pg;

  const auto grammar = R"(
    A <- 'a' A / 'b'
  )";

  ASSERT_TRUE(pg.load_grammar(grammar)) << pg.log;

  EXPECT_TRUE(pg.parse("b"));
  EXPECT_TRUE(pg.parse("ab"));
  EXPECT_TRUE(pg.parse("aaab"));
  EXPECT_FALSE(pg.parse(""));
  EXPECT_FALSE(pg.parse("a"));
}

TEST(PeglibLeftRecursionDetectionTest, MutualReferenceWithoutLeftRecursionIsAccepted) {
  parser pg;

  const auto grammar = R"(
    A <- 'x' B / 'z'
    B <- 'y' A / 'w'
  )";

  ASSERT_TRUE(pg.load_grammar(grammar)) << pg.log;

  EXPECT_TRUE(pg.parse("z"));
  EXPECT_TRUE(pg.parse("xw"));
  EXPECT_TRUE(pg.parse("xyz"));
  EXPECT_TRUE(pg.parse("xyxyw"));

  EXPECT_FALSE(pg.parse(""));
  EXPECT_FALSE(pg.parse("x"));
  EXPECT_FALSE(pg.parse("xy"));
  EXPECT_FALSE(pg.parse("w"));
}

TEST(PeglibLeftRecursionDetectionTest, ChoiceBranchWithDirectLeftRecursionIsStillRejected) {
  parser pg;

  const auto grammar = R"(
    Start <- Start / ''
  )";

  EXPECT_FALSE(pg.load_grammar(grammar));

  const auto msg = pg.log;
  EXPECT_NE(msg.find("Start"), std::string::npos);
  EXPECT_NE(msg.find("left"), std::string::npos);
}

TEST(PeglibLeftRecursionDetectionTest, LeftRecursionHiddenBehindAnotherReferenceIsRejected) {
  parser pg;

  const auto grammar = R"(
    Start <- Prefix
    Prefix <- Start 'a' / 'b'
  )";

  EXPECT_FALSE(pg.load_grammar(grammar));

  const auto msg = pg.log;
  EXPECT_NE(msg.find("Start"), std::string::npos);
  EXPECT_NE(msg.find("left"), std::string::npos);
}

TEST(PeglibLeftRecursionDetectionTest, RepeatedReferenceToSameNonRecursiveRuleDoesNotCauseFalsePositive) {
  parser pg;

  const auto grammar = R"(
    Start <- A A
    A <- 'a'
  )";

  ASSERT_TRUE(pg.load_grammar(grammar)) << pg.log;

  EXPECT_TRUE(pg.parse("aa"));
  EXPECT_FALSE(pg.parse("a"));
  EXPECT_FALSE(pg.parse("aaa"));
}

TEST(PeglibLeftRecursionDetectionTest, LongerIndirectCycleIsRejected) {
  parser pg;

  const auto grammar = R"(
    S <- A
    A <- B
    B <- C
    C <- D
    D <- S / 'd'
  )";

  EXPECT_FALSE(pg.load_grammar(grammar));

  const auto msg = pg.log;
  EXPECT_NE(msg.find("S"), std::string::npos);
  EXPECT_NE(msg.find("left"), std::string::npos);
}

}  // namespace