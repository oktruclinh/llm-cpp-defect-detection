#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>
#include <memory>

// Test fixture for parser with AST enabled
class EnableAstTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple arithmetic grammar
        valid_grammar = R"(
            Expr <- Sum
            Sum  <- Product ( '+' Product / '-' Product )*
            Product <- Value ( '*' Value / '/' Value )*
            Value  <- < [0-9]+ >
        )";
        
        // Grammar with no actions defined
        no_action_grammar = R"(
            A <- 'a'+
            B <- 'b'+
        )";
        
        // Grammar with some actions already defined
        partial_action_grammar = R"(
            Start <- 'hello'
            End <- 'world'
        )";
    }

    std::string valid_grammar;
    std::string no_action_grammar;
    std::string partial_action_grammar;
};

// Test that enable_ast() works with a valid grammar and no pre-existing actions
TEST_F(EnableAstTest, ValidGrammarNoActions) {
    peg::parser parser(no_action_grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_ast();
    
    // Parse a simple input and verify AST is created
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("aaa", ast));
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ("A", ast->name);
    EXPECT_EQ("aaa", ast->token);
}

// Test that enable_ast() works with a grammar that already has some actions
TEST_F(EnableAstTest, ValidGrammarWithSomeActions) {
    peg::parser parser(partial_action_grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Add an action to one rule
    parser["Start"] = [](const peg::SemanticValues& vs) {
        return std::string("custom_action");
    };
    
    parser.enable_ast();
    
    // Parse "hello" - should use custom action for Start, AST for End
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("hello", ast));
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ("Start", ast->name);
    EXPECT_EQ("hello", ast->token);
    
    // Parse "world" - should use AST for End (no custom action)
    ASSERT_TRUE(parser.parse("world", ast));
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ("End", ast->name);
    EXPECT_EQ("world", ast->token);
}

// Test that enable_ast() works with a complex grammar and produces correct AST structure
TEST_F(EnableAstTest, ComplexGrammarAstStructure) {
    peg::parser parser(valid_grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_ast();
    
    // Parse "1+2*3" and verify AST structure
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("1+2*3", ast));
    ASSERT_NE(nullptr, ast);
    
    // The AST should have a tree structure
    EXPECT_EQ("Expr", ast->name);
    ASSERT_GE(ast->nodes.size(), 1);
    
    // Check that the AST has proper children
    auto sum_node = ast->nodes[0];
    EXPECT_EQ("Sum", sum_node->name);
    ASSERT_GE(sum_node->nodes.size(), 1);
}

// Test that enable_ast() returns *this (chainable)
TEST_F(EnableAstTest, ReturnsThis) {
    peg::parser parser(no_action_grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    peg::parser& result = parser.enable_ast();
    EXPECT_EQ(&parser, &result);
}

// Test that enable_ast() can be called multiple times safely
TEST_F(EnableAstTest, MultipleCalls) {
    peg::parser parser(no_action_grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_ast();
    parser.enable_ast();  // Second call should be safe (no-op for rules with actions)
    parser.enable_ast();  // Third call should also be safe
    
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("bbb", ast));
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ("B", ast->name);
}

// Test that enable_ast() works with custom AST type
TEST_F(EnableAstTest, CustomAstType) {
    struct CustomAst : peg::Ast {
        int custom_data = 42;
    };
    
    peg::parser parser(no_action_grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_ast<CustomAst>();
    
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("a", ast));
    ASSERT_NE(nullptr, ast);
    
    // Verify it's actually a CustomAst
    auto custom_ast = std::dynamic_pointer_cast<CustomAst>(ast);
    EXPECT_NE(nullptr, custom_ast);
    EXPECT_EQ(42, custom_ast->custom_data);
}

// Test that enable_ast() works with empty grammar (edge case)
TEST_F(EnableAstTest, EmptyGrammar) {
    peg::parser parser("");
    ASSERT_FALSE(static_cast<bool>(parser));  // Empty grammar is invalid
    
    // enable_ast() should still be callable but won't do anything useful
    parser.enable_ast();
    // No parsing possible since grammar is invalid
}

// Test that enable_ast() preserves existing actions
TEST_F(EnableAstTest, PreservesExistingActions) {
    peg::parser parser(no_action_grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    bool action_called = false;
    parser["A"] = [&action_called](const peg::SemanticValues& vs) {
        action_called = true;
        return std::string("custom");
    };
    
    parser.enable_ast();
    
    // Parse "a" - should call the custom action, not create AST for A
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("a", ast));
    EXPECT_TRUE(action_called);
    // The custom action returns a string, so AST should be null or different
    // Actually, when custom action returns non-AST, parse with AST output may fail
    // Let's test without AST output
    EXPECT_TRUE(parser.parse("a"));  // Should still parse successfully
}

// Test that enable_ast() works with packrat parsing enabled
TEST_F(EnableAstTest, WithPackratParsing) {
    peg::parser parser(valid_grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_packrat_parsing();
    parser.enable_ast();
    
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("5+3*2", ast));
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ("Expr", ast->name);
}

// Test that enable_ast() works with error logging
TEST_F(EnableAstTest, WithErrorLogging) {
    peg::parser parser(valid_grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    std::vector<std::string> errors;
    parser.log = [&errors](size_t line, size_t col, const std::string& msg) {
        errors.push_back(msg);
    };
    
    parser.enable_ast();
    
    // Parse invalid input
    std::shared_ptr<peg::Ast> ast;
    EXPECT_FALSE(parser.parse("invalid!", ast));
    EXPECT_FALSE(errors.empty());
}

// Test that enable_ast() works with single character grammar
TEST_F(EnableAstTest, SingleCharacterGrammar) {
    peg::parser parser("A <- 'x'");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_ast();
    
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("x", ast));
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ("A", ast->name);
    EXPECT_EQ("x", ast->token);
}

// Test that enable_ast() works with grammar that has multiple alternatives
TEST_F(EnableAstTest, MultipleAlternatives) {
    peg::parser parser(R"(
        Start <- 'cat' / 'dog' / 'bird'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_ast();
    
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("cat", ast));
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ("Start", ast->name);
    EXPECT_EQ("cat", ast->token);
    
    ASSERT_TRUE(parser.parse("dog", ast));
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ("Start", ast->name);
    EXPECT_EQ("dog", ast->token);
}

// Test that enable_ast() works with repetition operators
TEST_F(EnableAstTest, RepetitionOperators) {
    peg::parser parser(R"(
        Sequence <- 'a'+
        Optional <- 'b'?
        ZeroOrMore <- 'c'*
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_ast();
    
    std::shared_ptr<peg::Ast> ast;
    
    // Test '+'
    ASSERT_TRUE(parser.parse("aaa", ast));
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ("Sequence", ast->name);
    
    // Test '?'
    ASSERT_TRUE(parser.parse("b", ast));
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ("Optional", ast->name);
    
    // Test '*'
    ASSERT_TRUE(parser.parse("ccc", ast));
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ("ZeroOrMore", ast->name);
}