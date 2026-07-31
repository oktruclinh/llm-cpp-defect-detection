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
    EXPECT_EQ(key_type, entt::resolve<int>());
    
    std::set<std::string> set_container;
    entt::meta_associative_container set_meta(set_container);
    
    auto set_key_type = set_meta.key_type_fn(set_container);
    EXPECT_NE(set_key_type, entt::meta_type{});
    EXPECT_EQ(set_key_type, entt::resolve<std::string>());
}

TEST(MetaAssociativeContainerTest, MappedTypeFunction) {
    std::map<int, std::string> map_container;
    entt::meta_associative_container meta(map_container);
    
    // Verify mapped_type_fn returns correct type info
    auto mapped_type = meta.mapped_type_fn(map_container);
    EXPECT_NE(mapped_type, entt::meta_type{});
    EXPECT_EQ(mapped_type, entt::resolve<std::string>());
    
    std::unordered_map<double, int> unordered_map_container;
    entt::meta_associative_container unordered_map_meta(unordered_map_container);
    
    auto unordered_map_mapped_type = unordered_map_meta.mapped_type_fn(unordered_map_container);
    EXPECT_NE(unordered_map_mapped_type, entt::meta_type{});
    EXPECT_EQ(unordered_map_mapped_type, entt::resolve<int>());
}

TEST(MetaAssociativeContainerTest, ValueTypeFunction) {
    std::map<int, std::string> map_container;
    entt::meta_associative_container meta(map_container);
    
    // Verify value_type_fn returns correct type info
    auto value_type = meta.value_type_fn(map_container);
    EXPECT_NE(value_type, entt::meta_type{});
    EXPECT_EQ(value_type, entt::resolve<std::pair<const int, std::string>>());
    
    std::set<int> set_container;
    entt::meta_associative_container set_meta(set_container);
    
    auto set_value_type = set_meta.value_type_fn(set_container);
    EXPECT_NE(set_value_type, entt::meta_type{});
    EXPECT_EQ(set_value_type, entt::resolve<int>());
}

TEST(MetaAssociativeContainerTest, EmptyContainers) {
    // Test with empty containers
    std::map<int, std::string> empty_map;
    entt::meta_associative_container empty_map_meta(empty_map);
    
    EXPECT_TRUE(empty_map_meta.key_only_container);
    EXPECT_NE(empty_map_meta.key_type_fn, nullptr);
    EXPECT_NE(empty_map_meta.mapped_type_fn, nullptr);
    EXPECT_NE(empty_map_meta.value_type_fn, nullptr);
    
    std::set<std::string> empty_set;
    entt::meta_associative_container empty_set_meta(empty_set);
    
    EXPECT_TRUE(empty_set_meta.key_only_container);
    EXPECT_NE(empty_set_meta.key_type_fn, nullptr);
    EXPECT_NE(empty_set_meta.mapped_type_fn, nullptr);
    EXPECT_NE(empty_set_meta.value_type_fn, nullptr);
}

TEST(MetaAssociativeContainerTest, ContainerTypeDetection) {
    // Verify that key-only containers are correctly identified
    std::set<int> set_container;
    std::unordered_set<int> unordered_set_container;
    std::map<int, std::string> map_container;
    std::unordered_map<int, std::string> unordered_map_container;
    
    entt::meta_associative_container set_meta(set_container);
    entt::meta_associative_container unordered_set_meta(unordered_set_container);
    entt::meta_associative_container map_meta(map_container);
    entt::meta_associative_container unordered_map_meta(unordered_map_container);
    
    // All should be key-only containers
    EXPECT_TRUE(set_meta.key_only_container);
    EXPECT_TRUE(unordered_set_meta.key_only_container);
    EXPECT_TRUE(map_meta.key_only_container);
    EXPECT_TRUE(unordered_map_meta.key_only_container);
}