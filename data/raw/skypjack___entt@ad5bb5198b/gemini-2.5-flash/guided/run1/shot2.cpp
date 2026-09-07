#include <gtest/gtest.h>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <entt/core/type_info.hpp>
#include <entt/meta/container.hpp>
#include <entt/meta/factory.hpp>
#include <entt/meta/meta.hpp>
#include <entt/meta/resolve.hpp>
#include <entt/meta/type_traits.hpp>

// Helper struct to register types for testing
struct MyKey {};
struct MyValue {};

// Define a simple key-only container for testing
namespace entt {
template<>
struct is_key_only_meta_associative_container<std::set<int>> : std::true_type {};
} // namespace entt

TEST(MetaAssociativeContainerTest, KeyOnlyContainerDetection) {
    // Register types to make them discoverable by EnTT's meta system
    entt::meta<int>().type("int");
    entt::meta<std::set<int>>().type("std::set<int>");
    entt::meta<std::map<int, std::string>>().type("std::map<int, std::string>");

    // Test a key-only container (std::set<int>)
    std::set<int> set_container;
    auto meta_set_type = entt::resolve<std::set<int>>();
    ASSERT_TRUE(meta_set_type);
    ASSERT_TRUE(meta_set_type.is_associative_container());

    auto meta_set_container = meta_set_type.as_associative_container();
    ASSERT_TRUE(meta_set_container);
    ASSERT_TRUE(meta_set_container.key_only()); // Should be true due to specialization

    // Test a key-value container (std::map<int, std::string>)
    std::map<int, std::string> map_container;
    auto meta_map_type = entt::resolve<std::map<int, std::string>>();
    ASSERT_TRUE(meta_map_type);
    ASSERT_TRUE(meta_map_type.is_associative_container());

    auto meta_map_container = meta_map_type.as_associative_container();
    ASSERT_TRUE(meta_map_container);
    ASSERT_FALSE(meta_map_container.key_only()); // Should be false by default
}

TEST(MetaAssociativeContainerTest, KeyAndValueTypeRetrieval) {
    // Register types for testing
    entt::meta<int>().type("int");
    entt::meta<std::string>().type("std::string");
    entt::meta<MyKey>().type("MyKey");
    entt::meta<MyValue>().type("MyValue");
    entt::meta<std::map<int, std::string>>().type("std::map<int, std::string>");
    entt::meta<std::unordered_map<MyKey, MyValue>>().type("std::unordered_map<MyKey, MyValue>");
    entt::meta<std::set<int>>().type("std::set<int>");

    // Test std::map<int, std::string>
    std::map<int, std::string> map_container;
    auto meta_map_type = entt::resolve<std::map<int, std::string>>();
    ASSERT_TRUE(meta_map_type);
    auto meta_map_container = meta_map_type.as_associative_container();
    ASSERT_TRUE(meta_map_container);

    ASSERT_EQ(meta_map_container.key_type(), entt::resolve<int>());
    ASSERT_EQ(meta_map_container.mapped_type(), entt::resolve<std::string>());
    // For key-value containers, value_type is std::pair<const Key, Value>
    ASSERT_EQ(meta_map_container.value_type(), entt::resolve<std::pair<const int, std::string>>());

    // Test std::unordered_map<MyKey, MyValue>
    std::unordered_map<MyKey, MyValue> unordered_map_container;
    auto meta_unordered_map_type = entt::resolve<std::unordered_map<MyKey, MyValue>>();
    ASSERT_TRUE(meta_unordered_map_type);
    auto meta_unordered_map_container = meta_unordered_map_type.as_associative_container();
    ASSERT_TRUE(meta_unordered_map_container);

    ASSERT_EQ(meta_unordered_map_container.key_type(), entt::resolve<MyKey>());
    ASSERT_EQ(meta_unordered_map_container.mapped_type(), entt::resolve<MyValue>());
    ASSERT_EQ(meta_unordered_map_container.value_type(), entt::resolve<std::pair<const MyKey, MyValue>>());

    // Test std::set<int> (key-only container)
    std::set<int> set_container;
    auto meta_set_type = entt::resolve<std::set<int>>();
    ASSERT_TRUE(meta_set_type);
    auto meta_set_container = meta_set_type.as_associative_container();
    ASSERT_TRUE(meta_set_container);

    ASSERT_EQ(meta_set_container.key_type(), entt::resolve<int>());
    ASSERT_EQ(meta_set_container.mapped_type(), entt::meta_type{}); // Key-only containers have no mapped type
    ASSERT_EQ(meta_set_container.value_type(), entt::resolve<int>()); // For key-only, value_type is Key
}

TEST(MetaAssociativeContainerTest, NonAssociativeContainer) {
    // Register a non-associative container type
    entt::meta<std::vector<int>>().type("std::vector<int>");

    auto meta_vector_type = entt::resolve<std::vector<int>>();
    ASSERT_TRUE(meta_vector_type);
    ASSERT_FALSE(meta_vector_type.is_associative_container());

    // Attempting to get as_associative_container should return an invalid handle
    auto meta_vector_container = meta_vector_type.as_associative_container();
    ASSERT_FALSE(meta_vector_container);

    // Calling methods on an invalid handle should return default/invalid values
    ASSERT_FALSE(meta_vector_container.key_only());
    ASSERT_EQ(meta_vector_container.key_type(), entt::meta_type{});
    ASSERT_EQ(meta_vector_container.mapped_type(), entt::meta_type{});
    ASSERT_EQ(meta_vector_container.value_type(), entt::meta_type{});
}

TEST(MetaAssociativeContainerTest, UnregisteredTypes) {
    // Do not register MyKey or MyValue
    struct UnregisteredKey {
        // Provide operator== for std::pair comparison
        bool operator==(const UnregisteredKey&) const { return true; }
    };
    struct UnregisteredValue {
        // Provide operator== for std::pair comparison
        bool operator==(const UnregisteredValue&) const { return true; }
    };

    // Register the map type, but not its key/value types
    entt::meta<std::map<UnregisteredKey, UnregisteredValue>>().type("std::map<UnregisteredKey, UnregisteredValue>");

    std::map<UnregisteredKey, UnregisteredValue> container;
    auto meta_type = entt::resolve<std::map<UnregisteredKey, UnregisteredValue>>();
    ASSERT_TRUE(meta_type);
    ASSERT_TRUE(meta_type.is_associative_container());

    auto meta_container = meta_type.as_associative_container();
    ASSERT_TRUE(meta_container);

    // The key_type and mapped_type should still be resolved correctly,
    // even if the types themselves aren't explicitly registered with entt::meta<T>().type("Name")
    // because entt::resolve<T>() can still find their type_info.
    ASSERT_EQ(meta_container.key_type(), entt::resolve<UnregisteredKey>());
    ASSERT_EQ(meta_container.mapped_type(), entt::resolve<UnregisteredValue>());
    ASSERT_EQ(meta_container.value_type(), entt::resolve<std::pair<const UnregisteredKey, UnregisteredValue>>());

    // Verify that the resolved types are indeed 'valid' meta_types,
    // even if they don't have a user-defined name or properties.
    ASSERT_TRUE(meta_container.key_type());
    ASSERT_TRUE(meta_container.mapped_type());
    ASSERT_TRUE(meta_container.value_type());
}