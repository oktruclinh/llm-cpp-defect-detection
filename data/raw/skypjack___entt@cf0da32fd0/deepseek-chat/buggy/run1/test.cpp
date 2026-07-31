#include <gtest/gtest.h>
#include <entt/entt.hpp>
#include <utility>
#include <vector>
#include <map>

// Test fixture for meta deref operations
class MetaDerefTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Register types for meta system
        entt::meta<int>().type();
        entt::meta<std::pair<int, double>>().type();
        entt::meta<std::vector<int>>().type();
        entt::meta<std::map<int, std::string>>().type();
    }
};

// Test dereferencing an iterator to a pair (KeyOnly = false)
TEST_F(MetaDerefTest, DerefIteratorToPair) {
    std::map<int, std::string> map = {{1, "one"}, {2, "two"}};
    auto it = map.begin();
    
    entt::meta_any any_it = entt::forward_as_meta(it);
    entt::meta_any result = any_it.invoke(entt::meta_func::deref);
    
    ASSERT_TRUE(result);
    ASSERT_TRUE(result.type() == entt::resolve<std::pair<int, std::string>>());
    
    auto& pair = result.cast<std::pair<int, std::string>&>();
    EXPECT_EQ(pair.first, 1);
    EXPECT_EQ(pair.second, "one");
}

// Test dereferencing an iterator to a single value (KeyOnly = true)
TEST_F(MetaDerefTest, DerefIteratorToValue) {
    std::vector<int> vec = {10, 20, 30};
    auto it = vec.begin();
    
    entt::meta_any any_it = entt::forward_as_meta(it);
    entt::meta_any result = any_it.invoke(entt::meta_func::deref);
    
    ASSERT_TRUE(result);
    ASSERT_TRUE(result.type() == entt::resolve<int>());
    EXPECT_EQ(result.cast<int>(), 10);
}

// Test dereferencing end iterator (should fail gracefully)
TEST_F(MetaDerefTest, DerefEndIterator) {
    std::vector<int> vec;
    auto it = vec.end();
    
    entt::meta_any any_it = entt::forward_as_meta(it);
    entt::meta_any result = any_it.invoke(entt::meta_func::deref);
    
    // Dereferencing end iterator is undefined behavior, but we expect the system to handle it
    // The result may be invalid or throw
    EXPECT_FALSE(result); // Should return empty meta_any for invalid deref
}

// Test dereferencing a const iterator
TEST_F(MetaDerefTest, DerefConstIterator) {
    std::map<int, double> const_map = {{5, 3.14}};
    auto it = const_map.cbegin();
    
    entt::meta_any any_it = entt::forward_as_meta(it);
    entt::meta_any result = any_it.invoke(entt::meta_func::deref);
    
    ASSERT_TRUE(result);
    auto& pair = result.cast<const std::pair<int, double>&>();
    EXPECT_EQ(pair.first, 5);
    EXPECT_DOUBLE_EQ(pair.second, 3.14);
}

// Test dereferencing a reverse iterator
TEST_F(MetaDerefTest, DerefReverseIterator) {
    std::vector<int> vec = {100, 200, 300};
    auto it = vec.rbegin();
    
    entt::meta_any any_it = entt::forward_as_meta(it);
    entt::meta_any result = any_it.invoke(entt::meta_func::deref);
    
    ASSERT_TRUE(result);
    EXPECT_EQ(result.cast<int>(), 300);
}

// Test dereferencing a map iterator with string values
TEST_F(MetaDerefTest, DerefMapIteratorStringValue) {
    std::map<int, std::string> map = {{42, "answer"}};
    auto it = map.begin();
    
    entt::meta_any any_it = entt::forward_as_meta(it);
    entt::meta_any result = any_it.invoke(entt::meta_func::deref);
    
    ASSERT_TRUE(result);
    auto& pair = result.cast<std::pair<int, std::string>&>();
    EXPECT_EQ(pair.first, 42);
    EXPECT_EQ(pair.second, "answer");
}

// Test dereferencing a non-iterator type (should fail)
TEST_F(MetaDerefTest, DerefNonIterator) {
    int value = 42;
    entt::meta_any any_value = entt::forward_as_meta(value);
    
    // This should not crash but return invalid result
    entt::meta_any result = any_value.invoke(entt::meta_func::deref);
    EXPECT_FALSE(result);
}

// Test dereferencing an empty container iterator
TEST_F(MetaDerefTest, DerefEmptyContainerIterator) {
    std::vector<int> empty_vec;
    auto it = empty_vec.begin();
    
    entt::meta_any any_it = entt::forward_as_meta(it);
    entt::meta_any result = any_it.invoke(entt::meta_func::deref);
    
    // Dereferencing begin of empty container is UB, but system should handle
    EXPECT_FALSE(result);
}

// Test dereferencing a map iterator and modifying through reference
TEST_F(MetaDerefTest, DerefMapIteratorModify) {
    std::map<int, double> map = {{1, 1.5}};
    auto it = map.begin();
    
    entt::meta_any any_it = entt::forward_as_meta(it);
    entt::meta_any result = any_it.invoke(entt::meta_func::deref);
    
    ASSERT_TRUE(result);
    auto& pair = result.cast<std::pair<int, double>&>();
    pair.second = 2.5;
    
    // Verify the original map was modified
    EXPECT_DOUBLE_EQ(map[1], 2.5);
}

// Test dereferencing a vector iterator and modifying through reference
TEST_F(MetaDerefTest, DerefVectorIteratorModify) {
    std::vector<int> vec = {1, 2, 3};
    auto it = vec.begin() + 1;
    
    entt::meta_any any_it = entt::forward_as_meta(it);
    entt::meta_any result = any_it.invoke(entt::meta_func::deref);
    
    ASSERT_TRUE(result);
    int& value = result.cast<int&>();
    value = 99;
    
    EXPECT_EQ(vec[1], 99);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[2], 3);
}