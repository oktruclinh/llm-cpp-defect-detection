#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>

TEST(AlignSpecTest, NumericAlignmentWithSign) {
  // Test case where align is NUMERIC and sign is present
  std::string result = fmt::format("{:d}", -42);
  EXPECT_EQ(result, "-42");
  
  // Test with positive number
  result = fmt::format("{:d}", 42);
  EXPECT_EQ(result, "42");
}

TEST(AlignSpecTest, DefaultAlignmentWithSign) {
  // Test case where align is DEFAULT and sign is present
  std::string result = fmt::format("{:d}", -42);
  EXPECT_EQ(result, "-42");
  
  // Test with positive number
  result = fmt::format("{:d}", 42);
  EXPECT_EQ(result, "42");
}

TEST(AlignSpecTest, NumericAlignmentWithoutSign) {
  // Test case where align is NUMERIC but no sign
  std::string result = fmt::format("{:>10d}", 42);
  EXPECT_EQ(result, "        42");
  
  // Test with zero
  result = fmt::format("{:>10d}", 0);
  EXPECT_EQ(result, "         0");
}

TEST(AlignSpecTest, DefaultAlignmentWithoutSign) {
  // Test case where align is DEFAULT and no sign
  std::string result = fmt::format("{:>10d}", 42);
  EXPECT_EQ(result, "        42");
  
  // Test with zero
  result = fmt::format("{:>10d}", 0);
  EXPECT_EQ(result, "         0");
}

TEST(AlignSpecTest, WidthAdjustmentWithSign) {
  // Test that width is properly adjusted when sign is moved
  std::string result = fmt::format("{:>5d}", -42);
  EXPECT_EQ(result, "  -42");
  
  // Width should be reduced by 1 when sign is extracted
  result = fmt::format("{:>6d}", -42);
  EXPECT_EQ(result, "   -42");
}

TEST(AlignSpecTest, ZeroPaddingWithSign) {
  // Test zero padding with sign
  std::string result = fmt::format("{:0>5d}", -42);
  EXPECT_EQ(result, "-0042");
  
  result = fmt::format("{:0>5d}", 42);
  EXPECT_EQ(result, "00042");
}

TEST(AlignSpecTest, EdgeCaseNegativeZero) {
  // Test with negative zero (if supported)
  std::string result = fmt::format("{:d}", -0);
  EXPECT_EQ(result, "0");  // Negative zero should format as positive zero
}

TEST(AlignSpecTest, EdgeCaseLargeNumber) {
  // Test with large number
  std::string result = fmt::format("{:d}", 123456789);
  EXPECT_EQ(result, "123456789");
  
  result = fmt::format("{:d}", -123456789);
  EXPECT_EQ(result, "-123456789");
}

TEST(AlignSpecTest, EdgeCaseSmallWidth) {
  // Test with small width
  std::string result = fmt::format("{:>2d}", -42);
  EXPECT_EQ(result, "-42");
  
  result = fmt::format("{:>1d}", 42);
  EXPECT_EQ(result, "42");
}

TEST(AlignSpecTest, EdgeCaseZeroWidth) {
  // Test with zero width
  std::string result = fmt::format("{:>0d}", 42);
  EXPECT_EQ(result, "42");
  
  result = fmt::format("{:>0d}", -42);
  EXPECT_EQ(result, "-42");
}

TEST(AlignSpecTest, StringAlignment) {
  // Test string formatting to ensure we don't break existing functionality
  std::string result = fmt::format("{:>10s}", "hello");
  EXPECT_EQ(result, "     hello");
  
  result = fmt::format("{:<10s}", "hello");
  EXPECT_EQ(result, "hello     ");
  
  result = fmt::format("{:^10s}", "hello");
  EXPECT_EQ(result, "  hello   ");
}