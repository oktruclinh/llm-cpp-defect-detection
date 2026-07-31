#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <vector>

// Test fixture for named argument tests
class NamedArgTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Test that get_id returns -1 when there are no named arguments
TEST_F(NamedArgTest, NoNamedArgsReturnsMinusOne) {
    auto result = fmt::format("{}", 42);
    // The format string has no named arguments, so get_id should return -1
    // We can't directly call get_id, but we can verify behavior through format
    EXPECT_EQ(result, "42");
}

// Test basic named argument lookup
TEST_F(NamedArgTest, BasicNamedArgLookup) {
    auto result = fmt::format("{name}", fmt::arg("name", "Alice"));
    EXPECT_EQ(result, "Alice");
}

// Test multiple named arguments
TEST_F(NamedArgTest, MultipleNamedArgs) {
    auto result = fmt::format("{first} {last}", 
                              fmt::arg("first", "John"),
                              fmt::arg("last", "Doe"));
    EXPECT_EQ(result, "John Doe");
}

// Test that get_id returns correct id for named arguments
TEST_F(NamedArgTest, NamedArgIdConsistency) {
    // Use format with named args and verify correct substitution
    auto result = fmt::format("{a}{b}{c}",
                              fmt::arg("a", 1),
                              fmt::arg("b", 2),
                              fmt::arg("c", 3));
    EXPECT_EQ(result, "123");
}

// Test that get_id returns -1 for non-existent named argument
TEST_F(NamedArgTest, NonExistentNamedArg) {
    // This should throw or produce unexpected output
    EXPECT_THROW(fmt::format("{nonexistent}", fmt::arg("existing", "value")),
                 fmt::format_error);
}

// Test empty string as named argument name
TEST_F(NamedArgTest, EmptyNamedArgName) {
    auto result = fmt::format("{}", fmt::arg("", "empty_name"));
    EXPECT_EQ(result, "empty_name");
}

// Test named argument with special characters in name
TEST_F(NamedArgTest, SpecialCharsInName) {
    auto result = fmt::format("{_name_123}", fmt::arg("_name_123", "special"));
    EXPECT_EQ(result, "special");
}

// Test that get_id works correctly with packed arguments
TEST_F(NamedArgTest, PackedArgsNamedLookup) {
    // Mix positional and named args to test packed scenario
    auto result = fmt::format("{0} {name}", 10, fmt::arg("name", "test"));
    EXPECT_EQ(result, "10 test");
}

// Test that get_id returns correct id when multiple named args have same prefix
TEST_F(NamedArgTest, SimilarNamedArgNames) {
    auto result = fmt::format("{abc} {abcd}",
                              fmt::arg("abc", 1),
                              fmt::arg("abcd", 2));
    EXPECT_EQ(result, "1 2");
}

// Test that get_id handles large number of named arguments
TEST_F(NamedArgTest, ManyNamedArgs) {
    std::string expected;
    fmt::dynamic_format_arg_store<fmt::format_context> store;
    
    for (int i = 0; i < 100; ++i) {
        std::string name = "arg" + std::to_string(i);
        store.push_back(fmt::arg(name.c_str(), i));
        if (i > 0) expected += " ";
        expected += std::to_string(i);
    }
    
    std::string format_str;
    for (int i = 0; i < 100; ++i) {
        if (i > 0) format_str += " ";
        format_str += "{arg" + std::to_string(i) + "}";
    }
    
    auto result = fmt::vformat(format_str, store);
    EXPECT_EQ(result, expected);
}

// Test that get_id works with string_view arguments
TEST_F(NamedArgTest, StringViewNamedArg) {
    std::string name = "key";
    auto result = fmt::format("{key}", fmt::arg(fmt::string_view(name), "value"));
    EXPECT_EQ(result, "value");
}

// Test that get_id returns correct id for repeated named argument usage
TEST_F(NamedArgTest, RepeatedNamedArg) {
    auto result = fmt::format("{name} {name}", fmt::arg("name", "repeat"));
    EXPECT_EQ(result, "repeat repeat");
}

// Test that get_id handles named arguments with numeric names
TEST_F(NamedArgTest, NumericNamedArg) {
    auto result = fmt::format("{123}", fmt::arg("123", "numeric_name"));
    EXPECT_EQ(result, "numeric_name");
}

// Test that get_id works with mixed positional and named arguments
TEST_F(NamedArgTest, MixedPositionalAndNamed) {
    auto result = fmt::format("{0} {name} {1}", 10, 20, fmt::arg("name", "middle"));
    EXPECT_EQ(result, "10 middle 20");
}

// Test that get_id returns -1 when no named args exist in packed format
TEST_F(NamedArgTest, NoNamedArgsPacked) {
    auto result = fmt::format("{0} {1}", "a", "b");
    EXPECT_EQ(result, "a b");
}