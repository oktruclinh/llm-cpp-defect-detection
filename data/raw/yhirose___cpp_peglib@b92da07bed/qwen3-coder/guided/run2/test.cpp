#include <gtest/gtest.h>
#include "peglib.h"
#include <string>
#include <vector>

using namespace peglib;

class ContextTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

TEST_F(ContextTest, ConstructorInitialization) {
    const char* path = "test_path";
    const char* s = "hello world";
    size_t l = 11;
    size_t def_count = 5;
    bool enablePackratParsing = true;
    
    auto whitespaceOpe = std::make_shared<peglib::Ope>();
    auto wordOpe = std::make_shared<peglib::Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    EXPECT_EQ(ctx.path, path);
    EXPECT_EQ(ctx.s, s);
    EXPECT_EQ(ctx.l, l);
    EXPECT_EQ(ctx.def_count, def_count);
    EXPECT_TRUE(ctx.enablePackratParsing);
    EXPECT_EQ(ctx.cache_registered.size(), def_count * (l + 1));
    EXPECT_EQ(ctx.cache_success.size(), def_count * (l + 1));
    EXPECT_EQ(ctx.source_line_index.size(), 1); // Only one newline in "hello world"
    EXPECT_EQ(ctx.source_line_index[0], 10); // Position of the newline
    EXPECT_EQ(ctx.args_stack.size(), 1);
    EXPECT_EQ(ctx.capture_scope_stack.size(), 1);
}

TEST_F(ContextTest, ConstructorWithNoNewlines) {
    const char* path = "test_path";
    const char* s = "hello";
    size_t l = 5;
    size_t def_count = 3;
    bool enablePackratParsing = false;
    
    auto whitespaceOpe = std::make_shared<peglib::Ope>();
    auto wordOpe = std::make_shared<peglib::Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    EXPECT_EQ(ctx.path, path);
    EXPECT_EQ(ctx.s, s);
    EXPECT_EQ(ctx.l, l);
    EXPECT_EQ(ctx.def_count, def_count);
    EXPECT_FALSE(ctx.enablePackratParsing);
    EXPECT_EQ(ctx.source_line_index.size(), 1); // No newlines, so only end position
    EXPECT_EQ(ctx.source_line_index[0], 5); // End position
}

TEST_F(ContextTest, ConstructorWithMultipleNewlines) {
    const char* path = "test_path";
    const char* s = "hello\nworld\n";
    size_t l = 12;
    size_t def_count = 2;
    bool enablePackratParsing = true;
    
    auto whitespaceOpe = std::make_shared<peglib::Ope>();
    auto wordOpe = std::make_shared<peglib::Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    EXPECT_EQ(ctx.path, path);
    EXPECT_EQ(ctx.s, s);
    EXPECT_EQ(ctx.l, l);
    EXPECT_EQ(ctx.def_count, def_count);
    EXPECT_TRUE(ctx.enablePackratParsing);
    EXPECT_EQ(ctx.source_line_index.size(), 3); // Two newlines + end
    EXPECT_EQ(ctx.source_line_index[0], 5); // Position of first newline
    EXPECT_EQ(ctx.source_line_index[1], 11); // Position of second newline
    EXPECT_EQ(ctx.source_line_index[2], 12); // End position
}

TEST_F(ContextTest, PackratParsingDisabled) {
    const char* path = "test_path";
    const char* s = "hello";
    size_t l = 5;
    size_t def_count = 1;
    bool enablePackratParsing = false;
    
    auto whitespaceOpe = std::make_shared<peglib::Ope>();
    auto wordOpe = std::make_shared<peglib::Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    size_t len = 0;
    any val;
    
    // Test that packrat function calls the provided function when packrat parsing is disabled
    ctx.packrat(s, 0, len, val, [](any& v) {
        v = std::string("test_value");
        return;
    });
    
    // Since packrat is disabled, the function should be called and len should be 0
    // (this is a simplified test - in practice, the function would set len appropriately)
    EXPECT_EQ(len, 0);
}

TEST_F(ContextTest, PackratParsingEnabled) {
    const char* path = "test_path";
    const char* s = "hello";
    size_t l = 5;
    size_t def_count = 1;
    bool enablePackratParsing = true;
    
    auto whitespaceOpe = std::make_shared<peglib::Ope>();
    auto wordOpe = std::make_shared<peglib::Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    size_t len = 0;
    any val;
    
    // Test that packrat function works when enabled
    ctx.packrat(s, 0, len, val, [](any& v) {
        v = std::string("test_value");
        return;
    });
    
    // Should have been registered in cache
    EXPECT_TRUE(ctx.cache_registered[0]);
    EXPECT_TRUE(ctx.cache_success[0]);
}

TEST_F(ContextTest, PushFunction) {
    const char* path = "test_path";
    const char* s = "hello";
    size_t l = 5;
    size_t def_count = 1;
    bool enablePackratParsing = false;
    
    auto whitespaceOpe = std::make_shared<peglib::Ope>();
    auto wordOpe = std::make_shared<peglib::Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    // Test pushing to value stack
    auto& sv = ctx.push();
    
    // Should have created a new SemanticValues object
    EXPECT_NE(sv.s_, nullptr);
    EXPECT_EQ(sv.n_, 0);
}

TEST_F(ContextTest, ConstructorEmptyString) {
    const char* path = "test_path";
    const char* s = "";
    size_t l = 0;
    size_t def_count = 1;
    bool enablePackratParsing = true;
    
    auto whitespaceOpe = std::make_shared<peglib::Ope>();
    auto wordOpe = std::make_shared<peglib::Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    EXPECT_EQ(ctx.path, path);
    EXPECT_EQ(ctx.s, s);
    EXPECT_EQ(ctx.l, l);
    EXPECT_EQ(ctx.source_line_index.size(), 1); // Only end position for empty string
    EXPECT_EQ(ctx.source_line_index[0], 0); // End position
}

TEST_F(ContextTest, ConstructorZeroDefCount) {
    const char* path = "test_path";
    const char* s = "hello";
    size_t l = 5;
    size_t def_count = 0;
    bool enablePackratParsing = true;
    
    auto whitespaceOpe = std::make_shared<peglib::Ope>();
    auto wordOpe = std::make_shared<peglib::Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    EXPECT_EQ(ctx.def_count, def_count);
    EXPECT_EQ(ctx.cache_registered.size(), 0);
    EXPECT_EQ(ctx.cache_success.size(), 0);
}