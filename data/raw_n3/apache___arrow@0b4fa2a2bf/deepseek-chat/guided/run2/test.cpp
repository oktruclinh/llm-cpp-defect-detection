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
    schema_ = schema({
        field("a", int32()),
        field("b", utf8()),
        field("c", float64()),
    });
  }

  std::shared_ptr<Schema> schema_;
};

TEST_F(TestRecordBatchBuilder, BasicConstruction) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));
  ASSERT_NE(builder, nullptr);
  EXPECT_EQ(builder->num_fields(), 3);
  EXPECT_EQ(builder->initial_capacity(), 0);
}

TEST_F(TestRecordBatchBuilder, FlushEmptyBatch) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));
  ASSERT_OK_AND_ASSIGN(auto batch, builder->Flush());
  ASSERT_NE(batch, nullptr);
  EXPECT_EQ(batch->num_rows(), 0);
  EXPECT_EQ(batch->num_columns(), 3);
  for (int i = 0; i < batch->num_columns(); ++i) {
    EXPECT_EQ(batch->column(i)->length(), 0);
  }
}

TEST_F(TestRecordBatchBuilder, FlushWithData) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));

  auto int_builder = builder->GetFieldAs<Int32Builder>(0);
  auto str_builder = builder->GetFieldAs<StringBuilder>(1);
  auto float_builder = builder->GetFieldAs<DoubleBuilder>(2);

  ASSERT_OK(int_builder->Append(42));
  ASSERT_OK(str_builder->Append("hello"));
  ASSERT_OK(float_builder->Append(3.14));

  ASSERT_OK(int_builder->Append(100));
  ASSERT_OK(str_builder->Append("world"));
  ASSERT_OK(float_builder->Append(2.71));

  ASSERT_OK_AND_ASSIGN(auto batch, builder->Flush());
  ASSERT_NE(batch, nullptr);
  EXPECT_EQ(batch->num_rows(), 2);
  EXPECT_EQ(batch->num_columns(), 3);

  // Verify int32 column
  auto int_array = std::static_pointer_cast<Int32Array>(batch->column(0));
  EXPECT_EQ(int_array->Value(0), 42);
  EXPECT_EQ(int_array->Value(1), 100);

  // Verify string column
  auto str_array = std::static_pointer_cast<StringArray>(batch->column(1));
  EXPECT_EQ(str_array->GetString(0), "hello");
  EXPECT_EQ(str_array->GetString(1), "world");

  // Verify float64 column
  auto float_array = std::static_pointer_cast<DoubleArray>(batch->column(2));
  EXPECT_DOUBLE_EQ(float_array->Value(0), 3.14);
  EXPECT_DOUBLE_EQ(float_array->Value(1), 2.71);
}

TEST_F(TestRecordBatchBuilder, FlushWithNulls) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));

  auto int_builder = builder->GetFieldAs<Int32Builder>(0);
  auto str_builder = builder->GetFieldAs<StringBuilder>(1);
  auto float_builder = builder->GetFieldAs<DoubleBuilder>(2);

  ASSERT_OK(int_builder->AppendNull());
  ASSERT_OK(str_builder->AppendNull());
  ASSERT_OK(float_builder->AppendNull());

  ASSERT_OK(int_builder->Append(5));
  ASSERT_OK(str_builder->Append("test"));
  ASSERT_OK(float_builder->Append(1.0));

  ASSERT_OK_AND_ASSIGN(auto batch, builder->Flush());
  ASSERT_NE(batch, nullptr);
  EXPECT_EQ(batch->num_rows(), 2);

  auto int_array = std::static_pointer_cast<Int32Array>(batch->column(0));
  EXPECT_TRUE(int_array->IsNull(0));
  EXPECT_FALSE(int_array->IsNull(1));
  EXPECT_EQ(int_array->Value(1), 5);

  auto str_array = std::static_pointer_cast<StringArray>(batch->column(1));
  EXPECT_TRUE(str_array->IsNull(0));
  EXPECT_FALSE(str_array->IsNull(1));
  EXPECT_EQ(str_array->GetString(1), "test");

  auto float_array = std::static_pointer_cast<DoubleArray>(batch->column(2));
  EXPECT_TRUE(float_array->IsNull(0));
  EXPECT_FALSE(float_array->IsNull(1));
  EXPECT_DOUBLE_EQ(float_array->Value(1), 1.0);
}

TEST_F(TestRecordBatchBuilder, MultipleFlushes) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));

  auto int_builder = builder->GetFieldAs<Int32Builder>(0);
  auto str_builder = builder->GetFieldAs<StringBuilder>(1);
  auto float_builder = builder->GetFieldAs<DoubleBuilder>(2);

  // First flush
  ASSERT_OK(int_builder->Append(1));
  ASSERT_OK(str_builder->Append("a"));
  ASSERT_OK(float_builder->Append(1.1));
  ASSERT_OK_AND_ASSIGN(auto batch1, builder->Flush());
  EXPECT_EQ(batch1->num_rows(), 1);

  // Second flush
  ASSERT_OK(int_builder->Append(2));
  ASSERT_OK(str_builder->Append("b"));
  ASSERT_OK(float_builder->Append(2.2));
  ASSERT_OK_AND_ASSIGN(auto batch2, builder->Flush());
  EXPECT_EQ(batch2->num_rows(), 1);

  // Verify first batch
  auto int_array1 = std::static_pointer_cast<Int32Array>(batch1->column(0));
  EXPECT_EQ(int_array1->Value(0), 1);

  // Verify second batch
  auto int_array2 = std::static_pointer_cast<Int32Array>(batch2->column(0));
  EXPECT_EQ(int_array2->Value(0), 2);
}

TEST_F(TestRecordBatchBuilder, SchemaPreservedAfterFlush) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));

  auto int_builder = builder->GetFieldAs<Int32Builder>(0);
  ASSERT_OK(int_builder->Append(1));

  ASSERT_OK_AND_ASSIGN(auto batch, builder->Flush());
  ASSERT_NE(batch, nullptr);

  auto batch_schema = batch->schema();
  ASSERT_NE(batch_schema, nullptr);
  EXPECT_EQ(batch_schema->num_fields(), 3);
  EXPECT_TRUE(batch_schema->field(0)->type()->Equals(int32()));
  EXPECT_TRUE(batch_schema->field(1)->type()->Equals(utf8()));
  EXPECT_TRUE(batch_schema->field(2)->type()->Equals(float64()));
}

TEST_F(TestRecordBatchBuilder, NullSchemaField) {
  auto null_schema = schema({
      field("a", null()),
      field("b", int32()),
  });

  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(null_schema, default_memory_pool()));
  auto null_builder = builder->GetFieldAs<NullBuilder>(0);
  auto int_builder = builder->GetFieldAs<Int32Builder>(1);

  ASSERT_OK(null_builder->AppendNull());
  ASSERT_OK(int_builder->Append(10));

  ASSERT_OK_AND_ASSIGN(auto batch, builder->Flush());
  ASSERT_NE(batch, nullptr);
  EXPECT_EQ(batch->num_rows(), 1);
  EXPECT_TRUE(batch->column(0)->IsNull(0));
  EXPECT_EQ(std::static_pointer_cast<Int32Array>(batch->column(1))->Value(0), 10);
}

TEST_F(TestRecordBatchBuilder, DictionaryTypePreserved) {
  // Test that dictionary types are properly resolved in schema after flush
  auto dict_type = dictionary(int32(), utf8());
  auto dict_schema = schema({
      field("dict_col", dict_type),
      field("int_col", int32()),
  });

  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(dict_schema, default_memory_pool()));
  auto dict_builder = builder->GetFieldAs<StringDictionaryBuilder>(0);
  auto int_builder = builder->GetFieldAs<Int32Builder>(1);

  ASSERT_OK(dict_builder->Append("foo"));
  ASSERT_OK(int_builder->Append(1));

  ASSERT_OK_AND_ASSIGN(auto batch, builder->Flush());
  ASSERT_NE(batch, nullptr);
  EXPECT_EQ(batch->num_rows(), 1);

  // Verify the schema field type is dictionary (not just the underlying value type)
  auto batch_field = batch->schema()->field(0);
  EXPECT_TRUE(batch_field->type()->id() == Type::DICTIONARY);
  EXPECT_TRUE(batch_field->type()->Equals(dict_type));
}

TEST_F(TestRecordBatchBuilder, ErrorOnMismatchedLength) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));

  auto int_builder = builder->GetFieldAs<Int32Builder>(0);
  auto str_builder = builder->GetFieldAs<StringBuilder>(1);

  // Append different numbers of elements to different builders
  ASSERT_OK(int_builder->Append(1));
  ASSERT_OK(int_builder->Append(2));
  ASSERT_OK(str_builder->Append("a"));

  // Flush should fail because column lengths don't match
  auto result = builder->Flush();
  EXPECT_FALSE(result.ok());
  EXPECT_TRUE(result.status().IsInvalid());
}

TEST_F(TestRecordBatchBuilder, ZeroLengthBatch) {
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(schema_, default_memory_pool()));

  // Flush without appending any data
  ASSERT_OK_AND_ASSIGN(auto batch, builder->Flush());
  EXPECT_EQ(batch->num_rows(), 0);
  EXPECT_EQ(batch->num_columns(), 3);
  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(batch->column(i)->length(), 0);
  }
}

TEST_F(TestRecordBatchBuilder, SingleFieldSchema) {
  auto single_schema = schema({field("x", int64())});
  ASSERT_OK_AND_ASSIGN(auto builder, RecordBatchBuilder::Make(single_schema, default_memory_pool()));

  auto int_builder = builder->GetFieldAs<Int64Builder>(0);
  ASSERT_OK(int_builder->Append(1234567890123LL));

  ASSERT_OK_AND_ASSIGN(auto batch, builder->Flush());
  EXPECT_EQ(batch->num_rows(), 1);
  EXPECT_EQ(batch->num_columns(), 1);
  EXPECT_EQ(std::static_pointer_cast<Int64Array>(batch->column(0))->Value(0), 1234567890123LL);
}

}  // namespace arrow