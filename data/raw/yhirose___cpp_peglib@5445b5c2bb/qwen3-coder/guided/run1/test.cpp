#include <gtest/gtest.h>
#include "peglib.h"

TEST(PeglibAssignIDToDefinitionTest, VisitReferenceWithRule) {
    // Create a parser with a rule that references another rule
    peglib::parser parser;
    
    // Define a rule that references another rule
    parser["A"] = peglib::lit("hello");
    parser["B"] = peglib::ref("A") >> peglib::lit("world");
    
    // Parse a string that matches the rule
    std::string input = "helloworld";
    bool result = parser.parse(input);
    
    // The parse should succeed
    ASSERT_TRUE(result);
}

TEST(PeglibAssignIDToDefinitionTest, VisitReferenceWithoutRule) {
    // Test with a reference that doesn't have a rule (edge case)
    peglib::parser parser;
    
    // Create a rule that references a non-existent rule
    parser["B"] = peglib::ref("nonexistent");
    
    // This should not crash and should fail parsing
    std::string input = "anything";
    bool result = parser.parse(input);
    
    // Parsing should fail since the referenced rule doesn't exist
    ASSERT_FALSE(result);
}

TEST(PeglibAssignIDToDefinitionTest, VisitReferenceWithArgs) {
    // Create a parser with a rule that has arguments
    peglib::parser parser;
    
    // Define a rule with arguments
    parser["A"] = peglib::lit("start") >> peglib::any_char() >> peglib::lit("end");
    parser["B"] = peglib::ref("A") >> peglib::lit("final");
    
    // Parse a string that matches the rule
    std::string input = "startXendfinal";
    bool result = parser.parse(input);
    
    // The parse should succeed
    ASSERT_TRUE(result);
}

TEST(PeglibAssignIDToDefinitionTest, VisitReferenceComplexNested) {
    // Create a complex nested reference structure
    peglib::parser parser;
    
    // Define multiple rules with references
    parser["X"] = peglib::lit("x");
    parser["Y"] = peglib::ref("X") >> peglib::lit("y");
    parser["Z"] = peglib::ref("Y") >> peglib::lit("z");
    
    // Parse a string that matches the complex rule
    std::string input = "xyz";
    bool result = parser.parse(input);
    
    // The parse should succeed
    ASSERT_TRUE(result);
}

TEST(PeglibAssignIDToDefinitionTest, VisitReferenceEmptyInput) {
    // Test with empty input string
    peglib::parser parser;
    
    parser["A"] = peglib::lit("hello");
    parser["B"] = peglib::ref("A");
    
    // Empty input should not match
    std::string input = "";
    bool result = parser.parse(input);
    
    // Should fail to parse
    ASSERT_FALSE(result);
}

TEST(PeglibAssignIDToDefinitionTest, VisitReferenceMultipleReferences) {
    // Test with multiple references in one rule
    peglib::parser parser;
    
    parser["A"] = peglib::lit("a");
    parser["B"] = peglib::lit("b");
    parser["C"] = peglib::lit("c");
    parser["D"] = peglib::ref("A") >> peglib::ref("B") >> peglib::ref("C");
    
    // Parse matching input
    std::string input = "abc";
    bool result = parser.parse(input);
    
    // Should succeed
    ASSERT_TRUE(result);
}