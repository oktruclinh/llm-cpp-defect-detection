#include <gtest/gtest.h>
#include <fmt/format.h> // Public header for fmtlib

// Helper function to expose the internal logic for testing.
// In a real scenario, we'd test this through public APIs that use it.
// For this specific snippet, we'll simulate its context.
namespace detail {
// This is a simplified version of count_digits for testing purposes,
// as the original snippet uses it but doesn't define it.
inline int count_digits(uint64_t n) {
  if (n == 0) return 1;
  int count = 0;
  while (n > 0) {
    n /= 10;
    count++;
  }
  return count;
}

// This function simulates the logic of the provided snippet.
// It calculates the 'size' based on 'num_digits' and 'groups'.
// The original snippet is part of a larger formatting logic,
// likely related to number grouping/thousands separators.
int calculate_grouped_size(uint64_t abs_value,
                           const std::string& groups,
                           int sep_size) {
  int num_digits = count_digits(abs_value);
  int size = num_digits, n = num_digits;
  std::string::const_iterator group = groups.cbegin();
  while (group != groups.cend() && n > *group && *group > 0 &&
         *group != std::numeric_limits<char>::max()) {
    size += sep_size;
    n -= *group;
    ++group;
  }
  return size;
}
} // namespace detail

// Test fixture for grouping size calculation
class GroupedSizeTest : public ::testing::Test {
protected:
  // You can set up common resources here if needed
};

// Test case 1: No grouping (empty groups string)
TEST_F(GroupedSizeTest, NoGrouping) {
  EXPECT_EQ(detail::calculate_grouped_size(12345, "", 1), 5);
  EXPECT_EQ(detail::calculate_grouped_size(0, "", 1), 1);
  EXPECT_EQ(detail::calculate_grouped_size(1, "", 1), 1);
  EXPECT_EQ(detail::calculate_grouped_size(1234567890, "", 1), 10);
}

// Test case 2: Standard grouping (e.g., "3" for thousands)
TEST_F(GroupedSizeTest, StandardGrouping) {
  // abs_value = 12345, groups = "3", sep_size = 1 (e.g., comma)
  // num_digits = 5
  // Initial: size = 5, n = 5
  // Loop 1: group = '3'. n (5) > *group (3). size = 5 + 1 = 6. n = 5 - 3 = 2.
  // Loop 2: group is still '3'. n (2) NOT > *group (3). Loop terminates.
  // Expected: 6 (12,345)
  EXPECT_EQ(detail::calculate_grouped_size(12345, "3", 1), 6);

  // abs_value = 1234567, groups = "3", sep_size = 1
  // num_digits = 7
  // Initial: size = 7, n = 7
  // Loop 1: group = '3'. n (7) > *group (3). size = 7 + 1 = 8. n = 7 - 3 = 4.
  // Loop 2: group is still '3'. n (4) > *group (3). size = 8 + 1 = 9. n = 4 - 3 = 1.
  // Loop 3: group is still '3'. n (1) NOT > *group (3). Loop terminates.
  // Expected: 9 (1,234,567)
  EXPECT_EQ(detail::calculate_grouped_size(1234567, "3", 1), 9);

  // abs_value = 123, groups = "3", sep_size = 1
  // num_digits = 3
  // Initial: size = 3, n = 3
  // Loop 1: group = '3'. n (3) NOT > *group (3). Loop terminates.
  // Expected: 3 (123)
  EXPECT_EQ(detail::calculate_grouped_size(123, "3", 1), 3);
}

// Test case 3: Multiple grouping characters (e.g., "32" for Indian numbering system)
TEST_F(GroupedSizeTest, MultipleGrouping) {
  // abs_value = 123456789, groups = "32", sep_size = 1
  // num_digits = 9
  // Initial: size = 9, n = 9
  // Loop 1: group = '3'. n (9) > *group (3). size = 9 + 1 = 10. n = 9 - 3 = 6.
  // Loop 2: group = '2'. n (6) > *group (2). size = 10 + 1 = 11. n = 6 - 2 = 4.
  // Loop 3: group is still '2'. n (4) > *group (2). size = 11 + 1 = 12. n = 4 - 2 = 2.
  // Loop 4: group is still '2'. n (2) NOT > *group (2). Loop terminates.
  // Expected: 12 (12,34,56,789) - Note: fmtlib's actual behavior might be different for repeating groups.
  // The snippet's logic implies repeating the last group if 'n' is still large enough.
  EXPECT_EQ(detail::calculate_grouped_size(123456789, "32", 1), 12);

  // abs_value = 12345, groups = "32", sep_size = 1
  // num_digits = 5
  // Initial: size = 5, n = 5
  // Loop 1: group = '3'. n (5) > *group (3). size = 5 + 1 = 6. n = 5 - 3 = 2.
  // Loop 2: group = '2'. n (2) NOT > *group (2). Loop terminates.
  // Expected: 6 (12,345)
  EXPECT_EQ(detail::calculate_grouped_size(12345, "32", 1), 6);
}

// Test case 4: Different separator sizes
TEST_F(GroupedSizeTest, DifferentSeparatorSize) {
  // abs_value = 1234567, groups = "3", sep_size = 2 (e.g., " ")
  // Expected: 7 + 2 + 2 = 11 (1 234 567)
  EXPECT_EQ(detail::calculate_grouped_size(1234567, "3", 2), 11);

  // abs_value = 123456789, groups = "32", sep_size = 3 (e.g., "---")
  // Expected: 9 + 3 + 3 + 3 = 18 (12---34---56---789)
  EXPECT_EQ(detail::calculate_grouped_size(123456789, "32", 3), 18);
}

// Test case 5: Edge case - abs_value is 0
TEST_F(GroupedSizeTest, ZeroValue) {
  EXPECT_EQ(detail::calculate_grouped_size(0, "3", 1), 1); // "0"
  EXPECT_EQ(detail::calculate_grouped_size(0, "32", 2), 1); // "0"
}

// Test case 6: Edge case - abs_value has fewer digits than first group
TEST_F(GroupedSizeTest, FewerDigitsThanFirstGroup) {
  EXPECT_EQ(detail::calculate_grouped_size(12, "3", 1), 2); // "12"
  EXPECT_EQ(detail::calculate_grouped_size(123, "4", 1), 3); // "123"
}

// Test case 7: Edge case - group value is 0 (should terminate loop)
TEST_F(GroupedSizeTest, ZeroGroupValue) {
  // abs_value = 123456, groups = "30", sep_size = 1
  // num_digits = 6
  // Initial: size = 6, n = 6
  // Loop 1: group = '3'. n (6) > *group (3). size = 6 + 1 = 7. n = 6 - 3 = 3.
  // Loop 2: group = '0'. *group (0) is not > 0. Loop terminates.
  // Expected: 7 (123,456)
  EXPECT_EQ(detail::calculate_grouped_size(123456, "30", 1), 7);
}

// Test case 8: Edge case - group value is max_value<char>() (should terminate loop)
TEST_F(GroupedSizeTest, MaxCharGroupValue) {
  // This simulates a sentinel value that should stop grouping.
  // In fmtlib, this is often used to indicate "no more grouping".
  std::string groups_with_sentinel;
  groups_with_sentinel += '3';
  groups_with_sentinel += std::numeric_limits<char>::max();

  // abs_value = 1234567, groups = "3<MAX_CHAR>", sep_size = 1
  // num_digits = 7
  // Initial: size = 7, n = 7
  // Loop 1: group = '3'. n (7) > *group (3). size = 7 + 1 = 8. n = 7 - 3 = 4.
  // Loop 2: group = MAX_CHAR. *group (MAX_CHAR) is equal to max_value<char>(). Loop terminates.
  // Expected: 8 (1,234,567)
  EXPECT_EQ(detail::calculate_grouped_size(1234567, groups_with_sentinel, 1), 8);
}

// Test case 9: Large number, standard grouping
TEST_F(GroupedSizeTest, LargeNumberStandardGrouping) {
  // 1,000,000,000,000,000,000 (19 digits)
  uint64_t large_num = 1000000000000000000ULL;
  // 19 digits, 6 separators for "3" grouping
  // 19 + 6 = 25
  EXPECT_EQ(detail::calculate_grouped_size(large_num, "3", 1), 25);
}

// Test case 10: Large number, complex grouping
TEST_F(GroupedSizeTest, LargeNumberComplexGrouping) {
  // 123,45,67,89,01,23,45,678 (18 digits)
  uint64_t large_num = 123456789012345678ULL;
  // num_digits = 18
  // groups = "32"
  // Initial: size = 18, n = 18
  // Loop 1: group = '3'. n (18) > 3. size = 18 + 1 = 19. n = 18 - 3 = 15.
  // Loop 2: group = '2'. n (15) > 2. size = 19 + 1 = 20. n = 15 - 2 = 13.
  // Loop 3: group = '2'. n (13) > 2. size = 20 + 1 = 21. n = 13 - 2 = 11.
  // Loop 4: group = '2'. n (11) > 2. size = 21 + 1 = 22. n = 11 - 2 = 9.
  // Loop 5: group = '2'. n (9) > 2. size = 22 + 1 = 23. n = 9 - 2 = 7.
  // Loop 6: group = '2'. n (7) > 2. size = 23 + 1 = 24. n = 7 - 2 = 5.
  // Loop 7: group = '2'. n (5) > 2. size = 24 + 1 = 25. n = 5 - 2 = 3.
  // Loop 8: group = '2'. n (3) > 2. size = 25 + 1 = 26. n = 3 - 2 = 1.
  // Loop 9: group = '2'. n (1) NOT > 2. Loop terminates.
  // Expected: 26
  EXPECT_EQ(detail::calculate_grouped_size(large_num, "32", 1), 26);
}

// Test case 11: Grouping with sep_size = 0 (no actual separator added)
TEST_F(GroupedSizeTest, ZeroSeparatorSize) {
  EXPECT_EQ(detail::calculate_grouped_size(12345, "3", 0), 5);
  EXPECT_EQ(detail::calculate_grouped_size(1234567, "32", 0), 7);
}

// Test case 12: Groups string contains non-digit characters (behavior undefined by snippet, but good to test)
// The snippet uses *group directly as an int, so non-digit chars will be their ASCII values.
// This test assumes the 'groups' string contains valid digit characters '0'-'9'
// as per fmtlib's typical usage for grouping. If it contained other chars,
// the behavior would be based on their ASCII values.
TEST_F(GroupedSizeTest, InvalidGroupCharacters) {
  // If 'groups' contains 'A' (ASCII 65), it will be treated as a group of 65.
  // This is likely an invalid input for fmtlib's intended use, but the snippet
  // itself would process it.
  // abs_value = 12345, groups = "A", sep_size = 1
  // num_digits = 5
  // Initial: size = 5, n = 5
  // Loop 1: group = 'A' (65). n (5) NOT > *group (65). Loop terminates.
  EXPECT_EQ(detail::calculate_grouped_size(12345, "A", 1), 5);

  // abs_value = 123456789, groups = "1", sep_size = 1
  // num_digits = 9
  // Initial: size = 9, n = 9
  // Loop 1: group = '1'. n (9) > 1. size = 9 + 1 = 10. n = 9 - 1 = 8.
  // Loop 2: group = '1'. n (8) > 1. size = 10 + 1 = 11. n = 8 - 1 = 7.
  // ... this would continue until n is 1.
  // 8 separators added. 9 + 8 = 17.
  EXPECT_EQ(detail::calculate_grouped_size(123456789, "1", 1), 17);
}