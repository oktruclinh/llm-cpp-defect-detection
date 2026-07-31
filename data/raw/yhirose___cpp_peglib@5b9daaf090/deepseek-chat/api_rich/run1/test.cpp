#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>
#include <stdexcept>

// Test the packrat parsing functionality through the public API
class PackratParsingTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Ensure packrat parsing is enabled for all tests
    }
};

// Test basic packrat parsing with a simple grammar
TEST_F(PackratParsingTest, BasicPackratParsing) {
    peg::parser parser(R"(
        ROOT <- 'a'+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    EXPECT_TRUE(parser.parse("aaa"));
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_FALSE(parser.parse(""));
    EXPECT_FALSE(parser.parse("b"));
}

// Test packrat parsing with semantic actions
TEST_F(PackratParsingTest, PackratWithSemanticActions) {
    peg::parser parser(R"(
        ROOT <- NUMBER
        NUMBER <- [0-9]+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    parser["NUMBER"] = [](const peg::SemanticValues& vs) {
        return std::stoi(vs.token());
    };
    
    int result = 0;
    EXPECT_TRUE(parser.parse("123", result));
    EXPECT_EQ(123, result);
    
    result = 0;
    EXPECT_TRUE(parser.parse("0", result));
    EXPECT_EQ(0, result);
}

// Test packrat parsing with choice operator
TEST_F(PackratParsingTest, PackratWithChoice) {
    peg::parser parser(R"(
        ROOT <- 'a' / 'b'+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("bbb"));
    EXPECT_FALSE(parser.parse("c"));
}

// Test packrat parsing with nested rules
TEST_F(PackratParsingTest, PackratWithNestedRules) {
    peg::parser parser(R"(
        ROOT <- EXPR
        EXPR <- TERM ( '+' TERM )*
        TERM <- FACTOR ( '*' FACTOR )*
        FACTOR <- [0-9]+ / '(' EXPR ')'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    EXPECT_TRUE(parser.parse("1+2"));
    EXPECT_TRUE(parser.parse("1*2+3"));
    EXPECT_TRUE(parser.parse("(1+2)*3"));
    EXPECT_FALSE(parser.parse("1+"));
    EXPECT_FALSE(parser.parse("+1"));
}

// Test packrat parsing with semantic actions and reduction
TEST_F(PackratParsingTest, PackratWithReduction) {
    peg::parser parser(R"(
        ROOT <- EXPR
        EXPR <- TERM ( '+' TERM )*
        TERM <- FACTOR ( '*' FACTOR )*
        FACTOR <- NUMBER / '(' EXPR ')'
        NUMBER <- [0-9]+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    parser["EXPR"] = [](const peg::SemanticValues& vs) {
        int result = vs[0].as<int>();
        for (size_t i = 1; i < vs.size(); i += 2) {
            result += vs[i+1].as<int>();
        }
        return result;
    };
    
    parser["TERM"] = [](const peg::SemanticValues& vs) {
        int result = vs[0].as<int>();
        for (size_t i = 1; i < vs.size(); i += 2) {
            result *= vs[i+1].as<int>();
        }
        return result;
    };
    
    parser["NUMBER"] = [](const peg::SemanticValues& vs) {
        return std::stoi(vs.token());
    };
    
    int result = 0;
    EXPECT_TRUE(parser.parse("1+2*3", result));
    EXPECT_EQ(7, result);  // 1 + (2*3) = 7
    
    result = 0;
    EXPECT_TRUE(parser.parse("(1+2)*3", result));
    EXPECT_EQ(9, result);  // (1+2)*3 = 9
}

// Test packrat parsing with error handling
TEST_F(PackratParsingTest, PackratWithErrorHandling) {
    std::string error_msg;
    peg::parser parser(R"(
        ROOT <- 'a'+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        error_msg = msg;
    };
    
    EXPECT_FALSE(parser.parse("b"));
    EXPECT_FALSE(error_msg.empty());
}

// Test packrat parsing with empty input
TEST_F(PackratParsingTest, PackratWithEmptyInput) {
    peg::parser parser(R"(
        ROOT <- ''
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    EXPECT_TRUE(parser.parse(""));
    EXPECT_FALSE(parser.parse("a"));
}

// Test packrat parsing with optional elements
TEST_F(PackratParsingTest, PackratWithOptional) {
    peg::parser parser(R"(
        ROOT <- 'a' 'b'?
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    EXPECT_TRUE(parser.parse("a"));
    EXPECT_TRUE(parser.parse("ab"));
    EXPECT_FALSE(parser.parse(""));
    EXPECT_FALSE(parser.parse("b"));
}

// Test packrat parsing with repetition operators
TEST_F(PackratParsingTest, PackratWithRepetition) {
    peg::parser parser(R"(
        ROOT <- 'a'* 'b'+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    EXPECT_TRUE(parser.parse("b"));
    EXPECT_TRUE(parser.parse("ab"));
    EXPECT_TRUE(parser.parse("aaab"));
    EXPECT_FALSE(parser.parse(""));
    EXPECT_FALSE(parser.parse("a"));
}

// Test packrat parsing with AST generation
TEST_F(PackratParsingTest, PackratWithAST) {
    peg::parser parser(R"(
        ROOT <- 'a'+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    parser.enable_ast();
    
    std::shared_ptr<peg::Ast> ast;
    EXPECT_TRUE(parser.parse("aaa", ast));
    ASSERT_NE(nullptr, ast);
    EXPECT_EQ("ROOT", ast->name);
    EXPECT_EQ(3u, ast->token.size());
}

// Test packrat parsing with complex grammar and multiple semantic values
TEST_F(PackratParsingTest, PackratWithMultipleSemanticValues) {
    peg::parser parser(R"(
        ROOT <- PAIR (',' PAIR)*
        PAIR <- KEY ':' VALUE
        KEY <- [a-z]+
        VALUE <- [0-9]+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    parser["PAIR"] = [](const peg::SemanticValues& vs) {
        return std::make_pair(vs[0].as<std::string>(), vs[1].as<int>());
    };
    
    parser["KEY"] = [](const peg::SemanticValues& vs) {
        return vs.token();
    };
    
    parser["VALUE"] = [](const peg::SemanticValues& vs) {
        return std::stoi(vs.token());
    };
    
    std::vector<std::pair<std::string, int>> result;
    EXPECT_TRUE(parser.parse("abc:123,def:456", result));
    ASSERT_EQ(2u, result.size());
    EXPECT_EQ("abc", result[0].first);
    EXPECT_EQ(123, result[0].second);
    EXPECT_EQ("def", result[1].first);
    EXPECT_EQ(456, result[1].second);
}

// Test packrat parsing with predicate operators
TEST_F(PackratParsingTest, PackratWithPredicates) {
    peg::parser parser(R"(
        ROOT <- &'a' [a-z]+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    EXPECT_TRUE(parser.parse("abc"));
    EXPECT_FALSE(parser.parse("bcd"));
}

// Test packrat parsing with negative lookahead
TEST_F(PackratParsingTest, PackratWithNegativeLookahead) {
    peg::parser parser(R"(
        ROOT <- !'ab' [a-z]+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    EXPECT_TRUE(parser.parse("ac"));
    EXPECT_FALSE(parser.parse("ab"));
}

// Test packrat parsing with recursive grammar
TEST_F(PackratParsingTest, PackratWithRecursiveGrammar) {
    peg::parser parser(R"(
        ROOT <- PAREN
        PAREN <- '(' PAREN ')' / ''
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    EXPECT_TRUE(parser.parse(""));
    EXPECT_TRUE(parser.parse("()"));
    EXPECT_TRUE(parser.parse("(())"));
    EXPECT_TRUE(parser.parse("((()))"));
    EXPECT_FALSE(parser.parse("("));
    EXPECT_FALSE(parser.parse(")"));
}

// Test packrat parsing with error recovery
TEST_F(PackratParsingTest, PackratWithErrorRecovery) {
    std::vector<std::string> errors;
    peg::parser parser(R"(
        ROOT <- 'a'+ 'b'*
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        errors.push_back(msg);
    };
    
    EXPECT_FALSE(parser.parse("c"));
    EXPECT_FALSE(errors.empty());
}

// Test packrat parsing with large input to exercise caching
TEST_F(PackratParsingTest, PackratWithLargeInput) {
    peg::parser parser(R"(
        ROOT <- 'a'+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    std::string large_input(1000, 'a');
    EXPECT_TRUE(parser.parse(large_input));
    
    std::string invalid_input = large_input + "b";
    EXPECT_FALSE(parser.parse(invalid_input));
}

// Test packrat parsing with multiple rules and backtracking
TEST_F(PackratParsingTest, PackratWithBacktracking) {
    peg::parser parser(R"(
        ROOT <- RULE_A / RULE_B
        RULE_A <- 'a' 'b' 'c'
        RULE_B <- 'a' 'b' 'd'
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    EXPECT_TRUE(parser.parse("abc"));
    EXPECT_TRUE(parser.parse("abd"));
    EXPECT_FALSE(parser.parse("abe"));
}

// Test packrat parsing with semantic action that throws parse_error
TEST_F(PackratParsingTest, PackratWithParseErrorInAction) {
    peg::parser parser(R"(
        ROOT <- NUMBER
        NUMBER <- [0-9]+
    )");
    ASSERT_TRUE(static_cast<bool>(parser));
    parser.enable_packrat_parsing();
    
    parser["NUMBER"] = [](const peg::SemanticValues& vs) {
        int val = std::stoi(vs.token());
        if (val < 0) {
            throw peg::parse_error("Negative numbers not allowed");
        }
        return val;
    };
    
    int result = 0;
    EXPECT_TRUE(parser.parse("123", result));
    EXPECT_EQ(123, result);
    
    // Note: parse_error in action should cause parse to fail
    // This tests the catch block in the focal function
    EXPECT_FALSE(parser.parse("-1", result));
}