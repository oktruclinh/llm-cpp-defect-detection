#include <gtest/gtest.h>
#include "fmt/format.h" // Assuming this is the correct public header

// Helper function to expose the internal logic for testing.
// In a real scenario, if this logic isn't directly exposed,
// we'd test it through functions that use it.
// For the purpose of this test, we'll simulate the context.
namespace fmt {
namespace detail {

// This is a simplified version of count_digits for testing purposes,
// as the original is likely a private helper.
// We assume it works correctly or is tested elsewhere.
inline int count_digits(uint64_t n) {
  if (n == 0) return 1;
  int count = 0;
  while (n > 0) {
    n /= 10;
    count++;
  }
  return count;
}

// This function simulates the logic under test.
// It's extracted to be testable directly.
int calculate_formatted_size(uint64_t abs_value,
                             std::string_view groups_str,
                             int sep_size) {
  int num_digits = count_digits(abs_value);
  int size = num_digits, n = num_digits;
  std::string::const_iterator group = groups_str.cbegin();
  while (group != groups_str.cend() && n > *group && *group > 0 &&
         *group != std::numeric_limits<char>::max()) {
    size += sep_size;
    n -= *group;
    ++group;
  }
  return size;
}

} // namespace detail
} // namespace fmt

TEST(FmtGroupingSizeTest, NoGrouping) {
  // No grouping characters means no separators added.
  EXPECT_EQ(fmt::detail::calculate_formatted_size(12345, "", 1), 5);
  EXPECT_EQ(fmt::detail::calculate_formatted_size(1, "", 1), 1);
  EXPECT_EQ(fmt::detail::calculate_formatted_size(0, "", 1), 1);
}

TEST(FmtGroupingSizeTest, SimpleGrouping) {
  // Grouping by 3, separator size 1
  // 123,456 -> 6 digits + 1 separator = 7
  EXPECT_EQ(fmt::detail::calculate_formatted_size(123456, "\x03", 1), 7);
  // 12,345 -> 5 digits + 1 separator = 6
  EXPECT_EQ(fmt::detail::calculate_formatted_size(12345, "\x03", 1), 6);
  // 1,234 -> 4 digits + 1 separator = 5
  EXPECT_EQ(fmt::detail::calculate_formatted_size(1234, "\x03", 1), 5);
  // 123 -> 3 digits + 0 separators = 3
  EXPECT_EQ(fmt::detail::calculate_formatted_size(123, "\x03", 1), 3);
  // 12 -> 2 digits + 0 separators = 2 (n > *group is false)
  EXPECT_EQ(fmt::detail::calculate_formatted_size(12, "\x03", 1), 2);
}

TEST(FmtGroupingSizeTest, MultipleGroupingLevels) {
  // Grouping by 3, then 2. Separator size 1.
  // 12,34,567 -> 7 digits + 2 separators = 9
  EXPECT_EQ(fmt::detail::calculate_formatted_size(1234567, "\x03\x02", 1), 9);
  // 1,23,456 -> 6 digits + 2 separators = 8
  EXPECT_EQ(fmt::detail::calculate_formatted_size(123456, "\x03\x02", 1), 8);
  // 12,345 -> 5 digits + 1 separator = 6
  EXPECT_EQ(fmt::detail::calculate_formatted_size(12345, "\x03\x02", 1), 6);
  // 1,234 -> 4 digits + 1 separator = 5
  EXPECT_EQ(fmt::detail::calculate_formatted_size(1234, "\x03\x02", 1), 5);
  // 123 -> 3 digits + 0 separators = 3
  EXPECT_EQ(fmt::detail::calculate_formatted_size(123, "\x03\x02", 1), 3);
}

TEST(FmtGroupingSizeTest, LargeNumbers) {
  // Max uint64_t, grouping by 3, sep size 1
  // 18,446,744,073,709,551,615 (20 digits)
  // 20 digits, 6 separators (after 3, 3, 3, 3, 3, 3) = 20 + 6 = 26
  EXPECT_EQ(fmt::detail::calculate_formatted_size(
                std::numeric_limits<uint64_t>::max(), "\x03", 1),
            26);

  // A number that exactly fits a group
  EXPECT_EQ(fmt::detail::calculate_formatted_size(100000, "\x03", 1), 7); // 100,000
  EXPECT_EQ(fmt::detail::calculate_formatted_size(99999, "\x03", 1), 6); // 99,999
}

TEST(FmtGroupingSizeTest, DifferentSeparatorSize) {
  // Grouping by 3, separator size 2 (e.g., " ")
  // 123 456 -> 6 digits + 1 separator * 2 chars = 8
  EXPECT_EQ(fmt::detail::calculate_formatted_size(123456, "\x03", 2), 8);
  // 12 345 678 -> 8 digits + 2 separators * 2 chars = 12
  EXPECT_EQ(fmt::detail::calculate_formatted_size(12345678, "\x03", 2), 12);
}

TEST(FmtGroupingSizeTest, ZeroValue) {
  // Zero has 1 digit, no grouping applies.
  EXPECT_EQ(fmt::detail::calculate_formatted_size(0, "\x03", 1), 1);
  EXPECT_EQ(fmt::detail::calculate_formatted_size(0, "\x02\x03", 2), 1);
}

TEST(FmtGroupingSizeTest, SingleDigitValue) {
  // Single digit, no grouping applies.
  EXPECT_EQ(fmt::detail::calculate_formatted_size(5, "\x03", 1), 1);
}

TEST(FmtGroupingSizeTest, GroupingValueEqualsNumDigits) {
  // If num_digits == *group, no separator is added.
  EXPECT_EQ(fmt::detail::calculate_formatted_size(123, "\x03", 1), 3); // n > *group is false
  EXPECT_EQ(fmt::detail::calculate_formatted_size(12345, "\x05", 1), 5); // n > *group is false
}

TEST(FmtGroupingSizeTest, GroupingValueGreaterThanNumDigits) {
  // If *group > num_digits, no separator is added.
  EXPECT_EQ(fmt::detail::calculate_formatted_size(123, "\x04", 1), 3);
}

TEST(FmtGroupingSizeTest, EmptyGroupsString) {
  // Empty groups string should result in no separators.
  EXPECT_EQ(fmt::detail::calculate_formatted_size(1234567, "", 1), 7);
}

TEST(FmtGroupingSizeTest, GroupValueZero) {
  // The condition `*group > 0` should prevent infinite loop/incorrect behavior
  // if a zero group value is encountered.
  // The loop should terminate immediately if *group is 0.
  EXPECT_EQ(fmt::detail::calculate_formatted_size(123456, "\x00\x03", 1), 6); // Should treat as no grouping
  EXPECT_EQ(fmt::detail::calculate_formatted_size(123456, "\x03\x00", 1), 7); // First group applies, second is skipped
}

TEST(FmtGroupingSizeTest, GroupValueMaxValueChar) {
  // The condition `*group != max_value<char>()` should prevent issues
  // with sentinel values or very large group sizes.
  // Assuming max_value<char>() is used as a sentinel to stop grouping.
  char sentinel_char = std::numeric_limits<char>::max();
  std::string groups_with_sentinel;
  groups_with_sentinel += '\x03';
  groups_with_sentinel += sentinel_char;
  groups_with_sentinel += '\x02'; // This should be ignored

  // 123,456 -> 6 digits + 1 separator = 7
  EXPECT_EQ(fmt::detail::calculate_formatted_size(123456, groups_with_sentinel, 1), 7);

  // If sentinel is first, no grouping
  groups_with_sentinel.clear();
  groups_with_sentinel += sentinel_char;
  groups_with_sentinel += '\x03';
  EXPECT_EQ(fmt::detail::calculate_formatted_size(123456, groups_with_sentinel, 1), 6);
}

TEST(FmtGroupingSizeTest, AllDigitsGrouped) {
  // Number of digits is exactly divisible by group size.
  // 123,456,789 -> 9 digits, 2 separators = 11
  EXPECT_EQ(fmt::detail::calculate_formatted_size(123456789, "\x03", 1), 11);
}

TEST(FmtGroupingSizeTest, RemainingDigitsLessThanNextGroup) {
  // 12,34,567 (7 digits)
  // First group: 3 digits (567), remaining 4. size += sep_size. n = 4.
  // Second group: 2 digits (34), remaining 2. size += sep_size. n = 2.
  // Third group: 3 digits (12), but n (2) < *group (3). Loop terminates.
  EXPECT_EQ(fmt::detail::calculate_formatted_size(1234567, "\x03\x02\x03", 1), 9);
}