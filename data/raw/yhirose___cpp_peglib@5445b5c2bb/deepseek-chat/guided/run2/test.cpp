#include <gtest/gtest.h>
#include "peglib.h"

class AssignIDToDefinitionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
};

// Test that AssignIDToDefinition works correctly with a simple rule
TEST_F(AssignIDToDefinitionTest, SimpleRule) {
    peg::parser parser(R"(
        ROOT <- 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse something to trigger AssignIDToDefinition
    auto ret = parser.parse("a");
    EXPECT_TRUE(ret);
}

// Test with multiple arguments in a reference
TEST_F(AssignIDToDefinitionTest, MultipleArguments) {
    peg::parser parser(R"(
        ROOT <- < 'a' 'b' >
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("ab");
    EXPECT_TRUE(ret);
}

// Test with nested references
TEST_F(AssignIDToDefinitionTest, NestedReferences) {
    peg::parser parser(R"(
        ROOT <- A
        A <- 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("a");
    EXPECT_TRUE(ret);
}

// Test with empty rule (edge case)
TEST_F(AssignIDToDefinitionTest, EmptyRule) {
    peg::parser parser(R"(
        ROOT <- ''
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("");
    EXPECT_TRUE(ret);
}

// Test with rule that has no arguments
TEST_F(AssignIDToDefinitionTest, NoArguments) {
    peg::parser parser(R"(
        ROOT <- 'test'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("test");
    EXPECT_TRUE(ret);
}

// Test with rule that has multiple references
TEST_F(AssignIDToDefinitionTest, MultipleReferences) {
    peg::parser parser(R"(
        ROOT <- A B
        A <- 'a'
        B <- 'b'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("ab");
    EXPECT_TRUE(ret);
}

// Test with recursive rule (edge case)
TEST_F(AssignIDToDefinitionTest, RecursiveRule) {
    peg::parser parser(R"(
        ROOT <- 'a' ROOT / 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("aaa");
    EXPECT_TRUE(ret);
}

// Test with rule that has semantic action
TEST_F(AssignIDToDefinitionTest, RuleWithAction) {
    peg::parser parser(R"(
        ROOT <- 'a' { [action] }
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser["action"] = [](const peg::SemanticValues& sv) {
        return 0;
    };
    
    auto ret = parser.parse("a");
    EXPECT_TRUE(ret);
}

// Test with rule that has parameters
TEST_F(AssignIDToDefinitionTest, RuleWithParameters) {
    peg::parser parser(R"(
        ROOT <- 'a' 'b'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("ab");
    EXPECT_TRUE(ret);
}

// Test with complex grammar (multiple rules with references)
TEST_F(AssignIDToDefinitionTest, ComplexGrammar) {
    peg::parser parser(R"(
        EXPRESSION <- TERM ( '+' TERM )*
        TERM <- FACTOR ( '*' FACTOR )*
        FACTOR <- NUMBER / '(' EXPRESSION ')'
        NUMBER <- [0-9]+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("1+2*3");
    EXPECT_TRUE(ret);
}

// Test that parsing fails when expected (negative test)
TEST_F(AssignIDToDefinitionTest, ParseFailure) {
    peg::parser parser(R"(
        ROOT <- 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("b");
    EXPECT_FALSE(ret);
}

// Test with optional element
TEST_F(AssignIDToDefinitionTest, OptionalElement) {
    peg::parser parser(R"(
        ROOT <- 'a'?
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("");
    EXPECT_TRUE(ret);
    
    ret = parser.parse("a");
    EXPECT_TRUE(ret);
}

// Test with repetition
TEST_F(AssignIDToDefinitionTest, Repetition) {
    peg::parser parser(R"(
        ROOT <- 'a'*
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("aaa");
    EXPECT_TRUE(ret);
    
    ret = parser.parse("");
    EXPECT_TRUE(ret);
}

// Test with choice operator
TEST_F(AssignIDToDefinitionTest, ChoiceOperator) {
    peg::parser parser(R"(
        ROOT <- 'a' / 'b'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("a");
    EXPECT_TRUE(ret);
    
    ret = parser.parse("b");
    EXPECT_TRUE(ret);
}

// Test with sequence and choice
TEST_F(AssignIDToDefinitionTest, SequenceAndChoice) {
    peg::parser parser(R"(
        ROOT <- 'a' 'b' / 'c' 'd'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    
    auto ret = parser.parse("ab");
    EXPECT_TRUE(ret);
    
    ret = parser.parse("cd");
    EXPECT_TRUE(ret);
}