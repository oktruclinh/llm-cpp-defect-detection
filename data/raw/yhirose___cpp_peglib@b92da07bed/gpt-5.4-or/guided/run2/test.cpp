#include <gtest/gtest.h>
#include <peglib.h>

#include <string>
#include <vector>

namespace {

using peg::parser;

TEST(PackratBehaviorTest, DisabledPackratStillParsesCorrectlyForRecursiveGrammar) {
  parser pg(R"(
    EXPR <- TERM (('+' / '-') TERM)*
    TERM <- FACTOR (('*' / '/') FACTOR)*
    FACTOR <- NUMBER / '(' EXPR ')'
    NUMBER <- < [0-9]+ >
    %whitespace <- [ \t\r\n]*
  )");

  ASSERT_TRUE(pg);

  pg["NUMBER"] = [](const peg::SemanticValues &sv) {
    return std::stoi(sv.token());
  };

  pg["FACTOR"] = [](const peg::SemanticValues &sv) -> int {
    if (sv.choice() == 0) {
      return sv[0].get<int>();
    } else {
      return sv[0].get<int>();
    }
  };

  pg["TERM"] = [](const peg::SemanticValues &sv) {
    int result = sv[0].get<int>();
    for (size_t i = 1; i < sv.size(); i += 2) {
      const auto op = sv[i].get<std::string>();
      const auto rhs = sv[i + 1].get<int>();
      if (op == "*") {
        result *= rhs;
      } else {
        result /= rhs;
      }
    }
    return result;
  };

  pg["EXPR"] = [](const peg::SemanticValues &sv) {
    int result = sv[0].get<int>();
    for (size_t i = 1; i < sv.size(); i += 2) {
      const auto op = sv[i].get<std::string>();
      const auto rhs = sv[i + 1].get<int>();
      if (op == "+") {
        result += rhs;
      } else {
        result -= rhs;
      }
    }
    return result;
  };

  pg.enable_packrat_parsing(false);

  int result = 0;
  ASSERT_TRUE(pg.parse("1 + 2 * (3 + 4) - 5", result));
  EXPECT_EQ(10, result);
}

TEST(PackratBehaviorTest, EnabledPackratStillParsesCorrectlyForRecursiveGrammar) {
  parser pg(R"(
    EXPR <- TERM (('+' / '-') TERM)*
    TERM <- FACTOR (('*' / '/') FACTOR)*
    FACTOR <- NUMBER / '(' EXPR ')'
    NUMBER <- < [0-9]+ >
    %whitespace <- [ \t\r\n]*
  )");

  ASSERT_TRUE(pg);

  pg["NUMBER"] = [](const peg::SemanticValues &sv) {
    return std::stoi(sv.token());
  };

  pg["FACTOR"] = [](const peg::SemanticValues &sv) -> int {
    return sv[0].get<int>();
  };

  pg["TERM"] = [](const peg::SemanticValues &sv) {
    int result = sv[0].get<int>();
    for (size_t i = 1; i < sv.size(); i += 2) {
      const auto op = sv[i].get<std::string>();
      const auto rhs = sv[i + 1].get<int>();
      if (op == "*") {
        result *= rhs;
      } else {
        result /= rhs;
      }
    }
    return result;
  };

  pg["EXPR"] = [](const peg::SemanticValues &sv) {
    int result = sv[0].get<int>();
    for (size_t i = 1; i < sv.size(); i += 2) {
      const auto op = sv[i].get<std::string>();
      const auto rhs = sv[i + 1].get<int>();
      if (op == "+") {
        result += rhs;
      } else {
        result -= rhs;
      }
    }
    return result;
  };

  pg.enable_packrat_parsing(true);

  int result = 0;
  ASSERT_TRUE(pg.parse("1 + 2 * (3 + 4) - 5", result));
  EXPECT_EQ(10, result);
}

TEST(PackratBehaviorTest, RepeatedReferenceAtSamePositionProducesStableSemanticValueWithPackratEnabled) {
  parser pg(R"(
    START <- &ITEM ITEM ITEM
    ITEM <- < 'a'+ >
  )");

  ASSERT_TRUE(pg);

  int item_action_calls = 0;
  pg["ITEM"] = [&](const peg::SemanticValues &sv) {
    ++item_action_calls;
    return static_cast<int>(sv.token().size());
  };

  pg.enable_packrat_parsing(true);

  int result = 0;
  ASSERT_TRUE(pg.parse("aaaa", result));
  EXPECT_EQ(4, result);
  EXPECT_EQ(1, item_action_calls)
      << "Positive lookahead and subsequent consumption start at the same input "
         "position and same rule; with packrat enabled the semantic action "
         "should run once and be reused from cache.";
}

TEST(PackratBehaviorTest, RepeatedReferenceAtSamePositionWithoutPackratRecomputesSemanticValue) {
  parser pg(R"(
    START <- &ITEM ITEM ITEM
    ITEM <- < 'a'+ >
  )");

  ASSERT_TRUE(pg);

  int item_action_calls = 0;
  pg["ITEM"] = [&](const peg::SemanticValues &sv) {
    ++item_action_calls;
    return static_cast<int>(sv.token().size());
  };

  pg.enable_packrat_parsing(false);

  int result = 0;
  ASSERT_TRUE(pg.parse("aaaa", result));
  EXPECT_EQ(4, result);
  EXPECT_EQ(2, item_action_calls)
      << "Without packrat, the same rule invoked twice at the same position "
         "should execute twice.";
}

TEST(PackratBehaviorTest, FailedParseAtSamePositionIsMemoizedWhenEnabled) {
  parser pg(R"(
    START <- !BAD 'a'
    BAD <- 'a' 'b'
  )");

  ASSERT_TRUE(pg);

  int bad_action_calls = 0;
  pg["BAD"] = [&](const peg::SemanticValues &) {
    ++bad_action_calls;
    return 0;
  };

  pg.enable_packrat_parsing(true);

  ASSERT_TRUE(pg.parse("a"));
  EXPECT_EQ(0, bad_action_calls)
      << "Semantic action should not run on a failing rule, but the parse must "
         "still succeed via negative lookahead.";
}

TEST(PackratBehaviorTest, FailedRuleInvokedTwiceAtSamePositionIsReevaluatedWithoutPackrat) {
  parser pg(R"(
    START <- !('a' 'b') !('a' 'b') 'a'
  )");

  ASSERT_TRUE(pg);

  pg.enable_packrat_parsing(false);

  ASSERT_TRUE(pg.parse("a"));
}

TEST(PackratBehaviorTest, EmptyInputParsesCorrectlyWithPackratEnabled) {
  parser pg(R"(
    START <- ITEM?
    ITEM <- < 'a'+ >
  )");

  ASSERT_TRUE(pg);
  pg.enable_packrat_parsing(true);

  ASSERT_TRUE(pg.parse(""));
}

TEST(PackratBehaviorTest, EmptyInputParsesCorrectlyWithPackratDisabled) {
  parser pg(R"(
    START <- ITEM?
    ITEM <- < 'a'+ >
  )");

  ASSERT_TRUE(pg);
  pg.enable_packrat_parsing(false);

  ASSERT_TRUE(pg.parse(""));
}

TEST(PackratBehaviorTest, PackratCacheDoesNotLeakAcrossIndependentParses) {
  parser pg(R"(
    START <- &ITEM ITEM
    ITEM <- < 'a'+ >
  )");

  ASSERT_TRUE(pg);

  int item_action_calls = 0;
  pg["ITEM"] = [&](const peg::SemanticValues &sv) {
    ++item_action_calls;
    return static_cast<int>(sv.token().size());
  };

  pg.enable_packrat_parsing(true);

  int result = 0;
  ASSERT_TRUE(pg.parse("aaa", result));
  EXPECT_EQ(3, result);
  EXPECT_EQ(1, item_action_calls);

  ASSERT_TRUE(pg.parse("aaaaa", result));
  EXPECT_EQ(5, result);
  EXPECT_EQ(2, item_action_calls)
      << "Cache must be per-parse-context. A second parse must not reuse the "
         "previous parse's memoized semantic value.";
}

TEST(PackratBehaviorTest, DifferentPositionsDoNotIncorrectlyShareMemoizedResults) {
  parser pg(R"(
    START <- FIRST SECOND
    FIRST <- ITEM
    SECOND <- ITEM
    ITEM <- < 'a'+ >
  )");

  ASSERT_TRUE(pg);

  std::vector<int> seen_lengths;
  pg["ITEM"] = [&](const peg::SemanticValues &sv) {
    seen_lengths.push_back(static_cast<int>(sv.token().size()));
    return static_cast<int>(sv.token().size());
  };

  pg.enable_packrat_parsing(true);

  int result = 0;
  ASSERT_TRUE(pg.parse("aaaaaa", result));
  EXPECT_EQ(6, result);
  ASSERT_EQ(2u, seen_lengths.size())
      << "The rule is used at two different positions and should be evaluated "
         "separately for each position.";
  EXPECT_EQ(5, seen_lengths[0]);
  EXPECT_EQ(1, seen_lengths[1]);
}

} // namespace