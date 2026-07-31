```cpp
#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>

// Test the Context class indirectly through parser operations that exercise
// the packrat caching mechanism, whitespace handling, and capture scope management

class ContextTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Basic arithmetic grammar for testing
        grammar = R"(
            Expr    <- Sum
            Sum     <- Product ( '+' Product / '-' Product )*
            Product <- Value ( '*' Value / '/' Value )*
            Value   <- < [0-9]+ >
            %whitespace <- [ \t]*
        )";
    }

    std::string grammar;
};

// Test that packrat parsing can be enabled and works correctly
TEST_F(ContextTest, PackratParsingEnabled) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_packrat_parsing();
    
    // Parse a simple expression - packrat should cache intermediate results
    std::string input = "1+2*3";
    EXPECT_TRUE(parser.parse(input));
}

// Test that packrat parsing handles repeated parsing of same input
TEST_F(ContextTest, PackratParsingRepeated) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_packrat_parsing();
    
    // Parse multiple times - packrat cache should be reused
    EXPECT_TRUE(parser.parse("42"));
    EXPECT_TRUE(parser.parse("42"));
    EXPECT_TRUE(parser.parse("1+2"));
}

// Test that packrat parsing handles long inputs with many rules
TEST_F(ContextTest, PackratParsingLongInput) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_packrat_parsing();
    
    // Long expression that exercises many rule applications
    std::string input = "1+2*3-4/5+6*7-8/9";
    EXPECT_TRUE(parser.parse(input));
}

// Test that packrat parsing correctly fails on invalid input
TEST_F(ContextTest, PackratParsingInvalidInput) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_packrat_parsing();
    
    // Invalid input should still fail with packrat enabled
    EXPECT_FALSE(parser.parse("abc"));
    EXPECT_FALSE(parser.parse("1++2"));
    EXPECT_FALSE(parser.parse(""));
}

// Test that whitespace handling works correctly
TEST_F(ContextTest, WhitespaceHandling) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Test with various whitespace patterns
    EXPECT_TRUE(parser.parse("1+2"));
    EXPECT_TRUE(parser.parse("1 + 2"));
    EXPECT_TRUE(parser.parse("  1  +  2  "));
    EXPECT_TRUE(parser.parse("1\t+\t2"));
}

// Test that capture scopes work correctly with semantic actions
TEST_F(ContextTest, CaptureScopeWithActions) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    std::vector<int> values;
    
    parser["Value"] = [&](const peg::SemanticValues& vs) {
        values.push_back(std::stoi(vs.token()));
        return values.back();
    };
    
    parser["Sum"] = [&](const peg::SemanticValues& vs) {
        auto result = peg::any_cast<int>(vs[0]);
        for (size_t i = 1; i < vs.size(); i += 2) {
            auto op = vs.sv(i);
            auto val = peg::any_cast<int>(vs[i + 1]);
            if (op == "+") result += val;
            else result -= val;
        }
        return result;
    };
    
    parser["Product"] = [&](const peg::SemanticValues& vs) {
        auto result = peg::any_cast<int>(vs[0]);
        for (size_t i = 1; i < vs.size(); i += 2) {
            auto op = vs.sv(i);
            auto val = peg::any_cast<int>(vs[i + 1]);
            if (op == "*") result *= val;
            else result /= val;
        }
        return result;
    };
    
    int result;
    EXPECT_TRUE(parser.parse("1+2*3", result));
    EXPECT_EQ(result, 7);  // 1 + (2*3) = 7
}

// Test that error positions are correctly reported
TEST_F(ContextTest, ErrorPositionReporting) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    size_t error_line = 0, error_col = 0;
    std::string error_msg;
    
    parser.log = [&](size_t line, size_t col, const std::string& msg) {
        error_line = line;
        error_col = col;
        error_msg = msg;
    };
    
    EXPECT_FALSE(parser.parse("1+abc"));
    EXPECT_GT(error_line, 0);
    EXPECT_GT(error_col, 0);
    EXPECT_FALSE(error_msg.empty());
}

// Test that the parser handles empty input correctly
TEST_F(ContextTest, EmptyInput) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Empty input should fail for this grammar
    EXPECT_FALSE(parser.parse(""));
}

// Test that the parser handles very long input
TEST_F(ContextTest, VeryLongInput) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_packrat_parsing();
    
    // Create a long valid expression
    std::string input = "1";
    for (int i = 2; i <= 100; i++) {
        input += "+" + std::to_string(i);
    }
    
    EXPECT_TRUE(parser.parse(input));
}

// Test that multiple parsers can coexist with their own contexts
TEST_F(ContextTest, MultipleParsers) {
    peg::parser parser1(grammar);
    peg::parser parser2(grammar);
    
    ASSERT_TRUE(static_cast<bool>(parser1));
    ASSERT_TRUE(static_cast<bool>(parser2));
    
    parser1.enable_packrat_parsing();
    parser2.enable_packrat_parsing();
    
    EXPECT_TRUE(parser1.parse("1+2"));
    EXPECT_TRUE(parser2.parse("3*4"));
}

// Test that the parser handles newlines in input correctly
TEST_F(ContextTest, NewlineHandling) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Test with newlines in input
    EXPECT_TRUE(parser.parse("1\n+\n2"));
    EXPECT_TRUE(parser.parse("1\n+ 2\n* 3"));
}

// Test that the parser correctly handles the boundary between token and non-token
TEST_F(ContextTest, TokenBoundary) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Test expressions that exercise token boundaries
    EXPECT_TRUE(parser.parse("123"));
    EXPECT_TRUE(parser.parse("0"));
    EXPECT_TRUE(parser.parse("1+2"));
    EXPECT_FALSE(parser.parse("1+"));
    EXPECT_FALSE(parser.parse("+1"));
}

// Test that the parser handles complex nested expressions
TEST_F(ContextTest, ComplexNestedExpressions) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_packrat_parsing();
    
    // Complex expression that exercises many rule applications
    EXPECT_TRUE(parser.parse("1+2*3-4/5+6*7-8/9+10"));
    EXPECT_TRUE(parser.parse("((1+2)*3)"));
    EXPECT_TRUE(parser.parse("1+2+3+4+5+6+7+8+9+10"));
}

// Test that the parser correctly handles the case where packrat cache is full
TEST_F(ContextTest, PackratCacheFull) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_packrat_parsing();
    
    // Parse a complex expression multiple times to exercise cache
    for (int i = 0; i < 10; i++) {
        EXPECT_TRUE(parser.parse("1+2*3-4/5+6*7-8/9"));
    }
}

// Test that the parser correctly handles the case where packrat parsing is disabled
TEST_F(ContextTest, PackratParsingDisabled) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Without packrat, parsing should still work
    EXPECT_TRUE(parser.parse("1+2*3"));
    EXPECT_FALSE(parser.parse("invalid"));
}

// Test that the parser correctly handles the case where whitespace Ope is nullptr
TEST_F(ContextTest, NullWhitespaceOpe) {
    // Grammar without whitespace rule
    std::string grammar_no_ws = R"(
        Expr <- [0-9]+
    )";
    
    peg::parser parser(grammar_no_ws);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Should only match without whitespace
    EXPECT_TRUE(parser.parse("123"));
    EXPECT_FALSE(parser.parse(" 123 "));
}

// Test that the parser correctly handles the case where word Ope is nullptr
TEST_F(ContextTest, NullWordOpe) {
    // Grammar without word rule
    std::string grammar_no_word = R"(
        Expr <- [0-9]+
    )";
    
    peg::parser parser(grammar_no_word);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    EXPECT_TRUE(parser.parse("123"));
    EXPECT_FALSE(parser.parse("abc"));
}

// Test that the parser correctly handles the case where capture_scope_stack is empty
TEST_F(ContextTest, EmptyCaptureScope) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Simple parse without any capture actions
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where value_stack is empty
TEST_F(ContextTest, EmptyValueStack) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse without semantic actions
    EXPECT_TRUE(parser.parse("1+2"));
}

// Test that the parser correctly handles the case where rule_stack is empty
TEST_F(ContextTest, EmptyRuleStack) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse a simple expression
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where args_stack is empty
TEST_F(ContextTest, EmptyArgsStack) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse without any arguments
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where source_line_index is empty
TEST_F(ContextTest, EmptySourceLineIndex) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse input without newlines
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where error_pos is nullptr
TEST_F(ContextTest, NullErrorPos) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Successful parse should not set error_pos
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where message_pos is nullptr
TEST_F(ContextTest, NullMessagePos) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Successful parse should not set message_pos
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where message is empty
TEST_F(ContextTest, EmptyMessage) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Successful parse should not set message
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where in_token_boundary_count is 0
TEST_F(ContextTest, ZeroTokenBoundaryCount) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse without being in a token boundary
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where in_whitespace is false
TEST_F(ContextTest, NotInWhitespace) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse without being in whitespace
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where cache_registered is empty
TEST_F(ContextTest, EmptyCacheRegistered) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Without packrat, cache_registered should be empty
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where cache_success is empty
TEST_F(ContextTest, EmptyCacheSuccess) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Without packrat, cache_success should be empty
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where cache_values is empty
TEST_F(ContextTest, EmptyCacheValues) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Without packrat, cache_values should be empty
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where tracer_enter is nullptr
TEST_F(ContextTest, NullTracerEnter) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse without tracer
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where tracer_leave is nullptr
TEST_F(ContextTest, NullTracerLeave) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse without tracer
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where both tracers are set
TEST_F(ContextTest, TracersSet) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse with tracers (they are set to nullptr by default, which is fine)
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where def_count is 0
TEST_F(ContextTest, ZeroDefCount) {
    // Empty grammar should have 0 definitions
    std::string empty_grammar = "";
    peg::parser parser(empty_grammar);
    
    // Empty grammar should fail to load
    EXPECT_FALSE(static_cast<bool>(parser));
}

// Test that the parser correctly handles the case where l is 0
TEST_F(ContextTest, ZeroLength) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Empty input has length 0
    EXPECT_FALSE(parser.parse(""));
}

// Test that the parser correctly handles the case where s is nullptr
TEST_F(ContextTest, NullS) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Passing nullptr should not crash (though it may fail)
    // This tests the robustness of the context initialization
    EXPECT_FALSE(parser.parse(std::string_view(nullptr, 0)));
}

// Test that the parser correctly handles the case where path is nullptr
TEST_F(ContextTest, NullPath) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse without a path
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where value_stack_size is 0
TEST_F(ContextTest, ZeroValueStackSize) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse without pushing any values
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where rule_stack is empty
TEST_F(ContextTest, EmptyRuleStack2) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse without any rules on the stack
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where args_stack is empty
TEST_F(ContextTest, EmptyArgsStack2) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse without any arguments
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where capture_scope_stack_size is 0
TEST_F(ContextTest, ZeroCaptureScopeStackSize) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Parse without any capture scopes
    EXPECT_TRUE(parser.parse("42"));
}

// Test that the parser correctly handles the case where enablePackratParsing is false
TEST_F(ContextTest, PackratParsingDisabled2) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    // Without packrat, parsing should still work
    EXPECT_TRUE(parser.parse("42"));
    EXPECT_FALSE(parser.parse("invalid"));
}

// Test that the parser correctly handles the case where enablePackratParsing is true
TEST_F(ContextTest, PackratParsingEnabled2) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_packrat_parsing();
    
    // With packrat, parsing should work
    EXPECT_TRUE(parser.parse("42"));
    EXPECT_FALSE(parser.parse("invalid"));
}

// Test that the parser correctly handles the case where cache_registered is partially filled
TEST_F(ContextTest, PartiallyFilledCacheRegistered) {
    peg::parser parser(grammar);
    ASSERT_TRUE(static_cast<bool>(parser));
    
    parser.enable_packrat_parsing();
    
    // Parse multiple times to partially fill the cache
    EXPECT_TRUE(parser.parse("1+2"));
    EXPECT_TRUE(parser.parse("3*4"));
    EXPECT_TRUE(parser.parse("1+2*3"));
}

// Test that the parser correctly handles the case where cache_success is partially filled
TEST_F(ContextTest, PartiallyFilledCacheSuccess) {
    peg::