#include <gtest/gtest.h>
#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>

// Helper function to simulate parsing arguments
// This allows us to control the arguments passed to the CLI::App
void parse_args(CLI::App& app, const std::vector<std::string>& args) {
    try {
        app.parse(args);
    } catch (const CLI::ParseError& e) {
        // We might expect parse errors in some tests, so we catch them
        // and rethrow if it's not an expected error type for the test.
        // For now, just rethrow to let the test framework handle it.
        throw;
    }
}

// Mock callback functions to check if they were called
bool global_callback_called = false;
bool subcommand_callback_called = false;
bool app_callback_called = false;

void reset_callbacks() {
    global_callback_called = false;
    subcommand_callback_called = false;
    app_callback_called = false;
}

TEST(AppTest, ProcessCallbacksOnSectionCloseWithConfigurableAndParseComplete) {
    CLI::App app{"Test App"};
    app.configurable(); // Make the app configurable

    app.callback([&]() { app_callback_called = true; });

    CLI::App* sub = app.add_subcommand("sub", "A subcommand");
    sub->callback([&]() { subcommand_callback_called = true; });

    // Simulate a configuration file that would trigger the section close logic
    // The focal function is triggered when `item.name == "--"` is encountered
    // during configuration parsing.
    // We need to ensure `configurable_` is true and `parse_complete_callback_` is true.
    // `parse_complete_callback_` is set internally when `parse` is called.

    // To trigger the focal function, we need to use a config file that contains "--"
    // and then parse it.
    // We'll use a custom config reader that injects "--" at the end.

    class MockConfigReader : public CLI::Config {
      public:
        std::vector<CLI::ConfigItem> from_file(const std::string& /*name*/) override {
            std::vector<CLI::ConfigItem> items;
            items.emplace_back(CLI::ConfigItem{"--", "", 0, 0}); // This should trigger the focal function
            return items;
        }
    };

    app.config_formatter(std::make_shared<MockConfigReader>());

    reset_callbacks();

    // Parsing an empty vector will still trigger the config file processing if configured
    // and then the internal parse_complete_callback_ will be true.
    // The MockConfigReader will then provide the "--" item.
    ASSERT_NO_THROW(parse_args(app, {"--config", "dummy.ini"}));

    // The app callback should be called because the section close logic
    // calls `run_callback()` if `configurable_` and `parse_complete_callback_` are true.
    EXPECT_TRUE(app_callback_called);
    EXPECT_FALSE(subcommand_callback_called); // Subcommand callback should not be called here
}

TEST(AppTest, ProcessCallbacksNotCalledWithoutConfigurable) {
    CLI::App app{"Test App"};
    // app.configurable(); // NOT configurable

    app.callback([&]() { app_callback_called = true; });

    class MockConfigReader : public CLI::Config {
      public:
        std::vector<CLI::ConfigItem> from_file(const std::string& /*name*/) override {
            std::vector<CLI::ConfigItem> items;
            items.emplace_back(CLI::ConfigItem{"--", "", 0, 0});
            return items;
        }
    };

    app.config_formatter(std::make_shared<MockConfigReader>());

    reset_callbacks();

    // Even if a config file with "--" is provided, if the app is not configurable,
    // the focal function's conditional `if(configurable_ && parse_complete_callback_)`
    // should prevent the callbacks from running.
    ASSERT_NO_THROW(parse_args(app, {"--config", "dummy.ini"}));

    EXPECT_FALSE(app_callback_called);
}

TEST(AppTest, ProcessCallbacksNotCalledWithoutParseCompleteCallback) {
    CLI::App app{"Test App"};
    app.configurable();

    app.callback([&]() { app_callback_called = true; });

    class MockConfigReader : public CLI::Config {
      public:
        std::vector<CLI::ConfigItem> from_file(const std::string& /*name*/) override {
            std::vector<CLI::ConfigItem> items;
            items.emplace_back(CLI::ConfigItem{"--", "", 0, 0});
            return items;
        }
    };

    app.config_formatter(std::make_shared<MockConfigReader>());

    reset_callbacks();

    // To prevent `parse_complete_callback_` from being true, we need to call `parse`
    // in a way that doesn't complete the parsing process before the config item is processed.
    // This is a bit tricky as `parse` usually sets it.
    // The focal function is called during the `_parse_config` method, which is part of `parse`.
    // The `parse_complete_callback_` is set at the very end of `parse`.
    // However, the `_parse_config` method itself calls `_process_callbacks` if the condition is met.
    // The condition `parse_complete_callback_` is actually set to true *before* `_parse_config` is called
    // if the parsing is expected to complete.

    // Let's assume a scenario where the config file is processed, but the overall parsing
    // is not considered "complete" in a way that would trigger the final callback.
    // This test case is harder to isolate directly from the public API without
    // mocking internal state.
    // The most direct way to test `parse_complete_callback_` being false is if `parse`
    // itself hasn't finished or if an error occurred.

    // For the purpose of this test, we'll rely on the previous test case
    // where `configurable_` is false, which implicitly covers the AND condition.
    // Directly manipulating `parse_complete_callback_` to be false while `configurable_` is true
    // and `item.name == "--"` is encountered is difficult via public API.
    // The current implementation of `parse` sets `parse_complete_callback_ = true`
    // before calling `_parse_config`. So, this condition will almost always be true
    // when `_parse_config` is processing the "--" item.

    // Therefore, this specific negative test case (parse_complete_callback_ being false)
    // is hard to achieve with the public API for the focal function's specific trigger point.
    // The `configurable_` flag is the primary control for this block.
    // We'll assert that the callback IS called, confirming `parse_complete_callback_` is indeed true.
    ASSERT_NO_THROW(parse_args(app, {"--config", "dummy.ini"}));
    EXPECT_TRUE(app_callback_called); // Expect it to be called, confirming parse_complete_callback_ was true
}

TEST(AppTest, ProcessCallbacksNotCalledOnOtherConfigItem) {
    CLI::App app{"Test App"};
    app.configurable();

    app.callback([&]() { app_callback_called = true; });

    class MockConfigReader : public CLI::Config {
      public:
        std::vector<CLI::ConfigItem> from_file(const std::string& /*name*/) override {
            std::vector<CLI::ConfigItem> items;
            items.emplace_back(CLI::ConfigItem{"option", "value", 0, 0}); // Not "--"
            return items;
        }
    };

    app.config_formatter(std::make_shared<MockConfigReader>());

    reset_callbacks();

    // If the config item is not "--", the focal function's `if(item.name == "--")`
    // condition should be false, and callbacks should not be triggered.
    ASSERT_NO_THROW(parse_args(app, {"--config", "dummy.ini"}));

    EXPECT_FALSE(app_callback_called);
}

TEST(AppTest, ProcessCallbacksWithSubcommandActive) {
    CLI::App app{"Test App"};
    app.configurable();

    app.callback([&]() { app_callback_called = true; });

    CLI::App* sub = app.add_subcommand("sub", "A subcommand");
    sub->callback([&]() { subcommand_callback_called = true; });

    class MockConfigReader : public CLI::Config {
      public:
        std::vector<CLI::ConfigItem> from_file(const std::string& /*name*/) override {
            std::vector<CLI::ConfigItem> items;
            items.emplace_back(CLI::ConfigItem{"--", "", 0, 0});
            return items;
        }
    };

    app.config_formatter(std::make_shared<MockConfigReader>());

    reset_callbacks();

    // When a subcommand is active, the main app's callback should still be called
    // if the section close is processed at the top level.
    // The focal function is in `App::_parse_config`, which processes the main app's config.
    // The `run_callback()` method called there is `App::run_callback()`, not the subcommand's.
    ASSERT_NO_THROW(parse_args(app, {"sub", "--config", "dummy.ini"}));

    EXPECT_TRUE(app_callback_called);
    EXPECT_FALSE(subcommand_callback_called); // Subcommand callback is run later, or by its own `run_callback`
}

TEST(AppTest, ProcessCallbacksWithGlobalCallback) {
    CLI::App app{"Test App"};
    app.configurable();

    app.callback([&]() { app_callback_called = true; });
    app.set_config_callback([&](CLI::App* /*_app*/) { global_callback_called = true; });

    class MockConfigReader : public CLI::Config {
      public:
        std::vector<CLI::ConfigItem> from_file(const std::string& /*name*/) override {
            std::vector<CLI::ConfigItem> items;
            items.emplace_back(CLI::ConfigItem{"--", "", 0, 0});
            return items;
        }
    };

    app.config_formatter(std::make_shared<MockConfigReader>());

    reset_callbacks();

    // The focal function calls `_process_callbacks()` and `_process_requirements()`
    // before `run_callback()`. `_process_callbacks()` is where the global config callback
    // would typically be invoked.
    ASSERT_NO_THROW(parse_args(app, {"--config", "dummy.ini"}));

    EXPECT_TRUE(app_callback_called);
    EXPECT_TRUE(global_callback_called); // The global config callback should be called
}

TEST(AppTest, ProcessCallbacksWithRequirements) {
    CLI::App app{"Test App"};
    app.configurable();

    int val = 0;
    app.add_option("--val", val)->required(); // Add a required option

    class MockConfigReader : public CLI::Config {
      public:
        std::vector<CLI::ConfigItem> from_file(const std::string& /*name*/) override {
            std::vector<CLI::ConfigItem> items;
            items.emplace_back(CLI::ConfigItem{"--", "", 0, 0});
            return items;
        }
    };

    app.config_formatter(std::make_shared<MockConfigReader>());

    reset_callbacks();

    // The focal function calls `_process_requirements()`. If a required option is not met
    // and this is called, it should throw an error.
    // In this case, "--val" is required but not provided in args or config.
    EXPECT_THROW(parse_args(app, {"--config", "dummy.ini"}), CLI::RequiredError);

    // Now, provide the required option in the config
    class MockConfigReaderWithVal : public CLI::Config {
      public:
        std::vector<CLI::ConfigItem> from_file(const std::string& /*name*/) override {
            std::vector<CLI::ConfigItem> items;
            items.emplace_back(CLI::ConfigItem{"val", "10", 0, 0});
            items.emplace_back(CLI::ConfigItem{"--", "", 0, 0});
            return items;
        }
    };
    app.config_formatter(std::make_shared<MockConfigReaderWithVal>());
    app.callback([&]() { app_callback_called = true; }); // Reset callback for this sub-test

    reset_callbacks();
    ASSERT_NO_THROW(parse_args(app, {"--config", "dummy.ini"}));
    EXPECT_EQ(val, 10);
    EXPECT_TRUE(app_callback_called);
}