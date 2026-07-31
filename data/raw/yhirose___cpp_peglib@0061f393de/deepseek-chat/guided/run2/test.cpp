#include <gtest/gtest.h>
#include "peglib.h"

class PerformCoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
};

// Test basic grammar parsing
TEST_F(PerformCoreTest, BasicGrammar) {
    peg::parser parser;
    
    // Define a simple grammar
    auto grammar = R"(
        Expr <- Term
        Term <- 'a'
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    
    // Test parsing a valid input
    auto result = parser.parse("a");
    EXPECT_TRUE(result.ret);
    EXPECT_EQ(result.value, "a");
}

// Test grammar with user-defined rules
TEST_F(PerformCoreTest, UserDefinedRules) {
    peg::parser parser;
    
    // Define grammar with user rules
    auto grammar = R"(
        Expr <- Term
        Term <- 'b'
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    
    // Test parsing
    auto result = parser.parse("b");
    EXPECT_TRUE(result.ret);
    EXPECT_EQ(result.value, "b");
}

// Test grammar with whitespace skipping
TEST_F(PerformCoreTest, WhitespaceSkipping) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Term
        Term <- 'a'
        %whitespace <- ' '*
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    
    // Test with whitespace
    auto result = parser.parse("  a  ");
    EXPECT_TRUE(result.ret);
    EXPECT_EQ(result.value, "a");
}

// Test grammar with word expression
TEST_F(PerformCoreTest, WordExpression) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Term
        Term <- < 'a' >
        %word <- [a-z]+
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    
    auto result = parser.parse("a");
    EXPECT_TRUE(result.ret);
}

// Test error handling - syntax error in grammar
TEST_F(PerformCoreTest, SyntaxErrorInGrammar) {
    peg::parser parser;
    
    // Invalid grammar syntax
    auto grammar = R"(
        Expr <- 
    )";
    
    EXPECT_FALSE(parser.load_grammar(grammar));
}

// Test error handling - duplicate rule definition
TEST_F(PerformCoreTest, DuplicateRuleDefinition) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- 'a'
        Expr <- 'b'
    )";
    
    EXPECT_FALSE(parser.load_grammar(grammar));
}

// Test error handling - missing rule reference
TEST_F(PerformCoreTest, MissingRuleReference) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- UndefinedRule
    )";
    
    EXPECT_FALSE(parser.load_grammar(grammar));
}

// Test error handling - left recursion
TEST_F(PerformCoreTest, LeftRecursion) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Expr '+' Term
        Term <- 'a'
    )";
    
    EXPECT_FALSE(parser.load_grammar(grammar));
}

// Test error handling - infinite loop detection
TEST_F(PerformCoreTest, InfiniteLoop) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- ''
    )";
    
    EXPECT_FALSE(parser.load_grammar(grammar));
}

// Test grammar with precedence instructions
TEST_F(PerformCoreTest, PrecedenceInstruction) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Term
        Term <- Factor
        Factor <- 'a'
        %precedence Expr {
            left '+' 1
            left '*' 2
        }
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    
    auto result = parser.parse("a");
    EXPECT_TRUE(result.ret);
}

// Test grammar with ignore operator on start rule
TEST_F(PerformCoreTest, IgnoreOperatorOnStartRule) {
    peg::parser parser;
    
    auto grammar = R"(
        ~Expr <- 'a'
    )";
    
    EXPECT_FALSE(parser.load_grammar(grammar));
}

// Test grammar with ignore operator on non-start rule
TEST_F(PerformCoreTest, IgnoreOperatorOnNonStartRule) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- ~Term
        Term <- 'a'
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    
    auto result = parser.parse("a");
    EXPECT_TRUE(result.ret);
}

// Test empty grammar
TEST_F(PerformCoreTest, EmptyGrammar) {
    peg::parser parser;
    
    auto grammar = "";
    EXPECT_FALSE(parser.load_grammar(grammar));
}

// Test grammar with only whitespace definition
TEST_F(PerformCoreTest, OnlyWhitespaceDefinition) {
    peg::parser parser;
    
    auto grammar = R"(
        %whitespace <- ' '*
    )";
    
    EXPECT_FALSE(parser.load_grammar(grammar));
}

// Test grammar with complex nested rules
TEST_F(PerformCoreTest, ComplexNestedRules) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Sum
        Sum <- Product ( '+' Product )*
        Product <- Value ( '*' Value )*
        Value <- '0' / '1' / '(' Expr ')'
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    
    auto result = parser.parse("1+0*1");
    EXPECT_TRUE(result.ret);
}

// Test grammar with semantic actions
TEST_F(PerformCoreTest, SemanticActions) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Term
        Term <- 'a' { [](const SemanticValues& vs) { return "action"; } }
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    
    auto result = parser.parse("a");
    EXPECT_TRUE(result.ret);
    EXPECT_EQ(result.value, "action");
}

// Test grammar with multiple user rules
TEST_F(PerformCoreTest, MultipleUserRules) {
    peg::parser parser;
    
    auto grammar = R"(
        Expr <- Term
        Term <- 'a'
    )";
    
    EXPECT_TRUE(parser.load_grammar(grammar));
    
    // Add user-defined rules
    parser["Expr"] = [](const peg::SemanticValues& vs) { return "expr"; };
    parser["Term"] = [](const peg::SemanticValues& vs) { return "term"; };
    
    auto result = parser.parse("a");
    EXPECT_TRUE(result.ret);
    EXPECT_EQ(result.value, "term");
}