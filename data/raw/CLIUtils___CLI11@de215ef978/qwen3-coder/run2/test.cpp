#include <gtest/gtest.h>
#include "CLI/App.hpp"
#include "CLI/CLI.hpp"

TEST(AppSectionCloseTest, NormalSectionClose) {
    CLI::App app("Test app");
    
    // Add a callback to test the section close functionality
    bool callback_called = false;
    app.callback([&callback_called]() {
        callback_called = true;
    });
    
    // Parse with -- to indicate section close
    std::vector<std::string> args = {"test", "--"};
    
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

TEST(AppSectionCloseTest, SectionCloseWithConfigurable) {
    CLI::App app("Test app");
    
    // Make app configurable
    app.configurable(true);
    
    bool callback_called = false;
    bool requirements_met = false;
    
    app.callback([&callback_called]() {
        callback_called = true;
    });
    
    // Add a required option to test requirements
    std::string value;
    app.add_option("--option", value, "Test option")->required();
    
    // Parse with -- to indicate section close
    std::vector<std::string> args = {"test", "--"};
    
    // This should not throw and should call callbacks
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

TEST(AppSectionCloseTest, SectionCloseWithParseCompleteCallback) {
    CLI::App app("Test app");
    
    // Make app configurable
    app.configurable(true);
    
    bool callback_called = false;
    bool parse_complete_called = false;
    
    app.callback([&callback_called]() {
        callback_called = true;
    });
    
    // Add parse complete callback
    app.parse_complete_callback([&parse_complete_called]() {
        parse_complete_called = true;
    });
    
    // Parse with -- to indicate section close
    std::vector<std::string> args = {"test", "--"};
    
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
    EXPECT_TRUE(parse_complete_called);
}

TEST(AppSectionCloseTest, SectionCloseWithBothCallbacks) {
    CLI::App app("Test app");
    
    // Make app configurable
    app.configurable(true);
    
    bool callback_called = false;
    bool parse_complete_called = false;
    bool requirements_met = false;
    
    app.callback([&callback_called]() {
        callback_called = true;
    });
    
    // Add parse complete callback
    app.parse_complete_callback([&parse_complete_called]() {
        parse_complete_called = true;
    });
    
    // Add a required option to test requirements
    std::string value;
    app.add_option("--option", value, "Test option")->required();
    
    // Parse with -- to indicate section close
    std::vector<std::string> args = {"test", "--"};
    
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
    EXPECT_TRUE(parse_complete_called);
}

TEST(AppSectionCloseTest, SectionCloseWithoutConfigurable) {
    CLI::App app("Test app");
    
    // Don't make app configurable
    // app.configurable(false); // Default is false
    
    bool callback_called = false;
    
    app.callback([&callback_called]() {
        callback_called = true;
    });
    
    // Parse with -- to indicate section close
    std::vector<std::string> args = {"test", "--"};
    
    EXPECT_NO_THROW(app.parse(args));
    // Callback should not be called when not configurable
    EXPECT_FALSE(callback_called);
}

TEST(AppSectionCloseTest, SectionCloseWithoutParseCompleteCallback) {
    CLI::App app("Test app");
    
    // Make app configurable
    app.configurable(true);
    
    bool callback_called = false;
    
    app.callback([&callback_called]() {
        callback_called = true;
    });
    
    // No parse complete callback set
    
    // Parse with -- to indicate section close
    std::vector<std::string> args = {"test", "--"};
    
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
    // Should not crash even without parse complete callback
}

TEST(AppSectionCloseTest, SectionCloseWithMultipleArgs) {
    CLI::App app("Test app");
    
    // Make app configurable
    app.configurable(true);
    
    bool callback_called = false;
    
    app.callback([&callback_called]() {
        callback_called = true;
    });
    
    // Add parse complete callback
    app.parse_complete_callback([]() {
        // Empty callback
    });
    
    // Parse with -- to indicate section close and additional args
    std::vector<std::string> args = {"test", "--", "extra", "args"};
    
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

TEST(AppSectionCloseTest, SectionCloseWithNoArgs) {
    CLI::App app("Test app");
    
    // Make app configurable
    app.configurable(true);
    
    bool callback_called = false;
    
    app.callback([&callback_called]() {
        callback_called = true;
    });
    
    // Parse with just -- to indicate section close
    std::vector<std::string> args = {"test", "--"};
    
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}

TEST(AppSectionCloseTest, SectionCloseWithEmptyApp) {
    CLI::App app("Test app");
    
    // Make app configurable
    app.configurable(true);
    
    // No callbacks set
    
    // Parse with -- to indicate section close
    std::vector<std::string> args = {"test", "--"};
    
    EXPECT_NO_THROW(app.parse(args));
    // Should not crash
}

TEST(AppSectionCloseTest, SectionCloseWithRequiredOptionNotProvided) {
    CLI::App app("Test app");
    
    // Make app configurable
    app.configurable(true);
    
    bool callback_called = false;
    
    app.callback([&callback_called]() {
        callback_called = true;
    });
    
    // Add a required option that won't be provided
    std::string value;
    app.add_option("--required-option", value, "Required option")->required();
    
    // Parse with -- to indicate section close
    std::vector<std::string> args = {"test", "--"};
    
    // Should not throw, but callback should still be called
    EXPECT_NO_THROW(app.parse(args));
    EXPECT_TRUE(callback_called);
}