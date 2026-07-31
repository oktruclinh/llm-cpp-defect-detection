#include <gtest/gtest.h>
#include "peglib.h"

// Helper function to create a parser and call make_grammar
// This allows us to test the effects of make_grammar through the public API.
std::shared_ptr<peg::parser> create_and_make_grammar() {
    auto g = std::make_shared<peg::parser>();
    g->make_grammar();
    return g;
}

TEST(MakeGrammarTest, BasicGrammarStructure) {
    auto g = create_and_make_grammar();

    // Check if essential rules are present
    ASSERT_TRUE(g->exists("Grammar"));
    ASSERT_TRUE(g->exists("Definition"));
    ASSERT_TRUE(g->exists("Expression"));
    ASSERT_TRUE(g->exists("Identifier"));
    ASSERT_TRUE(g->exists("Literal"));
    ASSERT_TRUE(g->exists("Spacing"));
    ASSERT_TRUE(g->exists("EndOfFile"));

    // Verify that rule names are set correctly
    // This is an internal detail, but make_grammar explicitly sets them.
    // We can't directly access the rule objects, but we can check if parsing works.
    // The existence check above implicitly verifies names are set for lookup.
}

TEST(MakeGrammarTest, ParseSimpleGrammar) {
    auto g = create_and_make_grammar();

    // Test a very simple grammar definition
    const char* grammar_str = "A <- 'a' B\nB <- 'b'";
    bool ret = g->parse(grammar_str);
    ASSERT_TRUE(ret) << "Failed to parse a simple grammar string.";

    // After parsing, the parser should have new rules defined by the input grammar
    ASSERT_TRUE(g->exists("A"));
    ASSERT_TRUE(g->exists("B"));

    // Test if the newly parsed grammar can parse its own input
    auto parser_for_A = std::make_shared<peg::parser>();
    parser_for_A->make_grammar(); // Re-initialize with the default grammar rules
    parser_for_A->parse(grammar_str); // Parse the grammar string to define A and B

    // Now, use the parser_for_A to parse "ab"
    bool parse_result = parser_for_A->parse("ab", "A");
    ASSERT_TRUE(parse_result) << "Failed to parse 'ab' with the generated grammar.";

    parse_result = parser_for_A->parse("ac", "A");
    ASSERT_FALSE(parse_result) << "Incorrectly parsed 'ac' with the generated grammar.";
}

TEST(MakeGrammarTest, ParseGrammarWithComplexRules) {
    auto g = create_and_make_grammar();

    // Test a grammar with various PEG features: choice, sequence, optional, zero-or-more, one-or-more
    const char* complex_grammar_str = R"(
        Start <- (A / B)+ C?
        A     <- 'a' 'A'
        B     <- 'b' 'B'
        C     <- 'c'
    )";
    bool ret = g->parse(complex_grammar_str);
    ASSERT_TRUE(ret) << "Failed to parse a complex grammar string.";

    ASSERT_TRUE(g->exists("Start"));
    ASSERT_TRUE(g->exists("A"));
    ASSERT_TRUE(g->exists("B"));
    ASSERT_TRUE(g->exists("C"));

    auto parser_for_complex = std::make_shared<peg::parser>();
    parser_for_complex->make_grammar();
    parser_for_complex->parse(complex_grammar_str);

    ASSERT_TRUE(parser_for_complex->parse("aAbB", "Start"));
    ASSERT_TRUE(parser_for_complex->parse("bBaAc", "Start"));
    ASSERT_TRUE(parser_for_complex->parse("aA", "Start"));
    ASSERT_TRUE(parser_for_complex->parse("bB", "Start"));
    ASSERT_TRUE(parser_for_complex->parse("aAc", "Start"));
    ASSERT_FALSE(parser_for_complex->parse("c", "Start")) << "Should not parse 'c' alone.";
    ASSERT_FALSE(parser_for_complex->parse("a", "Start")) << "Should not parse 'a' alone.";
    ASSERT_FALSE(parser_for_complex->parse("", "Start")) << "Should not parse empty string.";
}

TEST(MakeGrammarTest, ParseGrammarWithLiteralsAndClasses) {
    auto g = create_and_make_grammar();

    const char* literal_class_grammar = R"(
        Expr <- Digit+ Letter*
        Digit <- [0-9]
        Letter <- [a-zA-Z]
    )";
    bool ret = g->parse(literal_class_grammar);
    ASSERT_TRUE(ret) << "Failed to parse grammar with literals and classes.";

    auto parser_for_lc = std::make_shared<peg::parser>();
    parser_for_lc->make_grammar();
    parser_for_lc->parse(literal_class_grammar);

    ASSERT_TRUE(parser_for_lc->parse("123abc", "Expr"));
    ASSERT_TRUE(parser_for_lc->parse("456", "Expr"));
    ASSERT_TRUE(parser_for_lc->parse("7", "Expr"));
    ASSERT_TRUE(parser_for_lc->parse("8def", "Expr"));
    ASSERT_FALSE(parser_for_lc->parse("abc", "Expr")) << "Should not parse 'abc' (no digits).";
    ASSERT_FALSE(parser_for_lc->parse("", "Expr")) << "Should not parse empty string.";
}

TEST(MakeGrammarTest, ParseGrammarWithCommentsAndSpacing) {
    auto g = create_and_make_grammar();

    const char* comment_spacing_grammar = R"(
        # This is a comment
        Start <- 'hello' # Another comment
                 'world'
        # End of grammar
    )";
    bool ret = g->parse(comment_spacing_grammar);
    ASSERT_TRUE(ret) << "Failed to parse grammar with comments and spacing.";

    auto parser_for_cs = std::make_shared<peg::parser>();
    parser_for_cs->make_grammar();
    parser_for_cs->parse(comment_spacing_grammar);

    ASSERT_TRUE(parser_for_cs->parse("helloworld", "Start"));
    ASSERT_FALSE(parser_for_cs->parse("hello world", "Start")) << "Spacing between literals should not be ignored by default.";
}

TEST(MakeGrammarTest, ParseGrammarWithActions) {
    auto g = create_and_make_grammar();

    // Test grammar with capture and ignore
    const char* action_grammar = R"(
        Start <- ~'ignore' <'capture'>
    )";
    bool ret = g->parse(action_grammar);
    ASSERT_TRUE(ret) << "Failed to parse grammar with actions.";

    auto parser_for_action = std::make_shared<peg::parser>();
    parser_for_action->make_grammar();
    parser_for_action->parse(action_grammar);

    std::string captured_text;
    parser_for_action->set_action("capture", [&](const char* s, size_t l, peg::semantic_value& sv) {
        captured_text = std::string(s, l);
    });

    ASSERT_TRUE(parser_for_action->parse("ignorecapture", "Start"));
    ASSERT_EQ(captured_text, "capture") << "Incorrect text captured.";

    // Test with BeginCap/EndCap
    const char* cap_grammar = R"(
        Start <- $<Ident> <'value'>
        Ident <- [a-z]+
    )";
    ret = g->parse(cap_grammar);
    ASSERT_TRUE(ret) << "Failed to parse grammar with BeginCap/EndCap.";

    auto parser_for_cap = std::make_shared<peg::parser>();
    parser_for_cap->make_grammar();
    parser_for_cap->parse(cap_grammar);

    std::string ident_val;
    std::string value_val;

    parser_for_cap->set_action("Ident", [&](const char* s, size_t l, peg::semantic_value& sv) {
        ident_val = std::string(s, l);
    });
    parser_for_cap->set_action("value", [&](const char* s, size_t l, peg::semantic_value& sv) {
        value_val = std::string(s, l);
    });

    ASSERT_TRUE(parser_for_cap->parse("myidvalue", "Start"));
    ASSERT_EQ(ident_val, "myid"); // The Ident rule itself captures 'myid'
    ASSERT_EQ(value_val, "value"); // The 'value' literal captures 'value'
}

TEST(MakeGrammarTest, ParseInvalidGrammar) {
    auto g = create_and_make_grammar();

    // Test an invalid grammar (syntax error)
    const char* invalid_grammar_str = "A <- 'a' B\nB < 'b'"; // Missing LEFTARROW
    bool ret = g->parse(invalid_grammar_str);
    ASSERT_FALSE(ret) << "Incorrectly parsed an invalid grammar string.";

    // Test another invalid grammar (unmatched parenthesis)
    const char* invalid_grammar_paren = "A <- ('a'";
    ret = g->parse(invalid_grammar_paren);
    ASSERT_FALSE(ret) << "Incorrectly parsed grammar with unmatched parenthesis.";

    // Test with undefined rule in the grammar definition itself
    const char* undefined_rule_grammar = "A <- B\nC <- 'c'"; // B is not defined
    ret = g->parse(undefined_rule_grammar);
    // The make_grammar itself defines the PEG grammar, so it should parse the *syntax*
    // of the input grammar. Semantic errors like undefined rules are typically caught
    // when the *generated* parser is used, or if the PEG library has a semantic check phase.
    // For peglib, the parse function only checks the syntax of the grammar definition.
    // So, this should pass if the syntax is valid, even if 'B' is not defined.
    ASSERT_TRUE(ret) << "Failed to parse syntactically valid grammar with undefined rule.";

    // To check for undefined rules, we'd need to try and use the resulting parser.
    auto parser_with_undefined = std::make_shared<peg::parser>();
    parser_with_undefined->make_grammar();
    parser_with_undefined->parse(undefined_rule_grammar);
    ASSERT_FALSE(parser_with_undefined->parse("c", "A")) << "Should fail to parse 'A' due to undefined 'B'.";
    ASSERT_TRUE(parser_with_undefined->parse("c", "C")) << "Should parse 'C' successfully.";
}

TEST(MakeGrammarTest, RuleNameCollision) {
    auto g = create_and_make_grammar();

    // Define a grammar where a rule name is redefined
    const char* colliding_grammar = R"(
        A <- 'a'
        A <- 'b'
    )";
    bool ret = g->parse(colliding_grammar);
    // peglib allows redefinition, the last one wins.
    ASSERT_TRUE(ret) << "Failed to parse grammar with rule redefinition.";

    auto parser_for_collision = std::make_shared<peg::parser>();
    parser_for_collision->make_grammar();
    parser_for_collision->parse(colliding_grammar);

    // The last definition 'A <- 'b'' should be active
    ASSERT_FALSE(parser_for_collision->parse("a", "A")) << "First definition of A should be overwritten.";
    ASSERT_TRUE(parser_for_collision->parse("b", "A")) << "Second definition of A should be active.";
}

TEST(MakeGrammarTest, EmptyGrammarString) {
    auto g = create_and_make_grammar();

    const char* empty_grammar = "";
    bool ret = g->parse(empty_grammar);
    ASSERT_TRUE(ret) << "Failed to parse an empty grammar string."; // An empty string is valid PEG syntax (no rules).

    // After parsing an empty grammar, the parser should still have its initial 'make_grammar' rules
    ASSERT_TRUE(g->exists("Grammar"));
    ASSERT_FALSE(g->exists("MyRule")); // No new rules should be added
}

TEST(MakeGrammarTest, GrammarWithOnlyComments) {
    auto g = create_and_make_grammar();

    const char* comments_only_grammar = "# This is a comment\n# Another comment";
    bool ret = g->parse(comments_only_grammar);
    ASSERT_TRUE(ret) << "Failed to parse a grammar string with only comments.";

    ASSERT_TRUE(g->exists("Grammar")); // Still has the base grammar rules
    ASSERT_FALSE(g->exists("MyRule")); // No new rules should be added
}