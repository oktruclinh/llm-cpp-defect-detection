#include <gtest/gtest.h>

#include <fmt/core.h>
#include <fmt/format.h>

#include <limits>
#include <string>
#include <type_traits>

namespace {

template <typename T>
std::string FormatWithPlus(T value) {
  return fmt::format("{:+}", value);
}

TEST(FmtCoreOnSignTest, SignedIntegerTypesAcceptPlusSign) {
  EXPECT_EQ(FormatWithPlus(int{-42}), "-42");
  EXPECT_EQ(FormatWithPlus(int{0}), "+0");
  EXPECT_EQ(FormatWithPlus(int{42}), "+42");

  EXPECT_EQ(FormatWithPlus(static_cast<long long>(-1234567890123LL)),
            "-1234567890123");
  EXPECT_EQ(FormatWithPlus(static_cast<long long>(1234567890123LL)),
            "+1234567890123");
}

TEST(FmtCoreOnSignTest, CharacterTypeAcceptsPlusSignAsSignedArgument) {
  EXPECT_EQ(fmt::format("{:+}", 'A'), "+65");
  EXPECT_EQ(fmt::format("{:+d}", 'A'), "+65");

  signed char sc = -5;
  EXPECT_EQ(fmt::format("{:+}", sc), "-5");
}

TEST(FmtCoreOnSignTest, UnsignedIntegralTypesRejectPlusSign) {
  EXPECT_THROW((void)fmt::format("{:+}", static_cast<unsigned>(1)),
               fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:+}", static_cast<unsigned long>(1)),
               fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:+}", static_cast<unsigned long long>(1)),
               fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:+}", static_cast<unsigned char>(1)),
               fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:+}", static_cast<bool>(true)),
               fmt::format_error);
}

TEST(FmtCoreOnSignTest, UnsignedIntegralTypesRejectMinusAndSpaceSignSpecifiers) {
  EXPECT_THROW((void)fmt::format("{:-}", static_cast<unsigned>(7)),
               fmt::format_error);
  EXPECT_THROW((void)fmt::format("{: }", static_cast<unsigned>(7)),
               fmt::format_error);
}

TEST(FmtCoreOnSignTest, NonNumericArgumentsRejectSignSpecifier) {
  EXPECT_THROW((void)fmt::format("{:+}", "text"), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:+}", std::string("text")),
               fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:+}", static_cast<const void*>(nullptr)),
               fmt::format_error);
}

TEST(FmtCoreOnSignTest, FloatingPointArgumentsAcceptSignSpecifier) {
  EXPECT_EQ(fmt::format("{:+}", 1.5), "+1.5");
  EXPECT_EQ(fmt::format("{:+}", -1.5), "-1.5");
  EXPECT_EQ(fmt::format("{:+}", 0.0), "+0");
}

TEST(FmtCoreOnSignTest, SignedIntegerBoundaryValuesFormatSuccessfully) {
  EXPECT_EQ(fmt::format("{:+}", std::numeric_limits<int>::max()),
            "+" + std::to_string(std::numeric_limits<int>::max()));
  EXPECT_EQ(fmt::format("{:+}", std::numeric_limits<int>::min()),
            std::to_string(std::numeric_limits<int>::min()));

  EXPECT_EQ(fmt::format("{:+}", std::numeric_limits<long long>::max()),
            "+" + std::to_string(std::numeric_limits<long long>::max()));
  EXPECT_EQ(fmt::format("{:+}", std::numeric_limits<long long>::min()),
            std::to_string(std::numeric_limits<long long>::min()));
}

TEST(FmtCoreOnSignTest, DynamicWidthAndPrecisionDoNotAffectSignValidationForUnsigned) {
  EXPECT_THROW((void)fmt::format("{:+{}}", static_cast<unsigned>(12), 5),
               fmt::format_error);
}

TEST(FmtCoreOnSignTest, FormatErrorMessageMentionsSignedArgumentForUnsignedIntegral) {
  try {
    (void)fmt::format("{:+}", static_cast<unsigned>(3));
    FAIL() << "Expected fmt::format_error";
  } catch (const fmt::format_error& e) {
    EXPECT_STREQ("format specifier requires signed argument", e.what());
  } catch (...) {
    FAIL() << "Expected fmt::format_error";
  }
}

}  // namespace