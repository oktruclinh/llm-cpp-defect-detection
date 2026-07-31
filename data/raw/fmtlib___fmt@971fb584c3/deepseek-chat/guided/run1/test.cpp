#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/compile.h>
#include <string>
#include <string_view>
#include <stdexcept>

// Test fixture for named argument tests
class NamedArgumentTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Ensure consistent locale behavior
        fmt::memory_buffer buf;
        fmt::format_to(std::back_inserter(buf), "{}", 42);
    }
};

// Test basic named argument retrieval
TEST_F(NamedArgumentTest, BasicNamedArgument) {
    std::string result = fmt::format("{name}", fmt::arg("name", "World"));
    EXPECT_EQ(result, "World");
}

// Test multiple named arguments
TEST_F(NamedArgumentTest, MultipleNamedArguments) {
    std::string result = fmt::format("{first} {last}",
                                     fmt::arg("first", "John"),
                                     fmt::arg("last", "Doe"));
    EXPECT_EQ(result, "John Doe");
}

// Test named argument with positional argument
TEST_F(NamedArgumentTest, MixedNamedAndPositional) {
    std::string result = fmt::format("{0} {name}", "Hello", fmt::arg("name", "World"));
    EXPECT_EQ(result, "Hello World");
}

// Test named argument with different types
TEST_F(NamedArgumentTest, NamedArgumentDifferentTypes) {
    std::string result = fmt::format("{name} is {age} years old",
                                     fmt::arg("name", "Alice"),
                                     fmt::arg("age", 30));
    EXPECT_EQ(result, "Alice is 30 years old");
}

// Test named argument with floating point
TEST_F(NamedArgumentTest, NamedArgumentFloat) {
    std::string result = fmt::format("{value:.2f}", fmt::arg("value", 3.14159));
    EXPECT_EQ(result, "3.14");
}

// Test named argument with custom formatting
TEST_F(NamedArgumentTest, NamedArgumentCustomFormat) {
    std::string result = fmt::format("{num:04d}", fmt::arg("num", 42));
    EXPECT_EQ(result, "0042");
}

// Test named argument with empty name (should work as it's just a string)
TEST_F(NamedArgumentTest, EmptyNamedArgument) {
    std::string result = fmt::format("{}", fmt::arg("", "empty"));
    EXPECT_EQ(result, "empty");
}

// Test named argument with special characters in name
TEST_F(NamedArgumentTest, SpecialCharacterName) {
    std::string result = fmt::format("{_name}", fmt::arg("_name", "underscore"));
    EXPECT_EQ(result, "underscore");
}

// Test named argument with long name
TEST_F(NamedArgumentTest, LongName) {
    std::string very_long_name = std::string(100, 'a');
    std::string result = fmt::format("{" + very_long_name + "}",
                                     fmt::arg(very_long_name, "long"));
    EXPECT_EQ(result, "long");
}

// Test that missing named argument throws an exception
TEST_F(NamedArgumentTest, MissingNamedArgumentThrows) {
    EXPECT_THROW({
        fmt::format("{nonexistent}", fmt::arg("existing", "value"));
    }, fmt::format_error);
}

// Test that missing named argument in format string with multiple args throws
TEST_F(NamedArgumentTest, MissingNamedArgumentMultipleThrows) {
    EXPECT_THROW({
        fmt::format("{a} {b}", fmt::arg("a", 1));
    }, fmt::format_error);
}

// Test named argument with same name used multiple times
TEST_F(NamedArgumentTest, RepeatedNamedArgument) {
    std::string result = fmt::format("{name} {name}",
                                     fmt::arg("name", "repeat"));
    EXPECT_EQ(result, "repeat repeat");
}

// Test named argument with string_view
TEST_F(NamedArgumentTest, StringViewNamedArgument) {
    std::string_view sv = "string_view";
    std::string result = fmt::format("{sv}", fmt::arg("sv", sv));
    EXPECT_EQ(result, "string_view");
}

// Test named argument with const char*
TEST_F(NamedArgumentTest, ConstCharNamedArgument) {
    const char* cstr = "C string";
    std::string result = fmt::format("{cstr}", fmt::arg("cstr", cstr));
    EXPECT_EQ(result, "C string");
}

// Test named argument with integer types
TEST_F(NamedArgumentTest, IntegerNamedArguments) {
    std::string result = fmt::format("{i} {j} {k}",
                                     fmt::arg("i", 1),
                                     fmt::arg("j", 2u),
                                     fmt::arg("k", 3l));
    EXPECT_EQ(result, "1 2 3");
}

// Test named argument with boolean
TEST_F(NamedArgumentTest, BooleanNamedArgument) {
    std::string result = fmt::format("{flag}", fmt::arg("flag", true));
    EXPECT_EQ(result, "true");
}

// Test named argument with char
TEST_F(NamedArgumentTest, CharNamedArgument) {
    std::string result = fmt::format("{ch}", fmt::arg("ch", 'A'));
    EXPECT_EQ(result, "A");
}

// Test named argument with pointer
TEST_F(NamedArgumentTest, PointerNamedArgument) {
    int value = 42;
    std::string result = fmt::format("{ptr}", fmt::arg("ptr", &value));
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0x0");
}

// Test named argument with null pointer
TEST_F(NamedArgumentTest, NullPointerNamedArgument) {
    std::string result = fmt::format("{ptr}", fmt::arg("ptr", nullptr));
    EXPECT_EQ(result, "nullptr");
}

// Test named argument with wide string (if supported)
TEST_F(NamedArgumentTest, WideStringNamedArgument) {
    std::wstring result = fmt::format(L"{name}", fmt::arg(L"name", L"wide"));
    EXPECT_EQ(result, L"wide");
}

// Test that named arguments work with fmt::print (no output check, just no crash)
TEST_F(NamedArgumentTest, NamedArgumentWithPrint) {
    EXPECT_NO_THROW({
        fmt::print("{name}", fmt::arg("name", "test"));
    });
}

// Test named argument with compile-time format string
TEST_F(NamedArgumentTest, CompileTimeNamedArgument) {
    std::string result = fmt::format(FMT_STRING("{name}"),
                                     fmt::arg("name", "compile"));
    EXPECT_EQ(result, "compile");
}

// Test named argument with dynamic format string
TEST_F(NamedArgumentTest, DynamicFormatString) {
    std::string fmt_str = "{name}";
    std::string result = fmt::format(fmt::runtime(fmt_str),
                                     fmt::arg("name", "dynamic"));
    EXPECT_EQ(result, "dynamic");
}

// Test that named arguments are properly scoped (not shared between calls)
TEST_F(NamedArgumentTest, NamedArgumentScoping) {
    auto arg = fmt::arg("name", "first");
    std::string result1 = fmt::format("{name}", arg);
    EXPECT_EQ(result1, "first");
    
    // Reuse the same arg object with different value
    std::string result2 = fmt::format("{name}", fmt::arg("name", "second"));
    EXPECT_EQ(result2, "second");
}

// Test named argument with very large number of arguments
TEST_F(NamedArgumentTest, ManyNamedArguments) {
    fmt::dynamic_format_arg_store<fmt::format_context> store;
    std::string expected;
    for (int i = 0; i < 100; ++i) {
        std::string key = "key" + std::to_string(i);
        store.push_back(fmt::arg(key.c_str(), i));
        if (i > 0) expected += " ";
        expected += std::to_string(i);
    }
    
    std::string format_str;
    for (int i = 0; i < 100; ++i) {
        if (i > 0) format_str += " ";
        format_str += "{" + std::string("key") + std::to_string(i) + "}";
    }
    
    std::string result = fmt::vformat(format_str, store);
    EXPECT_EQ(result, expected);
}