#include <gtest/gtest.h>
#include <peglib.h>

#include <string>

namespace {

using peg::parser;

// This test verifies that a reference matching a macro parameter is treated
// as a macro argument placeholder rather than as a grammar rule reference.
// If LinkReferences::visit fails to recognize macro parameters, the parser
// construction should fail because the generated parser would try to resolve
// the parameter name as a normal rule.
TEST(PeglibLinkReferencesTest, MacroParameterReferenceIsResolvedInsideMacro) {
  parser pg(R"(
    List(x) <- x (',' x)*
    Start <- List('a')
  )");

  ASSERT_TRUE(static_cast<bool>(pg));

  std::string out;
  ASSERT_TRUE(pg.parse("a,a,a", out));
  EXPECT_EQ("a,a,a", out);

  EXPECT_FALSE(pg.parse("b", out));
  EXPECT_FALSE(pg.parse("a,b", out));
}

// This test checks that ordinary references are linked to actual grammar rules.
// The macro body references `Item`, which is not a macro parameter, so it must
// resolve as a definition rule in the grammar.
TEST(PeglibLinkReferencesTest, NonParameterReferenceResolvesToDefinitionRule) {
  parser pg(R"(
    Wrap() <- Item
    Item <- 'x'
    Start <- Wrap()
  )");

  ASSERT_TRUE(static_cast<bool>(pg));

  std::string out;
  EXPECT_TRUE(pg.parse("x", out));
  EXPECT_FALSE(pg.parse("y", out));
}

// This exercises recursive visiting of arguments in ope.args_.
// The argument supplied to the outer macro is itself another macro invocation,
// so LinkReferences::visit must recurse into arguments and link references
// within nested argument expressions correctly.
TEST(PeglibLinkReferencesTest, NestedMacroArgumentsAreVisitedRecursively) {
  parser pg(R"(
    Pair(x) <- x x
    Id(y) <- y
    Start <- Pair(Id('a'))
  )");

  ASSERT_TRUE(static_cast<bool>(pg));

  std::string out;
  EXPECT_TRUE(pg.parse("aa", out));
  EXPECT_FALSE(pg.parse("a", out));
  EXPECT_FALSE(pg.parse("ab", out));
}

// This verifies the precedence encoded in the focal function: matching a macro
// parameter must win over an existing grammar rule with the same name.
// Here there is a rule named `x`, but inside Use(x) the reference `x` should
// still refer to the macro parameter, not the grammar rule.
TEST(PeglibLinkReferencesTest, MacroParameterTakesPrecedenceOverRuleWithSameName) {
  parser pg(R"(
    x <- 'b'
    Use(x) <- x
    Start <- Use('a')
  )");

  ASSERT_TRUE(static_cast<bool>(pg));

  std::string out;
  EXPECT_TRUE(pg.parse("a", out));
  EXPECT_FALSE(pg.parse("b", out));
}

// This checks the edge case where a reference is neither a macro parameter nor
// a grammar rule. Construction should fail because the unresolved reference
// remains invalid. A buggy LinkReferences::visit that incorrectly links or
// ignores missing references would cause this to pass unexpectedly.
TEST(PeglibLinkReferencesTest, UnresolvedReferenceCausesGrammarConstructionFailure) {
  parser pg(R"(
    Use(x) <- Missing
    Start <- Use('a')
  )");

  EXPECT_FALSE(static_cast<bool>(pg));
}

// This exercises the boundary case of an empty parameter list. Since there are
// no macro parameters, a reference in the body must be resolved only as a
// grammar rule if one exists.
TEST(PeglibLinkReferencesTest, EmptyParameterListStillLinksDefinitionRuleReferences) {
  parser pg(R"(
    Value <- 'z'
    Use() <- Value
    Start <- Use()
  )");

  ASSERT_TRUE(static_cast<bool>(pg));

  std::string out;
  EXPECT_TRUE(pg.parse("z", out));
  EXPECT_FALSE(pg.parse("a", out));
}

// This test ensures that parameter matching is positional and works for later
// parameters too, not just the first one. If iarg_ assignment/search were
// broken, the substituted expansion would use the wrong argument.
TEST(PeglibLinkReferencesTest, LaterMacroParameterIsResolvedCorrectlyByPosition) {
  parser pg(R"(
    Second(a, b) <- b
    Start <- Second('x', 'y')
  )");

  ASSERT_TRUE(static_cast<bool>(pg));

  std::string out;
  EXPECT_TRUE(pg.parse("y", out));
  EXPECT_FALSE(pg.parse("x", out));
}

// This test verifies that references appearing inside nested expressions in the
// macro body are properly linked. It indirectly exercises traversal from the
// reference node through argument expressions and full parser construction.
TEST(PeglibLinkReferencesTest, ParameterReferenceWorksInsideCompositeExpression) {
  parser pg(R"(
    Bracket(x) <- '(' x ')'
    Start <- Bracket('a')
  )");

  ASSERT_TRUE(static_cast<bool>(pg));

  std::string out;
  EXPECT_TRUE(pg.parse("(a)", out));
  EXPECT_FALSE(pg.parse("a", out));
  EXPECT_FALSE(pg.parse("(b)", out));
}

}  // namespace