#include <gtest/gtest.h>
#include "CLI/Validators.hpp"
#include "CLI/App.hpp"
#include "CLI/CLI.hpp"

TEST(ValidatorsTest, SplitProgramNameNormalCase) {
    // Test normal case where program name is present
    std::string commandline = "/path/to/program arg1 arg2";
    auto result = CLI::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1 arg2");
}

TEST(ValidatorsTest, SplitProgramNameNoArgs) {
    // Test case where there are no arguments after program name
    std::string commandline = "/path/to/program";
    auto result = CLI::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "");
}

TEST(ValidatorsTest, SplitProgramNameEmptyString) {
    // Test empty string case
    std::string commandline = "";
    auto result = CLI::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "");
    EXPECT_EQ(result.second, "");
}

TEST(ValidatorsTest, SplitProgramNameOnlySpaces) {
    // Test string with only spaces
    std::string commandline = "   ";
    auto result = CLI::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "");
    EXPECT_EQ(result.second, "");
}

TEST(ValidatorsTest, SplitProgramNameLeadingSpaces) {
    // Test leading spaces in command line
    std::string commandline = "   /path/to/program arg1";
    auto result = CLI::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1");
}

TEST(ValidatorsTest, SplitProgramNameMultipleSpaces) {
    // Test multiple spaces between program name and args
    std::string commandline = "/path/to/program    arg1   arg2";
    auto result = CLI::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1   arg2");
}

TEST(ValidatorsTest, SplitProgramNameSingleCharProgram) {
    // Test program name with single character
    std::string commandline = "a arg1";
    auto result = CLI::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "a");
    EXPECT_EQ(result.second, "arg1");
}

TEST(ValidatorsTest, SplitProgramNameComplexPath) {
    // Test complex path with special characters
    std::string commandline = "/usr/bin/my-program-with-dashes arg1 arg2";
    auto result = CLI::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/usr/bin/my-program-with-dashes");
    EXPECT_EQ(result.second, "arg1 arg2");
}

TEST(ValidatorsTest, SplitProgramNameWindowsPath) {
    // Test Windows-style path
    std::string commandline = "C:\\path\\to\\program.exe arg1";
    auto result = CLI::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "C:\\path\\to\\program.exe");
    EXPECT_EQ(result.second, "arg1");
}

TEST(ValidatorsTest, SplitProgramNameNoSpaceAfterProgram) {
    // Test program name at end with no trailing space
    std::string commandline = "/path/to/program";
    auto result = CLI::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "");
}

TEST(ValidatorsTest, SplitProgramNameTrailingSpaces) {
    // Test trailing spaces after program name
    std::string commandline = "/path/to/program   ";
    auto result = CLI::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "");
}