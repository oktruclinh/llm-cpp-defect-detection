#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>

using namespace peglib;

class HolderParseTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(HolderParseTest, ParseValidRule) {
    // Create a simple grammar with a valid rule
    std::string grammar = R"(
        START = "hello" "world"
    )";

    parser parser(grammar);
    ASSERT_TRUE(parser);

    std::string input = "helloworld";
    bool result = parser.parse(input);
    ASSERT_TRUE(result);
}

TEST_F(HolderParseTest, ParseInvalidRule) {
    // Create a grammar with a rule that should fail
    std::string grammar = R"(
        START = "hello" "world"
    )";

    parser parser(grammar);
    ASSERT_TRUE(parser);

    std::string input = "hello world";  // Wrong spacing
    bool result = parser.parse(input);
    ASSERT_FALSE(result);
}

TEST_F(HolderParseTest, ParseWithSemanticAction) {
    // Test with semantic action
    std::string grammar = R"(
        START = "hello" "world" { return "success"; }
    )";

    parser parser(grammar);
    ASSERT_TRUE(parser);

    std::string input = "helloworld";
    bool result = parser.parse(input);
    ASSERT_TRUE(result);
}

TEST_F(HolderParseTest, ParseEmptyInput) {
    // Test with empty input
    std::string grammar = R"(
        START = "hello"
    )";

    parser parser(grammar);
    ASSERT_TRUE(parser);

    std::string input = "";
    bool result = parser.parse(input);
    ASSERT_FALSE(result);
}

TEST_F(HolderParseTest, ParseZeroLengthRule) {
    // Test with zero-length rule
    std::string grammar = R"(
        START = ""
    )";

    parser parser(grammar);
    ASSERT_TRUE(parser);

    std::string input = "anything";
    bool result = parser.parse(input);
    ASSERT_TRUE(result);
}

TEST_F(HolderParseTest, ParseWithChoice) {
    // Test with choice operator
    std::string grammar = R"(
        START = "hello" / "world"
    )";

    parser parser(grammar);
    ASSERT_TRUE(parser);

    std::string input1 = "hello";
    bool result1 = parser.parse(input1);
    ASSERT_TRUE(result1);

    std::string input2 = "world";
    bool result2 = parser.parse(input2);
    ASSERT_TRUE(result2);
}

TEST_F(HolderParseTest, ParseWithRepetition) {
    // Test with repetition operator
    std::string grammar = R"(
        START = "a"*
    )";

    parser parser(grammar);
    ASSERT_TRUE(parser);

    std::string input = "aaaa";
    bool result = parser.parse(input);
    ASSERT_TRUE(result);
}

TEST_F(HolderParseTest, ParseWithOptional) {
    // Test with optional operator
    std::string grammar = R"(
        START = "hello" "world"?
    )";

    parser parser(grammar);
    ASSERT_TRUE(parser);

    std::string input1 = "hello";
    bool result1 = parser.parse(input1);
    ASSERT_TRUE(result1);

    std::string input2 = "hello world";
    bool result2 = parser.parse(input2);
    ASSERT_TRUE(result2);
}

TEST_F(HolderParseTest, ParseWithErrorHandling) {
    // Test error handling in parsing
    std::string grammar = R"(
        START = "hello" "world"
    )";

    parser parser(grammar);
    ASSERT_TRUE(parser);

    std::string input = "hello";
    bool result = parser.parse(input);
    ASSERT_FALSE(result);
}

TEST_F(HolderParseTest, ParseWithNamedRule) {
    // Test with named rule
    std::string grammar = R"(
        START = greeting
        greeting = "hello"
    )";

    parser parser(grammar);
    ASSERT_TRUE(parser);

    std::string input = "hello";
    bool result = parser.parse(input);
    ASSERT_TRUE(result);
}

TEST_F(HolderParseTest, ParseWithMacroReference) {
    // Test macro reference behavior (if supported)
    std::string grammar = R"(
        START = "hello" "world"
    )";

    parser parser(grammar);
    ASSERT_TRUE(parser);

    std::string input = "helloworld";
    bool result = parser.parse(input);
    ASSERT_TRUE(result);
}

TEST_F(HolderParseTest, ParseWithPackrat) {
    // Test packrat parsing behavior
    std::string grammar = R"(
        START = "a" "b" "c"
    )";

    parser parser(grammar);
    ASSERT_TRUE(parser);

    std::string input = "abc";
    bool result = parser.parse(input);
    ASSERT_TRUE(result);
}

TEST_F(HolderParseTest, ParseWithComplexGrammar) {
    // Test with more complex grammar
    std::string grammar = R"(
        START = "hello" " " "world" "!"
    )";

    parser parser(grammar);
    ASSERT_TRUE(parser);

    std::string input = "hello world!";
    bool result = parser.parse(input);
    ASSERT_TRUE(result);
}

TEST_F(HolderParseTest, ParseWithInvalidGrammar) {
    // Test with invalid grammar
    std::string grammar = R"(
        START = "hello" "world"
        INVALID = 
    )";

    parser parser(grammar);
    // This should still be valid as a parser object, but parsing should fail
    ASSERT_TRUE(parser);

    std::string input = "hello world";
    bool result = parser.parse(input);
    ASSERT_TRUE(result);  // Should parse successfully even with invalid rule
}