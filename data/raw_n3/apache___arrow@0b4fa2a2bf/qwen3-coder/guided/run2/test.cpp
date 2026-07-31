#include <gtest/gtest.h>
#include <arrow/table.h>
#include <arrow/array.h>
#include <arrow/builder.h>
#include <arrow/record_batch.h>
#include <arrow/schema.h>

namespace arrow {

TEST(TableBuilderTest, SchemaTypeResolutionAfterFlush) {
  // Test that TableBuilder correctly resolves field types (e.g., dictionary types)
  // after flushing, as per ARROW-9969

  // Create a schema with a dictionary field
  auto int32_type = int32();
  auto dict_type = dictionary(int32(), utf8());
  auto field = field("dict_col", dict_type);
  auto schema = schema({field});

  // Create a TableBuilder with the schema
  std::unique_ptr<TableBuilder> builder;
  ASSERT_OK(TableBuilder::Make(default_memory_pool(), schema, &builder));

  // Get the dictionary builder for the first column
  auto dict_builder = dynamic_cast<DictionaryBuilder<StringType>*>(builder->GetColumnBuilder(0));
  ASSERT_NE(dict_builder, nullptr);

  // Add some dictionary values
  ASSERT_OK(dict_builder->Append("apple"));
  ASSERT_OK(dict_builder->Append("banana"));
  ASSERT_OK(dict_builder->Append("apple")); // repeat to ensure dictionary encoding

  // Flush the builder to force type resolution
  std::shared_ptr<RecordBatch> batch;
  ASSERT_OK(builder->Flush(&batch));

  // Verify the resulting batch has the correct schema and type
  ASSERT_NE(batch, nullptr);
  ASSERT_EQ(batch->num_columns(), 1);
  ASSERT_EQ(batch->schema()->field(0)->name(), "dict_col");
  ASSERT_TRUE(batch->schema()->field(0)->type()->Equals(dict_type));

  // Verify the array is a dictionary array
  ASSERT_TRUE(batch->column(0)->type()->Equals(dict_type));
  auto dict_array = std::dynamic_pointer_cast<DictionaryArray>(batch->column(0));
  ASSERT_NE(dict_array, nullptr);

  // Verify the dictionary has the expected values
  auto dict_values = std::dynamic_pointer_cast<StringArray>(dict_array->dictionary());
  ASSERT_NE(dict_values, nullptr);
  ASSERT_EQ(dict_values->length(), 2);
  ASSERT_EQ(dict_values->GetString(0), "apple");
  ASSERT_EQ(dict_values->GetString(1), "banana");

  // Verify the indices
  auto indices = dict_array->indices();
  ASSERT_EQ(indices->length(), 3);
  auto indices_data = indices->data()->GetValues<int32_t>(1);
  ASSERT_EQ(indices_data[0], 0); // apple
  ASSERT_EQ(indices_data[1], 1); // banana
  ASSERT_EQ(indices_data[2], 0); // apple
}

TEST(TableBuilderTest, EmptyTableWithDictionarySchema) {
  // Test edge case: empty table with dictionary schema
  auto dict_type = dictionary(int32(), utf8());
  auto field = field("empty_dict", dict_type);
  auto schema = schema({field});

  std::unique_ptr<TableBuilder> builder;
  ASSERT_OK(TableBuilder::Make(default_memory_pool(), schema, &builder));

  // Flush empty builder
  std::shared_ptr<RecordBatch> batch;
  ASSERT_OK(builder->Flush(&batch));

  // Verify schema type is preserved even with zero rows
  ASSERT_NE(batch, nullptr);
  ASSERT_EQ(batch->num_columns(), 1);
  ASSERT_TRUE(batch->schema()->field(0)->type()->Equals(dict_type));
  ASSERT_EQ(batch->num_rows(), 0);

  // The dictionary array should still be a DictionaryArray with empty indices
  auto dict_array = std::dynamic_pointer_cast<DictionaryArray>(batch->column(0));
  ASSERT_NE(dict_array, nullptr);
  ASSERT_EQ(dict_array->length(), 0);
  ASSERT_EQ(dict_array->dictionary()->length(), 0);
}

TEST(TableBuilderTest, MultipleFieldsWithMixedTypes) {
  // Test multiple fields including dictionary and non-dictionary types
  auto int_type = int32();
  auto dict_type = dictionary(int32(), utf8());
  auto schema = schema({
    field("int_col", int_type),
    field("dict_col", dict_type)
  });

  std::unique_ptr<TableBuilder> builder;
  ASSERT_OK(TableBuilder::Make(default_memory_pool(), schema, &builder));

  // Add data to both columns
  auto int_builder = dynamic_cast<Int32Builder*>(builder->GetColumnBuilder(0));
  auto dict_builder = dynamic_cast<DictionaryBuilder<StringType>*>(builder->GetColumnBuilder(1));

  ASSERT_NE(int_builder, nullptr);
  ASSERT_NE(dict_builder, nullptr);

  ASSERT_OK(int_builder->Append(42));
  ASSERT_OK(int_builder->Append(100));

  ASSERT_OK(dict_builder->Append("test"));
  ASSERT_OK(dict_builder->Append("test")); // repeat to trigger dictionary encoding

  std::shared_ptr<RecordBatch> batch;
  ASSERT_OK(builder->Flush(&batch));

  // Verify both fields have correct types after flush
  ASSERT_EQ(batch->num_columns(), 2);
  ASSERT_TRUE(batch->schema()->field(0)->type()->Equals(int_type));
  ASSERT_TRUE(batch->schema()->field(1)->type()->Equals(dict_type));

  // Verify the dictionary array has correct values
  auto dict_array = std::dynamic_pointer_cast<DictionaryArray>(batch->column(1));
  ASSERT_NE(dict_array, nullptr);
  auto dict_values = std::dynamic_pointer_cast<StringArray>(dict_array->dictionary());
  ASSERT_EQ(dict_values->length(), 1);
  ASSERT_EQ(dict_values->GetString(0), "test");
  ASSERT_EQ(dict_array->indices()->length(), 2);
}

TEST(TableBuilderTest, SchemaPreservedAfterMultipleFlushes) {
  // Test that schema types are preserved across multiple flushes
  auto dict_type = dictionary(int32(), utf8());
  auto field = field("dict_col", dict_type);
  auto schema = schema({field});

  std::unique_ptr<TableBuilder> builder;
  ASSERT_OK(TableBuilder::Make(default_memory_pool(), schema, &builder));

  auto dict_builder = dynamic_cast<DictionaryBuilder<StringType>*>(builder->GetColumnBuilder(0));
  ASSERT_NE(dict_builder, nullptr);

  // First flush: empty
  std::shared_ptr<RecordBatch> batch;
  ASSERT_OK(builder->Flush(&batch));
  ASSERT_TRUE(batch->schema()->field(0)->type()->Equals(dict_type));

  // Add data
  ASSERT_OK(dict_builder->Append("first"));
  ASSERT_OK(dict_builder->Append("second"));

  // Second flush
  ASSERT_OK(builder->Flush(&batch));
  ASSERT_TRUE(batch->schema()->field(0)->type()->Equals(dict_type));
  auto dict_array = std::dynamic_pointer_cast<DictionaryArray>(batch->column(0));
  ASSERT_NE(dict_array, nullptr);
  ASSERT_EQ(dict_array->dictionary()->length(), 2);

  // Add more data
  ASSERT_OK(dict_builder->Append("first")); // repeat

  // Third flush
  ASSERT_OK(builder->Flush(&batch));
  ASSERT_TRUE(batch->schema()->field(0)->type()->Equals(dict_type));
  dict_array = std::dynamic_pointer_cast<DictionaryArray>(batch->column(0));
  ASSERT_NE(dict_array, nullptr);
  ASSERT_EQ(dict_array->dictionary()->length(), 2); // still 2 unique values
  ASSERT_EQ(dict_array->length(), 3);
}

} // namespace arrow