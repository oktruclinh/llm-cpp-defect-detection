#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <limits>

TEST(FmtFormatTest, WriteLongDouble) {
    // Test normal long double values
    std::string buffer;
    fmt::format_to(std::back_inserter(buffer), "{}", 3.14159L);
    EXPECT_EQ(buffer, "3.14159");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", 0.0L);
    EXPECT_EQ(buffer, "0");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -123.456L);
    EXPECT_EQ(buffer, "-123.456");
    
    // Test edge cases
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<long double>::infinity());
    EXPECT_TRUE(buffer == "inf" || buffer == "infinity");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -std::numeric_limits<long double>::infinity());
    EXPECT_TRUE(buffer == "-inf" || buffer == "-infinity");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<long double>::quiet_NaN());
    EXPECT_EQ(buffer, "nan");
}

TEST(FmtFormatTest, WriteChar) {
    // Test normal char values
    std::string buffer;
    fmt::format_to(std::back_inserter(buffer), "{}", 'A');
    EXPECT_EQ(buffer, "A");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", 'z');
    EXPECT_EQ(buffer, "z");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", '0');
    EXPECT_EQ(buffer, "0");
    
    // Test special characters
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", '\n');
    EXPECT_EQ(buffer, "\n");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", '\t');
    EXPECT_EQ(buffer, "\t");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", '\0');
    EXPECT_EQ(buffer, "\0");  // Null character should be preserved
}

TEST(FmtFormatTest, WriteWideChar) {
    // Test wide character handling (should work when char_type is wchar_t)
    std::wstring buffer;
    fmt::format_to(std::back_inserter(buffer), L"{}", L'A');
    EXPECT_EQ(buffer, L"A");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), L"{}", L'z');
    EXPECT_EQ(buffer, L"z");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), L"{}", L'\n');
    EXPECT_EQ(buffer, L"\n");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), L"{}", L'\t');
    EXPECT_EQ(buffer, L"\t");
}