#include <gtest/gtest.h>
#include "CLI/CLI.hpp"

// Test that get_flag_value is called when option has flag names and value is non-empty
TEST(ConfigInlTest, GetFlagValueCalledWithFlagNames) {
    CLI::App app;
    auto opt = app.add_flag("--flag,-f");
    std::string name = "--flag";
    std::string value = "true";
    
    // Simulate the config processing logic that calls the focal function
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value(name, value);
        }
    }
    
    EXPECT_EQ(value, "1");  // get_flag_value("--flag", "true") should return "1"
}

// Test that get_flag_value is called with short flag name
TEST(ConfigInlTest, GetFlagValueCalledWithShortName) {
    CLI::App app;
    auto opt = app.add_flag("--verbose,-v");
    std::string name = "-v";
    std::string value = "yes";
    
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value(name, value);
        }
    }
    
    EXPECT_EQ(value, "1");  // get_flag_value("-v", "yes") should return "1"
}

// Test that value is NOT modified when option has no flag names (get_fnames empty)
TEST(ConfigInlTest, NoModificationWhenNoFlagNames) {
    CLI::App app;
    int var = 0;
    auto opt = app.add_option("--option", var, "description");
    std::string name = "--option";
    std::string value = "some_value";
    
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value(name, value);
        }
    }
    
    EXPECT_EQ(value, "some_value");  // Value should remain unchanged
}

// Test that value is NOT modified when value is empty
TEST(ConfigInlTest, NoModificationWhenEmptyValue) {
    CLI::App app;
    auto opt = app.add_flag("--flag,-f");
    std::string name = "--flag";
    std::string value = "";
    
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value(name, value);
        }
    }
    
    EXPECT_TRUE(value.empty());  // Empty value should remain empty
}

// Test with boolean false value
TEST(ConfigInlTest, GetFlagValueWithFalseValue) {
    CLI::App app;
    auto opt = app.add_flag("--enable,-e");
    std::string name = "--enable";
    std::string value = "false";
    
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value(name, value);
        }
    }
    
    EXPECT_EQ(value, "0");  // get_flag_value("--enable", "false") should return "0"
}

// Test with numeric value
TEST(ConfigInlTest, GetFlagValueWithNumericValue) {
    CLI::App app;
    auto opt = app.add_flag("--debug,-d");
    std::string name = "--debug";
    std::string value = "1";
    
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value(name, value);
        }
    }
    
    EXPECT_EQ(value, "1");  // get_flag_value("--debug", "1") should return "1"
}

// Test with multiple flag names (long and short)
TEST(ConfigInlTest, MultipleFlagNames) {
    CLI::App app;
    auto opt = app.add_flag("--quiet,-q,--silent");
    std::string name = "--silent";
    std::string value = "on";
    
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value(name, value);
        }
    }
    
    EXPECT_EQ(value, "1");  // get_flag_value("--silent", "on") should return "1"
}

// Test that get_fnames returns expected names
TEST(ConfigInlTest, GetFnamesReturnsCorrectNames) {
    CLI::App app;
    auto opt = app.add_flag("--flag,-f");
    
    auto fnames = opt->get_fnames();
    EXPECT_FALSE(fnames.empty());
    EXPECT_EQ(fnames.size(), 2u);
    EXPECT_EQ(fnames[0], "--flag");
    EXPECT_EQ(fnames[1], "-f");
}

// Test edge case: flag with no short name
TEST(ConfigInlTest, FlagWithOnlyLongName) {
    CLI::App app;
    auto opt = app.add_flag("--long-only");
    std::string name = "--long-only";
    std::string value = "true";
    
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value(name, value);
        }
    }
    
    EXPECT_EQ(value, "1");
    EXPECT_EQ(opt->get_fnames().size(), 1u);
}

// Test edge case: flag with only short name
TEST(ConfigInlTest, FlagWithOnlyShortName) {
    CLI::App app;
    auto opt = app.add_flag("-x");
    std::string name = "-x";
    std::string value = "yes";
    
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value(name, value);
        }
    }
    
    EXPECT_EQ(value, "1");
    EXPECT_EQ(opt->get_fnames().size(), 1u);
}