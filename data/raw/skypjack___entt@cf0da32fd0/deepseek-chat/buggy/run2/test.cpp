#include <gtest/gtest.h>
#include <entt/entt.hpp>
#include <utility>

struct MetaDerefTest : ::testing::Test {
    void SetUp() override {
        entt::meta<double>().type("double"_hs);
        entt::meta<int>().type("int"_hs);
    }
};

TEST_F(MetaDerefTest, DerefKeyOnlyFromMap) {
    entt::meta_any map_any = entt::meta_any{std::unordered_map<int, double>{{42, 3.14}}};
    auto view = map_any.as_ref().allow(entt::as_ref_t{});
    auto it = *view.begin();

    // Dereference the iterator (key-only path)
    entt::meta_any key = it;
    ASSERT_TRUE(key);
    ASSERT_EQ(key.cast<int>(), 42);
}

TEST_F(MetaDerefTest, DerefKeyValueFromMap) {
    entt::meta_any map_any = entt::meta_any{std::unordered_map<int, double>{{7, 2.71}}};
    auto view = map_any.as_ref().allow(entt::as_ref_t{});
    auto it = *view.begin();

    // Dereference the iterator (key-value path)
    auto pair = it.as_ref().allow(entt::as_ref_t{});
    ASSERT_TRUE(pair);
    ASSERT_EQ(pair->first.cast<int>(), 7);
    ASSERT_DOUBLE_EQ(pair->second.cast<double>(), 2.71);
}

TEST_F(MetaDerefTest, DerefEmptyMap) {
    entt::meta_any map_any = entt::meta_any{std::unordered_map<int, double>{}};
    auto view = map_any.as_ref().allow(entt::as_ref_t{});
    ASSERT_EQ(view.begin(), view.end());
}

TEST_F(MetaDerefTest, DerefSingleElementMap) {
    entt::meta_any map_any = entt::meta_any{std::unordered_map<int, double>{{-1, 0.0}}};
    auto view = map_any.as_ref().allow(entt::as_ref_t{});
    auto it = *view.begin();

    auto pair = it.as_ref().allow(entt::as_ref_t{});
    ASSERT_EQ(pair->first.cast<int>(), -1);
    ASSERT_DOUBLE_EQ(pair->second.cast<double>(), 0.0);
}

TEST_F(MetaDerefTest, DerefMultipleElementsMap) {
    entt::meta_any map_any = entt::meta_any{std::unordered_map<int, double>{{1, 1.1}, {2, 2.2}, {3, 3.3}}};
    auto view = map_any.as_ref().allow(entt::as_ref_t{});
    int count = 0;
    for (auto it = view.begin(); it != view.end(); ++it) {
        auto pair = it->as_ref().allow(entt::as_ref_t{});
        ASSERT_TRUE(pair);
        ASSERT_TRUE(pair->first.cast<int>() >= 1 && pair->first.cast<int>() <= 3);
        ++count;
    }
    ASSERT_EQ(count, 3);
}

TEST_F(MetaDerefTest, DerefNonMapContainer) {
    entt::meta_any vec_any = entt::meta_any{std::vector<int>{10, 20, 30}};
    auto view = vec_any.as_ref().allow(entt::as_ref_t{});
    auto it = *view.begin();
    ASSERT_TRUE(it);
    ASSERT_EQ(it.cast<int>(), 10);
}

TEST_F(MetaDerefTest, DerefConstMap) {
    const std::unordered_map<int, double> const_map{{5, 5.5}};
    entt::meta_any map_any = entt::meta_any{std::cref(const_map)};
    auto view = map_any.as_ref().allow(entt::as_ref_t{});
    auto it = *view.begin();

    auto pair = it.as_ref().allow(entt::as_ref_t{});
    ASSERT_EQ(pair->first.cast<int>(), 5);
    ASSERT_DOUBLE_EQ(pair->second.cast<double>(), 5.5);
}

TEST_F(MetaDerefTest, DerefMapWithStringKeys) {
    entt::meta_any map_any = entt::meta_any{std::unordered_map<std::string, int>{{"hello", 1}}};
    auto view = map_any.as_ref().allow(entt::as_ref_t{});
    auto it = *view.begin();

    auto pair = it.as_ref().allow(entt::as_ref_t{});
    ASSERT_EQ(pair->first.cast<std::string>(), "hello");
    ASSERT_EQ(pair->second.cast<int>(), 1);
}

TEST_F(MetaDerefTest, DerefMapWithCustomTypeKey) {
    struct CustomKey { int id; };
    entt::meta<CustomKey>().type("custom_key"_hs).data<&CustomKey::id>("id"_hs);
    entt::meta_any map_any = entt::meta_any{std::unordered_map<CustomKey, double>{{{1}, 1.0}}};
    auto view = map_any.as_ref().allow(entt::as_ref_t{});
    auto it = *view.begin();

    auto pair = it.as_ref().allow(entt::as_ref_t{});
    ASSERT_EQ(pair->first.cast<CustomKey>().id, 1);
    ASSERT_DOUBLE_EQ(pair->second.cast<double>(), 1.0);
}