#include <gtest/gtest.h>
#include <entt/entt.hpp>
#include <unordered_map>
#include <vector>
#include <set>
#include <type_traits>

// Test fixture for meta_associative_container construction and basic operations
class MetaAssociativeContainerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Register types for meta system
        entt::meta<int>()
            .type("int"_hs)
            .conv<int>();

        entt::meta<double>()
            .type("double"_hs)
            .conv<double>();

        entt::meta<std::string>()
            .type("string"_hs)
            .conv<std::string>();
    }
};

// Test construction from key-value associative container (std::unordered_map)
TEST_F(MetaAssociativeContainerTest, ConstructFromMap) {
    std::unordered_map<int, double> map{{1, 1.5}, {2, 2.5}, {3, 3.5}};
    entt::meta_associative_container container{map};

    // Verify it's not key-only
    EXPECT_FALSE(container.key_only_container);

    // Verify key type is int
    auto key_type = container.key_type();
    ASSERT_TRUE(key_type);
    EXPECT_EQ(key_type, entt::resolve<int>());

    // Verify mapped type is double
    auto mapped_type = container.mapped_type();
    ASSERT_TRUE(mapped_type);
    EXPECT_EQ(mapped_type, entt::resolve<double>());

    // Verify value type is std::pair<const int, double>
    auto value_type = container.value_type();
    ASSERT_TRUE(value_type);
    EXPECT_EQ(value_type, entt::resolve<std::pair<const int, double>>());

    // Verify size
    EXPECT_EQ(container.size(), 3u);

    // Verify iteration
    std::vector<std::pair<int, double>> elements;
    for (auto [key, value] : container) {
        elements.emplace_back(key.cast<int>(), value.cast<double>());
    }
    EXPECT_EQ(elements.size(), 3u);
    EXPECT_EQ(elements[0], std::make_pair(1, 1.5));
    EXPECT_EQ(elements[1], std::make_pair(2, 2.5));
    EXPECT_EQ(elements[2], std::make_pair(3, 3.5));
}

// Test construction from key-only associative container (std::set)
TEST_F(MetaAssociativeContainerTest, ConstructFromSet) {
    std::set<int> set{10, 20, 30};
    entt::meta_associative_container container{set};

    // Verify it's key-only
    EXPECT_TRUE(container.key_only_container);

    // Verify key type is int
    auto key_type = container.key_type();
    ASSERT_TRUE(key_type);
    EXPECT_EQ(key_type, entt::resolve<int>());

    // Verify mapped type is void (no mapped type for key-only)
    auto mapped_type = container.mapped_type();
    EXPECT_FALSE(mapped_type);

    // Verify value type is int (same as key for key-only)
    auto value_type = container.value_type();
    ASSERT_TRUE(value_type);
    EXPECT_EQ(value_type, entt::resolve<int>());

    // Verify size
    EXPECT_EQ(container.size(), 3u);

    // Verify iteration
    std::vector<int> elements;
    for (auto [key, value] : container) {
        elements.push_back(key.cast<int>());
        EXPECT_FALSE(value); // value should be invalid for key-only
    }
    EXPECT_EQ(elements.size(), 3u);
    EXPECT_EQ(elements[0], 10);
    EXPECT_EQ(elements[1], 20);
    EXPECT_EQ(elements[2], 30);
}

// Test construction from empty associative container
TEST_F(MetaAssociativeContainerTest, ConstructFromEmptyContainer) {
    std::unordered_map<std::string, int> empty_map;
    entt::meta_associative_container container{empty_map};

    EXPECT_FALSE(container.key_only_container);
    EXPECT_EQ(container.size(), 0u);

    // Verify types are still accessible even when empty
    auto key_type = container.key_type();
    ASSERT_TRUE(key_type);
    EXPECT_EQ(key_type, entt::resolve<std::string>());

    auto mapped_type = container.mapped_type();
    ASSERT_TRUE(mapped_type);
    EXPECT_EQ(mapped_type, entt::resolve<int>());

    // Verify iteration over empty container yields nothing
    for (auto [key, value] : container) {
        FAIL() << "Should not iterate over empty container";
    }
}

// Test construction from empty key-only container
TEST_F(MetaAssociativeContainerTest, ConstructFromEmptyKeyOnlyContainer) {
    std::set<double> empty_set;
    entt::meta_associative_container container{empty_set};

    EXPECT_TRUE(container.key_only_container);
    EXPECT_EQ(container.size(), 0u);

    auto key_type = container.key_type();
    ASSERT_TRUE(key_type);
    EXPECT_EQ(key_type, entt::resolve<double>());

    auto mapped_type = container.mapped_type();
    EXPECT_FALSE(mapped_type);

    for (auto [key, value] : container) {
        FAIL() << "Should not iterate over empty container";
    }
}

// Test construction from container with single element
TEST_F(MetaAssociativeContainerTest, ConstructFromSingleElementContainer) {
    std::unordered_map<int, std::string> single{{42, "answer"}};
    entt::meta_associative_container container{single};

    EXPECT_FALSE(container.key_only_container);
    EXPECT_EQ(container.size(), 1u);

    for (auto [key, value] : container) {
        EXPECT_EQ(key.cast<int>(), 42);
        EXPECT_EQ(value.cast<std::string>(), "answer");
    }
}

// Test construction from container with custom key type (string)
TEST_F(MetaAssociativeContainerTest, ConstructFromStringKeyMap) {
    std::unordered_map<std::string, int> map{{"one", 1}, {"two", 2}};
    entt::meta_associative_container container{map};

    EXPECT_FALSE(container.key_only_container);
    EXPECT_EQ(container.size(), 2u);

    auto key_type = container.key_type();
    ASSERT_TRUE(key_type);
    EXPECT_EQ(key_type, entt::resolve<std::string>());

    auto mapped_type = container.mapped_type();
    ASSERT_TRUE(mapped_type);
    EXPECT_EQ(mapped_type, entt::resolve<int>());
}

// Test that construction works with const containers
TEST_F(MetaAssociativeContainerTest, ConstructFromConstContainer) {
    const std::unordered_map<int, double> const_map{{1, 1.0}, {2, 2.0}};
    entt::meta_associative_container container{const_map};

    EXPECT_FALSE(container.key_only_container);
    EXPECT_EQ(container.size(), 2u);

    // Verify we can still iterate over const container
    int count = 0;
    for ([[maybe_unused]] auto [key, value] : container) {
        ++count;
    }
    EXPECT_EQ(count, 2);
}

// Test that construction works with different associative container types
TEST_F(MetaAssociativeContainerTest, ConstructFromDifferentContainerTypes) {
    // std::map
    std::map<int, double> map{{1, 1.0}};
    entt::meta_associative_container container_from_map{map};
    EXPECT_FALSE(container_from_map.key_only_container);
    EXPECT_EQ(container_from_map.size(), 1u);

    // std::multimap
    std::multimap<int, double> multimap{{1, 1.0}, {1, 2.0}};
    entt::meta_associative_container container_from_multimap{multimap};
    EXPECT_FALSE(container_from_multimap.key_only_container);
    EXPECT_EQ(container_from_multimap.size(), 2u);

    // std::unordered_multimap
    std::unordered_multimap<int, double> unordered_multimap{{1, 1.0}, {1, 2.0}};
    entt::meta_associative_container container_from_unordered_multimap{unordered_multimap};
    EXPECT_FALSE(container_from_unordered_multimap.key_only_container);
    EXPECT_EQ(container_from_unordered_multimap.size(), 2u);

    // std::multiset (key-only)
    std::multiset<int> multiset{1, 1, 2};
    entt::meta_associative_container container_from_multiset{multiset};
    EXPECT_TRUE(container_from_multiset.key_only_container);
    EXPECT_EQ(container_from_multiset.size(), 3u);

    // std::unordered_multiset (key-only)
    std::unordered_multiset<int> unordered_multiset{1, 1, 2};
    entt::meta_associative_container container_from_unordered_multiset{unordered_multiset};
    EXPECT_TRUE(container_from_unordered_multiset.key_only_container);
    EXPECT_EQ(container_from_unordered_multiset.size(), 3u);
}

// Test that the proxy functions are correctly set up
TEST_F(MetaAssociativeContainerTest, ProxyFunctionsAreCallable) {
    std::unordered_map<int, double> map{{1, 1.0}};
    entt::meta_associative_container container{map};

    // Verify that key_type_fn works
    EXPECT_NE(container.key_type_fn, nullptr);
    auto key_type_result = container.key_type_fn(entt::forward_as_meta(map));
    EXPECT_TRUE(key_type_result);
    EXPECT_EQ(key_type_result, entt::resolve<int>());

    // Verify that mapped_type_fn works
    EXPECT_NE(container.mapped_type_fn, nullptr);
    auto mapped_type_result = container.mapped_type_fn(entt::forward_as_meta(map));
    EXPECT_TRUE(mapped_type_result);
    EXPECT_EQ(mapped_type_result, entt::resolve<double>());

    // Verify that value_type_fn works
    EXPECT_NE(container.value_type_fn, nullptr);
    auto value_type_result = container.value_type_fn(entt::forward_as_meta(map));
    EXPECT_TRUE(value_type_result);
    EXPECT_EQ(value_type_result, entt::resolve<std::pair<const int, double>>());
}

// Test that key-only container proxy functions return correct types
TEST_F(MetaAssociativeContainerTest, KeyOnlyProxyFunctions) {
    std::set<int> set{1, 2, 3};
    entt::meta_associative_container container{set};

    EXPECT_NE(container.key_type_fn, nullptr);
    auto key_type_result = container.key_type_fn(entt::forward_as_meta(set));
    EXPECT_TRUE(key_type_result);
    EXPECT_EQ(key_type_result, entt::resolve<int>());

    // For key-only containers, mapped_type_fn should return invalid meta type
    EXPECT_NE(container.mapped_type_fn, nullptr);
    auto mapped_type_result = container.mapped_type_fn(entt::forward_as_meta(set));
    EXPECT_FALSE(mapped_type_result);

    // For key-only containers, value_type_fn should return the key type
    EXPECT_NE(container.value_type_fn, nullptr);
    auto value_type_result = container.value_type_fn(entt::forward_as_meta(set));
    EXPECT_TRUE(value_type_result);
    EXPECT_EQ(value_type_result, entt::resolve<int>());
}