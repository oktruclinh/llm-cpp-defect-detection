#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test the behavior of the align_spec logic through public fmt::format API
// The focal function handles numeric alignment with sign handling

class AlignSpecTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

// Test default alignment (ALIGN_DEFAULT) - should become ALIGN_RIGHT
TEST_F(AlignSpecTest, DefaultAlignmentBecomesRight) {
    // Default alignment for numbers should be right-aligned
    EXPECT_EQ(fmt::format("{:5}", 42), "   42");
    EXPECT_EQ(fmt::format("{:5}", -42), "  -42");
}

// Test explicit right alignment
TEST_F(AlignSpecTest, ExplicitRightAlignment) {
    EXPECT_EQ(fmt::format("{:>5}", 42), "   42");
    EXPECT_EQ(fmt::format("{:>5}", -42), "  -42");
}

// Test left alignment (should not be affected by the focal function)
TEST_F(AlignSpecTest, LeftAlignment) {
    EXPECT_EQ(fmt::format("{:<5}", 42), "42   ");
    EXPECT_EQ(fmt::format("{:<5}", -42), "-42  ");
}

// Test center alignment (should not be affected by the focal function)
TEST_F(AlignSpecTest, CenterAlignment) {
    EXPECT_EQ(fmt::format("{:^5}", 42), " 42  ");
    EXPECT_EQ(fmt::format("{:^5}", -42), " -42 ");
}

// Test numeric alignment with sign
TEST_F(AlignSpecTest, NumericAlignmentWithSign) {
    // Numeric alignment should become right alignment and sign should be placed before padding
    EXPECT_EQ(fmt::format("{:=+5}", 42), "+  42");
    EXPECT_EQ(fmt::format("{:=+5}", -42), "-  42");
}

// Test numeric alignment with sign and zero padding
TEST_F(AlignSpecTest, NumericAlignmentWithSignAndZeroPad) {
    // Zero padding with sign should place sign first, then zeros
    EXPECT_EQ(fmt::format("{:=+05}", 42), "+0042");
    EXPECT_EQ(fmt::format("{:=+05}", -42), "-0042");
}

// Test numeric alignment with sign and width larger than number
TEST_F(AlignSpecTest, NumericAlignmentWithSignLargeWidth) {
    EXPECT_EQ(fmt::format("{:=+10}", 42), "+       42");
    EXPECT_EQ(fmt::format("{:=+10}", -42), "-       42");
}

// Test numeric alignment with sign and width equal to number
TEST_F(AlignSpecTest, NumericAlignmentWithSignEqualWidth) {
    EXPECT_EQ(fmt::format("{:=+3}", 42), "+42");
    EXPECT_EQ(fmt::format("{:=+3}", -42), "-42");
}

// Test numeric alignment with sign and width smaller than number
TEST_F(AlignSpecTest, NumericAlignmentWithSignSmallWidth) {
    // Width smaller than number should not truncate
    EXPECT_EQ(fmt::format("{:=+2}", 42), "42");
    EXPECT_EQ(fmt::format("{:=+2}", -42), "-42");
}

// Test numeric alignment with positive sign
TEST_F(AlignSpecTest, NumericAlignmentWithPositiveSign) {
    EXPECT_EQ(fmt::format("{:=+5}", 42), "+  42");
    EXPECT_EQ(fmt::format("{:=+5}", 0), "+   0");
}

// Test numeric alignment with negative sign
TEST_F(AlignSpecTest, NumericAlignmentWithNegativeSign) {
    EXPECT_EQ(fmt::format("{:=+5}", -42), "-  42");
    EXPECT_EQ(fmt::format("{:=+5}", -1), "-   1");
}

// Test numeric alignment with space for sign
TEST_F(AlignSpecTest, NumericAlignmentWithSpaceSign) {
    EXPECT_EQ(fmt::format("{:= 5}", 42), "   42");
    EXPECT_EQ(fmt::format("{:= 5}", -42), "-  42");
}

// Test numeric alignment with zero value
TEST_F(AlignSpecTest, NumericAlignmentWithZero) {
    EXPECT_EQ(fmt::format("{:=+5}", 0), "+   0");
    EXPECT_EQ(fmt::format("{:=+05}", 0), "+0000");
}

// Test numeric alignment with large positive number
TEST_F(AlignSpecTest, NumericAlignmentWithLargePositive) {
    EXPECT_EQ(fmt::format("{:=+10}", 12345), "+    12345");
}

// Test numeric alignment with large negative number
TEST_F(AlignSpecTest, NumericAlignmentWithLargeNegative) {
    EXPECT_EQ(fmt::format("{:=+10}", -12345), "-    12345");
}

// Test that numeric alignment does not affect non-numeric types
TEST_F(AlignSpecTest, NumericAlignmentWithString) {
    // Strings should not be affected by numeric alignment
    EXPECT_EQ(fmt::format("{:=<10}", "hello"), "hello=====");
}

// Test that numeric alignment with sign and no width specifier
TEST_F(AlignSpecTest, NumericAlignmentWithSignNoWidth) {
    // Without width, sign should be directly before number
    EXPECT_EQ(fmt::format("{:=+}", 42), "+42");
    EXPECT_EQ(fmt::format("{:=+}", -42), "-42");
}

// Test combination of numeric alignment with fill character
TEST_F(AlignSpecTest, NumericAlignmentWithFillChar) {
    EXPECT_EQ(fmt::format("{:*=+5}", 42), "+**42");
    EXPECT_EQ(fmt::format("{:*=+5}", -42), "-**42");
}

// Test that default alignment for non-numeric types remains left-aligned
TEST_F(AlignSpecTest, DefaultAlignmentForNonNumeric) {
    EXPECT_EQ(fmt::format("{:5}", "hi"), "hi   ");
}

// Test edge case: very large width
TEST_F(AlignSpecTest, VeryLargeWidth) {
    std::string result = fmt::format("{:=+100}", 42);
    EXPECT_EQ(result.size(), 100);
    EXPECT_EQ(result[0], '+');
    EXPECT_EQ(result[result.size() - 2], '4');
    EXPECT_EQ(result[result.size() - 1], '2');
}

// Test edge case: width of 1 with sign
TEST_F(AlignSpecTest, WidthOneWithSign) {
    EXPECT_EQ(fmt::format("{:=+1}", 42), "42");
    EXPECT_EQ(fmt::format("{:=+1}", -42), "-42");
}

// Test that numeric alignment with sign works for different integer types
TEST_F(AlignSpecTest, NumericAlignmentWithDifferentIntTypes) {
    EXPECT_EQ(fmt::format("{:=+5}", short(42)), "+  42");
    EXPECT_EQ(fmt::format("{:=+5}", long(42)), "+  42");
    EXPECT_EQ(fmt::format("{:=+5}", (unsigned short)42), "   42");
}