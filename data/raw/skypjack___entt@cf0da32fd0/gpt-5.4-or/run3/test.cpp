#include <gtest/gtest.h>

#include <entt/meta/meta.hpp>
#include <entt/meta/factory.hpp>

#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <type_traits>
#include <utility>

namespace {

struct boxed_int {
    int value{};
};

void register_meta_types() {
    static const bool registered = [] {
        entt::meta_factory<int>{};
        entt::meta_factory<boxed_int>{}.data<&boxed_int::value>("value"_hs);
        return true;
    }();

    (void)registered;
}

template<typename Type>
const Type &as_const_ref(entt::meta_any &any) {
    return entt::any_cast<const Type &>(any);
}

template<typename Type>
Type &as_ref(entt::meta_any &any) {
    return entt::any_cast<Type &>(any);
}

TEST(MetaAnyDeref, DereferencePointerLikeYieldsReferenceToPointee) {
    register_meta_types();

    boxed_int value{42};
    entt::meta_any any = std::make_shared<boxed_int>(value);

    ASSERT_TRUE(any);
    ASSERT_TRUE(any.allow_cast<boxed_int>());

    entt::meta_any deref = *any;

    ASSERT_TRUE(deref);
    EXPECT_EQ(deref.type().id(), entt::resolve<boxed_int>().id());

    auto &obj = as_ref<boxed_int>(deref);
    EXPECT_EQ(obj.value, 42);

    obj.value = 7;
    EXPECT_EQ(as_const_ref<boxed_int>(deref).value, 7);

    auto &ptr_obj = *entt::any_cast<const std::shared_ptr<boxed_int> &>(any);
    EXPECT_EQ(&obj, &ptr_obj);
    EXPECT_EQ(ptr_obj.value, 7);
}

TEST(MetaAnyDeref, DereferenceConstPointerLikePreservesConstAccess) {
    register_meta_types();

    const auto ptr = std::make_shared<const boxed_int>(boxed_int{9});
    entt::meta_any any = ptr;

    ASSERT_TRUE(any);

    entt::meta_any deref = *any;

    ASSERT_TRUE(deref);
    EXPECT_EQ(deref.type().id(), entt::resolve<boxed_int>().id());

    const auto &obj = as_const_ref<boxed_int>(deref);
    EXPECT_EQ(obj.value, 9);
    EXPECT_EQ(&obj, ptr.get());
}

TEST(MetaAnyDeref, DereferenceMapIteratorReturnsPairReferences) {
    register_meta_types();

    std::map<int, boxed_int> map{{1, {10}}, {2, {20}}};
    auto it = map.find(2);
    ASSERT_NE(it, map.end());

    entt::meta_any any = it;
    ASSERT_TRUE(any);

    entt::meta_any deref = *any;
    ASSERT_TRUE(deref);

    using pair_type = std::pair<const int, boxed_int>;
    EXPECT_EQ(deref.type().id(), entt::resolve<pair_type>().id());

    auto &entry = as_ref<pair_type>(deref);
    EXPECT_EQ(entry.first, 2);
    EXPECT_EQ(entry.second.value, 20);

    entry.second.value = 99;
    EXPECT_EQ(map.at(2).value, 99);
    EXPECT_EQ(&entry.second, &it->second);
    EXPECT_EQ(&entry.first, &it->first);
}

TEST(MetaAnyDeref, DereferenceConstMapIteratorReturnsReadableReferences) {
    register_meta_types();

    const std::map<int, boxed_int> map{{4, {40}}};
    auto it = map.begin();
    ASSERT_NE(it, map.end());

    entt::meta_any any = it;
    ASSERT_TRUE(any);

    entt::meta_any deref = *any;
    ASSERT_TRUE(deref);

    using pair_type = std::pair<const int, boxed_int>;
    const auto &entry = as_const_ref<pair_type>(deref);

    EXPECT_EQ(entry.first, 4);
    EXPECT_EQ(entry.second.value, 40);
    EXPECT_EQ(&entry.first, &it->first);
    EXPECT_EQ(&entry.second, &it->second);
}

TEST(MetaAnyDeref, DereferenceEndIteratorStillExposesReferencedStorageObject) {
    register_meta_types();

    std::map<int, boxed_int> map{};
    auto it = map.end();

    entt::meta_any any = it;
    ASSERT_TRUE(any);

    entt::meta_any deref = *any;

    // Even for end iterators, the public API should not synthesize emptiness here;
    // a buggy implementation that mishandles the underlying pair/reference wiring
    // would fail subsequent casts/type checks.
    ASSERT_TRUE(deref);
    using pair_type = std::pair<const int, boxed_int>;
    EXPECT_EQ(deref.type().id(), entt::resolve<pair_type>().id());
}

TEST(MetaAnyDeref, DereferenceInvalidMetaAnyProducesEmptyMetaAny) {
    entt::meta_any any{};

    ASSERT_FALSE(any);

    entt::meta_any deref = *any;

    EXPECT_FALSE(deref);
    EXPECT_EQ(deref.type().id(), entt::type_id<void>().hash());
}

TEST(MetaAnyDeref, DereferenceNonPointerNonIteratorProducesEmptyMetaAny) {
    register_meta_types();

    entt::meta_any any = 123;

    ASSERT_TRUE(any);

    entt::meta_any deref = *any;

    EXPECT_FALSE(deref);
    EXPECT_EQ(deref.type().id(), entt::type_id<void>().hash());
}

} // namespace