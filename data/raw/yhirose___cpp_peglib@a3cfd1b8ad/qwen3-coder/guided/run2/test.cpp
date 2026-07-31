#include <gtest/gtest.h>
#include <peglib.h>
#include <string>

TEST(PegLibGrammarTest, MakeGrammarInitializesGrammarCorrectly) {
    peg::parser parser;
    
    // The make_grammar function is called internally during parser construction
    // We test by attempting to parse a simple valid PEG grammar string
    std::string grammar = R"(
        Expr <- Term ( "+" Term )*
        Term <- Factor ( "*" Factor )*
        Factor <- Number | "(" Expr ")"
        Number <- [0-9]+
        Spacing <- [ \t\n\r]*
    )";
    
    // We need to set the grammar via public API
    parser.define(grammar);
    
    // Test that the parser can parse a simple expression
    std::string input = "3 + 4 * 5";
    bool result = parser.parse(input);
    
    // If make_grammar was correctly initialized, the parser should successfully parse
    // a valid arithmetic expression matching the defined grammar
    EXPECT_TRUE(result) << "Parser should successfully parse '3 + 4 * 5' with defined grammar";
    
    // Test with whitespace variations (edge case: multiple spaces, tabs)
    std::string input2 = "3\t+\t4\t*\t5";
    bool result2 = parser.parse(input2);
    EXPECT_TRUE(result2) << "Parser should handle tabs and multiple spaces in input";
    
    // Test empty input - should fail since grammar requires at least a Number
    std::string empty_input = "";
    bool result_empty = parser.parse(empty_input);
    EXPECT_FALSE(result_empty) << "Empty input should fail to parse with this grammar";
    
    // Test invalid input
    std::string invalid_input = "3 & 4";
    bool result_invalid = parser.parse(invalid_input);
    EXPECT_FALSE(result_invalid) << "Invalid operator '&' should fail to parse";
    
    // Test with comment and spacing as defined in make_grammar
    std::string grammar_with_comments = R"(
        # This is a comment
        Expr <- Term ( "+" Term )*   # Another comment
        Term <- Factor ( "*" Factor )*
        Factor <- Number | "(" Expr ")"
        Number <- [0-9]+
        Spacing <- [ \t\n\r]*
    )";
    
    parser.define(grammar_with_comments);
    bool result_with_comments = parser.parse("123");
    EXPECT_TRUE(result_with_comments) << "Parser should handle comments in grammar definition";
    
    // Test identifier parsing (edge case: underscore and non-ASCII in identifier)
    std::string grammar_with_identifiers = R"(
        _var <- "test"
        café <- "coffee"
        var123 <- "number"
        Spacing <- [ \t\n\r]*
    )";
    
    parser.define(grammar_with_identifiers);
    bool result_identifier = parser.parse("test");
    EXPECT_TRUE(result_identifier) << "Parser should recognize identifiers with underscore and non-ASCII";
    
    // Test literal with escaped characters (edge case: escaped quote)
    std::string grammar_with_escapes = R"(
        Str <- "'" ( !'"' . )* "'"
        Spacing <- [ \t\n\r]*
    )";
    
    parser.define(grammar_with_escapes);
    bool result_escape = parser.parse("'a\\'b'");
    EXPECT_FALSE(result_escape) << "Escaped quote inside single quotes should fail with this simple literal definition";
    
    // Test class with range (edge case: character range)
    std::string grammar_with_class = R"(
        DigitRange <- [0-9]
        Spacing <- [ \t\n\r]*
    )";
    
    parser.define(grammar_with_class);
    bool result_class = parser.parse("5");
    EXPECT_TRUE(result_class) << "Character class with range should match digit";
    
    // Test EndOfFile constraint (edge case: trailing characters)
    std::string grammar_eof = R"(
        Expr <- Number
        Number <- [0-9]+
        Spacing <- [ \t\n\r]*
    )";
    
    parser.define(grammar_eof);
    bool result_eof = parser.parse("42");
    EXPECT_TRUE(result_eof) << "Valid input should parse successfully with EndOfFile";
    
    bool result_eof_trailing = parser.parse("42 ");
    EXPECT_FALSE(result_eof_trailing) << "Trailing space should fail due to EndOfFile constraint";
    
    // Test IGNORE token (edge case: tilde prefix)
    std::string grammar_ignore = R"(
        ~Expr <- Number
        Number <- [0-9]+
        Spacing <- [ \t\n\r]*
    )";
    
    parser.define(grammar_ignore);
    bool result_ignore = parser.parse("42");
    EXPECT_TRUE(result_ignore) << "IGNORE token should be accepted in grammar definition";
    
    // Test LEFTARROW with spacing (edge case: multiple spaces around <-)
    std::string grammar_leftarrow = R"(
        Expr   <-   Number
        Number <- [0-9]+
        Spacing <- [ \t\n\r]*
    )";
    
    parser.define(grammar_leftarrow);
    bool result_leftarrow = parser.parse("100");
    EXPECT_TRUE(result_leftarrow) << "LEFTARROW with multiple spaces should be parsed correctly";
    
    // Test complex nested structure
    std::string complex_grammar = R"(
        Start <- Expr
        Expr <- Term ( ( "+" / "-" ) Term )*
        Term <- Factor ( ( "*" / "/" ) Factor )*
        Factor <- Number | "(" Expr ")"
        Number <- [0-9]+
        Spacing <- [ \t\n\r]*
    )";
    
    parser.define(complex_grammar);
    bool result_complex = parser.parse("1 + 2 * 3 - 4");
    EXPECT_TRUE(result_complex) << "Complex nested expression with operators should parse correctly";
}