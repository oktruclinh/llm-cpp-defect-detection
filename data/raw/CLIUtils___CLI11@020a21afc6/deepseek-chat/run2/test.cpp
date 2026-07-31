#include <gtest/gtest.h>
#include "CLI/CLI.hpp"

// Test fixture for Validator-related tests
class ValidatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        app.name("test_program");
    }

    CLI::App app;
};

// Test that the program name is stripped from the beginning of the command line
TEST_F(ValidatorTest, StripProgramName) {
    std::string commandline = "test_program --option value";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(result.at(0), "test_program");
    EXPECT_EQ(result.at(1), "--option value");
}

// Test with no arguments after program name
TEST_F(ValidatorTest, NoArguments) {
    std::string commandline = "test_program";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(result.at(0), "test_program");
    EXPECT_TRUE(result.at(1).empty());
}

// Test with only whitespace after program name
TEST_F(ValidatorTest, WhitespaceOnlyAfterProgram) {
    std::string commandline = "test_program   ";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(result.at(0), "test_program");
    EXPECT_TRUE(result.at(1).empty());
}

// Test with multiple spaces between program and arguments
TEST_F(ValidatorTest, MultipleSpacesBetween) {
    std::string commandline = "test_program    --option value";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(result.at(0), "test_program");
    EXPECT_EQ(result.at(1), "--option value");
}

// Test with leading whitespace before program name
TEST_F(ValidatorTest, LeadingWhitespace) {
    std::string commandline = "   test_program --option value";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(result.at(0), "test_program");
    EXPECT_EQ(result.at(1), "--option value");
}

// Test with program name containing special characters
TEST_F(ValidatorTest, ProgramNameWithSpecialChars) {
    std::string commandline = "./my_program_v2.0 --flag";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(result.at(0), "./my_program_v2.0");
    EXPECT_EQ(result.at(1), "--flag");
}

// Test with empty command line
TEST_F(ValidatorTest, EmptyCommandLine) {
    std::string commandline;
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_TRUE(result.at(0).empty());
    EXPECT_TRUE(result.at(1).empty());
}

// Test with only whitespace
TEST_F(ValidatorTest, OnlyWhitespace) {
    std::string commandline = "   ";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_TRUE(result.at(0).empty());
    EXPECT_TRUE(result.at(1).empty());
}

// Test with program name that is a substring of a later argument
TEST_F(ValidatorTest, ProgramNameSubstringInArgs) {
    std::string commandline = "test_program --test_program_option";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(result.at(0), "test_program");
    EXPECT_EQ(result.at(1), "--test_program_option");
}

// Test with quoted arguments containing spaces
TEST_F(ValidatorTest, QuotedArguments) {
    std::string commandline = "test_program --name \"John Doe\"";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(result.at(0), "test_program");
    EXPECT_EQ(result.at(1), "--name \"John Doe\"");
}

// Test with program name that has no space before arguments (should not happen in practice, but test edge case)
TEST_F(ValidatorTest, NoSpaceBeforeArgs) {
    std::string commandline = "test_program--option";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(result.at(0), "test_program--option");
    EXPECT_TRUE(result.at(1).empty());
}

// Test with program name containing spaces (unusual but possible)
TEST_F(ValidatorTest, ProgramNameWithSpaces) {
    std::string commandline = "\"my program\" --option value";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(result.at(0), "\"my program\"");
    EXPECT_EQ(result.at(1), "--option value");
}

// Test with multiple arguments and flags
TEST_F(ValidatorTest, MultipleArguments) {
    std::string commandline = "test_program -a -b --long value1 value2";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(result.at(0), "test_program");
    EXPECT_EQ(result.at(1), "-a -b --long value1 value2");
}

// Test that the function correctly handles the case where program name is the entire string
TEST_F(ValidatorTest, ProgramNameOnly) {
    std::string commandline = "test_program";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(result.at(0), "test_program");
    EXPECT_TRUE(result.at(1).empty());
}

// Test with program name followed by newline
TEST_F(ValidatorTest, ProgramNameWithNewline) {
    std::string commandline = "test_program\n--option value";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(result.at(0), "test_program");
    EXPECT_EQ(result.at(1), "--option value");
}

// Test with program name followed by tab
TEST_F(ValidatorTest, ProgramNameWithTab) {
    std::string commandline = "test_program\t--option value";
    auto result = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(result.at(0), "test_program");
    EXPECT_EQ(result.at(1), "--option value");
}