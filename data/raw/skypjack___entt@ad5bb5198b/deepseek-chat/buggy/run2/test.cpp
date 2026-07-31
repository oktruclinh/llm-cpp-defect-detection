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

// Test construction with a key-only associative container (std::set)
TEST_F(MetaAssociativeContainerTest, KeyOnlyContainer) {
    std::set<int> container = {1, 2, 3};
    entt::meta_associative_container meta(container);

    // Verify it's a key-only container (no mapped type)
    EXPECT_FALSE(meta.key_only_container); // key_only_container is false when it IS key-only? Check semantics
    // Actually, looking at the constructor: key_only_container{!is_key_only_meta_associative_container_v<Type>}
    // For std::set, is_key_only_meta_associative_container_v is true, so key_only_container = false
    // This is confusing naming, but we test the actual behavior
    EXPECT_FALSE(meta.key_only_container); // For key-only containers, this is false

    // Verify key_type function exists and returns correct type
    ASSERT_NE(meta.key_type_fn, nullptr);
    auto key_type_id = meta.key_type_fn();
    EXPECT_EQ(key_type_id, entt::type_id<int>());

    // For key-only containers, mapped_type_fn should be nullptr
    EXPECT_EQ(meta.mapped_type_fn, nullptr);

    // value_type_fn should return the value type (which is same as key for set)
    ASSERT_NE(meta.value_type_fn, nullptr);
    auto value_type_id = meta.value_type_fn();
    EXPECT_EQ(value_type_id, entt::type_id<int>());
}

// Test construction with a key-value associative container (std::map)
TEST_F(MetaAssociativeContainerTest, KeyValueContainer) {
    std::unordered_map<std::string, double> container = {{"pi", 3.14}, {"e", 2.718}};
    entt::meta_associative_container meta(container);

    // For map-like containers, key_only_container should be true
    EXPECT_TRUE(meta.key_only_container);

    // Verify key_type function
    ASSERT_NE(meta.key_type_fn, nullptr);
    auto key_type_id = meta.key_type_fn();
    EXPECT_EQ(key_type_id, entt::type_id<std::string>());

    // Verify mapped_type function
    ASSERT_NE(meta.mapped_type_fn, nullptr);
    auto mapped_type_id = meta.mapped_type_fn();
    EXPECT_EQ(mapped_type_id, entt::type_id<double>());

    // Verify value_type function (pair<const string, double>)
    ASSERT_NE(meta.value_type_fn, nullptr);
    auto value_type_id = meta.value_type_fn();
    EXPECT_EQ(value_type_id, entt::type_id<std::pair<const std::string, double>>());
}

// Test construction with an empty container
TEST_F(MetaAssociativeContainerTest, EmptyContainer) {
    std::map<int, int> empty_map;
    entt::meta_associative_container meta(empty_map);

    // Type information should still be correct even for empty containers
    EXPECT_TRUE(meta.key_only_container);
    ASSERT_NE(meta.key_type_fn, nullptr);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<int>());
    ASSERT_NE(meta.mapped_type_fn, nullptr);
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<int>());
    ASSERT_NE(meta.value_type_fn, nullptr);
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<std::pair<const int, int>>());
}

// Test construction with a std::multiset (key-only multi-container)
TEST_F(MetaAssociativeContainerTest, MultiSetContainer) {
    std::multiset<long> container = {10L, 20L, 10L};
    entt::meta_associative_container meta(container);

    EXPECT_FALSE(meta.key_only_container);
    ASSERT_NE(meta.key_type_fn, nullptr);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<long>());
    EXPECT_EQ(meta.mapped_type_fn, nullptr);
    ASSERT_NE(meta.value_type_fn, nullptr);
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<long>());
}

// Test construction with a std::multimap (key-value multi-container)
TEST_F(MetaAssociativeContainerTest, MultiMapContainer) {
    std::multimap<char, float> container = {{'a', 1.0f}, {'b', 2.0f}, {'a', 3.0f}};
    entt::meta_associative_container meta(container);

    EXPECT_TRUE(meta.key_only_container);
    ASSERT_NE(meta.key_type_fn, nullptr);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<char>());
    ASSERT_NE(meta.mapped_type_fn, nullptr);
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<float>());
    ASSERT_NE(meta.value_type_fn, nullptr);
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<std::pair<const char, float>>());
}

// Test construction with a std::unordered_set (key-only unordered container)
TEST_F(MetaAssociativeContainerTest, UnorderedSetContainer) {
    std::unordered_set<unsigned int> container = {100u, 200u};
    entt::meta_associative_container meta(container);

    EXPECT_FALSE(meta.key_only_container);
    ASSERT_NE(meta.key_type_fn, nullptr);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<unsigned int>());
    EXPECT_EQ(meta.mapped_type_fn, nullptr);
    ASSERT_NE(meta.value_type_fn, nullptr);
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<unsigned int>());
}

// Test construction with a std::unordered_multimap (key-value unordered multi-container)
TEST_F(MetaAssociativeContainerTest, UnorderedMultiMapContainer) {
    std::unordered_multimap<int, std::string> container = {{1, "one"}, {2, "two"}};
    entt::meta_associative_container meta(container);

    EXPECT_TRUE(meta.key_only_container);
    ASSERT_NE(meta.key_type_fn, nullptr);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<int>());
    ASSERT_NE(meta.mapped_type_fn, nullptr);
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<std::string>());
    ASSERT_NE(meta.value_type_fn, nullptr);
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<std::pair<const int, std::string>>());
}

// Test that the function pointers are correctly set for different container types
TEST_F(MetaAssociativeContainerTest, FunctionPointersAreDistinct) {
    std::map<int, double> map_container;
    std::set<int> set_container;

    entt::meta_associative_container meta_map(map_container);
    entt::meta_associative_container meta_set(set_container);

    // Key type functions should be different objects (different instantiations)
    EXPECT_NE(meta_map.key_type_fn, meta_set.key_type_fn);

    // Map should have mapped_type_fn, set should not
    EXPECT_NE(meta_map.mapped_type_fn, nullptr);
    EXPECT_EQ(meta_set.mapped_type_fn, nullptr);

    // Value type functions should be different
    EXPECT_NE(meta_map.value_type_fn, meta_set.value_type_fn);
}

// Test with a custom type that satisfies associative container requirements
TEST_F(MetaAssociativeContainerTest, CustomAssociativeContainer) {
    // Using std::map with custom comparator to ensure template flexibility
    std::map<int, int, std::greater<int>> container;
    container[3] = 30;
    container[1] = 10;

    entt::meta_associative_container meta(container);

    EXPECT_TRUE(meta.key_only_container);
    ASSERT_NE(meta.key_type_fn, nullptr);
    EXPECT_EQ(meta.key_type_fn(), entt::type_id<int>());
    ASSERT_NE(meta.mapped_type_fn, nullptr);
    EXPECT_EQ(meta.mapped_type_fn(), entt::type_id<int>());
    ASSERT_NE(meta.value_type_fn, nullptr);
    EXPECT_EQ(meta.value_type_fn(), entt::type_id<std::pair<const int, int>>());
}