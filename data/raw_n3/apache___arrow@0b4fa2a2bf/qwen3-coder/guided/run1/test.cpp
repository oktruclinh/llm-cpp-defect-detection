#include <gtest/gtest.h>
#include <arrow/api.h>
#include <arrow/table_builder.h>
#include <arrow/type.h>
#include <arrow/schema.h>
#include <memory>

namespace arrow {
namespace {

TEST(TableBuilderTest, TestSchemaFieldTypes) {
  // Test that schema fields maintain correct types after building
  auto int32_type = int32();
  auto string_type = utf8();
  
  // Create a schema with fields
  auto field1 = field("int_field", int32_type);
  auto field2 = field("string_field", string_type);
  auto schema = schema({field1, field2});
  
  // Create a table builder
  std::unique_ptr<TableBuilder> builder;
  ASSERT_OK(TableBuilder::Make(schema, default_memory_pool(), &builder));
  
  // Add some data
  auto int_builder = static_cast<Int32Builder*>(builder->GetFieldBuilder(0).get());
  auto string_builder = static_cast<StringBuilder*>(builder->GetFieldBuilder(1).get());
  
  // Add values
  ASSERT_OK(int_builder->Append(42));
  ASSERT_OK(string_builder->Append("hello"));
  
  // Finalize the table
  std::shared_ptr<Table> table;
  ASSERT_OK(builder->Flush(&table));
  
  // Verify schema fields have correct types
  ASSERT_EQ(table->schema()->num_fields(), 2);
  ASSERT_EQ(table->schema()->field(0)->type()->id(), Type::INT32);
  ASSERT_EQ(table->schema()->field(1)->type()->id(), Type::STRING);
}

TEST(TableBuilderTest, TestEmptyTableBuilder) {
  // Test building an empty table
  auto int32_type = int32();
  auto field1 = field("int_field", int32_type);
  auto schema = schema({field1});
  
  std::unique_ptr<TableBuilder> builder;
  ASSERT_OK(TableBuilder::Make(schema, default_memory_pool(), &builder));
  
  // Don't add any data, just flush
  std::shared_ptr<Table> table;
  ASSERT_OK(builder->Flush(&table));
  
  // Verify empty table has correct schema
  ASSERT_EQ(table->schema()->num_fields(), 1);
  ASSERT_EQ(table->schema()->field(0)->type()->id(), Type::INT32);
  ASSERT_EQ(table->num_rows(), 0);
}

TEST(TableBuilderTest, TestMultipleFieldTypes) {
  // Test with multiple field types that might have type determination issues
  auto int32_type = int32();
  auto float64_type = float64();
  auto bool_type = boolean();
  auto string_type = utf8();
  
  auto field1 = field("int_field", int32_type);
  auto field2 = field("float_field", float64_type);
  auto field3 = field("bool_field", bool_type);
  auto field4 = field("string_field", string_type);
  auto schema = schema({field1, field2, field3, field4});
  
  std::unique_ptr<TableBuilder> builder;
  ASSERT_OK(TableBuilder::Make(schema, default_memory_pool(), &builder));
  
  // Add data to all fields
  auto int_builder = static_cast<Int32Builder*>(builder->GetFieldBuilder(0).get());
  auto float_builder = static_cast<DoubleBuilder*>(builder->GetFieldBuilder(1).get());
  auto bool_builder = static_cast<BooleanBuilder*>(builder->GetFieldBuilder(2).get());
  auto string_builder = static_cast<StringBuilder*>(builder->GetFieldBuilder(3).get());
  
  ASSERT_OK(int_builder->Append(100));
  ASSERT_OK(float_builder->Append(3.14));
  ASSERT_OK(bool_builder->Append(true));
  ASSERT_OK(string_builder->Append("test"));
  
  // Finalize
  std::shared_ptr<Table> table;
  ASSERT_OK(builder->Flush(&table));
  
  // Verify all field types are preserved
  ASSERT_EQ(table->schema()->num_fields(), 4);
  ASSERT_EQ(table->schema()->field(0)->type()->id(), Type::INT32);
  ASSERT_EQ(table->schema()->field(1)->type()->id(), Type::DOUBLE);
  ASSERT_EQ(table->schema()->field(2)->type()->id(), Type::BOOL);
  ASSERT_EQ(table->schema()->field(3)->type()->id(), Type::STRING);
}

TEST(TableBuilderTest, TestDictionaryTypeHandling) {
  // Test with dictionary type which might have type determination issues
  auto dict_type = dictionary(int32(), utf8());
  auto field1 = field("dict_field", dict_type);
  auto schema = schema({field1});
  
  std::unique_ptr<TableBuilder> builder;
  ASSERT_OK(TableBuilder::Make(schema, default_memory_pool(), &builder));
  
  // Add data
  auto dict_builder = static_cast<DictionaryBuilder<int32_t>*>(builder->GetFieldBuilder(0).get());
  ASSERT_OK(dict_builder->Append("apple"));
  ASSERT_OK(dict_builder->Append("banana"));
  
  // Finalize
  std::shared_ptr<Table> table;
  ASSERT_OK(builder->Flush(&table));
  
  // Verify dictionary type is preserved
  ASSERT_EQ(table->schema()->num_fields(), 1);
  ASSERT_EQ(table->schema()->field(0)->type()->id(), Type::DICTIONARY);
}

} // anonymous namespace
} // namespace arrow