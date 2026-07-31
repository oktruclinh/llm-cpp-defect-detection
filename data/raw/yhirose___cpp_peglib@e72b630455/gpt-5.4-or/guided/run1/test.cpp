#include <gtest/gtest.h>
#include "peglib.h"

#include <string>
#include <vector>

namespace {

using peg::SemanticValues;
using peg::any;

TEST(SemanticValuesTest, DefaultConstructedState) {
  SemanticValues sv;

  EXPECT_EQ(sv.c_str(), nullptr);
  EXPECT_EQ(sv.length(), 0u);
  EXPECT_EQ(sv.str(), "");
  EXPECT_EQ(sv.choice(), 0u);
  EXPECT_TRUE(sv.empty());
  EXPECT_EQ(sv.size(), 0u);
  EXPECT_TRUE(sv.tokens.empty());
  EXPECT_EQ(sv.token(), "");
}

TEST(SemanticValuesTest, BehavesLikeVectorOfAnyForStorageAndAccess) {
  SemanticValues sv;

  sv.push_back(any(10));
  sv.push_back(any(std::string("hello")));
  sv.emplace_back(42);

  ASSERT_EQ(sv.size(), 3u);
  EXPECT_EQ(sv[0].get<int>(), 10);
  EXPECT_EQ(sv[1].get<std::string>(), "hello");
  EXPECT_EQ(sv[2].get<int>(), 42);

  EXPECT_EQ(sv.front().get<int>(), 10);
  EXPECT_EQ(sv.back().get<int>(), 42);
}

TEST(SemanticValuesTest, TransformTypedOverEntireRange) {
  SemanticValues sv;
  sv.push_back(any(1));
  sv.push_back(any(2));
  sv.push_back(any(3));

  auto values = sv.transform<int>();

  ASSERT_EQ(values.size(), 3u);
  EXPECT_EQ(values[0], 1);
  EXPECT_EQ(values[1], 2);
  EXPECT_EQ(values[2], 3);
}

TEST(SemanticValuesTest, TransformTypedWithBeginAndEndSubrange) {
  SemanticValues sv;
  sv.push_back(any(5));
  sv.push_back(any(10));
  sv.push_back(any(15));
  sv.push_back(any(20));

  auto values = sv.transform<int>(1, 3);

  ASSERT_EQ(values.size(), 2u);
  EXPECT_EQ(values[0], 10);
  EXPECT_EQ(values[1], 15);
}

TEST(SemanticValuesTest, TransformTypedWithEndPastSizeIsClamped) {
  SemanticValues sv;
  sv.push_back(any(7));
  sv.push_back(any(8));
  sv.push_back(any(9));

  auto values = sv.transform<int>(1, 100);

  ASSERT_EQ(values.size(), 2u);
  EXPECT_EQ(values[0], 8);
  EXPECT_EQ(values[1], 9);
}

TEST(SemanticValuesTest, TransformTypedWithEmptyRangeReturnsEmptyVector) {
  SemanticValues sv;
  sv.push_back(any(1));
  sv.push_back(any(2));

  auto values1 = sv.transform<int>(1, 1);
  auto values2 = sv.transform<int>(2, 1);
  auto values3 = sv.transform<int>(5, 10);

  EXPECT_TRUE(values1.empty());
  EXPECT_TRUE(values2.empty());
  EXPECT_TRUE(values3.empty());
}

TEST(SemanticValuesTest, TransformPreservesValueTypesIncludingStrings) {
  SemanticValues sv;
  sv.push_back(any(std::string("alpha")));
  sv.push_back(any(std::string("beta")));
  sv.push_back(any(std::string("gamma")));

  auto values = sv.transform<std::string>(0, 2);

  ASSERT_EQ(values.size(), 2u);
  EXPECT_EQ(values[0], "alpha");
  EXPECT_EQ(values[1], "beta");
}

TEST(SemanticValuesTest, TokenFallsBackToMatchedStringWhenNoTokensPresent) {
  SemanticValues sv;

  EXPECT_TRUE(sv.tokens.empty());
  EXPECT_EQ(sv.token(), "");

  char mutable_input[] = "abcdef";
  sv.tokens.push_back({mutable_input + 2, 3});
  EXPECT_EQ(sv.token(), "cde");
  EXPECT_EQ(sv.token(0), "cde");
}

TEST(SemanticValuesTest, TokenReturnsSpecifiedTokenByIndex) {
  SemanticValues sv;

  static const char text[] = "first second third";
  sv.tokens.push_back({text, 5});
  sv.tokens.push_back({text + 6, 6});
  sv.tokens.push_back({text + 13, 5});

  ASSERT_EQ(sv.tokens.size(), 3u);
  EXPECT_EQ(sv.token(0), "first");
  EXPECT_EQ(sv.token(1), "second");
  EXPECT_EQ(sv.token(2), "third");
}

TEST(SemanticValuesTest, TokenHandlesZeroLengthToken) {
  SemanticValues sv;

  static const char text[] = "xyz";
  sv.tokens.push_back({text + 1, 0});

  EXPECT_EQ(sv.token(), "");
  EXPECT_EQ(sv.token(0), "");
}

TEST(SemanticValuesTest, ChoiceDefaultIsZeroAndVectorOperationsDoNotAlterIt) {
  SemanticValues sv;
  EXPECT_EQ(sv.choice(), 0u);

  sv.push_back(any(1));
  sv.push_back(any(2));
  sv.pop_back();
  sv.clear();

  EXPECT_EQ(sv.choice(), 0u);
}

TEST(SemanticValuesTest, StrAndLengthAreConsistentInDefaultState) {
  SemanticValues sv;

  EXPECT_EQ(sv.length(), sv.str().size());
  EXPECT_TRUE(sv.str().empty());
}

}  // namespace