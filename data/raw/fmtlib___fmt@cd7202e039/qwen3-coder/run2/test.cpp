#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <chrono>
#include <string>

TEST(ChronoTest, WriteFractionalSeconds) {
  // Test normal case with fractional seconds
  auto time_point = std::chrono::system_clock::from_time_t(1234567890);
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
      time_point.time_since_epoch());
  
  std::string result = fmt::format("{:%S}", duration);
  // Should contain seconds and fractional part
  EXPECT_NE(result.find('.'), std::string::npos);
  EXPECT_TRUE(result.length() > 2); // At least "30." + fractional part
  
  // Test with zero fractional seconds
  auto zero_duration = std::chrono::seconds(42);
  std::string zero_result = fmt::format("{:%S}", zero_duration);
  EXPECT_EQ(zero_result, "42");
  
  // Test with exactly one second
  auto one_second = std::chrono::seconds(1);
  std::string one_result = fmt::format("{:%S}", one_second);
  EXPECT_EQ(one_result, "01");
  
  // Test with subsecond precision
  auto subsecond = std::chrono::milliseconds(123);
  std::string subsecond_result = fmt::format("{:%S}", subsecond);
  EXPECT_NE(subsecond_result.find('.'), std::string::npos);
  EXPECT_TRUE(subsecond_result.length() > 4); // "00." + 3 digits
  
  // Test with microsecond precision
  auto microsecond = std::chrono::microseconds(567890);
  std::string microsecond_result = fmt::format("{:%S}", microsecond);
  EXPECT_NE(microsecond_result.find('.'), std::string::npos);
  EXPECT_TRUE(microsecond_result.length() > 4); // "00." + 6 digits
  
  // Test with nanosecond precision
  auto nanosecond = std::chrono::nanoseconds(123456789);
  std::string nanosecond_result = fmt::format("{:%S}", nanosecond);
  EXPECT_NE(nanosecond_result.find('.'), std::string::npos);
  EXPECT_TRUE(nanosecond_result.length() > 4); // "00." + 9 digits
  
  // Test boundary case - exactly 60 seconds (should wrap to next minute)
  auto sixty_seconds = std::chrono::seconds(60);
  std::string sixty_result = fmt::format("{:%S}", sixty_seconds);
  EXPECT_EQ(sixty_result, "00");
  
  // Test boundary case - negative time
  auto negative = std::chrono::seconds(-15);
  std::string negative_result = fmt::format("{:%S}", negative);
  EXPECT_EQ(negative_result, "45"); // -15 seconds = 45 seconds in next minute
}

TEST(ChronoTest, WriteFractionalSecondsEdgeCases) {
  // Test with very small fractional seconds
  auto tiny = std::chrono::nanoseconds(1);
  std::string tiny_result = fmt::format("{:%S}", tiny);
  EXPECT_NE(tiny_result.find('.'), std::string::npos);
  EXPECT_TRUE(tiny_result.length() > 2);
  
  // Test with very large fractional seconds
  auto large = std::chrono::hours(24);
  std::string large_result = fmt::format("{:%S}", large);
  EXPECT_EQ(large_result, "00");
  
  // Test with fractional seconds that round to whole number
  auto roundable = std::chrono::milliseconds(1000); // Exactly 1 second
  std::string roundable_result = fmt::format("{:%S}", roundable);
  EXPECT_EQ(roundable_result, "01");
  
  // Test with fractional seconds that have trailing zeros
  auto trailing_zeros = std::chrono::milliseconds(1200); // 1.2 seconds
  std::string trailing_result = fmt::format("{:%S}", trailing_zeros);
  EXPECT_NE(trailing_result.find('.'), std::string::npos);
  EXPECT_TRUE(trailing_result.length() > 3);
  
  // Test with zero duration
  auto zero = std::chrono::duration<double>(0.0);
  std::string zero_format = fmt::format("{:%S}", zero);
  EXPECT_EQ(zero_format, "00");
}