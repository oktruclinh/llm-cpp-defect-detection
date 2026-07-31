#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <limits>
#include <cstring>

// Test check_cstring_type_spec behavior
TEST(CheckCstringTypeSpecTest, ValidTypes) {
    // none and string should return true
    EXPECT_TRUE(fmt::detail::check_cstring_type_spec(fmt::presentation_type::none));
    EXPECT_TRUE(fmt::detail::check_cstring_type_spec(fmt::presentation_type::string));
    
    // pointer should return false
    EXPECT_FALSE(fmt::detail::check_cstring_type_spec(fmt::presentation_type::pointer));
}

TEST(CheckCstringTypeSpecTest, InvalidType) {
    // Should call on_error for invalid types
    auto error_handler = [](fmt::string_view msg) {
        EXPECT_STREQ("invalid type specifier", msg.data());
    };
    
    EXPECT_FALSE(fmt::detail::check_cstring_type_spec(fmt::presentation_type::hexfloat_lower, error_handler));
}

// Test check_string_type_spec behavior
TEST(CheckStringTypeSpecTest, ValidTypes) {
    // Should not throw for valid types
    EXPECT_NO_THROW(fmt::detail::check_string_type_spec(fmt::presentation_type::none));
    EXPECT_NO_THROW(fmt::detail::check_string_type_spec(fmt::presentation_type::string));
    EXPECT_NO_THROW(fmt::detail::check_string_type_spec(fmt::presentation_type::debug));
}

TEST(CheckStringTypeSpecTest, InvalidType) {
    // Should call on_error for invalid types
    auto error_handler = [](fmt::string_view msg) {
        EXPECT_STREQ("invalid type specifier", msg.data());
    };
    
    EXPECT_NO_THROW(fmt::detail::check_string_type_spec(fmt::presentation_type::pointer, error_handler));
}

// Test check_pointer_type_spec behavior
TEST(CheckPointerTypeSpecTest, ValidTypes) {
    // Should not throw for valid types
    auto error_handler = [](fmt::string_view msg) {
        FAIL() << "Unexpected error: " << msg.data();
    };
    
    EXPECT_NO_THROW(fmt::detail::check_pointer_type_spec(fmt::presentation_type::none, error_handler));
    EXPECT_NO_THROW(fmt::detail::check_pointer_type_spec(fmt::presentation_type::pointer, error_handler));
}

TEST(CheckPointerTypeSpecTest, InvalidType) {
    // Should call on_error for invalid types
    bool error_called = false;
    auto error_handler = [&error_called](fmt::string_view msg) {
        error_called = true;
        EXPECT_STREQ("invalid type specifier", msg.data());
    };
    
    EXPECT_NO_THROW(fmt::detail::check_pointer_type_spec(fmt::presentation_type::string, error_handler));
    EXPECT_TRUE(error_called);
}

// Test specs_checker with various argument types
TEST(SpecsCheckerTest, NumericArgumentRequired) {
    // Test with non-numeric argument type
    auto handler = fmt::detail::specs_checker<fmt::detail::error_handler>(
        fmt::detail::error_handler(), fmt::detail::type::string_type);
    
    // on_align with numeric alignment should fail for non-numeric types
    EXPECT_ANY_THROW(handler.on_align(fmt::align::numeric));
    
    // on_align with non-numeric alignment should work
    EXPECT_NO_THROW(handler.on_align(fmt::align::left));
}

TEST(SpecsCheckerTest, SignRequiresNumeric) {
    // Test with non-numeric argument type
    auto handler = fmt::detail::specs_checker<fmt::detail::error_handler>(
        fmt::detail::error_handler(), fmt::detail::type::string_type);
    
    // on_sign should fail for non-numeric types
    EXPECT_ANY_THROW(handler.on_sign(fmt::sign::plus));
}

TEST(SpecsCheckerTest, SignRequiresSigned) {
    // Test with unsigned integer type
    auto handler = fmt::detail::specs_checker<fmt::detail::error_handler>(
        fmt::detail::error_handler(), fmt::detail::type::uint_type);
    
    // on_sign should fail for unsigned types
    EXPECT_ANY_THROW(handler.on_sign(fmt::sign::plus));
}

TEST(SpecsCheckerTest, SignWorksWithSigned) {
    // Test with signed integer type
    auto handler = fmt::detail::specs_checker<fmt::detail::error_handler>(
        fmt::detail::error_handler(), fmt::detail::type::int_type);
    
    // on_sign should work for signed types
    EXPECT_NO_THROW(handler.on_sign(fmt::sign::plus));
}

TEST(SpecsCheckerTest, HashRequiresNumeric) {
    // Test with non-numeric argument type
    auto handler = fmt::detail::specs_checker<fmt::detail::error_handler>(
        fmt::detail::error_handler(), fmt::detail::type::string_type);
    
    // on_hash should fail for non-numeric types
    EXPECT_ANY_THROW(handler.on_hash());
}

TEST(SpecsCheckerTest, HashWorksWithNumeric) {
    // Test with numeric argument type
    auto handler = fmt::detail::specs_checker<fmt::detail::error_handler>(
        fmt::detail::error_handler(), fmt::detail::type::int_type);
    
    // on_hash should work for numeric types
    EXPECT_NO_THROW(handler.on_hash());
}

TEST(SpecsCheckerTest, LocalizedRequiresNumeric) {
    // Test with non-numeric argument type
    auto handler = fmt::detail::specs_checker<fmt::detail::error_handler>(
        fmt::detail::error_handler(), fmt::detail::type::string_type);
    
    // on_localized should fail for non-numeric types
    EXPECT_ANY_THROW(handler.on_localized());
}

TEST(SpecsCheckerTest, ZeroRequiresNumeric) {
    // Test with non-numeric argument type
    auto handler = fmt::detail::specs_checker<fmt::detail::error_handler>(
        fmt::detail::error_handler(), fmt::detail::type::string_type);
    
    // on_zero should fail for non-numeric types
    EXPECT_ANY_THROW(handler.on_zero());
}

TEST(SpecsCheckerTest, EndPrecisionForIntegral) {
    // Test with integral type
    auto handler = fmt::detail::specs_checker<fmt::detail::error_handler>(
        fmt::detail::error_handler(), fmt::detail::type::int_type);
    
    // end_precision should fail for integral types
    EXPECT_ANY_THROW(handler.end_precision());
}

TEST(SpecsCheckerTest, EndPrecisionForPointer) {
    // Test with pointer type
    auto handler = fmt::detail::specs_checker<fmt::detail::error_handler>(
        fmt::detail::error_handler(), fmt::detail::type::pointer_type);
    
    // end_precision should fail for pointer types
    EXPECT_ANY_THROW(handler.end_precision());
}

TEST(SpecsCheckerTest, EndPrecisionForNonIntegral) {
    // Test with floating point type
    auto handler = fmt::detail::specs_checker<fmt::detail::error_handler>(
        fmt::detail::error_handler(), fmt::detail::type::double_type);
    
    // end_precision should work for non-integral, non-pointer types
    EXPECT_NO_THROW(handler.end_precision());
}

// Test invalid_arg_index constant
TEST(InvalidArgIndexTest, ConstantValue) {
    EXPECT_EQ(-1, fmt::detail::invalid_arg_index);
}

// Test that the public API works correctly with format specifiers
TEST(FormatSpecIntegrationTest, StringFormatting) {
    EXPECT_EQ("hello", fmt::format("{:s}", "hello"));
    EXPECT_EQ("hello", fmt::format("{}", "hello"));
}

TEST(FormatSpecIntegrationTest, NumericFormatting) {
    EXPECT_EQ("42", fmt::format("{:d}", 42));
    EXPECT_EQ("2a", fmt::format("{:x}", 42));
    EXPECT_EQ("2A", fmt::format("{:X}", 42));
}

TEST(FormatSpecIntegrationTest, PrecisionWithFloatingPoint) {
    EXPECT_EQ("3.14", fmt::format("{:.2f}", 3.14159));
    EXPECT_EQ("3.1416", fmt::format("{:.4f}", 3.14159));
}

TEST(FormatSpecIntegrationTest, PointerFormatting) {
    int x = 42;
    std::string result = fmt::format("{}", fmt::ptr(&x));
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.substr(0, 2), "0x");
}