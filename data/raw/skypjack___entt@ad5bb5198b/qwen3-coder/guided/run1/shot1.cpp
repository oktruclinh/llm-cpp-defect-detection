#include <gtest/gtest.h>
#include <entt/meta/meta.hpp>
#include <entt/container/dense_map.hpp>
#include <unordered_map>
#include <map>

TEST(MetaAssociativeContainerTest, ConstructWithDenseMap) {
    entt::dense_map<int, std::string> container;
    
    // Test that we can construct meta_associative_container with dense_map
    entt::meta_associative_container meta_container(container);
    
    // Verify key_only_container flag is set correctly
    EXPECT_FALSE(meta_container.key_only_container);
    
    // Verify function pointers are properly initialized
    EXPECT_NE(meta_container.key_type_fn, nullptr);
    EXPECT_NE(meta_container.mapped_type_fn, nullptr);
    EXPECT_NE(meta_container.value_type_fn, nullptr);
}

TEST(MetaAssociativeContainerTest, ConstructWithUnorderedMap) {
    std::unordered_map<int, double> container;
    
    // Test that we can construct meta_associative_container with unordered_map
    entt::meta_associative_container meta_container(container);
    
    // Verify key_only_container flag is set correctly
    EXPECT_FALSE(meta_container.key_only_container);
    
    // Verify function pointers are properly initialized
    EXPECT_NE(meta_container.key_type_fn, nullptr);
    EXPECT_NE(meta_container.mapped_type_fn, nullptr);
    EXPECT_NE(meta_container.value_type_fn, nullptr);
}

TEST(MetaAssociativeContainerTest, ConstructWithMap) {
    std::map<std::string, int> container;
    
    // Test that we can construct meta_associative_container with map
    entt::meta_associative_container meta_container(container);
    
    // Verify key_only_container flag is set correctly
    EXPECT_FALSE(meta_container.key_only_container);
    
    // Verify function pointers are properly initialized
    EXPECT_NE(meta_container.key_type_fn, nullptr);
    EXPECT_NE(meta_container.mapped_type_fn, nullptr);
    EXPECT_NE(meta_container.value_type_fn, nullptr);
}

TEST(MetaAssociativeContainerTest, KeyOnlyContainerDetection) {
    // Test with a container that should be detected as key-only
    // Note: This requires a specific container type that satisfies key-only criteria
    
    // For now, just verify the constructor doesn't crash with various containers
    std::map<int, int> regular_map;
    entt::dense_map<int, int> dense_map;
    
    entt::meta_associative_container meta_map(regular_map);
    entt::meta_associative_container meta_dense(dense_map);
    
    // Both should have key_only_container = false since they have both key and mapped types
    EXPECT_FALSE(meta_map.key_only_container);
    EXPECT_FALSE(meta_dense.key_only_container);
}

TEST(MetaAssociativeContainerTest, EmptyContainerConstruction) {
    // Test construction with empty containers
    std::unordered_map<float, bool> empty_unordered_map;
    std::map<std::string, int> empty_map;
    
    entt::meta_associative_container meta_unordered(empty_unordered_map);
    entt::meta_associative_container meta_map(empty_map);
    
    // Should still initialize all members properly
    EXPECT_FALSE(meta_unordered.key_only_container);
    EXPECT_FALSE(meta_map.key_only_container);
    
    EXPECT_NE(meta_unordered.key_type_fn, nullptr);
    EXPECT_NE(meta_unordered.mapped_type_fn, nullptr);
    EXPECT_NE(meta_unordered.value_type_fn, nullptr);
    
    EXPECT_NE(meta_map.key_type_fn, nullptr);
    EXPECT_NE(meta_map.mapped_type_fn, nullptr);
    EXPECT_NE(meta_map.value_type_fn, nullptr);
}