#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test the grouping behavior of fmt::format with thousands separators
// This exercises the focal function's digit grouping logic

TEST(FormatGroupingTest, NoGroupingForSmallNumbers) {
    // Numbers with fewer digits than the first group size should have no separators
    EXPECT_EQ(fmt::format("{:L}", 123), "123");
    EXPECT_EQ(fmt::format("{:L}", 0), "0");
    EXPECT_EQ(fmt::format("{:L}", 1), "1");
    EXPECT_EQ(fmt::format("{:L}", 999), "999");
}

TEST(FormatGroupingTest, BasicThousandsGrouping) {
    // Default locale grouping (typically 3-digit groups)
    EXPECT_EQ(fmt::format("{:L}", 1000), "1,000");
    EXPECT_EQ(fmt::format("{:L}", 1234567), "1,234,567");
    EXPECT_EQ(fmt::format("{:L}", 1234567890), "1,234,567,890");
}

TEST(FormatGroupingTest, NegativeNumbersWithGrouping) {
    // Negative numbers should be grouped the same way
    EXPECT_EQ(fmt::format("{:L}", -1000), "-1,000");
    EXPECT_EQ(fmt::format("{:L}", -1234567), "-1,234,567");
}

TEST(FormatGroupingTest, GroupingWithCustomLocale) {
    // Test with Indian numbering system (first group of 3, then groups of 2)
    fmt::format("{}", ""); // Ensure locale is loaded
    // Note: This test assumes the "en_IN" locale is available on the system
    // If not available, this test may need adjustment
    try {
        auto result = fmt::format(fmt::locale("en_IN"), "{:L}", 123456789);
        // Indian grouping: 12,34,56,789
        EXPECT_EQ(result, "12,34,56,789");
    } catch (const std::runtime_error&) {
        // Locale not available, skip this test
        GTEST_SKIP() << "en_IN locale not available";
    }
}

TEST(FormatGroupingTest, GroupingWithDifferentGroupSizes) {
    // Test with a locale that has non-standard grouping (e.g., 4-3-3)
    // This uses a custom locale if available
    try {
        // Using "de_CH" which typically uses 3-digit groups but with apostrophe
        auto result = fmt::format(fmt::locale("de_CH"), "{:L}", 1234567);
        EXPECT_EQ(result, "1'234'567");
    } catch (const std::runtime_error&) {
        GTEST_SKIP() << "de_CH locale not available";
    }
}

TEST(FormatGroupingTest, LargeNumbersWithMultipleGroups) {
    // Very large numbers should be grouped correctly
    EXPECT_EQ(fmt::format("{:L}", 1000000000), "1,000,000,000");
    EXPECT_EQ(fmt::format("{:L}", 999999999999), "999,999,999,999");
}

TEST(FormatGroupingTest, ZeroAndEdgeCases) {
    // Zero should have no grouping
    EXPECT_EQ(fmt::format("{:L}", 0), "0");
    
    // Numbers just below grouping threshold
    EXPECT_EQ(fmt::format("{:L}", 999), "999");
    EXPECT_EQ(fmt::format("{:L}", 1000), "1,000");
    
    // Negative zero (should be treated as zero)
    EXPECT_EQ(fmt::format("{:L}", -0), "0");
}

TEST(FormatGroupingTest, GroupingWithFillAndAlign) {
    // Grouping should work with other format specifiers
    EXPECT_EQ(fmt::format("{:>10L}", 1234), "     1,234");
    EXPECT_EQ(fmt::format("{:*<10L}", 1234), "1,234*****");
    EXPECT_EQ(fmt::format("{:^12L}", 1234567), " 1,234,567  ");
}

TEST(FormatGroupingTest, GroupingWithPrecision) {
    // Grouping should work with floating-point numbers
    EXPECT_EQ(fmt::format("{:L}", 1234.5), "1,234.5");
    EXPECT_EQ(fmt::format("{:.2L}", 1234567.89), "1,234,567.89");
}

TEST(FormatGroupingTest, GroupingWithDifferentBases) {
    // Grouping should work with hex and octal (though less common)
    EXPECT_EQ(fmt::format("{:L}", 0xABCD), "43,981"); // Decimal representation
    EXPECT_EQ(fmt::format("{:L}", 01234567), "342,391"); // Decimal representation
}

TEST(FormatGroupingTest, GroupingWithVerySmallGroupSizes) {
    // Edge case: group size of 1 (every digit separated)
    // This requires a custom locale, which may not be available
    // Testing with default locale behavior instead
    EXPECT_EQ(fmt::format("{:L}", 1), "1");
    EXPECT_EQ(fmt::format("{:L}", 12), "12");
    EXPECT_EQ(fmt::format("{:L}", 123), "123");
}

TEST(FormatGroupingTest, GroupingWithMaxValues) {
    // Test with maximum integer values
    EXPECT_EQ(fmt::format("{:L}", std::numeric_limits<int>::max()), "2,147,483,647");
    EXPECT_EQ(fmt::format("{:L}", std::numeric_limits<long long>::max()), "9,223,372,036,854,775,807");
}

TEST(FormatGroupingTest, GroupingWithMinValues) {
    // Test with minimum integer values
    EXPECT_EQ(fmt::format("{:L}", std::numeric_limits<int>::min()), "-2,147,483,648");
    EXPECT_EQ(fmt::format("{:L}", std::numeric_limits<long long>::min()), "-9,223,372,036,854,775,808");
}