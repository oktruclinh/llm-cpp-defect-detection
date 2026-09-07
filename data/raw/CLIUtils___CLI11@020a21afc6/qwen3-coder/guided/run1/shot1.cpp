#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include "CLI/Validators.hpp"

TEST(ValidatorsTest, StripProgramNameNormalCase) {
    // Test normal case where program name is present and followed by arguments
    std::string commandline = "/path/to/program arg1 arg2 arg3";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1 arg2 arg3");
}

TEST(ValidatorsTest, StripProgramNameNoArguments) {
    // Test case where program name is present but no arguments follow
    std::string commandline = "/path/to/program";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "");
}

TEST(ValidatorsTest, StripProgramNameEmptyString) {
    // Test empty command line
    std::string commandline = "";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "");
    EXPECT_EQ(result.second, "");
}

TEST(ValidatorsTest, StripProgramNameSingleChar) {
    // Test single character command line
    std::string commandline = "a";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "a");
    EXPECT_EQ(result.second, "");
}

TEST(ValidatorsTest, StripProgramNameOnlySpaces) {
    // Test command line with only spaces
    std::string commandline = "   ";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "   ");
    EXPECT_EQ(result.second, "");
}

TEST(ValidatorsTest, StripProgramNameLeadingSpaces) {
    // Test command line with leading spaces before program name
    std::string commandline = "   /path/to/program arg1";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "   /path/to/program");
    EXPECT_EQ(result.second, "arg1");
}

TEST(ValidatorsTest, StripProgramNameTrailingSpaces) {
    // Test command line with trailing spaces after program name
    std::string commandline = "/path/to/program   ";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "");
}

TEST(ValidatorsTest, StripProgramNameMultipleSpaces) {
    // Test command line with multiple spaces between program name and arguments
    std::string commandline = "/path/to/program    arg1   arg2";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1   arg2");
}

TEST(ValidatorsTest, StripProgramNameNoPath) {
    // Test command line with just program name (no path)
    std::string commandline = "program arg1";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "program");
    EXPECT_EQ(result.second, "arg1");
}

TEST(ValidatorsTest, StripProgramNameComplexPath) {
    // Test command line with complex path and arguments
    std::string commandline = "/usr/local/bin/my_program --verbose -o output.txt";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/usr/local/bin/my_program");
    EXPECT_EQ(result.second, "--verbose -o output.txt");
}