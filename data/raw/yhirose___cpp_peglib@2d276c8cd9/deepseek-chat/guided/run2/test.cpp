#include <gtest/gtest.h>
#include "peglib.h"

// Test that DetectLeftRecursion correctly identifies direct left recursion
TEST(DetectLeftRecursionTest, DirectLeftRecursion) {
    // A grammar with direct left recursion: A <- A 'a' / 'b'
    peg::parser parser(R"(
        A <- A 'a' / 'b'
    )");
    
    // The parser should fail to compile due to left recursion
    ASSERT_FALSE(parser);
    ASSERT_FALSE(parser.set_logger([](size_t, size_t, const std::string&, const std::string&) {}));
}

// Test that DetectLeftRecursion correctly identifies indirect left recursion
TEST(DetectLeftRecursionTest, IndirectLeftRecursion) {
    // A grammar with indirect left recursion: A <- B 'a' / 'b', B <- A 'c'
    peg::parser parser(R"(
        A <- B 'a' / 'b'
        B <- A 'c'
    )");
    
    // The parser should fail to compile due to left recursion
    ASSERT_FALSE(parser);
}

// Test that DetectLeftRecursion does not flag non-recursive grammars
TEST(DetectLeftRecursionTest, NoLeftRecursion) {
    // A simple non-recursive grammar
    peg::parser parser(R"(
        A <- 'a' B
        B <- 'b'
    )");
    
    // The parser should compile successfully
    ASSERT_TRUE(parser);
    ASSERT_TRUE(parser.parse("ab"));
    ASSERT_FALSE(parser.parse("a"));
    ASSERT_FALSE(parser.parse("b"));
}

// Test that DetectLeftRecursion handles empty rules correctly
TEST(DetectLeftRecursionTest, EmptyRule) {
    // A grammar with an empty rule (no left recursion)
    peg::parser parser(R"(
        A <- B
        B <- ''
    )");
    
    // Should compile successfully
    ASSERT_TRUE(parser);
    ASSERT_TRUE(parser.parse(""));
}

// Test that DetectLeftRecursion handles rules that reference themselves through optional elements
TEST(DetectLeftRecursionTest, OptionalLeftRecursion) {
    // A <- A? 'a' - this is left recursive because A? can match empty and then A matches again
    peg::parser parser(R"(
        A <- A? 'a'
    )");
    
    // Should fail due to left recursion
    ASSERT_FALSE(parser);
}

// Test that DetectLeftRecursion handles rules with repetition operators
TEST(DetectLeftRecursionTest, RepetitionLeftRecursion) {
    // A <- A* 'a' - left recursive
    peg::parser parser(R"(
        A <- A* 'a'
    )");
    
    // Should fail due to left recursion
    ASSERT_FALSE(parser);
}

// Test that DetectLeftRecursion handles multiple rules with complex dependencies
TEST(DetectLeftRecursionTest, ComplexDependencies) {
    // A <- B 'x', B <- C 'y', C <- A 'z' - indirect left recursion
    peg::parser parser(R"(
        A <- B 'x'
        B <- C 'y'
        C <- A 'z'
    )");
    
    // Should fail due to left recursion
    ASSERT_FALSE(parser);
}

// Test that DetectLeftRecursion handles grammars with no left recursion but with rule references
TEST(DetectLeftRecursionTest, RuleReferencesNoRecursion) {
    // A <- B 'a', B <- 'b' - no recursion
    peg::parser parser(R"(
        A <- B 'a'
        B <- 'b'
    )");
    
    ASSERT_TRUE(parser);
    ASSERT_TRUE(parser.parse("ba"));
    ASSERT_FALSE(parser.parse("ab"));
}

// Test that DetectLeftRecursion handles self-referencing rules that are not left recursive
// (e.g., through a terminal prefix)
TEST(DetectLeftRecursionTest, RightRecursion) {
    // A <- 'a' A / 'b' - right recursion, not left recursion
    peg::parser parser(R"(
        A <- 'a' A / 'b'
    )");
    
    // Should compile successfully (right recursion is allowed)
    ASSERT_TRUE(parser);
    ASSERT_TRUE(parser.parse("b"));
    ASSERT_TRUE(parser.parse("ab"));
    ASSERT_TRUE(parser.parse("aab"));
    ASSERT_FALSE(parser.parse("a"));
}

// Test that DetectLeftRecursion handles grammars with multiple rules where only some are left recursive
TEST(DetectLeftRecursionTest, MixedRecursion) {
    // A <- A 'a' / 'b' (left recursive), B <- 'c' (not recursive)
    peg::parser parser(R"(
        A <- A 'a' / 'b'
        B <- 'c'
    )");
    
    // Should fail due to left recursion in A
    ASSERT_FALSE(parser);
}

// Test that DetectLeftRecursion handles grammars with character classes and no recursion
TEST(DetectLeftRecursionTest, CharacterClassNoRecursion) {
    peg::parser parser(R"(
        A <- [a-z]+
    )");
    
    ASSERT_TRUE(parser);
    ASSERT_TRUE(parser.parse("hello"));
    ASSERT_FALSE(parser.parse("Hello"));
}

// Test that DetectLeftRecursion handles grammars with predicates (no recursion)
TEST(DetectLeftRecursionTest, PredicateNoRecursion) {
    peg::parser parser(R"(
        A <- &'a' 'a' / 'b'
    )");
    
    ASSERT_TRUE(parser);
    ASSERT_TRUE(parser.parse("a"));
    ASSERT_TRUE(parser.parse("b"));
    ASSERT_FALSE(parser.parse("c"));
}