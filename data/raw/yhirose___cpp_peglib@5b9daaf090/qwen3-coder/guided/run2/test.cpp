#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>

using namespace peglib;

TEST(PackratParserTest, BasicParsing) {
    parser parser;
    parser["S"] = "a" >> "b" >> "c";
    
    std::string input = "abc";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PackratParserTest, ParsingWithAction) {
    parser parser;
    int action_called = 0;
    parser["S"] = "a" >> "b" >> "c" >> [&](const char* s, size_t n, const std::vector<any>& v, any& val) {
        action_called++;
        val = std::string(s, n);
    };
    
    std::string input = "abc";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
    EXPECT_EQ(action_called, 1);
}

TEST(PackratParserTest, ParsingFailure) {
    parser parser;
    parser["S"] = "a" >> "b" >> "c";
    
    std::string input = "abd";
    bool result = parser.parse(input);
    EXPECT_FALSE(result);
}

TEST(PackratParserTest, EmptyInput) {
    parser parser;
    parser["S"] = epsilon();
    
    std::string input = "";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PackratParserTest, EmptyRule) {
    parser parser;
    parser["S"] = epsilon() >> "a";
    
    std::string input = "a";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PackratParserTest, BeforeAfterActions) {
    parser parser;
    int before_called = 0;
    int after_called = 0;
    
    parser["S"] = rule("S").before([&]() {
        before_called++;
    }).after([&]() {
        after_called++;
    }) >> "a";
    
    std::string input = "a";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
    EXPECT_EQ(before_called, 1);
    EXPECT_EQ(after_called, 1);
}

TEST(PackratParserTest, ParseErrorInAction) {
    parser parser;
    parser["S"] = "a" >> "b" >> "c" >> [&](const char* s, size_t n, const std::vector<any>& v, any& val) {
        throw parse_error("Custom error");
    };
    
    std::string input = "abc";
    bool result = parser.parse(input);
    EXPECT_FALSE(result);
}

TEST(PackratParserTest, ComplexGrammar) {
    parser parser;
    parser["Expr"] = "a" >> "b" >> "c" | "x" >> "y" >> "z";
    
    std::string input1 = "abc";
    std::string input2 = "xyz";
    std::string input3 = "abx";
    
    EXPECT_TRUE(parser.parse(input1));
    EXPECT_TRUE(parser.parse(input2));
    EXPECT_FALSE(parser.parse(input3));
}

TEST(PackratParserTest, TokenBoundaryTracking) {
    parser parser;
    parser["S"] = "a" >> "b" >> "c";
    
    std::string input = "abc";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PackratParserTest, NestedRules) {
    parser parser;
    parser["A"] = "a";
    parser["B"] = "b";
    parser["S"] = "a" >> "b" >> "c" | "a" >> "b";
    
    std::string input1 = "ab";
    std::string input2 = "abc";
    
    EXPECT_TRUE(parser.parse(input1));
    EXPECT_TRUE(parser.parse(input2));
}

TEST(PackratParserTest, RuleWithQuantifiers) {
    parser parser;
    parser["S"] = "a"* >> "b";
    
    std::string input1 = "aaab";
    std::string input2 = "b";
    
    EXPECT_TRUE(parser.parse(input1));
    EXPECT_TRUE(parser.parse(input2));
}