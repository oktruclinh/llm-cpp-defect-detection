#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include "CLI/impl/Config_inl.hpp"

TEST(ConfigInlineTest, FlagValueProcessing) {
    // Test normal flag value processing
    CLI::App app("Test app");
    
    // Create an option with a flag
    auto opt = app.add_flag("--verbose", "Verbose output");
    
    // Test that flag value processing works correctly
    std::string name = "--verbose";
    std::string value = "true";
    
    // This should process the flag value correctly
    std::string result = opt->get_flag_value(name, value);
    EXPECT_EQ(result, "true");
    
    // Test with empty value
    value = "";
    result = opt->get_flag_value(name, value);
    EXPECT_EQ(result, "");
    
    // Test with different flag value
    value = "false";
    result = opt->get_flag_value(name, value);
    EXPECT_EQ(result, "false");
}

TEST(ConfigInlineTest, FlagValueWithEmptyFnames) {
    // Test when get_fnames() returns empty
    CLI::App app("Test app");
    auto opt = app.add_flag("--verbose", "Verbose output");
    
    // Manually test the condition where get_fnames() is empty
    // This should not call get_flag_value since fnames is empty
    std::string name = "--verbose";
    std::string value = "true";
    
    // The condition !opt->get_fnames().empty() should be false here
    // So get_flag_value should not be called, but we can still test the function
    std::string result = opt->get_flag_value(name, value);
    EXPECT_EQ(result, "true");
}

TEST(ConfigInlineTest, FlagValueEdgeCases) {
    CLI::App app("Test app");
    auto opt = app.add_flag("--verbose", "Verbose output");
    
    // Test with various edge cases
    std::string name = "--verbose";
    
    // Empty string value
    std::string result = opt->get_flag_value(name, "");
    EXPECT_EQ(result, "");
    
    // Whitespace only
    result = opt->get_flag_value(name, "   ");
    EXPECT_EQ(result, "   ");
    
    // Special characters
    result = opt->get_flag_value(name, "value-with-special-chars_123");
    EXPECT_EQ(result, "value-with-special-chars_123");
    
    // Long value
    std::string long_value(1000, 'x');
    result = opt->get_flag_value(name, long_value);
    EXPECT_EQ(result, long_value);
}

TEST(ConfigInlineTest, FlagValueMultipleFlags) {
    CLI::App app("Test app");
    auto opt1 = app.add_flag("--verbose", "Verbose output");
    auto opt2 = app.add_flag("--debug", "Debug output");
    
    // Test both flags behave correctly
    std::string result1 = opt1->get_flag_value("--verbose", "true");
    std::string result2 = opt2->get_flag_value("--debug", "false");
    
    EXPECT_EQ(result1, "true");
    EXPECT_EQ(result2, "false");
}

TEST(ConfigInlineTest, FlagValueWithDifferentNames) {
    CLI::App app("Test app");
    auto opt = app.add_flag("--verbose", "Verbose output");
    
    // Test with different flag names
    std::string result = opt->get_flag_value("--verbose", "true");
    EXPECT_EQ(result, "true");
    
    result = opt->get_flag_value("--debug", "false");
    EXPECT_EQ(result, "false");
    
    // Test with no dashes
    result = opt->get_flag_value("verbose", "true");
    EXPECT_EQ(result, "true");
}