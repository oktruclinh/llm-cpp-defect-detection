#include <gtest/gtest.h>
#include "peglib.h"

TEST(LinkReferencesTest, VisitMacroParameterReference) {
    peglib::parser parser;
    
    // Define a grammar with a macro parameter
    const char* grammar = R"(
        START = "hello" %param1
        %param1 = "world"
    )";
    
    // This should not crash and should properly link the reference
    ASSERT_TRUE(parser.load_grammar(grammar));
    
    // Verify that the parser can parse the input
    std::string input = "helloworld";
    ASSERT_TRUE(parser.parse(input));
}

TEST(LinkReferencesTest, VisitDefinitionRuleReference) {
    peglib::parser parser;
    
    // Define a grammar with a rule reference
    const char* grammar = R"(
        START = WORD
        WORD = "hello"
    )";
    
    // This should not crash and should properly link the reference
    ASSERT_TRUE(parser.load_grammar(grammar));
    
    // Verify that the parser can parse the input
    std::string input = "hello";
    ASSERT_TRUE(parser.parse(input));
}

TEST(LinkReferencesTest, VisitNestedReferences) {
    peglib::parser parser;
    
    // Define a grammar with nested references
    const char* grammar = R"(
        START = WORD
        WORD = "hello" "world"
        "hello" = "hi"
    )";
    
    // This should not crash and should properly link the references
    ASSERT_TRUE(parser.load_grammar(grammar));
    
    // Verify that the parser can parse the input
    std::string input = "helloworld";
    ASSERT_TRUE(parser.parse(input));
}

TEST(LinkReferencesTest, VisitNonExistentReference) {
    peglib::parser parser;
    
    // Define a grammar with a non-existent reference (should not crash)
    const char* grammar = R"(
        START = NONEXISTENT
    )";
    
    // This should not crash and should properly handle the missing reference
    ASSERT_TRUE(parser.load_grammar(grammar));
    
    // Parsing should fail gracefully
    std::string input = "anything";
    ASSERT_FALSE(parser.parse(input));
}

TEST(LinkReferencesTest, VisitEmptyGrammar) {
    peglib::parser parser;
    
    // Define an empty grammar
    const char* grammar = "";
    
    // This should not crash
    ASSERT_TRUE(parser.load_grammar(grammar));
    
    // Parsing should fail since there's no start rule
    std::string input = "anything";
    ASSERT_FALSE(parser.parse(input));
}

TEST(LinkReferencesTest, VisitComplexGrammarWithParameters) {
    peglib::parser parser;
    
    // Define a complex grammar with parameters
    const char* grammar = R"(
        START = "hello" %param1 "world" %param2
        %param1 = "beautiful"
        %param2 = "test"
    )";
    
    // This should not crash and should properly link all references
    ASSERT_TRUE(parser.load_grammar(grammar));
    
    // Verify that the parser can parse the input
    std::string input = "hellobeautifulworldtest";
    ASSERT_TRUE(parser.parse(input));
}