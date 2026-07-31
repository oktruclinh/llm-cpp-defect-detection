#include <gtest/gtest.h>
#include <CLI/CLI.hpp>

#include <string>
#include <vector>

TEST(CLI11SectionCloseTest, DoubleDashTriggersImmediateCallbackForConfigurableApp) {
    CLI::App app{"test app"};

    std::vector<int> values;
    int callback_count = 0;

    app.configurable();
    app.add_option("-n,--num", values);
    app.callback([&]() {
        ++callback_count;
        // Capture state at callback time; this should contain only values
        // parsed before the section close token.
        values.push_back(1000 + static_cast<int>(values.size()));
    });

    std::vector<std::string> args{"-n", "1", "--", "-n", "2"};
    app.parse(args);

    ASSERT_EQ(1, callback_count);
    ASSERT_EQ(3u, values.size());
    EXPECT_EQ(1, values[0]);
    // If "--" did not trigger the callback immediately, this marker would be
    // based on size 2 instead of size 1.
    EXPECT_EQ(1001, values[1]);
    EXPECT_EQ(2, values[2]);
}

TEST(CLI11SectionCloseTest, DoubleDashDoesNotTriggerCallbackForNonConfigurableApp) {
    CLI::App app{"test app"};

    std::vector<int> values;
    int callback_count = 0;

    app.add_option("-n,--num", values);
    app.callback([&]() { ++callback_count; });

    std::vector<std::string> args{"-n", "1", "--", "-n", "2"};
    app.parse(args);

    EXPECT_EQ(1, callback_count);
    ASSERT_EQ(2u, values.size());
    EXPECT_EQ(1, values[0]);
    EXPECT_EQ(2, values[1]);
}

TEST(CLI11SectionCloseTest, DoubleDashWithoutRegisteredCallbackIsAcceptedForConfigurableApp) {
    CLI::App app{"test app"};

    std::vector<int> values;
    app.configurable();
    app.add_option("-n,--num", values);

    std::vector<std::string> args{"-n", "3", "--", "-n", "4"};

    EXPECT_NO_THROW(app.parse(args));
    ASSERT_EQ(2u, values.size());
    EXPECT_EQ(3, values[0]);
    EXPECT_EQ(4, values[1]);
}

TEST(CLI11SectionCloseTest, MultipleSectionClosesTriggerCallbackEachTimeForConfigurableApp) {
    CLI::App app{"test app"};

    std::vector<int> values;
    std::vector<size_t> observed_sizes;

    app.configurable();
    app.add_option("-n,--num", values);
    app.callback([&]() { observed_sizes.push_back(values.size()); });

    std::vector<std::string> args{"-n", "1", "--", "-n", "2", "--", "-n", "3"};
    app.parse(args);

    ASSERT_EQ(3u, observed_sizes.size());
    EXPECT_EQ(1u, observed_sizes[0]);
    EXPECT_EQ(2u, observed_sizes[1]);
    EXPECT_EQ(3u, observed_sizes[2]);

    ASSERT_EQ(3u, values.size());
    EXPECT_EQ(1, values[0]);
    EXPECT_EQ(2, values[1]);
    EXPECT_EQ(3, values[2]);
}

TEST(CLI11SectionCloseTest, RequirementsAreProcessedAtSectionClose) {
    CLI::App app{"test app"};

    int callback_count = 0;
    std::string required_value;

    app.configurable();
    app.callback([&]() { ++callback_count; });

    auto *opt = app.add_option("--req", required_value);
    opt->required();

    std::vector<std::string> args{"--"};
    EXPECT_THROW(app.parse(args), CLI::RequiredError);

    // The callback should not complete successfully when requirements fail
    // during section-close processing.
    EXPECT_EQ(0, callback_count);
}

TEST(CLI11SectionCloseTest, EmptySectionCloseOnlyStillRunsCallbackForConfigurableApp) {
    CLI::App app{"test app"};

    int callback_count = 0;

    app.configurable();
    app.callback([&]() { ++callback_count; });

    std::vector<std::string> args{"--"};
    app.parse(args);

    EXPECT_EQ(1, callback_count);
}