#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <string>
#include <vector>

// Test the printf formatter's behavior with various format specifications
// The focal function handles format specs for non-numeric types, specifically
// overriding align::numeric to align::right and resetting sign/alt flags

class PrintfFormatterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // No setup needed
    }
};

// Test that align::numeric is overridden to align::right for non-numeric types
TEST_F(PrintfFormatterTest, AlignNumericOverriddenToRight) {
    // Using %-10s should normally left-align, but with '0' flag it becomes right-align
    // The focal function overrides align::numeric to align::right
    std::string result = fmt::sprintf("%-10s", "test");
    EXPECT_EQ(result, "test      ");  // Left-aligned (no '0' flag, so no override)
    
    // With '0' flag and string type, align::numeric should become align::right
    result = fmt::sprintf("%010s", "test");
    EXPECT_EQ(result, "      test");  // Right-aligned with spaces (0 flag ignored for strings)
    
    // Test that align::none becomes align::right
    result = fmt::sprintf("%10s", "test");
    EXPECT_EQ(result, "      test");  // Default right-alignment
}

// Test that sign is reset to none for non-numeric types
TEST_F(PrintfFormatterTest, SignResetToNone) {
    // '+' sign should be ignored for strings
    std::string result = fmt::sprintf("%+10s", "test");
    EXPECT_EQ(result, "      test");  // No sign prefix for strings
    
    // ' ' (space) sign should be ignored for strings
    result = fmt::sprintf("% 10s", "test");
    EXPECT_EQ(result, "      test");  // No space prefix for strings
    
    // '-' sign should be ignored for strings (left-align still works)
    result = fmt::sprintf("%-+10s", "test");
    EXPECT_EQ(result, "test      ");  // Left-aligned, no sign
}

// Test that alt flag is reset to false for non-numeric types
TEST_F(PrintfFormatterTest, AltFlagResetToFalse) {
    // '#' flag should be ignored for strings
    std::string result = fmt::sprintf("%#10s", "test");
    EXPECT_EQ(result, "      test");  // No alternate form for strings
    
    // '#' flag with '0' flag
    result = fmt::sprintf("%#010s", "test");
    EXPECT_EQ(result, "      test");  // Both flags ignored for strings
}

// Test with empty string
TEST_F(PrintfFormatterTest, EmptyString) {
    std::string result = fmt::sprintf("%10s", "");
    EXPECT_EQ(result, "          ");  // 10 spaces
    
    result = fmt::sprintf("%-10s", "");
    EXPECT_EQ(result, "          ");  // 10 spaces (left-aligned empty)
    
    result = fmt::sprintf("%010s", "");
    EXPECT_EQ(result, "          ");  // 10 spaces (0 flag ignored)
}

// Test with single character
TEST_F(PrintfFormatterTest, SingleCharacter) {
    std::string result = fmt::sprintf("%5c", 'A');
    EXPECT_EQ(result, "    A");  // Right-aligned with spaces
    
    result = fmt::sprintf("%-5c", 'A');
    EXPECT_EQ(result, "A    ");  // Left-aligned
    
    result = fmt::sprintf("%05c", 'A');
    EXPECT_EQ(result, "    A");  // 0 flag ignored, right-aligned with spaces
}

// Test with pointer (non-numeric type)
TEST_F(PrintfFormatterTest, PointerType) {
    int x = 42;
    void* ptr = &x;
    
    // Pointer formatting should ignore sign and alt flags
    std::string result = fmt::sprintf("%+20p", ptr);
    EXPECT_NE(result.find("0x"), std::string::npos);  // Should contain hex prefix
    EXPECT_EQ(result.size(), 20);  // Should be padded to width
    
    // With '0' flag, align::numeric should become align::right
    result = fmt::sprintf("%020p", ptr);
    EXPECT_NE(result.find("0x"), std::string::npos);
    EXPECT_EQ(result.size(), 20);
}

// Test with custom string-like type
TEST_F(PrintfFormatterTest, StringViewType) {
    fmt::string_view sv = "hello";
    std::string result = fmt::sprintf("%10s", sv);
    EXPECT_EQ(result, "     hello");  // Right-aligned
    
    result = fmt::sprintf("%010s", sv);
    EXPECT_EQ(result, "     hello");  // 0 flag ignored, right-aligned with spaces
    
    result = fmt::sprintf("%+10s", sv);
    EXPECT_EQ(result, "     hello");  // Sign ignored
}

// Test that numeric types still respect the flags (regression test)
TEST_F(PrintfFormatterTest, NumericTypesStillWork) {
    // Integers should still use '0' padding
    std::string result = fmt::sprintf("%010d", 42);
    EXPECT_EQ(result, "0000000042");  // Zero-padded
    
    // Integers should still use '+' sign
    result = fmt::sprintf("%+10d", 42);
    EXPECT_EQ(result, "       +42");  // With plus sign
    
    // Integers should still use '#' alt flag
    result = fmt::sprintf("%#010x", 255);
    EXPECT_EQ(result, "0x000000ff");  // Hex with 0x prefix
}

// Test with very long string
TEST_F(PrintfFormatterTest, LongString) {
    std::string long_str(100, 'a');
    std::string result = fmt::sprintf("%10s", long_str);
    EXPECT_EQ(result, long_str);  // No truncation, width ignored if string longer
    
    result = fmt::sprintf("%010s", long_str);
    EXPECT_EQ(result, long_str);  // Same with '0' flag
}

// Test with various width specifications
TEST_F(PrintfFormatterTest, VariousWidths) {
    std::string result = fmt::sprintf("%1s", "test");
    EXPECT_EQ(result, "test");  // Width smaller than string
    
    result = fmt::sprintf("%0s", "test");
    EXPECT_EQ(result, "test");  // Zero width
    
    result = fmt::sprintf("%*s", 10, "test");
    EXPECT_EQ(result, "      test");  // Dynamic width
}

// Test that precision is handled correctly (not affected by focal function)
TEST_F(PrintfFormatterTest, PrecisionHandling) {
    std::string result = fmt::sprintf("%.3s", "hello");
    EXPECT_EQ(result, "hel");  // Precision limits string length
    
    result = fmt::sprintf("%10.3s", "hello");
    EXPECT_EQ(result, "       hel");  // Width with precision
    
    result = fmt::sprintf("%010.3s", "hello");
    EXPECT_EQ(result, "       hel");  // 0 flag ignored, precision still works
}