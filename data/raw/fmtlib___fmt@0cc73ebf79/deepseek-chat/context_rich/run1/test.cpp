#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <vector>
#include <cstddef>

// Test basic_format_args default construction
TEST(BasicFormatArgsTest, DefaultConstructor) {
    fmt::basic_format_args<fmt::format_context> args;
    EXPECT_EQ(args.max_size(), 0);
    fmt::format_arg arg = args.get(0);
    EXPECT_FALSE(static_cast<bool>(arg));
}

// Test construction from format_arg_store (variadic arguments)
TEST(BasicFormatArgsTest, FromFormatArgStore) {
    auto store = fmt::make_format_args(42, 3.14, std::string("hello"));
    fmt::basic_format_args<fmt::format_context> args(store);
    EXPECT_GT(args.max_size(), 0);
    
    fmt::format_arg arg0 = args.get(0);
    EXPECT_TRUE(static_cast<bool>(arg0));
    EXPECT_EQ(arg0.get<int>(), 42);
    
    fmt::format_arg arg1 = args.get(1);
    EXPECT_TRUE(static_cast<bool>(arg1));
    EXPECT_FLOAT_EQ(arg1.get<double>(), 3.14);
    
    fmt::format_arg arg2 = args.get(2);
    EXPECT_TRUE(static_cast<bool>(arg2));
    EXPECT_EQ(arg2.get<std::string>(), "hello");
}

// Test construction from dynamic_format_arg_store
TEST(BasicFormatArgsTest, FromDynamicFormatArgStore) {
    fmt::dynamic_format_arg_store<fmt::format_context> store;
    store.push_back(100);
    store.push_back(2.718);
    store.push_back(std::string("world"));
    
    fmt::basic_format_args<fmt::format_context> args(store);
    EXPECT_GT(args.max_size(), 0);
    
    fmt::format_arg arg0 = args.get(0);
    EXPECT_TRUE(static_cast<bool>(arg0));
    EXPECT_EQ(arg0.get<int>(), 100);
    
    fmt::format_arg arg1 = args.get(1);
    EXPECT_TRUE(static_cast<bool>(arg1));
    EXPECT_DOUBLE_EQ(arg1.get<double>(), 2.718);
    
    fmt::format_arg arg2 = args.get(2);
    EXPECT_TRUE(static_cast<bool>(arg2));
    EXPECT_EQ(arg2.get<std::string>(), "world");
}

// Test construction from format_arg array (unpacked)
TEST(BasicFormatArgsTest, FromFormatArgArray) {
    fmt::format_arg args_array[3];
    args_array[0] = fmt::format_arg(fmt::detail::make_value<fmt::format_context>(10));
    args_array[1] = fmt::format_arg(fmt::detail::make_value<fmt::format_context>(20.5f));
    args_array[2] = fmt::format_arg(fmt::detail::make_value<fmt::format_context>(std::string("test")));
    
    fmt::basic_format_args<fmt::format_context> args(args_array, 3);
    EXPECT_EQ(args.max_size(), 3);
    
    fmt::format_arg arg0 = args.get(0);
    EXPECT_TRUE(static_cast<bool>(arg0));
    EXPECT_EQ(arg0.get<int>(), 10);
    
    fmt::format_arg arg1 = args.get(1);
    EXPECT_TRUE(static_cast<bool>(arg1));
    EXPECT_FLOAT_EQ(arg1.get<float>(), 20.5f);
    
    fmt::format_arg arg2 = args.get(2);
    EXPECT_TRUE(static_cast<bool>(arg2));
    EXPECT_EQ(arg2.get<std::string>(), "test");
}

// Test get with out-of-range index
TEST(BasicFormatArgsTest, GetOutOfRange) {
    auto store = fmt::make_format_args(1, 2, 3);
    fmt::basic_format_args<fmt::format_context> args(store);
    
    fmt::format_arg arg = args.get(100);
    EXPECT_FALSE(static_cast<bool>(arg));
}

// Test get with negative index
TEST(BasicFormatArgsTest, GetNegativeIndex) {
    auto store = fmt::make_format_args(42);
    fmt::basic_format_args<fmt::format_context> args(store);
    
    fmt::format_arg arg = args.get(-1);
    EXPECT_FALSE(static_cast<bool>(arg));
}

// Test get_id with named arguments (if supported)
TEST(BasicFormatArgsTest, GetIdWithNamedArgs) {
    // Named arguments are typically used with format strings, not directly with basic_format_args
    // This test verifies that get_id returns -1 when there are no named args
    auto store = fmt::make_format_args(1, 2);
    fmt::basic_format_args<fmt::format_context> args(store);
    
    int id = args.get_id(fmt::string_view("nonexistent"));
    EXPECT_EQ(id, -1);
}

// Test max_size for packed arguments
TEST(BasicFormatArgsTest, MaxSizePacked) {
    auto store = fmt::make_format_args(1, 2, 3, 4, 5);
    fmt::basic_format_args<fmt::format_context> args(store);
    EXPECT_EQ(args.max_size(), fmt::detail::max_packed_args);
}

// Test max_size for unpacked arguments
TEST(BasicFormatArgsTest, MaxSizeUnpacked) {
    fmt::format_arg args_array[10];
    for (int i = 0; i < 10; ++i) {
        args_array[i] = fmt::format_arg(fmt::detail::make_value<fmt::format_context>(i));
    }
    fmt::basic_format_args<fmt::format_context> args(args_array, 10);
    EXPECT_EQ(args.max_size(), 10);
}

// Test format_args alias (for format_context)
TEST(FormatArgsTest, AliasConstruction) {
    fmt::format_args args(42, 3.14, std::string("test"));
    EXPECT_GT(args.max_size(), 0);
    
    fmt::format_arg arg0 = args.get(0);
    EXPECT_TRUE(static_cast<bool>(arg0));
    EXPECT_EQ(arg0.get<int>(), 42);
}

// Test wformat_args alias (for wformat_context)
TEST(WFormatArgsTest, AliasConstruction) {
    fmt::wformat_args args(42, 3.14, std::wstring(L"test"));
    EXPECT_GT(args.max_size(), 0);
    
    fmt::basic_format_arg<fmt::wformat_context> arg0 = args.get(0);
    EXPECT_TRUE(static_cast<bool>(arg0));
    EXPECT_EQ(arg0.get<int>(), 42);
}

// Test get with different argument types
TEST(BasicFormatArgsTest, GetVariousTypes) {
    auto store = fmt::make_format_args(
        static_cast<char>('A'),
        static_cast<unsigned long long>(1234567890123ULL),
        static_cast<float>(1.5f),
        static_cast<const char*>("c-string"),
        std::string("std-string")
    );
    fmt::basic_format_args<fmt::format_context> args(store);
    
    EXPECT_EQ(args.get(0).get<char>(), 'A');
    EXPECT_EQ(args.get(1).get<unsigned long long>(), 1234567890123ULL);
    EXPECT_FLOAT_EQ(args.get(2).get<float>(), 1.5f);
    EXPECT_STREQ(args.get(3).get<const char*>(), "c-string");
    EXPECT_EQ(args.get(4).get<std::string>(), "std-string");
}

// Test that get returns empty arg for missing index in packed args
TEST(BasicFormatArgsTest, GetMissingPackedArg) {
    auto store = fmt::make_format_args(1, 2);
    fmt::basic_format_args<fmt::format_context> args(store);
    
    // Index 2 should be beyond the packed args (only 2 args stored)
    fmt::format_arg arg = args.get(2);
    EXPECT_FALSE(static_cast<bool>(arg));
}

// Test copy construction and assignment (if applicable)
TEST(BasicFormatArgsTest, CopySemantics) {
    auto store = fmt::make_format_args(42, std::string("hello"));
    fmt::basic_format_args<fmt::format_context> args1(store);
    
    // Copy construct
    fmt::basic_format_args<fmt::format_context> args2(args1);
    EXPECT_EQ(args2.get(0).get<int>(), 42);
    EXPECT_EQ(args2.get(1).get<std::string>(), "hello");
    
    // Copy assign
    fmt::basic_format_args<fmt::format_context> args3;
    args3 = args1;
    EXPECT_EQ(args3.get(0).get<int>(), 42);
    EXPECT_EQ(args3.get(1).get<std::string>(), "hello");
}

// Test with empty argument store
TEST(BasicFormatArgsTest, EmptyStore) {
    auto store = fmt::make_format_args();
    fmt::basic_format_args<fmt::format_context> args(store);
    EXPECT_EQ(args.max_size(), fmt::detail::max_packed_args);
    
    fmt::format_arg arg = args.get(0);
    EXPECT_FALSE(static_cast<bool>(arg));
}

// Test with many arguments (exceeding packed limit)
TEST(BasicFormatArgsTest, ManyArguments) {
    std::vector<int> values;
    for (int i = 0; i < 20; ++i) {
        values.push_back(i);
    }
    
    // Create format_arg array for unpacked storage
    fmt::format_arg args_array[20];
    for (int i = 0; i < 20; ++i) {
        args_array[i] = fmt::format_arg(fmt::detail::make_value<fmt::format_context>(values[i]));
    }
    
    fmt::basic_format_args<fmt::format_context> args(args_array, 20);
    EXPECT_EQ(args.max_size(), 20);
    
    for (int i = 0; i < 20; ++i) {
        fmt::format_arg arg = args.get(i);
        EXPECT_TRUE(static_cast<bool>(arg));
        EXPECT_EQ(arg.get<int>(), i);
    }
}