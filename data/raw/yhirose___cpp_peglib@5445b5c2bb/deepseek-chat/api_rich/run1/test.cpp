#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>
#include <memory>

// Test that AssignIDToDefinition::visit correctly assigns IDs to definitions
// by verifying that the parser correctly handles references with arguments

TEST(AssignIDToDefinitionTest, SimpleReferenceWithoutArgs) {
    // A grammar with a simple reference (no arguments)
    peg::parser parser(R"(
        ROOT <- A
        A <- 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Enable AST to verify structure
    parser.enable_ast();
    
    // Parse a simple input
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("a", ast));
    
    // Verify the AST structure - A should be a child of ROOT
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->name, "ROOT");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "A");
}

TEST(AssignIDToDefinitionTest, ReferenceWithArguments) {
    // A grammar with a reference that has arguments (parameterized rule)
    peg::parser parser(R"(
        ROOT <- A('x')
        A(X) <- X
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Enable AST to verify structure
    parser.enable_ast();
    
    // Parse input that matches the parameterized rule
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("x", ast));
    
    // Verify the AST structure
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->name, "ROOT");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "A");
}

TEST(AssignIDToDefinitionTest, MultipleArgumentsInReference) {
    // A grammar with multiple arguments in a reference
    peg::parser parser(R"(
        ROOT <- A('x', 'y')
        A(X, Y) <- X Y
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Enable AST to verify structure
    parser.enable_ast();
    
    // Parse input that matches the parameterized rule
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("xy", ast));
    
    // Verify the AST structure
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->name, "ROOT");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "A");
}

TEST(AssignIDToDefinitionTest, NestedReferences) {
    // A grammar with nested references (reference inside another reference)
    peg::parser parser(R"(
        ROOT <- A(B('x'))
        A(X) <- X
        B(Y) <- Y
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Enable AST to verify structure
    parser.enable_ast();
    
    // Parse input that matches the nested parameterized rules
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("x", ast));
    
    // Verify the AST structure
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->name, "ROOT");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "A");
}

TEST(AssignIDToDefinitionTest, ReferenceWithEmptyArgs) {
    // A grammar with a reference that has empty arguments
    peg::parser parser(R"(
        ROOT <- A()
        A() <- 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Enable AST to verify structure
    parser.enable_ast();
    
    // Parse input that matches the parameterized rule
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("a", ast));
    
    // Verify the AST structure
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->name, "ROOT");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "A");
}

TEST(AssignIDToDefinitionTest, ReferenceToUndefinedRule) {
    // A grammar with a reference to an undefined rule (should fail)
    peg::parser parser(R"(
        ROOT <- UNDEFINED
    )");
    // This should fail because UNDEFINED is not defined
    ASSERT_FALSE(static_cast<bool>(parser));
}

TEST(AssignIDToDefinitionTest, SelfReferentialRule) {
    // A grammar with a self-referential rule (recursive)
    peg::parser parser(R"(
        ROOT <- A
        A <- 'a' A / 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Enable AST to verify structure
    parser.enable_ast();
    
    // Parse input that matches the recursive rule
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("aaa", ast));
    
    // Verify the AST structure
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->name, "ROOT");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "A");
}

TEST(AssignIDToDefinitionTest, MultipleReferencesToSameRule) {
    // A grammar with multiple references to the same rule
    peg::parser parser(R"(
        ROOT <- A A
        A <- 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Enable AST to verify structure
    parser.enable_ast();
    
    // Parse input that matches the repeated rule
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("aa", ast));
    
    // Verify the AST structure
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->name, "ROOT");
    ASSERT_EQ(ast->nodes.size(), 2);
    ASSERT_EQ(ast->nodes[0]->name, "A");
    ASSERT_EQ(ast->nodes[1]->name, "A");
}

TEST(AssignIDToDefinitionTest, ReferenceInChoice) {
    // A grammar with a reference inside a choice
    peg::parser parser(R"(
        ROOT <- A / B
        A <- 'a'
        B <- 'b'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Enable AST to verify structure
    parser.enable_ast();
    
    // Parse input that matches the first choice
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("a", ast));
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->name, "ROOT");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "A");
    
    // Parse input that matches the second choice
    ASSERT_TRUE(parser.parse("b", ast));
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->name, "ROOT");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "B");
}

TEST(AssignIDToDefinitionTest, ReferenceWithSemanticAction) {
    // A grammar with a reference that has a semantic action attached
    peg::parser parser(R"(
        ROOT <- A
        A <- 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Attach a semantic action to the rule
    bool action_called = false;
    parser["A"] = [&](const peg::SemanticValues& vs) {
        action_called = true;
        return vs.token();
    };
    
    // Parse input
    ASSERT_TRUE(parser.parse("a"));
    ASSERT_TRUE(action_called);
}

TEST(AssignIDToDefinitionTest, ReferenceWithPackratParsing) {
    // A grammar with references and packrat parsing enabled
    peg::parser parser(R"(
        ROOT <- A
        A <- 'a' A / 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Enable packrat parsing
    parser.enable_packrat_parsing();
    
    // Enable AST to verify structure
    parser.enable_ast();
    
    // Parse input that matches the recursive rule
    std::shared_ptr<peg::Ast> ast;
    ASSERT_TRUE(parser.parse("aaaa", ast));
    
    // Verify the AST structure
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->name, "ROOT");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "A");
}