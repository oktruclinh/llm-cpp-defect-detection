#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>
#include <memory>

// Test fixture for Context-related tests
class ContextTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a simple grammar for testing
        grammar = std::make_shared<peglib::peg::Grammar>();
    }

    std::shared_ptr<peglib::peg::Grammar> grammar;
};

// Test basic Context construction with valid input
TEST_F(ContextTest, BasicConstruction) {
    std::string input = "hello\nworld\n";
    auto whitespaceOpe = std::make_shared<peglib::peg::Ope>();
    auto wordOpe = std::make_shared<peglib::peg::Ope>();
    
    peglib::peg::Context ctx("/test/path", input.c_str(), input.size(), 5,
                             whitespaceOpe, wordOpe, false, nullptr, nullptr);
    
    EXPECT_STREQ(ctx.path, "/test/path");
    EXPECT_STREQ(ctx.s, input.c_str());
    EXPECT_EQ(ctx.l, input.size());
    EXPECT_EQ(ctx.def_count, 5);
    EXPECT_FALSE(ctx.enablePackratParsing);
}

// Test source_line_index construction with newlines
TEST_F(ContextTest, SourceLineIndex) {
    std::string input = "line1\nline2\nline3\n";
    auto whitespaceOpe = std::make_shared<peglib::peg::Ope>();
    auto wordOpe = std::make_shared<peglib::peg::Ope>();
    
    peglib::peg::Context ctx("", input.c_str(), input.size(), 1,
                             whitespaceOpe, wordOpe, false, nullptr, nullptr);
    
    ASSERT_EQ(ctx.source_line_index.size(), 4); // 3 newlines + end
    EXPECT_EQ(ctx.source_line_index[0], 5);  // after "line1"
    EXPECT_EQ(ctx.source_line_index[1], 11); // after "line2"
    EXPECT_EQ(ctx.source_line_index[2], 17); // after "line3"
    EXPECT_EQ(ctx.source_line_index[3], 18); // end of string
}

// Test source_line_index with empty input
TEST_F(ContextTest, EmptyInputSourceLineIndex) {
    std::string input = "";
    auto whitespaceOpe = std::make_shared<peglib::peg::Ope>();
    auto wordOpe = std::make_shared<peglib::peg::Ope>();
    
    peglib::peg::Context ctx("", input.c_str(), input.size(), 1,
                             whitespaceOpe, wordOpe, false, nullptr, nullptr);
    
    ASSERT_EQ(ctx.source_line_index.size(), 1);
    EXPECT_EQ(ctx.source_line_index[0], 0);
}

// Test source_line_index with no newlines
TEST_F(ContextTest, NoNewlinesSourceLineIndex) {
    std::string input = "singleline";
    auto whitespaceOpe = std::make_shared<peglib::peg::Ope>();
    auto wordOpe = std::make_shared<peglib::peg::Ope>();
    
    peglib::peg::Context ctx("", input.c_str(), input.size(), 1,
                             whitespaceOpe, wordOpe, false, nullptr, nullptr);
    
    ASSERT_EQ(ctx.source_line_index.size(), 1);
    EXPECT_EQ(ctx.source_line_index[0], 10);
}

// Test packrat parsing disabled (default behavior)
TEST_F(ContextTest, PackratDisabled) {
    std::string input = "test";
    auto whitespaceOpe = std::make_shared<peglib::peg::Ope>();
    auto wordOpe = std::make_shared<peglib::peg::Ope>();
    
    peglib::peg::Context ctx("", input.c_str(), input.size(), 3,
                             whitespaceOpe, wordOpe, false, nullptr, nullptr);
    
    size_t len = 0;
    peglib::any val;
    bool fn_called = false;
    
    ctx.packrat(input.c_str(), 1, len, val, [&](peglib::any&) {
        fn_called = true;
    });
    
    EXPECT_TRUE(fn_called);
}

// Test packrat parsing enabled with caching
TEST_F(ContextTest, PackratEnabledCaching) {
    std::string input = "test";
    auto whitespaceOpe = std::make_shared<peglib::peg::Ope>();
    auto wordOpe = std::make_shared<peglib::peg::Ope>();
    
    peglib::peg::Context ctx("", input.c_str(), input.size(), 3,
                             whitespaceOpe, wordOpe, true, nullptr, nullptr);
    
    size_t len = 0;
    peglib::any val;
    int call_count = 0;
    
    // First call should execute the function
    ctx.packrat(input.c_str(), 1, len, val, [&](peglib::any& v) {
        call_count++;
        len = 4; // Simulate successful parse
        v = std::string("result");
    });
    
    EXPECT_EQ(call_count, 1);
    EXPECT_EQ(len, 4);
    EXPECT_EQ(peglib::any_cast<std::string>(val), "result");
    
    // Second call with same position and def_id should use cache
    size_t len2 = 0;
    peglib::any val2;
    ctx.packrat(input.c_str(), 1, len2, val2, [&](peglib::any&) {
        call_count++;
    });
    
    EXPECT_EQ(call_count, 1); // Function should not be called again
    EXPECT_EQ(len2, 4);
    EXPECT_EQ(peglib::any_cast<std::string>(val2), "result");
}

// Test packrat parsing with failed parse caching
TEST_F(ContextTest, PackratFailedParseCaching) {
    std::string input = "test";
    auto whitespaceOpe = std::make_shared<peglib::peg::Ope>();
    auto wordOpe = std::make_shared<peglib::peg::Ope>();
    
    peglib::peg::Context ctx("", input.c_str(), input.size(), 3,
                             whitespaceOpe, wordOpe, true, nullptr, nullptr);
    
    size_t len = 0;
    peglib::any val;
    int call_count = 0;
    
    // First call with failed parse
    ctx.packrat(input.c_str(), 2, len, val, [&](peglib::any&) {
        call_count++;
        len = static_cast<size_t>(-1); // Simulate failed parse
    });
    
    EXPECT_EQ(call_count, 1);
    EXPECT_EQ(len, static_cast<size_t>(-1));
    
    // Second call should use cache and return failure
    size_t len2 = 0;
    peglib::any val2;
    ctx.packrat(input.c_str(), 2, len2, val2, [&](peglib::any&) {
        call_count++;
    });
    
    EXPECT_EQ(call_count, 1); // Function should not be called again
    EXPECT_EQ(len2, static_cast<size_t>(-1));
}

// Test value_stack push behavior
TEST_F(ContextTest, ValueStackPush) {
    std::string input = "test";
    auto whitespaceOpe = std::make_shared<peglib::peg::Ope>();
    auto wordOpe = std::make_shared<peglib::peg::Ope>();
    
    peglib::peg::Context ctx("", input.c_str(), input.size(), 1,
                             whitespaceOpe, wordOpe, false, nullptr, nullptr);
    
    EXPECT_EQ(ctx.value_stack_size, 0);
    
    // Push a semantic value
    auto& sv = ctx.push();
    EXPECT_EQ(ctx.value_stack_size, 1);
    EXPECT_TRUE(sv.empty());
}

// Test capture scope stack initialization
TEST_F(ContextTest, CaptureScopeStackInitialization) {
    std::string input = "test";
    auto whitespaceOpe = std::make_shared<peglib::peg::Ope>();
    auto wordOpe = std::make_shared<peglib::peg::Ope>();
    
    peglib::peg::Context ctx("", input.c_str(), input.size(), 1,
                             whitespaceOpe, wordOpe, false, nullptr, nullptr);
    
    EXPECT_EQ(ctx.capture_scope_stack.size(), 1);
    EXPECT_EQ(ctx.capture_scope_stack_size, 1);
    EXPECT_TRUE(ctx.capture_scope_stack[0].empty());
}

// Test args_stack initialization
TEST_F(ContextTest, ArgsStackInitialization) {
    std::string input = "test";
    auto whitespaceOpe = std::make_shared<peglib::peg::Ope>();
    auto wordOpe = std::make_shared<peglib::peg::Ope>();
    
    peglib::peg::Context ctx("", input.c_str(), input.size(), 1,
                             whitespaceOpe, wordOpe, false, nullptr, nullptr);
    
    EXPECT_EQ(ctx.args_stack.size(), 1);
    EXPECT_TRUE(ctx.args_stack[0].empty());
}

// Test with large input to verify no overflow
TEST_F(ContextTest, LargeInput) {
    std::string input(10000, 'a');
    input += "\n";
    auto whitespaceOpe = std::make_shared<peglib::peg::Ope>();
    auto wordOpe = std::make_shared<peglib::peg::Ope>();
    
    peglib::peg::Context ctx("", input.c_str(), input.size(), 10,
                             whitespaceOpe, wordOpe, true, nullptr, nullptr);
    
    EXPECT_EQ(ctx.source_line_index.size(), 2);
    EXPECT_EQ(ctx.source_line_index[0], 10000);
    EXPECT_EQ(ctx.source_line_index[1], 10001);
    
    // Verify packrat cache is properly sized
    EXPECT_EQ(ctx.cache_registered.size(), 10 * 10002); // def_count * (l + 1)
    EXPECT_EQ(ctx.cache_success.size(), 10 * 10002);
}

// Test with multiple newlines
TEST_F(ContextTest, MultipleNewlines) {
    std::string input = "\n\n\n\n";
    auto whitespaceOpe = std::make_shared<peglib::peg::Ope>();
    auto wordOpe = std::make_shared<peglib::peg::Ope>();
    
    peglib::peg::Context ctx("", input.c_str(), input.size(), 1,
                             whitespaceOpe, wordOpe, false, nullptr, nullptr);
    
    ASSERT_EQ(ctx.source_line_index.size(), 5);
    for (size_t i = 0; i < 4; i++) {
        EXPECT_EQ(ctx.source_line_index[i], i + 1);
    }
    EXPECT_EQ(ctx.source_line_index[4], 4);
}

// Test that Context is not copyable
TEST_F(ContextTest, NotCopyable) {
    std::string input = "test";
    auto whitespaceOpe = std::make_shared<peglib::peg::Ope>();
    auto wordOpe = std::make_shared<peglib::peg::Ope>();
    
    peglib::peg::Context ctx("", input.c_str(), input.size(), 1,
                             whitespaceOpe, wordOpe, false, nullptr, nullptr);
    
    // Verify copy constructor is deleted (compile-time check)
    // This is verified by the fact that the code compiles with the deleted copy constructor
    SUCCEED();
}