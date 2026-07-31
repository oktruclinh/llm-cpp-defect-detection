#include <gtest/gtest.h>
#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>

// Helper function to simulate parsing arguments
// This allows us to test the internal state changes after parsing
void parse_args(CLI::App& app, const std::vector<std::string>& args) {
    try {
        app.parse(args);
    } catch (const CLI::ParseError& e) {
        // We might expect parse errors in some tests, so catch them
        // and rethrow if not expected, or just ignore for now.
        // For these tests, we are primarily interested in the state after parsing.
    }
}

// Test fixture for CLI::App to set up common objects
class AppTest : public ::testing::Test {
protected:
    CLI::App app{"Test App"};
    bool callback_called = false;
    bool configurable_callback_called = false;

    void SetUp() override {
        callback_called = false;
        configurable_callback_called = false;
        app.callback([this]() { callback_called = true; });
    }
};

// Test case 1: Basic functionality - -- terminates parsing and triggers callback
TEST_F(AppTest, DashDashTerminatesParsingAndCallsCallback) {
    app.add_option("--opt", "An option");
    app.add_flag("--flag", "A flag");

    std::vector<std::string> args = {"--opt", "value", "--", "--another_arg", "foo"};
    parse_args(app, args);

    // Expect callback to be called because -- was encountered
    EXPECT_TRUE(callback_called);
    // The arguments after -- should not be parsed by CLI11
    EXPECT_EQ(app.remaining_for_help().size(), 2);
    EXPECT_EQ(app.remaining_for_help()[0], "--another_arg");
    EXPECT_EQ(app.remaining_for_help()[1], "foo");
}

// Test case 2: -- at the beginning of arguments
TEST_F(AppTest, DashDashAtBeginningCallsCallbackImmediately) {
    app.add_option("--opt", "An option");

    std::vector<std::string> args = {"--", "--opt", "value"};
    parse_args(app, args);

    EXPECT_TRUE(callback_called);
    EXPECT_EQ(app.remaining_for_help().size(), 2);
    EXPECT_EQ(app.remaining_for_help()[0], "--opt");
    EXPECT_EQ(app.remaining_for_help()[1], "value");
}

// Test case 3: -- as the only argument
TEST_F(AppTest, DashDashOnlyArgumentCallsCallback) {
    std::vector<std::string> args = {"--"};
    parse_args(app, args);

    EXPECT_TRUE(callback_called);
    EXPECT_TRUE(app.remaining_for_help().empty());
}

// Test case 4: No -- in arguments, callback should still be called at the end of parsing
TEST_F(AppTest, NoDashDashStillCallsCallbackAtEndOfParsing) {
    app.add_flag("--flag", "A flag");
    std::vector<std::string> args = {"--flag"};
    parse_args(app, args);

    EXPECT_TRUE(callback_called);
    EXPECT_TRUE(app.remaining_for_help().empty());
}

// Test case 5: Subcommand with --
TEST_F(AppTest, SubcommandWithDashDash) {
    CLI::App* sub = app.add_subcommand("sub", "A subcommand");
    sub->add_flag("--sflag", "Subcommand flag");

    std::vector<std::string> args = {"sub", "--sflag", "--", "extra_arg"};
    parse_args(app, args);

    EXPECT_TRUE(callback_called); // Main app callback should still be called
    EXPECT_TRUE(sub->parsed());
    EXPECT_EQ(sub->remaining_for_help().size(), 1);
    EXPECT_EQ(sub->remaining_for_help()[0], "extra_arg");
}

// Test case 6: Nested subcommands with --
TEST_F(AppTest, NestedSubcommandWithDashDash) {
    CLI::App* sub1 = app.add_subcommand("sub1", "Subcommand 1");
    CLI::App* sub2 = sub1->add_subcommand("sub2", "Subcommand 2");
    sub2->add_flag("--s2flag", "Subcommand 2 flag");

    std::vector<std::string> args = {"sub1", "sub2", "--s2flag", "--", "extra_arg1", "extra_arg2"};
    parse_args(app, args);

    EXPECT_TRUE(callback_called);
    EXPECT_TRUE(sub1->parsed());
    EXPECT_TRUE(sub2->parsed());
    EXPECT_EQ(sub2->remaining_for_help().size(), 2);
    EXPECT_EQ(sub2->remaining_for_help()[0], "extra_arg1");
    EXPECT_EQ(sub2->remaining_for_help()[1], "extra_arg2");
}

// Test case 7: Callback not set, -- should still terminate parsing
TEST_F(AppTest, DashDashTerminatesParsingWithoutCallback) {
    CLI::App no_callback_app{"No Callback App"};
    no_callback_app.add_option("--opt", "An option");

    std::vector<std::string> args = {"--opt", "value", "--", "remaining"};
    parse_args(no_callback_app, args);

    // Callback_called is from the fixture, not relevant here.
    // The important part is that remaining arguments are captured.
    EXPECT_EQ(no_callback_app.remaining_for_help().size(), 1);
    EXPECT_EQ(no_callback_app.remaining_for_help()[0], "remaining");
}

// Test case 8: Check configurable_ flag interaction
// The focal function checks `configurable_ && parse_complete_callback_`
// `configurable_` is true by default for CLI::App.
// `parse_complete_callback_` is true if a callback is set.
TEST_F(AppTest, ConfigurableAndCallbackTrue) {
    // Setup already ensures configurable_ is true and a callback is set
    app.add_flag("--test-flag");
    std::vector<std::string> args = {"--test-flag", "--", "extra"};
    parse_args(app, args);
    EXPECT_TRUE(callback_called);
    EXPECT_TRUE(app.remaining_for_help().size() == 1);
}

// Test case 9: configurable_ is false, callback should not be called by --
// This is a bit tricky as `configurable_` is internal and not directly exposed.
// However, we can infer its behavior. If an app is not configurable, it might
// not process callbacks in the same way.
// For CLI11, `configurable_` is generally true for the main app and subcommands.
// The primary way `configurable_` becomes false is if the app is a "group" or
// if it's explicitly set to false (which is not a public API).
// Let's assume for this test that `configurable_` is always true for a standard App.
// The `parse_complete_callback_` is the more relevant part here.

// Test case 10: No callback set (parse_complete_callback_ is false)
TEST_F(AppTest, DashDashWithoutCallbackSet) {
    CLI::App local_app{"Local App No Callback"}; // No callback set
    local_app.add_option("--opt", "An option");

    std::vector<std::string> args = {"--opt", "value", "--", "remaining"};
    parse_args(local_app, args);

    // The internal `_process_callbacks()` and `run_callback()` would not be called
    // if `parse_complete_callback_` is false.
    // We can only observe the side effect of parsing termination.
    EXPECT_EQ(local_app.remaining_for_help().size(), 1);
    EXPECT_EQ(local_app.remaining_for_help()[0], "remaining");
}

// Test case 11: Callback with requirements
TEST_F(AppTest, DashDashWithRequiredOptionsAndCallback) {
    int val = 0;
    app.add_option("-v,--value", val, "A required value")->required();

    // If -- is encountered before a required option is met, it should still trigger
    // the callback and then potentially fail on requirements check.
    // However, CLI11's parsing logic usually checks requirements at the very end.
    // The focal function is called when `item.name == "--"`. At this point,
    // `_process_requirements()` is called *before* `run_callback()`.
    // So, if a required option is missing, it should throw *before* the user callback.

    std::vector<std::string> args = {"--", "extra"}; // Missing required option
    
    // Expect a ParseError due to missing required option
    EXPECT_THROW(app.parse(args), CLI::RequiredError);
    
    // The callback should NOT be called if a RequiredError is thrown during _process_requirements()
    EXPECT_FALSE(callback_called);

    // Now, with the required option provided
    callback_called = false; // Reset for next test
    args = {"--value", "10", "--", "extra"};
    parse_args(app, args);
    EXPECT_TRUE(callback_called);
    EXPECT_EQ(val, 10);
    EXPECT_EQ(app.remaining_for_help().size(), 1);
}

// Test case 12: Callback with requirements, but -- is after all requirements are met
TEST_F(AppTest, DashDashAfterAllRequirementsMet) {
    int val = 0;
    app.add_option("-v,--value", val, "A required value")->required();

    std::vector<std::string> args = {"--value", "10", "--", "extra_arg"};
    parse_args(app, args);

    EXPECT_TRUE(callback_called);
    EXPECT_EQ(val, 10);
    EXPECT_EQ(app.remaining_for_help().size(), 1);
    EXPECT_EQ(app.remaining_for_help()[0], "extra_arg");
}

// Test case 13: Empty arguments list
TEST_F(AppTest, EmptyArgumentsList) {
    std::vector<std::string> args = {};
    parse_args(app, args);

    EXPECT_TRUE(callback_called); // Callback should still be called at the end of parsing
    EXPECT_TRUE(app.remaining_for_help().empty());
}

// Test case 14: Multiple -- in arguments (only the first one should terminate parsing)
TEST_F(AppTest, MultipleDashDashOnlyFirstTerminatesParsing) {
    app.add_option("--opt", "An option");
    std::vector<std::string> args = {"--opt", "value", "--", "arg1", "--", "arg2"};
    parse_args(app, args);

    EXPECT_TRUE(callback_called);
    EXPECT_EQ(app.remaining_for_help().size(), 3); // "arg1", "--", "arg2"
    EXPECT_EQ(app.remaining_for_help()[0], "arg1");
    EXPECT_EQ(app.remaining_for_help()[1], "--");
    EXPECT_EQ(app.remaining_for_help()[2], "arg2");
}

// Test case 15: Callback with a subcommand that has a callback
TEST_F(AppTest, SubcommandCallbackAndMainCallbackWithDashDash) {
    CLI::App* sub = app.add_subcommand("sub", "A subcommand");
    bool sub_callback_called = false;
    sub->callback([&]() { sub_callback_called = true; });

    std::vector<std::string> args = {"sub", "--", "extra"};
    parse_args(app, args);

    EXPECT_TRUE(callback_called); // Main app callback
    EXPECT_TRUE(sub_callback_called); // Subcommand callback
    EXPECT_TRUE(sub->parsed());
    EXPECT_EQ(sub->remaining_for_help().size(), 1);
}

// Test case 16: Callback with a subcommand that has a callback, and -- is before subcommand
TEST_F(AppTest, DashDashBeforeSubcommand) {
    CLI::App* sub = app.add_subcommand("sub", "A subcommand");
    bool sub_callback_called = false;
    sub->callback([&]() { sub_callback_called = true; });

    std::vector<std::string> args = {"--", "sub", "extra"};
    parse_args(app, args);

    EXPECT_TRUE(callback_called); // Main app callback
    EXPECT_FALSE(sub_callback_called); // Subcommand callback should NOT be called as 'sub' is a remaining arg
    EXPECT_FALSE(sub->parsed());
    EXPECT_EQ(app.remaining_for_help().size(), 2);
    EXPECT_EQ(app.remaining_for_help()[0], "sub");
    EXPECT_EQ(app.remaining_for_help()[1], "extra");
}