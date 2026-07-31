#include <gtest/gtest.h>
#include "peglib.h"

#include <string>
#include <vector>

namespace {

using peg::SemanticValues;
using peg::parser;

TEST(PeglibCaptureScopeTest, SuccessfulCaptureProducesSemanticValueForToken) {
  parser pg(R"(
    ROOT   <- ITEM
    ITEM   <- < 'a'+ >
  )");

  ASSERT_TRUE(pg);

  std::vector<std::string> seen;

  pg["ITEM"] = [&](const SemanticValues& sv) {
    seen.push_back(sv.token());
    return sv.token();
  };

  std::string out;
  ASSERT_TRUE(pg.parse("aaa", out));
  ASSERT_EQ("aaa", out);

  ASSERT_EQ(1u, seen.size());
  EXPECT_EQ("aaa", seen[0]);
}

TEST(PeglibCaptureScopeTest, EmptyCaptureIsSuccessfulAndTokenIsEmptyString) {
  parser pg(R"(
    ROOT   <- ITEM
    ITEM   <- < '' >
  )");

  ASSERT_TRUE(pg);

  std::vector<std::string> seen;

  pg["ITEM"] = [&](const SemanticValues& sv) {
    seen.push_back(sv.token());
    return sv.token();
  };

  std::string out = "not-set";
  ASSERT_TRUE(pg.parse("", out));
  ASSERT_EQ("", out);

  ASSERT_EQ(1u, seen.size());
  EXPECT_EQ("", seen[0]);
}

TEST(PeglibCaptureScopeTest, FailedAlternativeDoesNotLeakSemanticActionFromFailedBranch) {
  parser pg(R"(
    ROOT   <- FIRST / SECOND
    FIRST  <- < 'a' > 'b'
    SECOND <- < 'a' >
  )");

  ASSERT_TRUE(pg);

  int first_calls = 0;
  int second_calls = 0;

  pg["FIRST"] = [&](const SemanticValues& sv) {
    ++first_calls;
    return sv.token();
  };
  pg["SECOND"] = [&](const SemanticValues& sv) {
    ++second_calls;
    return sv.token();
  };

  std::string out;
  ASSERT_TRUE(pg.parse("a", out));
  EXPECT_EQ("a", out);

  EXPECT_EQ(0, first_calls) << "Action for failing branch must not run.";
  EXPECT_EQ(1, second_calls) << "Action for succeeding branch must run exactly once.";
}

TEST(PeglibCaptureScopeTest, SemanticActionExceptionTurnsParseIntoFailureWithMessage) {
  parser pg(R"(
    ROOT <- ITEM
    ITEM <- < 'a'+ >
  )");

  ASSERT_TRUE(pg);

  pg["ITEM"] = [&](const SemanticValues&) -> std::string {
    throw peg::parse_error("semantic failure");
  };

  std::string out = "unchanged";
  EXPECT_FALSE(pg.parse("aaa", out));

  std::string log = pg.log;
  EXPECT_NE(std::string::npos, log.find("semantic failure"))
      << "parse_error message should be surfaced in parser log.";
}

TEST(PeglibCaptureScopeTest, BeforeAndAfterPredicatesRunOnSuccess) {
  parser pg(R"(
    ROOT <- ITEM
    ITEM <- < 'a'+ >
  )");

  ASSERT_TRUE(pg);

  std::vector<std::string> events;

  pg["ITEM"].enter = [&](const char*, size_t, SemanticValues&, std::any&) {
    events.push_back("enter");
  };
  pg["ITEM"].leave = [&](const char*, size_t, SemanticValues&, std::any&) {
    events.push_back("leave");
  };
  pg["ITEM"] = [&](const SemanticValues& sv) {
    events.push_back("action:" + sv.token());
    return sv.token();
  };

  std::string out;
  ASSERT_TRUE(pg.parse("aa", out));
  EXPECT_EQ("aa", out);

  ASSERT_EQ(3u, events.size());
  EXPECT_EQ("enter", events[0]);
  EXPECT_EQ("action:aa", events[1]);
  EXPECT_EQ("leave", events[2]);
}

TEST(PeglibCaptureScopeTest, BeforeAndAfterPredicatesRunEvenWhenInnerParseFails) {
  parser pg(R"(
    ROOT <- ITEM
    ITEM <- < 'a' > 'b'
  )");

  ASSERT_TRUE(pg);

  std::vector<std::string> events;
  int action_calls = 0;

  pg["ITEM"].enter = [&](const char*, size_t, SemanticValues&, std::any&) {
    events.push_back("enter");
  };
  pg["ITEM"].leave = [&](const char*, size_t, SemanticValues&, std::any&) {
    events.push_back("leave");
  };
  pg["ITEM"] = [&](const SemanticValues&) {
    ++action_calls;
    events.push_back("action");
    return std::string("should-not-happen");
  };

  std::string out;
  EXPECT_FALSE(pg.parse("a", out));

  EXPECT_EQ(0, action_calls) << "Semantic action must not run when parse fails.";
  ASSERT_EQ(2u, events.size());
  EXPECT_EQ("enter", events[0]);
  EXPECT_EQ("leave", events[1]);
}

TEST(PeglibCaptureScopeTest, NestedCapturesUseInnermostTokenForInnerRuleAndOuterTokenForOuterRule) {
  parser pg(R"(
    ROOT  <- OUTER
    OUTER <- < INNER 'b' >
    INNER <- < 'a' >
  )");

  ASSERT_TRUE(pg);

  std::string inner_token;
  std::string outer_token;

  pg["INNER"] = [&](const SemanticValues& sv) {
    inner_token = sv.token();
    return sv.token();
  };

  pg["OUTER"] = [&](const SemanticValues& sv) {
    outer_token = sv.token();
    return sv.token();
  };

  std::string out;
  ASSERT_TRUE(pg.parse("ab", out));
  EXPECT_EQ("ab", out);

  EXPECT_EQ("a", inner_token);
  EXPECT_EQ("ab", outer_token);
}

TEST(PeglibCaptureScopeTest, RepetitionWithActionInvokesActionForEachSuccessfulIterationOnly) {
  parser pg(R"(
    ROOT <- ITEM*
    ITEM <- < 'a' >
  )");

  ASSERT_TRUE(pg);

  std::vector<std::string> tokens;

  pg["ITEM"] = [&](const SemanticValues& sv) {
    tokens.push_back(sv.token());
    return sv.token();
  };

  std::vector<std::string> out;
  ASSERT_TRUE(pg.parse("aaa", out));

  ASSERT_EQ(3u, tokens.size());
  EXPECT_EQ("a", tokens[0]);
  EXPECT_EQ("a", tokens[1]);
  EXPECT_EQ("a", tokens[2]);

  ASSERT_EQ(3u, out.size());
  EXPECT_EQ("a", out[0]);
  EXPECT_EQ("a", out[1]);
  EXPECT_EQ("a", out[2]);
}

}  // namespace