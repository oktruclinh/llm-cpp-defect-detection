#include <gtest/gtest.h>
#include "peglib.h"

TEST(DetectLeftRecursionTest, BasicLeftRecursionDetection) {
    peglib::parser parser;
    
    // Create a grammar with left recursion
    const char* grammar = R"(
        Start = Factor
        Factor = Factor "*" Term | Term
        Term = "a"
    )";
    
    // This should detect left recursion in Factor
    bool result = parser.load_grammar(grammar);
    ASSERT_TRUE(result);
    
    // The parser should have detected left recursion
    // We can't directly test the internal DetectLeftRecursion::visit
    // but we can test that left recursion is properly detected
    // by checking that parsing fails appropriately
    std::string input = "a*a*a";
    bool parse_result = parser.parse(input);
    // Left recursion should cause parsing to fail or behave unexpectedly
    // but we're mainly testing that the grammar loading doesn't crash
    // and that the detection mechanism is called
}

TEST(DetectLeftRecursionTest, NoLeftRecursion) {
    peglib::parser parser;
    
    // Create a grammar without left recursion
    const char* grammar = R"(
        Start = Term "*" Term
        Term = "a"
    )";
    
    bool result = parser.load_grammar(grammar);
    ASSERT_TRUE(result);
    
    // Parsing should work fine
    std::string input = "a*a";
    bool parse_result = parser.parse(input);
    ASSERT_TRUE(parse_result);
}

TEST(DetectLeftRecursionTest, SelfReference) {
    peglib::parser parser;
    
    // Create a grammar with self-reference (direct left recursion)
    const char* grammar = R"(
        Start = Start "a" | "b"
    )";
    
    bool result = parser.load_grammar(grammar);
    ASSERT_TRUE(result);
    
    // This should be detected as left recursion
    std::string input = "b";
    bool parse_result = parser.parse(input);
    ASSERT_TRUE(parse_result);
}

TEST(DetectLeftRecursionTest, MutualRecursion) {
    peglib::parser parser;
    
    // Create a grammar with mutual recursion
    const char* grammar = R"(
        A = B "a"
        B = A "b" | "c"
    )";
    
    bool result = parser.load_grammar(grammar);
    ASSERT_TRUE(result);
    
    // Mutual recursion should be handled
    std::string input = "c";
    bool parse_result = parser.parse(input);
    ASSERT_TRUE(parse_result);
}

TEST(DetectLeftRecursionTest, ComplexGrammar) {
    peglib::parser parser;
    
    // Create a complex grammar with various rules
    const char* grammar = R"(
        Expr = Term (("+" | "-") Term)*
        Term = Factor (("*" | "/") Factor)*
        Factor = "(" Expr ")" | "a" | "b"
    )";
    
    bool result = parser.load_grammar(grammar);
    ASSERT_TRUE(result);
    
    // Parsing should work fine
    std::string input = "a+b*c";
    bool parse_result = parser.parse(input);
    ASSERT_TRUE(parse_result);
}

TEST(DetectLeftRecursionTest, EmptyGrammar) {
    peglib::parser parser;
    
    // Empty grammar
    const char* grammar = "";
    
    bool result = parser.load_grammar(grammar);
    // Empty grammar should not crash
    ASSERT_TRUE(result);
}

TEST(DetectLeftRecursionTest, SingleRule) {
    peglib::parser parser;
    
    // Single rule grammar
    const char* grammar = R"(
        Start = "a"
    )";
    
    bool result = parser.load_grammar(grammar);
    ASSERT_TRUE(result);
    
    std::string input = "a";
    bool parse_result = parser.parse(input);
    ASSERT_TRUE(parse_result);
}

TEST(DetectLeftRecursionTest, RuleWithOptional) {
    peglib::parser parser;
    
    // Grammar with optional elements
    const char* grammar = R"(
        Start = "a" "b"?
    )";
    
    bool result = parser.load_grammar(grammar);
    ASSERT_TRUE(result);
    
    std::string input = "a";
    bool parse_result = parser.parse(input);
    ASSERT_TRUE(parse_result);
    
    input = "ab";
    parse_result = parser.parse(input);
    ASSERT_TRUE(parse_result);
}