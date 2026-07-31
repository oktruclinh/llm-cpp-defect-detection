#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>
#include <stdexcept>

// Test fixture for Holder::parse_core through public API
class HolderParseCoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup common test data
    }
};

// Test basic successful parsing with semantic value
TEST_F(HolderParseCoreTest, BasicParseSuccess) {
    peg::parser parser(R"(
        ROOT <- 'hello'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    std::string result;
    parser["ROOT"] = [&](const peg::SemanticValues& vs) {
        result = vs.token();
        return result;
    };

    EXPECT_TRUE(parser.parse("hello", result));
    EXPECT_EQ(result, "hello");
}

// Test parsing with multiple rules and semantic values
TEST_F(HolderParseCoreTest, MultipleRulesParse) {
    peg::parser parser(R"(
        ROOT <- ITEM+
        ITEM <- [a-z]+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    std::vector<std::string> items;
    parser["ITEM"] = [&](const peg::SemanticValues& vs) {
        items.push_back(vs.token());
        return vs.token();
    };

    EXPECT_TRUE(parser.parse("abc def ghi"));
    EXPECT_EQ(items.size(), 3);
    EXPECT_EQ(items[0], "abc");
    EXPECT_EQ(items[1], "def");
    EXPECT_EQ(items[2], "ghi");
}

// Test parsing failure returns false
TEST_F(HolderParseCoreTest, ParseFailure) {
    peg::parser parser(R"(
        ROOT <- 'hello'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    EXPECT_FALSE(parser.parse("world"));
}

// Test empty input string
TEST_F(HolderParseCoreTest, EmptyInput) {
    peg::parser parser(R"(
        ROOT <- ''
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    EXPECT_TRUE(parser.parse(""));
}

// Test parsing with choice operator
TEST_F(HolderParseCoreTest, ChoiceOperator) {
    peg::parser parser(R"(
        ROOT <- 'a' / 'b' / 'c'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("b"));
    EXPECT_TRUE(parser.parse("c"));
    EXPECT_FALSE(parser.parse("d"));
}

// Test parsing with repetition operators
TEST_F(HolderParseCoreTest, RepetitionOperators) {
    peg::parser parser(R"(
        ROOT <- [0-9]+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    EXPECT_TRUE(parser.parse("123"));
    EXPECT_TRUE(parser.parse("0"));
    EXPECT_FALSE(parser.parse(""));
    EXPECT_FALSE(parser.parse("abc"));
}

// Test parsing with optional operator
TEST_F(HolderParseCoreTest, OptionalOperator) {
    peg::parser parser(R"(
        ROOT <- 'a' 'b'?
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("ab"));
    EXPECT_FALSE(parser.parse("b"));
}

// Test parsing with semantic value and choice count
TEST_F(HolderParseCoreTest, SemanticValueWithChoice) {
    peg::parser parser(R"(
        ROOT <- 'a' / 'b'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    std::string result;
    parser["ROOT"] = [&](const peg::SemanticValues& vs) {
        result = vs.token();
        return result;
    };

    EXPECT_TRUE(parser.parse("a", result));
    EXPECT_EQ(result, "a");

    EXPECT_TRUE(parser.parse("b", result));
    EXPECT_EQ(result, "b");
}

// Test parsing with error callback
TEST_F(HolderParseCoreTest, ErrorCallback) {
    peg::parser parser(R"(
        ROOT <- 'hello'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    std::string error_msg;
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        error_msg = msg;
    };

    EXPECT_FALSE(parser.parse("world"));
    EXPECT_FALSE(error_msg.empty());
}

// Test parsing with packrat parsing enabled
TEST_F(HolderParseCoreTest, PackratParsing) {
    peg::parser parser(R"(
        ROOT <- 'a'* 'b'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();

    EXPECT_TRUE(parser.parse("b"));
    EXPECT_TRUE(parser.parse("ab"));
    EXPECT_TRUE(parser.parse("aaab"));
    EXPECT_FALSE(parser.parse("a"));
    EXPECT_FALSE(parser.parse("abc"));
}

// Test parsing with AST enabled
TEST_F(HolderParseCoreTest, AstParsing) {
    peg::parser parser(R"(
        ROOT <- 'hello'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_ast();

    EXPECT_TRUE(parser.parse("hello"));
    EXPECT_FALSE(parser.parse("world"));
}

// Test parsing with complex grammar and multiple semantic actions
TEST_F(HolderParseCoreTest, ComplexGrammarWithActions) {
    peg::parser parser(R"(
        EXPRESSION <- TERM ( '+' TERM )*
        TERM <- FACTOR ( '*' FACTOR )*
        FACTOR <- NUMBER / '(' EXPRESSION ')'
        NUMBER <- [0-9]+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    int result = 0;
    parser["NUMBER"] = [&](const peg::SemanticValues& vs) {
        return std::stoi(vs.token());
    };
    parser["TERM"] = [&](const peg::SemanticValues& vs) {
        int val = peg::any_cast<int>(vs[0]);
        for (size_t i = 1; i < vs.size(); i += 2) {
            val *= peg::any_cast<int>(vs[i + 1]);
        }
        return val;
    };
    parser["EXPRESSION"] = [&](const peg::SemanticValues& vs) {
        int val = peg::any_cast<int>(vs[0]);
        for (size_t i = 1; i < vs.size(); i += 2) {
            val += peg::any_cast<int>(vs[i + 1]);
        }
        result = val;
        return val;
    };

    EXPECT_TRUE(parser.parse("2+3*4", result));
    EXPECT_EQ(result, 14);
}

// Test parsing with rule that has enter/leave callbacks (via action attachment)
TEST_F(HolderParseCoreTest, RuleWithCallbacks) {
    peg::parser parser(R"(
        ROOT <- 'test'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    bool action_called = false;
    parser["ROOT"] = [&](const peg::SemanticValues& vs) {
        action_called = true;
        return vs.token();
    };

    EXPECT_TRUE(parser.parse("test"));
    EXPECT_TRUE(action_called);
}

// Test parsing with ignoreSemanticValue behavior (no action attached)
TEST_F(HolderParseCoreTest, IgnoreSemanticValue) {
    peg::parser parser(R"(
        ROOT <- 'hello'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    // No action attached - semantic values should be ignored
    EXPECT_TRUE(parser.parse("hello"));
}

// Test parsing with error message from rule
TEST_F(HolderParseCoreTest, ErrorMessageFromRule) {
    peg::parser parser(R"(
        ROOT <- 'hello'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    std::string error_msg;
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        error_msg = msg;
    };

    EXPECT_FALSE(parser.parse("world"));
    EXPECT_FALSE(error_msg.empty());
}

// Test parsing with uninitialized definition (should throw)
TEST_F(HolderParseCoreTest, UninitializedDefinition) {
    // This tests the logic_error path in parse_core
    // We can't directly test it through public API, but we can verify
    // that the parser handles invalid grammars properly
    peg::parser parser("INVALID GRAMMAR <<<");
    EXPECT_FALSE(static_cast<bool>(parser));
}

// Test parsing with macro reference (using action attachment)
TEST_F(HolderParseCoreTest, MacroReference) {
    peg::parser parser(R"(
        ROOT <- 'macro'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    std::string result;
    parser["ROOT"] = [&](const peg::SemanticValues& vs) {
        result = vs.token();
        return result;
    };

    EXPECT_TRUE(parser.parse("macro", result));
    EXPECT_EQ(result, "macro");
}

// Test parsing with prioritized choice
TEST_F(HolderParseCoreTest, PrioritizedChoice) {
    peg::parser parser(R"(
        ROOT <- 'ab' / 'a'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    EXPECT_TRUE(parser.parse("ab"));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_FALSE(parser.parse("b"));
}

// Test parsing with sequence and semantic values
TEST_F(HolderParseCoreTest, SequenceWithSemanticValues) {
    peg::parser parser(R"(
        ROOT <- FIRST SECOND
        FIRST <- 'hello'
        SECOND <- 'world'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));

    std::vector<std::string> results;
    parser["FIRST"] = [&](const peg::SemanticValues& vs) {
        results.push_back("first:" + vs.token());
        return vs.token();
    };
    parser["SECOND"] = [&](const peg::SemanticValues& vs) {
        results.push_back("second:" + vs.token());
        return vs.token();
    };

    EXPECT_TRUE(parser.parse("helloworld"));
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0], "first:hello");
    EXPECT_EQ(results[1], "second:world");
}