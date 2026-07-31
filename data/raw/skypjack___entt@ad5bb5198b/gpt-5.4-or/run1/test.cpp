#include <gtest/gtest.h>

#include <entt/meta/meta.hpp>

#include <map>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

namespace {

using namespace entt;

struct non_default_constructible {
    non_default_constructible() = delete;
    explicit non_default_constructible(int v)
        : value{v} {}

    int value{};
};

TEST(MetaAssociativeContainerCtor, EmptyStdMapExposesMappedAssociativeTraits) {
    std::map<int, char> container{};
    meta_associative_container view{container};

    ASSERT_TRUE(static_cast<bool>(view));
    EXPECT_FALSE(view.key_only());

    const meta_type key = view.key_type();
    const meta_type mapped = view.mapped_type();
    const meta_type value = view.value_type();

    ASSERT_TRUE(static_cast<bool>(key));
    ASSERT_TRUE(static_cast<bool>(mapped));
    ASSERT_TRUE(static_cast<bool>(value));

    EXPECT_EQ(key, resolve<int>());
    EXPECT_EQ(mapped, resolve<char>());
    EXPECT_EQ(value, resolve<std::pair<const int, char>>());

    EXPECT_TRUE(view.empty());
    EXPECT_EQ(view.size(), 0u);

    EXPECT_EQ(view.begin(), view.end());
    EXPECT_EQ(view.find(42), view.end());
}

TEST(MetaAssociativeContainerCtor, EmptyStdUnorderedSetExposesKeyOnlyAssociativeTraits) {
    std::unordered_set<int> container{};
    meta_associative_container view{container};

    ASSERT_TRUE(static_cast<bool>(view));
    EXPECT_TRUE(view.key_only());

    const meta_type key = view.key_type();
    const meta_type mapped = view.mapped_type();
    const meta_type value = view.value_type();

    ASSERT_TRUE(static_cast<bool>(key));
    EXPECT_FALSE(static_cast<bool>(mapped));
    ASSERT_TRUE(static_cast<bool>(value));

    EXPECT_EQ(key, resolve<int>());
    EXPECT_EQ(value, resolve<int>());

    EXPECT_TRUE(view.empty());
    EXPECT_EQ(view.size(), 0u);

    EXPECT_EQ(view.begin(), view.end());
    EXPECT_EQ(view.find(1), view.end());
}

TEST(MetaAssociativeContainerCtor, NonEmptyMapSupportsFindAndMutationThroughPublicApi) {
    std::map<int, std::string> container{{1, "one"}, {2, "two"}};
    meta_associative_container view{container};

    ASSERT_TRUE(static_cast<bool>(view));
    EXPECT_FALSE(view.key_only());
    EXPECT_EQ(view.size(), 2u);
    EXPECT_FALSE(view.empty());

    auto it = view.find(2);
    ASSERT_NE(it, view.end());

    auto pair = *it;

    ASSERT_TRUE(static_cast<bool>(pair.first));
    ASSERT_TRUE(static_cast<bool>(pair.second));

    EXPECT_EQ(pair.first.cast<int>(), 2);
    EXPECT_EQ(pair.second.cast<std::string>(), "two");

    ASSERT_TRUE(view.insert(3, std::string{"three"}));
    EXPECT_EQ(container.size(), 3u);
    EXPECT_EQ(container.at(3), "three");

    ASSERT_FALSE(view.insert(3, std::string{"THREE"}));
    EXPECT_EQ(container.at(3), "three");

    auto inserted = view.find(3);
    ASSERT_NE(inserted, view.end());
    auto inserted_pair = *inserted;
    EXPECT_EQ(inserted_pair.first.cast<int>(), 3);
    EXPECT_EQ(inserted_pair.second.cast<std::string>(), "three");

    EXPECT_TRUE(view.erase(1));
    EXPECT_EQ(container.count(1), 0u);

    EXPECT_FALSE(view.erase(42));
    EXPECT_EQ(container.size(), 2u);
}

TEST(MetaAssociativeContainerCtor, KeyOnlyContainerIgnoresMappedValueAndUsesKeyTypeOnly) {
    std::unordered_set<int> container{4, 8};
    meta_associative_container view{container};

    ASSERT_TRUE(static_cast<bool>(view));
    EXPECT_TRUE(view.key_only());

    auto it = view.find(8);
    ASSERT_NE(it, view.end());

    auto elem = *it;
    ASSERT_TRUE(static_cast<bool>(elem.first));
    EXPECT_FALSE(static_cast<bool>(elem.second));
    EXPECT_EQ(elem.first.cast<int>(), 8);

    EXPECT_TRUE(view.insert(16, meta_any{}));
    EXPECT_EQ(container.count(16), 1u);

    EXPECT_FALSE(view.insert(16, meta_any{}));
    EXPECT_EQ(container.count(16), 1u);

    EXPECT_TRUE(view.erase(4));
    EXPECT_EQ(container.count(4), 0u);

    EXPECT_FALSE(view.erase(999));
}

TEST(MetaAssociativeContainerCtor, IteratorValueTypeMatchesUnderlyingContainerKind) {
    std::map<int, char> mapped_container{{7, 'x'}};
    meta_associative_container mapped_view{mapped_container};

    auto mapped_it = mapped_view.begin();
    ASSERT_NE(mapped_it, mapped_view.end());
    auto mapped_pair = *mapped_it;

    EXPECT_EQ(mapped_view.value_type(), resolve<std::pair<const int, char>>());
    EXPECT_EQ(mapped_pair.first.cast<int>(), 7);
    EXPECT_EQ(mapped_pair.second.cast<char>(), 'x');

    std::unordered_set<int> key_only_container{9};
    meta_associative_container key_only_view{key_only_container};

    auto key_only_it = key_only_view.begin();
    ASSERT_NE(key_only_it, key_only_view.end());
    auto key_only_pair = *key_only_it;

    EXPECT_EQ(key_only_view.value_type(), resolve<int>());
    EXPECT_EQ(key_only_pair.first.cast<int>(), 9);
    EXPECT_FALSE(static_cast<bool>(key_only_pair.second));
}

TEST(MetaAssociativeContainerCtor, SupportsContainersWithNonDefaultConstructibleMappedType) {
    std::map<int, non_default_constructible> container{};
    container.emplace(std::piecewise_construct,
                      std::forward_as_tuple(5),
                      std::forward_as_tuple(99));

    meta_associative_container view{container};

    ASSERT_TRUE(static_cast<bool>(view));
    EXPECT_FALSE(view.key_only());
    EXPECT_EQ(view.key_type(), resolve<int>());
    EXPECT_EQ(view.mapped_type(), resolve<non_default_constructible>());
    EXPECT_EQ(view.value_type(), resolve<std::pair<const int, non_default_constructible>>());

    auto it = view.find(5);
    ASSERT_NE(it, view.end());

    auto pair = *it;
    EXPECT_EQ(pair.first.cast<int>(), 5);

    const auto &ref = pair.second.cast<non_default_constructible &>();
    EXPECT_EQ(ref.value, 99);
}

TEST(MetaAssociativeContainerCtor, DefaultConstructedViewIsInvalidButConstructedViewIsValid) {
    meta_associative_container invalid{};
    EXPECT_FALSE(static_cast<bool>(invalid));

    std::map<int, int> container{};
    meta_associative_container valid{container};
    EXPECT_TRUE(static_cast<bool>(valid));

    EXPECT_FALSE(valid.key_only());
    EXPECT_EQ(valid.key_type(), resolve<int>());
    EXPECT_EQ(valid.mapped_type(), resolve<int>());
    EXPECT_EQ(valid.value_type(), resolve<std::pair<const int, int>>());
}

} // namespace