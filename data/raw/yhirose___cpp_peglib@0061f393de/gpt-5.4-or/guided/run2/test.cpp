#include <gtest/gtest.h>
#include "peglib.h"

#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace {

using LogEntry = std::tuple<size_t, size_t, std::string>;

struct LogCollector {
  std::vector<LogEntry> entries;

  void operator()(size_t line, size_t col, const std::string& msg) {
    entries.emplace_back(line, col, msg);
  }
};

TEST(PegParserGeneratorTest, BuildsGrammarFromValidDefinitionAndParsesInput) {
  peg::parser pg;

  const char *grammar = R"(
    START <- 'a' 'b'
  )";

  LogCollector log;
  ASSERT_TRUE(pg.load_grammar(grammar, log));

  EXPECT_TRUE(log.entries.empty());

  std::string out;
  EXPECT_TRUE(pg.parse("ab", out));
  EXPECT_FALSE(pg.parse("a", out));
  EXPECT_FALSE(pg.parse("abc", out));
}

TEST(PegParserGeneratorTest, EmptyGrammarFailsWithSyntaxError) {
  peg::parser pg;
  LogCollector log;

  EXPECT_FALSE(pg.load_grammar("", log));
  ASSERT_FALSE(log.entries.empty());

  bool found_syntax_error = false;
  for (const auto& e : log.entries) {
    if (std::get<2>(e) == "syntax error") {
      found_syntax_error = true;
      EXPECT_GE(std::get<0>(e), 1u);
      EXPECT_GE(std::get<1>(e), 1u);
    }
  }
  EXPECT_TRUE(found_syntax_error);
}

TEST(PegParserGeneratorTest, InvalidGrammarReportsSyntaxError) {
  peg::parser pg;
  LogCollector log;

  EXPECT_FALSE(pg.load_grammar("START <- ", log));
  ASSERT_FALSE(log.entries.empty());

  bool found = false;
  for (const auto& e : log.entries) {
    if (std::get<2>(e) == "syntax error") {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found);
}

TEST(PegParserGeneratorTest, DuplicateRuleDefinitionIsRejected) {
  peg::parser pg;
  LogCollector log;

  const char *grammar = R"(
    A <- 'a'
    A <- 'b'
  )";

  EXPECT_FALSE(pg.load_grammar(grammar, log));
  ASSERT_FALSE(log.entries.empty());

  bool found_duplicate = false;
  for (const auto& e : log.entries) {
    if (std::get<2>(e) == "'A' is already defined.") {
      found_duplicate = true;
      EXPECT_GE(std::get<0>(e), 1u);
      EXPECT_GE(std::get<1>(e), 1u);
    }
  }
  EXPECT_TRUE(found_duplicate);
}

TEST(PegParserGeneratorTest, MissingReferencedDefinitionIsRejected) {
  peg::parser pg;
  LogCollector log;

  const char *grammar = R"(
    START <- Missing
  )";

  EXPECT_FALSE(pg.load_grammar(grammar, log));
  ASSERT_FALSE(log.entries.empty());

  bool found_missing_reference = false;
  for (const auto& e : log.entries) {
    const auto& msg = std::get<2>(e);
    if (msg.find("Missing") != std::string::npos) {
      found_missing_reference = true;
      EXPECT_NE(msg.find("not defined"), std::string::npos);
    }
  }
  EXPECT_TRUE(found_missing_reference);
}

TEST(PegParserGeneratorTest, LeftRecursiveGrammarIsRejected) {
  peg::parser pg;
  LogCollector log;

  const char *grammar = R"(
    A <- A / 'a'
  )";

  EXPECT_FALSE(pg.load_grammar(grammar, log));
  ASSERT_FALSE(log.entries.empty());

  bool found_left_recursion = false;
  for (const auto& e : log.entries) {
    if (std::get<2>(e) == "'A' is left recursive.") {
      found_left_recursion = true;
      EXPECT_GE(std::get<0>(e), 1u);
      EXPECT_GE(std::get<1>(e), 1u);
    }
  }
  EXPECT_TRUE(found_left_recursion);
}

TEST(PegParserGeneratorTest, InfiniteLoopGrammarIsRejected) {
  peg::parser pg;
  LogCollector log;

  const char *grammar = R"(
    A <- ''
  )";

  EXPECT_FALSE(pg.load_grammar(grammar, log));
  ASSERT_FALSE(log.entries.empty());

  bool found_infinite_loop = false;
  for (const auto& e : log.entries) {
    if (std::get<2>(e) == "infinite loop is detected in 'A'.") {
      found_infinite_loop = true;
      EXPECT_GE(std::get<0>(e), 1u);
      EXPECT_GE(std::get<1>(e), 1u);
    }
  }
  EXPECT_TRUE(found_infinite_loop);
}

TEST(PegParserGeneratorTest, StartRuleCannotUseIgnoreOperator) {
  peg::parser pg;
  LogCollector log;

  const char *grammar = R"(
    ~A <- 'a'
  )";

  EXPECT_FALSE(pg.load_grammar(grammar, log));
  ASSERT_FALSE(log.entries.empty());

  bool found_ignore_error = false;
  for (const auto& e : log.entries) {
    if (std::get<2>(e) == "Ignore operator cannot be applied to 'A'.") {
      found_ignore_error = true;
      EXPECT_GE(std::get<0>(e), 1u);
      EXPECT_GE(std::get<1>(e), 1u);
    }
  }
  EXPECT_TRUE(found_ignore_error);
}

TEST(PegParserGeneratorTest, AutomaticWhitespaceSkippingWorksWhenWhitespaceRuleExists) {
  peg::parser pg;
  LogCollector log;

  const char *grammar = R"(
    %whitespace <- [ \t\r\n]*
    START <- 'a' 'b'
  )";

  ASSERT_TRUE(pg.load_grammar(grammar, log));
  EXPECT_TRUE(log.entries.empty());

  std::string out;
  EXPECT_TRUE(pg.parse("ab", out));
  EXPECT_TRUE(pg.parse("a b", out));
  EXPECT_TRUE(pg.parse("a \n\t b", out));
  EXPECT_FALSE(pg.parse("a  c", out));
}

TEST(PegParserGeneratorTest, NoAutomaticWhitespaceSkippingWithoutWhitespaceRule) {
  peg::parser pg;
  LogCollector log;

  const char *grammar = R"(
    START <- 'a' 'b'
  )";

  ASSERT_TRUE(pg.load_grammar(grammar, log));
  EXPECT_TRUE(log.entries.empty());

  std::string out;
  EXPECT_TRUE(pg.parse("ab", out));
  EXPECT_FALSE(pg.parse("a b", out));
}

TEST(PegParserGeneratorTest, UserProvidedRuleCanOverrideGrammarRule) {
  peg::parser pg;
  LogCollector log;

  const char *grammar = R"(
    A <- 'a'
  )";

  peg::parser::rule rule = peg::seq(peg::lit("b"));
  pg["A"] <= rule;

  ASSERT_TRUE(pg.load_grammar(grammar, log));
  EXPECT_TRUE(log.entries.empty());

  std::string out;
  EXPECT_FALSE(pg.parse("a", out));
  EXPECT_TRUE(pg.parse("b", out));
}

TEST(PegParserGeneratorTest, UserProvidedIgnoredStartRuleIsRejected) {
  peg::parser pg;
  LogCollector log;

  const char *grammar = R"(
    A <- 'a'
  )";

  peg::parser::rule rule = peg::seq(peg::lit("a"));
  pg["~A"] <= rule;

  EXPECT_FALSE(pg.load_grammar(grammar, log));
  ASSERT_FALSE(log.entries.empty());

  bool found_ignore_error = false;
  for (const auto& e : log.entries) {
    if (std::get<2>(e) == "Ignore operator cannot be applied to 'A'.") {
      found_ignore_error = true;
      break;
    }
  }
  EXPECT_TRUE(found_ignore_error);
}

TEST(PegParserGeneratorTest, LoadGrammarFromBufferWithExplicitSizeHandlesEmbeddedNul) {
  peg::parser pg;
  LogCollector log;

  const std::string grammar = std::string("START <- 'a'\0TRAILING", 21);
  ASSERT_TRUE(pg.load_grammar(grammar.c_str(), 12, log));
  EXPECT_TRUE(log.entries.empty());

  std::string out;
  EXPECT_TRUE(pg.parse("a", out));
  EXPECT_FALSE(pg.parse("b", out));
}

} // namespace