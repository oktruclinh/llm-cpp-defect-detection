#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/core.h> // For fmt::format_to and fmt::memory_buffer

// Helper function to format a value into a string for comparison
template <typename T>
std::string format_to_string(T value) {
    fmt::memory_buffer buf;
    fmt::format_to(fmt::appender(buf), "{}", value);
    return fmt::to_string(buf);
}

// Test fixture for fmt::format_to with different types
class FormatToTest : public ::testing::Test {
protected:
    // We'll use fmt::memory_buffer as a common buffer type for testing
    // fmt::format_to, which internally uses the write methods.
    fmt::memory_buffer buffer_;
};

// Test case for writing a long double
TEST_F(FormatToTest, WriteLongDouble) {
    long double value = 123.4567890123456789L;
    fmt::format_to(fmt::appender(buffer_), "{}", value);
    std::string expected = fmt::format("{}", value); // Use fmt's own formatting for comparison
    ASSERT_EQ(fmt::to_string(buffer_), expected);

    buffer_.clear();
    value = 0.0L;
    fmt::format_to(fmt::appender(buffer_), "{}", value);
    expected = fmt::format("{}", value);
    ASSERT_EQ(fmt::to_string(buffer_), expected);

    buffer_.clear();
    value = -1.0L;
    fmt::format_to(fmt::appender(buffer_), "{}", value);
    expected = fmt::format("{}", value);
    ASSERT_EQ(fmt::to_string(buffer_), expected);

    buffer_.clear();
    value = std::numeric_limits<long double>::max();
    fmt::format_to(fmt::appender(buffer_), "{}", value);
    expected = fmt::format("{}", value);
    ASSERT_EQ(fmt::to_string(buffer_), expected);

    buffer_.clear();
    value = std::numeric_limits<long double>::min();
    fmt::format_to(fmt::appender(buffer_), "{}", value);
    expected = fmt::format("{}", value);
    ASSERT_EQ(fmt::to_string(buffer_), expected);

    buffer_.clear();
    value = std::numeric_limits<long double>::infinity();
    fmt::format_to(fmt::appender(buffer_), "{}", value);
    expected = fmt::format("{}", value);
    ASSERT_EQ(fmt::to_string(buffer_), expected);

    buffer_.clear();
    value = -std::numeric_limits<long double>::infinity();
    fmt::format_to(fmt::appender(buffer_), "{}", value);
    expected = fmt::format("{}", value);
    ASSERT_EQ(fmt::to_string(buffer_), expected);

    buffer_.clear();
    value = std::numeric_limits<long double>::quiet_NaN();
    fmt::format_to(fmt::appender(buffer_), "{}", value);
    // NaN comparison is tricky, check for "nan" substring
    ASSERT_NE(fmt::to_string(buffer_).find("nan"), std::string::npos);
}

// Test case for writing a char
TEST_F(FormatToTest, WriteChar) {
    char value = 'A';
    fmt::format_to(fmt::appender(buffer_), "{}", value);
    ASSERT_EQ(fmt::to_string(buffer_), "A");

    buffer_.clear();
    value = 'z';
    fmt::format_to(fmt::appender(buffer_), "{}", value);
    ASSERT_EQ(fmt::to_string(buffer_), "z");

    buffer_.clear();
    value = ' ';
    fmt::format_to(fmt::appender(buffer_), "{}", value);
    ASSERT_EQ(fmt::to_string(buffer_), " ");

    buffer_.clear();
    value = '\n'; // Newline character
    fmt::format_to(fmt::appender(buffer_), "{}", value);
    ASSERT_EQ(fmt::to_string(buffer_), "\n");

    buffer_.clear();
    value = '\0'; // Null character (should be written, but might not be visible in string)
    fmt::format_to(fmt::appender(buffer_), "{}", value);
    // The buffer should contain one null character.
    ASSERT_EQ(buffer_.size(), 1);
    ASSERT_EQ(buffer_[0], '\0');
    // fmt::to_string might truncate at null, so check size and content directly
}

// Test case for writing a wchar_t (requires wide character buffer)
// This test will only compile and run if FMT_USE_WCHAR_T is defined,
// or if the char_type of the formatter is wchar_t.
// For fmt::format_to, the char_type is typically char.
// To test the wchar_t overload, we need to explicitly use a wide character buffer.
TEST(FormatToWCharTest, WriteWChar) {
    // We need a wide character buffer and formatter for this.
    // fmt::wmemory_buffer is the equivalent for wide characters.
    fmt::wmemory_buffer wbuffer;
    wchar_t value = L'Ω'; // A non-ASCII wide character
    fmt::format_to(fmt::appender(wbuffer), L"{}", value);
    ASSERT_EQ(fmt::to_string(wbuffer), L"Ω");

    wbuffer.clear();
    value = L'A';
    fmt::format_to(fmt::appender(wbuffer), L"{}", value);
    ASSERT_EQ(fmt::to_string(wbuffer), L"A");

    wbuffer.clear();
    value = L' ';
    fmt::format_to(fmt::appender(wbuffer), L"{}", value);
    ASSERT_EQ(fmt::to_string(wbuffer), L" ");

    wbuffer.clear();
    value = L'\n';
    fmt::format_to(fmt::appender(wbuffer), L"{}", value);
    ASSERT_EQ(fmt::to_string(wbuffer), L"\n");

    wbuffer.clear();
    value = L'\0';
    fmt::format_to(fmt::appender(wbuffer), L"{}", value);
    ASSERT_EQ(wbuffer.size(), 1);
    ASSERT_EQ(wbuffer[0], L'\0');
}

// Test writing multiple characters to ensure buffer growth and correct appending
TEST_F(FormatToTest, WriteMultipleChars) {
    fmt::format_to(fmt::appender(buffer_), "{}{}{}", 'a', 'b', 'c');
    ASSERT_EQ(fmt::to_string(buffer_), "abc");

    buffer_.clear();
    fmt::format_to(fmt::appender(buffer_), "{} {} {}", '1', '2', '3');
    ASSERT_EQ(fmt::to_string(buffer_), "1 2 3");
}

// Test writing multiple long doubles
TEST_F(FormatToTest, WriteMultipleLongDoubles) {
    long double val1 = 1.1L;
    long double val2 = 2.2L;
    fmt::format_to(fmt::appender(buffer_), "{}{}", val1, val2);
    std::string expected = fmt::format("{}{}", val1, val2);
    ASSERT_EQ(fmt::to_string(buffer_), expected);
}

// Test mixed types
TEST_F(FormatToTest, WriteMixedTypes) {
    long double ld_val = 3.14L;
    char char_val = 'X';
    fmt::format_to(fmt::appender(buffer_), "LD:{} Char:{}", ld_val, char_val);
    std::string expected = fmt::format("LD:{} Char:{}", ld_val, char_val);
    ASSERT_EQ(fmt::to_string(buffer_), expected);
}