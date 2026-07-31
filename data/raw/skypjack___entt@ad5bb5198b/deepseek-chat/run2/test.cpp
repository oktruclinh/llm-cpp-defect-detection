#include <gtest/gtest.h>
#include <entt/entt.hpp>
#include <unordered_map>
#include <vector>
#include <set>
#include <type_traits>

// Test fixture for meta_associative_container construction
class MetaAssociativeContainerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Register types for meta system
        entt::meta<int>()
            .type("int"_hs);
        
        entt::meta<double>()
            .type("double"_hs);
    }
};

// Test construction with key-only associative container (std::set)
TEST_F(MetaAssociativeContainerTest, KeyOnlyContainer) {
    std::set<int> container = {1, 2, 3};
    
    entt::meta_associative_container meta_container{container};
    
    // Verify it's identified as key-only
    EXPECT_TRUE(meta_container.key_only_container);
    
    // Key type should be int
    EXPECT_EQ(meta_container.key_type_fn(), entt::resolve<int>());
    
    // Mapped type should be void for key-only containers
    EXPECT_EQ(meta_container.mapped_type_fn(), entt::meta_type{});
    
    // Value type should be the same as key type for key-only
    EXPECT_EQ(meta_container.value_type_fn(), entt::resolve<int>());
}

// Test construction with key-value associative container (std::map)
TEST_F(MetaAssociativeContainerTest, KeyValueContainer) {
    std::unordered_map<int, double> container = {{1, 1.0}, {2, 2.0}};
    
    entt::meta_associative_container meta_container{container};
    
    // Verify it's NOT key-only
    EXPECT_FALSE(meta_container.key_only_container);
    
    // Key type should be int
    EXPECT_EQ(meta_container.key_type_fn(), entt::resolve<int>());
    
    // Mapped type should be double
    EXPECT_EQ(meta_container.mapped_type_fn(), entt::resolve<double>());
    
    // Value type should be std::pair<const int, double>
    auto value_type = meta_container.value_type_fn();
    EXPECT_TRUE(value_type);
    EXPECT_NE(value_type, entt::resolve<int>());
    EXPECT_NE(value_type, entt::resolve<double>());
}

// Test construction with empty key-only container
TEST_F(MetaAssociativeContainerTest, EmptyKeyOnlyContainer) {
    std::set<int> container;
    
    entt::meta_associative_container meta_container{container};
    
    EXPECT_TRUE(meta_container.key_only_container);
    EXPECT_EQ(meta_container.key_type_fn(), entt::resolve<int>());
    EXPECT_EQ(meta_container.mapped_type_fn(), entt::meta_type{});
    EXPECT_EQ(meta_container.value_type_fn(), entt::resolve<int>());
}

// Test construction with empty key-value container
TEST_F(MetaAssociativeContainerTest, EmptyKeyValueContainer) {
    std::unordered_map<int, double> container;
    
    entt::meta_associative_container meta_container{container};
    
    EXPECT_FALSE(meta_container.key_only_container);
    EXPECT_EQ(meta_container.key_type_fn(), entt::resolve<int>());
    EXPECT_EQ(meta_container.mapped_type_fn(), entt::resolve<double>());
    auto value_type = meta_container.value_type_fn();
    EXPECT_TRUE(value_type);
}

// Test construction with std::multiset (key-only multi-container)
TEST_F(MetaAssociativeContainerTest, MultiSetContainer) {
    std::multiset<int> container = {1, 2, 2, 3};
    
    entt::meta_associative_container meta_container{container};
    
    EXPECT_TRUE(meta_container.key_only_container);
    EXPECT_EQ(meta_container.key_type_fn(), entt::resolve<int>());
    EXPECT_EQ(meta_container.mapped_type_fn(), entt::meta_type{});
    EXPECT_EQ(meta_container.value_type_fn(), entt::resolve<int>());
}

// Test construction with std::multimap (key-value multi-container)
TEST_F(MetaAssociativeContainerTest, MultiMapContainer) {
    std::multimap<int, double> container = {{1, 1.0}, {2, 2.0}, {2, 3.0}};
    
    entt::meta_associative_container meta_container{container};
    
    EXPECT_FALSE(meta_container.key_only_container);
    EXPECT_EQ(meta_container.key_type_fn(), entt::resolve<int>());
    EXPECT_EQ(meta_container.mapped_type_fn(), entt::resolve<double>());
    auto value_type = meta_container.value_type_fn();
    EXPECT_TRUE(value_type);
}

// Test that function pointers are correctly set and invocable
TEST_F(MetaAssociativeContainerTest, FunctionPointersAreSet) {
    std::unordered_map<int, double> container;
    
    entt::meta_associative_container meta_container{container};
    
    // Verify function pointers are not null
    EXPECT_NE(meta_container.key_type_fn, nullptr);
    EXPECT_NE(meta_container.mapped_type_fn, nullptr);
    EXPECT_NE(meta_container.value_type_fn, nullptr);
    
    // Verify they return consistent results
    auto key_type = meta_container.key_type_fn();
    auto mapped_type = meta_container.mapped_type_fn();
    auto value_type = meta_container.value_type_fn();
    
    EXPECT_TRUE(key_type);
    EXPECT_TRUE(mapped_type);
    EXPECT_TRUE(value_type);
}

// Test with different key types
TEST_F(MetaAssociativeContainerTest, DifferentKeyTypes) {
    // Test with string keys
    std::map<std::string, int> string_map = {{"hello", 1}};
    
    entt::meta_associative_container meta_container{string_map};
    
    EXPECT_FALSE(meta_container.key_only_container);
    EXPECT_EQ(meta_container.key_type_fn(), entt::resolve<std::string>());
    EXPECT_EQ(meta_container.mapped_type_fn(), entt::resolve<int>());
}

// Test with custom type as key
TEST_F(MetaAssociativeContainerTest, CustomKeyType) {
    struct CustomKey { int id; };
    
    entt::meta<CustomKey>()
        .type("CustomKey"_hs)
        .data<&CustomKey::id>("id"_hs);
    
    std::map<CustomKey, double> custom_map;
    
    entt::meta_associative_container meta_container{custom_map};
    
    EXPECT_FALSE(meta_container.key_only_container);
    EXPECT_EQ(meta_container.key_type_fn(), entt::resolve<CustomKey>());
    EXPECT_EQ(meta_container.mapped_type_fn(), entt::resolve<double>());
}

// Test that construction works with const containers
TEST_F(MetaAssociativeContainerTest, ConstContainer) {
    const std::map<int, double> const_map = {{1, 1.0}};
    
    entt::meta_associative_container meta_container{const_map};
    
    EXPECT_FALSE(meta_container.key_only_container);
    EXPECT_EQ(meta_container.key_type_fn(), entt::resolve<int>());
    EXPECT_EQ(meta_container.mapped_type_fn(), entt::resolve<double>());
}

// Test that the proxy correctly identifies unordered containers
TEST_F(MetaAssociativeContainerTest, UnorderedContainer) {
    std::unordered_set<int> unordered_set = {1, 2, 3};
    
    entt::meta_associative_container meta_container{unordered_set};
    
    EXPECT_TRUE(meta_container.key_only_container);
    EXPECT_EQ(meta_container.key_type_fn(), entt::resolve<int>());
    EXPECT_EQ(meta_container.mapped_type_fn(), entt::meta_type{});
    EXPECT_EQ(meta_container.value_type_fn(), entt::resolve<int>());
}

// Test that the proxy correctly identifies unordered map
TEST_F(MetaAssociativeContainerTest, UnorderedMapContainer) {
    std::unordered_map<int, double> unordered_map = {{1, 1.0}};
    
    entt::meta_associative_container meta_container{unordered_map};
    
    EXPECT_FALSE(meta_container.key_only_container);
    EXPECT_EQ(meta_container.key_type_fn(), entt::resolve<int>());
    EXPECT_EQ(meta_container.mapped_type_fn(), entt::resolve<double>());
}