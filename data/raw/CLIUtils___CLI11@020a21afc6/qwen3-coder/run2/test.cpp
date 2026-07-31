#include <gtest/gtest.h>
#include "CLI/Validators.hpp"
#include "CLI/App.hpp"
#include "CLI/CLI.hpp"

TEST(ValidatorsTest, TestStripProgramName) {
    // Test normal case with program name and arguments
    std::string commandline = "/path/to/program arg1 arg2 arg3";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1 arg2 arg3");
    
    // Test case with no arguments
    commandline = "/path/to/program";
    result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "");
    
    // Test case with only spaces after program name
    commandline = "/path/to/program   ";
    result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "");
    
    // Test case with program name and leading spaces
    commandline = "   /path/to/program arg1";
    result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1");
    
    // Test empty string
    commandline = "";
    result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "");
    EXPECT_EQ(result.second, "");
    
    // Test single character program name
    commandline = "a arg1";
    result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "a");
    EXPECT_EQ(result.second, "arg1");
    
    // Test program name with spaces
    commandline = "/path/to/my program arg1";
    result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/my program");
    EXPECT_EQ(result.second, "arg1");
    
    // Test program name at end of string with no space
    commandline = "/path/to/program";
    result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "");
    
    // Test program name at end of string with trailing space
    commandline = "/path/to/program ";
    result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "");
    
    // Test complex case with multiple spaces
    commandline = "   /path/to/program   arg1   arg2   ";
    result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1   arg2");
}