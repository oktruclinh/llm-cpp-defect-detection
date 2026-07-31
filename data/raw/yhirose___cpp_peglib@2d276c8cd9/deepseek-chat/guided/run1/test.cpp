#include <gtest/gtest.h>
#include "peglib.h"

// Test that DetectLeftRecursion correctly identifies direct left recursion
TEST(DetectLeftRecursionTest, DirectLeftRecursion) {
    // Grammar: A <- A 'a' / 'b'
    // This has direct left recursion on A
    peg::parser parser(R"(
        A <- A 'a' / 'b'
    )");
    
    // The parser should detect left recursion and report an error
    EXPECT_FALSE(static_cast<bool>(parser));  // Should fail to compile due to left recursion
}

// Test that DetectLeftRecursion correctly identifies indirect left recursion
TEST(DetectLeftRecursionTest, IndirectLeftRecursion) {
    // Grammar: A <- B 'a' / 'b'
    //          B <- A 'c' / 'd'
    // This has indirect left recursion (A -> B -> A)
    peg::parser parser(R"(
        A <- B 'a' / 'b'
        B <- A 'c' / 'd'
    )");
    
    EXPECT_FALSE(static_cast<bool>(parser));  // Should fail to compile due to left recursion
}

// Test that DetectLeftRecursion correctly handles non-recursive grammars
TEST(DetectLeftRecursionTest, NoLeftRecursion) {
    // Grammar: A <- B 'a' / 'b'
    //          B <- 'c' / 'd'
    // No left recursion here
    peg::parser parser(R"(
        A <- B 'a' / 'b'
        B <- 'c' / 'd'
    )");
    
    EXPECT_TRUE(static_cast<bool>(parser));  // Should compile successfully
}

// Test that DetectLeftRecursion handles grammars with multiple rules but no recursion
TEST(DetectLeftRecursionTest, MultipleRulesNoRecursion) {
    // Grammar: Start <- Expr
    //          Expr  <- Term '+' Expr / Term
    //          Term  <- Factor '*' Term / Factor
    //          Factor <- '(' Expr ')' / '0' / '1'
    // This is a standard expression grammar without left recursion
    peg::parser parser(R"(
        Start <- Expr
        Expr  <- Term '+' Expr / Term
        Term  <- Factor '*' Term / Factor
        Factor <- '(' Expr ')' / '0' / '1'
    )");
    
    EXPECT_TRUE(static_cast<bool>(parser));  // Should compile successfully
}

// Test that DetectLeftRecursion handles empty grammar (edge case)
TEST(DetectLeftRecursionTest, EmptyGrammar) {
    peg::parser parser("");
    EXPECT_FALSE(static_cast<bool>(parser));  // Empty grammar should fail
}

// Test that DetectLeftRecursion handles grammar with only terminal rules
TEST(DetectLeftRecursionTest, OnlyTerminalRules) {
    // Grammar: A <- 'a' / 'b'
    //          B <- 'c' / 'd'
    // No non-terminal references, so no recursion possible
    peg::parser parser(R"(
        A <- 'a' / 'b'
        B <- 'c' / 'd'
    )");
    
    EXPECT_TRUE(static_cast<bool>(parser));  // Should compile successfully
}

// Test that DetectLeftRecursion handles self-referencing rule that is not left-recursive
// (right recursion is allowed)
TEST(DetectLeftRecursionTest, RightRecursion) {
    // Grammar: A <- 'a' A / 'b'
    // This is right-recursive, not left-recursive, so it should be allowed
    peg::parser parser(R"(
        A <- 'a' A / 'b'
    )");
    
    EXPECT_TRUE(static_cast<bool>(parser));  // Should compile successfully
}

// Test that DetectLeftRecursion handles grammar with unused rules
TEST(DetectLeftRecursionTest, UnusedRules) {
    // Grammar: Start <- 'a'
    //          Unused <- Start 'b'
    // The unused rule references Start, but Start doesn't reference Unused
    peg::parser parser(R"(
        Start <- 'a'
        Unused <- Start 'b'
    )");
    
    EXPECT_TRUE(static_cast<bool>(parser));  // Should compile successfully
}

// Test that DetectLeftRecursion handles grammar with multiple levels of indirection
TEST(DetectLeftRecursionTest, DeepIndirectRecursion) {
    // Grammar: A <- B 'a'
    //          B <- C 'b'
    //          C <- A 'c'
    // Deep indirect left recursion (A -> B -> C -> A)
    peg::parser parser(R"(
        A <- B 'a'
        B <- C 'b'
        C <- A 'c'
    )");
    
    EXPECT_FALSE(static_cast<bool>(parser));  // Should fail to compile due to left recursion
}

// Test that DetectLeftRecursion handles grammar with mutual recursion that is not left-recursive
TEST(DetectLeftRecursionTest, MutualRecursionNotLeftRecursive) {
    // Grammar: A <- 'a' B
    //          B <- 'b' A
    // This is mutual recursion but not left-recursive (both start with terminals)
    peg::parser parser(R"(
        A <- 'a' B
        B <- 'b' A
    )");
    
    EXPECT_TRUE(static_cast<bool>(parser));  // Should compile successfully
}

// Test that DetectLeftRecursion handles grammar with optional left recursion
TEST(DetectLeftRecursionTest, OptionalLeftRecursion) {
    // Grammar: A <- A? 'a' / 'b'
    // This has left recursion through the optional
    peg::parser parser(R"(
        A <- A? 'a' / 'b'
    )");
    
    EXPECT_FALSE(static_cast<bool>(parser));  // Should fail to compile due to left recursion
}

// Test that DetectLeftRecursion handles grammar with repetition that could cause left recursion
TEST(DetectLeftRecursionTest, RepetitionLeftRecursion) {
    // Grammar: A <- A* 'a' / 'b'
    // This has left recursion through the repetition
    peg::parser parser(R"(
        A <- A* 'a' / 'b'
    )");
    
    EXPECT_FALSE(static_cast<bool>(parser));  // Should fail to compile due to left recursion
}