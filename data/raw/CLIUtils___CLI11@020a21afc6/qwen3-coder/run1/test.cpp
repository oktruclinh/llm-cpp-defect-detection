#include <gtest/gtest.h>
#include "CLI/App.hpp"
#include "CLI/CLI.hpp"

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

TEST(ValidatorsTest, StripProgramNameMultipleSpaces) {
    // Test program name with multiple spaces before arguments
    std::string commandline = "/path/to/program    arg1 arg2";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1 arg2");
}

TEST(ValidatorsTest, StripProgramNameLeadingSpaces) {
    // Test command line with leading spaces
    std::string commandline = "   /path/to/program arg1";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1");
}

TEST(ValidatorsTest, StripProgramNameTrailingSpaces) {
    // Test command line with trailing spaces after program name
    std::string commandline = "/path/to/program   arg1";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1");
}

TEST(ValidatorsTest, StripProgramNameOnlySpaces) {
    // Test command line with only spaces
    std::string commandline = "   ";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "");
    EXPECT_EQ(result.second, "");
}

TEST(ValidatorsTest, StripProgramNameNoPath) {
    // Test program name without path
    std::string commandline = "program arg1 arg2";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "program");
    EXPECT_EQ(result.second, "arg1 arg2");
}

TEST(ValidatorsTest, StripProgramNameRelativePath) {
    // Test relative path program name
    std::string commandline = "./program arg1";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "./program");
    EXPECT_EQ(result.second, "arg1");
}

TEST(ValidatorsTest, StripProgramNameWithSpecialChars) {
    // Test program name with special characters
    std::string commandline = "/path/to/my-program_v1.0 arg1";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/my-program_v1.0");
    EXPECT_EQ(result.second, "arg1");
}

TEST(ValidatorsTest, StripProgramNameWithQuotes) {
    // Test program name with quotes in arguments
    std::string commandline = "/path/to/program \"arg with spaces\" arg2";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "\"arg with spaces\" arg2");
}