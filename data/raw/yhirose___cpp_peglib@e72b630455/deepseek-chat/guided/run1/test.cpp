#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>
#include <utility>

// Test fixture for SemanticValues tests
class SemanticValuesTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple grammar that captures semantic values
        grammar = R"(
            ROOT      <- NUMBER
            NUMBER    <- < [0-9]+ >
        )";
        
        parser = std::make_unique<peg::parser>(grammar);
        ASSERT_TRUE(parser->parse("42"));
    }

    std::string grammar;
    std::unique_ptr<peg::parser> parser;
};

// Test basic construction and default values
TEST_F(SemanticValuesTest, DefaultConstruction) {
    peg::SemanticValues sv;
    EXPECT_EQ(sv.length(), 0);
    EXPECT_EQ(sv.c_str(), nullptr);
    EXPECT_EQ(sv.str(), "");
    EXPECT_EQ(sv.choice(), 0);
    EXPECT_TRUE(sv.empty());
    EXPECT_EQ(sv.size(), 0);
}

// Test with captured values from parsing
TEST_F(SemanticValuesTest, CapturedValues) {
    auto result = parser->parse("42");
    ASSERT_TRUE(result);
    
    // Test that we can access the matched string
    EXPECT_EQ(result->length(), 2);
    EXPECT_STREQ(result->c_str(), "42");
    EXPECT_EQ(result->str(), "42");
}

// Test token access
TEST_F(SemanticValuesTest, TokenAccess) {
    peg::SemanticValues sv;
    sv.tokens.push_back({"hello", 5});
    sv.tokens.push_back({"world", 5});
    
    EXPECT_EQ(sv.token(0), "hello");
    EXPECT_EQ(sv.token(1), "world");
}

// Test token access with empty tokens (falls back to matched string)
TEST_F(SemanticValuesTest, TokenAccessEmptyTokens) {
    peg::SemanticValues sv;
    // Set matched string via internal mechanism (normally done by parser)
    // Since we can't directly set s_ and n_, we test through parser
    auto result = parser->parse("42");
    ASSERT_TRUE(result);
    
    // When tokens is empty, token() should return the matched string
    EXPECT_TRUE(result->tokens.empty());
    EXPECT_EQ(result->token(), "42");
}

// Test line_info functionality
TEST_F(SemanticValuesTest, LineInfo) {
    peg::SemanticValues sv;
    // line_info requires ss and s_ to be set, which happens during parsing
    // We test through the parser with multi-line input
    auto multiLineParser = std::make_unique<peg::parser>(R"(
        ROOT <- LINE+
        LINE <- < [^\n]* > '\n'?
    )");
    
    auto result = multiLineParser->parse("first\nsecond\nthird");
    ASSERT_TRUE(result);
    
    // The first match should be on line 1
    auto info = result->line_info();
    EXPECT_EQ(info.first, 1);  // line number
    EXPECT_EQ(info.second, 1); // column
}

// Test choice number
TEST_F(SemanticValuesTest, ChoiceNumber) {
    peg::SemanticValues sv;
    EXPECT_EQ(sv.choice(), 0);
    
    // Choice number is set internally by the parser during prioritized choice
    // We test through a grammar with alternatives
    auto choiceParser = std::make_unique<peg::parser>(R"(
        ROOT <- A / B
        A    <- "hello"
        B    <- "world"
    )");
    
    auto result = choiceParser->parse("hello");
    ASSERT_TRUE(result);
    EXPECT_EQ(result->choice(), 0);  // First alternative
    
    result = choiceParser->parse("world");
    ASSERT_TRUE(result);
    EXPECT_EQ(result->choice(), 1);  // Second alternative
}

// Test transform with default range
TEST_F(SemanticValuesTest, TransformDefaultRange) {
    peg::SemanticValues sv;
    sv.push_back(any(42));
    sv.push_back(any(100));
    sv.push_back(any(200));
    
    auto result = sv.transform<int>();
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], 42);
    EXPECT_EQ(result[1], 100);
    EXPECT_EQ(result[2], 200);
}

// Test transform with custom range
TEST_F(SemanticValuesTest, TransformCustomRange) {
    peg::SemanticValues sv;
    sv.push_back(any(10));
    sv.push_back(any(20));
    sv.push_back(any(30));
    sv.push_back(any(40));
    
    auto result = sv.transform<int>(1, 3);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 20);
    EXPECT_EQ(result[1], 30);
}

// Test transform with empty range
TEST_F(SemanticValuesTest, TransformEmptyRange) {
    peg::SemanticValues sv;
    sv.push_back(any(1));
    sv.push_back(any(2));
    
    auto result = sv.transform<int>(0, 0);
    EXPECT_TRUE(result.empty());
}

// Test transform with out-of-bounds range
TEST_F(SemanticValuesTest, TransformOutOfBoundsRange) {
    peg::SemanticValues sv;
    sv.push_back(any(1));
    sv.push_back(any(2));
    
    auto result = sv.transform<int>(0, 10);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 2);
}

// Test vector operations inherited from std::vector
TEST_F(SemanticValuesTest, VectorOperations) {
    peg::SemanticValues sv;
    
    // push_back and size
    sv.push_back(any(1));
    sv.push_back(any(2));
    EXPECT_EQ(sv.size(), 2);
    
    // operator[]
    EXPECT_EQ(sv[0].get<int>(), 1);
    EXPECT_EQ(sv[1].get<int>(), 2);
    
    // front and back
    EXPECT_EQ(sv.front().get<int>(), 1);
    EXPECT_EQ(sv.back().get<int>(), 2);
    
    // pop_back
    sv.pop_back();
    EXPECT_EQ(sv.size(), 1);
    
    // clear
    sv.clear();
    EXPECT_TRUE(sv.empty());
}

// Test with different value types
TEST_F(SemanticValuesTest, DifferentValueTypes) {
    peg::SemanticValues sv;
    
    sv.push_back(any(42));
    sv.push_back(any(std::string("hello")));
    sv.push_back(any(3.14));
    
    EXPECT_EQ(sv[0].get<int>(), 42);
    EXPECT_EQ(sv[1].get<std::string>(), "hello");
    EXPECT_DOUBLE_EQ(sv[2].get<double>(), 3.14);
}

// Test edge case: empty string match
TEST_F(SemanticValuesTest, EmptyStringMatch) {
    auto emptyParser = std::make_unique<peg::parser>(R"(
        ROOT <- ""
    )");
    
    auto result = emptyParser->parse("");
    ASSERT_TRUE(result);
    EXPECT_EQ(result->length(), 0);
    EXPECT_EQ(result->str(), "");
    EXPECT_EQ(result->c_str(), nullptr);
}

// Test edge case: very long matched string
TEST_F(SemanticValuesTest, LongStringMatch) {
    std::string longStr(10000, 'a');
    auto longParser = std::make_unique<peg::parser>(R"(
        ROOT <- < [a]+ >
    )");
    
    auto result = longParser->parse(longStr);
    ASSERT_TRUE(result);
    EXPECT_EQ(result->length(), 10000);
    EXPECT_EQ(result->str(), longStr);
}

// Test multiple tokens
TEST_F(SemanticValuesTest, MultipleTokens) {
    peg::SemanticValues sv;
    sv.tokens.push_back({"first", 5});
    sv.tokens.push_back({"second", 6});
    sv.tokens.push_back({"third", 5});
    
    EXPECT_EQ(sv.token(0), "first");
    EXPECT_EQ(sv.token(1), "second");
    EXPECT_EQ(sv.token(2), "third");
}

// Test transform with custom lambda
TEST_F(SemanticValuesTest, TransformWithLambda) {
    peg::SemanticValues sv;
    sv.push_back(any(1));
    sv.push_back(any(2));
    sv.push_back(any(3));
    
    auto result = sv.transform<int>(0, sv.size(), [](const any& v) {
        return v.get<int>() * 2;
    });
    
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], 2);
    EXPECT_EQ(result[1], 4);
    EXPECT_EQ(result[2], 6);
}