#include <gtest/gtest.h>
#include <entt/entt.hpp>
#include <utility>
#include <vector>
#include <map>
#include <string>

struct MetaDerefTest : ::testing::Test {
    void SetUp() override {
        entt::meta<double>()
            .type("double"_hs)
            .data<&get_double, &set_double>("value"_hs);

        entt::meta<std::pair<int, double>>()
            .type("pair_id"_hs)
            .data<&get_first, &set_first>("first"_hs)
            .data<&get_second, &set_second>("second"_hs);
    }

    static double get_double(const double &d) { return d; }
    static void set_double(double &d, double val) { d = val; }

    static int get_first(const std::pair<int, double> &p) { return p.first; }
    static void set_first(std::pair<int, double> &p, int val) { p.first = val; }
    static double get_second(const std::pair<int, double> &p) { return p.second; }
    static void set_second(std::pair<int, double> &p, double val) { p.second = val; }
};

TEST_F(MetaDerefTest, DerefKeyOnlyFromVector) {
    std::vector<std::pair<int, double>> vec = {{1, 1.5}, {2, 2.5}};
    auto view = entt::meta_any(std::ref(vec));
    auto it_any = view.invoke("begin"_hs);
    ASSERT_TRUE(it_any);

    // Dereference the iterator (key-only path via pair<meta_any,meta_any>)
    auto deref_result = it_any.invoke("deref"_hs);
    ASSERT_TRUE(deref_result);

    // The result should be a pair<meta_any, meta_any> with first = key, second = value
    auto *pair_ptr = entt::any_cast<std::pair<entt::meta_any, entt::meta_any>>(&deref_result);
    ASSERT_NE(pair_ptr, nullptr);

    // First element should be the key (int)
    auto first_int = entt::any_cast<int>(&pair_ptr->first);
    ASSERT_NE(first_int, nullptr);
    EXPECT_EQ(*first_int, 1);

    // Second element should be the value (double)
    auto second_double = entt::any_cast<double>(&pair_ptr->second);
    ASSERT_NE(second_double, nullptr);
    EXPECT_DOUBLE_EQ(*second_double, 1.5);
}

TEST_F(MetaDerefTest, DerefKeyValueFromMap) {
    std::map<std::string, double> m = {{"alpha", 10.0}, {"beta", 20.0}};
    auto view = entt::meta_any(std::ref(m));
    auto it_any = view.invoke("begin"_hs);
    ASSERT_TRUE(it_any);

    auto deref_result = it_any.invoke("deref"_hs);
    ASSERT_TRUE(deref_result);

    // For map, deref returns a pair<meta_any, meta_any> with first=key, second=value
    auto *pair_ptr = entt::any_cast<std::pair<entt::meta_any, entt::meta_any>>(&deref_result);
    ASSERT_NE(pair_ptr, nullptr);

    // Key should be std::string
    auto key_str = entt::any_cast<std::string>(&pair_ptr->first);
    ASSERT_NE(key_str, nullptr);
    EXPECT_EQ(*key_str, "alpha");

    // Value should be double
    auto val_double = entt::any_cast<double>(&pair_ptr->second);
    ASSERT_NE(val_double, nullptr);
    EXPECT_DOUBLE_EQ(*val_double, 10.0);
}

TEST_F(MetaDerefTest, DerefKeyOnlyFromSet) {
    std::set<int> s = {42, 99};
    auto view = entt::meta_any(std::ref(s));
    auto it_any = view.invoke("begin"_hs);
    ASSERT_TRUE(it_any);

    auto deref_result = it_any.invoke("deref"_hs);
    ASSERT_TRUE(deref_result);

    // For set (key-only), deref returns pair<meta_any, meta_any> with first=element, second=empty
    auto *pair_ptr = entt::any_cast<std::pair<entt::meta_any, entt::meta_any>>(&deref_result);
    ASSERT_NE(pair_ptr, nullptr);

    auto first_int = entt::any_cast<int>(&pair_ptr->first);
    ASSERT_NE(first_int, nullptr);
    EXPECT_EQ(*first_int, 42);

    // Second should be empty (default constructed meta_any)
    EXPECT_FALSE(pair_ptr->second);
}

TEST_F(MetaDerefTest, DerefEmptyContainer) {
    std::vector<std::pair<int, double>> empty_vec;
    auto view = entt::meta_any(std::ref(empty_vec));
    auto it_any = view.invoke("begin"_hs);
    ASSERT_TRUE(it_any);

    auto end_any = view.invoke("end"_hs);
    ASSERT_TRUE(end_any);

    // Dereferencing end iterator is undefined behavior, but we can check that begin == end
    EXPECT_TRUE(it_any.invoke("eq"_hs, end_any).cast<bool>());
}

TEST_F(MetaDerefTest, DerefSingleElementContainer) {
    std::vector<std::pair<int, double>> single = {{7, 3.14}};
    auto view = entt::meta_any(std::ref(single));
    auto it_any = view.invoke("begin"_hs);
    ASSERT_TRUE(it_any);

    auto deref_result = it_any.invoke("deref"_hs);
    ASSERT_TRUE(deref_result);

    auto *pair_ptr = entt::any_cast<std::pair<entt::meta_any, entt::meta_any>>(&deref_result);
    ASSERT_NE(pair_ptr, nullptr);

    EXPECT_EQ(pair_ptr->first.cast<int>(), 7);
    EXPECT_DOUBLE_EQ(pair_ptr->second.cast<double>(), 3.14);
}

TEST_F(MetaDerefTest, DerefAfterAdvance) {
    std::vector<std::pair<int, double>> vec = {{1, 1.1}, {2, 2.2}, {3, 3.3}};
    auto view = entt::meta_any(std::ref(vec));
    auto it_any = view.invoke("begin"_hs);
    ASSERT_TRUE(it_any);

    // Advance to second element
    it_any.invoke("inc"_hs);
    auto deref_result = it_any.invoke("deref"_hs);
    ASSERT_TRUE(deref_result);

    auto *pair_ptr = entt::any_cast<std::pair<entt::meta_any, entt::meta_any>>(&deref_result);
    ASSERT_NE(pair_ptr, nullptr);

    EXPECT_EQ(pair_ptr->first.cast<int>(), 2);
    EXPECT_DOUBLE_EQ(pair_ptr->second.cast<double>(), 2.2);
}

TEST_F(MetaDerefTest, DerefModifyThroughReference) {
    std::vector<std::pair<int, double>> vec = {{10, 100.0}};
    auto view = entt::meta_any(std::ref(vec));
    auto it_any = view.invoke("begin"_hs);
    ASSERT_TRUE(it_any);

    auto deref_result = it_any.invoke("deref"_hs);
    ASSERT_TRUE(deref_result);

    auto *pair_ptr = entt::any_cast<std::pair<entt::meta_any, entt::meta_any>>(&deref_result);
    ASSERT_NE(pair_ptr, nullptr);

    // Modify the second element through the reference wrapper
    auto &second_ref = pair_ptr->second;
    second_ref = entt::meta_any{200.0};

    // Check that the original container was modified
    EXPECT_DOUBLE_EQ(vec[0].second, 200.0);
}

TEST_F(MetaDerefTest, DerefConstContainer) {
    const std::vector<std::pair<int, double>> const_vec = {{5, 5.5}};
    auto view = entt::meta_any(std::ref(const_vec));
    auto it_any = view.invoke("begin"_hs);
    ASSERT_TRUE(it_any);

    auto deref_result = it_any.invoke("deref"_hs);
    ASSERT_TRUE(deref_result);

    auto *pair_ptr = entt::any_cast<std::pair<entt::meta_any, entt::meta_any>>(&deref_result);
    ASSERT_NE(pair_ptr, nullptr);

    EXPECT_EQ(pair_ptr->first.cast<int>(), 5);
    EXPECT_DOUBLE_EQ(pair_ptr->second.cast<double>(), 5.5);
}