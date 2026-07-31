#include <gtest/gtest.h>
#include "peglib.h"

class MakeGrammarTest : public ::testing::Test {
protected:
    peg::parser parser;
};

TEST_F(MakeGrammarTest, ParsesSimpleGrammar) {
    // Test that a simple grammar can be parsed after make_grammar() is called
    // The grammar is implicitly set up by the parser constructor
    EXPECT_TRUE(parser.load_grammar("A <- 'hello'"));
    EXPECT_TRUE(parser.parse("hello"));
    EXPECT_FALSE(parser.parse("world"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithSequence) {
    EXPECT_TRUE(parser.load_grammar("A <- 'hello' 'world'"));
    EXPECT_TRUE(parser.parse("helloworld"));
    EXPECT_FALSE(parser.parse("hello"));
    EXPECT_FALSE(parser.parse("world"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithChoice) {
    EXPECT_TRUE(parser.load_grammar("A <- 'hello' / 'world'"));
    EXPECT_TRUE(parser.parse("hello"));
    EXPECT_TRUE(parser.parse("world"));
    EXPECT_FALSE(parser.parse("helloworld"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithRepetition) {
    EXPECT_TRUE(parser.load_grammar("A <- 'a'+"));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("aaa"));
    EXPECT_FALSE(parser.parse("b"));
    
    EXPECT_TRUE(parser.load_grammar("A <- 'a'*"));
    EXPECT_TRUE(parser.parse(""));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("aaa"));
    
    EXPECT_TRUE(parser.load_grammar("A <- 'a'?"));
    EXPECT_TRUE(parser.parse(""));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_FALSE(parser.parse("aa"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithAndPredicate) {
    EXPECT_TRUE(parser.load_grammar("A <- &'a' 'a'"));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_FALSE(parser.parse("b"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithNotPredicate) {
    EXPECT_TRUE(parser.load_grammar("A <- !'a' ."));
    EXPECT_TRUE(parser.parse("b"));
    EXPECT_FALSE(parser.parse("a"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithDot) {
    EXPECT_TRUE(parser.load_grammar("A <- ."));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("z"));
    EXPECT_FALSE(parser.parse(""));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithCharacterClass) {
    EXPECT_TRUE(parser.load_grammar("A <- [a-z]"));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("m"));
    EXPECT_TRUE(parser.parse("z"));
    EXPECT_FALSE(parser.parse("A"));
    EXPECT_FALSE(parser.parse("1"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithCharacterRange) {
    EXPECT_TRUE(parser.load_grammar("A <- [a-zA-Z]"));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("Z"));
    EXPECT_FALSE(parser.parse("1"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithEscapeSequences) {
    EXPECT_TRUE(parser.load_grammar("A <- '\\n'"));
    EXPECT_TRUE(parser.parse("\n"));
    EXPECT_FALSE(parser.parse("n"));
    
    EXPECT_TRUE(parser.load_grammar("A <- '\\t'"));
    EXPECT_TRUE(parser.parse("\t"));
    
    EXPECT_TRUE(parser.load_grammar("A <- '\\r'"));
    EXPECT_TRUE(parser.parse("\r"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithHexEscape) {
    EXPECT_TRUE(parser.load_grammar("A <- '\\x41'"));
    EXPECT_TRUE(parser.parse("A"));
    EXPECT_FALSE(parser.parse("B"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithOctalEscape) {
    EXPECT_TRUE(parser.load_grammar("A <- '\\101'"));
    EXPECT_TRUE(parser.parse("A"));
    EXPECT_FALSE(parser.parse("B"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithComments) {
    EXPECT_TRUE(parser.load_grammar("# This is a comment\nA <- 'hello'"));
    EXPECT_TRUE(parser.parse("hello"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithIgnore) {
    EXPECT_TRUE(parser.load_grammar("A <- ~'hello'"));
    EXPECT_TRUE(parser.parse("hello"));
    // The ~ operator makes the match invisible to actions
}

TEST_F(MakeGrammarTest, ParsesGrammarWithCapture) {
    EXPECT_TRUE(parser.load_grammar("A <- $<[a-z]+>"));
    EXPECT_TRUE(parser.parse("hello"));
    EXPECT_FALSE(parser.parse("123"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithMultipleDefinitions) {
    EXPECT_TRUE(parser.load_grammar(
        "A <- B C\n"
        "B <- 'hello'\n"
        "C <- 'world'\n"
    ));
    EXPECT_TRUE(parser.parse("helloworld"));
    EXPECT_FALSE(parser.parse("hello"));
    EXPECT_FALSE(parser.parse("world"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithRecursiveDefinition) {
    EXPECT_TRUE(parser.load_grammar("A <- 'a' A / 'a'"));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("aa"));
    EXPECT_TRUE(parser.parse("aaa"));
    EXPECT_FALSE(parser.parse("b"));
}

TEST_F(MakeGrammarTest, HandlesEmptyGrammar) {
    EXPECT_FALSE(parser.load_grammar(""));
    EXPECT_FALSE(parser.load_grammar("   "));
}

TEST_F(MakeGrammarTest, HandlesInvalidGrammar) {
    EXPECT_FALSE(parser.load_grammar("A <-- 'hello'"));
    EXPECT_FALSE(parser.load_grammar("A <- "));
    EXPECT_FALSE(parser.load_grammar("<- 'hello'"));
}

TEST_F(MakeGrammarTest, HandlesGrammarWithSpacing) {
    EXPECT_TRUE(parser.load_grammar("  A   <-   'hello'  "));
    EXPECT_TRUE(parser.parse("hello"));
}

TEST_F(MakeGrammarTest, HandlesGrammarWithNewlines) {
    EXPECT_TRUE(parser.load_grammar("\n\nA <- 'hello'\n\n"));
    EXPECT_TRUE(parser.parse("hello"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithGrouping) {
    EXPECT_TRUE(parser.load_grammar("A <- ('a' 'b')+"));
    EXPECT_TRUE(parser.parse("ab"));
    EXPECT_TRUE(parser.parse("abab"));
    EXPECT_FALSE(parser.parse("a"));
    EXPECT_FALSE(parser.parse("aba"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithIdentifierContainingUnderscore) {
    EXPECT_TRUE(parser.load_grammar("my_rule <- 'hello'"));
    EXPECT_TRUE(parser.parse("hello"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithIdentifierStartingWithUnderscore) {
    EXPECT_TRUE(parser.load_grammar("_rule <- 'hello'"));
    EXPECT_TRUE(parser.parse("hello"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithMultipleChoices) {
    EXPECT_TRUE(parser.load_grammar("A <- 'a' / 'b' / 'c' / 'd'"));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("b"));
    EXPECT_TRUE(parser.parse("c"));
    EXPECT_TRUE(parser.parse("d"));
    EXPECT_FALSE(parser.parse("e"));
}

TEST_F(MakeGrammarTest, ParsesGrammarWithComplexExpression) {
    EXPECT_TRUE(parser.load_grammar("A <- ('a'+ 'b'?)*"));
    EXPECT_TRUE(parser.parse(""));
    EXPECT_TRUE(parser.parse("ab"));
    EXPECT_TRUE(parser.parse("aab"));
    EXPECT_TRUE(parser.parse("aaab"));
    EXPECT_FALSE(parser.parse("ba"));
}