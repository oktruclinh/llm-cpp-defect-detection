#include <gtest/gtest.h>
#include <entt/entt.hpp>
#include <type_traits>
#include <unordered_map>
#include <vector>

// Test fixture for meta_associative_container construction
class MetaAssociativeContainerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize the meta context
        entt::meta_ctx ctx;
        entt::meta_ctx::bind(ctx);
    }
};

// Test construction with a key-only associative container (std::set-like)
TEST_F(MetaAssociativeContainerTest, KeyOnlyContainer) {
    std::set<int> container = {1, 2, 3};
    entt::meta_associative_container mac(container);

    // key_only_container should be false for std::set (it's key-only)
    EXPECT_FALSE(mac.key_only_container);

    // Verify key_type returns int
    ASSERT_NE(mac.key_type_fn, nullptr);
    auto key_type = mac.key_type_fn();
    EXPECT_EQ(key_type, entt::resolve<int>());

    // mapped_type_fn should return void for key-only containers
    ASSERT_NE(mac.mapped_type_fn, nullptr);
    auto mapped_type = mac.mapped_type_fn();
    EXPECT_EQ(mapped_type, entt::resolve<void>());

    // value_type_fn should return std::pair<const int, void> or similar
    ASSERT_NE(mac.value_type_fn, nullptr);
    auto value_type = mac.value_type_fn();
    EXPECT_NE(value_type, entt::resolve<void>());
}

// Test construction with a key-value associative container (std::map-like)
TEST_F(MetaAssociativeContainerTest, KeyValueContainer) {
    std::map<std::string, double> container = {{"pi", 3.14}, {"e", 2.718}};
    entt::meta_associative_container mac(container);

    // key_only_container should be true for std::map (it's not key-only)
    EXPECT_TRUE(mac.key_only_container);

    // Verify key_type returns std::string
    ASSERT_NE(mac.key_type_fn, nullptr);
    auto key_type = mac.key_type_fn();
    EXPECT_EQ(key_type, entt::resolve<std::string>());

    // mapped_type_fn should return double
    ASSERT_NE(mac.mapped_type_fn, nullptr);
    auto mapped_type = mac.mapped_type_fn();
    EXPECT_EQ(mapped_type, entt::resolve<double>());

    // value_type_fn should return std::pair<const std::string, double>
    ASSERT_NE(mac.value_type_fn, nullptr);
    auto value_type = mac.value_type_fn();
    EXPECT_NE(value_type, entt::resolve<void>());
}

// Test construction with an unordered associative container
TEST_F(MetaAssociativeContainerTest, UnorderedContainer) {
    std::unordered_map<int, char> container = {{1, 'a'}, {2, 'b'}};
    entt::meta_associative_container mac(container);

    EXPECT_TRUE(mac.key_only_container);
    ASSERT_NE(mac.key_type_fn, nullptr);
    EXPECT_EQ(mac.key_type_fn(), entt::resolve<int>());
    ASSERT_NE(mac.mapped_type_fn, nullptr);
    EXPECT_EQ(mac.mapped_type_fn(), entt::resolve<char>());
}

// Test construction with an empty container
TEST_F(MetaAssociativeContainerTest, EmptyContainer) {
    std::map<int, double> empty_container;
    entt::meta_associative_container mac(empty_container);

    EXPECT_TRUE(mac.key_only_container);
    ASSERT_NE(mac.key_type_fn, nullptr);
    EXPECT_EQ(mac.key_type_fn(), entt::resolve<int>());
    ASSERT_NE(mac.mapped_type_fn, nullptr);
    EXPECT_EQ(mac.mapped_type_fn(), entt::resolve<double>());
}

// Test construction with a multi-container (std::multimap)
TEST_F(MetaAssociativeContainerTest, MultiContainer) {
    std::multimap<int, std::string> container = {{1, "one"}, {1, "uno"}};
    entt::meta_associative_container mac(container);

    EXPECT_TRUE(mac.key_only_container);
    ASSERT_NE(mac.key_type_fn, nullptr);
    EXPECT_EQ(mac.key_type_fn(), entt::resolve<int>());
    ASSERT_NE(mac.mapped_type_fn, nullptr);
    EXPECT_EQ(mac.mapped_type_fn(), entt::resolve<std::string>());
}

// Test construction with a set (key-only container)
TEST_F(MetaAssociativeContainerTest, SetContainer) {
    std::set<double> container = {1.5, 2.7};
    entt::meta_associative_container mac(container);

    EXPECT_FALSE(mac.key_only_container);
    ASSERT_NE(mac.key_type_fn, nullptr);
    EXPECT_EQ(mac.key_type_fn(), entt::resolve<double>());
    ASSERT_NE(mac.mapped_type_fn, nullptr);
    EXPECT_EQ(mac.mapped_type_fn(), entt::resolve<void>());
}

// Test construction with an unordered_set (key-only unordered container)
TEST_F(MetaAssociativeContainerTest, UnorderedSetContainer) {
    std::unordered_set<char> container = {'x', 'y', 'z'};
    entt::meta_associative_container mac(container);

    EXPECT_FALSE(mac.key_only_container);
    ASSERT_NE(mac.key_type_fn, nullptr);
    EXPECT_EQ(mac.key_type_fn(), entt::resolve<char>());
    ASSERT_NE(mac.mapped_type_fn, nullptr);
    EXPECT_EQ(mac.mapped_type_fn(), entt::resolve<void>());
}

// Test that the function pointers are correctly set for different container types
TEST_F(MetaAssociativeContainerTest, FunctionPointersNotNull) {
    std::map<int, int> map_container;
    entt::meta_associative_container mac(map_container);

    EXPECT_NE(mac.key_type_fn, nullptr);
    EXPECT_NE(mac.mapped_type_fn, nullptr);
    EXPECT_NE(mac.value_type_fn, nullptr);

    std::set<int> set_container;
    entt::meta_associative_container mac2(set_container);

    EXPECT_NE(mac2.key_type_fn, nullptr);
    EXPECT_NE(mac2.mapped_type_fn, nullptr);
    EXPECT_NE(mac2.value_type_fn, nullptr);
}

// Test that key_only_container flag is correctly set for different container types
TEST_F(MetaAssociativeContainerTest, KeyOnlyFlagCorrectness) {
    // Key-value containers should have key_only_container = true
    std::map<int, int> map_container;
    entt::meta_associative_container mac1(map_container);
    EXPECT_TRUE(mac1.key_only_container);

    std::unordered_map<int, int> umap_container;
    entt::meta_associative_container mac2(umap_container);
    EXPECT_TRUE(mac2.key_only_container);

    std::multimap<int, int> mmap_container;
    entt::meta_associative_container mac3(mmap_container);
    EXPECT_TRUE(mac3.key_only_container);

    // Key-only containers should have key_only_container = false
    std::set<int> set_container;
    entt::meta_associative_container mac4(set_container);
    EXPECT_FALSE(mac4.key_only_container);

    std::unordered_set<int> uset_container;
    entt::meta_associative_container mac5(uset_container);
    EXPECT_FALSE(mac5.key_only_container);

    std::multiset<int> mset_container;
    entt::meta_associative_container mac6(mset_container);
    EXPECT_FALSE(mac6.key_only_container);
}