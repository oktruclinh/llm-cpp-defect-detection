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
        // Reset any global state if needed (entt doesn't require this)
    }
};

// Test construction with key-only associative container (std::set)
TEST_F(MetaAssociativeContainerTest, KeyOnlyContainer) {
    std::set<int> container = {1, 2, 3};
    entt::meta_associative_container meta(container);
    
    // Verify key-only nature
    EXPECT_TRUE(meta.key_only_container);
    
    // Verify key type is int
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<int>());
    
    // For key-only containers, mapped_type should be void
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<void>());
    
    // value_type should be the same as key_type for key-only containers
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<int>());
}

// Test construction with key-value associative container (std::map)
TEST_F(MetaAssociativeContainerTest, KeyValueContainer) {
    std::map<int, std::string> container = {{1, "one"}, {2, "two"}};
    entt::meta_associative_container meta(container);
    
    // Verify not key-only
    EXPECT_FALSE(meta.key_only_container);
    
    // Verify key type is int
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<int>());
    
    // Verify mapped type is std::string
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<std::string>());
    
    // value_type should be std::pair<const int, std::string>
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<std::pair<const int, std::string>>());
}

// Test construction with unordered associative container
TEST_F(MetaAssociativeContainerTest, UnorderedContainer) {
    std::unordered_map<double, char> container = {{1.0, 'a'}, {2.0, 'b'}};
    entt::meta_associative_container meta(container);
    
    EXPECT_FALSE(meta.key_only_container);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<double>());
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<char>());
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<std::pair<const double, char>>());
}

// Test construction with empty container
TEST_F(MetaAssociativeContainerTest, EmptyContainer) {
    std::map<int, double> empty_map;
    entt::meta_associative_container meta(empty_map);
    
    EXPECT_FALSE(meta.key_only_container);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<int>());
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<double>());
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<std::pair<const int, double>>());
}

// Test construction with empty key-only container
TEST_F(MetaAssociativeContainerTest, EmptyKeyOnlyContainer) {
    std::set<std::string> empty_set;
    entt::meta_associative_container meta(empty_set);
    
    EXPECT_TRUE(meta.key_only_container);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<std::string>());
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<void>());
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<std::string>());
}

// Test construction with multiset (key-only multi-container)
TEST_F(MetaAssociativeContainerTest, MultiSetContainer) {
    std::multiset<float> container = {1.0f, 2.0f, 1.0f};
    entt::meta_associative_container meta(container);
    
    EXPECT_TRUE(meta.key_only_container);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<float>());
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<void>());
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<float>());
}

// Test construction with multimap (key-value multi-container)
TEST_F(MetaAssociativeContainerTest, MultiMapContainer) {
    std::multimap<int, bool> container = {{1, true}, {2, false}, {1, false}};
    entt::meta_associative_container meta(container);
    
    EXPECT_FALSE(meta.key_only_container);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<int>());
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<bool>());
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<std::pair<const int, bool>>());
}

// Test construction with unordered_multiset
TEST_F(MetaAssociativeContainerTest, UnorderedMultiSetContainer) {
    std::unordered_multiset<long> container = {1L, 2L, 1L};
    entt::meta_associative_container meta(container);
    
    EXPECT_TRUE(meta.key_only_container);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<long>());
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<void>());
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<long>());
}

// Test construction with unordered_multimap
TEST_F(MetaAssociativeContainerTest, UnorderedMultiMapContainer) {
    std::unordered_multimap<char, int> container = {{'a', 1}, {'b', 2}, {'a', 3}};
    entt::meta_associative_container meta(container);
    
    EXPECT_FALSE(meta.key_only_container);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<char>());
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<int>());
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<std::pair<const char, int>>());
}

// Test that function pointers are correctly set and callable
TEST_F(MetaAssociativeContainerTest, FunctionPointersAreCallable) {
    std::map<int, double> container = {{1, 1.5}, {2, 2.5}};
    entt::meta_associative_container meta(container);
    
    // Verify function pointers are not null
    EXPECT_NE(meta.key_type_fn, nullptr);
    EXPECT_NE(meta.mapped_type_fn, nullptr);
    EXPECT_NE(meta.value_type_fn, nullptr);
    
    // Verify they return consistent results when called multiple times
    auto key_type1 = meta.key_type_fn();
    auto key_type2 = meta.key_type_fn();
    EXPECT_EQ(key_type1, key_type2);
    
    auto mapped_type1 = meta.mapped_type_fn();
    auto mapped_type2 = meta.mapped_type_fn();
    EXPECT_EQ(mapped_type1, mapped_type2);
}

// Test with custom type as key
TEST_F(MetaAssociativeContainerTest, CustomKeyType) {
    struct CustomKey {
        int id;
        bool operator<(const CustomKey& other) const { return id < other.id; }
    };
    
    std::map<CustomKey, std::vector<int>> container;
    entt::meta_associative_container meta(container);
    
    EXPECT_FALSE(meta.key_only_container);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<CustomKey>());
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<std::vector<int>>());
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<std::pair<const CustomKey, std::vector<int>>>());
}

// Test with reference wrapper (should still work)
TEST_F(MetaAssociativeContainerTest, ReferenceWrapperContainer) {
    std::set<int> original = {1, 2, 3};
    std::reference_wrapper<std::set<int>> ref(original);
    entt::meta_associative_container meta(ref);
    
    EXPECT_TRUE(meta.key_only_container);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<int>());
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<void>());
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<int>());
}

// Test that different container types produce different type IDs
TEST_F(MetaAssociativeContainerTest, DifferentContainerTypes) {
    std::set<int> set_container;
    std::map<int, int> map_container;
    
    entt::meta_associative_container meta_set(set_container);
    entt::meta_associative_container meta_map(map_container);
    
    // Key-only vs key-value should differ
    EXPECT_NE(meta_set.key_only_container, meta_map.key_only_container);
    
    // Both have int keys
    EXPECT_EQ(meta_set.key_type_fn(), meta_map.key_type_fn());
    
    // Mapped types differ (void vs int)
    EXPECT_NE(meta_set.mapped_type_fn(), meta_map.mapped_type_fn());
    
    // Value types differ (int vs pair)
    EXPECT_NE(meta_set.value_type_fn(), meta_map.value_type_fn());
}