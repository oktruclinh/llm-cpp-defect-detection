#include <gtest/gtest.h>
#include "peglib.h"

using namespace peg;

namespace {
struct CountingSemanticValues {
  static int a_count;
  static int b_count;
  static int c_count;
  static int start_count;

  static void Reset() {
    a_count = 0;
    b_count = 0;
    c_count = 0;
    start_count = 0;
  }
};

int CountingSemanticValues::a_count = 0;
int CountingSemanticValues::b_count = 0;
int CountingSemanticValues::c_count = 0;
int CountingSemanticValues::start_count = 0;
} // namespace

TEST(AssignIDToDefinitionReferenceVisitTest, ReferencedRuleAndArgumentsReceiveActions) {
  CountingSemanticValues::Reset();

  parser pg(R"(
    Start <- Ref
    Ref   <- A<B, C>
    A     <- 'a'
    B     <- 'b'
    C     <- 'c'
  )");

  ASSERT_TRUE(static_cast<bool>(pg));

  pg["Start"] = [](const SemanticValues&) {
    CountingSemanticValues::start_count++;
    return 0;
  };
  pg["A"] = [](const SemanticValues&) {
    CountingSemanticValues::a_count++;
    return 0;
  };
  pg["B"] = [](const SemanticValues&) {
    CountingSemanticValues::b_count++;
    return 0;
  };
  pg["C"] = [](const SemanticValues&) {
    CountingSemanticValues::c_count++;
    return 0;
  };

  int out = -1;
  ASSERT_TRUE(pg.parse("a", out));

  EXPECT_EQ(1, CountingSemanticValues::start_count);
  EXPECT_EQ(1, CountingSemanticValues::a_count);
  EXPECT_EQ(1, CountingSemanticValues::b_count);
  EXPECT_EQ(1, CountingSemanticValues::c_count);
}

TEST(AssignIDToDefinitionReferenceVisitTest, MultipleReferenceArgumentsAreAllTraversed) {
  CountingSemanticValues::Reset();

  parser pg(R"(
    Start <- Ref / 'x'
    Ref   <- A<B, C>
    A     <- 'a'
    B     <- 'b'
    C     <- 'c'
  )");

  ASSERT_TRUE(static_cast<bool>(pg));

  pg["A"] = [](const SemanticValues&) {
    CountingSemanticValues::a_count++;
    return 0;
  };
  pg["B"] = [](const SemanticValues&) {
    CountingSemanticValues::b_count++;
    return 0;
  };
  pg["C"] = [](const SemanticValues&) {
    CountingSemanticValues::c_count++;
    return 0;
  };

  int out = -1;
  ASSERT_TRUE(pg.parse("a", out));

  EXPECT_EQ(1, CountingSemanticValues::a_count);
  EXPECT_EQ(1, CountingSemanticValues::b_count);
  EXPECT_EQ(1, CountingSemanticValues::c_count);
}

TEST(AssignIDToDefinitionReferenceVisitTest, UndefinedReferencedRuleCausesGrammarConstructionFailure) {
  parser pg(R"(
    Start <- MissingRule
  )");

  EXPECT_FALSE(static_cast<bool>(pg));
}

TEST(AssignIDToDefinitionReferenceVisitTest, UndefinedTemplateArgumentCausesGrammarConstructionFailure) {
  parser pg(R"(
    Start <- Ref
    Ref   <- A<MissingArg>
    A     <- 'a'
  )");

  EXPECT_FALSE(static_cast<bool>(pg));
}

TEST(AssignIDToDefinitionReferenceVisitTest, ReferenceWithoutArgumentsStillTraversesReferencedRule) {
  CountingSemanticValues::Reset();

  parser pg(R"(
    Start <- Ref
    Ref   <- A
    A     <- 'a'
  )");

  ASSERT_TRUE(static_cast<bool>(pg));

  pg["A"] = [](const SemanticValues&) {
    CountingSemanticValues::a_count++;
    return 0;
  };

  int out = -1;
  ASSERT_TRUE(pg.parse("a", out));
  EXPECT_EQ(1, CountingSemanticValues::a_count);
  EXPECT_EQ(0, CountingSemanticValues::b_count);
  EXPECT_EQ(0, CountingSemanticValues::c_count);
}