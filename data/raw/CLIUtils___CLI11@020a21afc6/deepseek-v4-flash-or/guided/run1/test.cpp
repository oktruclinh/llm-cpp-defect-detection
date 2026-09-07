#include <gtest/gtest.h>
#include <CLI/CLI.hpp>

TEST(CommandLineSplit, NormalProgramAndArgs) {
    CLI::App app;
    int flag = 0;
    app.add_option("--flag", flag, "A flag");
    std::string pos;
    app.add_option("pos", pos, "A positional argument");

    app.parse("myprogram --flag 42 pos_value");

    EXPECT_EQ(flag, 42);
    EXPECT_EQ(pos, "pos_value");
}

TEST(CommandLineSplit, ProgramNameOnly) {
    CLI::App app;
    int flag = 0;
    app.add_option("--flag", flag);

    app.parse("myprogram");

    EXPECT_EQ(flag, 0);
}

TEST(CommandLineSplit, ProgramNameWithTrailingSpaces) {
    CLI::App app;
    int flag = 0;
    app.add_option("--flag", flag);

    // Trailing spaces after the program name should be ignored.
    app.parse("myprogram   ");

    EXPECT_EQ(flag, 0);
}

TEST(CommandLineSplit, MultipleSpacesBeforeArgs) {
    CLI::App app;
    int value = 0;
    app.add_option("--value", value);

    // Multiple spaces between the program name and the first argument
    // must be trimmed before tokenization.
    app.parse("myprogram    --value 7");

    EXPECT_EQ(value, 7);
}