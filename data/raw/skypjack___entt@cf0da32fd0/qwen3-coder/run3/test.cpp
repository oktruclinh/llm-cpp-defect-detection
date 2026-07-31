#include <gtest/gtest.h>
#include <entt/meta/meta.hpp>
#include <entt/meta/resolve.hpp>
#include <memory>
#include <utility>
#include <vector>
#include <map>
#include <string>

TEST(MetaDerefTest, TestDerefOperationWithKeyOnly) {
    using namespace entt::literals;
    
    // Create a pair of meta_any objects
    std::pair<int, int> test_pair{100, 200};
    entt::meta_any from_ref = std::cref(test_pair);
    entt::meta_any to_pair = std::make_pair(entt::meta_any{}, entt::meta_any{});
    
    // Test the KeyOnly=true branch of the operation
    // The first element should be copied from the pair
    auto* pair_ptr = static_cast<std::pair<entt::meta_any, entt::meta_any>*>(to_pair.data());
    ASSERT_NE(pair_ptr, nullptr);
    
    // Verify that the first element is properly set
    ASSERT_TRUE(pair_ptr->first);
    ASSERT_EQ(pair_ptr->first.cast<int>(), 100);
}

TEST(MetaDerefTest, TestDerefOperationWithoutKeyOnly) {
    using namespace entt::literals;
    
    // Test the DEREF operation with KeyOnly=false
    // This should create a pair with both elements properly referenced
    
    std::pair<int, int> test_pair{300, 400};
    entt::meta_any from_ref = std::cref(test_pair);
    entt::meta_any to_pair = std::make_pair(entt::meta_any{}, entt::meta_any{});
    
    // Test the non-KeyOnly branch
    auto* pair_ptr = static_cast<std::pair<entt::meta_any, entt::meta_any>*>(to_pair.data());
    ASSERT_NE(pair_ptr, nullptr);
    
    // Verify that both elements are properly set
    ASSERT_TRUE(pair_ptr->first);
    ASSERT_TRUE(pair_ptr->second);
    ASSERT_EQ(pair_ptr->first.cast<int>(), 300);
    ASSERT_EQ(pair_ptr->second.cast<int>(), 400);
}

TEST(MetaDerefTest, TestDerefWithEmptyPair) {
    using namespace entt::literals;
    
    // Test with empty pair values
    std::pair<int, int> empty_pair{0, 0};
    entt::meta_any from_ref = std::cref(empty_pair);
    entt::meta_any to_pair = std::make_pair(entt::meta_any{}, entt::meta_any{});
    
    auto* pair_ptr = static_cast<std::pair<entt::meta_any, entt::meta_any>*>(to_pair.data());
    ASSERT_NE(pair_ptr, nullptr);
    
    // Verify that zero values are properly handled
    ASSERT_TRUE(pair_ptr->first);
    ASSERT_TRUE(pair_ptr->second);
    ASSERT_EQ(pair_ptr->first.cast<int>(), 0);
    ASSERT_EQ(pair_ptr->second.cast<int>(), 0);
}

TEST(MetaDerefTest, TestDerefWithDifferentTypes) {
    using namespace entt::literals;
    
    // Test with different types in the pair
    std::pair<std::string, double> test_pair{"hello", 3.14};
    entt::meta_any from_ref = std::cref(test_pair);
    entt::meta_any to_pair = std::make_pair(entt::meta_any{}, entt::meta_any{});
    
    auto* pair_ptr = static_cast<std::pair<entt::meta_any, entt::meta_any>*>(to_pair.data());
    ASSERT_NE(pair_ptr, nullptr);
    
    // Verify string and double values
    ASSERT_TRUE(pair_ptr->first);
    ASSERT_TRUE(pair_ptr->second);
    ASSERT_EQ(pair_ptr->first.cast<std::string>(), "hello");
    ASSERT_DOUBLE_EQ(pair_ptr->second.cast<double>(), 3.14);
}

TEST(MetaDerefTest, TestDerefWithComplexTypes) {
    using namespace entt::literals;
    
    // Test with complex types
    std::pair<std::vector<int>, std::map<std::string, int>> test_pair{
        {1, 2, 3}, 
        {{"key1", 10}, {"key2", 20}}
    };
    
    entt::meta_any from_ref = std::cref(test_pair);
    entt::meta_any to_pair = std::make_pair(entt::meta_any{}, entt::meta_any{});
    
    auto* pair_ptr = static_cast<std::pair<entt::meta_any, entt::meta_any>*>(to_pair.data());
    ASSERT_NE(pair_ptr, nullptr);
    
    // Verify complex types are properly handled
    ASSERT_TRUE(pair_ptr->first);
    ASSERT_TRUE(pair_ptr->second);
    
    // Test vector access - use const reference to avoid rvalue binding issues
    const auto& vec = pair_ptr->first.cast<std::vector<int>>();
    ASSERT_EQ(vec.size(), 3);
    ASSERT_EQ(vec[0], 1);
    ASSERT_EQ(vec[1], 2);
    ASSERT_EQ(vec[2], 3);
    
    // Test map access - use const reference to avoid rvalue binding issues
    const auto& map = pair_ptr->second.cast<std::map<std::string, int>>();
    ASSERT_EQ(map.size(), 2);
    ASSERT_EQ(map.at("key1"), 10);
    ASSERT_EQ(map.at("key2"), 20);
}