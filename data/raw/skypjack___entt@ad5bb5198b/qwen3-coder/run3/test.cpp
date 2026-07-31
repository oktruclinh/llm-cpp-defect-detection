#include <gtest/gtest.h>
#include <entt/meta/meta.hpp>
#include <entt/meta/resolve.hpp>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>

TEST(MetaAssociativeContainerTest, ConstructorInitialization) {
    // Test with std::map
    std::map<int, std::string> map_container;
    entt::meta_associative_container map_meta(map_container);
    
    EXPECT_TRUE(map_meta.key_only_container);
    EXPECT_NE(map_meta.key_type_fn, nullptr);
    EXPECT_NE(map_meta.mapped_type_fn, nullptr);
    EXPECT_NE(map_meta.value_type_fn, nullptr);
    
    // Test with std::unordered_map
    std::unordered_map<int, std::string> unordered_map_container;
    entt::meta_associative_container unordered_map_meta(unordered_map_container);
    
    EXPECT_TRUE(unordered_map_meta.key_only_container);
    EXPECT_NE(unordered_map_meta.key_type_fn, nullptr);
    EXPECT_NE(unordered_map_meta.mapped_type_fn, nullptr);
    EXPECT_NE(unordered_map_meta.value_type_fn, nullptr);
    
    // Test with std::set (key-only container)
    std::set<int> set_container;
    entt::meta_associative_container set_meta(set_container);
    
    EXPECT_TRUE(set_meta.key_only_container);
    EXPECT_NE(set_meta.key_type_fn, nullptr);
    EXPECT_NE(set_meta.mapped_type_fn, nullptr);
    EXPECT_NE(set_meta.value_type_fn, nullptr);
    
    // Test with std::unordered_set (key-only container)
    std::unordered_set<int> unordered_set_container;
    entt::meta_associative_container unordered_set_meta(unordered_set_container);
    
    EXPECT_TRUE(unordered_set_meta.key_only_container);
    EXPECT_NE(unordered_set_meta.key_type_fn, nullptr);
    EXPECT_NE(unordered_set_meta.mapped_type_fn, nullptr);
    EXPECT_NE(unordered_set_meta.value_type_fn, nullptr);
}

TEST(MetaAssociativeContainerTest, KeyOnlyContainerDetection) {
    // Test key-only containers
    std::set<int> set_container;
    entt::meta_associative_container set_meta(set_container);
    EXPECT_TRUE(set_meta.key_only_container);
    
    std::unordered_set<int> unordered_set_container;
    entt::meta_associative_container unordered_set_meta(unordered_set_container);
    EXPECT_TRUE(unordered_set_meta.key_only_container);
    
    // Test key-value containers
    std::map<int, std::string> map_container;
    entt::meta_associative_container map_meta(map_container);
    EXPECT_TRUE(map_meta.key_only_container);
    
    std::unordered_map<int, std::string> unordered_map_container;
    entt::meta_associative_container unordered_map_meta(unordered_map_container);
    EXPECT_TRUE(unordered_map_meta.key_only_container);
}

TEST(MetaAssociativeContainerTest, FunctionPointerValidity) {
    std::map<int, std::string> container;
    entt::meta_associative_container meta(container);
    
    // Verify function pointers are not null
    EXPECT_NE(meta.key_type_fn, nullptr);
    EXPECT_NE(meta.mapped_type_fn, nullptr);
    EXPECT_NE(meta.value_type_fn, nullptr);
    
    // Test that function pointers can be called (they should not crash)
    // This tests the actual function pointer behavior
    auto key_type = meta.key_type_fn();
    auto mapped_type = meta.mapped_type_fn();
    auto value_type = meta.value_type_fn();
    
    // These should not be null meta types
    EXPECT_NE(key_type, entt::meta_type{});
    EXPECT_NE(mapped_type, entt::meta_type{});
    EXPECT_NE(value_type, entt::meta_type{});
}

TEST(MetaAssociativeContainerTest, EmptyContainer) {
    // Test with empty containers
    std::map<int, std::string> empty_map;
    std::set<int> empty_set;
    std::unordered_map<int, std::string> empty_unordered_map;
    std::unordered_set<int> empty_unordered_set;
    
    entt::meta_associative_container map_meta(empty_map);
    entt::meta_associative_container set_meta(empty_set);
    entt::meta_associative_container unordered_map_meta(empty_unordered_map);
    entt::meta_associative_container unordered_set_meta(empty_unordered_set);
    
    // All should initialize correctly
    EXPECT_TRUE(map_meta.key_only_container);
    EXPECT_TRUE(set_meta.key_only_container);
    EXPECT_TRUE(unordered_map_meta.key_only_container);
    EXPECT_TRUE(unordered_set_meta.key_only_container);
    
    EXPECT_NE(map_meta.key_type_fn, nullptr);
    EXPECT_NE(set_meta.key_type_fn, nullptr);
    EXPECT_NE(unordered_map_meta.key_type_fn, nullptr);
    EXPECT_NE(unordered_set_meta.key_type_fn, nullptr);
}

TEST(MetaAssociativeContainerTest, TypeConsistency) {
    std::map<int, std::string> map_container;
    std::set<int> set_container;
    
    entt::meta_associative_container map_meta(map_container);
    entt::meta_associative_container set_meta(set_container);
    
    // Verify that the key type functions return correct types
    auto map_key_type = map_meta.key_type_fn();
    auto set_key_type = set_meta.key_type_fn();
    
    // Both should resolve to int type
    EXPECT_EQ(map_key_type, entt::resolve<int>());
    EXPECT_EQ(set_key_type, entt::resolve<int>());
    
    // Verify mapped type for map
    auto map_mapped_type = map_meta.mapped_type_fn();
    EXPECT_EQ(map_mapped_type, entt::resolve<std::string>());
    
    // Verify value type for map
    auto map_value_type = map_meta.value_type_fn();
    EXPECT_EQ(map_value_type, entt::resolve<std::pair<const int, std::string>>());
    
    // Verify value type for set (should be same as key type)
    auto set_value_type = set_meta.value_type_fn();
    EXPECT_EQ(set_value_type, entt::resolve<int>());
}