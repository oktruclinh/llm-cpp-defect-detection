#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/core.h> // For fmt::format_string and other core functionalities

// This test suite focuses on the alignment and sign handling logic
// within fmt::format, specifically the part of the code that adjusts
// the alignment specification based on the presence of a sign and
// the initial alignment.

// Helper function to format a value with a given format string
// and return the result. This allows us to test the internal logic
// through the public API.
template <typename T>
std::string format_value(const char* format_str, T value) {
    // fmt::runtime is in fmt/compile.h or fmt/format.h depending on version/context.
    // For basic runtime format strings, fmt::format directly accepts const char*.
    return fmt::format(format_str, value);
}

TEST(FmtAlignSignTest, NumericAlignmentWithPositiveSign) {
    // ALIGN_NUMERIC with a positive sign (implicitly handled by the format specifier)
    // Expected: sign is not explicitly added, alignment remains numeric (right for numbers)
    // The focal code snippet handles the 'sign' variable, which is set internally
    // based on the format specifier (e.g., '+', ' ').
    // For positive numbers, if '+' is used, 'sign' will be '+'.
    // If ' ' is used, 'sign' will be ' '.
    // If no sign specifier, 'sign' will be 0.

    // Test with '+' sign specifier
    EXPECT_EQ(format_value("{:+5}", 123), " +123"); // width 5, sign takes 1 char, 4 for number
    EXPECT_EQ(format_value("{:+05}", 123), "+0123"); // zero padding, sign takes 1 char
    EXPECT_EQ(format_value("{:+}", 123), "+123");

    // Test with ' ' sign specifier
    EXPECT_EQ(format_value("{: 5}", 123), "  123"); // width 5, sign takes 1 char (space), 4 for number
    EXPECT_EQ(format_value("{: 05}", 123), " 0123"); // zero padding, sign takes 1 char (space)
    EXPECT_EQ(format_value("{: }", 123), " 123");
}

TEST(FmtAlignSignTest, NumericAlignmentWithNegativeSign) {
    // ALIGN_NUMERIC with a negative sign (implicitly handled)
    // Expected: sign is part of the number, alignment remains numeric (right for numbers)
    // The focal code snippet's 'sign' variable would be '-' in this case.
    // The 'if (sign)' block should execute, decrementing width if present,
    // and then 'as.align_ = ALIGN_RIGHT;' is set.
    EXPECT_EQ(format_value("{:5}", -123), " -123"); // width 5, sign takes 1 char, 4 for number
    EXPECT_EQ(format_value("{:05}", -123), "-0123"); // zero padding, sign takes 1 char
    EXPECT_EQ(format_value("{}", -123), "-123");

    // Test with '+' sign specifier for negative number (should still show '-')
    EXPECT_EQ(format_value("{:+5}", -123), " -123");
    EXPECT_EQ(format_value("{:+05}", -123), "-0123");

    // Test with ' ' sign specifier for negative number (should still show '-')
    EXPECT_EQ(format_value("{: 5}", -123), " -123");
    EXPECT_EQ(format_value("{: 05}", -123), "-0123");
}

TEST(FmtAlignSignTest, NumericAlignmentNoSignSpecifier) {
    // ALIGN_NUMERIC without explicit sign specifier for positive numbers
    // Expected: no sign, alignment remains numeric (right for numbers)
    // 'sign' variable would be 0. The 'if (sign)' block should not execute.
    EXPECT_EQ(format_value("{:5}", 123), "  123");
    EXPECT_EQ(format_value("{:05}", 123), "00123");
    EXPECT_EQ(format_value("{}", 123), "123");
}

TEST(FmtAlignSignTest, DefaultAlignmentWithSign) {
    // ALIGN_DEFAULT (no explicit alignment specifier) with a sign
    // Expected: alignment becomes ALIGN_RIGHT, sign is handled.
    // The 'else' block is taken. 'as.align_' is set to ALIGN_RIGHT.
    // '++n' (which corresponds to the internal 'size_t n' for string length)
    // is incremented if 'sign' is present.
    // This means the sign character is counted towards the total width.

    // Positive with '+'
    EXPECT_EQ(format_value("{:+5}", 123), " +123"); // Default alignment is right for numbers
    EXPECT_EQ(format_value("{:+}", 123), "+123");

    // Positive with ' '
    EXPECT_EQ(format_value("{: 5}", 123), "  123");
    EXPECT_EQ(format_value("{: }", 123), " 123");

    // Negative (sign is always present)
    EXPECT_EQ(format_value("{:5}", -123), " -123");
    EXPECT_EQ(format_value("{}", -123), "-123");
}

TEST(FmtAlignSignTest, DefaultAlignmentNoSign) {
    // ALIGN_DEFAULT (no explicit alignment specifier) without a sign
    // Expected: alignment becomes ALIGN_RIGHT, no sign.
    // The 'else' block is taken. 'as.align_' is set to ALIGN_RIGHT.
    // 'if (sign)' is false, so '++n' does not execute.
    EXPECT_EQ(format_value("{:5}", 123), "  123");
    EXPECT_EQ(format_value("{}", 123), "123");
}

TEST(FmtAlignSignTest, LeftAlignmentWithSign) {
    // ALIGN_LEFT with a sign
    // Expected: sign is printed, alignment is left.
    // The 'else' block is taken. 'as.align_' is NOT changed from ALIGN_LEFT.
    // '++n' is incremented if 'sign' is present.
    EXPECT_EQ(format_value("{:<+5}", 123), "+123 "); // sign is part of the left-aligned string
    EXPECT_EQ(format_value("{:< 5}", 123), " 123 ");
    EXPECT_EQ(format_value("{:<5}", -123), "-123 ");
}

TEST(FmtAlignSignTest, LeftAlignmentNoSign) {
    // ALIGN_LEFT without a sign
    // Expected: no sign, alignment is left.
    // The 'else' block is taken. 'as.align_' is NOT changed.
    // 'if (sign)' is false.
    EXPECT_EQ(format_value("{:<5}", 123), "123  ");
}

TEST(FmtAlignSignTest, CenterAlignmentWithSign) {
    // ALIGN_CENTER with a sign
    // Expected: sign is printed, alignment is center.
    // The 'else' block is taken. 'as.align_' is NOT changed.
    // '++n' is incremented if 'sign' is present.
    EXPECT_EQ(format_value("{:^+5}", 123), "+123 "); // Center alignment for 5 width, +123 is 4 chars
    EXPECT_EQ(format_value("{:^ 5}", 123), " 123 ");
    EXPECT_EQ(format_value("{:^5}", -123), "-123 ");
}

TEST(FmtAlignSignTest, CenterAlignmentNoSign) {
    // ALIGN_CENTER without a sign
    // Expected: no sign, alignment is center.
    // The 'else' block is taken. 'as.align_' is NOT changed.
    // 'if (sign)' is false.
    EXPECT_EQ(format_value("{:^5}", 123), " 123 ");
}

TEST(FmtAlignSignTest, ZeroWidth) {
    // Test with zero width, sign handling should still be correct.
    EXPECT_EQ(format_value("{:+0}", 123), "+123");
    EXPECT_EQ(format_value("{: 0}", 123), " 123");
    EXPECT_EQ(format_value("{:0}", -123), "-123");
    EXPECT_EQ(format_value("{:0}", 123), "123"); // No sign, no padding
}

TEST(FmtAlignSignTest, WidthOne) {
    // Test with width 1, which is an edge case for padding.
    // For numeric alignment, if sign is present, width is decremented to 0.
    // This means the sign takes up the entire width, or the number overflows.

    // Positive with '+'
    // Width 1, sign takes 1 char. Number '1' cannot fit.
    // fmtlib typically expands width if content is larger.
    EXPECT_EQ(format_value("{:+1}", 1), "+1");
    EXPECT_EQ(format_value("{:+1}", 123), "+123"); // Width is ignored if too small

    // Negative
    EXPECT_EQ(format_value("{:1}", -1), "-1");
    EXPECT_EQ(format_value("{:1}", -123), "-123");

    // No sign
    EXPECT_EQ(format_value("{:1}", 1), "1");
}

TEST(FmtAlignSignTest, LargeWidth) {
    // Ensure behavior is consistent with large widths.
    EXPECT_EQ(format_value("{:+10}", 123), "      +123"); // Numeric alignment, right
    EXPECT_EQ(format_value("{:<+10}", 123), "+123      "); // Left alignment
    EXPECT_EQ(format_value("{:^+10}", 123), "   +123   "); // Center alignment
}

TEST(FmtAlignSignTest, DifferentTypes) {
    // The focal function logic is primarily about integer formatting,
    // but ensure it doesn't break for other types if they pass through.
    // Floats also use numeric alignment by default.
    EXPECT_EQ(format_value("{:+10.2f}", 123.45), "    +123.45");
    EXPECT_EQ(format_value("{: 10.2f}", 123.45), "     123.45");
    EXPECT_EQ(format_value("{:10.2f}", -123.45), "   -123.45");

    // String formatting does not typically use ALIGN_NUMERIC,
    // so the 'else' branch should be taken, and sign handling
    // (if a custom sign were somehow introduced) would be different.
    // However, standard string formatting doesn't have a 'sign' concept.
    EXPECT_EQ(format_value("{:10}", "test"), "      test"); // Default right for strings
    EXPECT_EQ(format_value("{:<10}", "test"), "test      ");
}

TEST(FmtAlignSignTest, CustomFillChar) {
    // Custom fill characters should work correctly with alignment and sign.
    EXPECT_EQ(format_value("{:*<+10}", 123), "+123******"); // Left align, sign included
    EXPECT_EQ(format_value("{:*>+10}", 123), "******+123"); // Right align, sign included
    EXPECT_EQ(format_value("{:*^+10}", 123), "***+123***"); // Center align, sign included

    // Numeric alignment with custom fill char (only works with zero padding for numbers)
    // The '0' fill character is special for numbers.
    EXPECT_EQ(format_value("{:0+10}", 123), "+000000123"); // Numeric alignment, '0' fill
    EXPECT_EQ(format_value("{:0 10}", 123), " 000000123"); // Numeric alignment, '0' fill
    EXPECT_EQ(format_value("{:010}", -123), "-000000123"); // Numeric alignment, '0' fill
}