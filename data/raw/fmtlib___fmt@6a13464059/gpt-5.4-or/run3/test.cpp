#include <gtest/gtest.h>

#include <fmt/core.h>
#include <fmt/format.h>

#include <string>
#include <type_traits>

namespace {

template <typename T>
std::string format_with_plus(T value) {
  return fmt::format("{:+}", value);
}

template <typename T>
void expect_plus_formats(T value, const std::string& expected) {
  EXPECT_EQ(format_with_plus(value), expected);
}

}  // namespace

TEST(FmtCoreOnSignTest, SignedIntegralTypesAcceptPlusSign) {
  expect_plus_formats(0, "+0");
  expect_plus_formats(1, "+1");
  expect_plus_formats(-1, "-1");

  expect_plus_formats(static_cast<long long>(0), "+0");
  expect_plus_formats(static_cast<long long>(42), "+42");
  expect_plus_formats(static_cast<long long>(-42), "-42");

  expect_plus_formats(static_cast<signed char>(0), "+0");
  expect_plus_formats(static_cast<signed char>(7), "+7");
  expect_plus_formats(static_cast<signed char>(-7), "-7");
}

TEST(FmtCoreOnSignTest, CharTypeAcceptsPlusSign) {
  char c = 65;  // numeric formatting for char prints its integer value
  EXPECT_EQ(fmt::format("{:+}", c), "+65");

  char zero = 0;
  EXPECT_EQ(fmt::format("{:+}", zero), "+0");
}

TEST(FmtCoreOnSignTest, UnsignedIntegralTypesRejectPlusSign) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{:+}", static_cast<unsigned>(1));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires signed argument", e.what());
          throw;
        }
      },
      fmt::format_error);

  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{:+}", static_cast<unsigned long>(1));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires signed argument", e.what());
          throw;
        }
      },
      fmt::format_error);

  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{:+}", static_cast<unsigned long long>(1));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires signed argument", e.what());
          throw;
        }
      },
      fmt::format_error);

  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{:+}", static_cast<unsigned char>(1));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires signed argument", e.what());
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtCoreOnSignTest, BoolRejectsPlusSignAsUnsignedIntegralLikeType) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{:+}", true);
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires signed argument", e.what());
          throw;
        }
      },
      fmt::format_error);

  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{:+}", false);
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires signed argument", e.what());
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtCoreOnSignTest, NonNumericArgumentsRejectSignSpecifier) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{:+}", "text");
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires numeric argument", e.what());
          throw;
        }
      },
      fmt::format_error);

  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{:+}", std::string("text"));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires numeric argument", e.what());
          throw;
        }
      },
      fmt::format_error);

  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{:+}", static_cast<const void*>(nullptr));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("format specifier requires numeric argument", e.what());
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtCoreOnSignTest, FloatingPointArgumentsAcceptPlusSign) {
  EXPECT_EQ(fmt::format("{:+}", 0.0), "+0");
  EXPECT_EQ(fmt::format("{:+}", 1.5), "+1.5");
  EXPECT_EQ(fmt::format("{:+}", -1.5), "-1.5");

  EXPECT_EQ(fmt::format("{:+}", static_cast<float>(2.25f)), "+2.25");
}