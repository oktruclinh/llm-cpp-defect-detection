#include <gtest/gtest.h>
#include "peglib.h"

TEST(LinkReferencesTest, VisitMacroParameter) {
    peglib::grammar grammar;
    peglib::LinkReferences linker(grammar);
    
    // Create a reference that matches a macro parameter
    peglib::Reference ref("param1");
    std::vector<std::string> params = {"param1", "param2"};
    linker.set_params(params);
    
    // The reference should be linked to the parameter
    linker.visit(ref);
    
    // Verify that the reference was linked to the parameter
    EXPECT_TRUE(ref.iarg_ >= 0);
    EXPECT_EQ(ref.iarg_, 0);  // First parameter
    EXPECT_EQ(ref.rule_, nullptr);  // Should not be linked to a rule
}

TEST(LinkReferencesTest, VisitDefinitionRule) {
    peglib::grammar grammar;
    
    // Create a rule in the grammar
    auto rule = std::make_shared<peglib::Rule>();
    grammar["rule1"] = rule;
    
    peglib::LinkReferences linker(grammar);
    
    // Create a reference that matches a definition rule
    peglib::Reference ref("rule1");
    
    // The reference should be linked to the rule
    linker.visit(ref);
    
    // Verify that the reference was linked to the rule
    EXPECT_EQ(ref.rule_, rule.get());
    EXPECT_EQ(ref.iarg_, -1);  // Should not be linked to a parameter
}

TEST(LinkReferencesTest, VisitNonExistentReference) {
    peglib::grammar grammar;
    peglib::LinkReferences linker(grammar);
    
    // Create a reference that doesn't match any parameter or rule
    peglib::Reference ref("nonexistent");
    std::vector<std::string> params = {"param1", "param2"};
    linker.set_params(params);
    
    // The reference should not be linked to anything
    linker.visit(ref);
    
    // Verify that the reference was not linked
    EXPECT_EQ(ref.iarg_, -1);  // Not linked to parameter
    EXPECT_EQ(ref.rule_, nullptr);  // Not linked to rule
}

TEST(LinkReferencesTest, VisitReferenceWithArgs) {
    peglib::grammar grammar;
    
    // Create a rule in the grammar
    auto rule = std::make_shared<peglib::Rule>();
    grammar["rule1"] = rule;
    
    peglib::LinkReferences linker(grammar);
    
    // Create a reference with arguments
    peglib::Reference ref("rule1");
    auto arg1 = std::make_shared<peglib::Reference>("arg1");
    auto arg2 = std::make_shared<peglib::Reference>("arg2");
    ref.args_ = {arg1, arg2};
    
    // The reference and its arguments should be processed
    linker.visit(ref);
    
    // Verify that the reference was linked to the rule
    EXPECT_EQ(ref.rule_, rule.get());
    // Verify that arguments were visited (they should be processed)
    EXPECT_EQ(arg1->iarg_, -1);
    EXPECT_EQ(arg2->iarg_, -1);
}

TEST(LinkReferencesTest, VisitEmptyParamsAndRules) {
    peglib::grammar grammar;
    peglib::LinkReferences linker(grammar);
    
    // Create a reference with no parameters or rules
    peglib::Reference ref("anything");
    
    // Should not crash
    linker.visit(ref);
    
    // Verify that nothing was linked
    EXPECT_EQ(ref.iarg_, -1);
    EXPECT_EQ(ref.rule_, nullptr);
}

TEST(LinkReferencesTest, VisitParameterPriority) {
    peglib::grammar grammar;
    
    // Create a rule with the same name as a parameter
    auto rule = std::make_shared<peglib::Rule>();
    grammar["param1"] = rule;
    
    peglib::LinkReferences linker(grammar);
    
    // Create a reference that matches both a parameter and a rule
    peglib::Reference ref("param1");
    std::vector<std::string> params = {"param1", "param2"};
    linker.set_params(params);
    
    // Parameter should take precedence over rule
    linker.visit(ref);
    
    // Verify that the reference was linked to the parameter, not the rule
    EXPECT_TRUE(ref.iarg_ >= 0);
    EXPECT_EQ(ref.iarg_, 0);  // Linked to parameter
    EXPECT_EQ(ref.rule_, nullptr);  // Not linked to rule
}