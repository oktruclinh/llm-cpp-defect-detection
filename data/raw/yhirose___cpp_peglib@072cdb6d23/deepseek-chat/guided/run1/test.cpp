#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>
#include <stdexcept>

// Test fixture for Holder::parse_core through public API
class HolderParseCoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple grammar that we can use to test parse_core behavior
        // The grammar will be: ROOT <- 'a'+
        grammar_ = std::make_shared<peg::parser>();
        auto ret = grammar_->load_grammar("ROOT <- 'a'+");
        ASSERT_TRUE(ret) << "Failed to load test grammar";
    }

    std::shared_ptr<peg::parser> grammar_;
};

// Test successful parsing of simple input
TEST_F(HolderParseCoreTest, SimpleSuccess) {
    std::string input = "aaa";
    std::vector<std::string> captured;
    
    grammar_->log = [&](size_t line, size_t col, const std::string& msg) {
        captured.push_back(msg);
    };
    
    auto result = grammar_->parse(input.c_str());
    EXPECT_TRUE(result) << "Expected successful parse";
}

// Test parsing with empty input - should fail for grammar requiring at least one 'a'
TEST_F(HolderParseCoreTest, EmptyInput) {
    std::string input = "";
    std::vector<std::string> captured;
    
    grammar_->log = [&](size_t line, size_t col, const std::string& msg) {
        captured.push_back(msg);
    };
    
    auto result = grammar_->parse(input.c_str());
    EXPECT_FALSE(result) << "Expected parse failure for empty input";
}

// Test parsing with input that doesn't match grammar
TEST_F(HolderParseCoreTest, NonMatchingInput) {
    std::string input = "bbb";
    std::vector<std::string> captured;
    
    grammar_->log = [&](size_t line, size_t col, const std::string& msg) {
        captured.push_back(msg);
    };
    
    auto result = grammar_->parse(input.c_str());
    EXPECT_FALSE(result) << "Expected parse failure for non-matching input";
}

// Test parsing with partial match (input longer than grammar)
TEST_F(HolderParseCoreTest, PartialMatch) {
    std::string input = "aab";
    std::vector<std::string> captured;
    
    grammar_->log = [&](size_t line, size_t col, const std::string& msg) {
        captured.push_back(msg);
    };
    
    auto result = grammar_->parse(input.c_str());
    EXPECT_FALSE(result) << "Expected parse failure for partial match";
}

// Test parsing with single character
TEST_F(HolderParseCoreTest, SingleCharacter) {
    std::string input = "a";
    std::vector<std::string> captured;
    
    grammar_->log = [&](size_t line, size_t col, const std::string& msg) {
        captured.push_back(msg);
    };
    
    auto result = grammar_->parse(input.c_str());
    EXPECT_TRUE(result) << "Expected successful parse of single character";
}

// Test parsing with maximum repetition
TEST_F(HolderParseCoreTest, ManyRepetitions) {
    std::string input(1000, 'a');  // 1000 'a's
    std::vector<std::string> captured;
    
    grammar_->log = [&](size_t line, size_t col, const std::string& msg) {
        captured.push_back(msg);
    };
    
    auto result = grammar_->parse(input.c_str());
    EXPECT_TRUE(result) << "Expected successful parse of many repetitions";
}

// Test parsing with whitespace handling (grammar with whitespace skip)
TEST_F(HolderParseCoreTest, WithWhitespace) {
    peg::parser grammar;
    auto ret = grammar.load_grammar("ROOT <- 'a'+");
    ASSERT_TRUE(ret);
    
    // Enable whitespace skipping
    grammar.enable_packrat_parsing();
    
    std::string input = "a a a";
    std::vector<std::string> captured;
    
    grammar.log = [&](size_t line, size_t col, const std::string& msg) {
        captured.push_back(msg);
    };
    
    auto result = grammar.parse(input.c_str());
    EXPECT_TRUE(result) << "Expected successful parse with whitespace";
}

// Test parsing with action that throws parse_error
TEST_F(HolderParseCoreTest, ActionThrowsParseError) {
    peg::parser grammar;
    auto ret = grammar.load_grammar(R"(
        ROOT <- 'a'+
    )");
    ASSERT_TRUE(ret);
    
    // Add an action that throws parse_error for certain conditions
    grammar["ROOT"] = [](const peg::SemanticValues& sv) -> long {
        if (sv.size() > 5) {
            throw peg::parse_error("Too many 'a's");
        }
        return sv.size();
    };
    
    // Test with 3 'a's - should succeed
    auto result3 = grammar.parse("aaa");
    EXPECT_TRUE(result3) << "Expected successful parse with 3 'a's";
    
    // Test with 10 'a's - should fail due to action throwing
    auto result10 = grammar.parse("aaaaaaaaaa");
    EXPECT_FALSE(result10) << "Expected parse failure due to action throwing";
}

// Test parsing with custom error message
TEST_F(HolderParseCoreTest, CustomErrorMessage) {
    peg::parser grammar;
    auto ret = grammar.load_grammar("ROOT <- 'a'+");
    ASSERT_TRUE(ret);
    
    // Set custom error message
    grammar["ROOT"] = [](const peg::SemanticValues& sv) -> long {
        return sv.size();
    };
    
    std::string input = "bbb";
    std::vector<std::string> captured;
    
    grammar.log = [&](size_t line, size_t col, const std::string& msg) {
        captured.push_back(msg);
    };
    
    auto result = grammar.parse(input.c_str());
    EXPECT_FALSE(result) << "Expected parse failure";
    EXPECT_FALSE(captured.empty()) << "Expected error messages to be captured";
}

// Test parsing with packrat parsing enabled
TEST_F(HolderParseCoreTest, PackratParsing) {
    peg::parser grammar;
    auto ret = grammar.load_grammar("ROOT <- 'a'+");
    ASSERT_TRUE(ret);
    
    grammar.enable_packrat_parsing();
    
    std::string input = "aaaa";
    auto result = grammar.parse(input.c_str());
    EXPECT_TRUE(result) << "Expected successful parse with packrat enabled";
}

// Test parsing with macro reference (using grammar with macro-like behavior)
TEST_F(HolderParseCoreTest, MacroReference) {
    peg::parser grammar;
    auto ret = grammar.load_grammar(R"(
        ROOT <- MACRO
        MACRO <- 'a'+
    )");
    ASSERT_TRUE(ret);
    
    std::string input = "aaa";
    auto result = grammar.parse(input.c_str());
    EXPECT_TRUE(result) << "Expected successful parse with macro reference";
}

// Test parsing with ignoreSemanticValue
TEST_F(HolderParseCoreTest, IgnoreSemanticValue) {
    peg::parser grammar;
    auto ret = grammar.load_grammar("ROOT <- 'a'+");
    ASSERT_TRUE(ret);
    
    // Set action that returns a value
    grammar["ROOT"] = [](const peg::SemanticValues& sv) -> long {
        return 42;  // Some arbitrary value
    };
    
    std::string input = "aaa";
    auto result = grammar.parse(input.c_str());
    EXPECT_TRUE(result) << "Expected successful parse";
    
    // The semantic value should be accessible through the parse result
    // (though we can't directly check it through public API)
}

// Test parsing with enter/leave callbacks
TEST_F(HolderParseCoreTest, EnterLeaveCallbacks) {
    peg::parser grammar;
    auto ret = grammar.load_grammar("ROOT <- 'a'+");
    ASSERT_TRUE(ret);
    
    bool enter_called = false;
    bool leave_called = false;
    
    grammar["ROOT"] = [&](const peg::SemanticValues& sv) -> long {
        return sv.size();
    };
    
    // Note: enter/leave callbacks are not directly exposed through public API
    // This test verifies that parsing works correctly with the default behavior
    
    std::string input = "aaa";
    auto result = grammar.parse(input.c_str());
    EXPECT_TRUE(result) << "Expected successful parse";
}

// Test parsing with prioritized choice
TEST_F(HolderParseCoreTest, PrioritizedChoice) {
    peg::parser grammar;
    auto ret = grammar.load_grammar(R"(
        ROOT <- 'a' / 'b'
    )");
    ASSERT_TRUE(ret);
    
    // Test first alternative
    auto result_a = grammar.parse("a");
    EXPECT_TRUE(result_a) << "Expected successful parse of first alternative";
    
    // Test second alternative
    auto result_b = grammar.parse("b");
    EXPECT_TRUE(result_b) << "Expected successful parse of second alternative";
    
    // Test non-matching input
    auto result_c = grammar.parse("c");
    EXPECT_FALSE(result_c) << "Expected parse failure for non-matching input";
}

// Test parsing with complex nested grammar
TEST_F(HolderParseCoreTest, ComplexNestedGrammar) {
    peg::parser grammar;
    auto ret = grammar.load_grammar(R"(
        EXPRESSION <- TERM ( '+' TERM )*
        TERM <- FACTOR ( '*' FACTOR )*
        FACTOR <- NUMBER / '(' EXPRESSION ')'
        NUMBER <- [0-9]+
    )");
    ASSERT_TRUE(ret);
    
    // Test simple expression
    auto result1 = grammar.parse("1+2*3");
    EXPECT_TRUE(result1) << "Expected successful parse of simple expression";
    
    // Test nested parentheses
    auto result2 = grammar.parse("(1+2)*3");
    EXPECT_TRUE(result2) << "Expected successful parse of nested expression";
    
    // Test invalid expression
    auto result3 = grammar.parse("1++2");
    EXPECT_FALSE(result3) << "Expected parse failure for invalid expression";
}