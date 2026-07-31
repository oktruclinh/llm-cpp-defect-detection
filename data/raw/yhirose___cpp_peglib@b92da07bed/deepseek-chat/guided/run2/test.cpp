#include <gtest/gtest.h>
#include "peglib.h"

class ContextTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple grammar that uses packrat parsing
        grammar = std::make_shared<peglib::peg::parser>();
        
        // Define a simple grammar with a rule that can be cached
        bool ret = grammar->load_grammar(
            "start <- 'a' 'b' 'c'\n"
        );
        ASSERT_TRUE(ret);
        
        // Enable packrat parsing
        grammar->enable_packrat_parsing();
    }

    std::shared_ptr<peglib::peg::parser> grammar;
};

// Test basic parsing with packrat enabled
TEST_F(ContextTest, BasicPackratParsing) {
    std::string input = "abc";
    std::shared_ptr<peglib::peg::Ast> ast;
    bool ret = grammar->parse(input, ast);
    EXPECT_TRUE(ret);
    EXPECT_NE(ast, nullptr);
}

// Test parsing failure with packrat enabled
TEST_F(ContextTest, PackratParsingFailure) {
    std::string input = "abd";
    std::shared_ptr<peglib::peg::Ast> ast;
    bool ret = grammar->parse(input, ast);
    EXPECT_FALSE(ret);
    EXPECT_EQ(ast, nullptr);
}

// Test empty input with packrat enabled
TEST_F(ContextTest, EmptyInputPackrat) {
    std::string input = "";
    std::shared_ptr<peglib::peg::Ast> ast;
    bool ret = grammar->parse(input, ast);
    EXPECT_FALSE(ret);
    EXPECT_EQ(ast, nullptr);
}

// Test that packrat caching works correctly with repeated parsing
TEST_F(ContextTest, PackratCacheReuse) {
    std::string input = "abc";
    std::shared_ptr<peglib::peg::Ast> ast1, ast2;
    
    // First parse should populate cache
    bool ret1 = grammar->parse(input, ast1);
    EXPECT_TRUE(ret1);
    
    // Second parse should use cache
    bool ret2 = grammar->parse(input, ast2);
    EXPECT_TRUE(ret2);
    
    // Both parses should produce equivalent ASTs
    EXPECT_EQ(ast1->nodes.size(), ast2->nodes.size());
}

// Test packrat with longer input to exercise multiple cache entries
TEST_F(ContextTest, PackratMultipleCacheEntries) {
    std::shared_ptr<peglib::peg::parser> complex_grammar = std::make_shared<peglib::peg::parser>();
    bool ret = complex_grammar->load_grammar(
        "start <- 'a' 'b' 'c' 'd' 'e'\n"
    );
    ASSERT_TRUE(ret);
    complex_grammar->enable_packrat_parsing();
    
    std::string input = "abcde";
    std::shared_ptr<peglib::peg::Ast> ast;
    ret = complex_grammar->parse(input, ast);
    EXPECT_TRUE(ret);
}

// Test that packrat parsing handles backtracking correctly
TEST_F(ContextTest, PackratBacktracking) {
    std::shared_ptr<peglib::peg::parser> backtrack_grammar = std::make_shared<peglib::peg::parser>();
    bool ret = backtrack_grammar->load_grammar(
        "start <- 'a' ('b' / 'c') 'd'\n"
    );
    ASSERT_TRUE(ret);
    backtrack_grammar->enable_packrat_parsing();
    
    // Test both alternatives
    std::string input1 = "abd";
    std::shared_ptr<peglib::peg::Ast> ast1;
    ret = backtrack_grammar->parse(input1, ast1);
    EXPECT_TRUE(ret);
    
    std::string input2 = "acd";
    std::shared_ptr<peglib::peg::Ast> ast2;
    ret = backtrack_grammar->parse(input2, ast2);
    EXPECT_TRUE(ret);
}

// Test that packrat parsing works with whitespace skipping
TEST_F(ContextTest, PackratWithWhitespace) {
    std::shared_ptr<peglib::peg::parser> ws_grammar = std::make_shared<peglib::peg::parser>();
    bool ret = ws_grammar->load_grammar(
        "start <- 'a' 'b' 'c'\n"
    );
    ASSERT_TRUE(ret);
    ws_grammar->enable_packrat_parsing();
    ws_grammar->enable_ast();
    
    std::string input = "a b c";
    std::shared_ptr<peglib::peg::Ast> ast;
    ret = ws_grammar->parse(input, ast);
    EXPECT_TRUE(ret);
}

// Test that packrat parsing handles errors at different positions
TEST_F(ContextTest, PackratErrorPosition) {
    std::shared_ptr<peglib::peg::parser> error_grammar = std::make_shared<peglib::peg::parser>();
    bool ret = error_grammar->load_grammar(
        "start <- 'a' 'b' 'c'\n"
    );
    ASSERT_TRUE(ret);
    error_grammar->enable_packrat_parsing();
    
    // Error at different positions
    std::string input1 = "axc";
    std::shared_ptr<peglib::peg::Ast> ast1;
    ret = error_grammar->parse(input1, ast1);
    EXPECT_FALSE(ret);
    
    std::string input2 = "abx";
    std::shared_ptr<peglib::peg::Ast> ast2;
    ret = error_grammar->parse(input2, ast2);
    EXPECT_FALSE(ret);
}

// Test that packrat parsing works with multiple rules
TEST_F(ContextTest, PackratMultipleRules) {
    std::shared_ptr<peglib::peg::parser> multi_grammar = std::make_shared<peglib::peg::parser>();
    bool ret = multi_grammar->load_grammar(
        "start <- rule1 rule2\n"
        "rule1 <- 'a' 'b'\n"
        "rule2 <- 'c' 'd'\n"
    );
    ASSERT_TRUE(ret);
    multi_grammar->enable_packrat_parsing();
    
    std::string input = "abcd";
    std::shared_ptr<peglib::peg::Ast> ast;
    ret = multi_grammar->parse(input, ast);
    EXPECT_TRUE(ret);
}

// Test that packrat parsing handles recursive rules
TEST_F(ContextTest, PackratRecursiveRules) {
    std::shared_ptr<peglib::peg::parser> rec_grammar = std::make_shared<peglib::peg::parser>();
    bool ret = rec_grammar->load_grammar(
        "start <- 'a' start? 'b'\n"
    );
    ASSERT_TRUE(ret);
    rec_grammar->enable_packrat_parsing();
    
    std::string input = "aaabbb";
    std::shared_ptr<peglib::peg::Ast> ast;
    ret = rec_grammar->parse(input, ast);
    EXPECT_TRUE(ret);
}

// Test that packrat parsing correctly handles partial matches
TEST_F(ContextTest, PackratPartialMatch) {
    std::shared_ptr<peglib::peg::parser> partial_grammar = std::make_shared<peglib::peg::parser>();
    bool ret = partial_grammar->load_grammar(
        "start <- 'a' 'b' 'c'\n"
    );
    ASSERT_TRUE(ret);
    partial_grammar->enable_packrat_parsing();
    
    // Input longer than grammar
    std::string input = "abcxyz";
    std::shared_ptr<peglib::peg::Ast> ast;
    ret = partial_grammar->parse(input, ast);
    EXPECT_TRUE(ret);  // Should match "abc" and ignore rest
}

// Test that packrat parsing works with character classes
TEST_F(ContextTest, PackratCharacterClass) {
    std::shared_ptr<peglib::peg::parser> class_grammar = std::make_shared<peglib::peg::parser>();
    bool ret = class_grammar->load_grammar(
        "start <- [a-z]+\n"
    );
    ASSERT_TRUE(ret);
    class_grammar->enable_packrat_parsing();
    
    std::string input = "hello";
    std::shared_ptr<peglib::peg::Ast> ast;
    ret = class_grammar->parse(input, ast);
    EXPECT_TRUE(ret);
}

// Test that packrat parsing works with repetition operators
TEST_F(ContextTest, PackratRepetition) {
    std::shared_ptr<peglib::peg::parser> rep_grammar = std::make_shared<peglib::peg::parser>();
    bool ret = rep_grammar->load_grammar(
        "start <- 'a'* 'b'+ 'c'?\n"
    );
    ASSERT_TRUE(ret);
    rep_grammar->enable_packrat_parsing();
    
    std::string input = "aaabc";
    std::shared_ptr<peglib::peg::Ast> ast;
    ret = rep_grammar->parse(input, ast);
    EXPECT_TRUE(ret);
}