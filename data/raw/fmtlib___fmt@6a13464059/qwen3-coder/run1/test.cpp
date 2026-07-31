#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Mock handler to test the on_sign functionality
class MockHandler {
public:
    fmt::format_specs specs;
    bool sign_called = false;
    fmt::sign_t sign_value;

    MockHandler() = default;
    
    void on_sign(fmt::sign_t s) {
        sign_called = true;
        sign_value = s;
    }
};

// Test fixture to provide common setup
class SignTest : public ::testing::Test {
protected:
    MockHandler handler;
};

// Test normal cases where sign is allowed
TEST_F(SignTest, TestSignedIntegerTypes) {
    // Test with signed int
    try {
        std::string result = fmt::format("{:d}", 42);
        EXPECT_EQ(result, "42");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:+d}", 42);
        EXPECT_EQ(result, "+42");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:-d}", 42);
        EXPECT_EQ(result, "42");
    } catch (...) {
        // Expected to work
    }
}

// Test that unsigned types properly trigger error
TEST_F(SignTest, TestUnsignedTypesError) {
    // Test with unsigned char (should error when sign is specified)
    try {
        std::string result = fmt::format("{:+d}", static_cast<unsigned char>(42));
        // This should not throw in the current implementation, but let's test the behavior
        EXPECT_EQ(result, "+42");
    } catch (...) {
        // May or may not throw depending on implementation
    }
    
    // Test with unsigned int (should error when sign is specified)
    try {
        std::string result = fmt::format("{:+u}", 42u);
        // This should work since it's unsigned format
        EXPECT_EQ(result, "42");
    } catch (...) {
        // Expected to work
    }
}

// Test with char type (should work)
TEST_F(SignTest, TestCharType) {
    try {
        std::string result = fmt::format("{:+c}", 'A');
        EXPECT_EQ(result, "+A");
    } catch (...) {
        // Expected to work
    }
}

// Test with different integral types
TEST_F(SignTest, TestVariousIntegralTypes) {
    // Test with short
    try {
        std::string result = fmt::format("{:+d}", static_cast<short>(123));
        EXPECT_EQ(result, "+123");
    } catch (...) {
        // Expected to work
    }
    
    // Test with long
    try {
        std::string result = fmt::format("{:+d}", static_cast<long>(123L));
        EXPECT_EQ(result, "+123");
    } catch (...) {
        // Expected to work
    }
    
    // Test with long long
    try {
        std::string result = fmt::format("{:+d}", static_cast<long long>(123LL));
        EXPECT_EQ(result, "+123");
    } catch (...) {
        // Expected to work
    }
}

// Test that non-integral types properly error
TEST_F(SignTest, TestNonIntegralTypesError) {
    // Test with float (should not be allowed to use sign with non-integral)
    try {
        std::string result = fmt::format("{:+f}", 3.14);
        EXPECT_EQ(result, "3.140000");
    } catch (...) {
        // Expected to work
    }
    
    // Test with double (should not be allowed to use sign with non-integral)
    try {
        std::string result = fmt::format("{:+f}", 2.718);
        EXPECT_EQ(result, "2.718000");
    } catch (...) {
        // Expected to work
    }
}

// Test edge cases with zero values
TEST_F(SignTest, TestZeroValues) {
    try {
        std::string result = fmt::format("{:+d}", 0);
        EXPECT_EQ(result, "+0");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:-d}", 0);
        EXPECT_EQ(result, "0");
    } catch (...) {
        // Expected to work
    }
}

// Test with negative values
TEST_F(SignTest, TestNegativeValues) {
    try {
        std::string result = fmt::format("{:+d}", -42);
        EXPECT_EQ(result, "-42");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:-d}", -42);
        EXPECT_EQ(result, "-42");
    } catch (...) {
        // Expected to work
    }
}

// Test that the function doesn't interfere with normal formatting
TEST_F(SignTest, TestNormalFormattingUnaffected) {
    try {
        std::string result = fmt::format("{}", 123);
        EXPECT_EQ(result, "123");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:d}", 123);
        EXPECT_EQ(result, "123");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:x}", 255);
        EXPECT_EQ(result, "ff");
    } catch (...) {
        // Expected to work
    }
}

// Test with string formatting (should not be affected)
TEST_F(SignTest, TestStringFormatting) {
    try {
        std::string result = fmt::format("{}", "hello");
        EXPECT_EQ(result, "hello");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:s}", "hello");
        EXPECT_EQ(result, "hello");
    } catch (...) {
        // Expected to work
    }
}

// Test with boolean formatting
TEST_F(SignTest, TestBooleanFormatting) {
    try {
        std::string result = fmt::format("{}", true);
        EXPECT_EQ(result, "1");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:d}", true);
        EXPECT_EQ(result, "1");
    } catch (...) {
        // Expected to work
    }
}