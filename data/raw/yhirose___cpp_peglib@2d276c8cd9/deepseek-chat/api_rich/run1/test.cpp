#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>

// Test that DetectLeftRecursion correctly identifies direct left recursion
TEST(DetectLeftRecursionTest, DirectLeftRecursion) {
    // Grammar with direct left recursion: A <- A 'a' / 'b'
    peg::parser pg("A <- A 'a' / 'b'");
    
    // The grammar should fail to load due to left recursion
    EXPECT_FALSE((bool)pg);
}

// Test that DetectLeftRecursion correctly identifies indirect left recursion
TEST(DetectLeftRecursionTest, IndirectLeftRecursion) {
    // Grammar with indirect left recursion: A <- B 'a' / 'c', B <- A 'b' / 'd'
    peg::parser pg("A <- B 'a' / 'c'\nB <- A 'b' / 'd'");
    
    // The grammar should fail to load due to left recursion
    EXPECT_FALSE((bool)pg);
}

// Test that DetectLeftRecursion correctly handles non-recursive grammars
TEST(DetectLeftRecursionTest, NoLeftRecursion) {
    // Simple non-recursive grammar
    peg::parser pg("A <- 'a' B\nB <- 'b'");
    
    // The grammar should load successfully
    EXPECT_TRUE((bool)pg);
    
    // Verify it can parse valid input
    EXPECT_TRUE(pg.parse("ab"));
    
    // Verify it rejects invalid input
    EXPECT_FALSE(pg.parse("ba"));
}

// Test that DetectLeftRecursion handles mutually recursive rules without left recursion
TEST(DetectLeftRecursionTest, MutualRecursionNoLeftRecursion) {
    // Grammar with mutual recursion but no left recursion: A <- 'a' B, B <- 'b' A / 'c'
    peg::parser pg("A <- 'a' B\nB <- 'b' A / 'c'");
    
    // The grammar should load successfully
    EXPECT_TRUE((bool)pg);
    
    // Verify it can parse valid input
    EXPECT_TRUE(pg.parse("abc"));
    EXPECT_TRUE(pg.parse("ac"));
    
    // Verify it rejects invalid input
    EXPECT_FALSE(pg.parse("bac"));
}

// Test that DetectLeftRecursion handles empty grammar
TEST(DetectLeftRecursionTest, EmptyGrammar) {
    // Empty grammar string
    peg::parser pg("");
    
    // An empty grammar should be invalid
    EXPECT_FALSE((bool)pg);
}

// Test that DetectLeftRecursion handles grammar with only terminals
TEST(DetectLeftRecursionTest, OnlyTerminals) {
    // Grammar with only terminal rules
    peg::parser pg("A <- 'a'\nB <- 'b'");
    
    // The grammar should load successfully
    EXPECT_TRUE((bool)pg);
    
    // Verify parsing works
    EXPECT_TRUE(pg.parse("a"));
    EXPECT_TRUE(pg.parse("b"));
    EXPECT_FALSE(pg.parse("ab"));
}

// Test that DetectLeftRecursion handles complex left recursion with multiple alternatives
TEST(DetectLeftRecursionTest, ComplexLeftRecursion) {
    // Grammar with left recursion in one alternative: A <- A 'a' / 'b' / 'c'
    peg::parser pg("A <- A 'a' / 'b' / 'c'");
    
    // The grammar should fail to load due to left recursion
    EXPECT_FALSE((bool)pg);
}

// Test that DetectLeftRecursion handles left recursion through optional elements
TEST(DetectLeftRecursionTest, LeftRecursionThroughOptional) {
    // Grammar with left recursion through optional: A <- A 'a'? / 'b'
    peg::parser pg("A <- A 'a'? / 'b'");
    
    // The grammar should fail to load due to left recursion
    EXPECT_FALSE((bool)pg);
}

// Test that DetectLeftRecursion handles left recursion through repetition
TEST(DetectLeftRecursionTest, LeftRecursionThroughRepetition) {
    // Grammar with left recursion through repetition: A <- A 'a'* / 'b'
    peg::parser pg("A <- A 'a'* / 'b'");
    
    // The grammar should fail to load due to left recursion
    EXPECT_FALSE((bool)pg);
}

// Test that DetectLeftRecursion handles deep indirect left recursion
TEST(DetectLeftRecursionTest, DeepIndirectLeftRecursion) {
    // Grammar with deep indirect left recursion: A <- B, B <- C, C <- A 'a' / 'd'
    peg::parser pg("A <- B\nB <- C\nC <- A 'a' / 'd'");
    
    // The grammar should fail to load due to left recursion
    EXPECT_FALSE((bool)pg);
}

// Test that DetectLeftRecursion handles grammar with no recursion at all
TEST(DetectLeftRecursionTest, NoRecursionAtAll) {
    // Grammar with no recursion: A <- 'a' 'b', B <- 'c'
    peg::parser pg("A <- 'a' 'b'\nB <- 'c'");
    
    // The grammar should load successfully
    EXPECT_TRUE((bool)pg);
    
    // Verify parsing works
    EXPECT_TRUE(pg.parse("ab"));
    EXPECT_TRUE(pg.parse("c"));
    EXPECT_FALSE(pg.parse("abc"));
}

// Test that DetectLeftRecursion handles grammar with right recursion (should be valid)
TEST(DetectLeftRecursionTest, RightRecursion) {
    // Grammar with right recursion: A <- 'a' A / 'b'
    peg::parser pg("A <- 'a' A / 'b'");
    
    // The grammar should load successfully (right recursion is not left recursion)
    EXPECT_TRUE((bool)pg);
    
    // Verify parsing works
    EXPECT_TRUE(pg.parse("b"));
    EXPECT_TRUE(pg.parse("ab"));
    EXPECT_TRUE(pg.parse("aaab"));
    EXPECT_FALSE(pg.parse("aab"));
}

// Test that DetectLeftRecursion handles grammar with multiple rules and mixed recursion
TEST(DetectLeftRecursionTest, MultipleRulesMixedRecursion) {
    // Grammar with one left-recursive rule: A <- B 'a', B <- A 'b' / 'c'
    peg::parser pg("A <- B 'a'\nB <- A 'b' / 'c'");
    
    // The grammar should fail to load due to left recursion
    EXPECT_FALSE((bool)pg);
}

// Test that DetectLeftRecursion handles grammar with self-referencing rule
TEST(DetectLeftRecursionTest, SelfReferencingRule) {
    // Grammar with self-referencing rule: A <- A
    peg::parser pg("A <- A");
    
    // The grammar should fail to load due to left recursion
    EXPECT_FALSE((bool)pg);
}

// Test that DetectLeftRecursion handles grammar with rule referencing itself through another rule
TEST(DetectLeftRecursionTest, SelfReferenceThroughAnother) {
    // Grammar: A <- B, B <- A
    peg::parser pg("A <- B\nB <- A");
    
    // The grammar should fail to load due to left recursion
    EXPECT_FALSE((bool)pg);
}