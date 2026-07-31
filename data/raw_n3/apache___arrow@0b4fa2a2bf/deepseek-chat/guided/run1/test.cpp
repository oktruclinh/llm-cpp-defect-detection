#include <gtest/gtest.h>
#include "arrow/table_builder.h"
#include "arrow/array/builder_binary.h"
#include "arrow/array/builder_primitive.h"
#include "arrow/schema.h"
#include "arrow/status.h"
#include "arrow/table.h"
#include "arrow/type.h"
#include "arrow/type_fwd.h"

#include <memory>
#include <vector>

namespace arrow {

class TestRecordBatchBuilder : public ::testing::Test {
 protected:
  void SetUp() override {
    // Create a simple schema with two fields
    auto field1 = field("a", int32());
    auto field2 = field("b", utf8());
    schema_ = schema({field1, field2});
  }

  std::shared_ptr<Schema> schema_;
};

TEST_F(TestRecordBatchBuilder, BasicConstruction) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));
  EXPECT_EQ(builder->num_fields(), 2);
  EXPECT_EQ(builder->schema()->num_fields(), 2);
}

TEST_F(TestRecordBatchBuilder, EmptyBatch) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));
  ASSERT_OK_AND_ASSIGN(auto batch, builder->Flush());
  EXPECT_EQ(batch->num_rows(), 0);
  EXPECT_EQ(batch->num_columns(), 2);
}

TEST_F(TestRecordBatchBuilder, SingleRow) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));

  auto int_builder = builder->GetFieldAs<Int32Builder>(0);
  auto str_builder = builder->GetFieldAs<StringBuilder>(1);

  ASSERT_OK(int_builder->Append(42));
  ASSERT_OK(str_builder->Append("hello"));

  ASSERT_OK_AND_ASSIGN(auto batch, builder->Flush());
  EXPECT_EQ(batch->num_rows(), 1);
  EXPECT_EQ(batch->num_columns(), 2);

  // Verify the values
  auto int_array = std::static_pointer_cast<Int32Array>(batch->column(0));
  auto str_array = std::static_pointer_cast<StringArray>(batch->column(1));
  EXPECT_EQ(int_array->Value(0), 42);
  EXPECT_EQ(str_array->GetString(0), "hello");
}

TEST_F(TestRecordBatchBuilder, MultipleRows) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));

  auto int_builder = builder->GetFieldAs<Int32Builder>(0);
  auto str_builder = builder->GetFieldAs<StringBuilder>(1);

  for (int i = 0; i < 10; ++i) {
    ASSERT_OK(int_builder->Append(i));
    ASSERT_OK(str_builder->Append(std::to_string(i)));
  }

  ASSERT_OK_AND_ASSIGN(auto batch, builder->Flush());
  EXPECT_EQ(batch->num_rows(), 10);

  auto int_array = std::static_pointer_cast<Int32Array>(batch->column(0));
  auto str_array = std::static_pointer_cast<StringArray>(batch->column(1));
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(int_array->Value(i), i);
    EXPECT_EQ(str_array->GetString(i), std::to_string(i));
  }
}

TEST_F(TestRecordBatchBuilder, NullValues) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));

  auto int_builder = builder->GetFieldAs<Int32Builder>(0);
  auto str_builder = builder->GetFieldAs<StringBuilder>(1);

  ASSERT_OK(int_builder->AppendNull());
  ASSERT_OK(str_builder->AppendNull());
  ASSERT_OK(int_builder->Append(1));
  ASSERT_OK(str_builder->Append("not null"));

  ASSERT_OK_AND_ASSIGN(auto batch, builder->Flush());
  EXPECT_EQ(batch->num_rows(), 2);

  auto int_array = std::static_pointer_cast<Int32Array>(batch->column(0));
  auto str_array = std::static_pointer_cast<StringArray>(batch->column(1));
  EXPECT_TRUE(int_array->IsNull(0));
  EXPECT_TRUE(str_array->IsNull(0));
  EXPECT_FALSE(int_array->IsNull(1));
  EXPECT_FALSE(str_array->IsNull(1));
  EXPECT_EQ(int_array->Value(1), 1);
  EXPECT_EQ(str_array->GetString(1), "not null");
}

TEST_F(TestRecordBatchBuilder, FlushMultipleTimes) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));

  auto int_builder = builder->GetFieldAs<Int32Builder>(0);
  auto str_builder = builder->GetFieldAs<StringBuilder>(1);

  ASSERT_OK(int_builder->Append(1));
  ASSERT_OK(str_builder->Append("a"));
  ASSERT_OK_AND_ASSIGN(auto batch1, builder->Flush());
  EXPECT_EQ(batch1->num_rows(), 1);

  ASSERT_OK(int_builder->Append(2));
  ASSERT_OK(str_builder->Append("b"));
  ASSERT_OK_AND_ASSIGN(auto batch2, builder->Flush());
  EXPECT_EQ(batch2->num_rows(), 1);

  // Verify first batch
  auto int_array1 = std::static_pointer_cast<Int32Array>(batch1->column(0));
  auto str_array1 = std::static_pointer_cast<StringArray>(batch1->column(1));
  EXPECT_EQ(int_array1->Value(0), 1);
  EXPECT_EQ(str_array1->GetString(0), "a");

  // Verify second batch
  auto int_array2 = std::static_pointer_cast<Int32Array>(batch2->column(0));
  auto str_array2 = std::static_pointer_cast<StringArray>(batch2->column(1));
  EXPECT_EQ(int_array2->Value(0), 2);
  EXPECT_EQ(str_array2->GetString(0), "b");
}

TEST_F(TestRecordBatchBuilder, DictionaryTypePreservation) {
  // Test that dictionary types are properly resolved in schema (ARROW-9969)
  auto dict_type = dictionary(int32(), utf8());
  auto field1 = field("dict_col", dict_type);
  auto field2 = field("int_col", int32());
  auto schema = schema({field1, field2});

  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema, default_memory_pool()));

  auto dict_builder = builder->GetFieldAs<StringDictionaryBuilder>(0);
  auto int_builder = builder->GetFieldAs<Int32Builder>(1);

  ASSERT_OK(dict_builder->Append("hello"));
  ASSERT_OK(int_builder->Append(42));

  ASSERT_OK_AND_ASSIGN(auto batch, builder->Flush());

  // The schema should have the dictionary type resolved
  auto batch_schema = batch->schema();
  auto batch_field = batch_schema->field(0);
  EXPECT_TRUE(batch_field->type()->id() == Type::DICTIONARY);
  EXPECT_EQ(batch_field->type()->ToString(), dict_type->ToString());

  // Verify the data
  auto dict_array = std::static_pointer_cast<DictionaryArray>(batch->column(0));
  EXPECT_EQ(dict_array->dictionary()->length(), 1);
  auto dict_values = std::static_pointer_cast<StringArray>(dict_array->dictionary());
  EXPECT_EQ(dict_values->GetString(0), "hello");
}

TEST_F(TestRecordBatchBuilder, ErrorOnMismatchedLengths) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));

  auto int_builder = builder->GetFieldAs<Int32Builder>(0);
  auto str_builder = builder->GetFieldAs<StringBuilder>(1);

  ASSERT_OK(int_builder->Append(1));
  ASSERT_OK(int_builder->Append(2));
  ASSERT_OK(str_builder->Append("a"));  // Only one string for two ints

  auto result = builder->Flush();
  EXPECT_FALSE(result.ok());
  EXPECT_TRUE(result.status().IsInvalid());
}

TEST_F(TestRecordBatchBuilder, ZeroLengthField) {
  // Test with a field that has no values
  auto field1 = field("empty", int32());
  auto field2 = field("data", utf8());
  auto schema = schema({field1, field2});

  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema, default_memory_pool()));

  auto empty_builder = builder->GetFieldAs<Int32Builder>(0);
  auto data_builder = builder->GetFieldAs<StringBuilder>(1);

  ASSERT_OK(data_builder->Append("test"));

  ASSERT_OK_AND_ASSIGN(auto batch, builder->Flush());
  EXPECT_EQ(batch->num_rows(), 1);
  EXPECT_TRUE(batch->column(0)->IsNull(0));
  EXPECT_EQ(batch->column(1)->length(), 1);
}

}  // namespace arrow