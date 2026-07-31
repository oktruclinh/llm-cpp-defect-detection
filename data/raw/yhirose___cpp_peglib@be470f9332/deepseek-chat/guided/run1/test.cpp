#include <gtest/gtest.h>
#include "peglib.h"

class LinkReferencesTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a grammar with some rules for testing
        grammar_ = std::make_shared<peg::Grammar>();
        (*grammar_)["rule1"] = peg::any();
        (*grammar_)["rule2"] = peg::seq({peg::lit("a"), peg::lit("b")});
        (*grammar_)["rule3"] = peg::lit("test");
    }

    std::shared_ptr<peg::Grammar> grammar_;
};

// Test that a reference to a macro parameter is correctly identified
TEST_F(LinkReferencesTest, MacroParameterReference) {
    peg::Parser parser(*grammar_);
    
    // Create a rule that uses a macro parameter reference
    auto rule = peg::seq({
        peg::lit("start"),
        peg::ref("$1"),  // Reference to first macro parameter
        peg::lit("end")
    });
    
    // This should not throw - the reference is a macro parameter
    EXPECT_NO_THROW(parser["test_rule"] = rule);
}

// Test that a reference to an existing grammar rule is correctly linked
TEST_F(LinkReferencesTest, ExistingRuleReference) {
    peg::Parser parser(*grammar_);
    
    // Create a rule that references an existing grammar rule
    auto rule = peg::seq({
        peg::lit("prefix"),
        peg::ref("rule1"),  // Reference to existing rule
        peg::lit("suffix")
    });
    
    // This should work fine
    EXPECT_NO_THROW(parser["test_rule"] = rule);
}

// Test that a reference to a non-existent rule throws an error
TEST_F(LinkReferencesTest, NonExistentRuleReference) {
    peg::Parser parser(*grammar_);
    
    // Create a rule that references a non-existent rule
    auto rule = peg::seq({
        peg::lit("prefix"),
        peg::ref("nonexistent_rule"),  // Reference to non-existent rule
        peg::lit("suffix")
    });
    
    // This should throw because the reference cannot be resolved
    EXPECT_THROW(parser["test_rule"] = rule, std::runtime_error);
}

// Test that a reference to a macro parameter takes precedence over a rule with the same name
TEST_F(LinkReferencesTest, MacroParameterPrecedence) {
    peg::Parser parser(*grammar_);
    
    // Create a rule that has both a macro parameter and a rule with the same name
    // The macro parameter should take precedence
    auto rule = peg::seq({
        peg::ref("$1"),  // This should be treated as macro parameter, not rule reference
        peg::lit("end")
    });
    
    // This should work fine
    EXPECT_NO_THROW(parser["test_rule"] = rule);
}

// Test that multiple references in a sequence are all resolved correctly
TEST_F(LinkReferencesTest, MultipleReferences) {
    peg::Parser parser(*grammar_);
    
    // Create a rule with multiple references
    auto rule = peg::seq({
        peg::ref("rule1"),
        peg::ref("rule2"),
        peg::ref("rule3")
    });
    
    // This should work fine
    EXPECT_NO_THROW(parser["test_rule"] = rule);
}

// Test that nested references (reference within a reference's arguments) are resolved
TEST_F(LinkReferencesTest, NestedReferences) {
    peg::Parser parser(*grammar_);
    
    // Create a rule that has a reference with arguments that contain references
    auto inner_ref = peg::ref("rule1");
    auto outer_ref = peg::ref("rule2", {inner_ref});
    
    // This should work fine
    EXPECT_NO_THROW(parser["test_rule"] = outer_ref);
}

// Test that empty reference name throws an error
TEST_F(LinkReferencesTest, EmptyReferenceName) {
    peg::Parser parser(*grammar_);
    
    // Create a rule with an empty reference name
    auto rule = peg::ref("");
    
    // This should throw because empty reference name is invalid
    EXPECT_THROW(parser["test_rule"] = rule, std::runtime_error);
}

// Test that reference with special characters in name is handled
TEST_F(LinkReferencesTest, SpecialCharacterReference) {
    peg::Parser parser(*grammar_);
    
    // Create a rule with a reference that has special characters
    auto rule = peg::ref("rule-with-dashes");
    
    // This should throw because the reference cannot be resolved
    EXPECT_THROW(parser["test_rule"] = rule, std::runtime_error);
}

// Test that a reference to a rule that is defined later is resolved correctly
TEST_F(LinkReferencesTest, ForwardReference) {
    peg::Parser parser(*grammar_);
    
    // Create a rule that references another rule that will be defined later
    auto rule = peg::seq({
        peg::ref("forward_rule"),
        peg::lit("end")
    });
    
    // Define the forward-referenced rule
    parser["forward_rule"] = peg::lit("start");
    
    // Now define the rule that uses the forward reference
    EXPECT_NO_THROW(parser["test_rule"] = rule);
}

// Test that a reference to a macro parameter with index 0 is handled
TEST_F(LinkReferencesTest, ZeroIndexMacroParameter) {
    peg::Parser parser(*grammar_);
    
    // Create a rule that references macro parameter $0
    auto rule = peg::seq({
        peg::ref("$0"),
        peg::lit("end")
    });
    
    // This should work fine - $0 is a valid macro parameter
    EXPECT_NO_THROW(parser["test_rule"] = rule);
}

// Test that a reference to a macro parameter with high index is handled
TEST_F(LinkReferencesTest, HighIndexMacroParameter) {
    peg::Parser parser(*grammar_);
    
    // Create a rule that references macro parameter $999
    auto rule = peg::seq({
        peg::ref("$999"),
        peg::lit("end")
    });
    
    // This should work fine - $999 is a valid macro parameter
    EXPECT_NO_THROW(parser["test_rule"] = rule);
}

// Test that a reference with no arguments is handled correctly
TEST_F(LinkReferencesTest, ReferenceWithoutArguments) {
    peg::Parser parser(*grammar_);
    
    // Create a simple reference without arguments
    auto rule = peg::ref("rule1");
    
    // This should work fine
    EXPECT_NO_THROW(parser["test_rule"] = rule);
}

// Test that a reference with multiple arguments is handled correctly
TEST_F(LinkReferencesTest, ReferenceWithMultipleArguments) {
    peg::Parser parser(*grammar_);
    
    // Create a reference with multiple arguments
    auto arg1 = peg::lit("arg1");
    auto arg2 = peg::lit("arg2");
    auto rule = peg::ref("rule1", {arg1, arg2});
    
    // This should work fine
    EXPECT_NO_THROW(parser["test_rule"] = rule);
}

// Test that a reference to a rule that is a sequence is resolved correctly
TEST_F(LinkReferencesTest, ReferenceToSequenceRule) {
    peg::Parser parser(*grammar_);
    
    // Create a rule that references a sequence rule
    auto rule = peg::ref("rule2");  // rule2 is a sequence
    
    // This should work fine
    EXPECT_NO_THROW(parser["test_rule"] = rule);
}

// Test that a reference to a rule that is a literal is resolved correctly
TEST_F(LinkReferencesTest, ReferenceToLiteralRule) {
    peg::Parser parser(*grammar_);
    
    // Create a rule that references a literal rule
    auto rule = peg::ref("rule3");  // rule3 is a literal
    
    // This should work fine
    EXPECT_NO_THROW(parser["test_rule"] = rule);
}

// Test that self-referential rule is handled (if allowed by the library)
TEST_F(LinkReferencesTest, SelfReferentialRule) {
    peg::Parser parser(*grammar_);
    
    // Create a self-referential rule
    auto rule = peg::seq({
        peg::lit("a"),
        peg::ref("self_rule")
    });
    
    // Define the rule with itself as a reference
    EXPECT_NO_THROW(parser["self_rule"] = rule);
}

// Test that circular references are handled (if allowed by the library)
TEST_F(LinkReferencesTest, CircularReference) {
    peg::Parser parser(*grammar_);
    
    // Create two rules that reference each other
    auto rule_a = peg::seq({
        peg::lit("a"),
        peg::ref("rule_b")
    });
    
    auto rule_b = peg::seq({
        peg::lit("b"),
        peg::ref("rule_a")
    });
    
    // Define both rules
    EXPECT_NO_THROW(parser["rule_a"] = rule_a);
    EXPECT_NO_THROW(parser["rule_b"] = rule_b);
}