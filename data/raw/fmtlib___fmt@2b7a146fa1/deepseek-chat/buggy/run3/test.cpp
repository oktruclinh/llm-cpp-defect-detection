#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/locale.h>
#include <string>
#include <locale>
#include <climits>

// Test helper: format a number with a custom grouping that triggers the focal code path.
// The focal code is inside fmt::detail::format_decimal or similar internal formatting.
// We exercise it via fmt::format with a locale that defines grouping.

// Custom facet that returns specific grouping rules
struct test_grouping : std::numpunct<char> {
    std::string do_grouping() const override { return grouping; }
    char do_thousands_sep() const override { return sep; }
    std::string grouping;
    char sep;
};

// Test: basic grouping with 3-digit groups (e.g., 1,234,567)
TEST(CountDigitsGroupingTest, BasicGrouping) {
    test_grouping facet;
    facet.grouping = "\3";
    facet.sep = ',';
    std::locale loc(std::locale::classic(), &facet);
    std::string result = fmt::format(loc, "{:L}", 1234567);
    EXPECT_EQ(result, "1,234,567");
}

// Test: grouping with 2-digit groups (e.g., 12,34,56)
TEST(CountDigitsGroupingTest, TwoDigitGroups) {
    test_grouping facet;
    facet.grouping = "\2";
    facet.sep = ',';
    std::locale loc(std::locale::classic(), &facet);
    std::string result = fmt::format(loc, "{:L}", 123456);
    EXPECT_EQ(result, "12,34,56");
}

// Test: grouping where first group is smaller (e.g., 1,23,45,678)
TEST(CountDigitsGroupingTest, IndianGrouping) {
    test_grouping facet;
    facet.grouping = "\3\2";  // 3 then 2
    facet.sep = ',';
    std::locale loc(std::locale::classic(), &facet);
    std::string result = fmt::format(loc, "{:L}", 12345678);
    EXPECT_EQ(result, "1,23,45,678");
}

// Test: number with fewer digits than first group size (no separators)
TEST(CountDigitsGroupingTest, FewerDigitsThanGroup) {
    test_grouping facet;
    facet.grouping = "\3";
    facet.sep = ',';
    std::locale loc(std::locale::classic(), &facet);
    std::string result = fmt::format(loc, "{:L}", 123);
    EXPECT_EQ(result, "123");
}

// Test: number with exactly the group size (no separators)
TEST(CountDigitsGroupingTest, ExactlyGroupSize) {
    test_grouping facet;
    facet.grouping = "\3";
    facet.sep = ',';
    std::locale loc(std::locale::classic(), &facet);
    std::string result = fmt::format(loc, "{:L}", 1234);
    EXPECT_EQ(result, "1,234");
}

// Test: zero value
TEST(CountDigitsGroupingTest, ZeroValue) {
    test_grouping facet;
    facet.grouping = "\3";
    facet.sep = ',';
    std::locale loc(std::locale::classic(), &facet);
    std::string result = fmt::format(loc, "{:L}", 0);
    EXPECT_EQ(result, "0");
}

// Test: negative number with grouping
TEST(CountDigitsGroupingTest, NegativeNumber) {
    test_grouping facet;
    facet.grouping = "\3";
    facet.sep = ',';
    std::locale loc(std::locale::classic(), &facet);
    std::string result = fmt::format(loc, "{:L}", -1234567);
    EXPECT_EQ(result, "-1,234,567");
}

// Test: grouping with a non-comma separator
TEST(CountDigitsGroupingTest, CustomSeparator) {
    test_grouping facet;
    facet.grouping = "\3";
    facet.sep = '.';
    std::locale loc(std::locale::classic(), &facet);
    std::string result = fmt::format(loc, "{:L}", 1234567);
    EXPECT_EQ(result, "1.234.567");
}

// Test: grouping with max_value<char> group (should be ignored per focal code)
TEST(CountDigitsGroupingTest, MaxValueGroupIgnored) {
    test_grouping facet;
    // CHAR_MAX group should be ignored (treated as no more grouping)
    facet.grouping = "\3" + std::string(1, CHAR_MAX);
    facet.sep = ',';
    std::locale loc(std::locale::classic(), &facet);
    std::string result = fmt::format(loc, "{:L}", 1234567);
    EXPECT_EQ(result, "1,234,567");
}

// Test: grouping with zero group (should be ignored per focal code)
TEST(CountDigitsGroupingTest, ZeroGroupIgnored) {
    test_grouping facet;
    facet.grouping = "\3\0";
    facet.sep = ',';
    std::locale loc(std::locale::classic(), &facet);
    std::string result = fmt::format(loc, "{:L}", 1234567);
    EXPECT_EQ(result, "1,234,567");
}

// Test: empty grouping (no separators)
TEST(CountDigitsGroupingTest, EmptyGrouping) {
    test_grouping facet;
    facet.grouping = "";
    facet.sep = ',';
    std::locale loc(std::locale::classic(), &facet);
    std::string result = fmt::format(loc, "{:L}", 1234567);
    EXPECT_EQ(result, "1234567");
}

// Test: large number with multiple groups
TEST(CountDigitsGroupingTest, LargeNumber) {
    test_grouping facet;
    facet.grouping = "\3";
    facet.sep = ',';
    std::locale loc(std::locale::classic(), &facet);
    std::string result = fmt::format(loc, "{:L}", 1234567890123LL);
    EXPECT_EQ(result, "1,234,567,890,123");
}

// Test: single digit number
TEST(CountDigitsGroupingTest, SingleDigit) {
    test_grouping facet;
    facet.grouping = "\3";
    facet.sep = ',';
    std::locale loc(std::locale::classic(), &facet);
    std::string result = fmt::format(loc, "{:L}", 5);
    EXPECT_EQ(result, "5");
}

// Test: two digit number with 3-digit grouping
TEST(CountDigitsGroupingTest, TwoDigits) {
    test_grouping facet;
    facet.grouping = "\3";
    facet.sep = ',';
    std::locale loc(std::locale::classic(), &facet);
    std::string result = fmt::format(loc, "{:L}", 42);
    EXPECT_EQ(result, "42");
}