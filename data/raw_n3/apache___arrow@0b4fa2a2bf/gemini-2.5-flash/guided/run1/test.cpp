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

// Helper function to create a simple DictionaryArray
Result<std::shared_ptr<Array>> MakeDictionaryArray(
    const std::shared_ptr<Array>& dictionary, const std::shared_ptr<Array>& indices) {
  return DictionaryArray::FromArrays(dictionary, indices);
}

class TableBuilderTest : public ::testing::Test {
 protected:
  void SetUp() override {
    pool_ = default_memory_pool();
    schema_ = arrow::schema({
        field("int_col", int32()),
        field("str_col", utf8()),
    });
  }

  MemoryPool* pool_;
  std::shared_ptr<Schema> schema_;
};

TEST_F(TableBuilderTest, BasicBuild) {
  std::unique_ptr<RecordBatchBuilder> builder;
  ASSERT_OK(RecordBatchBuilder::Make(schema_, pool_, &builder));

  // Add first batch
  ASSERT_OK(builder->Get  <Int32Builder>(0)->AppendValues({1, 2, 3}));
  ASSERT_OK(builder->Get  <StringBuilder>(1)->AppendValues({"a", "b", "c"}));
  ASSERT_OK(builder->Flush());

  // Add second batch
  ASSERT_OK(builder->Get  <Int32Builder>(0)->AppendValues({4, 5}));
  ASSERT_OK(builder->Get  <StringBuilder>(1)->AppendValues({"d", "e"}));
  ASSERT_OK(builder->Flush());

  std::shared_ptr<Table> table;
  ASSERT_OK(builder->Finish(&table));

  ASSERT_NE(table, nullptr);
  ASSERT_EQ(table->num_rows(), 5);
  ASSERT_EQ(table->num_columns(), 2);
  ASSERT_EQ(table->schema()->ToString(), schema_->ToString());

  ASSERT_EQ(table->column(0)->num_chunks(), 2);
  ASSERT_EQ(table->column(1)->num_chunks(), 2);

  ASSERT_EQ(table->column(0)->chunk(0)->length(), 3);
  ASSERT_EQ(table->column(0)->chunk(1)->length(), 2);
  ASSERT_EQ(table->column(1)->chunk(0)->length(), 3);
  ASSERT_EQ(table->column(1)->chunk(1)->length(), 2);
}

TEST_F(TableBuilderTest, EmptyTable) {
  std::unique_ptr<RecordBatchBuilder> builder;
  ASSERT_OK(RecordBatchBuilder::Make(schema_, pool_, &builder));

  std::shared_ptr<Table> table;
  ASSERT_OK(builder->Finish(&table));

  ASSERT_NE(table, nullptr);
  ASSERT_EQ(table->num_rows(), 0);
  ASSERT_EQ(table->num_columns(), 2);
  ASSERT_EQ(table->schema()->ToString(), schema_->ToString());

  ASSERT_EQ(table->column(0)->num_chunks(), 0);
  ASSERT_EQ(table->column(1)->num_chunks(), 0);
}

TEST_F(TableBuilderTest, SingleEmptyFlush) {
  std::unique_ptr<RecordBatchBuilder> builder;
  ASSERT_OK(RecordBatchBuilder::Make(schema_, pool_, &builder));

  ASSERT_OK(builder->Flush()); // Flush an empty batch

  std::shared_ptr<Table> table;
  ASSERT_OK(builder->Finish(&table));

  ASSERT_NE(table, nullptr);
  ASSERT_EQ(table->num_rows(), 0);
  ASSERT_EQ(table->num_columns(), 2);
  ASSERT_EQ(table->schema()->ToString(), schema_->ToString());

  ASSERT_EQ(table->column(0)->num_chunks(), 0); // No chunks should be added for empty flush
  ASSERT_EQ(table->column(1)->num_chunks(), 0);
}

TEST_F(TableBuilderTest, MixedEmptyAndNonEmptyFlushes) {
  std::unique_ptr<RecordBatchBuilder> builder;
  ASSERT_OK(RecordBatchBuilder::Make(schema_, pool_, &builder));

  ASSERT_OK(builder->Get  <Int32Builder>(0)->AppendValues({1}));
  ASSERT_OK(builder->Get  <StringBuilder>(1)->AppendValues({"a"}));
  ASSERT_OK(builder->Flush()); // Batch 1 (1 row)

  ASSERT_OK(builder->Flush()); // Empty batch 2

  ASSERT_OK(builder->Get  <Int32Builder>(0)->AppendValues({2, 3}));
  ASSERT_OK(builder->Get  <StringBuilder>(1)->AppendValues({"b", "c"}));
  ASSERT_OK(builder->Flush()); // Batch 3 (2 rows)

  std::shared_ptr<Table> table;
  ASSERT_OK(builder->Finish(&table));

  ASSERT_NE(table, nullptr);
  ASSERT_EQ(table->num_rows(), 3);
  ASSERT_EQ(table->num_columns(), 2);

  ASSERT_EQ(table->column(0)->num_chunks(), 2); // Two non-empty chunks
  ASSERT_EQ(table->column(1)->num_chunks(), 2);

  ASSERT_EQ(table->column(0)->chunk(0)->length(), 1);
  ASSERT_EQ(table->column(0)->chunk(1)->length(), 2);
}

TEST_F(TableBuilderTest, DictionaryTypeResolution) {
  // This test specifically targets the ARROW-9969 fix mentioned in the focal function.
  // The dictionary type might not be fully determined until Flush() or Finish().
  auto dict_type = dictionary(int8(), utf8());
  auto dict_field = field("dict_col", dict_type);
  auto schema_with_dict = arrow::schema({dict_field});

  std::unique_ptr<RecordBatchBuilder> builder;
  ASSERT_OK(RecordBatchBuilder::Make(schema_with_dict, pool_, &builder));

  auto dict_builder = checked_cast<DictionaryBuilder*>(builder->Get  (0));
  auto index_builder = checked_cast<Int8Builder*>(dict_builder->indices_builder());
  auto value_builder = checked_cast<StringBuilder*>(dict_builder->dictionary_builder());

  // Add some values, the dictionary type should be inferred/finalized on flush
  ASSERT_OK(value_builder->AppendValues({"apple", "banana"}));
  ASSERT_OK(index_builder->AppendValues({0, 1, 0}));
  ASSERT_OK(builder->Flush());

  ASSERT_OK(value_builder->AppendValues({"orange"}));
  ASSERT_OK(index_builder->AppendValues({2, 1}));
  ASSERT_OK(builder->Flush());

  std::shared_ptr<Table> table;
  ASSERT_OK(builder->Finish(&table));

  ASSERT_NE(table, nullptr);
  ASSERT_EQ(table->num_rows(), 5);
  ASSERT_EQ(table->num_columns(), 1);

  // Verify the schema of the finished table has the correct dictionary type
  ASSERT_TRUE(table->schema()->field(0)->type()->Equals(dict_type));

  // Verify the chunks
  ASSERT_EQ(table->column(0)->num_chunks(), 2);
  ASSERT_EQ(table->column(0)->chunk(0)->length(), 3);
  ASSERT_EQ(table->column(0)->chunk(1)->length(), 2);

  auto chunk0 = checked_cast<const DictionaryArray*>(table->column(0)->chunk(0).get());
  auto chunk1 = checked_cast<const DictionaryArray*>(table->column(0)->chunk(1).get());

  // Check dictionary values
  auto expected_dict_chunk0 = MakeStringArray({"apple", "banana"});
  ASSERT_OK(expected_dict_chunk0);
  ASSERT_TRUE(chunk0->dictionary()->Equals(**expected_dict_chunk0));

  // The dictionary for chunk1 should contain all unique values encountered so far
  auto expected_dict_chunk1 = MakeStringArray({"apple", "banana", "orange"});
  ASSERT_OK(expected_dict_chunk1);
  ASSERT_TRUE(chunk1->dictionary()->Equals(**expected_dict_chunk1));
}

TEST_F(TableBuilderTest, SchemaMetadataPreservation) {
  auto metadata = key_value_metadata({"key1", "key2"}, {"value1", "value2"});
  auto field_with_meta = field("int_col", int32(), false, metadata);
  auto schema_with_meta = arrow::schema({field_with_meta});

  std::unique_ptr<RecordBatchBuilder> builder;
  ASSERT_OK(RecordBatchBuilder::Make(schema_with_meta, pool_, &builder));

  ASSERT_OK(builder->Get  <Int32Builder>(0)->AppendValues({1, 2}));
  ASSERT_OK(builder->Flush());

  std::shared_ptr<Table> table;
  ASSERT_OK(builder->Finish(&table));

  ASSERT_NE(table, nullptr);
  ASSERT_EQ(table->num_rows(), 2);
  ASSERT_EQ(table->num_columns(), 1);

  // Verify that the metadata is preserved in the final table's schema
  ASSERT_TRUE(table->schema()->field(0)->metadata()->Equals(*metadata));
}

TEST_F(TableBuilderTest, ColumnLengthMismatchError) {
  std::unique_ptr<RecordBatchBuilder> builder;
  ASSERT_OK(RecordBatchBuilder::Make(schema_, pool_, &builder));

  ASSERT_OK(builder->Get  <Int32Builder>(0)->AppendValues({1, 2, 3}));
  // Missing value for the second column, leading to length mismatch on Flush
  // ASSERT_OK(builder->Get<StringBuilder>(1)->AppendValues({"a", "b", "c"}));

  // Flush should return an error due to length mismatch
  ASSERT_RAISES(Invalid, builder->Flush());

  // After an error, the builder should ideally be in a state where Finish() can be called
  // or it can be reset. For now, we just check that Flush failed.
  // We cannot call Finish() directly after a failed Flush without clearing the builders.
  // The current implementation of Flush() leaves the builders in an inconsistent state
  // if an error occurs, so we can't reliably call Finish() without further actions.
  // This test primarily verifies that Flush() correctly identifies and reports the error.
}

TEST_F(TableBuilderTest, FlushWithNoDataInSomeColumns) {
  std::unique_ptr<RecordBatchBuilder> builder;
  ASSERT_OK(RecordBatchBuilder::Make(schema_, pool_, &builder));

  // Add data to only one column
  ASSERT_OK(builder->Get  <Int32Builder>(0)->AppendValues({1, 2, 3}));
  // Second column has no data appended, but its length is 0, so it's consistent.
  ASSERT_OK(builder->Flush());

  std::shared_ptr<Table> table;
  ASSERT_OK(builder->Finish(&table));

  ASSERT_NE(table, nullptr);
  ASSERT_EQ(table->num_rows(), 3);
  ASSERT_EQ(table->num_columns(), 2);

  // The second column should contain a chunk of nulls or empty strings
  ASSERT_EQ(table->column(1)->num_chunks(), 1);
  ASSERT_EQ(table->column(1)->chunk(0)->length(), 3);
  ASSERT_TRUE(table->column(1)->chunk(0)->IsNull(0)); // Assuming default nulls for string builder
}

}  // namespace arrow