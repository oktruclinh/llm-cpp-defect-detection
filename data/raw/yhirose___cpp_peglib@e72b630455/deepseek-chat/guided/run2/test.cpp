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
        parser = std::make_unique<peg::parser>(
            "ROOT <- 'hello' 'world'"
        );
        
        // Define actions to populate semantic values
        parser->["ROOT"] = [](const peg::SemanticValues& sv) {
            return sv.str();
        };
    }

    std::unique_ptr<peg::parser> parser;
};

// Test basic construction and default values
TEST_F(SemanticValuesTest, DefaultConstruction) {
    peg::SemanticValues sv;
    EXPECT_EQ(sv.c_str(), nullptr);
    EXPECT_EQ(sv.length(), 0);
    EXPECT_EQ(sv.str(), "");
    EXPECT_EQ(sv.choice(), 0);
    EXPECT_TRUE(sv.empty());
    EXPECT_EQ(sv.size(), 0);
}

// Test after successful parse with matched string
TEST_F(SemanticValuesTest, AfterParse) {
    auto ret = parser->parse("helloworld");
    ASSERT_TRUE(ret.ret);
    
    // The semantic values should contain the matched string
    auto result = ret.val.get<std::string>();
    EXPECT_EQ(result, "helloworld");
}

// Test with tokens
TEST_F(SemanticValuesTest, WithTokens) {
    peg::parser p("ROOT <- < 'a' > < 'b' >");
    p["ROOT"] = [](const peg::SemanticValues& sv) {
        EXPECT_EQ(sv.tokens.size(), 2);
        EXPECT_EQ(sv.token(0), "a");
        EXPECT_EQ(sv.token(1), "b");
        return sv.str();
    };
    
    auto ret = p.parse("ab");
    ASSERT_TRUE(ret.ret);
    EXPECT_EQ(ret.val.get<std::string>(), "ab");
}

// Test line_info functionality
TEST_F(SemanticValuesTest, LineInfo) {
    peg::parser p("ROOT <- 'test'");
    p["ROOT"] = [](const peg::SemanticValues& sv) {
        auto info = sv.line_info();
        EXPECT_GE(info.first, 1);  // Line number should be >= 1
        EXPECT_GE(info.second, 1); // Column should be >= 1
        return sv.str();
    };
    
    auto ret = p.parse("test");
    ASSERT_TRUE(ret.ret);
}

// Test choice number
TEST_F(SemanticValuesTest, ChoiceNumber) {
    peg::parser p("ROOT <- 'a' / 'b'");
    p["ROOT"] = [](const peg::SemanticValues& sv) {
        EXPECT_TRUE(sv.choice() == 0 || sv.choice() == 1);
        return sv.str();
    };
    
    auto ret = p.parse("a");
    ASSERT_TRUE(ret.ret);
    EXPECT_EQ(ret.val.get<std::string>(), "a");
}

// Test transform method
TEST_F(SemanticValuesTest, Transform) {
    peg::parser p("ROOT <- 'a' 'b' 'c'");
    p["ROOT"] = [](const peg::SemanticValues& sv) {
        // Test transform with default range
        auto result = sv.transform<std::string>();
        EXPECT_EQ(result.size(), 3);
        
        // Test transform with specific range
        auto partial = sv.transform<std::string>(1, 3);
        EXPECT_EQ(partial.size(), 2);
        
        return sv.str();
    };
    
    auto ret = p.parse("abc");
    ASSERT_TRUE(ret.ret);
}

// Test empty semantic values
TEST_F(SemanticValuesTest, EmptySemanticValues) {
    peg::parser p("ROOT <- ''");
    p["ROOT"] = [](const peg::SemanticValues& sv) {
        EXPECT_TRUE(sv.empty());
        EXPECT_EQ(sv.size(), 0);
        EXPECT_EQ(sv.str(), "");
        EXPECT_EQ(sv.length(), 0);
        return sv.str();
    };
    
    auto ret = p.parse("");
    ASSERT_TRUE(ret.ret);
    EXPECT_EQ(ret.val.get<std::string>(), "");
}

// Test with multiple nested semantic values
TEST_F(SemanticValuesTest, NestedSemanticValues) {
    peg::parser p("ROOT <- A B\nA <- 'x'\nB <- 'y'");
    p["ROOT"] = [](const peg::SemanticValues& sv) {
        EXPECT_EQ(sv.size(), 2);
        return sv.str();
    };
    p["A"] = [](const peg::SemanticValues& sv) {
        return sv.str();
    };
    p["B"] = [](const peg::SemanticValues& sv) {
        return sv.str();
    };
    
    auto ret = p.parse("xy");
    ASSERT_TRUE(ret.ret);
    EXPECT_EQ(ret.val.get<std::string>(), "xy");
}

// Test token access with empty tokens
TEST_F(SemanticValuesTest, EmptyTokens) {
    peg::parser p("ROOT <- 'test'");
    p["ROOT"] = [](const peg::SemanticValues& sv) {
        EXPECT_TRUE(sv.tokens.empty());
        // token() should return the matched string when tokens are empty
        EXPECT_EQ(sv.token(), "test");
        return sv.str();
    };
    
    auto ret = p.parse("test");
    ASSERT_TRUE(ret.ret);
}

// Test transform with empty range
TEST_F(SemanticValuesTest, TransformEmptyRange) {
    peg::parser p("ROOT <- 'a' 'b'");
    p["ROOT"] = [](const peg::SemanticValues& sv) {
        auto result = sv.transform<std::string>(0, 0);
        EXPECT_TRUE(result.empty());
        return sv.str();
    };
    
    auto ret = p.parse("ab");
    ASSERT_TRUE(ret.ret);
}

// Test transform with out-of-bounds range
TEST_F(SemanticValuesTest, TransformOutOfBounds) {
    peg::parser p("ROOT <- 'a'");
    p["ROOT"] = [](const peg::SemanticValues& sv) {
        auto result = sv.transform<std::string>(0, 100);
        EXPECT_EQ(result.size(), 1);  // Should clamp to actual size
        return sv.str();
    };
    
    auto ret = p.parse("a");
    ASSERT_TRUE(ret.ret);
}

// Test that SemanticValues can be used with different value types
TEST_F(SemanticValuesTest, DifferentValueTypes) {
    peg::parser p("ROOT <- '1' '2'");
    p["ROOT"] = [](const peg::SemanticValues& sv) {
        auto ints = sv.transform<int>([](const peg::any& v) { 
            return std::stoi(v.get<std::string>()); 
        });
        EXPECT_EQ(ints.size(), 2);
        EXPECT_EQ(ints[0], 1);
        EXPECT_EQ(ints[1], 2);
        return sv.str();
    };
    
    auto ret = p.parse("12");
    ASSERT_TRUE(ret.ret);
}