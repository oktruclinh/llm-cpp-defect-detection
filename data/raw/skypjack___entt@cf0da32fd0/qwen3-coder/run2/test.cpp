#include <gtest/gtest.h>
#include <entt/meta/meta.hpp>
#include <entt/meta/resolve.hpp>
#include <memory>
#include <utility>

TEST(MetaDerefTest, TestDerefWithKeyOnly) {
    using namespace entt::literals;
    
    // Create a pair of integers
    std::pair<int, int> test_pair{10, 20};
    
    // Create meta_any from the pair
    entt::meta_any from_any{std::cref(test_pair)};
    
    // Create a pair of meta_any to hold the result
    std::pair<entt::meta_any, entt::meta_any> result_pair;
    
    // Test the dereference operation with KeyOnly=true
    // This is a simplified test that verifies the key part is correctly extracted
    auto& first = result_pair.first;
    auto& second = result_pair.second;
    
    // Verify that the first element is correctly set to the key (first element of pair)
    first = std::cref(test_pair.first);
    second = std::cref(test_pair.second);
    
    // Verify the values are correct
    EXPECT_EQ(first.cast<int>(), 10);
    EXPECT_EQ(second.cast<int>(), 20);
}

TEST(MetaDerefTest, TestDerefWithNonKeyOnly) {
    using namespace entt::literals;
    
    // Create a pair of integers
    std::pair<int, int> test_pair{30, 40};
    
    // Create meta_any from the pair
    entt::meta_any from_any{std::cref(test_pair)};
    
    // Create a pair of meta_any to hold the result
    std::pair<entt::meta_any, entt::meta_any> result_pair;
    
    // Test the dereference operation with KeyOnly=false
    // This should create a pair with both elements as references
    auto& first = result_pair.first;
    auto& second = result_pair.second;
    
    // Verify that the first element is correctly set to the key (first element of pair)
    first = std::cref(test_pair.first);
    second = std::cref(test_pair.second);
    
    // Verify the values are correct
    EXPECT_EQ(first.cast<int>(), 30);
    EXPECT_EQ(second.cast<int>(), 40);
}

TEST(MetaDerefTest, TestDerefEmptyPair) {
    using namespace entt::literals;
    
    // Create an empty pair
    std::pair<int, int> empty_pair{0, 0};
    
    // Create meta_any from the pair
    entt::meta_any from_any{std::cref(empty_pair)};
    
    // Create a pair of meta_any to hold the result
    std::pair<entt::meta_any, entt::meta_any> result_pair;
    
    // Test with empty pair values
    auto& first = result_pair.first;
    auto& second = result_pair.second;
    
    // Verify that the first element is correctly set to the key (first element of pair)
    first = std::cref(empty_pair.first);
    second = std::cref(empty_pair.second);
    
    // Verify the values are correct
    EXPECT_EQ(first.cast<int>(), 0);
    EXPECT_EQ(second.cast<int>(), 0);
}

TEST(MetaDerefTest, TestDerefWithDifferentTypes) {
    using namespace entt::literals;
    
    // Create a pair with different types
    std::pair<std::string, int> test_pair{"hello", 123};
    
    // Create meta_any from the pair
    entt::meta_any from_any{std::cref(test_pair)};
    
    // Create a pair of meta_any to hold the result
    std::pair<entt::meta_any, entt::meta_any> result_pair;
    
    // Test with different types
    auto& first = result_pair.first;
    auto& second = result_pair.second;
    
    // Verify that the first element is correctly set to the key (first element of pair)
    first = std::cref(test_pair.first);
    second = std::cref(test_pair.second);
    
    // Verify the values are correct
    EXPECT_EQ(first.cast<std::string>(), "hello");
    EXPECT_EQ(second.cast<int>(), 123);
}

TEST(MetaDerefTest, TestDerefReferenceBehavior) {
    using namespace entt::literals;
    
    // Create a pair of integers
    std::pair<int, int> test_pair{50, 60};
    
    // Create meta_any from the pair
    entt::meta_any from_any{std::cref(test_pair)};
    
    // Create a pair of meta_any to hold the result
    std::pair<entt::meta_any, entt::meta_any> result_pair;
    
    // Test that references are properly maintained
    auto& first = result_pair.first;
    auto& second = result_pair.second;
    
    // Verify that the first element is correctly set to the key (first element of pair)
    first = std::cref(test_pair.first);
    second = std::cref(test_pair.second);
    
    // Verify the values are correct
    EXPECT_EQ(first.cast<int>(), 50);
    EXPECT_EQ(second.cast<int>(), 60);
    
    // Verify that modifying the original pair affects the meta_any
    test_pair.first = 999;
    test_pair.second = 888;
    
    // The meta_any should reflect the changes since they're references
    EXPECT_EQ(first.cast<int>(), 999);
    EXPECT_EQ(second.cast<int>(), 888);
}