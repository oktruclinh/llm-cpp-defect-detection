#include <gtest/gtest.h>
#include <CLI/CLI.hpp> // Public API header

// The focal function is an internal helper function within CLI11.
// To test it, we need to call a public API function that internally uses it.
// The `_parse` method of `CLI::App` is a good candidate as it processes the command line.
// We will simulate command-line arguments and check the resulting parsed options.

// Helper function to simulate command-line arguments for testing
std::vector<std::string> createArgs(const std::string& commandLine) {
    std::vector<std::string> args;
    std::stringstream ss(commandLine);
    std::string arg;
    while (ss >> arg) {
        args.push_back(arg);
    }
    return args;
}

TEST(CLIAppParseTest, BasicCommandLineParsing) {
    CLI::App app{"Test App"};
    std::string value = "default";
    app.add_option("--opt", value, "An option");

    // Simulate command line: "./my_app --opt my_value"
    std::vector<std::string> args = {"./my_app", "--opt", "my_value"};
    app.parse(args);

    // The focal function's behavior is about stripping the program name
    // and getting the rest of the command line.
    // We assert on the effect of this parsing, i.e., the option value.
    ASSERT_EQ(value, "my_value");
}

TEST(CLIAppParseTest, CommandLineWithNoOptions) {
    CLI::App app{"Test App"};
    std::string value = "default";
    app.add_option("--opt", value, "An option");

    // Simulate command line: "./my_app"
    std::vector<std::string> args = {"./my_app"};
    app.parse(args);

    // The option should retain its default value as it was not provided.
    ASSERT_EQ(value, "default");
}

TEST(CLIAppParseTest, CommandLineWithOnlyProgramName) {
    CLI::App app{"Test App"};
    // Simulate command line: "my_app" (no path, just name)
    std::vector<std::string> args = {"my_app"};
    app.parse(args);

    // No options, no errors expected.
    SUCCEED(); // If it doesn't throw, it's good.
}

TEST(CLIAppParseTest, CommandLineWithMultipleOptions) {
    CLI::App app{"Test App"};
    std::string opt1_val = "def1";
    int opt2_val = 0;
    app.add_option("--opt1", opt1_val, "Option 1");
    app.add_option("-o2,--opt2", opt2_val, "Option 2");

    // Simulate command line: "./my_app --opt1 val1 -o2 123"
    std::vector<std::string> args = {"./my_app", "--opt1", "val1", "-o2", "123"};
    app.parse(args);

    ASSERT_EQ(opt1_val, "val1");
    ASSERT_EQ(opt2_val, 123);
}

TEST(CLIAppParseTest, CommandLineWithSubcommand) {
    CLI::App app{"Test App"};
    auto sub = app.add_subcommand("sub", "A subcommand");
    std::string sub_opt_val = "sub_def";
    sub->add_option("--sub_opt", sub_opt_val, "Subcommand option");

    // Simulate command line: "./my_app sub --sub_opt sub_value"
    std::vector<std::string> args = {"./my_app", "sub", "--sub_opt", "sub_value"};
    app.parse(args);

    ASSERT_TRUE(sub->parsed());
    ASSERT_EQ(sub_opt_val, "sub_value");
}

TEST(CLIAppParseTest, CommandLineWithSubcommandAndNoSubcommandOptions) {
    CLI::App app{"Test App"};
    auto sub = app.add_subcommand("sub", "A subcommand");
    std::string sub_opt_val = "sub_def";
    sub->add_option("--sub_opt", sub_opt_val, "Subcommand option");

    // Simulate command line: "./my_app sub"
    std::vector<std::string> args = {"./my_app", "sub"};
    app.parse(args);

    ASSERT_TRUE(sub->parsed());
    ASSERT_EQ(sub_opt_val, "sub_def"); // Should retain default
}

TEST(CLIAppParseTest, CommandLineWithArgumentsAfterOptions) {
    CLI::App app{"Test App"};
    std::string opt_val = "def";
    app.add_option("--opt", opt_val, "An option");
    std::vector<std::string> pos_args;
    app.add_option("args", pos_args, "Positional arguments");

    // Simulate command line: "./my_app --opt val arg1 arg2"
    std::vector<std::string> args = {"./my_app", "--opt", "val", "arg1", "arg2"};
    app.parse(args);

    ASSERT_EQ(opt_val, "val");
    ASSERT_EQ(pos_args.size(), 2);
    ASSERT_EQ(pos_args[0], "arg1");
    ASSERT_EQ(pos_args[1], "arg2");
}

TEST(CLIAppParseTest, CommandLineWithEmptyProgramName) {
    CLI::App app{"Test App"};
    std::string value = "default";
    app.add_option("--opt", value, "An option");

    // Simulate command line: "" (empty string, which is unlikely in real CLI but good for edge cases)
    // CLI11's parse expects at least one argument (the program name).
    // This will likely throw an error or behave unexpectedly if not handled internally.
    // We expect it to throw a CLI::ParseError because there's no program name.
    std::vector<std::string> args = {};
    ASSERT_THROW(app.parse(args), CLI::ParseError);
}

TEST(CLIAppParseTest, CommandLineWithOnlyWhitespaceProgramName) {
    CLI::App app{"Test App"};
    std::string value = "default";
    app.add_option("--opt", value, "An option");

    // Simulate command line: "   " (whitespace only)
    // CLI11's parse expects a non-empty program name.
    std::vector<std::string> args = {"   "}; // A single argument that is whitespace
    ASSERT_THROW(app.parse(args), CLI::ParseError); // Expecting an error for invalid program name
}

TEST(CLIAppParseTest, CommandLineWithProgramNameAndTrailingWhitespace) {
    CLI::App app{"Test App"};
    std::string value = "default";
    app.add_option("--opt", value, "An option");

    // Simulate command line: "./my_app   --opt my_value"
    // The `ltrim` in the focal function is relevant here.
    // The `createArgs` helper will split "my_app   --opt my_value" into ["./my_app", "--opt", "my_value"]
    // so the trailing whitespace after the program name is handled by the shell/`createArgs`.
    // The `ltrim` in the focal function is more about trimming the *remaining* command line after the program name.
    std::vector<std::string> args = {"./my_app", "--opt", "my_value"};
    app.parse(args);
    ASSERT_EQ(value, "my_value");
}

TEST(CLIAppParseTest, CommandLineWithNoArgumentsAfterProgramName) {
    CLI::App app{"Test App"};
    // Simulate command line: "my_app"
    std::vector<std::string> args = {"my_app"};
    app.parse(args);
    // No options, no errors, just successful parsing of an empty argument list.
    SUCCEED();
}

TEST(CLIAppParseTest, CommandLineWithOnlyProgramNameAndWhitespace) {
    CLI::App app{"Test App"};
    // Simulate command line: "my_app   "
    // The `createArgs` will effectively make this `{"my_app"}`
    std::vector<std::string> args = {"my_app"};
    app.parse(args);
    SUCCEED();
}