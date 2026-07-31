```cpp
#include <gtest/gtest.h>
#include <entt/meta/factory.hpp>
#include <entt/meta/meta.hpp>
#include <entt/core/any.hpp>
#include <map>
#include <vector>
#include <string>

namespace {

struct MyStruct {
    int value;
    std::string name;

    bool operator==(const MyStruct& other) const {
        return value == other.value && name == other.name;
    }
};

struct MyKeyOnlyStruct {
    int id;
    // No second element for KeyOnly test
};

} // namespace

TEST(MetaIteratorDerefTest, MapIteratorDerefKeyOnly) {
    entt::meta_factory<std::map<int, MyStruct>>()
        .func<&std::map<int, MyStruct>::begin>("begin")
        .func<&std::map<int, MyStruct>::end>("end");

    entt::meta_factory<std::map<int, MyStruct>::iterator>()
        .deref<true>(); // KeyOnly = true

    entt::meta_factory<std::map<int, MyStruct>::const_iterator>()
        .deref<true>(); // KeyOnly = true

    entt::meta_ctx::bind_custom(
        entt::internal::resolve_meta_type<std::map<int, MyStruct>::iterator>(),
        [](entt::meta_any& any, entt::meta_any& to) {
            if (any.type().is_const()) {
                static_cast<std::pair<entt::meta_any, entt::meta_any>*>(to.data())->first = std::cref(*entt::any_cast<const std::map<int, MyStruct>::const_iterator&>(any));
            } else {
                static_cast<std::pair<entt::meta_any, entt::meta_any>*>(to.data())->first = std::cref(*entt::any_cast<const std::map<int, MyStruct>::iterator&>(any));
            }
        },
        entt::internal::meta_operation::DEREF,
        true // KeyOnly
    );

    entt::meta_ctx::bind_custom(
        entt::internal::resolve_meta_type<std::map<int, MyStruct>::const_iterator>(),
        [](entt::meta_any& any, entt::meta_any& to) {
            if (any.type().is_const()) {
                static_cast<std::pair<entt::meta_any, entt::meta_any>*>(to.data())->first = std::cref(*entt::any_cast<const std::map<int, MyStruct>::const_iterator&>(any));
            } else {
                static_cast<std::pair<entt::meta_any, entt::meta_any>*>(to.data())->first = std::cref(*entt::any_cast<const std::map<int, MyStruct>::iterator&>(any));
            }
        },
        entt::internal::meta_operation::DEREF,
        true // KeyOnly
    );

    std::map<int, MyStruct> myMap = {{1, {10, "ten"}}, {2, {20, "twenty"}}};
    entt::meta_any map_any = entt::meta_any{std::ref(myMap)};

    auto begin_func = map_any.type().func("begin");
    ASSERT_TRUE(begin_func);

    entt::meta_any it_any = begin_func.invoke(map_any);
    ASSERT_TRUE(it_any);

    entt::meta_any deref_result_pair = entt::meta_any{std::in_place_type<std::pair<entt::meta_any, entt::meta_any>>};
    ASSERT_TRUE(it_any.deref(deref_result_pair));

    ASSERT_TRUE(deref_result_pair.type().is_pair());
    ASSERT_TRUE(deref_result_pair.type().first_type().id() == entt::resolve<int>().id());
    ASSERT_TRUE(deref_result_pair.type().second_type().id() == entt::resolve<entt::meta_any>().id()); // Second element is default constructed meta_any

    int key = deref_result_pair.type().first_type().construct().cast<int>();
    ASSERT_TRUE(deref_result_pair.get("first").try_cast(key));
    ASSERT_EQ(key, 1);

    // Check that the second element is an empty meta_any as KeyOnly was true
    entt::meta_any second_element = deref_result_pair.get("second");
    ASSERT_TRUE(second_element.type() == entt::resolve<entt::meta_any>());
    ASSERT_FALSE(second_element.has_value());

    // Test const iterator
    const std::map<int, MyStruct> const_myMap = {{3, {30, "thirty"}}};
    entt::meta_any const_map_any = entt::meta_any{std::cref(const_myMap)};

    auto const_begin_func = const_map_any.type().func("begin");
    ASSERT_TRUE(const_begin_func);

    entt::meta_any const_it_any = const_begin_func.invoke(const_map_any);
    ASSERT_TRUE(const_it_any);

    entt::meta_any const_deref_result_pair = entt::meta_any{std::in_place_type<std::pair<entt::meta_any, entt::meta_any>>};
    ASSERT_TRUE(const_it_any.deref(const_deref_result_pair));

    int const_key = const_deref_result_pair.type().first_type().construct().cast<int>();
    ASSERT_TRUE(const_deref_result_pair.get("first").try_cast(const_key));
    ASSERT_EQ(const_key, 3);

    entt::meta_any const_second_element = const_deref_result_pair.get("second");
    ASSERT_TRUE(const_second_element.type() == entt::resolve<entt::meta_any>());
    ASSERT_FALSE(const_second_element.has_value());
}

TEST(MetaIteratorDerefTest, MapIteratorDerefFullPair) {
    entt::meta_factory<MyStruct>()
        .data<&MyStruct::value>("value")
        .data<&MyStruct::name>("name");

    entt::meta_factory<std::map<int, MyStruct>>()
        .func<&std::map<int, MyStruct>::begin>("begin")
        .func<&std::map<int, MyStruct>::end>("end");

    entt::meta_factory<std::map<int, MyStruct>::iterator>()
        .deref<false>(); // KeyOnly = false

    entt::meta_factory<std::map<int, MyStruct>::const_iterator>()
        .deref<false>(); // KeyOnly = false

    entt::meta_ctx::bind_custom(
        entt::internal::resolve_meta_type<std::map<int, MyStruct>::iterator>(),
        [](entt::meta_any& any, entt::meta_any& to) {
            if (any.type().is_const()) {
                *static_cast<std::pair<entt::meta_any, entt::meta_any>*>(to.data()) = std::make_pair<entt::meta_any, entt::meta_any>(std::cref(entt::any_cast<const std::map<int, MyStruct>::const_iterator&>(any)->first), std::reference_wrapper{entt::any_cast<const std::map<int, MyStruct>::const_iterator&>(any)->second});
            } else {
                *static_cast<std::pair<entt::meta_any, entt::meta_any>*>(to.data()) = std::make_pair<entt::meta_any, entt::meta_any>(std::cref(entt::any_cast<const std::map<int, MyStruct>::iterator&>(any)->first), std::reference_wrapper{entt::any_cast<const std::map<int, MyStruct>::iterator&>(any)->second});
            }
        },
        entt::internal::meta_operation::DEREF,
        false // KeyOnly
    );

    entt::meta_ctx::bind_custom(
        entt::internal::resolve_meta_type<std::map<int, MyStruct>::const_iterator>(),
        [](entt::meta_any& any, entt::meta_any& to) {
            if (any.type().is_const()) {
                *static_cast<std::pair<entt::meta_any, entt::meta_any>*>(to.data()) = std::make_pair<entt::meta_any, entt::meta_any>(std::cref(entt::any_cast<const std::map<int, MyStruct>::const_iterator&>(any)->first), std::reference_wrapper{entt::any_cast<const std::map<int, MyStruct>::const_iterator&>(any)->second});
            } else {
                *static_cast<std::pair<entt::meta_any, entt::meta_any>*>(to.data()) = std::make_pair<entt::meta_any, entt::meta_any>(std::cref(entt::any_cast<const std::map<int, MyStruct>::iterator&>(any)->first), std::reference_wrapper{entt::any_cast<const std::map<int, MyStruct>::iterator&>(any)->second});
            }
        },
        entt::internal::meta_operation::DEREF,
        false // KeyOnly
    );

    std::map<int, MyStruct> myMap = {{1, {10, "ten"}}, {2, {20, "twenty"}}};
    entt::meta_any map_any = entt::meta_any{std::ref(myMap)};

    auto begin_func = map_any.type().func("begin");
    ASSERT_TRUE(begin_func);

    entt::meta_any it_any = begin_func.invoke(map_any);
    ASSERT_TRUE(it_any);

    entt::meta_any deref_result_pair = entt::meta_any{std::in_place_type<std::pair<entt::meta_any, entt::meta_any>>};
    ASSERT_TRUE(it_any.deref(deref_result_pair));

    ASSERT_TRUE(deref_result_pair.type().is_pair());
    ASSERT_TRUE(deref_result_pair.type().first_type().id() == entt::resolve<int>().id());
    ASSERT_TRUE(deref_result_pair.type().second_type().id() == entt::resolve<MyStruct>().id());

    int key = deref_result_pair.type().first_type().construct().cast<int>();
    ASSERT_TRUE(deref_result_pair.get("first").try_cast(key));
    ASSERT_EQ(key, 1);

    MyStruct value = deref_result_pair.type().second_type().construct().cast<MyStruct>();
    ASSERT_TRUE(deref_result_pair.get("second").try_cast(value));
    ASSERT_EQ(value, MyStruct{10, "ten"});

    // Modify the value through the meta_any reference
    entt::meta_any second_element_ref = deref_result_pair.get("second");
    ASSERT_TRUE(second_element_ref);
    ASSERT_TRUE(second_element_ref.type().is_const() == false); // Should be mutable

    auto value_data = second_element_ref.type().data("value");
    ASSERT_TRUE(value_data);
    ASSERT_TRUE(value_data.set(second_element_ref, 100));

    auto name_data = second_element_ref.type().data("name");
    ASSERT_TRUE(name_data);
    ASSERT_TRUE(name_data.set(second_element_ref, std::string("one hundred")));

    ASSERT_EQ(myMap.at(1).value, 100);
    ASSERT_EQ(myMap.at(1).name, "one hundred");

    // Test const iterator
    const std::map<int, MyStruct> const_myMap = {{3, {30, "thirty"}}};
    entt::meta_any const_map_any = entt::meta_any{std::cref(const_myMap)};

    auto const_begin_func = const_map_any.type().func("begin");
    ASSERT_TRUE(const_begin_func);

    entt::meta_any const_it_any = const_begin_func.invoke(const_map_any);
    ASSERT_TRUE(const_it_any);

    entt::meta_any const_deref_result_pair = entt::meta_any{std::in_place_type<std::pair<entt::meta_any, entt::meta_any>>};
    ASSERT_TRUE(const_it_any.deref(const_deref_result_pair));

    int const_key = const_deref_result_pair.type().first_type().construct().cast<int>();
    ASSERT_TRUE(const_deref_result_pair.get("first").try_cast(const_key));
    ASSERT_EQ(const_key, 3);

    MyStruct const_value = const_deref_result_pair.type().second_type().construct().cast<MyStruct>();
    ASSERT_TRUE(const_deref_result_pair.get("second").try_cast(const_value));
    ASSERT_EQ(const_value, MyStruct{30, "thirty"});

    // Attempt to modify through const_deref_result_pair should fail
    entt::meta_any const_second_element_ref = const_deref_result_pair.get("second");
    ASSERT_TRUE(const_second_element_ref);
    ASSERT_TRUE(const_second_element_ref.type().is_const()); // Should be const

    auto const_value_data = const_second_element_ref.type().data("value");
    ASSERT_TRUE(const_value_data);
    ASSERT_FALSE(const_value_data.set(const_second_element_ref, 300)); // Should fail to set
    ASSERT_EQ(const_myMap.at(3).value, 30); // Value should remain unchanged
}

TEST(MetaIteratorDerefTest, EmptyMapIteratorDeref) {
    entt::meta_factory<std::map<int, MyStruct>>()
        .func<&std::map<int, MyStruct>::begin>("begin")
        .func<&std::map<int, MyStruct>::end>("end");

    entt::meta_factory<std::map<int, MyStruct>::iterator>()
        .deref<false>();

    std::map<int, MyStruct> emptyMap;
    entt::meta_any map_any = entt::meta_any{std::ref(emptyMap)};

    auto begin_func = map_any.type().func("begin");
    ASSERT_TRUE(begin_func);

    entt::meta_any it_any = begin_func.invoke(map_any);
    ASSERT_TRUE(it_any);

    auto end_func = map_any.type().func("end");
    ASSERT_TRUE(end_func);

    entt::meta_any end_it_any = end_func.invoke(map_any);
    ASSERT_TRUE(end_it_any);

    // Dereferencing an end iterator should fail
    entt::meta_any deref_result_pair = entt::meta_any{std::in_place_type<std::pair<entt::meta_any, entt::meta_any>>};
    ASSERT_FALSE(end_it_any.deref(deref_result_pair));
    ASSERT_FALSE(deref_result_pair.has_value()); // Should not have been assigned
}

TEST(MetaIteratorDerefTest, DerefToWrongType) {
    entt::meta_factory<std::map<int, MyStruct>>()
        .func<&std::map<int, MyStruct>::begin>("begin")
        .func<&std::map<int, MyStruct>::end>("end");

    entt::meta_factory<std::map<int, MyStruct>::iterator>()
        .deref<false>();

    entt::meta_ctx::bind_custom(
        entt::internal::resolve_meta_type<std::map<int, MyStruct>::iterator>(),
        [](entt::meta_any& any, entt::meta_any& to) {
            if (any.type().is_const()) {
                *static_cast<std::pair<entt::meta_any, entt::meta_any>*>(to.data()) = std::make_pair<entt::meta_any,