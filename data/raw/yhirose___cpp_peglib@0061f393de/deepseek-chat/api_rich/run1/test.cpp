#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>
#include <utility>

class PerformCoreTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Test basic grammar parsing with a simple rule
TEST_F(PerformCoreTest, BasicGrammar) {
    peg::parser parser(R"(
        ROOT <- 'hello'
    )");
    
    EXPECT_TRUE(static_cast<bool>(parser));
    EXPECT_TRUE(parser.parse("hello"));
    EXPECT_FALSE(parser.parse("world"));
}

// Test grammar with user-defined rules via operator[]
TEST_F(PerformCoreTest, UserDefinedRules) {
    peg::parser parser(R"(
        ROOT <- NUMBER
        NUMBER <- [0-9]+
    )");
    
    EXPECT_TRUE(static_cast<bool>(parser));
    EXPECT_TRUE(parser.parse("123"));
    EXPECT_FALSE(parser.parse("abc"));
}

// Test grammar with semantic actions
TEST_F(PerformCoreTest, SemanticActions) {
    peg::parser parser(R"(
        ROOT <- NUMBER
        NUMBER <- [0-9]+
    )");
    
    parser["NUMBER"] = [](const peg::SemanticValues& vs) {
        return std::stoi(vs.token());
    };
    
    EXPECT_TRUE(static_cast<bool>(parser));
    
    int result = 0;
    EXPECT_TRUE(parser.parse("42", result));
    EXPECT_EQ(42, result);
}

// Test grammar with ignore operator (~)
TEST_F(PerformCoreTest, IgnoreOperator) {
    peg::parser parser(R"(
        ROOT <- ~' '? 'hello'
    )");
    
    EXPECT_TRUE(static_cast<bool>(parser));
    EXPECT_TRUE(parser.parse("hello"));
    EXPECT_TRUE(parser.parse(" hello"));
}

// Test error logging callback
TEST_F(PerformCoreTest, ErrorLogging) {
    std::vector<std::pair<size_t, std::string>> errors;
    
    peg::parser parser(R"(
        ROOT <- 'hello'
    )");
    
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        errors.push_back({line, msg});
    };
    
    EXPECT_TRUE(static_cast<bool>(parser));
    EXPECT_FALSE(parser.parse("world"));
    EXPECT_FALSE(errors.empty());
}

// Test grammar with left recursion detection
TEST_F(PerformCoreTest, LeftRecursionDetection) {
    std::vector<std::string> errors;
    
    peg::parser parser(R"(
        ROOT <- ROOT 'a'
    )");
    
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        errors.push_back(msg);
    };
    
    EXPECT_FALSE(static_cast<bool>(parser));
    EXPECT_FALSE(errors.empty());
    bool foundLeftRecursive = false;
    for (const auto& err : errors) {
        if (err.find("left recursive") != std::string::npos) {
            foundLeftRecursive = true;
            break;
        }
    }
    EXPECT_TRUE(foundLeftRecursive);
}

// Test grammar with duplicate rule definitions
TEST_F(PerformCoreTest, DuplicateDefinitions) {
    std::vector<std::string> errors;
    
    peg::parser parser(R"(
        ROOT <- 'a'
        ROOT <- 'b'
    )");
    
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        errors.push_back(msg);
    };
    
    EXPECT_FALSE(static_cast<bool>(parser));
    EXPECT_FALSE(errors.empty());
    bool foundDuplicate = false;
    for (const auto& err : errors) {
        if (err.find("already defined") != std::string::npos) {
            foundDuplicate = true;
            break;
        }
    }
    EXPECT_TRUE(foundDuplicate);
}

// Test grammar with missing rule references
TEST_F(PerformCoreTest, MissingRuleReferences) {
    std::vector<std::string> errors;
    
    peg::parser parser(R"(
        ROOT <- UNDEFINED_RULE
    )");
    
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        errors.push_back(msg);
    };
    
    EXPECT_FALSE(static_cast<bool>(parser));
    EXPECT_FALSE(errors.empty());
}

// Test grammar with precedence instruction
TEST_F(PerformCoreTest, PrecedenceInstruction) {
    peg::parser parser(R"(
        ROOT <- EXPRESSION
        EXPRESSION <- ATOM ( '+' ATOM )*
        ATOM <- [0-9]+
        %precedence EXPRESSION left '+' 1
    )");
    
    EXPECT_TRUE(static_cast<bool>(parser));
    EXPECT_TRUE(parser.parse("1+2+3"));
}

// Test grammar with whitespace skipping
TEST_F(PerformCoreTest, WhitespaceSkipping) {
    peg::parser parser(R"(
        ROOT <- 'hello'
        %whitespace <- [ \t]*
    )");
    
    EXPECT_TRUE(static_cast<bool>(parser));
    EXPECT_TRUE(parser.parse("hello"));
    EXPECT_TRUE(parser.parse("  hello  "));
}

// Test grammar with word expression
TEST_F(PerformCoreTest, WordExpression) {
    peg::parser parser(R"(
        ROOT <- WORD
        %word <- [a-zA-Z]+
    )");
    
    EXPECT_TRUE(static_cast<bool>(parser));
    EXPECT_TRUE(parser.parse("hello"));
    EXPECT_FALSE(parser.parse("123"));
}

// Test grammar with ignore operator on start rule (should fail)
TEST_F(PerformCoreTest, IgnoreOperatorOnStartRule) {
    std::vector<std::string> errors;
    
    peg::parser parser(R"(
        ~ROOT <- 'hello'
    )");
    
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        errors.push_back(msg);
    };
    
    EXPECT_FALSE(static_cast<bool>(parser));
    EXPECT_FALSE(errors.empty());
    bool foundIgnoreError = false;
    for (const auto& err : errors) {
        if (err.find("Ignore operator") != std::string::npos) {
            foundIgnoreError = true;
            break;
        }
    }
    EXPECT_TRUE(foundIgnoreError);
}

// Test grammar with infinite loop detection
TEST_F(PerformCoreTest, InfiniteLoopDetection) {
    std::vector<std::string> errors;
    
    peg::parser parser(R"(
        ROOT <- ''
    )");
    
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        errors.push_back(msg);
    };
    
    EXPECT_FALSE(static_cast<bool>(parser));
    EXPECT_FALSE(errors.empty());
    bool foundInfiniteLoop = false;
    for (const auto& err : errors) {
        if (err.find("infinite loop") != std::string::npos) {
            foundInfiniteLoop = true;
            break;
        }
    }
    EXPECT_TRUE(foundInfiniteLoop);
}

// Test grammar with AST enabled
TEST_F(PerformCoreTest, ASTEnabled) {
    peg::parser parser(R"(
        ROOT <- 'hello'
    )");
    
    parser.enable_ast();
    
    EXPECT_TRUE(static_cast<bool>(parser));
    
    std::shared_ptr<peg::Ast> ast;
    EXPECT_TRUE(parser.parse("hello", ast));
    EXPECT_NE(nullptr, ast);
    EXPECT_EQ("ROOT", ast->name);
}

// Test grammar with packrat parsing enabled
TEST_F(PerformCoreTest, PackratParsing) {
    peg::parser parser(R"(
        ROOT <- 'hello'
    )");
    
    parser.enable_packrat_parsing();
    
    EXPECT_TRUE(static_cast<bool>(parser));
    EXPECT_TRUE(parser.parse("hello"));
}

// Test grammar with complex nested rules
TEST_F(PerformCoreTest, ComplexNestedRules) {
    peg::parser parser(R"(
        ROOT <- EXPR
        EXPR <- TERM ( '+' TERM )*
        TERM <- FACTOR ( '*' FACTOR )*
        FACTOR <- NUMBER / '(' EXPR ')'
        NUMBER <- [0-9]+
    )");
    
    EXPECT_TRUE(static_cast<bool>(parser));
    EXPECT_TRUE(parser.parse("1+2*3"));
    EXPECT_TRUE(parser.parse("(1+2)*3"));
    EXPECT_FALSE(parser.parse("1+"));
}

// Test grammar with empty input
TEST_F(PerformCoreTest, EmptyGrammar) {
    std::vector<std::string> errors;
    
    peg::parser parser("");
    
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        errors.push_back(msg);
    };
    
    EXPECT_FALSE(static_cast<bool>(parser));
    EXPECT_FALSE(errors.empty());
}

// Test grammar with only whitespace
TEST_F(PerformCoreTest, WhitespaceOnlyGrammar) {
    std::vector<std::string> errors;
    
    peg::parser parser("   ");
    
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        errors.push_back(msg);
    };
    
    EXPECT_FALSE(static_cast<bool>(parser));
    EXPECT_FALSE(errors.empty());
}

// Test grammar with multiple error types
TEST_F(PerformCoreTest, MultipleErrors) {
    std::vector<std::string> errors;
    
    peg::parser parser(R"(
        ROOT <- UNDEFINED
        ROOT <- 'duplicate'
    )");
    
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        errors.push_back(msg);
    };
    
    EXPECT_FALSE(static_cast<bool>(parser));
    EXPECT_GE(errors.size(), 2); // Should have both duplicate and missing definition errors
}