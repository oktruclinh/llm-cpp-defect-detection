```cpp
#include <gtest/gtest.h>
#include <entt/core/any.hpp>
#include <entt/meta/factory.hpp>
#include <entt/meta/meta.hpp>
#include <entt/meta/resolve.hpp>
#include <map>
#include <string>
#include <vector>

namespace {

struct MyStruct {
    int value;
    std::string name;

    bool operator==(const MyStruct& other) const {
        return value == other.value && name == other.name;
    }
};

struct MyMapKey {
    int id;
    bool operator<(const MyMapKey& other) const { return id < other.id; }
    bool operator==(const MyMapKey& other) const { return id == other.id; }
};

} // namespace

TEST(MetaIteratorTest, DereferenceMapIteratorKeyOnly) {
    entt::meta_factory<MyMapKey>::template func<&MyMapKey::operator<>("operator<"_hs);
    entt::meta_factory<MyStruct>::template ctor<>();
    entt::meta_factory<MyStruct>::data<&MyStruct::value>("value"_hs);
    entt::meta_factory<MyStruct>::data<&MyStruct::name>("name"_hs);

    std::map<MyMapKey, MyStruct> myMap;
    myMap.emplace(MyMapKey{1}, MyStruct{10, "Ten"});
    myMap.emplace(MyMapKey{2}, MyStruct{20, "Twenty"});

    entt::meta_any map_any = entt::meta_any{std::ref(myMap)};
    ASSERT_TRUE(map_any);

    auto it_any = map_any.begin();
    ASSERT_TRUE(it_any);

    // Dereference with KeyOnly = true
    // This is implicitly tested by the meta_any::operator*() when the underlying type is an iterator to a pair,
    // and the pair's second element is not directly accessible or is a reference wrapper.
    // The meta_any::operator*() for iterators to std::map::value_type (std::pair<const Key, Value>)
    // will return a meta_any representing the key when dereferenced directly.
    // To explicitly test the KeyOnly path, we need to ensure the dereference operation
    // is specifically targeting the key. The current meta_any::operator*() behavior
    // for map iterators already does this for the 'first' element.

    // When dereferencing a map iterator, the default behavior of meta_any::operator*()
    // is to return a meta_any representing the key (first element of the pair).
    // This corresponds to the KeyOnly path in the focal function.
    entt::meta_any key_any = *it_any;
    ASSERT_TRUE(key_any);
    ASSERT_EQ(key_any.type(), entt::resolve<MyMapKey>());
    ASSERT_EQ(key_any.cast<const MyMapKey&>().id, 1);

    // Move to the next element and dereference
    ++it_any;
    ASSERT_TRUE(it_any);
    key_any = *it_any;
    ASSERT_TRUE(key_any);
    ASSERT_EQ(key_any.type(), entt::resolve<MyMapKey>());
    ASSERT_EQ(key_any.cast<const MyMapKey&>().id, 2);

    // Test dereferencing an empty map iterator (should be invalid)
    std::map<MyMapKey, MyStruct> emptyMap;
    entt::meta_any empty_map_any = entt::meta_any{std::ref(emptyMap)};
    auto empty_it_any = empty_map_any.begin();
    ASSERT_FALSE(empty_it_any); // begin() on empty map returns end(), which is often considered invalid for dereference

    // If we try to dereference an end iterator, it should result in an invalid meta_any
    auto end_it_any = map_any.end();
    ASSERT_TRUE(end_it_any); // end() iterator is valid, just not dereferenceable
    entt::meta_any invalid_deref = *end_it_any;
    ASSERT_FALSE(invalid_deref);
}

TEST(MetaIteratorTest, DereferenceMapIteratorFullPair) {
    entt::meta_factory<MyMapKey>::template func<&MyMapKey::operator<>("operator<"_hs);
    entt::meta_factory<MyStruct>::template ctor<>();
    entt::meta_factory<MyStruct>::data<&MyStruct::value>("value"_hs);
    entt::meta_factory<MyStruct>::data<&MyStruct::name>("name"_hs);

    std::map<MyMapKey, MyStruct> myMap;
    myMap.emplace(MyMapKey{1}, MyStruct{10, "Ten"});
    myMap.emplace(MyMapKey{2}, MyStruct{20, "Twenty"});

    entt::meta_any map_any = entt::meta_any{std::ref(myMap)};
    ASSERT_TRUE(map_any);

    auto it_any = map_any.begin();
    ASSERT_TRUE(it_any);

    // To get the full pair (key and value), we typically use the meta_any::operator->()
    // or access members 'first' and 'second' if the dereferenced type is a pair.
    // The focal function's 'else' branch (KeyOnly = false) is for when the dereferenced
    // type is expected to be a pair of meta_any. This is usually triggered by
    // `meta_any::operator->()` or `meta_any::get("first"_hs)`/`meta_any::get("second"_hs)`.

    // Accessing 'first' and 'second' members of the dereferenced iterator
    // This implicitly uses the 'else' branch of the focal function, as it needs to
    // construct a pair of meta_any to represent the key and value.
    entt::meta_any dereferenced_pair = *it_any; // This will return the key (KeyOnly path)

    // To get the full pair, we need to use `meta_any::get("first"_hs)` and `meta_any::get("second"_hs)`
    // on the iterator itself, or use `meta_any::operator->()` which returns a proxy.
    // The `meta_any::operator->()` is designed to return a proxy that allows access to 'first' and 'second'.
    auto proxy = it_any.operator->();
    ASSERT_TRUE(proxy);

    entt::meta_any key_from_proxy = proxy.get("first"_hs);
    ASSERT_TRUE(key_from_proxy);
    ASSERT_EQ(key_from_proxy.type(), entt::resolve<MyMapKey>());
    ASSERT_EQ(key_from_proxy.cast<const MyMapKey&>().id, 1);

    entt::meta_any value_from_proxy = proxy.get("second"_hs);
    ASSERT_TRUE(value_from_proxy);
    ASSERT_EQ(value_from_proxy.type(), entt::resolve<MyStruct>());
    ASSERT_EQ(value_from_proxy.cast<const MyStruct&>().value, 10);
    ASSERT_EQ(value_from_proxy.cast<const MyStruct&>().name, "Ten");

    // Modify the value through the meta_any reference wrapper
    MyStruct newValue{100, "Hundred"};
    ASSERT_TRUE(value_from_proxy.set(newValue));
    ASSERT_EQ(myMap.at(MyMapKey{1}).value, 100);
    ASSERT_EQ(myMap.at(MyMapKey{1}).name, "Hundred");

    // Move to the next element and test again
    ++it_any;
    ASSERT_TRUE(it_any);
    proxy = it_any.operator->();
    ASSERT_TRUE(proxy);

    key_from_proxy = proxy.get("first"_hs);
    ASSERT_TRUE(key_from_proxy);
    ASSERT_EQ(key_from_proxy.type(), entt::resolve<MyMapKey>());
    ASSERT_EQ(key_from_proxy.cast<const MyMapKey&>().id, 2);

    value_from_proxy = proxy.get("second"_hs);
    ASSERT_TRUE(value_from_proxy);
    ASSERT_EQ(value_from_proxy.type(), entt::resolve<MyStruct>());
    ASSERT_EQ(value_from_proxy.cast<const MyStruct&>().value, 20);
    ASSERT_EQ(value_from_proxy.cast<const MyStruct&>().name, "Twenty");

    // Test dereferencing an empty map iterator (should be invalid)
    std::map<MyMapKey, MyStruct> emptyMap;
    entt::meta_any empty_map_any = entt::meta_any{std::ref(emptyMap)};
    auto empty_it_any = empty_map_any.begin();
    ASSERT_FALSE(empty_it_any); // begin() on empty map returns end(), which is often considered invalid for dereference

    // If we try to dereference an end iterator, it should result in an invalid meta_any
    auto end_it_any = map_any.end();
    ASSERT_TRUE(end_it_any); // end() iterator is valid, just not dereferenceable
    auto invalid_proxy = end_it_any.operator->();
    ASSERT_FALSE(invalid_proxy);
    ASSERT_FALSE(invalid_proxy.get("first"_hs));
    ASSERT_FALSE(invalid_proxy.get("second"_hs));
}

TEST(MetaIteratorTest, DereferenceVectorIterator) {
    entt::meta_factory<MyStruct>::template ctor<>();
    entt::meta_factory<MyStruct>::data<&MyStruct::value>("value"_hs);
    entt::meta_factory<MyStruct>::data<&MyStruct::name>("name"_hs);

    std::vector<MyStruct> myVector;
    myVector.push_back({10, "Ten"});
    myVector.push_back({20, "Twenty"});

    entt::meta_any vec_any = entt::meta_any{std::ref(myVector)};
    ASSERT_TRUE(vec_any);

    auto it_any = vec_any.begin();
    ASSERT_TRUE(it_any);

    // Dereference a vector iterator. This should NOT go into the KeyOnly path
    // as it's not an iterator to a pair. It should return a meta_any representing MyStruct.
    entt::meta_any element_any = *it_any;
    ASSERT_TRUE(element_any);
    ASSERT_EQ(element_any.type(), entt::resolve<MyStruct>());
    ASSERT_EQ(element_any.cast<const MyStruct&>().value, 10);
    ASSERT_EQ(element_any.cast<const MyStruct&>().name, "Ten");

    // Modify the element through the meta_any
    MyStruct newValue{100, "Hundred"};
    ASSERT_TRUE(element_any.set(newValue));
    ASSERT_EQ(myVector[0].value, 100);
    ASSERT_EQ(myVector[0].name, "Hundred");

    // Move to the next element and dereference
    ++it_any;
    ASSERT_TRUE(it_any);
    element_any = *it_any;
    ASSERT_TRUE(element_any);
    ASSERT_EQ(element_any.type(), entt::resolve<MyStruct>());
    ASSERT_EQ(element_any.cast<const MyStruct&>().value, 20);
    ASSERT_EQ(element_any.cast<const MyStruct&>().name, "Twenty");

    // Test dereferencing an empty vector iterator (should be invalid)
    std::vector<MyStruct> emptyVector;
    entt::meta_any empty_vec_any = entt::meta_any{std::ref(emptyVector)};
    auto empty_it_any = empty_vec_any.begin();
    ASSERT_FALSE(empty_it_any); // begin() on empty vector returns end(), which is often considered invalid for dereference

    // If we try to dereference an end iterator, it should result in an invalid meta_any
    auto end_it_any = vec_any.end();
    ASSERT_TRUE(end_it_any); // end() iterator is valid, just not dereferenceable
    entt::meta_any invalid_deref = *end_it_any;
    ASSERT_FALSE(invalid_deref);
}

TEST(MetaIteratorTest, DereferenceConstMapIterator) {
    entt::meta_factory<MyMapKey>::template func<&MyMapKey::operator<>("operator<"_hs);
    entt::meta_factory<MyStruct>::template ctor<>();
    entt::meta_factory<MyStruct>::data<&MyStruct::value>("value"_hs);
    entt::meta_factory<MyStruct>::data<&MyStruct::name>("name"_hs);

    const std::map<MyMapKey, MyStruct> myConstMap = {
        {MyMapKey{1}, MyStruct{10, "Ten"}},
        {MyMapKey{2}, MyStruct{20, "Twenty"}}
    };

    entt::meta_any map_any = entt::meta_any{std::cref(myConstMap)};
    ASSERT_TRUE(map_any);

    auto it_any = map_any.begin();
    ASSERT_TRUE(it_any);

    // Dereference with KeyOnly = true (default for map iterators)
    entt::meta_any key_any = *it_any;
    ASSERT_TRUE(key_any);
    ASSERT_EQ(key_any.type(), entt::resolve<MyMapKey>());
    ASSERT_EQ(key_any.cast<const MyMapKey&>().id, 1);

    // Attempt to modify the key (should fail as it's const)
    MyMapKey newKey{100};
    ASSERT_FALSE(key_any.set(newKey));

    // Access 'first' and 'second' members of the dereferenced iterator
    auto proxy = it_any.operator->();
    ASSERT_TRUE(proxy);

    entt::meta_any key_from_proxy = proxy.get("first"_hs);
    ASSERT_TRUE(key_from_proxy);
    ASSERT_EQ(key_from_proxy.type(), entt::resolve<MyMapKey>());
    ASSERT_EQ(key_from_proxy.cast<const MyMapKey&>().id, 1);
    ASSERT_FALSE(key_from_proxy.set(newKey)); // Should fail

    entt::meta_any value_from_proxy = proxy.get("second"_hs);
    ASSERT_TRUE(value_from_proxy);
    ASSERT_EQ(value_from_proxy.type(), entt::resolve<MyStruct>());
    ASSERT_EQ(value_from_proxy.cast<const MyStruct&>().value, 10);
    ASSERT_EQ(value_from_proxy.cast<const MyStruct&>().name, "Ten");

    // Attempt to modify the value (should fail as the map is const)
    MyStruct newValue{100, "Hundred"};
    ASSERT_FALSE(value_from_proxy.set(newValue));
}

TEST(MetaIteratorTest, DereferenceConstVectorIterator) {
    entt::meta_factory<MyStruct>::template ctor<>();
    entt::meta_factory<MyStruct>::data<&MyStruct::value>("value"_hs);
    entt::meta_factory<MyStruct>::data<&MyStruct::name>("name"_hs);

    const std::vector<MyStruct> myConstVector = {
        {10, "Ten"},
        {20, "Twenty"}
    };

    entt::meta_any vec_any = entt::meta_any{std::cref(myConstVector)};
    ASSERT_TRUE(vec_any);

    auto it_any = vec_any.begin();
    ASSERT_TRUE(it_any);

    // Dereference a const vector iterator.
    entt::meta_any element_any = *it_any;
    ASSERT_TRUE(element_any);
    ASSERT_EQ(element_any.type(), entt::resolve<MyStruct>());
    ASSERT_EQ(element_any.cast<const MyStruct&>().value, 10);
    ASSERT_EQ(element_any.cast<const MyStruct&>().name, "Ten");

    // Attempt to modify the element (should fail as the vector is const)
    MyStruct newValue{100, "Hundred"};
    ASSERT_FALSE(element_any.set(newValue));

    // Move to the next element and dereference
    ++it_any;
    ASSERT_TRUE(it_any);
    element_any = *it_any;
    ASSERT_TRUE(element_any);
    ASSERT_EQ(element_any.type(), entt::resolve<MyStruct>());
    ASSERT_EQ(element_any.cast<const MyStruct&>().value, 20);
    ASSERT_EQ(element_any.cast<const MyStruct&>().name, "Twenty");
    ASSERT_FALSE(element_any.set(newValue)); // Should fail
}

TEST(MetaIteratorTest, DereferenceEmptyMap) {
    std::map<MyMapKey, MyStruct> empty