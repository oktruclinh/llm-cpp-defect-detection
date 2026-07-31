#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include <string>
#include <sstream>
#include <vector>

// Test fixture for config output tests
class ConfigOutputTest : public ::testing::Test {
protected:
    CLI::App app;
    std::stringstream output;
    std::string commentLead = "; ";

    void SetUp() override {
        app.footer("test footer");
    }
};

// Test that a simple option with description writes description when write_description is true
TEST_F(ConfigOutputTest, OptionWithDescriptionWriteDescriptionTrue) {
    std::string opt_name = "my_option";
    std::string opt_desc = "This is a test option";
    app.add_option("--" + opt_name, opt_desc);

    // Simulate config output with write_description = true
    app.config_to_stream(output, true, false, commentLead);
    std::string result = output.str();

    // Should contain the description line
    EXPECT_NE(result.find(opt_desc), std::string::npos);
    // Should contain the comment lead before description
    EXPECT_NE(result.find(commentLead + opt_desc), std::string::npos);
}

// Test that a simple option with description does NOT write description when write_description is false
TEST_F(ConfigOutputTest, OptionWithDescriptionWriteDescriptionFalse) {
    std::string opt_name = "my_option";
    std::string opt_desc = "This is a test option";
    app.add_option("--" + opt_name, opt_desc);

    app.config_to_stream(output, false, false, commentLead);
    std::string result = output.str();

    // Should NOT contain the description
    EXPECT_EQ(result.find(opt_desc), std::string::npos);
}

// Test that an option without description does not write description even when write_description is true
TEST_F(ConfigOutputTest, OptionWithoutDescriptionWriteDescriptionTrue) {
    std::string opt_name = "no_desc_option";
    app.add_option("--" + opt_name);

    app.config_to_stream(output, true, false, commentLead);
    std::string result = output.str();

    // Should not contain any comment lead lines (except possibly header/footer)
    // The option line itself should be present but no description comment
    EXPECT_NE(result.find(opt_name), std::string::npos);
    // Count comment lines - should only be from header/footer, not from option description
    size_t comment_count = 0;
    size_t pos = 0;
    while ((pos = result.find(commentLead, pos)) != std::string::npos) {
        comment_count++;
        pos += commentLead.length();
    }
    // Header/footer may add comments, but option without description adds none
    EXPECT_LE(comment_count, 2u); // At most header and footer comments
}

// Test that an option with empty description does not write description
TEST_F(ConfigOutputTest, OptionWithEmptyDescription) {
    std::string opt_name = "empty_desc";
    app.add_option("--" + opt_name, "");

    app.config_to_stream(output, true, false, commentLead);
    std::string result = output.str();

    // Should not contain any description comment for this option
    // The option line should be present
    EXPECT_NE(result.find(opt_name), std::string::npos);
    // No description comment should follow the option value
    // Check that there's no comment lead immediately after the option value
    size_t opt_pos = result.find(opt_name);
    ASSERT_NE(opt_pos, std::string::npos);
    // Find the end of the option line
    size_t line_end = result.find('\n', opt_pos);
    ASSERT_NE(line_end, std::string::npos);
    // Check that the next line (if any) doesn't start with comment lead
    if (line_end + 1 < result.length()) {
        std::string next_line = result.substr(line_end + 1, commentLead.length());
        EXPECT_NE(next_line, commentLead);
    }
}

// Test that option with multi-line description is properly formatted
TEST_F(ConfigOutputTest, OptionWithMultiLineDescription) {
    std::string opt_name = "multi_line";
    std::string opt_desc = "Line one\nLine two\nLine three";
    app.add_option("--" + opt_name, opt_desc);

    app.config_to_stream(output, true, false, commentLead);
    std::string result = output.str();

    // Each line of description should be prefixed with comment lead
    EXPECT_NE(result.find(commentLead + "Line one"), std::string::npos);
    EXPECT_NE(result.find(commentLead + "Line two"), std::string::npos);
    EXPECT_NE(result.find(commentLead + "Line three"), std::string::npos);
}

// Test that option with description containing special characters is handled
TEST_F(ConfigOutputTest, OptionWithSpecialCharsInDescription) {
    std::string opt_name = "special";
    std::string opt_desc = "Description with <angle> &ampersand";
    app.add_option("--" + opt_name, opt_desc);

    app.config_to_stream(output, true, false, commentLead);
    std::string result = output.str();

    // The description should appear as-is (no escaping in config output)
    EXPECT_NE(result.find(opt_desc), std::string::npos);
}

// Test that multiple options with descriptions all get their descriptions written
TEST_F(ConfigOutputTest, MultipleOptionsWithDescriptions) {
    std::string opt1_name = "opt1";
    std::string opt1_desc = "First option";
    std::string opt2_name = "opt2";
    std::string opt2_desc = "Second option";
    
    app.add_option("--" + opt1_name, opt1_desc);
    app.add_option("--" + opt2_name, opt2_desc);

    app.config_to_stream(output, true, false, commentLead);
    std::string result = output.str();

    // Both descriptions should be present
    EXPECT_NE(result.find(opt1_desc), std::string::npos);
    EXPECT_NE(result.find(opt2_desc), std::string::npos);
}

// Test that option with description but no value set still writes description
TEST_F(ConfigOutputTest, OptionWithDescriptionNoValueSet) {
    std::string opt_name = "no_value";
    std::string opt_desc = "Option without value";
    app.add_option("--" + opt_name, opt_desc);

    // Don't set any value for the option
    app.config_to_stream(output, true, false, commentLead);
    std::string result = output.str();

    // The option should not appear in output (no value set), so description should not appear either
    EXPECT_EQ(result.find(opt_name), std::string::npos);
    EXPECT_EQ(result.find(opt_desc), std::string::npos);
}

// Test that option with value set and description is written correctly
TEST_F(ConfigOutputTest, OptionWithValueAndDescription) {
    std::string opt_name = "with_value";
    std::string opt_desc = "Option with a value";
    auto* opt = app.add_option("--" + opt_name, opt_desc);
    
    // Parse to set the value
    app.parse(std::vector<std::string>{"./app", "--" + opt_name, "test_value"});

    app.config_to_stream(output, true, false, commentLead);
    std::string result = output.str();

    // Should contain the option value
    EXPECT_NE(result.find("test_value"), std::string::npos);
    // Should contain the description
    EXPECT_NE(result.find(opt_desc), std::string::npos);
    // Description should be on a separate line after the option
    size_t value_pos = result.find("test_value");
    size_t desc_pos = result.find(opt_desc);
    EXPECT_GT(desc_pos, value_pos);
}

// Test that custom comment lead is used for descriptions
TEST_F(ConfigOutputTest, CustomCommentLead) {
    std::string opt_name = "custom_comment";
    std::string opt_desc = "Custom comment test";
    app.add_option("--" + opt_name, opt_desc);

    std::string custom_comment = "# ";
    app.config_to_stream(output, true, false, custom_comment);
    std::string result = output.str();

    // Should use the custom comment lead
    EXPECT_NE(result.find(custom_comment + opt_desc), std::string::npos);
    // Should NOT use the default comment lead
    EXPECT_EQ(result.find(commentLead + opt_desc), std::string::npos);
}

// Test that option with very long description is handled
TEST_F(ConfigOutputTest, VeryLongDescription) {
    std::string opt_name = "long_desc";
    std::string long_desc(1000, 'x');
    app.add_option("--" + opt_name, long_desc);

    app.config_to_stream(output, true, false, commentLead);
    std::string result = output.str();

    // The long description should be present
    EXPECT_NE(result.find(long_desc), std::string::npos);
}

// Test that option with description containing newlines and spaces is properly formatted
TEST_F(ConfigOutputTest, DescriptionWithNewlinesAndSpaces) {
    std::string opt_name = "formatting";
    std::string opt_desc = "  Indented line\n\nMultiple blank lines\n  More indentation";
    app.add_option("--" + opt_name, opt_desc);

    app.config_to_stream(output, true, false, commentLead);
    std::string result = output.str();

    // Each line should be prefixed with comment lead
    EXPECT_NE(result.find(commentLead + "  Indented line"), std::string::npos);
    EXPECT_NE(result.find(commentLead + "Multiple blank lines"), std::string::npos);
    EXPECT_NE(result.find(commentLead + "  More indentation"), std::string::npos);
}