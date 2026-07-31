#include <gtest/gtest.h>
#include <peglib.h>

// Helper function to create a simple grammar for testing
std::shared_ptr<peg::Grammar> create_test_grammar() {
    auto g = std::make_shared<peg::Grammar>();
    g->set_definition("A", "a");
    g->set_definition("B", "b");
    g->set_definition("C", "c");
    return g;
}

// Test fixture for LinkReferences
class LinkReferencesTest : public ::testing::Test {
protected:
    void SetUp() override {
        grammar_ = create_test_grammar();
    }

    std::shared_ptr<peg::Grammar> grammar_;
};

TEST_F(LinkReferencesTest, LinksToDefinitionRule) {
    peg::Reference ref("A");
    peg::LinkReferences linker(*grammar_, {}); // No parameters

    ref.accept(linker);

    // Expect the reference to be linked to the "A" rule
    ASSERT_NE(ref.rule_, nullptr);
    ASSERT_EQ(ref.rule_->name, "A");
    ASSERT_EQ(ref.iarg_, -1); // Not a parameter
}

TEST_F(LinkReferencesTest, LinksToMacroParameter) {
    peg::Reference ref("param1");
    peg::LinkReferences linker(*grammar_, {"param1", "param2"}); // With parameters

    ref.accept(linker);

    // Expect the reference to be linked to the first parameter
    ASSERT_EQ(ref.rule_, nullptr); // Not a rule
    ASSERT_EQ(ref.iarg_, 0);       // Index of "param1"
}

TEST_F(LinkReferencesTest, LinksToSecondMacroParameter) {
    peg::Reference ref("param2");
    peg::LinkReferences linker(*grammar_, {"param1", "param2"}); // With parameters

    ref.accept(linker);

    // Expect the reference to be linked to the second parameter
    ASSERT_EQ(ref.rule_, nullptr); // Not a rule
    ASSERT_EQ(ref.iarg_, 1);       // Index of "param2"
}

TEST_F(LinkReferencesTest, NotLinkedIfNeitherRuleNorParameter) {
    peg::Reference ref("NonExistent");
    peg::LinkReferences linker(*grammar_, {"param1"});

    ref.accept(linker);

    // Expect no link to rule or parameter
    ASSERT_EQ(ref.rule_, nullptr);
    ASSERT_EQ(ref.iarg_, -1);
}

TEST_F(LinkReferencesTest, PrioritizesMacroParameterOverRuleWithSameName) {
    // Create a rule named "param1"
    grammar_->set_definition("param1", "x");

    peg::Reference ref("param1");
    peg::LinkReferences linker(*grammar_, {"param1"}); // "param1" is also a parameter

    ref.accept(linker);

    // Expect it to be linked as a parameter, not a rule
    ASSERT_EQ(ref.rule_, nullptr);
    ASSERT_EQ(ref.iarg_, 0);
}

TEST_F(LinkReferencesTest, HandlesReferenceWithArguments) {
    // Create a reference with nested arguments
    peg::Reference arg_ref("B");
    peg::Reference main_ref("A", {std::make_shared<peg::Reference>(arg_ref)});
    peg::LinkReferences linker(*grammar_, {});

    main_ref.accept(linker);

    // Expect both the main reference and its argument to be linked
    ASSERT_NE(main_ref.rule_, nullptr);
    ASSERT_EQ(main_ref.rule_->name, "A");
    ASSERT_EQ(main_ref.iarg_, -1);

    ASSERT_NE(main_ref.args_[0]->rule_, nullptr);
    ASSERT_EQ(main_ref.args_[0]->rule_->name, "B");
    ASSERT_EQ(main_ref.args_[0]->iarg_, -1);
}

TEST_F(LinkReferencesTest, HandlesReferenceWithMultipleArguments) {
    peg::Reference arg1_ref("B");
    peg::Reference arg2_ref("C");
    peg::Reference main_ref("A", {std::make_shared<peg::Reference>(arg1_ref), std::make_shared<peg::Reference>(arg2_ref)});
    peg::LinkReferences linker(*grammar_, {});

    main_ref.accept(linker);

    // Expect all references to be linked
    ASSERT_NE(main_ref.rule_, nullptr);
    ASSERT_EQ(main_ref.rule_->name, "A");

    ASSERT_NE(main_ref.args_[0]->rule_, nullptr);
    ASSERT_EQ(main_ref.args_[0]->rule_->name, "B");

    ASSERT_NE(main_ref.args_[1]->rule_, nullptr);
    ASSERT_EQ(main_ref.args_[1]->rule_->name, "C");
}

TEST_F(LinkReferencesTest, EmptyGrammar) {
    peg::Grammar empty_grammar;
    peg::Reference ref("A");
    peg::LinkReferences linker(empty_grammar, {});

    ref.accept(linker);

    // Should not link to anything
    ASSERT_EQ(ref.rule_, nullptr);
    ASSERT_EQ(ref.iarg_, -1);
}

TEST_F(LinkReferencesTest, EmptyParametersList) {
    peg::Reference ref("A");
    peg::LinkReferences linker(*grammar_, {}); // Empty parameters

    ref.accept(linker);

    // Should link to rule "A"
    ASSERT_NE(ref.rule_, nullptr);
    ASSERT_EQ(ref.rule_->name, "A");
    ASSERT_EQ(ref.iarg_, -1);
}

TEST_F(LinkReferencesTest, ReferenceToNonExistentRuleWithParameters) {
    peg::Reference ref("NonExistent");
    peg::LinkReferences linker(*grammar_, {"param1"});

    ref.accept(linker);

    // Should not link to anything
    ASSERT_EQ(ref.rule_, nullptr);
    ASSERT_EQ(ref.iarg_, -1);
}

TEST_F(LinkReferencesTest, ReferenceToNonExistentParameterWithRules) {
    peg::Reference ref("NonExistentParam");
    peg::LinkReferences linker(*grammar_, {"param1"});

    ref.accept(linker);

    // Should not link to anything
    ASSERT_EQ(ref.rule_, nullptr);
    ASSERT_EQ(ref.iarg_, -1);
}

TEST_F(LinkReferencesTest, DeeplyNestedReferences) {
    // A -> B(C)
    peg::Reference ref_c("C");
    peg::Reference ref_b("B", {std::make_shared<peg::Reference>(ref_c)});
    peg::Reference ref_a("A", {std::make_shared<peg::Reference>(ref_b)});

    peg::LinkReferences linker(*grammar_, {});

    ref_a.accept(linker);

    ASSERT_NE(ref_a.rule_, nullptr);
    ASSERT_EQ(ref_a.rule_->name, "A");

    ASSERT_NE(ref_a.args_[0]->rule_, nullptr);
    ASSERT_EQ(ref_a.args_[0]->rule_->name, "B");

    ASSERT_NE(ref_a.args_[0]->args_[0]->rule_, nullptr);
    ASSERT_EQ(ref_a.args_[0]->args_[0]->rule_->name, "C");
}