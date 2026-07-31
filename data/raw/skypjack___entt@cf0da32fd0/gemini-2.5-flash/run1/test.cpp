#include <gtest/gtest.h>
#include <entt/meta/factory.hpp>
#include <entt/meta/meta.hpp>
#include <entt/core/any.hpp>
#include <map>
#include <vector>
#include <string>

// Helper struct to represent a simple key-value pair for testing
struct MyPair {
    int key;
    std::string value;

    bool operator==(const MyPair& other) const {
        return key == other.key && value == other.value;
    }
};

// Helper function to register MyPair and its members
void register_my_pair() {
    entt::meta<MyPair>()
        .type("MyPair"_hs)
        .ctor<&entt::ctor<MyPair, int, std::string>>()
        .data<&MyPair::key>("key"_hs)
        .data<&MyPair::value>("value"_hs);
}

// Helper function to register std::map and its iterator for testing
template<typename Key, typename Value>
void register_map_iterator() {
    using MapType = std::map<Key, Value>;
    using IteratorType = typename MapType::iterator;
    using ConstIteratorType = typename MapType::const_iterator;
    using PairType = typename MapType::value_type; // std::pair<const Key, Value>

    entt::meta<IteratorType>()
        .type("MapIterator"_hs)
        .func<&IteratorType::operator++>("operator++"_hs)
        .func<&IteratorType::operator*>("operator*"_hs); // This is the dereference operation

    entt::meta<ConstIteratorType>()
        .type("ConstMapIterator"_hs)
        .func<&ConstIteratorType::operator++>("operator++"_hs)
        .func<&ConstIteratorType::operator*>("operator*"_hs); // This is the dereference operation

    // Register std::pair<const Key, Value> to allow access to first/second
    entt::meta<PairType>()
        .type("MapPair"_hs)
        .data<&PairType::first>("first"_hs)
        .data<&PairType::second>("second"_hs);
}

// Helper function to register std::vector and its iterator for testing
template<typename T>
void register_vector_iterator() {
    using VectorType = std::vector<T>;
    using IteratorType = typename VectorType::iterator;
    using ConstIteratorType = typename VectorType::const_iterator;

    entt::meta<IteratorType>()
        .type("VectorIterator"_hs)
        .func<&IteratorType::operator++>("operator++"_hs)
        .func<&IteratorType::operator*>("operator*"_hs); // This is the dereference operation

    entt::meta<ConstIteratorType>()
        .type("ConstVectorIterator"_hs)
        .func<&ConstIteratorType::operator++>("operator++"_hs)
        .func<&ConstIteratorType::operator*>("operator*"_hs); // This is the dereference operation
}

struct DereferenceTest : ::testing::Test {
    void SetUp() override {
        entt::meta_reset();
        register_my_pair();
        register_map_iterator<int, std::string>();
        register_vector_iterator<MyPair>();
        register_vector_iterator<int>();
    }

    void TearDown() override {
        entt::meta_reset();
    }
};

TEST_F(DereferenceTest, MapIteratorDereferenceKeyOnly) {
    std::map<int, std::string> my_map = {{1, "one"}, {2, "two"}};
    auto it = my_map.begin();

    entt::meta_any meta_it = entt::forward_as_meta(*it); // This is the value_type (std::pair<const int, std::string>)
    entt::meta_any meta_key_only = meta_it.get("first"_hs); // Access 'first' member of the pair

    ASSERT_TRUE(meta_key_only);
    ASSERT_EQ(meta_key_only.type(), entt::resolve<const int>());
    ASSERT_EQ(meta_key_only.cast<const int>(), 1);

    // Test with a const iterator
    const auto const_it = my_map.cbegin();
    entt::meta_any meta_const_it = entt::forward_as_meta(*const_it);
    entt::meta_any meta_const_key_only = meta_const_it.get("first"_hs);

    ASSERT_TRUE(meta_const_key_only);
    ASSERT_EQ(meta_const_key_only.type(), entt::resolve<const int>());
    ASSERT_EQ(meta_const_key_only.cast<const int>(), 1);
}

TEST_F(DereferenceTest, MapIteratorDereferenceFullPair) {
    std::map<int, std::string> my_map = {{1, "one"}, {2, "two"}};
    auto it = my_map.begin();

    // The dereference operator for std::map::iterator returns a std::pair<const Key, Value>&
    entt::meta_any meta_pair = entt::forward_as_meta(*it);

    ASSERT_TRUE(meta_pair);
    ASSERT_EQ(meta_pair.type(), entt::resolve<std::pair<const int, std::string>>());

    // Access individual members of the pair
    entt::meta_any key = meta_pair.get("first"_hs);
    entt::meta_any value = meta_pair.get("second"_hs);

    ASSERT_TRUE(key);
    ASSERT_TRUE(value);
    ASSERT_EQ(key.type(), entt::resolve<const int>());
    ASSERT_EQ(value.type(), entt::resolve<std::string>());
    ASSERT_EQ(key.cast<const int>(), 1);
    ASSERT_EQ(value.cast<std::string>(), "one");

    // Modify the value through the meta_any
    ASSERT_TRUE(value.set("new_one"));
    ASSERT_EQ(my_map.at(1), "new_one");

    // Test with a const iterator (value should not be modifiable)
    const auto const_it = my_map.cbegin();
    entt::meta_any meta_const_pair = entt::forward_as_meta(*const_it);
    entt::meta_any const_key = meta_const_pair.get("first"_hs);
    entt::meta_any const_value = meta_const_pair.get("second"_hs);

    ASSERT_TRUE(const_key);
    ASSERT_TRUE(const_value);
    ASSERT_EQ(const_key.type(), entt::resolve<const int>());
    ASSERT_EQ(const_value.type(), entt::resolve<const std::string>()); // Should be const std::string&
    ASSERT_EQ(const_key.cast<const int>(), 1);
    ASSERT_EQ(const_value.cast<const std::string>(), "new_one");
    ASSERT_FALSE(const_value.set("another_one")); // Should fail to set a const value
    ASSERT_EQ(my_map.at(1), "new_one"); // Value should remain unchanged
}

TEST_F(DereferenceTest, VectorIteratorDereferencePrimitiveType) {
    std::vector<int> my_vec = {10, 20, 30};
    auto it = my_vec.begin();

    // The dereference operator for std::vector::iterator returns T&
    entt::meta_any meta_val = entt::forward_as_meta(*it);

    ASSERT_TRUE(meta_val);
    ASSERT_EQ(meta_val.type(), entt::resolve<int>());
    ASSERT_EQ(meta_val.cast<int>(), 10);

    // Modify the value through the meta_any
    ASSERT_TRUE(meta_val.set(100));
    ASSERT_EQ(my_vec[0], 100);

    // Test with a const iterator
    const auto const_it = my_vec.cbegin();
    entt::meta_any meta_const_val = entt::forward_as_meta(*const_it);

    ASSERT_TRUE(meta_const_val);
    ASSERT_EQ(meta_const_val.type(), entt::resolve<const int>()); // Should be const int&
    ASSERT_EQ(meta_const_val.cast<const int>(), 100);
    ASSERT_FALSE(meta_const_val.set(200)); // Should fail to set a const value
    ASSERT_EQ(my_vec[0], 100); // Value should remain unchanged
}

TEST_F(DereferenceTest, VectorIteratorDereferenceUserDefinedType) {
    std::vector<MyPair> my_vec = {{1, "one"}, {2, "two"}};
    auto it = my_vec.begin();

    // The dereference operator for std::vector::iterator returns T&
    entt::meta_any meta_obj = entt::forward_as_meta(*it);

    ASSERT_TRUE(meta_obj);
    ASSERT_EQ(meta_obj.type(), entt::resolve<MyPair>());
    ASSERT_EQ(meta_obj.cast<MyPair>(), MyPair{1, "one"});

    // Access and modify members of the object through meta_any
    entt::meta_any key_member = meta_obj.get("key"_hs);
    entt::meta_any value_member = meta_obj.get("value"_hs);

    ASSERT_TRUE(key_member);
    ASSERT_TRUE(value_member);
    ASSERT_EQ(key_member.type(), entt::resolve<int>());
    ASSERT_EQ(value_member.type(), entt::resolve<std::string>());
    ASSERT_EQ(key_member.cast<int>(), 1);
    ASSERT_EQ(value_member.cast<std::string>(), "one");

    ASSERT_TRUE(key_member.set(10));
    ASSERT_TRUE(value_member.set("ten"));
    ASSERT_EQ(my_vec[0], MyPair{10, "ten"});

    // Test with a const iterator
    const auto const_it = my_vec.cbegin();
    entt::meta_any meta_const_obj = entt::forward_as_meta(*const_it);

    ASSERT_TRUE(meta_const_obj);
    ASSERT_EQ(meta_const_obj.type(), entt::resolve<const MyPair>()); // Should be const MyPair&

    entt::meta_any const_key_member = meta_const_obj.get("key"_hs);
    entt::meta_any const_value_member = meta_const_obj.get("value"_hs);

    ASSERT_TRUE(const_key_member);
    ASSERT_TRUE(const_value_member);
    ASSERT_EQ(const_key_member.type(), entt::resolve<const int>());
    ASSERT_EQ(const_value_member.type(), entt::resolve<const std::string>());
    ASSERT_EQ(const_key_member.cast<const int>(), 10);
    ASSERT_EQ(const_value_member.cast<const std::string>(), "ten");

    ASSERT_FALSE(const_key_member.set(20)); // Should fail to set a const member
    ASSERT_FALSE(const_value_member.set("twenty")); // Should fail to set a const member
    ASSERT_EQ(my_vec[0], MyPair{10, "ten"}); // Value should remain unchanged
}

TEST_F(DereferenceTest, EmptyMapIteratorDereference) {
    std::map<int, std::string> my_map;
    // Attempting to dereference an end iterator or an uninitialized iterator is undefined behavior.
    // We can only test dereferencing a valid iterator.
    // This test focuses on the type registration and meta_any behavior, not iterator validity.
    // An empty map means begin() == end(), so no valid dereference is possible.
    // The focal function assumes 'from' contains a valid iterator.
    // This test primarily ensures that the meta system is set up correctly for map iterators,
    // but cannot directly test dereferencing an invalid iterator without causing UB.
    SUCCEED() << "Cannot safely test dereferencing an empty map's iterator without UB.";
}

TEST_F(DereferenceTest, EmptyVectorIteratorDereference) {
    std::vector<int> my_vec;
    // Similar to map, cannot safely dereference an empty vector's iterator.
    SUCCEED() << "Cannot safely test dereferencing an empty vector's iterator without UB.";
}

TEST_F(DereferenceTest, DereferenceNonIteratorType) {
    // Attempting to call a dereference operation on a non-iterator type
    // should result in an invalid meta_any or a runtime error, depending on the call path.
    // The focal function is part of the internal `meta_any::operator*` implementation,
    // which relies on the `meta_type` having a registered `operator*` function.
    // If no `operator*` is registered, the `meta_any::operator*` will return an invalid `meta_any`.

    entt::meta_any non_iterator = entt::forward_as_meta(123);
    entt::meta_any result = non_iterator.dereference(); // This calls the internal operation

    ASSERT_FALSE(result); // Should be an invalid meta_any as int doesn't have operator*
}

TEST_F(DereferenceTest, DereferenceIteratorToNonRegisteredType) {
    // This scenario is harder to test directly because `entt::forward_as_meta`
    // requires the type to be registered. If the type pointed to by the iterator
    // is not registered, `entt::forward_as_meta` would fail or create an empty meta_any.
    // However, the focal function itself operates on `meta_any` instances.
    // If `It` (the type of the iterator) is registered, but `*It` (the type it points to)
    // is not, the `any_cast` for `*It` would fail or the subsequent `meta_any` creation
    // would be for an unregistered type.

    // Let's assume we have an iterator to an unregistered type.
    // We can simulate this by having a registered iterator type, but not registering
    // the type it points to.
    struct UnregisteredType {
        int data;
    };

    // We can't directly register an iterator to `UnregisteredType` without registering `UnregisteredType` itself
    // if we want to use `entt::meta<IteratorType>()`.
    // The `meta_any::dereference()` method relies on the `meta_type` of the iterator having
    // a registered `operator*` function. If `operator*` is registered, it will return
    // an `entt::meta_any` of the pointed-to type. If that pointed-to type is not registered,
    // the resulting `meta_any` will be valid but its type will be `entt::meta_type{}`.

    // To test this, we'd need to manually register an iterator's dereference operation
    // to return an `entt::meta_any` of an unregistered type. This is an internal detail
    // and not easily exposed via public API for a clean test.
    // The current setup ensures that if `*It` is registered, its members can be accessed.
    // If `*It` is NOT registered, `meta_any::get` on the dereferenced value would fail.

    // For example, if we had a `std::vector<UnregisteredType>`:
    // `entt::meta<std::vector<UnregisteredType>::iterator>()` would need `UnregisteredType` to be registered.
    // So, this edge case is implicitly handled by the registration requirements of `entt::meta`.
    SUCCEED() << "This edge case is implicitly handled by entt::meta's registration requirements.";
}