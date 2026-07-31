#include <gtest/gtest.h>
#include <peglib.h>
#include <string>
#include <vector>

using namespace peg;

TEST(PeglibTest, PerformCoreNormalCase) {
    std::string start_rule;
    std::vector<std::string> log_messages;
    auto log = [&](size_t line, size_t col, const std::string& msg) {
        log_messages.push_back("Line " + std::to_string(line) + ", Col " + std::to_string(col) + ": " + msg);
    };

    // Test a valid grammar
    const char* grammar_str = R"(
        Grammar <- Expression
        Expression <- Term (AddOp Term)*
        Term <- Factor (MulOp Factor)*
        Factor <- Number / '(' Expression ')'
        AddOp <- '+' / '-'
        MulOp <- '*' / '/'
        Number <- [0-9]+
    )";

    auto grammar = perform_core(grammar_str, strlen(grammar_str), {}, start_rule, log);
    ASSERT_NE(grammar, nullptr);
    EXPECT_EQ(start_rule, "Grammar");
    EXPECT_TRUE(log_messages.empty());
}

TEST(PeglibTest, PerformCoreSyntaxError) {
    std::string start_rule;
    std::vector<std::string> log_messages;
    auto log = [&](size_t line, size_t col, const std::string& msg) {
        log_messages.push_back("Line " + std::to_string(line) + ", Col " + std::to_string(col) + ": " + msg);
    };

    // Test with invalid grammar syntax
    const char* grammar_str = R"(
        Grammar <- Expression
        Expression <- Term (AddOp Term)*
        Term <- Factor (MulOp Factor)*
        Factor <- Number / '(' Expression ')'
        AddOp <- '+' / '-'
        MulOp <- '*' / '/'
        Number <- [0-9]+
        // Missing semicolon
        ExtraRule <- "test"
    )";

    auto grammar = perform_core(grammar_str, strlen(grammar_str), {}, start_rule, log);
    ASSERT_EQ(grammar, nullptr);
    EXPECT_FALSE(log_messages.empty());
}

TEST(PeglibTest, PerformCoreDuplicateDefinition) {
    std::string start_rule;
    std::vector<std::string> log_messages;
    auto log = [&](size_t line, size_t col, const std::string& msg) {
        log_messages.push_back("Line " + std::to_string(line) + ", Col " + std::to_string(col) + ": " + msg);
    };

    // Test duplicate rule definition
    const char* grammar_str = R"(
        Grammar <- Expression
        Expression <- Term
        Expression <- Factor  // Duplicate definition
        Term <- Number
        Number <- [0-9]+
    )";

    auto grammar = perform_core(grammar_str, strlen(grammar_str), {}, start_rule, log);
    ASSERT_EQ(grammar, nullptr);
    EXPECT_FALSE(log_messages.empty());
    EXPECT_NE(log_messages[0].find("already defined"), std::string::npos);
}

TEST(PeglibTest, PerformCoreLeftRecursion) {
    std::string start_rule;
    std::vector<std::string> log_messages;
    auto log = [&](size_t line, size_t col, const std::string& msg) {
        log_messages.push_back("Line " + std::to_string(line) + ", Col " + std::to_string(col) + ": " + msg);
    };

    // Test left recursion
    const char* grammar_str = R"(
        Grammar <- Expression
        Expression <- Expression '+' Term / Term
        Term <- Number
        Number <- [0-9]+
    )";

    auto grammar = perform_core(grammar_str, strlen(grammar_str), {}, start_rule, log);
    ASSERT_EQ(grammar, nullptr);
    EXPECT_FALSE(log_messages.empty());
    EXPECT_NE(log_messages[0].find("left recursive"), std::string::npos);
}

TEST(PeglibTest, PerformCoreMissingDefinition) {
    std::string start_rule;
    std::vector<std::string> log_messages;
    auto log = [&](size_t line, size_t col, const std::string& msg) {
        log_messages.push_back("Line " + std::to_string(line) + ", Col " + std::to_string(col) + ": " + msg);
    };

    // Test missing rule reference
    const char* grammar_str = R"(
        Grammar <- Expression
        Expression <- Term
        Term <- UnknownRule  // Unknown rule reference
    )";

    auto grammar = perform_core(grammar_str, strlen(grammar_str), {}, start_rule, log);
    ASSERT_EQ(grammar, nullptr);
    EXPECT_FALSE(log_messages.empty());
}

TEST(PeglibTest, PerformCoreIgnoreOperatorOnStart) {
    std::string start_rule;
    std::vector<std::string> log_messages;
    auto log = [&](size_t line, size_t col, const std::string& msg) {
        log_messages.push_back("Line " + std::to_string(line) + ", Col " + std::to_string(col) + ": " + msg);
    };

    // Test ignore operator on start rule
    const char* grammar_str = R"(
        Grammar <- ~Expression
        Expression <- Term
        Term <- Number
        Number <- [0-9]+
    )";

    auto grammar = perform_core(grammar_str, strlen(grammar_str), {}, start_rule, log);
    ASSERT_EQ(grammar, nullptr);
    EXPECT_FALSE(log_messages.empty());
    EXPECT_NE(log_messages[0].find("Ignore operator cannot be applied"), std::string::npos);
}

TEST(PeglibTest, PerformCoreEmptyGrammar) {
    std::string start_rule;
    std::vector<std::string> log_messages;
    auto log = [&](size_t line, size_t col, const std::string& msg) {
        log_messages.push_back("Line " + std::to_string(line) + ", Col " + std::to_string(col) + ": " + msg);
    };

    // Test empty grammar
    const char* grammar_str = "";

    auto grammar = perform_core(grammar_str, strlen(grammar_str), {}, start_rule, log);
    ASSERT_EQ(grammar, nullptr);
    EXPECT_FALSE(log_messages.empty());
}

TEST(PeglibTest, PerformCoreUserRules) {
    std::string start_rule;
    std::vector<std::string> log_messages;
    auto log = [&](size_t line, size_t col, const std::string& msg) {
        log_messages.push_back("Line " + std::to_string(line) + ", Col " + std::to_string(col) + ": " + msg);
    };

    // Test with user rules
    const char* grammar_str = R"(
        Grammar <- Expression
        Expression <- Term
        Term <- Number
        Number <- [0-9]+
    )";

    Rules user_rules;
    user_rules["CustomRule"] = "test";

    auto grammar = perform_core(grammar_str, strlen(grammar_str), user_rules, start_rule, log);
    ASSERT_NE(grammar, nullptr);
    EXPECT_EQ(start_rule, "Grammar");
    EXPECT_TRUE(log_messages.empty());
}

TEST(PeglibTest, PerformCoreWhitespaceHandling) {
    std::string start_rule;
    std::vector<std::string> log_messages;
    auto log = [&](size_t line, size_t col, const std::string& msg) {
        log_messages.push_back("Line " + std::to_string(line) + ", Col " + std::to_string(col) + ": " + msg);
    };

    // Test grammar with whitespace definition
    const char* grammar_str = R"(
        Grammar <- Expression
        Expression <- Term
        Term <- Number
        Number <- [0-9]+
        %whitespace <- [ \t]+
    )";

    auto grammar = perform_core(grammar_str, strlen(grammar_str), {}, start_rule, log);
    ASSERT_NE(grammar, nullptr);
    EXPECT_EQ(start_rule, "Grammar");
    EXPECT_TRUE(log_messages.empty());
}