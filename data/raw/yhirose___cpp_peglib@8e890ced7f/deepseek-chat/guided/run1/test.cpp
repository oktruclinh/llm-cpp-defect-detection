#include <gtest/gtest.h>
#include "peglib.h"

class EnableAstTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple grammar that can be used for testing
        grammar_ = std::make_shared<peg::Grammar>(
            R"(
                ROOT      <-  NUMBER
                NUMBER    <-  [0-9]+
                %whitespace <- [ \t]*
            )"
        );
    }

    std::shared_ptr<peg::Grammar> grammar_;
};

TEST_F(EnableAstTest, BasicAstEnabled) {
    // Create parser and enable AST
    peg::parser parser(*grammar_);
    parser.enable_ast();
    
    // Parse a simple number and verify AST is generated
    auto ast = parser.parse("123");
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->name, "ROOT");
    EXPECT_EQ(ast->nodes.size(), 1);
    EXPECT_EQ(ast->nodes[0]->name, "NUMBER");
    EXPECT_EQ(ast->nodes[0]->token, "123");
}

TEST_F(EnableAstTest, MultipleCallsIdempotent) {
    peg::parser parser(*grammar_);
    
    // Call enable_ast multiple times
    parser.enable_ast();
    parser.enable_ast();
    parser.enable_ast();
    
    // Verify AST is still generated correctly
    auto ast = parser.parse("456");
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->name, "ROOT");
    EXPECT_EQ(ast->nodes.size(), 1);
    EXPECT_EQ(ast->nodes[0]->name, "NUMBER");
    EXPECT_EQ(ast->nodes[0]->token, "456");
}

TEST_F(EnableAstTest, CustomAstType) {
    struct CustomAst : peg::Ast {
        CustomAst() : peg::Ast() {}
    };
    
    peg::parser parser(*grammar_);
    parser.enable_ast<CustomAst>();
    
    auto ast = parser.parse("789");
    ASSERT_NE(ast, nullptr);
    // Verify it's actually a CustomAst (dynamic_cast should succeed)
    EXPECT_NE(dynamic_cast<CustomAst*>(ast.get()), nullptr);
    EXPECT_EQ(ast->name, "ROOT");
}

TEST_F(EnableAstTest, EmptyGrammar) {
    // Create parser with empty grammar
    peg::parser parser("");
    
    // Should not crash when enabling AST on empty grammar
    EXPECT_NO_THROW(parser.enable_ast());
    
    // Parsing empty input should still work
    auto ast = parser.parse("");
    EXPECT_EQ(ast, nullptr);
}

TEST_F(EnableAstTest, GrammarWithActions) {
    // Create grammar with custom actions
    auto grammar_with_actions = std::make_shared<peg::Grammar>(
        R"(
            ROOT      <-  NUMBER
            NUMBER    <-  [0-9]+
            %whitespace <- [ \t]*
        )"
    );
    
    // Add a custom action to NUMBER rule
    grammar_with_actions->operator[]("NUMBER").action = [](const peg::SemanticValues& sv) -> peg::Any {
        return std::stoi(sv.token());
    };
    
    peg::parser parser(*grammar_with_actions);
    parser.enable_ast();
    
    // Parse and verify AST is generated despite existing actions
    auto ast = parser.parse("42");
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->name, "ROOT");
    EXPECT_EQ(ast->nodes.size(), 1);
    EXPECT_EQ(ast->nodes[0]->name, "NUMBER");
    EXPECT_EQ(ast->nodes[0]->token, "42");
}

TEST_F(EnableAstTest, ComplexGrammar) {
    auto complex_grammar = std::make_shared<peg::Grammar>(
        R"(
            EXPRESSION <-  TERM ( '+' TERM )*
            TERM       <-  FACTOR ( '*' FACTOR )*
            FACTOR     <-  NUMBER / '(' EXPRESSION ')'
            NUMBER     <-  [0-9]+
            %whitespace <- [ \t]*
        )"
    );
    
    peg::parser parser(*complex_grammar);
    parser.enable_ast();
    
    // Parse a complex expression
    auto ast = parser.parse("1+2*3");
    ASSERT_NE(ast, nullptr);
    EXPECT_EQ(ast->name, "EXPRESSION");
    
    // Verify the AST structure
    ASSERT_GE(ast->nodes.size(), 1);
    EXPECT_EQ(ast->nodes[0]->name, "TERM");
    
    // Check that we have the correct number of children for the expression
    // EXPRESSION should have TERM, '+', TERM structure
    EXPECT_GE(ast->nodes.size(), 3);
}

TEST_F(EnableAstTest, ParseFailureReturnsNull) {
    peg::parser parser(*grammar_);
    parser.enable_ast();
    
    // Parse invalid input
    auto ast = parser.parse("abc");
    EXPECT_EQ(ast, nullptr);
}

TEST_F(EnableAstTest, EnableAstBeforeParse) {
    peg::parser parser(*grammar_);
    
    // Enable AST before any parsing
    parser.enable_ast();
    
    // First parse
    auto ast1 = parser.parse("111");
    ASSERT_NE(ast1, nullptr);
    EXPECT_EQ(ast1->nodes[0]->token, "111");
    
    // Second parse
    auto ast2 = parser.parse("222");
    ASSERT_NE(ast2, nullptr);
    EXPECT_EQ(ast2->nodes[0]->token, "222");
}

TEST_F(EnableAstTest, EnableAstAfterParse) {
    peg::parser parser(*grammar_);
    
    // Parse without AST first
    auto result1 = parser.parse("333");
    EXPECT_NE(result1, nullptr);
    
    // Now enable AST
    parser.enable_ast();
    
    // Parse again with AST
    auto ast2 = parser.parse("444");
    ASSERT_NE(ast2, nullptr);
    EXPECT_EQ(ast2->nodes[0]->token, "444");
}