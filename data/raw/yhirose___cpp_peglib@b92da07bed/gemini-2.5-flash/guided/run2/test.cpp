```cpp
#include <gtest/gtest.h>
#include <peglib.h>

// Helper function to create a basic parser for testing Context.
// This parser will recognize a single character 'a'.
std::shared_ptr<peg::Parser> create_simple_parser() {
  auto parser = std::make_shared<peg::Parser>();
  parser->setStartRule("A");
  parser->setGrammar(R"(
        A <- 'a'
    )");
  return parser;
}

// Test fixture for Context related tests
class ContextTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Initialize common resources for tests
    test_path = "test_file.txt";
    test_s = "abc\ndefg\nhijkl";
    test_l = strlen(test_s);
    def_count = 1; // For a simple grammar, assume 1 definition
    whitespace_ope = nullptr;
    word_ope = nullptr;
    enable_packrat_parsing = false;
    tracer_enter = [](const char *, size_t, const peg::any &) {};
    tracer_leave = [](const char *, size_t, const peg::any &, size_t) {};
  }

  const char *test_path;
  const char *test_s;
  size_t test_l;
  size_t def_count;
  std::shared_ptr<peg::Ope> whitespace_ope;
  std::shared_ptr<peg::Ope> word_ope;
  bool enable_packrat_parsing;
  peg::TracerEnter tracer_enter;
  peg::TracerLeave tracer_leave;
};

TEST_F(ContextTest, ConstructorInitializesMembersCorrectly) {
  peg::Context ctx(test_path, test_s, test_l, def_count, whitespace_ope,
                   word_ope, enable_packrat_parsing, tracer_enter,
                   tracer_leave);

  ASSERT_EQ(ctx.path, test_path);
  ASSERT_EQ(ctx.s, test_s);
  ASSERT_EQ(ctx.l, test_l);
  ASSERT_EQ(ctx.def_count, def_count);
  ASSERT_EQ(ctx.whitespaceOpe, whitespace_ope);
  ASSERT_EQ(ctx.wordOpe, word_ope);
  ASSERT_EQ(ctx.enablePackratParsing, enable_packrat_parsing);
  // Tracers are function objects, direct comparison might not work,
  // but we can assume they are set if the constructor doesn't throw.

  ASSERT_EQ(ctx.error_pos, nullptr);
  ASSERT_EQ(ctx.message_pos, nullptr);
  ASSERT_TRUE(ctx.message.empty());

  ASSERT_EQ(ctx.value_stack_size, 0);
  ASSERT_TRUE(ctx.rule_stack.empty());
  ASSERT_EQ(ctx.args_stack.size(), 1); // Initialized with one empty vector

  ASSERT_EQ(ctx.in_token_boundary_count, 0);
  ASSERT_FALSE(ctx.in_whitespace);

  ASSERT_EQ(ctx.capture_scope_stack_size, 1); // push_capture_scope() called
  ASSERT_EQ(ctx.capture_scope_stack.size(), 1);
  ASSERT_TRUE(ctx.capture_scope_stack[0].empty());

  ASSERT_TRUE(ctx.cache_registered.empty()); // Packrat parsing disabled
  ASSERT_TRUE(ctx.cache_success.empty());    // Packrat parsing disabled
  ASSERT_TRUE(ctx.cache_values.empty());
}

TEST_F(ContextTest, ConstructorInitializesSourceLineIndexCorrectly) {
  peg::Context ctx(test_path, test_s, test_l, def_count, whitespace_ope,
                   word_ope, enable_packrat_parsing, tracer_enter,
                   tracer_leave);

  // Input: "abc\ndefg\nhijkl"
  // Newlines at index 3 and 8
  // Expected source_line_index: [3, 8, 13] (l is 13)
  std::vector<size_t> expected_line_indices = {3, 8, 13};
  ASSERT_EQ(ctx.source_line_index, expected_line_indices);
}

TEST_F(ContextTest, ConstructorWithEmptyString) {
  const char *empty_s = "";
  size_t empty_l = 0;
  peg::Context ctx(test_path, empty_s, empty_l, def_count, whitespace_ope,
                   word_ope, enable_packrat_parsing, tracer_enter,
                   tracer_leave);

  ASSERT_EQ(ctx.s, empty_s);
  ASSERT_EQ(ctx.l, empty_l);
  // For an empty string, source_line_index should contain only the length (0)
  std::vector<size_t> expected_line_indices = {0};
  ASSERT_EQ(ctx.source_line_index, expected_line_indices);
}

TEST_F(ContextTest, ConstructorWithNoNewlines) {
  const char *no_newline_s = "abcdefg";
  size_t no_newline_l = strlen(no_newline_s);
  peg::Context ctx(test_path, no_newline_s, no_newline_l, def_count,
                   whitespace_ope, word_ope, enable_packrat_parsing,
                   tracer_enter, tracer_leave);

  // For a string with no newlines, source_line_index should contain only the length
  std::vector<size_t> expected_line_indices = {no_newline_l};
  ASSERT_EQ(ctx.source_line_index, expected_line_indices);
}

TEST_F(ContextTest, ConstructorWithPackratParsingEnabled) {
  enable_packrat_parsing = true;
  peg::Context ctx(test_path, test_s, test_l, def_count, whitespace_ope,
                   word_ope, enable_packrat_parsing, tracer_enter,
                   tracer_leave);

  ASSERT_TRUE(ctx.enablePackratParsing);
  // cache_registered and cache_success should be sized def_count * (l + 1)
  ASSERT_EQ(ctx.cache_registered.size(), def_count * (test_l + 1));
  ASSERT_EQ(ctx.cache_success.size(), def_count * (test_l + 1));
  // All cache entries should be false initially
  for (size_t i = 0; i < ctx.cache_registered.size(); ++i) {
    ASSERT_FALSE(ctx.cache_registered[i]);
    ASSERT_FALSE(ctx.cache_success[i]);
  }
  ASSERT_TRUE(ctx.cache_values.empty());
}

TEST_F(ContextTest, PackratFunctionality_NoPackratParsing) {
  peg::Context ctx(test_path, test_s, test_l, def_count, whitespace_ope,
                   word_ope, false, tracer_enter, tracer_leave);

  size_t len = 0;
  peg::any val;
  bool fn_called = false;

  // The lambda function should always be called if packrat parsing is disabled
  ctx.packrat(test_s, 0, len, val, [&](peg::any &v) {
    fn_called = true;
    len = 1; // Simulate a successful parse
    v = 'a';
  });

  ASSERT_TRUE(fn_called);
  ASSERT_EQ(len, 1);
  ASSERT_EQ(peg::any_cast<char>(val), 'a');

  // Cache should remain empty
  ASSERT_TRUE(ctx.cache_registered.empty());
  ASSERT_TRUE(ctx.cache_success.empty());
  ASSERT_TRUE(ctx.cache_values.empty());
}

TEST_F(ContextTest, PackratFunctionality_FirstCallSuccess) {
  enable_packrat_parsing = true;
  peg::Context ctx(test_path, test_s, test_l, def_count, whitespace_ope,
                   word_ope, enable_packrat_parsing, tracer_enter,
                   tracer_leave);

  size_t len = 0;
  peg::any val;
  bool fn_called = false;
  const char *current_pos = test_s;
  size_t def_id = 0;
  size_t col = current_pos - test_s;
  size_t idx = def_count * col + def_id;

  ctx.packrat(current_pos, def_id, len, val, [&](peg::any &v) {
    fn_called = true;
    len = 1; // Simulate a successful parse
    v = 'a';
  });

  ASSERT_TRUE(fn_called);
  ASSERT_EQ(len, 1);
  ASSERT_EQ(peg::any_cast<char>(val), 'a');

  ASSERT_TRUE(ctx.cache_registered[idx]);
  ASSERT_TRUE(ctx.cache_success[idx]);
  ASSERT_FALSE(ctx.cache_values.empty());
  ASSERT_EQ(peg::any_cast<char>(std::get<1>(ctx.cache_values[{col, def_id}])),
            'a');
  ASSERT_EQ(std::get<0>(ctx.cache_values[{col, def_id}]), 1);
}

TEST_F(ContextTest, PackratFunctionality_SecondCallHitSuccess) {
  enable_packrat_parsing = true;
  peg::Context ctx(test_path, test_s, test_l, def_count, whitespace_ope,
                   word_ope, enable_packrat_parsing, tracer_enter,
                   tracer_leave);

  size_t len1 = 0;
  peg::any val1;
  const char *current_pos = test_s;
  size_t def_id = 0;
  size_t col = current_pos - test_s;
  size_t idx = def_count * col + def_id;

  // First call: populate cache
  ctx.packrat(current_pos, def_id, len1, val1, [&](peg::any &v) {
    len1 = 1;
    v = 'a';
  });

  ASSERT_EQ(len1, 1);
  ASSERT_EQ(peg::any_cast<char>(val1), 'a');
  ASSERT_TRUE(ctx.cache_registered[idx]);
  ASSERT_TRUE(ctx.cache_success[idx]);

  size_t len2 = 0;
  peg::any val2;
  bool fn_called_second_time = false;

  // Second call: should hit cache, fn should NOT be called
  ctx.packrat(current_pos, def_id, len2, val2, [&](peg::any &v) {
    fn_called_second_time = true;
    len2 = 99; // If this is set, it means fn was called
    v = 'z';
  });

  ASSERT_FALSE(fn_called_second_time); // Crucial assertion for cache hit
  ASSERT_EQ(len2, 1);                  // Should retrieve from cache
  ASSERT_EQ(peg::any_cast<char>(val2), 'a'); // Should retrieve from cache
}

TEST_F(ContextTest, PackratFunctionality_FirstCallFailure) {
  enable_packrat_parsing = true;
  peg::Context ctx(test_path, test_s, test_l, def_count, whitespace_ope,
                   wordOpe, enable_packrat_parsing, tracer_enter,
                   tracer_leave);

  size_t len = 0;
  peg::any val;
  bool fn_called = false;
  const char *current_pos = test_s;
  size_t def_id = 0;
  size_t col = current_pos - test_s;
  size_t idx = def_count * col + def_id;

  ctx.packrat(current_pos, def_id, len, val, [&](peg::any &v) {
    fn_called = true;
    len = static_cast<size_t>(-1); // Simulate a failed parse
    v = peg::any();                // Value should be default for failure
  });

  ASSERT_TRUE(fn_called);
  ASSERT_EQ(len, static_cast<size_t>(-1));
  ASSERT_TRUE(val.empty()); // Value should be empty for a failed parse

  ASSERT_TRUE(ctx.cache_registered[idx]);
  ASSERT_FALSE(ctx.cache_success[idx]); // Cache should record failure
  ASSERT_TRUE(ctx.cache_values.empty()); // No value stored for failure
}

TEST_F(ContextTest, PackratFunctionality_SecondCallHitFailure) {
  enable_packrat_parsing = true;
  peg::Context ctx(test_path, test_s, test_l, def_count, whitespace_ope,
                   word_ope, enable_packrat_parsing, tracer_enter,
                   tracer_leave);

  size_t len1 = 0;
  peg::any val1;
  const char *current_pos = test_s;
  size_t def_id = 0;
  size_t col = current_pos - test_s;
  size_t idx = def_count * col + def_id;

  // First call: populate cache with failure
  ctx.packrat(current_pos, def_id, len1, val1, [&](peg::any &v) {
    len1 = static_cast<size_t>(-1);
    v = peg::any();
  });

  ASSERT_EQ(len1, static_cast<size_t>(-1));
  ASSERT_TRUE(val1.empty());
  ASSERT_TRUE(ctx.cache_registered[idx]);
  ASSERT_FALSE(ctx.cache_success[idx]);

  size_t len2 = 0;
  peg::any val2;
  bool fn_called_second_time = false;

  // Second call: should hit cache, fn should NOT be called
  ctx.packrat(current_pos, def_id, len2, val2, [&](peg::any &v) {
    fn_called_second_time = true;
    len2 = 99; // If this is set, it means fn was called
    v = 'z';
  });

  ASSERT_FALSE(fn_called_second_time); // Crucial assertion for cache hit
  ASSERT_EQ(len2, static_cast<size_t>(-1)); // Should retrieve failure from cache
  ASSERT_TRUE(val2.empty());                // Value should be empty
}

TEST_F(ContextTest, PackratFunctionality_DifferentPositionsAndDefIds) {
  enable_packrat_parsing = true;
  def_count = 2; // Assume two definitions for this test
  peg::Context ctx(test_path, test_s, test_l, def_count, whitespace_ope,
                   word_ope, enable_packrat_parsing, tracer_enter,
                   tracer_leave);

  // Test position 0, def_id 0
  size_t len00 = 0;
  peg::any val00;
  ctx.packrat(test_s, 0, len00, val00, [&](peg::any &v) {
    len00 = 1;
    v = 'A';
  });
  ASSERT_EQ(len00, 1);
  ASSERT_EQ(peg::any_cast<char>(val00), 'A');

  // Test position 0, def_id 1
  size_t len01 = 0;
  peg::any val01;
  ctx.packrat(test_s, 1, len01, val01, [&](peg::any &v) {
    len01 = 2;
    v = "AB";
  });
  ASSERT_EQ(len01, 2);
  ASSERT_EQ(peg::any_cast<const char *>(val01), std::string("AB"));

  // Test position 1, def_id 0
  size_t len10 = 0;
  peg::any val10;
  ctx.packrat(test_s + 1, 0, len10, val10, [&](peg::any &v) {
    len10 = static_cast<size_t>(-1); // Failure
  });
  ASSERT_EQ(len10, static_cast<size_t>(-1));
  ASSERT_TRUE(val10.empty());

  // Verify cache states
  ASSERT_TRUE(ctx.cache_registered[def_count * 0 + 0]);
  ASSERT_TRUE(ctx.cache_success[def_count * 0 + 0]);
  ASSERT_TRUE(ctx.cache_registered[def_count * 0 + 1]);
  ASSERT_TRUE(ctx.cache_success[def_count * 0 + 1]);
  ASSERT_TRUE(ctx.cache_registered[def_count * 1 + 0]);
  ASSERT_FALSE(ctx.cache_success[def_count * 1 + 0]);

  // Re-test to ensure cache hits
  size_t len_