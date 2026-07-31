#include <gtest/gtest.h>

#include <CLI/CLI.hpp>
#include <sstream>
#include <string>

namespace {

std::string to_config(const CLI::App &app) {
    std::stringstream out;
    app.config_to_str(true, true, out);
    return out.str();
}

TEST(ConfigFlagValueMappingTest, FlagWithDefaultFlagValueUsesMappedNameInConfigOutput) {
    CLI::App app{"test app"};

    bool enabled{false};
    auto *opt = app.add_flag("--mode", enabled);
    opt->default_flag_values({"on"});

    app.parse(std::vector<std::string>{"prog", "--mode"});

    const std::string cfg = to_config(app);

    EXPECT_NE(cfg.find("mode=on"), std::string::npos) << cfg;
    EXPECT_EQ(cfg.find("mode=true"), std::string::npos) << cfg;
    EXPECT_TRUE(enabled);
}

TEST(ConfigFlagValueMappingTest, FlagWithExplicitFalseDefaultFlagValueUsesMappedNameInConfigOutput) {
    CLI::App app{"test app"};

    bool enabled{true};
    auto *opt = app.add_flag("--mode", enabled);
    opt->default_flag_values({"off"});

    app.parse(std::vector<std::string>{"prog", "--mode"});

    const std::string cfg = to_config(app);

    EXPECT_NE(cfg.find("mode=off"), std::string::npos) << cfg;
    EXPECT_EQ(cfg.find("mode=true"), std::string::npos) << cfg;
    EXPECT_TRUE(enabled);
}

TEST(ConfigFlagValueMappingTest, MultiFlagCountMapsPositiveCountUsingFlagValueNames) {
    CLI::App app{"test app"};

    int count{0};
    auto *opt = app.add_flag("-v,--verbose", count);
    opt->multi_option_policy(CLI::MultiOptionPolicy::Sum);
    opt->default_flag_values({"set"});

    app.parse(std::vector<std::string>{"prog", "-v"});

    const std::string cfg = to_config(app);

    EXPECT_NE(cfg.find("verbose=set"), std::string::npos) << cfg;
    EXPECT_EQ(cfg.find("verbose=1"), std::string::npos) << cfg;
    EXPECT_EQ(count, 1);
}

TEST(ConfigFlagValueMappingTest, FlagWithoutFlagValueNamesDoesNotAttemptMappingAndKeepsBooleanText) {
    CLI::App app{"test app"};

    bool enabled{false};
    app.add_flag("--debug", enabled);

    app.parse(std::vector<std::string>{"prog", "--debug"});

    const std::string cfg = to_config(app);

    EXPECT_NE(cfg.find("debug=true"), std::string::npos) << cfg;
    EXPECT_TRUE(enabled);
}

TEST(ConfigFlagValueMappingTest, UnsetFlagDoesNotEmitMappedOrUnmappedValue) {
    CLI::App app{"test app"};

    bool enabled{false};
    auto *opt = app.add_flag("--feature", enabled);
    opt->default_flag_values({"enabled"});

    app.parse(std::vector<std::string>{"prog"});

    const std::string cfg = to_config(app);

    EXPECT_EQ(cfg.find("feature=enabled"), std::string::npos) << cfg;
    EXPECT_EQ(cfg.find("feature=true"), std::string::npos) << cfg;
    EXPECT_FALSE(enabled);
}

TEST(ConfigFlagValueMappingTest, EmptyFlagValueNameDoesNotReplaceExistingValue) {
    CLI::App app{"test app"};

    bool enabled{false};
    auto *opt = app.add_flag("--flag", enabled);
    opt->default_flag_values({""});

    app.parse(std::vector<std::string>{"prog", "--flag"});

    const std::string cfg = to_config(app);

    // The focal logic only remaps when the current textual value is non-empty.
    // An empty configured flag value name should therefore not erase output.
    EXPECT_NE(cfg.find("flag=true"), std::string::npos) << cfg;
    EXPECT_EQ(cfg.find("flag="), std::string::npos) << cfg;
    EXPECT_TRUE(enabled);
}

TEST(ConfigFlagValueMappingTest, NegativeFlagUsesConfiguredFalseValueName) {
    CLI::App app{"test app"};

    bool enabled{true};
    auto *opt = app.add_flag("--feature,!--no-feature", enabled);
    opt->disable_flag_override();
    opt->default_flag_values({"enabled", "disabled"});

    app.parse(std::vector<std::string>{"prog", "--no-feature"});

    const std::string cfg = to_config(app);

    EXPECT_NE(cfg.find("feature=disabled"), std::string::npos) << cfg;
    EXPECT_EQ(cfg.find("feature=false"), std::string::npos) << cfg;
    EXPECT_FALSE(enabled);
}

}  // namespace