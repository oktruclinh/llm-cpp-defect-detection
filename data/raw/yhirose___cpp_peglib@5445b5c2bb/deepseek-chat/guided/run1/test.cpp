#include <gtest/gtest.h>
#include "peglib.h"

class AssignIDToDefinitionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // No setup needed for these tests
    }
};

// Test that AssignIDToDefinition works with a simple rule reference
TEST_F(AssignIDToDefinitionTest, SimpleRuleReference) {
    peg::parser parser(R"(
        ROOT <- A
        A <- 'a'
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse a simple input to trigger AssignIDToDefinition
    auto ret = parser.parse("a");
    EXPECT_TRUE(ret);
}

// Test with multiple arguments in a reference
TEST_F(AssignIDToDefinitionTest, MultipleArgumentsInReference) {
    peg::parser parser(R"(
        ROOT <- A
        A <- B C
        B <- 'b'
        C <- 'c'
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("bc");
    EXPECT_TRUE(ret);
}

// Test with empty rule reference (should not crash)
TEST_F(AssignIDToDefinitionTest, EmptyRuleReference) {
    peg::parser parser(R"(
        ROOT <- A
        A <- ''
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("");
    EXPECT_TRUE(ret);
}

// Test with recursive rule reference
TEST_F(AssignIDToDefinitionTest, RecursiveRuleReference) {
    peg::parser parser(R"(
        ROOT <- A
        A <- 'a' A / 'a'
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("aaa");
    EXPECT_TRUE(ret);
}

// Test with rule that has no reference (leaf rule)
TEST_F(AssignIDToDefinitionTest, LeafRuleNoReference) {
    peg::parser parser(R"(
        ROOT <- 'a'
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("a");
    EXPECT_TRUE(ret);
}

// Test with multiple levels of nested references
TEST_F(AssignIDToDefinitionTest, NestedReferences) {
    peg::parser parser(R"(
        ROOT <- A
        A <- B
        B <- C
        C <- 'c'
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("c");
    EXPECT_TRUE(ret);
}

// Test with choice operator containing references
TEST_F(AssignIDToDefinitionTest, ChoiceWithReferences) {
    peg::parser parser(R"(
        ROOT <- A / B
        A <- 'a'
        B <- 'b'
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret1 = parser.parse("a");
    EXPECT_TRUE(ret1);
    
    auto ret2 = parser.parse("b");
    EXPECT_TRUE(ret2);
}

// Test with repetition operator containing references
TEST_F(AssignIDToDefinitionTest, RepetitionWithReferences) {
    peg::parser parser(R"(
        ROOT <- A*
        A <- 'a'
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("aaa");
    EXPECT_TRUE(ret);
}

// Test with optional operator containing references
TEST_F(AssignIDToDefinitionTest, OptionalWithReferences) {
    peg::parser parser(R"(
        ROOT <- A?
        A <- 'a'
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret1 = parser.parse("a");
    EXPECT_TRUE(ret1);
    
    auto ret2 = parser.parse("");
    EXPECT_TRUE(ret2);
}

// Test with predicate operators containing references
TEST_F(AssignIDToDefinitionTest, PredicateWithReferences) {
    peg::parser parser(R"(
        ROOT <- &A 'a'
        A <- 'a'
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("a");
    EXPECT_TRUE(ret);
}

// Test that invalid grammar (undefined rule reference) is handled
TEST_F(AssignIDToDefinitionTest, UndefinedRuleReference) {
    peg::parser parser(R"(
        ROOT <- A
    )");
    
    // This should fail because A is not defined
    EXPECT_FALSE(static_cast<bool>(parser));
}

// Test with complex grammar involving multiple rule types
TEST_F(AssignIDToDefinitionTest, ComplexGrammar) {
    peg::parser parser(R"(
        ROOT <- EXPR
        EXPR <- TERM ( '+' TERM )*
        TERM <- FACTOR ( '*' FACTOR )*
        FACTOR <- NUMBER / '(' EXPR ')'
        NUMBER <- [0-9]+
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("1+2*3");
    EXPECT_TRUE(ret);
}

// Test that AssignIDToDefinition works with rule that has no args
TEST_F(AssignIDToDefinitionTest, RuleWithNoArgs) {
    peg::parser parser(R"(
        ROOT <- A
        A <- B
        B <- 'b'
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("b");
    EXPECT_TRUE(ret);
}

// Test with literal character reference
TEST_F(AssignIDToDefinitionTest, LiteralCharacterReference) {
    peg::parser parser(R"(
        ROOT <- 'a'
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("a");
    EXPECT_TRUE(ret);
}

// Test with character class reference
TEST_F(AssignIDToDefinitionTest, CharacterClassReference) {
    peg::parser parser(R"(
        ROOT <- [a-z]
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("m");
    EXPECT_TRUE(ret);
}

// Test with dot (any character) reference
TEST_F(AssignIDToDefinitionTest, DotReference) {
    peg::parser parser(R"(
        ROOT <- .
    )");
    
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("x");
    EXPECT_TRUE(ret);
}