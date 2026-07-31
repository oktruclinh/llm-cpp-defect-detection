#include <gtest/gtest.h>
#include "peglib.h"

#include <string>
#include <vector>

namespace {

using peg::parser;

TEST(PeglibGrammarConstructionTest, ParsesMinimalGrammarDefinition) {
  parser p;
  ASSERT_TRUE(p.load_grammar("Start <- 'a'\n"));

  EXPECT_EQ(p.get_rule_count(), 1u);

  std::string out;
  EXPECT_TRUE(p.parse("a", out));
  EXPECT_EQ(out, "a");

  out.clear();
  EXPECT_FALSE(p.parse("b", out));
}

TEST(PeglibGrammarConstructionTest, ParsesMultipleDefinitionsWithCommentsAndSpacing) {
  parser p;
  const char *grammar =
      "# Leading comment\r\n"
      "  Start   <-   Expr   # trailing comment\r\n"
      "Expr <- 'a' / 'b'\n"
      "# final comment\n";

  ASSERT_TRUE(p.load_grammar(grammar));
  EXPECT_EQ(p.get_rule_count(), 2u);

  std::string out;
  EXPECT_TRUE(p.parse("a", out));
  EXPECT_EQ(out, "a");

  out.clear();
  EXPECT_TRUE(p.parse("b", out));
  EXPECT_EQ(out, "b");

  out.clear();
  EXPECT_FALSE(p.parse("c", out));
}

TEST(PeglibGrammarConstructionTest, EmptyGrammarIsRejected) {
  parser p;
  EXPECT_FALSE(p.load_grammar(""));
  EXPECT_FALSE(p.load_grammar("   \t\r\n  "));
  EXPECT_FALSE(p.load_grammar("# only comment\n"));
}

TEST(PeglibGrammarConstructionTest, MissingExpressionAfterLeftArrowIsRejected) {
  parser p;
  EXPECT_FALSE(p.load_grammar("Start <- "));
  EXPECT_FALSE(p.load_grammar("Start <-\n"));
}

TEST(PeglibGrammarConstructionTest, MissingLeftArrowIsRejected) {
  parser p;
  EXPECT_FALSE(p.load_grammar("Start 'a'\n"));
}

TEST(PeglibGrammarConstructionTest, PrimaryIdentifierDoesNotConsumeDefinitionLeftArrow) {
  parser p;
  const char *grammar =
      "Start <- Rule\n"
      "Rule <- 'x'\n";

  ASSERT_TRUE(p.load_grammar(grammar));
  EXPECT_EQ(p.get_rule_count(), 2u);

  std::string out;
  EXPECT_TRUE(p.parse("x", out));
  EXPECT_EQ(out, "x");

  out.clear();
  EXPECT_FALSE(p.parse("y", out));
}

TEST(PeglibGrammarConstructionTest, GroupingAndQuantifiersAreRecognized) {
  parser p;
  const char *grammar = "Start <- ('a' / 'b')+ 'c'?\n";

  ASSERT_TRUE(p.load_grammar(grammar));

  std::string out;
  EXPECT_TRUE(p.parse("a", out));
  EXPECT_EQ(out, "a");

  out.clear();
  EXPECT_TRUE(p.parse("bbbc", out));
  EXPECT_EQ(out, "bbbc");

  out.clear();
  EXPECT_FALSE(p.parse("", out));

  out.clear();
  EXPECT_FALSE(p.parse("c", out));
}

TEST(PeglibGrammarConstructionTest, PredicatesAreRecognized) {
  parser p;
  const char *grammar =
      "Start <- &'a' 'a' / !'a' .\n";

  ASSERT_TRUE(p.load_grammar(grammar));

  std::string out;
  EXPECT_TRUE(p.parse("a", out));
  EXPECT_EQ(out, "a");

  out.clear();
  EXPECT_TRUE(p.parse("b", out));
  EXPECT_EQ(out, "b");

  out.clear();
  EXPECT_FALSE(p.parse("", out));
}

TEST(PeglibGrammarConstructionTest, DotLiteralAndCharacterClassAreRecognized) {
  parser p;
  const char *grammar =
      "Start <- [a-c] .\n";

  ASSERT_TRUE(p.load_grammar(grammar));

  std::string out;
  EXPECT_TRUE(p.parse("az", out));
  EXPECT_EQ(out, "az");

  out.clear();
  EXPECT_TRUE(p.parse("c1", out));
  EXPECT_EQ(out, "c1");

  out.clear();
  EXPECT_FALSE(p.parse("dz", out));

  out.clear();
  EXPECT_FALSE(p.parse("a", out));
}

TEST(PeglibGrammarConstructionTest, CharacterEscapesInLiteralAndClassAreRecognized) {
  parser p;
  const char *grammar =
      "Start <- \"\\n\" [\\]] [\\[] [\\\\]\n";

  ASSERT_TRUE(p.load_grammar(grammar));

  const std::string input = std::string("\n][\\", 4);
  std::string out;
  EXPECT_TRUE(p.parse(input, out));
  EXPECT_EQ(out, input);

  out.clear();
  EXPECT_FALSE(p.parse("x][\\", out));
}

TEST(PeglibGrammarConstructionTest, OctalAndHexEscapesInClassAreRecognized) {
  parser p;
  const char *grammar =
      "Start <- [\\101] [\\x42]\n";

  ASSERT_TRUE(p.load_grammar(grammar));

  std::string out;
  EXPECT_TRUE(p.parse("AB", out));
  EXPECT_EQ(out, "AB");

  out.clear();
  EXPECT_FALSE(p.parse("AC", out));

  out.clear();
  EXPECT_FALSE(p.parse("CB", out));
}

TEST(PeglibGrammarConstructionTest, InvalidCharacterEscapeInGrammarIsRejected) {
  parser p;
  EXPECT_FALSE(p.load_grammar("Start <- \"\\q\"\n"));
  EXPECT_FALSE(p.load_grammar("Start <- [\\q]\n"));
}

TEST(PeglibGrammarConstructionTest, BeginEndTokensForTokenBoundaryAreRecognized) {
  parser p;
  const char *grammar = "Start <- < 'a'+ >\n";

  ASSERT_TRUE(p.load_grammar(grammar));

  std::string out;
  EXPECT_TRUE(p.parse("aaa", out));
  EXPECT_EQ(out, "aaa");

  out.clear();
  EXPECT_FALSE(p.parse("", out));
  EXPECT_FALSE(p.parse("b", out));
}

TEST(PeglibGrammarConstructionTest, CaptureSyntaxWithOptionalIdentifierIsRecognized) {
  parser p_named;
  ASSERT_TRUE(p_named.load_grammar("Start <- $cap< 'a'+ >\n"));

  std::string out;
  EXPECT_TRUE(p_named.parse("aaa", out));
  EXPECT_EQ(out, "aaa");

  parser p_unnamed;
  ASSERT_TRUE(p_unnamed.load_grammar("Start <- $< 'b'+ >\n"));

  out.clear();
  EXPECT_TRUE(p_unnamed.parse("bb", out));
  EXPECT_EQ(out, "bb");
}

TEST(PeglibGrammarConstructionTest, IgnorePrefixOnDefinitionIsAccepted) {
  parser p;
  const char *grammar =
      "~Ignored <- 'x'\n"
      "Start <- Ignored 'y'\n";

  ASSERT_TRUE(p.load_grammar(grammar));
  EXPECT_EQ(p.get_rule_count(), 2u);

  std::string out;
  EXPECT_TRUE(p.parse("xy", out));
  EXPECT_EQ(out, "xy");

  out.clear();
  EXPECT_FALSE(p.parse("y", out));
}

TEST(PeglibGrammarConstructionTest, NonAsciiIdentifierStartIsAccepted) {
  parser p;
  const std::string grammar =
      std::string("\xC0", 1) + "rule <- 'z'\n"
      "Start <- " + std::string("\xC0", 1) + "rule\n";

  ASSERT_TRUE(p.load_grammar(grammar));

  std::string out;
  EXPECT_TRUE(p.parse("z", out));
  EXPECT_EQ(out, "z");

  out.clear();
  EXPECT_FALSE(p.parse("x", out));
}

TEST(PeglibGrammarConstructionTest, IdentifierCannotStartWithDigit) {
  parser p;
  EXPECT_FALSE(p.load_grammar("1Start <- 'a'\n"));
}

TEST(PeglibGrammarConstructionTest, EndOfFileIsEnforcedWhenLoadingGrammar) {
  parser p;
  EXPECT_FALSE(p.load_grammar("Start <- 'a' garbage"));
}

TEST(PeglibGrammarConstructionTest, CommentRequiresEndOfLineAndSupportsDifferentLineEndings) {
  parser p1;
  ASSERT_TRUE(p1.load_grammar("Start <- 'a'#comment\r\n"));
  std::string out;
  EXPECT_TRUE(p1.parse("a", out));
  EXPECT_EQ(out, "a");

  parser p2;
  ASSERT_TRUE(p2.load_grammar("Start <- 'a'#comment\n"));
  out.clear();
  EXPECT_TRUE(p2.parse("a", out));
  EXPECT_EQ(out, "a");

  parser p3;
  ASSERT_TRUE(p3.load_grammar("Start <- 'a'#comment\r"));
  out.clear();
  EXPECT_TRUE(p3.parse("a", out));
  EXPECT_EQ(out, "a");

  parser p4;
  EXPECT_FALSE(p4.load_grammar("Start <- 'a'#comment"));
}

}  // namespace