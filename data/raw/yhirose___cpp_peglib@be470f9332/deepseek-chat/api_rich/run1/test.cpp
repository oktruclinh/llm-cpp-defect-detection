#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>

// Test that macro parameters are correctly resolved in grammar rules
TEST(LinkReferencesTest, MacroParameterResolution) {
    // Grammar with a macro that uses parameters
    const char* grammar = R"(
        ROOT      <- MACRO(1, 2)
        MACRO(X, Y) <- X Y
    )";
    
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Should parse "12" because X=1, Y=2
    EXPECT_TRUE(parser.parse("12"));
    
    // Should NOT parse "21" because order is X then Y
    EXPECT_FALSE(parser.parse("21"));
}

// Test that definition rules are correctly referenced
TEST(LinkReferencesTest, DefinitionRuleReference) {
    const char* grammar = R"(
        ROOT <- A B
        A    <- "hello"
        B    <- "world"
    )";
    
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    EXPECT_TRUE(parser.parse("helloworld"));
    EXPECT_FALSE(parser.parse("worldhello"));
    EXPECT_FALSE(parser.parse("hello"));
    EXPECT_FALSE(parser.parse("world"));
}

// Test that macro parameters take precedence over definition rules with same name
TEST(LinkReferencesTest, MacroParameterPrecedenceOverRule) {
    const char* grammar = R"(
        ROOT      <- MACRO("a")
        MACRO(X)  <- X X
        X         <- "b"
    )";
    
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // MACRO's parameter X should be used, not the rule X
    // So it should match "aa" not "bb"
    EXPECT_TRUE(parser.parse("aa"));
    EXPECT_FALSE(parser.parse("bb"));
}

// Test that empty macro parameter list works
TEST(LinkReferencesTest, EmptyMacroParameters) {
    const char* grammar = R"(
        ROOT      <- MACRO()
        MACRO()   <- "hello"
    )";
    
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    EXPECT_TRUE(parser.parse("hello"));
    EXPECT_FALSE(parser.parse("world"));
}

// Test that references to non-existent rules are handled gracefully
TEST(LinkReferencesTest, NonExistentRuleReference) {
    const char* grammar = R"(
        ROOT <- NONEXISTENT
    )";
    
    // This should fail to load because NONEXISTENT is not defined
    peg::parser parser(grammar);
    EXPECT_FALSE(static_cast<bool>(parser));
}

// Test that macro with multiple parameters works correctly
TEST(LinkReferencesTest, MultipleMacroParameters) {
    const char* grammar = R"(
        ROOT          <- MACRO("a", "b", "c")
        MACRO(X, Y, Z) <- X Y Z
    )";
    
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    EXPECT_TRUE(parser.parse("abc"));
    EXPECT_FALSE(parser.parse("cba"));
    EXPECT_FALSE(parser.parse("ab"));
}

// Test that nested macro references work
TEST(LinkReferencesTest, NestedMacroReferences) {
    const char* grammar = R"(
        ROOT          <- OUTER("x", "y")
        OUTER(A, B)   <- INNER(A) INNER(B)
        INNER(C)      <- C C
    )";
    
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // OUTER("x","y") -> INNER("x") INNER("y") -> "xx" "yy" -> "xxyy"
    EXPECT_TRUE(parser.parse("xxyy"));
    EXPECT_FALSE(parser.parse("xyxy"));
    EXPECT_FALSE(parser.parse("xxy"));
}

// Test that macro parameter can be used multiple times in body
TEST(LinkReferencesTest, RepeatedMacroParameter) {
    const char* grammar = R"(
        ROOT      <- MACRO("z")
        MACRO(X)  <- X X X
    )";
    
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    EXPECT_TRUE(parser.parse("zzz"));
    EXPECT_FALSE(parser.parse("zz"));
    EXPECT_FALSE(parser.parse("z"));
}

// Test that definition rules with same name as macro parameter are shadowed
TEST(LinkReferencesTest, RuleShadowedByMacroParameter) {
    const char* grammar = R"(
        ROOT      <- MACRO("hello")
        MACRO(X)  <- X
        X         <- "world"
    )";
    
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // X should be the macro parameter "hello", not the rule "world"
    EXPECT_TRUE(parser.parse("hello"));
    EXPECT_FALSE(parser.parse("world"));
}

// Test that macro without parameters still works
TEST(LinkReferencesTest, MacroWithoutParameters) {
    const char* grammar = R"(
        ROOT  <- MACRO
        MACRO <- "test"
    )";
    
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    EXPECT_TRUE(parser.parse("test"));
    EXPECT_FALSE(parser.parse("other"));
}

// Test that recursive macro references are handled
TEST(LinkReferencesTest, RecursiveMacroReference) {
    const char* grammar = R"(
        ROOT          <- MACRO("a")
        MACRO(X)      <- X MACRO(X) / X
    )";
    
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("aa"));
    EXPECT_TRUE(parser.parse("aaa"));
    EXPECT_FALSE(parser.parse("b"));
    EXPECT_FALSE(parser.parse("ab"));
}