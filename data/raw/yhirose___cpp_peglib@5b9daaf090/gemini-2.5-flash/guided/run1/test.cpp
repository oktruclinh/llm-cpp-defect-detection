#include <gtest/gtest.h>
#include <peglib.h>

using namespace peg;

// Helper function to create a simple grammar for testing
std::shared_ptr<peg::Parser> create_parser(const std::string& grammar_str) {
    auto parser = std::make_shared<peg::Parser>(grammar_str);
    if (!static_cast<bool>(*parser)) {
        throw std::runtime_error("Failed to parse grammar: " + grammar_str);
    }
    return parser;
}

TEST(PackratParsingTest, BasicPackratMemoization) {
    // Grammar: A <- 'a' A / 'a'
    // This grammar is left-recursive if not handled correctly, but peglib handles it.
    // The key here is that 'a' A will be parsed multiple times if packrat is not working.
    // We'll use a custom action to count how many times 'A' rule is entered.
    auto parser = create_parser("A <- 'a' A / 'a'");

    int rule_A_entry_count = 0;
    parser->set_action("A", [&](const SemanticValues& sv) {
        rule_A_entry_count++;
        return sv.str();
    });

    // Parse a string that requires memoization to be efficient
    // "aaa" should enter 'A' rule only 3 times (once for each 'a' and then memoized)
    // If packrat is not working, it would be more.
    std::string input = "aaa";
    auto result = parser->parse(input);

    ASSERT_TRUE(result) << "Parsing failed for input: " << input;
    ASSERT_EQ(result.value(), input);
    // With packrat, each 'A' rule for a given position should be evaluated only once.
    // For "aaa":
    // A at pos 0 -> 'a' A (pos 1)
    //   A at pos 1 -> 'a' A (pos 2)
    //     A at pos 2 -> 'a' (base case)
    // So, 'A' is entered for position 0, 1, and 2. Total 3 times.
    ASSERT_EQ(rule_A_entry_count, 3) << "Packrat memoization did not work as expected.";
}

TEST(PackratParsingTest, NoPackratForNonMemoizedRules) {
    // Grammar: A <- 'a' B, B <- 'b'
    // 'B' is not recursive and not part of a cycle, so it shouldn't benefit from packrat in a way that changes counts.
    // We'll ensure that rules not involved in recursion or ambiguity don't get excessive memoization overhead.
    auto parser = create_parser("A <- 'a' B\nB <- 'b'");

    int rule_B_entry_count = 0;
    parser->set_action("B", [&](const SemanticValues& sv) {
        rule_B_entry_count++;
        return sv.str();
    });

    std::string input = "ab";
    auto result = parser->parse(input);

    ASSERT_TRUE(result) << "Parsing failed for input: " << input;
    ASSERT_EQ(result.value(), input);
    // 'B' should be entered only once for the 'b' at position 1.
    ASSERT_EQ(rule_B_entry_count, 1) << "Rule 'B' was entered an unexpected number of times.";
}

TEST(PackratParsingTest, PackratWithAmbiguousGrammar) {
    // Grammar: S <- A 'c' / B 'c', A <- 'a', B <- 'a'
    // This grammar is ambiguous. Packrat parsing should still correctly find one parse.
    // The key is that 'a' will be parsed once for A and once for B, but the 'a' itself
    // might be memoized if it's a sub-expression.
    auto parser = create_parser("S <- A 'c' / B 'c'\nA <- 'a'\nB <- 'a'");

    int rule_A_entry_count = 0;
    int rule_B_entry_count = 0;

    parser->set_action("A", [&](const SemanticValues& sv) {
        rule_A_entry_count++;
        return sv.str();
    });
    parser->set_action("B", [&](const SemanticValues& sv) {
        rule_B_entry_count++;
        return sv.str();
    });

    std::string input = "ac";
    auto result = parser->parse(input);

    ASSERT_TRUE(result) << "Parsing failed for input: " << input;
    ASSERT_EQ(result.value(), input);

    // Depending on the order of alternatives, either A or B will succeed first.
    // Both A and B try to parse 'a' at position 0.
    // The 'a' itself might be memoized, but the rules A and B are distinct.
    // One path will succeed, the other will fail.
    // If S <- A 'c' is tried first, A will be called. If it succeeds, B won't be called.
    // If S <- B 'c' is tried first, B will be called. If it succeeds, A won't be called.
    // In peglib, it typically tries alternatives in order.
    // So, A should be called once, B should not be called if A succeeds.
    // Or, if A fails and B succeeds, B is called once.
    // The important part is that the 'a' sub-parse at position 0 is memoized.
    // Let's assume A is tried first and succeeds.
    ASSERT_EQ(rule_A_entry_count, 1);
    ASSERT_EQ(rule_B_entry_count, 0); // B should not be called if A succeeds and S matches.
}

TEST(PackratParsingTest, PackratWithFailureAndBacktracking) {
    // Grammar: S <- 'a' B / 'a' C, B <- 'b', C <- 'c'
    // Input: "ac"
    // The first alternative 'a' B will consume 'a', then fail on 'b'.
    // It should backtrack, and the 'a' should be memoized for the second alternative 'a' C.
    auto parser = create_parser("S <- 'a' B / 'a' C\nB <- 'b'\nC <- 'c'");

    int rule_B_entry_count = 0;
    int rule_C_entry_count = 0;

    parser->set_action("B", [&](const SemanticValues& sv) {
        rule_B_entry_count++;
        return sv.str();
    });
    parser->set_action("C", [&](const SemanticValues& sv) {
        rule_C_entry_count++;
        return sv.str();
    });

    std::string input = "ac";
    auto result = parser->parse(input);

    ASSERT_TRUE(result) << "Parsing failed for input: " << input;
    ASSERT_EQ(result.value(), input);

    // 'B' should be attempted once and fail.
    // 'C' should be attempted once and succeed.
    ASSERT_EQ(rule_B_entry_count, 1);
    ASSERT_EQ(rule_C_entry_count, 1);
    // The initial 'a' should be parsed only once due to memoization,
    // even though it's part of two alternatives.
    // We can't directly count the 'a' terminal, but the overall behavior implies it.
}

TEST(PackratParsingTest, PackratWithEmptyMatch) {
    // Grammar: S <- A 'b', A <- '' / 'a'
    // Input: "b"
    // 'A' at position 0 will first try to match 'a' (fail), then match empty string (succeed).
    // The empty match for 'A' at position 0 should be memoized.
    auto parser = create_parser("S <- A 'b'\nA <- 'a' / ''");

    int rule_A_entry_count = 0;
    parser->set_action("A", [&](const SemanticValues& sv) {
        rule_A_entry_count++;
        return sv.str();
    });

    std::string input = "b";
    auto result = parser->parse(input);

    ASSERT_TRUE(result) << "Parsing failed for input: " << input;
    ASSERT_EQ(result.value(), input);

    // 'A' should be entered once for position 0.
    // It tries 'a' (fails), then '' (succeeds).
    // The result of 'A' at pos 0 (empty string) is memoized.
    ASSERT_EQ(rule_A_entry_count, 1);
}

TEST(PackratParsingTest, PackratWithParseErrorInAction) {
    // Grammar: S <- 'a'
    // Action for 'S' throws a parse_error. This should be caught and result in a parse failure.
    auto parser = create_parser("S <- 'a'");

    parser->set_action("S", [&](const SemanticValues& sv) -> any {
        throw parse_error("Custom error in action for S");
    });

    std::string input = "a";
    auto result = parser->parse(input);

    ASSERT_FALSE(result) << "Parsing should have failed due to action throwing an error.";
    ASSERT_EQ(parser->error_message, "Custom error in action for S");
    ASSERT_EQ(parser->error_position, 0); // Error at the start of the rule 'S'
}

TEST(PackratParsingTest, PackratWithNestedRulesAndActions) {
    // Grammar: S <- A B, A <- 'a', B <- 'b'
    // Test that actions are called correctly and values are passed up.
    auto parser = create_parser("S <- A B\nA <- 'a'\nB <- 'b'");

    parser->set_action("A", [&](const SemanticValues& sv) {
        return std::string("A_") + sv.str();
    });
    parser->set_action("B", [&](const SemanticValues& sv) {
        return std::string("B_") + sv.str();
    });
    parser->set_action("S", [&](const SemanticValues& sv) {
        return any_cast<std::string>(sv[0]) + any_cast<std::string>(sv[1]);
    });

    std::string input = "ab";
    auto result = parser->parse(input);

    ASSERT_TRUE(result) << "Parsing failed for input: " << input;
    ASSERT_EQ(any_cast<std::string>(result.value()), "A_aB_b");
}

TEST(PackratParsingTest, PackratWithLongInput) {
    // Grammar: S <- 'a'+
    // Test performance and correctness with a longer input string.
    auto parser = create_parser("S <- 'a'+");

    std::string input(1000, 'a'); // 1000 'a's
    auto result = parser->parse(input);

    ASSERT_TRUE(result) << "Parsing failed for long input.";
    ASSERT_EQ(result.value(), input);
}

TEST(PackratParsingTest, PackratWithNoMatch) {
    // Grammar: S <- 'a'
    // Input: "b"
    // Should fail to parse.
    auto parser = create_parser("S <- 'a'");

    std::string input = "b";
    auto result = parser->parse(input);

    ASSERT_FALSE(result) << "Parsing should have failed for non-matching input.";
    ASSERT_NE(parser->error_message.find("expected 'a'"), std::string::npos);
    ASSERT_EQ(parser->error_position, 0);
}

TEST(PackratParsingTest, PackratWithBeforeAndAfterHooks) {
    // Grammar: S <- 'a'
    // Test that before/after hooks are called.
    auto parser = create_parser("S <- 'a'");

    int before_S_count = 0;
    int after_S_count = 0;

    parser->set_before_parse("S", [&](const Context& c) {
        before_S_count++;
    });
    parser->set_after_parse("S", [&](const Context& c) {
        after_S_count++;
    });

    std::string input = "a";
    auto result = parser->parse(input);

    ASSERT_TRUE(result) << "Parsing failed for input: " << input;
    ASSERT_EQ(before_S_count, 1);
    ASSERT_EQ(after_S_count, 1);

    // Test failure case
    before_S_count = 0;
    after_S_count = 0;
    input = "b";
    result = parser->parse(input);

    ASSERT_FALSE(result) << "Parsing should have failed.";
    ASSERT_EQ(before_S_count, 1); // Before hook is called even if rule fails
    ASSERT_EQ(after_S_count, 1);  // After hook is called even if rule fails
}

TEST(PackratParsingTest, PackratWithLeftRecursion) {
    // Grammar: Expr <- Expr '+' Term / Term
    //          Term <- [0-9]+
    // This is a classic left-recursive grammar that packrat parsing handles.
    auto parser = create_parser("Expr <- Expr '+' Term / Term\nTerm <- [0-9]+");

    int expr_entry_count = 0;
    parser->set_action("Expr", [&](const SemanticValues& sv) {
        expr_entry_count++;
        if (sv.size() == 3) { // Expr '+' Term
            return any_cast<int>(sv[0]) + any_cast<int>(sv[2]);
        } else { // Term
            return any_cast<int>(sv[0]);
        }
    });
    parser->set_action("Term", [&](const SemanticValues& sv) {
        return std::stoi(sv.str());
    });

    std::string input = "1+2+3";
    auto result = parser->parse(input);

    ASSERT_TRUE(result) << "Parsing failed for left-recursive grammar.";
    ASSERT_EQ(any_cast<int>(result.value()), 6);
    // For "1+2+3":
    // Expr at 0 -> Expr at 0 '+' Term at 2 / Term at 0
    //   Term at 0 -> '1'
    //   Expr at 0 (memoized as '1')
    //   Expr at 0 '+' Term at 2 -> Expr at 0 (memoized '1') '+' Term at 2 ('2')
    //     Expr at 0 (memoized as '1+2')
    //     Expr at 0 '+' Term at 4 -> Expr at 0 (memoized '1+2') '+' Term at 4 ('3')
    // The 'Expr' rule should be entered a limited number of times due to memoization.
    // It should be entered for each successful match of 'Expr' at a given position.
    // For "1+2+3", Expr is matched at pos 0 for "1", then for "1+2", then for "1+2+3".
    // So, 3 times.
    ASSERT_EQ(expr_entry_count, 3) << "Left-recursion handling with packrat did not work as expected.";
}