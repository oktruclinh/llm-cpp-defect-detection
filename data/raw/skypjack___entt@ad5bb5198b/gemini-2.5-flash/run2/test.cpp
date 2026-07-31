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
#include <entt/meta/type_traits.hpp>

// Helper struct to register types for testing
struct MyKey {
    int value;
    bool operator<(const MyKey& other) const { return value < other.value; }
    bool operator==(const MyKey& other) const { return value == other.value; }
};

struct MyValue {
    std::string name;
    bool operator==(const MyValue& other) const { return name == other.name; }
};

// Register types with EnTT's meta system
void register_meta_types() {
    entt::meta<MyKey>()
        .type("MyKey"_hs)
        .ctor<&entt::ctor<MyKey, int>>();

    entt::meta<MyValue>()
        .type("MyValue"_hs)
        .ctor<&entt::ctor<MyValue, const std::string&>>();

    entt::meta<std::map<MyKey, MyValue>>()
        .type("MyMap"_hs);

    entt::meta<std::set<MyKey>>()
        .type("MySet"_hs);

    entt::meta<std::vector<int>>()
        .type("MyVector"_hs); // Not an associative container, for negative testing
}

// Fixture for meta tests
struct MetaAssociativeContainerTest : ::testing::Test {
    void SetUp() override {
        register_meta_types();
    }

    void TearDown() override {
        entt::internal::meta_container_traits<std::map<MyKey, MyValue>>::unreg();
        entt::internal::meta_container_traits<std::set<MyKey>>::unreg();
        entt::internal::meta_container_traits<std::vector<int>>::unreg();
        entt::internal::meta_type_traits<MyKey>::unreg();
        entt::internal::meta_type_traits<MyValue>::unreg();
    }
};

TEST_F(MetaAssociativeContainerTest, MapContainerProperties) {
    std::map<MyKey, MyValue> my_map;
    entt::meta_any container_any = entt::meta_any{std::ref(my_map)};

    ASSERT_TRUE(container_any.type().is_associative_container());
    entt::meta_associative_container meta_container = container_any.type().as_associative_container();

    // Test key_only_container property
    ASSERT_FALSE(meta_container.key_only_container());

    // Test key_type_fn
    ASSERT_TRUE(meta_container.key_type() == entt::resolve<MyKey>());
    ASSERT_EQ(meta_container.key_type().id(), entt::type_id<MyKey>().id());

    // Test mapped_type_fn
    ASSERT_TRUE(meta_container.mapped_type() == entt::resolve<MyValue>());
    ASSERT_EQ(meta_container.mapped_type().id(), entt::type_id<MyValue>().id());

    // Test value_type_fn (pair type for map)
    ASSERT_TRUE(meta_container.value_type() == entt::resolve<std::pair<const MyKey, MyValue>>());
    ASSERT_EQ(meta_container.value_type().id(), entt::type_id<std::pair<const MyKey, MyValue>>().id());
}

TEST_F(MetaAssociativeContainerTest, SetContainerProperties) {
    std::set<MyKey> my_set;
    entt::meta_any container_any = entt::meta_any{std::ref(my_set)};

    ASSERT_TRUE(container_any.type().is_associative_container());
    entt::meta_associative_container meta_container = container_any.type().as_associative_container();

    // Test key_only_container property
    ASSERT_TRUE(meta_container.key_only_container());

    // Test key_type_fn
    ASSERT_TRUE(meta_container.key_type() == entt::resolve<MyKey>());
    ASSERT_EQ(meta_container.key_type().id(), entt::type_id<MyKey>().id());

    // Test mapped_type_fn for key-only container (should be invalid)
    ASSERT_FALSE(meta_container.mapped_type());
    ASSERT_EQ(meta_container.mapped_type().id(), entt::type_id<void>().id());

    // Test value_type_fn (key type for set)
    ASSERT_TRUE(meta_container.value_type() == entt::resolve<MyKey>());
    ASSERT_EQ(meta_container.value_type().id(), entt::type_id<MyKey>().id());
}

TEST_F(MetaAssociativeContainerTest, EmptyContainer) {
    std::map<MyKey, MyValue> empty_map;
    entt::meta_any container_any = entt::meta_any{std::ref(empty_map)};

    ASSERT_TRUE(container_any.type().is_associative_container());
    entt::meta_associative_container meta_container = container_any.type().as_associative_container();

    // Properties should still be valid even if the container is empty
    ASSERT_FALSE(meta_container.key_only_container());
    ASSERT_TRUE(meta_container.key_type() == entt::resolve<MyKey>());
    ASSERT_TRUE(meta_container.mapped_type() == entt::resolve<MyValue>());
    ASSERT_TRUE(meta_container.value_type() == entt::resolve<std::pair<const MyKey, MyValue>>());
}

TEST_F(MetaAssociativeContainerTest, NonAssociativeContainer) {
    std::vector<int> my_vector;
    entt::meta_any container_any = entt::meta_any{std::ref(my_vector)};

    // A vector is not an associative container
    ASSERT_FALSE(container_any.type().is_associative_container());

    // Attempting to call as_associative_container() on a non-associative type
    // should result in an invalid meta_associative_container object.
    // The constructor of meta_associative_container is templated and takes a Type&,
    // so it's not directly callable with a non-associative type in the same way
    // as the public API. The public API path is through meta_type::as_associative_container().
    entt::meta_associative_container meta_container = container_any.type().as_associative_container();

    ASSERT_FALSE(static_cast<bool>(meta_container)); // Check if the object is valid
    ASSERT_FALSE(meta_container.key_only_container()); // Default constructed should return false
    ASSERT_FALSE(meta_container.key_type());
    ASSERT_FALSE(meta_container.mapped_type());
    ASSERT_FALSE(meta_container.value_type());
}

TEST_F(MetaAssociativeContainerTest, ConstContainer) {
    const std::map<MyKey, MyValue> const_map;
    entt::meta_any container_any = entt::meta_any{std::ref(const_map)};

    ASSERT_TRUE(container_any.type().is_associative_container());
    entt::meta_associative_container meta_container = container_any.type().as_associative_container();

    // Properties should be the same for const containers
    ASSERT_FALSE(meta_container.key_only_container());
    ASSERT_TRUE(meta_container.key_type() == entt::resolve<MyKey>());
    ASSERT_TRUE(meta_container.mapped_type() == entt::resolve<MyValue>());
    ASSERT_TRUE(meta_container.value_type() == entt::resolve<std::pair<const MyKey, MyValue>>());
}

TEST_F(MetaAssociativeContainerTest, DifferentKeyAndValueTypes) {
    // Test with a map of different key/value types
    entt::meta<std::map<int, std::string>>()
        .type("IntStringMap"_hs);

    std::map<int, std::string> int_string_map;
    entt::meta_any container_any = entt::meta_any{std::ref(int_string_map)};

    ASSERT_TRUE(container_any.type().is_associative_container());
    entt::meta_associative_container meta_container = container_any.type().as_associative_container();

    ASSERT_FALSE(meta_container.key_only_container());
    ASSERT_TRUE(meta_container.key_type() == entt::resolve<int>());
    ASSERT_TRUE(meta_container.mapped_type() == entt::resolve<std::string>());
    ASSERT_TRUE(meta_container.value_type() == entt::resolve<std::pair<const int, std::string>>());

    entt::internal::meta_container_traits<std::map<int, std::string>>::unreg();
}

TEST_F(MetaAssociativeContainerTest, UnorderedMapContainerProperties) {
    entt::meta<std::unordered_map<MyKey, MyValue>>()
        .type("MyUnorderedMap"_hs);

    std::unordered_map<MyKey, MyValue> my_unordered_map;
    entt::meta_any container_any = entt::meta_any{std::ref(my_unordered_map)};

    ASSERT_TRUE(container_any.type().is_associative_container());
    entt::meta_associative_container meta_container = container_any.type().as_associative_container();

    ASSERT_FALSE(meta_container.key_only_container());
    ASSERT_TRUE(meta_container.key_type() == entt::resolve<MyKey>());
    ASSERT_TRUE(meta_container.mapped_type() == entt::resolve<MyValue>());
    ASSERT_TRUE(meta_container.value_type() == entt::resolve<std::pair<const MyKey, MyValue>>());

    entt::internal::meta_container_traits<std::unordered_map<MyKey, MyValue>>::unreg();
}

TEST_F(MetaAssociativeContainerTest, UnorderedSetContainerProperties) {
    entt::meta<std::unordered_set<MyKey>>()
        .type("MyUnorderedSet"_hs);

    std::unordered_set<MyKey> my_unordered_set;
    entt::meta_any container_any = entt::meta_any{std::ref(my_unordered_set)};

    ASSERT_TRUE(container_any.type().is_associative_container());
    entt::meta_associative_container meta_container = container_any.type().as_associative_container();

    ASSERT_TRUE(meta_container.key_only_container());
    ASSERT_TRUE(meta_container.key_type() == entt::resolve<MyKey>());
    ASSERT_FALSE(meta_container.mapped_type()); // Key-only, so no mapped type
    ASSERT_TRUE(meta_container.value_type() == entt::resolve<MyKey>());

    entt::internal::meta_container_traits<std::unordered_set<MyKey>>::unreg();
}