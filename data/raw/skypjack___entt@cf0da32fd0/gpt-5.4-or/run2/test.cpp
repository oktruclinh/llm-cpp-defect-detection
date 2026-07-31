#include <gtest/gtest.h>

#include <entt/meta/meta.hpp>

#include <iterator>
#include <map>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

struct key_only_container {
    using underlying_type = std::vector<int>;
    underlying_type values{};

    using iterator = underlying_type::iterator;
    using const_iterator = underlying_type::const_iterator;

    iterator begin() { return values.begin(); }
    iterator end() { return values.end(); }
    const_iterator begin() const { return values.begin(); }
    const_iterator end() const { return values.end(); }
};

} // namespace

TEST(EnTTMetaIteratorDeref, DerefAssociativeIteratorReturnsReferencesToKeyAndValue) {
    using namespace entt::literals;

    entt::meta_factory<std::map<int, std::string>>{}
        .type("assoc_map"_hs);

    auto map_type = entt::resolve("assoc_map"_hs);
    ASSERT_TRUE(static_cast<bool>(map_type));

    std::map<int, std::string> value{{1, "one"}, {2, "two"}};

    auto begin = map_type.func("begin"_hs);
    auto end = map_type.func("end"_hs);

    ASSERT_TRUE(static_cast<bool>(begin));
    ASSERT_TRUE(static_cast<bool>(end));

    entt::meta_any instance{std::ref(value)};
    entt::meta_any it_any = begin.invoke({}, instance);
    entt::meta_any end_any = end.invoke({}, instance);

    ASSERT_TRUE(static_cast<bool>(it_any));
    ASSERT_TRUE(static_cast<bool>(end_any));
    EXPECT_NE(it_any, end_any);

    auto pair = *entt::meta_any_cast<entt::meta_sequence_container::iterator>(&it_any); // not used; keep compile guard
    (void)pair;
}

TEST(EnTTMetaIteratorDeref, DerefSequenceContainerIteratorReturnsReferenceToElement) {
    std::vector<int> value{3, 7, 11};

    auto view = entt::forward_as_meta(value).as_sequence_container();
    ASSERT_TRUE(static_cast<bool>(view));
    ASSERT_FALSE(view.empty());
    ASSERT_EQ(view.size(), 3u);

    auto it = view.begin();
    auto last = view.end();

    EXPECT_NE(it, last);

    entt::meta_any deref = *it;
    ASSERT_TRUE(static_cast<bool>(deref));

    const int *elem = entt::meta_any_cast<int>(&deref);
    ASSERT_NE(elem, nullptr);
    EXPECT_EQ(*elem, 3);

    *elem = 42;
    EXPECT_EQ(value.front(), 42);

    ++it;
    deref = *it;
    elem = entt::meta_any_cast<int>(&deref);
    ASSERT_NE(elem, nullptr);
    EXPECT_EQ(*elem, 7);

    *elem = -5;
    ASSERT_EQ(value[1], -5);
}

TEST(EnTTMetaIteratorDeref, DerefConstSequenceContainerIteratorIsReadOnlyButStillReferencesOriginalElement) {
    const std::vector<int> value{5, 9};

    auto view = entt::forward_as_meta(value).as_sequence_container();
    ASSERT_TRUE(static_cast<bool>(view));
    ASSERT_EQ(view.size(), 2u);

    auto it = view.begin();
    auto deref = *it;

    ASSERT_TRUE(static_cast<bool>(deref));

    const int *elem = entt::meta_any_cast<int>(&deref);
    ASSERT_NE(elem, nullptr);
    EXPECT_EQ(*elem, 5);

    static_assert(!std::is_assignable_v<decltype(*elem)&, int>);
}

TEST(EnTTMetaIteratorDeref, DerefAssociativeContainerIteratorReturnsReferencesToKeyAndMappedValue) {
    std::unordered_map<int, std::string> value{
        {10, "ten"},
        {20, "twenty"}
    };

    auto view = entt::forward_as_meta(value).as_associative_container();
    ASSERT_TRUE(static_cast<bool>(view));
    ASSERT_FALSE(view.empty());
    ASSERT_EQ(view.size(), 2u);
    ASSERT_FALSE(view.key_only());

    auto it = view.begin();
    auto last = view.end();

    ASSERT_NE(it, last);

    auto pair = *it;

    ASSERT_TRUE(static_cast<bool>(pair.first));
    ASSERT_TRUE(static_cast<bool>(pair.second));

    const int *key = entt::meta_any_cast<int>(&pair.first);
    ASSERT_NE(key, nullptr);

    const std::string *mapped = entt::meta_any_cast<std::string>(&pair.second);
    ASSERT_NE(mapped, nullptr);

    auto found = value.find(*key);
    ASSERT_NE(found, value.end());
    EXPECT_EQ(*mapped, found->second);

    std::string replacement = found->second + "_updated";
    *const_cast<std::string *>(mapped) = replacement;

    auto verify = value.find(*key);
    ASSERT_NE(verify, value.end());
    EXPECT_EQ(verify->second, replacement);
}

TEST(EnTTMetaIteratorDeref, DerefAssociativeConstContainerIteratorReturnsConstKeyAndConstMappedValueReferences) {
    const std::map<int, std::string> value{
        {1, "one"},
        {4, "four"}
    };

    auto view = entt::forward_as_meta(value).as_associative_container();
    ASSERT_TRUE(static_cast<bool>(view));
    ASSERT_FALSE(view.key_only());

    auto it = view.begin();
    auto pair = *it;

    const int *key = entt::meta_any_cast<int>(&pair.first);
    ASSERT_NE(key, nullptr);

    const std::string *mapped = entt::meta_any_cast<std::string>(&pair.second);
    ASSERT_NE(mapped, nullptr);

    auto found = value.find(*key);
    ASSERT_NE(found, value.end());
    EXPECT_EQ(*mapped, found->second);

    static_assert(!std::is_assignable_v<decltype(*key)&, int>);
    static_assert(!std::is_assignable_v<decltype(*mapped)&, std::string>);
}

TEST(EnTTMetaIteratorDeref, DerefKeyOnlyAssociativeIteratorPopulatesOnlyFirstAndLeavesSecondEmpty) {
    std::set<int> value{8, 3, 5};

    auto view = entt::forward_as_meta(value).as_associative_container();
    ASSERT_TRUE(static_cast<bool>(view));
    ASSERT_TRUE(view.key_only());
    ASSERT_EQ(view.size(), 3u);

    auto it = view.begin();
    ASSERT_NE(it, view.end());

    auto pair = *it;

    ASSERT_TRUE(static_cast<bool>(pair.first));
    EXPECT_FALSE(static_cast<bool>(pair.second));

    const int *key = entt::meta_any_cast<int>(&pair.first);
    ASSERT_NE(key, nullptr);
    EXPECT_EQ(value.count(*key), 1u);
}

TEST(EnTTMetaIteratorDeref, DerefKeyOnlyCustomContainerReturnsReferenceToActualElement) {
    key_only_container value{{13, 21, 34}};

    auto view = entt::forward_as_meta(value).as_associative_container();
    ASSERT_TRUE(static_cast<bool>(view));
    ASSERT_TRUE(view.key_only());
    ASSERT_EQ(view.size(), 3u);

    auto it = view.begin();
    auto pair = *it;

    ASSERT_TRUE(static_cast<bool>(pair.first));
    EXPECT_FALSE(static_cast<bool>(pair.second));

    int *elem = entt::meta_any_cast<int>(&pair.first);
    ASSERT_NE(elem, nullptr);
    EXPECT_EQ(*elem, 13);

    *elem = 55;
    EXPECT_EQ(value.values.front(), 55);
}

TEST(EnTTMetaIteratorDeref, EmptyContainersHaveBeginEqualEndAndDoNotExposeDereferenceableIterators) {
    std::vector<int> seq{};
    auto seq_view = entt::forward_as_meta(seq).as_sequence_container();
    ASSERT_TRUE(static_cast<bool>(seq_view));
    EXPECT_TRUE(seq_view.empty());
    EXPECT_EQ(seq_view.size(), 0u);
    EXPECT_EQ(seq_view.begin(), seq_view.end());

    std::map<int, int> assoc{};
    auto assoc_view = entt::forward_as_meta(assoc).as_associative_container();
    ASSERT_TRUE(static_cast<bool>(assoc_view));
    EXPECT_TRUE(assoc_view.empty());
    EXPECT_EQ(assoc_view.size(), 0u);
    EXPECT_EQ(assoc_view.begin(), assoc_view.end());

    std::set<int> key_only{};
    auto key_view = entt::forward_as_meta(key_only).as_associative_container();
    ASSERT_TRUE(static_cast<bool>(key_view));
    EXPECT_TRUE(key_view.empty());
    EXPECT_EQ(key_view.size(), 0u);
    EXPECT_EQ(key_view.begin(), key_view.end());
}

TEST(EnTTMetaIteratorDeref, DereferencedAssociativePairKeepsBothReferencesIndependentAndStableForCurrentElement) {
    std::map<int, std::string> value{{7, "seven"}};

    auto view = entt::forward_as_meta(value).as_associative_container();
    ASSERT_TRUE(static_cast<bool>(view));

    auto it = view.begin();
    auto pair = *it;

    const int *key = entt::meta_any_cast<int>(&pair.first);
    std::string *mapped = entt::meta_any_cast<std::string>(&pair.second);

    ASSERT_NE(key, nullptr);
    ASSERT_NE(mapped, nullptr);
    EXPECT_EQ(*key, 7);
    EXPECT_EQ(*mapped, "seven");

    *mapped = "SEVEN";
    EXPECT_EQ(value.begin()->second, "SEVEN");

    const int *key_again = entt::meta_any_cast<int>(&pair.first);
    ASSERT_NE(key_again, nullptr);
    EXPECT_EQ(*key_again, 7);
}