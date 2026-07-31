#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include <string>
#include <vector>

// Test fixture for App tests that involve the "--" section close behavior
class AppSectionCloseTest : public ::testing::Test {
protected:
    CLI::App app;
    std::string callback_result;
    int int_val = 0;
    std::string str_val;

    void SetUp() override {
        app.configurable(true);
        app.callback([this]() { callback_result = "app_callback"; });
    }
};

// Test that "--" triggers callbacks and requirements processing when configurable
TEST_F(AppSectionCloseTest, SectionCloseTriggersCallbacks) {
    bool flag_called = false;
    app.add_flag("--flag", flag_called);

    bool req_called = false;
    auto* req_opt = app.add_option("--req", int_val);
    req_opt->required(true);

    // Simulate parsing that would encounter "--" as section close
    app.parse("--flag --req 42 --");

    EXPECT_TRUE(flag_called);
    EXPECT_EQ(int_val, 42);
    EXPECT_EQ(callback_result, "app_callback");
}

// Test that "--" does NOT trigger callbacks when configurable is false
TEST_F(AppSectionCloseTest, SectionCloseNoEffectWhenNotConfigurable) {
    app.configurable(false);
    bool flag_called = false;
    app.add_flag("--flag", flag_called);

    app.parse("--flag --");

    EXPECT_TRUE(flag_called);
    EXPECT_TRUE(callback_result.empty()); // callback should NOT have been called
}

// Test that "--" with no preceding options still processes empty state
TEST_F(AppSectionCloseTest, SectionCloseWithNoOptions) {
    app.parse("--");

    // Callbacks should still run even with no options
    EXPECT_EQ(callback_result, "app_callback");
}

// Test that "--" processes requirements and fails if unmet
TEST_F(AppSectionCloseTest, SectionCloseEnforcesRequirements) {
    auto* req_opt = app.add_option("--req", int_val);
    req_opt->required(true);

    // Missing required option before "--" should cause failure
    EXPECT_THROW(app.parse("--"), CLI::RequiredError);
}

// Test that "--" processes callbacks for subcommands
TEST_F(AppSectionCloseTest, SectionCloseWithSubcommand) {
    CLI::App* sub = app.add_subcommand("sub", "subcommand");
    bool sub_flag = false;
    sub->add_flag("--subflag", sub_flag);
    std::string sub_callback_result;
    sub->callback([&sub_callback_result]() { sub_callback_result = "sub_callback"; });
    sub->configurable(true);

    app.parse("sub --subflag --");

    EXPECT_TRUE(sub_flag);
    EXPECT_EQ(sub_callback_result, "sub_callback");
}

// Test that "--" with positional arguments still processes correctly
TEST_F(AppSectionCloseTest, SectionCloseWithPositional) {
    std::vector<std::string> positional;
    app.add_option("pos", positional);

    app.parse("arg1 arg2 --");

    ASSERT_EQ(positional.size(), 2u);
    EXPECT_EQ(positional[0], "arg1");
    EXPECT_EQ(positional[1], "arg2");
    EXPECT_EQ(callback_result, "app_callback");
}

// Test that "--" after all options processes callbacks exactly once
TEST_F(AppSectionCloseTest, SectionCloseProcessesCallbacksOnce) {
    int callback_count = 0;
    app.callback([&callback_count]() { callback_count++; });

    app.parse("--");

    EXPECT_EQ(callback_count, 1);
}

// Test that "--" with no configurable subcommand does not trigger its callbacks
TEST_F(AppSectionCloseTest, SectionCloseNonConfigurableSubcommand) {
    CLI::App* sub = app.add_subcommand("sub", "subcommand");
    sub->configurable(false);
    std::string sub_callback_result;
    sub->callback([&sub_callback_result]() { sub_callback_result = "sub_callback"; });

    app.parse("sub --");

    EXPECT_TRUE(sub_callback_result.empty()); // callback should NOT have been called
    EXPECT_EQ(callback_result, "app_callback"); // parent callback should still run
}

// Test that "--" with multiple subcommands processes correctly
TEST_F(AppSectionCloseTest, SectionCloseWithNestedSubcommands) {
    CLI::App* sub1 = app.add_subcommand("sub1", "first");
    CLI::App* sub2 = sub1->add_subcommand("sub2", "second");
    sub1->configurable(true);
    sub2->configurable(true);

    std::string sub1_result, sub2_result;
    sub1->callback([&sub1_result]() { sub1_result = "sub1"; });
    sub2->callback([&sub2_result]() { sub2_result = "sub2"; });

    app.parse("sub1 sub2 --");

    EXPECT_EQ(sub2_result, "sub2");
    EXPECT_EQ(sub1_result, "sub1");
    EXPECT_EQ(callback_result, "app_callback");
}

// Test that "--" with empty options list still processes callbacks
TEST_F(AppSectionCloseTest, SectionCloseEmptyOptions) {
    app.parse("--");

    EXPECT_EQ(callback_result, "app_callback");
}

// Test that "--" does not interfere with normal option parsing before it
TEST_F(AppSectionCloseTest, SectionClosePreservesPriorOptions) {
    std::string name;
    int count = 0;
    app.add_option("--name", name);
    app.add_option("--count", count);

    app.parse("--name test --count 5 --");

    EXPECT_EQ(name, "test");
    EXPECT_EQ(count, 5);
    EXPECT_EQ(callback_result, "app_callback");
}

// Test that "--" with no callbacks defined does not crash
TEST_F(AppSectionCloseTest, SectionCloseNoCallbacks) {
    app.callback(nullptr);
    app.parse("--");

    // Should not throw or crash
    SUCCEED();
}

// Test that "--" processes requirements even when no options are present
TEST_F(AppSectionCloseTest, SectionCloseRequirementsWithNoOptions) {
    auto* req_opt = app.add_option("--req", int_val);
    req_opt->required(true);

    EXPECT_THROW(app.parse("--"), CLI::RequiredError);
}

// Test that "--" with validators processes them
TEST_F(AppSectionCloseTest, SectionCloseWithValidators) {
    app.add_option("--num", int_val)->check(CLI::Range(1, 10));

    app.parse("--num 5 --");

    EXPECT_EQ(int_val, 5);
    EXPECT_EQ(callback_result, "app_callback");
}

// Test that "--" with failing validators throws
TEST_F(AppSectionCloseTest, SectionCloseWithFailingValidators) {
    app.add_option("--num", int_val)->check(CLI::Range(1, 10));

    EXPECT_THROW(app.parse("--num 15 --"), CLI::ValidationError);
}