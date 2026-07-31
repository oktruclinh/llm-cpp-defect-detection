#include <gtest/gtest.h>

#include <CLI/CLI.hpp>

#include <sstream>
#include <string>
#include <vector>

namespace {

std::string make_config(const std::vector<std::string> &lines) {
    std::ostringstream out;
    for(const auto &line : lines) {
        out << line << '\n';
    }
    return out.str();
}

TEST(ConfigFlagValueSelectionTest, MultiOptionPolicyUsesConfiguredFlagValueForNamedValue) {
    CLI::App app{"test app"};

    std::string mode;
    auto *opt = app.add_option("--mode", mode, "mode");
    ASSERT_NE(opt, nullptr);
    opt->expected(0, 1);
    opt->multi_option_policy(CLI::MultiOptionPolicy::TakeLast);
    opt->transform(CLI::IsMember({"alpha", "beta"}));

    const std::string config_text = make_config({
        "mode=alpha",
        "mode=beta",
    });

    std::stringstream input(config_text);
    EXPECT_NO_THROW(app.parse_from_stream(input));

    EXPECT_EQ(mode, "beta");
}

TEST(ConfigFlagValueSelectionTest, EmptyValueDoesNotTriggerFlagValueMappingAndBehavesLikeBareFlag) {
    CLI::App app{"test app"};

    bool enabled{false};
    auto *opt = app.add_flag("--enable{true}", enabled, "enable");
    ASSERT_NE(opt, nullptr);

    const std::string config_text = make_config({
        "enable=",
    });

    std::stringstream input(config_text);
    EXPECT_NO_THROW(app.parse_from_stream(input));

    EXPECT_TRUE(enabled);
}

TEST(ConfigFlagValueSelectionTest, DistinctFlagValuesMapToCorrectTargets) {
    CLI::App app{"test app"};

    std::string level;
    auto *opt = app.add_option("--level", level, "level");
    ASSERT_NE(opt, nullptr);
    opt->expected(0, 1);
    opt->transform(CLI::IsMember({"low", "high"}));

    {
        std::stringstream input(make_config({"level=low"}));
        EXPECT_NO_THROW(app.parse_from_stream(input));
        EXPECT_EQ(level, "low");
    }

    CLI::App app2{"test app"};
    std::string level2;
    auto *opt2 = app2.add_option("--level", level2, "level");
    ASSERT_NE(opt2, nullptr);
    opt2->expected(0, 1);
    opt2->transform(CLI::IsMember({"low", "high"}));

    {
        std::stringstream input(make_config({"level=high"}));
        EXPECT_NO_THROW(app2.parse_from_stream(input));
        EXPECT_EQ(level2, "high");
    }
}

TEST(ConfigFlagValueSelectionTest, InvalidNamedFlagValueInConfigThrowsConversionError) {
    CLI::App app{"test app"};

    std::string mode;
    auto *opt = app.add_option("--mode", mode, "mode");
    ASSERT_NE(opt, nullptr);
    opt->expected(0, 1);
    opt->transform(CLI::IsMember({"alpha", "beta"}));

    std::stringstream input(make_config({"mode=gamma"}));

    EXPECT_THROW(app.parse_from_stream(input), CLI::ConversionError);
    EXPECT_TRUE(mode.empty());
}

TEST(ConfigFlagValueSelectionTest, FlagWithoutConfiguredNamesIgnoresValueMappingPathAndStillParses) {
    CLI::App app{"test app"};

    bool verbose{false};
    auto *opt = app.add_flag("--verbose", verbose, "verbose");
    ASSERT_NE(opt, nullptr);

    std::stringstream input(make_config({"verbose=1"}));

    EXPECT_NO_THROW(app.parse_from_stream(input));
    EXPECT_TRUE(verbose);
}

TEST(ConfigFlagValueSelectionTest, RepeatedNamedFlagValuesRespectTakeFirstPolicy) {
    CLI::App app{"test app"};

    std::string pick;
    auto *opt = app.add_option("--pick", pick, "pick");
    ASSERT_NE(opt, nullptr);
    opt->expected(0, 1);
    opt->multi_option_policy(CLI::MultiOptionPolicy::TakeFirst);
    opt->transform(CLI::IsMember({"first", "second"}));

    std::stringstream input(make_config({
        "pick=second",
        "pick=first",
    }));

    EXPECT_NO_THROW(app.parse_from_stream(input));

    EXPECT_EQ(pick, "second");
}

}  // namespace