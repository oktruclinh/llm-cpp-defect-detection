#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>
#include <memory>

class PerformCoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup common test data
    }
};

// Test basic successful grammar parsing
TEST_F(PerformCoreTest, BasicGrammar) {
    peg::parser parser;
    
    // Define a simple grammar
    auto grammar = R"(
        Expr <- Sum
        Sum <- Number '+' Number
        Number <- [0-9]+
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    
    // Test parsing with the grammar
    EXPECT_TRUE(parser.parse("123+456"));
    EXPECT_FALSE(parser.parse("abc"));
}

// Test grammar with whitespace skipping
TEST_F(PerformCoreTest, WhitespaceSkipping) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Sum
        Sum <- Number '+' Number
        Number <- [0-9]+
        %whitespace <- [ \t]*
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    EXPECT_TRUE(parser.parse("123 + 456"));
    EXPECT_TRUE(parser.parse("  123+456  "));
}

// Test grammar with word expression
TEST_F(PerformCoreTest, WordExpression) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Word
        Word <- [a-zA-Z]+
        %word <- [a-zA-Z]
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    EXPECT_TRUE(parser.parse("hello"));
    EXPECT_FALSE(parser.parse("123"));
}

// Test grammar with ignore operator
TEST_F(PerformCoreTest, IgnoreOperator) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Number
        Number <- [0-9]+
        ~Separator <- ','
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    EXPECT_TRUE(parser.parse("123"));
}

// Test grammar with user-defined rules
TEST_F(PerformCoreTest, UserDefinedRules) {
    peg::parser parser;
    
    // Add a user-defined rule
    parser["Number"] = [](const peg::SemanticValues& sv) {
        return std::stoi(sv.str());
    };
    
    auto grammar = R"(
        Expr <- Number
        Number <- [0-9]+
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    EXPECT_TRUE(parser.parse("42"));
}

// Test grammar with duplicate definitions (should fail)
TEST_F(PerformCoreTest, DuplicateDefinitions) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Number
        Number <- [0-9]+
        Number <- [a-f]+
    )";
    
    EXPECT_FALSE(parser.load_grammar(grammar));
}

// Test grammar with left recursion (should fail)
TEST_F(PerformCoreTest, LeftRecursion) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Expr '+' Number / Number
        Number <- [0-9]+
    )";
    
    EXPECT_FALSE(parser.load_grammar(grammar));
}

// Test grammar with missing definition (should fail)
TEST_F(PerformCoreTest, MissingDefinition) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- UndefinedRule
        Number <- [0-9]+
    )";
    
    EXPECT_FALSE(parser.load_grammar(grammar));
}

// Test grammar with infinite loop detection
TEST_F(PerformCoreTest, InfiniteLoop) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Expr
        Number <- [0-9]+
    )";
    
    EXPECT_FALSE(parser.load_grammar(grammar));
}

// Test grammar with ignore operator on start rule (should fail)
TEST_F(PerformCoreTest, IgnoreOnStartRule) {
    peg::parser parser;
    
    auto grammar = R"(
        ~Expr <- Number
        Number <- [0-9]+
    )";
    
    EXPECT_FALSE(parser.load_grammar(grammar));
}

// Test grammar with complex nested rules
TEST_F(PerformCoreTest, ComplexNestedRules) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Sum
        Sum <- Product ( '+' Product / '-' Product )*
        Product <- Value ( '*' Value / '/' Value )*
        Value <- Number / '(' Expr ')'
        Number <- [0-9]+
        %whitespace <- [ \t]*
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    EXPECT_TRUE(parser.parse("1+2*3"));
    EXPECT_TRUE(parser.parse("(1+2)*3"));
    EXPECT_FALSE(parser.parse("1+"));
}

// Test grammar with empty rules
TEST_F(PerformCoreTest, EmptyRule) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Empty / Number
        Empty <- ''
        Number <- [0-9]+
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    EXPECT_TRUE(parser.parse(""));
    EXPECT_TRUE(parser.parse("123"));
}

// Test grammar with character classes
TEST_F(PerformCoreTest, CharacterClasses) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- [a-zA-Z0-9]+
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    EXPECT_TRUE(parser.parse("Hello123"));
    EXPECT_FALSE(parser.parse("Hello!"));
}

// Test grammar with repetition operators
TEST_F(PerformCoreTest, RepetitionOperators) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- 'a'* 'b'+ 'c'?
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    EXPECT_TRUE(parser.parse("bc"));
    EXPECT_TRUE(parser.parse("aabc"));
    EXPECT_TRUE(parser.parse("b"));
    EXPECT_FALSE(parser.parse("c"));
}

// Test grammar with ordered choice
TEST_F(PerformCoreTest, OrderedChoice) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- 'keyword' / 'key'
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    EXPECT_TRUE(parser.parse("keyword"));
    EXPECT_TRUE(parser.parse("key"));
}

// Test grammar with semantic actions
TEST_F(PerformCoreTest, SemanticActions) {
    peg::parser parser;
    std::vector<int> values;
    
    parser["Number"] = [&](const peg::SemanticValues& sv) {
        values.push_back(std::stoi(sv.str()));
        return values.back();
    };
    
    auto grammar = R"(
        Expr <- Number (',' Number)*
        Number <- [0-9]+
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    EXPECT_TRUE(parser.parse("1,2,3"));
    ASSERT_EQ(values.size(), 3);
    EXPECT_EQ(values[0], 1);
    EXPECT_EQ(values[1], 2);
    EXPECT_EQ(values[2], 3);
}

// Test grammar with error logging
TEST_F(PerformCoreTest, ErrorLogging) {
    peg::parser parser;
    std::string error_message;
    
    parser.set_logger([&](size_t line, size_t col, const std::string& msg) {
        error_message = msg;
    });
    
    auto grammar = R"(
        Expr <- Number
        Number <- [0-9]+
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    EXPECT_FALSE(parser.parse("abc"));
    EXPECT_FALSE(error_message.empty());
}

// Test grammar with multiple start rules
TEST_F(PerformCoreTest, MultipleStartRules) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Number
        Number <- [0-9]+
        Letter <- [a-z]+
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar, "Letter"));
    EXPECT_TRUE(parser.parse("hello"));
    EXPECT_FALSE(parser.parse("123"));
}

// Test grammar with parameterized rules
TEST_F(PerformCoreTest, ParameterizedRules) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Pair<Number>
        Pair<T> <- '(' T ',' T ')'
        Number <- [0-9]+
        %whitespace <- [ \t]*
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    EXPECT_TRUE(parser.parse("(1, 2)"));
    EXPECT_FALSE(parser.parse("(1, 2, 3)"));
}