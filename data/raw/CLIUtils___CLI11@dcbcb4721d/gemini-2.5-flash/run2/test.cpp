#include <gtest/gtest.h>
#include <CLI/CLI.hpp> // Public API header

// The focal function is part of CLI::Option's internal logic for parsing config files.
// We will test its behavior by creating CLI::App and CLI::Option objects,
// and then simulating config file parsing.

TEST(ConfigInlTest, GetFlagValueNormalCase) {
    CLI::App app;
    std::string my_string_var;
    CLI::Option* opt = app.add_option("--flag", my_string_var)->delimiter(',')->expected(1, 2);

    // Simulate a config file entry where the flag is present
    // and the value needs to be processed by get_flag_value.
    // The focal function is called when `opt->get_fnames()` is not empty.
    // For a flag, get_fnames() would contain the flag names.

    // The add_option call already adds the flag name to the option.
    // opt->add_flag("--flag"); // This is usually done internally by add_option

    // Simulate the internal state before the focal function is called
    // In a real scenario, 'name' would be the flag name, and 'value' would be the config value.
    // std::string name = "--flag"; // Not directly used in this test setup
    // std::string value = "true"; // Not directly used in this test setup

    // The focal function is part of the logic that processes the value.
    // We need to ensure that if get_flag_value is called, it returns the correct value.
    // For a simple flag, "true" should map to "1" or be kept as "true" depending on the type.
    // Since my_string_var is a string, it should remain "true".
    // If it were a bool, it would convert to "1".

    // The focal function is not directly exposed. We need to trigger its execution
    // through the config parsing mechanism.
    // Let's create a config string that would trigger this.
    std::string config_content = "flag=true";
    std::stringstream ss(config_content);
    app.set_config("--config", "", "Config file", false);

    // Parse the config file
    // This will internally call the logic containing the focal function.
    // We expect my_string_var to be set to "true".
    app.parse_from_stream(ss);

    ASSERT_EQ(my_string_var, "true");

    // Test with a different value
    std::string config_content2 = "flag=false";
    std::stringstream ss2(config_content2);
    app.parse_from_stream(ss2);
    ASSERT_EQ(my_string_var, "false");
}

TEST(ConfigInlTest, GetFlagValueEmptyValue) {
    CLI::App app;
    std::string my_string_var;
    CLI::Option* opt = app.add_option("--flag", my_string_var);

    // The add_option call already adds the flag name to the option.
    // opt->add_flag("--flag");

    // Simulate an empty value in the config.
    // The focal function has a check `if(!value.empty())`.
    // If value is empty, the block containing get_flag_value should be skipped.
    // This means my_string_var should not be set by this specific config entry.
    // However, CLI11's config parsing will typically treat an empty value for a string
    // as an empty string.

    std::string config_content = "flag="; // Empty value
    std::stringstream ss(config_content);
    app.set_config("--config", "", "Config file", false);

    app.parse_from_stream(ss);

    ASSERT_EQ(my_string_var, ""); // Expect an empty string
}

TEST(ConfigInlTest, GetFlagValueNoFlagNames) {
    CLI::App app;
    std::string my_string_var = "initial";
    // Create an option that does not have flag names (e.g., a positional argument)
    // or where get_fnames() would return empty.
    // The focal function has a check `if(!opt->get_fnames().empty())`.
    // If get_fnames() is empty, the block containing get_flag_value should be skipped.
    CLI::Option* opt = app.add_option("positional", my_string_var);

    // Ensure get_fnames() is empty for this option type
    // Positional arguments do not have flag names, so get_fnames() should be empty.
    ASSERT_TRUE(opt->get_fnames().empty());

    // Simulate a config entry for this option.
    // Even if a value is provided, get_flag_value should NOT be called.
    // The value should be assigned directly.
    std::string config_content = "positional=some_value";
    std::stringstream ss(config_content);
    app.set_config("--config", "", "Config file", false);

    app.parse_from_stream(ss);

    ASSERT_EQ(my_string_var, "some_value"); // Value should be assigned directly, not via get_flag_value

    // Let's try to make a flag-like option but without explicitly adding flag names
    // (though add_option usually adds them).
    // This scenario is harder to achieve naturally as add_option usually populates fnames.
    // We rely on the previous test case where get_fnames() is empty.
}

TEST(ConfigInlTest, GetFlagValueBooleanFlag) {
    CLI::App app;
    bool my_bool_var = false;
    CLI::Option* opt = app.add_flag("--enable", my_bool_var);

    // The add_flag call already adds the flag name to the option.
    // opt->add_flag("--enable"); // Ensure flag names are present

    std::string config_content_true = "enable=true";
    std::stringstream ss_true(config_content_true);
    app.set_config("--config", "", "Config file", false);
    app.parse_from_stream(ss_true);
    ASSERT_TRUE(my_bool_var);

    my_bool_var = true; // Reset for next test
    std::string config_content_false = "enable=false";
    std::stringstream ss_false(config_content_false);
    app.parse_from_stream(ss_false);
    ASSERT_FALSE(my_bool_var);

    my_bool_var = false; // Reset for next test
    std::string config_content_one = "enable=1";
    std::stringstream ss_one(config_content_one);
    app.parse_from_stream(ss_one);
    ASSERT_TRUE(my_bool_var);

    my_bool_var = true; // Reset for next test
    std::string config_content_zero = "enable=0";
    std::stringstream ss_zero(config_content_zero);
    app.parse_from_stream(ss_zero);
    ASSERT_FALSE(my_bool_var);
}

TEST(ConfigInlTest, GetFlagValueMultiValueOption) {
    CLI::App app;
    std::vector<int> my_int_vec;
    CLI::Option* opt = app.add_option("--values", my_int_vec)->delimiter(',');

    // The add_option call already adds the flag name to the option.
    // opt->add_flag("--values");

    // The focal function processes a single value at a time.
    // For multi-value options, the parser splits the string first.
    // Here, we're testing how a single value "1,2,3" is handled if it were passed
    // as a single string to the internal logic.
    // However, CLI11's config parser usually handles delimiters before calling
    // the value processing logic for individual elements.
    // Let's test with a single value that would be part of a list.

    std::string config_content = "values=10";
    std::stringstream ss(config_content);
    app.set_config("--config", "", "Config file", false);
    app.parse_from_stream(ss);
    ASSERT_EQ(my_int_vec.size(), 1);
    ASSERT_EQ(my_int_vec[0], 10);

    // Test with multiple values (parser should split them)
    my_int_vec.clear();
    std::string config_content_multi = "values=20,30";
    std::stringstream ss_multi(config_content_multi);
    app.parse_from_stream(ss_multi);
    ASSERT_EQ(my_int_vec.size(), 2);
    ASSERT_EQ(my_int_vec[0], 20);
    ASSERT_EQ(my_int_vec[1], 30);
}

TEST(ConfigInlTest, GetFlagValueRequiredOption) {
    CLI::App app;
    std::string my_string_var;
    CLI::Option* opt = app.add_option("--required", my_string_var)->required();

    // The add_option call already adds the flag name to the option.
    // opt->add_flag("--required");

    // Providing a value for a required option
    std::string config_content = "required=some_value";
    std::stringstream ss(config_content);
    app.set_config("--config", "", "Config file", false);
    app.parse_from_stream(ss);
    ASSERT_EQ(my_string_var, "some_value");

    // Not providing a value for a required option should throw an error
    // This tests the broader context, not directly the focal function's path,
    // but ensures the config parsing works as expected for required options.
    std::string config_content_missing = ""; // Empty config, required option not set
    std::stringstream ss_missing(config_content_missing);
    app.set_config("--config", "", "Config file", false);
    // Reset app to clear previous state
    CLI::App app2;
    std::string my_string_var2;
    // The add_option call already adds the flag name to the option.
    app2.add_option("--required", my_string_var2)->required();

    EXPECT_THROW(app2.parse_from_stream(ss_missing), CLI::RequiredError);
}

TEST(ConfigInlTest, GetFlagValueDefaultValue) {
    CLI::App app;
    std::string my_string_var = "default_val";
    CLI::Option* opt = app.add_option("--opt", my_string_var);

    // The add_option call already adds the flag name to the option.
    // opt->add_flag("--opt");

    // If no value is provided in config, the default should remain.
    std::string config_content_empty = "";
    std::stringstream ss_empty(config_content_empty);
    app.set_config("--config", "", "Config file", false);
    app.parse_from_stream(ss_empty);
    ASSERT_EQ(my_string_var, "default_val");

    // If a value is provided, it should override the default.
    std::string config_content_override = "opt=new_val";
    std::stringstream ss_override(config_content_override);
    app.parse_from_stream(ss_override);
    ASSERT_EQ(my_string_var, "new_val");
}