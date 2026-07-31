#include <gtest/gtest.h>
#include "CLI/CLI.hpp"

// Test that flag values are correctly transformed when the option has flag names
TEST(ConfigInlTest, FlagValueTransformationWithFlagNames) {
    CLI::App app;
    auto opt = app.add_flag("--verbose,-v", "Enable verbose output");
    
    // Simulate the focal function behavior: when value is non-empty and option has flag names
    // The flag_value should be applied (e.g., "true" -> "1", "false" -> "0")
    std::string value = "true";
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value("verbose", value);
        }
    }
    EXPECT_EQ(value, "1");
    
    value = "false";
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value("verbose", value);
        }
    }
    EXPECT_EQ(value, "0");
}

// Test that flag values are NOT transformed when the option has no flag names
TEST(ConfigInlTest, NoFlagValueTransformationWithoutFlagNames) {
    CLI::App app;
    auto opt = app.add_option("--value", "A regular option");
    
    std::string value = "some_value";
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value("value", value);
        }
    }
    // No transformation should occur since option has no flag names
    EXPECT_EQ(value, "some_value");
}

// Test that empty value is not processed (no transformation)
TEST(ConfigInlTest, EmptyValueNoTransformation) {
    CLI::App app;
    auto opt = app.add_flag("--verbose,-v", "Enable verbose output");
    
    std::string value = "";
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value("verbose", value);
        }
    }
    // Empty value should remain empty
    EXPECT_TRUE(value.empty());
}

// Test with multiple flag names and various boolean-like values
TEST(ConfigInlTest, FlagValueTransformationMultipleNames) {
    CLI::App app;
    auto opt = app.add_flag("--enable,-e,--activate", "Enable feature");
    
    std::string value = "1";
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value("enable", value);
        }
    }
    EXPECT_EQ(value, "1");
    
    value = "0";
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value("enable", value);
        }
    }
    EXPECT_EQ(value, "0");
    
    value = "yes";
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value("enable", value);
        }
    }
    EXPECT_EQ(value, "1");
    
    value = "no";
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value("enable", value);
        }
    }
    EXPECT_EQ(value, "0");
}

// Test that non-boolean values are passed through unchanged for flag options
TEST(ConfigInlTest, NonBooleanValuePassedThrough) {
    CLI::App app;
    auto opt = app.add_flag("--flag,-f", "A flag");
    
    std::string value = "custom_value";
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value("flag", value);
        }
    }
    // Non-boolean values should be passed through unchanged
    EXPECT_EQ(value, "custom_value");
}

// Test edge case: option with flag names but empty flag value map
TEST(ConfigInlTest, EmptyFlagValueMap) {
    CLI::App app;
    // Create a flag option that will have flag names but no predefined flag values
    auto opt = app.add_flag("--test,-t");
    
    std::string value = "some_value";
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value("test", value);
        }
    }
    // Without predefined flag values, the input should be returned as-is
    EXPECT_EQ(value, "some_value");
}

// Test that the transformation is case-sensitive for boolean values
TEST(ConfigInlTest, CaseSensitiveBooleanValues) {
    CLI::App app;
    auto opt = app.add_flag("--verbose,-v", "Verbose mode");
    
    std::string value = "True";
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value("verbose", value);
        }
    }
    // "True" (capital T) should not be transformed to "1"
    EXPECT_EQ(value, "True");
    
    value = "FALSE";
    if(!value.empty()) {
        if(!opt->get_fnames().empty()) {
            value = opt->get_flag_value("verbose", value);
        }
    }
    // "FALSE" (all caps) should not be transformed to "0"
    EXPECT_EQ(value, "FALSE");
}