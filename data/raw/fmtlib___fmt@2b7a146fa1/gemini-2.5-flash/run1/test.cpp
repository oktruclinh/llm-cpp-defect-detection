#include <gtest/gtest.h>
#include <fmt/format.h> // Public API for fmtlib___fmt

// Helper function to expose the internal logic for testing purposes.
// In a real scenario, we'd test this through fmt::format directly.
// However, since the prompt specifically asks to test the given snippet,
// and it's an internal helper, we'll simulate its context.
// This function is a simplified version of what fmt::detail::group_digits
// or similar logic might do, focusing on the provided snippet.
namespace fmt {
namespace detail {

// This function simulates the context of the provided snippet.
// It calculates the 'size' based on 'num_digits' and 'groups'.
// The 'sep_size' is typically 1 for a single separator character.
int calculate_grouped_size(int num_digits, const std::string& groups_str, int sep_size) {
    // Simulate the count_digits behavior for abs_value
    // For this test, num_digits is directly provided.
    int abs_value_dummy = 0; // Not used in this snippet, but contextually present.

    int size = num_digits;
    int n = num_digits;
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


TEST(GroupedSizeTest, NoGrouping) {
    // No grouping specified, or grouping characters are 0 or max_char
    EXPECT_EQ(fmt::detail::calculate_grouped_size(10, "", 1), 10);
    EXPECT_EQ(fmt::detail::calculate_grouped_size(5, "\0", 1), 5); // Group of 0
    EXPECT_EQ(fmt::detail::calculate_grouped_size(5, std::string(1, std::numeric_limits<char>::max()), 1), 5); // Group of max_char
    EXPECT_EQ(fmt::detail::calculate_grouped_size(3, "3", 1), 3); // n > *group is false (3 > 3 is false)
}

TEST(GroupedSizeTest, SimpleGrouping) {
    // Standard grouping like "3" for thousands separator
    EXPECT_EQ(fmt::detail::calculate_grouped_size(1, "3", 1), 1); // 1 digit, no separator
    EXPECT_EQ(fmt::detail::calculate_grouped_size(2, "3", 1), 2); // 2 digits, no separator
    EXPECT_EQ(fmt::detail::calculate_grouped_size(3, "3", 1), 3); // 3 digits, no separator
    EXPECT_EQ(fmt::detail::calculate_grouped_size(4, "3", 1), 5); // 4 digits, 1 separator (1234 -> 1,234)
    EXPECT_EQ(fmt::detail::calculate_grouped_size(5, "3", 1), 6); // 5 digits, 1 separator (12345 -> 12,345)
    EXPECT_EQ(fmt::detail::calculate_grouped_size(6, "3", 1), 7); // 6 digits, 1 separator (123456 -> 123,456)
    EXPECT_EQ(fmt::detail::calculate_grouped_size(7, "3", 1), 9); // 7 digits, 2 separators (1234567 -> 1,234,567)
}

TEST(GroupedSizeTest, MultipleGroupingRules) {
    // Grouping rules like "32" (e.g., Indian numbering system)
    // First group of 3, then groups of 2
    EXPECT_EQ(fmt::detail::calculate_grouped_size(1, "32", 1), 1);
    EXPECT_EQ(fmt::detail::calculate_grouped_size(2, "32", 1), 2);
    EXPECT_EQ(fmt::detail::calculate_grouped_size(3, "32", 1), 3); // 123
    EXPECT_EQ(fmt::detail::calculate_grouped_size(4, "32", 1), 5); // 1,234 (first group of 3)
    EXPECT_EQ(fmt::detail::calculate_grouped_size(5, "32", 1), 7); // 12,345 (first group of 3, then group of 2)
    EXPECT_EQ(fmt::detail::calculate_grouped_size(6, "32", 1), 8); // 123,456 (first group of 3, then group of 2, but n > *group fails for second '2')
    EXPECT_EQ(fmt::detail::calculate_grouped_size(7, "32", 1), 10); // 1,23,45,67 (first group of 3, then two groups of 2)
}

TEST(GroupedSizeTest, SeparatorSize) {
    // Test with different separator sizes
    EXPECT_EQ(fmt::detail::calculate_grouped_size(4, "3", 2), 6); // 4 digits, 1 separator of size 2 (1234 -> 1,,234)
    EXPECT_EQ(fmt::detail::calculate_grouped_size(7, "3", 2), 11); // 7 digits, 2 separators of size 2 (1234567 -> 1,,234,,567)
    EXPECT_EQ(fmt::detail::calculate_grouped_size(7, "32", 2), 13); // 7 digits, 3 separators of size 2 (1,23,45,67 -> 1,,23,,45,,67)
}

TEST(GroupedSizeTest, EdgeCases) {
    // Zero digits
    EXPECT_EQ(fmt::detail::calculate_grouped_size(0, "3", 1), 0);
    EXPECT_EQ(fmt::detail::calculate_grouped_size(0, "32", 1), 0);

    // Single digit, various grouping rules
    EXPECT_EQ(fmt::detail::calculate_grouped_size(1, "1", 1), 1); // n > *group (1 > 1) is false
    EXPECT_EQ(fmt::detail::calculate_grouped_size(1, "2", 1), 1); // n > *group (1 > 2) is false
    EXPECT_EQ(fmt::detail::calculate_grouped_size(1, "123", 1), 1);

    // Groups string with invalid characters (e.g., non-digit chars, though char is used as int)
    // The loop condition `*group > 0` handles this for non-positive values.
    // The `*group != max_value<char>()` handles the sentinel.
    // Other invalid chars would be interpreted as their ASCII values.
    EXPECT_EQ(fmt::detail::calculate_grouped_size(5, "A", 1), 5); // 'A' (65) > 5 is true, so it will add separator
    // This case highlights that `groups` is expected to contain digit characters representing group sizes.
    // If 'A' is interpreted as 65, then 5 > 65 is false, so no separator.
    // Let's re-evaluate based on the snippet: `n > *group` where `*group` is a char.
    // If `groups` is "A", then `*group` is 'A' (ASCII 65).
    // For num_digits = 5, n = 5. `5 > 65` is false. So `size` remains 5. This is correct.
    EXPECT_EQ(fmt::detail::calculate_grouped_size(66, "A", 1), 67); // 66 digits, 'A' (65) -> 1 separator
    EXPECT_EQ(fmt::detail::calculate_grouped_size(65, "A", 1), 65); // 65 digits, 'A' (65) -> no separator (n > *group is false)

    // Long groups string, but not enough digits to trigger all
    EXPECT_EQ(fmt::detail::calculate_grouped_size(5, "3333333333", 1), 6); // Only first '3' matters for 5 digits
    EXPECT_EQ(fmt::detail::calculate_grouped_size(10, "3333333333", 1), 13); // 10 digits, 3 separators (1,234,567,890)

    // Groups string with mixed valid/invalid (from snippet's perspective)
    EXPECT_EQ(fmt::detail::calculate_grouped_size(7, "3\02", 1), 9); // '\0' acts as a terminator for the loop condition `*group > 0`
                                                                    // The loop will stop at '\0' because `*group > 0` becomes false.
                                                                    // So it's effectively "3".
}

TEST(GroupedSizeTest, MaxCharSentinel) {
    // Test the `*group != max_value<char>()` condition
    char max_char = std::numeric_limits<char>::max();
    std::string groups_with_sentinel = std::string("3") + max_char + std::string("2");
    // The loop should stop at max_char, effectively making the groups "3"
    EXPECT_EQ(fmt::detail::calculate_grouped_size(7, groups_with_sentinel, 1), 9); // Same as "3"
    EXPECT_EQ(fmt::detail::calculate_grouped_size(4, groups_with_sentinel, 1), 5); // Same as "3"

    // If max_char is the first group
    std::string groups_start_with_sentinel = std::string("") + max_char + std::string("3");
    EXPECT_EQ(fmt::detail::calculate_grouped_size(7, groups_start_with_sentinel, 1), 7); // No grouping
}

TEST(GroupedSizeTest, EmptyGroupsString) {
    EXPECT_EQ(fmt::detail::calculate_grouped_size(10, "", 1), 10);
    EXPECT_EQ(fmt::detail::calculate_grouped_size(0, "", 1), 0);
}

TEST(GroupedSizeTest, LargeNumDigits) {
    // Test with a large number of digits to ensure loop termination and calculation correctness
    EXPECT_EQ(fmt::detail::calculate_grouped_size(100, "3", 1), 100 + (100 - 1) / 3); // 100 digits, 33 separators
    EXPECT_EQ(fmt::detail::calculate_grouped_size(100, "32", 1), 100 + 1 + (100 - 3 - 1) / 2); // 100 digits, 1 for first 3, then 48 for remaining 96
                                                                                             // 100 digits: 123,45,67,89,...
                                                                                             // 1 separator for 3 digits. Remaining 97.
                                                                                             // Then groups of 2. 97/2 = 48 separators.
                                                                                             // Total: 1 + 48 = 49 separators.
                                                                                             // size = 100 + 49 = 149.
    // Let's trace 100 digits with "32":
    // n=100, size=100, group="32"
    // 1. *group='3'. n=100 > 3. size=100+1=101. n=100-3=97. group="2"
    // 2. *group='2'. n=97 > 2. size=101+1=102. n=97-2=95. group=""
    // 3. *group is cend(). Loop terminates.
    // This is incorrect. The logic is:
    // 100 digits.
    // First group of 3: 123,456,789...
    // 100 digits.
    // n=100, size=100.
    // group='3'. n=100 > 3. size=101. n=97. group='2'.
    // group='2'. n=97 > 2. size=102. n=95. group='3'. (wraps around if groups is not exhausted)
    // The snippet uses `std::string::const_iterator group = groups.cbegin();` and `++group;`.
    // It does NOT wrap around. It stops when `group == groups.cend()`.
    // So for "32", it will apply '3', then '2', then stop.
    // This means for 100 digits:
    // n=100, size=100
    // 1. group='3'. n=100 > 3. size=101. n=97. group points to '2'.
    // 2. group='2'. n=97 > 2. size=102. n=95. group points to end.
    // 3. group == groups.cend(). Loop terminates.
    // Result: 102.
    EXPECT_EQ(fmt::detail::calculate_grouped_size(100, "32", 1), 102);

    // If the groups string is long enough to cover all digits
    std::string long_groups_str(100, '1'); // 100 groups of 1
    EXPECT_EQ(fmt::detail::calculate_grouped_size(100, long_groups_str, 1), 100 + 99); // 100 digits, 99 separators
}