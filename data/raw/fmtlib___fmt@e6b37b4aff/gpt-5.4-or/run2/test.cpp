#include <gtest/gtest.h>

#include <fmt/format.h>
#include <fmt/xchar.h>

#include <string>
#include <vector>

namespace {

std::string repeat_char(char c, std::size_t n) { return std::string(n, c); }

std::wstring repeat_wchar(wchar_t c, std::size_t n) {
  return std::wstring(n, c);
}

TEST(FmtFormatInlTranscodeRemainderTest, EmptyUtf8ToWideString) {
  std::wstring out = fmt::to_wstring(std::string());
  EXPECT_TRUE(out.empty());
}

TEST(FmtFormatInlTranscodeRemainderTest, EmptyWideToUtf8String) {
  std::string out = fmt::to_string(std::wstring());
  EXPECT_TRUE(out.empty());
}

TEST(FmtFormatInlTranscodeRemainderTest, AsciiRoundTripSmallLengthsIncludingRemainderBoundaries) {
  // Exercise sizes around the internal SIMD/block processing boundaries and
  // the tail-copy loop:
  // 0, 1, 2, 3, ..., plus values around 7/8 and 15/16 are especially useful
  // for implementations that process fixed-size blocks and then a remainder.
  const std::vector<std::size_t> lengths = {
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 14, 15, 16, 17, 31, 32, 33};

  for (std::size_t n : lengths) {
    std::string input = repeat_char('A', n);

    std::wstring wide = fmt::to_wstring(input);
    ASSERT_EQ(wide.size(), n) << "length=" << n;
    for (std::size_t i = 0; i < n; ++i) {
      EXPECT_EQ(wide[i], L'A') << "index=" << i << " length=" << n;
    }

    std::string roundtrip = fmt::to_string(wide);
    EXPECT_EQ(roundtrip, input) << "length=" << n;
  }
}

TEST(FmtFormatInlTranscodeRemainderTest, AsciiWithEmbeddedNulPreservedAcrossConversion) {
  const std::string input = std::string("ab\0cd\0ef", 8);

  std::wstring wide = fmt::to_wstring(input);
  ASSERT_EQ(wide.size(), input.size());
  EXPECT_EQ(wide[0], L'a');
  EXPECT_EQ(wide[1], L'b');
  EXPECT_EQ(wide[2], L'\0');
  EXPECT_EQ(wide[3], L'c');
  EXPECT_EQ(wide[4], L'd');
  EXPECT_EQ(wide[5], L'\0');
  EXPECT_EQ(wide[6], L'e');
  EXPECT_EQ(wide[7], L'f');

  std::string roundtrip = fmt::to_string(wide);
  EXPECT_EQ(roundtrip.size(), input.size());
  EXPECT_EQ(roundtrip, input);
}

TEST(FmtFormatInlTranscodeRemainderTest, MixedUtf8ContentRoundTripsWhenTailContainsMultibyteCodePoint) {
  // Choose lengths so the final characters are multibyte UTF-8 and likely land
  // in the remainder buffer path.
  const std::string input =
      "1234567"          // ASCII prefix
      u8"é"              // 2-byte UTF-8
      "ABCDEFG"          // more ASCII
      u8"€"              // 3-byte UTF-8
      "XYZ"
      u8"😀";            // 4-byte UTF-8 near the end

  std::wstring wide = fmt::to_wstring(input);
  std::string roundtrip = fmt::to_string(wide);

  EXPECT_EQ(roundtrip, input);
  EXPECT_FALSE(wide.empty());
}

TEST(FmtFormatInlTranscodeRemainderTest, SingleMultibyteCharactersConvertCorrectly) {
  struct Case {
    const char* utf8;
    std::size_t wchar_count;
  };

  const Case cases[] = {
      {u8"é", 1},
      {u8"€", 1},
      {u8"你", 1},
      {u8"😀", 1},
  };

  for (const auto& tc : cases) {
    std::string input = tc.utf8;
    std::wstring wide = fmt::to_wstring(input);
    EXPECT_EQ(wide.size(), tc.wchar_count) << "input=" << input;
    EXPECT_EQ(fmt::to_string(wide), input);
  }
}

TEST(FmtFormatInlTranscodeRemainderTest, LongMixedUtf8RoundTripsAcrossMultipleBlocksAndTail) {
  std::string input;
  for (int i = 0; i < 20; ++i) {
    input += "ABCDE";
    input += u8"é";
    input += "0123";
    input += u8"€";
    input += "xy";
    input += u8"😀";
    input += "Z";
  }

  std::wstring wide = fmt::to_wstring(input);
  std::string roundtrip = fmt::to_string(wide);

  EXPECT_EQ(roundtrip, input);
}

TEST(FmtFormatInlTranscodeRemainderTest, InvalidUtf8ThrowsFormatErrorForTruncatedSequenceAtEnd) {
  // Truncated UTF-8 sequence at the very end specifically exercises handling
  // of the final incomplete bytes in the tail buffer.
  const std::string invalid = std::string("abc", 3) + char(0xE2) + char(0x82);

  EXPECT_THROW(
      {
        try {
          (void)fmt::to_wstring(invalid);
        } catch (const fmt::format_error& e) {
          EXPECT_NE(std::string(e.what()).find("invalid utf8"), std::string::npos);
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtFormatInlTranscodeRemainderTest, InvalidUtf8ThrowsFormatErrorForLoneContinuationByteAtEnd) {
  const std::string invalid = std::string("abcdefg", 7) + char(0x80);

  EXPECT_THROW(
      {
        try {
          (void)fmt::to_wstring(invalid);
        } catch (const fmt::format_error& e) {
          EXPECT_NE(std::string(e.what()).find("invalid utf8"), std::string::npos);
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtFormatInlTranscodeRemainderTest, InvalidUtf8ThrowsFormatErrorForBadSequenceNearBlockBoundary) {
  // Put an invalid sequence after a prefix length that is likely to trigger
  // block processing before the tail handling.
  std::string invalid = repeat_char('a', 15);
  invalid.push_back(char(0xF0));
  invalid.push_back(char(0x28));
  invalid.push_back(char(0x8C));
  invalid.push_back(char(0x28));

  EXPECT_THROW((void)fmt::to_wstring(invalid), fmt::format_error);
}

TEST(FmtFormatInlTranscodeRemainderTest, WideAsciiToUtf8PreservesLengthAroundTailBoundaries) {
  const std::vector<std::size_t> lengths = {
      0, 1, 2, 3, 7, 8, 9, 15, 16, 17, 31, 32, 33};

  for (std::size_t n : lengths) {
    std::wstring input = repeat_wchar(L'Q', n);
    std::string out = fmt::to_string(input);

    ASSERT_EQ(out.size(), n) << "length=" << n;
    EXPECT_EQ(out, repeat_char('Q', n)) << "length=" << n;
  }
}

TEST(FmtFormatInlTranscodeRemainderTest, WideMixedUnicodeRoundTrips) {
  const std::wstring input = L"Hello \u00E9 \u20AC \u4F60\u597D";

  std::string utf8 = fmt::to_string(input);
  std::wstring roundtrip = fmt::to_wstring(utf8);

  EXPECT_EQ(roundtrip, input);
}

}  // namespace