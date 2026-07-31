#include <gtest/gtest.h>
#include <peglib.h>
#include <string>
#include <vector>

using namespace peglib;

TEST(PeglibTest, PerformCoreNormalCase) {
    std::string start_rule;
    std::string grammar_str = R"(
        Grammar = Expression
        Expression = Term (AddOp Term)*
        Term = Factor (MulOp Factor)*
        Factor = Number / "(" Expression ")"
        AddOp = "+" / "-"
        MulOp = "*" / "/"
        Number = [0-9]+
    )";
    
    auto grammar = parse(grammar_str, start_rule);
    ASSERT_NE(grammar, nullptr);
    EXPECT_EQ(start_rule, "Grammar");
}

TEST(PeglibTest, PerformCoreEmptyGrammar) {
    std::string start_rule;
    std::string grammar_str = "";
    
    auto grammar = parse(grammar_str, start_rule);
    EXPECT_EQ(grammar, nullptr);
}

TEST(PeglibTest, PerformCoreInvalidSyntax) {
    std::string start_rule;
    std::string grammar_str = R"(
        Grammar = Expression
        Expression = Term (AddOp Term)*
        Term = Factor (MulOp Factor)*
        Factor = Number / "(" Expression ")"
        AddOp = "+" / "-"
        MulOp = "*" / "/"
        Number = [0-9]+
        // Missing semicolon
        ExtraRule = "test"
    )";
    
    auto grammar = parse(grammar_str, start_rule);
    EXPECT_EQ(grammar, nullptr);
}

TEST(PeglibTest, PerformCoreDuplicateDefinition) {
    std::string start_rule;
    std::string grammar_str = R"(
        Grammar = Expression
        Expression = "test"
        Expression = "test2"
    )";
    
    auto grammar = parse(grammar_str, start_rule);
    EXPECT_EQ(grammar, nullptr);
}

TEST(PeglibTest, PerformCoreLeftRecursion) {
    std::string start_rule;
    std::string grammar_str = R"(
        Grammar = Expression
        Expression = Expression "+" Number / Number
        Number = [0-9]+
    )";
    
    auto grammar = parse(grammar_str, start_rule);
    EXPECT_EQ(grammar, nullptr);
}

TEST(PeglibTest, PerformCoreIgnoreOperatorOnStart) {
    std::string start_rule;
    std::string grammar_str = R"(
        Grammar = ~Expression
        Expression = "test"
    )";
    
    auto grammar = parse(grammar_str, start_rule);
    EXPECT_EQ(grammar, nullptr);
}

TEST(PeglibTest, PerformCoreMissingReference) {
    std::string start_rule;
    std::string grammar_str = R"(
        Grammar = Expression
        Expression = NonExistentRule
    )";
    
    auto grammar = parse(grammar_str, start_rule);
    EXPECT_EQ(grammar, nullptr);
}

TEST(PeglibTest, PerformCoreWhitespaceHandling) {
    std::string start_rule;
    std::string grammar_str = R"(
        Grammar = Expression
        Expression = "test"
        %whitespace = [ \t]+
    )";
    
    auto grammar = parse(grammar_str, start_rule);
    ASSERT_NE(grammar, nullptr);
    EXPECT_EQ(start_rule, "Grammar");
}

TEST(PeglibTest, PerformCoreWordHandling) {
    std::string start_rule;
    std::string grammar_str = R"(
        Grammar = Expression
        Expression = "test"
        %word = [a-zA-Z]+
    )";
    
    auto grammar = parse(grammar_str, start_rule);
    ASSERT_NE(grammar, nullptr);
    EXPECT_EQ(start_rule, "Grammar");
}

TEST(PeglibTest, PerformCorePrecedenceInstruction) {
    std::string start_rule;
    std::string grammar_str = R"(
        Grammar = Expression
        Expression = Term (AddOp Term)*
        Term = Factor (MulOp Factor)*
        Factor = Number / "(" Expression ")"
        AddOp = "+" / "-"
        MulOp = "*" / "/"
        Number = [0-9]+
        %precedence = left AddOp MulOp
    )";
    
    auto grammar = parse(grammar_str, start_rule);
    ASSERT_NE(grammar, nullptr);
    EXPECT_EQ(start_rule, "Grammar");
}

TEST(PeglibTest, PerformCoreComplexGrammar) {
    std::string start_rule;
    std::string grammar_str = R"(
        Grammar = Statement+
        Statement = Assignment / Expression
        Assignment = Identifier "=" Expression
        Expression = Term (AddOp Term)*
        Term = Factor (MulOp Factor)*
        Factor = Number / Identifier / "(" Expression ")"
        AddOp = "+" / "-"
        MulOp = "*" / "/"
        Number = [0-9]+
        Identifier = [a-zA-Z_][a-zA-Z0-9_]*
        %whitespace = [ \t]+
    )";
    
    auto grammar = parse(grammar_str, start_rule);
    ASSERT_NE(grammar, nullptr);
    EXPECT_EQ(start_rule, "Grammar");
}

TEST(PeglibTest, PerformCoreWithRulesOverride) {
    std::string start_rule;
    std::string grammar_str = R"(
        Grammar = Expression
        Expression = Term (AddOp Term)*
        Term = Factor (MulOp Factor)*
        Factor = Number / "(" Expression ")"
        AddOp = "+" / "-"
        MulOp = "*" / "/"
        Number = [0-9]+
    )";
    
    Rules rules;
    rules["Number"] = rule("[0-9]+");
    
    auto grammar = parse(grammar_str, start_rule, rules);
    ASSERT_NE(grammar, nullptr);
    EXPECT_EQ(start_rule, "Grammar");
}

TEST(PeglibTest, PerformCoreWithEmptyRules) {
    std::string start_rule;
    std::string grammar_str = R"(
        Grammar = Expression
        Expression = Term (AddOp Term)*
        Term = Factor (MulOp Factor)*
        Factor = Number / "(" Expression ")"
        AddOp = "+" / "-"
        MulOp = "*" / "/"
        Number = [0-9]+
    )";
    
    Rules rules;
    auto grammar = parse(grammar_str, start_rule, rules);
    ASSERT_NE(grammar, nullptr);
    EXPECT_EQ(start_rule, "Grammar");
}