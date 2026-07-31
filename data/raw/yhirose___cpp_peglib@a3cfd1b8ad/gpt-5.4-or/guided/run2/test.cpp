#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "peglib.h"

namespace {

using peg::parser;

TEST(PeglibMakeGrammarTest, ParsesMinimalGrammar) {
  parser p;
  ASSERT_TRUE(p.load_grammar("Start <- 'a'\n"));

  std::string out;
  EXPECT_TRUE(p.parse("a", out));
  EXPECT_EQ("a", out);

  out.clear();
  EXPECT_FALSE(p.parse("b", out));
}

TEST(PeglibMakeGrammarTest, RequiresAtLeastOneDefinition) {
  parser p;
  EXPECT_FALSE(p.load_grammar(""));
  EXPECT_FALSE(p.load_grammar("   \t  \n  # only comment\n"));
}

TEST(PeglibMakeGrammarTest, AcceptsLeadingIgnoreOnDefinition) {
  parser p;
  ASSERT_TRUE(p.load_grammar("~Start <- 'a'\n"));

  std::string out;
  EXPECT_TRUE(p.parse("a", out));
  EXPECT_EQ("a", out);
  EXPECT_FALSE(p.parse("aa", out));
}

TEST(PeglibMakeGrammarTest, SupportsOrderedChoiceSequenceAndQuantifiers) {
  parser p;
  ASSERT_TRUE(p.load_grammar(
      "Start <- 'a' 'b'? 'c'* 'd'+ / 'x'\n"));

  std::string out;

  EXPECT_TRUE(p.parse("ad", out));
  EXPECT_TRUE(p.parse("abcd", out));
  EXPECT_TRUE(p.parse("abccddd", out));
  EXPECT_TRUE(p.parse("x", out));

  EXPECT_FALSE(p.parse("a", out));
  EXPECT_FALSE(p.parse("abc", out));
  EXPECT_FALSE(p.parse("abccd", out + "extra")); // keep compile-safe use of out
  EXPECT_FALSE(p.parse("y", out));
}

TEST(PeglibMakeGrammarTest, SupportsAndNotPredicates) {
  parser p;
  ASSERT_TRUE(p.load_grammar(
      "Start <- &'a' 'a' !'b' .\n"));

  std::string out;
  EXPECT_TRUE(p.parse("ac", out));
  EXPECT_EQ("ac", out);

  EXPECT_FALSE(p.parse("ab", out));
  EXPECT_FALSE(p.parse("bc", out));
  EXPECT_FALSE(p.parse("a", out));
}

TEST(PeglibMakeGrammarTest, SupportsParenthesizedExpressions) {
  parser p;
  ASSERT_TRUE(p.load_grammar(
      "Start <- ('a' / 'b') 'c'\n"));

  std::string out;
  EXPECT_TRUE(p.parse("ac", out));
  EXPECT_TRUE(p.parse("bc", out));
  EXPECT_FALSE(p.parse("cc", out));
  EXPECT_FALSE(p.parse("a", out));
}

TEST(PeglibMakeGrammarTest, SupportsAngleBracketTokenBoundarySyntax) {
  parser p;
  ASSERT_TRUE(p.load_grammar(
      "Start <- < 'a'+ >\n"));

  std::string out;
  EXPECT_TRUE(p.parse("aaa", out));
  EXPECT_EQ("aaa", out);
  EXPECT_FALSE(p.parse("bbb", out));
}

TEST(PeglibMakeGrammarTest, SupportsCaptureSyntaxWithAndWithoutName) {
  {
    parser p;
    ASSERT_TRUE(p.load_grammar(
        "Start <- $< 'a'+ >\n"));

    std::string out;
    EXPECT_TRUE(p.parse("aaaa", out));
    EXPECT_EQ("aaaa", out);
    EXPECT_FALSE(p.parse("b", out));
  }

  {
    parser p;
    ASSERT_TRUE(p.load_grammar(
        "Start <- $cap< 'b'+ >\n"));

    std::string out;
    EXPECT_TRUE(p.parse("bbb", out));
    EXPECT_EQ("bbb", out);
    EXPECT_FALSE(p.parse("a", out));
  }
}

TEST(PeglibMakeGrammarTest, SupportsCharacterClassesAndRanges) {
  parser p;
  ASSERT_TRUE(p.load_grammar(
      "Start <- [a-cx-z][0-9]\n"));

  std::string out;
  EXPECT_TRUE(p.parse("a0", out));
  EXPECT_TRUE(p.parse("c9", out));
  EXPECT_TRUE(p.parse("x5", out));
  EXPECT_TRUE(p.parse("z1", out));

  EXPECT_FALSE(p.parse("d0", out));
  EXPECT_FALSE(p.parse("a", out));
  EXPECT_FALSE(p.parse("a10", out));
}

TEST(PeglibMakeGrammarTest, SupportsLiteralEscapes) {
  parser p;
  ASSERT_TRUE(p.load_grammar(
      "NL <- '\\n'\n"
      "TAB <- \"\\t\"\n"
      "HEX <- '\\x41'\n"));

  std::string out;
  EXPECT_TRUE(p["NL"].parse("\n", out));
  EXPECT_EQ("\n", out);

  out.clear();
  EXPECT_TRUE(p["TAB"].parse("\t", out));
  EXPECT_EQ("\t", out);

  out.clear();
  EXPECT_TRUE(p["HEX"].parse("A", out));
  EXPECT_EQ("A", out);

  EXPECT_FALSE(p["HEX"].parse("B", out));
}

TEST(PeglibMakeGrammarTest, SupportsClassCharacterEscapes) {
  parser p;
  ASSERT_TRUE(p.load_grammar(
      "Start <- [\\n\\t\\]\\[]+\n"));

  std::string out;
  EXPECT_TRUE(p.parse("\n\t][", out));
  EXPECT_EQ("\n\t][", out);

  EXPECT_FALSE(p.parse("a", out));
}

TEST(PeglibMakeGrammarTest, SupportsCommentsAndSpacingAroundTokens) {
  parser p;
  ASSERT_TRUE(p.load_grammar(
      "# first comment\r\n"
      "Start   <-   'a'   # trailing comment\n"
      "         /   'b'   \r"
      "# final comment\n"));

  std::string out;
  EXPECT_TRUE(p.parse("a", out));
  EXPECT_TRUE(p.parse("b", out));
  EXPECT_FALSE(p.parse("c", out));
}

TEST(PeglibMakeGrammarTest, IdentifierCanUseExtendedAsciiAndDigitsAfterStart) {
  parser p;

  const std::string grammar =
      std::string("\xC0") + "rule9 <- 'x'\n";
  ASSERT_TRUE(p.load_grammar(grammar));

  std::string out;
  EXPECT_TRUE(p.parse("x", out));
  EXPECT_EQ("x", out);
  EXPECT_FALSE(p.parse("y", out));
}

TEST(PeglibMakeGrammarTest, DotMatchesAnySingleCharacter) {
  parser p;
  ASSERT_TRUE(p.load_grammar("Start <- .\n"));

  std::string out;
  EXPECT_TRUE(p.parse("a", out));
  EXPECT_EQ("a", out);

  out.clear();
  EXPECT_TRUE(p.parse("\n", out));
  EXPECT_EQ("\n", out);

  EXPECT_FALSE(p.parse("", out));
  EXPECT_FALSE(p.parse("ab", out));
}

TEST(PeglibMakeGrammarTest, InvalidGrammarMissingLeftArrowFails) {
  parser p;
  EXPECT_FALSE(p.load_grammar("Start 'a'\n"));
}

TEST(PeglibMakeGrammarTest, InvalidGrammarUnterminatedLiteralFails) {
  parser p;
  EXPECT_FALSE(p.load_grammar("Start <- 'abc\n"));
  EXPECT_FALSE(p.load_grammar("Start <- \"abc\n"));
}

TEST(PeglibMakeGrammarTest, InvalidGrammarUnterminatedClassFails) {
  parser p;
  EXPECT_FALSE(p.load_grammar("Start <- [abc\n"));
}

TEST(PeglibMakeGrammarTest, InvalidGrammarIdentifierCannotStartWithDigit) {
  parser p;
  EXPECT_FALSE(p.load_grammar("1Start <- 'a'\n"));
}

TEST(PeglibMakeGrammarTest, GrammarMustConsumeEntireGrammarText) {
  parser p;
  EXPECT_FALSE(p.load_grammar("Start <- 'a' trailing"));
}

TEST(PeglibMakeGrammarTest, RuleLookupByDefinitionNameWorksAfterGrammarConstruction) {
  parser p;
  ASSERT_TRUE(p.load_grammar(
      "First <- 'a'\n"
      "Second <- 'b'\n"));

  std::string out;
  EXPECT_TRUE(p["First"].parse("a", out));
  EXPECT_EQ("a", out);

  out.clear();
  EXPECT_TRUE(p["Second"].parse("b", out));
  EXPECT_EQ("b", out);

  EXPECT_FALSE(p["First"].parse("b", out));
  EXPECT_FALSE(p["Second"].parse("a", out));
}

}  // namespace