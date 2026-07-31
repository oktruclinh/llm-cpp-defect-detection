#include <gtest/gtest.h>

#include <map>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include <entt/meta/meta.hpp>

namespace {

TEST(MetaAssociativeContainerCtor, SetIsRecognizedAsKeyOnlyAssociativeContainer) {
    std::map<int, char> map_like{};
    std::set<int> key_only{};

    entt::meta_associative_container map_meta{map_like};
    entt::meta_associative_container set_meta{key_only};

    EXPECT_FALSE(map_meta.key_only());
    EXPECT_TRUE(set_meta.key_only());
}

TEST(MetaAssociativeContainerCtor, EmptyContainersExposeValidTypeInformation) {
    std::map<int, std::string> map_like{};
    std::set<long> key_only{};

    entt::meta_associative_container map_meta{map_like};
    entt::meta_associative_container set_meta{key_only};

    const auto map_key_type = map_meta.key_type();
    const auto map_mapped_type = map_meta.mapped_type();
    const auto map_value_type = map_meta.value_type();

    EXPECT_EQ(map_key_type, entt::resolve<int>());
    EXPECT_EQ(map_mapped_type, entt::resolve<std::string>());
    EXPECT_EQ(map_value_type, entt::resolve<std::pair<const int, std::string>>());

    const auto set_key_type = set_meta.key_type();
    const auto set_mapped_type = set_meta.mapped_type();
    const auto set_value_type = set_meta.value_type();

    EXPECT_EQ(set_key_type, entt::resolve<long>());
    EXPECT_FALSE(set_mapped_type);
    EXPECT_EQ(set_value_type, entt::resolve<long>());
}

TEST(MetaAssociativeContainerCtor, UnorderedContainersExposeCorrectTypeInformation) {
    std::unordered_map<std::string, int> unordered_map_like{};
    std::unordered_set<unsigned int> unordered_set_like{};

    entt::meta_associative_container map_meta{unordered_map_like};
    entt::meta_associative_container set_meta{unordered_set_like};

    EXPECT_FALSE(map_meta.key_only());
    EXPECT_EQ(map_meta.key_type(), entt::resolve<std::string>());
    EXPECT_EQ(map_meta.mapped_type(), entt::resolve<int>());
    EXPECT_EQ(map_meta.value_type(), entt::resolve<std::pair<const std::string, int>>());

    EXPECT_TRUE(set_meta.key_only());
    EXPECT_EQ(set_meta.key_type(), entt::resolve<unsigned int>());
    EXPECT_FALSE(set_meta.mapped_type());
    EXPECT_EQ(set_meta.value_type(), entt::resolve<unsigned int>());
}

TEST(MetaAssociativeContainerCtor, TypeInformationDoesNotDependOnContainerContents) {
    std::map<int, char> map_like{{1, 'a'}, {2, 'b'}};
    std::set<int> key_only{3, 4, 5};

    entt::meta_associative_container map_meta{map_like};
    entt::meta_associative_container set_meta{key_only};

    EXPECT_EQ(map_meta.key_type(), entt::resolve<int>());
    EXPECT_EQ(map_meta.mapped_type(), entt::resolve<char>());
    EXPECT_EQ(map_meta.value_type(), entt::resolve<std::pair<const int, char>>());

    EXPECT_EQ(set_meta.key_type(), entt::resolve<int>());
    EXPECT_FALSE(set_meta.mapped_type());
    EXPECT_EQ(set_meta.value_type(), entt::resolve<int>());
}

TEST(MetaAssociativeContainerCtor, SupportsConstQualifiedContainerReferences) {
    const std::map<short, double> map_like{};
    const std::set<std::string> key_only{};

    entt::meta_associative_container map_meta{map_like};
    entt::meta_associative_container set_meta{key_only};

    EXPECT_FALSE(map_meta.key_only());
    EXPECT_EQ(map_meta.key_type(), entt::resolve<short>());
    EXPECT_EQ(map_meta.mapped_type(), entt::resolve<double>());
    EXPECT_EQ(map_meta.value_type(), entt::resolve<std::pair<const short, double>>());

    EXPECT_TRUE(set_meta.key_only());
    EXPECT_EQ(set_meta.key_type(), entt::resolve<std::string>());
    EXPECT_FALSE(set_meta.mapped_type());
    EXPECT_EQ(set_meta.value_type(), entt::resolve<std::string>());
}

TEST(MetaAssociativeContainerCtor, DistinguishesMappedAndValueTypesForMapLikeContainers) {
    std::map<int, long> map_like{};

    entt::meta_associative_container meta{map_like};

    const auto mapped = meta.mapped_type();
    const auto value = meta.value_type();

    ASSERT_TRUE(mapped);
    ASSERT_TRUE(value);

    EXPECT_EQ(mapped, entt::resolve<long>());
    EXPECT_EQ(value, entt::resolve<std::pair<const int, long>>());
    EXPECT_NE(mapped, value);
}

} // namespace