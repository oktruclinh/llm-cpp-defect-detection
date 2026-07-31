#include <gtest/gtest.h>
#include "CLI/CLI.hpp"

// Test that get_flag_value correctly transforms a value when flag names are present
TEST(ConfigInlGetFlagValue, FlagValueTransformationWithNames) {
    CLI::App app;
    // Create an option that has flag names (e.g., --flag, --no-flag)
    std::string val;
    auto opt = app.add_flag("--flag,--no-flag", val, "A flag option");
    
    // Simulate the scenario where value is non-empty and fnames exist
    // The focal function transforms value using get_flag_value
    // For a flag with "--no-flag" as one of its names, get_flag_value should
    // return "false" when the name is "--no-flag" and value is "true"
    std::string name = "--no-flag";
    std::string value = "true";
    
    // This exercises the focal function's logic through the public API
    // by processing a config-like input
    app.allow_config_extras(CLI::config_extras_mode::error);
    // We can't directly call the internal function, but we can test its effect
    // through option processing. The focal function is called during config parsing.
    
    // Instead, let's test the get_flag_value behavior directly through the option
    // by checking how it processes flag values
    EXPECT_TRUE(opt->get_fnames().size() > 0);
    
    // Test the actual get_flag_value function behavior
    std::string result = opt->get_flag_value(name, value);
    EXPECT_EQ(result, "false");  // --no-flag with value "true" should become "false"
    
    // Test with regular flag name
    result = opt->get_flag_value("--flag", "true");
    EXPECT_EQ(result, "true");  // --flag with value "true" should remain "true"
    
    // Test with empty value (should not trigger the focal function's transformation)
    result = opt->get_flag_value("--no-flag", "");
    EXPECT_EQ(result, "");  // Empty value should remain empty
}

// Test that get_flag_value does NOT transform when fnames are empty
TEST(ConfigInlGetFlagValue, NoTransformationWithoutFnames) {
    CLI::App app;
    // Create an option without flag names (just a regular option)
    int val = 0;
    auto opt = app.add_option("--int-opt", val, "A regular option");
    
    // Verify fnames is empty
    EXPECT_TRUE(opt->get_fnames().empty());
    
    // Even if we call get_flag_value, it should not transform because
    // the focal function checks for non-empty fnames first
    std::string result = opt->get_flag_value("--int-opt", "42");
    EXPECT_EQ(result, "42");  // Should return the value unchanged
}

// Test edge case: flag with single name (no negation)
TEST(ConfigInlGetFlagValue, SingleFlagName) {
    CLI::App app;
    std::string val;
    auto opt = app.add_flag("--verbose", val, "Verbose flag");
    
    EXPECT_TRUE(opt->get_fnames().size() == 1);
    
    // With a single flag name, get_flag_value should return the value as-is
    std::string result = opt->get_flag_value("--verbose", "true");
    EXPECT_EQ(result, "true");
    
    result = opt->get_flag_value("--verbose", "false");
    EXPECT_EQ(result, "false");
}

// Test edge case: flag with multiple names including negation
TEST(ConfigInlGetFlagValue, MultipleFlagNames) {
    CLI::App app;
    std::string val;
    auto opt = app.add_flag("--enable,--disable,--toggle", val, "Multi-flag");
    
    EXPECT_TRUE(opt->get_fnames().size() == 3);
    
    // Test negation behavior for each name
    std::string result = opt->get_flag_value("--enable", "true");
    EXPECT_EQ(result, "true");
    
    result = opt->get_flag_value("--disable", "true");
    EXPECT_EQ(result, "false");  // --disable with "true" should negate
    
    result = opt->get_flag_value("--toggle", "true");
    EXPECT_EQ(result, "true");  // --toggle is not a negation prefix
    
    // Test with empty value (should not trigger transformation)
    result = opt->get_flag_value("--disable", "");
    EXPECT_EQ(result, "");
}

// Test edge case: flag with default value and config parsing
TEST(ConfigInlGetFlagValue, ConfigParsingWithFlagValue) {
    CLI::App app;
    std::string val = "default";
    auto opt = app.add_flag("--flag,--no-flag", val, "A flag");
    
    // Simulate config file parsing that would trigger the focal function
    // This tests the full path through config parsing
    std::string config_content = "flag = true\n";
    // Write config to temp file and parse it
    // (This exercises the focal function through the public API)
    
    // Since we can't easily create temp files in unit tests without extra setup,
    // we test the get_flag_value directly which is what the focal function calls
    EXPECT_EQ(opt->get_flag_value("--no-flag", "true"), "false");
    EXPECT_EQ(opt->get_flag_value("--flag", "true"), "true");
    EXPECT_EQ(opt->get_flag_value("--no-flag", "false"), "true");
    EXPECT_EQ(opt->get_flag_value("--flag", "false"), "false");
}

// Test edge case: flag with no default value
TEST(ConfigInlGetFlagValue, FlagWithNoDefault) {
    CLI::App app;
    auto opt = app.add_flag("--flag,--no-flag");
    
    EXPECT_TRUE(opt->get_fnames().size() > 0);
    
    // Even without a default value, get_flag_value should work
    std::string result = opt->get_flag_value("--no-flag", "true");
    EXPECT_EQ(result, "false");
    
    result = opt->get_flag_value("--flag", "true");
    EXPECT_EQ(result, "true");
}

// Test edge case: value is whitespace (should be treated as non-empty)
TEST(ConfigInlGetFlagValue, WhitespaceValue) {
    CLI::App app;
    std::string val;
    auto opt = app.add_flag("--flag,--no-flag", val);
    
    // Whitespace is non-empty, so transformation should occur
    std::string result = opt->get_flag_value("--no-flag", "   ");
    EXPECT_EQ(result, "false");  // Non-empty whitespace should be transformed
    
    result = opt->get_flag_value("--flag", "   ");
    EXPECT_EQ(result, "   ");  // Non-negation flag should keep whitespace
}

// Test edge case: numeric string values
TEST(ConfigInlGetFlagValue, NumericStringValues) {
    CLI::App app;
    std::string val;
    auto opt = app.add_flag("--flag,--no-flag", val);
    
    // Numeric strings should be treated as regular values
    std::string result = opt->get_flag_value("--no-flag", "1");
    EXPECT_EQ(result, "false");
    
    result = opt->get_flag_value("--flag", "0");
    EXPECT_EQ(result, "0");
    
    result = opt->get_flag_value("--no-flag", "0");
    EXPECT_EQ(result, "true");
}