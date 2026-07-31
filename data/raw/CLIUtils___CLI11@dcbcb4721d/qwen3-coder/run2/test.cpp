#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include "CLI/impl/Config_inl.hpp"

TEST(ConfigInlineTest, FlagValueProcessing) {
    CLI::App app{"Test app"};
    
    // Test case 1: Normal flag processing with non-empty value
    auto opt1 = app.add_flag("--flag1", "Flag option");
    opt1->set_flag_value("--flag1", "true");
    
    // Test case 2: Empty value should not process flag
    auto opt2 = app.add_flag("--flag2", "Flag option");
    opt2->set_flag_value("--flag2", "false");
    
    // Test case 3: Flag with empty flag names
    auto opt3 = app.add_flag("--flag3", "Flag option");
    opt3->set_flag_value("", "true");
    
    // Test case 4: Multiple flag names
    auto opt4 = app.add_flag("--flag4", "--flag4-alt", "Flag option");
    opt4->set_flag_value("--flag4", "true");
    
    // Test case 5: No flag names at all
    auto opt5 = app.add_flag("--flag5", "Flag option");
    opt5->set_flag_value("", "false");
    
    // Verify that the flag values are properly set
    EXPECT_TRUE(opt1->get_fnames().size() > 0);
    EXPECT_TRUE(opt2->get_fnames().size() > 0);
    EXPECT_TRUE(opt3->get_fnames().size() > 0);
    EXPECT_TRUE(opt4->get_fnames().size() > 0);
    EXPECT_TRUE(opt5->get_fnames().size() > 0);
    
    // Test the specific behavior: when value is not empty and flag names exist
    std::string test_value = "test_value";
    std::string result1 = opt1->get_flag_value("--flag1", test_value);
    EXPECT_EQ(result1, "true"); // Should return the flag value, not the original
    
    std::string result2 = opt2->get_flag_value("--flag2", test_value);
    EXPECT_EQ(result2, "false"); // Should return the flag value, not the original
    
    // Test with empty value - should not process flag
    std::string empty_result = opt3->get_flag_value("", test_value);
    EXPECT_EQ(empty_result, test_value); // Should return original value when no flag processing
    
    // Test with empty flag name but non-empty value
    std::string result4 = opt4->get_flag_value("", test_value);
    EXPECT_EQ(result4, test_value); // Should return original value when no flag name
    
    // Test with empty flag name and empty value
    std::string result5 = opt5->get_flag_value("", "");
    EXPECT_EQ(result5, ""); // Should return empty string when both are empty
}

TEST(ConfigInlineTest, EdgeCases) {
    CLI::App app{"Test app"};
    
    // Test with empty flag names vector
    auto opt = app.add_flag("--test", "Test flag");
    // Manually clear flag names to test edge case
    auto& fnames = const_cast<std::vector<std::string>&>(opt->get_fnames());
    fnames.clear();
    
    std::string test_value = "original";
    std::string result = opt->get_flag_value("--test", test_value);
    
    // When flag names are empty, should return original value
    EXPECT_EQ(result, test_value);
    
    // Test with very long flag name
    std::string long_flag = std::string(1000, 'a');
    std::string long_result = opt->get_flag_value(long_flag, test_value);
    EXPECT_EQ(long_result, test_value);
    
    // Test with special characters in flag name
    std::string special_flag = "flag-with-special-chars_123";
    std::string special_result = opt->get_flag_value(special_flag, test_value);
    EXPECT_EQ(special_result, test_value);
    
    // Test with empty string as flag name
    std::string empty_flag = "";
    std::string empty_result = opt->get_flag_value(empty_flag, test_value);
    EXPECT_EQ(empty_result, test_value);
}

TEST(ConfigInlineTest, NormalFlagProcessing) {
    CLI::App app{"Test app"};
    
    // Create a flag with a known flag value
    auto opt = app.add_flag("--verbose", "Verbose output");
    opt->set_flag_value("--verbose", "1");
    
    // Test normal processing when value is not empty and flag names exist
    std::string original_value = "some_value";
    std::string processed_value = opt->get_flag_value("--verbose", original_value);
    
    // Should return the flag value, not the original
    EXPECT_EQ(processed_value, "1");
    
    // Test that flag names are properly populated
    EXPECT_FALSE(opt->get_fnames().empty());
    EXPECT_TRUE(opt->get_fnames().size() > 0);
    
    // Test with different flag name
    auto opt2 = app.add_flag("-v", "--verbose", "Verbose output");
    opt2->set_flag_value("--verbose", "true");
    
    std::string processed_value2 = opt2->get_flag_value("--verbose", original_value);
    EXPECT_EQ(processed_value2, "true");
}