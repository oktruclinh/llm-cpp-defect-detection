#include <gtest/gtest.h>
#include <CLI/CLI.hpp> // Public API header

// The focal function is part of CLI::Option's internal logic for parsing
// config files. We need to simulate this context to test it.
// Specifically, the snippet is within a loop that processes key-value pairs
// from a config file, where 'opt' is a pointer to a CLI::Option,
// 'name' is the key (flag name), and 'value' is the string value.
// The `get_flag_value` method is the one being called.

TEST(ConfigFlagValueTest, BasicFlagValueRetrieval) {
    CLI::App app;
    auto opt = app.add_flag("--flag");

    // Simulate the context where the focal function is called.
    // We expect get_flag_value to return the original value if no specific
    // flag value mapping is defined.
    std::string name = "flag";
    std::string value = "true"; // A common value for a flag

    // The focal function snippet:
    // if(!opt->get_fnames().empty()) {
    //     value = opt->get_flag_value(name, value);
    // }
    // For a simple flag, get_fnames() will be empty, so the line won't execute.
    // To test get_flag_value, we need to set up a flag with specific values.

    // Let's create a flag with specific values
    auto opt_mapped = app.add_flag("--mapped_flag{on,off,enable,disable}");
    opt_mapped->group(""); // Hide from help for this test

    std::string mapped_name = "mapped_flag";

    // Test case 1: Value present in mapping
    std::string mapped_value_on = "on";
    std::string expected_on = "on"; // Expect "on" to map to "on"
    std::string result_on = opt_mapped->get_flag_value(mapped_name, mapped_value_on);
    EXPECT_EQ(result_on, expected_on);

    std::string mapped_value_enable = "enable";
    std::string expected_enable = "enable"; // Expect "enable" to map to "enable"
    std::string result_enable = opt_mapped->get_flag_value(mapped_name, mapped_value_enable);
    EXPECT_EQ(result_enable, expected_enable);

    // Test case 2: Value not present in mapping, should return original value
    std::string mapped_value_unknown = "yes";
    std::string expected_unknown = "yes"; // Should return original "yes"
    std::string result_unknown = opt_mapped->get_flag_value(mapped_name, mapped_value_unknown);
    EXPECT_EQ(result_unknown, expected_unknown);

    // Test case 3: Empty value
    std::string mapped_value_empty = "";
    std::string expected_empty = ""; // Should return original empty string
    std::string result_empty = opt_mapped->get_flag_value(mapped_name, mapped_value_empty);
    EXPECT_EQ(result_empty, expected_empty);

    // Test case 4: Case insensitivity (if applicable, CLI11's default is case-sensitive for flag values)
    // Let's assume CLI11's default behavior for flag values is case-sensitive unless specified.
    // The current implementation of get_flag_value does a direct lookup.
    std::string mapped_value_ON = "ON";
    std::string expected_ON = "ON"; // Should return original "ON" as it's not "on"
    std::string result_ON = opt_mapped->get_flag_value(mapped_name, mapped_value_ON);
    EXPECT_EQ(result_ON, expected_ON);
}

TEST(ConfigFlagValueTest, FlagWithDefaultValue) {
    CLI::App app;
    // A flag with a default value, but no specific flag values defined.
    // get_flag_value should still return the input value.
    auto opt = app.add_flag("--myflag", "A flag description")->default_val(false);
    opt->group(""); // Hide from help for this test

    std::string name = "myflag";

    // Test with "true"
    std::string value_true = "true";
    std::string expected_true = "true";
    std::string result_true = opt->get_flag_value(name, value_true);
    EXPECT_EQ(result_true, expected_true);

    // Test with "false"
    std::string value_false = "false";
    std::string expected_false = "false";
    std::string result_false = opt->get_flag_value(name, value_false);
    EXPECT_EQ(result_false, expected_false);

    // Test with an arbitrary string
    std::string value_arbitrary = "some_string";
    std::string expected_arbitrary = "some_string";
    std::string result_arbitrary = opt->get_flag_value(name, value_arbitrary);
    EXPECT_EQ(result_arbitrary, expected_arbitrary);
}

TEST(ConfigFlagValueTest, FlagWithCustomTrueFalseStrings) {
    CLI::App app;
    // A flag with custom true/false strings
    auto opt = app.add_flag("--verbose", "A verbose flag");
    opt->set_config_option_key("verbose"); // Ensure the config key matches
    opt->set_true_false_strings("yes", "no");
    opt->group(""); // Hide from help for this test

    std::string name = "verbose";

    // Test with custom true string
    std::string value_yes = "yes";
    std::string expected_yes = "yes";
    std::string result_yes = opt->get_flag_value(name, value_yes);
    EXPECT_EQ(result_yes, expected_yes);

    // Test with custom false string
    std::string value_no = "no";
    std::string expected_no = "no";
    std::string result_no = opt->get_flag_value(name, value_no);
    EXPECT_EQ(result_no, expected_no);

    // Test with default true/false strings (should not map)
    std::string value_true = "true";
    std::string expected_true = "true";
    std::string result_true = opt->get_flag_value(name, value_true);
    EXPECT_EQ(result_true, expected_true);

    std::string value_false = "false";
    std::string expected_false = "false";
    std::string result_false = opt->get_flag_value(name, value_false);
    EXPECT_EQ(result_false, expected_false);

    // Test with an unknown string
    std::string value_unknown = "maybe";
    std::string expected_unknown = "maybe";
    std::string result_unknown = opt->get_flag_value(name, value_unknown);
    EXPECT_EQ(result_unknown, expected_unknown);
}

TEST(ConfigFlagValueTest, OptionWithChoices) {
    CLI::App app;
    // An option with choices, which internally uses the same mechanism as flags
    // for mapping values if `get_flag_value` is called.
    std::string choice_val;
    auto opt = app.add_option("--mode", choice_val, "Operation mode")->choices({"fast", "slow", "medium"});
    opt->group(""); // Hide from help for this test

    std::string name = "mode";

    // Test with a valid choice
    std::string value_fast = "fast";
    std::string expected_fast = "fast";
    std::string result_fast = opt->get_flag_value(name, value_fast);
    EXPECT_EQ(result_fast, expected_fast);

    // Test with another valid choice
    std::string value_medium = "medium";
    std::string expected_medium = "medium";
    std::string result_medium = opt->get_flag_value(name, value_medium);
    EXPECT_EQ(result_medium, expected_medium);

    // Test with an invalid choice (should return original)
    std::string value_invalid = "turbo";
    std::string expected_invalid = "turbo";
    std::string result_invalid = opt->get_flag_value(name, value_invalid);
    EXPECT_EQ(result_invalid, expected_invalid);

    // Test with empty string
    std::string value_empty = "";
    std::string expected_empty = "";
    std::string result_empty = opt->get_flag_value(name, value_empty);
    EXPECT_EQ(result_empty, expected_empty);
}

TEST(ConfigFlagValueTest, OptionWithoutChoices) {
    CLI::App app;
    // A regular option without specific choices or flag values.
    // get_flag_value should always return the input value.
    std::string path_val;
    auto opt = app.add_option("--path", path_val, "A path");
    opt->group(""); // Hide from help for this test

    std::string name = "path";

    // Test with a normal string
    std::string value_normal = "/usr/local/bin";
    std::string expected_normal = "/usr/local/bin";
    std::string result_normal = opt->get_flag_value(name, value_normal);
    EXPECT_EQ(result_normal, expected_normal);

    // Test with an empty string
    std::string value_empty = "";
    std::string expected_empty = "";
    std::string result_empty = opt->get_flag_value(name, value_empty);
    EXPECT_EQ(result_empty, expected_empty);

    // Test with a string containing spaces
    std::string value_spaces = "my folder";
    std::string expected_spaces = "my folder";
    std::string result_spaces = opt->get_flag_value(name, value_spaces);
    EXPECT_EQ(result_spaces, expected_spaces);
}

TEST(ConfigFlagValueTest, SubcommandFlagValue) {
    CLI::App app;
    auto sub = app.add_subcommand("sub", "A subcommand");
    auto opt = sub->add_flag("--sub_flag{on,off}");
    opt->group(""); // Hide from help for this test

    std::string name = "sub_flag";

    // Test with a valid value for the subcommand's flag
    std::string value_on = "on";
    std::string expected_on = "on";
    std::string result_on = opt->get_flag_value(name, value_on);
    EXPECT_EQ(result_on, expected_on);

    // Test with an invalid value for the subcommand's flag
    std::string value_maybe = "maybe";
    std::string expected_maybe = "maybe";
    std::string result_maybe = opt->get_flag_value(name, value_maybe);
    EXPECT_EQ(result_maybe, expected_maybe);
}

TEST(ConfigFlagValueTest, FlagWithMultipleNames) {
    CLI::App app;
    // A flag with multiple names, but no specific flag values defined.
    // get_flag_value should still return the input value.
    auto opt = app.add_flag("-f,--flag", "A flag description");
    opt->group(""); // Hide from help for this test

    // The 'name' parameter to get_flag_value is the config key,
    // which usually corresponds to one of the flag names or a custom config key.
    // Here, we use the long name.
    std::string name = "flag";
    std::string value = "true";
    std::string expected = "true";
    std::string result = opt->get_flag_value(name, value);
    EXPECT_EQ(result, expected);

    // If the config key was the short name, it should still work the same.
    std::string short_name = "f";
    std::string result_short = opt->get_flag_value(short_name, value);
    EXPECT_EQ(result_short, expected);
}

TEST(ConfigFlagValueTest, FlagWithCustomConfigKeyAndValues) {
    CLI::App app;
    auto opt = app.add_flag("--myflag", "A flag");
    opt->set_config_option_key("custom_key");
    opt->set_true_false_strings("enabled", "disabled");
    opt->group(""); // Hide from help for this test

    // The 'name' parameter to get_flag_value should be the config key.
    std::string name = "custom_key";

    std::string value_enabled = "enabled";
    std::string expected_enabled = "enabled";
    std::string result_enabled = opt->get_flag_value(name, value_enabled);
    EXPECT_EQ(result_enabled, expected_enabled);

    std::string value_disabled = "disabled";
    std::string expected_disabled = "disabled";
    std::string result_disabled = opt->get_flag_value(name, value_disabled);
    EXPECT_EQ(result_disabled, expected_disabled);

    // Test with original flag name (should not map if custom_key is set)
    std::string original_name = "myflag";
    std::string value_true = "true";
    std::string expected_true = "true";
    std::string result_true = opt->get_flag_value(original_name, value_true);
    EXPECT_EQ(result_true, expected_true); // Should not map "true" to "enabled"
}