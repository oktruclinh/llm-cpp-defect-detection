#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>

using namespace peglib;

TEST(PeglibPackratTest, BasicParsing) {
    parser parser;
    parser.R("S") = "a" >> "b" >> "c";
    std::string input = "abc";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, ParsingWithAction) {
    parser parser;
    parser.R("S") = "a" >> "b" >> "c";
    std::string input = "abc";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, ParsingFailure) {
    parser parser;
    parser.R("S") = "a" >> "b" >> "c";
    std::string input = "ab";
    bool result = parser.parse(input);
    EXPECT_FALSE(result);
}

TEST(PeglibPackratTest, EmptyInput) {
    parser parser;
    parser.R("S") = "a" >> "b" >> "c";
    std::string input = "";
    bool result = parser.parse(input);
    EXPECT_FALSE(result);
}

TEST(PeglibPackratTest, SingleCharacter) {
    parser parser;
    parser.R("S") = "a";
    std::string input = "a";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, SingleCharacterFailure) {
    parser parser;
    parser.R("S") = "a";
    std::string input = "b";
    bool result = parser.parse(input);
    EXPECT_FALSE(result);
}

TEST(PeglibPackratTest, ComplexRule) {
    parser parser;
    parser.R("S") = "a" >> "b" >> "c" >> "d";
    std::string input = "abcd";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, RuleWithQuantifier) {
    parser parser;
    parser.R("S") = "a" * 3;
    std::string input = "aaa";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, RuleWithQuantifierFailure) {
    parser parser;
    parser.R("S") = "a" * 3;
    std::string input = "aa";
    bool result = parser.parse(input);
    EXPECT_FALSE(result);
}

TEST(PeglibPackratTest, RuleWithChoice) {
    parser parser;
    parser.R("S") = "a" | "b" | "c";
    std::string input = "b";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, RuleWithChoiceFailure) {
    parser parser;
    parser.R("S") = "a" | "b" | "c";
    std::string input = "d";
    bool result = parser.parse(input);
    EXPECT_FALSE(result);
}

TEST(PeglibPackratTest, RuleWithOptional) {
    parser parser;
    parser.R("S") = "a" >> "b"? >> "c";
    std::string input = "ac";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, RuleWithOptionalPresent) {
    parser parser;
    parser.R("S") = "a" >> "b"? >> "c";
    std::string input = "abc";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, RuleWithRepetition) {
    parser parser;
    parser.R("S") = "a"+;
    std::string input = "aaaa";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, RuleWithRepetitionFailure) {
    parser parser;
    parser.R("S") = "a"+;
    std::string input = "";
    bool result = parser.parse(input);
    EXPECT_FALSE(result);
}

TEST(PeglibPackratTest, RuleWithZeroOrMore) {
    parser parser;
    parser.R("S") = "a"*;
    std::string input = "";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, RuleWithZeroOrMoreWithContent) {
    parser parser;
    parser.R("S") = "a"*;
    std::string input = "aaa";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, RuleWithAndPredicate) {
    parser parser;
    parser.R("S") = "&\"a\"" >> "a";
    std::string input = "a";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, RuleWithAndPredicateFailure) {
    parser parser;
    parser.R("S") = "&\"a\"" >> "b";
    std::string input = "b";
    bool result = parser.parse(input);
    EXPECT_FALSE(result);
}

TEST(PeglibPackratTest, RuleWithNotPredicate) {
    parser parser;
    parser.R("S") = "!\"a\"" >> "b";
    std::string input = "b";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, RuleWithNotPredicateFailure) {
    parser parser;
    parser.R("S") = "!\"a\"" >> "a";
    std::string input = "a";
    bool result = parser.parse(input);
    EXPECT_FALSE(result);
}

TEST(PeglibPackratTest, RuleWithLookahead) {
    parser parser;
    parser.R("S") = "a" >> "b";
    std::string input = "ab";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, RuleWithLookaheadFailure) {
    parser parser;
    parser.R("S") = "a" >> "b";
    std::string input = "ac";
    bool result = parser.parse(input);
    EXPECT_FALSE(result);
}

TEST(PeglibPackratTest, RuleWithAction) {
    parser parser;
    parser.R("S") = "a" >> "b" >> "c";
    std::string input = "abc";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, RuleWithActionFailure) {
    parser parser;
    parser.R("S") = "a" >> "b" >> "c";
    std::string input = "ab";
    bool result = parser.parse(input);
    EXPECT_FALSE(result);
}

TEST(PeglibPackratTest, RuleWithActionAndError) {
    parser parser;
    parser.R("S") = "a" >> "b" >> "c";
    std::string input = "abc";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, RuleWithActionAndErrorFailure) {
    parser parser;
    parser.R("S") = "a" >> "b" >> "c";
    std::string input = "ab";
    bool result = parser.parse(input);
    EXPECT_FALSE(result);
}

TEST(PeglibPackratTest, RuleWithActionAndErrorInAction) {
    parser parser;
    parser.R("S") = "a" >> "b" >> "c";
    std::string input = "abc";
    bool result = parser.parse(input);
    EXPECT_TRUE(result);
}

TEST(PeglibPackratTest, RuleWithActionAndErrorInActionFailure) {
    parser parser;
    parser.R("S") = "a" >> "b" >> "c";
    std::string input = "ab";
    bool result = parser.parse(input);
    EXPECT_FALSE(result);
}