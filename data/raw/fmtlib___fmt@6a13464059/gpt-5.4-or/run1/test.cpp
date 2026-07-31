#include <gtest/gtest.h>

#include <fmt/core.h>
#include <fmt/format.h>

#include <string>

namespace {

template <typename T>
std::string format_with_plus(T value) {
  return fmt::format("{:+}", value);
}

template <typename T>
std::string format_with_space_sign(T value) {
  return fmt::format("{: }", value);
}

template <typename T>
std::string format_with_minus(T value) {
  return fmt::format("{:-}", value);
}

TEST(FmtOnSignTest, SignedIntAcceptsPlusFlag) {
  EXPECT_EQ(format_with_plus(42), "+42");
  EXPECT_EQ(format_with_plus(0), "+0");
  EXPECT_EQ(format_with_plus(-42), "-42");
}

TEST(FmtOnSignTest, SignedLongLongAcceptsPlusFlag) {
  long long pos = 1234567890123LL;
  long long neg = -1234567890123LL;
  EXPECT_EQ(format_with_plus(pos), "+1234567890123");
  EXPECT_EQ(format_with_plus(neg), "-1234567890123");
}

TEST(FmtOnSignTest, CharAcceptsSignFlagAsSignedCharacterFormatting) {
  char c = 65;  // 'A'
  EXPECT_EQ(format_with_plus(c), "+65");
  EXPECT_EQ(format_with_space_sign(c), " 65");

  char zero = 0;
  EXPECT_EQ(format_with_plus(zero), "+0");
}

TEST(FmtOnSignTest, FloatingPointAcceptsSignFlag) {
  EXPECT_EQ(format_with_plus(1.5), "+1.5");
  EXPECT_EQ(format_with_plus(-1.5), "-1.5");
  EXPECT_EQ(format_with_space_sign(2.0), " 2");
}

TEST(FmtOnSignTest, UnsignedIntRejectsSignFlagPlus) {
  EXPECT_THROW(
      {
        try {
          (void)format_with_plus(42u);
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires signed argument", e.what());
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtOnSignTest, UnsignedIntRejectsSignFlagSpace) {
  EXPECT_THROW(
      {
        try {
          (void)format_with_space_sign(0u);
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires signed argument", e.what());
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtOnSignTest, UnsignedIntRejectsExplicitMinusSignFlag) {
  EXPECT_THROW(
      {
        try {
          (void)format_with_minus(7u);
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires signed argument", e.what());
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtOnSignTest, UnsignedLongLongRejectsSignFlag) {
  unsigned long long value = 999999999999ULL;
  EXPECT_THROW(
      {
        try {
          (void)format_with_plus(value);
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires signed argument", e.what());
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtOnSignTest, BoolRejectsSignFlagBecauseItIsIntegralButNotAllowedSignedType) {
  EXPECT_THROW(
      {
        try {
          (void)format_with_plus(true);
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires signed argument", e.what());
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtOnSignTest, UnsignedCharRejectsSignFlag) {
  unsigned char value = 200;
  EXPECT_THROW(
      {
        try {
          (void)format_with_plus(value);
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires signed argument", e.what());
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtOnSignTest, StringRejectsSignFlagBecauseArgumentMustBeNumeric) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{:+}", std::string("abc"));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires numeric argument", e.what());
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtOnSignTest, CStringRejectsSignFlagBecauseArgumentMustBeNumeric) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{:+}", "abc");
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires numeric argument", e.what());
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtOnSignTest, VoidPointerRejectsSignFlagBecauseArgumentMustBeNumeric) {
  void* p = nullptr;
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{:+}", p);
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires numeric argument", e.what());
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtOnSignTest, SignFlagWorksTogetherWithWidthForSignedType) {
  EXPECT_EQ(fmt::format("{:+5}", 12), "  +12");
  EXPECT_EQ(fmt::format("{: +5}", 12), "   12");
  EXPECT_EQ(fmt::format("{:+5}", -12), "  -12");
}

TEST(FmtOnSignTest, SignFlagOnUnsignedStillFailsEvenWithOtherValidNumericFormattingOptions) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{:+08}", 12u);
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires signed argument", e.what());
          throw;
        }
      },
      fmt::format_error);
}

#if defined(FMT_USE_INT128) && FMT_USE_INT128
TEST(FmtOnSignTest, Int128AcceptsSignFlag) {
  __int128 value = static_cast<__int128>(1) << 100;
  __int128 neg = -value;

  EXPECT_EQ(fmt::format("{:+}", value), "+1267650600228229401496703205376");
  EXPECT_EQ(fmt::format("{:+}", neg), "-1267650600228229401496703205376");
}
#endif

}  // namespace