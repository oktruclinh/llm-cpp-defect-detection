#include <gtest/gtest.h>
#include <peglib.h>

#include <any>
#include <string>

namespace {

using peg::SemanticValues;
using peg::parser;

// Verifies successful parse path, semantic action invocation, and reduction result.
TEST(PeglibPrioritizedChoicePackratTest, SemanticActionProducesReducedValue) {
  parser pg(R"(
    ROOT <- ITEM
    ITEM <- 'a' {
      return std::string("ok");
    }
  )");

  EXPECT_TRUE(static_cast<bool>(pg));

  std::string result;
  EXPECT_TRUE(pg.parse("a", result));
  EXPECT_EQ("ok", result);
}

// Verifies that when the first alternative fails after consuming input,
// a later alternative can still succeed and its semantic action result is used.
// This exercises packrat/memoized parsing around a Definition-like rule.
TEST(PeglibPrioritizedChoicePackratTest, FailedAlternativeDoesNotPoisonLaterSuccess) {
  parser pg(R"(
    ROOT <- ITEM
    ITEM <- ('a' 'b' {
      return std::string("ab");
    }) / ('a' {
      return std::string("a");
    })
  )");

  EXPECT_TRUE(static_cast<bool>(pg));

  std::string result;
  EXPECT_TRUE(pg.parse("a", result));
  EXPECT_EQ("a", result);

  result.clear();
  EXPECT_TRUE(pg.parse("ab", result));
  EXPECT_EQ("ab", result);
}

// Verifies empty match behavior and semantic reduction on zero-length success.
TEST(PeglibPrioritizedChoicePackratTest, EmptyMatchActionRunsAndReturnsValue) {
  parser pg(R"(
    ROOT <- EMPTY
    EMPTY <- '' {
      return std::string("empty");
    }
  )");

  EXPECT_TRUE(static_cast<bool>(pg));

  std::string result = "unchanged";
  EXPECT_TRUE(pg.parse("", result));
  EXPECT_EQ("empty", result);

  result = "unchanged";
  EXPECT_TRUE(pg.parse("x", result, 0));
  EXPECT_EQ("empty", result);
}

// Verifies child token boundary propagation into semantic values when a rule
// wraps another rule and the wrapped rule sets the matched span.
TEST(PeglibPrioritizedChoicePackratTest, WrappedRuleReceivesCorrectMatchedToken) {
  parser pg(R"(
    ROOT <- WRAP
    WRAP <- INNER {
      return std::string(_sv.token());
    }
    INNER <- 'abc'
  )");

  EXPECT_TRUE(static_cast<bool>(pg));

  std::string result;
  EXPECT_TRUE(pg.parse("abc", result));
  EXPECT_EQ("abc", result);
}

// Verifies that a zero-length wrapped rule still exposes the correct token span
// to the action through the public SemanticValues API.
TEST(PeglibPrioritizedChoicePackratTest, WrappedEmptyRuleReceivesEmptyToken) {
  parser pg(R"(
    ROOT <- WRAP
    WRAP <- INNER {
      return std::string(_sv.token());
    }
    INNER <- ''
  )");

  EXPECT_TRUE(static_cast<bool>(pg));

  std::string result = "nonempty";
  EXPECT_TRUE(pg.parse("", result));
  EXPECT_EQ("", result);
}

// Verifies parse_error thrown from a semantic action is caught and parse fails.
TEST(PeglibPrioritizedChoicePackratTest, ParseErrorFromSemanticActionFails) {
  parser pg(R"(
    ROOT <- ITEM
    ITEM <- 'a' {
      throw peg::parse_error("semantic failure");
      return std::string("unreachable");
    }
  )");

  EXPECT_TRUE(static_cast<bool>(pg));

  std::string result = "initial";
  EXPECT_FALSE(pg.parse("a", result));
  EXPECT_EQ("initial", result);
}

// Verifies parse_error with null what() still fails parsing and does not alter output.
TEST(PeglibPrioritizedChoicePackratTest, ParseErrorWithoutMessageFailsCleanly) {
  parser pg(R"(
    ROOT <- ITEM
    ITEM <- 'a' {
      throw peg::parse_error(nullptr);
      return std::string("unreachable");
    }
  )");

  EXPECT_TRUE(static_cast<bool>(pg));

  std::string result = "initial";
  EXPECT_FALSE(pg.parse("a", result));
  EXPECT_EQ("initial", result);
}

// Verifies reduction from child semantic values in a wrapped rule using the
// child stack pushed/popped internally by the focal logic.
TEST(PeglibPrioritizedChoicePackratTest, ChildSemanticValuesAreReducedCorrectly) {
  parser pg(R"(
    ROOT <- PAIR
    PAIR <- NUM NUM {
      return std::any_cast<int>(_sv[0]) * 10 + std::any_cast<int>(_sv[1]);
    }
    NUM <- [0-9] {
      return static_cast<int>(_sv.token()[0] - '0');
    }
  )");

  EXPECT_TRUE(static_cast<bool>(pg));

  int result = 0;
  EXPECT_TRUE(pg.parse("42", result));
  EXPECT_EQ(42, result);
}

// Verifies failure path when inner rule does not match at all; semantic action
// must not run and output value should remain unchanged.
TEST(PeglibPrioritizedChoicePackratTest, NoSemanticActionOnMatchFailure) {
  parser pg(R"(
    ROOT <- ITEM
    ITEM <- 'a' {
      return std::string("matched");
    }
  )");

  EXPECT_TRUE(static_cast<bool>(pg));

  std::string result = "initial";
  EXPECT_FALSE(pg.parse("b", result));
  EXPECT_EQ("initial", result);
}

} // namespace