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
    // Test normal character values
    std::string buffer;
    fmt::format_to(std::back_inserter(buffer), "{}", 'A');
    EXPECT_EQ(buffer, "A");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", 'z');
    EXPECT_EQ(buffer, "z");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", '\0');
    EXPECT_EQ(buffer.size(), 1u);
    EXPECT_EQ(buffer[0], '\0');
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", ' ');
    EXPECT_EQ(buffer, " ");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", '\n');
    EXPECT_EQ(buffer, "\n");
}

TEST(FmtFormatTest, WriteWideChar) {
    // Test wide character handling (should work with wchar_t char_type)
    std::wstring buffer;
    fmt::format_to(std::back_inserter(buffer), L"{}", L'A');
    EXPECT_EQ(buffer, L"A");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), L"{}", L'z');
    EXPECT_EQ(buffer, L"z");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), L"{}", L'\0');
    EXPECT_EQ(buffer.size(), 1u);
    EXPECT_EQ(buffer[0], L'\0');
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), L"{}", L' ');
    EXPECT_EQ(buffer, L" ");
}

TEST(FmtFormatTest, WriteLongDoublePrecision) {
    // Test precision handling for long double
    std::string buffer;
    fmt::format_to(std::back_inserter(buffer), "{:.2L}", 3.14159L);
    EXPECT_EQ(buffer, "3.14");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{:.0L}", 3.14159L);
    EXPECT_EQ(buffer, "3");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{:.10L}", 1.0L);
    EXPECT_EQ(buffer, "1.0000000000");
}

TEST(FmtFormatTest, WriteCharEdgeCases) {
    // Test character boundaries
    std::string buffer;
    
    // ASCII range
    for (char c = 0; c <= 127; ++c) {
        buffer.clear();
        fmt::format_to(std::back_inserter(buffer), "{}", c);
        EXPECT_EQ(buffer.size(), 1u);
        EXPECT_EQ(buffer[0], c);
    }
    
    // Test some specific edge cases
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", static_cast<char>(128));
    EXPECT_EQ(buffer.size(), 1u);
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", static_cast<char>(255));
    EXPECT_EQ(buffer.size(), 1u);
}