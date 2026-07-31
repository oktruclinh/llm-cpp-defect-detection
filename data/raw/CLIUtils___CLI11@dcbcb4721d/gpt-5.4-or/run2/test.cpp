#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <vector>

#include "CLI/CLI.hpp"

namespace {

TEST(ConfigFlagValueRoutingTest, FlagLikeOptionUsesConfiguredFlagValueFromConfig) {
    CLI::App app{"test app"};

    bool flag = false;
    auto *opt = app.add_flag("--flag", flag);
    opt->default_str("OFF");
    opt->expected(0, 1);

    app.config_formatter(std::make_shared<CLI::ConfigINI>());

    std::istringstream config_stream{"flag=ON\n"};
    EXPECT_NO_THROW(app.parse_from_stream(config_stream));

    EXPECT_TRUE(flag);
    EXPECT_EQ(opt->count(), 1u);
    EXPECT_EQ(opt->results().size(), 1u);
    EXPECT_EQ(opt->results().at(0), "true");
}

TEST(ConfigFlagValueRoutingTest, EmptyConfigValueDoesNotInvokeFlagValueMapping) {
    CLI::App app{"test app"};

    bool flag = false;
    auto *opt = app.add_flag("--flag", flag);
    opt->default_str("OFF");
    opt->expected(0, 1);

    app.config_formatter(std::make_shared<CLI::ConfigINI>());

    std::istringstream config_stream{"flag=\n"};
    EXPECT_NO_THROW(app.parse_from_stream(config_stream));

    EXPECT_FALSE(flag);
    EXPECT_EQ(opt->count(), 1u);
    ASSERT_EQ(opt->results().size(), 1u);
    EXPECT_EQ(opt->results().at(0), "");
}

TEST(ConfigFlagValueRoutingTest, RegularOptionWithValueIsNotTreatedAsFlagValueOption) {
    CLI::App app{"test app"};

    std::string value;
    auto *opt = app.add_option("--name", value);

    app.config_formatter(std::make_shared<CLI::ConfigINI>());

    std::istringstream config_stream{"name=alpha\n"};
    EXPECT_NO_THROW(app.parse_from_stream(config_stream));

    EXPECT_EQ(value, "alpha");
    EXPECT_EQ(opt->count(), 1u);
    ASSERT_EQ(opt->results().size(), 1u);
    EXPECT_EQ(opt->results().at(0), "alpha");
}

TEST(ConfigFlagValueRoutingTest, DifferentConfiguredFlagValueMapsToFalseForBooleanFlag) {
    CLI::App app{"test app"};

    bool flag = true;
    auto *opt = app.add_flag("--flag", flag);
    opt->default_str("ON");
    opt->expected(0, 1);

    app.config_formatter(std::make_shared<CLI::ConfigINI>());

    std::istringstream config_stream{"flag=OFF\n"};
    EXPECT_NO_THROW(app.parse_from_stream(config_stream));

    EXPECT_FALSE(flag);
    EXPECT_EQ(opt->count(), 1u);
    ASSERT_EQ(opt->results().size(), 1u);
    EXPECT_EQ(opt->results().at(0), "false");
}

TEST(ConfigFlagValueRoutingTest, NumericFlagOptionAcceptsConfigValueAsRegularResult) {
    CLI::App app{"test app"};

    int level = 0;
    auto *opt = app.add_flag("--level", level);
    opt->default_str("0");

    app.config_formatter(std::make_shared<CLI::ConfigINI>());

    std::istringstream config_stream{"level=1\n"};
    EXPECT_NO_THROW(app.parse_from_stream(config_stream));

    EXPECT_EQ(level, 1);
    EXPECT_EQ(opt->count(), 1u);
    ASSERT_EQ(opt->results().size(), 1u);
    EXPECT_EQ(opt->results().at(0), "1");
}

TEST(ConfigFlagValueRoutingTest, UnknownConfiguredFlagValueProducesValidationError) {
    CLI::App app{"test app"};

    bool flag = false;
    auto *opt = app.add_flag("--flag", flag);
    opt->default_str("OFF");
    opt->expected(0, 1);

    app.config_formatter(std::make_shared<CLI::ConfigINI>());

    std::istringstream config_stream{"flag=maybe\n"};
    EXPECT_THROW(app.parse_from_stream(config_stream), CLI::ValidationError);
}

}  // namespace