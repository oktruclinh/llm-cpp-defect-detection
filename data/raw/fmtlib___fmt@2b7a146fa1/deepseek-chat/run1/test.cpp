#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <climits>

// Test the digit grouping logic indirectly through fmt::format with locale-specific grouping
// The focal function is used internally when formatting integers with grouping separators

class DigitGroupingTest : public ::testing::Test {
protected:
    // Helper to create a locale with custom grouping patterns
    struct custom_numpunct : std::numpunct<char> {
        std::string grouping;
        char separator;
        
        custom_numpunct(const std::string& g, char sep) 
            : grouping(g), separator(sep) {}
        
        char do_thousands_sep() const override { return separator; }
        std::string do_grouping() const override { return grouping; }
    };
    
    std::locale create_locale(const std::string& grouping, char sep = ',') {
        return std::locale(std::locale(), new custom_numpunct(grouping, sep));
    }
};

// Normal case: standard 3-digit grouping
TEST_F(DigitGroupingTest, StandardGrouping) {
    auto loc = create_locale("\3");
    EXPECT_EQ(fmt::format(loc, "{:L}", 1234567), "1,234,567");
    EXPECT_EQ(fmt::format(loc, "{:L}", 12345), "12,345");
    EXPECT_EQ(fmt::format(loc, "{:L}", 123), "123");
}

// Edge case: single digit
TEST_F(DigitGroupingTest, SingleDigit) {
    auto loc = create_locale("\3");
    EXPECT_EQ(fmt::format(loc, "{:L}", 0), "0");
    EXPECT_EQ(fmt::format(loc, "{:L}", 5), "5");
    EXPECT_EQ(fmt::format(loc, "{:L}", 9), "9");
}

// Edge case: negative numbers
TEST_F(DigitGroupingTest, NegativeNumbers) {
    auto loc = create_locale("\3");
    EXPECT_EQ(fmt::format(loc, "{:L}", -1234567), "-1,234,567");
    EXPECT_EQ(fmt::format(loc, "{:L}", -123), "-123");
    EXPECT_EQ(fmt::format(loc, "{:L}", -1), "-1");
}

// Edge case: zero
TEST_F(DigitGroupingTest, Zero) {
    auto loc = create_locale("\3");
    EXPECT_EQ(fmt::format(loc, "{:L}", 0), "0");
}

// Edge case: empty grouping (no separators)
TEST_F(DigitGroupingTest, EmptyGrouping) {
    auto loc = create_locale("");
    EXPECT_EQ(fmt::format(loc, "{:L}", 1234567), "1234567");
    EXPECT_EQ(fmt::format(loc, "{:L}", 0), "0");
}

// Edge case: grouping with zero in pattern (should be treated as end)
TEST_F(DigitGroupingTest, ZeroInGrouping) {
    auto loc = create_locale("\3\0\2", '\0'); // null separator
    // The function should stop at the zero value in grouping
    EXPECT_EQ(fmt::format(loc, "{:L}", 1234567), "1234567");
}

// Edge case: very large grouping (larger than number of digits)
TEST_F(DigitGroupingTest, LargeGrouping) {
    auto loc = create_locale("\10");
    EXPECT_EQ(fmt::format(loc, "{:L}", 12345), "12345");
    EXPECT_EQ(fmt::format(loc, "{:L}", 123456789), "12,3456789");
}

// Edge case: multiple grouping sizes
TEST_F(DigitGroupingTest, MultipleGroupingSizes) {
    // Indian numbering system: 3,2,2 grouping
    auto loc = create_locale("\3\2\2");
    EXPECT_EQ(fmt::format(loc, "{:L}", 12345678), "1,23,45,678");
    EXPECT_EQ(fmt::format(loc, "{:L}", 1234567), "12,34,567");
    EXPECT_EQ(fmt::format(loc, "{:L}", 123456), "1,23,456");
}

// Edge case: grouping with CHAR_MAX (should be treated as end)
TEST_F(DigitGroupingTest, CharMaxGrouping) {
    auto loc = create_locale(std::string(1, CHAR_MAX));
    EXPECT_EQ(fmt::format(loc, "{:L}", 1234567), "1234567");
}

// Edge case: minimum value for int
TEST_F(DigitGroupingTest, MinInt) {
    auto loc = create_locale("\3");
    EXPECT_EQ(fmt::format(loc, "{:L}", INT_MIN), "-2,147,483,648");
}

// Edge case: maximum value for int
TEST_F(DigitGroupingTest, MaxInt) {
    auto loc = create_locale("\3");
    EXPECT_EQ(fmt::format(loc, "{:L}", INT_MAX), "2,147,483,647");
}

// Edge case: grouping with non-standard separator
TEST_F(DigitGroupingTest, CustomSeparator) {
    auto loc = create_locale("\3", '.');
    EXPECT_EQ(fmt::format(loc, "{:L}", 1234567), "1.234.567");
    EXPECT_EQ(fmt::format(loc, "{:L}", 12345), "12.345");
}

// Edge case: number with exactly the grouping size
TEST_F(DigitGroupingTest, ExactGroupingSize) {
    auto loc = create_locale("\3");
    EXPECT_EQ(fmt::format(loc, "{:L}", 1000), "1,000");
    EXPECT_EQ(fmt::format(loc, "{:L}", 1000000), "1,000,000");
}

// Edge case: number just below grouping threshold
TEST_F(DigitGroupingTest, BelowGroupingThreshold) {
    auto loc = create_locale("\3");
    EXPECT_EQ(fmt::format(loc, "{:L}", 999), "999");
    EXPECT_EQ(fmt::format(loc, "{:L}", 99), "99");
    EXPECT_EQ(fmt::format(loc, "{:L}", 9), "9");
}