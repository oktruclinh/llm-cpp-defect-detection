```cpp
#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "arrow/array.h"
#include "arrow/array/builder_primitive.h"
#include "arrow/chunked_array.h"
#include "arrow/record_batch.h"
#include "arrow/table.h"
#include "arrow/table_builder.h"
#include "arrow/type.h"
#include "arrow/type_fwd.h"
#include "arrow/util/checked_cast.h"
#include "arrow/util/key_value_metadata.h"

namespace arrow {

using internal::checked_cast;

// Helper function to create a simple Int32Array
Result<std::shared_ptr<Array>> MakeInt32Array(const std::vector<int32_t>& values) {
  Int32Builder builder;
  RETURN_NOT_OK(builder.AppendValues(values));
  return builder.Finish();
}

// Helper function to create a simple StringArray
Result<std::shared_ptr<Array>> MakeStringArray(const std::vector<std::string>& values) {
  StringBuilder builder;
  for (const auto& val : values) {
    RETURN_NOT_OK(builder.Append(val));
  }
  return builder.Finish();
}

// Test fixture for TableBuilder
class TableBuilderTest : public ::testing::Test {
 protected:
  void SetUp() override {
    pool_ = default_memory_pool();
  }

  MemoryPool* pool_;
};

// Test case for a basic table with primitive types
TEST_F(TableBuilderTest, BasicTable) {
  auto f0 = field("f0", int32());
  auto f1 = field("f1", utf8());
  auto schema = arrow::schema({f0, f1});

  TableBuilder builder(pool_, schema);

  // Add some data
  std::shared_ptr<Array> arr0_0, arr0_1;
  ASSERT_OK_AND_ASSIGN(arr0_0, MakeInt32Array({1, 2, 3}));
  ASSERT_OK_AND_ASSIGN(arr0_1, MakeStringArray({"a", "b", "c"}));
  ASSERT_OK(builder.AppendChunk(0, arr0_0));
  ASSERT_OK(builder.AppendChunk(1, arr0_1));

  std::shared_ptr<Array> arr1_0, arr1_1;
  ASSERT_OK_AND_ASSIGN(arr1_0, MakeInt32Array({4, 5}));
  ASSERT_OK_AND_ASSIGN(arr1_1, MakeStringArray({"d", "e"}));
  ASSERT_OK(builder.AppendChunk(0, arr1_0));
  ASSERT_OK(builder.AppendChunk(1, arr1_1));

  // Build the table
  std::shared_ptr<Table> table;
  ASSERT_OK_AND_ASSIGN(table, builder.Finish());

  // Verify the table
  ASSERT_EQ(table->num_rows(), 5);
  ASSERT_EQ(table->num_columns(), 2);
  ASSERT_TRUE(table->schema()->Equals(*schema));

  // Verify column 0
  ASSERT_EQ(table->column(0)->num_chunks(), 2);
  ASSERT_TRUE(table->column(0)->chunk(0)->Equals(*arr0_0));
  ASSERT_TRUE(table->column(0)->chunk(1)->Equals(*arr1_0));

  // Verify column 1
  ASSERT_EQ(table->column(1)->num_chunks(), 2);
  ASSERT_TRUE(table->column(1)->chunk(0)->Equals(*arr0_1));
  ASSERT_TRUE(table->column(1)->chunk(1)->Equals(*arr1_1));
}

// Test case for an empty table
TEST_F(TableBuilderTest, EmptyTable) {
  auto f0 = field("f0", int32());
  auto schema = arrow::schema({f0});

  TableBuilder builder(pool_, schema);
  std::shared_ptr<Table> table;
  ASSERT_OK_AND_ASSIGN(table, builder.Finish());

  ASSERT_EQ(table->num_rows(), 0);
  ASSERT_EQ(table->num_columns(), 1);
  ASSERT_TRUE(table->schema()->Equals(*schema));
  ASSERT_EQ(table->column(0)->num_chunks(), 0);
}

// Test case with multiple chunks for a single column
TEST_F(TableBuilderTest, MultipleChunksSingleColumn) {
  auto f0 = field("f0", int32());
  auto schema = arrow::schema({f0});

  TableBuilder builder(pool_, schema);

  std::shared_ptr<Array> arr0, arr1, arr2;
  ASSERT_OK_AND_ASSIGN(arr0, MakeInt32Array({1, 2}));
  ASSERT_OK_AND_ASSIGN(arr1, MakeInt32Array({3, 4, 5}));
  ASSERT_OK_AND_ASSIGN(arr2, MakeInt32Array({6}));

  ASSERT_OK(builder.AppendChunk(0, arr0));
  ASSERT_OK(builder.AppendChunk(0, arr1));
  ASSERT_OK(builder.AppendChunk(0, arr2));

  std::shared_ptr<Table> table;
  ASSERT_OK_AND_ASSIGN(table, builder.Finish());

  ASSERT_EQ(table->num_rows(), 6);
  ASSERT_EQ(table->num_columns(), 1);
  ASSERT_TRUE(table->schema()->Equals(*schema));

  ASSERT_EQ(table->column(0)->num_chunks(), 3);
  ASSERT_TRUE(table->column(0)->chunk(0)->Equals(*arr0));
  ASSERT_TRUE(table->column(0)->chunk(1)->Equals(*arr1));
  ASSERT_TRUE(table->column(0)->chunk(2)->Equals(*arr2));
}

// Test case with different chunk lengths across columns for the same "row group"
TEST_F(TableBuilderTest, MismatchedChunkLengthsInRowGroup) {
  auto f0 = field("f0", int32());
  auto f1 = field("f1", utf8());
  auto schema = arrow::schema({f0, f1});

  TableBuilder builder(pool_, schema);

  std::shared_ptr<Array> arr0_0, arr0_1;
  ASSERT_OK_AND_ASSIGN(arr0_0, MakeInt32Array({1, 2, 3}));
  ASSERT_OK_AND_ASSIGN(arr0_1, MakeStringArray({"a", "b"})); // Mismatched length

  // This should return an error because chunk lengths don't match for the same "row group"
  ASSERT_OK(builder.AppendChunk(0, arr0_0));
  Status s = builder.AppendChunk(1, arr0_1);
  ASSERT_TRUE(s.IsInvalid());
  ASSERT_THAT(s.message(), ::testing::HasSubstr("Mismatching chunk length"));

  // Try to finish, should also fail
  std::shared_ptr<Table> table;
  s = builder.Finish();
  ASSERT_TRUE(s.IsInvalid());
  ASSERT_THAT(s.message(), ::testing::HasSubstr("Mismatching chunk length"));
}

// Test case with dictionary type fields (ARROW-9969 related)
TEST_F(TableBuilderTest, DictionaryTypeFields) {
  auto dict_type = dictionary(int8(), utf8());
  auto f0 = field("f0", dict_type);
  auto schema = arrow::schema({f0});

  TableBuilder builder(pool_, schema);

  // First chunk
  std::shared_ptr<Array> dict_indices0;
  ASSERT_OK_AND_ASSIGN(dict_indices0, MakeInt32Array({0, 1, 0}));
  std::shared_ptr<Array> dict_values0;
  ASSERT_OK_AND_ASSIGN(dict_values0, MakeStringArray({"red", "blue"}));
  auto dict_array0 = std::make_shared<DictionaryArray>(dict_type, dict_indices0, dict_values0);
  ASSERT_OK(builder.AppendChunk(0, dict_array0));

  // Second chunk with different dictionary values
  std::shared_ptr<Array> dict_indices1;
  ASSERT_OK_AND_ASSIGN(dict_indices1, MakeInt32Array({0, 1, 0, 2}));
  std::shared_ptr<Array> dict_values1;
  ASSERT_OK_AND_ASSIGN(dict_values1, MakeStringArray({"green", "yellow", "orange"}));
  auto dict_array1 = std::make_shared<DictionaryArray>(dict_type, dict_indices1, dict_values1);
  ASSERT_OK(builder.AppendChunk(0, dict_array1));

  std::shared_ptr<Table> table;
  ASSERT_OK_AND_ASSIGN(table, builder.Finish());

  ASSERT_EQ(table->num_rows(), 7); // 3 + 4
  ASSERT_EQ(table->num_columns(), 1);
  ASSERT_TRUE(table->schema()->Equals(*schema)); // Schema should still be dict_type

  // The dictionary values should be merged and the indices remapped
  auto column = checked_cast<const ChunkedArray&>(*table->column(0));
  ASSERT_EQ(column.num_chunks(), 2);

  // Verify the first chunk's type and data
  auto chunk0 = checked_cast<const DictionaryArray&>(*column.chunk(0));
  ASSERT_TRUE(chunk0.type()->Equals(*dict_type));
  ASSERT_EQ(chunk0.length(), 3);
  ASSERT_EQ(chunk0.dictionary()->length(), 5); // red, blue, green, yellow, orange
  ASSERT_EQ(chunk0.indices()->length(), 3);

  // Verify the second chunk's type and data
  auto chunk1 = checked_cast<const DictionaryArray&>(*column.chunk(1));
  ASSERT_TRUE(chunk1.type()->Equals(*dict_type));
  ASSERT_EQ(chunk1.length(), 4);
  ASSERT_EQ(chunk1.dictionary()->length(), 5); // red, blue, green, yellow, orange
  ASSERT_EQ(chunk1.indices()->length(), 4);

  // Check some values to ensure remapping worked
  // Original: {0:"red", 1:"blue", 0:"red"}
  // Merged: {0:"red", 1:"blue", 2:"green", 3:"yellow", 4:"orange"}
  // Expected indices for first chunk: {0, 1, 0}
  auto expected_indices0 = ArrayFromVector<int8_t>({0, 1, 0}).ValueOrDie();
  ASSERT_TRUE(chunk0.indices()->Equals(*expected_indices0));

  // Original: {0:"green", 1:"yellow", 0:"green", 2:"orange"}
  // Expected indices for second chunk: {2, 3, 2, 4}
  auto expected_indices1 = ArrayFromVector<int8_t>({2, 3, 2, 4}).ValueOrDie();
  ASSERT_TRUE(chunk1.indices()->Equals(*expected_indices1));
}

// Test case for appending a record batch
TEST_F(TableBuilderTest, AppendRecordBatch) {
  auto f0 = field("f0", int32());
  auto f1 = field("f1", utf8());
  auto schema = arrow::schema({f0, f1});

  TableBuilder builder(pool_, schema);

  std::shared_ptr<Array> arr0_0, arr0_1;
  ASSERT_OK_AND_ASSIGN(arr0_0, MakeInt32Array({1, 2}));
  ASSERT_OK_AND_ASSIGN(arr0_1, MakeStringArray({"a", "b"}));
  auto batch0 = RecordBatch::Make(schema, 2, {arr0_0, arr0_1});
  ASSERT_OK(builder.AppendRecordBatch(*batch0));

  std::shared_ptr<Array> arr1_0, arr1_1;
  ASSERT_OK_AND_ASSIGN(arr1_0, MakeInt32Array({3, 4, 5}));
  ASSERT_OK_AND_ASSIGN(arr1_1, MakeStringArray({"c", "d", "e"}));
  auto batch1 = RecordBatch::Make(schema, 3, {arr1_0, arr1_1});
  ASSERT_OK(builder.AppendRecordBatch(*batch1));

  std::shared_ptr<Table> table;
  ASSERT_OK_AND_ASSIGN(table, builder.Finish());

  ASSERT_EQ(table->num_rows(), 5);
  ASSERT_EQ(table->num_columns(), 2);
  ASSERT_TRUE(table->schema()->Equals(*schema));

  ASSERT_EQ(table->column(0)->num_chunks(), 2);
  ASSERT_TRUE(table->column(0)->chunk(0)->Equals(*arr0_0));
  ASSERT_TRUE(table->column(0)->chunk(1)->Equals(*arr1_0));

  ASSERT_EQ(table->column(1)->num_chunks(), 2);
  ASSERT_TRUE(table->column(1)->chunk(0)->Equals(*arr0_1));
  ASSERT_TRUE(table->column(1)->chunk(1)->Equals(*arr1_1));
}

// Test case for appending a record batch with schema mismatch
TEST_F(TableBuilderTest, AppendRecordBatchSchemaMismatch) {
  auto f0 = field("f0", int32());
  auto f1 = field("f1", utf8());
  auto schema = arrow::schema({f0, f1});

  TableBuilder builder(pool_, schema);

  auto f2 = field("f2", float64());
  auto mismatched_schema = arrow::schema({f0, f2});

  std::shared_ptr<Array> arr0_0, arr0_1;
  ASSERT_OK_AND_ASSIGN(arr0_0, MakeInt32Array({1, 2}));
  DoubleBuilder double_builder;
  ASSERT_OK(double_builder.AppendValues({1.0, 2.0}));
  ASSERT_OK_AND_ASSIGN(arr0_1, double_builder.Finish());
  auto batch = RecordBatch::Make(mismatched_schema, 2, {arr0_0, arr0_1});

  Status s = builder.AppendRecordBatch(*batch);
  ASSERT_TRUE(s.IsInvalid());
  ASSERT_THAT(s.message(), ::testing::HasSubstr("Schemas are not equal"));
}

// Test case for appending a record batch with different number of columns
TEST_F(TableBuilderTest, AppendRecordBatchColumnCountMismatch) {
  auto f0 = field("f0", int32());
  auto f1 = field("f1", utf8());
  auto schema = arrow::schema({f0, f1});

  TableBuilder builder(pool_, schema);

  auto single_column_schema = arrow::schema({f0});
  std::shared_ptr<Array> arr0_0;
  ASSERT_OK_AND_ASSIGN(arr0_0, MakeInt32Array({1, 2}));
  auto batch = RecordBatch::Make(single_column_schema, 2, {arr0_0});

  Status s = builder.AppendRecordBatch(*batch);
  ASSERT_TRUE(s.IsInvalid());
  ASSERT_THAT(s.message(), ::testing::HasSubstr("RecordBatch has 1 columns but builder expects 2"));
}

// Test case for appending a record batch with different number of rows
TEST_F(TableBuilderTest, AppendRecordBatchRowCountMismatch) {
  auto f0 = field("f0", int32());
  auto f1 = field("f1", utf8());
  auto schema = arrow::schema({f0, f1});

  TableBuilder builder(pool_, schema);

  std::shared_ptr<Array> arr0_0, arr0_1;
  ASSERT_OK_AND_ASSIGN(arr0_0, MakeInt32Array({1, 2}));
  ASSERT_OK_AND_ASSIGN(arr0_1, MakeStringArray({"a", "b", "c"})); // Mismatched length
  auto batch = RecordBatch::Make(schema, 2, {arr0_0, arr0_1}); // Declared length 2, but arr0_1 has 3

  Status s = builder.AppendRecordBatch(*batch);
  ASSERT_TRUE(s.IsInvalid());
  ASSERT_THAT(s.message(), ::testing::HasSubstr("Array length 3 does not match record batch length 2"));
}

// Test case for appending a record batch with null schema
TEST_F(TableBuilderTest, AppendRecordBatchNullSchema) {
  auto f0 = field("f0", int32());
  auto f1 = field("f1", utf8());
  auto schema = arrow::schema({f0, f1});

  TableBuilder builder(pool_, schema);

  std::shared_ptr<RecordBatch> null_schema_batch =
      RecordBatch::Make(nullptr, 0, {}); // Create a batch with null schema

  Status s = builder.AppendRecordBatch