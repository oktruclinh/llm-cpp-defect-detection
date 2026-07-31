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
    fmt::format_to(std::back_inserter(buffer), "{}", -1.5L);
    EXPECT_EQ(buffer, "-1.5");
    
    // Test edge cases
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<long double>::infinity());
    EXPECT_EQ(buffer, "inf");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -std::numeric_limits<long double>::infinity());
    EXPECT_EQ(buffer, "-inf");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<long double>::quiet_NaN());
    EXPECT_EQ(buffer, "nan");
}

TEST(FmtFormatTest, WriteChar) {
    // Test normal character
    std::string buffer;
    fmt::format_to(std::back_inserter(buffer), "{}", 'A');
    EXPECT_EQ(buffer, "A");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", 'z');
    EXPECT_EQ(buffer, "z");
    
    // Test special characters
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", '\0');
    EXPECT_EQ(buffer.size(), 1);
    EXPECT_EQ(buffer[0], '\0');
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", '\n');
    EXPECT_EQ(buffer, "\n");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", ' ');
    EXPECT_EQ(buffer, " ");
}

TEST(FmtFormatTest, WriteWideChar) {
    // Test wide character (only valid when char_type is wchar_t)
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
    fmt::format_to(std::back_inserter(buffer), L"{}", L' ');
    EXPECT_EQ(buffer, L" ");
}

TEST(FmtFormatTest, WriteDouble) {
    // Test normal double values
    std::string buffer;
    fmt::format_to(std::back_inserter(buffer), "{}", 3.14159);
    EXPECT_EQ(buffer, "3.14159");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", 0.0);
    EXPECT_EQ(buffer, "0");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -1.5);
    EXPECT_EQ(buffer, "-1.5");
    
    // Test edge cases
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(buffer, "inf");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -std::numeric_limits<double>::infinity());
    EXPECT_EQ(buffer, "-inf");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<double>::quiet_NaN());
    EXPECT_EQ(buffer, "nan");
}

TEST(FmtFormatTest, WriteFloat) {
    // Test normal float values
    std::string buffer;
    fmt::format_to(std::back_inserter(buffer), "{}", 3.14159f);
    EXPECT_EQ(buffer, "3.14159");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", 0.0f);
    EXPECT_EQ(buffer, "0");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -1.5f);
    EXPECT_EQ(buffer, "-1.5");
    
    // Test edge cases
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<float>::infinity());
    EXPECT_EQ(buffer, "inf");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -std::numeric_limits<float>::infinity());
    EXPECT_EQ(buffer, "-inf");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<float>::quiet_NaN());
    EXPECT_EQ(buffer, "nan");
}