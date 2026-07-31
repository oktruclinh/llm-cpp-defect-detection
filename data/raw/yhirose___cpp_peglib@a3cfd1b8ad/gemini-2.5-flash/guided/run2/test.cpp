#include <gtest/gtest.h>
#include "peglib.h"

// Helper function to create a parser and call make_grammar
// This allows us to test the effects of make_grammar through the public API
std::shared_ptr<peg::parser> create_and_make_grammar() {
    auto g = std::make_shared<peg::parser>();
    g->make_grammar();
    return g;
}

TEST(MakeGrammarTest, BasicGrammarRulesExist) {
    auto g = create_and_make_grammar();

    // Check if some fundamental rules are present
    ASSERT_TRUE(g->exists("Grammar"));
    ASSERT_TRUE(g->exists("Definition"));
    ASSERT_TRUE(g->exists("Expression"));
    ASSERT_TRUE(g->exists("Identifier"));
    ASSERT_TRUE(g->exists("Literal"));
    ASSERT_TRUE(g->exists("Spacing"));
    ASSERT_TRUE(g->exists("EndOfFile"));
}

TEST(MakeGrammarTest, GrammarRuleStructure) {
    auto g = create_and_make_grammar();

    // Test the 'Grammar' rule by trying to parse a simple valid grammar
    // This implicitly tests 'Definition', 'Identifier', 'LEFTARROW', 'Expression', etc.
    // We can't directly inspect the internal structure of the rules easily,
    // so we test by parsing valid and invalid inputs.

    // A very simple valid grammar
    std::string valid_grammar_input = "A <- 'a' B <- 'b' # comment\n";
    ASSERT_TRUE(g->parse(valid_grammar_input, "Grammar")) << "Failed to parse a simple valid grammar.";

    // Grammar with a more complex expression
    std::string complex_expr_grammar = "A <- 'a' / 'b' B <- ('c' 'd')*";
    ASSERT_TRUE(g->parse(complex_expr_grammar, "Grammar")) << "Failed to parse grammar with complex expression.";

    // Grammar with character classes
    std::string char_class_grammar = "A <- [a-z] B <- [0-9]+";
    ASSERT_TRUE(g->parse(char_class_grammar, "Grammar")) << "Failed to parse grammar with character classes.";

    // Grammar with ignore prefix
    std::string ignore_grammar = "~A <- 'a'";
    ASSERT_TRUE(g->parse(ignore_grammar, "Grammar")) << "Failed to parse grammar with ignore prefix.";
}

TEST(MakeGrammarTest, GrammarRuleFailureCases) {
    auto g = create_and_make_grammar();

    // Invalid grammar: missing LEFTARROW
    std::string invalid_grammar_no_arrow = "A 'a'";
    ASSERT_FALSE(g->parse(invalid_grammar_no_arrow, "Grammar")) << "Parsed invalid grammar (missing LEFTARROW).";

    // Invalid grammar: malformed identifier
    std::string invalid_grammar_malformed_id = "1A <- 'a'";
    ASSERT_FALSE(g->parse(invalid_grammar_malformed_id, "Grammar")) << "Parsed invalid grammar (malformed identifier).";

    // Invalid grammar: unclosed literal
    std::string invalid_grammar_unclosed_literal = "A <- 'a";
    ASSERT_FALSE(g->parse(invalid_grammar_unclosed_literal, "Grammar")) << "Parsed invalid grammar (unclosed literal).";

    // Invalid grammar: unclosed parenthesis
    std::string invalid_grammar_unclosed_paren = "A <- ('a'";
    ASSERT_FALSE(g->parse(invalid_grammar_unclosed_paren, "Grammar")) << "Parsed invalid grammar (unclosed parenthesis).";
}

TEST(MakeGrammarTest, SpacingAndCommentHandling) {
    auto g = create_and_make_grammar();

    // Test spacing
    std::string spaced_grammar = "  A   <-   'a'  ";
    ASSERT_TRUE(g->parse(spaced_grammar, "Grammar")) << "Failed to parse grammar with excessive spacing.";

    // Test comments
    std::string commented_grammar = "# This is a comment\nA <- 'a' # Another comment\nB <- 'b'";
    ASSERT_TRUE(g->parse(commented_grammar, "Grammar")) << "Failed to parse grammar with comments.";

    // Test mixed spacing and comments
    std::string mixed_grammar = " A <- 'a' # comment\n B <- 'b' ";
    ASSERT_TRUE(g->parse(mixed_grammar, "Grammar")) << "Failed to parse grammar with mixed spacing and comments.";
}

TEST(MakeGrammarTest, EndOfFileHandling) {
    auto g = create_and_make_grammar();

    // Valid grammar ending with EOF
    std::string valid_eof_grammar = "A <- 'a'";
    ASSERT_TRUE(g->parse(valid_eof_grammar, "Grammar")) << "Failed to parse grammar ending correctly.";

    // Grammar with trailing garbage (should fail EOF)
    std::string trailing_garbage_grammar = "A <- 'a' Z";
    ASSERT_FALSE(g->parse(trailing_garbage_grammar, "Grammar")) << "Parsed grammar with trailing garbage.";
}

TEST(MakeGrammarTest, LiteralAndCharRules) {
    auto g = create_and_make_grammar();

    // Test various literal forms
    std::string literal_grammar = R"(A <- 'a' B <- "b" C <- '\n' D <- '\x41' E <- '\0' F <- '\123')";
    ASSERT_TRUE(g->parse(literal_grammar, "Grammar")) << "Failed to parse grammar with various literal types.";

    // Invalid literal (unrecognized escape sequence) - this might be parsed as two separate chars
    // The current grammar for Char allows any char not preceded by backslash if not ']' or '"'
    // This test might be tricky as the grammar might accept it as two chars.
    // Let's test a clearly malformed one.
    std::string invalid_literal = R"(A <- '\z')"; // '\z' is not a valid escape
    ASSERT_FALSE(g->parse(invalid_literal, "Grammar")) << "Parsed grammar with invalid escape sequence in literal.";
}

TEST(MakeGrammarTest, ClassRules) {
    auto g = create_and_make_grammar();

    // Test various class forms
    std::string class_grammar = R"(A <- [a-z] B <- [0-9A-F] C <- [^a-z] D <- [a-zA-Z_])";
    ASSERT_TRUE(g->parse(class_grammar, "Grammar")) << "Failed to parse grammar with various class types.";

    // Invalid class (unclosed)
    std::string invalid_class = R"(A <- [a-z)";
    ASSERT_FALSE(g->parse(invalid_class, "Grammar")) << "Parsed grammar with unclosed class.";

    // Invalid class (malformed range) - e.g., [z-a] is not explicitly forbidden by the grammar
    // but might lead to unexpected behavior. The grammar allows `Char '-' Char`.
    // Let's test a simple unclosed one.
    std::string malformed_class = R"(A <- [a-)"; // Malformed range
    ASSERT_FALSE(g->parse(malformed_class, "Grammar")) << "Parsed grammar with malformed class range.";
}

TEST(MakeGrammarTest, SpecialOperators) {
    auto g = create_and_make_grammar();

    // Test AND, NOT, QUESTION, STAR, PLUS, DOT
    std::string operators_grammar = "A <- &B B <- !C C <- D? D <- E* E <- F+ F <- .";
    ASSERT_TRUE(g->parse(operators_grammar, "Grammar")) << "Failed to parse grammar with special operators.";

    // Test combined operators
    std::string combined_operators_grammar = "A <- ('a' 'b')? B <- ('c' / 'd')+ C <- &'e'";
    ASSERT_TRUE(g->parse(combined_operators_grammar, "Grammar")) << "Failed to parse grammar with combined operators.";
}

TEST(MakeGrammarTest, CaptureOperators) {
    auto g = create_and_make_grammar();

    // Test Begin/End and BeginCap/EndCap
    std::string capture_grammar = "A <- <'a'> B <- $id<'b'> C <- $<'c'>";
    ASSERT_TRUE(g->parse(capture_grammar, "Grammar")) << "Failed to parse grammar with capture operators.";

    // Invalid capture (unclosed)
    std::string invalid_capture = "A <- <'a'";
    ASSERT_FALSE(g->parse(invalid_capture, "Grammar")) << "Parsed grammar with unclosed capture.";

    // Invalid capture (malformed identifier in BeginCap)
    std::string invalid_cap_id = "A <- $1id<'a'>";
    ASSERT_FALSE(g->parse(invalid_cap_id, "Grammar")) << "Parsed grammar with malformed identifier in BeginCap.";
}

TEST(MakeGrammarTest, RuleNameAssignment) {
    auto g = create_and_make_grammar();

    // Verify that rule names are assigned correctly
    // This is an internal detail, but we can check if the parser can find rules by name.
    // The `exists` checks already cover this to some extent.
    // A more direct check would require inspecting internal parser state, which is not public API.
    // The fact that `g->parse` works with rule names implies they are correctly set.
    ASSERT_TRUE(g->exists("Grammar"));
    ASSERT_TRUE(g->exists("Definition"));
    ASSERT_TRUE(g->exists("Spacing"));
    ASSERT_TRUE(g->exists("LEFTARROW")); // Terminal rules also get names
}

TEST(MakeGrammarTest, EmptyInput) {
    auto g = create_and_make_grammar();

    // An empty string is not a valid grammar
    std::string empty_input = "";
    ASSERT_FALSE(g->parse(empty_input, "Grammar")) << "Parsed an empty string as a valid grammar.";
}

TEST(MakeGrammarTest, OnlySpacingInput) {
    auto g = create_and_make_grammar();

    // Input with only spacing and comments is not a valid grammar
    std::string only_spacing = "   # comment\n\t";
    ASSERT_FALSE(g->parse(only_spacing, "Grammar")) << "Parsed only spacing/comments as a valid grammar.";
}