#include <gtest/gtest.h>
#include <CLI/CLI.hpp>

// Test the focal function's behavior through the public API: CLI::App::parse(const std::string&)
// This function internally splits a command line string into program name and arguments,
// which is the exact code path containing the provided snippet.

TEST(CommandLineSplit, NormalProgramAndArgs) {
    CLI::App app;
    int flag = 0;
    app.add_option("--flag", flag, "A flag");
    std::string pos;
    app.add_option("pos", pos, "A positional argument");
    // The parse method expects a full command line including program name.
    app.parse("myprogram --flag 42 pos_value");
    EXPECT_EQ(flag, 42);
    EXPECT_EQ(pos, "pos_value");
}

TEST(CommandLineSplit, ProgramNameOnly) {
    CLI::App app;
    int flag = 0;
    app.add_option("--flag", flag);
    // No arguments after program name – nothing should be parsed.
    app.parse("myprogram");
    EXPECT_EQ(flag, 0);
    // Ensure no error or leftover arguments.
    EXPECT_EQ(app.remaining_size(), 0u);
}

TEST(CommandLineSplit, ProgramNameWithTrailingSpaces) {
    CLI::App app;
    int flag = 0;
    app.add_option("--flag", flag);
    // Trailing spaces after the program name should be ignored.
    app.parse("myprogram