#include <gtest/gtest.h>
#include "arrow/compute/function.h"

namespace arrow {
namespace compute {

TEST(ArityTest, Ternary) {
  // Test that Ternary() returns an arity with exactly 3 arguments and is not variable
  Arity arity = Arity::Ternary();
  EXPECT_EQ(arity.num_args, 3);
  EXPECT_FALSE(arity.is_varargs);
}

TEST(ArityTest, VarArgsDefault) {
  // Test VarArgs with default min_args = 0
  Arity arity = Arity::VarArgs();
  EXPECT_EQ(arity.num_args, 0);
  EXPECT_TRUE(arity.is_varargs);
}

TEST(ArityTest, VarArgsPositiveMinArgs) {
  // Test VarArgs with positive min_args
  Arity arity = Arity::VarArgs(2);
  EXPECT_EQ(arity.num_args, 2);
  EXPECT_TRUE(arity.is_varargs);
}

TEST(ArityTest, VarArgsLargeMinArgs) {
  // Test VarArgs with a large min_args value
  Arity arity = Arity::VarArgs(100);
  EXPECT_EQ(arity.num_args, 100);
  EXPECT_TRUE(arity.is_varargs);
}

TEST(ArityTest, VarArgsZeroMinArgs) {
  // Test VarArgs with explicit zero min_args
  Arity arity = Arity::VarArgs(0);
  EXPECT_EQ(arity.num_args, 0);
  EXPECT_TRUE(arity.is_varargs);
}

TEST(ArityTest, TernaryIsNotVarArgs) {
  // Verify that Ternary is not variable arguments
  Arity ternary_arity = Arity::Ternary();
  EXPECT_FALSE(ternary_arity.is_varargs);
}

TEST(ArityTest, VarArgsIsNotFixed) {
  // Verify that VarArgs is not a fixed arity
  Arity var_arity = Arity::VarArgs(1);
  EXPECT_TRUE(var_arity.is_varargs);
}

TEST(ArityTest, TernaryNumArgsIsExactlyThree) {
  // Verify that Ternary always has exactly 3 arguments
  Arity arity = Arity::Ternary();
  EXPECT_EQ(arity.num_args, 3);
}

TEST(ArityTest, VarArgsMinArgsPreserved) {
  // Verify that the min_args value is preserved in num_args
  Arity arity = Arity::VarArgs(5);
  EXPECT_EQ(arity.num_args, 5);
}

TEST(ArityTest, VarArgsNegativeMinArgs) {
  // Test VarArgs with negative min_args (should still store the value)
  Arity arity = Arity::VarArgs(-1);
  EXPECT_EQ(arity.num_args, -1);
  EXPECT_TRUE(arity.is_varargs);
}

}  // namespace compute
}  // namespace arrow