#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>

TEST(SemanticValuesTest, TestCStrAndLength) {
    peglib::SemanticValues sv;
    sv.s_ = "hello";
    sv.n_ = 5;
    
    EXPECT_EQ(sv.c_str(), std::string("hello"));
    EXPECT_EQ(sv.length(), 5);
}

TEST(SemanticValuesTest, TestStr) {
    peglib::SemanticValues sv;
    sv.s_ = "world";
    sv.n_ = 5;
    
    EXPECT_EQ(sv.str(), std::string("world"));
}

TEST(SemanticValuesTest, TestLineInfo) {
    const char* input = "line1\nline2\nline3";
    peglib::SemanticValues sv;
    sv.ss = input;
    sv.s_ = input + 6;  // Point to 'l' in 'line2'
    
    auto line_info = sv.line_info();
    EXPECT_EQ(line_info.first, 2);  // Line numbers are 1-based
    EXPECT_EQ(line_info.second, 1); // Column numbers are 0-based
}

TEST(SemanticValuesTest, TestChoice) {
    peglib::SemanticValues sv;
    sv.choice_ = 3;
    
    EXPECT_EQ(sv.choice(), 3);
}

TEST(SemanticValuesTest, TestTokenDefault) {
    peglib::SemanticValues sv;
    sv.s_ = "test";
    sv.n_ = 4;
    
    EXPECT_EQ(sv.token(), std::string("test"));
}

TEST(SemanticValuesTest, TestTokenWithId) {
    peglib::SemanticValues sv;
    sv.tokens.push_back(std::make_pair("first", 5));
    sv.tokens.push_back(std::make_pair("second", 6));
    
    EXPECT_EQ(sv.token(0), std::string("first"));
    EXPECT_EQ(sv.token(1), std::string("second"));
}

TEST(SemanticValuesTest, TestTokenEmptyTokens) {
    peglib::SemanticValues sv;
    sv.s_ = "test";
    sv.n_ = 4;
    
    EXPECT_EQ(sv.token(0), std::string("test"));
}

TEST(SemanticValuesTest, TestTransformEmpty) {
    peglib::SemanticValues sv;
    
    auto result = sv.transform<int>();
    EXPECT_TRUE(result.empty());
}

TEST(SemanticValuesTest, TestTransformWithValues) {
    peglib::SemanticValues sv;
    sv.push_back(peglib::any(42));
    sv.push_back(peglib::any(100));
    
    auto result = sv.transform<int>();
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 42);
    EXPECT_EQ(result[1], 100);
}

TEST(SemanticValuesTest, TestTransformWithRange) {
    peglib::SemanticValues sv;
    sv.push_back(peglib::any(10));
    sv.push_back(peglib::any(20));
    sv.push_back(peglib::any(30));
    
    auto result = sv.transform<int>(1, 3);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 20);
    EXPECT_EQ(result[1], 30);
}

TEST(SemanticValuesTest, TestTransformWithRangeBoundary) {
    peglib::SemanticValues sv;
    sv.push_back(peglib::any(10));
    sv.push_back(peglib::any(20));
    
    auto result = sv.transform<int>(0, static_cast<size_t>(-1));
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], 10);
    EXPECT_EQ(result[1], 20);
}

TEST(SemanticValuesTest, TestVectorOperations) {
    peglib::SemanticValues sv;
    
    // Test size and empty
    EXPECT_TRUE(sv.empty());
    EXPECT_EQ(sv.size(), 0);
    
    // Test push_back and size
    sv.push_back(peglib::any(42));
    EXPECT_FALSE(sv.empty());
    EXPECT_EQ(sv.size(), 1);
    
    // Test operator[]
    EXPECT_EQ(sv[0].get<int>(), 42);
    
    // Test at
    EXPECT_EQ(sv.at(0).get<int>(), 42);
    
    // Test front and back
    EXPECT_EQ(sv.front().get<int>(), 42);
    EXPECT_EQ(sv.back().get<int>(), 42);
    
    // Test pop_back
    sv.pop_back();
    EXPECT_TRUE(sv.empty());
    
    // Test clear
    sv.push_back(peglib::any(1));
    sv.push_back(peglib::any(2));
    sv.clear();
    EXPECT_TRUE(sv.empty());
}

TEST(SemanticValuesTest, TestVectorOperationsWithIterator) {
    peglib::SemanticValues sv;
    sv.push_back(peglib::any(10));
    sv.push_back(peglib::any(20));
    
    // Test begin and end
    auto it = sv.begin();
    EXPECT_EQ(it->get<int>(), 10);
    ++it;
    EXPECT_EQ(it->get<int>(), 20);
    
    // Test rbegin and rend
    auto rit = sv.rbegin();
    EXPECT_EQ(rit->get<int>(), 20);
    ++rit;
    EXPECT_EQ(rit->get<int>(), 10);
}

TEST(SemanticValuesTest, TestVectorOperationsWithResize) {
    peglib::SemanticValues sv;
    sv.resize(3);
    EXPECT_EQ(sv.size(), 3);
    
    // Test resize with value
    sv.resize(5, peglib::any(42));
    EXPECT_EQ(sv.size(), 5);
    EXPECT_EQ(sv[4].get<int>(), 42);
}

TEST(SemanticValuesTest, TestVectorOperationsWithInsert) {
    peglib::SemanticValues sv;
    sv.push_back(peglib::any(1));
    sv.push_back(peglib::any(3));
    
    sv.insert(sv.begin() + 1, peglib::any(2));
    EXPECT_EQ(sv.size(), 3);
    EXPECT_EQ(sv[1].get<int>(), 2);
}

TEST(SemanticValuesTest, TestVectorOperationsWithErase) {
    peglib::SemanticValues sv;
    sv.push_back(peglib::any(1));
    sv.push_back(peglib::any(2));
    sv.push_back(peglib::any(3));
    
    sv.erase(sv.begin() + 1);
    EXPECT_EQ(sv.size(), 2);
    EXPECT_EQ(sv[0].get<int>(), 1);
    EXPECT_EQ(sv[1].get<int>(), 3);
}

TEST(SemanticValuesTest, TestVectorOperationsWithAssign) {
    peglib::SemanticValues sv;
    std::vector<peglib::any> values = {peglib::any(10), peglib::any(20)};
    
    sv.assign(values.begin(), values.end());
    EXPECT_EQ(sv.size(), 2);
    EXPECT_EQ(sv[0].get<int>(), 10);
    EXPECT_EQ(sv[1].get<int>(), 20);
}