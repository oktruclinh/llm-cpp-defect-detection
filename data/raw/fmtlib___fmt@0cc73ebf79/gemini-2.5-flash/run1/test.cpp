#include <fmt/core.h>
#include <gtest/gtest.h>

// Helper struct to simulate a format_args object for testing purposes.
// This is a simplified version and doesn't fully replicate fmt::format_args,
// but it's sufficient to test the get_id logic.
struct MockFormatArgs {
    struct NamedArgInfo {
        fmt::string_view name;
        int id;
    };

    struct NamedArgsData {
        const NamedArgInfo* data;
        size_t size;
    };

    // This simulates the internal structure that get_id accesses.
    // In real fmt, this is part of internal::format_args_impl.
    struct Value {
        NamedArgsData named_args;
    };

    // For is_packed() == true
    Value values_[-1]; // Access as values_[-1]

    // For is_packed() == false
    struct Arg {
        Value value_;
    };
    Arg args_[-1]; // Access as args_[-1].value_

    bool packed_ = false;
    bool has_named_args_ = false;

    MockFormatArgs(bool packed, bool has_named_args) : packed_(packed), has_named_args_(has_named_args) {}

    bool has_named_args() const { return has_named_args_; }
    bool is_packed() const { return packed_; }

    // The focal function under test, copied directly from fmt/core.h
    template <typename Char> int get_id(fmt::basic_string_view<Char> name) const {
        if (!has_named_args()) return -1;
        const auto& named_args =
            (is_packed() ? values_[-1] : args_[-1].value_).named_args;
        for (size_t i = 0; i < named_args.size; ++i) {
            if (named_args.data[i].name == name) return named_args.data[i].id;
        }
        return -1; // Original function implicitly returns -1 if not found
    }
};

TEST(FormatArgsGetIdTest, NoNamedArgs) {
    MockFormatArgs args(false, false); // Not packed, no named args
    EXPECT_EQ(args.get_id("test"), -1);

    MockFormatArgs packed_args(true, false); // Packed, no named args
    EXPECT_EQ(packed_args.get_id("test"), -1);
}

TEST(FormatArgsGetIdTest, NamedArgNotFound) {
    MockFormatArgs::NamedArgInfo data[] = {
        {"arg1", 0},
        {"arg2", 1}
    };
    MockFormatArgs::NamedArgsData named_args_data = {data, 2};

    // Test non-packed case
    MockFormatArgs args(false, true);
    args.args_[-1].value_.named_args = named_args_data;
    EXPECT_EQ(args.get_id("non_existent_arg"), -1);

    // Test packed case
    MockFormatArgs packed_args(true, true);
    packed_args.values_[-1].named_args = named_args_data;
    EXPECT_EQ(packed_args.get_id("non_existent_arg"), -1);
}

TEST(FormatArgsGetIdTest, NamedArgFoundNonPacked) {
    MockFormatArgs::NamedArgInfo data[] = {
        {"first", 0},
        {"second", 1},
        {"third", 2}
    };
    MockFormatArgs::NamedArgsData named_args_data = {data, 3};

    MockFormatArgs args(false, true);
    args.args_[-1].value_.named_args = named_args_data;

    EXPECT_EQ(args.get_id("first"), 0);
    EXPECT_EQ(args.get_id("second"), 1);
    EXPECT_EQ(args.get_id("third"), 2);
}

TEST(FormatArgsGetIdTest, NamedArgFoundPacked) {
    MockFormatArgs::NamedArgInfo data[] = {
        {"alpha", 10},
        {"beta", 20},
        {"gamma", 30}
    };
    MockFormatArgs::NamedArgsData named_args_data = {data, 3};

    MockFormatArgs packed_args(true, true);
    packed_args.values_[-1].named_args = named_args_data;

    EXPECT_EQ(packed_args.get_id("alpha"), 10);
    EXPECT_EQ(packed_args.get_id("beta"), 20);
    EXPECT_EQ(packed_args.get_id("gamma"), 30);
}

TEST(FormatArgsGetIdTest, EmptyNamedArgsList) {
    MockFormatArgs::NamedArgInfo data[] = {};
    MockFormatArgs::NamedArgsData named_args_data = {data, 0};

    // Test non-packed case
    MockFormatArgs args(false, true);
    args.args_[-1].value_.named_args = named_args_data;
    EXPECT_EQ(args.get_id("any_arg"), -1);

    // Test packed case
    MockFormatArgs packed_args(true, true);
    packed_args.values_[-1].named_args = named_args_data;
    EXPECT_EQ(packed_args.get_id("any_arg"), -1);
}

TEST(FormatArgsGetIdTest, DuplicateNamedArgsReturnsFirstMatch) {
    MockFormatArgs::NamedArgInfo data[] = {
        {"duplicate", 0},
        {"unique", 1},
        {"duplicate", 2} // This one should not be found by get_id
    };
    MockFormatArgs::NamedArgsData named_args_data = {data, 3};

    // Test non-packed case
    MockFormatArgs args(false, true);
    args.args_[-1].value_.named_args = named_args_data;
    EXPECT_EQ(args.get_id("duplicate"), 0); // Should return the ID of the first match

    // Test packed case
    MockFormatArgs packed_args(true, true);
    packed_args.values_[-1].named_args = named_args_data;
    EXPECT_EQ(packed_args.get_id("duplicate"), 0); // Should return the ID of the first match
}

TEST(FormatArgsGetIdTest, CaseSensitivity) {
    MockFormatArgs::NamedArgInfo data[] = {
        {"Name", 0},
        {"name", 1}
    };
    MockFormatArgs::NamedArgsData named_args_data = {data, 2};

    MockFormatArgs args(false, true);
    args.args_[-1].value_.named_args = named_args_data;

    EXPECT_EQ(args.get_id("Name"), 0);
    EXPECT_EQ(args.get_id("name"), 1);
    EXPECT_EQ(args.get_id("NAME"), -1); // Should not match
}

TEST(FormatArgsGetIdTest, LongNamedArg) {
    std::string long_name_str(100, 'a');
    fmt::string_view long_name_sv(long_name_str);

    MockFormatArgs::NamedArgInfo data[] = {
        {long_name_sv, 42}
    };
    MockFormatArgs::NamedArgsData named_args_data = {data, 1};

    MockFormatArgs args(false, true);
    args.args_[-1].value_.named_args = named_args_data;

    EXPECT_EQ(args.get_id(long_name_sv), 42);
    EXPECT_EQ(args.get_id("short"), -1);
}

TEST(FormatArgsGetIdTest, UnicodeNamedArg) {
    // Note: fmt::string_view is byte-based, so this should work as long as the
    // comparison is byte-for-byte.
    fmt::string_view unicode_name = "你好世界";
    fmt::string_view different_unicode_name = "こんにちは";

    MockFormatArgs::NamedArgInfo data[] = {
        {unicode_name, 123}
    };
    MockFormatArgs::NamedArgsData named_args_data = {data, 1};

    MockFormatArgs args(false, true);
    args.args_[-1].value_.named_args = named_args_data;

    EXPECT_EQ(args.get_id(unicode_name), 123);
    EXPECT_EQ(args.get_id(different_unicode_name), -1);
}

// Test with wide characters (wchar_t)
TEST(FormatArgsGetIdTest, WideCharNamedArg) {
    // This test requires fmt::basic_string_view<wchar_t>
    // The MockFormatArgs::NamedArgInfo uses fmt::string_view (char),
    // so we need to adapt the test to simulate the Char template parameter.

    // To properly test the template, we'd need a MockFormatArgs that can hold
    // NamedArgInfo with basic_string_view<wchar_t>.
    // For simplicity and to test the core logic of get_id, we'll assume
    // the underlying comparison mechanism is robust for different Char types
    // as long as the string views match.
    // The focal function itself is templated on Char, so the comparison
    // `named_args.data[i].name == name` will use `basic_string_view<Char>::operator==`.

    // We can't easily put wchar_t literals into the current MockFormatArgs::NamedArgInfo
    // which uses fmt::string_view (char).
    // However, the template `get_id<Char>` means the input `name` can be `basic_string_view<wchar_t>`.
    // The comparison `named_args.data[i].name == name` will then compare `fmt::string_view` (char)
    // with `fmt::basic_string_view<wchar_t>`, which will always be false unless
    // fmt::basic_string_view has a cross-char-type comparison, which it doesn't by default.
    // This implies that `get_id` is intended to be used with `Char` matching the `Char`
    // of the stored named arguments.

    // Let's create a specialized mock for wchar_t to properly test this.
    struct MockWFormatArgs {
        struct NamedArgInfoW {
            fmt::basic_string_view<wchar_t> name;
            int id;
        };

        struct NamedArgsDataW {
            const NamedArgInfoW* data;
            size_t size;
        };

        struct ValueW {
            NamedArgsDataW named_args;
        };

        ValueW values_[-1];
        struct ArgW {
            ValueW value_;
        };
        ArgW args_[-1];

        bool packed_ = false;
        bool has_named_args_ = false;

        MockWFormatArgs(bool packed, bool has_named_args) : packed_(packed), has_named_args_(has_named_args) {}

        bool has_named_args() const { return has_named_args_; }
        bool is_packed() const { return packed_; }

        template <typename Char> int get_id(fmt::basic_string_view<Char> name) const {
            if (!has_named_args()) return -1;
            // This cast is problematic if Char is not wchar_t.
            // The original fmt::format_args stores `internal::named_arg_info<Char>`
            // so the `named_args.data` would already be `const internal::named_arg_info<Char>*`.
            // For this test, we assume Char is wchar_t.
            const auto& named_args =
                (is_packed() ? values_[-1] : args_[-1].value_).named_args;
            for (size_t i = 0; i < named_args.size; ++i) {
                if (named_args.data[i].name == name) return named_args.data[i].id;
            }
            return -1;
        }
    };

    MockWFormatArgs::NamedArgInfoW wdata[] = {
        {L"wide_arg", 100},
        {L"another_wide", 200}
    };
    MockWFormatArgs::NamedArgsDataW w_named_args_data = {wdata, 2};

    MockWFormatArgs w_args(false, true);
    w_args.args_[-1].value_.named_args = w_named_args_data;

    EXPECT_EQ(w_args.get_id(L"wide_arg"), 100);
    EXPECT_EQ(w_args.get_id(L"another_wide"), 200);
    EXPECT_EQ(w_args.get_id(L"non_existent_wide"), -1);

    // Test with char string_view, should not match
    EXPECT_EQ(w_args.get_id("wide_arg"), -1);
}