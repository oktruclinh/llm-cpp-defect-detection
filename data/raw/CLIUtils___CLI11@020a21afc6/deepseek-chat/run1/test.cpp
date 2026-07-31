#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include <string>
#include <vector>

// Test fixture for command line parsing with program name stripping
class CommandLineParsingTest : public ::testing::Test {
protected:
    CLI::App app;
    
    void SetUp() override {
        app.name("test_program");
    }
};

// Helper to parse a command line string and return the parsed arguments
std::vector<std::string> parseCommandLine(const std::string& commandline) {
    CLI::App temp_app;
    temp_app.name("test_program");
    std::vector<std::string> args;
    temp_app.parse(commandline);
    // After parsing, the remaining arguments are stored in the app
    // We need to access them through the public API
    // For CLI11, we can use the parse method that returns void and then check the app state
    return temp_app.remaining();
}

// Test that a simple command with no arguments is parsed correctly
TEST_F(CommandLineParsingTest, SimpleCommandNoArgs) {
    std::string commandline = "test_program";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "test_program");
    EXPECT_TRUE(temp_app.remaining().empty());
}

// Test that a command with one argument is parsed correctly
TEST_F(CommandLineParsingTest, CommandWithOneArg) {
    std::string commandline = "test_program arg1";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "test_program");
    ASSERT_EQ(temp_app.remaining().size(), 1);
    EXPECT_EQ(temp_app.remaining()[0], "arg1");
}

// Test that a command with multiple arguments is parsed correctly
TEST_F(CommandLineParsingTest, CommandWithMultipleArgs) {
    std::string commandline = "test_program arg1 arg2 arg3";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "test_program");
    ASSERT_EQ(temp_app.remaining().size(), 3);
    EXPECT_EQ(temp_app.remaining()[0], "arg1");
    EXPECT_EQ(temp_app.remaining()[1], "arg2");
    EXPECT_EQ(temp_app.remaining()[2], "arg3");
}

// Test that leading whitespace before program name is stripped
TEST_F(CommandLineParsingTest, LeadingWhitespace) {
    std::string commandline = "  test_program arg1";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "test_program");
    ASSERT_EQ(temp_app.remaining().size(), 1);
    EXPECT_EQ(temp_app.remaining()[0], "arg1");
}

// Test that trailing whitespace after arguments is stripped
TEST_F(CommandLineParsingTest, TrailingWhitespace) {
    std::string commandline = "test_program arg1   ";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "test_program");
    ASSERT_EQ(temp_app.remaining().size(), 1);
    EXPECT_EQ(temp_app.remaining()[0], "arg1");
}

// Test that multiple spaces between program name and arguments are handled
TEST_F(CommandLineParsingTest, MultipleSpacesBetween) {
    std::string commandline = "test_program    arg1";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "test_program");
    ASSERT_EQ(temp_app.remaining().size(), 1);
    EXPECT_EQ(temp_app.remaining()[0], "arg1");
}

// Test that empty command line returns empty program name and no arguments
TEST_F(CommandLineParsingTest, EmptyCommandLine) {
    std::string commandline = "";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_TRUE(temp_app.get_name().empty());
    EXPECT_TRUE(temp_app.remaining().empty());
}

// Test that command line with only whitespace returns empty program name and no arguments
TEST_F(CommandLineParsingTest, OnlyWhitespace) {
    std::string commandline = "   ";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_TRUE(temp_app.get_name().empty());
    EXPECT_TRUE(temp_app.remaining().empty());
}

// Test that program name with special characters is handled correctly
TEST_F(CommandLineParsingTest, ProgramNameWithSpecialChars) {
    std::string commandline = "my-program_v2.0 arg1";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "my-program_v2.0");
    ASSERT_EQ(temp_app.remaining().size(), 1);
    EXPECT_EQ(temp_app.remaining()[0], "arg1");
}

// Test that arguments with special characters are preserved
TEST_F(CommandLineParsingTest, ArgsWithSpecialChars) {
    std::string commandline = "test_program --flag=value \"quoted arg\"";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "test_program");
    ASSERT_EQ(temp_app.remaining().size(), 2);
    EXPECT_EQ(temp_app.remaining()[0], "--flag=value");
    EXPECT_EQ(temp_app.remaining()[1], "\"quoted arg\"");
}

// Test that program name with path is handled correctly
TEST_F(CommandLineParsingTest, ProgramNameWithPath) {
    std::string commandline = "/usr/local/bin/test_program arg1";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "/usr/local/bin/test_program");
    ASSERT_EQ(temp_app.remaining().size(), 1);
    EXPECT_EQ(temp_app.remaining()[0], "arg1");
}

// Test that program name with relative path is handled correctly
TEST_F(CommandLineParsingTest, ProgramNameWithRelativePath) {
    std::string commandline = "./test_program arg1";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "./test_program");
    ASSERT_EQ(temp_app.remaining().size(), 1);
    EXPECT_EQ(temp_app.remaining()[0], "arg1");
}

// Test that program name with only one character is handled correctly
TEST_F(CommandLineParsingTest, SingleCharProgramName) {
    std::string commandline = "a arg1";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "a");
    ASSERT_EQ(temp_app.remaining().size(), 1);
    EXPECT_EQ(temp_app.remaining()[0], "arg1");
}

// Test that program name with only one character and no arguments
TEST_F(CommandLineParsingTest, SingleCharProgramNameNoArgs) {
    std::string commandline = "a";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "a");
    EXPECT_TRUE(temp_app.remaining().empty());
}

// Test that command line with only program name and trailing whitespace
TEST_F(CommandLineParsingTest, ProgramNameWithTrailingWhitespace) {
    std::string commandline = "test_program   ";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "test_program");
    EXPECT_TRUE(temp_app.remaining().empty());
}

// Test that command line with only program name and leading whitespace
TEST_F(CommandLineParsingTest, ProgramNameWithLeadingWhitespace) {
    std::string commandline = "   test_program";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "test_program");
    EXPECT_TRUE(temp_app.remaining().empty());
}

// Test that command line with multiple spaces and tabs is handled
TEST_F(CommandLineParsingTest, MixedWhitespace) {
    std::string commandline = "  test_program  \t arg1  \t arg2  ";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_EQ(temp_app.get_name(), "test_program");
    ASSERT_EQ(temp_app.remaining().size(), 2);
    EXPECT_EQ(temp_app.remaining()[0], "arg1");
    EXPECT_EQ(temp_app.remaining()[1], "arg2");
}

// Test that command line with only one argument and no program name
TEST_F(CommandLineParsingTest, NoProgramNameWithArg) {
    std::string commandline = " arg1";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_TRUE(temp_app.get_name().empty());
    ASSERT_EQ(temp_app.remaining().size(), 1);
    EXPECT_EQ(temp_app.remaining()[0], "arg1");
}

// Test that command line with only arguments and no program name
TEST_F(CommandLineParsingTest, NoProgramNameWithMultipleArgs) {
    std::string commandline = " arg1 arg2 arg3";
    CLI::App temp_app;
    temp_app.name("test_program");
    temp_app.parse(commandline);
    EXPECT_TRUE(temp_app.get_name().empty());
    ASSERT_EQ(temp_app.remaining().size(), 3);
    EXPECT_EQ(temp_app.remaining()[0], "arg1");
    EXPECT_EQ(temp_app.remaining()[1], "arg2");
    EXPECT_EQ(temp_app.remaining()[2], "arg3");
}