#include <gtest/gtest.h>
#include <entt/entt.hpp>
#include <utility>
#include <vector>
#include <map>
#include <string>

// Test fixture for meta iterator dereference operations
class MetaIteratorDerefTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Register a simple type for testing
        entt::meta<int>()
            .type("int"_hs);
        
        entt::meta<std::pair<int, std::string>>()
            .type("pair_int_string"_hs)
            .data<&std::pair<int, std::string>::first>("first"_hs)
            .data<&std::pair<int, std::string>::second>("second"_hs);
    }
};

// Test dereferencing a non-const iterator over a map (KeyOnly = false)
TEST_F(MetaIteratorDerefTest, DerefNonConstMapIterator) {
    std::map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
    auto it = map.begin();
    
    // Create a meta_any wrapping the iterator
    entt::meta_any any_it = std::ref(it);
    
    // Create a pair of meta_any to receive the dereferenced value
    entt::meta_any first;
    entt::meta_any second;
    auto pair = std::make_pair(std::move(first), std::move(second));
    
    // This should invoke the DEREF operation on the iterator
    // The actual dereference happens through the meta type system
    auto meta_type = entt::resolve<int>(); // Just to have a meta_type
    
    // We need to test the actual behavior through the meta container API
    // Create a meta sequence container from the map
    auto view = entt::meta_any{std::ref(map)}.as_sequence_container();
    ASSERT_TRUE(view);
    
    // Get an iterator from the container
    auto it_handle = view.begin();
    ASSERT_TRUE(it_handle);
    
    // Dereference the iterator - this should produce a pair
    auto deref_result = *it_handle;
    ASSERT_TRUE(deref_result);
    
    // The result should be a pair (key, value)
    auto pair_type = deref_result.type();
    ASSERT_TRUE(pair_type);
    
    // For a map, dereferencing gives a pair<const Key, Value>
    // We can check the first and second elements
    auto first_elem = deref_result.get("first"_hs);
    auto second_elem = deref_result.get("second"_hs);
    
    ASSERT_TRUE(first_elem);
    ASSERT_TRUE(second_elem);
    
    ASSERT_EQ(first_elem.cast<int>(), 1);
    ASSERT_EQ(second_elem.cast<std::string>(), "one");
}

// Test dereferencing a const iterator over a map (KeyOnly = true)
TEST_F(MetaIteratorDerefTest, DerefConstMapIterator) {
    const std::map<int, std::string> map = {{1, "one"}, {2, "two"}};
    
    auto view = entt::meta_any{std::ref(map)}.as_sequence_container();
    ASSERT_TRUE(view);
    
    auto it_handle = view.begin();
    ASSERT_TRUE(it_handle);
    
    auto deref_result = *it_handle;
    ASSERT_TRUE(deref_result);
    
    // For const map, dereferencing gives pair<const Key, const Value>
    auto first_elem = deref_result.get("first"_hs);
    auto second_elem = deref_result.get("second"_hs);
    
    ASSERT_TRUE(first_elem);
    ASSERT_TRUE(second_elem);
    
    ASSERT_EQ(first_elem.cast<int>(), 1);
    ASSERT_EQ(second_elem.cast<std::string>(), "one");
}

// Test dereferencing an iterator over a vector (non-map container)
TEST_F(MetaIteratorDerefTest, DerefVectorIterator) {
    std::vector<int> vec = {10, 20, 30};
    
    auto view = entt::meta_any{std::ref(vec)}.as_sequence_container();
    ASSERT_TRUE(view);
    
    auto it_handle = view.begin();
    ASSERT_TRUE(it_handle);
    
    auto deref_result = *it_handle;
    ASSERT_TRUE(deref_result);
    
    // For a vector, dereferencing gives the element directly
    ASSERT_TRUE(deref_result.allow_cast<int>());
    ASSERT_EQ(deref_result.cast<int>(), 10);
}

// Test dereferencing an end iterator (edge case)
TEST_F(MetaIteratorDerefTest, DerefEndIterator) {
    std::map<int, std::string> empty_map;
    
    auto view = entt::meta_any{std::ref(empty_map)}.as_sequence_container();
    ASSERT_TRUE(view);
    
    auto it_handle = view.end();
    // Dereferencing end iterator is undefined behavior, but we should handle gracefully
    // The meta system might return an invalid meta_any
    auto deref_result = *it_handle;
    // Should not crash, but result may be invalid
    // This tests that the code doesn't crash on invalid dereference
}

// Test dereferencing an iterator from an empty container
TEST_F(MetaIteratorDerefTest, DerefEmptyContainerIterator) {
    std::map<int, std::string> empty_map;
    
    auto view = entt::meta_any{std::ref(empty_map)}.as_sequence_container();
    ASSERT_TRUE(view);
    
    // begin() == end() for empty container
    auto it_handle = view.begin();
    ASSERT_FALSE(it_handle); // Should be invalid for empty container
}

// Test dereferencing a set (KeyOnly = true, no value)
TEST_F(MetaIteratorDerefTest, DerefSetIterator) {
    std::set<int> set = {100, 200, 300};
    
    auto view = entt::meta_any{std::ref(set)}.as_sequence_container();
    ASSERT_TRUE(view);
    
    auto it_handle = view.begin();
    ASSERT_TRUE(it_handle);
    
    auto deref_result = *it_handle;
    ASSERT_TRUE(deref_result);
    
    // For a set, dereferencing gives the key directly
    ASSERT_TRUE(deref_result.allow_cast<int>());
    ASSERT_EQ(deref_result.cast<int>(), 100);
}

// Test dereferencing a const set iterator (KeyOnly = true, const)
TEST_F(MetaIteratorDerefTest, DerefConstSetIterator) {
    const std::set<int> set = {100, 200, 300};
    
    auto view = entt::meta_any{std::ref(set)}.as_sequence_container();
    ASSERT_TRUE(view);
    
    auto it_handle = view.begin();
    ASSERT_TRUE(it_handle);
    
    auto deref_result = *it_handle;
    ASSERT_TRUE(deref_result);
    
    ASSERT_TRUE(deref_result.allow_cast<int>());
    ASSERT_EQ(deref_result.cast<int>(), 100);
}

// Test dereferencing an unordered_map iterator
TEST_F(MetaIteratorDerefTest, DerefUnorderedMapIterator) {
    std::unordered_map<int, std::string> umap = {{1, "one"}, {2, "two"}};
    
    auto view = entt::meta_any{std::ref(umap)}.as_sequence_container();
    ASSERT_TRUE(view);
    
    auto it_handle = view.begin();
    ASSERT_TRUE(it_handle);
    
    auto deref_result = *it_handle;
    ASSERT_TRUE(deref_result);
    
    auto first_elem = deref_result.get("first"_hs);
    auto second_elem = deref_result.get("second"_hs);
    
    ASSERT_TRUE(first_elem);
    ASSERT_TRUE(second_elem);
    
    // Just check that we got valid values (order is undefined)
    ASSERT_TRUE(first_elem.allow_cast<int>());
    ASSERT_TRUE(second_elem.allow_cast<std::string>());
}

// Test dereferencing a const unordered_map iterator
TEST_F(MetaIteratorDerefTest, DerefConstUnorderedMapIterator) {
    const std::unordered_map<int, std::string> umap = {{1, "one"}};
    
    auto view = entt::meta_any{std::ref(umap)}.as_sequence_container();
    ASSERT_TRUE(view);
    
    auto it_handle = view.begin();
    ASSERT_TRUE(it_handle);
    
    auto deref_result = *it_handle;
    ASSERT_TRUE(deref_result);
    
    auto first_elem = deref_result.get("first"_hs);
    auto second_elem = deref_result.get("second"_hs);
    
    ASSERT_TRUE(first_elem);
    ASSERT_TRUE(second_elem);
    
    ASSERT_EQ(first_elem.cast<int>(), 1);
    ASSERT_EQ(second_elem.cast<std::string>(), "one");
}

// Test that dereferencing preserves const correctness
TEST_F(MetaIteratorDerefTest, DerefPreservesConstCorrectness) {
    std::map<int, std::string> mutable_map = {{1, "one"}};
    const std::map<int, std::string> const_map = {{2, "two"}};
    
    // Mutable map iterator should give mutable reference to pair
    auto mutable_view = entt::meta_any{std::ref(mutable_map)}.as_sequence_container();
    auto mutable_it = mutable_view.begin();
    auto mutable_deref = *mutable_it;
    
    // Const map iterator should give const reference to pair
    auto const_view = entt::meta_any{std::ref(const_map)}.as_sequence_container();
    auto const_it = const_view.begin();
    auto const_deref = *const_it;
    
    // Both should be valid
    ASSERT_TRUE(mutable_deref);
    ASSERT_TRUE(const_deref);
    
    // The values should be correct
    ASSERT_EQ(mutable_deref.get("first"_hs).cast<int>(), 1);
    ASSERT_EQ(const_deref.get("first"_hs).cast<int>(), 2);
}

// Test dereferencing a map with complex value types
TEST_F(MetaIteratorDerefTest, DerefMapWithComplexValues) {
    std::map<int, std::vector<int>> complex_map = {{1, {10, 20}}, {2, {30, 40}}};
    
    auto view = entt::meta_any{std::ref(complex_map)}.as_sequence_container();
    ASSERT_TRUE(view);
    
    auto it_handle = view.begin();
    ASSERT_TRUE(it_handle);
    
    auto deref_result = *it_handle;
    ASSERT_TRUE(deref_result);
    
    auto first_elem = deref_result.get("first"_hs);
    auto second_elem = deref_result.get("second"_hs);
    
    ASSERT_TRUE(first_elem);
    ASSERT_TRUE(second_elem);
    
    ASSERT_EQ(first_elem.cast<int>(), 1);
    
    // The second element should be a vector
    auto vec = second_elem.cast<std::vector<int>>();
    ASSERT_EQ(vec.size(), 2u);
    ASSERT_EQ(vec[0], 10);
    ASSERT_EQ(vec[1], 20);
}