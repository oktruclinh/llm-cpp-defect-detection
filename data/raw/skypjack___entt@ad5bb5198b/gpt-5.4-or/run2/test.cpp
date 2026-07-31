#include <gtest/gtest.h>

#include <entt/meta/meta.hpp>

#include <map>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

namespace {

template<typename Type>
entt::id_type type_id_value() {
    return entt::type_id<Type>().hash();
}

TEST(MetaAssociativeContainerCtor, KeyOnlyContainerReportsCorrectTraitsForUnorderedSet) {
    std::unordered_set<int> container{1, 2, 3};

    entt::meta_any any{std::ref(container)};
    ASSERT_TRUE(any);

    auto view = any.as_associative_container();
    ASSERT_TRUE(view);

    EXPECT_TRUE(view.key_only());
    EXPECT_EQ(view.size(), container.size());

    const entt::meta_type key_type = view.key_type();
    const entt::meta_type value_type = view.value_type();
    const entt::meta_type mapped_type = view.mapped_type();

    ASSERT_TRUE(key_type);
    ASSERT_TRUE(value_type);
    EXPECT_FALSE(mapped_type);

    EXPECT_EQ(key_type.id(), type_id_value<int>());
    EXPECT_EQ(value_type.id(), type_id_value<int>());
}

TEST(MetaAssociativeContainerCtor, MappedContainerReportsCorrectTraitsForMap) {
    std::map<int, char> container{{1, 'a'}, {2, 'b'}};

    entt::meta_any any{std::ref(container)};
    ASSERT_TRUE(any);

    auto view = any.as_associative_container();
    ASSERT_TRUE(view);

    EXPECT_FALSE(view.key_only());
    EXPECT_EQ(view.size(), container.size());

    const entt::meta_type key_type = view.key_type();
    const entt::meta_type value_type = view.value_type();
    const entt::meta_type mapped_type = view.mapped_type();

    ASSERT_TRUE(key_type);
    ASSERT_TRUE(value_type);
    ASSERT_TRUE(mapped_type);

    EXPECT_EQ(key_type.id(), type_id_value<int>());
    EXPECT_EQ(mapped_type.id(), type_id_value<char>());

    using expected_value_type = std::pair<const int, char>;
    EXPECT_EQ(value_type.id(), type_id_value<expected_value_type>());
}

TEST(MetaAssociativeContainerCtor, EmptyMappedContainerStillExposesStaticMetaInformation) {
    std::unordered_map<std::string, long> container{};

    entt::meta_any any{std::ref(container)};
    ASSERT_TRUE(any);

    auto view = any.as_associative_container();
    ASSERT_TRUE(view);

    EXPECT_FALSE(view.key_only());
    EXPECT_TRUE(view.empty());
    EXPECT_EQ(view.size(), 0u);

    const entt::meta_type key_type = view.key_type();
    const entt::meta_type value_type = view.value_type();
    const entt::meta_type mapped_type = view.mapped_type();

    ASSERT_TRUE(key_type);
    ASSERT_TRUE(value_type);
    ASSERT_TRUE(mapped_type);

    EXPECT_EQ(key_type.id(), type_id_value<std::string>());
    EXPECT_EQ(mapped_type.id(), type_id_value<long>());

    using expected_value_type = std::pair<const std::string, long>;
    EXPECT_EQ(value_type.id(), type_id_value<expected_value_type>());
}

TEST(MetaAssociativeContainerCtor, EmptyKeyOnlyContainerStillExposesStaticMetaInformation) {
    std::unordered_set<std::string> container{};

    entt::meta_any any{std::ref(container)};
    ASSERT_TRUE(any);

    auto view = any.as_associative_container();
    ASSERT_TRUE(view);

    EXPECT_TRUE(view.key_only());
    EXPECT_TRUE(view.empty());
    EXPECT_EQ(view.size(), 0u);

    const entt::meta_type key_type = view.key_type();
    const entt::meta_type value_type = view.value_type();
    const entt::meta_type mapped_type = view.mapped_type();

    ASSERT_TRUE(key_type);
    ASSERT_TRUE(value_type);
    EXPECT_FALSE(mapped_type);

    EXPECT_EQ(key_type.id(), type_id_value<std::string>());
    EXPECT_EQ(value_type.id(), type_id_value<std::string>());
}

TEST(MetaAssociativeContainerCtor, DifferentContainerKindsExposeDifferentKeyOnlyFlagsAndMappedTypes) {
    std::unordered_set<int> set_container{};
    std::map<int, int> map_container{};

    entt::meta_any set_any{std::ref(set_container)};
    entt::meta_any map_any{std::ref(map_container)};

    auto set_view = set_any.as_associative_container();
    auto map_view = map_any.as_associative_container();

    ASSERT_TRUE(set_view);
    ASSERT_TRUE(map_view);

    EXPECT_TRUE(set_view.key_only());
    EXPECT_FALSE(map_view.key_only());

    EXPECT_FALSE(set_view.mapped_type());
    EXPECT_TRUE(map_view.mapped_type());

    EXPECT_EQ(set_view.key_type().id(), type_id_value<int>());
    EXPECT_EQ(set_view.value_type().id(), type_id_value<int>());

    EXPECT_EQ(map_view.key_type().id(), type_id_value<int>());
    EXPECT_EQ(map_view.mapped_type().id(), type_id_value<int>());

    using expected_map_value_type = std::pair<const int, int>;
    EXPECT_EQ(map_view.value_type().id(), type_id_value<expected_map_value_type>());
}

TEST(MetaAssociativeContainerCtor, NonAssociativeMetaAnyDoesNotProduceAssociativeContainerView) {
    int value = 42;

    entt::meta_any any{std::ref(value)};
    ASSERT_TRUE(any);

    auto view = any.as_associative_container();
    EXPECT_FALSE(view);
}

TEST(MetaAssociativeContainerCtor, ConstAssociativeContainerPreservesTraits) {
    const std::map<short, double> container{{1, 1.5}, {2, 3.0}};

    entt::meta_any any{std::cref(container)};
    ASSERT_TRUE(any);

    auto view = any.as_associative_container();
    ASSERT_TRUE(view);

    EXPECT_FALSE(view.key_only());
    EXPECT_EQ(view.size(), container.size());

    ASSERT_TRUE(view.key_type());
    ASSERT_TRUE(view.mapped_type());
    ASSERT_TRUE(view.value_type());

    EXPECT_EQ(view.key_type().id(), type_id_value<short>());
    EXPECT_EQ(view.mapped_type().id(), type_id_value<double>());

    using expected_value_type = std::pair<const short, double>;
    EXPECT_EQ(view.value_type().id(), type_id_value<expected_value_type>());
}

} // namespace