#include <gtest/gtest.h>
#include "peglib.h"

#include <string>
#include <vector>

namespace {

TEST(SemanticValuesTest, TokenReturnsMatchedStringWhenNoTokensExist) {
  peg::SemanticValues sv;

  const std::string input = "abcdef";
  sv.ss = input.c_str();
  sv.path = "dummy";

  struct SemanticValuesAccess : peg::SemanticValues {
    void set_match(const char* s, size_t n) {
      this->s_ = s;
      this->n_ = n;
    }
  };

  auto& access = reinterpret_cast<SemanticValuesAccess&>(sv);
  access.set_match(input.c_str() + 2, 3); // "cde"

  EXPECT_EQ("cde", sv.token());
  EXPECT_EQ("cde", sv.str());
  EXPECT_EQ("cde", std::string(sv.c_str(), sv.length()));
}

TEST(SemanticValuesTest, TokenReturnsRequestedStoredTokenInsteadOfWholeMatch) {
  peg::SemanticValues sv;

  const std::string input = "whole-match";
  sv.ss = input.c_str();
  sv.path = "dummy";

  struct SemanticValuesAccess : peg::SemanticValues {
    void set_match_and_choice(const char* s, size_t n, size_t choice) {
      this->s_ = s;
      this->n_ = n;
      this->choice_ = choice;
    }
  };

  auto& access = reinterpret_cast<SemanticValuesAccess&>(sv);
  access.set_match_and_choice(input.c_str(), input.size(), 2);

  const char tok1[] = "first";
  const char tok2[] = "second-token";
  sv.tokens.push_back({tok1, 5});
  sv.tokens.push_back({tok2, 12});

  EXPECT_EQ("first", sv.token());
  EXPECT_EQ("second-token", sv.token(1));

  // Ensure token() prefers tokens over the whole matched string.
  EXPECT_NE(sv.str(), sv.token());
  EXPECT_EQ(2u, sv.choice());
}

TEST(SemanticValuesTest, StrCStringLengthAndLineInfoReflectMatchedRange) {
  peg::SemanticValues sv;

  const std::string input = "abc\nxyz\n12345";
  sv.ss = input.c_str();
  sv.path = "testfile";

  struct SemanticValuesAccess : peg::SemanticValues {
    void set_match(const char* s, size_t n) {
      this->s_ = s;
      this->n_ = n;
    }
  };

  auto& access = reinterpret_cast<SemanticValuesAccess&>(sv);

  // Point to "123"
  const char* match = input.c_str() + 8;
  access.set_match(match, 3);

  EXPECT_EQ(match, sv.c_str());
  EXPECT_EQ(3u, sv.length());
  EXPECT_EQ("123", sv.str());

  const auto info = sv.line_info();
  EXPECT_EQ(3u, info.first);
  EXPECT_EQ(1u, info.second);
}

TEST(SemanticValuesTest, TransformWithoutRangeConvertsAllStoredSemanticValues) {
  peg::SemanticValues sv;
  sv.push_back(peg::any(1));
  sv.push_back(peg::any(2));
  sv.push_back(peg::any(3));

  const auto values = sv.transform<int>();

  ASSERT_EQ(3u, values.size());
  EXPECT_EQ(1, values[0]);
  EXPECT_EQ(2, values[1]);
  EXPECT_EQ(3, values[2]);
}

TEST(SemanticValuesTest, TransformWithRangeUsesBeginInclusiveEndExclusive) {
  peg::SemanticValues sv;
  sv.push_back(peg::any(10));
  sv.push_back(peg::any(20));
  sv.push_back(peg::any(30));
  sv.push_back(peg::any(40));

  const auto values = sv.transform<int>(1, 3);

  ASSERT_EQ(2u, values.size());
  EXPECT_EQ(20, values[0]);
  EXPECT_EQ(30, values[1]);
}

TEST(SemanticValuesTest, TransformWithEndBeyondSizeClampsToContainerSize) {
  peg::SemanticValues sv;
  sv.push_back(peg::any(std::string("a")));
  sv.push_back(peg::any(std::string("b")));

  const auto values = sv.transform<std::string>(0, 100);

  ASSERT_EQ(2u, values.size());
  EXPECT_EQ("a", values[0]);
  EXPECT_EQ("b", values[1]);
}

TEST(SemanticValuesTest, TransformWithEmptyRangeAndEmptyContainerReturnsEmptyVector) {
  peg::SemanticValues empty_sv;
  const auto empty_values = empty_sv.transform<int>();
  EXPECT_TRUE(empty_values.empty());

  peg::SemanticValues sv;
  sv.push_back(peg::any(7));
  sv.push_back(peg::any(8));

  const auto ranged_empty = sv.transform<int>(1, 1);
  EXPECT_TRUE(ranged_empty.empty());
}

TEST(SemanticValuesTest, TransformCanPreserveConstQualifiedElementTypeAsValueType) {
  peg::SemanticValues sv;
  sv.push_back(peg::any(5));
  sv.push_back(peg::any(9));

  const auto values = sv.transform<const int>();

  ASSERT_EQ(2u, values.size());
  EXPECT_EQ(5, values[0]);
  EXPECT_EQ(9, values[1]);

  static_assert(std::is_same<decltype(values), std::vector<int>>::value,
                "transform should remove const qualification from result value type");
}

}  // namespace