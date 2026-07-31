#include <gtest/gtest.h>
#include <peglib.h>

// Helper function to create a parser from a grammar string
std::shared_ptr<peg::Parser> create_parser(const char* grammar) {
    auto parser = std::make_shared<peg::Parser>();
    if (!parser->load_grammar(grammar)) {
        return nullptr;
    }
    return parser;
}

TEST(DetectLeftRecursionTest, DirectLeftRecursion) {
    // A grammar with direct left recursion: A <- A 'a' | 'b'
    const char* grammar = "A <- A 'a' / 'b'";
    auto parser = create_parser(grammar);
    ASSERT_NE(parser, nullptr);

    // Attempt to parse something. This should trigger the left recursion detection.
    // peglib's parse() method returns false if there's a parsing error,
    // which includes left recursion detection during grammar loading/preparation.
    // However, the actual detection happens during grammar loading.
    // We need to check the internal state or rely on the parser's behavior.
    // The parser should report an error if left recursion is detected.
    // The current peglib API doesn't expose a direct way to query if left recursion was detected
    // after load_grammar. The typical behavior is that parsing will fail or the grammar
    // will be considered invalid.
    // For this test, we'll rely on the fact that a parser with direct left recursion
    // will likely fail to parse or will be marked as invalid internally.
    // A more robust test would involve a direct API to check for left recursion.
    // Since we are testing the internal `DetectLeftRecursion::visit` function,
    // we assume that if `load_grammar` succeeds, it means no *unhandled* left recursion.
    // However, the `DetectLeftRecursion` visitor is specifically designed to *find* it.
    // The `load_grammar` method itself uses this visitor.

    // If peglib successfully loads a grammar with direct left recursion, it means
    // it has handled it (e.g., by transforming it or reporting an error).
    // The original `DetectLeftRecursion` visitor is used to *detect* it.
    // A successful `load_grammar` implies that the detection mechanism worked
    // and either reported an error or transformed the grammar.
    // In the context of the provided `DetectLeftRecursion::visit` function,
    // if it finds a left recursion, it sets `error_s`.

    // Let's try a grammar that *should* cause an error if left recursion isn't handled.
    // peglib's `load_grammar` returns false if there's a grammar error,
    // which includes unhandled left recursion.
    // The `DetectLeftRecursion` visitor is part of the process that determines
    // if the grammar is valid.

    // A direct left-recursive grammar should ideally fail to load or be transformed.
    // peglib's current implementation (as of the version this function is from)
    // detects direct left recursion and reports it as a grammar error.
    // So, `load_grammar` should return false.
    auto parser_direct_lr = std::make_shared<peg::Parser>();
    ASSERT_FALSE(parser_direct_lr->load_grammar(grammar))
        << "Direct left recursion was not detected or handled correctly.";
}

TEST(DetectLeftRecursionTest, IndirectLeftRecursion) {
    // A grammar with indirect left recursion: A <- B 'a', B <- A 'b' | 'c'
    const char* grammar = "A <- B 'a'\nB <- A 'b' / 'c'";
    auto parser = std::make_shared<peg::Parser>();
    ASSERT_FALSE(parser->load_grammar(grammar))
        << "Indirect left recursion was not detected or handled correctly.";
}

TEST(DetectLeftRecursionTest, NoLeftRecursion) {
    // A grammar with no left recursion: A <- 'a' A | 'b' (right recursion)
    const char* grammar_right_recursion = "A <- 'a' A / 'b'";
    auto parser_right_recursion = create_parser(grammar_right_recursion);
    ASSERT_NE(parser_right_recursion, nullptr);
    ASSERT_TRUE(parser_right_recursion->load_grammar(grammar_right_recursion))
        << "Valid right-recursive grammar failed to load.";

    // A simple grammar with no recursion
    const char* grammar_simple = "A <- 'a' 'b'";
    auto parser_simple = create_parser(grammar_simple);
    ASSERT_NE(parser_simple, nullptr);
    ASSERT_TRUE(parser_simple->load_grammar(grammar_simple))
        << "Valid simple grammar failed to load.";

    // Test parsing with the non-left-recursive grammar
    std::string result;
    ASSERT_TRUE(parser_right_recursion->parse("abb", result));
    ASSERT_EQ(result, "abb"); // Assuming default AST generation or simple match

    ASSERT_TRUE(parser_simple->parse("ab", result));
    ASSERT_EQ(result, "ab");
}

TEST(DetectLeftRecursionTest, SelfReferenceInDifferentRule) {
    // A grammar where a rule refers to itself, but not in a left-recursive way
    // A <- B, B <- A 'a' / 'b'
    // This is indirect left recursion, but the `DetectLeftRecursion::visit`
    // function is called for each reference.
    const char* grammar = "A <- B\nB <- A 'a' / 'b'";
    auto parser = std::make_shared<peg::Parser>();
    ASSERT_FALSE(parser->load_grammar(grammar))
        << "Indirect left recursion was not detected when a rule refers to another that refers back.";
}

TEST(DetectLeftRecursionTest, ComplexGrammarNoLeftRecursion) {
    // A more complex grammar without left recursion
    const char* grammar = R"(
        Program <- Statement+
        Statement <- Assignment / Print
        Assignment <- Identifier '=' Expression
        Print <- 'print' Expression
        Expression <- Term (('+' / '-') Term)*
        Term <- Factor (('*' / '/') Factor)*
        Factor <- Number / Identifier / '(' Expression ')'
        Number <- [0-9]+
        Identifier <- [a-zA-Z_][a-zA-Z0-9_]*
    )";
    auto parser = create_shared_ptr<peg::Parser>();
    ASSERT_TRUE(parser->load_grammar(grammar))
        << "Complex valid grammar failed to load.";

    // Test parsing a simple expression
    std::string result;
    ASSERT_TRUE(parser->parse("x = 1 + 2 * 3", result));
    ASSERT_TRUE(parser->parse("print (y_var + 10)", result));
}

TEST(DetectLeftRecursionTest, EmptyGrammar) {
    // An empty grammar should load successfully but won't parse anything.
    const char* grammar = "";
    auto parser = std::make_shared<peg::Parser>();
    ASSERT_TRUE(parser->load_grammar(grammar))
        << "Empty grammar failed to load.";
    std::string result;
    ASSERT_FALSE(parser->parse("a", result)); // Should not parse anything
}

TEST(DetectLeftRecursionTest, GrammarWithUndefinedReference) {
    // A grammar with an undefined reference should fail to load,
    // but this is a different error than left recursion.
    const char* grammar = "A <- B\n"; // B is undefined
    auto parser = std::make_shared<peg::Parser>();
    ASSERT_FALSE(parser->load_grammar(grammar))
        << "Grammar with undefined reference should fail to load.";
}