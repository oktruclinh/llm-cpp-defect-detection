#include <gtest/gtest.h>
#include "fmt/format.h" // Assuming this is the correct public header

// Helper function to simulate the context where the focal code snippet
// would be executed. This is a simplified version for testing purposes.
// The focal code is part of a larger formatting logic, specifically
// within the `format_handler::on_align` or similar method.
// We'll create a mock or simplified structure to test its behavior.

// This struct mimics the `align_spec` used in fmtlib.
struct align_spec {
  enum Align { ALIGN_DEFAULT, ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER, ALIGN_NUMERIC };
  Align align_ : 4;
  unsigned width_ : 28;
  char fill_;

  align_spec() : align_(ALIGN_DEFAULT), width_(0), fill_(' ') {}

  Align align() const { return align_; }
  void set_align(Align a) { align_ = a; }
  void set_width(unsigned w) { width_ = w; }
};

// This function encapsulates the focal logic for testing.
// It takes `spec` by value to simulate the `align_spec as = spec;` line.
// It returns the modified `align_spec` and the potentially modified `sign`
// and `n` values, as these are outputs of the focal snippet.
struct FocalResult {
  align_spec as;
  char sign;
  size_t n;
};

FocalResult apply_focal_logic(align_spec spec, char initial_sign, size_t initial_n) {
  align_spec as = spec;
  char sign = initial_sign;
  size_t n = initial_n;

  // Simulate `reserve` and `*it++ = sign;` by just modifying `sign` and `as.width_`
  // as the actual buffer manipulation is outside the scope of this snippet's
  // direct effect on `as`, `sign`, and `n`.
  // The `reserve` and `*it++ = sign;` part implies `sign` is consumed.

  if (spec.align() == align_spec::ALIGN_NUMERIC) {
    if (sign) {
      // Simulate `auto &&it = reserve(1); *it++ = sign;`
      sign = 0; // Sign is "consumed" or written
      if (as.width_)
        --as.width_;
    }
    as.align_ = align_spec::ALIGN_RIGHT;
  } else {
    if (spec.align() == align_spec::ALIGN_DEFAULT)
      as.align_ = align_spec::ALIGN_RIGHT;
    if (sign)
      ++n;
  }

  return {as, sign, n};
}

TEST(FocalFunctionTest, NumericAlignWithSignAndWidth) {
  align_spec spec;
  spec.set_align(align_spec::ALIGN_NUMERIC);
  spec.set_width(10);
  char initial_sign = '+';
  size_t initial_n = 5;

  FocalResult result = apply_focal_logic(spec, initial_sign, initial_n);

  // Expect ALIGN_RIGHT, width decremented, sign consumed, n unchanged
  EXPECT_EQ(result.as.align(), align_spec::ALIGN_RIGHT);
  EXPECT_EQ(result.as.width_, 9);
  EXPECT_EQ(result.sign, 0); // Sign should be consumed
  EXPECT_EQ(result.n, 5);    // n should be unchanged for numeric align
}

TEST(FocalFunctionTest, NumericAlignWithSignNoWidth) {
  align_spec spec;
  spec.set_align(align_spec::ALIGN_NUMERIC);
  spec.set_width(0); // No width
  char initial_sign = '-';
  size_t initial_n = 5;

  FocalResult result = apply_focal_logic(spec, initial_sign, initial_n);

  // Expect ALIGN_RIGHT, width unchanged (was 0), sign consumed, n unchanged
  EXPECT_EQ(result.as.align(), align_spec::ALIGN_RIGHT);
  EXPECT_EQ(result.as.width_, 0);
  EXPECT_EQ(result.sign, 0);
  EXPECT_EQ(result.n, 5);
}

TEST(FocalFunctionTest, NumericAlignNoSign) {
  align_spec spec;
  spec.set_align(align_spec::ALIGN_NUMERIC);
  spec.set_width(10);
  char initial_sign = 0; // No sign
  size_t initial_n = 5;

  FocalResult result = apply_focal_logic(spec, initial_sign, initial_n);

  // Expect ALIGN_RIGHT, width unchanged, sign unchanged, n unchanged
  EXPECT_EQ(result.as.align(), align_spec::ALIGN_RIGHT);
  EXPECT_EQ(result.as.width_, 10);
  EXPECT_EQ(result.sign, 0);
  EXPECT_EQ(result.n, 5);
}

TEST(FocalFunctionTest, DefaultAlignWithSign) {
  align_spec spec;
  spec.set_align(align_spec::ALIGN_DEFAULT); // Default align
  spec.set_width(10);
  char initial_sign = '+';
  size_t initial_n = 5;

  FocalResult result = apply_focal_logic(spec, initial_sign, initial_n);

  // Expect ALIGN_RIGHT, width unchanged, sign unchanged, n incremented
  EXPECT_EQ(result.as.align(), align_spec::ALIGN_RIGHT);
  EXPECT_EQ(result.as.width_, 10);
  EXPECT_EQ(result.sign, '+'); // Sign is NOT consumed here
  EXPECT_EQ(result.n, 6);      // n should be incremented
}

TEST(FocalFunctionTest, DefaultAlignNoSign) {
  align_spec spec;
  spec.set_align(align_spec::ALIGN_DEFAULT);
  spec.set_width(10);
  char initial_sign = 0; // No sign
  size_t initial_n = 5;

  FocalResult result = apply_focal_logic(spec, initial_sign, initial_n);

  // Expect ALIGN_RIGHT, width unchanged, sign unchanged, n unchanged
  EXPECT_EQ(result.as.align(), align_spec::ALIGN_RIGHT);
  EXPECT_EQ(result.as.width_, 10);
  EXPECT_EQ(result.sign, 0);
  EXPECT_EQ(result.n, 5);
}

TEST(FocalFunctionTest, LeftAlignWithSign) {
  align_spec spec;
  spec.set_align(align_spec::ALIGN_LEFT); // Explicit Left align
  spec.set_width(10);
  char initial_sign = '-';
  size_t initial_n = 5;

  FocalResult result = apply_focal_logic(spec, initial_sign, initial_n);

  // Expect ALIGN_LEFT (unchanged), width unchanged, sign unchanged, n incremented
  EXPECT_EQ(result.as.align(), align_spec::ALIGN_LEFT);
  EXPECT_EQ(result.as.width_, 10);
  EXPECT_EQ(result.sign, '-'); // Sign is NOT consumed here
  EXPECT_EQ(result.n, 6);      // n should be incremented
}

TEST(FocalFunctionTest, LeftAlignNoSign) {
  align_spec spec;
  spec.set_align(align_spec::ALIGN_LEFT);
  spec.set_width(10);
  char initial_sign = 0;
  size_t initial_n = 5;

  FocalResult result = apply_focal_logic(spec, initial_sign, initial_n);

  // Expect ALIGN_LEFT (unchanged), width unchanged, sign unchanged, n unchanged
  EXPECT_EQ(result.as.align(), align_spec::ALIGN_LEFT);
  EXPECT_EQ(result.as.width_, 10);
  EXPECT_EQ(result.sign, 0);
  EXPECT_EQ(result.n, 5);
}

TEST(FocalFunctionTest, RightAlignWithSign) {
  align_spec spec;
  spec.set_align(align_spec::ALIGN_RIGHT); // Explicit Right align
  spec.set_width(10);
  char initial_sign = '+';
  size_t initial_n = 5;

  FocalResult result = apply_focal_logic(spec, initial_sign, initial_n);

  // Expect ALIGN_RIGHT (unchanged), width unchanged, sign unchanged, n incremented
  EXPECT_EQ(result.as.align(), align_spec::ALIGN_RIGHT);
  EXPECT_EQ(result.as.width_, 10);
  EXPECT_EQ(result.sign, '+'); // Sign is NOT consumed here
  EXPECT_EQ(result.n, 6);      // n should be incremented
}

TEST(FocalFunctionTest, RightAlignNoSign) {
  align_spec spec;
  spec.set_align(align_spec::ALIGN_RIGHT);
  spec.set_width(10);
  char initial_sign = 0;
  size_t initial_n = 5;

  FocalResult result = apply_focal_logic(spec, initial_sign, initial_n);

  // Expect ALIGN_RIGHT (unchanged), width unchanged, sign unchanged, n unchanged
  EXPECT_EQ(result.as.align(), align_spec::ALIGN_RIGHT);
  EXPECT_EQ(result.as.width_, 10);
  EXPECT_EQ(result.sign, 0);
  EXPECT_EQ(result.n, 5);
}

TEST(FocalFunctionTest, CenterAlignWithSign) {
  align_spec spec;
  spec.set_align(align_spec::ALIGN_CENTER); // Explicit Center align
  spec.set_width(10);
  char initial_sign = '+';
  size_t initial_n = 5;

  FocalResult result = apply_focal_logic(spec, initial_sign, initial_n);

  // Expect ALIGN_CENTER (unchanged), width unchanged, sign unchanged, n incremented
  EXPECT_EQ(result.as.align(), align_spec::ALIGN_CENTER);
  EXPECT_EQ(result.as.width_, 10);
  EXPECT_EQ(result.sign, '+'); // Sign is NOT consumed here
  EXPECT_EQ(result.n, 6);      // n should be incremented
}

TEST(FocalFunctionTest, CenterAlignNoSign) {
  align_spec spec;
  spec.set_align(align_spec::ALIGN_CENTER);
  spec.set_width(10);
  char initial_sign = 0;
  size_t initial_n = 5;

  FocalResult result = apply_focal_logic(spec, initial_sign, initial_n);

  // Expect ALIGN_CENTER (unchanged), width unchanged, sign unchanged, n unchanged
  EXPECT_EQ(result.as.align(), align_spec::ALIGN_CENTER);
  EXPECT_EQ(result.as.width_, 10);
  EXPECT_EQ(result.sign, 0);
  EXPECT_EQ(result.n, 5);
}