#include <gtest/gtest.h>

#include "arrow/compute/function.h"

namespace arrow {
namespace compute {

TEST(Arity, Ternary) {
  // Test the Ternary static method
  Arity ternary_arity = Arity::Ternary();

  // Ternary functions should have exactly 3 arguments and not be variadic
  ASSERT_EQ(ternary_arity.num_args(), 3);
  ASSERT_FALSE(ternary_arity.is_variadic());
}

TEST(Arity, VarArgs_DefaultMinArgs) {
  // Test VarArgs with default min_args (0)
  Arity varargs_default = Arity::VarArgs();

  // Should be variadic with a minimum of 0 arguments
  ASSERT_EQ(varargs_default.num_args(), 0);
  ASSERT_TRUE(varargs_default.is_variadic());
}

TEST(Arity, VarArgs_PositiveMinArgs) {
  // Test VarArgs with a positive minimum number of arguments
  int min_args = 2;
  Arity varargs_positive = Arity::VarArgs(min_args);

  // Should be variadic with the specified minimum number of arguments
  ASSERT_EQ(varargs_positive.num_args(), min_args);
  ASSERT_TRUE(varargs_positive.is_variadic());
}

TEST(Arity, VarArgs_ZeroMinArgs) {
  // Test VarArgs with an explicit zero minimum number of arguments
  int min_args = 0;
  Arity varargs_zero = Arity::VarArgs(min_args);

  // Should be variadic with a minimum of 0 arguments
  ASSERT_EQ(varargs_zero.num_args(), min_args);
  ASSERT_TRUE(varargs_zero.is_variadic());
}

TEST(Arity, VarArgs_LargeMinArgs) {
  // Test VarArgs with a relatively large minimum number of arguments
  int min_args = 100;
  Arity varargs_large = Arity::VarArgs(min_args);

  // Should be variadic with the specified minimum number of arguments
  ASSERT_EQ(varargs_large.num_args(), min_args);
  ASSERT_TRUE(varargs_large.is_variadic());
}

}  // namespace compute
}  // namespace arrow