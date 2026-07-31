#include <gtest/gtest.h>

#include <CLI/CLI.hpp>

#include <string>
#include <vector>

TEST(CLI11SectionCloseTest, SectionCloseRunsCallbackWhenConfigurableAndCallbackExists) {
    CLI::App app{"test app"};

    int callback_count = 0;
    bool flag = false;

    app.configurable(true);
    app.callback([&]() { ++callback_count; });
    app.add_flag("--flag", flag);

    EXPECT_NO_THROW(app.parse(std::vector<std::string>{"--flag", "--"}));

    EXPECT_TRUE(flag);
    EXPECT_EQ(callback_count, 1);
}

TEST(CLI11SectionCloseTest, SectionCloseDoesNotRunCallbackWithoutSectionCloseToken) {
    CLI::App app{"test app"};

    int callback_count = 0;
    bool flag = false;

    app.configurable(true);
    app.callback([&]() { ++callback_count; });
    app.add_flag("--flag", flag);

    EXPECT_NO_THROW(app.parse(std::vector<std::string>{"--flag"}));

    EXPECT_TRUE(flag);
    EXPECT_EQ(callback_count, 0);
}

TEST(CLI11SectionCloseTest, SectionCloseDoesNotRunCallbackWhenNotConfigurable) {
    CLI::App app{"test app"};

    int callback_count = 0;
    bool flag = false;

    app.configurable(false);
    app.callback([&]() { ++callback_count; });
    app.add_flag("--flag", flag);

    EXPECT_NO_THROW(app.parse(std::vector<std::string>{"--flag", "--"}));

    EXPECT_TRUE(flag);
    EXPECT_EQ(callback_count, 0);
}

TEST(CLI11SectionCloseTest, SectionCloseProcessesRequirementsBeforeRunningCallback) {
    CLI::App app{"test app"};

    int callback_count = 0;
    bool trigger = false;
    bool required_flag = false;

    app.configurable(true);
    app.callback([&]() { ++callback_count; });

    app.add_flag("--trigger", trigger);
    auto *req = app.add_flag("--required", required_flag);
    req->required();

    try {
        app.parse(std::vector<std::string>{"--trigger", "--"});
        FAIL() << "Expected parse to throw due to missing required option at section close";
    } catch(const CLI::RequiredError &) {
        // expected
    } catch(const CLI::ParseError &e) {
        FAIL() << "Expected RequiredError, got different ParseError: " << e.what();
    }

    EXPECT_TRUE(trigger);
    EXPECT_FALSE(required_flag);
    EXPECT_EQ(callback_count, 0);
}

TEST(CLI11SectionCloseTest, EmptySectionCloseStillRunsCallbackWhenEnabled) {
    CLI::App app{"test app"};

    int callback_count = 0;

    app.configurable(true);
    app.callback([&]() { ++callback_count; });

    EXPECT_NO_THROW(app.parse(std::vector<std::string>{"--"}));

    EXPECT_EQ(callback_count, 1);
}

TEST(CLI11SectionCloseTest, MultipleSectionClosesRunCallbackEachTimeEncountered) {
    CLI::App app{"test app"};

    int callback_count = 0;

    app.configurable(true);
    app.callback([&]() { ++callback_count; });

    EXPECT_NO_THROW(app.parse(std::vector<std::string>{"--", "--"}));

    EXPECT_EQ(callback_count, 2);
}

TEST(CLI11SectionCloseTest, CallbackCanObserveParsedValuesBeforeSectionClose) {
    CLI::App app{"test app"};

    int callback_count = 0;
    std::string captured_value;
    std::string value;

    app.configurable(true);
    app.add_option("--name", value);
    app.callback([&]() {
        ++callback_count;
        captured_value = value;
    });

    EXPECT_NO_THROW(app.parse(std::vector<std::string>{"--name", "alice", "--"}));

    EXPECT_EQ(value, "alice");
    EXPECT_EQ(captured_value, "alice");
    EXPECT_EQ(callback_count, 1);
}