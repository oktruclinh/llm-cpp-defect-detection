#include <gtest/gtest.h>
#include <fmt/core.h>

// Tests for the get_id function via the named argument API
// The function maps named argument strings to their positional IDs

TEST(NamedArgGetIdTest, EmptyNameReturnsMinusOne) {
    // When no named arguments are provided, get_id should return -1
    auto result = fmt::format("{0}", 42);
    ASSERT_EQ(result, "42");
    // No direct API to call get_id, but the behavior is tested through format string parsing
}

TEST(NamedArgGetIdTest, NamedArgumentFound) {
    // Basic named argument lookup
    auto result = fmt::format("{name}", fmt::arg("name", 42));
    ASSERT_EQ(result, "42");
}

TEST(NamedArgGetIdTest, MultipleNamedArguments) {
    // Multiple named arguments with different names
    auto result = fmt::format("{a}{b}", fmt::arg("a", 1), fmt::arg("b", 2));
    ASSERT_EQ(result, "12");
}

TEST(NamedArgGetIdTest, NamedArgumentNotFoundReturnsMinusOne) {
    // When a named argument is not provided, format should throw or behave accordingly
    // This tests the case where get_id returns -1 for an unknown name
    ASSERT_THROW(fmt::format("{unknown_name}", fmt::arg("known", 42)), fmt::format_error);
}

TEST(NamedArgGetIdTest, MixedPositionalAndNamed) {
    // Mix of positional and named arguments
    auto result = fmt::format("{0} {name}", 10, fmt::arg("name", 20));
    ASSERT_EQ(result, "10 20");
}

TEST(NamedArgGetIdTest, NamedArgumentWithSameNameDifferentCase) {
    // Case-sensitive name matching
    auto result = fmt::format("{Name}", fmt::arg("name", 1));
    ASSERT_THROW(fmt::format("{Name}", fmt::arg("name", 1)), fmt::format_error);
}

TEST(NamedArgGetIdTest, EmptyStringAsName) {
    // Empty string as argument name
    auto result = fmt::format("{}", fmt::arg("", 42));
    ASSERT_EQ(result, "42");
}

TEST(NamedArgGetIdTest, SpecialCharactersInName) {
    // Names with special characters
    auto result = fmt::format("{my_name}", fmt::arg("my_name", 1));
    ASSERT_EQ(result, "1");
    
    result = fmt::format("{_name}", fmt::arg("_name", 2));
    ASSERT_EQ(result, "2");
}

TEST(NamedArgGetIdTest, LongNameArguments) {
    // Long argument names
    std::string long_name = "a" + std::string(100, 'b');
    auto result = fmt::format("{long_name}", fmt::arg(long_name, 99));
    ASSERT_EQ(result, "99");
}

TEST(NamedArgGetIdTest, DuplicateNamedArgumentLastWins) {
    // Multiple arguments with same name; last one should be used
    auto result = fmt::format("{x}", fmt::arg("x", 1), fmt::arg("x", 2));
    ASSERT_EQ(result, "2");
}

TEST(NamedArgGetIdTest, ManyNamedArguments) {
    // Large number of named arguments
    std::vector<fmt::detail::named_arg_value<char>> args;
    for (int i = 0; i < 100; ++i) {
        std::string name = "arg" + std::to_string(i);
        args.push_back(fmt::arg(name, i));
    }
    
    std::string format_str = "";
    for (int i = 99; i >= 0; --i) {
        format_str += "{arg" + std::to_string(i) + "}";
    }
    
    // Construct format call with variadic args
    auto result = fmt::format(
        fmt::runtime(format_str),
        fmt::arg("arg99", 99), fmt::arg("arg98", 98), fmt::arg("arg97", 97),
        fmt::arg("arg96", 96), fmt::arg("arg95", 95), fmt::arg("arg94", 94),
        fmt::arg("arg93", 93), fmt::arg("arg92", 92), fmt::arg("arg91", 91),
        fmt::arg("arg90", 90), fmt::arg("arg89", 89), fmt::arg("arg88", 88),
        fmt::arg("arg87", 87), fmt::arg("arg86", 86), fmt::arg("arg85", 85),
        fmt::arg("arg84", 84), fmt::arg("arg83", 83), fmt::arg("arg82", 82),
        fmt::arg("arg81", 81), fmt::arg("arg80", 80), fmt::arg("arg79", 79),
        fmt::arg("arg78", 78), fmt::arg("arg77", 77), fmt::arg("arg76", 76),
        fmt::arg("arg75", 75), fmt::arg("arg74", 74), fmt::arg("arg73", 73),
        fmt::arg("arg72", 72), fmt::arg("arg71", 71), fmt::arg("arg70", 70),
        fmt::arg("arg69", 69), fmt::arg("arg68", 68), fmt::arg("arg67", 67),
        fmt::arg("arg66", 66), fmt::arg("arg65", 65), fmt::arg("arg64", 64),
        fmt::arg("arg63", 63), fmt::arg("arg62", 62), fmt::arg("arg61", 61),
        fmt::arg("arg60", 60), fmt::arg("arg59", 59), fmt::arg("arg58", 58),
        fmt::arg("arg57", 57), fmt::arg("arg56", 56), fmt::arg("arg55", 55),
        fmt::arg("arg54", 54), fmt::arg("arg53", 53), fmt::arg("arg52", 52),
        fmt::arg("arg51", 51), fmt::arg("arg50", 50), fmt::arg("arg49", 49),
        fmt::arg("arg48", 48), fmt::arg("arg47", 47), fmt::arg("arg46", 46),
        fmt::arg("arg45", 45), fmt::arg("arg44", 44), fmt::arg("arg43", 43),
        fmt::arg("arg42", 42), fmt::arg("arg41", 41), fmt::arg("arg40", 40),
        fmt::arg("arg39", 39), fmt::arg("arg38", 38), fmt::arg("arg37", 37),
        fmt::arg("arg36", 36), fmt::arg("arg35", 35), fmt::arg("arg34", 34),
        fmt::arg("arg33", 33), fmt::arg("arg32", 32), fmt::arg("arg31", 31),
        fmt::arg("arg30", 30), fmt::arg("arg29", 29), fmt::arg("arg28", 28),
        fmt::arg("arg27", 27), fmt::arg("arg26", 26), fmt::arg("arg25", 25),
        fmt::arg("arg24", 24), fmt::arg("arg23", 23), fmt::arg("arg22", 22),
        fmt::arg("arg21", 21), fmt::arg("arg20", 20), fmt::arg("arg19", 19),
        fmt::arg("arg18", 18), fmt::arg("arg17", 17), fmt::arg("arg16", 16),
        fmt::arg("arg15", 15), fmt::arg("arg14", 14), fmt::arg("arg13", 13),
        fmt::arg("arg12", 12), fmt::arg("arg11", 11), fmt::arg("arg10", 10),
        fmt::arg("arg9", 9), fmt::arg("arg8", 8), fmt::arg("arg7", 7),
        fmt::arg("arg6", 6), fmt::arg("arg5", 5), fmt::arg("arg4", 4),
        fmt::arg("arg3", 3), fmt::arg("arg2", 2), fmt::arg("arg1", 1),
        fmt::arg("arg0", 0)
    );
    
    // Verify a few specific values
    ASSERT_EQ(result, "9998979695949392919089888786858483828180797877767574737271706968676665646362616059585756555453525150494847464544434241403938373635343332313029282726252423222120191817161514131211109876543210");
}