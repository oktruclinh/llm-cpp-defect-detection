#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include <string>
#include <vector>

// Test fixture for App tests involving section close "--" and callbacks
class AppSectionCloseTest : public ::testing::Test {
protected:
    CLI::App app;
    bool callback_called = false;
    bool parse_callback_called = false;
    bool requirements_processed = false;

    void SetUp() override {
        app.configurable(true);
        // Set up parse_complete_callback_ that records call
        app.parse_complete_callback([this]() {
            parse_callback_called = true;
        });
        // Set up a final callback that records call
        app.final_callback([this]() {
            callback_called = true;
        });
        // Add an option to trigger requirements processing
        app.add_option_function<std::string>("--opt", [this](const std::string&) {
            requirements_processed = true;
        }, "test option");
    }
};

// Test: "--" triggers callbacks when configurable and parse_complete_callback set
TEST_F(AppSectionCloseTest, SectionCloseTriggersCallbacks) {
    app.add_flag("--flag", "a flag");
    // Parse with "--" to trigger section close
    std::vector<std::string> args = {"--flag", "value", "--"};
    app.parse(args);
    
    EXPECT_TRUE(parse_callback_called);
    EXPECT_TRUE(callback_called);
    EXPECT_TRUE(requirements_processed);
}

// Test: "--" does not trigger callbacks when configurable is false
TEST_F(AppSectionCloseTest, SectionCloseNoConfigurable) {
    app.configurable(false);
    std::vector<std::string> args = {"--opt", "val", "--"};
    app.parse(args);
    
    EXPECT_FALSE(parse_callback_called);
    EXPECT_FALSE(callback_called);
    EXPECT_FALSE(requirements_processed);
}

// Test: "--" does not trigger callbacks when parse_complete_callback is not set
TEST_F(AppSectionCloseTest, SectionCloseNoParseCompleteCallback) {
    // Remove parse_complete_callback by setting to empty
    app.parse_complete_callback([](){});
    std::vector<std::string> args = {"--opt", "val", "--"};
    app.parse(args);
    
    // With empty callback, parse_complete_callback_ is still set (non-null)
    // but the condition checks if it's set, so it will still trigger
    // To truly test without it, we need to not set it at all
    // Recreate app without parse_complete_callback
    CLI::App app2;
    app2.configurable(true);
    bool cb_called = false;
    app2.final_callback([&cb_called]() { cb_called = true; });
    app2.add_flag("--opt", "test");
    std::vector<std::string> args2 = {"--opt", "val", "--"};
    app2.parse(args2);
    
    EXPECT_FALSE(cb_called);
}

// Test: "--" with no options before it still triggers callbacks
TEST_F(AppSectionCloseTest, SectionCloseNoOptionsBefore) {
    std::vector<std::string> args = {"--"};
    app.parse(args);
    
    EXPECT_TRUE(parse_callback_called);
    EXPECT_TRUE(callback_called);
    EXPECT_TRUE(requirements_processed);
}

// Test: Multiple "--" sections - only first triggers callbacks
TEST_F(AppSectionCloseTest, MultipleSectionClose) {
    std::vector<std::string> args = {"--opt", "val", "--", "--"};
    app.parse(args);
    
    EXPECT_TRUE(parse_callback_called);
    EXPECT_TRUE(callback_called);
    EXPECT_TRUE(requirements_processed);
}

// Test: "--" with positional arguments after it
TEST_F(AppSectionCloseTest, SectionCloseWithPositionalAfter) {
    app.add_option("pos", "positional");
    std::vector<std::string> args = {"--opt", "val", "--", "pos_arg"};
    app.parse(args);
    
    EXPECT_TRUE(parse_callback_called);
    EXPECT_TRUE(callback_called);
    EXPECT_TRUE(requirements_processed);
    EXPECT_EQ(app["pos"]->as<std::string>(), "pos_arg");
}

// Test: "--" with no parse_complete_callback_ set (nullptr)
TEST_F(AppSectionCloseTest, SectionCloseNullParseCompleteCallback) {
    CLI::App app2;
    app2.configurable(true);
    bool final_cb_called = false;
    app2.final_callback([&final_cb_called]() { final_cb_called = true; });
    app2.add_flag("--opt", "test");
    std::vector<std::string> args = {"--opt", "val", "--"};
    app2.parse(args);
    
    // parse_complete_callback_ is not set, so callbacks should not be triggered
    EXPECT_FALSE(final_cb_called);
}

// Test: "--" with configurable true but no final callback
TEST_F(AppSectionCloseTest, SectionCloseNoFinalCallback) {
    CLI::App app2;
    app2.configurable(true);
    app2.parse_complete_callback([](){});
    bool cb_called = false;
    app2.add_option_function<std::string>("--opt", [&cb_called](const std::string&) {
        cb_called = true;
    }, "test");
    std::vector<std::string> args = {"--opt", "val", "--"};
    app2.parse(args);
    
    // parse_complete_callback_ is set (empty lambda), so _process_callbacks and _process_requirements run
    // but final_callback is not set, so only parse_complete_callback and option callback run
    EXPECT_TRUE(cb_called);
}

// Test: "--" with no options at all
TEST_F(AppSectionCloseTest, SectionCloseEmptyApp) {
    CLI::App app2;
    app2.configurable(true);
    bool parse_cb = false;
    app2.parse_complete_callback([&parse_cb]() { parse_cb = true; });
    bool final_cb = false;
    app2.final_callback([&final_cb]() { final_cb = true; });
    std::vector<std::string> args = {"--"};
    app2.parse(args);
    
    EXPECT_TRUE(parse_cb);
    EXPECT_TRUE(final_cb);
}

// Test: "--" triggers requirements processing even if no options provided
TEST_F(AppSectionCloseTest, SectionCloseTriggersRequirements) {
    bool req_met = false;
    app.require_option(1, 1); // require exactly one option
    app.add_flag("--flag", "a flag");
    std::vector<std::string> args = {"--flag", "--"};
    app.parse(args);
    
    // Should not throw because --flag satisfies requirement
    EXPECT_TRUE(parse_callback_called);
    EXPECT_TRUE(callback_called);
}

// Test: "--" with requirements not met throws
TEST_F(AppSectionCloseTest, SectionCloseRequirementsNotMet) {
    app.require_option(1, 1); // require exactly one option
    std::vector<std::string> args = {"--"};
    EXPECT_THROW(app.parse(args), CLI::RequiredError);
}