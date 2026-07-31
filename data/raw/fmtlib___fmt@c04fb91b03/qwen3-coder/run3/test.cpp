#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Test the formatter specialization for enum types
TEST(EnumFormatterTest, BasicEnumFormatting) {
    enum Color { RED = 1, GREEN = 2, BLUE = 3 };
    
    // Test basic enum formatting
    std::string result = fmt::format("{}", Color::RED);
    EXPECT_EQ(result, "1");
    
    result = fmt::format("{}", Color::GREEN);
    EXPECT_EQ(result, "2");
    
    result = fmt::format("{}", Color::BLUE);
    EXPECT_EQ(result, "3");
}

TEST(EnumFormatterTest, EnumWithCustomFormat) {
    enum Status { PENDING = 0, RUNNING = 1, COMPLETED = 2 };
    
    // Test with format specifiers
    std::string result = fmt::format("{:d}", Status::COMPLETED);
    EXPECT_EQ(result, "2");
    
    result = fmt::format("{:02d}", Status::RUNNING);
    EXPECT_EQ(result, "01");
}

TEST(EnumFormatterTest, EdgeCases) {
    enum SmallEnum { MIN_VAL = 0, MAX_VAL = 255 };
    
    // Test boundary values
    std::string result = fmt::format("{}", SmallEnum::MIN_VAL);
    EXPECT_EQ(result, "0");
    
    result = fmt::format("{}", SmallEnum::MAX_VAL);
    EXPECT_EQ(result, "255");
    
    // Test negative enum values if supported
    enum NegativeEnum { NEG_ONE = -1, NEG_ZERO = 0 };
    result = fmt::format("{}", NegativeEnum::NEG_ONE);
    EXPECT_EQ(result, "-1");
}

TEST(EnumFormatterTest, ComplexEnumValues) {
    enum Flags { FLAG_A = 1, FLAG_B = 2, FLAG_C = 4, FLAG_ALL = 7 };
    
    std::string result = fmt::format("{}", FLAG_ALL);
    EXPECT_EQ(result, "7");
    
    result = fmt::format("{:x}", FLAG_ALL);
    EXPECT_EQ(result, "7");
    
    result = fmt::format("{:b}", FLAG_ALL);
    EXPECT_EQ(result, "111");
}

TEST(EnumFormatterTest, PrecisionHandling) {
    enum TestEnum { VALUE = 42 };
    
    // Test precision specification (should be ignored for integers)
    std::string result = fmt::format("{:.5}", TestEnum::VALUE);
    EXPECT_EQ(result, "42");  // Precision should not affect integer output
    
    result = fmt::format("{:05.2}", TestEnum::VALUE);
    EXPECT_EQ(result, "00042");  // Zero padding should work
}

TEST(EnumFormatterTest, LargeEnumValues) {
    enum LargeEnum { 
        SMALL = 1, 
        MEDIUM = 1000, 
        LARGE = 1000000,
        MAX_ENUM = std::numeric_limits<int>::max()
    };
    
    std::string result = fmt::format("{}", LargeEnum::LARGE);
    EXPECT_EQ(result, "1000000");
    
    result = fmt::format("{}", LargeEnum::MEDIUM);
    EXPECT_EQ(result, "1000");
}

TEST(EnumFormatterTest, EnumWithZeroValue) {
    enum ZeroEnum { ZERO = 0, ONE = 1, TWO = 2 };
    
    std::string result = fmt::format("{}", ZeroEnum::ZERO);
    EXPECT_EQ(result, "0");
    
    result = fmt::format("{:03}", ZeroEnum::ZERO);
    EXPECT_EQ(result, "000");
}

TEST(EnumFormatterTest, FormatWithWidth) {
    enum TestEnum { VALUE = 5 };
    
    std::string result = fmt::format("{:5}", TestEnum::VALUE);
    EXPECT_EQ(result, "    5");  // Right-aligned width
    
    result = fmt::format("{:>5}", TestEnum::VALUE);
    EXPECT_EQ(result, "    5");  // Explicit right-aligned
    
    result = fmt::format("{:<5}", TestEnum::VALUE);
    EXPECT_EQ(result, "5    ");  // Left-aligned
}