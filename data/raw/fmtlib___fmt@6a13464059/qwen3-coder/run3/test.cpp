#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Mock handler to capture calls
class TestHandler {
public:
    bool sign_called = false;
    fmt::sign_t last_sign = fmt::sign_t::none;
    
    FMT_CONSTEXPR void on_sign(fmt::sign_t s) {
        sign_called = true;
        last_sign = s;
    }
    
    FMT_CONSTEXPR void on_error(const char* msg) {
        throw std::runtime_error(msg);
    }
};

// Mock formatter that inherits from the real formatter but overrides the handler
template<typename Char>
class MockFormatter : public fmt::basic_format_arg<Char>::formatter_type {
private:
    TestHandler handler_;
    fmt::type arg_type_;
    
public:
    MockFormatter(fmt::type type) : arg_type_(type) {}
    
    FMT_CONSTEXPR void require_numeric_argument() {}
    
    FMT_CONSTEXPR void on_sign(fmt::sign_t s) {
        // This mimics the exact logic from the focal function
        require_numeric_argument();
        if (fmt::is_integral_type(arg_type_) && 
            arg_type_ != fmt::type::int_type &&
            arg_type_ != fmt::type::long_long_type && 
            arg_type_ != fmt::type::int128_type &&
            arg_type_ != fmt::type::char_type) {
            handler_.on_error("format specifier requires signed argument");
        }
        handler_.on_sign(s);
    }
    
    bool sign_called() const { return handler_.sign_called; }
    fmt::sign_t last_sign() const { return handler_.last_sign; }
};

TEST(FmtCoreTest, OnSign_ValidSignedIntegers) {
    // Test with valid signed integer types that should not throw
    MockFormatter<char> formatter1(fmt::type::int_type);
    formatter1.on_sign(fmt::sign_t::plus);
    EXPECT_TRUE(formatter1.sign_called());
    EXPECT_EQ(formatter1.last_sign(), fmt::sign_t::plus);
    
    MockFormatter<char> formatter2(fmt::type::long_long_type);
    formatter2.on_sign(fmt::sign_t::minus);
    EXPECT_TRUE(formatter2.sign_called());
    EXPECT_EQ(formatter2.last_sign(), fmt::sign_t::minus);
    
    MockFormatter<char> formatter3(fmt::type::char_type);
    formatter3.on_sign(fmt::sign_t::space);
    EXPECT_TRUE(formatter3.sign_called());
    EXPECT_EQ(formatter3.last_sign(), fmt::sign_t::space);
}

TEST(FmtCoreTest, OnSign_InvalidUnsignedTypes) {
    // Test with unsigned integer types that should throw
    MockFormatter<char> formatter1(fmt::type::uint_type);
    EXPECT_THROW(formatter1.on_sign(fmt::sign_t::plus), std::runtime_error);
    
    MockFormatter<char> formatter2(fmt::type::ulong_type);
    EXPECT_THROW(formatter2.on_sign(fmt::sign_t::minus), std::runtime_error);
    
    MockFormatter<char> formatter3(fmt::type::ushort_type);
    EXPECT_THROW(formatter3.on_sign(fmt::sign_t::space), std::runtime_error);
}

TEST(FmtCoreTest, OnSign_NonIntegralTypes) {
    // Test with non-integral types that should not throw (they don't match the condition)
    MockFormatter<char> formatter1(fmt::type::float_type);
    formatter1.on_sign(fmt::sign_t::plus);
    EXPECT_TRUE(formatter1.sign_called());
    
    MockFormatter<char> formatter2(fmt::type::double_type);
    formatter2.on_sign(fmt::sign_t::minus);
    EXPECT_TRUE(formatter2.sign_called());
    
    MockFormatter<char> formatter3(fmt::type::cstring_type);
    formatter3.on_sign(fmt::sign_t::space);
    EXPECT_TRUE(formatter3.sign_called());
}

TEST(FmtCoreTest, OnSign_EmptyAndZeroValues) {
    // Test edge cases with zero values
    MockFormatter<char> formatter1(fmt::type::int_type);
    formatter1.on_sign(fmt::sign_t::none);
    EXPECT_TRUE(formatter1.sign_called());
    EXPECT_EQ(formatter1.last_sign(), fmt::sign_t::none);
    
    MockFormatter<char> formatter2(fmt::type::uint_type);
    EXPECT_THROW(formatter2.on_sign(fmt::sign_t::none), std::runtime_error);
}

TEST(FmtCoreTest, OnSign_ComprehensiveIntegration) {
    // Test integration with actual fmt library functionality
    try {
        // This should work - signed int formatting
        std::string result = fmt::format("{:+}", 42);
        EXPECT_EQ(result, "+42");
        
        // This should work - signed long long formatting  
        result = fmt::format("{:-}", static_cast<long long>(-123));
        EXPECT_EQ(result, "-123");
        
        // This should fail - unsigned int formatting with sign
        EXPECT_THROW(fmt::format("{:+}", static_cast<unsigned int>(42)), 
                    std::runtime_error);
    } catch (const std::runtime_error& e) {
        // Expected for the unsigned case
        EXPECT_STREQ(e.what(), "format specifier requires signed argument");
    }
}