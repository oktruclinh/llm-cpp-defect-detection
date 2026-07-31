#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

TEST(FormatTest, GroupingWithSeparators) {
  // Test normal case with grouping
  std::string result = fmt::format("{:L}", 1234567);
  EXPECT_EQ(result, "1,234,567");
  
  // Test with custom grouping
  std::vector<int> groups = {3, 2};
  std::string result2 = fmt::format("{:L}", 123456789);
  EXPECT_EQ(result2, "12,34,56,789");
  
  // Test zero value
  std::string result3 = fmt::format("{:L}", 0);
  EXPECT_EQ(result3, "0");
  
  // Test negative value
  std::string result4 = fmt::format("{:L}", -1234567);
  EXPECT_EQ(result4, "-1,234,567");
  
  // Test single digit
  std::string result5 = fmt::format("{:L}", 5);
  EXPECT_EQ(result5, "5");
  
  // Test exact grouping boundary
  std::string result6 = fmt::format("{:L}", 123456);
  EXPECT_EQ(result6, "123,456");
  
  // Test with different separator
  std::string result7 = fmt::format("{:L}", 1234567890);
  EXPECT_EQ(result7, "1,234,567,890");
  
  // Test large number
  std::string result8 = fmt::format("{:L}", 123456789012345LL);
  EXPECT_EQ(result8, "123,456,789,012,345");
}

TEST(FormatTest, GroupingEdgeCases) {
  // Test with empty grouping (should not add separators)
  std::string result = fmt::format("{:L}", 1234567);
  EXPECT_EQ(result, "1,234,567");
  
  // Test with single group
  std::string result2 = fmt::format("{:L}", 123456789);
  EXPECT_EQ(result2, "123,456,789");
  
  // Test with group size of 1
  std::string result3 = fmt::format("{:L}", 1234567890);
  EXPECT_EQ(result3, "1,234,567,890");
  
  // Test with group size larger than number
  std::string result4 = fmt::format("{:L}", 123);
  EXPECT_EQ(result4, "123");
  
  // Test with very large number
  std::string result5 = fmt::format("{:L}", 999999999999999LL);
  EXPECT_EQ(result5, "999,999,999,999,999");
}