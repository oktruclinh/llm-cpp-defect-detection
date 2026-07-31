#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>

TEST(ParserAstTest, EnableAstBasicFunctionality) {
    peg::parser parser;
    
    // Test that enable_ast() can be called without throwing
    EXPECT_NO_THROW(parser.enable_ast());
    
    // Test with a simple grammar
    parser = peg::parser(R"(
        Start = "hello"
    )");
    
    EXPECT_TRUE(parser;
    
    // Enable AST and verify it works
    auto& result = parser.enable_ast();
    EXPECT_EQ(&parser, &result);
}

TEST(ParserAstTest, EnableAstWithExistingActions) {
    peg::parser parser;
    
    // Create a parser with a rule that has an action
    parser = peg::parser(R"(
        Start = "hello" { return 42; }
    )");
    
    // Enable AST - should not override existing actions
    EXPECT_NO_THROW(parser.enable_ast());
}

TEST(ParserAstTest, EnableAstMultipleCalls) {
    peg::parser parser;
    
    // Test multiple calls to enable_ast()
    parser = peg::parser(R"(
        Start = "test"
    )");
    
    EXPECT_NO_THROW(parser.enable_ast());
    EXPECT_NO_THROW(parser.enable_ast());
    EXPECT_NO_THROW(parser.enable_ast());
}

TEST(ParserAstTest, EnableAstEmptyGrammar) {
    peg::parser parser;
    
    // Test with empty grammar
    parser = peg::parser("");
    
    EXPECT_NO_THROW(parser.enable_ast());
}

TEST(ParserAstTest, EnableAstComplexGrammar) {
    peg::parser parser;
    
    // Test with a more complex grammar
    parser = peg::parser(R"(
        Start = Expr
        Expr = Term (["+-"] Term)*
        Term = Factor (["*/"] Factor)*
        Factor = Number / "(" Expr ")"
        Number = [0-9]+
    )");
    
    EXPECT_NO_THROW(parser.enable_ast());
    
    // Test parsing with AST enabled
    std::string input = "1+2*3";
    peg::ast_ptr ast;
    EXPECT_TRUE(parser.parse(input, ast));
    EXPECT_NE(nullptr, ast);
}

TEST(ParserAstTest, EnableAstWithCustomAstType) {
    peg::parser parser;
    
    // Test with custom AST type
    parser = peg::parser(R"(
        Start = "hello"
    )");
    
    // This should compile and work
    EXPECT_NO_THROW(parser.enable_ast<peg::ast>());
}

TEST(ParserAstTest, EnableAstRuleStructure) {
    peg::parser parser;
    
    parser = peg::parser(R"(
        Start = "test"
        Other = "other"
    )");
    
    // Enable AST and verify the parser still works
    parser.enable_ast();
    
    // Test that parsing still works
    std::string input = "test";
    peg::ast_ptr ast;
    EXPECT_TRUE(parser.parse(input, ast));
    EXPECT_NE(nullptr, ast);
}

TEST(ParserAstTest, EnableAstReturnReference) {
    peg::parser parser;
    
    parser = peg::parser(R"(
        Start = "hello"
    )");
    
    // Test that enable_ast() returns the correct reference
    peg::parser& returned_ref = parser.enable_ast();
    EXPECT_EQ(&parser, &returned_ref);
}