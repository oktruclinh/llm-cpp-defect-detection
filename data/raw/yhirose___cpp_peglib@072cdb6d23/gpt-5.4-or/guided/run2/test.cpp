#include <gtest/gtest.h>
#include "peglib.h"

#include <string>
#include <vector>

namespace peg = peglib;

TEST(PeglibHolderParseCoreTest, SuccessfulRulePushesSemanticValueAndActionResult) {
  peg::parser parser(R"(
    Start <- Word
    Word  <- < [a-z]+ > { return std::string(sv.token()); }
  )");

  ASSERT_TRUE(parser) << parser.log;

  std::vector<std::string> out;
  parser["Start"] = [&](const peg::SemanticValues& sv) {
    out.push_back(sv[0].get<std::string>());
    return sv[0].get<std::string>();
  };

  std::string result;
  EXPECT_TRUE(parser.parse("hello", result));
  ASSERT_EQ(out.size(), 1u);
  EXPECT_EQ(out[0], "hello");
  EXPECT_EQ(result, "hello");
}

TEST(PeglibHolderParseCoreTest, IgnoreSemanticValuePreventsPropagationToParent) {
  peg::parser parser(R"(
    Start <- Ign 'b'
    Ign   ~ < 'a' > { return std::string("kept-locally"); }
  )");

  ASSERT_TRUE(parser) << parser.log;

  size_t semantic_count_seen_by_start = 999;
  std::string result;
  parser["Start"] = [&](const peg::SemanticValues& sv) {
    semantic_count_seen_by_start = sv.size();
    return std::string("ok");
  };

  EXPECT_TRUE(parser.parse("ab", result));
  EXPECT_EQ(semantic_count_seen_by_start, 1u)
      << "Ignored rule must not contribute a semantic value to its parent.";
  EXPECT_EQ(result, "ok");
}

TEST(PeglibHolderParseCoreTest, FailedRuleUsesCustomErrorMessage) {
  peg::parser parser(R"(
    Start <- Number
    Number "number expected" <- [0-9]+
  )");

  ASSERT_TRUE(parser) << parser.log;

  std::string result;
  EXPECT_FALSE(parser.parse("abc", result));
  EXPECT_NE(parser.log.find("number expected"), std::string::npos)
      << "Custom rule error message should be reported on failure.";
}

TEST(PeglibHolderParseCoreTest, ActionParseErrorMessageOverridesAndCausesFailure) {
  peg::parser parser(R"(
    Start <- Bad
    Bad <- < [a-z]+ > {
      throw peglib::parse_error("action failed");
      return std::string();
    }
  )");

  ASSERT_TRUE(parser) << parser.log;

  std::string result;
  EXPECT_FALSE(parser.parse("abc", result));
  EXPECT_NE(parser.log.find("action failed"), std::string::npos)
      << "parse_error thrown by action should become the parse failure message.";
}

TEST(PeglibHolderParseCoreTest, EmptyMatchCanSucceedAndProduceSemanticValue) {
  peg::parser parser(R"(
    Start <- Empty
    Empty <- '' { return 42; }
  )");

  ASSERT_TRUE(parser) << parser.log;

  int result = 0;
  EXPECT_TRUE(parser.parse("", result));
  EXPECT_EQ(result, 42);
}

TEST(PeglibHolderParseCoreTest, EnterAndLeaveCallbacksAreInvokedForRule) {
  peg::parser parser(R"(
    Start <- Item
    Item  <- 'x'
  )");

  ASSERT_TRUE(parser) << parser.log;

  int enter_count = 0;
  int leave_count = 0;
  std::vector<size_t> leave_lengths;
  std::vector<bool> leave_success;

  parser["Item"].enter = [&](const char* /*s*/, size_t n, peg::any& /*dt*/) {
    ++enter_count;
    EXPECT_EQ(n, 1u);
  };

  parser["Item"].leave =
      [&](const char* /*s*/, size_t /*n*/, size_t matched_len, peg::any& val, peg::any& /*dt*/) {
        ++leave_count;
        leave_lengths.push_back(matched_len);
        leave_success.push_back(matched_len != static_cast<size_t>(-1));
        (void)val;
      };

  std::string result;
  EXPECT_TRUE(parser.parse("x", result));
  EXPECT_EQ(enter_count, 1);
  EXPECT_EQ(leave_count, 1);
  ASSERT_EQ(leave_lengths.size(), 1u);
  EXPECT_EQ(leave_lengths[0], 1u);
  EXPECT_TRUE(leave_success[0]);
}

TEST(PeglibHolderParseCoreTest, LeaveCallbackRunsOnFailureAndReceivesFailureLength) {
  peg::parser parser(R"(
    Start <- Item
    Item  <- 'x'
  )");

  ASSERT_TRUE(parser) << parser.log;

  int enter_count = 0;
  int leave_count = 0;
  size_t observed_leave_len = 0;

  parser["Item"].enter = [&](const char*, size_t, peg::any&) {
    ++enter_count;
  };

  parser["Item"].leave =
      [&](const char*, size_t, size_t matched_len, peg::any&, peg::any&) {
        ++leave_count;
        observed_leave_len = matched_len;
      };

  std::string result;
  EXPECT_FALSE(parser.parse("y", result));
  EXPECT_EQ(enter_count, 1);
  EXPECT_EQ(leave_count, 1);
  EXPECT_EQ(observed_leave_len, static_cast<size_t>(-1))
      << "Leave callback should observe failure length for unsuccessful parses.";
}

TEST(PeglibHolderParseCoreTest, ParseErrorWithoutMessageStillFails) {
  peg::parser parser(R"(
    Start <- Bad
    Bad <- 'a' {
      throw peglib::parse_error();
      return 0;
    }
  )");

  ASSERT_TRUE(parser) << parser.log;

  int result = 0;
  EXPECT_FALSE(parser.parse("a", result));
}

TEST(PeglibHolderParseCoreTest, PrioritizedChoiceSelectsCorrectAlternativeAndActionResult) {
  peg::parser parser(R"(
    Start <- Item
    Item  <- First / Second
    First <- 'a' { return 1; }
    Second <- 'b' { return 2; }
  )");

  ASSERT_TRUE(parser) << parser.log;

  int result = 0;
  EXPECT_TRUE(parser.parse("b", result));
  EXPECT_EQ(result, 2);

  EXPECT_TRUE(parser.parse("a", result));
  EXPECT_EQ(result, 1);
}

TEST(PeglibHolderParseCoreTest, MacroRuleParsesSuccessfullyThroughPublicApi) {
  peg::parser parser(R"(
    Start <- Pair('a', 'b')
    Pair(x, y) <- x y
  )");

  ASSERT_TRUE(parser) << parser.log;

  std::string result;
  EXPECT_TRUE(parser.parse("ab", result));
  EXPECT_FALSE(parser.parse("aa", result));
}