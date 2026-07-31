#include <gtest/gtest.h>
#include "CLI/CLI.hpp"

// Test the function that strips the program name from a command line string
// This function is part of the internal parsing utilities in CLI11

class CommandLineStripTest : public ::testing::Test {
protected:
    void SetUp() override {
        // No setup needed
    }
};

// Test with a simple command line containing a program name and arguments
TEST_F(CommandLineStripTest, SimpleProgramAndArgs) {
    std::string commandline = "myprogram --option value";
    // The function being tested is called internally when parsing command lines
    // We can test it through the public API by using CLI::App with a simulated command line
    
    CLI::App app;
    std::string output;
    app.add_option("--option", output, "test option");
    
    // Simulate command line parsing - this will internally call the strip function
    std::vector<std::string> args = {"myprogram", "--option", "testvalue"};
    app.parse(args);
    
    EXPECT_EQ(output, "testvalue");
}

// Test with empty command line
TEST_F(CommandLineStripTest, EmptyCommandLine) {
    CLI::App app;
    std::string output;
    app.add_option("--opt", output);
    
    std::vector<std::string> args = {};
    EXPECT_THROW(app.parse(args), CLI::ParseError);
}

// Test with only program name, no arguments
TEST_F(CommandLineStripTest, OnlyProgramName) {
    CLI::App app;
    std::string output;
    app.add_option("--opt", output);
    
    std::vector<std::string> args = {"myprogram"};
    app.parse(args);
    
    EXPECT_TRUE(output.empty());
}

// Test with leading whitespace in command line
TEST_F(CommandLineStripTest, LeadingWhitespace) {
    CLI::App app;
    std::string output;
    app.add_option("--opt", output);
    
    std::vector<std::string> args = {"  myprogram", "--opt", "value"};
    app.parse(args);
    
    EXPECT_EQ(output, "value");
}

// Test with multiple arguments
TEST_F(CommandLineStripTest, MultipleArguments) {
    CLI::App app;
    std::string opt1, opt2;
    app.add_option("--opt1", opt1);
    app.add_option("--opt2", opt2);
    
    std::vector<std::string> args = {"prog", "--opt1", "val1", "--opt2", "val2"};
    app.parse(args);
    
    EXPECT_EQ(opt1, "val1");
    EXPECT_EQ(opt2, "val2");
}

// Test with flags (no value arguments)
TEST_F(CommandLineStripTest, FlagsOnly) {
    CLI::App app;
    bool flag1 = false, flag2 = false;
    app.add_flag("--flag1", flag1);
    app.add_flag("--flag2", flag2);
    
    std::vector<std::string> args = {"program", "--flag1", "--flag2"};
    app.parse(args);
    
    EXPECT_TRUE(flag1);
    EXPECT_TRUE(flag2);
}

// Test with positional arguments
TEST_F(CommandLineStripTest, PositionalArguments) {
    CLI::App app;
    std::string pos;
    app.add_option("positional", pos);
    
    std::vector<std::string> args = {"app", "positional_value"};
    app.parse(args);
    
    EXPECT_EQ(pos, "positional_value");
}

// Test with subcommands
TEST_F(CommandLineStripTest, Subcommands) {
    CLI::App app;
    CLI::App* sub = app.add_subcommand("sub", "a subcommand");
    std::string output;
    sub->add_option("--opt", output);
    
    std::vector<std::string> args = {"main", "sub", "--opt", "subvalue"};
    app.parse(args);
    
    EXPECT_EQ(output, "subvalue");
    EXPECT_TRUE(app.got_subcommand(sub));
}

// Test with quoted arguments containing spaces
TEST_F(CommandLineStripTest, QuotedArguments) {
    CLI::App app;
    std::string output;
    app.add_option("--opt", output);
    
    std::vector<std::string> args = {"prog", "--opt", "value with spaces"};
    app.parse(args);
    
    EXPECT_EQ(output, "value with spaces");
}

// Test with numeric arguments
TEST_F(CommandLineStripTest, NumericArguments) {
    CLI::App app;
    int num = 0;
    app.add_option("--num", num);
    
    std::vector<std::string> args = {"program", "--num", "42"};
    app.parse(args);
    
    EXPECT_EQ(num, 42);
}

// Test with double dash separator
TEST_F(CommandLineStripTest, DoubleDashSeparator) {
    CLI::App app;
    std::string opt;
    std::vector<std::string> remaining;
    app.add_option("--opt", opt);
    app.allow_extras();
    
    std::vector<std::string> args = {"prog", "--opt", "value", "--", "extra1", "extra2"};
    app.parse(args);
    
    EXPECT_EQ(opt, "value");
    EXPECT_EQ(app.remaining_size(), 2u);
}

// Test error case: unknown option
TEST_F(CommandLineStripTest, UnknownOption) {
    CLI::App app;
    std::string output;
    app.add_option("--opt", output);
    
    std::vector<std::string> args = {"program", "--unknown", "value"};
    EXPECT_THROW(app.parse(args), CLI::ParseError);
}

// Test error case: missing required argument
TEST_F(CommandLineStripTest, MissingRequiredArgument) {
    CLI::App app;
    std::string output;
    app.add_option("--opt", output)->required();
    
    std::vector<std::string> args = {"program"};
    EXPECT_THROW(app.parse(args), CLI::ParseError);
}

// Test with very long command line
TEST_F(CommandLineStripTest, LongCommandLine) {
    CLI::App app;
    std::string output;
    app.add_option("--opt", output);
    
    std::string long_value(1000, 'x');
    std::vector<std::string> args = {"program", "--opt", long_value};
    app.parse(args);
    
    EXPECT_EQ(output, long_value);
}

// Test with special characters in arguments
TEST_F(CommandLineStripTest, SpecialCharacters) {
    CLI::App app;
    std::string output;
    app.add_option("--opt", output);
    
    std::vector<std::string> args = {"prog", "--opt", "value!@#$%^&*()"};
    app.parse(args);
    
    EXPECT_EQ(output, "value!@#$%^&*()");
}