#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include <string>
#include <sstream>
#include <vector>

// Test fixture for config output tests
class ConfigOutputTest : public ::testing::Test {
protected:
    CLI::App app;
    std::stringstream output_stream;
    std::string comment_lead = "; ";

    void SetUp() override {
        app.footer("test footer");
    }
};

// Test that a simple option with description writes the description in config output
TEST_F(ConfigOutputTest, OptionWithDescription) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "This is a description");
    
    // Generate config output
    app.config_to_str(true, false);
    std::string output = app.config_to_str(true, false);
    
    // Check that description appears after the option value
    EXPECT_NE(output.find("This is a description"), std::string::npos);
    EXPECT_NE(output.find("; This is a description"), std::string::npos);
}

// Test that an option without description does NOT write description in config output
TEST_F(ConfigOutputTest, OptionWithoutDescription) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value);
    
    std::string output = app.config_to_str(false, false);
    
    // Should not contain any description line
    EXPECT_EQ(output.find("; "), std::string::npos);
}

// Test that write_description=false suppresses description output
TEST_F(ConfigOutputTest, WriteDescriptionFalse) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "Should not appear");
    
    std::string output = app.config_to_str(false, false);
    
    // Description should not be present when write_description is false
    EXPECT_EQ(output.find("Should not appear"), std::string::npos);
}

// Test that write_description=true includes description output
TEST_F(ConfigOutputTest, WriteDescriptionTrue) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "Should appear");
    
    std::string output = app.config_to_str(true, false);
    
    // Description should be present when write_description is true
    EXPECT_NE(output.find("Should appear"), std::string::npos);
}

// Test empty value with description - description should still be written
TEST_F(ConfigOutputTest, EmptyValueWithDescription) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "Description for empty");
    opt->default_str("");  // Force empty default
    
    std::string output = app.config_to_str(true, false);
    
    // Description should appear even with empty value
    EXPECT_NE(output.find("Description for empty"), std::string::npos);
}

// Test that description with newlines is properly formatted
TEST_F(ConfigOutputTest, DescriptionWithNewlines) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "Line1\nLine2\nLine3");
    
    std::string output = app.config_to_str(true, false);
    
    // Each line should be prefixed with comment lead
    EXPECT_NE(output.find("; Line1"), std::string::npos);
    EXPECT_NE(output.find("; Line2"), std::string::npos);
    EXPECT_NE(output.find("; Line3"), std::string::npos);
}

// Test that description with leading/trailing whitespace is preserved
TEST_F(ConfigOutputTest, DescriptionWithWhitespace) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "  spaced description  ");
    
    std::string output = app.config_to_str(true, false);
    
    // Whitespace should be preserved in the description
    EXPECT_NE(output.find("  spaced description  "), std::string::npos);
}

// Test multiple options with descriptions
TEST_F(ConfigOutputTest, MultipleOptionsWithDescriptions) {
    std::string opt1, opt2;
    app.add_option("--opt1", opt1, "First description");
    app.add_option("--opt2", opt2, "Second description");
    
    std::string output = app.config_to_str(true, false);
    
    // Both descriptions should appear
    EXPECT_NE(output.find("First description"), std::string::npos);
    EXPECT_NE(output.find("Second description"), std::string::npos);
}

// Test that description is written after the option value line
TEST_F(ConfigOutputTest, DescriptionAfterValue) {
    std::string opt_value = "test_value";
    auto* opt = app.add_option("--my-option", opt_value, "Description text");
    
    std::string output = app.config_to_str(true, false);
    
    // Find positions to verify order
    size_t value_pos = output.find("test_value");
    size_t desc_pos = output.find("Description text");
    
    EXPECT_NE(value_pos, std::string::npos);
    EXPECT_NE(desc_pos, std::string::npos);
    EXPECT_LT(value_pos, desc_pos);  // Description should come after value
}

// Test that description with special characters is handled
TEST_F(ConfigOutputTest, DescriptionWithSpecialChars) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "Description with #@$%^&*()");
    
    std::string output = app.config_to_str(true, false);
    
    // Special characters should be preserved
    EXPECT_NE(output.find("Description with #@$%^&*()"), std::string::npos);
}

// Test that empty description string is handled (no description written)
TEST_F(ConfigOutputTest, EmptyDescriptionString) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "");
    
    std::string output = app.config_to_str(true, false);
    
    // Empty description should not produce a comment line
    EXPECT_EQ(output.find("; \n"), std::string::npos);
}

// Test that description with only whitespace is handled
TEST_F(ConfigOutputTest, WhitespaceOnlyDescription) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "   ");
    
    std::string output = app.config_to_str(true, false);
    
    // Whitespace-only description should not produce a comment line
    EXPECT_EQ(output.find(";    \n"), std::string::npos);
}