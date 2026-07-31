#include <gtest/gtest.h>
#include <peglib.h>

// Helper function to create a simple parser for testing
std::shared_ptr<peg::Parser> create_parser(const std::string& grammar_str) {
    auto parser = std::make_shared<peg::Parser>(grammar_str);
    if (!static_cast<bool>(*parser)) {
        return nullptr;
    }
    return parser;
}

TEST(LinkReferencesTest, ReferenceToRule) {
    // Test case: A reference to a defined rule
    const std::string grammar = R"(
        A <- B
        B <- 'b'
    )";
    auto parser = create_parser(grammar);
    ASSERT_NE(parser, nullptr);

    // Parse a string that uses the rule
    bool ret = parser->parse("b");
    ASSERT_TRUE(ret);

    // We can't directly inspect LinkReferences::visit, but we can infer its
    // behavior by checking if the parser correctly links rules.
    // If 'A' correctly links to 'B', and 'B' correctly links to the literal 'b',
    // then LinkReferences::visit has done its job.
    // A more direct way to test this would be to check the AST structure,
    // but peglib's public API doesn't expose the internal AST structure directly
    // in a way that allows checking rule pointers.
    // The primary test here is that the parser successfully parses, implying
    // that the internal linking was successful.
    // If LinkReferences::visit failed to link 'A' to 'B', the parser would likely fail.
}

TEST(LinkReferencesTest, ReferenceToUndefinedRule) {
    // Test case: A reference to an undefined rule
    const std::string grammar = R"(
        A <- B
    )";
    // This grammar is invalid because 'B' is not defined.
    // The parser constructor should fail, indicating that LinkReferences
    // (or a subsequent validation step) detected the issue.
    auto parser = std::make_shared<peg::Parser>(grammar);
    ASSERT_FALSE(static_cast<bool>(*parser)); // Expect parser creation to fail
}

TEST(LinkReferencesTest, ReferenceToMacroParameter) {
    // Test case: A reference within a macro to a macro parameter
    const std::string grammar = R"(
        @macro MY_MACRO(X) = X '!'
        START <- MY_MACRO('a')
    )";
    auto parser = create_parser(grammar);
    ASSERT_NE(parser, nullptr);

    // Parse a string that uses the macro
    bool ret = parser->parse("a!");
    ASSERT_TRUE(ret);

    // If 'X' within MY_MACRO was not correctly linked to the parameter 'a',
    // the parser would fail to parse "a!".
    // This implicitly tests that ope.iarg_ was set correctly for the parameter reference.
}

TEST(LinkReferencesTest, ReferenceToMacroParameterAndRuleWithSameName) {
    // Test case: A macro parameter and a rule have the same name.
    // The macro parameter should take precedence.
    const std::string grammar = R"(
        @macro MY_MACRO(X) = X '!'
        X <- 'b'
        START <- MY_MACRO('a')
    )";
    auto parser = create_parser(grammar);
    ASSERT_NE(parser, nullptr);

    // If 'X' inside MY_MACRO linked to the rule 'X' ('b') instead of the parameter 'a',
    // then "a!" would fail, and "b!" would succeed.
    // We expect "a!" to succeed, meaning the parameter link took precedence.
    bool ret_a = parser->parse("a!");
    ASSERT_TRUE(ret_a);

    bool ret_b = parser->parse("b!"); // This should fail as 'X' in macro is 'a'
    ASSERT_FALSE(ret_b);
}

TEST(LinkReferencesTest, NestedMacrosWithParameters) {
    // Test case: Nested macros with parameters
    const std::string grammar = R"(
        @macro INNER_MACRO(Y) = Y Y
        @macro OUTER_MACRO(X) = INNER_MACRO(X) '!'
        START <- OUTER_MACRO('a')
    )";
    auto parser = create_parser(grammar);
    ASSERT_NE(parser, nullptr);

    // If parameters are not correctly linked through nested macro calls,
    // this parse would fail.
    bool ret = parser->parse("aa!");
    ASSERT_TRUE(ret);
}

TEST(LinkReferencesTest, ReferenceInMacroToUndefinedParameter) {
    // Test case: A macro references a parameter that is not in its parameter list.
    // This should result in an undefined reference, causing parser creation to fail.
    const std::string grammar = R"(
        @macro MY_MACRO(X) = Y '!' // Y is not a parameter
        START <- MY_MACRO('a')
    )";
    auto parser = std::make_shared<peg::Parser>(grammar);
    ASSERT_FALSE(static_cast<bool>(*parser)); // Expect parser creation to fail
}

TEST(LinkReferencesTest, ReferenceInMacroToUndefinedRuleAndNotAParameter) {
    // Test case: A macro references a name that is neither a parameter nor a rule.
    const std::string grammar = R"(
        @macro MY_MACRO(X) = Z '!' // Z is neither a param nor a rule
        START <- MY_MACRO('a')
    )";
    auto parser = std::make_shared<peg::Parser>(grammar);
    ASSERT_FALSE(static_cast<bool>(*parser)); // Expect parser creation to fail
}

TEST(LinkReferencesTest, MultipleReferencesToSameRule) {
    // Test case: Multiple references to the same rule
    const std::string grammar = R"(
        A <- B B
        B <- 'b'
    )";
    auto parser = create_parser(grammar);
    ASSERT_NE(parser, nullptr);

    bool ret = parser->parse("bb");
    ASSERT_TRUE(ret);
}

TEST(LinkReferencesTest, ReferenceInActionBlock) {
    // Test case: References within action blocks are not handled by LinkReferences.
    // This test ensures that LinkReferences doesn't try to link 'C' in the action.
    // The grammar should still be valid if 'C' is not a rule.
    const std::string grammar = R"(
        A <- 'a' { return C; }
    )";
    auto parser = create_parser(grammar);
    ASSERT_NE(parser, nullptr); // Parser creation should succeed even if 'C' is undefined.

    // The action block itself might fail at runtime if 'C' is used,
    // but LinkReferences is only concerned with grammar rule/parameter references.
    // This test confirms that LinkReferences doesn't incorrectly flag 'C' as an
    // undefined rule reference during grammar parsing.
}

TEST(LinkReferencesTest, EmptyGrammar) {
    // Edge case: Empty grammar
    const std::string grammar = "";
    auto parser = std::make_shared<peg::Parser>(grammar);
    ASSERT_FALSE(static_cast<bool>(*parser)); // An empty grammar is invalid
}

TEST(LinkReferencesTest, GrammarWithOnlyLiterals) {
    // Edge case: Grammar with only literals, no references
    const std::string grammar = R"(
        A <- 'hello'
    )";
    auto parser = create_parser(grammar);
    ASSERT_NE(parser, nullptr);
    ASSERT_TRUE(parser->parse("hello"));
    ASSERT_FALSE(parser->parse("world"));
}

TEST(LinkReferencesTest, GrammarWithOnlyMacros) {
    // Edge case: Grammar with only macros, no top-level rules
    const std::string grammar = R"(
        @macro MY_MACRO(X) = X
    )";
    auto parser = std::make_shared<peg::Parser>(grammar);
    ASSERT_FALSE(static_cast<bool>(*parser)); // No starting rule, so invalid
}