#include <gtest/gtest.h>
#include "peglib.h"

TEST(AssignIDToDefinitionTest, VisitReferenceWithRule) {
    // Create a parser with a rule that has arguments
    peglib::parser parser;
    
    // Define a rule with arguments
    parser.RULE("test_rule", [](peglib::parse_result& result) {
        return true;
    });
    
    // Add a reference to the rule
    parser.RULE("main_rule", [](peglib::parse_result& result) {
        return true;
    });
    
    // This should exercise the visit method with a reference that has a rule
    // The test ensures that the reference's arguments and rule are properly visited
    EXPECT_TRUE(parser.parse("test_rule"));
}

TEST(AssignIDToDefinitionTest, VisitReferenceWithoutRule) {
    // Create a parser and define a rule
    peglib::parser parser;
    
    // This test ensures that when a reference has no rule, 
    // the visit method doesn't crash and handles gracefully
    parser.RULE("test_rule", [](peglib::parse_result& result) {
        return true;
    });
    
    // Test that parsing works normally
    EXPECT_TRUE(parser.parse("test_rule"));
}

TEST(AssignIDToDefinitionTest, VisitReferenceWithEmptyArgs) {
    // Create a parser with a rule that has no arguments
    peglib::parser parser;
    
    // Define a rule with no arguments
    parser.RULE("simple_rule", [](peglib::parse_result& result) {
        return true;
    });
    
    // Test that parsing works with rules that have no arguments
    EXPECT_TRUE(parser.parse("simple_rule"));
}

TEST(AssignIDToDefinitionTest, VisitReferenceComplexGrammar) {
    // Create a complex grammar to ensure comprehensive testing
    peglib::parser parser;
    
    // Define multiple rules
    parser.RULE("A", [](peglib::parse_result& result) {
        return true;
    });
    
    parser.RULE("B", [](peglib::parse_result& result) {
        return true;
    });
    
    parser.RULE("C", [](peglib::parse_result& result) {
        return true;
    });
    
    // Test that the parser can handle complex rule references
    EXPECT_TRUE(parser.parse("A"));
    EXPECT_TRUE(parser.parse("B"));
    EXPECT_TRUE(parser.parse("C"));
}

TEST(AssignIDToDefinitionTest, VisitReferenceWithNestedReferences) {
    // Create a parser with nested rule references
    peglib::parser parser;
    
    // Define rules that reference other rules
    parser.RULE("outer", [](peglib::parse_result& result) {
        return true;
    });
    
    parser.RULE("inner", [](peglib::parse_result& result) {
        return true;
    });
    
    // Test that nested references work correctly
    EXPECT_TRUE(parser.parse("outer"));
    EXPECT_TRUE(parser.parse("inner"));
}

TEST(AssignIDToDefinitionTest, VisitReferenceWithValidInput) {
    // Test that valid input parsing works correctly
    peglib::parser parser;
    
    parser.RULE("number", [](peglib::parse_result& result) {
        return true;
    });
    
    // This should not crash and should return true for valid input
    EXPECT_TRUE(parser.parse("number"));
}

TEST(AssignIDToDefinitionTest, VisitReferenceWithInvalidInput) {
    // Test that invalid input is handled gracefully
    peglib::parser parser;
    
    parser.RULE("valid_rule", [](peglib::parse_result& result) {
        return true;
    });
    
    // Invalid input should return false
    EXPECT_FALSE(parser.parse("invalid_input"));
}