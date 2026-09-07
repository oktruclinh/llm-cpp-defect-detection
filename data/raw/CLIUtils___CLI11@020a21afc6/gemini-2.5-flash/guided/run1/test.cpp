#include <gtest/gtest.h>
#include <CLI/CLI.hpp> // Public API header

// This test suite focuses on the behavior of the internal parsing logic
// that extracts the command line and program name, which is indirectly
// exercised when CLI11 processes command line arguments.
// The focal function's logic is about stripping the program name from
// the command line string. We'll test this by providing various
// command line strings to CLI::App::parse and observing the effect
// on argument parsing.

TEST(CLIValidatorsTest, StripProgramNameNormalCase) {
    CLI::App app{"Test App"};
    std::string arg1_val;
    app.add_option("--arg1", arg1_val, "Argument 1");

    // Simulate a command line where the program name is present
    std::vector<std::string> args = {"./my_program", "--arg1", "value1"};
    app.parse(args);

    // The program name should be stripped, and --arg1 should be parsed correctly
    ASSERT_EQ(arg1_val, "value1");
}

TEST(CLIValidatorsTest, StripProgramNameNoProgramName) {
    CLI::App app{"Test App"};
    std::string arg1_val;
    app.add_option("--arg1", arg1_val, "Argument 1");

    // Simulate a command line where the program name is NOT explicitly present
    // (e.g., when called from a shell script or directly with arguments)
    std::vector<std::string> args = {"--arg1", "value1"};
    app.parse(args);

    // The parsing should still work correctly, as if the first element was the program name
    ASSERT_EQ(arg1_val, "value1");
}

TEST(CLIValidatorsTest, StripProgramNameEmptyCommandLine) {
    CLI::App app{"Test App"};
    // No options added, just parsing an empty command line

    std::vector<std::string> args = {}; // Empty command line
    // Parsing an empty command line should not throw an error and should result in no options being set.
    ASSERT_NO_THROW(app.parse(args));
}

TEST(CLIValidatorsTest, StripProgramNameOnlyProgramName) {
    CLI::App app{"Test App"};
    // No options added

    std::vector<std::string> args = {"./my_program"}; // Only program name
    // Parsing only the program name should not throw an error.
    ASSERT_NO_THROW(app.parse(args));
}

TEST(CLIValidatorsTest, StripProgramNameProgramNameWithSpaces) {
    CLI::App app{"Test App"};
    std::string arg1_val;
    app.add_option("--arg1", arg1_val, "Argument 1");

    // Simulate a command line where the program name has spaces (unlikely for argv[0],
    // but possible if the commandline string is constructed differently internally)
    // The internal logic should still correctly identify the first token as the program name.
    std::vector<std::string> args = {"/path/to/my program", "--arg1", "value1"};
    app.parse(args);

    ASSERT_EQ(arg1_val, "value1");
}

TEST(CLIValidatorsTest, StripProgramNameMultipleArguments) {
    CLI::App app{"Test App"};
    std::string arg1_val;
    int arg2_val = 0;
    app.add_option("--arg1", arg1_val, "Argument 1");
    app.add_option("--arg2", arg2_val, "Argument 2");

    std::vector<std::string> args = {"./app", "--arg1", "val1", "--arg2", "123"};
    app.parse(args);

    ASSERT_EQ(arg1_val, "val1");
    ASSERT_EQ(arg2_val, 123);
}

TEST(CLIValidatorsTest, StripProgramNameSubcommand) {
    CLI::App app{"Test App"};
    auto sub = app.add_subcommand("sub", "A subcommand");
    std::string sub_arg_val;
    sub->add_option("--sub_arg", sub_arg_val, "Subcommand argument");

    std::vector<std::string> args = {"./main_app", "sub", "--sub_arg", "sub_value"};
    app.parse(args);

    ASSERT_TRUE(sub->parsed());
    ASSERT_EQ(sub_arg_val, "sub_value");
}

TEST(CLIValidatorsTest, StripProgramNameNoArgumentsAfterProgramName) {
    CLI::App app{"Test App"};
    // No options added

    std::vector<std::string> args = {"./my_program"};
    ASSERT_NO_THROW(app.parse(args));
    // No arguments should be parsed, and no errors should occur.
}

TEST(CLIValidatorsTest, StripProgramNameWithPositionalArguments) {
    CLI::App app{"Test App"};
    std::string pos_arg_val;
    app.add_option("pos_arg", pos_arg_val, "Positional argument");

    std::vector<std::string> args = {"./my_program", "positional_value"};
    app.parse(args);

    ASSERT_EQ(pos_arg_val, "positional_value");
}

TEST(CLIValidatorsTest, StripProgramNameWithMixedArguments) {
    CLI::App app{"Test App"};
    std::string opt_arg_val;
    std::string pos_arg_val;
    app.add_option("--opt_arg", opt_arg_val, "Optional argument");
    app.add_option("pos_arg", pos_arg_val, "Positional argument");

    std::vector<std::string> args = {"./my_program", "--opt_arg", "opt_value", "pos_value"};
    app.parse(args);

    ASSERT_EQ(opt_arg_val, "opt_value");
    ASSERT_EQ(pos_arg_val, "pos_value");
}