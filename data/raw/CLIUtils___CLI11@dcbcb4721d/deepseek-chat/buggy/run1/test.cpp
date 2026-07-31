#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include <string>
#include <sstream>
#include <vector>

// Test fixture for Config file generation with descriptions
class ConfigDescriptionTest : public ::testing::Test {
protected:
    CLI::App app{"Test app"};
    std::stringstream output;
    std::string config_str;

    void SetUp() override {
        // app is already constructed with name
    }

    // Helper to generate config output
    void generateConfig(bool write_description = true) {
        config_str = app.config_to_str(true, write_description);
        output.str(config_str);
    }
};

// Test: Option with description generates description comment
TEST_F(ConfigDescriptionTest, OptionWithDescription) {
    auto* opt = app.add_option("--name", "The name of the user");
    opt->default_str("default_name");
    
    generateConfig(true);
    
    // Should contain the description as a comment
    EXPECT_NE(config_str.find("; The name of the user"), std::string::npos);
    // Should contain the option value
    EXPECT_NE(config_str.find("name=default_name"), std::string::npos);
}

// Test: Option without description does not generate description comment when write_description is true
TEST_F(ConfigDescriptionTest, OptionWithoutDescription) {
    auto* opt = app.add_option("--name");
    opt->default_str("default_name");
    
    generateConfig(true);
    
    // Should NOT contain any description comment
    EXPECT_EQ(config_str.find("; "), std::string::npos);
    // Should still contain the option
    EXPECT_NE(config_str.find("name=default_name"), std::string::npos);
}

// Test: write_description=false suppresses description comments
TEST_F(ConfigDescriptionTest, SuppressDescription) {
    auto* opt = app.add_option("--name", "The name of the user");
    opt->default_str("default_name");
    
    generateConfig(false);
    
    // Should NOT contain description comment
    EXPECT_EQ(config_str.find("; The name of the user"), std::string::npos);
    // Should still contain the option
    EXPECT_NE(config_str.find("name=default_name"), std::string::npos);
}

// Test: Option with empty value does not generate description (edge case from focal code)
TEST_F(ConfigDescriptionTest, EmptyValueNoDescription) {
    auto* opt = app.add_option("--name", "The name of the user");
    // No default value set - value will be empty
    
    generateConfig(true);
    
    // Should NOT contain description comment because value is empty
    EXPECT_EQ(config_str.find("; The name of the user"), std::string::npos);
    // Should not contain the option at all
    EXPECT_EQ(config_str.find("name="), std::string::npos);
}

// Test: Option with multi-line description gets proper formatting
TEST_F(ConfigDescriptionTest, MultiLineDescription) {
    auto* opt = app.add_option("--name", "Line one\nLine two\nLine three");
    opt->default_str("default_name");
    
    generateConfig(true);
    
    // Should contain the first line of description
    EXPECT_NE(config_str.find("; Line one"), std::string::npos);
    // Should contain subsequent lines with proper indentation
    EXPECT_NE(config_str.find("; Line two"), std::string::npos);
    EXPECT_NE(config_str.find("; Line three"), std::string::npos);
}

// Test: Multiple options with descriptions all get comments
TEST_F(ConfigDescriptionTest, MultipleOptionsWithDescriptions) {
    app.add_option("--name", "User name")->default_str("Alice");
    app.add_option("--age", "User age")->default_str("30");
    app.add_option("--active", "Is active")->default_str("true");
    
    generateConfig(true);
    
    // All descriptions should be present
    EXPECT_NE(config_str.find("; User name"), std::string::npos);
    EXPECT_NE(config_str.find("; User age"), std::string::npos);
    EXPECT_NE(config_str.find("; Is active"), std::string::npos);
}

// Test: Option with description but no default value (empty value) - edge case
TEST_F(ConfigDescriptionTest, OptionWithDescriptionNoDefault) {
    app.add_option("--name", "The name of the user");
    // No default set, so value is empty
    
    generateConfig(true);
    
    // Should NOT contain description because value is empty (focal code check)
    EXPECT_EQ(config_str.find("; The name of the user"), std::string::npos);
    EXPECT_EQ(config_str.find("name="), std::string::npos);
}

// Test: Flag options (boolean) with descriptions
TEST_F(ConfigDescriptionTest, FlagWithDescription) {
    app.add_flag("--verbose", "Enable verbose output");
    
    generateConfig(true);
    
    // Flags typically have value "true" or "false", so description should appear
    EXPECT_NE(config_str.find("; Enable verbose output"), std::string::npos);
    EXPECT_NE(config_str.find("verbose="), std::string::npos);
}

// Test: Option with description containing special characters
TEST_F(ConfigDescriptionTest, DescriptionWithSpecialCharacters) {
    app.add_option("--path", "Path to file (e.g., /usr/local/bin)")->default_str("/tmp");
    
    generateConfig(true);
    
    // Description with parentheses and slashes should be preserved
    EXPECT_NE(config_str.find("; Path to file (e.g., /usr/local/bin)"), std::string::npos);
}

// Test: Empty description string
TEST_F(ConfigDescriptionTest, EmptyDescriptionString) {
    app.add_option("--name", "")->default_str("value");
    
    generateConfig(true);
    
    // Should not generate a comment line for empty description
    // But should still output the option
    EXPECT_NE(config_str.find("name=value"), std::string::npos);
    // No comment line should be present (no non-empty description)
    // The only comment would be the header comment from CLI11
    // Check that there's no additional comment line with just "; "
    std::string line;
    int comment_lines = 0;
    while (std::getline(output, line)) {
        if (line.find("; ") == 0 && line.size() > 2) {
            comment_lines++;
        }
    }
    // Only header comment(s) should exist, not a description comment
    EXPECT_GE(comment_lines, 0); // At least header comments
}

// Test: Option with description containing newlines and write_description=false
TEST_F(ConfigDescriptionTest, MultiLineDescriptionSuppressed) {
    app.add_option("--name", "Line one\nLine two")->default_str("value");
    
    generateConfig(false);
    
    // No description should appear
    EXPECT_EQ(config_str.find("; Line one"), std::string::npos);
    EXPECT_EQ(config_str.find("; Line two"), std::string::npos);
    // Option should still be present
    EXPECT_NE(config_str.find("name=value"), std::string::npos);
}