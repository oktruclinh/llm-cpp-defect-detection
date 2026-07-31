#include <gtest/gtest.h>
#include "peglib.h"

class MakeGrammarTest : public ::testing::Test {
protected:
    peg::parser parser;
};

// Test that make_grammar() creates a valid grammar that can parse basic PEG syntax
TEST_F(MakeGrammarTest, ParsesSimpleGrammar) {
    EXPECT_TRUE(parser.load_grammar("A <- 'hello'"));
}

// Test that make_grammar() handles empty grammar definition
TEST_F(MakeGrammarTest, ParsesEmptyGrammar) {
    EXPECT_TRUE(parser.load_grammar(""));
}

// Test that make_grammar() handles grammar with multiple rules
TEST_F(MakeGrammarTest, ParsesMultipleRules) {
    EXPECT_TRUE(parser.load_grammar(
        "A <- 'a'\n"
        "B <- 'b'\n"
        "C <- 'c'"
    ));
}

// Test that make_grammar() handles sequence operator
TEST_F(MakeGrammarTest, ParsesSequence) {
    EXPECT_TRUE(parser.load_grammar("A <- 'a' 'b' 'c'"));
}

// Test that make_grammar() handles choice operator
TEST_F(MakeGrammarTest, ParsesChoice) {
    EXPECT_TRUE(parser.load_grammar("A <- 'a' / 'b' / 'c'"));
}

// Test that make_grammar() handles zero-or-more operator
TEST_F(MakeGrammarTest, ParsesZeroOrMore) {
    EXPECT_TRUE(parser.load_grammar("A <- 'a'*"));
}

// Test that make_grammar() handles one-or-more operator
TEST_F(MakeGrammarTest, ParsesOneOrMore) {
    EXPECT_TRUE(parser.load_grammar("A <- 'a'+"));
}

// Test that make_grammar() handles optional operator
TEST_F(MakeGrammarTest, ParsesOptional) {
    EXPECT_TRUE(parser.load_grammar("A <- 'a'?"));
}

// Test that make_grammar() handles and-predicate
TEST_F(MakeGrammarTest, ParsesAndPredicate) {
    EXPECT_TRUE(parser.load_grammar("A <- &'a' 'a'"));
}

// Test that make_grammar() handles not-predicate
TEST_F(MakeGrammarTest, ParsesNotPredicate) {
    EXPECT_TRUE(parser.load_grammar("A <- !'b' 'a'"));
}

// Test that make_grammar() handles grouping with parentheses
TEST_F(MakeGrammarTest, ParsesGrouping) {
    EXPECT_TRUE(parser.load_grammar("A <- ('a' / 'b') 'c'"));
}

// Test that make_grammar() handles character classes
TEST_F(MakeGrammarTest, ParsesCharacterClass) {
    EXPECT_TRUE(parser.load_grammar("A <- [a-z]"));
}

// Test that make_grammar() handles dot (any character)
TEST_F(MakeGrammarTest, ParsesDot) {
    EXPECT_TRUE(parser.load_grammar("A <- ."));
}

// Test that make_grammar() handles literal strings with single quotes
TEST_F(MakeGrammarTest, ParsesSingleQuotedLiteral) {
    EXPECT_TRUE(parser.load_grammar("A <- 'hello world'"));
}

// Test that make_grammar() handles literal strings with double quotes
TEST_F(MakeGrammarTest, ParsesDoubleQuotedLiteral) {
    EXPECT_TRUE(parser.load_grammar("A <- \"hello world\""));
}

// Test that make_grammar() handles escape sequences in literals
TEST_F(MakeGrammarTest, ParsesEscapedCharacters) {
    EXPECT_TRUE(parser.load_grammar("A <- 'hello\\nworld'"));
    EXPECT_TRUE(parser.load_grammar("A <- 'tab\\there'"));
    EXPECT_TRUE(parser.load_grammar("A <- 'quote\\'inside'"));
}

// Test that make_grammar() handles comments
TEST_F(MakeGrammarTest, ParsesComments) {
    EXPECT_TRUE(parser.load_grammar(
        "# This is a comment\n"
        "A <- 'a'"
    ));
}

// Test that make_grammar() handles ignore operator (~)
TEST_F(MakeGrammarTest, ParsesIgnoreOperator) {
    EXPECT_TRUE(parser.load_grammar("A <- ~' ' 'a'"));
}

// Test that make_grammar() handles labeled begin/end markers
TEST_F(MakeGrammarTest, ParsesBeginEndMarkers) {
    EXPECT_TRUE(parser.load_grammar("A <- < 'a' >"));
}

// Test that make_grammar() handles capture begin/end markers
TEST_F(MakeGrammarTest, ParsesCaptureMarkers) {
    EXPECT_TRUE(parser.load_grammar("A <- $< 'a' >"));
}

// Test that make_grammar() handles named capture with identifier
TEST_F(MakeGrammarTest, ParsesNamedCapture) {
    EXPECT_TRUE(parser.load_grammar("A <- $name< 'a' >"));
}

// Test that make_grammar() handles left arrow operator
TEST_F(MakeGrammarTest, ParsesLeftArrow) {
    EXPECT_TRUE(parser.load_grammar("A <- 'a'"));
}

// Test that make_grammar() rejects invalid grammar with missing left arrow
TEST_F(MakeGrammarTest, RejectsMissingLeftArrow) {
    EXPECT_FALSE(parser.load_grammar("A 'a'"));
}

// Test that make_grammar() rejects invalid grammar with unterminated literal
TEST_F(MakeGrammarTest, RejectsUnterminatedLiteral) {
    EXPECT_FALSE(parser.load_grammar("A <- 'hello"));
}

// Test that make_grammar() rejects invalid grammar with invalid identifier
TEST_F(MakeGrammarTest, RejectsInvalidIdentifier) {
    EXPECT_FALSE(parser.load_grammar("123 <- 'a'"));
}

// Test that make_grammar() handles complex nested grammar
TEST_F(MakeGrammarTest, ParsesComplexNestedGrammar) {
    EXPECT_TRUE(parser.load_grammar(
        "Expr    <- Sum\n"
        "Sum     <- Product (('+' / '-') Product)*\n"
        "Product <- Value (('*' / '/') Value)*\n"
        "Value   <- [0-9]+ / '(' Expr ')'"
    ));
}

// Test that make_grammar() handles whitespace and spacing correctly
TEST_F(MakeGrammarTest, HandlesWhitespace) {
    EXPECT_TRUE(parser.load_grammar("A   <-   'a'"));
    EXPECT_TRUE(parser.load_grammar("A<-\t'a'\n"));
}

// Test that make_grammar() handles Unicode identifiers
TEST_F(MakeGrammarTest, ParsesUnicodeIdentifiers) {
    EXPECT_TRUE(parser.load_grammar("café <- 'coffee'"));
}

// Test that make_grammar() handles character class with ranges
TEST_F(MakeGrammarTest, ParsesCharacterClassWithRanges) {
    EXPECT_TRUE(parser.load_grammar("A <- [a-zA-Z0-9]"));
}

// Test that make_grammar() handles character class with escaped characters
TEST_F(MakeGrammarTest, ParsesCharacterClassWithEscapes) {
    EXPECT_TRUE(parser.load_grammar("A <- [\\[\\]]"));
}

// Test that make_grammar() handles octal escape sequences
TEST_F(MakeGrammarTest, ParsesOctalEscapes) {
    EXPECT_TRUE(parser.load_grammar("A <- '\\101'"));
    EXPECT_TRUE(parser.load_grammar("A <- '\\x41'"));
}

// Test that make_grammar() handles grammar with only comments
TEST_F(MakeGrammarTest, HandlesOnlyComments) {
    EXPECT_TRUE(parser.load_grammar("# Just a comment\n# Another comment"));
}

// Test that make_grammar() handles grammar with multiple line endings
TEST_F(MakeGrammarTest, HandlesMultipleLineEndings) {
    EXPECT_TRUE(parser.load_grammar("A <- 'a'\r\nB <- 'b'\rC <- 'c'\n"));
}