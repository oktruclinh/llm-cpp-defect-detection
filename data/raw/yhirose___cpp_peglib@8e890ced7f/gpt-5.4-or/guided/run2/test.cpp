#include <gtest/gtest.h>
#include <peglib.h>

#include <memory>
#include <string>
#include <vector>

namespace {
using peg::Ast;
using peg::parser;

std::shared_ptr<Ast> parse_with_ast(parser& p, const std::string& input) {
  std::shared_ptr<Ast> ast;
  EXPECT_TRUE(p.parse(input, ast)) << "parse failed for input: " << input;
  return ast;
}

TEST(PeglibEnableAstTest, GeneratesAstForSimpleGrammarWithoutUserActions) {
  parser p(R"(
    Start <- 'a' 'b' 'c'
  )");

  p.enable_ast();

  auto ast = parse_with_ast(p, "abc");
  ASSERT_TRUE(ast);

  EXPECT_EQ(ast->name, "Start");
  EXPECT_EQ(ast->token, "abc");
  EXPECT_FALSE(ast->nodes.empty());
}

TEST(PeglibEnableAstTest, DoesNotOverrideExistingUserActionOnRule) {
  parser p(R"(
    Start <- Item
    Item  <- 'a'+
  )");

  bool custom_action_called = false;
  p["Item"] = [&](const peg::SemanticValues& sv) {
    custom_action_called = true;
    return std::string("custom");
  };

  p.enable_ast();

  std::shared_ptr<Ast> ast;
  ASSERT_TRUE(p.parse("aaa", ast));
  ASSERT_TRUE(ast);

  EXPECT_TRUE(custom_action_called);

  // Start should still get AST because it had no action.
  EXPECT_EQ(ast->name, "Start");

  // Item had a user action before enable_ast(), so enable_ast() must not
  // replace it with AST generation. Therefore Start's child list should not
  // contain an AST node for Item.
  EXPECT_TRUE(ast->nodes.empty());
}

TEST(PeglibEnableAstTest, ExistingActionSetBeforeEnableAstPreventsAstInsertionButOthersStillGetAst) {
  parser p(R"(
    Start <- Left Right
    Left  <- 'x'
    Right <- 'y'
  )");

  p["Left"] = [](const peg::SemanticValues&) { return 42; };

  p.enable_ast();

  auto ast = parse_with_ast(p, "xy");
  ASSERT_TRUE(ast);
  EXPECT_EQ(ast->name, "Start");

  // Left has a custom action, so only Right should contribute an AST child.
  ASSERT_EQ(ast->nodes.size(), 1u);
  ASSERT_TRUE(ast->nodes[0]);
  EXPECT_EQ(ast->nodes[0]->name, "Right");
  EXPECT_EQ(ast->nodes[0]->token, "y");
}

TEST(PeglibEnableAstTest, RulesWithActionsAddedAfterEnableAstCanOverrideAstBehavior) {
  parser p(R"(
    Start <- Left Right
    Left  <- 'x'
    Right <- 'y'
  )");

  p.enable_ast();

  // Override one rule after enable_ast() using the public API.
  p["Right"] = [](const peg::SemanticValues&) { return std::string("override"); };

  auto ast = parse_with_ast(p, "xy");
  ASSERT_TRUE(ast);
  EXPECT_EQ(ast->name, "Start");

  // Left should still appear as AST child, Right should no longer contribute one.
  ASSERT_EQ(ast->nodes.size(), 1u);
  ASSERT_TRUE(ast->nodes[0]);
  EXPECT_EQ(ast->nodes[0]->name, "Left");
  EXPECT_EQ(ast->nodes[0]->token, "x");
}

TEST(PeglibEnableAstTest, EnableAstIsChainableAndIdempotentForParsingBehavior) {
  parser p(R"(
    Start <- A B
    A <- 'a'
    B <- 'b'
  )");

  parser& ref = p.enable_ast();
  EXPECT_EQ(&ref, &p);

  auto ast1 = parse_with_ast(p, "ab");
  ASSERT_TRUE(ast1);
  ASSERT_EQ(ast1->nodes.size(), 2u);
  EXPECT_EQ(ast1->nodes[0]->name, "A");
  EXPECT_EQ(ast1->nodes[1]->name, "B");

  // Calling enable_ast again should not break existing behavior.
  p.enable_ast();

  auto ast2 = parse_with_ast(p, "ab");
  ASSERT_TRUE(ast2);
  ASSERT_EQ(ast2->nodes.size(), 2u);
  EXPECT_EQ(ast2->nodes[0]->name, "A");
  EXPECT_EQ(ast2->nodes[1]->name, "B");
}

TEST(PeglibEnableAstTest, EmptyMatchGrammarStillProducesAst) {
  parser p(R"(
    Start <- ''
  )");

  p.enable_ast();

  auto ast = parse_with_ast(p, "");
  ASSERT_TRUE(ast);
  EXPECT_EQ(ast->name, "Start");
  EXPECT_EQ(ast->token, "");
}

TEST(PeglibEnableAstTest, ParseFailureDoesNotProduceSuccessfulAstResult) {
  parser p(R"(
    Start <- 'a'
  )");

  p.enable_ast();

  std::shared_ptr<Ast> ast;
  EXPECT_FALSE(p.parse("b", ast));
}

TEST(PeglibEnableAstTest, CustomAstTypeIsSupportedThroughPublicApi) {
  struct MyAst : Ast {
    int marker = 123;
  };

  parser p(R"(
    Start <- 'z'
  )");

  p.enable_ast<MyAst>();

  std::shared_ptr<MyAst> ast;
  ASSERT_TRUE(p.parse("z", ast));
  ASSERT_TRUE(ast);
  EXPECT_EQ(ast->name, "Start");
  EXPECT_EQ(ast->token, "z");
  EXPECT_EQ(ast->marker, 123);
}

TEST(PeglibEnableAstTest, RecursiveGrammarProducesNestedAstStructure) {
  parser p(R"(
    Expr   <- Term (('+' / '-') Term)*
    Term   <- Factor (('*' / '/') Factor)*
    Factor <- Number / '(' Expr ')'
    Number <- < [0-9]+ >
  )");

  p.enable_ast();

  auto ast = parse_with_ast(p, "1+(2*3)");
  ASSERT_TRUE(ast);
  EXPECT_EQ(ast->name, "Expr");
  EXPECT_FALSE(ast->token.empty());

  // The tree should contain nested nodes from subrules when AST actions are enabled.
  bool found_term = false;
  bool found_factor = false;
  bool found_number = false;

  std::vector<std::shared_ptr<Ast>> stack{ast};
  while (!stack.empty()) {
    auto node = stack.back();
    stack.pop_back();
    if (!node) continue;

    if (node->name == "Term") found_term = true;
    if (node->name == "Factor") found_factor = true;
    if (node->name == "Number") found_number = true;

    for (auto& child : node->nodes) {
      stack.push_back(child);
    }
  }

  EXPECT_TRUE(found_term);
  EXPECT_TRUE(found_factor);
  EXPECT_TRUE(found_number);
}

}  // namespace