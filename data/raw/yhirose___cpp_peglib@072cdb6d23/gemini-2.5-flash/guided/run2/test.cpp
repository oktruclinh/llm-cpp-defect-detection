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

// Test case 1: Basic successful parsing
TEST_F(HolderParseCoreTest, BasicSuccessfulParse) {
    auto parser = create_parser("ROOT = 'hello'");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("ROOT");
    auto ret = parser->parse("hello");
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.value(), 5);
}

// Test case 2: Basic failed parsing
TEST_F(HolderParseCoreTest, BasicFailedParse) {
    auto parser = create_parser("ROOT = 'hello'");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("ROOT");
    auto ret = parser->parse("world");
    ASSERT_FALSE(ret);
}

// Test case 3: Parsing with semantic value capture
TEST_F(HolderParseCoreTest, SemanticValueCapture) {
    auto parser = create_parser("ROOT = <'hello'>");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("ROOT");
    std::string val;
    auto ret = parser->parse("hello", val);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.value(), 5);
    ASSERT_EQ(val, "hello");
}

// Test case 4: Parsing with multiple rules and semantic values
TEST_F(HolderParseCoreTest, MultipleRulesAndSemanticValues) {
    auto parser = create_parser("ROOT = A B\nA = <'foo'>\nB = <'bar'>");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("ROOT");
    std::vector<std::string> vals;
    auto ret = parser->parse("foobar", vals);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.value(), 6);
    ASSERT_EQ(vals.size(), 2);
    ASSERT_EQ(vals[0], "foo");
    ASSERT_EQ(vals[1], "bar");
}

// Test case 5: Rule with action that modifies semantic value
TEST_F(HolderParseCoreTest, RuleActionModifiesSemanticValue) {
    auto parser = create_parser("NUMBER = <[0-9]+>");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_action("NUMBER", [](const SemanticValues& sv) {
        return std::stoi(sv.token());
    });

    parser->set_start_rule("NUMBER");
    int val = 0;
    auto ret = parser->parse("123", val);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.value(), 3);
    ASSERT_EQ(val, 123);
}

// Test case 6: Rule with action that throws a parse_error
TEST_F(HolderParseCoreTest, RuleActionThrowsParseError) {
    auto parser = create_parser("NUMBER = <[0-9]+>");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_action("NUMBER", [](const SemanticValues& sv) -> int {
        int num = std::stoi(sv.token());
        if (num % 2 != 0) {
            throw parse_error("Odd number not allowed");
        }
        return num;
    });

    parser->set_start_rule("NUMBER");

    // Test case where action succeeds
    int val_even = 0;
    auto ret_even = parser->parse("12", val_even);
    ASSERT_TRUE(ret_even);
    ASSERT_EQ(ret_even.value(), 2);
    ASSERT_EQ(val_even, 12);

    // Test case where action throws an error
    int val_odd = 0;
    auto ret_odd = parser->parse("13", val_odd);
    ASSERT_FALSE(ret_odd);
    // The parse should fail, and the value should not be set or remain default
    ASSERT_EQ(val_odd, 0); // Assuming val_odd was initialized to 0
}

// Test case 7: Rule with action that throws a parse_error and check error message
TEST_F(HolderParseCoreTest, RuleActionThrowsParseErrorWithMessage) {
    auto parser = create_parser("NUMBER = <[0-9]+>");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_action("NUMBER", [](const SemanticValues& sv) -> int {
        int num = std::stoi(sv.token());
        if (num % 2 != 0) {
            throw parse_error("Odd number not allowed");
        }
        return num;
    });

    parser->set_start_rule("NUMBER");

    // Test case where action throws an error
    int val_odd = 0;
    auto ret_odd = parser->parse("13", val_odd);
    ASSERT_FALSE(ret_odd);
    ASSERT_EQ(parser->error_message(), "Odd number not allowed");
}

// Test case 8: Empty input string
TEST_F(HolderParseCoreTest, EmptyInput) {
    auto parser = create_parser("ROOT = 'hello'");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("ROOT");
    auto ret = parser->parse("");
    ASSERT_FALSE(ret); // Should fail as 'hello' cannot be matched
}

// Test case 9: Empty rule matching empty string
TEST_F(HolderParseCoreTest, EmptyRuleMatchEmptyString) {
    auto parser = create_parser("ROOT = ''"); // Empty string literal
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("ROOT");
    auto ret = parser->parse("");
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.value(), 0); // Should match 0 characters
}

// Test case 10: Rule with error message
TEST_F(HolderParseCoreTest, RuleWithErrorMessages) {
    auto parser = create_parser("ROOT = A\nA = 'foo'");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_error_message("A", "Expected 'foo'");

    parser->set_start_rule("ROOT");
    auto ret = parser->parse("bar");
    ASSERT_FALSE(ret);
    ASSERT_EQ(parser->error_message(), "Expected 'foo'");
}

// Test case 11: Prioritized choice (first option matches)
TEST_F(HolderParseCoreTest, PrioritizedChoiceFirstMatch) {
    auto parser = create_parser("ROOT = 'foo' / 'bar'");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("ROOT");
    auto ret = parser->parse("foo");
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.value(), 3);
}

// Test case 12: Prioritized choice (second option matches)
TEST_F(HolderParseCoreTest, PrioritizedChoiceSecondMatch) {
    auto parser = create_parser("ROOT = 'foo' / 'bar'");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("ROOT");
    auto ret = parser->parse("bar");
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.value(), 3);
}

// Test case 13: Prioritized choice (no option matches)
TEST_F(HolderParseCoreTest, PrioritizedChoiceNoMatch) {
    auto parser = create_parser("ROOT = 'foo' / 'bar'");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("ROOT");
    auto ret = parser->parse("baz");
    ASSERT_FALSE(ret);
}

// Test case 14: Macro reference (simple case)
TEST_F(HolderParseCoreTest, MacroReference) {
    auto parser = create_parser("ROOT = @macro_rule\nmacro_rule = 'test'");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("ROOT");
    auto ret = parser->parse("test");
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.value(), 4);
}

// Test case 15: Macro reference with semantic value
TEST_F(HolderParseCoreTest, MacroReferenceWithSemanticValue) {
    auto parser = create_parser("ROOT = @macro_rule\nmacro_rule = <'test'>");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("ROOT");
    std::string val;
    auto ret = parser->parse("test", val);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.value(), 4);
    ASSERT_EQ(val, "test");
}

// Test case 16: Packrat parsing (ensuring memoization works)
// This is hard to test directly from public API, but we can infer it
// by testing a recursive grammar that would otherwise fail without packrat.
TEST_F(HolderParseCoreTest, PackratParsingRecursiveGrammar) {
    // A simple left-recursive grammar that requires packrat parsing
    // to avoid infinite recursion.
    // This grammar is ambiguous, but peglib's packrat will handle it.
    auto parser = create_parser("Expr = Expr '+' Term / Term\nTerm = <[0-9]+>");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_start_rule("Expr");
    std::vector<std::string> vals; // To capture terms
    auto ret = parser->parse("1+2+3", vals);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.value(), 5);
    // The semantic values might be nested depending on the reduction,
    // but the key is that it parses without stack overflow.
    // For this simple test, we just check success.
}

// Test case 17: Custom enter/leave callbacks
TEST_F(HolderParseCoreTest, CustomEnterLeaveCallbacks) {
    auto parser = create_parser("ROOT = 'hello'");
    ASSERT_TRUE(static_cast<bool>(*parser));

    bool entered = false;
    bool left = false;
    size_t leave_len = 0;
    std::string leave_val;

    parser->set_enter_callback("ROOT", [&](const char* s, size_t n, any& dt) {
        entered = true;
        // dt can be used to pass data between callbacks or actions
        dt = std::string("data_from_enter");
    });

    parser->set_leave_callback("ROOT", [&](const char* s, size_t n, size_t len, any& val, any& dt) {
        left = true;
        leave_len = len;
        if (val.is<std::string>()) {
            leave_val = val.get<std::string>();
        }
        // Check data from enter callback
        ASSERT_TRUE(dt.is<std::string>());
        ASSERT_EQ(dt.get<std::string>(), "data_from_enter");
    });

    parser->set_action("ROOT", [](const SemanticValues& sv) {
        return std::string("action_value");
    });

    parser->set_start_rule("ROOT");
    std::string result_val;
    auto ret = parser->parse("hello", result_val);

    ASSERT_TRUE(ret);
    ASSERT_TRUE(entered);
    ASSERT_TRUE(left);
    ASSERT_EQ(leave_len, 5);
    ASSERT_EQ(leave_val, "action_value");
    ASSERT_EQ(result_val, "action_value");
}

// Test case 18: Custom enter/leave callbacks for a failing parse
TEST_F(HolderParseCoreTest, CustomEnterLeaveCallbacksFailingParse) {
    auto parser = create_parser("ROOT = 'hello'");
    ASSERT_TRUE(static_cast<bool>(*parser));

    bool entered = false;
    bool left = false;
    size_t leave_len = 0; // Should be -1 for failure

    parser->set_enter_callback("ROOT", [&](const char* s, size_t n, any& dt) {
        entered = true;
    });

    parser->set_leave_callback("ROOT", [&](const char* s, size_t n, size_t len, any& val, any& dt) {
        left = true;
        leave_len = len;
    });

    parser->set_start_rule("ROOT");
    auto ret = parser->parse("world"); // This should fail

    ASSERT_FALSE(ret);
    ASSERT_TRUE(entered); // Enter should still be called
    ASSERT_TRUE(left);    // Leave should still be called
    ASSERT_EQ(leave_len, static_cast<size_t>(-1)); // len should indicate failure
}

// Test case 19: Ignore semantic value
TEST_F(HolderParseCoreTest, IgnoreSemanticValue) {
    auto parser = create_parser("ROOT = A B\nA = <'foo'>\nB = <'bar'>");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_action("A", [](const SemanticValues& sv) { return sv.token(); });
    parser->set_action("B", [](const SemanticValues& sv) { return sv.token(); });

    parser->set_ignore_semantic_value("A"); // Ignore 'foo'

    parser->set_start_rule("ROOT");
    std::vector<std::string> vals;
    auto ret = parser->parse("foobar", vals);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.value(), 6);
    ASSERT_EQ(vals.size(), 1); // Only 'bar' should be captured
    ASSERT_EQ(vals[0], "bar");
}

// Test case 20: Uninitialized definition (should not happen with valid grammar)
// This case is hard to trigger via public API because the parser constructor
// already validates the grammar. If `ope_` is null, it means the grammar
// was malformed or a rule was referenced that doesn't exist.
// The `create_parser` helper already checks for this.
// We can simulate it by trying to parse with a parser that failed to initialize.
TEST_F(HolderParseCoreTest, UninitializedDefinitionError) {
    // Create a parser with an invalid grammar
    auto parser = std::make_shared<Parser>("ROOT = UndefinedRule");
    ASSERT_FALSE(static_cast<bool>(*parser)); // Parser construction should fail

    parser->set_start_rule("ROOT"); // This might not even be necessary as the parser is invalid

    // Attempting to parse with an invalid parser should return false
    auto ret = parser->parse("test");
    ASSERT_FALSE(ret);
    // The internal error message should reflect the grammar issue, not necessarily the logic_error
    // from parse_core directly, as the error happens earlier in the setup.
    ASSERT_NE(parser->error_message().find("undefined rule"), std::string::npos);
}

// Test case 21: Nested semantic values
TEST_F(HolderParseCoreTest, NestedSemanticValues) {
    auto parser = create_parser("ROOT = A B\nA = <'x'>\nB = <'y'>");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_action("ROOT", [](const SemanticValues& sv) {
        std::string result = sv[0].get<std::string>() + sv[1].get<std::string>();
        return result;
    });

    parser->set_start_rule("ROOT");
    std::string val;
    auto ret = parser->parse("xy", val);
    ASSERT_TRUE(ret);
    ASSERT_EQ(ret.value(), 2);
    ASSERT_EQ(val, "xy");
}

// Test case 22: Semantic value tags
TEST_F(HolderParseCoreTest, SemanticValueTags) {
    auto parser = create_parser("ROOT = A B\nA = <'x'>\nB = <'y'>");
    ASSERT_TRUE(static_cast<bool>(*parser));

    parser->set_action("ROOT", [](const SemanticValues& sv) {
        std::string result = "";
        for (size_t i = 0; i < sv.size(); ++i) {
            result += sv.tags[i].name;
            result += ":";
            result += sv[i].get<std::string>();