#include <gtest/gtest.h>
#include <CLI/CLI.hpp> // Public API header

// The focal function is part of CLI::ConfigBase::get_flag_value,
// which is called internally when parsing a config file or string.
// We will test its behavior by setting up CLI::App and parsing
// configuration strings that trigger the relevant logic.

TEST(ConfigFlagValueTest, BasicFlagValueRetrieval) {
    CLI::App app;
    std::string config_value = "default";
    app.add_option("--config-value", config_value)->group("Config");

    // Simulate a config file with a flag value
    std::string config_str = "config-value=new_value";
    app.parse_config(config_str);

    // The focal function's logic should have been applied,
    // and the option's value should be updated.
    EXPECT_EQ(config_value, "new_value");
}

TEST(ConfigFlagValueTest, FlagValueWithEmptyInput) {
    CLI::App app;
    std::string config_value = "default";
    app.add_option("--config-value", config_value)->group("Config");

    // Simulate a config file with an empty flag value
    std::string config_str = "config-value=";
    app.parse_config(config_str);

    // The focal function's `if(!value.empty())` condition should be false,
    // so the value should remain "default" if the option doesn't have a default,
    // or be set to empty if it's explicitly set to empty.
    // CLI11's default behavior for an empty string in config is to set it to empty.
    EXPECT_EQ(config_value, "");
}

TEST(ConfigFlagValueTest, FlagValueWithNoFnames) {
    CLI::App app;
    std::string config_value = "default";
    // Add an option without explicit flag names (e.g., positional argument)
    // However, for config parsing, options usually have names.
    // To test `!opt->get_fnames().empty()`, we need an option that *does* have fnames.
    // The focal function is specifically for named options in config.
    // Let's re-evaluate the condition: `if(!opt->get_fnames().empty())`
    // This condition checks if the option *itself* has flag names defined.
    // Most options added via `add_option` will have fnames.
    // We need to ensure `get_flag_value` is called.

    // Let's assume an option with a name will always have fnames.
    // The `get_flag_value` is called if `!opt->get_fnames().empty()` is true.
    // So, to test the branch where it's *false*, we'd need an option without fnames,
    // which is not typical for config parsing.
    // The focal function is inside a block that is already handling a named option.
    // Thus, `opt->get_fnames()` will almost certainly not be empty.
    // The important part is `value = opt->get_flag_value(name, value);`

    // Let's test a scenario where `get_flag_value` might return the original value.
    // This typically happens if the option doesn't have special flag handling.
    app.add_option("--my-option", config_value);
    std::string config_str = "my-option=test_value";
    app.parse_config(config_str);
    EXPECT_EQ(config_value, "test_value"); // Should be updated
}

TEST(ConfigFlagValueTest, FlagValueWithSpecialHandling) {
    CLI::App app;
    bool my_flag = false;
    // Add a flag that has special handling (e.g., `set_flag()`)
    // The `get_flag_value` method is designed to handle these.
    app.add_flag("--my-flag", my_flag)->group("Flags");

    // Test setting the flag to true
    std::string config_str_true = "my-flag=true";
    app.parse_config(config_str_true);
    EXPECT_TRUE(my_flag);

    // Reset and test setting the flag to false
    my_flag = true; // Set to true initially
    std::string config_str_false = "my-flag=false";
    app.parse_config(config_str_false);
    EXPECT_FALSE(my_flag);

    // Reset and test setting the flag with no value (should be true)
    my_flag = false;
    std::string config_str_no_value = "my-flag"; // No value implies true for flags
    app.parse_config(config_str_no_value);
    EXPECT_TRUE(my_flag);

    // Reset and test setting the flag with an empty value (should be false)
    my_flag = true;
    std::string config_str_empty_value = "my-flag=";
    app.parse_config(config_str_empty_value);
    EXPECT_FALSE(my_flag);
}

TEST(ConfigFlagValueTest, MultipleOptionsInConfig) {
    CLI::App app;
    std::string opt1_val = "def1";
    int opt2_val = 0;
    bool opt3_val = false;

    app.add_option("--opt1", opt1_val);
    app.add_option("--opt2", opt2_val);
    app.add_flag("--opt3", opt3_val);

    std::string config_str = R"(
        opt1=new_val1
        opt2=123
        opt3=true
    )";
    app.parse_config(config_str);

    EXPECT_EQ(opt1_val, "new_val1");
    EXPECT_EQ(opt2_val, 123);
    EXPECT_TRUE(opt3_val);
}

TEST(ConfigFlagValueTest, OptionWithDefaultValueAndEmptyConfig) {
    CLI::App app;
    std::string config_value = "initial";
    app.add_option("--my-option", config_value)->default_val("default_from_cli");

    // If config provides an empty string, it should override the default.
    std::string config_str = "my-option=";
    app.parse_config(config_str);
    EXPECT_EQ(config_value, ""); // Empty string from config should be honored.

    // If config provides no value, the default should be used if no other value is set.
    // However, `parse_config` will typically set the value if the option is mentioned.
    // Let's test a scenario where the option is not mentioned in config.
    CLI::App app2;
    std::string config_value2 = "initial";
    app2.add_option("--my-option", config_value2)->default_val("default_from_cli");
    std::string config_str2 = ""; // Empty config string
    app2.parse_config(config_str2);
    EXPECT_EQ(config_value2, "default_from_cli"); // Default should be applied.

    // If config provides a value, it should override the default.
    CLI::App app3;
    std::string config_value3 = "initial";
    app3.add_option("--my-option", config_value3)->default_val("default_from_cli");
    std::string config_str3 = "my-option=config_override";
    app3.parse_config(config_str3);
    EXPECT_EQ(config_value3, "config_override");
}

TEST(ConfigFlagValueTest, OptionWithRequiredFlag) {
    CLI::App app;
    std::string value;
    app.add_option("--my-option", value)->required();

    // Providing a value in config should satisfy required.
    std::string config_str = "my-option=some_value";
    EXPECT_NO_THROW(app.parse_config(config_str));
    EXPECT_EQ(value, "some_value");

    // Providing an empty value in config should also be valid for string options.
    // The required check happens after parsing.
    std::string config_str_empty = "my-option=";
    EXPECT_NO_THROW(app.parse_config(config_str_empty));
    EXPECT_EQ(value, "");
}

TEST(ConfigFlagValueTest, OptionWithVectorOfValues) {
    CLI::App app;
    std::vector<std::string> values;
    app.add_option("--my-options", values);

    // Config parsing for vectors typically involves multiple entries or a comma-separated list.
    // The focal function handles a single `value` string at a time.
    // Let's test how CLI11 handles this.
    std::string config_str = R"(
        my-options=val1
        my-options=val2
    )";
    app.parse_config(config_str);
    ASSERT_EQ(values.size(), 2);
    EXPECT_EQ(values[0], "val1");
    EXPECT_EQ(values[1], "val2");

    // Test with a single empty value
    values.clear();
    std::string config_str_empty = "my-options=";
    app.parse_config(config_str_empty);
    ASSERT_EQ(values.size(), 1);
    EXPECT_EQ(values[0], "");

    // Test with multiple empty values
    values.clear();
    std::string config_str_multiple_empty = R"(
        my-options=
        my-options=
    )";
    app.parse_config(config_str_multiple_empty);
    ASSERT_EQ(values.size(), 2);
    EXPECT_EQ(values[0], "");
    EXPECT_EQ(values[1], "");
}

TEST(ConfigFlagValueTest, OptionWithTransformCallback) {
    CLI::App app;
    std::string value;
    app.add_option("--my-option", value)
        ->transform([](std::string s) {
            std::transform(s.begin(), s.end(), s.begin(), ::toupper);
            return s;
        });

    std::string config_str = "my-option=test_value";
    app.parse_config(config_str);
    EXPECT_EQ(value, "TEST_VALUE");

    // Test with empty string, transform should still apply (to an empty string)
    std::string config_str_empty = "my-option=";
    app.parse_config(config_str_empty);
    EXPECT_EQ(value, ""); // toupper("") is ""
}