#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>
#include <stdexcept>

// Test fixture for Holder::parse_core through public API
class HolderParseCoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple grammar that will exercise parse_core
        // Grammar: ROOT <- 'a'+
    }

    // Helper to create a parser and test parsing
    bool test_parse(const std::string& grammar, const std::string& input, 
                    std::vector<std::string>& captures) {
        peg::parser parser(grammar);
        if (!parser) return false;
        
        parser.enable_packrat_parsing(); // Enable packrat for consistency
        
        // Set up capture action
        parser["ROOT"] = [&](const peg::SemanticValues& sv) {
            captures.push_back(sv.str());
            return sv.str();
        };
        
        return parser.parse(input);
    }
};

// Test basic successful parse
TEST_F(HolderParseCoreTest, BasicParse) {
    std::vector<std::string> captures;
    EXPECT_TRUE(test_parse("ROOT <- 'a'+", "aaa", captures));
    ASSERT_EQ(1, captures.size());
    EXPECT_EQ("aaa", captures[0]);
}

// Test empty input fails
TEST_F(HolderParseCoreTest, EmptyInput) {
    std::vector<std::string> captures;
    EXPECT_FALSE(test_parse("ROOT <- 'a'+", "", captures));
    EXPECT_TRUE(captures.empty());
}

// Test single character match
TEST_F(HolderParseCoreTest, SingleChar) {
    std::vector<std::string> captures;
    EXPECT_TRUE(test_parse("ROOT <- 'a'", "a", captures));
    ASSERT_EQ(1, captures.size());
    EXPECT_EQ("a", captures[0]);
}

// Test no match
TEST_F(HolderParseCoreTest, NoMatch) {
    std::vector<std::string> captures;
    EXPECT_FALSE(test_parse("ROOT <- 'a'+", "bbb", captures));
    EXPECT_TRUE(captures.empty());
}

// Test with ignoreSemanticValue flag
TEST_F(HolderParseCoreTest, IgnoreSemanticValue) {
    peg::parser parser("ROOT <- 'a'+");
    ASSERT_TRUE(parser);
    
    // Enable packrat
    parser.enable_packrat_parsing();
    
    // Set up action that should NOT be called when ignore is true
    bool action_called = false;
    parser["ROOT"] = [&](const peg::SemanticValues& sv) {
        action_called = true;
        return sv.str();
    };
    
    // Parse without ignore - action should be called
    EXPECT_TRUE(parser.parse("aaa"));
    EXPECT_TRUE(action_called);
    
    // Now test with ignore
    action_called = false;
    parser["ROOT"].ignore_semantic_value(true);
    EXPECT_TRUE(parser.parse("aaa"));
    EXPECT_FALSE(action_called); // Action should NOT be called
}

// Test with enter/leave callbacks
TEST_F(HolderParseCoreTest, EnterLeaveCallbacks) {
    peg::parser parser("ROOT <- 'a'+");
    ASSERT_TRUE(parser);
    
    parser.enable_packrat_parsing();
    
    bool enter_called = false;
    bool leave_called = false;
    
    parser["ROOT"].enter = [&](const char* s, size_t n, peg::any& dt) {
        enter_called = true;
    };
    
    parser["ROOT"].leave = [&](const char* s, size_t n, size_t len, 
                                peg::any& val, peg::any& dt) {
        leave_called = true;
    };
    
    EXPECT_TRUE(parser.parse("aaa"));
    EXPECT_TRUE(enter_called);
    EXPECT_TRUE(leave_called);
}

// Test with error_message callback
TEST_F(HolderParseCoreTest, ErrorMessageCallback) {
    peg::parser parser("ROOT <- 'a'+");
    ASSERT_TRUE(parser);
    
    parser.enable_packrat_parsing();
    
    bool error_called = false;
    parser["ROOT"].error_message = [&]() -> std::string {
        error_called = true;
        return "Custom error";
    };
    
    EXPECT_FALSE(parser.parse("bbb"));
    EXPECT_TRUE(error_called);
}

// Test with macro reference (is_macro flag)
TEST_F(HolderParseCoreTest, MacroReference) {
    // Create a grammar with a macro-like rule
    peg::parser parser("ROOT <- MACRO MACRO <- 'a'+");
    ASSERT_TRUE(parser);
    
    parser.enable_packrat_parsing();
    
    // Mark MACRO as a macro
    parser["MACRO"].is_macro(true);
    
    std::vector<std::string> captures;
    parser["ROOT"] = [&](const peg::SemanticValues& sv) {
        captures.push_back(sv.str());
        return sv.str();
    };
    
    EXPECT_TRUE(parser.parse("aaa"));
    ASSERT_EQ(1, captures.size());
    EXPECT_EQ("aaa", captures[0]);
}

// Test with prioritized choice (choice_count/choice tracking)
TEST_F(HolderParseCoreTest, PrioritizedChoice) {
    peg::parser parser("ROOT <- 'a' / 'b'");
    ASSERT_TRUE(parser);
    
    parser.enable_packrat_parsing();
    
    std::vector<std::string> captures;
    parser["ROOT"] = [&](const peg::SemanticValues& sv) {
        captures.push_back(sv.str());
        EXPECT_EQ(0, sv.choice()); // Should be first choice
        return sv.str();
    };
    
    EXPECT_TRUE(parser.parse("a"));
    ASSERT_EQ(1, captures.size());
    EXPECT_EQ("a", captures[0]);
}

// Test with parse_error thrown from action
TEST_F(HolderParseCoreTest, ParseErrorInAction) {
    peg::parser parser("ROOT <- 'a'+");
    ASSERT_TRUE(parser);
    
    parser.enable_packrat_parsing();
    
    parser["ROOT"] = [&](const peg::SemanticValues& sv) -> peg::any {
        throw peg::parse_error("Intentional error");
        return std::string();
    };
    
    // Should fail due to parse_error
    EXPECT_FALSE(parser.parse("aaa"));
}

// Test nested rules (multiple Holder::parse_core calls)
TEST_F(HolderParseCoreTest, NestedRules) {
    peg::parser parser("ROOT <- CHILD CHILD <- 'a'+");
    ASSERT_TRUE(parser);
    
    parser.enable_packrat_parsing();
    
    std::vector<std::string> captures;
    parser["ROOT"] = [&](const peg::SemanticValues& sv) {
        captures.push_back("ROOT:" + sv.str());
        return sv.str();
    };
    parser["CHILD"] = [&](const peg::SemanticValues& sv) {
        captures.push_back("CHILD:" + sv.str());
        return sv.str();
    };
    
    EXPECT_TRUE(parser.parse("aaa"));
    ASSERT_EQ(2, captures.size());
    EXPECT_EQ("CHILD:aaa", captures[0]);
    EXPECT_EQ("ROOT:aaa", captures[1]);
}

// Test with packrat caching (multiple parses of same input)
TEST_F(HolderParseCoreTest, PackratCaching) {
    peg::parser parser("ROOT <- 'a'+");
    ASSERT_TRUE(parser);
    
    parser.enable_packrat_parsing();
    
    // Parse same input multiple times to exercise packrat cache
    for (int i = 0; i < 5; i++) {
        std::vector<std::string> captures;
        EXPECT_TRUE(test_parse("ROOT <- 'a'+", "aaa", captures));
        ASSERT_EQ(1, captures.size());
        EXPECT_EQ("aaa", captures[0]);
    }
}

// Test with zero-length match (empty rule)
TEST_F(HolderParseCoreTest, ZeroLengthMatch) {
    peg::parser parser("ROOT <- ''");
    ASSERT_TRUE(parser);
    
    parser.enable_packrat_parsing();
    
    std::vector<std::string> captures;
    parser["ROOT"] = [&](const peg::SemanticValues& sv) {
        captures.push_back(sv.str());
        return sv.str();
    };
    
    EXPECT_TRUE(parser.parse(""));
    ASSERT_EQ(1, captures.size());
    EXPECT_EQ("", captures[0]);
}

// Test with multiple semantic values
TEST_F(HolderParseCoreTest, MultipleSemanticValues) {
    peg::parser parser("ROOT <- 'a'+ 'b'+");
    ASSERT_TRUE(parser);
    
    parser.enable_packrat_parsing();
    
    std::vector<std::string> captures;
    parser["ROOT"] = [&](const peg::SemanticValues& sv) {
        captures.push_back(sv.str());
        return sv.str();
    };
    
    EXPECT_TRUE(parser.parse("aabbb"));
    ASSERT_EQ(1, captures.size());
    EXPECT_EQ("aabbb", captures[0]);
}