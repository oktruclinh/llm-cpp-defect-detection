#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>
#include <stdexcept>

// Test fixture for packrat parsing tests
class PackratTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup common test data
    }
};

// Test basic successful packrat parsing
TEST_F(PackratTest, BasicPackratParsing) {
    peg::parser parser(R"(
        ROOT <- 'hello'
    )");
    
    ASSERT_TRUE(parser);
    
    std::string input = "hello";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
}

// Test packrat parsing with semantic action
TEST_F(PackratTest, PackratWithAction) {
    peg::parser parser(R"(
        ROOT <- 'world'
    )");
    
    ASSERT_TRUE(parser);
    
    parser["ROOT"] = [](const peg::SemanticValues& sv) -> std::any {
        return std::string("transformed");
    };
    
    std::string input = "world";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
    EXPECT_EQ("transformed", std::any_cast<std::string>(val));
}

// Test packrat parsing with before/after callbacks
TEST_F(PackratTest, PackratWithCallbacks) {
    peg::parser parser(R"(
        ROOT <- 'test'
    )");
    
    ASSERT_TRUE(parser);
    
    bool before_called = false;
    bool after_called = false;
    
    parser["ROOT"].before = [&before_called](peg::any& dt) {
        before_called = true;
    };
    
    parser["ROOT"].after = [&after_called](peg::any& dt) {
        after_called = true;
    };
    
    std::string input = "test";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
    EXPECT_TRUE(before_called);
    EXPECT_TRUE(after_called);
}

// Test packrat parsing with parse_error exception
TEST_F(PackratTest, PackratWithParseError) {
    peg::parser parser(R"(
        ROOT <- 'valid'
    )");
    
    ASSERT_TRUE(parser);
    
    parser["ROOT"] = [](const peg::SemanticValues& sv) -> std::any {
        throw peg::parse_error("Custom parse error");
    };
    
    std::string input = "valid";
    std::any val;
    EXPECT_FALSE(parser.parse(input, val));
}

// Test packrat parsing with empty input
TEST_F(PackratTest, PackratEmptyInput) {
    peg::parser parser(R"(
        ROOT <- ''
    )");
    
    ASSERT_TRUE(parser);
    
    std::string input = "";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
}

// Test packrat parsing with nested rules
TEST_F(PackratTest, PackratNestedRules) {
    peg::parser parser(R"(
        ROOT <- A B
        A <- 'a'
        B <- 'b'
    )");
    
    ASSERT_TRUE(parser);
    
    std::string input = "ab";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
}

// Test packrat parsing with complex semantic values
TEST_F(PackratTest, PackratComplexValues) {
    peg::parser parser(R"(
        ROOT <- NUMBER
        NUMBER <- [0-9]+
    )");
    
    ASSERT_TRUE(parser);
    
    parser["NUMBER"] = [](const peg::SemanticValues& sv) -> std::any {
        return std::stoi(sv.str());
    };
    
    std::string input = "12345";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
    EXPECT_EQ(12345, std::any_cast<int>(val));
}

// Test packrat parsing with backtracking
TEST_F(PackratTest, PackratBacktracking) {
    peg::parser parser(R"(
        ROOT <- 'abc' / 'ab'
    )");
    
    ASSERT_TRUE(parser);
    
    std::string input = "ab";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
}

// Test packrat parsing with optional elements
TEST_F(PackratTest, PackratOptional) {
    peg::parser parser(R"(
        ROOT <- 'a' 'b'?
    )");
    
    ASSERT_TRUE(parser);
    
    std::string input = "a";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
}

// Test packrat parsing with repetition
TEST_F(PackratTest, PackratRepetition) {
    peg::parser parser(R"(
        ROOT <- [0-9]*
    )");
    
    ASSERT_TRUE(parser);
    
    std::string input = "123";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
}

// Test packrat parsing failure case
TEST_F(PackratTest, PackratFailure) {
    peg::parser parser(R"(
        ROOT <- 'expected'
    )");
    
    ASSERT_TRUE(parser);
    
    std::string input = "wrong";
    std::any val;
    EXPECT_FALSE(parser.parse(input, val));
}

// Test packrat parsing with multiple semantic actions
TEST_F(PackratTest, PackratMultipleActions) {
    peg::parser parser(R"(
        ROOT <- A B
        A <- 'hello'
        B <- 'world'
    )");
    
    ASSERT_TRUE(parser);
    
    parser["A"] = [](const peg::SemanticValues& sv) -> std::any {
        return std::string("greeting");
    };
    
    parser["B"] = [](const peg::SemanticValues& sv) -> std::any {
        return std::string("planet");
    };
    
    std::string input = "helloworld";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
}

// Test packrat parsing with user-defined data
TEST_F(PackratTest, PackratWithUserData) {
    peg::parser parser(R"(
        ROOT <- 'data'
    )");
    
    ASSERT_TRUE(parser);
    
    std::any user_data = std::string("user_context");
    std::string input = "data";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val, user_data));
}

// Test packrat parsing with lambda that modifies captured state
TEST_F(PackratTest, PackratStateModification) {
    peg::parser parser(R"(
        ROOT <- 'state'
    )");
    
    ASSERT_TRUE(parser);
    
    int counter = 0;
    parser["ROOT"] = [&counter](const peg::SemanticValues& sv) -> std::any {
        counter++;
        return counter;
    };
    
    std::string input = "state";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
    EXPECT_EQ(1, std::any_cast<int>(val));
    EXPECT_EQ(1, counter);
}

// Test packrat parsing with whitespace skipping
TEST_F(PackratTest, PackratWhitespaceSkip) {
    peg::parser parser(R"(
        ROOT <- 'a' 'b'
        %whitespace <- [ \t]*
    )");
    
    ASSERT_TRUE(parser);
    
    std::string input = "a b";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
}

// Test packrat parsing with character class
TEST_F(PackratTest, PackratCharacterClass) {
    peg::parser parser(R"(
        ROOT <- [a-z]+
    )");
    
    ASSERT_TRUE(parser);
    
    std::string input = "abcdef";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
}

// Test packrat parsing with negative lookahead
TEST_F(PackratTest, PackratNegativeLookahead) {
    peg::parser parser(R"(
        ROOT <- !'bad' 'good'
    )");
    
    ASSERT_TRUE(parser);
    
    std::string input = "good";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
}

// Test packrat parsing with positive lookahead
TEST_F(PackratTest, PackratPositiveLookahead) {
    peg::parser parser(R"(
        ROOT <- &'prefix' 'prefixsuffix'
    )");
    
    ASSERT_TRUE(parser);
    
    std::string input = "prefixsuffix";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
}

// Test packrat parsing with recursive grammar
TEST_F(PackratTest, PackratRecursive) {
    peg::parser parser(R"(
        ROOT <- NUMBER
        NUMBER <- [0-9]+
    )");
    
    ASSERT_TRUE(parser);
    
    std::string input = "42";
    std::any val;
    EXPECT_TRUE(parser.parse(input, val));
}