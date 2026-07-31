#include <gtest/gtest.h>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

// Include the necessary EnTT public headers
#include <entt/core/type_info.hpp>
#include <entt/meta/container.hpp>
#include <entt/meta/factory.hpp>
#include <entt/meta/meta.hpp>
#include <entt/meta/resolve.hpp>

// Helper struct to register types for testing
struct MyKey {};
struct MyValue {};

// Register types to make them meta-accessible
void register_test_types() {
    entt::meta<MyKey>().type("MyKey"_hs);
    entt::meta<MyValue>().type("MyValue"_hs);
    entt::meta<int>().type("int"_hs);
    entt::meta<std::string>().type("string"_hs);
}

// Test fixture for meta_associative_container tests
class MetaAssociativeContainerTest : public ::testing::Test {
protected:
    void SetUp() override {
        register_test_types();
    }

    void TearDown() override {
        entt::internal::meta_container_traits<int>::unreg();
        entt::internal::meta_container_traits<std::string>::unreg();
        entt::internal::meta_container_traits<MyKey>::unreg();
        entt::internal::meta_container_traits<MyValue>::unreg();
    }
};

// Test case for a standard std::map (key-value container)
TEST_F(MetaAssociativeContainerTest, StdMapContainer) {
    std::map<int, std::string> container;
    entt::meta_associative_container meta_container{container};

    // Assert that it's not a key-only container
    ASSERT_FALSE(meta_container.key_only_container);

    // Assert key type
    ASSERT_TRUE(meta_container.key_type_fn);
    ASSERT_EQ(meta_container.key_type_fn(), entt::resolve<int>());

    // Assert mapped type
    ASSERT_TRUE(meta_container.mapped_type_fn);
    ASSERT_EQ(meta_container.mapped_type_fn(), entt::resolve<std::string>());

    // Assert value type (std::pair<const Key, Value>)
    ASSERT_TRUE(meta_container.value_type_fn);
    ASSERT_EQ(meta_container.value_type_fn(), entt::resolve<std::pair<const int, std::string>>());
}

// Test case for a std::set (key-only container)
TEST_F(MetaAssociativeContainerTest, StdSetContainer) {
    std::set<MyKey> container;
    entt::meta_associative_container meta_container{container};

    // Assert that it is a key-only container
    ASSERT_TRUE(meta_container.key_only_container);

    // Assert key type
    ASSERT_TRUE(meta_container.key_type_fn);
    ASSERT_EQ(meta_container.key_type_fn(), entt::resolve<MyKey>());

    // Assert mapped type is invalid for key-only containers
    ASSERT_FALSE(meta_container.mapped_type_fn);
    ASSERT_EQ(meta_container.mapped_type_fn(), entt::meta_type{});

    // Assert value type (which is the key type for key-only containers)
    ASSERT_TRUE(meta_container.value_type_fn);
    ASSERT_EQ(meta_container.value_type_fn(), entt::resolve<MyKey>());
}

// Test case for std::multimap (another key-value container)
TEST_F(MetaAssociativeContainerTest, StdMultiMapContainer) {
    std::multimap<std::string, MyValue> container;
    entt::meta_associative_container meta_container{container};

    ASSERT_FALSE(meta_container.key_only_container);
    ASSERT_TRUE(meta_container.key_type_fn);
    ASSERT_EQ(meta_container.key_type_fn(), entt::resolve<std::string>());
    ASSERT_TRUE(meta_container.mapped_type_fn);
    ASSERT_EQ(meta_container.mapped_type_fn(), entt::resolve<MyValue>());
    ASSERT_TRUE(meta_container.value_type_fn);
    ASSERT_EQ(meta_container.value_type_fn(), entt::resolve<std::pair<const std::string, MyValue>>());
}

// Test case for std::multiset (another key-only container)
TEST_F(MetaAssociativeContainerTest, StdMultiSetContainer) {
    std::multiset<int> container;
    entt::meta_associative_container meta_container{container};

    ASSERT_TRUE(meta_container.key_only_container);
    ASSERT_TRUE(meta_container.key_type_fn);
    ASSERT_EQ(meta_container.key_type_fn(), entt::resolve<int>());
    ASSERT_FALSE(meta_container.mapped_type_fn);
    ASSERT_EQ(meta_container.mapped_type_fn(), entt::meta_type{});
    ASSERT_TRUE(meta_container.value_type_fn);
    ASSERT_EQ(meta_container.value_type_fn(), entt::resolve<int>());
}

// Test with an empty container (should not affect type resolution)
TEST_F(MetaAssociativeContainerTest, EmptyMapContainer) {
    std::map<int, std::string> container; // Empty map
    entt::meta_associative_container meta_container{container};

    ASSERT_FALSE(meta_container.key_only_container);
    ASSERT_TRUE(meta_container.key_type_fn);
    ASSERT_EQ(meta_container.key_type_fn(), entt::resolve<int>());
    ASSERT_TRUE(meta_container.mapped_type_fn);
    ASSERT_EQ(meta_container.mapped_type_fn(), entt::resolve<std::string>());
    ASSERT_TRUE(meta_container.value_type_fn);
    ASSERT_EQ(meta_container.value_type_fn(), entt::resolve<std::pair<const int, std::string>>());
}

// Test with an empty set container
TEST_F(MetaAssociativeContainerTest, EmptySetContainer) {
    std::set<MyKey> container; // Empty set
    entt::meta_associative_container meta_container{container};

    ASSERT_TRUE(meta_container.key_only_container);
    ASSERT_TRUE(meta_container.key_type_fn);
    ASSERT_EQ(meta_container.key_type_fn(), entt::resolve<MyKey>());
    ASSERT_FALSE(meta_container.mapped_type_fn);
    ASSERT_EQ(meta_container.mapped_type_fn(), entt::meta_type{});
    ASSERT_TRUE(meta_container.value_type_fn);
    ASSERT_EQ(meta_container.value_type_fn(), entt::resolve<MyKey>());
}

// Test with a custom associative container that mimics std::map
namespace Custom {
    template<typename K, typename V>
    struct MyCustomMap {
        using key_type = K;
        using mapped_type = V;
        using value_type = std::pair<const K, V>; // Required for associative containers
        // Minimal interface to be recognized by EnTT's meta_container traits
        auto begin() { return std::map<K,V>{}.begin(); }
        auto end() { return std::map<K,V>{}.end(); }
        auto begin() const { return std::map<K,V>{}.begin(); }
        auto end() const { return std::map<K,V>{}.end(); }
        size_t size() const { return 0; }
        bool empty() const { return true; }
    };
}

TEST_F(MetaAssociativeContainerTest, CustomMapContainer) {
    Custom::MyCustomMap<int, MyValue> container;
    entt::meta_associative_container meta_container{container};

    ASSERT_FALSE(meta_container.key_only_container);
    ASSERT_TRUE(meta_container.key_type_fn);
    ASSERT_EQ(meta_container.key_type_fn(), entt::resolve<int>());
    ASSERT_TRUE(meta_container.mapped_type_fn);
    ASSERT_EQ(meta_container.mapped_type_fn(), entt::resolve<MyValue>());
    ASSERT_TRUE(meta_container.value_type_fn);
    ASSERT_EQ(meta_container.value_type_fn(), entt::resolve<std::pair<const int, MyValue>>());
}

// Test with a custom associative container that mimics std::set
namespace Custom {
    template<typename K>
    struct MyCustomSet {
        using key_type = K;
        using value_type = K; // Required for key-only associative containers
        // Minimal interface
        auto begin() { return std::set<K>{}.begin(); }
        auto end() { return std::set<K>{}.end(); }
        auto begin() const { return std::set<K>{}.begin(); }
        auto end() const { return std::set<K>{}.end(); }
        size_t size() const { return 0; }
        bool empty() const { return true; }
    };
}

TEST_F(MetaAssociativeContainerTest, CustomSetContainer) {
    Custom::MyCustomSet<std::string> container;
    entt::meta_associative_container meta_container{container};

    ASSERT_TRUE(meta_container.key_only_container);
    ASSERT_TRUE(meta_container.key_type_fn);
    ASSERT_EQ(meta_container.key_type_fn(), entt::resolve<std::string>());
    ASSERT_FALSE(meta_container.mapped_type_fn); // Should be null for key-only
    ASSERT_EQ(meta_container.mapped_type_fn(), entt::meta_type{});
    ASSERT_TRUE(meta_container.value_type_fn);
    ASSERT_EQ(meta_container.value_type_fn(), entt::resolve<std::string>());
}

// Test with a non-associative container (e.g., std::vector)
// This should technically not compile if the constructor is properly constrained,
// but if it were to compile, the behavior should reflect the lack of associative traits.
// However, the focal function is templated and relies on SFINAE or concepts
// to only accept associative containers. We'll test that it correctly identifies
// the traits for an associative container, not that it handles non-associative ones.
// The current implementation of meta_associative_container constructor is
// `template<typename Type> meta_associative_container(Type &container)`
// and relies on `is_key_only_meta_associative_container_v<Type>` and similar traits.
// If `Type` is not an associative container, these traits would be false/invalid,
// leading to potentially incorrect or uninitialized function pointers.
// The correct usage implies `Type` *is* an associative container.
// We'll assume the user provides a valid associative container.
// If we tried to pass a `std::vector`, it would likely fail to compile
// due to missing `key_type`, `mapped_type`, etc., which is the desired behavior.
// Therefore, we focus on valid associative containers.