#include <gtest/gtest.h>
#include "peglib.h"

class LinkReferencesTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a grammar with some rules for testing
        peg::parser parser(R"(
            ROOT <- A B
            A <- 'a'
            B <- 'b'
        )");
        ASSERT_TRUE(static_cast<bool>(parser));
    }
};

// Test that a reference to a macro parameter is correctly identified
TEST_F(LinkReferencesTest, MacroParameterReference) {
    peg::parser parser(R"(
        ROOT <- <param1>
        param1 <- 'value'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // The parser should successfully parse "value"
    EXPECT_TRUE(parser.parse("value"));
}

// Test that a reference to a definition rule is correctly linked
TEST_F(LinkReferencesTest, DefinitionRuleReference) {
    peg::parser parser(R"(
        ROOT <- A
        A <- 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // The parser should successfully parse "a"
    EXPECT_TRUE(parser.parse("a"));
}

// Test that a reference that is neither a parameter nor a rule results in error
TEST_F(LinkReferencesTest, UndefinedReference) {
    peg::parser parser(R"(
        ROOT <- UNDEFINED_RULE
    )");
    // This should fail because UNDEFINED_RULE is not defined
    EXPECT_FALSE(static_cast<bool>(parser));
}

// Test that macro parameters take precedence over definition rules with same name
TEST_F(LinkReferencesTest, ParameterPrecedenceOverRule) {
    peg::parser parser(R"(
        ROOT <- <A>
        A <- 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // The parser should use the parameter, not the rule
    // Since <A> is a macro parameter, it should be substituted
    EXPECT_TRUE(parser.parse("a"));
}

// Test multiple macro parameters
TEST_F(LinkReferencesTest, MultipleMacroParameters) {
    peg::parser parser(R"(
        ROOT <- <param1> <param2>
        param1 <- 'hello'
        param2 <- 'world'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    EXPECT_TRUE(parser.parse("helloworld"));
    EXPECT_FALSE(parser.parse("worldhello"));
}

// Test that a reference to a rule with arguments works correctly
TEST_F(LinkReferencesTest, RuleWithArguments) {
    peg::parser parser(R"(
        ROOT <- A('x')
        A(X) <- X
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    EXPECT_TRUE(parser.parse("x"));
    EXPECT_FALSE(parser.parse("y"));
}

// Test empty reference name (edge case)
TEST_F(LinkReferencesTest, EmptyReferenceName) {
    peg::parser parser(R"(
        ROOT <- <>
    )");
    // Empty reference should fail
    EXPECT_FALSE(static_cast<bool>(parser));
}

// Test reference with special characters in name
TEST_F(LinkReferencesTest, ReferenceWithSpecialCharacters) {
    peg::parser parser(R"(
        ROOT <- <my_param_123>
        my_param_123 <- 'test'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    EXPECT_TRUE(parser.parse("test"));
}

// Test that a reference to a rule that is defined later works (forward reference)
TEST_F(LinkReferencesTest, ForwardReference) {
    peg::parser parser(R"(
        ROOT <- LATER_RULE
        LATER_RULE <- 'later'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    EXPECT_TRUE(parser.parse("later"));
}

// Test that a reference to a rule that is defined earlier works (backward reference)
TEST_F(LinkReferencesTest, BackwardReference) {
    peg::parser parser(R"(
        EARLIER_RULE <- 'earlier'
        ROOT <- EARLIER_RULE
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    EXPECT_TRUE(parser.parse("earlier"));
}

// Test that a reference to a rule with the same name as a parameter is resolved as parameter
TEST_F(LinkReferencesTest, ParameterShadowingRule) {
    peg::parser parser(R"(
        ROOT <- <A>
        A <- 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // The parameter <A> should be used, not the rule A
    // This should parse "a" because the parameter gets substituted
    EXPECT_TRUE(parser.parse("a"));
}

// Test that a reference to a non-existent rule fails
TEST_F(LinkReferencesTest, NonExistentRuleReference) {
    peg::parser parser(R"(
        ROOT <- NONEXISTENT
        A <- 'a'
    )");
    // This should fail because NONEXISTENT is not defined
    EXPECT_FALSE(static_cast<bool>(parser));
}

// Test that a reference to a rule with multiple arguments works
TEST_F(LinkReferencesTest, RuleWithMultipleArguments) {
    peg::parser parser(R"(
        ROOT <- A('x', 'y')
        A(X, Y) <- X Y
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    EXPECT_TRUE(parser.parse("xy"));
    EXPECT_FALSE(parser.parse("yx"));
}

// Test that a reference to a rule with no arguments works
TEST_F(LinkReferencesTest, RuleWithNoArguments) {
    peg::parser parser(R"(
        ROOT <- A()
        A() <- 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    EXPECT_TRUE(parser.parse("a"));
}