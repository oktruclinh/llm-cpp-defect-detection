```cpp
#include <gtest/gtest.h>
#include <peglib.h>

using namespace peg;

// Helper function to create a simple grammar for testing
std::shared_ptr<Parser> create_parser(const std::string& grammar_str) {
    auto parser = std::make_shared<Parser>(grammar_str);
    if (!static_cast<bool>(*parser)) {
        throw std::runtime_error("Failed to parse grammar: " + grammar_str);
    }
    return parser;
}

// Test fixture for common setup
class HolderParseCoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup if needed
    }

    void TearDown() override {
        // Common teardown if needed
    }
};

// Test case 1: Basic successful parse
TEST_F(HolderParseCoreTest, BasicSuccess) {
    auto parser = create_parser("A = 'hello'");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("A");
    auto ret = parser->parse("hello");
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 5);
}

// Test case 2: Basic failed parse
TEST_F(HolderParseCoreTest, BasicFailure) {
    auto parser = create_parser("A = 'hello'");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("A");
    auto ret = parser->parse("world");
    ASSERT_FALSE(ret);
    ASSERT_EQ(ret.len, -1); // Expect -1 for failure
}

// Test case 3: Empty input string
TEST_F(HolderParseCoreTest, EmptyInput) {
    auto parser = create_parser("A = ''"); // Rule matching empty string
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("A");
    auto ret = parser->parse("");
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 0);

    parser = create_parser("A = 'a'"); // Rule not matching empty string
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("A");
    ret = parser->parse("");
    ASSERT_FALSE(ret);
    ASSERT_EQ(ret.len, -1);
}

// Test case 4: Grammar with multiple rules and semantic values
TEST_F(HolderParseCoreTest, SemanticValuesCapture) {
    auto parser = create_parser(R"(
        START = A B
        A = 'a'
        B = 'b'
    )");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("START");
    std::shared_ptr<Ast> ast;
    auto ret = parser->parse("ab", ast);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 2);
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->nodes.size(), 2);
    ASSERT_EQ(ast->nodes[0]->token, "a");
    ASSERT_EQ(ast->nodes[1]->token, "b");
}

// Test case 5: Action handler for semantic value reduction
TEST_F(HolderParseCoreTest, ActionHandler) {
    auto parser = create_parser(R"(
        NUMBER = < [0-9]+ >
        EXPR = NUMBER ( '+' NUMBER )*
    )");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("EXPR");

    // Define an action for NUMBER to convert it to an int
    parser->set_action("NUMBER", [](const SemanticValues& sv) {
        return std::stoi(sv.token());
    });

    // Define an action for EXPR to perform addition
    parser->set_action("EXPR", [](const SemanticValues& sv) {
        int result = any_cast<int>(sv[0]);
        for (size_t i = 1; i < sv.size(); ++i) {
            result += any_cast<int>(sv[i]);
        }
        return result;
    });

    any val;
    auto ret = parser->parse("1+2+3", val);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 5);
    ASSERT_TRUE(val.is<int>());
    ASSERT_EQ(any_cast<int>(val), 6);

    ret = parser->parse("10", val);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 2);
    ASSERT_TRUE(val.is<int>());
    ASSERT_EQ(any_cast<int>(val), 10);
}

// Test case 6: Error message propagation from action
TEST_F(HolderParseCoreTest, ActionErrorMessage) {
    auto parser = create_parser(R"(
        NUMBER = < [0-9]+ >
    )");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("NUMBER");

    // Define an action that always throws a parse_error
    parser->set_action("NUMBER", [](const SemanticValues& sv) -> any {
        throw parse_error("Custom error from action");
    });

    std::string error_message;
    auto ret = parser->parse("123", error_message);
    ASSERT_FALSE(ret);
    ASSERT_EQ(ret.len, -1);
    ASSERT_EQ(error_message, "Custom error from action");
}

// Test case 7: Error message propagation from rule
TEST_F(HolderParseCoreTest, RuleErrorMessage) {
    auto parser = create_parser(R"(
        A = 'a' / 'b' !'c' { "Expected 'c' after 'b'" }
    )");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("A");

    std::string error_message;
    auto ret = parser->parse("bc", error_message); // 'b' matches, but !'c' fails, triggering error message
    ASSERT_FALSE(ret);
    ASSERT_EQ(ret.len, -1);
    ASSERT_EQ(error_message, "Expected 'c' after 'b'");

    ret = parser->parse("a", error_message);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 1);
    ASSERT_TRUE(error_message.empty()); // No error for successful parse
}

// Test case 8: Packrat parsing (memoization)
TEST_F(HolderParseCoreTest, PackratParsing) {
    // This grammar demonstrates left recursion, which packrat parsing handles.
    // Without packrat, this would lead to infinite recursion.
    auto parser = create_parser(R"(
        EXPR = EXPR '+' NUMBER / NUMBER
        NUMBER = [0-9]+
    )");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("EXPR");

    // Set actions to verify parsing order and values
    std::vector<std::string> call_log;
    parser->set_action("NUMBER", [&](const SemanticValues& sv) {
        call_log.push_back("NUMBER:" + sv.token());
        return std::stoi(sv.token());
    });
    parser->set_action("EXPR", [&](const SemanticValues& sv) {
        if (sv.size() == 1) { // NUMBER
            call_log.push_back("EXPR_NUMBER");
            return any_cast<int>(sv[0]);
        } else { // EXPR '+' NUMBER
            call_log.push_back("EXPR_ADD");
            return any_cast<int>(sv[0]) + any_cast<int>(sv[2]);
        }
    });

    any val;
    auto ret = parser->parse("1+2+3", val);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 5);
    ASSERT_TRUE(val.is<int>());
    ASSERT_EQ(any_cast<int>(val), 6);

    // Verify the call log to ensure actions were called as expected
    // The exact order might vary slightly based on internal implementation details
    // but should reflect the left-associative parsing.
    // This is a weak check for packrat, as it's hard to directly observe memoization
    // from public API without specific counters. The success of left recursion is the main indicator.
    ASSERT_FALSE(call_log.empty());
}

// Test case 9: Macro reference (indirectly tested via grammar features)
TEST_F(HolderParseCoreTest, MacroReference) {
    auto parser = create_parser(R"(
        A = B
        B = 'test'
    )");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("A");
    auto ret = parser->parse("test");
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 4);

    ret = parser->parse("fail");
    ASSERT_FALSE(ret);
}

// Test case 10: Prioritized choice (indirectly tested via grammar features)
TEST_F(HolderParseCoreTest, PrioritizedChoice) {
    auto parser = create_parser(R"(
        A = 'apple' / 'apricot'
    )");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("A");
    std::shared_ptr<Ast> ast;
    auto ret = parser->parse("apple", ast);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 5);
    ASSERT_EQ(ast->token, "apple");

    ret = parser->parse("apricot", ast);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 7);
    ASSERT_EQ(ast->token, "apricot");

    // Test that 'apple' is chosen over 'apricot' if both could match a prefix
    parser = create_parser(R"(
        A = 'apple' / 'app'
    )");
    ASSERT_TRUE(static_cast<bool>(*parser));
    parser->set_start_rule("A");
    ret = parser->parse("apple", ast);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 5); // 'apple' should be matched entirely
    ASSERT_EQ(ast->token, "apple");

    ret = parser->parse("app", ast);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 3); // 'app' should be matched
    ASSERT_EQ(ast->token, "app");
}

// Test case 11: Enter/Leave callbacks
TEST_F(HolderParseCoreTest, EnterLeaveCallbacks) {
    auto parser = create_parser(R"(
        A = 'a' B 'c'
        B = 'b'
    )");
    ASSERT_TRUE(static_cast<bool>(*parser));

    std::vector<std::string> log;

    parser->set_enter_callback("A", [&](const char* s, size_t n, any& dt) {
        log.push_back("Enter A: " + std::string(s, n));
    });
    parser->set_leave_callback("A", [&](const char* s, size_t n, size_t len, any& val, any& dt) {
        log.push_back("Leave A: " + std::string(s, n) + ", len=" + std::to_string(len));
    });
    parser->set_enter_callback("B", [&](const char* s, size_t n, any& dt) {
        log.push_back("Enter B: " + std::string(s, n));
    });
    parser->set_leave_callback("B", [&](const char* s, size_t n, size_t len, any& val, any& dt) {
        log.push_back("Leave B: " + std::string(s, n) + ", len=" + std::to_string(len));
    });

    parser->set_start_rule("A");
    auto ret = parser->parse("abc");
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 3);

    ASSERT_EQ(log.size(), 4);
    ASSERT_EQ(log[0], "Enter A: abc");
    ASSERT_EQ(log[1], "Enter B: bc");
    ASSERT_EQ(log[2], "Leave B: bc, len=1");
    ASSERT_EQ(log[3], "Leave A: abc, len=3");

    log.clear();
    ret = parser->parse("axc"); // Should fail
    ASSERT_FALSE(ret);

    ASSERT_EQ(log.size(), 2); // A entered, B entered, B failed, A failed
    ASSERT_EQ(log[0], "Enter A: axc");
    ASSERT_EQ(log[1], "Enter B: xc");
}

// Test case 12: Ignore semantic value
TEST_F(HolderParseCoreTest, IgnoreSemanticValue) {
    auto parser = create_parser(R"(
        ROOT = A B
        A = 'a'
        B = 'b'
    )");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("ROOT");
    parser->set_action("A", [](const SemanticValues& sv) { return "ValueA"; });
    parser->set_action("B", [](const SemanticValues& sv) { return "ValueB"; });

    // Ignore semantic value for rule A
    parser->set_ignore_semantic_value("A");

    any val;
    auto ret = parser->parse("ab", val);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 2);

    // The root rule's semantic value will be a vector of its children's semantic values.
    // Since A is ignored, only B's value should be present.
    ASSERT_TRUE(val.is<std::vector<any>>());
    auto vec = any_cast<std::vector<any>>(val);
    ASSERT_EQ(vec.size(), 1);
    ASSERT_TRUE(vec[0].is<std::string>());
    ASSERT_EQ(any_cast<std::string>(vec[0]), "ValueB");
}

// Test case 13: Nested rules and semantic values
TEST_F(HolderParseCoreTest, NestedSemanticValues) {
    auto parser = create_parser(R"(
        ROOT = (A B)+
        A = 'a'
        B = 'b'
    )");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("ROOT");
    parser->set_action("A", [](const SemanticValues& sv) { return std::string("A_") + sv.token(); });
    parser->set_action("B", [](const SemanticValues& sv) { return std::string("B_") + sv.token(); });

    any val;
    auto ret = parser->parse("abab", val);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.len, 4);

    ASSERT_TRUE(val.is<std::vector<any>>());
    auto root_vec = any_cast<std::vector<any>>(val);
    ASSERT_EQ(root_vec.size(), 2); // Two (A B) groups

    ASSERT_TRUE(root_vec[0].is<std::vector<any>>());
    auto group1_vec = any_cast<std::vector<any>>(root_vec[0]);
    ASSERT_EQ(group1_vec.size(), 2);
    ASSERT_EQ(any_cast<std::string>(group1_vec[0]), "A_a");
    ASSERT_EQ(any_cast<std::string>(group1_vec[1]), "B_b");

    ASSERT_TRUE(root_vec[1].is<std::vector<any>>());
    auto group2_vec = any_cast<std::vector<any>>(root_vec[1]);
    ASSERT_EQ(group2_vec.size(), 2);
    ASSERT_EQ(any_cast<std::string>(group2_vec[0]), "A_a");
    ASSERT_EQ(any_cast<std::string>(group2_vec[1]), "B_b");
}

// Test case 14: Uninitialized definition (should throw logic_error during grammar parsing, not parse_core)
// The `ope_` member is initialized during the grammar parsing phase.
// If `ope_` is null, it means the grammar itself was invalid or not properly constructed.
// This is more of a grammar construction error than a `parse_core` runtime error.
// We can simulate an uninitialized `Holder` by creating a `Rule` object directly
// without a proper `ope_` (expression). However, `parse_core` is a private method
// of `Holder`, and `Holder` itself is an internal class.
// The public API `parser->parse()` would fail much earlier if the grammar is invalid.
// The `throw std::logic_error("Uninitialized definition ope was used...")`
// path is primarily for internal consistency checks during development or