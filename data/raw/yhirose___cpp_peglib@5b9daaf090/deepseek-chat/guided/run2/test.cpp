#include <gtest/gtest.h>
#include "peglib.h"

using namespace peg;

// Test that a simple grammar with a packrat rule works correctly
TEST(PackratTest, SimpleGrammar) {
    parser p(R"(
        ROOT <- 'a'+
    )");
    
    EXPECT_TRUE(p.parse("aaa"));
    EXPECT_TRUE(p.parse("a"));
    EXPECT_FALSE(p.parse(""));
    EXPECT_FALSE(p.parse("b"));
}

// Test that packrat parsing correctly handles backtracking
TEST(PackratTest, Backtracking) {
    parser p(R"(
        ROOT <- 'a'* 'b'
    )");
    
    EXPECT_TRUE(p.parse("b"));
    EXPECT_TRUE(p.parse("ab"));
    EXPECT_TRUE(p.parse("aaab"));
    EXPECT_FALSE(p.parse("a"));
    EXPECT_FALSE(p.parse("aaa"));
}

// Test that packrat parsing works with semantic actions
TEST(PackratTest, WithSemanticAction) {
    parser p(R"(
        ROOT <- < 'a'+ >
    )");
    
    p["ROOT"] = [](const SemanticValues& sv) {
        return static_cast<int>(sv.str().size());
    };
    
    int result;
    EXPECT_TRUE(p.parse("aaa", result));
    EXPECT_EQ(3, result);
    
    EXPECT_TRUE(p.parse("a", result));
    EXPECT_EQ(1, result);
}

// Test that packrat parsing correctly handles empty matches
TEST(PackratTest, EmptyMatch) {
    parser p(R"(
        ROOT <- ''
    )");
    
    EXPECT_TRUE(p.parse(""));
    EXPECT_TRUE(p.parse("abc"));
}

// Test that packrat parsing correctly handles optional elements
TEST(PackratTest, OptionalElement) {
    parser p(R"(
        ROOT <- 'a'? 'b'
    )");
    
    EXPECT_TRUE(p.parse("b"));
    EXPECT_TRUE(p.parse("ab"));
    EXPECT_FALSE(p.parse("a"));
    EXPECT_FALSE(p.parse(""));
}

// Test that packrat parsing correctly handles choice with backtracking
TEST(PackratTest, ChoiceWithBacktracking) {
    parser p(R"(
        ROOT <- 'ab' / 'a'
    )");
    
    EXPECT_TRUE(p.parse("ab"));
    EXPECT_TRUE(p.parse("a"));
    EXPECT_FALSE(p.parse("b"));
    EXPECT_FALSE(p.parse(""));
}

// Test that packrat parsing correctly handles repetition with minimum count
TEST(PackratTest, RepetitionWithMinimum) {
    parser p(R"(
        ROOT <- 'a'{2,4}
    )");
    
    EXPECT_TRUE(p.parse("aa"));
    EXPECT_TRUE(p.parse("aaa"));
    EXPECT_TRUE(p.parse("aaaa"));
    EXPECT_FALSE(p.parse("a"));
    EXPECT_FALSE(p.parse("aaaaa"));
    EXPECT_FALSE(p.parse(""));
}

// Test that packrat parsing correctly handles sequences with actions
TEST(PackratTest, SequenceWithActions) {
    parser p(R"(
        ROOT <- NUMBER PLUS NUMBER
        NUMBER <- < [0-9]+ >
        PLUS <- '+'
    )");
    
    p["NUMBER"] = [](const SemanticValues& sv) {
        return std::stoi(sv.str());
    };
    
    p["ROOT"] = [](const SemanticValues& sv) {
        return sv[0].get<int>() + sv[1].get<int>();
    };
    
    int result;
    EXPECT_TRUE(p.parse("3+4", result));
    EXPECT_EQ(7, result);
    
    EXPECT_TRUE(p.parse("10+20", result));
    EXPECT_EQ(30, result);
}

// Test that packrat parsing correctly handles nested rules
TEST(PackratTest, NestedRules) {
    parser p(R"(
        ROOT <- EXPR
        EXPR <- TERM ( '+' TERM )*
        TERM <- FACTOR ( '*' FACTOR )*
        FACTOR <- NUMBER / '(' EXPR ')'
        NUMBER <- < [0-9]+ >
    )");
    
    p["NUMBER"] = [](const SemanticValues& sv) {
        return std::stoi(sv.str());
    };
    
    p["ROOT"] = [](const SemanticValues& sv) {
        return sv[0].get<int>();
    };
    
    int result;
    EXPECT_TRUE(p.parse("3+4*5", result));
    EXPECT_EQ(23, result);
    
    EXPECT_TRUE(p.parse("(3+4)*5", result));
    EXPECT_EQ(35, result);
}

// Test that packrat parsing correctly handles parse errors
TEST(PackratTest, ParseError) {
    parser p(R"(
        ROOT <- 'a'+
    )");
    
    EXPECT_THROW({
        p.parse("b");
    }, std::exception);
}

// Test that packrat parsing correctly handles complex backtracking scenarios
TEST(PackratTest, ComplexBacktracking) {
    parser p(R"(
        ROOT <- 'a'* 'b' 'c'
    )");
    
    EXPECT_TRUE(p.parse("bc"));
    EXPECT_TRUE(p.parse("abc"));
    EXPECT_TRUE(p.parse("aaabc"));
    EXPECT_FALSE(p.parse("b"));
    EXPECT_FALSE(p.parse("a"));
    EXPECT_FALSE(p.parse("ab"));
}

// Test that packrat parsing correctly handles predicates
TEST(PackratTest, Predicates) {
    parser p(R"(
        ROOT <- &'a' 'a'+
    )");
    
    EXPECT_TRUE(p.parse("a"));
    EXPECT_TRUE(p.parse("aaa"));
    EXPECT_FALSE(p.parse("b"));
    EXPECT_FALSE(p.parse(""));
}

// Test that packrat parsing correctly handles negative predicates
TEST(PackratTest, NegativePredicates) {
    parser p(R"(
        ROOT <- !'b' 'a'+
    )");
    
    EXPECT_TRUE(p.parse("a"));
    EXPECT_TRUE(p.parse("aaa"));
    EXPECT_FALSE(p.parse("b"));
    EXPECT_FALSE(p.parse("ab"));
}

// Test that packrat parsing correctly handles whitespace skipping
TEST(PackratTest, WhitespaceSkipping) {
    parser p(R"(
        ROOT <- 'a' 'b'
    )");
    
    p.enable_packrat_parsing();
    
    EXPECT_TRUE(p.parse("ab"));
    EXPECT_TRUE(p.parse("a b"));
    EXPECT_TRUE(p.parse("a  b"));
    EXPECT_FALSE(p.parse("a"));
    EXPECT_FALSE(p.parse("b"));
}