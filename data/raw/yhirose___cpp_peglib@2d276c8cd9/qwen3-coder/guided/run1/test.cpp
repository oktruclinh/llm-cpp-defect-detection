#include <gtest/gtest.h>
#include "peglib.h"

TEST(DetectLeftRecursionTest, SimpleNonRecursiveRule) {
    peglib::parser parser;
    
    // Define a simple non-recursive rule
    parser["S"] = peglib::lit("hello");
    
    // This should not trigger any left recursion detection
    EXPECT_TRUE(parser.parse("hello"));
    EXPECT_FALSE(parser.parse("world"));
}

TEST(DetectLeftRecursionTest, DirectLeftRecursion) {
    peglib::parser parser;
    
    // Define direct left recursion: S -> S "a"
    parser["S"] = peglib::seq(peglib::ref("S"), peglib::lit("a"));
    
    // This should detect left recursion
    EXPECT_FALSE(parser.parse("aa"));
    EXPECT_FALSE(parser.parse("aaa"));
}

TEST(DetectLeftRecursionTest, IndirectLeftRecursion) {
    peglib::parser parser;
    
    // Define indirect left recursion:
    // S -> A "b"
    // A -> S "a"
    parser["S"] = peglib::seq(peglib::ref("A"), peglib::lit("b"));
    parser["A"] = peglib::seq(peglib::ref("S"), peglib::lit("a"));
    
    // This should detect left recursion
    EXPECT_FALSE(parser.parse("aba"));
    EXPECT_FALSE(parser.parse("abab"));
}

TEST(DetectLeftRecursionTest, MutualRecursion) {
    peglib::parser parser;
    
    // Define mutual recursion:
    // A -> B "a"
    // B -> A "b"
    parser["A"] = peglib::seq(peglib::ref("B"), peglib::lit("a"));
    parser["B"] = peglib::seq(peglib::ref("A"), peglib::lit("b"));
    
    // This should detect left recursion
    EXPECT_FALSE(parser.parse("ab"));
    EXPECT_FALSE(parser.parse("aba"));
}

TEST(DetectLeftRecursionTest, ComplexNonRecursiveGrammar) {
    peglib::parser parser;
    
    // Define a complex but non-recursive grammar
    parser["S"] = peglib::seq(
        peglib::ref("A"),
        peglib::lit("b"),
        peglib::ref("B")
    );
    parser["A"] = peglib::alt(peglib::lit("a"), peglib::lit("c"));
    parser["B"] = peglib::lit("d");
    
    // This should work correctly
    EXPECT_TRUE(parser.parse("abd"));
    EXPECT_TRUE(parser.parse("cbd"));
    EXPECT_FALSE(parser.parse("abc"));
}

TEST(DetectLeftRecursionTest, EmptyRuleHandling) {
    peglib::parser parser;
    
    // Test with empty rules
    parser["S"] = peglib::seq(peglib::ref("A"), peglib::ref("B"));
    parser["A"] = peglib::lit("");
    parser["B"] = peglib::lit("x");
    
    // Should parse successfully
    EXPECT_TRUE(parser.parse("x"));
}

TEST(DetectLeftRecursionTest, SingleRuleSelfReference) {
    peglib::parser parser;
    
    // Self-reference without recursion
    parser["S"] = peglib::lit("test");
    
    // Should parse successfully
    EXPECT_TRUE(parser.parse("test"));
    EXPECT_FALSE(parser.parse("testing"));
}

TEST(DetectLeftRecursionTest, NestedReferences) {
    peglib::parser parser;
    
    // Nested references that don't cause recursion
    parser["S"] = peglib::seq(peglib::ref("A"), peglib::ref("B"));
    parser["A"] = peglib::ref("C");
    parser["B"] = peglib::lit("b");
    parser["C"] = peglib::lit("a");
    
    // Should parse successfully
    EXPECT_TRUE(parser.parse("ab"));
    EXPECT_FALSE(parser.parse("ac"));
}