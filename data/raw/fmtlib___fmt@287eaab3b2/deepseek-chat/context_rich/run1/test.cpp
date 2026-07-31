#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdint>

// Test fixture for basic_writer tests
class BasicWriterTest : public ::testing::Test {
protected:
    std::string buffer;
    fmt::basic_writer<std::back_insert_iterator<std::string>>* writer;

    void SetUp() override {
        writer = new fmt::basic_writer<std::back_insert_iterator<std::string>>(
            std::back_inserter(buffer));
    }

    void TearDown() override {
        delete writer;
        buffer.clear();
    }
};

// Test integer write overloads
TEST_F(BasicWriterTest, WriteInt) {
    writer->write(42);
    EXPECT_EQ(buffer, "42");
}

TEST_F(BasicWriterTest, WriteLong) {
    writer->write(123456789L);
    EXPECT_EQ(buffer, "123456789");
}

TEST_F(BasicWriterTest, WriteLongLong) {
    writer->write(9876543210LL);
    EXPECT_EQ(buffer, "9876543210");
}

TEST_F(BasicWriterTest, WriteUnsigned) {
    writer->write(100U);
    EXPECT_EQ(buffer, "100");
}

TEST_F(BasicWriterTest, WriteUnsignedLong) {
    writer->write(999999UL);
    EXPECT_EQ(buffer, "999999");
}

TEST_F(BasicWriterTest, WriteUnsignedLongLong) {
    writer->write(18446744073709551615ULL);
    EXPECT_EQ(buffer, "18446744073709551615");
}

// Test integer write with format specs
TEST_F(BasicWriterTest, WriteIntWithFormatSpec) {
    buffer.clear();
    writer->write(255, fmt::format_specs());
    EXPECT_EQ(buffer, "255");
}

TEST_F(BasicWriterTest, WriteIntWithMultipleSpecs) {
    buffer.clear();
    fmt::format_specs spec;
    spec.width_ = 5;
    writer->write(42, spec);
    EXPECT_EQ(buffer, "   42");
}

// Test double write
TEST_F(BasicWriterTest, WriteDouble) {
    writer->write(3.14159);
    EXPECT_EQ(buffer, "3.14159");
}

TEST_F(BasicWriterTest, WriteDoubleZero) {
    writer->write(0.0);
    EXPECT_EQ(buffer, "0");
}

TEST_F(BasicWriterTest, WriteDoubleNegative) {
    writer->write(-2.71828);
    EXPECT_EQ(buffer, "-2.71828");
}

TEST_F(BasicWriterTest, WriteDoubleInfinity) {
    writer->write(std::numeric_limits<double>::infinity());
    EXPECT_EQ(buffer, "inf");
}

TEST_F(BasicWriterTest, WriteDoubleNaN) {
    writer->write(std::numeric_limits<double>::quiet_NaN());
    EXPECT_EQ(buffer, "nan");
}

// Test long double write
TEST_F(BasicWriterTest, WriteLongDouble) {
    writer->write(1.234567890123456789L);
    EXPECT_EQ(buffer, "1.2345678901234567");
}

// Test char write
TEST_F(BasicWriterTest, WriteChar) {
    writer->write('A');
    EXPECT_EQ(buffer, "A");
}

TEST_F(BasicWriterTest, WriteCharNull) {
    writer->write('\0');
    EXPECT_EQ(buffer, std::string("\0", 1));
}

// Test string_view write
TEST_F(BasicWriterTest, WriteStringView) {
    writer->write(fmt::string_view("Hello"));
    EXPECT_EQ(buffer, "Hello");
}

TEST_F(BasicWriterTest, WriteStringViewEmpty) {
    writer->write(fmt::string_view(""));
    EXPECT_EQ(buffer, "");
}

TEST_F(BasicWriterTest, WriteStringViewWithNulls) {
    std::string s("He\0llo", 6);
    writer->write(fmt::string_view(s));
    EXPECT_EQ(buffer, s);
}

// Test formatted string write
TEST_F(BasicWriterTest, WriteFormattedString) {
    buffer.clear();
    fmt::align_spec spec;
    spec.width_ = 10;
    writer->write("test", 4, spec);
    EXPECT_EQ(buffer, "      test");
}

TEST_F(BasicWriterTest, WriteFormattedStringExactWidth) {
    buffer.clear();
    fmt::align_spec spec;
    spec.width_ = 4;
    writer->write("test", 4, spec);
    EXPECT_EQ(buffer, "test");
}

TEST_F(BasicWriterTest, WriteFormattedStringSmallerWidth) {
    buffer.clear();
    fmt::align_spec spec;
    spec.width_ = 2;
    writer->write("test", 4, spec);
    EXPECT_EQ(buffer, "test");
}

// Test basic_string_view write
TEST_F(BasicWriterTest, WriteBasicStringView) {
    buffer.clear();
    writer->write(fmt::string_view("Hello World"));
    EXPECT_EQ(buffer, "Hello World");
}

TEST_F(BasicWriterTest, WriteBasicStringViewWithPrecision) {
    buffer.clear();
    fmt::format_specs spec;
    spec.precision = 5;
    writer->write(fmt::string_view("Hello World"), spec);
    EXPECT_EQ(buffer, "Hello");
}

TEST_F(BasicWriterTest, WriteBasicStringViewEmpty) {
    buffer.clear();
    writer->write(fmt::string_view(""));
    EXPECT_EQ(buffer, "");
}

// Test out() method
TEST_F(BasicWriterTest, OutIterator) {
    auto it = writer->out();
    *it++ = 'X';
    EXPECT_EQ(buffer, "X");
}

// Test edge cases
TEST_F(BasicWriterTest, WriteMaxInt) {
    writer->write(std::numeric_limits<int>::max());
    EXPECT_EQ(buffer, std::to_string(std::numeric_limits<int>::max()));
}

TEST_F(BasicWriterTest, WriteMinInt) {
    writer->write(std::numeric_limits<int>::min());
    EXPECT_EQ(buffer, std::to_string(std::numeric_limits<int>::min()));
}

TEST_F(BasicWriterTest, WriteMaxDouble) {
    writer->write(std::numeric_limits<double>::max());
    EXPECT_FALSE(buffer.empty());
}

TEST_F(BasicWriterTest, WriteMinDouble) {
    writer->write(std::numeric_limits<double>::min());
    EXPECT_FALSE(buffer.empty());
}

TEST_F(BasicWriterTest, WriteDenormalizedDouble) {
    writer->write(std::numeric_limits<double>::denorm_min());
    EXPECT_FALSE(buffer.empty());
}

// Test multiple writes
TEST_F(BasicWriterTest, MultipleWrites) {
    writer->write(1);
    writer->write(2.0);
    writer->write('3');
    writer->write(fmt::string_view("4"));
    EXPECT_EQ(buffer, "1234");
}

// Test with locale
TEST_F(BasicWriterTest, WithLocale) {
    fmt::basic_writer<std::back_insert_iterator<std::string>> locale_writer(
        std::back_inserter(buffer), fmt::internal::locale_ref(std::locale::classic()));
    locale_writer.write(1234567);
    EXPECT_EQ(buffer, "1234567");
}