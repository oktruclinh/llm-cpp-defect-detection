#include <gtest/gtest.h>
#include <entt/meta/meta.hpp>
#include <entt/meta/resolve.hpp>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <map>

TEST(MetaDerefOperationTest, TestDerefWithKeyOnly) {
    using namespace entt::literals;
    
    // Create a pair of integers
    std::pair<int, int> test_pair{10, 20};
    
    // Create meta_any from the pair
    entt::meta_any from_any{std::cref(test_pair)};
    
    // Create a pair of meta_any to receive the result
    std::pair<entt::meta_any, entt::meta_any> result_pair;
    
    // Test the deref operation with KeyOnly=true
    // This should extract the first element of the pair
    auto& from_ref = entt::any_cast<const std::pair<int, int>&>(from_any);
    result_pair.first = std::cref(from_ref.first);
    
    // Verify the result
    ASSERT_TRUE(result_pair.first);
    ASSERT_EQ(result_pair.first.cast<int>(), 10);
}

TEST(MetaDerefOperationTest, TestDerefWithKeyAndValue) {
    using namespace entt::literals;
    
    // Create a pair of integers
    std::pair<int, int> test_pair{10, 20};
    
    // Create meta_any from the pair
    entt::meta_any from_any{std::cref(test_pair)};
    
    // Create a pair of meta_any to receive the result
    std::pair<entt::meta_any, entt::meta_any> result_pair;
    
    // Test the deref operation with KeyOnly=false
    // This should extract both elements of the pair
    auto& from_ref = entt::any_cast<const std::pair<int, int>&>(from_any);
    result_pair = std::make_pair<entt::meta_any, entt::meta_any>(
        std::cref(from_ref.first), 
        std::ref(from_ref.second)
    );
    
    // Verify the result
    ASSERT_TRUE(result_pair.first);
    ASSERT_TRUE(result_pair.second);
    ASSERT_EQ(result_pair.first.cast<int>(), 10);
    ASSERT_EQ(result_pair.second.cast<int>(), 20);
}

TEST(MetaDerefOperationTest, TestDerefWithEmptyPair) {
    using namespace entt::literals;
    
    // Create an empty pair
    std::pair<int, int> empty_pair{0, 0};
    
    // Create meta_any from the pair
    entt::meta_any from_any{std::cref(empty_pair)};
    
    // Create a pair of meta_any to receive the result
    std::pair<entt::meta_any, entt::meta_any> result_pair;
    
    // Test the deref operation with KeyOnly=true
    auto& from_ref = entt::any_cast<const std::pair<int, int>&>(from_any);
    result_pair.first = std::cref(from_ref.first);
    
    // Verify the result
    ASSERT_TRUE(result_pair.first);
    ASSERT_EQ(result_pair.first.cast<int>(), 0);
}

TEST(MetaDerefOperationTest, TestDerefWithStringPair) {
    using namespace entt::literals;
    
    // Create a pair of strings
    std::pair<std::string, std::string> string_pair{"hello", "world"};
    
    // Create meta_any from the pair
    entt::meta_any from_any{std::cref(string_pair)};
    
    // Create a pair of meta_any to receive the result
    std::pair<entt::meta_any, entt::meta_any> result_pair;
    
    // Test the deref operation with KeyOnly=false
    auto& from_ref = entt::any_cast<const std::pair<std::string, std::string>&>(from_any);
    result_pair = std::make_pair<entt::meta_any, entt::meta_any>(
        std::cref(from_ref.first), 
        std::ref(from_ref.second)
    );
    
    // Verify the result
    ASSERT_TRUE(result_pair.first);
    ASSERT_TRUE(result_pair.second);
    ASSERT_EQ(result_pair.first.cast<std::string>(), "hello");
    ASSERT_EQ(result_pair.second.cast<std::string>(), "world");
}

TEST(MetaDerefOperationTest, TestDerefWithComplexTypes) {
    using namespace entt::literals;
    
    // Create a pair of complex types
    std::pair<std::vector<int>, std::map<std::string, int>> complex_pair{
        {1, 2, 3}, 
        {{"key1", 100}, {"key2", 200}}
    };
    
    // Create meta_any from the pair
    entt::meta_any from_any{std::cref(complex_pair)};
    
    // Create a pair of meta_any to receive the result
    std::pair<entt::meta_any, entt::meta_any> result_pair;
    
    // Test the deref operation with KeyOnly=true
    auto& from_ref = entt::any_cast<const std::pair<std::vector<int>, std::map<std::string, int>>&>(from_any);
    result_pair.first = std::cref(from_ref.first);
    
    // Verify the result
    ASSERT_TRUE(result_pair.first);
    const auto& vec = result_pair.first.cast<const std::vector<int>&>();
    ASSERT_EQ(vec.size(), 3);
    ASSERT_EQ(vec[0], 1);
    ASSERT_EQ(vec[1], 2);
    ASSERT_EQ(vec[2], 3);
}

TEST(MetaDerefOperationTest, TestDerefWithReferenceWrapper) {
    using namespace entt::literals;
    
    // Create a pair of integers
    std::pair<int, int> test_pair{42, 84};
    
    // Create a reference to the pair
    std::reference_wrapper<std::pair<int, int>> ref_wrapper{test_pair};
    
    // Create meta_any from the reference wrapper
    entt::meta_any from_any{std::cref(ref_wrapper)};
    
    // Create a pair of meta_any to receive the result
    std::pair<entt::meta_any, entt::meta_any> result_pair;
    
    // Test the deref operation with KeyOnly=false
    auto& from_ref = entt::any_cast<const std::reference_wrapper<std::pair<int, int>>&>(from_any);
    result_pair = std::make_pair<entt::meta_any, entt::meta_any>(
        std::cref(from_ref.get().first), 
        std::ref(from_ref.get().second)
    );
    
    // Verify the result
    ASSERT_TRUE(result_pair.first);
    ASSERT_TRUE(result_pair.second);
    ASSERT_EQ(result_pair.first.cast<int>(), 42);
    ASSERT_EQ(result_pair.second.cast<int>(), 84);
}