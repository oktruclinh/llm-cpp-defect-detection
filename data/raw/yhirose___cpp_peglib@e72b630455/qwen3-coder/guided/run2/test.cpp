#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>

TEST(SemanticValuesTest, Constructor) {
    peglib::SemanticValues sv;
    EXPECT_EQ(sv.c_str(), nullptr);
    EXPECT_EQ(sv.length(), 0);
    EXPECT_EQ(sv.choice(), 0);
    EXPECT_TRUE(sv.empty());
    EXPECT_EQ(sv.size(), 0);
}

TEST(SemanticValuesTest, StrMethod) {
    peglib::SemanticValues sv;
    sv.s_ = "test";
    sv.n_ = 4;
    EXPECT_EQ(sv.str(), "test");
}

TEST(SemanticValuesTest, TokenMethod) {
    peglib::SemanticValues sv;
    sv.s_ = "hello";
    sv.n_ = 5;
    
    // Test default token (should return the matched string)
    EXPECT_EQ(sv.token(), "hello");
    
    // Test with tokens
    sv.tokens.push_back(std::make_pair("token1", 6));
    EXPECT_EQ(sv.token(0), "token1");
    
    // Test with invalid index (should assert in debug, but we can't test that easily)
    // Just make sure it doesn't crash with valid access
    EXPECT_EQ(sv.token(0), "token1");
}

TEST(SemanticValuesTest, TransformMethod) {
    peglib::SemanticValues sv;
    
    // Test transform with empty semantic values
    auto result1 = sv.transform<int>();
    EXPECT_TRUE(result1.empty());
    
    // Add some values
    sv.push_back(42);
    sv.push_back(24);
    
    // Test transform to int
    auto result2 = sv.transform<int>();
    ASSERT_EQ(result2.size(), 2);
    EXPECT_EQ(result2[0], 42);
    EXPECT_EQ(result2[1], 24);
    
    // Test transform with range
    auto result3 = sv.transform<int>(0, 1);
    ASSERT_EQ(result3.size(), 1);
    EXPECT_EQ(result3[0], 42);
}

TEST(SemanticValuesTest, LineInfoMethod) {
    peglib::SemanticValues sv;
    sv.ss = "line1\nline2\nline3";
    sv.s_ = "line2";
    sv.n_ = 5;
    
    // This should return line 2, column 1 (0-indexed)
    auto line_info = sv.line_info();
    EXPECT_EQ(line_info.first, 2);  // line number (1-indexed in the API)
    EXPECT_EQ(line_info.second, 1); // column number (0-indexed)
}

TEST(SemanticValuesTest, ChoiceMethod) {
    peglib::SemanticValues sv;
    sv.choice_ = 5;
    EXPECT_EQ(sv.choice(), 5);
}

TEST(SemanticValuesTest, TokensMethod) {
    peglib::SemanticValues sv;
    
    // Test empty tokens
    EXPECT_TRUE(sv.tokens.empty());
    
    // Add tokens
    sv.tokens.push_back(std::make_pair("token1", 6));
    sv.tokens.push_back(std::make_pair("token2", 6));
    
    EXPECT_EQ(sv.tokens.size(), 2);
    EXPECT_EQ(sv.tokens[0].first, "token1");
    EXPECT_EQ(sv.tokens[0].second, 6);
    EXPECT_EQ(sv.tokens[1].first, "token2");
    EXPECT_EQ(sv.tokens[1].second, 6);
}

TEST(SemanticValuesTest, VectorOperations) {
    peglib::SemanticValues sv;
    
    // Test basic vector operations
    EXPECT_TRUE(sv.empty());
    EXPECT_EQ(sv.size(), 0);
    
    sv.push_back(100);
    EXPECT_FALSE(sv.empty());
    EXPECT_EQ(sv.size(), 1);
    EXPECT_EQ(sv[0], 100);
    
    sv.pop_back();
    EXPECT_TRUE(sv.empty());
    EXPECT_EQ(sv.size(), 0);
    
    sv.assign(3, 42);
    EXPECT_EQ(sv.size(), 3);
    EXPECT_EQ(sv[0], 42);
    EXPECT_EQ(sv[1], 42);
    EXPECT_EQ(sv[2], 42);
    
    sv.clear();
    EXPECT_TRUE(sv.empty());
    EXPECT_EQ(sv.size(), 0);
}

TEST(SemanticValuesTest, CStrAndLength) {
    peglib::SemanticValues sv;
    sv.s_ = "test string";
    sv.n_ = 11;
    
    EXPECT_STREQ(sv.c_str(), "test string");
    EXPECT_EQ(sv.length(), 11);
}

TEST(SemanticValuesTest, TransformWithLambda) {
    peglib::SemanticValues sv;
    sv.push_back(std::string("hello"));
    sv.push_back(std::string("world"));
    
    // Test transform with lambda
    auto result = sv.transform<std::string>(0, 2, [](const peglib::any& v) {
        return v.get<std::string>();
    });
    
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "hello");
    EXPECT_EQ(result[1], "world");
}