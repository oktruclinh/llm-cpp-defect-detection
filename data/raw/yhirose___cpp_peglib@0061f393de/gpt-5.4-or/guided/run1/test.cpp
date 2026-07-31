#include <gtest/gtest.h>
#include "peglib.h"

#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace {

using peg::Grammar;
using LogEntry = std::tuple<size_t, size_t, std::string>;

std::vector<LogEntry> MakeLogStorage() { return {}; }

peg::Log MakeLogger(std::vector<LogEntry>& logs) {
  return [&logs](size_t line, size_t col, const std::string& msg) {
    logs.emplace_back(line, col, msg);
  };
}

TEST(PeglibPerformCorePublicApiTest, ValidGrammarParsesAndProvidesStartRule) {
  const char* grammar_text =
      "Expr <- Term ('+' Term)*\n"
      "Term <- 'n'\n";

  std::vector<LogEntry> logs;
  std::string start;

  auto grammar = peg::parser::load_grammar(grammar_text, start, MakeLogger(logs));

  ASSERT_NE(grammar, nullptr);
  EXPECT_EQ(start, "Expr");
  EXPECT_TRUE(logs.empty());

  std::string out;
  EXPECT_TRUE(grammar->parse("n+n+n", out));
  EXPECT_FALSE(grammar->parse("x", out));
}

TEST(PeglibPerformCorePublicApiTest, SyntaxErrorReturnsNullAndReportsLocation) {
  const char* grammar_text =
      "A <- 'a'\n"
      "B <-\n";

  std::vector<LogEntry> logs;
  std::string start;

  auto grammar = peg::parser::load_grammar(grammar_text, start, MakeLogger(logs));

  EXPECT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());

  bool saw_syntax_related_message = false;
  bool saw_valid_position = false;

  for (const auto& entry : logs) {
    const auto line = std::get<0>(entry);
    const auto col = std::get<1>(entry);
    const auto& msg = std::get<2>(entry);

    if (line >= 1 && col >= 1) {
      saw_valid_position = true;
    }
    if (!msg.empty()) {
      saw_syntax_related_message = true;
    }
  }

  EXPECT_TRUE(saw_valid_position);
  EXPECT_TRUE(saw_syntax_related_message);
}

TEST(PeglibPerformCorePublicApiTest, DuplicateDefinitionReturnsNullAndLogsRuleName) {
  const char* grammar_text =
      "Start <- 'a'\n"
      "Start <- 'b'\n";

  std::vector<LogEntry> logs;
  std::string start;

  auto grammar = peg::parser::load_grammar(grammar_text, start, MakeLogger(logs));

  EXPECT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());

  bool found_duplicate_message = false;
  for (const auto& entry : logs) {
    const auto& msg = std::get<2>(entry);
    if (msg.find("'Start' is already defined.") != std::string::npos) {
      found_duplicate_message = true;
      EXPECT_GE(std::get<0>(entry), 1u);
      EXPECT_GE(std::get<1>(entry), 1u);
    }
  }

  EXPECT_TRUE(found_duplicate_message);
}

TEST(PeglibPerformCorePublicApiTest, MissingDefinitionReturnsNullAndLogsReferencedName) {
  const char* grammar_text =
      "Start <- Missing\n";

  std::vector<LogEntry> logs;
  std::string start;

  auto grammar = peg::parser::load_grammar(grammar_text, start, MakeLogger(logs));

  EXPECT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());

  bool found_missing_reference = false;
  for (const auto& entry : logs) {
    const auto& msg = std::get<2>(entry);
    if (msg.find("Missing") != std::string::npos) {
      found_missing_reference = true;
      EXPECT_GE(std::get<0>(entry), 1u);
      EXPECT_GE(std::get<1>(entry), 1u);
    }
  }

  EXPECT_TRUE(found_missing_reference);
}

TEST(PeglibPerformCorePublicApiTest, LeftRecursionReturnsNullAndLogsSpecificMessage) {
  const char* grammar_text =
      "Start <- Start 'a' / 'b'\n";

  std::vector<LogEntry> logs;
  std::string start;

  auto grammar = peg::parser::load_grammar(grammar_text, start, MakeLogger(logs));

  EXPECT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());

  bool found_left_recursion_message = false;
  for (const auto& entry : logs) {
    const auto& msg = std::get<2>(entry);
    if (msg.find("'Start' is left recursive.") != std::string::npos) {
      found_left_recursion_message = true;
    }
  }

  EXPECT_TRUE(found_left_recursion_message);
}

TEST(PeglibPerformCorePublicApiTest, InfiniteLoopReturnsNullAndLogsSpecificMessage) {
  const char* grammar_text =
      "Start <- ('')*\n";

  std::vector<LogEntry> logs;
  std::string start;

  auto grammar = peg::parser::load_grammar(grammar_text, start, MakeLogger(logs));

  EXPECT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());

  bool found_infinite_loop_message = false;
  for (const auto& entry : logs) {
    const auto& msg = std::get<2>(entry);
    if (msg.find("infinite loop is detected in 'Start'.") != std::string::npos) {
      found_infinite_loop_message = true;
    }
  }

  EXPECT_TRUE(found_infinite_loop_message);
}

TEST(PeglibPerformCorePublicApiTest, EmptyGrammarFailsToLoad) {
  const char* grammar_text = "";

  std::vector<LogEntry> logs;
  std::string start = "unchanged";

  auto grammar = peg::parser::load_grammar(grammar_text, start, MakeLogger(logs));

  EXPECT_EQ(grammar, nullptr);
  EXPECT_FALSE(logs.empty());
}

TEST(PeglibPerformCorePublicApiTest, UserProvidedRulesCanOverrideAndSetIgnoreSemanticPrefixWithoutBreakingParse) {
  const char* grammar_text =
      "Start <- Item\n"
      "Item <- 'a'\n";

  std::vector<LogEntry> logs;
  std::string start;

  peg::parser parser;
  peg::parser::rules rules;
  rules["~Item"] = peg::lit("b");

  auto grammar = parser.load_grammar(grammar_text, start, rules, MakeLogger(logs));

  ASSERT_NE(grammar, nullptr);
  EXPECT_EQ(start, "Start");
  EXPECT_TRUE(logs.empty());

  std::string out;
  EXPECT_TRUE(grammar->parse("b", out));
  EXPECT_FALSE(grammar->parse("a", out));
}

TEST(PeglibPerformCorePublicApiTest, IgnoreOperatorOnStartRuleIsRejected) {
  const char* grammar_text =
      "~Start <- 'a'\n";

  std::vector<LogEntry> logs;
  std::string start;

  auto grammar = peg::parser::load_grammar(grammar_text, start, MakeLogger(logs));

  EXPECT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());

  bool found_ignore_start_message = false;
  for (const auto& entry : logs) {
    const auto& msg = std::get<2>(entry);
    if (msg.find("Ignore operator cannot be applied to 'Start'.") !=
        std::string::npos) {
      found_ignore_start_message = true;
    }
  }

  EXPECT_TRUE(found_ignore_start_message);
}

TEST(PeglibPerformCorePublicApiTest, AutomaticWhitespaceAndWordDefinitionsAreApplied) {
  const char* grammar_text =
      "Start <- 'a' 'b'\n"
      "%whitespace <- [ \\t\\n]*\n"
      "%word <- [A-Za-z]+\n";

  std::vector<LogEntry> logs;
  std::string start;

  auto grammar = peg::parser::load_grammar(grammar_text, start, MakeLogger(logs));

  ASSERT_NE(grammar, nullptr);
  EXPECT_EQ(start, "Start");
  EXPECT_TRUE(logs.empty());

  std::string out;
  EXPECT_TRUE(grammar->parse("ab", out));
  EXPECT_TRUE(grammar->parse("a   b", out));
  EXPECT_TRUE(grammar->parse("a\t\nb", out));
  EXPECT_FALSE(grammar->parse("a c", out));
}

}  // namespace