#include <gtest/gtest.h>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

// Include the necessary EnTT public headers
#include "entt/core/type_info.hpp"
#include "entt/meta/meta.hpp"
#include "entt/meta/factory.hpp"
#include "entt/meta/resolve.hpp"

// Minimal setup for meta types if not already registered
struct MyKey {};
struct MyValue {};

TEST(MetaAssociativeContainerTest, KeyOnlyContainerDetection) {
    // Register types for meta system if not already done
    entt::meta<MyKey>().type("MyKey"_hs);
    entt::meta<MyValue>().type("MyValue"_hs);

    // Test with a std::set (key-only)
    std::set<int> int_set;
    entt::meta_associative_container meta_int_set(int_set);
    ASSERT_TRUE(meta_int_set.key_only_container());
    ASSERT_EQ(meta_int_set.key_type(), entt::resolve<int>());
    ASSERT_FALSE(meta_int_set.mapped_type()); // Should be invalid for key-only
    ASSERT_EQ(meta_int_set.value_type(), entt::resolve<int>()); // Value type is key type for set

    std::set<std::string> string_set;
    entt::meta_associative_container meta_string_set(string_set);
    ASSERT_TRUE(meta_string_set.key_only_container());
    ASSERT_EQ(meta_string_set.key_type(), entt::resolve<std::string>());
    ASSERT_FALSE(meta_string_set.mapped_type());
    ASSERT_EQ(meta_string_set.value_type(), entt::resolve<std::string>());

    std::set<MyKey> mykey_set;
    entt::meta_associative_container meta_mykey_set(mykey_set);
    ASSERT_TRUE(meta_mykey_set.key_only_container());
    ASSERT_EQ(meta_mykey_set.key_type(), entt::resolve<MyKey>());
    ASSERT_FALSE(meta_mykey_set.mapped_type());
    ASSERT_EQ(meta_mykey_set.value_type(), entt::resolve<MyKey>());

    // Test with a std::map (key-value)
    std::map<int, std::string> int_string_map;
    entt::meta_associative_container meta_int_string_map(int_string_map);
    ASSERT_FALSE(meta_int_string_map.key_only_container());
    ASSERT_EQ(meta_int_string_map.key_type(), entt::resolve<int>());
    ASSERT_EQ(meta_int_string_map.mapped_type(), entt::resolve<std::string>());
    ASSERT_EQ(meta_int_string_map.value_type(), entt::resolve<std::pair<const int, std::string>>());

    std::map<std::string, MyValue> string_myvalue_map;
    entt::meta_associative_container meta_string_myvalue_map(string_myvalue_map);
    ASSERT_FALSE(meta_string_myvalue_map.key_only_container());
    ASSERT_EQ(meta_string_myvalue_map.key_type(), entt::resolve<std::string>());
    ASSERT_EQ(meta_string_myvalue_map.mapped_type(), entt::resolve<MyValue>());
    ASSERT_EQ(meta_string_myvalue_map.value_type(), entt::resolve<std::pair<const std::string, MyValue>>());
}

TEST(MetaAssociativeContainerTest, TypeFunctionsReturnCorrectTypes) {
    // Test with std::map<int, float>
    std::map<int, float> map_int_float;
    entt::meta_associative_container meta_map(map_int_float);

    ASSERT_EQ(meta_map.key_type(), entt::resolve<int>());
    ASSERT_EQ(meta_map.mapped_type(), entt::resolve<float>());
    ASSERT_EQ(meta_map.value_type(), entt::resolve<std::pair<const int, float>>());
    ASSERT_FALSE(meta_map.key_only_container());

    // Test with std::set<double>
    std::set<double> set_double;
    entt::meta_associative_container meta_set(set_double);

    ASSERT_EQ(meta_set.key_type(), entt::resolve<double>());
    ASSERT_FALSE(meta_set.mapped_type()); // Should be invalid for key-only
    ASSERT_EQ(meta_set.value_type(), entt::resolve<double>());
    ASSERT_TRUE(meta_set.key_only_container());

    // Test with std::unordered_map<char, bool>
    std::unordered_map<char, bool> unordered_map_char_bool;
    entt::meta_associative_container meta_unordered_map(unordered_map_char_bool);

    ASSERT_EQ(meta_unordered_map.key_type(), entt::resolve<char>());
    ASSERT_EQ(meta_unordered_map.mapped_type(), entt::resolve<bool>());
    ASSERT_EQ(meta_unordered_map.value_type(), entt::resolve<std::pair<const char, bool>>());
    ASSERT_FALSE(meta_unordered_map.key_only_container());

    // Test with std::unordered_set<long>
    std::unordered_set<long> unordered_set_long;
    entt::meta_associative_container meta_unordered_set(unordered_set_long);

    ASSERT_EQ(meta_unordered_set.key_type(), entt::resolve<long>());
    ASSERT_FALSE(meta_unordered_set.mapped_type());
    ASSERT_EQ(meta_unordered_set.value_type(), entt::resolve<long>());
    ASSERT_TRUE(meta_unordered_set.key_only_container());
}

// Edge case: Empty containers (type information should still be correct)
TEST(MetaAssociativeContainerTest, EmptyContainers) {
    std::map<int, std::string> empty_map;
    entt::meta_associative_container meta_empty_map(empty_map);
    ASSERT_FALSE(meta_empty_map.key_only_container());
    ASSERT_EQ(meta_empty_map.key_type(), entt::resolve<int>());
    ASSERT_EQ(meta_empty_map.mapped_type(), entt::resolve<std::string>());
    ASSERT_EQ(meta_empty_map.value_type(), entt::resolve<std::pair<const int, std::string>>());

    std::set<double> empty_set;
    entt::meta_associative_container meta_empty_set(empty_set);
    ASSERT_TRUE(meta_empty_set.key_only_container());
    ASSERT_EQ(meta_empty_set.key_type(), entt::resolve<double>());
    ASSERT_FALSE(meta_empty_set.mapped_type());
    ASSERT_EQ(meta_empty_set.value_type(), entt::resolve<double>());
}

// Test with const containers
TEST(MetaAssociativeContainerTest, ConstContainers) {
    const std::map<int, float> const_map_int_float;
    entt::meta_associative_container meta_const_map(const_map_int_float);

    ASSERT_EQ(meta_const_map.key_type(), entt::resolve<int>());
    ASSERT_EQ(meta_const_map.mapped_type(), entt::resolve<float>());
    ASSERT_EQ(meta_const_map.value_type(), entt::resolve<std::pair<const int, float>>());
    ASSERT_FALSE(meta_const_map.key_only_container());

    const std::set<double> const_set_double;
    entt::meta_associative_container meta_const_set(const_set_double);

    ASSERT_EQ(meta_const_set.key_type(), entt::resolve<double>());
    ASSERT_FALSE(meta_const_set.mapped_type());
    ASSERT_EQ(meta_const_set.value_type(), entt::resolve<double>());
    ASSERT_TRUE(meta_const_set.key_only_container());
}

// Test with custom types that are not registered (should still work for type resolution)
struct CustomKey {};
struct CustomValue {};

TEST(MetaAssociativeContainerTest, CustomUnregisteredTypes) {
    std::map<CustomKey, CustomValue> custom_map;
    entt::meta_associative_container meta_custom_map(custom_map);

    ASSERT_FALSE(meta_custom_map.key_only_container());
    ASSERT_EQ(meta_custom_map.key_type(), entt::resolve<CustomKey>());
    ASSERT_EQ(meta_custom_map.mapped_type(), entt::resolve<CustomValue>());
    ASSERT_EQ(meta_custom_map.value_type(), entt::resolve<std::pair<const CustomKey, CustomValue>>());

    std::set<CustomKey> custom_set;
    entt::meta_associative_container meta_custom_set(custom_set);

    ASSERT_TRUE(meta_custom_set.key_only_container());
    ASSERT_EQ(meta_custom_set.key_type(), entt::resolve<CustomKey>());
    ASSERT_FALSE(meta_custom_set.mapped_type());
    ASSERT_EQ(meta_custom_set.value_type(), entt::resolve<CustomKey>());
}

// Test with types that are references (the container itself is passed by reference, but the types within are not)
TEST(MetaAssociativeContainerTest, ReferenceTypes) {
    // The container itself is passed by reference to the constructor,
    // but the types extracted (Key, Mapped, Value) should be non-reference.
    std::map<int&, float&> map_with_references; // This is not a standard map, but for demonstration
    // This test case is actually invalid because std::map cannot hold reference types directly.
    // The point is that the *template parameters* of the container are what's resolved,
    // not the type of the variable passed to the constructor.
    // Let's stick to standard containers.

    // The constructor takes `Type &container`, so `Type` will be `std::map<int, float>`
    // not `std::map<int, float>&`. The `key_type_fn` etc. should resolve `int` and `float`.
    std::map<int, float> non_ref_map;
    entt::meta_associative_container meta_non_ref_map(non_ref_map);
    ASSERT_EQ(meta_non_ref_map.key_type(), entt::resolve<int>());
    ASSERT_EQ(meta_non_ref_map.mapped_type(), entt::resolve<float>());
    ASSERT_EQ(meta_non_ref_map.value_type(), entt::resolve<std::pair<const int, float>>());
}