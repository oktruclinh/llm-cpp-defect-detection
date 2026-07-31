#include <gtest/gtest.h>
#include "CLI/CLI.hpp"

// Test fixture for Validator-related tests
class ValidatorTest : public ::testing::Test {
protected:
    CLI::App app{"Test App"};
};

// Test that a simple validator works correctly
TEST_F(ValidatorTest, SimpleValidator) {
    int val = 0;
    app.add_option("--val", val, "A value")->check(CLI::Range(1, 10));
    
    // Valid input
    app.parse("--val 5");
    EXPECT_EQ(val, 5);
    
    // Invalid input should throw
    EXPECT_THROW(app.parse("--val 15"), CLI::ValidationError);
}

// Test the Range validator with various edge cases
TEST_F(ValidatorTest, RangeValidator) {
    int val = 0;
    auto* opt = app.add_option("--val", val, "A value");
    opt->check(CLI::Range(0, 100));
    
    // Lower bound
    app.parse("--val 0");
    EXPECT_EQ(val, 0);
    
    // Upper bound
    app.parse("--val 100");
    EXPECT_EQ(val, 100);
    
    // Just below lower bound
    EXPECT_THROW(app.parse("--val -1"), CLI::ValidationError);
    
    // Just above upper bound
    EXPECT_THROW(app.parse("--val 101"), CLI::ValidationError);
}

// Test the PositiveNumber validator
TEST_F(ValidatorTest, PositiveNumberValidator) {
    int val = 0;
    auto* opt = app.add_option("--val", val, "A value");
    opt->check(CLI::PositiveNumber);
    
    // Positive number
    app.parse("--val 42");
    EXPECT_EQ(val, 42);
    
    // Zero should fail
    EXPECT_THROW(app.parse("--val 0"), CLI::ValidationError);
    
    // Negative should fail
    EXPECT_THROW(app.parse("--val -5"), CLI::ValidationError);
}

// Test the NonNegativeNumber validator
TEST_F(ValidatorTest, NonNegativeNumberValidator) {
    int val = 0;
    auto* opt = app.add_option("--val", val, "A value");
    opt->check(CLI::NonNegativeNumber);
    
    // Zero should pass
    app.parse("--val 0");
    EXPECT_EQ(val, 0);
    
    // Positive should pass
    app.parse("--val 10");
    EXPECT_EQ(val, 10);
    
    // Negative should fail
    EXPECT_THROW(app.parse("--val -1"), CLI::ValidationError);
}

// Test the ExistingFile validator
TEST_F(ValidatorTest, ExistingFileValidator) {
    std::string filename;
    auto* opt = app.add_option("--file", filename, "A file");
    opt->check(CLI::ExistingFile);
    
    // Non-existent file should fail
    EXPECT_THROW(app.parse("--file /nonexistent/file.txt"), CLI::ValidationError);
}

// Test the ExistingDirectory validator
TEST_F(ValidatorTest, ExistingDirectoryValidator) {
    std::string dirname;
    auto* opt = app.add_option("--dir", dirname, "A directory");
    opt->check(CLI::ExistingDirectory);
    
    // Non-existent directory should fail
    EXPECT_THROW(app.parse("--dir /nonexistent/directory"), CLI::ValidationError);
}

// Test the NonexistentPath validator
TEST_F(ValidatorTest, NonexistentPathValidator) {
    std::string path;
    auto* opt = app.add_option("--path", path, "A path");
    opt->check(CLI::NonexistentPath);
    
    // Non-existent path should pass
    app.parse("--path /unique/nonexistent/path_12345");
    EXPECT_EQ(path, "/unique/nonexistent/path_12345");
}

// Test the ExistingPath validator
TEST_F(ValidatorTest, ExistingPathValidator) {
    std::string path;
    auto* opt = app.add_option("--path", path, "A path");
    opt->check(CLI::ExistingPath);
    
    // Non-existent path should fail
    EXPECT_THROW(app.parse("--path /nonexistent/path"), CLI::ValidationError);
}

// Test validator chaining
TEST_F(ValidatorTest, ValidatorChaining) {
    int val = 0;
    auto* opt = app.add_option("--val", val, "A value");
    opt->check(CLI::Range(1, 100) | CLI::PositiveNumber);
    
    // Valid input
    app.parse("--val 50");
    EXPECT_EQ(val, 50);
    
    // Should fail both validators
    EXPECT_THROW(app.parse("--val 0"), CLI::ValidationError);
    EXPECT_THROW(app.parse("--val -5"), CLI::ValidationError);
    EXPECT_THROW(app.parse("--val 101"), CLI::ValidationError);
}

// Test custom validator with lambda
TEST_F(ValidatorTest, CustomLambdaValidator) {
    std::string str;
    auto* opt = app.add_option("--str", str, "A string");
    opt->check([](const std::string& input) {
        if (input.length() < 3) {
            return std::string("String must be at least 3 characters long");
        }
        return std::string();
    });
    
    // Valid input
    app.parse("--str hello");
    EXPECT_EQ(str, "hello");
    
    // Invalid input
    EXPECT_THROW(app.parse("--str hi"), CLI::ValidationError);
}

// Test validator with empty input
TEST_F(ValidatorTest, EmptyInputValidator) {
    std::string str;
    auto* opt = app.add_option("--str", str, "A string");
    opt->check(CLI::ExistingFile);
    
    // Empty string should fail
    EXPECT_THROW(app.parse("--str \"\""), CLI::ValidationError);
}

// Test that validator is applied correctly with default values
TEST_F(ValidatorTest, ValidatorWithDefault) {
    int val = 5;
    app.add_option("--val", val, "A value")->check(CLI::Range(1, 10));
    
    // Default value should be preserved
    EXPECT_EQ(val, 5);
    
    // Override with valid value
    app.parse("--val 7");
    EXPECT_EQ(val, 7);
}

// Test multiple validators on same option
TEST_F(ValidatorTest, MultipleValidators) {
    int val = 0;
    auto* opt = app.add_option("--val", val, "A value");
    opt->check(CLI::Range(1, 100));
    opt->check(CLI::PositiveNumber);
    
    // Valid input
    app.parse("--val 50");
    EXPECT_EQ(val, 50);
    
    // Should fail first validator
    EXPECT_THROW(app.parse("--val 150"), CLI::ValidationError);
    
    // Should fail second validator
    EXPECT_THROW(app.parse("--val 0"), CLI::ValidationError);
}