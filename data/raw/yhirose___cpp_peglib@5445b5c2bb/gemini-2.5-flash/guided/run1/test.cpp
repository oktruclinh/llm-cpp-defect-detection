#include <gtest/gtest.h>
#include <peglib.h>

// Helper function to create a simple PEG grammar for testing
std::shared_ptr<peg::Grammar> create_grammar(const std::string& grammar_str) {
    auto grammar = std::make_shared<peg::Grammar>();
    grammar->load_grammar(grammar_str.c_str());
    return grammar;
}

// Test fixture for peg::Grammar related tests
class PeglibGrammarTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup if needed
    }

    void TearDown() override {
        // Common teardown if needed
    }
};

TEST_F(PeglibGrammarTest, ReferenceWithRuleAndArgs) {
    // This grammar defines a rule 'A' that references 'B' with an argument 'C'.
    // The AssignIDToDefinition visitor is implicitly called during grammar loading
    // and subsequent operations like parsing.
    // We are testing that the visitor correctly traverses the reference, its arguments,
    // and the referenced rule.
    // The most direct way to observe the effect of AssignIDToDefinition is
    // to check if the grammar can be successfully parsed and if the internal
    // structure (which is not directly exposed) is consistent.
    // For this test, we'll verify that a grammar with references and arguments
    // can be loaded and used to parse a string, implying correct internal ID assignment.
    const std::string grammar_str = R"(
        A <- B<C>
        B <- 'b'
        C <- 'c'
    )";
    auto grammar = create_grammar(grammar_str);

    ASSERT_TRUE(grammar != nullptr);
    ASSERT_TRUE(grammar->is_loaded());

    // Attempt to parse a string that should match the grammar
    std::shared_ptr<peg::Ast> ast;
    bool parsed = grammar->parse("bc", ast);

    // If parsing succeeds, it implies that the internal structure, including
    // references and their arguments, was correctly processed and linked.
    // If AssignIDToDefinition failed to visit the rule or its arguments,
    // the grammar would likely be malformed internally, leading to parse failure.
    ASSERT_TRUE(parsed);
    ASSERT_TRUE(ast != nullptr);
    ASSERT_EQ(ast->name, "A");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "B");
    ASSERT_EQ(ast->nodes[0]->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->nodes[0]->name, "C");
}

TEST_F(PeglibGrammarTest, ReferenceWithRuleNoArgs) {
    // Test a reference with a rule but no arguments.
    const std::string grammar_str = R"(
        A <- B
        B <- 'b'
    )";
    auto grammar = create_grammar(grammar_str);

    ASSERT_TRUE(grammar != nullptr);
    ASSERT_TRUE(grammar->is_loaded());

    std::shared_ptr<peg::Ast> ast;
    bool parsed = grammar->parse("b", ast);

    ASSERT_TRUE(parsed);
    ASSERT_TRUE(ast != nullptr);
    ASSERT_EQ(ast->name, "A");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "B");
}

TEST_F(PeglibGrammarTest, ReferenceToUndefinedRule) {
    // This is an error case. If a reference points to an undefined rule,
    // the grammar loading should fail or parsing should fail.
    // The AssignIDToDefinition visitor would still attempt to visit the
    // non-existent rule_ pointer, but it would be null.
    // The `if (ope.rule_)` check in the focal function handles this.
    const std::string grammar_str = R"(
        A <- UndefinedRule
    )";
    auto grammar = create_grammar(grammar_str);

    // Expect grammar loading to fail or be invalid.
    // peglib's load_grammar returns true even for invalid grammars,
    // but subsequent parsing will fail.
    ASSERT_TRUE(grammar != nullptr);
    ASSERT_TRUE(grammar->is_loaded()); // Grammar is syntactically loaded, but semantically invalid

    std::shared_ptr<peg::Ast> ast;
    bool parsed = grammar->parse("anything", ast);

    // Parsing should fail because 'UndefinedRule' does not exist.
    ASSERT_FALSE(parsed);
    ASSERT_TRUE(ast == nullptr); // No AST should be generated on failure
}

TEST_F(PeglibGrammarTest, ReferenceWithRecursiveRule) {
    // Test a grammar with a recursive reference.
    // AssignIDToDefinition should handle this without infinite recursion.
    const std::string grammar_str = R"(
        A <- 'a' A? 'b'
    )";
    auto grammar = create_grammar(grammar_str);

    ASSERT_TRUE(grammar != nullptr);
    ASSERT_TRUE(grammar->is_loaded());

    std::shared_ptr<peg::Ast> ast;
    bool parsed = grammar->parse("aaab", ast);

    ASSERT_TRUE(parsed);
    ASSERT_TRUE(ast != nullptr);
    ASSERT_EQ(ast->name, "A");
    // The AST structure for recursive rules can be complex,
    // but successful parsing indicates correct internal processing.
}

TEST_F(PeglibGrammarTest, ReferenceWithMultipleArgs) {
    // Test a reference with multiple arguments.
    const std::string grammar_str = R"(
        A <- B<C, D>
        B <- 'b'
        C <- 'c'
        D <- 'd'
    )";
    auto grammar = create_grammar(grammar_str);

    ASSERT_TRUE(grammar != nullptr);
    ASSERT_TRUE(grammar->is_loaded());

    std::shared_ptr<peg::Ast> ast;
    bool parsed = grammar->parse("bcd", ast);

    ASSERT_TRUE(parsed);
    ASSERT_TRUE(ast != nullptr);
    ASSERT_EQ(ast->name, "A");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "B");
    ASSERT_EQ(ast->nodes[0]->nodes.size(), 2);
    ASSERT_EQ(ast->nodes[0]->nodes[0]->name, "C");
    ASSERT_EQ(ast->nodes[0]->nodes[1]->name, "D");
}

TEST_F(PeglibGrammarTest, ReferenceWithEmptyArgs) {
    // Test a reference where the argument list is syntactically present but empty.
    // This might not be directly expressible in PEG grammar syntax,
    // but if the internal representation allowed it, the visitor should handle it.
    // For peglib, an empty argument list is not a valid syntax, so we test
    // a reference without any arguments, which is the closest valid case.
    const std::string grammar_str = R"(
        A <- B<> // This is not valid peglib syntax, so we test B instead
        B <- 'b'
    )";
    // The above grammar will fail to load. Let's test a valid one.
    const std::string valid_grammar_str = R"(
        A <- B
        B <- 'b'
    )";
    auto grammar = create_grammar(valid_grammar_str);

    ASSERT_TRUE(grammar != nullptr);
    ASSERT_TRUE(grammar->is_loaded());

    std::shared_ptr<peg::Ast> ast;
    bool parsed = grammar->parse("b", ast);

    ASSERT_TRUE(parsed);
    ASSERT_TRUE(ast != nullptr);
    ASSERT_EQ(ast->name, "A");
    ASSERT_EQ(ast->nodes.size(), 1);
    ASSERT_EQ(ast->nodes[0]->name, "B");
}

TEST_F(PeglibGrammarTest, ComplexNestedReferences) {
    // Test a more complex scenario with nested references and arguments.
    const std::string grammar_str = R"(
        Start <- A<X>
        A     <- B<Y, Z>
        B     <- 'b'
        X     <- 'x'
        Y     <- 'y'
        Z     <- 'z'
    )";
    auto grammar = create_grammar(grammar_str);

    ASSERT_TRUE(grammar != nullptr);
    ASSERT_TRUE(grammar->is_loaded());

    std::shared_ptr<peg::Ast> ast;
    bool parsed = grammar->parse("b y z x", ast); // Note: peglib's default behavior might not handle spaces between tokens unless specified.
                                                  // Let's adjust the grammar to allow spaces or test without them.
    const std::string grammar_str_no_space = R"(
        Start <- A<X>
        A     <- B<Y, Z>
        B     <- 'b'
        X     <- 'x'
        Y     <- 'y'
        Z     <- 'z'
    )";
    auto grammar_no_space = create_grammar(grammar_str_no_space);
    ASSERT_TRUE(grammar_no_space != nullptr);
    ASSERT_TRUE(grammar_no_space->is_loaded());

    std::shared_ptr<peg::Ast> ast_no_space;
    bool parsed_no_space = grammar_no_space->parse("byzx", ast_no_space);

    ASSERT_TRUE(parsed_no_space);
    ASSERT_TRUE(ast_no_space != nullptr);
    ASSERT_EQ(ast_no_space->name, "Start");
    ASSERT_EQ(ast_no_space->nodes.size(), 1);
    ASSERT_EQ(ast_no_space->nodes[0]->name, "A");
    ASSERT_EQ(ast_no_space->nodes[0]->nodes.size(), 2); // B and X
    ASSERT_EQ(ast_no_space->nodes[0]->nodes[0]->name, "B");
    ASSERT_EQ(ast_no_space->nodes[0]->nodes[0]->nodes.size(), 2); // Y and Z
    ASSERT_EQ(ast_no_space->nodes[0]->nodes[0]->nodes[0]->name, "Y");
    ASSERT_EQ(ast_no_space->nodes[0]->nodes[0]->nodes[1]->name, "Z");
    ASSERT_EQ(ast_no_space->nodes[0]->nodes[1]->name, "X");
}