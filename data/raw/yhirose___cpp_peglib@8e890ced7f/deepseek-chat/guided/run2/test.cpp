#include <gtest/gtest.h>
#include "peglib.h"

class EnableAstTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple grammar that will be used across tests
        grammar_ = std::make_shared<peglib::peg::grammar>();
        
        // Add some rules without actions
        auto& rule1 = (*grammar_)["rule1"];
        rule1 = peglib::peg::rule("a");
        
        auto& rule2 = (*grammar_)["rule2"];
        rule2 = peglib::peg::rule("b");
        
        // Add a rule with an existing action (should not be overwritten)
        auto& rule3 = (*grammar_)["rule3"];
        rule3 = peglib::peg::rule("c");
        rule3.action = [](const peglib::peg::SemanticValues& sv) -> std::any {
            return std::string("custom_action");
        };
    }

    std::shared_ptr<peglib::peg::grammar> grammar_;
};

TEST_F(EnableAstTest, AddsAstActionsToRulesWithoutActions) {
    peglib::peg::parser parser(*grammar_);
    
    // Before enable_ast, rules without actions should not have AST actions
    auto& rule1 = (*grammar_)["rule1"];
    EXPECT_FALSE(rule1.action.has_value());
    
    auto& rule2 = (*grammar_)["rule2"];
    EXPECT_FALSE(rule2.action.has_value());
    
    // Enable AST
    parser.enable_ast();
    
    // After enable_ast, rules without actions should have AST actions
    EXPECT_TRUE(rule1.action.has_value());
    EXPECT_TRUE(rule2.action.has_value());
    
    // Rule with existing action should still have its custom action
    auto& rule3 = (*grammar_)["rule3"];
    EXPECT_TRUE(rule3.action.has_value());
    
    // Verify the AST action works by parsing and getting AST
    auto ast = parser.parse("a");
    ASSERT_TRUE(ast != nullptr);
    EXPECT_EQ(ast->name, "rule1");
}

TEST_F(EnableAstTest, DoesNotOverwriteExistingActions) {
    peglib::peg::parser parser(*grammar_);
    
    // Enable AST
    parser.enable_ast();
    
    // Parse with rule3 which has custom action
    auto result = parser.parse("c");
    ASSERT_TRUE(result.has_value());
    
    // The custom action should still be used, not AST
    EXPECT_EQ(std::any_cast<std::string>(result.value()), "custom_action");
}

TEST_F(EnableAstTest, CanBeCalledMultipleTimes) {
    peglib::peg::parser parser(*grammar_);
    
    // Call enable_ast multiple times
    parser.enable_ast();
    parser.enable_ast();
    parser.enable_ast();
    
    // Should still work correctly
    auto ast = parser.parse("a");
    ASSERT_TRUE(ast != nullptr);
    EXPECT_EQ(ast->name, "rule1");
}

TEST_F(EnableAstTest, WorksWithEmptyGrammar) {
    auto empty_grammar = std::make_shared<peglib::peg::grammar>();
    peglib::peg::parser parser(*empty_grammar);
    
    // Should not crash with empty grammar
    EXPECT_NO_THROW(parser.enable_ast());
}

TEST_F(EnableAstTest, ProducesValidAstForComplexGrammar) {
    // Create a more complex grammar
    auto complex_grammar = std::make_shared<peglib::peg::grammar>();
    (*complex_grammar)["expr"] = peglib::peg::rule("term ('+' term)*");
    (*complex_grammar)["term"] = peglib::peg::rule("factor ('*' factor)*");
    (*complex_grammar)["factor"] = peglib::peg::rule("'(' expr ')' / number");
    (*complex_grammar)["number"] = peglib::peg::rule("[0-9]+");
    
    peglib::peg::parser parser(*complex_grammar);
    parser.enable_ast();
    
    // Parse a simple expression
    auto ast = parser.parse("1+2*3");
    ASSERT_TRUE(ast != nullptr);
    
    // Verify AST structure
    EXPECT_EQ(ast->name, "expr");
    ASSERT_GE(ast->nodes.size(), 1);
    
    // The AST should have proper children
    bool found_term = false;
    for (const auto& node : ast->nodes) {
        if (node->name == "term") {
            found_term = true;
            break;
        }
    }
    EXPECT_TRUE(found_term);
}

TEST_F(EnableAstTest, WorksWithCustomAstType) {
    struct CustomAst : peglib::peg::Ast {
        int custom_data = 42;
    };
    
    peglib::peg::parser parser(*grammar_);
    parser.enable_ast<CustomAst>();
    
    auto ast = parser.parse("a");
    ASSERT_TRUE(ast != nullptr);
    
    // Verify it's our custom type
    auto custom_ast = std::dynamic_pointer_cast<CustomAst>(ast);
    ASSERT_NE(custom_ast, nullptr);
    EXPECT_EQ(custom_ast->custom_data, 42);
}

TEST_F(EnableAstTest, HandlesGrammarWithAllRulesHavingActions) {
    auto all_actions_grammar = std::make_shared<peglib::peg::grammar>();
    
    auto& rule1 = (*all_actions_grammar)["rule1"];
    rule1 = peglib::peg::rule("a");
    rule1.action = [](const peglib::peg::SemanticValues& sv) -> std::any {
        return std::string("action1");
    };
    
    auto& rule2 = (*all_actions_grammar)["rule2"];
    rule2 = peglib::peg::rule("b");
    rule2.action = [](const peglib::peg::SemanticValues& sv) -> std::any {
        return std::string("action2");
    };
    
    peglib::peg::parser parser(*all_actions_grammar);
    parser.enable_ast();
    
    // All rules should still have their custom actions
    EXPECT_TRUE(rule1.action.has_value());
    EXPECT_TRUE(rule2.action.has_value());
    
    // Custom actions should still work
    auto result1 = parser.parse("a");
    ASSERT_TRUE(result1.has_value());
    EXPECT_EQ(std::any_cast<std::string>(result1.value()), "action1");
}