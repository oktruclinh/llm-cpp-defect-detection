#include <gtest/gtest.h>
#include "CLI/CLI.hpp"

// Test the function that strips the program name from a command line string
// This function is part of the internal parsing utilities in CLI11

TEST(StripProgramName, EmptyString) {
    std::string commandline = "";
    auto result = CLI::detail::split_up(commandline);
    EXPECT_TRUE(result.empty());
}

TEST(StripProgramName, OnlyProgramName) {
    std::string commandline = "myprogram";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0], "myprogram");
}

TEST(StripProgramName, ProgramNameWithOneArgument) {
    std::string commandline = "myprogram arg1";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], "myprogram");
    EXPECT_EQ(result[1], "arg1");
}

TEST(StripProgramName, ProgramNameWithMultipleArguments) {
    std::string commandline = "myprogram --flag value -o output.txt";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 4u);
    EXPECT_EQ(result[0], "myprogram");
    EXPECT_EQ(result[1], "--flag");
    EXPECT_EQ(result[2], "value");
    EXPECT_EQ(result[3], "-o");
    EXPECT_EQ(result[4], "output.txt");
}

TEST(StripProgramName, LeadingWhitespace) {
    std::string commandline = "   myprogram arg1";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], "myprogram");
    EXPECT_EQ(result[1], "arg1");
}

TEST(StripProgramName, TrailingWhitespace) {
    std::string commandline = "myprogram arg1   ";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], "myprogram");
    EXPECT_EQ(result[1], "arg1");
}

TEST(StripProgramName, MultipleSpacesBetweenTokens) {
    std::string commandline = "myprogram    arg1     arg2";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], "myprogram");
    EXPECT_EQ(result[1], "arg1");
    EXPECT_EQ(result[2], "arg2");
}

TEST(StripProgramName, QuotedArguments) {
    std::string commandline = "myprogram \"arg with spaces\" 'single quoted'";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], "myprogram");
    EXPECT_EQ(result[1], "arg with spaces");
    EXPECT_EQ(result[2], "single quoted");
}

TEST(StripProgramName, MixedQuotesAndSpaces) {
    std::string commandline = "myprogram --name=\"John Doe\" --path='/some/path with spaces'";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], "myprogram");
    EXPECT_EQ(result[1], "--name=John Doe");
    EXPECT_EQ(result[2], "--path=/some/path with spaces");
}

TEST(StripProgramName, EscapedCharacters) {
    std::string commandline = "myprogram arg\\ with\\ spaces";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], "myprogram");
    EXPECT_EQ(result[1], "arg with spaces");
}

TEST(StripProgramName, OnlyWhitespace) {
    std::string commandline = "   \t  \n  ";
    auto result = CLI::detail::split_up(commandline);
    EXPECT_TRUE(result.empty());
}

TEST(StripProgramName, ProgramNameWithPath) {
    std::string commandline = "/usr/local/bin/myprogram --version";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], "/usr/local/bin/myprogram");
    EXPECT_EQ(result[1], "--version");
}

TEST(StripProgramName, WindowsPath) {
    std::string commandline = "C:\\Program Files\\MyApp\\app.exe --help";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], "C:\\Program Files\\MyApp\\app.exe");
    EXPECT_EQ(result[1], "--help");
}

TEST(StripProgramName, SingleCharacterProgramName) {
    std::string commandline = "a -b";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "-b");
}

TEST(StripProgramName, ProgramNameWithSpecialCharacters) {
    std::string commandline = "my-program_v2.0 --flag";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], "my-program_v2.0");
    EXPECT_EQ(result[1], "--flag");
}

TEST(StripProgramName, NoArgumentsAfterProgramName) {
    std::string commandline = "myprogram ";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0], "myprogram");
}

TEST(StripProgramName, OnlyProgramNameWithTrailingSpaces) {
    std::string commandline = "myprogram   ";
    auto result = CLI::detail::split_up(commandline);
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0], "myprogram");
}