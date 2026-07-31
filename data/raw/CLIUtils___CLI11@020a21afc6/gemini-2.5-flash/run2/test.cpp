#include <gtest/gtest.h>
#include <CLI/CLI.hpp> // Public API header

// The focal function is an internal helper function within CLI11.
// To test it, we need to call a public API function that internally
// uses this helper. The `_parse` method of `CLI::App` is a good candidate
// as it processes the command line and would involve stripping the program name.

// We'll create a fixture to set up a basic CLI::App for testing.
class CLIAppParseTest : public ::testing::Test {
protected:
    CLI::App app;

    // Helper to simulate command line arguments
    std::vector<std::string> createArgs(const std::string& arg_str) {
        std::istringstream iss(arg_str);
        std::vector<std::string> args;
        std::string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }
        return args;
    }
};

// Test case 1: Normal command line with program name and arguments
TEST_F(CLIAppParseTest, NormalCommandLine) {
    auto file_option = app.add_option("-f,--file", "A file name.");
    auto verbose_flag = app.add_flag("-v,--verbose", "Enable verbose output.");

    std::vector<std::string> args = createArgs("./my_program -f input.txt --verbose");
    app.parse(args);

    // After parsing, the program name should be stripped, and arguments processed.
    // We can check if the options were correctly set.
    EXPECT_EQ(file_option->results().at(0), "input.txt");
    EXPECT_TRUE(*verbose_flag);
}

// Test case 2: Command line with only program name
TEST_F(CLIAppParseTest, OnlyProgramName) {
    auto help_flag = app.add_flag("-h,--help", "Show help message.");

    std::vector<std::string> args = createArgs("./my_program");
    app.parse(args);

    // No arguments should be processed, and no options should be set.
    EXPECT_FALSE(*help_flag);
}

// Test case 3: Command line with program name and leading/trailing spaces
TEST_F(CLIAppParseTest, ProgramNameWithSpaces) {
    auto output_option = app.add_option("-o,--output", "Output file.");

    std::vector<std::string> args = createArgs("  ./my_program   -o result.log  ");
    app.parse(args);

    EXPECT_EQ(output_option->results().at(0), "result.log");
}

// Test case 4: Program name with spaces in path (not directly handled by the focal function,
// but CLI11 should handle it if the shell passes it as a single argument)
TEST_F(CLIAppParseTest, ProgramNameWithPathSpaces) {
    auto input_option = app.add_option("-i,--input", "Input file.");

    // Simulate a program name with spaces, which would typically be quoted by the shell
    std::vector<std::string> args = {"./my program with spaces", "-i", "data.csv"};
    app.parse(args);

    EXPECT_EQ(input_option->results().at(0), "data.csv");
}


// Test case 5: Empty command line (should not happen in practice, but good for robustness)
TEST_F(CLIAppParseTest, EmptyCommandLine) {
    auto version_flag = app.add_flag("--version", "Show version.");

    std::vector<std::string> args = {}; // Empty vector
    // Parsing an empty vector should not throw and result in no options set.
    ASSERT_NO_THROW(app.parse(args));
    EXPECT_FALSE(*version_flag);
}

// Test case 6: Command line with only spaces (should behave like empty)
TEST_F(CLIAppParseTest, CommandLineWithOnlySpaces) {
    auto debug_flag = app.add_flag("--debug", "Enable debug mode.");

    // This scenario is tricky as `createArgs` would produce an empty vector.
    // We'll manually create a vector that might represent this.
    std::vector<std::string> args = {"   "}; // A single argument that is just spaces
    ASSERT_NO_THROW(app.parse(args));
    EXPECT_FALSE(*debug_flag);

    // If the shell passes an empty string as the first argument
    std::vector<std::string> args_empty_string = {""};
    ASSERT_NO_THROW(app.parse(args_empty_string));
    EXPECT_FALSE(*debug_flag);
}


// Test case 7: Program name is a single character
TEST_F(CLIAppParseTest, SingleCharProgramName) {
    auto name_option = app.add_option("-n,--name", "Your name.");

    std::vector<std::string> args = createArgs("a -n John");
    app.parse(args);

    EXPECT_EQ(name_option->results().at(0), "John");
}

// Test case 8: Program name is a long string
TEST_F(CLIAppParseTest, LongProgramName) {
    auto help_flag = app.add_flag("--help");

    std::string long_name = "this_is_a_very_long_program_name_that_should_still_work_correctly_when_stripped";
    std::vector<std::string> args = createArgs(long_name + " --help");
    app.parse(args);

    EXPECT_TRUE(*help_flag);
}

// Test case 9: No arguments after program name, but with trailing spaces
TEST_F(CLIAppParseTest, NoArgsAfterProgramNameWithTrailingSpaces) {
    auto test_flag = app.add_flag("--test");
    std::vector<std::string> args = createArgs("./my_app   ");
    app.parse(args);
    EXPECT_FALSE(*test_flag);
}

// Test case 10: Program name contains special characters (if passed as a single argument)
TEST_F(CLIAppParseTest, ProgramNameWithSpecialChars) {
    auto version_flag = app.add_flag("--version");
    std::vector<std::string> args = {"./my_app!@#$", "--version"};
    app.parse(args);
    EXPECT_TRUE(*version_flag);
}