#include <gtest/gtest.h>

#include <string>
#include <utility>

#include "CLI/Error.hpp"
#include "CLI/Validators.hpp"

namespace {

using CLI::detail::split_program_name;

TEST(SplitProgramNameTest, EmptyCommandLineReturnsEmptyParts) {
    auto vals = split_program_name("");
    EXPECT_TRUE(vals.first.empty());
    EXPECT_TRUE(vals.second.empty());
}

TEST(SplitProgramNameTest, OnlyProgramNameNoArguments) {
    auto vals = split_program_name("myprog");
    EXPECT_EQ(vals.first, "myprog");
    EXPECT_TRUE(vals.second.empty());
}

TEST(SplitProgramNameTest, ProgramNameAndSingleArgument) {
    auto vals = split_program_name("myprog --help");
    EXPECT_EQ(vals.first, "myprog");
    EXPECT_EQ(vals.second, "--help");
}

TEST(SplitProgramNameTest, ProgramNameAndMultipleArguments) {
    auto vals = split_program_name("myprog arg1 arg2 --flag=value");
    EXPECT_EQ(vals.first, "myprog");
    EXPECT_EQ(vals.second, "arg1 arg2 --flag=value");
}

TEST(SplitProgramNameTest, LeadingSpacesBeforeProgramNameAreIgnored) {
    auto vals = split_program_name("   myprog arg1 arg2");
    EXPECT_EQ(vals.first, "myprog");
    EXPECT_EQ(vals.second, "arg1 arg2");
}

TEST(SplitProgramNameTest, TrailingSpacesAfterProgramNameProduceEmptyRemainder) {
    auto vals = split_program_name("myprog   ");
    EXPECT_EQ(vals.first, "myprog");
    EXPECT_TRUE(vals.second.empty());
}

TEST(SplitProgramNameTest, MultipleSpacesBeforeArgumentsAreTrimmedFromRemainder) {
    auto vals = split_program_name("myprog      arg1   arg2");
    EXPECT_EQ(vals.first, "myprog");
    EXPECT_EQ(vals.second, "arg1   arg2");
    ASSERT_FALSE(vals.second.empty());
    EXPECT_NE(vals.second.front(), ' ');
}

TEST(SplitProgramNameTest, OnlyWhitespaceReturnsEmptyParts) {
    auto vals = split_program_name("     ");
    EXPECT_TRUE(vals.first.empty());
    EXPECT_TRUE(vals.second.empty());
}

TEST(SplitProgramNameTest, QuotedProgramNameWithArguments) {
    auto vals = split_program_name("\"/path to/my prog\" --flag value");
    EXPECT_EQ(vals.first, "\"/path to/my prog\"");
    EXPECT_EQ(vals.second, "--flag value");
}

TEST(SplitProgramNameTest, QuotedProgramNameWithoutArguments) {
    auto vals = split_program_name("\"/path to/my prog\"");
    EXPECT_EQ(vals.first, "\"/path to/my prog\"");
    EXPECT_TRUE(vals.second.empty());
}

TEST(SplitProgramNameTest, QuotedProgramNameWithExtraSpacesBeforeArgumentsTrimsRemainder) {
    auto vals = split_program_name("\"prog name\"      --opt 123");
    EXPECT_EQ(vals.first, "\"prog name\"");
    EXPECT_EQ(vals.second, "--opt 123");
    ASSERT_FALSE(vals.second.empty());
    EXPECT_NE(vals.second.front(), ' ');
}

TEST(SplitProgramNameTest, UnterminatedQuoteTreatsRestAsProgramName) {
    auto vals = split_program_name("\"prog name --flag value");
    EXPECT_EQ(vals.first, "\"prog name --flag value");
    EXPECT_TRUE(vals.second.empty());
}

TEST(SplitProgramNameTest, SingleQuoteProgramNameWithArguments) {
    auto vals = split_program_name("'prog name' arg1");
    EXPECT_EQ(vals.first, "'prog name'");
    EXPECT_EQ(vals.second, "arg1");
}

TEST(SplitProgramNameTest, TabBetweenProgramAndArgumentsDoesNotSplitOnTab) {
    auto vals = split_program_name("myprog\targ1 arg2");
    EXPECT_EQ(vals.first, "myprog\targ1");
    EXPECT_EQ(vals.second, "arg2");
}

TEST(SplitProgramNameTest, SpaceThenTabsBeforeArgumentRemainAfterLeftTrimOfSpacesOnly) {
    auto vals = split_program_name("myprog \t\targ1");
    EXPECT_EQ(vals.first, "myprog");
    EXPECT_EQ(vals.second, "\t\targ1");
    ASSERT_FALSE(vals.second.empty());
    EXPECT_EQ(vals.second.front(), '\t');
}

TEST(SplitProgramNameTest, BoundarySingleCharacterProgram) {
    auto vals = split_program_name("a b");
    EXPECT_EQ(vals.first, "a");
    EXPECT_EQ(vals.second, "b");
}

TEST(SplitProgramNameTest, BoundarySingleCharacterProgramNoArguments) {
    auto vals = split_program_name("a");
    EXPECT_EQ(vals.first, "a");
    EXPECT_TRUE(vals.second.empty());
}

}  // namespace