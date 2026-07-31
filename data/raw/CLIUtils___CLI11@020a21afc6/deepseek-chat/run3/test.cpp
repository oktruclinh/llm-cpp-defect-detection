#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include <string>

// Test fixture for validator-related tests
class ValidatorTest : public ::testing::Test {
protected:
    CLI::App app;
    std::string output;
};

// Test that the program name is stripped from the beginning of the command line
TEST_F(ValidatorTest, StripProgramName) {
    std::string commandline = "./myprogram --input file.txt";
    auto vals = CLI::detail::split_up(commandline);
    
    // The first element should be the program name
    EXPECT_EQ(vals[0], "./myprogram");
    // The second element should be everything after the program name, trimmed
    EXPECT_EQ(vals[1], "--input file.txt");
}

// Test with no arguments after program name
TEST_F(ValidatorTest, NoArguments) {
    std::string commandline = "./myprogram";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(vals[0], "./myprogram");
    EXPECT_EQ(vals.size(), 1);
}

// Test with only whitespace after program name
TEST_F(ValidatorTest, WhitespaceOnlyAfterProgram) {
    std::string commandline = "./myprogram   ";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(vals[0], "./myprogram");
    EXPECT_EQ(vals.size(), 1);
}

// Test with program name at the very end (no space after)
TEST_F(ValidatorTest, ProgramNameAtEnd) {
    std::string commandline = "./myprogram";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(vals[0], "./myprogram");
    EXPECT_EQ(vals.size(), 1);
}

// Test with multiple spaces between program name and arguments
TEST_F(ValidatorTest, MultipleSpacesBetween) {
    std::string commandline = "./myprogram     --verbose";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(vals[0], "./myprogram");
    EXPECT_EQ(vals[1], "--verbose");
}

// Test with leading whitespace in command line
TEST_F(ValidatorTest, LeadingWhitespace) {
    std::string commandline = "  ./myprogram --input file.txt";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(vals[0], "./myprogram");
    EXPECT_EQ(vals[1], "--input file.txt");
}

// Test with quoted arguments containing spaces
TEST_F(ValidatorTest, QuotedArguments) {
    std::string commandline = "./myprogram --name \"John Doe\"";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(vals[0], "./myprogram");
    EXPECT_EQ(vals[1], "--name \"John Doe\"");
}

// Test with empty command line
TEST_F(ValidatorTest, EmptyCommandLine) {
    std::string commandline = "";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_TRUE(vals.empty());
}

// Test with only whitespace
TEST_F(ValidatorTest, OnlyWhitespace) {
    std::string commandline = "   ";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_TRUE(vals.empty());
}

// Test with program name containing path
TEST_F(ValidatorTest, ProgramNameWithPath) {
    std::string commandline = "/usr/local/bin/myapp --version";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(vals[0], "/usr/local/bin/myapp");
    EXPECT_EQ(vals[1], "--version");
}

// Test with program name containing dots
TEST_F(ValidatorTest, ProgramNameWithDots) {
    std::string commandline = "./test.program --flag";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(vals[0], "./test.program");
    EXPECT_EQ(vals[1], "--flag");
}

// Test that the second part is left-trimmed (no leading spaces)
TEST_F(ValidatorTest, SecondPartTrimmed) {
    std::string commandline = "./program   arg1 arg2";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(vals[0], "./program");
    // Ensure no leading spaces in the second part
    EXPECT_EQ(vals[1].front(), 'a');
    EXPECT_EQ(vals[1], "arg1 arg2");
}

// Test with single character program name
TEST_F(ValidatorTest, SingleCharProgramName) {
    std::string commandline = "a --option";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(vals[0], "a");
    EXPECT_EQ(vals[1], "--option");
}

// Test with program name that is just a dot
TEST_F(ValidatorTest, DotAsProgramName) {
    std::string commandline = ". --help";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(vals[0], ".");
    EXPECT_EQ(vals[1], "--help");
}

// Test with very long program name
TEST_F(ValidatorTest, LongProgramName) {
    std::string long_name = "./" + std::string(1000, 'a') + ".exe";
    std::string commandline = long_name + " --long-option";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(vals[0], long_name);
    EXPECT_EQ(vals[1], "--long-option");
}

// Test with special characters in program name
TEST_F(ValidatorTest, SpecialCharsInProgramName) {
    std::string commandline = "./my-app_v2.0 --config";
    auto vals = CLI::detail::split_up(commandline);
    
    EXPECT_EQ(vals[0], "./my-app_v2.0");
    EXPECT_EQ(vals[1], "--config");
}

// Test that the function correctly handles the case where program name contains spaces (should not happen in practice)
TEST_F(ValidatorTest, ProgramNameWithSpaces) {
    std::string commandline = "\"my program\" --option";
    auto vals = CLI::detail::split_up(commandline);
    
    // The function splits on first space, so quoted program name is not handled specially
    EXPECT_EQ(vals[0], "\"my");
    EXPECT_EQ(vals[1], "program\" --option");
}