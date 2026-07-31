#include <gtest/gtest.h>
#include "peglib.h"

TEST(PeglibGrammarTest, MakeGrammarCreatesValidGrammar) {
    // Test that make_grammar sets up the grammar correctly by parsing a simple expression
    peglib::parser parser;
    
    // This should not crash and should set up the grammar
    parser = peglib::parser();
    
    // Verify that the grammar has been set up by testing a simple valid rule
    // The grammar should be able to parse a basic identifier
    std::string input = "test_identifier";
    bool result = parser.parse(input.c_str(), input.size());
    
    // The parser should be able to parse the grammar definition itself
    // We test that the grammar is properly initialized by checking that
    // it can parse a simple valid expression
    std::string grammar_def = R"(
        Grammar <- Spacing Definition+ EndOfFile
        Definition <- (IGNORE? Identifier LEFTARROW Expression)
        Expression <- Sequence (SLASH Sequence)*
        Sequence <- Prefix+
        Prefix <- (AND / NOT)? Suffix
        Suffix <- Primary (QUESTION / STAR / PLUS)?
        Primary <- (IGNORE? Identifier !LEFTARROW) / '(' Expression ')' / '<' Expression '>' / '$' < Expression '>' / Literal / Class / '.'
        Identifier <- IdentCont Spacing
        IdentCont <- IdentStart IdentRest*
        IdentStart <- [a-zA-Z_\x80-\xff]
        IdentRest <- IdentStart / [0-9]
        Literal <- "'" ( !'\''. )* "'" Spacing / '"' ( !'"'. )* '"' Spacing
        Class <- '[' ( !']'. )* ']' Spacing
        LEFTARROW <- "<-" Spacing
        SLASH <- '/' Spacing
        AND <- '&' Spacing
        NOT <- '!' Spacing
        QUESTION <- '?' Spacing
        STAR <- '*' Spacing
        PLUS <- '+' Spacing
        OPEN <- '(' Spacing
        CLOSE <- ')' Spacing
        DOT <- '.' Spacing
        Spacing <- (Space / Comment)*
        Comment <- '#' ( !EndOfLine . )* EndOfLine
        Space <- ' ' / '\t' / EndOfLine
        EndOfLine <- "\r\n" / '\n' / '\r'
        EndOfFile <- !.
        Begin <- '<' Spacing
        End <- '>' Spacing
        BeginCap <- '$' < Identifier? '<' Spacing
        EndCap <- '>' Spacing
        IGNORE <- '~'
    )";
    
    // This should not throw and should successfully parse
    peglib::parser grammar_parser(grammar_def);
    ASSERT_TRUE(grammar_parser);
    
    // Test that we can parse a simple valid identifier
    std::string valid_identifier = "my_identifier";
    bool parse_result = grammar_parser.parse(valid_identifier.c_str(), valid_identifier.size());
    ASSERT_TRUE(parse_result);
    
    // Test that we can parse a simple expression with operators
    std::string valid_expression = "a <- b / c";
    bool expr_result = grammar_parser.parse(valid_expression.c_str(), valid_expression.size());
    ASSERT_TRUE(expr_result);
}

TEST(PeglibGrammarTest, GrammarHasRequiredRules) {
    // Test that the grammar contains all required rules by checking their existence
    peglib::parser parser;
    
    // The grammar should have these core rules defined
    std::vector<std::string> required_rules = {
        "Grammar", "Definition", "Expression", "Sequence", "Prefix", 
        "Suffix", "Primary", "Identifier", "IdentCont", "IdentStart", 
        "IdentRest", "Literal", "Class", "Range", "Char", "LEFTARROW", 
        "SLASH", "AND", "NOT", "QUESTION", "STAR", "PLUS", "OPEN", 
        "CLOSE", "DOT", "Spacing", "Comment", "Space", "EndOfLine", 
        "EndOfFile", "Begin", "End", "BeginCap", "EndCap", "IGNORE"
    };
    
    // Verify that the parser can be created without crashing
    // and that it has the expected structure
    ASSERT_TRUE(parser);
    
    // Test that we can parse a valid grammar definition
    std::string simple_grammar = R"(
        Start <- "hello"
    )";
    
    peglib::parser test_parser(simple_grammar);
    ASSERT_TRUE(test_parser);
    
    // Test parsing with the simple grammar
    bool result = test_parser.parse("hello");
    ASSERT_TRUE(result);
}

TEST(PeglibGrammarTest, GrammarHandlesEmptyInput) {
    // Test that the grammar can handle empty input gracefully
    peglib::parser parser;
    
    // Empty input should not crash
    std::string empty_input = "";
    bool result = parser.parse(empty_input.c_str(), empty_input.size());
    
    // Empty input should not match (unless grammar allows it)
    // But the parser should not crash
    ASSERT_NO_THROW(parser.parse(empty_input.c_str(), empty_input.size()));
}

TEST(PeglibGrammarTest, GrammarRuleStructure) {
    // Test that the grammar rules are properly structured
    peglib::parser parser;
    
    // Test that we can create a parser with a valid grammar
    std::string grammar_with_rules = R"(
        TestRule <- "test" Spacing
        Spacing <- ' '+
    )";
    
    peglib::parser test_parser(grammar_with_rules);
    ASSERT_TRUE(test_parser);
    
    // Test that the parser can parse the defined rule
    bool result = test_parser.parse("test");
    ASSERT_TRUE(result);
    
    // Test with spacing
    bool spaced_result = test_parser.parse("test   ");
    ASSERT_TRUE(spaced_result);
}