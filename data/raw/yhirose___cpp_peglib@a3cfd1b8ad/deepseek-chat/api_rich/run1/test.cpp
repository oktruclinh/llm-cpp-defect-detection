#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>

class MakeGrammarTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Ensure we have a fresh parser for each test
    }
};

// Test that a default-constructed parser is invalid (no grammar loaded)
TEST_F(MakeGrammarTest, DefaultParserIsInvalid) {
    peg::parser parser;
    EXPECT_FALSE(static_cast<bool>(parser));
}

// Test that loading a valid grammar makes the parser valid
TEST_F(MakeGrammarTest, LoadValidGrammar) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a'";
    EXPECT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(static_cast<bool>(parser));
}

// Test that loading an invalid grammar makes the parser invalid
TEST_F(MakeGrammarTest, LoadInvalidGrammar) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a' 'b'";  // Missing operator between sequences
    EXPECT_FALSE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_FALSE(static_cast<bool>(parser));
}

// Test parsing a simple literal rule
TEST_F(MakeGrammarTest, ParseSimpleLiteral) {
    peg::parser parser;
    const char* grammar = "Expr <- 'hello'";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse("hello"));
    EXPECT_FALSE(parser.parse("world"));
    EXPECT_FALSE(parser.parse(""));
}

// Test parsing with choice operator
TEST_F(MakeGrammarTest, ParseChoice) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a' / 'b'";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("b"));
    EXPECT_FALSE(parser.parse("c"));
}

// Test parsing with sequence operator
TEST_F(MakeGrammarTest, ParseSequence) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a' 'b'";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse("ab"));
    EXPECT_FALSE(parser.parse("a"));
    EXPECT_FALSE(parser.parse("b"));
    EXPECT_FALSE(parser.parse("ba"));
}

// Test parsing with zero-or-more operator
TEST_F(MakeGrammarTest, ParseZeroOrMore) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a'*";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse(""));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("aaa"));
    EXPECT_FALSE(parser.parse("b"));
}

// Test parsing with one-or-more operator
TEST_F(MakeGrammarTest, ParseOneOrMore) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a'+";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_FALSE(parser.parse(""));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("aaa"));
    EXPECT_FALSE(parser.parse("b"));
}

// Test parsing with optional operator
TEST_F(MakeGrammarTest, ParseOptional) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a'?";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse(""));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_FALSE(parser.parse("aa"));
}

// Test parsing with AND predicate
TEST_F(MakeGrammarTest, ParseAndPredicate) {
    peg::parser parser;
    const char* grammar = "Expr <- &'a' 'a'";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_FALSE(parser.parse("b"));
}

// Test parsing with NOT predicate
TEST_F(MakeGrammarTest, ParseNotPredicate) {
    peg::parser parser;
    const char* grammar = "Expr <- !'a' .";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse("b"));
    EXPECT_FALSE(parser.parse("a"));
}

// Test parsing with dot (any character)
TEST_F(MakeGrammarTest, ParseDot) {
    peg::parser parser;
    const char* grammar = "Expr <- .";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse("x"));
    EXPECT_TRUE(parser.parse(" "));
    EXPECT_FALSE(parser.parse(""));
    EXPECT_FALSE(parser.parse("ab"));
}

// Test parsing with character class
TEST_F(MakeGrammarTest, ParseCharacterClass) {
    peg::parser parser;
    const char* grammar = "Expr <- [a-z]";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("m"));
    EXPECT_TRUE(parser.parse("z"));
    EXPECT_FALSE(parser.parse("A"));
    EXPECT_FALSE(parser.parse("1"));
}

// Test parsing with character class range
TEST_F(MakeGrammarTest, ParseCharacterClassRange) {
    peg::parser parser;
    const char* grammar = "Expr <- [0-9]";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse("0"));
    EXPECT_TRUE(parser.parse("5"));
    EXPECT_TRUE(parser.parse("9"));
    EXPECT_FALSE(parser.parse("a"));
}

// Test parsing with escaped characters in literal
TEST_F(MakeGrammarTest, ParseEscapedLiteral) {
    peg::parser parser;
    const char* grammar = "Expr <- '\\n'";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse("\n"));
    EXPECT_FALSE(parser.parse("n"));
}

// Test parsing with comments
TEST_F(MakeGrammarTest, ParseWithComments) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a' # This is a comment\n";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse("a"));
}

// Test parsing with ignore operator (~)
TEST_F(MakeGrammarTest, ParseWithIgnore) {
    peg::parser parser;
    const char* grammar = "Expr <- ~' ' 'a'";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse(" a"));
    EXPECT_FALSE(parser.parse("a"));
}

// Test parsing with semantic actions
TEST_F(MakeGrammarTest, ParseWithSemanticAction) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a' 'b'";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    
    std::string result;
    parser["Expr"] = [&](const peg::SemanticValues& vs) {
        result = vs.token();
    };
    
    EXPECT_TRUE(parser.parse("ab"));
    EXPECT_EQ("ab", result);
}

// Test parsing with AST enabled
TEST_F(MakeGrammarTest, ParseWithAST) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a' 'b'";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    parser.enable_ast();
    
    std::shared_ptr<peg::Ast> ast;
    EXPECT_TRUE(parser.parse("ab", ast));
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ("Expr", ast->name);
    EXPECT_EQ("ab", ast->token);
}

// Test parsing with packrat parsing enabled
TEST_F(MakeGrammarTest, ParseWithPackrat) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a' 'b'";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    parser.enable_packrat_parsing();
    
    EXPECT_TRUE(parser.parse("ab"));
    EXPECT_FALSE(parser.parse("ba"));
}

// Test error logging
TEST_F(MakeGrammarTest, ParseWithErrorLog) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a'";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    
    bool errorCalled = false;
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        errorCalled = true;
        EXPECT_EQ(1, line);
        EXPECT_EQ(1, col);
        EXPECT_FALSE(msg.empty());
    };
    
    EXPECT_FALSE(parser.parse("b"));
    EXPECT_TRUE(errorCalled);
}

// Test parsing with multiple rules
TEST_F(MakeGrammarTest, ParseMultipleRules) {
    peg::parser parser;
    const char* grammar = "Expr <- Digit Digit\nDigit <- [0-9]";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse("42"));
    EXPECT_TRUE(parser.parse("00"));
    EXPECT_FALSE(parser.parse("a"));
    EXPECT_FALSE(parser.parse("1"));
    EXPECT_FALSE(parser.parse("123"));
}

// Test parsing with recursive grammar
TEST_F(MakeGrammarTest, ParseRecursiveGrammar) {
    peg::parser parser;
    const char* grammar = "Expr <- '(' Expr ')' / 'a'";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("(a)"));
    EXPECT_TRUE(parser.parse("((a))"));
    EXPECT_FALSE(parser.parse("("));
    EXPECT_FALSE(parser.parse(")"));
}

// Test parsing with empty grammar (should fail)
TEST_F(MakeGrammarTest, ParseEmptyGrammar) {
    peg::parser parser;
    const char* grammar = "";
    EXPECT_FALSE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_FALSE(static_cast<bool>(parser));
}

// Test parsing with whitespace-sensitive grammar
TEST_F(MakeGrammarTest, ParseWithWhitespace) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a' 'b'";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    EXPECT_TRUE(parser.parse("ab"));
    EXPECT_FALSE(parser.parse("a b"));  // Whitespace not allowed by default
}

// Test parsing with semantic values using any_cast
TEST_F(MakeGrammarTest, ParseWithSemanticValues) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a' 'b'";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    
    std::vector<std::string> values;
    parser["Expr"] = [&](const peg::SemanticValues& vs) {
        values.push_back(vs.token());
    };
    
    EXPECT_TRUE(parser.parse("ab"));
    ASSERT_EQ(1, values.size());
    EXPECT_EQ("ab", values[0]);
}

// Test parsing with choice in semantic action
TEST_F(MakeGrammarTest, ParseChoiceWithSemanticAction) {
    peg::parser parser;
    const char* grammar = "Expr <- 'a' / 'b'";
    ASSERT_TRUE(parser.load_grammar(grammar, strlen(grammar)));
    
    std::string result;
    parser["Expr"] = [&](const peg::SemanticValues& vs) {
        result = vs.token();
    };
    
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_EQ("a", result);
    
    EXPECT_TRUE(parser.parse("b"));
    EXPECT_EQ("b", result);
}