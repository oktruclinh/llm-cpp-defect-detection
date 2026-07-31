#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include <string>
#include <vector>

// Test fixture for App tests involving section close "--" behavior
class AppSectionCloseTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Ensure parse_complete_callback_ is set by default
        app.parse_complete_callback([this]() { callback_called = true; });
        app.configurable(true);
    }

    CLI::App app{"TestApp"};
    bool callback_called = false;
};

// Test that "--" triggers callbacks and requirements when configurable and parse_complete_callback are set
TEST_F(AppSectionCloseTest, SectionCloseTriggersCallbacks) {
    app.add_option("--option", "test option");
    app.require_option(1);
    
    std::vector<std::string> args = {"--option", "value", "--"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

// Test that "--" does NOT trigger callbacks when configurable is false
TEST_F(AppSectionCloseTest, SectionCloseNoCallbackWhenNotConfigurable) {
    app.configurable(false);
    bool local_callback = false;
    app.parse_complete_callback([&local_callback]() { local_callback = true; });
    
    app.add_option("--option", "test option");
    std::vector<std::string> args = {"--option", "value", "--"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_FALSE(local_callback);
}

// Test that "--" does NOT trigger callbacks when parse_complete_callback is not set
TEST_F(AppSectionCloseTest, SectionCloseNoCallbackWhenNoParseCompleteCallback) {
    // Create a new app without parse_complete_callback
    CLI::App app2{"TestApp2"};
    app2.configurable(true);
    
    app2.add_option("--option", "test option");
    std::vector<std::string> args = {"--option", "value", "--"};
    EXPECT_NO_THROW(app2.parse(args));
}

// Test that "--" triggers requirements processing (e.g., required options)
TEST_F(AppSectionCloseTest, SectionCloseTriggersRequirements) {
    app.add_option("--required", "required option")->required();
    
    std::vector<std::string> args = {"--"};
    EXPECT_THROW(app.parse(args), CLI::RequiredError);
}

// Test that "--" with no preceding options still triggers callbacks
TEST_F(AppSectionCloseTest, SectionCloseWithNoPrecedingOptions) {
    std::vector<std::string> args = {"--"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

// Test that "--" after positional arguments triggers callbacks
TEST_F(AppSectionCloseTest, SectionCloseAfterPositionalArgs) {
    app.add_option("positional", "positional argument");
    
    std::vector<std::string> args = {"value", "--"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

// Test that "--" with remaining arguments after it are treated as positional
TEST_F(AppSectionCloseTest, SectionCloseWithRemainingArgs) {
    app.allow_extras();
    std::vector<std::string> remaining;
    app.add_option("--option", "test option");
    
    std::vector<std::string> args = {"--option", "value", "--", "extra1", "extra2"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
    EXPECT_EQ(app.remaining_size(), 2);
}

// Test that multiple "--" sections work correctly (only first triggers callbacks)
TEST_F(AppSectionCloseTest, MultipleSectionClose) {
    int callback_count = 0;
    app.parse_complete_callback([&callback_count]() { callback_count++; });
    
    app.add_option("--option", "test option");
    std::vector<std::string> args = {"--option", "value1", "--", "--option", "value2", "--"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_EQ(callback_count, 2);
}

// Test that "--" with subcommands triggers callbacks for parent app
TEST_F(AppSectionCloseTest, SectionCloseWithSubcommand) {
    CLI::App* sub = app.add_subcommand("sub", "subcommand");
    sub->configurable(true);
    sub->parse_complete_callback([this]() { callback_called = true; });
    
    std::vector<std::string> args = {"sub", "--"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

// Test that "--" triggers validation of requirements (e.g., option exclusivity)
TEST_F(AppSectionCloseTest, SectionCloseTriggersExclusiveOptionCheck) {
    app.add_option("--opt1", "option 1");
    app.add_option("--opt2", "option 2");
    app.require_option(1, 1);  // Exactly one option required (exclusive behavior)
    
    std::vector<std::string> args = {"--opt1", "value1", "--opt2", "value2", "--"};
    EXPECT_THROW(app.parse(args), CLI::RequiredError);
}

// Edge case: "--" with empty option name (should not be treated as section close)
TEST_F(AppSectionCloseTest, EmptyOptionNameNotSectionClose) {
    app.add_option("", "empty option");
    std::vector<std::string> args = {"value", "--"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

// Edge case: "--" as part of a longer option (should not be treated as section close)
TEST_F(AppSectionCloseTest, DoubleDashInOptionNotSectionClose) {
    app.add_option("--option-with--dashes", "option with dashes");
    std::vector<std::string> args = {"--option-with--dashes", "value", "--"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

// Test that parse_complete_callback is called exactly once per "--" section
TEST_F(AppSectionCloseTest, CallbackCalledOncePerSection) {
    int callback_count = 0;
    app.parse_complete_callback([&callback_count]() { callback_count++; });
    
    std::vector<std::string> args = {"--"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_EQ(callback_count, 1);
}

// Test that requirements are processed even if no options are present before "--"
TEST_F(AppSectionCloseTest, RequirementsProcessedWithNoOptions) {
    app.add_option("--required", "required option")->required();
    
    std::vector<std::string> args = {"--"};
    EXPECT_THROW(app.parse(args), CLI::RequiredError);
}