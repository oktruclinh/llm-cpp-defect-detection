#include <gtest/gtest.h>
#include "CLI/App.hpp"
#include "CLI/CLI.hpp"

TEST(AppTest, SectionCloseProcessing) {
    CLI::App app("Test app");
    
    // Test normal case: section close with configurable and parse complete callback
    bool callback_called = false;
    bool requirements_met = false;
    bool run_callback_called = false;
    
    app.set_config("--config");
    app.parse_complete_callback([&]() {
        callback_called = true;
    });
    
    app.add_option("--test", "test option");
    
    // Add a requirement to test _process_requirements
    auto* opt = app.add_option("--required", "required option");
    opt->required();
    
    // Test with section close
    std::vector<std::string> args = {"--", "--test", "value"};
    
    EXPECT_NO_THROW(app.parse(args));
    
    // Verify that the callbacks were called
    EXPECT_TRUE(callback_called);
    
    // Test with section close and no requirements
    CLI::App app2("Test app 2");
    app2.set_config("--config");
    app2.parse_complete_callback([&]() {
        callback_called = true;
    });
    app2.add_option("--test2", "test option 2");
    
    args = {"--", "--test2", "value"};
    EXPECT_NO_THROW(app2.parse(args));
    
    // Test with section close but no parse complete callback
    CLI::App app3("Test app 3");
    app3.set_config("--config");
    app3.add_option("--test3", "test option 3");
    
    args = {"--", "--test3", "value"};
    EXPECT_NO_THROW(app3.parse(args));
    
    // Test with section close but not configurable
    CLI::App app4("Test app 4");
    app4.add_option("--test4", "test option 4");
    
    args = {"--", "--test4", "value"};
    EXPECT_NO_THROW(app4.parse(args));
}

TEST(AppTest, SectionCloseEdgeCases) {
    CLI::App app("Test app");
    
    // Test empty args with section close
    std::vector<std::string> args = {"--"};
    EXPECT_NO_THROW(app.parse(args));
    
    // Test section close with no options
    CLI::App app2("Test app 2");
    app2.set_config("--config");
    app2.parse_complete_callback([]() {
        // Empty callback
    });
    
    args = {"--"};
    EXPECT_NO_THROW(app2.parse(args));
    
    // Test section close with multiple options
    CLI::App app3("Test app 3");
    app3.set_config("--config");
    app3.parse_complete_callback([]() {
        // Empty callback
    });
    
    app3.add_option("--opt1", "option 1");
    app3.add_option("--opt2", "option 2");
    
    args = {"--", "--opt1", "val1", "--opt2", "val2"};
    EXPECT_NO_THROW(app3.parse(args));
    
    // Test section close with required options (should still process callbacks)
    CLI::App app4("Test app 4");
    app4.set_config("--config");
    app4.parse_complete_callback([]() {
        // Empty callback
    });
    
    auto* req_opt = app4.add_option("--required", "required option");
    req_opt->required();
    app4.add_option("--optional", "optional option");
    
    args = {"--", "--required", "value", "--optional", "value2"};
    EXPECT_NO_THROW(app4.parse(args));
}

TEST(AppTest, SectionCloseErrorHandling) {
    CLI::App app("Test app");
    
    // Test section close with invalid option after it
    app.set_config("--config");
    app.parse_complete_callback([]() {
        // Empty callback
    });
    
    app.add_option("--test", "test option");
    
    // This should not throw as -- is a valid separator
    std::vector<std::string> args = {"--", "--invalid-option"};
    EXPECT_NO_THROW(app.parse(args));
    
    // Test section close with missing required option
    CLI::App app2("Test app 2");
    app2.set_config("--config");
    app2.parse_complete_callback([]() {
        // Empty callback
    });
    
    auto* req_opt = app2.add_option("--required", "required option");
    req_opt->required();
    
    // This should not throw because -- stops option parsing
    args = {"--", "--required", "value"};
    EXPECT_NO_THROW(app2.parse(args));
}