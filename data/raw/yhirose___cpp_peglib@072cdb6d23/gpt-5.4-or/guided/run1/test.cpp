#include <gtest/gtest.h>
#include <peglib.h>

#include <string>
#include <vector>

namespace {

using peg::SemanticValues;
using peg::any;
using peg::parser;

TEST(PeglibHolderParseCoreTest, SuccessfulRuleProducesSemanticValueAndTag) {
  parser pg(R"(
    ROOT <- ITEM
    ITEM <- 'a'
  )");

  ASSERT_TRUE(pg);

  bool action_called = false;
  pg["ITEM"] = [&](const SemanticValues& sv) -> int {
    action_called = true;
    EXPECT_EQ("a", sv.token_to_string());
    EXPECT_EQ("ITEM", sv.name());
    return 42;
  };

  any dt;
  bool ok = pg.parse("a", dt);

  ASSERT_TRUE(ok);
  ASSERT_TRUE(action_called);

  ASSERT_TRUE(dt.has_value());
  EXPECT_EQ(any_cast<int>(dt), 42);
}

TEST(PeglibHolderParseCoreTest, FailedRuleUsesCustomErrorMessage) {
  parser pg(R"(
    ROOT <- ITEM
    ITEM <- 'a'
  )");

  ASSERT_TRUE(pg);

  pg["ITEM"].set_error_message([](const char*, size_t, const SemanticValues&, any&) {
    return std::string("expected item");
  });

  std::string msg;
  size_t line = 0;
  size_t col = 0;
  const bool ok = pg.parse("b", msg, line, col);

  ASSERT_FALSE(ok);
  EXPECT_EQ("expected item", msg);
  EXPECT_EQ(1u, line);
  EXPECT_EQ(1u, col);
}

TEST(PeglibHolderParseCoreTest, ParseErrorThrownFromActionBecomesParseFailureWithMessage) {
  parser pg(R"(
    ROOT <- ITEM
    ITEM <- 'a'
  )");

  ASSERT_TRUE(pg);

  pg["ITEM"] = [&](const SemanticValues&) -> int {
    throw peg::parse_error("semantic failure");
  };

  std::string msg;
  size_t line = 0;
  size_t col = 0;
  const bool ok = pg.parse("a", msg, line, col);

  ASSERT_FALSE(ok);
  EXPECT_EQ("semantic failure", msg);
  EXPECT_EQ(1u, line);
  EXPECT_EQ(1u, col);
}

TEST(PeglibHolderParseCoreTest, RuleSpecificErrorMessageDoesNotOverrideActionParseErrorAtSamePosition) {
  parser pg(R"(
    ROOT <- ITEM
    ITEM <- 'a'
  )");

  ASSERT_TRUE(pg);

  pg["ITEM"] = [&](const SemanticValues&) -> int {
    throw peg::parse_error("action message");
  };
  pg["ITEM"].set_error_message([](const char*, size_t, const SemanticValues&, any&) {
    return std::string("rule message");
  });

  std::string msg;
  size_t line = 0;
  size_t col = 0;
  const bool ok = pg.parse("a", msg, line, col);

  ASSERT_FALSE(ok);
  EXPECT_EQ("action message", msg);
  EXPECT_EQ(1u, line);
  EXPECT_EQ(1u, col);
}

TEST(PeglibHolderParseCoreTest, IgnoreSemanticValuePreventsParentActionFromSeeingChildValue) {
  parser pg(R"(
    ROOT <- A B
    A <- 'a'
    B <- 'b'
  )");

  ASSERT_TRUE(pg);

  pg["A"].enable_ast();

  bool root_called = false;
  size_t seen_size = 999;
  std::vector<std::string> seen_tokens;
  pg["ROOT"] = [&](const SemanticValues& sv) -> int {
    root_called = true;
    seen_size = sv.size();
    for (size_t i = 0; i < sv.size(); i++) {
      seen_tokens.push_back(sv.token_to_string(i));
    }
    return static_cast<int>(sv.size());
  };

  any dt;
  ASSERT_TRUE(pg.parse("ab", dt));
  ASSERT_TRUE(root_called);

  EXPECT_EQ(1u, seen_size);
  ASSERT_EQ(1u, seen_tokens.size());
  EXPECT_EQ("b", seen_tokens[0]);
  EXPECT_EQ(1, any_cast<int>(dt));
}

TEST(PeglibHolderParseCoreTest, EmptyMatchActionRunsAndCanProduceValue) {
  parser pg(R"(
    ROOT <- EMPTY
    EMPTY <- ''
  )");

  ASSERT_TRUE(pg);

  bool called = false;
  pg["EMPTY"] = [&](const SemanticValues& sv) -> std::string {
    called = true;
    EXPECT_EQ("", sv.token_to_string());
    EXPECT_EQ("EMPTY", sv.name());
    return "ok";
  };

  any dt;
  const bool ok = pg.parse("", dt);

  ASSERT_TRUE(ok);
  ASSERT_TRUE(called);
  EXPECT_EQ("ok", any_cast<std::string>(dt));
}

TEST(PeglibHolderParseCoreTest, EnterAndLeaveHandlersAreCalledOnSuccessAndReceiveReducedValue) {
  parser pg(R"(
    ROOT <- ITEM
    ITEM <- 'a'
  )");

  ASSERT_TRUE(pg);

  bool enter_called = false;
  bool leave_called = false;
  size_t leave_len = static_cast<size_t>(-1);
  int leave_value = 0;

  pg["ITEM"].enter = [&](const char* s, size_t n, any&) {
    enter_called = true;
    ASSERT_NE(nullptr, s);
    EXPECT_EQ(1u, n);
  };

  pg["ITEM"] = [&](const SemanticValues&) -> int {
    return 7;
  };

  pg["ITEM"].leave = [&](const char* s, size_t n, size_t matched_len, const any& v, any&) {
    leave_called = true;
    ASSERT_NE(nullptr, s);
    EXPECT_EQ(1u, n);
    leave_len = matched_len;
    leave_value = any_cast<int>(v);
  };

  any dt;
  ASSERT_TRUE(pg.parse("a", dt));

  EXPECT_TRUE(enter_called);
  EXPECT_TRUE(leave_called);
  EXPECT_EQ(1u, leave_len);
  EXPECT_EQ(7, leave_value);
  EXPECT_EQ(7, any_cast<int>(dt));
}

TEST(PeglibHolderParseCoreTest, LeaveHandlerIsCalledEvenWhenActionThrowsParseError) {
  parser pg(R"(
    ROOT <- ITEM
    ITEM <- 'a'
  )");

  ASSERT_TRUE(pg);

  bool enter_called = false;
  bool leave_called = false;
  size_t leave_len = 12345;
  bool leave_value_has_value = true;

  pg["ITEM"].enter = [&](const char*, size_t, any&) {
    enter_called = true;
  };

  pg["ITEM"] = [&](const SemanticValues&) -> int {
    throw peg::parse_error("boom");
  };

  pg["ITEM"].leave = [&](const char*, size_t, size_t matched_len, const any& v, any&) {
    leave_called = true;
    leave_len = matched_len;
    leave_value_has_value = v.has_value();
  };

  std::string msg;
  size_t line = 0;
  size_t col = 0;
  const bool ok = pg.parse("a", msg, line, col);

  ASSERT_FALSE(ok);
  EXPECT_TRUE(enter_called);
  EXPECT_TRUE(leave_called);
  EXPECT_EQ(static_cast<size_t>(-1), leave_len);
  EXPECT_FALSE(leave_value_has_value);
  EXPECT_EQ("boom", msg);
  EXPECT_EQ(1u, line);
  EXPECT_EQ(1u, col);
}

}  // namespace