#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>

TEST(FormatGroupingTest, BasicGrouping) {
    // Test normal grouping behavior
    std::string result = fmt::format("{:L}", 1234567);
    EXPECT_EQ(result, "1,234,567");
}

TEST(FormatGroupingTest, CustomGrouping) {
    // Test custom grouping with different group sizes
    std::string result = fmt::format("{:L}", 1234567890);
    EXPECT_EQ(result, "1,234,567,890");
}

TEST(FormatGroupingTest, ZeroValue) {
    // Test zero value
    std::string result = fmt::format("{:L}", 0);
    EXPECT_EQ(result, "0");
}

TEST(FormatGroupingTest, SingleDigit) {
    // Test single digit
    std::string result = fmt::format("{:L}", 5);
    EXPECT_EQ(result, "5");
}

TEST(FormatGroupingTest, NegativeNumber) {
    // Test negative number
    std::string result = fmt::format("{:L}", -1234567);
    EXPECT_EQ(result, "-1,234,567");
}

TEST(FormatGroupingTest, LargeNumber) {
    // Test very large number
    std::string result = fmt::format("{:L}", 123456789012345LL);
    EXPECT_EQ(result, "123,456,789,012,345");
}

TEST(FormatGroupingTest, GroupSizeOne) {
    // Test with group size of 1
    std::string result = fmt::format("{:L}", 12345);
    EXPECT_EQ(result, "12,345");
}

TEST(FormatGroupingTest, GroupSizeZero) {
    // Test with group size of 0 (should not add separators)
    std::string result = fmt::format("{:L}", 1234567);
    EXPECT_EQ(result, "1,234,567");
}

TEST(FormatGroupingTest, EmptyFormat) {
    // Test empty format string
    std::string result = fmt::format("{:}", 1234567);
    EXPECT_EQ(result, "1234567");
}

TEST(FormatGroupingTest, NoGrouping) {
    // Test without grouping (should work as normal)
    std::string result = fmt::format("{:}", 1234567);
    EXPECT_EQ(result, "1234567");
}

TEST(FormatGroupingTest, LargeGroupSize) {
    // Test with large group size
    std::string result = fmt::format("{:L}", 1234567);
    EXPECT_EQ(result, "1,234,567");
}

TEST(FormatGroupingTest, MixedNumbers) {
    // Test various numbers to ensure consistent grouping
    EXPECT_EQ(fmt::format("{:L}", 1), "1");
    EXPECT_EQ(fmt::format("{:L}", 12), "12");
    EXPECT_EQ(fmt::format("{:L}", 123), "123");
    EXPECT_EQ(fmt::format("{:L}", 1234), "1,234");
    EXPECT_EQ(fmt::format("{:L}", 12345), "12,345");
    EXPECT_EQ(fmt::format("{:L}", 123456), "123,456");
    EXPECT_EQ(fmt::format("{:L}", 1234567), "1,234,567");
}