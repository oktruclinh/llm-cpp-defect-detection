#include <gtest/gtest.h>
#include "arrow/compute/function.h"

namespace arrow {
namespace compute {

TEST(ArityTest, TernaryFunction) {
  // Test that Ternary() creates an Arity with 3 arguments and is not variadic
  Arity arity = Arity::Ternary();
  
  // Verify the arity count is exactly 3
  EXPECT_EQ(arity.num_args(), 3);
  
  // Verify it's not variadic
  EXPECT_FALSE(arity.is_variadic());
}

TEST(ArityTest, VarArgsFunction) {
  // Test default VarArgs (min_args = 0)
  Arity arity_default = Arity::VarArgs();
  
  // Verify default min_args is 0
  EXPECT_EQ(arity_default.num_args(), 0);
  
  // Verify it's variadic
  EXPECT_TRUE(arity_default.is_variadic());
  
  // Test VarArgs with specific min_args
  Arity arity_custom = Arity::VarArgs(5);
  
  // Verify custom min_args is set correctly
  EXPECT_EQ(arity_custom.num_args(), 5);
  
  // Verify it's still variadic
  EXPECT_TRUE(arity_custom.is_variadic());
  
  // Test edge case with min_args = 1
  Arity arity_one = Arity::VarArgs(1);
  EXPECT_EQ(arity_one.num_args(), 1);
  EXPECT_TRUE(arity_one.is_variadic());
  
  // Test edge case with min_args = 100
  Arity arity_hundred = Arity::VarArgs(100);
  EXPECT_EQ(arity_hundred.num_args(), 100);
  EXPECT_TRUE(arity_hundred.is_variadic());
}

TEST(ArityTest, ArityComparison) {
  // Test that different Arity objects can be compared correctly
  Arity ternary = Arity::Ternary();
  Arity varargs_zero = Arity::VarArgs(0);
  Arity varargs_five = Arity::VarArgs(5);
  
  // Ternary should not equal varargs
  EXPECT_NE(ternary, varargs_zero);
  EXPECT_NE(ternary, varargs_five);
  
  // VarArgs with different min_args should not be equal
  EXPECT_NE(varargs_zero, varargs_five);
  
  // Same varargs should be equal
  Arity varargs_zero_copy = Arity::VarArgs(0);
  EXPECT_EQ(varargs_zero, varargs_zero_copy);
  
  // Ternary should be equal to itself
  Arity ternary_copy = Arity::Ternary();
  EXPECT_EQ(ternary, ternary_copy);
}

TEST(ArityTest, ArityProperties) {
  // Test that Arity properties are correctly set for Ternary
  Arity ternary = Arity::Ternary();
  EXPECT_EQ(ternary.num_args(), 3);
  EXPECT_FALSE(ternary.is_variadic());
  
  // Test that Arity properties are correctly set for VarArgs
  Arity varargs = Arity::VarArgs(2);
  EXPECT_EQ(varargs.num_args(), 2);
  EXPECT_TRUE(varargs.is_variadic());
  
  // Test that Arity properties are correctly set for VarArgs with zero min_args
  Arity varargs_zero = Arity::VarArgs(0);
  EXPECT_EQ(varargs_zero.num_args(), 0);
  EXPECT_TRUE(varargs_zero.is_variadic());
}

}  // namespace compute
}  // namespace arrow