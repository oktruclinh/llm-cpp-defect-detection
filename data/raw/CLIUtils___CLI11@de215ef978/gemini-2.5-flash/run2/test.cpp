#include <gtest/gtest.h>
#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>

// Helper function to simulate parsing arguments
void parse_args(CLI::App& app, const std::vector<std::string>& args_in) {
    // CLI::App::parse expects a non-const reference to a vector of strings
    // because it might modify the vector (e.g., remove parsed arguments).
    // Create a mutable copy for parsing.
    std::vector<std::string> args = args_in;
    try {
        app.parse(args);
    } catch (const CLI::ParseError& e) {
        // We might expect parse errors in some tests, so catch them
        // and rethrow if it's not an expected error.
        // For these tests, we are primarily interested in the callback behavior.
        if (e.get_exit_code() != 0) { // Non-zero exit code usually means an error
            throw;
        }
    }
}

// Mock callback functions to track calls
bool global_callback_called = false;
bool sub_callback_called = false;
bool option_callback_called = false;
bool positional_callback_called = false;
bool parse_complete_callback_called = false;

void reset_callbacks() {
    global_callback_called = false;
    sub_callback_called = false;
    option_callback_called = false;
    positional_callback_called = false;
    parse_complete_callback_called = false;
}

TEST(AppParseCompleteCallbackTest, NoDashDashNoCallbackCalled) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });

    int val = 0;
    app.add_option("--val", val);

    parse_args(app, {"--val", "10"});

    EXPECT_FALSE(parse_complete_callback_called);
}

TEST(AppParseCompleteCallbackTest, DashDashWithConfigurableAndCallbackCalled) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });

    int val = 0;
    app.add_option("--val", val);

    // The presence of "--" should trigger the parse_complete_callback if configurable_ is true
    // By default, App is configurable_ = true
    parse_args(app, {"--val", "10", "--"});

    EXPECT_TRUE(parse_complete_callback_called);
    EXPECT_EQ(val, 10); // Ensure parsing still happened
}

TEST(AppParseCompleteCallbackTest, DashDashWithoutConfigurableNoCallbackCalled) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });

    // Set configurable_ to false
    app.configurable(false);

    int val = 0;
    app.add_option("--val", val);

    parse_args(app, {"--val", "10", "--"});

    EXPECT_FALSE(parse_complete_callback_called);
    EXPECT_EQ(val, 10); // Ensure parsing still happened
}

TEST(AppParseCompleteCallbackTest, DashDashWithoutCallbackSetNoCallbackCalled) {
    reset_callbacks();
    CLI::App app("Test App");
    // Do NOT set parse_complete_callback

    int val = 0;
    app.add_option("--val", val);

    parse_args(app, {"--val", "10", "--"});

    EXPECT_FALSE(parse_complete_callback_called); // Should still be false as it was never set
    EXPECT_EQ(val, 10); // Ensure parsing still happened
}

TEST(AppParseCompleteCallbackTest, DashDashWithSubcommandNoCallbackCalledOnMainApp) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });

    auto sub = app.add_subcommand("sub", "A subcommand");
    sub->parse_complete_callback([&]() { sub_callback_called = true; });

    int val = 0;
    sub->add_option("--val", val);

    // The "--" is for the subcommand, not the main app
    parse_args(app, {"sub", "--val", "10", "--"});

    EXPECT_FALSE(parse_complete_callback_called); // Main app callback should not be called
    EXPECT_TRUE(sub_callback_called); // Subcommand callback should be called
    EXPECT_EQ(val, 10);
}

TEST(AppParseCompleteCallbackTest, DashDashWithSubcommandAndMainAppCallbackCalled) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });

    auto sub = app.add_subcommand("sub", "A subcommand");
    sub->parse_complete_callback([&]() { sub_callback_called = true; });

    int val = 0;
    app.add_option("--val", val);

    // "--" for the main app, subcommand is not active
    parse_args(app, {"--val", "10", "--"});

    EXPECT_TRUE(parse_complete_callback_called); // Main app callback should be called
    EXPECT_FALSE(sub_callback_called); // Subcommand callback should not be called
    EXPECT_EQ(val, 10);
}

TEST(AppParseCompleteCallbackTest, DashDashWithPositionalArgumentsAfter) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });

    std::vector<std::string> pos_args;
    app.add_option("pos", pos_args)->expected(-1);

    parse_args(app, {"--", "arg1", "arg2"});

    EXPECT_TRUE(parse_complete_callback_called);
    ASSERT_EQ(pos_args.size(), 2);
    EXPECT_EQ(pos_args[0], "arg1");
    EXPECT_EQ(pos_args[1], "arg2");
}

TEST(AppParseCompleteCallbackTest, DashDashWithMixedOptionsAndPositionalArguments) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });

    int opt_val = 0;
    app.add_option("--opt", opt_val);

    std::vector<std::string> pos_args;
    app.add_option("pos", pos_args)->expected(-1);

    parse_args(app, {"--opt", "100", "--", "arg1", "arg2"});

    EXPECT_TRUE(parse_complete_callback_called);
    EXPECT_EQ(opt_val, 100);
    ASSERT_EQ(pos_args.size(), 2);
    EXPECT_EQ(pos_args[0], "arg1");
    EXPECT_EQ(pos_args[1], "arg2");
}

TEST(AppParseCompleteCallbackTest, DashDashEmptyArgsList) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });

    // Parsing an empty list should not trigger the callback
    parse_args(app, {});

    EXPECT_FALSE(parse_complete_callback_called);
}

TEST(AppParseCompleteCallbackTest, DashDashOnly) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });

    parse_args(app, {"--"});

    EXPECT_TRUE(parse_complete_callback_called);
}

TEST(AppParseCompleteCallbackTest, DashDashMultipleTimesIgnored) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });

    int val = 0;
    app.add_option("--val", val);

    // Only the first "--" should be processed for the callback
    parse_args(app, {"--val", "10", "--", "--", "extra"});

    EXPECT_TRUE(parse_complete_callback_called);
    EXPECT_EQ(val, 10);
    // "extra" should be treated as a positional argument if one is defined,
    // otherwise it might be ignored or cause an error depending on app config.
    // For this test, we just care about the callback.
}

// Test that other callbacks are also processed when parse_complete_callback is triggered
TEST(AppParseCompleteCallbackTest, OtherCallbacksProcessed) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });
    app.callback([&]() { global_callback_called = true; }); // App's run_callback

    int opt_val = 0;
    auto opt = app.add_option("--opt", opt_val);
    // Use the public `check` method for option callbacks, or `preparse_callback`
    // `callback` on Option is protected.
    opt->check([&](const std::string& /*s*/) { option_callback_called = true; return ""; });

    std::string pos_val;
    auto pos = app.add_option("pos", pos_val);
    pos->check([&](const std::string& /*s*/) { positional_callback_called = true; return ""; });

    parse_args(app, {"--opt", "10", "my_pos_arg", "--"});

    EXPECT_TRUE(parse_complete_callback_called);
    EXPECT_TRUE(global_callback_called);
    EXPECT_TRUE(option_callback_called);
    EXPECT_TRUE(positional_callback_called);

    EXPECT_EQ(opt_val, 10);
    EXPECT_EQ(pos_val, "my_pos_arg");
}

// Test with a subcommand that also has a parse_complete_callback
TEST(AppParseCompleteCallbackTest, SubcommandWithItsOwnParseCompleteCallback) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });

    auto sub = app.add_subcommand("sub", "A subcommand");
    sub->parse_complete_callback([&]() { sub_callback_called = true; });
    sub->callback([&]() { global_callback_called = true; }); // Subcommand's run_callback

    int sub_opt_val = 0;
    sub->add_option("--sub-opt", sub_opt_val);

    parse_args(app, {"sub", "--sub-opt", "20", "--"});

    EXPECT_FALSE(parse_complete_callback_called); // Main app's callback not called
    EXPECT_TRUE(sub_callback_called); // Subcommand's parse_complete_callback called
    EXPECT_TRUE(global_callback_called); // Subcommand's run_callback called
    EXPECT_EQ(sub_opt_val, 20);
}

// Test requirements processing when -- is encountered
TEST(AppParseCompleteCallbackTest, RequirementsProcessedOnDashDash) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });

    int opt1 = 0, opt2 = 0;
    app.add_option("--opt1", opt1)->required();
    app.add_option("--opt2", opt2);

    // This should throw because --opt1 is required but not provided before --
    EXPECT_THROW(parse_args(app, {"--", "--opt2", "5"}), CLI::RequiredError);
    EXPECT_FALSE(parse_complete_callback_called); // Callback should not be called on error

    reset_callbacks();
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });
    // This should succeed
    parse_args(app, {"--opt1", "1", "--", "--opt2", "5"});
    EXPECT_TRUE(parse_complete_callback_called);
    EXPECT_EQ(opt1, 1);
    EXPECT_EQ(opt2, 0); // opt2 is after --, so it's treated as positional if allowed, or ignored.
                        // In this case, it's ignored as there's no positional option defined.
}

// Test that the callback is not called if parsing fails before -- is encountered
TEST(AppParseCompleteCallbackTest, CallbackNotCalledOnErrorBeforeDashDash) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });

    int val = 0;
    app.add_option("--val", val)->required();

    // Missing required option before --
    EXPECT_THROW(parse_args(app, {"--", "extra"}), CLI::RequiredError);
    EXPECT_FALSE(parse_complete_callback_called);

    reset_callbacks();
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });
    // Unknown option before --
    EXPECT_THROW(parse_args(app, {"--unknown", "--"}), CLI::ParseError);
    EXPECT_FALSE(parse_complete_callback_called);
}

// Test that the callback is called even if there are unparsed arguments after --
TEST(AppParseCompleteCallbackTest, CallbackCalledWithUnparsedAfterDashDash) {
    reset_callbacks();
    CLI::App app("Test App");
    app.parse_complete_callback([&]() { parse_complete_callback_called = true; });

    // No positional arguments defined, so "extra" will be unparsed
    parse_args(app, {"--", "extra"});

    EXPECT_TRUE(parse_complete_callback_called);
    // CLI11 by default ignores extra arguments after -- if no positional is defined
    // or if they don't match a positional. No error is thrown by default.
}