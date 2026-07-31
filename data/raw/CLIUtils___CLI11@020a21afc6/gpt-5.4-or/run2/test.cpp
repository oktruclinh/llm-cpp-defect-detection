#include <gtest/gtest.h>

#include <string>
#include <utility>

#include <CLI/Error.hpp>
#include <CLI/Validators.hpp>

namespace {

using CLI::detail::split_program_name;

// Normal quoted executable path with arguments.
TEST(SplitProgramNameTest, SplitsQuotedProgramAndArguments) {
    const std::string command = "\"/usr/local/bin/my app\" --flag value";
    auto result = split_program_name(command);

    EXPECT_EQ(result.first, "/usr/local/bin/my app");
    EXPECT_EQ(result.second, "--flag value");
}

// Quoted executable path with no arguments.
TEST(SplitProgramNameTest, QuotedProgramOnlyReturnsEmptyRemainder) {
    const std::string command = "\"/usr/local/bin/my app\"";
    auto result = split_program_name(command);

    EXPECT_EQ(result.first, "/usr/local/bin/my app");
    EXPECT_TRUE(result.second.empty());
}

// Unquoted executable without spaces and with arguments.
TEST(SplitProgramNameTest, SplitsSimpleProgramAndArguments) {
    const std::string command = "myprog --count 3";
    auto result = split_program_name(command);

    EXPECT_EQ(result.first, "myprog");
    EXPECT_EQ(result.second, "--count 3");
}

// Unquoted executable only.
TEST(SplitProgramNameTest, ProgramOnlyReturnsEmptyRemainder) {
    const std::string command = "myprog";
    auto result = split_program_name(command);

    EXPECT_EQ(result.first, "myprog");
    EXPECT_TRUE(result.second.empty());
}

// Multiple spaces after program name should be stripped from the remainder.
TEST(SplitProgramNameTest, LeadingSpacesInRemainderAreTrimmed) {
    const std::string command = "myprog     --flag   value";
    auto result = split_program_name(command);

    EXPECT_EQ(result.first, "myprog");
    EXPECT_EQ(result.second, "--flag   value");
    ASSERT_FALSE(result.second.empty());
    EXPECT_NE(result.second.front(), ' ');
}

// Tabs/newlines after program name are also leading whitespace and should be trimmed.
TEST(SplitProgramNameTest, LeadingWhitespaceInRemainderIsTrimmed) {
    const std::string command = "myprog \t\n  --flag";
    auto result = split_program_name(command);

    EXPECT_EQ(result.first, "myprog");
    EXPECT_EQ(result.second, "--flag");
}

// If command line ends right after spaces following the program name, remainder is empty.
TEST(SplitProgramNameTest, TrailingSpacesAfterProgramYieldEmptyRemainder) {
    const std::string command = "myprog    ";
    auto result = split_program_name(command);

    EXPECT_EQ(result.first, "myprog");
    EXPECT_TRUE(result.second.empty());
}

// Empty input should produce empty program name and empty remainder.
TEST(SplitProgramNameTest, EmptyInputReturnsEmptyParts) {
    const std::string command;
    auto result = split_program_name(command);

    EXPECT_TRUE(result.first.empty());
    EXPECT_TRUE(result.second.empty());
}

// Input with only whitespace should not produce spurious arguments.
TEST(SplitProgramNameTest, WhitespaceOnlyInputReturnsEmptyRemainder) {
    const std::string command = "   \t  ";
    auto result = split_program_name(command);

    EXPECT_TRUE(result.second.empty());
}

// Quoted program followed by many spaces before args should trim those spaces.
TEST(SplitProgramNameTest, QuotedProgramTrimsLeadingSpacesBeforeArguments) {
    const std::string command = "\"C:\\Program Files\\tool.exe\"      /a /b";
    auto result = split_program_name(command);

    EXPECT_EQ(result.first, "C:\\Program Files\\tool.exe");
    EXPECT_EQ(result.second, "/a /b");
}

// Quoted empty program name is handled and remainder still trimmed correctly.
TEST(SplitProgramNameTest, EmptyQuotedProgramNameHandled) {
    const std::string command = "\"\"   --arg";
    auto result = split_program_name(command);

    EXPECT_EQ(result.first, "");
    EXPECT_EQ(result.second, "--arg");
}

// Unterminated quote should still return the content after the opening quote as the program name.
TEST(SplitProgramNameTest, UnterminatedQuotedProgramConsumesRestAsProgramName) {
    const std::string command = "\"unterminated program";
    auto result = split_program_name(command);

    EXPECT_EQ(result.first, "unterminated program");
    EXPECT_TRUE(result.second.empty());
}

}  // namespace