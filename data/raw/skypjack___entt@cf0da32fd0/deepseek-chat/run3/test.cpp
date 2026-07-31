#include <gtest/gtest.h>
#include <entt/entt.hpp>
#include <utility>
#include <vector>
#include <map>
#include <string>

// Test fixture for meta container iteration with dereference operations
class MetaContainerDerefTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Register types for meta system
        entt::meta<int>()
            .type("int"_hs)
            .conv<int>();

        entt::meta<std::string>()
            .type("string"_hs)
            .conv<std::string>();

        entt::meta<std::pair<const int, std::string>>()
            .type("int_string_pair"_hs)
            .data<&std::pair<const int, std::string>::first>("first"_hs)
            .data<&std::pair<const int, std::string>::second>("second"_hs);
    }
};

// Test dereference of a non-empty map iterator (KeyOnly = false)
TEST_F(MetaContainerDerefTest, DerefMapIteratorNonEmpty) {
    std::map<int, std::string> map{{1, "one"}, {2, "two"}};
    auto it = map.begin();

    entt::meta_any container_any = entt::forward_as_meta(map);
    auto view = container_any.as_ref().allow("int_string_pair"_hs);

    // Simulate iteration and dereference through meta API
    entt::meta_any it_any = entt::forward_as_meta(it);
    auto deref_result = it_any.as_ref().invoke("deref"_hs);

    ASSERT_TRUE(deref_result);
    ASSERT_TRUE(deref_result->as_ref().is_pair());

    auto pair = deref_result->as_ref().try_cast<std::pair<meta_any, meta_any>>();
    ASSERT_NE(pair, nullptr);

    // Verify first element (key)
    ASSERT_TRUE(pair->first);
    ASSERT_EQ(pair->first.as_ref().cast<int>(), 1);

    // Verify second element (value)
    ASSERT_TRUE(pair->second);
    ASSERT_EQ(pair->second.as_ref().cast<std::string>(), "one");
}

// Test dereference of a map iterator with KeyOnly = true (e.g., set iteration)
TEST_F(MetaContainerDerefTest, DerefSetIteratorKeyOnly) {
    std::set<int> set{10, 20, 30};
    auto it = set.begin();

    entt::meta_any it_any = entt::forward_as_meta(it);
    auto deref_result = it_any.as_ref().invoke("deref"_hs);

    ASSERT_TRUE(deref_result);
    ASSERT_TRUE(deref_result->as_ref().is_pair());

    auto pair = deref_result->as_ref().try_cast<std::pair<meta_any, meta_any>>();
    ASSERT_NE(pair, nullptr);

    // For KeyOnly, first should contain the key, second should be empty
    ASSERT_TRUE(pair->first);
    ASSERT_EQ(pair->first.as_ref().cast<int>(), 10);

    // Second element should be empty (default constructed)
    ASSERT_FALSE(pair->second);
}

// Test dereference of an end iterator (should fail gracefully)
TEST_F(MetaContainerDerefTest, DerefEndIterator) {
    std::map<int, std::string> empty_map;
    auto it = empty_map.end();

    entt::meta_any it_any = entt::forward_as_meta(it);
    auto deref_result = it_any.as_ref().invoke("deref"_hs);

    // Dereferencing end iterator should return invalid meta_any
    ASSERT_FALSE(deref_result);
}

// Test dereference of a single-element map
TEST_F(MetaContainerDerefTest, DerefSingleElementMap) {
    std::map<int, std::string> map{{42, "answer"}};
    auto it = map.begin();

    entt::meta_any it_any = entt::forward_as_meta(it);
    auto deref_result = it_any.as_ref().invoke("deref"_hs);

    ASSERT_TRUE(deref_result);
    auto pair = deref_result->as_ref().try_cast<std::pair<meta_any, meta_any>>();
    ASSERT_NE(pair, nullptr);

    ASSERT_TRUE(pair->first);
    ASSERT_EQ(pair->first.as_ref().cast<int>(), 42);

    ASSERT_TRUE(pair->second);
    ASSERT_EQ(pair->second.as_ref().cast<std::string>(), "answer");
}

// Test dereference of a vector iterator (KeyOnly = true behavior)
TEST_F(MetaContainerDerefTest, DerefVectorIteratorKeyOnly) {
    std::vector<double> vec{3.14, 2.718};
    auto it = vec.begin();

    entt::meta_any it_any = entt::forward_as_meta(it);
    auto deref_result = it_any.as_ref().invoke("deref"_hs);

    ASSERT_TRUE(deref_result);
    auto pair = deref_result->as_ref().try_cast<std::pair<meta_any, meta_any>>();
    ASSERT_NE(pair, nullptr);

    // For vector, KeyOnly is true, so first contains the element
    ASSERT_TRUE(pair->first);
    ASSERT_DOUBLE_EQ(pair->first.as_ref().cast<double>(), 3.14);

    // Second should be empty
    ASSERT_FALSE(pair->second);
}

// Test that dereference preserves const correctness
TEST_F(MetaContainerDerefTest, DerefConstIterator) {
    const std::map<int, std::string> const_map{{1, "one"}};
    auto it = const_map.begin();

    entt::meta_any it_any = entt::forward_as_meta(it);
    auto deref_result = it_any.as_ref().invoke("deref"_hs);

    ASSERT_TRUE(deref_result);
    auto pair = deref_result->as_ref().try_cast<std::pair<meta_any, meta_any>>();
    ASSERT_NE(pair, nullptr);

    // Values should be accessible as const references
    ASSERT_TRUE(pair->first);
    ASSERT_EQ(pair->first.as_ref().cast<int>(), 1);

    ASSERT_TRUE(pair->second);
    ASSERT_EQ(pair->second.as_ref().cast<std::string>(), "one");
}

// Test dereference of a map with multiple elements (verify correct element)
TEST_F(MetaContainerDerefTest, DerefSecondElement) {
    std::map<int, std::string> map{{1, "first"}, {2, "second"}, {3, "third"}};
    auto it = std::next(map.begin());

    entt::meta_any it_any = entt::forward_as_meta(it);
    auto deref_result = it_any.as_ref().invoke("deref"_hs);

    ASSERT_TRUE(deref_result);
    auto pair = deref_result->as_ref().try_cast<std::pair<meta_any, meta_any>>();
    ASSERT_NE(pair, nullptr);

    ASSERT_TRUE(pair->first);
    ASSERT_EQ(pair->first.as_ref().cast<int>(), 2);

    ASSERT_TRUE(pair->second);
    ASSERT_EQ(pair->second.as_ref().cast<std::string>(), "second");
}

// Test dereference of an empty set (should fail)
TEST_F(MetaContainerDerefTest, DerefEmptySet) {
    std::set<int> empty_set;
    auto it = empty_set.begin();

    entt::meta_any it_any = entt::forward_as_meta(it);
    auto deref_result = it_any.as_ref().invoke("deref"_hs);

    // begin() == end() for empty container, dereference should fail
    ASSERT_FALSE(deref_result);
}