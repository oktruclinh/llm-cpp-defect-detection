#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include <string>
#include <vector>

// Test fixture for App tests involving the "--" section close behavior
class AppSectionCloseTest : public ::testing::Test {
protected:
    CLI::App app;
    std::string captured_output;
    bool callback_called = false;

    void SetUp() override {
        app.configurable(true);
        app.callback([this]() {
            callback_called = true;
        });
    }
};

// Test that "--" triggers callbacks and requirements processing when configurable
TEST_F(AppSectionCloseTest, SectionCloseTriggersCallbacks) {
    int val = 0;
    app.add_option("--value", val, "A test option")->required();
    
    // Simulate parsing with "--" as a section close
    std::vector<std::string> args = {"--value", "42", "--"};
    app.parse(args);
    
    EXPECT_TRUE(callback_called);
    EXPECT_EQ(val, 42);
}

// Test that "--" does NOT trigger callbacks when configurable is false
TEST_F(AppSectionCloseTest, SectionCloseNoEffectWhenNotConfigurable) {
    app.configurable(false);
    int val = 0;
    app.add_option("--value", val, "A test option")->required();
    
    std::vector<std::string> args = {"--value", "42", "--"};
    EXPECT_THROW(app.parse(args), CLI::ParseError);
    EXPECT_FALSE(callback_called);
    EXPECT_EQ(val, 0); // Should not have been processed
}

// Test that "--" triggers requirements check (missing required option should fail)
TEST_F(AppSectionCloseTest, SectionCloseEnforcesRequirements) {
    app.add_option("--required-opt", "A required option")->required();
    
    std::vector<std::string> args = {"--"};
    EXPECT_THROW(app.parse(args), CLI::RequiredError);
    EXPECT_FALSE(callback_called);
}

// Test that "--" with all requirements satisfied works correctly
TEST_F(AppSectionCloseTest, SectionCloseWithAllRequirements) {
    int val = 0;
    app.add_option("--value", val, "A test option")->required();
    
    std::vector<std::string> args = {"--value", "10", "--"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
    EXPECT_EQ(val, 10);
}

// Test that "--" triggers callbacks for subcommands
TEST_F(AppSectionCloseTest, SectionCloseInSubcommand) {
    CLI::App* sub = app.add_subcommand("sub", "A subcommand");
    sub->configurable(true);
    
    int sub_val = 0;
    sub->add_option("--sub-opt", sub_val, "Sub option")->required();
    
    bool sub_callback_called = false;
    sub->callback([&sub_callback_called]() {
        sub_callback_called = true;
    });
    
    std::vector<std::string> args = {"sub", "--sub-opt", "99", "--"};
    app.parse(args);
    
    EXPECT_TRUE(sub_callback_called);
    EXPECT_EQ(sub_val, 99);
}

// Test that "--" does not affect non-configurable subcommand
TEST_F(AppSectionCloseTest, SectionCloseInNonConfigurableSubcommand) {
    CLI::App* sub = app.add_subcommand("sub", "A subcommand");
    sub->configurable(false);
    
    int sub_val = 0;
    sub->add_option("--sub-opt", sub_val, "Sub option")->required();
    
    std::vector<std::string> args = {"sub", "--sub-opt", "99", "--"};
    EXPECT_THROW(app.parse(args), CLI::ParseError);
    EXPECT_EQ(sub_val, 0);
}

// Test that "--" with no options or requirements still triggers callback
TEST_F(AppSectionCloseTest, SectionCloseWithNoOptions) {
    std::vector<std::string> args = {"--"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

// Test that "--" after positional arguments works correctly
TEST_F(AppSectionCloseTest, SectionCloseAfterPositional) {
    std::string pos;
    app.add_option("positional", pos, "A positional argument");
    
    std::vector<std::string> args = {"hello", "--"};
    app.parse(args);
    
    EXPECT_TRUE(callback_called);
    EXPECT_EQ(pos, "hello");
}

// Test that "--" with multiple section closes only processes once
TEST_F(AppSectionCloseTest, MultipleSectionCloses) {
    int val = 0;
    app.add_option("--value", val, "A test option");
    
    std::vector<std::string> args = {"--value", "5", "--", "--"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
    EXPECT_EQ(val, 5);
}

// Test that "--" with no callback set does not crash
TEST_F(AppSectionCloseTest, SectionCloseWithNoCallback) {
    app.callback(nullptr);
    int val = 0;
    app.add_option("--value", val, "A test option");
    
    std::vector<std::string> args = {"--value", "42", "--"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_EQ(val, 42);
}

// Test that "--" with empty option list still processes
TEST_F(AppSectionCloseTest, SectionCloseWithEmptyOptions) {
    std::vector<std::string> args = {"--"};
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

// Test that "--" with only flags works
TEST_F(AppSectionCloseTest, SectionCloseWithFlags) {
    bool flag = false;
    app.add_flag("--verbose", flag, "A flag");
    
    std::vector<std::string> args = {"--verbose", "--"};
    app.parse(args);
    
    EXPECT_TRUE(callback_called);
    EXPECT_TRUE(flag);
}

// Test that "--" with invalid option before it still throws
TEST_F(AppSectionCloseTest, SectionCloseAfterInvalidOption) {
    std::vector<std::string> args = {"--invalid-option", "--"};
    EXPECT_THROW(app.parse(args), CLI::ParseError);
    EXPECT_FALSE(callback_called);
}

// Test that "--" with valid options and then invalid option after it
TEST_F(AppSectionCloseTest, SectionCloseWithValidThenInvalid) {
    int val = 0;
    app.add_option("--value", val, "A test option");
    
    std::vector<std::string> args = {"--value", "10", "--", "--invalid"};
    EXPECT_THROW(app.parse(args), CLI::ParseError);
    EXPECT_TRUE(callback_called); // Callback should have been called before the error
    EXPECT_EQ(val, 10);
}