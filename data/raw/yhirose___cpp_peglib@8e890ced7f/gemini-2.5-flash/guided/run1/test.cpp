```cpp
#include <gtest/gtest.h>
#include <peglib.h>

// Define a custom AST node type for testing
struct CustomAstNode {
    std::string name;
    std::vector<CustomAstNode> children;
};

// Custom AST builder function
std::shared_ptr<CustomAstNode> custom_ast_builder(const peg::Ast& ast) {
    auto node = std::make_shared<CustomAstNode>();
    node->name = ast.name;
    for (const auto& child : ast.nodes) {
        node->children.push_back(*custom_ast_builder(*child));
    }
    return node;
}

TEST(EnableAstTest, BasicGrammarWithDefaultAst) {
    peg::parser parser(R"(
        ROOT <- A B
        A <- 'a'
        B <- 'b'
    )");

    ASSERT_TRUE(parser.valid());

    // Enable AST generation with default Ast type
    parser.enable_ast();

    std::shared_ptr<peg::Ast> ast;
    bool ret = parser.parse("ab", ast);

    ASSERT_TRUE(ret);
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->name, "ROOT");
    ASSERT_EQ(ast->nodes.size(), 2);
    ASSERT_EQ(ast->nodes[0]->name, "A");
    ASSERT_EQ(ast->nodes[0]->token, "a");
    ASSERT_EQ(ast->nodes[1]->name, "B");
    ASSERT_EQ(ast->nodes[1]->token, "b");
}

TEST(EnableAstTest, GrammarWithExistingActionShouldNotBeOverwritten) {
    peg::parser parser(R"(
        ROOT <- A B
        A <- 'a'
        B <- 'b'
    )");

    ASSERT_TRUE(parser.valid());

    // Add an action to rule A
    parser["A"] = [](const peg::SemanticValues& sv) {
        return "RuleA_Action";
    };

    // Enable AST generation. Rule A's action should not be replaced.
    parser.enable_ast();

    // Parse and check if the action for A is still present
    std::shared_ptr<peg::Ast> ast;
    auto ret = parser.parse("ab", ast);

    ASSERT_TRUE(ret);
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->name, "ROOT");
    ASSERT_EQ(ast->nodes.size(), 2);

    // The action for 'A' should have been executed, so 'A' should not appear as an AST node
    // in the default AST generation if an action is present.
    // Instead, the result of the action would be used if the parent rule consumes it.
    // However, enable_ast() specifically adds an AST action if no action is present.
    // If an action IS present, enable_ast() should NOT overwrite it.
    // The default behavior of peglib is that if a rule has an action, it doesn't create an AST node for it
    // unless the action explicitly returns an Ast object or the parent rule explicitly requests it.
    // Here, we are testing that enable_ast() doesn't *add* an AST action to 'A'.
    // So, 'A' should not appear as a direct child node in the AST if its action was triggered.
    // Let's re-evaluate the expected AST structure.
    // If 'A' has an action, and 'enable_ast()' is called, 'enable_ast()' should *not* add an AST action to 'A'.
    // Thus, when parsing, 'A' will execute its action and its result will be passed up.
    // The default AST builder for 'ROOT' will then see the result of 'A' and 'B'.
    // If 'A' returns a string, it won't be an Ast node.
    // This test needs to verify that 'A' does *not* get an Ast action added.
    // The simplest way to check this is to see if 'A' appears as an Ast node.
    // If enable_ast() *had* overwritten the action, 'A' would appear as an Ast node.

    // Let's verify that the action for 'A' was indeed called by checking the return value
    // if we were to use `parse_and_get_value`.
    // For `parse` with `std::shared_ptr<peg::Ast>`, if a rule has an action,
    // that rule's node is typically not included in the AST unless the action
    // explicitly returns an `Ast` object.
    // So, if 'A' has an action, it should NOT appear as a child of ROOT in the AST.
    // 'B' should still appear.

    // The current implementation of peglib's `enable_ast` adds an AST action *only if* `rule.action` is null.
    // So, if an action is already present, it should be preserved, and no AST action should be added.
    // This means 'A' should not be an AST node.

    ASSERT_EQ(ast->name, "ROOT");
    // ROOT should have only one child, 'B', because 'A' had an action.
    // This is the expected behavior when a rule has an action and enable_ast() is called.
    // enable_ast() doesn't override existing actions.
    ASSERT_EQ(ast->nodes.size(), 1); // Only 'B' should be an AST node
    ASSERT_EQ(ast->nodes[0]->name, "B");
    ASSERT_EQ(ast->nodes[0]->token, "b");
}


TEST(EnableAstTest, EmptyGrammar) {
    peg::parser parser("");
    ASSERT_TRUE(parser.valid()); // An empty grammar is valid but won't parse anything.

    // Calling enable_ast on an empty grammar should not crash and should return *this.
    parser.enable_ast();

    std::shared_ptr<peg::Ast> ast;
    bool ret = parser.parse("", ast);

    // Parsing an empty string with an empty grammar will likely fail or return no AST.
    // The important part is that enable_ast() didn't crash.
    ASSERT_FALSE(ret); // Empty grammar cannot parse anything meaningful.
    ASSERT_EQ(ast, nullptr); // No AST should be generated.
}

TEST(EnableAstTest, GrammarWithNoRules) {
    peg::parser parser(" "); // Grammar with only whitespace
    ASSERT_TRUE(parser.valid());

    parser.enable_ast();

    std::shared_ptr<peg::Ast> ast;
    bool ret = parser.parse("a", ast);

    ASSERT_FALSE(ret);
    ASSERT_EQ(ast, nullptr);
}

TEST(EnableAstTest, GrammarWithOnlyTerminalRules) {
    peg::parser parser(R"(
        ROOT <- 'a' 'b'
    )");
    ASSERT_TRUE(parser.valid());

    parser.enable_ast();

    std::shared_ptr<peg::Ast> ast;
    bool ret = parser.parse("ab", ast);

    ASSERT_TRUE(ret);
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->name, "ROOT");
    ASSERT_EQ(ast->nodes.size(), 0); // Terminal rules don't create child AST nodes by default
    ASSERT_EQ(ast->token, "ab"); // The token for ROOT should be the matched string
}

TEST(EnableAstTest, GrammarWithRecursiveRules) {
    peg::parser parser(R"(
        EXPR <- TERM (('+' / '-') TERM)*
        TERM <- FACT (('*' / '/') FACT)*
        FACT <- NUMBER / '(' EXPR ')'
        NUMBER <- [0-9]+
    )");
    ASSERT_TRUE(parser.valid());

    parser.enable_ast();

    std::shared_ptr<peg::Ast> ast;
    bool ret = parser.parse("1+(2*3)", ast);

    ASSERT_TRUE(ret);
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->name, "EXPR");
    ASSERT_EQ(ast->nodes.size(), 3); // TERM, '+', TERM
    ASSERT_EQ(ast->nodes[0]->name, "TERM");
    ASSERT_EQ(ast->nodes[0]->nodes[0]->name, "FACT");
    ASSERT_EQ(ast->nodes[0]->nodes[0]->nodes[0]->name, "NUMBER");
    ASSERT_EQ(ast->nodes[0]->nodes[0]->nodes[0]->token, "1");

    ASSERT_EQ(ast->nodes[1]->token, "+");

    ASSERT_EQ(ast->nodes[2]->name, "TERM");
    ASSERT_EQ(ast->nodes[2]->nodes.size(), 3); // FACT, '*', FACT
    ASSERT_EQ(ast->nodes[2]->nodes[0]->name, "FACT");
    ASSERT_EQ(ast->nodes[2]->nodes[0]->nodes[0]->name, "EXPR"); // (EXPR)
    ASSERT_EQ(ast->nodes[2]->nodes[0]->nodes[0]->nodes[0]->name, "TERM");
    ASSERT_EQ(ast->nodes[2]->nodes[0]->nodes[0]->nodes[0]->nodes[0]->name, "FACT");
    ASSERT_EQ(ast->nodes[2]->nodes[0]->nodes[0]->nodes[0]->nodes[0]->nodes[0]->name, "NUMBER");
    ASSERT_EQ(ast->nodes[2]->nodes[0]->nodes[0]->nodes[0]->nodes[0]->nodes[0]->token, "2");

    ASSERT_EQ(ast->nodes[2]->nodes[1]->token, "*");

    ASSERT_EQ(ast->nodes[2]->nodes[2]->name, "FACT");
    ASSERT_EQ(ast->nodes[2]->nodes[2]->nodes[0]->name, "NUMBER");
    ASSERT_EQ(ast->nodes[2]->nodes[2]->nodes[0]->token, "3");
}

TEST(EnableAstTest, CustomAstType) {
    peg::parser parser(R"(
        ROOT <- A B
        A <- 'a'
        B <- 'b'
    )");

    ASSERT_TRUE(parser.valid());

    // Enable AST generation with a custom Ast type
    parser.enable_ast<CustomAstNode>();

    // Set a custom AST builder
    parser.set_ast_builder(custom_ast_builder);

    std::shared_ptr<CustomAstNode> custom_ast;
    bool ret = parser.parse("ab", custom_ast);

    ASSERT_TRUE(ret);
    ASSERT_NE(custom_ast, nullptr);
    ASSERT_EQ(custom_ast->name, "ROOT");
    ASSERT_EQ(custom_ast->children.size(), 2);
    ASSERT_EQ(custom_ast->children[0].name, "A");
    ASSERT_EQ(custom_ast->children[0].children.size(), 0); // 'a' is a terminal, no children
    ASSERT_EQ(custom_ast->children[1].name, "B");
    ASSERT_EQ(custom_ast->children[1].children.size(), 0); // 'b' is a terminal, no children
}

TEST(EnableAstTest, CustomAstTypeWithoutCustomBuilder) {
    peg::parser parser(R"(
        ROOT <- A B
        A <- 'a'
        B <- 'b'
    )");

    ASSERT_TRUE(parser.valid());

    // Enable AST generation with a custom Ast type, but don't set a custom builder.
    // This should still work, using the default builder for the custom type.
    // The default builder for a custom type `T` expects `T` to be convertible from `peg::Ast`.
    // Since CustomAstNode is not directly convertible from peg::Ast, this will likely fail
    // or produce an empty AST if the default builder for `T` is used without a conversion.
    // The `enable_ast<T>()` function itself only sets the `add_ast_action<T>(rule)`.
    // The `add_ast_action<T>` template function will create an `Ast` object and then
    // attempt to convert it to `T`. If `T` doesn't have a constructor from `Ast`, it will fail.
    // For this test, we expect it to work if `T` can be default constructed and then populated,
    // or if `T` has a constructor from `peg::Ast`.
    // Let's assume `CustomAstNode` can be constructed from `peg::Ast` for this test to pass.
    // If not, this test would need to be adjusted or removed.
    // For `peglib`, the `add_ast_action<T>` creates a `peg::Ast` and then `std::make_shared<T>(ast_node)`.
    // This implies `T` needs a constructor that takes `const peg::Ast&`.
    // Let's modify CustomAstNode to have such a constructor for this test.

    // Re-define CustomAstNode with a constructor from peg::Ast
    struct CustomAstNodeWithCtor {
        std::string name;
        std::string token;
        std::vector<std::shared_ptr<CustomAstNodeWithCtor>> children;

        CustomAstNodeWithCtor() = default; // Default constructor needed for some cases

        // Constructor from peg::Ast
        explicit CustomAstNodeWithCtor(const peg::Ast& ast)
            : name(ast.name), token(ast.token) {
            for (const auto& child_ast : ast.nodes) {
                children.push_back(std::make_shared<CustomAstNodeWithCtor>(*child_ast));
            }
        }
    };

    peg::parser parser2(R"(
        ROOT <- A B
        A <- 'a'
        B <- 'b'
    )");
    ASSERT_TRUE(parser2.valid());

    parser2.enable_ast<CustomAstNodeWithCtor>();

    std::shared_ptr<CustomAstNodeWithCtor> custom_ast_with_ctor;
    bool ret = parser2.parse("ab", custom_ast_with_ctor);

    ASSERT_TRUE(ret);
    ASSERT_NE(custom_ast_with_ctor, nullptr);
    ASSERT_EQ(custom_ast_with_ctor->name, "ROOT");
    ASSERT_EQ(custom_ast_with_ctor->token, "ab");
    ASSERT_EQ(custom_ast_with_ctor->children.size(), 2);
    ASSERT_EQ(custom_ast_with_ctor->children[0]->name, "A");
    ASSERT_EQ(custom_ast_with_ctor->children[0]->token, "a");
    ASSERT_EQ(custom_ast_with_ctor->children[1]->name, "B");
    ASSERT_EQ(custom_ast_with_ctor->children[1]->token, "b");
}

TEST(EnableAstTest, GrammarWithNoMatch) {
    peg::parser parser(R"(
        ROOT <- 'a'
    )");
    ASSERT_TRUE(parser.valid());

    parser.enable_ast();

    std::shared_ptr<peg::Ast> ast;
    bool ret = parser.parse("b", ast); // Input does not match grammar

    ASSERT_FALSE(ret);
    ASSERT_EQ(ast, nullptr); // No AST should be generated on parse failure
}

TEST(EnableAstTest, EnableAstMultipleTimes) {
    peg::parser parser(R"(
        ROOT <- A
        A <- 'a'
    )");
    ASSERT_TRUE(parser.valid());

    // Enable AST once
    parser.enable_ast();

    std::shared_ptr<peg::Ast> ast1;
    ASSERT_TRUE(parser.parse("a", ast1));
    ASSERT_NE(ast1, nullptr);
    ASSERT_EQ(ast1->name, "ROOT");
    ASSERT_EQ(ast1->nodes.size(), 1);
    ASSERT_EQ(ast1->nodes[0]->name, "A");

    // Enable AST again (should not change behavior or cause issues)
    parser.enable_ast();

    std::shared_ptr<peg::Ast> ast2;
    ASSERT_TRUE(parser.parse("a", ast2));
    ASSERT_NE(ast2, nullptr);
    ASSERT_EQ(ast2->name, "ROOT");
    ASSERT_EQ(ast2->nodes.size(), 1);
    ASSERT_EQ(ast2->nodes[0]->name, "A");

    // Enable AST with a different type (should reconfigure actions)
    struct AnotherCustomAstNode {
        std::string id;
        explicit AnotherCustomAstNode(const peg::Ast& ast) : id(ast.name + "_custom") {}
    };
    parser.enable_ast<AnotherCustomAstNode>();

    std::shared_ptr<AnotherCustomAstNode> custom_ast;
    ASSERT_TRUE(parser.parse("a", custom_ast));
    ASSERT_NE(custom_ast, nullptr);
    ASSERT_EQ(custom_ast->id, "ROOT_custom");
}

TEST(EnableAstTest, GrammarWithSkippedRules) {
    peg::parser parser(R"(
        ROOT <- A B
        ~A <- 'a'
        B <- 'b'
    )");
    ASSERT_TRUE(parser.valid());

    parser.enable_ast();

    std::shared_ptr<peg::Ast> ast;
    bool ret = parser.parse("ab", ast);

    ASSERT_TRUE(ret);
    ASSERT_NE(ast, nullptr);
    ASSERT_EQ(ast->