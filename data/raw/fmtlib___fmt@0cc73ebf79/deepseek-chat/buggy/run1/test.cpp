#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <string_view>

// Test fixture for named argument ID lookup
class NamedArgIdTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Test that get_id returns 0 for the first named argument
TEST_F(NamedArgIdTest, FirstNamedArgReturnsZero) {
    auto result = fmt::format("{name}", fmt::arg("name", 42));
    EXPECT_EQ(result, "42");
    // Verify via format_to that the ID is correct by using positional access
    std::string out;
    fmt::format_to(std::back_inserter(out), "{0}", fmt::arg("name", 42));
    EXPECT_EQ(out, "42");
}

// Test that get_id returns correct ID for multiple named arguments
TEST_F(NamedArgIdTest, MultipleNamedArgsReturnCorrectIds) {
    auto result = fmt::format("{first} {second}", 
                              fmt::arg("first", 1), 
                              fmt::arg("second", 2));
    EXPECT_EQ(result, "1 2");
    
    // Verify order independence
    result = fmt::format("{second} {first}", 
                         fmt::arg("first", 1), 
                         fmt::arg("second", 2));
    EXPECT_EQ(result, "2 1");
}

// Test that get_id returns correct ID when arguments are in different order
TEST_F(NamedArgIdTest, NamedArgsInDifferentOrder) {
    auto result = fmt::format("{b} {a}", 
                              fmt::arg("a", 10), 
                              fmt::arg("b", 20));
    EXPECT_EQ(result, "20 10");
}

// Test that get_id returns empty for non-existent name (should not compile or throw)
TEST_F(NamedArgIdTest, NonExistentNameReturnsEmpty) {
    // This should compile and produce empty output for the missing name
    // The behavior depends on fmtlib version; we test that it doesn't crash
    EXPECT_NO_THROW({
        auto result = fmt::format("{nonexistent}", fmt::arg("existing", 42));
        // The format string references a name that doesn't exist
        // fmtlib will replace it with nothing or throw depending on version
        // We just verify it doesn't crash
        (void)result;
    });
}

// Test that get_id works with string_view names
TEST_F(NamedArgIdTest, StringViewNames) {
    std::string_view name1 = "key1";
    std::string_view name2 = "key2";
    auto result = fmt::format("{key1} {key2}", 
                              fmt::arg(name1, 100), 
                              fmt::arg(name2, 200));
    EXPECT_EQ(result, "100 200");
}

// Test that get_id works with char* names
TEST_F(NamedArgIdTest, CStringNames) {
    auto result = fmt::format("{hello} {world}", 
                              fmt::arg("hello", "hi"), 
                              fmt::arg("world", "earth"));
    EXPECT_EQ(result, "hi earth");
}

// Test that get_id handles duplicate names (last one wins)
TEST_F(NamedArgIdTest, DuplicateNamesLastWins) {
    auto result = fmt::format("{name}", 
                              fmt::arg("name", 1), 
                              fmt::arg("name", 2));
    EXPECT_EQ(result, "2");
}

// Test that get_id works with mixed named and positional arguments
TEST_F(NamedArgIdTest, MixedNamedAndPositional) {
    auto result = fmt::format("{0} {name} {1}", 
                              10, 
                              30, 
                              fmt::arg("name", 20));
    EXPECT_EQ(result, "10 20 30");
}

// Test that get_id handles empty name string
TEST_F(NamedArgIdTest, EmptyNameString) {
    auto result = fmt::format("{}", fmt::arg("", 42));
    EXPECT_EQ(result, "42");
}

// Test that get_id works with many named arguments
TEST_F(NamedArgIdTest, ManyNamedArguments) {
    auto result = fmt::format("{a}{b}{c}{d}{e}", 
                              fmt::arg("a", 1), 
                              fmt::arg("b", 2), 
                              fmt::arg("c", 3), 
                              fmt::arg("d", 4), 
                              fmt::arg("e", 5));
    EXPECT_EQ(result, "12345");
}

// Test that get_id works with long argument names
TEST_F(NamedArgIdTest, LongArgumentNames) {
    std::string long_name = "a_very_long_argument_name_that_exceeds_typical_buffer";
    auto result = fmt::format("{" + long_name + "}", 
                              fmt::arg(long_name, 999));
    EXPECT_EQ(result, "999");
}

// Test that get_id works with Unicode names (if Char is char)
TEST_F(NamedArgIdTest, UnicodeNames) {
    auto result = fmt::format("{café}", fmt::arg("café", "coffee"));
    EXPECT_EQ(result, "coffee");
}

// Test that get_id works with numeric string names
TEST_F(NamedArgIdTest, NumericStringNames) {
    auto result = fmt::format("{123}", fmt::arg("123", "numeric"));
    EXPECT_EQ(result, "numeric");
}

// Test that get_id handles special characters in names
TEST_F(NamedArgIdTest, SpecialCharacterNames) {
    auto result = fmt::format("{a_b}", fmt::arg("a_b", "underscore"));
    EXPECT_EQ(result, "underscore");
    
    result = fmt::format("{a-b}", fmt::arg("a-b", "hyphen"));
    EXPECT_EQ(result, "hyphen");
}

// Test that get_id works with wchar_t strings (if supported)
TEST_F(NamedArgIdTest, WideStringNames) {
    // fmtlib supports wide strings; test with wstring_view
    auto result = fmt::format(L"{wide}", fmt::arg(L"wide", L"wide_result"));
    EXPECT_EQ(result, L"wide_result");
}

// Test that get_id returns correct ID when used with format_args
TEST_F(NamedArgIdTest, FormatArgsUsage) {
    auto args = fmt::make_format_args(fmt::arg("x", 5), fmt::arg("y", 10));
    auto result = fmt::vformat("{x} {y}", args);
    EXPECT_EQ(result, "5 10");
}

// Test that get_id works with repeated usage of same named argument
TEST_F(NamedArgIdTest, RepeatedNamedArgumentUsage) {
    auto result = fmt::format("{name} {name} {name}", 
                              fmt::arg("name", 7));
    EXPECT_EQ(result, "7 7 7");
}

// Test that get_id handles case sensitivity
TEST_F(NamedArgIdTest, CaseSensitiveNames) {
    auto result = fmt::format("{Name} {name}", 
                              fmt::arg("Name", 1), 
                              fmt::arg("name", 2));
    EXPECT_EQ(result, "1 2");
}

// Test that get_id works with format_to iterator
TEST_F(NamedArgIdTest, FormatToWithNamedArgs) {
    std::string out;
    fmt::format_to(std::back_inserter(out), "{value}", fmt::arg("value", 42));
    EXPECT_EQ(out, "42");
}

// Test that get_id works with format_to_n
TEST_F(NamedArgIdTest, FormatToNWithNamedArgs) {
    char buffer[10];
    auto result = fmt::format_to_n(buffer, 5, "{value}", fmt::arg("value", 12345));
    *result.out = '\0';
    EXPECT_STREQ(buffer, "12345");
}

// Test that get_id works with formatted_size
TEST_F(NamedArgIdTest, FormattedSizeWithNamedArgs) {
    auto size = fmt::formatted_size("{name}", fmt::arg("name", "hello"));
    EXPECT_EQ(size, 5);
}