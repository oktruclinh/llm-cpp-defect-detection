```cpp
#include <gtest/gtest.h>
#include <peglib.h>

// Helper function to create a basic parser for testing Context.
// The actual grammar doesn't matter much for Context's constructor,
// but we need a valid parser object.
std::shared_ptr<peg::Parser> create_test_parser() {
  auto parser = std::make_shared<peg::Parser>("start = 'a'");
  parser->enable_packrat_parsing(); // Enable packrat parsing for relevant tests
  return parser;
}

// Define a simple tracer function for testing
peg::TracerEnter test_tracer_enter = [](const char *, size_t, const char *,
                                        size_t, const peg::any &) {};
peg::TracerLeave test_tracer_leave = [](const char *, size_t, const char *,
                                        size_t, const peg::any &, size_t) {};

TEST(ContextTest, ConstructorBasic) {
  const char *test_path = "test.txt";
  const char *test_s = "abc\ndef";
  size_t test_l = strlen(test_s);
  size_t def_count = 10; // Arbitrary definition count
  auto parser = create_test_parser();

  // Create a Context object
  peg::Context context(test_path, test_s, test_l, def_count,
                       parser->whitespace_operator(), parser->word_operator(),
                       true, test_tracer_enter, test_tracer_leave);

  // Assert initial state set by the constructor
  ASSERT_EQ(context.path, test_path);
  ASSERT_EQ(context.s, test_s);
  ASSERT_EQ(context.l, test_l);
  ASSERT_EQ(context.def_count, def_count);
  ASSERT_TRUE(context.enablePackratParsing);
  ASSERT_EQ(context.error_pos, nullptr);
  ASSERT_EQ(context.message_pos, nullptr);
  ASSERT_TRUE(context.message.empty());
  ASSERT_EQ(context.value_stack_size, 0);
  ASSERT_EQ(context.rule_stack.size(), 0);
  ASSERT_EQ(context.args_stack.size(), 1); // Should be initialized with one empty vector
  ASSERT_EQ(context.in_token_boundary_count, 0);
  ASSERT_FALSE(context.in_whitespace);
  ASSERT_EQ(context.capture_scope_stack_size, 1); // push_capture_scope() is called
  ASSERT_EQ(context.capture_scope_stack.size(), 1);
  ASSERT_TRUE(context.capture_scope_stack[0].empty());

  // Check cache sizes when packrat parsing is enabled
  ASSERT_EQ(context.cache_registered.size(), def_count * (test_l + 1));
  ASSERT_EQ(context.cache_success.size(), def_count * (test_l + 1));
  ASSERT_TRUE(context.cache_values.empty()); // Should be empty initially
}

TEST(ContextTest, ConstructorNoPackratParsing) {
  const char *test_path = "test.txt";
  const char *test_s = "abc";
  size_t test_l = strlen(test_s);
  size_t def_count = 5;
  auto parser = create_test_parser();

  // Create a Context object with packrat parsing disabled
  peg::Context context(test_path, test_s, test_l, def_count,
                       parser->whitespace_operator(), parser->word_operator(),
                       false, test_tracer_enter, test_tracer_leave);

  ASSERT_FALSE(context.enablePackratParsing);
  // Cache vectors should be empty if packrat parsing is disabled
  ASSERT_TRUE(context.cache_registered.empty());
  ASSERT_TRUE(context.cache_success.empty());
  ASSERT_TRUE(context.cache_values.empty());
}

TEST(ContextTest, ConstructorEmptyString) {
  const char *test_path = "empty.txt";
  const char *test_s = "";
  size_t test_l = strlen(test_s);
  size_t def_count = 1;
  auto parser = create_test_parser();

  peg::Context context(test_path, test_s, test_l, def_count,
                       parser->whitespace_operator(), parser->word_operator(),
                       true, test_tracer_enter, test_tracer_leave);

  ASSERT_EQ(context.s, test_s);
  ASSERT_EQ(context.l, test_l);
  ASSERT_EQ(context.source_line_index.size(), 1); // Always contains 'l'
  ASSERT_EQ(context.source_line_index[0], test_l);
  ASSERT_EQ(context.cache_registered.size(), def_count * (test_l + 1)); // 1 * (0 + 1) = 1
}

TEST(ContextTest, ConstructorStringWithMultipleNewlines) {
  const char *test_path = "multi_line.txt";
  const char *test_s = "line1\nline2\nline3";
  size_t test_l = strlen(test_s);
  size_t def_count = 2;
  auto parser = create_test_parser();

  peg::Context context(test_path, test_s, test_l, def_count,
                       parser->whitespace_operator(), parser->word_operator(),
                       true, test_tracer_enter, test_tracer_leave);

  // Check source_line_index for correct newline positions
  ASSERT_EQ(context.source_line_index.size(), 3); // Two newlines + 'l'
  ASSERT_EQ(context.source_line_index[0], 5);     // Position of first '\n'
  ASSERT_EQ(context.source_line_index[1], 11);    // Position of second '\n'
  ASSERT_EQ(context.source_line_index[2], test_l); // End of string
}

TEST(ContextTest, ConstructorStringEndingWithNewline) {
  const char *test_path = "ends_newline.txt";
  const char *test_s = "line1\n";
  size_t test_l = strlen(test_s);
  size_t def_count = 2;
  auto parser = create_test_parser();

  peg::Context context(test_path, test_s, test_l, def_count,
                       parser->whitespace_operator(), parser->word_operator(),
                       true, test_tracer_enter, test_tracer_leave);

  ASSERT_EQ(context.source_line_index.size(), 2); // One newline + 'l'
  ASSERT_EQ(context.source_line_index[0], 5);     // Position of '\n'
  ASSERT_EQ(context.source_line_index[1], test_l); // End of string
}

TEST(ContextTest, ConstructorZeroDefCount) {
  const char *test_path = "zero_def.txt";
  const char *test_s = "test";
  size_t test_l = strlen(test_s);
  size_t def_count = 0; // Edge case: zero definitions
  auto parser = create_test_parser();

  peg::Context context(test_path, test_s, test_l, def_count,
                       parser->whitespace_operator(), parser->word_operator(),
                       true, test_tracer_enter, test_tracer_leave);

  ASSERT_EQ(context.def_count, 0);
  // Cache sizes should still be calculated as def_count * (l + 1)
  // which is 0 * (4 + 1) = 0
  ASSERT_TRUE(context.cache_registered.empty());
  ASSERT_TRUE(context.cache_success.empty());
}

// Test the packrat function's caching behavior
TEST(ContextTest, PackratFunctionCachingSuccess) {
  const char *test_s = "abc";
  size_t test_l = strlen(test_s);
  size_t def_count = 1;
  auto parser = create_test_parser();

  peg::Context context("test.txt", test_s, test_l, def_count,
                       parser->whitespace_operator(), parser->word_operator(),
                       true, test_tracer_enter, test_tracer_leave);

  size_t len = 0;
  peg::any val;
  const char *current_s = test_s;
  size_t def_id = 0;

  // First call: should execute fn and cache the result
  bool fn_called = false;
  context.packrat(current_s, def_id, len, val,
                  [&](peg::any &v) {
                    fn_called = true;
                    len = 1; // Simulate matching 'a'
                    v = std::string("matched_a");
                  });

  ASSERT_TRUE(fn_called);
  ASSERT_EQ(len, 1);
  ASSERT_EQ(peg::any_cast<std::string>(val), "matched_a");

  // Verify cache state
  auto col = current_s - test_s;
  auto idx = def_count * static_cast<size_t>(col) + def_id;
  ASSERT_TRUE(context.cache_registered[idx]);
  ASSERT_TRUE(context.cache_success[idx]);
  auto key = std::make_pair(col, def_id);
  ASSERT_TRUE(context.cache_values.count(key));
  ASSERT_EQ(std::get<0>(context.cache_values[key]), 1);
  ASSERT_EQ(peg::any_cast<std::string>(std::get<1>(context.cache_values[key])),
            "matched_a");

  // Second call: should use cached result, fn should NOT be called
  fn_called = false;
  len = 0; // Reset for second call
  val = peg::any();
  context.packrat(current_s, def_id, len, val,
                  [&](peg::any &v) {
                    fn_called = true;
                    len = 99; // If this is set, fn was called
                    v = std::string("wrong_value");
                  });

  ASSERT_FALSE(fn_called); // Should use cache
  ASSERT_EQ(len, 1);
  ASSERT_EQ(peg::any_cast<std::string>(val), "matched_a");
}

TEST(ContextTest, PackratFunctionCachingFailure) {
  const char *test_s = "abc";
  size_t test_l = strlen(test_s);
  size_t def_count = 1;
  auto parser = create_test_parser();

  peg::Context context("test.txt", test_s, test_l, def_count,
                       parser->whitespace_operator(), parser->word_operator(),
                       true, test_tracer_enter, test_tracer_leave);

  size_t len = 0;
  peg::any val;
  const char *current_s = test_s;
  size_t def_id = 0;

  // First call: simulate a parse failure
  bool fn_called = false;
  context.packrat(current_s, def_id, len, val,
                  [&](peg::any &v) {
                    fn_called = true;
                    len = static_cast<size_t>(-1); // Simulate failure
                    v = peg::any();
                  });

  ASSERT_TRUE(fn_called);
  ASSERT_EQ(len, static_cast<size_t>(-1));
  ASSERT_TRUE(val.empty());

  // Verify cache state for failure
  auto col = current_s - test_s;
  auto idx = def_count * static_cast<size_t>(col) + def_id;
  ASSERT_TRUE(context.cache_registered[idx]);
  ASSERT_FALSE(context.cache_success[idx]); // Should be false for failure
  ASSERT_FALSE(context.cache_values.count(std::make_pair(col, def_id))); // No value cached on failure

  // Second call: should use cached failure, fn should NOT be called
  fn_called = false;
  len = 0; // Reset
  val = peg::any();
  context.packrat(current_s, def_id, len, val,
                  [&](peg::any &v) {
                    fn_called = true;
                    len = 1; // If this is set, fn was called
                    v = std::string("should_not_be_set");
                  });

  ASSERT_FALSE(fn_called); // Should use cache
  ASSERT_EQ(len, static_cast<size_t>(-1));
  ASSERT_TRUE(val.empty());
}

TEST(ContextTest, PackratFunctionNoPackratParsing) {
  const char *test_s = "abc";
  size_t test_l = strlen(test_s);
  size_t def_count = 1;
  auto parser = create_test_parser();

  // Disable packrat parsing
  peg::Context context("test.txt", test_s, test_l, def_count,
                       parser->whitespace_operator(), parser->word_operator(),
                       false, test_tracer_enter, test_tracer_leave);

  size_t len = 0;
  peg::any val;
  const char *current_s = test_s;
  size_t def_id = 0;

  // Call packrat: fn should always be called, no caching
  bool fn_called_first = false;
  context.packrat(current_s, def_id, len, val,
                  [&](peg::any &v) {
                    fn_called_first = true;
                    len = 1;
                    v = std::string("result1");
                  });

  ASSERT_TRUE(fn_called_first);
  ASSERT_EQ(len, 1);
  ASSERT_EQ(peg::any_cast<std::string>(val), "result1");

  // Verify no caching happened
  ASSERT_TRUE(context.cache_registered.empty());
  ASSERT_TRUE(context.cache_success.empty());
  ASSERT_TRUE(context.cache_values.empty());

  // Call again: fn should be called again
  bool fn_called_second = false;
  len = 0;
  val = peg::any();
  context.packrat(current_s, def_id, len, val,
                  [&](peg::any &v) {
                    fn_called_second = true;
                    len = 2; // Different result
                    v = std::string("result2");
                  });

  ASSERT_TRUE(fn_called_second);
  ASSERT_EQ(len, 2);
  ASSERT_EQ(peg::any_cast<std::string>(val), "result2");
}

TEST(ContextTest, PackratFunctionDifferentPositionsAndDefs) {
  const char *test_s = "abcde";
  size_t test_l = strlen(test_s);
  size_t def_count = 2; // Two definitions
  auto parser = create_test_parser();

  peg::Context context("test.txt", test_s, test_l, def_count,
                       parser->whitespace_operator(), parser->word_operator(),
                       true, test_tracer_enter, test_tracer_leave);

  size_t len1 = 0, len2 = 0, len3 = 0;
  peg::any val1, val2, val3;

  // Call 1: def_id 0 at position 0 ('a')
  context.packrat(test_s, 0, len1, val1,
                  [&](peg::any &v) {
                    len1 = 1;
                    v = std::string("A");
                  });
  ASSERT_EQ(len1, 1);
  ASSERT_EQ(peg::any_cast<std::string>(val1), "A");

  // Call 2: def_id 1 at position 0 ('a')
  context.packrat(test_s, 1, len2, val2,
                  [&](peg::any &v) {
                    len2 = 2;
                    v = std::string("AB");
                  });
  ASSERT_EQ(len2, 2);
  ASSERT_EQ(peg::any_cast<std::string>(val2), "AB");

  // Call 3: def_id 0 at position 1 ('b')
  context.packrat(test_s + 1, 0, len3, val3,
                  [&](peg::any &v) {
                    len3 = 1;
                    v = std::string("B");
                  });
  ASSERT_EQ(len3, 1);
  ASSERT_EQ(peg::any_cast<std::string>(val3), "B");

  // Verify cache entries are distinct
  auto col0_def0_idx = def_count * 0 + 0;
  auto col0_def1_idx = def_count * 0 + 1;
  auto col1_def0_idx = def_count * 1 + 0;

  ASSERT_TRUE(context.cache