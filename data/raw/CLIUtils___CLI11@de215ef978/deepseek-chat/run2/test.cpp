#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include <string>
#include <vector>

// Test fixture for section close tests
class SectionCloseTest : public ::testing::Test {
protected:
    CLI::App app;
    bool callback_called = false;
    bool requirements_met = false;
    
    void SetUp() override {
        app.configurable(true);
        app.parse_complete_callback([this]() {
            callback_called = true;
        });
        // Add a requirement that will be checked
        app.add_option("--req", requirements_met);
    }
};

// Test that "--" triggers callbacks and requirements when configurable and parse_complete_callback are set
TEST_F(SectionCloseTest, SectionCloseTriggersCallbacks) {
    app.add_option("--option", 0);
    
    // Parse with section close marker
    app.parse("--option 42 -- --req true");
    
    EXPECT_TRUE(callback_called);
    EXPECT_TRUE(requirements_met);
}

// Test that "--" does NOT trigger callbacks when configurable is false
TEST_F(SectionCloseTest, NoCallbacksWhenNotConfigurable) {
    app.configurable(false);
    app.add_option("--option", 0);
    
    app.parse("--option 42 -- --req true");
    
    EXPECT_FALSE(callback_called);
    EXPECT_FALSE(requirements_met);
}

// Test that "--" does NOT trigger callbacks when parse_complete_callback is not set
TEST_F(SectionCloseTest, NoCallbacksWithoutParseCompleteCallback) {
    app.parse_complete_callback(nullptr);
    app.add_option("--option", 0);
    
    app.parse("--option 42 -- --req true");
    
    EXPECT_FALSE(callback_called);
    EXPECT_FALSE(requirements_met);
}

// Test that "--" at the end of arguments still triggers callbacks
TEST_F(SectionCloseTest, SectionCloseAtEnd) {
    app.add_option("--option", 0);
    
    app.parse("--option 42 --");
    
    EXPECT_TRUE(callback_called);
}

// Test that "--" with no preceding options still triggers callbacks
TEST_F(SectionCloseTest, SectionCloseWithNoPrecedingOptions) {
    app.parse("--");
    
    EXPECT_TRUE(callback_called);
}

// Test that "--" with only positional arguments after it triggers callbacks
TEST_F(SectionCloseTest, SectionCloseWithPositionalArgs) {
    std::vector<std::string> positional;
    app.add_option("pos", positional);
    
    app.parse("-- arg1 arg2");
    
    EXPECT_TRUE(callback_called);
    EXPECT_EQ(positional, std::vector<std::string>({"arg1", "arg2"}));
}

// Test that multiple "--" sections work correctly (only first triggers callbacks)
TEST_F(SectionCloseTest, MultipleSectionCloses) {
    app.add_option("--option", 0);
    int count = 0;
    app.parse_complete_callback([&count]() { count++; });
    
    app.parse("--option 1 -- --option 2 --");
    
    EXPECT_EQ(count, 2);
}

// Test that "--" with no arguments after it still triggers callbacks
TEST_F(SectionCloseTest, SectionCloseWithNoArguments) {
    app.parse("--");
    
    EXPECT_TRUE(callback_called);
}

// Test that "--" in the middle of options triggers callbacks and processes remaining args
TEST_F(SectionCloseTest, SectionCloseInMiddle) {
    std::vector<std::string> positional;
    app.add_option("pos", positional);
    
    app.parse("--option 42 -- pos1 pos2");
    
    EXPECT_TRUE(callback_called);
    EXPECT_EQ(positional, std::vector<std::string>({"pos1", "pos2"}));
}

// Test that "--" with subcommands triggers callbacks for the parent app
TEST_F(SectionCloseTest, SectionCloseWithSubcommand) {
    CLI::App* sub = app.add_subcommand("sub", "A subcommand");
    sub->configurable(true);
    bool sub_callback = false;
    sub->parse_complete_callback([&sub_callback]() { sub_callback = true; });
    
    app.parse("sub --");
    
    EXPECT_TRUE(callback_called);  // Parent callback should be called
    EXPECT_TRUE(sub_callback);     // Subcommand callback should also be called
}

// Test that "--" with empty parse_complete_callback doesn't crash
TEST_F(SectionCloseTest, SectionCloseWithNullCallback) {
    app.parse_complete_callback(nullptr);
    app.configurable(true);
    
    EXPECT_NO_THROW(app.parse("--"));
}

// Test that "--" triggers requirements processing
TEST_F(SectionCloseTest, SectionCloseTriggersRequirements) {
    bool req_met = false;
    app.add_option("--req", req_met)->required();
    
    app.parse("--req true --");
    
    EXPECT_TRUE(callback_called);
    EXPECT_TRUE(req_met);
}

// Test that "--" with unmet requirements throws
TEST_F(SectionCloseTest, SectionCloseWithUnmetRequirements) {
    app.add_option("--req", 0)->required();
    
    EXPECT_THROW(app.parse("--"), CLI::RequiredError);
    EXPECT_FALSE(callback_called);  // Callback should not be called if requirements fail
}

// Test that "--" with configurable false and no callback doesn't crash
TEST_F(SectionCloseTest, SectionCloseWithNoConfigNoCallback) {
    app.configurable(false);
    app.parse_complete_callback(nullptr);
    
    EXPECT_NO_THROW(app.parse("--"));
}

// Test that "--" with only subcommands triggers callbacks correctly
TEST_F(SectionCloseTest, SectionCloseWithOnlySubcommands) {
    app.add_subcommand("sub1");
    app.add_subcommand("sub2");
    
    app.parse("sub1 -- sub2");
    
    EXPECT_TRUE(callback_called);
}