#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <stdexcept>

// Test that named arguments work correctly with format
TEST(BasicContextGetArgTest, NamedArgumentFound) {
    std::string result = fmt::format("{name}", fmt::arg("name", "World"));
    EXPECT_EQ(result, "World");
}

// Test that multiple named arguments work correctly
TEST(BasicContextGetArgTest, MultipleNamedArguments) {
    std::string result = fmt::format("{first} {last}", 
                                     fmt::arg("first", "John"), 
                                     fmt::arg("last", "Doe"));
    EXPECT_EQ(result, "John Doe");
}

// Test that named argument with integer value works
TEST(BasicContextGetArgTest, NamedArgumentInteger) {
    std::string result = fmt::format("{age}", fmt::arg("age", 25));
    EXPECT_EQ(result, "25");
}

// Test that named argument with floating point value works
TEST(BasicContextGetArgTest, NamedArgumentFloat) {
    std::string result = fmt::format("{pi}", fmt::arg("pi", 3.14159));
    EXPECT_EQ(result, "3.14159");
}

// Test that named argument with custom format spec works
TEST(BasicContextGetArgTest, NamedArgumentWithFormatSpec) {
    std::string result = fmt::format("{value:.2f}", fmt::arg("value", 3.14159));
    EXPECT_EQ(result, "3.14");
}

// Test that named argument can be used multiple times in the same format string
TEST(BasicContextGetArgTest, NamedArgumentReused) {
    std::string result = fmt::format("{name} {name}", fmt::arg("name", "Echo"));
    EXPECT_EQ(result, "Echo Echo");
}

// Test that named argument with empty name throws an error
TEST(BasicContextGetArgTest, EmptyNameThrowsError) {
    EXPECT_THROW({
        fmt::format("{:}", fmt::arg("", "value"));
    }, fmt::format_error);
}

// Test that missing named argument throws an error
TEST(BasicContextGetArgTest, MissingNamedArgumentThrowsError) {
    EXPECT_THROW({
        fmt::format("{missing}", fmt::arg("present", "value"));
    }, fmt::format_error);
}

// Test that named argument with special characters in name works
TEST(BasicContextGetArgTest, NamedArgumentWithSpecialChars) {
    std::string result = fmt::format("{_name_123}", fmt::arg("_name_123", "special"));
    EXPECT_EQ(result, "special");
}

// Test mixing named and positional arguments (if supported)
TEST(BasicContextGetArgTest, MixedNamedAndPositional) {
    std::string result = fmt::format("{0} {name}", fmt::arg("name", "World"), "Hello");
    EXPECT_EQ(result, "Hello World");
}

// Test that named argument with boolean value works
TEST(BasicContextGetArgTest, NamedArgumentBoolean) {
    std::string result = fmt::format("{flag}", fmt::arg("flag", true));
    EXPECT_EQ(result, "true");
}

// Test that named argument with char value works
TEST(BasicContextGetArgTest, NamedArgumentChar) {
    std::string result = fmt::format("{letter}", fmt::arg("letter", 'A'));
    EXPECT_EQ(result, "A");
}

// Test that named argument with string_view works
TEST(BasicContextGetArgTest, NamedArgumentStringView) {
    std::string_view sv = "test";
    std::string result = fmt::format("{sv}", fmt::arg("sv", sv));
    EXPECT_EQ(result, "test");
}

// Test that named argument with const char* works
TEST(BasicContextGetArgTest, NamedArgumentConstCharPtr) {
    const char* str = "C-string";
    std::string result = fmt::format("{str}", fmt::arg("str", str));
    EXPECT_EQ(result, "C-string");
}

// Test that named argument with nullptr throws or handles gracefully
TEST(BasicContextGetArgTest, NamedArgumentNullptr) {
    const char* null_str = nullptr;
    std::string result = fmt::format("{str}", fmt::arg("str", null_str));
    EXPECT_EQ(result, "");
}

// Test format_to with named arguments
TEST(BasicContextGetArgTest, FormatToWithNamedArgs) {
    std::string buffer;
    fmt::format_to(std::back_inserter(buffer), "{name}", fmt::arg("name", "Output"));
    EXPECT_EQ(buffer, "Output");
}

// Test formatted_size with named arguments
TEST(BasicContextGetArgTest, FormattedSizeWithNamedArgs) {
    size_t size = fmt::formatted_size("{name}", fmt::arg("name", "Size"));
    EXPECT_EQ(size, 4);  // "Size" has 4 characters
}

// Test that named argument with very long name works
TEST(BasicContextGetArgTest, NamedArgumentLongName) {
    std::string long_name(1000, 'a');
    std::string result = fmt::format("{name}", fmt::arg(long_name, "long"));
    EXPECT_EQ(result, "long");
}

// Test that named argument with numeric name works
TEST(BasicContextGetArgTest, NamedArgumentNumericName) {
    std::string result = fmt::format("{123}", fmt::arg("123", "numeric"));
    EXPECT_EQ(result, "numeric");
}

// Test that named argument with Unicode name works
TEST(BasicContextGetArgTest, NamedArgumentUnicodeName) {
    std::string result = fmt::format("{名前}", fmt::arg("名前", "Unicode"));
    EXPECT_EQ(result, "Unicode");
}