#include <gtest/gtest.h>
#include "CLI/App.hpp"
#include "CLI/CLI.hpp"

TEST(AppSectionCloseTest, NormalSectionClose) {
    CLI::App app("Test app");
    
    bool callback_called = false;
    auto callback = [&callback_called]() {
        callback_called = true;
    };
    
    app.set_config("--config", "", "Config file");
    app.callback(callback);
    
    // Parse with section close
    std::vector<std::string> args = {"--", "--config", "test.ini"};
    
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

TEST(AppSectionCloseTest, SectionCloseWithoutConfigurable) {
    CLI::App app("Test app");
    
    // Disable configurable
    app.set_config("--config", "", "Config file", false);
    
    bool callback_called = false;
    auto callback = [&callback_called]() {
        callback_called = true;
    };
    
    app.callback(callback);
    
    // Parse with section close
    std::vector<std::string> args = {"--", "--config", "test.ini"};
    
    EXPECT_NO_THROW(app.parse(args));
    // Callback should not be called because configurable_ is false
    EXPECT_FALSE(callback_called);
}

TEST(AppSectionCloseTest, SectionCloseWithoutParseCompleteCallback) {
    CLI::App app("Test app");
    
    // Don't set any callback to trigger parse_complete_callback_ being null
    app.set_config("--config", "", "Config file");
    
    // Parse with section close
    std::vector<std::string> args = {"--", "--config", "test.ini"};
    
    EXPECT_NO_THROW(app.parse(args));
}

TEST(AppSectionCloseTest, SectionCloseWithRequirements) {
    CLI::App app("Test app");
    
    // Add a required option
    std::string value;
    app.add_option("--required", value, "Required option")->required();
    
    bool callback_called = false;
    auto callback = [&callback_called]() {
        callback_called = true;
    };
    
    app.callback(callback);
    
    // Parse with section close - should fail due to missing required option
    std::vector<std::string> args = {"--"};
    
    EXPECT_THROW(app.parse(args), CLI::RequiredError);
    // Callback should not be called due to error
    EXPECT_FALSE(callback_called);
}

TEST(AppSectionCloseTest, SectionCloseWithValidRequirements) {
    CLI::App app("Test app");
    
    // Add a required option
    std::string value;
    app.add_option("--required", value, "Required option")->required();
    
    bool callback_called = false;
    auto callback = [&callback_called]() {
        callback_called = true;
    };
    
    app.callback(callback);
    
    // Parse with section close and valid option
    std::vector<std::string> args = {"--", "--required", "value"};
    
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

TEST(AppSectionCloseTest, MultipleSectionClose) {
    CLI::App app("Test app");
    
    bool callback_called = false;
    auto callback = [&callback_called]() {
        callback_called = true;
    };
    
    app.set_config("--config", "", "Config file");
    app.callback(callback);
    
    // Parse with multiple section closes (should work)
    std::vector<std::string> args = {"--", "--", "--config", "test.ini"};
    
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

TEST(AppSectionCloseTest, SectionCloseAtEnd) {
    CLI::App app("Test app");
    
    bool callback_called = false;
    auto callback = [&callback_called]() {
        callback_called = true;
    };
    
    app.set_config("--config", "", "Config file");
    app.callback(callback);
    
    // Parse with section close at end
    std::vector<std::string> args = {"--config", "test.ini", "--"};
    
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

TEST(AppSectionCloseTest, SectionCloseWithNoArgs) {
    CLI::App app("Test app");
    
    bool callback_called = false;
    auto callback = [&callback_called]() {
        callback_called = true;
    };
    
    app.set_config("--config", "", "Config file");
    app.callback(callback);
    
    // Parse with only section close
    std::vector<std::string> args = {"--"};
    
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}