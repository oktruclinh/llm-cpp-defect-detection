```cpp
#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>
#include <utility>

// Test fixture for SemanticValues tests
class SemanticValuesTest : public ::testing::Test {
protected:
    peg::parser parser;
    
    void SetUp() override {
        // Create a simple grammar that produces semantic values
        std::string grammar = R"(
            ROOT <- NUMBER
            NUMBER <- [0-9]+
        )";
        ASSERT_TRUE(parser.load_grammar(grammar.c_str(), grammar.size()));
    }
};

// Test basic SemanticValues construction and default values
TEST_F(SemanticValuesTest, DefaultConstruction) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- 'a'
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    bool action_called = false;
    p["ROOT"] = [&](const peg::SemanticValues& vs) {
        action_called = true;
        // Test default values
        EXPECT_EQ(vs.choice(), 0);
        EXPECT_EQ(vs.length(), 1);
        EXPECT_STREQ(vs.c_str(), "a");
        EXPECT_EQ(vs.str(), "a");
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("a", result));
    EXPECT_TRUE(action_called);
}

// Test token() method with empty tokens
TEST_F(SemanticValuesTest, TokenEmptyTokens) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- 'hello'
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    p["ROOT"] = [](const peg::SemanticValues& vs) {
        // When tokens is empty, token() should return the matched string
        EXPECT_EQ(vs.token(), "hello");
        EXPECT_EQ(vs.token(0), "hello");  // id=0 should also work
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("hello", result));
}

// Test token() method with actual tokens
TEST_F(SemanticValuesTest, TokenWithTokens) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- <A> <B>
        A <- [a]+
        B <- [b]+
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    p["ROOT"] = [](const peg::SemanticValues& vs) {
        EXPECT_EQ(vs.tokens.size(), 2);
        EXPECT_EQ(vs.token(0), "aaa");
        EXPECT_EQ(vs.token(1), "bb");
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("aaabb", result));
}

// Test line_info() method
TEST_F(SemanticValuesTest, LineInfo) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- LINE
        LINE <- [^\n]+
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    p["ROOT"] = [](const peg::SemanticValues& vs) {
        auto info = vs.line_info();
        EXPECT_EQ(info.first, 1);  // line number
        EXPECT_EQ(info.second, 1); // column
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("test", result));
}

// Test line_info() with multi-line input
TEST_F(SemanticValuesTest, LineInfoMultiLine) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- LINE
        LINE <- [^\n]*
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    p["LINE"] = [](const peg::SemanticValues& vs) {
        auto info = vs.line_info();
        // Second line should have line number 2
        EXPECT_EQ(info.first, 2);
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("first\nsecond", result));
}

// Test choice() method with alternatives
TEST_F(SemanticValuesTest, ChoiceNumber) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- A / B
        A <- 'alpha'
        B <- 'beta'
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    p["ROOT"] = [](const peg::SemanticValues& vs) {
        // When matching 'alpha', choice should be 0
        EXPECT_EQ(vs.choice(), 0);
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("alpha", result));
}

// Test choice() with second alternative
TEST_F(SemanticValuesTest, ChoiceNumberSecond) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- A / B
        A <- 'alpha'
        B <- 'beta'
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    p["ROOT"] = [](const peg::SemanticValues& vs) {
        // When matching 'beta', choice should be 1
        EXPECT_EQ(vs.choice(), 1);
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("beta", result));
}

// Test transform() method
TEST_F(SemanticValuesTest, Transform) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- A+
        A <- [0-9]
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    p["A"] = [](const peg::SemanticValues& vs) {
        return std::stoi(vs.str());
    };
    
    p["ROOT"] = [](const peg::SemanticValues& vs) {
        auto transformed = vs.transform<int>();
        EXPECT_EQ(transformed.size(), 3);
        EXPECT_EQ(transformed[0], 1);
        EXPECT_EQ(transformed[1], 2);
        EXPECT_EQ(transformed[2], 3);
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("123", result));
}

// Test transform() with range
TEST_F(SemanticValuesTest, TransformWithRange) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- A+
        A <- [0-9]
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    p["A"] = [](const peg::SemanticValues& vs) {
        return std::stoi(vs.str());
    };
    
    p["ROOT"] = [](const peg::SemanticValues& vs) {
        // Transform only elements 1 to 2 (exclusive end)
        auto transformed = vs.transform<int>(1, 3);
        EXPECT_EQ(transformed.size(), 2);
        EXPECT_EQ(transformed[0], 2);
        EXPECT_EQ(transformed[1], 3);
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("12345", result));
}

// Test transform() with empty range
TEST_F(SemanticValuesTest, TransformEmptyRange) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- A+
        A <- [0-9]
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    p["A"] = [](const peg::SemanticValues& vs) {
        return std::stoi(vs.str());
    };
    
    p["ROOT"] = [](const peg::SemanticValues& vs) {
        // Transform with beg == end should return empty vector
        auto transformed = vs.transform<int>(2, 2);
        EXPECT_TRUE(transformed.empty());
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("123", result));
}

// Test transform() with out-of-bounds range
TEST_F(SemanticValuesTest, TransformOutOfBounds) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- A+
        A <- [0-9]
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    p["A"] = [](const peg::SemanticValues& vs) {
        return std::stoi(vs.str());
    };
    
    p["ROOT"] = [](const peg::SemanticValues& vs) {
        // Transform with end beyond size should clamp to size
        auto transformed = vs.transform<int>(1, 100);
        EXPECT_EQ(transformed.size(), 2);  // Only 2 elements from index 1 to end
        EXPECT_EQ(transformed[0], 2);
        EXPECT_EQ(transformed[1], 3);
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("123", result));
}

// Test str() method
TEST_F(SemanticValuesTest, StrMethod) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- 'hello world'
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    p["ROOT"] = [](const peg::SemanticValues& vs) {
        EXPECT_EQ(vs.str(), "hello world");
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("hello world", result));
}

// Test c_str() and length() methods
TEST_F(SemanticValuesTest, CStrAndLength) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- 'test'
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    p["ROOT"] = [](const peg::SemanticValues& vs) {
        EXPECT_STREQ(vs.c_str(), "test");
        EXPECT_EQ(vs.length(), 4);
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("test", result));
}

// Test empty match
TEST_F(SemanticValuesTest, EmptyMatch) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- ''
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    p["ROOT"] = [](const peg::SemanticValues& vs) {
        EXPECT_EQ(vs.length(), 0);
        EXPECT_EQ(vs.str(), "");
        EXPECT_STREQ(vs.c_str(), "");
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("", result));
}

// Test semantic values with multiple children
TEST_F(SemanticValuesTest, MultipleChildren) {
    peg::parser p;
    std::string grammar = R"(
        ROOT <- A B C
        A <- 'a'
        B <- 'b'
        C <- 'c'
    )";
    ASSERT_TRUE(p.load_grammar(grammar.c_str(), grammar.size()));
    
    p["A"] = [](const peg::SemanticValues& vs) { return 1; };
    p["B"] = [](const peg::SemanticValues& vs) { return 2; };
    p["C"] = [](const peg::SemanticValues& vs) { return 3; };
    
    p["ROOT"] = [](const peg::SemanticValues& vs) {
        EXPECT_EQ(vs.size(), 3);
        EXPECT_EQ(peg::any_cast<int>(vs[0]), 1);
        EXPECT_EQ(peg::any_cast<int>(vs[1]), 2);
        EXPECT_EQ(peg::any_cast<int>(vs[2]), 3);
        return 0;
    };
    
    int result;
    ASSERT_TRUE(p.parse("abc", result));
}