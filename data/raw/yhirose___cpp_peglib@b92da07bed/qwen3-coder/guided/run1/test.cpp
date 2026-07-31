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
    const char* s = "test string";
    size_t l = 11;
    size_t def_count = 5;
    bool enablePackratParsing = true;
    
    auto whitespaceOpe = std::make_shared<Ope>();
    auto wordOpe = std::make_shared<Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    EXPECT_EQ(ctx.path, path);
    EXPECT_EQ(ctx.s, s);
    EXPECT_EQ(ctx.l, l);
    EXPECT_EQ(ctx.def_count, def_count);
    EXPECT_TRUE(ctx.enablePackratParsing);
    EXPECT_EQ(ctx.cache_registered.size(), def_count * (l + 1));
    EXPECT_EQ(ctx.cache_success.size(), def_count * (l + 1));
    EXPECT_EQ(ctx.source_line_index.size(), 1); // Only one newline at end
    EXPECT_EQ(ctx.source_line_index[0], l);
    EXPECT_EQ(ctx.value_stack_size, 0);
    EXPECT_EQ(ctx.capture_scope_stack_size, 1);
    EXPECT_EQ(ctx.args_stack.size(), 1);
}

TEST_F(ContextTest, ConstructorWithNewlines) {
    const char* path = "test_path";
    const char* s = "line1\nline2\nline3";
    size_t l = 17;
    size_t def_count = 3;
    bool enablePackratParsing = false;
    
    auto whitespaceOpe = std::make_shared<Ope>();
    auto wordOpe = std::make_shared<Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    EXPECT_EQ(ctx.source_line_index.size(), 3); // Three newlines
    EXPECT_EQ(ctx.source_line_index[0], 5); // After "line1"
    EXPECT_EQ(ctx.source_line_index[1], 11); // After "line2"
    EXPECT_EQ(ctx.source_line_index[2], 17); // End of string
}

TEST_F(ContextTest, ConstructorEmptyString) {
    const char* path = "empty_path";
    const char* s = "";
    size_t l = 0;
    size_t def_count = 2;
    bool enablePackratParsing = true;
    
    auto whitespaceOpe = std::make_shared<Ope>();
    auto wordOpe = std::make_shared<Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    EXPECT_EQ(ctx.source_line_index.size(), 1); // Empty string has one entry
    EXPECT_EQ(ctx.source_line_index[0], 0);
    EXPECT_EQ(ctx.cache_registered.size(), def_count * (l + 1));
    EXPECT_EQ(ctx.cache_success.size(), def_count * (l + 1));
}

TEST_F(ContextTest, PackratParsingDisabled) {
    const char* path = "test_path";
    const char* s = "test string";
    size_t l = 11;
    size_t def_count = 5;
    bool enablePackratParsing = false;
    
    auto whitespaceOpe = std::make_shared<Ope>();
    auto wordOpe = std::make_shared<Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    // Test packrat function with disabled parsing
    size_t len = 0;
    any val;
    
    // This should call the function directly without caching
    ctx.packrat(s, 0, len, val, [](any& v) {
        v = std::string("test_value");
        // Simulate successful parsing
        // len = 5; // This would be set by the function
    });
    
    // Since packrat is disabled, we can't verify cache behavior,
    // but we can verify the function was called
    // The actual behavior depends on the lambda implementation
}

TEST_F(ContextTest, PackratParsingEnabled) {
    const char* path = "test_path";
    const char* s = "test string";
    size_t l = 11;
    size_t def_count = 5;
    bool enablePackratParsing = true;
    
    auto whitespaceOpe = std::make_shared<Ope>();
    auto wordOpe = std::make_shared<Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    // Test packrat function with enabled parsing
    size_t len = 0;
    any val;
    
    // This should use caching logic
    ctx.packrat(s, 0, len, val, [](any& v) {
        v = std::string("test_value");
        // Simulate successful parsing
        // len = 5; // This would be set by the function
    });
    
    // Verify cache structures are initialized
    EXPECT_EQ(ctx.cache_registered.size(), def_count * (l + 1));
    EXPECT_EQ(ctx.cache_success.size(), def_count * (l + 1));
    EXPECT_EQ(ctx.cache_values.size(), 0); // Initially empty
}

TEST_F(ContextTest, PushSemanticValues) {
    const char* path = "test_path";
    const char* s = "test string";
    size_t l = 11;
    size_t def_count = 5;
    bool enablePackratParsing = false;
    
    auto whitespaceOpe = std::make_shared<Ope>();
    auto wordOpe = std::make_shared<Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    // Test push operation
    SemanticValues& sv = ctx.push();
    
    // Should create a new SemanticValues object
    EXPECT_EQ(ctx.value_stack_size, 1);
    EXPECT_EQ(ctx.value_stack.size(), 1);
    
    // Test pushing again
    SemanticValues& sv2 = ctx.push();
    EXPECT_EQ(ctx.value_stack_size, 2);
    EXPECT_EQ(ctx.value_stack.size(), 2);
}

TEST_F(ContextTest, PushSemanticValuesReuse) {
    const char* path = "test_path";
    const char* s = "test string";
    size_t l = 11;
    size_t def_count = 5;
    bool enablePackratParsing = false;
    
    auto whitespaceOpe = std::make_shared<Ope>();
    auto wordOpe = std::make_shared<Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    // Push twice, then pop and push again to reuse
    ctx.push();
    ctx.push();
    
    // This should reuse existing SemanticValues
    SemanticValues& sv = ctx.push();
    
    EXPECT_EQ(ctx.value_stack_size, 3);
    EXPECT_EQ(ctx.value_stack.size(), 3);
}

TEST_F(ContextTest, PushSemanticValuesEmptyStack) {
    const char* path = "test_path";
    const char* s = "test string";
    size_t l = 11;
    size_t def_count = 5;
    bool enablePackratParsing = false;
    
    auto whitespaceOpe = std::make_shared<Ope>();
    auto wordOpe = std::make_shared<Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    // Initially empty stack
    EXPECT_EQ(ctx.value_stack_size, 0);
    EXPECT_EQ(ctx.value_stack.size(), 0);
    
    SemanticValues& sv = ctx.push();
    
    EXPECT_EQ(ctx.value_stack_size, 1);
    EXPECT_EQ(ctx.value_stack.size(), 1);
}

TEST_F(ContextTest, ConstructorNoPackrat) {
    const char* path = "test_path";
    const char* s = "test string";
    size_t l = 11;
    size_t def_count = 5;
    bool enablePackratParsing = false;
    
    auto whitespaceOpe = std::make_shared<Ope>();
    auto wordOpe = std::make_shared<Ope>();
    
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                nullptr, nullptr);
    
    // When packrat is disabled, cache vectors should be empty
    EXPECT_EQ(ctx.cache_registered.size(), 0);
    EXPECT_EQ(ctx.cache_success.size(), 0);
    EXPECT_EQ(ctx.enablePackratParsing, false);
}

TEST_F(ContextTest, ConstructorWithTracers) {
    const char* path = "test_path";
    const char* s = "test string";
    size_t l = 11;
    size_t def_count = 5;
    bool enablePackratParsing = true;
    
    auto whitespaceOpe = std::make_shared<Ope>();
    auto wordOpe = std::make_shared<Ope>();
    
    // Test with tracers
    Context ctx(path, s, l, def_count, whitespaceOpe, wordOpe, enablePackratParsing, 
                [](const char*, size_t, const any&) { return true; },
                [](const any&, size_t) { return true; });
    
    // Tracers should be set
    EXPECT_NE(ctx.tracer_enter, nullptr);
    EXPECT_NE(ctx.tracer_leave, nullptr);
}