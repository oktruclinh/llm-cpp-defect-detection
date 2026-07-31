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

    // Verify it's not a key-value container
    EXPECT_FALSE(meta.key_only_container());

    // Verify key type is int
    EXPECT_EQ(meta.key_type(), entt::resolve<int>());

    // Verify value type is int (same as key for key-only)
    EXPECT_EQ(meta.value_type(), entt::resolve<int>());

    // Verify mapped type is void (no mapped type for key-only)
    EXPECT_EQ(meta.mapped_type(), entt::resolve<void>());
}

// Test construction with a key-value associative container (std::map)
TEST_F(MetaAssociativeContainerTest, KeyValueContainer) {
    std::unordered_map<std::string, double> container = {{"pi", 3.14}, {"e", 2.718}};
    entt::meta_associative_container meta(container);

    // Verify it's a key-value container
    EXPECT_TRUE(meta.key_only_container());

    // Verify key type is std::string
    EXPECT_EQ(meta.key_type(), entt::resolve<std::string>());

    // Verify mapped type is double
    EXPECT_EQ(meta.mapped_type(), entt::resolve<double>());

    // Verify value type is std::pair<const std::string, double>
    auto value_type = meta.value_type();
    EXPECT_EQ(value_type, entt::resolve<std::pair<const std::string, double>>());
}

// Test construction with an empty container
TEST_F(MetaAssociativeContainerTest, EmptyContainer) {
    std::map<int, char> empty_map;
    entt::meta_associative_container meta(empty_map);

    // Even empty, the type information should be correct
    EXPECT_TRUE(meta.key_only_container());
    EXPECT_EQ(meta.key_type(), entt::resolve<int>());
    EXPECT_EQ(meta.mapped_type(), entt::resolve<char>());
    EXPECT_EQ(meta.value_type(), entt::resolve<std::pair<const int, char>>());
}

// Test construction with a single-element container
TEST_F(MetaAssociativeContainerTest, SingleElementContainer) {
    std::set<std::string> single = {"only"};
    entt::meta_associative_container meta(single);

    EXPECT_FALSE(meta.key_only_container());
    EXPECT_EQ(meta.key_type(), entt::resolve<std::string>());
    EXPECT_EQ(meta.value_type(), entt::resolve<std::string>());
    EXPECT_EQ(meta.mapped_type(), entt::resolve<void>());
}

// Test construction with a container that has custom comparator (still key-only)
TEST_F(MetaAssociativeContainerTest, CustomComparatorKeyOnly) {
    struct CustomCompare {
        bool operator()(int a, int b) const { return a < b; }
    };
    std::set<int, CustomCompare> container = {10, 20, 30};
    entt::meta_associative_container meta(container);

    EXPECT_FALSE(meta.key_only_container());
    EXPECT_EQ(meta.key_type(), entt::resolve<int>());
    EXPECT_EQ(meta.value_type(), entt::resolve<int>());
    EXPECT_EQ(meta.mapped_type(), entt::resolve<void>());
}

// Test construction with std::unordered_multimap (key-value with multiple keys)
TEST_F(MetaAssociativeContainerTest, MultiMapContainer) {
    std::unordered_multimap<int, std::string> container = {{1, "one"}, {1, "uno"}, {2, "two"}};
    entt::meta_associative_container meta(container);

    EXPECT_TRUE(meta.key_only_container());
    EXPECT_EQ(meta.key_type(), entt::resolve<int>());
    EXPECT_EQ(meta.mapped_type(), entt::resolve<std::string>());
    EXPECT_EQ(meta.value_type(), entt::resolve<std::pair<const int, std::string>>());
}

// Test that the proxy functions are correctly bound (verify they can be called)
TEST_F(MetaAssociativeContainerTest, ProxyFunctionsCallable) {
    std::map<int, double> container = {{1, 1.5}, {2, 2.5}};
    entt::meta_associative_container meta(container);

    // The proxy functions should be callable and return correct types
    auto key_type = meta.key_type();
    auto mapped_type = meta.mapped_type();
    auto value_type = meta.value_type();

    EXPECT_TRUE(key_type);
    EXPECT_TRUE(mapped_type);
    EXPECT_TRUE(value_type);

    EXPECT_EQ(key_type, entt::resolve<int>());
    EXPECT_EQ(mapped_type, entt::resolve<double>());
    EXPECT_EQ(value_type, entt::resolve<std::pair<const int, double>>());
}

// Test that the key_only_container flag is correctly set for different container types
TEST_F(MetaAssociativeContainerTest, KeyOnlyFlagCorrectness) {
    // Key-only containers should have key_only_container = false
    std::set<int> set_container;
    entt::meta_associative_container set_meta(set_container);
    EXPECT_FALSE(set_meta.key_only_container());

    // Key-value containers should have key_only_container = true
    std::map<int, int> map_container;
    entt::meta_associative_container map_meta(map_container);
    EXPECT_TRUE(map_meta.key_only_container());

    // std::unordered_set is key-only
    std::unordered_set<std::string> uset_container;
    entt::meta_associative_container uset_meta(uset_container);
    EXPECT_FALSE(uset_meta.key_only_container());

    // std::unordered_map is key-value
    std::unordered_map<std::string, int> umap_container;
    entt::meta_associative_container umap_meta(umap_container);
    EXPECT_TRUE(umap_meta.key_only_container());
}

// Test with a container that has non-default allocator
TEST_F(MetaAssociativeContainerTest, CustomAllocatorContainer) {
    using CustomAlloc = std::allocator<std::pair<const int, double>>;
    std::map<int, double, std::less<int>, CustomAlloc> container;
    container[1] = 1.0;
    container[2] = 2.0;

    entt::meta_associative_container meta(container);
    EXPECT_TRUE(meta.key_only_container());
    EXPECT_EQ(meta.key_type(), entt::resolve<int>());
    EXPECT_EQ(meta.mapped_type(), entt::resolve<double>());
    EXPECT_EQ(meta.value_type(), entt::resolve<std::pair<const int, double>>());
}

// Test that the constructor works with const containers
TEST_F(MetaAssociativeContainerTest, ConstContainer) {
    const std::set<int> const_set = {1, 2, 3};
    entt::meta_associative_container meta(const_set);

    EXPECT_FALSE(meta.key_only_container());
    EXPECT_EQ(meta.key_type(), entt::resolve<int>());
    EXPECT_EQ(meta.value_type(), entt::resolve<int>());
    EXPECT_EQ(meta.mapped_type(), entt::resolve<void>());
}

// Test that the constructor works with mutable containers (non-const)
TEST_F(MetaAssociativeContainerTest, MutableContainer) {
    std::set<int> mutable_set = {1, 2, 3};
    entt::meta_associative_container meta(mutable_set);

    EXPECT_FALSE(meta.key_only_container());
    EXPECT_EQ(meta.key_type(), entt::resolve<int>());
    EXPECT_EQ(meta.value_type(), entt::resolve<int>());
    EXPECT_EQ(meta.mapped_type(), entt::resolve<void>());
}

// Test with std::multiset (key-only, allows duplicates)
TEST_F(MetaAssociativeContainerTest, MultiSetContainer) {
    std::multiset<int> container = {1, 1, 2, 3};
    entt::meta_associative_container meta(container);

    EXPECT_FALSE(meta.key_only_container());
    EXPECT_EQ(meta.key_type(), entt::resolve<int>());
    EXPECT_EQ(meta.value_type(), entt::resolve<int>());
    EXPECT_EQ(meta.mapped_type(), entt::resolve<void>());
}

// Test that the type functions return the correct meta types for complex types
TEST_F(MetaAssociativeContainerTest, ComplexValueTypes) {
    std::map<int, std::vector<double>> container;
    container[0] = {1.0, 2.0};
    entt::meta_associative_container meta(container);

    EXPECT_TRUE(meta.key_only_container());
    EXPECT_EQ(meta.key_type(), entt::resolve<int>());
    EXPECT_EQ(meta.mapped_type(), entt::resolve<std::vector<double>>());
    EXPECT_EQ(meta.value_type(), entt::resolve<std::pair<const int, std::vector<double>>>());
}