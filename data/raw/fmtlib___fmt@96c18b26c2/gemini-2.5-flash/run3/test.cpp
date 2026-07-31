#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <fmt/core.h> // For fmt::format

// This test suite focuses on the formatting of integral types using printf-style formatting,
// specifically exercising the path where '0' flag is ignored for non-numeric types
// and align::numeric is overwritten.

TEST(PrintfFormatIntegralTest, ZeroFlagWithNonNumericAlign) {
  // Test case: '0' flag with a non-numeric type (implicitly, as the focal function
  // handles the conversion to int for the 'd' specifier).
  // The key here is to observe the behavior when fmt_specs.align is align::numeric
  // and the '0' flag is present. The focal function should change align::numeric
  // to align::right if the '0' flag is ignored.

  // Scenario 1: Positive integer with '0' flag and default alignment (which is align::none)
  // The focal function's logic for '0' flag and align::numeric is not directly hit here
  // because the '0' flag is for numeric types, and the initial align is 'none'.
  // However, it's a baseline to ensure '0' flag works as expected for numeric types.
  EXPECT_EQ(fmt::sprintf("%05d", 123), "00123");
  EXPECT_EQ(fmt::sprintf("%05d", -123), "-0123"); // '0' flag applies after sign

  // Scenario 2: Positive integer with '0' flag and align::numeric (which is implicitly set by '0' flag)
  // The focal function's logic for '0' flag and align::numeric is implicitly handled.
  // The '0' flag implies numeric alignment.
  EXPECT_EQ(fmt::sprintf("%05d", 123), "00123");

  // Scenario 3: Test with a value that would trigger the '0' flag logic
  // and the subsequent alignment adjustment if it were a non-numeric type.
  // Since we are testing the 'd' specifier, it's always numeric.
  // The focal function's specific branch `if (fmt_specs.align == align::none || fmt_specs.align == align::numeric)`
  // and `fmt_specs.align = align::right;` is for cases where '0' flag is ignored.
  // For 'd', 'i', 'u', 'x', 'X', 'o' specifiers, '0' flag is NOT ignored.
  // Therefore, align::numeric should persist, and padding should be with '0'.

  // To truly test the branch where '0' flag is ignored and align::numeric becomes align::right,
  // we would need to use a non-numeric type specifier (like 's' for string, 'c' for char, 'p' for pointer)
  // with the '0' flag. However, the focal function snippet is within a context
  // that seems to be handling an integer conversion (`static_cast<int>(value)`).
  // Let's assume the context is for integer formatting, and the `else` branch
  // (not shown) would handle non-numeric types.

  // Re-evaluating the focal function:
  // `return (*this)(static_cast<int>(value));` -> This line suggests the current path is for integer conversion.
  // `fmt_specs.sign = sign::none;`
  // `fmt_specs.alt = false;`
  // `if (fmt_specs.align == align::none || fmt_specs.align == align::numeric)`
  //   `fmt_specs.align = align::right;`
  // `return base::operator()(value);`
  // `} else {`

  // This snippet is tricky. It looks like it's part of a larger `operator()` overload
  // or a method that handles different format specifiers.
  // The `return (*this)(static_cast<int>(value));` suggests a recursive call for integer formatting.
  // The subsequent lines (setting sign, alt, and adjusting align) seem to be for a *different* path
  // within the same function, likely for a non-numeric type *after* the integer conversion path.
  // This implies the focal function is a dispatcher or a complex handler.

  // Given the snippet, the most direct interpretation is that if the initial `return (*this)(static_cast<int>(value));`
  // is NOT taken (e.g., if the specifier is not 'd', 'i', etc., but something else that the `else` branch handles),
  // then the `fmt_specs` are modified, and `base::operator()(value)` is called.
  // The comment "align::numeric needs to be overwritten here since the '0' flag is ignored for non-numeric types"
  // strongly suggests we need to test a non-numeric type with the '0' flag.

  // Let's assume the focal function is part of a generic `operator()` that dispatches
  // based on the format specifier. If the specifier is for an integer, it takes the first `return`.
  // If it's for a non-integer type (e.g., char, string, pointer), it proceeds to the
  // `fmt_specs` modification and `base::operator()(value)`.

  // Test case: Character with '0' flag.
  // The '0' flag is ignored for characters. Default alignment is right.
  // The focal function should ensure align::numeric (if set by '0' flag) becomes align::right.
  EXPECT_EQ(fmt::sprintf("%05c", 'A'), "    A"); // '0' flag ignored, right aligned, space padded
  EXPECT_EQ(fmt::sprintf("%0-5c", 'A'), "A    "); // '-' flag overrides '0' and sets left align

  // Test case: String with '0' flag.
  // The '0' flag is ignored for strings. Default alignment is right.
  EXPECT_EQ(fmt::sprintf("%05s", "hi"), "   hi"); // '0' flag ignored, right aligned, space padded
  EXPECT_EQ(fmt::sprintf("%0-5s", "hi"), "hi   "); // '-' flag overrides '0' and sets left align

  // Test case: Pointer with '0' flag.
  // The '0' flag is ignored for pointers. Default alignment is right.
  void* ptr = reinterpret_cast<void*>(0x1234);
  // The exact output for pointer depends on platform and fmtlib's internal pointer formatting.
  // We are interested in the padding character and alignment.
  // For pointers, '0' flag typically means zero-padding *if* it's treated as a numeric type.
  // However, the comment "the '0' flag is ignored for non-numeric types" implies
  // that for pointer (which is often treated as hex number but can be considered non-numeric in this context),
  // it should revert to space padding and right alignment.
  // Let's verify that '0' flag does not result in '0' padding for pointers.
  // The default printf behavior for %p with width is space padding.
  // Example: printf("%010p", (void*)0x123); -> "0x00000123" (often zero-padded by default for hex)
  // This is a subtle point. The focal function's comment implies a specific behavior.
  // If fmtlib treats %p as a numeric type where '0' flag is *not* ignored, then this test might fail.
  // If it treats %p as non-numeric where '0' flag *is* ignored, then it should be space-padded.

  // Let's test based on the assumption that for the purpose of the focal function's comment,
  // pointers are considered "non-numeric" in the context of '0' flag handling.
  // This means '0' flag should be ignored, and padding should be spaces.
  // However, standard C printf often zero-pads pointers.
  // fmtlib's printf implementation might follow standard C behavior.
  // If fmtlib's %p *does* zero-pad, then the focal function's comment might be misleading
  // or apply to a different internal path.

  // Let's test with a known behavior of fmt::sprintf for pointers.
  // fmt::sprintf("%p", ptr) usually produces "0x..." or similar.
  // fmt::sprintf("%10p", ptr) usually right-pads with spaces.
  // fmt::sprintf("%010p", ptr) usually zero-pads.
  // This suggests that for %p, the '0' flag is *not* ignored by fmtlib's printf.
  // This implies the focal function's `if (fmt_specs.align == align::none || fmt_specs.align == align::numeric)`
  // and `fmt_specs.align = align::right;` block is *not* hit for %p if it's zero-padded.

  // This makes the focal function's snippet context-dependent.
  // The most robust way to test is to assume the snippet is for types where '0' flag *is* ignored.
  // The most common such types are 'c' and 's'.

  // Let's re-focus on the `static_cast<int>(value)` part.
  // If the initial `return (*this)(static_cast<int>(value));` is taken,
  // then the subsequent lines are not executed.
  // If the *current* `operator()` is for a non-integer type, and it *doesn't* take the `static_cast<int>` path,
  // then it proceeds to modify `fmt_specs`.

  // Consider a scenario where `value` is an `int`, but the format specifier
  // is for a character (`%c`). In this case, `static_cast<int>(value)` would be redundant,
  // and the code would likely proceed to the `fmt_specs` modification.

  // Test with an integer value formatted as a character.
  // '0' flag is ignored for '%c'.
  EXPECT_EQ(fmt::sprintf("%05c", 65), "    A"); // 65 is 'A'
  EXPECT_EQ(fmt::sprintf("%05c", 97), "    a"); // 97 is 'a'

  // Test with an integer value formatted as a string (this is not standard printf, but fmtlib might allow it).
  // No, fmt::sprintf("%s", 123) is a type mismatch.
  // The focal function's `base::operator()(value)` implies it's handling the actual formatting.

  // The key assertion for the focal function's `if` block:
  // If `fmt_specs.align` is `align::numeric` (which is implicitly set by '0' flag for numeric types)
  // or `align::none` (default), AND the '0' flag is ignored (i.e., for non-numeric types),
  // THEN `fmt_specs.align` should become `align::right`.
  // This means for `%05c`, the padding should be spaces on the left, not zeros.

  // Let's confirm the behavior of fmt::sprintf for character with '0' flag.
  // If it produces "0000A", then the focal function's logic is not applied or is overridden.
  // If it produces "    A", then the focal function's logic is correctly applied.
  EXPECT_EQ(fmt::sprintf("%05c", 'A'), "    A"); // Confirms '0' flag is ignored, and it's right-aligned with spaces.
                                                // This implies the `if` block in the focal function was hit
                                                // and `fmt_specs.align` was set to `align::right`.

  // Test with explicit alignment flags combined with '0' flag for non-numeric.
  // '-' flag overrides '0' flag and sets left alignment.
  EXPECT_EQ(fmt::sprintf("%-05c", 'A'), "A    "); // Left align, space padded. '0' ignored.
  EXPECT_EQ(fmt::sprintf("%-05s", "hi"), "hi   "); // Left align, space padded. '0' ignored.

  // Test with '0' flag and width 0 (edge case).
  EXPECT_EQ(fmt::sprintf("%0c", 'A'), "A");
  EXPECT_EQ(fmt::sprintf("%0s", "hi"), "hi");

  // Test with '0' flag and width less than value length.
  EXPECT_EQ(fmt::sprintf("%02c", 'A'), "A");
  EXPECT_EQ(fmt::sprintf("%02s", "hi"), "hi");
}

// Additional tests to ensure other flags/specifiers behave as expected
// and to provide context for the focal function's behavior.
TEST(PrintfFormatIntegralTest, BasicIntegerFormatting) {
  EXPECT_EQ(fmt::sprintf("%d", 123), "123");
  EXPECT_EQ(fmt::sprintf("%d", -123), "-123");
  EXPECT_EQ(fmt::sprintf("%d", 0), "0");
  EXPECT_EQ(fmt::sprintf("%u", 123u), "123");
  EXPECT_EQ(fmt::sprintf("%x", 255), "ff");
  EXPECT_EQ(fmt::sprintf("%X", 255), "FF");
  EXPECT_EQ(fmt::sprintf("%o", 8), "10");
}

TEST(PrintfFormatIntegralTest, WidthAndPrecision) {
  EXPECT_EQ(fmt::sprintf("%5d", 123), "  123");
  EXPECT_EQ(fmt::sprintf("%-5d", 123), "123  ");
  EXPECT_EQ(fmt::sprintf("%+5d", 123), " +123");
  EXPECT_EQ(fmt::sprintf("% 5d", 123), "  123"); // Space flag for positive numbers
  EXPECT_EQ(fmt::sprintf("% 5d", -123), " -123");

  // Precision for integers specifies minimum number of digits.
  EXPECT_EQ(fmt::sprintf("%.3d", 1), "001");
  EXPECT_EQ(fmt::sprintf("%.3d", 1234), "1234"); // Precision ignored if shorter than value
  EXPECT_EQ(fmt::sprintf("%5.3d", 1), "  001");
  EXPECT_EQ(fmt::sprintf("%-5.3d", 1), "001  ");
}

TEST(PrintfFormatIntegralTest, SignFlags) {
  EXPECT_EQ(fmt::sprintf("%+d", 123), "+123");
  EXPECT_EQ(fmt::sprintf("%+d", -123), "-123");
  EXPECT_EQ(fmt::sprintf("% d", 123), " 123");
  EXPECT_EQ(fmt::sprintf("% d", -123), "-123");
  EXPECT_EQ(fmt::sprintf("% d", 0), " 0");
}

TEST(PrintfFormatIntegralTest, AlternateForm) {
  EXPECT_EQ(fmt::sprintf("%#x", 10), "0xa");
  EXPECT_EQ(fmt::sprintf("%#X", 10), "0XA");
  EXPECT_EQ(fmt::sprintf("%#o", 8), "010");
  EXPECT_EQ(fmt::sprintf("%#o", 0), "0"); // Special case for 0
  EXPECT_EQ(fmt::sprintf("%#x", 0), "0"); // No "0x" for 0
}

TEST(PrintfFormatIntegralTest, ZeroValue) {
  EXPECT_EQ(fmt::sprintf("%d", 0), "0");
  EXPECT_EQ(fmt::sprintf("%05d", 0), "00000");
  EXPECT_EQ(fmt::sprintf("%05c", 0), "    \0"); // Null character, padded with spaces
  EXPECT_EQ(fmt::sprintf("%05s", ""), "     "); // Empty string, padded with spaces
}

TEST(PrintfFormatIntegralTest, MaxIntValues) {
  EXPECT_EQ(fmt::sprintf("%d", std::numeric_limits<int>::max()), fmt::format("{}", std::numeric_limits<int>::max()));
  EXPECT_EQ(fmt::sprintf("%d", std::numeric_limits<int>::min()), fmt::format("{}", std::numeric_limits<int>::min()));
  EXPECT_EQ(fmt::sprintf("%u", std::numeric_limits<unsigned int>::max()), fmt::format("{}", std::numeric_limits<unsigned int>::max()));
}