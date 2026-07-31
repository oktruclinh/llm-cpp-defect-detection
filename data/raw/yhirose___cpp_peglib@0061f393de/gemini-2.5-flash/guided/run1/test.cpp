```cpp
#include <gtest/gtest.h>
#include <peglib.h>

#include <memory>
#include <string>
#include <vector>

// Helper function to capture log messages
struct LogEntry {
  size_t line;
  size_t column;
  std::string message;
};

using LogEntries = std::vector<LogEntry>;

static peg::Log create_capturing_log(LogEntries& logs) {
  return [&](size_t line, size_t col, const std::string& msg) {
    logs.push_back({line, col, msg});
  };
}

TEST(PeglibPerformCoreTest, ValidGrammar) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = "ROOT = 'a' 'b'";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_NE(grammar, nullptr);
  ASSERT_TRUE(logs.empty());
  ASSERT_EQ(start_rule_name, "ROOT"); // Default start rule
  ASSERT_TRUE(grammar->count("ROOT"));
}

TEST(PeglibPerformCoreTest, InvalidGrammarSyntax) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = "ROOT = 'a' ("; // Unclosed parenthesis
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());
  ASSERT_EQ(logs.size(), 1);
  ASSERT_EQ(logs[0].line, 1);
  ASSERT_EQ(logs[0].column, 13);
  ASSERT_EQ(logs[0].message, "syntax error");
}

TEST(PeglibPerformCoreTest, DuplicatedRuleDefinition) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = R"(
    A = 'a'
    A = 'b'
  )";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());
  ASSERT_EQ(logs.size(), 1);
  ASSERT_EQ(logs[0].line, 3);
  ASSERT_EQ(logs[0].column, 5);
  ASSERT_EQ(logs[0].message, "'A' is already defined.");
}

TEST(PeglibPerformCoreTest, MissingRuleDefinition) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = "ROOT = A\n";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());
  ASSERT_EQ(logs.size(), 1);
  ASSERT_EQ(logs[0].line, 1);
  ASSERT_EQ(logs[0].column, 8);
  ASSERT_EQ(logs[0].message, "'A' is not defined.");
}

TEST(PeglibPerformCoreTest, LeftRecursionDetection) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = "A = A 'a'";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());
  ASSERT_EQ(logs.size(), 1);
  ASSERT_EQ(logs[0].line, 1);
  ASSERT_EQ(logs[0].column, 1);
  ASSERT_EQ(logs[0].message, "'A' is left recursive.");
}

TEST(PeglibPerformCoreTest, IndirectLeftRecursionDetection) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = R"(
    A = B 'a'
    B = A 'b'
  )";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());
  ASSERT_EQ(logs.size(), 1);
  ASSERT_EQ(logs[0].line, 2);
  ASSERT_EQ(logs[0].column, 5);
  ASSERT_EQ(logs[0].message, "'A' is left recursive.");
}

TEST(PeglibPerformCoreTest, InfiniteLoopDetection) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = "A = '' A"; // A rule that can match empty string and then call itself
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());
  ASSERT_EQ(logs.size(), 1);
  ASSERT_EQ(logs[0].line, 1);
  ASSERT_EQ(logs[0].column, 1);
  ASSERT_EQ(logs[0].message, "infinite loop is detected in 'A'.");
}

TEST(PeglibPerformCoreTest, InfiniteLoopDetectionWithOptional) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = "A = B?\nB = A";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());
  ASSERT_EQ(logs.size(), 1);
  ASSERT_EQ(logs[0].line, 1);
  ASSERT_EQ(logs[0].column, 1);
  ASSERT_EQ(logs[0].message, "infinite loop is detected in 'A'.");
}

TEST(PeglibPerformCoreTest, StartRuleWithIgnoreOperator) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = "ROOT = ~'a'";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());
  ASSERT_EQ(logs.size(), 1);
  ASSERT_EQ(logs[0].line, 1);
  ASSERT_EQ(logs[0].column, 8);
  ASSERT_EQ(logs[0].message, "Ignore operator cannot be applied to 'ROOT'.");
}

TEST(PeglibPerformCoreTest, UserProvidedRules) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = "ROOT = 'a'";
  std::string start_rule_name;
  peg::Rules rules;
  rules["USER_RULE"] = peg::create_literal_expression("b");
  rules["~IGNORED_RULE"] = peg::create_literal_expression("c");

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_NE(grammar, nullptr);
  ASSERT_TRUE(logs.empty());
  ASSERT_TRUE(grammar->count("ROOT"));
  ASSERT_TRUE(grammar->count("USER_RULE"));
  ASSERT_TRUE(grammar->count("IGNORED_RULE"));

  // Check if ignoreSemanticValue is set correctly
  ASSERT_FALSE((*grammar)["USER_RULE"].ignoreSemanticValue);
  ASSERT_TRUE((*grammar)["IGNORED_RULE"].ignoreSemanticValue);
}

TEST(PeglibPerformCoreTest, UserProvidedRulesEmptyName) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = "ROOT = 'a'";
  std::string start_rule_name;
  peg::Rules rules;
  rules[""] = peg::create_literal_expression("b"); // Empty rule name

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_NE(grammar, nullptr); // Should still parse the main grammar
  ASSERT_TRUE(logs.empty());   // Empty rule name should be ignored without error
  ASSERT_TRUE(grammar->count("ROOT"));
  ASSERT_FALSE(grammar->count("")); // Ensure no empty rule was added
}

TEST(PeglibPerformCoreTest, WhitespaceSkippingEnabled) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = R"(
    %whitespace = [ \t\r\n]+
    ROOT = 'a' 'b'
  )";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_NE(grammar, nullptr);
  ASSERT_TRUE(logs.empty());
  ASSERT_TRUE(grammar->count("ROOT"));
  ASSERT_TRUE(grammar->count(peg::WHITESPACE_DEFINITION_NAME));

  // Verify that the whitespace operator is set on the start rule
  auto& start_rule = (*grammar)["ROOT"];
  ASSERT_NE(start_rule.whitespaceOpe, nullptr);
}

TEST(PeglibPerformCoreTest, WordExpressionEnabled) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = R"(
    %word = [a-zA-Z_][a-zA-Z_0-9]*
    ROOT = 'a' 'b'
  )";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_NE(grammar, nullptr);
  ASSERT_TRUE(logs.empty());
  ASSERT_TRUE(grammar->count("ROOT"));
  ASSERT_TRUE(grammar->count(peg::WORD_DEFINITION_NAME));

  // Verify that the word operator is set on the start rule
  auto& start_rule = (*grammar)["ROOT"];
  ASSERT_NE(start_rule.wordOpe, nullptr);
}

TEST(PeglibPerformCoreTest, EmptyGrammarString) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = "";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_EQ(grammar, nullptr); // Empty grammar is invalid
  ASSERT_FALSE(logs.empty());
  ASSERT_EQ(logs.size(), 1);
  ASSERT_EQ(logs[0].line, 1);
  ASSERT_EQ(logs[0].column, 1);
  ASSERT_EQ(logs[0].message, "syntax error");
}

TEST(PeglibPerformCoreTest, GrammarWithOnlyComments) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = "// This is a comment\n/* Another comment */";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_EQ(grammar, nullptr); // Grammar must define at least one rule
  ASSERT_FALSE(logs.empty());
  ASSERT_EQ(logs.size(), 1);
  ASSERT_EQ(logs[0].line, 1);
  ASSERT_EQ(logs[0].column, 1);
  ASSERT_EQ(logs[0].message, "syntax error");
}

TEST(PeglibPerformCoreTest, GrammarWithMultipleRules) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = R"(
    ROOT = A B
    A = 'a'
    B = 'b'
  )";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_NE(grammar, nullptr);
  ASSERT_TRUE(logs.empty());
  ASSERT_TRUE(grammar->count("ROOT"));
  ASSERT_TRUE(grammar->count("A"));
  ASSERT_TRUE(grammar->count("B"));
  ASSERT_EQ(start_rule_name, "ROOT");
}

TEST(PeglibPerformCoreTest, GrammarWithCustomStartRule) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = R"(
    %start MY_START_RULE
    MY_START_RULE = 'hello'
  )";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_NE(grammar, nullptr);
  ASSERT_TRUE(logs.empty());
  ASSERT_TRUE(grammar->count("MY_START_RULE"));
  ASSERT_EQ(start_rule_name, "MY_START_RULE");
}

TEST(PeglibPerformCoreTest, GrammarWithUndefinedCustomStartRule) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = R"(
    %start UNDEFINED_RULE
    ROOT = 'hello'
  )";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_EQ(grammar, nullptr);
  ASSERT_FALSE(logs.empty());
  ASSERT_EQ(logs.size(), 1);
  ASSERT_EQ(logs[0].line, 2);
  ASSERT_EQ(logs[0].column, 5);
  ASSERT_EQ(logs[0].message, "'UNDEFINED_RULE' is not defined.");
}

TEST(PeglibPerformCoreTest, GrammarWithParametrizedRule) {
  peg::parser parser;
  LogEntries logs;
  auto log_fn = create_capturing_log(logs);

  const char* grammar_str = R"(
    Expr<T> = T '+' T
    ROOT = Expr<Digit>
    Digit = [0-9]
  )";
  std::string start_rule_name;
  peg::Rules rules;

  auto grammar = parser.perform_core(grammar_str, strlen(grammar_str), rules,
                                     start_rule_name, log_fn);

  ASSERT_NE(grammar, nullptr);
  ASSERT_TRUE(logs.empty());
  ASSERT_TRUE(grammar->count("Expr<T>"));
  ASSERT_TRUE(grammar->count("ROOT"));
  ASSERT_TRUE