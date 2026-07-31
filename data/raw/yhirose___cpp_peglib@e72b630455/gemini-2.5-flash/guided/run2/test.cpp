#include <gtest/gtest.h>
#include <peglib.h> // Public header for the peglib library

// Helper function to create a SemanticValues object for testing
// In a real scenario, SemanticValues are populated by the PEG parser.
// For unit testing, we need to manually construct one.
peg::SemanticValues create_test_semantic_values(
    const std::vector<std::any>& values,
    const char* s_ptr,
    size_t n_len,
    size_t choice_num,
    const char* input_ss = "",
    const char* input_path = ""
) {
    peg::SemanticValues sv;
    // Accessing private members for testing purposes.
    // In a real scenario, these would be set by the parser.
    // This is generally discouraged but necessary here to construct a valid
    // SemanticValues object for testing its methods.
    struct SemanticValuesAccessor : public peg::SemanticValues {
        void set_private_members(const char* s, size_t n, size_t choice, const char* ss_in, const char* path_in) {
            s_ = s;
            n_ = n;
            choice_ = choice;
            ss = ss_in;
            path = path_in;
        }
    };

    static_cast<SemanticValuesAccessor*>(&sv)->set_private_members(s_ptr, n_len, choice_num, input_ss, input_path);

    for (const auto& val : values) {
        sv.push_back(val);
    }
    return sv;
}

TEST(SemanticValuesTest, DefaultConstructor) {
    peg::SemanticValues sv;
    ASSERT_EQ(sv.c_str(), nullptr);
    ASSERT_EQ(sv.length(), 0);
    ASSERT_EQ(sv.str(), "");
    ASSERT_EQ(sv.choice(), 0);
    ASSERT_TRUE(sv.empty());
    ASSERT_EQ(sv.size(), 0);
    ASSERT_TRUE(sv.tokens.empty());
    ASSERT_EQ(sv.path, nullptr);
    ASSERT_EQ(sv.ss, nullptr);
}

TEST(SemanticValuesTest, CStrAndLength) {
    const char* test_str = "hello";
    peg::SemanticValues sv = create_test_semantic_values({}, test_str, 5, 0);
    ASSERT_EQ(sv.c_str(), test_str);
    ASSERT_EQ(sv.length(), 5);

    sv = create_test_semantic_values({}, "", 0, 0);
    ASSERT_EQ(sv.c_str(), "");
    ASSERT_EQ(sv.length(), 0);
}

TEST(SemanticValuesTest, Str) {
    const char* test_str = "world";
    peg::SemanticValues sv = create_test_semantic_values({}, test_str, 5, 0);
    ASSERT_EQ(sv.str(), "world");

    sv = create_test_semantic_values({}, "test string", 4, 0); // Only first 4 chars
    ASSERT_EQ(sv.str(), "test");

    sv = create_test_semantic_values({}, "", 0, 0);
    ASSERT_EQ(sv.str(), "");
}

TEST(SemanticValuesTest, LineInfo) {
    const char* input_text = "line 1\nline 2\nline 3";
    peg::SemanticValues sv = create_test_semantic_values({}, input_text + 7, 6, 0, input_text); // "line 2"
    std::pair<size_t, size_t> info = sv.line_info();
    ASSERT_EQ(info.first, 2); // Line number (1-based)
    ASSERT_EQ(info.second, 1); // Column number (1-based)

    sv = create_test_semantic_values({}, input_text + 15, 6, 0, input_text); // "line 3"
    info = sv.line_info();
    ASSERT_EQ(info.first, 3);
    ASSERT_EQ(info.second, 1);

    sv = create_test_semantic_values({}, input_text + 17, 4, 0, input_text); // "ne 3"
    info = sv.line_info();
    ASSERT_EQ(info.first, 3);
    ASSERT_EQ(info.second, 3);

    // Empty input string
    sv = create_test_semantic_values({}, "", 0, 0, "");
    info = sv.line_info();
    ASSERT_EQ(info.first, 1);
    ASSERT_EQ(info.second, 1);

    // Matched string at the beginning of the input
    sv = create_test_semantic_values({}, input_text, 6, 0, input_text); // "line 1"
    info = sv.line_info();
    ASSERT_EQ(info.first, 1);
    ASSERT_EQ(info.second, 1);
}

TEST(SemanticValuesTest, Choice) {
    peg::SemanticValues sv = create_test_semantic_values({}, "abc", 3, 0);
    ASSERT_EQ(sv.choice(), 0);

    sv = create_test_semantic_values({}, "abc", 3, 5);
    ASSERT_EQ(sv.choice(), 5);
}

TEST(SemanticValuesTest, Token) {
    peg::SemanticValues sv = create_test_semantic_values({}, "full_match", 10, 0);
    ASSERT_EQ(sv.token(), "full_match"); // Default token is the matched string

    sv.tokens.push_back({"tok1", 4});
    sv.tokens.push_back({"token2", 6});
    ASSERT_EQ(sv.token(), "tok1"); // First token if tokens exist
    ASSERT_EQ(sv.token(0), "tok1");
    ASSERT_EQ(sv.token(1), "token2");

    // Test with out-of-bounds access (should assert in debug, but return garbage in release)
    // We can't reliably test the assert itself in GoogleTest, but we can ensure valid access works.
    // For release builds, this would be undefined behavior.
    // ASSERT_DEATH(sv.token(2), ""); // This would test the assert, but depends on build type.
}

TEST(SemanticValuesTest, TokenEmpty) {
    peg::SemanticValues sv = create_test_semantic_values({}, "matched", 7, 0);
    ASSERT_TRUE(sv.tokens.empty());
    ASSERT_EQ(sv.token(), "matched"); // Should return the main matched string
}

TEST(SemanticValuesTest, TransformBasic) {
    peg::SemanticValues sv = create_test_semantic_values({std::any(10), std::any(20), std::any(30)}, "test", 4, 0);

    std::vector<int> transformed_ints = sv.transform<int>();
    ASSERT_EQ(transformed_ints.size(), 3);
    ASSERT_EQ(transformed_ints[0], 10);
    ASSERT_EQ(transformed_ints[1], 20);
    ASSERT_EQ(transformed_ints[2], 30);

    // Test with different type
    peg::SemanticValues sv_str = create_test_semantic_values({std::any(std::string("a")), std::any(std::string("b"))}, "test", 4, 0);
    std::vector<std::string> transformed_strings = sv_str.transform<std::string>();
    ASSERT_EQ(transformed_strings.size(), 2);
    ASSERT_EQ(transformed_strings[0], "a");
    ASSERT_EQ(transformed_strings[1], "b");
}

TEST(SemanticValuesTest, TransformWithRange) {
    peg::SemanticValues sv = create_test_semantic_values({std::any(10), std::any(20), std::any(30), std::any(40)}, "test", 4, 0);

    std::vector<int> transformed_ints = sv.transform<int>(1, 3); // Elements at index 1 and 2
    ASSERT_EQ(transformed_ints.size(), 2);
    ASSERT_EQ(transformed_ints[0], 20);
    ASSERT_EQ(transformed_ints[1], 30);

    transformed_ints = sv.transform<int>(0, 1); // Only first element
    ASSERT_EQ(transformed_ints.size(), 1);
    ASSERT_EQ(transformed_ints[0], 10);

    transformed_ints = sv.transform<int>(2); // From index 2 to end
    ASSERT_EQ(transformed_ints.size(), 2);
    ASSERT_EQ(transformed_ints[0], 30);
    ASSERT_EQ(transformed_ints[1], 40);

    transformed_ints = sv.transform<int>(0, 0); // Empty range
    ASSERT_TRUE(transformed_ints.empty());

    transformed_ints = sv.transform<int>(5, 10); // Out of bounds start
    ASSERT_TRUE(transformed_ints.empty());

    transformed_ints = sv.transform<int>(0, 100); // End out of bounds, should cap at size()
    ASSERT_EQ(transformed_ints.size(), 4);
    ASSERT_EQ(transformed_ints[0], 10);
    ASSERT_EQ(transformed_ints[1], 20);
    ASSERT_EQ(transformed_ints[2], 30);
    ASSERT_EQ(transformed_ints[3], 40);
}

TEST(SemanticValuesTest, TransformEmptySemanticValues) {
    peg::SemanticValues sv = create_test_semantic_values({}, "test", 4, 0);
    std::vector<int> transformed_ints = sv.transform<int>();
    ASSERT_TRUE(transformed_ints.empty());

    transformed_ints = sv.transform<int>(0, 1);
    ASSERT_TRUE(transformed_ints.empty());
}

TEST(SemanticValuesTest, TransformWithMismatchedType) {
    peg::SemanticValues sv = create_test_semantic_values({std::any(10), std::any(std::string("hello"))}, "test", 4, 0);

    // Attempting to transform to a type that doesn't match all elements will throw std::bad_any_cast
    ASSERT_NO_THROW({
        std::vector<int> transformed_ints = sv.transform<int>(0, 1); // This should work
        ASSERT_EQ(transformed_ints.size(), 1);
        ASSERT_EQ(transformed_ints[0], 10);
    });

    ASSERT_THROW({
        std::vector<int> transformed_ints = sv.transform<int>(); // This should throw because of "hello"
    }, std::bad_any_cast);

    ASSERT_THROW({
        std::vector<std::string> transformed_strings = sv.transform<std::string>(); // This should throw because of 10
    }, std::bad_any_cast);
}

TEST(SemanticValuesTest, VectorOperations) {
    peg::SemanticValues sv;
    ASSERT_TRUE(sv.empty());
    ASSERT_EQ(sv.size(), 0);

    sv.push_back(std::any(1));
    ASSERT_FALSE(sv.empty());
    ASSERT_EQ(sv.size(), 1);
    ASSERT_EQ(std::any_cast<int>(sv[0]), 1);
    ASSERT_EQ(std::any_cast<int>(sv.at(0)), 1);
    ASSERT_EQ(std::any_cast<int>(sv.front()), 1);
    ASSERT_EQ(std::any_cast<int>(sv.back()), 1);

    sv.push_back(std::any("two"));
    ASSERT_EQ(sv.size(), 2);
    ASSERT_EQ(std::any_cast<std::string>(sv[1]), "two");
    ASSERT_EQ(std::any_cast<std::string>(sv.back()), "two");

    sv.pop_back();
    ASSERT_EQ(sv.size(), 1);
    ASSERT_EQ(std::any_cast<int>(sv.back()), 1);

    sv.clear();
    ASSERT_TRUE(sv.empty());
    ASSERT_EQ(sv.size(), 0);

    sv.emplace_back(3.14);
    ASSERT_EQ(sv.size(), 1);
    ASSERT_EQ(std::any_cast<double>(sv[0]), 3.14);

    // Test iterators
    sv.push_back(std::any(100));
    sv.push_back(std::any(200));
    int sum = 0;
    for (const auto& val : sv) {
        if (val.type() == typeid(double)) {
            sum += static_cast<int>(std::any_cast<double>(val));
        } else if (val.type() == typeid(int)) {
            sum += std::any_cast<int>(val);
        }
    }
    ASSERT_EQ(sum, 303); // 3.14 + 100 + 200 (int cast for double)
}

TEST(SemanticValuesTest, PathAndSS) {
    const char* test_path = "/path/to/file.txt";
    const char* test_ss = "input string content";
    peg::SemanticValues sv = create_test_semantic_values({}, "match", 5, 0, test_ss, test_path);

    ASSERT_EQ(sv.path, test_path);
    ASSERT_EQ(sv.ss, test_ss);

    // Test with nullptr
    sv = create_test_semantic_values({}, "match", 5, 0, nullptr, nullptr);
    ASSERT_EQ(sv.path, nullptr);
    ASSERT_EQ(sv.ss, nullptr);
}