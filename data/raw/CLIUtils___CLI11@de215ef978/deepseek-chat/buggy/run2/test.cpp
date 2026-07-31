#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include <string>
#include <vector>

// Test fixture for App tests
class AppTest : public ::testing::Test {
protected:
    CLI::App app;
    std::string output;
    bool callback_called = false;

    void SetUp() override {
        app = CLI::App{"Test app"};
        callback_called = false;
    }

    void test_callback() {
        callback_called = true;
    }
};

// Test that "--" triggers callbacks and requirements processing when configurable
TEST_F(AppTest, DoubleDashTriggersCallbacksWhenConfigurable) {
    app.configurable(true);
    int val = 0;
    app.add_option("--value", val, "A value")->check(CLI::Range(0, 100));
    app.callback([this]() { test_callback(); });

    // Simulate parsing "--" which should trigger callbacks and requirements
    app.parse("-- --value 42");
    
    EXPECT_TRUE(callback_called);
    EXPECT_EQ(val, 42);
}

// Test that "--" does NOT trigger callbacks when not configurable
TEST_F(AppTest, DoubleDashNoCallbacksWhenNotConfigurable) {
    app.configurable(false);
    int val = 0;
    app.add_option("--value", val, "A value");
    app.callback([this]() { test_callback(); });

    // "--" should not trigger callbacks when not configurable
    app.parse("-- --value 42");
    
    EXPECT_FALSE(callback_called);
    EXPECT_EQ(val, 0); // Value should not be set either
}

// Test that "--" triggers requirements checking when configurable
TEST_F(AppTest, DoubleDashTriggersRequirementsWhenConfigurable) {
    app.configurable(true);
    std::string required_opt;
    app.add_option("--required", required_opt, "Required option")->required();
    app.callback([this]() { test_callback(); });

    // "--" without the required option should throw
    EXPECT_THROW(app.parse("--"), CLI::RequiredError);
    EXPECT_FALSE(callback_called);
}

// Test that "--" with all requirements satisfied works when configurable
TEST_F(AppTest, DoubleDashWithSatisfiedRequirementsWhenConfigurable) {
    app.configurable(true);
    std::string required_opt;
    app.add_option("--required", required_opt, "Required option")->required();
    app.callback([this]() { test_callback(); });

    app.parse("-- --required value");
    
    EXPECT_TRUE(callback_called);
    EXPECT_EQ(required_opt, "value");
}

// Test that "--" processes callbacks in order with multiple options
TEST_F(AppTest, DoubleDashProcessesCallbacksInOrder) {
    app.configurable(true);
    std::vector<int> callback_order;
    
    auto* opt1 = app.add_flag("--flag1", "First flag");
    auto* opt2 = app.add_flag("--flag2", "Second flag");
    
    opt1->callback([&callback_order]() { callback_order.push_back(1); });
    opt2->callback([&callback_order]() { callback_order.push_back(2); });
    
    app.callback([&callback_order]() { callback_order.push_back(3); });

    app.parse("-- --flag1 --flag2");
    
    ASSERT_EQ(callback_order.size(), 3u);
    EXPECT_EQ(callback_order[0], 1);
    EXPECT_EQ(callback_order[1], 2);
    EXPECT_EQ(callback_order[2], 3);
}

// Test that "--" with no options still triggers callbacks when configurable
TEST_F(AppTest, DoubleDashWithNoOptionsTriggersCallbacks) {
    app.configurable(true);
    app.callback([this]() { test_callback(); });

    app.parse("--");
    
    EXPECT_TRUE(callback_called);
}

// Test that "--" with subcommands triggers callbacks on the subcommand
TEST_F(AppTest, DoubleDashWithSubcommandTriggersCallbacks) {
    app.configurable(true);
    CLI::App* sub = app.add_subcommand("sub", "A subcommand");
    sub->configurable(true);
    
    bool sub_callback_called = false;
    sub->callback([&sub_callback_called]() { sub_callback_called = true; });
    
    int val = 0;
    sub->add_option("--value", val, "A value");

    app.parse("sub -- --value 42");
    
    EXPECT_TRUE(sub_callback_called);
    EXPECT_EQ(val, 42);
}

// Test that "--" with empty subcommand name still works
TEST_F(AppTest, DoubleDashWithEmptyName) {
    app.configurable(true);
    app.callback([this]() { test_callback(); });

    // This tests the edge case where item.name is "--" (the section close marker)
    app.parse("--");
    
    EXPECT_TRUE(callback_called);
}

// Test that "--" processes requirements and throws on unmet requirements
TEST_F(AppTest, DoubleDashThrowsOnUnmetRequirements) {
    app.configurable(true);
    std::string opt;
    app.add_option("--opt", opt, "An option")->required();
    app.callback([this]() { test_callback(); });

    // "--" should trigger requirements check and throw because --opt is missing
    EXPECT_THROW(app.parse("--"), CLI::RequiredError);
    EXPECT_FALSE(callback_called);
}

// Test that "--" with positional arguments still processes callbacks
TEST_F(AppTest, DoubleDashWithPositionalArguments) {
    app.configurable(true);
    std::string pos;
    app.add_option("positional", pos, "A positional argument");
    app.callback([this]() { test_callback(); });

    app.parse("-- value");
    
    EXPECT_TRUE(callback_called);
    EXPECT_EQ(pos, "value");
}

// Test that "--" does not interfere with normal parsing when not configurable
TEST_F(AppTest, DoubleDashNormalParsingWhenNotConfigurable) {
    app.configurable(false);
    int val = 0;
    app.add_option("--value", val, "A value");
    app.callback([this]() { test_callback(); });

    // Normal parsing should work even with "--" present
    app.parse("--value 42 --");
    
    EXPECT_FALSE(callback_called); // Callback not called because "--" didn't trigger it
    EXPECT_EQ(val, 42);
}

// Test that "--" with multiple subcommands works correctly
TEST_F(AppTest, DoubleDashWithMultipleSubcommands) {
    app.configurable(true);
    auto* sub1 = app.add_subcommand("sub1", "First subcommand");
    auto* sub2 = app.add_subcommand("sub2", "Second subcommand");
    
    sub1->configurable(true);
    sub2->configurable(true);
    
    bool sub1_called = false;
    bool sub2_called = false;
    
    sub1->callback([&sub1_called]() { sub1_called = true; });
    sub2->callback([&sub2_called]() { sub2_called = true; });

    app.parse("sub1 -- sub2 --");
    
    EXPECT_TRUE(sub1_called);
    EXPECT_TRUE(sub2_called);
}