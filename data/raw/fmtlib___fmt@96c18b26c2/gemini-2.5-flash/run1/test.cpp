#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <fmt/core.h> // For fmt::format

// This test suite focuses on the printf formatting of integer types,
// specifically exercising the path where 'value' is an integer and
// the '0' flag is used, which implies numeric alignment.

TEST(PrintfFormatterTest, IntegerZeroFlagAndAlignment) {
  // Test case 1: Basic integer with zero flag and default alignment
  // The '0' flag should pad with zeros, and default alignment is right.
  EXPECT_EQ(fmt::sprintf("%05d", 123), "00123");
  EXPECT_EQ(fmt::sprintf("%08d", -456), "-000456");

  // Test case 2: Zero flag with specified width, positive number
  EXPECT_EQ(fmt::sprintf("%010d", 12345), "0000012345");

  // Test case 3: Zero flag with specified width, negative number
  EXPECT_EQ(fmt::sprintf("%010d", -12345), "-000012345");

  // Test case 4: Zero flag with width smaller than number of digits
  // Width should be ignored, and the full number should be printed.
  EXPECT_EQ(fmt::sprintf("%02d", 12345), "12345");
  EXPECT_EQ(fmt::sprintf("%02d", -12345), "-12345");

  // Test case 5: Zero flag with explicit right alignment (should be redundant)
  EXPECT_EQ(fmt::sprintf("%0>5d", 123), "00123"); // '0' flag takes precedence for padding
  EXPECT_EQ(fmt::sprintf("%0>8d", -456), "-000456");

  // Test case 6: Zero flag with explicit left alignment (should override '0' for padding char, but not for sign placement)
  // The '0' flag is typically for numeric types and implies padding with '0's
  // after the sign. Left alignment with '0' flag is a bit ambiguous in standard printf,
  // but fmtlib's behavior for %0-d is usually to ignore '0' and pad with spaces.
  // However, the focal function's logic specifically handles `fmt_specs.align == align::numeric`
  // and sets it to `align::right` if `align::none`.
  // Let's test what happens when left alignment is explicitly requested with '0'.
  // Standard printf behavior for "%0-5d" is usually "-   123" (padded with spaces).
  // fmtlib's printf implementation might differ slightly or follow a specific interpretation.
  // Based on the focal function, if '0' flag is present, and align is 'none' or 'numeric',
  // it becomes 'right'. If align is explicitly 'left', it should remain 'left'.
  // However, the '0' flag for padding is usually ignored with left alignment.
  EXPECT_EQ(fmt::sprintf("%0-5d", 123), "123  "); // '0' flag is ignored for padding char with left alignment
  EXPECT_EQ(fmt::sprintf("%0-8d", -456), "-456    ");

  // Test case 7: Zero flag with explicit center alignment
  EXPECT_EQ(fmt::sprintf("%0^7d", 123), " 123   "); // '0' flag is ignored for padding char with center alignment
  EXPECT_EQ(fmt::sprintf("%0^9d", -456), " -456   ");

  // Test case 8: Zero value with zero flag
  EXPECT_EQ(fmt::sprintf("%05d", 0), "00000");
  EXPECT_EQ(fmt::sprintf("%01d", 0), "0");

  // Test case 9: Largest/smallest int values with zero flag
  EXPECT_EQ(fmt::sprintf("%015d", std::numeric_limits<int>::max()), fmt::format("{:015d}", std::numeric_limits<int>::max()));
  EXPECT_EQ(fmt::sprintf("%015d", std::numeric_limits<int>::min()), fmt::format("{:015d}", std::numeric_limits<int>::min()));

  // Test case 10: Zero flag with plus sign
  EXPECT_EQ(fmt::sprintf("%0+5d", 123), "+0123");
  EXPECT_EQ(fmt::sprintf("%0+5d", -123), "-0123"); // Plus sign is ignored for negative numbers

  // Test case 11: Zero flag with space sign
  EXPECT_EQ(fmt::sprintf("%0 5d", 123), " 0123");
  EXPECT_EQ(fmt::sprintf("%0 5d", -123), "-0123"); // Space sign is ignored for negative numbers

  // Test case 12: Zero flag with alternative form ('#') - should have no effect on integers
  EXPECT_EQ(fmt::sprintf("%0#5d", 123), "00123");
}

TEST(PrintfFormatterTest, IntegerNoZeroFlagAndAlignment) {
  // Test case 1: Basic integer with default alignment (right)
  EXPECT_EQ(fmt::sprintf("%5d", 123), "  123");
  EXPECT_EQ(fmt::sprintf("%8d", -456), "    -456");

  // Test case 2: Explicit left alignment
  EXPECT_EQ(fmt::sprintf("%-5d", 123), "123  ");
  EXPECT_EQ(fmt::sprintf("%-8d", -456), "-456    ");

  // Test case 3: Explicit center alignment
  EXPECT_EQ(fmt::sprintf("%^7d", 123), " 123   ");
  EXPECT_EQ(fmt::sprintf("%^9d", -456), " -456   ");

  // Test case 4: Width smaller than number of digits
  EXPECT_EQ(fmt::sprintf("%2d", 12345), "12345");
  EXPECT_EQ(fmt::sprintf("%2d", -12345), "-12345");

  // Test case 5: Plus sign
  EXPECT_EQ(fmt::sprintf("%+5d", 123), " +123");
  EXPECT_EQ(fmt::sprintf("%+5d", -123), " -123");

  // Test case 6: Space sign
  EXPECT_EQ(fmt::sprintf("% 5d", 123), "  123");
  EXPECT_EQ(fmt::sprintf("% 5d", -123), " -123");
}

// Test cases for non-integer types to ensure the integer-specific logic
// (like '0' flag handling and align::numeric to align::right conversion)
// does not incorrectly apply to them.
TEST(PrintfFormatterTest, NonIntegerTypesZeroFlagIgnored) {
  // For non-numeric types, the '0' flag should be ignored, and padding should be spaces.
  // The focal function's `if (fmt_specs.align == align::none || fmt_specs.align == align::numeric)`
  // block should not be entered for non-numeric types, or if it is, the `0` flag
  // should not result in zero padding.

  // Character type
  EXPECT_EQ(fmt::sprintf("%05c", 'A'), "    A"); // '0' flag ignored, padded with spaces
  EXPECT_EQ(fmt::sprintf("%0-5c", 'A'), "A    "); // '0' flag ignored, left aligned

  // String type
  EXPECT_EQ(fmt::sprintf("%010s", "test"), "      test"); // '0' flag ignored, padded with spaces
  EXPECT_EQ(fmt::sprintf("%0-10s", "test"), "test      "); // '0' flag ignored, left aligned

  // Pointer type (address)
  void* ptr = reinterpret_cast<void*>(0x1234);
  // The exact output for pointers can vary, but the padding character should be space, not '0'.
  // We'll compare against fmt::format which is known to be correct for this.
  EXPECT_EQ(fmt::sprintf("%010p", ptr), fmt::format("{:10p}", ptr));
  EXPECT_NE(fmt::sprintf("%010p", ptr).find('0'), 2); // Should not be padded with '0's
}

// Test cases for edge values of integers
TEST(PrintfFormatterTest, IntegerEdgeValues) {
  // Max int
  EXPECT_EQ(fmt::sprintf("%020d", std::numeric_limits<int>::max()),
            fmt::format("{:020d}", std::numeric_limits<int>::max()));
  EXPECT_EQ(fmt::sprintf("%20d", std::numeric_limits<int>::max()),
            fmt::format("{:20d}", std::numeric_limits<int>::max()));

  // Min int
  EXPECT_EQ(fmt::sprintf("%020d", std::numeric_limits<int>::min()),
            fmt::format("{:020d}", std::numeric_limits<int>::min()));
  EXPECT_EQ(fmt::sprintf("%20d", std::numeric_limits<int>::min()),
            fmt::format("{:20d}", std::numeric_limits<int>::min()));

  // Zero
  EXPECT_EQ(fmt::sprintf("%05d", 0), "00000");
  EXPECT_EQ(fmt::sprintf("%5d", 0), "    0");
}