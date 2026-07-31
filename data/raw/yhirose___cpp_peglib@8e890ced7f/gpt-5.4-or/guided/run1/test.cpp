#include <gtest/gtest.h>
#include <peglib.h>

#include <memory>
#include <string>
#include <vector>

namespace {

using peg::Ast;
using peg::parser;

TEST(EnableAstTest, BuildsAstForSimpleGrammarWithoutExplicitActions) {
  parser pg(R"(
    Expr    <- Term (('+' / '-') Term)*
    Term    <- Factor (('*' / '/') Factor)*
    Factor  <- Number / '(' Expr ')'
    Number  <- < [0-9]+ >
    %whitespace <- [ \t\r\n]*
  )");

  pg.enable_ast();

  std::shared_ptr<Ast> ast;
  ASSERT_TRUE(pg.parse("1 + 2 * 3", ast));
  ASSERT_NE(ast, nullptr);

  EXPECT_EQ(ast->name, "Expr");
  ASSERT_FALSE(ast->nodes.empty());

  bool found_number_capture = false;
  std::vector<std::shared_ptr<Ast>> stack{ast};
  while (!stack.empty()) {
    auto node = stack.back();
    stack.pop_back();

    if (node->name == "Number") {
      found_number_capture = true;
      EXPECT_EQ(node->token, "1");
      break;
    }

    for (auto &child : node->nodes) {
      stack.push_back(child);
    }
  }

  EXPECT_TRUE(found_number_capture);
}

TEST(EnableAstTest, ExistingSemanticActionIsPreservedAndAstAddedToOtherRules) {
  parser pg(R"(
    Start  <- Word
    Word   <- < [a-z]+ >
  )");

  bool action_called = false;
  pg["Word"] = [&](const peg::SemanticValues &sv) {
    action_called = true;
    return sv.token_to_string();
  };

  pg.enable_ast();

  std::shared_ptr<Ast> ast;
  ASSERT_TRUE(pg.parse("hello", ast));
  EXPECT_TRUE(action_called);
  ASSERT_NE(ast, nullptr);

  EXPECT_EQ(ast->name, "Start");
  ASSERT_EQ(ast->nodes.size(), 1u);
  ASSERT_NE(ast->nodes[0], nullptr);
  EXPECT_EQ(ast->nodes[0]->name, "Word");
  EXPECT_EQ(ast->nodes[0]->token, "hello");
}

TEST(EnableAstTest, CanBeCalledMultipleTimesWithoutBreakingParsing) {
  parser pg(R"(
    Start <- Item*
    Item  <- < [a-z] >
  )");

  pg.enable_ast();
  pg.enable_ast();
  pg.enable_ast();

  std::shared_ptr<Ast> ast;
  ASSERT_TRUE(pg.parse("abc", ast));
  ASSERT_NE(ast, nullptr);
  EXPECT_EQ(ast->name, "Start");

  size_t item_count = 0;
  for (const auto &child : ast->nodes) {
    ASSERT_NE(child, nullptr);
    if (child->name == "Item") {
      item_count++;
    }
  }
  EXPECT_EQ(item_count, 3u);
}

TEST(EnableAstTest, ParseFailureDoesNotProduceAst) {
  parser pg(R"(
    Start <- < [0-9]+ >
  )");

  pg.enable_ast();

  std::shared_ptr<Ast> ast = std::make_shared<Ast>();
  EXPECT_FALSE(pg.parse("abc", ast));
}

TEST(EnableAstTest, EmptyInputForZeroOrMoreStillProducesAstRoot) {
  parser pg(R"(
    Start <- Item*
    Item  <- < [a-z] >
  )");

  pg.enable_ast();

  std::shared_ptr<Ast> ast;
  ASSERT_TRUE(pg.parse("", ast));
  ASSERT_NE(ast, nullptr);
  EXPECT_EQ(ast->name, "Start");
  EXPECT_TRUE(ast->nodes.empty());
}

TEST(EnableAstTest, EnableAstWithCustomNodeTypeProducesDerivedAstInstances) {
  struct MyAst : Ast {
    int marker = 123;
  };

  parser pg(R"(
    Start <- Word
    Word  <- < [a-z]+ >
  )");

  pg.enable_ast<MyAst>();

  std::shared_ptr<MyAst> ast;
  ASSERT_TRUE(pg.parse("test", ast));
  ASSERT_NE(ast, nullptr);
  EXPECT_EQ(ast->name, "Start");
  EXPECT_EQ(ast->marker, 123);

  ASSERT_EQ(ast->nodes.size(), 1u);
  auto child = std::dynamic_pointer_cast<MyAst>(ast->nodes[0]);
  ASSERT_NE(child, nullptr);
  EXPECT_EQ(child->name, "Word");
  EXPECT_EQ(child->token, "test");
  EXPECT_EQ(child->marker, 123);
}

TEST(EnableAstTest, ReturnValueAllowsChainingAndReferencesSameParser) {
  parser pg(R"(
    Start <- < [a-z]+ >
  )");

  parser *returned = &pg.enable_ast<>();
  EXPECT_EQ(returned, &pg);

  std::shared_ptr<Ast> ast;
  ASSERT_TRUE(returned->parse("abc", ast));
  ASSERT_NE(ast, nullptr);
  EXPECT_EQ(ast->name, "Start");
  EXPECT_EQ(ast->token, "abc");
}

}  // namespace