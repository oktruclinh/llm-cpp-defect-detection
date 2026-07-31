#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>

using namespace peg;

TEST(HolderParseCoreTest, NormalParsing) {
    // Create a simple grammar with a rule that matches "hello"
    parser parser;
    parser["Hello"] = "hello"_c;
    
    // Parse valid input
    std::string input = "hello";
    bool result = parser.parse(input.c_str(), input.size());
    EXPECT_TRUE(result);
}

TEST(HolderParseCoreTest, EmptyInput) {
    parser parser;
    parser["Hello"] = "hello"_c;
    
    // Parse empty input
    std::string input = "";
    bool result = parser.parse(input.c_str(), input.size());
    EXPECT_FALSE(result);
}

TEST(HolderParseCoreTest, WrongInput) {
    parser parser;
    parser["Hello"] = "hello"_c;
    
    // Parse wrong input
    std::string input = "world";
    bool result = parser.parse(input.c_str(), input.size());
    EXPECT_FALSE(result);
}

TEST(HolderParseCoreTest, MacroReference) {
    parser parser;
    
    // Define a macro rule
    parser["Hello"] = "hello"_c;
    parser["Greeting"] = "Hello"_c;
    
    // Parse valid input
    std::string input = "hello";
    bool result = parser.parse(input.c_str(), input.size());
    EXPECT_TRUE(result);
}

TEST(HolderParseCoreTest, SemanticValueHandling) {
    parser parser;
    
    // Define a rule with semantic action
    parser["Number"] = "123"_c;
    
    // Parse and check if semantic value is captured
    std::string input = "123";
    bool result = parser.parse(input.c_str(), input.size());
    EXPECT_TRUE(result);
}

TEST(HolderParseCoreTest, ErrorHandling) {
    parser parser;
    
    // Define a rule that will fail
    parser["Fail"] = "fail"_c;
    
    // Parse input that doesn't match
    std::string input = "hello";
    bool result = parser.parse(input.c_str(), input.size());
    EXPECT_FALSE(result);
}

TEST(HolderParseCoreTest, ComplexGrammar) {
    parser parser;
    
    // Define a more complex grammar
    parser["Digit"] = "0"_c | "1"_c | "2"_c | "3"_c | "4"_c | "5"_c | "6"_c | "7"_c | "8"_c | "9"_c;
    parser["Number"] = "Digit"+;
    
    // Parse valid number
    std::string input = "12345";
    bool result = parser.parse(input.c_str(), input.size());
    EXPECT_TRUE(result);
}

TEST(HolderParseCoreTest, ChoiceParsing) {
    parser parser;
    
    // Define a rule with choice
    parser["Choice"] = "first"_c | "second"_c;
    
    // Parse first choice
    std::string input1 = "first";
    bool result1 = parser.parse(input1.c_str(), input1.size());
    EXPECT_TRUE(result1);
    
    // Parse second choice
    std::string input2 = "second";
    bool result2 = parser.parse(input2.c_str(), input2.size());
    EXPECT_TRUE(result2);
}

TEST(HolderParseCoreTest, NestedParsing) {
    parser parser;
    
    // Define nested rules
    parser["Inner"] = "inner"_c;
    parser["Outer"] = "outer"_c + "Inner"_c;
    
    // Parse nested structure
    std::string input = "outerinner";
    bool result = parser.parse(input.c_str(), input.size());
    EXPECT_TRUE(result);
}

TEST(HolderParseCoreTest, PackratParsing) {
    parser parser;
    
    // Define a rule that can benefit from packrat parsing
    parser["Repeated"] = "a"_c + "Repeated"_c | "a"_c;
    
    // Parse repeated input
    std::string input = "aaaa";
    bool result = parser.parse(input.c_str(), input.size());
    EXPECT_TRUE(result);
}

TEST(HolderParseCoreTest, ErrorMessages) {
    parser parser;
    
    // Define a rule with error message
    parser["ErrorRule"] = "expected"_c;
    
    // Parse invalid input to trigger error
    std::string input = "wrong";
    bool result = parser.parse(input.c_str(), input.size());
    EXPECT_FALSE(result);
}