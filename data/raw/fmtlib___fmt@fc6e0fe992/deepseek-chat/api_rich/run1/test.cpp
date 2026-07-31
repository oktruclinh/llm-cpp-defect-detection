#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test the align_spec behavior through public API calls
// The focal function handles ALIGN_NUMERIC and ALIGN_DEFAULT cases
// with sign handling and width adjustments

TEST(AlignSpecTest, NumericAlignWithSignAndWidth) {
    // Test numeric alignment with sign and width
    // This exercises the branch: spec.align() == ALIGN_NUMERIC with sign
    auto result = fmt::format("{:+>5}", 42);
    EXPECT_EQ(result, "  +42");
    
    // Test numeric alignment with sign and zero width
    result = fmt::format("{:+>}", 42);
    EXPECT_EQ(result, "+42");
    
    // Test numeric alignment with sign and width=1
    result = fmt::format("{:+>1}", 42);
    EXPECT_EQ(result, "+42");
}

TEST(AlignSpecTest, NumericAlignWithoutSign) {
    // Test numeric alignment without sign
    auto result = fmt::format("{:>5}", 42);
    EXPECT_EQ(result, "   42");
    
    // Test numeric alignment with zero width
    result = fmt::format("{:>}", 42);
    EXPECT_EQ(result, "42");
}

TEST(AlignSpecTest, DefaultAlignWithSign) {
    // Test default alignment with sign (should become ALIGN_RIGHT)
    // This exercises the else branch: spec.align() == ALIGN_DEFAULT with sign
    auto result = fmt::format("{:+5}", 42);
    EXPECT_EQ(result, "  +42");
    
    // Test default alignment with sign and zero width
    result = fmt::format("{:+}", 42);
    EXPECT_EQ(result, "+42");
}

TEST(AlignSpecTest, DefaultAlignWithoutSign) {
    // Test default alignment without sign
    auto result = fmt::format("{:5}", 42);
    EXPECT_EQ(result, "   42");
    
    // Test default alignment with zero width
    result = fmt::format("{:}", 42);
    EXPECT_EQ(result, "42");
}

TEST(AlignSpecTest, NumericAlignWithNegativeSign) {
    // Test numeric alignment with negative sign
    auto result = fmt::format("{:>5}", -42);
    EXPECT_EQ(result, "  -42");
    
    // Test numeric alignment with negative sign and width=1
    result = fmt::format("{:>1}", -42);
    EXPECT_EQ(result, "-42");
}

TEST(AlignSpecTest, NumericAlignWithSpaceSign) {
    // Test numeric alignment with space sign
    auto result = fmt::format("{: >5}", 42);
    EXPECT_EQ(result, "   42");
    
    // Test numeric alignment with space sign and width=1
    result = fmt::format("{: >1}", 42);
    EXPECT_EQ(result, " 42");
}

TEST(AlignSpecTest, NumericAlignWithZeroPadding) {
    // Test numeric alignment with zero padding (which implies ALIGN_NUMERIC)
    auto result = fmt::format("{:+05}", 42);
    EXPECT_EQ(result, "+0042");
    
    // Test numeric alignment with zero padding and negative
    result = fmt::format("{:05}", -42);
    EXPECT_EQ(result, "-0042");
}

TEST(AlignSpecTest, LeftAlignWithSign) {
    // Test left alignment with sign (should not trigger numeric align branch)
    auto result = fmt::format("{:+<5}", 42);
    EXPECT_EQ(result, "+42  ");
    
    // Test left alignment with sign and width=1
    result = fmt::format("{:+<1}", 42);
    EXPECT_EQ(result, "+42");
}

TEST(AlignSpecTest, CenterAlignWithSign) {
    // Test center alignment with sign (should not trigger numeric align branch)
    auto result = fmt::format("{:+^5}", 42);
    EXPECT_EQ(result, "+42  ");
    
    // Test center alignment with sign and width=1
    result = fmt::format("{:+^1}", 42);
    EXPECT_EQ(result, "+42");
}

TEST(AlignSpecTest, NumericAlignWithWidthAndMultipleSigns) {
    // Test numeric alignment with explicit plus sign and width
    auto result = fmt::format("{:+>10}", 123);
    EXPECT_EQ(result, "      +123");
    
    // Test numeric alignment with minus sign and width
    result = fmt::format("{:>10}", -123);
    EXPECT_EQ(result, "      -123");
    
    // Test numeric alignment with space sign and width
    result = fmt::format("{: >10}", 123);
    EXPECT_EQ(result, "      123");
}

TEST(AlignSpecTest, DefaultAlignWithWidthAndMultipleSigns) {
    // Test default alignment with explicit plus sign and width
    auto result = fmt::format("{:+10}", 123);
    EXPECT_EQ(result, "      +123");
    
    // Test default alignment with minus sign and width
    result = fmt::format("{:10}", -123);
    EXPECT_EQ(result, "      -123");
    
    // Test default alignment with space sign and width
    result = fmt::format("{: 10}", 123);
    EXPECT_EQ(result, "      123");
}

TEST(AlignSpecTest, NumericAlignWithZeroWidthAndSign) {
    // Test numeric alignment with zero width and sign
    auto result = fmt::format("{:+>0}", 42);
    EXPECT_EQ(result, "+42");
    
    // Test numeric alignment with zero width and negative
    result = fmt::format("{:>0}", -42);
    EXPECT_EQ(result, "-42");
}

TEST(AlignSpecTest, NumericAlignWithLargeWidth) {
    // Test numeric alignment with large width
    auto result = fmt::format("{:+>100}", 42);
    EXPECT_EQ(result, std::string(97, ' ') + "+42");
    
    // Test numeric alignment with large width and negative
    result = fmt::format("{:>100}", -42);
    EXPECT_EQ(result, std::string(97, ' ') + "-42");
}

TEST(AlignSpecTest, DefaultAlignWithLargeWidth) {
    // Test default alignment with large width
    auto result = fmt::format("{:+100}", 42);
    EXPECT_EQ(result, std::string(97, ' ') + "+42");
    
    // Test default alignment with large width and negative
    result = fmt::format("{:100}", -42);
    EXPECT_EQ(result, std::string(97, ' ') + "-42");
}

TEST(AlignSpecTest, NumericAlignWithZeroAndSign) {
    // Test numeric alignment with zero value and sign
    auto result = fmt::format("{:+>5}", 0);
    EXPECT_EQ(result, "   +0");
    
    // Test numeric alignment with zero value and negative
    result = fmt::format("{:>5}", 0);
    EXPECT_EQ(result, "    0");
}

TEST(AlignSpecTest, DefaultAlignWithZeroAndSign) {
    // Test default alignment with zero value and sign
    auto result = fmt::format("{:+5}", 0);
    EXPECT_EQ(result, "   +0");
    
    // Test default alignment with zero value
    result = fmt::format("{:5}", 0);
    EXPECT_EQ(result, "    0");
}

TEST(AlignSpecTest, NumericAlignWithMaxInt) {
    // Test numeric alignment with maximum integer value
    auto result = fmt::format("{:+>20}", 2147483647);
    EXPECT_EQ(result, "        +2147483647");
    
    // Test numeric alignment with minimum integer value
    result = fmt::format("{:>20}", -2147483648LL);
    EXPECT_EQ(result, "       -2147483648");
}

TEST(AlignSpecTest, DefaultAlignWithMaxInt) {
    // Test default alignment with maximum integer value
    auto result = fmt::format("{:+20}", 2147483647);
    EXPECT_EQ(result, "        +2147483647");
    
    // Test default alignment with minimum integer value
    result = fmt::format("{:20}", -2147483648LL);
    EXPECT_EQ(result, "       -2147483648");
}