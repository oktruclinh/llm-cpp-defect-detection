#include <gtest/gtest.h>
#include <string>
#include "peglib.h"

namespace {
using peg::parser;

// Helper to build a parser from a grammar and return whether generation succeeded.
bool BuildParser(const char* grammar, parser& p) {
  return p.load_grammar(grammar);
}

TEST(PeglibDetectLeftRecursionTest, DirectLeftRecursionIsRejected) {
  parser p;
  const char* grammar = R"(
    Expr <- Expr 'a' / 'b'
  )";

  ASSERT_FALSE(BuildParser(grammar, p));
}

TEST(PeglibDetectLeftRecursionTest, IndirectLeftRecursionIsRejected) {
  parser p;
  const char* grammar = R"(
    A <- B
    B <- A
  )";

  ASSERT_FALSE(BuildParser(grammar, p));
}

TEST(PeglibDetectLeftRecursionTest, LongerIndirectLeftRecursionIsRejected) {
  parser p;
  const char* grammar = R"(
    A <- B
    B <- C
    C <- A / 'x'
  )";

  ASSERT_FALSE(BuildParser(grammar, p));
}

TEST(PeglibDetectLeftRecursionTest, NonRecursiveReferenceChainIsAccepted) {
  parser p;
  const char* grammar = R"(
    A <- B
    B <- C
    C <- 'x'
  )";

  ASSERT_TRUE(BuildParser(grammar, p));

  std::string out;
  EXPECT_TRUE(p.parse("x", out));
  EXPECT_EQ("x", out);

  out.clear();
  EXPECT_FALSE(p.parse("y", out));
}

TEST(PeglibDetectLeftRecursionTest, RightRecursionIsAccepted) {
  parser p;
  const char* grammar = R"(
    A <- 'a' A / 'b'
  )";

  ASSERT_TRUE(BuildParser(grammar, p));

  std::string out;
  EXPECT_TRUE(p.parse("b", out));
  EXPECT_EQ("b", out);

  out.clear();
  EXPECT_TRUE(p.parse("aaab", out));
  EXPECT_EQ("aaab", out);

  out.clear();
  EXPECT_FALSE(p.parse("aaaa", out));
}

TEST(PeglibDetectLeftRecursionTest, ChoiceCanAvoidRecursiveAlternativeWhenNotLeftRecursive) {
  parser p;
  const char* grammar = R"(
    A <- B / 'x'
    B <- 'y'
  )";

  ASSERT_TRUE(BuildParser(grammar, p));

  std::string out;
  EXPECT_TRUE(p.parse("x", out));
  EXPECT_EQ("x", out);

  out.clear();
  EXPECT_TRUE(p.parse("y", out));
  EXPECT_EQ("y", out);

  out.clear();
  EXPECT_FALSE(p.parse("z", out));
}

TEST(PeglibDetectLeftRecursionTest, RepeatedReferenceToSameNonRecursiveRuleDoesNotCauseFalsePositive) {
  parser p;
  const char* grammar = R"(
    A <- B B
    B <- 'x'
  )";

  ASSERT_TRUE(BuildParser(grammar, p));

  std::string out;
  EXPECT_TRUE(p.parse("xx", out));
  EXPECT_EQ("xx", out);

  out.clear();
  EXPECT_FALSE(p.parse("x", out));

  out.clear();
  EXPECT_FALSE(p.parse("xxx", out));
}

TEST(PeglibDetectLeftRecursionTest, RecursiveCycleNotReachingStartRuleThroughVisitedReferenceIsAccepted) {
  parser p;
  const char* grammar = R"(
    Start <- A
    A <- B / 'ok'
    B <- C
    C <- B
  )";

  ASSERT_TRUE(BuildParser(grammar, p));

  std::string out;
  EXPECT_TRUE(p.parse("ok", out));
  EXPECT_EQ("ok", out);

  out.clear();
  EXPECT_FALSE(p.parse("ng", out));
}

TEST(PeglibDetectLeftRecursionTest, SelfReferenceBehindConsumedInputIsAccepted) {
  parser p;
  const char* grammar = R"(
    S <- '(' S ')' / 'x'
  )";

  ASSERT_TRUE(BuildParser(grammar, p));

  std::string out;
  EXPECT_TRUE(p.parse("x", out));
  EXPECT_EQ("x", out);

  out.clear();
  EXPECT_TRUE(p.parse("((x))", out));
  EXPECT_EQ("((x))", out);

  out.clear();
  EXPECT_FALSE(p.parse("(x", out));
}

TEST(PeglibDetectLeftRecursionTest, EmptyGrammarIsRejected) {
  parser p;
  EXPECT_FALSE(BuildParser("", p));
}

TEST(PeglibDetectLeftRecursionTest, UndefinedReferenceIsRejected) {
  parser p;
  const char* grammar = R"(
    A <- Missing
  )";

  EXPECT_FALSE(BuildParser(grammar, p));
}

TEST(PeglibDetectLeftRecursionTest, MutualRecursionWithProductiveAlternativeStillRejectedWhenLeftRecursiveFromStart) {
  parser p;
  const char* grammar = R"(
    A <- B / 'a'
    B <- A 'b'
  )";

  EXPECT_FALSE(BuildParser(grammar, p));
}

}  // namespace