#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <climits>

// Test the digit grouping logic indirectly through fmt::format with locale-specific grouping
// The focal function is part of the internal digit grouping mechanism

TEST(FormatDigitGroupingTest, NoGrouping) {
    // When no grouping is specified, the function should not add any separators
    auto result = fmt::format("{:d}", 1234567);
    EXPECT_EQ(result, "1234567");
}

TEST(FormatDigitGroupingTest, BasicGrouping) {
    // Test with a locale that uses grouping (e.g., en_US style with thousands separators)
    // This exercises the focal function through the public API
    auto result = fmt::format("{:L}", 1234567);
    // The exact format depends on the locale, but we can verify grouping behavior
    EXPECT_NE(result.find(','), std::string::npos) << "Expected grouping separator";
}

TEST(FormatDigitGroupingTest, SmallNumberNoGrouping) {
    // Numbers smaller than the first group size should not be grouped
    auto result = fmt::format("{:L}", 123);
    EXPECT_EQ(result, "123");
}

TEST(FormatDigitGroupingTest, ExactlyOneGroup) {
    // Number with exactly the first group size (typically 3 digits)
    auto result = fmt::format("{:L}", 999);
    EXPECT_EQ(result, "999");
}

TEST(FormatDigitGroupingTest, MultipleGroups) {
    // Number that requires multiple grouping separators
    auto result = fmt::format("{:L}", 123456789);
    // Should have at least 2 separators for 9 digits with 3-digit groups
    int comma_count = 0;
    for (char c : result) {
        if (c == ',') comma_count++;
    }
    EXPECT_GE(comma_count, 2);
}

TEST(FormatDigitGroupingTest, Zero) {
    // Zero should not trigger any grouping
    auto result = fmt::format("{:L}", 0);
    EXPECT_EQ(result, "0");
}

TEST(FormatDigitGroupingTest, NegativeNumber) {
    // Negative numbers should be grouped correctly (minus sign before grouping)
    auto result = fmt::format("{:L}", -1234567);
    EXPECT_EQ(result[0], '-');
    EXPECT_NE(result.find(','), std::string::npos);
}

TEST(FormatDigitGroupingTest, LargeNumber) {
    // Very large number to stress test the grouping logic
    auto result = fmt::format("{:L}", 1234567890123LL);
    // Should have multiple grouping separators
    int comma_count = 0;
    for (char c : result) {
        if (c == ',') comma_count++;
    }
    EXPECT_GE(comma_count, 3);
}

TEST(FormatDigitGroupingTest, CustomGrouping) {
    // Test with custom grouping pattern via locale
    // This exercises the while loop in the focal function with different group sizes
    auto result = fmt::format("{:L}", 1234567);
    // Verify the grouping pattern (typically 3-3-...)
    auto first_comma = result.find(',');
    auto last_comma = result.rfind(',');
    if (first_comma != std::string::npos && last_comma != std::string::npos && first_comma != last_comma) {
        // Check that groups are of expected size (typically 3 digits)
        int first_group_size = first_comma;
        int last_group_size = result.length() - last_comma - 1;
        EXPECT_EQ(last_group_size, 3) << "Last group should be 3 digits";
    }
}

TEST(FormatDigitGroupingTest, EdgeCaseMaxDigits) {
    // Test with maximum number of digits that might cause overflow in the focal function
    auto result = fmt::format("{:L}", 999999999999999999ULL);
    EXPECT_FALSE(result.empty());
    // Should have multiple grouping separators
    EXPECT_NE(result.find(','), std::string::npos);
}

TEST(FormatDigitGroupingTest, SingleDigit) {
    // Single digit should not be grouped
    auto result = fmt::format("{:L}", 5);
    EXPECT_EQ(result, "5");
}

TEST(FormatDigitGroupingTest, TwoDigits) {
    // Two digits should not be grouped
    auto result = fmt::format("{:L}", 42);
    EXPECT_EQ(result, "42");
}

TEST(FormatDigitGroupingTest, ThreeDigits) {
    // Three digits should not be grouped (first group boundary)
    auto result = fmt::format("{:L}", 100);
    EXPECT_EQ(result, "100");
}

TEST(FormatDigitGroupingTest, FourDigits) {
    // Four digits should have one separator
    auto result = fmt::format("{:L}", 1000);
    EXPECT_NE(result.find(','), std::string::npos);
    EXPECT_EQ(result.length(), 5); // 4 digits + 1 separator
}

TEST(FormatDigitGroupingTest, SixDigits) {
    // Six digits should have one separator (2 groups of 3)
    auto result = fmt::format("{:L}", 123456);
    EXPECT_EQ(result.find(','), 3);
    EXPECT_EQ(result.length(), 7); // 6 digits + 1 separator
}

TEST(FormatDigitGroupingTest, SevenDigits) {
    // Seven digits should have two separators (3-3-1 pattern)
    auto result = fmt::format("{:L}", 1234567);
    EXPECT_EQ(result.length(), 9); // 7 digits + 2 separators
    EXPECT_EQ(result[3], ',');
    EXPECT_EQ(result[7], ',');
}