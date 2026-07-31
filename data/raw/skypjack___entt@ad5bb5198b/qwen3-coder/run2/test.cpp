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

TEST(MetaAssociativeContainerTest, KeyTypeFunction) {
    std::map<int, std::string> map_container;
    entt::meta_associative_container meta(map_container);
    
    // Verify key_type_fn returns correct type info
    auto key_type = meta.key_type_fn(map_container);
    EXPECT_NE(key_type, entt::meta_type{});
    
    // Verify it's the correct type
    EXPECT_EQ(key_type, entt::resolve<int>());
}

TEST(MetaAssociativeContainerTest, MappedTypeFunction) {
    std::map<int, std::string> map_container;
    entt::meta_associative_container meta(map_container);
    
    // Verify mapped_type_fn returns correct type info
    auto mapped_type = meta.mapped_type_fn(map_container);
    EXPECT_NE(mapped_type, entt::meta_type{});
    
    // Verify it's the correct type
    EXPECT_EQ(mapped_type, entt::resolve<std::string>());
}

TEST(MetaAssociativeContainerTest, ValueTypeFunction) {
    std::map<int, std::string> map_container;
    entt::meta_associative_container meta(map_container);
    
    // Verify value_type_fn returns correct type info
    auto value_type = meta.value_type_fn(map_container);
    EXPECT_NE(value_type, entt::meta_type{});
    
    // Verify it's the correct type (std::pair<const int, std::string>)
    using expected_type = std::pair<const int, std::string>;
    EXPECT_EQ(value_type, entt::resolve<expected_type>());
}

TEST(MetaAssociativeContainerTest, KeyOnlyContainerFlag) {
    // Test key-only containers
    std::set<int> set_container;
    entt::meta_associative_container set_meta(set_container);
    EXPECT_TRUE(set_meta.key_only_container);
    
    std::unordered_set<int> unordered_set_container;
    entt::meta_associative_container unordered_set_meta(unordered_set_container);
    EXPECT_TRUE(unordered_set_meta.key_only_container);
    
    // Test containers with both key and mapped types
    std::map<int, std::string> map_container;
    entt::meta_associative_container map_meta(map_container);
    EXPECT_TRUE(map_meta.key_only_container);
    
    std::unordered_map<int, std::string> unordered_map_container;
    entt::meta_associative_container unordered_map_meta(unordered_map_container);
    EXPECT_TRUE(unordered_map_meta.key_only_container);
}

TEST(MetaAssociativeContainerTest, EmptyContainer) {
    // Test with empty containers
    std::map<int, std::string> empty_map;
    entt::meta_associative_container empty_map_meta(empty_map);
    
    EXPECT_TRUE(empty_map_meta.key_only_container);
    EXPECT_NE(empty_map_meta.key_type_fn, nullptr);
    EXPECT_NE(empty_map_meta.mapped_type_fn, nullptr);
    EXPECT_NE(empty_map_meta.value_type_fn, nullptr);
    
    std::set<int> empty_set;
    entt::meta_associative_container empty_set_meta(empty_set);
    
    EXPECT_TRUE(empty_set_meta.key_only_container);
    EXPECT_NE(empty_set_meta.key_type_fn, nullptr);
    EXPECT_NE(empty_set_meta.mapped_type_fn, nullptr);
    EXPECT_NE(empty_set_meta.value_type_fn, nullptr);
}

TEST(MetaAssociativeContainerTest, DifferentContainerTypes) {
    // Test various container types to ensure consistent behavior
    std::map<int, double> map_double;
    entt::meta_associative_container map_double_meta(map_double);
    
    std::unordered_map<std::string, int> unordered_map_string;
    entt::meta_associative_container unordered_map_string_meta(unordered_map_string);
    
    std::set<float> set_float;
    entt::meta_associative_container set_float_meta(set_float);
    
    std::unordered_set<std::string> unordered_set_string;
    entt::meta_associative_container unordered_set_string_meta(unordered_set_string);
    
    // All should be key-only containers
    EXPECT_TRUE(map_double_meta.key_only_container);
    EXPECT_TRUE(unordered_map_string_meta.key_only_container);
    EXPECT_TRUE(set_float_meta.key_only_container);
    EXPECT_TRUE(unordered_set_string_meta.key_only_container);
    
    // All should have valid function pointers
    EXPECT_NE(map_double_meta.key_type_fn, nullptr);
    EXPECT_NE(unordered_map_string_meta.key_type_fn, nullptr);
    EXPECT_NE(set_float_meta.key_type_fn, nullptr);
    EXPECT_NE(unordered_set_string_meta.key_type_fn, nullptr);
    
    EXPECT_NE(map_double_meta.mapped_type_fn, nullptr);
    EXPECT_NE(unordered_map_string_meta.mapped_type_fn, nullptr);
    EXPECT_NE(set_float_meta.mapped_type_fn, nullptr);
    EXPECT_NE(unordered_set_string_meta.mapped_type_fn, nullptr);
    
    EXPECT_NE(map_double_meta.value_type_fn, nullptr);
    EXPECT_NE(unordered_map_string_meta.value_type_fn, nullptr);
    EXPECT_NE(set_float_meta.value_type_fn, nullptr);
    EXPECT_NE(unordered_set_string_meta.value_type_fn, nullptr);
}