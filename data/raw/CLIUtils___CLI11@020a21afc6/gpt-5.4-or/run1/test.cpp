#include <gtest/gtest.h>

#include <string>
#include <utility>

#include "CLI/Error.hpp"
#include "CLI/Validators.hpp"

// The focal code strips the executable/program token from a command line string,
// trims leading whitespace from the remainder, and returns the program name
// plus the remaining command line. In CLI11 this behavior is exposed through
// the public API function split_program_name.

TEST(ValidatorsSplitProgramNameTest, EmptyStringReturnsEmptyParts) {
    auto result = CLI::detail::split_program_name("");

    EXPECT_TRUE(result.first.empty());
    EXPECT_TRUE(result.second.empty());
}

TEST(ValidatorsSplitProgramNameTest, WhitespaceOnlyStringHasNoProgramAndNoRemainder) {
    auto result = CLI::detail::split_program_name("   \t  ");

    EXPECT_TRUE(result.first.empty());
    EXPECT_TRUE(result.second.empty());
}

TEST(ValidatorsSplitProgramNameTest, SingleProgramWithoutArguments) {
    auto result = CLI::detail::split_program_name("myprog");

    EXPECT_EQ(result.first, "myprog");
    EXPECT_TRUE(result.second.empty());
}

TEST(ValidatorsSplitProgramNameTest, ProgramFollowedBySingleArgument) {
    auto result = CLI::detail::split_program_name("myprog --flag");

    EXPECT_EQ(result.first, "myprog");
    EXPECT_EQ(result.second, "--flag");
}

TEST(ValidatorsSplitProgramNameTest, ProgramFollowedByMultipleSpacesBeforeArgumentsIsTrimmed) {
    auto result = CLI::detail::split_program_name("myprog     --flag value");

    EXPECT_EQ(result.first, "myprog");
    EXPECT_EQ(result.second, "--flag value");
    ASSERT_FALSE(result.second.empty());
    EXPECT_NE(result.second.front(), ' ');
}

TEST(ValidatorsSplitProgramNameTest, LeadingWhitespaceBeforeProgramIsIgnored) {
    auto result = CLI::detail::split_program_name("   \tmyprog --flag");

    EXPECT_EQ(result.first, "myprog");
    EXPECT_EQ(result.second, "--flag");
}

TEST(ValidatorsSplitProgramNameTest, TrailingWhitespaceAfterProgramOnlyProducesEmptyRemainder) {
    auto result = CLI::detail::split_program_name("myprog   \t ");

    EXPECT_EQ(result.first, "myprog");
    EXPECT_TRUE(result.second.empty());
}

TEST(ValidatorsSplitProgramNameTest, QuotedProgramNameIsPreservedAndArgumentsReturned) {
    auto result = CLI::detail::split_program_name("\"/path to/my prog\" --opt 1");

    EXPECT_EQ(result.first, "\"/path to/my prog\"");
    EXPECT_EQ(result.second, "--opt 1");
}

TEST(ValidatorsSplitProgramNameTest, QuotedProgramNameWithoutArgumentsHasEmptyRemainder) {
    auto result = CLI::detail::split_program_name("\"/path to/my prog\"");

    EXPECT_EQ(result.first, "\"/path to/my prog\"");
    EXPECT_TRUE(result.second.empty());
}

TEST(ValidatorsSplitProgramNameTest, RemainderPreservesInternalSpacingAfterInitialTrim) {
    auto result = CLI::detail::split_program_name("prog    arg1   arg2\targ3");

    EXPECT_EQ(result.first, "prog");
    EXPECT_EQ(result.second, "arg1   arg2\targ3");
}

TEST(ValidatorsSplitProgramNameTest, ProgramAndRemainderWithTabsAreHandled) {
    auto result = CLI::detail::split_program_name("prog\t\t--flag\tvalue");

    EXPECT_EQ(result.first, "prog");
    EXPECT_EQ(result.second, "--flag\tvalue");
}

TEST(ValidatorsSplitProgramNameTest, QuotedArgumentInRemainderIsPreserved) {
    auto result = CLI::detail::split_program_name("prog   --name \"value with spaces\"  tail");

    EXPECT_EQ(result.first, "prog");
    EXPECT_EQ(result.second, "--name \"value with spaces\"  tail");
}

TEST(ValidatorsSplitProgramNameTest, ProgramWithPathAndNoArguments) {
    auto result = CLI::detail::split_program_name("/usr/bin/myprog");

    EXPECT_EQ(result.first, "/usr/bin/myprog");
    EXPECT_TRUE(result.second.empty());
}

TEST(ValidatorsSplitProgramNameTest, ProgramWithPathAndArguments) {
    auto result = CLI::detail::split_program_name("/usr/bin/myprog    subcommand --x");

    EXPECT_EQ(result.first, "/usr/bin/myprog");
    EXPECT_EQ(result.second, "subcommand --x");
}

TEST(ValidatorsSplitProgramNameTest, OnlyQuotedEmptyProgramAndNoArguments) {
    auto result = CLI::detail::split_program_name("\"\"");

    EXPECT_EQ(result.first, "\"\"");
    EXPECT_TRUE(result.second.empty());
}