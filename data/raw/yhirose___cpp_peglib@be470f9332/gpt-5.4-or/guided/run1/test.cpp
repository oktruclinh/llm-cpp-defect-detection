#include <gtest/gtest.h>
#include "peglib.h"

#include <memory>
#include <string>

namespace {

using peg::parser;

TEST(LinkReferencesVisitTest, MacroParameterReferenceMatchesAndParsesSuccessfully) {
    parser pg;
    const auto grammar = R"(
        Start <- Pair< 'a', 'b' >
        Pair<x, y> <- x y
    )";

    ASSERT_TRUE(pg.load_grammar(grammar)) << pg.log;

    EXPECT_TRUE(pg.parse("ab"));
    EXPECT_FALSE(pg.parse("aa"));
    EXPECT_FALSE(pg.parse("b"));
    EXPECT_FALSE(pg.parse(""));

    EXPECT_EQ("", pg.log);
}

TEST(LinkReferencesVisitTest, DefinitionRuleReferenceResolvesAndParsesThroughReferencedRule) {
    parser pg;
    const auto grammar = R"(
        Start <- Wrapper
        Wrapper <- Item
        Item <- 'x'
    )";

    ASSERT_TRUE(pg.load_grammar(grammar)) << pg.log;

    EXPECT_TRUE(pg.parse("x"));
    EXPECT_FALSE(pg.parse("y"));
    EXPECT_FALSE(pg.parse(""));

    EXPECT_EQ("", pg.log);
}

TEST(LinkReferencesVisitTest, MacroParameterShadowsDefinitionRuleWithSameName) {
    parser pg;
    const auto grammar = R"(
        Start <- Use<B>
        B <- 'x'
        Use<B> <- B
    )";

    ASSERT_TRUE(pg.load_grammar(grammar)) << pg.log;

    // If the reference inside Use<B> incorrectly linked to the rule B <- 'x'
    // instead of the macro parameter B, parsing "x" would succeed.
    EXPECT_FALSE(pg.parse("x"));
    EXPECT_TRUE(pg.parse("B"));
    EXPECT_FALSE(pg.parse(""));

    EXPECT_EQ("", pg.log);
}

TEST(LinkReferencesVisitTest, NestedMacroArgumentsAreAlsoLinkedRecursively) {
    parser pg;
    const auto grammar = R"(
        Start <- Outer< 'a', Inner<'b'> >
        Outer<x, y> <- x y
        Inner<z> <- z
    )";

    ASSERT_TRUE(pg.load_grammar(grammar)) << pg.log;

    EXPECT_TRUE(pg.parse("ab"));
    EXPECT_FALSE(pg.parse("aa"));
    EXPECT_FALSE(pg.parse("b"));
    EXPECT_FALSE(pg.parse(""));

    EXPECT_EQ("", pg.log);
}

TEST(LinkReferencesVisitTest, ReferencesInsideMacroArgumentsResolveDefinitionRules) {
    parser pg;
    const auto grammar = R"(
        Start <- Wrap<Item>
        Wrap<x> <- x
        Item <- 'q'
    )";

    ASSERT_TRUE(pg.load_grammar(grammar)) << pg.log;

    EXPECT_TRUE(pg.parse("q"));
    EXPECT_FALSE(pg.parse("x"));
    EXPECT_FALSE(pg.parse(""));

    EXPECT_EQ("", pg.log);
}

TEST(LinkReferencesVisitTest, MissingReferenceProducesGrammarLoadFailure) {
    parser pg;
    const auto grammar = R"(
        Start <- MissingRule
    )";

    EXPECT_FALSE(pg.load_grammar(grammar));
    EXPECT_NE(std::string::npos, pg.log.find("MissingRule"))
        << "Expected error log to mention unresolved reference. Log: " << pg.log;
}

TEST(LinkReferencesVisitTest, EmptyParameterListGrammarWithoutReferencesStillLoadsAndParses) {
    parser pg;
    const auto grammar = R"(
        Start <- 'z'
    )";

    ASSERT_TRUE(pg.load_grammar(grammar)) << pg.log;

    EXPECT_TRUE(pg.parse("z"));
    EXPECT_FALSE(pg.parse("a"));
    EXPECT_FALSE(pg.parse(""));

    EXPECT_EQ("", pg.log);
}

TEST(LinkReferencesVisitTest, MultipleParametersUseCorrectParameterByPositionAndName) {
    parser pg;
    const auto grammar = R"(
        Start <- Triple<'a', 'b', 'c'>
        Triple<x, y, z> <- x y z
    )";

    ASSERT_TRUE(pg.load_grammar(grammar)) << pg.log;

    EXPECT_TRUE(pg.parse("abc"));
    EXPECT_FALSE(pg.parse("acb"));
    EXPECT_FALSE(pg.parse("ab"));
    EXPECT_FALSE(pg.parse(""));

    EXPECT_EQ("", pg.log);
}

TEST(LinkReferencesVisitTest, SameParameterReferencedMultipleTimesUsesParameterEachTime) {
    parser pg;
    const auto grammar = R"(
        Start <- Dup<'k'>
        Dup<x> <- x x
    )";

    ASSERT_TRUE(pg.load_grammar(grammar)) << pg.log;

    EXPECT_TRUE(pg.parse("kk"));
    EXPECT_FALSE(pg.parse("k"));
    EXPECT_FALSE(pg.parse("kx"));
    EXPECT_FALSE(pg.parse(""));

    EXPECT_EQ("", pg.log);
}

TEST(LinkReferencesVisitTest, ParameterAndRuleResolutionCanCoexistInSameMacroBody) {
    parser pg;
    const auto grammar = R"(
        Start <- Mix<'a'>
        Tail <- 'b'
        Mix<x> <- x Tail
    )";

    ASSERT_TRUE(pg.load_grammar(grammar)) << pg.log;

    EXPECT_TRUE(pg.parse("ab"));
    EXPECT_FALSE(pg.parse("aa"));
    EXPECT_FALSE(pg.parse("b"));
    EXPECT_FALSE(pg.parse(""));

    EXPECT_EQ("", pg.log);
}

} // namespace