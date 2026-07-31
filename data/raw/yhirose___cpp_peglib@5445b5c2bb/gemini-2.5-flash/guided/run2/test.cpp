#include <gtest/gtest.h>
#include <peglib.h>

// This test suite focuses on the behavior of the AssignIDToDefinition visitor
// when visiting a Reference node, specifically ensuring that it correctly
// traverses its arguments and the referenced rule.
// The AssignIDToDefinition visitor is an internal component used during
// the parsing and compilation of PEG grammars to assign unique IDs to
// definitions, which is crucial for optimizing the parsing process.
// We will test this behavior indirectly by parsing a grammar and then
// inspecting the internal state (if accessible via public API or by
// observing side effects) or by verifying that the grammar can be
// successfully compiled and used.

// Helper function to create a simple PEG grammar for testing
std::shared_ptr<peg::parser> create_grammar(const std::string& grammar_str) {
    auto parser = std::make_shared<peg::parser>(grammar_str.c_str());
    if (!static_cast<bool>(*parser)) { // Check if parser creation was successful
        return nullptr;
    }
    return parser;
}

TEST(AssignIDToDefinitionTest, ReferenceWithRuleAndArgs) {
    // Grammar: A <- B(C)
    // Here, 'B(C)' is a Reference to rule 'B' with argument 'C'.
    // The visitor should traverse 'C' and then 'B'.
    const std::string grammar_str = R"(
        A <- B(C)
        B(X) <- X
        C <- 'c'
    )";

    auto parser = create_grammar(grammar_str);
    ASSERT_NE(parser, nullptr) << "Failed to load grammar: " << grammar_str;

    // The AssignIDToDefinition visitor is implicitly run when the grammar is loaded
    // and prepared for parsing. If it fails to assign IDs correctly,
    // subsequent operations like parsing or AST generation might fail or produce
    // incorrect results.

    // Test a successful parse to confirm the grammar is correctly processed.
    // This indirectly verifies that internal structures, including references,
    // were correctly handled by the AssignIDToDefinition visitor.
    
    std::shared_ptr<peg::Ast> ast;
    bool ret = parser->parse("c", ast);
    ASSERT_TRUE(ret) << "Parsing 'c' failed.";
    ASSERT_NE(ast, nullptr) << "AST should not be null after successful parse.";

    // Verify the structure of the AST to ensure the reference was resolved correctly.
    // The AST for "A <- B(C)" parsing "c" should essentially resolve to 'c'.
    ASSERT_EQ(ast->name, "A");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "C");
    ASSERT_EQ(ast->nodes[0]->token, "c");
}

TEST(AssignIDToDefinitionTest, ReferenceWithRuleNoArgs) {
    // Grammar: A <- B
    // Here, 'B' is a Reference to rule 'B' with no arguments.
    // The visitor should traverse 'B'.
    const std::string grammar_str = R"(
        A <- B
        B <- 'b'
    )";

    auto parser = create_grammar(grammar_str);
    ASSERT_NE(parser, nullptr) << "Failed to load grammar: " << grammar_str;

    std::shared_ptr<peg::Ast> ast;
    bool ret = parser->parse("b", ast);
    ASSERT_TRUE(ret) << "Parsing 'b' failed.";
    ASSERT_NE(ast, nullptr) << "AST should not be null after successful parse.";

    ASSERT_EQ(ast->name, "A");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "B");
    ASSERT_EQ(ast->nodes[0]->token, "b");
}

TEST(AssignIDToDefinitionTest, ReferenceWithMultipleArgs) {
    // Grammar: A <- B(C, D)
    // The visitor should traverse 'C', then 'D', then 'B'.
    const std::string grammar_str = R"(
        A <- B(C, D)
        B(X, Y) <- X Y
        C <- 'c'
        D <- 'd'
    )";

    auto parser = create_grammar(grammar_str);
    ASSERT_NE(parser, nullptr) << "Failed to load grammar: " << grammar_str;

    std::shared_ptr<peg::Ast> ast;
    bool ret = parser->parse("cd", ast);
    ASSERT_TRUE(ret) << "Parsing 'cd' failed.";
    ASSERT_NE(ast, nullptr) << "AST should not be null after successful parse.";

    ASSERT_EQ(ast->name, "A");
    ASSERT_EQ(ast->nodes.size(), 1); // B(C,D) -> C D
    ASSERT_EQ(ast->nodes[0]->name, "B");
    ASSERT_EQ(ast->nodes[0]->nodes.size(), 2);
    ASSERT_EQ(ast->nodes[0]->nodes[0]->name, "C");
    ASSERT_EQ(ast->nodes[0]->nodes[0]->token, "c");
    ASSERT_EQ(ast->nodes[0]->nodes[1]->name, "D");
    ASSERT_EQ(ast->nodes[0]->nodes[1]->token, "d");
}

TEST(AssignIDToDefinitionTest, ReferenceToUndefinedRule) {
    // Grammar: A <- UndefinedRule
    // This is an error case. The AssignIDToDefinition visitor expects 'ope.rule_'
    // to be valid. If it's not, it might indicate a malformed grammar or an
    // internal inconsistency.
    // peglib's `load_grammar` should ideally catch this and return false.
    const std::string grammar_str = R"(
        A <- UndefinedRule
    )";

    // Expect parser creation to fail because 'UndefinedRule' is not defined.
    auto parser = create_grammar(grammar_str);
    ASSERT_EQ(parser, nullptr)
        << "Creating parser with undefined rule should fail.";
}

TEST(AssignIDToDefinitionTest, ReferenceWithRecursiveRule) {
    // Grammar: A <- B / 'a', B <- A
    // This tests if the visitor can handle recursive references without infinite loops
    // or stack overflows during the ID assignment phase.
    const std::string grammar_str = R"(
        A <- B / 'a'
        B <- A
    )";

    auto parser = create_grammar(grammar_str);
    ASSERT_NE(parser, nullptr) << "Failed to load grammar: " << grammar_str;

    std::shared_ptr<peg::Ast> ast;
    bool ret = parser->parse("a", ast);
    ASSERT_TRUE(ret) << "Parsing 'a' failed with recursive grammar.";
    ASSERT_NE(ast, nullptr) << "AST should not be null after successful parse.";

    ASSERT_EQ(ast->name, "A");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "a"); // The 'a' terminal
}

TEST(AssignIDToDefinitionTest, ReferenceInComplexGrammar) {
    // Test with a slightly more complex grammar involving multiple references
    // and nested structures.
    const std::string grammar_str = R"(
        Start <- Expr
        Expr  <- Term (('+' / '-') Term)*
        Term  <- Factor (('*' / '/') Factor)*
        Factor <- Number / '(' Expr ')'
        Number <- [0-9]+
    )";

    auto parser = create_grammar(grammar_str);
    ASSERT_NE(parser, nullptr) << "Failed to load grammar: " << grammar_str;

    std::shared_ptr<peg::Ast> ast;
    bool ret = parser->parse("1 + (2 * 3)", ast);
    ASSERT_TRUE(ret) << "Parsing '1 + (2 * 3)' failed.";
    ASSERT_NE(ast, nullptr) << "AST should not be null after successful parse.";

    // Basic AST structure verification to ensure parsing worked,
    // implying correct ID assignment and traversal.
    ASSERT_EQ(ast->name, "Start");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "Expr");
    // Further detailed AST checks would be extensive, but the successful parse
    // and basic structure confirm the underlying grammar processing.
}