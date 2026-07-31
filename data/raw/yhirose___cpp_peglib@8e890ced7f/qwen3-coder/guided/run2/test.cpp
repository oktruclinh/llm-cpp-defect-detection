#include <gtest/gtest.h>
#include "peglib.h"

TEST(ParserAstTest, EnableAstBasicFunctionality) {
    // Test basic enable_ast functionality
    peglib::parser parser;
    
    // Add a simple rule
    parser.R["S"] = peglib::any;
    
    // Enable AST - should not crash and should return *this
    auto& result = parser.enable_ast<>();
    EXPECT_EQ(&parser, &result);
}

TEST(ParserAstTest, EnableAstWithExistingActions) {
    // Test enable_ast when rules already have actions
    peglib::parser parser;
    
    // Add a rule with an existing action
    parser.R["S"] = peglib::any;
    parser.R["S"].action = [](const peglib::Ast& ast) {
        return nullptr;
    };
    
    // Enable AST - should not overwrite existing actions
    parser.enable_ast<>();
    
    // Verify the action still exists
    EXPECT_TRUE(parser.R["S"].action != nullptr);
}

TEST(ParserAstTest, EnableAstEmptyGrammar) {
    // Test enable_ast with empty grammar
    peglib::parser parser;
    
    // Enable AST on empty parser - should not crash
    parser.enable_ast<>();
    
    // Verify parser is still usable
    EXPECT_TRUE(parser.parse("test"));
}

TEST(ParserAstTest, EnableAstMultipleCalls) {
    // Test calling enable_ast multiple times
    peglib::parser parser;
    
    parser.R["S"] = peglib::any;
    
    // Call enable_ast multiple times
    parser.enable_ast<>();
    parser.enable_ast<>();
    parser.enable_ast<>();
    
    // Should work without issues
    EXPECT_TRUE(parser.parse("test"));
}

TEST(ParserAstTest, EnableAstWithDifferentAstTypes) {
    // Test enable_ast with different AST types
    peglib::parser parser;
    
    parser.R["S"] = peglib::any;
    
    // Test with default Ast type
    parser.enable_ast<>();
    
    // Test with custom AST type (if supported)
    struct CustomAst {
        std::string value;
    };
    
    parser.enable_ast<CustomAst>();
    
    // Should not crash and remain functional
    EXPECT_TRUE(parser.parse("test"));
}

TEST(ParserAstTest, EnableAstIntegrationWithParsing) {
    // Test that enable_ast works correctly with actual parsing
    peglib::parser parser;
    
    // Define a grammar that creates AST nodes
    parser.R["S"] = peglib::any;
    parser.R["S"].action = [](const peglib::Ast& ast) {
        return std::make_shared<peglib::Ast>(ast);
    };
    
    // Enable AST
    parser.enable_ast<>();
    
    // Parse something and verify AST is created
    auto result = parser.parse("test");
    EXPECT_TRUE(result);
    
    // Verify the AST structure is correct
    if (result) {
        EXPECT_NE(result->ast, nullptr);
    }
}

TEST(ParserAstTest, EnableAstRuleWithoutAction) {
    // Test enable_ast on rule that doesn't have action
    peglib::parser parser;
    
    // Add a rule without action
    parser.R["S"] = peglib::any;
    
    // Enable AST - should add action to rules without actions
    parser.enable_ast<>();
    
    // Parse should still work
    EXPECT_TRUE(parser.parse("test"));
}