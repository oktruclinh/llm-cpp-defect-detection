#include <gtest/gtest.h>
#include "peglib.h"

#include <string>
#include <vector>

namespace {
using peg::parser;

// Helper that parses all of `input` and returns whether parsing succeeded.
bool parse_all(parser& p, const std::string& input) {
  return p.parse(input.data(), input.size());
}

TEST(PeglibReferenceTraversalTest, ReferencedRuleWithNoActionStillParsesWhenUsedThroughSequence) {
  parser pg(R"(
    Start <- Ref 'b'
    Ref   <- 'a'
  )");

  ASSERT_TRUE(pg) << pg.log;

  EXPECT_TRUE(parse_all(pg, "ab"));
  EXPECT_FALSE(parse_all(pg, "a"));
  EXPECT_FALSE(parse_all(pg, "b"));
  EXPECT_FALSE(parse_all(pg, "abc"));
}

TEST(PeglibReferenceTraversalTest, ActionsAttachedToReferencedRuleAreAppliedWhenReferenceHasNoArguments) {
  parser pg(R"(
    Start <- Ref
    Ref   <- 'a'
  )");

  ASSERT_TRUE(pg) << pg.log;

  int ref_action_calls = 0;
  int start_action_calls = 0;

  pg["Ref"] = [&](const peg::SemanticValues&) {
    ref_action_calls++;
    return std::string("from_ref");
  };

  pg["Start"] = [&](const peg::SemanticValues& sv) {
    start_action_calls++;
    EXPECT_EQ(1u, sv.size());
    EXPECT_TRUE(sv[0].is<std::string>());
    EXPECT_EQ("from_ref", sv[0].get<std::string>());
    return sv[0].get<std::string>();
  };

  std::string out;
  EXPECT_TRUE(pg.parse("a", out));
  EXPECT_EQ("from_ref", out);
  EXPECT_EQ(1, ref_action_calls);
  EXPECT_EQ(1, start_action_calls);

  out.clear();
  EXPECT_FALSE(pg.parse("b", out));
  EXPECT_EQ(1, ref_action_calls);
  EXPECT_EQ(1, start_action_calls);
}

TEST(PeglibReferenceTraversalTest, MacroReferenceArgumentsAreTraversedAndApplied) {
  parser pg(R"(
    Start  <- Pair< 'a', 'b' >
    Pair<x, y> <- x y
  )");

  ASSERT_TRUE(pg) << pg.log;

  int pair_calls = 0;
  int start_calls = 0;

  pg["Pair"] = [&](const peg::SemanticValues& sv) {
    pair_calls++;
    EXPECT_EQ(2u, sv.size());
    EXPECT_TRUE(sv[0].is<std::string>());
    EXPECT_TRUE(sv[1].is<std::string>());
    EXPECT_EQ("a", sv[0].get<std::string>());
    EXPECT_EQ("b", sv[1].get<std::string>());
    return std::string("ab");
  };

  pg["Start"] = [&](const peg::SemanticValues& sv) {
    start_calls++;
    EXPECT_EQ(1u, sv.size());
    EXPECT_TRUE(sv[0].is<std::string>());
    EXPECT_EQ("ab", sv[0].get<std::string>());
    return sv[0].get<std::string>();
  };

  std::string out;
  EXPECT_TRUE(pg.parse("ab", out));
  EXPECT_EQ("ab", out);
  EXPECT_EQ(1, pair_calls);
  EXPECT_EQ(1, start_calls);

  out.clear();
  EXPECT_FALSE(pg.parse("aa", out));
  EXPECT_EQ(1, pair_calls);
  EXPECT_EQ(1, start_calls);
}

TEST(PeglibReferenceTraversalTest, NestedMacroArgumentReferenceIsVisitedRecursively) {
  parser pg(R"(
    Start       <- Wrapper< Atom >
    Wrapper<x>  <- x
    Atom        <- 'z'
  )");

  ASSERT_TRUE(pg) << pg.log;

  int atom_calls = 0;
  int wrapper_calls = 0;
  int start_calls = 0;

  pg["Atom"] = [&](const peg::SemanticValues&) {
    atom_calls++;
    return std::string("atom");
  };

  pg["Wrapper"] = [&](const peg::SemanticValues& sv) {
    wrapper_calls++;
    EXPECT_EQ(1u, sv.size());
    EXPECT_TRUE(sv[0].is<std::string>());
    EXPECT_EQ("atom", sv[0].get<std::string>());
    return std::string("wrapped:" + sv[0].get<std::string>());
  };

  pg["Start"] = [&](const peg::SemanticValues& sv) {
    start_calls++;
    EXPECT_EQ(1u, sv.size());
    EXPECT_TRUE(sv[0].is<std::string>());
    EXPECT_EQ("wrapped:atom", sv[0].get<std::string>());
    return sv[0].get<std::string>();
  };

  std::string out;
  EXPECT_TRUE(pg.parse("z", out));
  EXPECT_EQ("wrapped:atom", out);
  EXPECT_EQ(1, atom_calls);
  EXPECT_EQ(1, wrapper_calls);
  EXPECT_EQ(1, start_calls);
}

TEST(PeglibReferenceTraversalTest, UndefinedReferenceCausesGrammarConstructionFailure) {
  parser pg(R"(
    Start <- Missing
  )");

  EXPECT_FALSE(static_cast<bool>(pg));
  EXPECT_FALSE(pg.log.empty());
}

TEST(PeglibReferenceTraversalTest, UnusedUndefinedReferenceDoesNotAffectValidGrammar) {
  parser pg(R"(
    Start <- 'x'
    Dead  <- Missing
  )");

  EXPECT_FALSE(static_cast<bool>(pg));
  EXPECT_FALSE(pg.log.empty());
}

TEST(PeglibReferenceTraversalTest, ForwardReferenceResolvesCorrectly) {
  parser pg(R"(
    Start <- Later
    Later <- 'q'
  )");

  ASSERT_TRUE(pg) << pg.log;

  int later_calls = 0;
  pg["Later"] = [&](const peg::SemanticValues&) {
    later_calls++;
    return std::string("later");
  };

  std::string out;
  EXPECT_TRUE(pg.parse("q", out));
  EXPECT_EQ("later", out);
  EXPECT_EQ(1, later_calls);

  out.clear();
  EXPECT_FALSE(pg.parse("p", out));
  EXPECT_EQ(1, later_calls);
}

TEST(PeglibReferenceTraversalTest, MacroReferenceWithMultipleArgumentsUsesEachArgumentInOrder) {
  parser pg(R"(
    Start <- Triple< 'a', 'b', 'c' >
    Triple<x, y, z> <- x y z
  )");

  ASSERT_TRUE(pg) << pg.log;

  int triple_calls = 0;
  pg["Triple"] = [&](const peg::SemanticValues& sv) {
    triple_calls++;
    EXPECT_EQ(3u, sv.size());
    EXPECT_EQ("a", sv[0].get<std::string>());
    EXPECT_EQ("b", sv[1].get<std::string>());
    EXPECT_EQ("c", sv[2].get<std::string>());
    return std::string("abc");
  };

  std::string out;
  EXPECT_TRUE(pg.parse("abc", out));
  EXPECT_EQ("abc", out);
  EXPECT_EQ(1, triple_calls);

  EXPECT_FALSE(pg.parse("acb", out));
  EXPECT_EQ(1, triple_calls);
}

}  // namespace