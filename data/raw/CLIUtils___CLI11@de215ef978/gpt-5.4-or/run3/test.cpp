#include <gtest/gtest.h>
#include <CLI/CLI.hpp>

#include <stdexcept>
#include <string>
#include <vector>

TEST(AppSectionCloseTest, DoubleDashTriggersCallbackAndParsesFollowingPositionalWhenConfigurable) {
    CLI::App app{"test app"};
    app.configurable();

    int callback_count = 0;
    app.parse_complete_callback([&]() { ++callback_count; });

    std::string first;
    std::string second;
    app.add_option("first", first);
    app.add_option("second", second);

    std::vector<std::string> args{"prog", "alpha", "--", "beta"};

    EXPECT_NO_THROW(app.parse(args));
    EXPECT_EQ("alpha", first);
    EXPECT_EQ("beta", second);
    EXPECT_EQ(1, callback_count);
}

TEST(AppSectionCloseTest, DoubleDashDoesNotTriggerParseCompleteCallbackWhenNotConfigurable) {
    CLI::App app{"test app"};

    int callback_count = 0;
    app.parse_complete_callback([&]() { ++callback_count; });

    std::string first;
    std::string second;
    app.add_option("first", first);
    app.add_option("second", second);

    std::vector<std::string> args{"prog", "alpha", "--", "beta"};

    EXPECT_NO_THROW(app.parse(args));
    EXPECT_EQ("alpha", first);
    EXPECT_EQ("beta", second);
    EXPECT_EQ(0, callback_count);
}

TEST(AppSectionCloseTest, DoubleDashWithoutRegisteredParseCompleteCallbackStillParsesSuccessfully) {
    CLI::App app{"test app"};
    app.configurable();

    std::string first;
    std::string second;
    app.add_option("first", first);
    app.add_option("second", second);

    std::vector<std::string> args{"prog", "alpha", "--", "beta"};

    EXPECT_NO_THROW(app.parse(args));
    EXPECT_EQ("alpha", first);
    EXPECT_EQ("beta", second);
}

TEST(AppSectionCloseTest, DoubleDashRunsCallbackEarlyEnoughToObservePreSectionState) {
    CLI::App app{"test app"};
    app.configurable();

    std::string first;
    std::string second;
    app.add_option("first", first);
    app.add_option("second", second);

    std::string observed_first;
    std::string observed_second = "unset";
    int callback_count = 0;

    app.parse_complete_callback([&]() {
        ++callback_count;
        observed_first = first;
        observed_second = second;
    });

    std::vector<std::string> args{"prog", "before", "--", "after"};

    EXPECT_NO_THROW(app.parse(args));

    EXPECT_EQ(1, callback_count);
    EXPECT_EQ("before", observed_first);
    EXPECT_TRUE(observed_second.empty());
    EXPECT_EQ("before", first);
    EXPECT_EQ("after", second);
}

TEST(AppSectionCloseTest, DoubleDashProcessesRequirementsAtSectionClose) {
    CLI::App app{"test app"};
    app.configurable();

    bool callback_ran = false;
    app.parse_complete_callback([&]() { callback_ran = true; });

    std::string required_value;
    auto *opt = app.add_option("value", required_value);
    opt->required();

    std::vector<std::string> args{"prog", "--"};

    EXPECT_THROW(app.parse(args), CLI::RequiredError);
    EXPECT_FALSE(callback_ran);
}

TEST(AppSectionCloseTest, ParseCompleteCallbackExceptionPropagatesAtSectionClose) {
    CLI::App app{"test app"};
    app.configurable();

    app.parse_complete_callback([]() { throw std::runtime_error("callback failure"); });

    std::string first;
    std::string second;
    app.add_option("first", first);
    app.add_option("second", second);

    std::vector<std::string> args{"prog", "alpha", "--", "beta"};

    EXPECT_THROW(app.parse(args), std::runtime_error);
}

TEST(AppSectionCloseTest, NoDoubleDashDoesNotInvokeParseCompleteCallbackDuringParse) {
    CLI::App app{"test app"};
    app.configurable();

    int callback_count = 0;
    app.parse_complete_callback([&]() { ++callback_count; });

    std::string first;
    std::string second;
    app.add_option("first", first);
    app.add_option("second", second);

    std::vector<std::string> args{"prog", "alpha", "beta"};

    EXPECT_NO_THROW(app.parse(args));
    EXPECT_EQ("alpha", first);
    EXPECT_EQ("beta", second);
    EXPECT_EQ(0, callback_count);
}

TEST(AppSectionCloseTest, LoneDoubleDashTriggersCallbackWhenConfigurableAndCallbackPresent) {
    CLI::App app{"test app"};
    app.configurable();

    int callback_count = 0;
    app.parse_complete_callback([&]() { ++callback_count; });

    std::vector<std::string> args{"prog", "--"};

    EXPECT_NO_THROW(app.parse(args));
    EXPECT_EQ(1, callback_count);
}