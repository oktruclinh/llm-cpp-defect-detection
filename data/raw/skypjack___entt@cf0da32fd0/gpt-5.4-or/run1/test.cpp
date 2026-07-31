#include <gtest/gtest.h>

#include <entt/meta/meta.hpp>

#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace {

struct value_type_only {
    int value{};
};

using pair_map_type = std::unordered_map<int, std::string>;
using key_only_map_type = std::unordered_map<int, value_type_only>;

template<typename Type>
void register_meta_type() {
    if(!entt::resolve<Type>()) {
        entt::meta<Type>().type();
    }
}

class MetaAssocDerefTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        register_meta_type<pair_map_type>();
        register_meta_type<key_only_map_type>();
        register_meta_type<std::string>();
        register_meta_type<value_type_only>();
        register_meta_type<int>();
    }
};

TEST_F(MetaAssocDerefTest, DerefOnNonEmptyKeyValueContainerReturnsReferencesToCurrentElement) {
    pair_map_type container;
    container.emplace(7, "seven");

    entt::meta_any any = std::ref(container);
    ASSERT_TRUE(any);

    auto type = any.type();
    ASSERT_TRUE(type);
    ASSERT_TRUE(type.is_associative_container());

    auto assoc = any.as_associative_container();
    ASSERT_TRUE(assoc);
    ASSERT_FALSE(assoc.empty());
    ASSERT_EQ(assoc.size(), 1u);

    auto it = assoc.begin();
    auto end = assoc.end();
    ASSERT_NE(it, end);

    auto element = *it;

    ASSERT_TRUE(element.first);
    ASSERT_TRUE(element.second);
    EXPECT_EQ(element.first.cast<int>(), 7);
    EXPECT_EQ(element.second.cast<std::string>(), "seven");

    auto key_ref = element.first.try_cast<const int>();
    ASSERT_NE(key_ref, nullptr);
    EXPECT_EQ(*key_ref, 7);

    auto value_ref = element.second.try_cast<std::string>();
    ASSERT_NE(value_ref, nullptr);
    EXPECT_EQ(*value_ref, "seven");

    *value_ref = "updated";
    EXPECT_EQ(container.at(7), "updated");

    container.at(7) = "mutated-again";
    EXPECT_EQ(element.second.cast<std::string>(), "mutated-again");
}

TEST_F(MetaAssocDerefTest, DerefOnKeyOnlyContainerReturnsKeyAndNoMappedValue) {
    key_only_map_type container;
    container.emplace(42, value_type_only{3});

    entt::meta_any any = std::ref(container);
    ASSERT_TRUE(any);

    auto assoc = any.as_associative_container();
    ASSERT_TRUE(assoc);
    ASSERT_FALSE(assoc.empty());
    ASSERT_EQ(assoc.size(), 1u);

    auto it = assoc.begin();
    ASSERT_NE(it, assoc.end());

    auto element = *it;

    ASSERT_TRUE(element.first);
    EXPECT_FALSE(element.second);

    EXPECT_EQ(element.first.cast<int>(), 42);

    auto key_ref = element.first.try_cast<const int>();
    ASSERT_NE(key_ref, nullptr);
    EXPECT_EQ(*key_ref, 42);
}

TEST_F(MetaAssocDerefTest, DerefOnEndIteratorIsNotPerformedByClientAndEmptyContainerHasBeginEqualEnd) {
    pair_map_type container;

    entt::meta_any any = std::ref(container);
    ASSERT_TRUE(any);

    auto assoc = any.as_associative_container();
    ASSERT_TRUE(assoc);
    EXPECT_TRUE(assoc.empty());
    EXPECT_EQ(assoc.size(), 0u);

    auto begin = assoc.begin();
    auto end = assoc.end();
    EXPECT_EQ(begin, end);
}

TEST_F(MetaAssocDerefTest, DerefPreservesIdentityAcrossIterationForDifferentElements) {
    pair_map_type container;
    container.emplace(1, "one");
    container.emplace(2, "two");

    entt::meta_any any = std::ref(container);
    ASSERT_TRUE(any);

    auto assoc = any.as_associative_container();
    ASSERT_TRUE(assoc);
    ASSERT_EQ(assoc.size(), 2u);

    bool seen_one = false;
    bool seen_two = false;

    for(auto it = assoc.begin(); it != assoc.end(); ++it) {
        auto [key_any, value_any] = *it;

        ASSERT_TRUE(key_any);
        ASSERT_TRUE(value_any);

        const int key = key_any.cast<int>();
        auto value_ptr = value_any.try_cast<std::string>();
        ASSERT_NE(value_ptr, nullptr);

        if(key == 1) {
            seen_one = true;
            EXPECT_EQ(*value_ptr, "one");
            *value_ptr = "ONE";
        } else if(key == 2) {
            seen_two = true;
            EXPECT_EQ(*value_ptr, "two");
            *value_ptr = "TWO";
        } else {
            FAIL() << "Unexpected key encountered";
        }
    }

    EXPECT_TRUE(seen_one);
    EXPECT_TRUE(seen_two);
    EXPECT_EQ(container.at(1), "ONE");
    EXPECT_EQ(container.at(2), "TWO");
}

TEST_F(MetaAssocDerefTest, DerefFromConstContainerExposesConstKeyAndConstMappedValue) {
    const pair_map_type container{{5, "five"}};

    entt::meta_any any = std::cref(container);
    ASSERT_TRUE(any);

    auto assoc = any.as_associative_container();
    ASSERT_TRUE(assoc);
    ASSERT_FALSE(assoc.empty());

    auto it = assoc.begin();
    ASSERT_NE(it, assoc.end());

    auto element = *it;

    ASSERT_TRUE(element.first);
    ASSERT_TRUE(element.second);

    EXPECT_EQ(element.first.cast<int>(), 5);
    EXPECT_EQ(element.second.cast<std::string>(), "five");

    auto key_const_ptr = element.first.try_cast<const int>();
    ASSERT_NE(key_const_ptr, nullptr);
    EXPECT_EQ(*key_const_ptr, 5);

    auto value_const_ptr = element.second.try_cast<const std::string>();
    ASSERT_NE(value_const_ptr, nullptr);
    EXPECT_EQ(*value_const_ptr, "five");

    EXPECT_EQ(element.second.try_cast<std::string>(), nullptr);
}

TEST_F(MetaAssocDerefTest, DerefYieldsStableReferencesToUnderlyingElementNotCopies) {
    pair_map_type container;
    container.emplace(9, "nine");

    entt::meta_any any = std::ref(container);
    ASSERT_TRUE(any);

    auto assoc = any.as_associative_container();
    ASSERT_TRUE(assoc);

    auto it = assoc.begin();
    ASSERT_NE(it, assoc.end());

    auto first_read = *it;
    auto second_read = *it;

    auto first_value_ptr = first_read.second.try_cast<std::string>();
    auto second_value_ptr = second_read.second.try_cast<std::string>();

    ASSERT_NE(first_value_ptr, nullptr);
    ASSERT_NE(second_value_ptr, nullptr);

    EXPECT_EQ(first_value_ptr, second_value_ptr);

    *first_value_ptr = "changed";
    EXPECT_EQ(*second_value_ptr, "changed");
    EXPECT_EQ(container.at(9), "changed");
}

} // namespace