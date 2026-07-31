```cpp
#include <gtest/gtest.h>
#include <peglib.h>

// Helper function to create a simple parser for testing
std::shared_ptr<peg::parser> create_parser(const std::string& grammar) {
    auto parser = std::make_shared<peg::parser>(grammar);
    if (!static_cast<bool>(*parser)) {
        throw std::runtime_error("Failed to parse grammar: " + grammar);
    }
    return parser;
}

TEST(PackratParsingTest, BasicPackratMemoization) {
    // Grammar: A <- 'a' A / 'a'
    // This grammar is left-recursive, but peglib handles it by converting to right-recursive internally.
    // The packrat parser should memoize the 'a' rule.
    auto parser = create_parser("A <- 'a' A / 'a'");

    // We need to enable packrat parsing explicitly
    parser->enable_packrat_parsing();

    // Set up a counter for rule invocations to verify memoization
    int a_rule_invocations = 0;
    parser->set_action("A", [&](const peg::peg_context& c, any& dt) {
        a_rule_invocations++;
        return c.token();
    });

    std::string test_input = "aaa";
    ASSERT_TRUE(parser->parse(test_input));

    // Expect 'A' to be invoked for each 'a' in the input.
    // Without packrat, a naive parser might re-evaluate 'a' multiple times.
    // With packrat, each 'a' should be parsed once and memoized.
    // For "aaa", the 'A' rule will be called for 'a' at index 0, then 'a' at index 1, then 'a' at index 2.
    // The recursive calls will hit the memoized results.
    // The action should be called for each successful match of 'A'.
    // For "aaa", the 'A' rule will be matched 3 times (A -> 'a'A, A -> 'a'A, A -> 'a').
    // So, the action should be called 3 times.
    ASSERT_EQ(a_rule_invocations, 3);
}

TEST(PackratParsingTest, PackratWithAmbiguousGrammar) {
    // Grammar: S <- 'a' 'a' / 'a'
    // Input: "a"
    // This grammar is ambiguous. Packrat parsing should still yield a single result.
    auto parser = create_parser("S <- 'a' 'a' / 'a'");
    parser->enable_packrat_parsing();

    int s_rule_invocations = 0;
    parser->set_action("S", [&](const peg::peg_context& c, any& dt) {
        s_rule_invocations++;
        return c.token();
    });

    std::string test_input = "a";
    ASSERT_TRUE(parser->parse(test_input));

    // The first rule 'a''a' will fail. The second rule 'a' will succeed.
    // The 'S' action should be invoked once for the successful match.
    ASSERT_EQ(s_rule_invocations, 1);
}

TEST(PackratParsingTest, PackratWithFailureAndBacktracking) {
    // Grammar: S <- 'a' 'b' / 'a' 'c'
    // Input: "ac"
    // The parser will try 'a''b', fail, backtrack, then try 'a''c' and succeed.
    // Packrat should memoize the parsing of 'a'.
    auto parser = create_parser("S <- 'a' 'b' / 'a' 'c'");
    parser->enable_packrat_parsing();

    int a_char_invocations = 0;
    parser->set_action("'a'", [&](const peg::peg_context& c, any& dt) {
        a_char_invocations++;
        return c.token();
    });

    std::string test_input = "ac";
    ASSERT_TRUE(parser->parse(test_input));

    // The character 'a' is parsed at the beginning of both alternatives.
    // With packrat, it should be parsed only once and memoized for the second alternative.
    // So, the action for 'a' should be called only once.
    ASSERT_EQ(a_char_invocations, 1);
}

TEST(PackratParsingTest, PackratWithNoMatch) {
    // Grammar: S <- 'a' 'b'
    // Input: "ac"
    // The parser should fail. Packrat should not interfere with failure.
    auto parser = create_parser("S <- 'a' 'b'");
    parser->enable_packrat_parsing();

    std::string test_input = "ac";
    ASSERT_FALSE(parser->parse(test_input));
}

TEST(PackratParsingTest, PackratWithEmptyInput) {
    // Grammar: S <- 'a'
    // Input: ""
    auto parser = create_parser("S <- 'a'");
    parser->enable_packrat_parsing();

    std::string test_input = "";
    ASSERT_FALSE(parser->parse(test_input));

    // Grammar: S <- '' (empty string)
    auto parser_empty_rule = create_parser("S <- ''");
    parser_empty_rule->enable_packrat_parsing();
    ASSERT_TRUE(parser_empty_rule->parse(test_input));
}

TEST(PackratParsingTest, PackratWithSemanticActionError) {
    // Grammar: S <- 'a'
    // Input: "a"
    // The semantic action throws an exception. The parser should catch it and report failure.
    auto parser = create_parser("S <- 'a'");
    parser->enable_packrat_parsing();

    parser->set_action("S", [&](const peg::peg_context& c, any& dt) -> any {
        throw peg::parse_error("Custom error in action");
    });

    std::string test_input = "a";
    std::string error_message;
    parser->set_error_callback([&](size_t line, size_t col, size_t len, const std::string& msg, any& dt) {
        error_message = msg;
    });

    ASSERT_FALSE(parser->parse(test_input));
    ASSERT_EQ(error_message, "Custom error in action");
}

TEST(PackratParsingTest, PackratWithNestedRulesAndActions) {
    // Grammar: S <- A B
    //          A <- 'a'
    //          B <- 'b'
    // Input: "ab"
    auto parser = create_parser("S <- A B\nA <- 'a'\nB <- 'b'");
    parser->enable_packrat_parsing();

    int s_action_count = 0;
    int a_action_count = 0;
    int b_action_count = 0;

    parser->set_action("S", [&](const peg::peg_context& c, any& dt) {
        s_action_count++;
        return c.token();
    });
    parser->set_action("A", [&](const peg::peg_context& c, any& dt) {
        a_action_count++;
        return c.token();
    });
    parser->set_action("B", [&](const peg::peg_context& c, any& dt) {
        b_action_count++;
        return c.token();
    });

    std::string test_input = "ab";
    ASSERT_TRUE(parser->parse(test_input));

    // Each rule's action should be called exactly once for a successful parse.
    ASSERT_EQ(s_action_count, 1);
    ASSERT_EQ(a_action_count, 1);
    ASSERT_EQ(b_action_count, 1);
}

TEST(PackratParsingTest, PackratWithLeftRecursionAndMemoization) {
    // Grammar: Expr <- Expr '+' Term / Term
    //          Term <- '1'
    // Input: "1+1+1"
    // This is a classic left-recursive grammar. peglib handles it by converting to right-recursive.
    // Packrat parsing should ensure that sub-expressions are memoized.
    auto parser = create_parser("Expr <- Expr '+' Term / Term\nTerm <- '1'");
    parser->enable_packrat_parsing();

    int expr_action_count = 0;
    int term_action_count = 0;

    parser->set_action("Expr", [&](const peg::peg_context& c, any& dt) {
        expr_action_count++;
        return c.token();
    });
    parser->set_action("Term", [&](const peg::peg_context& c, any& dt) {
        term_action_count++;
        return c.token();
    });

    std::string test_input = "1+1+1";
    ASSERT_TRUE(parser->parse(test_input));

    // For "1+1+1":
    // Term '1' will be matched 3 times.
    // Expr will be matched for "1", then "1+1", then "1+1+1".
    // The internal mechanism for left-recursion might involve more calls to the rule itself,
    // but the actions should reflect the successful matches of the rule.
    // For "1+1+1", there are 3 'Term' matches and 3 'Expr' matches.
    ASSERT_EQ(term_action_count, 3);
    ASSERT_EQ(expr_action_count, 3);
}

TEST(PackratParsingTest, PackratWithBeforeAndAfterHooks) {
    // Grammar: S <- 'a'
    auto parser = create_parser("S <- 'a'");
    parser->enable_packrat_parsing();

    int before_hook_count = 0;
    int after_hook_count = 0;

    parser->set_before_parse_rule_callback([&](const char* s, size_t n, const peg::Rule& rule, any& dt) {
        if (rule.name == "S") {
            before_hook_count++;
        }
    });
    parser->set_after_parse_rule_callback([&](const char* s, size_t n, const peg::Rule& rule, any& dt) {
        if (rule.name == "S") {
            after_hook_count++;
        }
    });

    std::string test_input = "a";
    ASSERT_TRUE(parser->parse(test_input));

    // The before and after hooks should be called for the 'S' rule.
    // Since 'S' is matched once, the hooks should be called once each.
    ASSERT_EQ(before_hook_count, 1);
    ASSERT_EQ(after_hook_count, 1);
}

TEST(PackratParsingTest, PackratWithBeforeAndAfterHooksOnFailure) {
    // Grammar: S <- 'a' 'b'
    // Input: "a" (will fail)
    auto parser = create_parser("S <- 'a' 'b'");
    parser->enable_packrat_parsing();

    int before_hook_count = 0;
    int after_hook_count = 0;

    parser->set_before_parse_rule_callback([&](const char* s, size_t n, const peg::Rule& rule, any& dt) {
        if (rule.name == "S") {
            before_hook_count++;
        }
    });
    parser->set_after_parse_rule_callback([&](const char* s, size_t n, const peg::Rule& rule, any& dt) {
        if (rule.name == "S") {
            after_hook_count++;
        }
    });

    std::string test_input = "a";
    ASSERT_FALSE(parser->parse(test_input));

    // The 'S' rule is attempted, so before hook should be called.
    // Even if it fails, the after hook should also be called.
    ASSERT_EQ(before_hook_count, 1);
    ASSERT_EQ(after_hook_count, 1);
}

TEST(PackratParsingTest, PackratMemoizationAcrossAlternatives) {
    // Grammar: S <- A 'b' / A 'c'
    //          A <- 'a'
    // Input: "ac"
    // 'A' should be parsed and memoized, so its action is called only once.
    auto parser = create_parser("S <- A 'b' / A 'c'\nA <- 'a'");
    parser->enable_packrat_parsing();

    int a_action_count = 0;
    parser->set_action("A", [&](const peg::peg_context& c, any& dt) {
        a_action_count++;
        return c.token();
    });

    std::string test_input = "ac";
    ASSERT_TRUE(parser->parse(test_input));

    // 'A' is attempted for the first alternative, succeeds, and is memoized.
    // When the first alternative fails ('b' not found), backtracking occurs.
    // For the second alternative, 'A' is retrieved from memoization.
    // Thus, the action for 'A' should only be called once.
    ASSERT_EQ(a_action_count, 1);
}

TEST(PackratParsingTest, PackratMemoizationWithDifferentStartPositions) {
    // Grammar: S <- 'a' A 'a'
    //          A <- 'a'
    // Input: "aaa"
    // The 'a' rule is called at different positions. Packrat should memoize for each position.
    auto parser = create_parser("S <- 'a' A 'a'\nA <- 'a'");
    parser->enable_packrat_parsing();

    int a_char_action_count = 0;
    parser->set_action("'a'", [&](const peg::peg_context& c, any& dt) {
        a_char_action_count++;
        return c.token();
    });

    std::string test_input = "aaa";
    ASSERT_TRUE(parser->parse(test_input));

    // The literal 'a' is matched three times, at index 0, 1, and 2.
    // Each match is at a different position, so packrat will store separate entries.
    // The action for 'a' should be called for each distinct match.
    ASSERT_EQ(a_char_action_count, 3);
}

TEST(PackratParsingTest, PackratWithCustomData) {
    // Grammar: S <- 'a'
    auto parser = create_parser("S <- 'a'");
    parser->enable_packrat_parsing();

    struct MyCustomData {
        int value = 0;
    };

    parser->set_action("S", [&](const peg::peg_context& c, any& dt) {
        MyCustomData& data = any_cast<MyCustomData&>(dt);
        data.value = 123;
        return c.token();
    });

    std::string test_input = "a";
    MyCustomData custom_data;
    ASSERT_TRUE(parser->parse(test_input, custom_data));
    ASSERT_EQ(custom_data.value, 123);
}

TEST(PackratParsingTest, PackratWithLongInput) {
    // Grammar: S <- 'a'+
    // Input: "aaaa...a" (1000 'a's)
    auto parser = create_parser("S <- 'a'+");
    parser->enable_packrat_parsing();

    std::string test_input(1000, 'a');
    ASSERT_TRUE(parser->parse(test_input));

    // This test primarily checks for performance and stack overflow issues with long inputs
    // when packrat parsing is enabled. No specific action count assertion needed,
    // just that it parses successfully.
}

TEST(PackratParsingTest, PackratDisabled) {
    // Grammar: A <- 'a' A / 'a'
    // Input: "aaa"
    // Verify that without packrat parsing, the behavior is different (e.g., potentially slower
    // or different action counts if not carefully designed).
    auto parser = create_parser("A <- 'a' A / 'a'");
    // Do NOT enable packrat parsing: parser->enable_packrat_parsing();

    int a_rule_invocations = 0;
    parser->set_action("A", [&](const peg::peg_context& c, any& dt) {
        a_rule_invocations++;
        return c.token();
    });

    std::string test_input = "aaa";
    ASSERT_TRUE(parser->parse(test_input));

    // Without packrat, the left-recursive rule might be transformed and still result in 3 calls
    // for the action, as the action is tied to successful matches. The key benefit of packrat
    // is memoizing the *parsing results* (length and value), not necessarily reducing action calls
    // for every successful match of a rule.
    // However, if the grammar was designed to show re-evaluation, this would be different.
    // For peglib's internal handling of left-recursion, the action count remains 3.
    ASSERT_EQ(a_rule_invocations, 3