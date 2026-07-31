#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test that numeric alignment with sign is handled correctly
TEST(AlignNumericTest, SignAndNumericAlignment) {
    // This exercises the case where spec.align() == ALIGN_NUMERIC and sign is present
    // The sign should be prepended and width decremented, alignment changed to right
    std::string result = fmt::format("{:=+8}", 42);
    EXPECT_EQ(result, "+     42");
    
    // Without sign, numeric alignment should still work
    result = fmt::format("{:=8}", 42);
    EXPECT_EQ(result, "42      ");
}

// Test that numeric alignment with sign and zero width works
TEST(AlignNumericTest, SignAndNumericAlignmentZeroWidth) {
    // When width is 0, sign should still be prepended but width decrement has no effect
    std::string result = fmt::format("{:=+0}", 42);
    EXPECT_EQ(result, "+42");
}

// Test that numeric alignment with sign and very large width
TEST(AlignNumericTest, SignAndNumericAlignmentLargeWidth) {
    std::string result = fmt::format("{:=+20}", 42);
    EXPECT_EQ(result, "+                  42");
}

// Test that numeric alignment with negative sign
TEST(AlignNumericTest, SignAndNumericAlignmentNegative) {
    std::string result = fmt::format("{:=+8}", -42);
    EXPECT_EQ(result, "-      42");
}

// Test default alignment (ALIGN_DEFAULT) without sign
TEST(AlignDefaultTest, NoSignDefaultAlignment) {
    // When spec.align() == ALIGN_DEFAULT and no sign, alignment should become ALIGN_RIGHT
    std::string result = fmt::format("{:8}", 42);
    EXPECT_EQ(result, "      42");
}

// Test default alignment with sign
TEST(AlignDefaultTest, SignDefaultAlignment) {
    // When spec.align() == ALIGN_DEFAULT and sign is present, n should be incremented
    std::string result = fmt::format("{:+8}", 42);
    EXPECT_EQ(result, "     +42");
}

// Test default alignment with negative sign
TEST(AlignDefaultTest, NegativeSignDefaultAlignment) {
    std::string result = fmt::format("{:8}", -42);
    EXPECT_EQ(result, "     -42");
}

// Test explicit right alignment without sign
TEST(AlignRightTest, NoSignRightAlignment) {
    // When spec.align() == ALIGN_RIGHT and no sign, no changes should occur
    std::string result = fmt::format("{:>8}", 42);
    EXPECT_EQ(result, "      42");
}

// Test explicit right alignment with sign
TEST(AlignRightTest, SignRightAlignment) {
    // When spec.align() == ALIGN_RIGHT and sign is present, n should be incremented
    std::string result = fmt::format("{:+>8}", 42);
    EXPECT_EQ(result, "     +42");
}

// Test explicit left alignment (should not be affected by the code)
TEST(AlignLeftTest, LeftAlignment) {
    // Left alignment should remain unchanged
    std::string result = fmt::format("{:<8}", 42);
    EXPECT_EQ(result, "42      ");
    
    result = fmt::format("{:+<8}", 42);
    EXPECT_EQ(result, "+42     ");
}

// Test center alignment (should not be affected by the code)
TEST(AlignCenterTest, CenterAlignment) {
    // Center alignment should remain unchanged
    std::string result = fmt::format("{:^8}", 42);
    EXPECT_EQ(result, "   42   ");
    
    result = fmt::format("{:+^8}", 42);
    EXPECT_EQ(result, " +42    ");
}

// Test with zero padding (which uses numeric alignment internally)
TEST(ZeroPaddingTest, ZeroPaddingWithSign) {
    // Zero padding uses ALIGN_NUMERIC internally
    std::string result = fmt::format("{:+08}", 42);
    EXPECT_EQ(result, "+0000042");
}

// Test with zero padding and negative number
TEST(ZeroPaddingTest, ZeroPaddingNegative) {
    std::string result = fmt::format("{:08}", -42);
    EXPECT_EQ(result, "-0000042");
}

// Test with zero padding and zero width
TEST(ZeroPaddingTest, ZeroPaddingZeroWidth) {
    std::string result = fmt::format("{:+0}", 42);
    EXPECT_EQ(result, "+42");
}

// Test edge case: zero value with various alignments
TEST(EdgeCaseTest, ZeroValue) {
    EXPECT_EQ(fmt::format("{:=+8}", 0), "+      0");
    EXPECT_EQ(fmt::format("{:8}", 0), "       0");
    EXPECT_EQ(fmt::format("{:+8}", 0), "      +0");
    EXPECT_EQ(fmt::format("{:08}", 0), "00000000");
}

// Test edge case: maximum integer values
TEST(EdgeCaseTest, MaxValues) {
    EXPECT_EQ(fmt::format("{:=+20}", 2147483647), "+          2147483647");
    EXPECT_EQ(fmt::format("{:20}", -2147483647), "         -2147483647");
}

// Test edge case: very small width (1)
TEST(EdgeCaseTest, MinimumWidth) {
    EXPECT_EQ(fmt::format("{:=+1}", 42), "+42");
    EXPECT_EQ(fmt::format("{:1}", 42), "42");
    EXPECT_EQ(fmt::format("{:+1}", 42), "+42");
}

// Test that the function handles the case where sign is present but width is exhausted
TEST(EdgeCaseTest, SignExhaustsWidth) {
    // When sign takes the only available space
    EXPECT_EQ(fmt::format("{:=+1}", 5), "+5");
    EXPECT_EQ(fmt::format("{:+1}", 5), "+5");
}

// Test with floating point numbers (which also go through this code path)
TEST(FloatingPointTest, FloatWithNumericAlignment) {
    EXPECT_EQ(fmt::format("{:=+10.2f}", 3.14), "+      3.14");
    EXPECT_EQ(fmt::format("{:10.2f}", 3.14), "      3.14");
    EXPECT_EQ(fmt::format("{:+10.2f}", 3.14), "     +3.14");
}

// Test with floating point negative numbers
TEST(FloatingPointTest, NegativeFloat) {
    EXPECT_EQ(fmt::format("{:=+10.2f}", -3.14), "-      3.14");
    EXPECT_EQ(fmt::format("{:10.2f}", -3.14), "     -3.14");
}

// Test with zero padding for floats
TEST(FloatingPointTest, FloatZeroPadding) {
    EXPECT_EQ(fmt::format("{:+010.2f}", 3.14), "+000003.14");
    EXPECT_EQ(fmt::format("{:010.2f}", -3.14), "-000003.14");
}