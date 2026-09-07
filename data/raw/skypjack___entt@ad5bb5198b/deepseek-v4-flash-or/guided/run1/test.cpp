#include <entt/meta/meta.hpp>

#include <gtest/gtest.h>

#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

TEST(MetaAssociativeContainerConstruction, StdMapWrapper) {
    std::map<int, std::string> map;
    entt::meta_associative_container container{map};

    ASSERT_FALSE(container.key_only());
    ASSERT_EQ(container.size(), 0u);

    ASSERT_TRUE(container.key_type() == entt::resolve<int>());
    ASSERT_TRUE(container.mapped_type() == entt::resolve<std::string>());
    ASSERT_TRUE(container.value_type() == entt::resolve<std::map<int, std::string>::value_type>());

    ASSERT_TRUE(container.insert(entt::meta_any{1}, entt::meta_any{std::string{"one"}}));
    ASSERT_TRUE(container.insert(entt::meta_any{2}, entt::meta_any{std::string{"two"}}));
    ASSERT_EQ(container.size(), 2u);
    ASSERT_EQ(map.size(), 2u);
    ASSERT_EQ(map.at(1), "one");
    ASSERT_EQ(map.at(2), "two");

    ASSERT_FALSE(container.insert(entt::meta_any{1}, entt::meta_any{std::string{"uno"}}));
    ASSERT_EQ(container.size(), 2u);
    ASSERT_EQ(map.at(1), "one");

    ASSERT_TRUE(container.find(entt::meta_any{1}) != container.end());
    ASSERT_TRUE(container.find(entt::meta_any{42}) == container.end());

    std::size_t count = 0u;
    for (auto it = container.begin(); it != container.end(); ++it) {
        ++count;
    }
    ASSERT_EQ(count, 2u);

    ASSERT_TRUE(container.erase(entt::meta_any{1}));
    ASSERT_EQ(container.size(), 1u);
    ASSERT_EQ(map.count(1), 0u);
    ASSERT_EQ(map.count(2), 1u);

    ASSERT_FALSE(container.erase(entt::meta_any{1}));
    ASSERT_EQ(container.size(), 1u);

    container.clear();
    ASSERT_TRUE(map.empty());
    ASSERT_EQ(container.size(), 0u);
}

TEST(MetaAssociativeContainerConstruction, StdUnorderedMapWrapper) {
    std::unordered_map<int, int> map{{1, 10}, {2, 20}};
    entt::meta_associative_container container{map};

    ASSERT_FALSE(container.key_only());
    ASSERT_EQ(container.size(), 2u);

    ASSERT_TRUE(container.key_type() == entt::resolve<int>());
    ASSERT_TRUE(container.mapped_type() == entt::resolve<int>());
    ASSERT_TRUE(container.value_type() == entt::resolve<std::unordered_map<int, int>::value_type>());

    ASSERT_TRUE(container.insert(entt::meta_any{3}, entt::meta_any{30}));
    ASSERT_EQ(map.at(3), 30);
    ASSERT_EQ(container.size(), 3u);

    ASSERT_FALSE(container.insert(entt::meta_any{3}, entt::meta_any{300}));
    ASSERT_EQ(map.at(3), 30);

    ASSERT_TRUE(container.find(entt::meta_any{2}) != container.end());
    ASSERT_TRUE(container.find(entt::meta_any{42}) == container.end());

    ASSERT_TRUE(container.erase(entt::meta_any{2}));
    ASSERT_EQ(map.count(2), 0u);
    ASSERT_EQ(container.size(), 2u);

    container.clear();
    ASSERT_TRUE(map.empty());
    ASSERT_EQ(container.size(), 0u);
}

TEST(MetaAssociativeContainerConstruction, StdSetWrapper) {
    std::set<int> set;
    entt::meta_associative_container container{set};

    ASSERT_TRUE(container.key_only());
    ASSERT_EQ(container.size(), 0u);

    ASSERT_TRUE(container.key_type() == entt::resolve<int>());
    ASSERT_TRUE(container.value_type() == entt::resolve<int>());

    ASSERT_TRUE(container.insert(entt::meta_any{1}));
    ASSERT_TRUE(container.insert(entt::meta_any{2}));
    ASSERT_EQ(container.size(), 2u);
    ASSERT_EQ(set.size(), 2u);

    ASSERT_FALSE(container.insert(entt::meta_any{1}));
    ASSERT_EQ(container.size(), 2u);

    ASSERT_TRUE(container.find(entt::meta_any{1}) != container.end());
    ASSERT_TRUE(container.find(entt::meta_any{42}) == container.end());

    std::size_t count = 0u;
    for (auto it = container.begin(); it != container.end(); ++it) {
        ++count;
    }
    ASSERT_EQ(count, 2u);

    ASSERT_TRUE(container.erase(entt::meta_any{1}));
    ASSERT_EQ(set.count(1), 0u);
    ASSERT_EQ(container.size(), 1u);

    ASSERT_FALSE(container.erase(entt::meta_any{1}));
    ASSERT_EQ(container.size(), 1u);

    container.clear();
    ASSERT_TRUE(set.empty());
    ASSERT_EQ(container.size(), 0u);
}

TEST(MetaAssociativeContainerConstruction, StdUnorderedSetWrapper) {
    std::unordered_set<int> set{10, 20};
    entt::meta_associative_container container{set};

    ASSERT_TRUE(container.key_only());
    ASSERT_EQ(container.size(), 2u);

    ASSERT_TRUE(container.key_type() == entt::resolve<int>());
    ASSERT_TRUE(container.value_type() == entt::resolve<int>());

    ASSERT_TRUE(container.insert(entt::meta_any{30}));
    ASSERT_EQ(set.count(30), 1u);
    ASSERT_EQ(set.size(), 3u);
    ASSERT_EQ(container.size(), 3u);

    ASSERT_FALSE(container.insert(entt::meta_any{30}));
    ASSERT_EQ(set.size(), 3u);

    ASSERT_TRUE(container.find(entt::meta_any{10}) != container.end());
    ASSERT_TRUE(container.find(entt::meta_any{42}) == container.end());

    ASSERT_TRUE(container.erase(entt::meta_any{10}));
    ASSERT_EQ(set.count(10), 0u);
    ASSERT_EQ(container.size(), 2u);

    container.clear();
    ASSERT_TRUE(set.empty());
    ASSERT_EQ(container.size(), 0u);
}