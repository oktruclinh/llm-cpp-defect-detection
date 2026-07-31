#include <gtest/gtest.h>

#include <fmt/core.h>
#include <string>
#include <utility>

namespace {

// A type whose formatter uses the public formatting API to query named args
// through format_parse_context::next_arg_id/get_arg_id.
struct Probe {
  std::string requested_name;
};

}  // namespace

template <>
struct fmt::formatter<Probe> {
  constexpr auto parse(fmt::format_parse_context& ctx) {
    auto it = ctx.begin();
    auto end = ctx.end();

    // Support:
    //   {}
    //   {0}
    //   {name}
    if (it != end && *it != '}') {
      if (*it >= '0' && *it <= '9') {
        int explicit_index = 0;
        while (it != end && *it >= '0' && *it <= '9') {
          explicit_index = explicit_index * 10 + (*it - '0');
          ++it;
        }
        if (it != end && *it == '}') {
          if (explicit_index == 0) {
            // Valid explicit positional reference.
            (void)ctx.check_arg_id(0);
          }
        }
      } else {
        std::string name;
        while (it != end && *it != '}') {
          name.push_back(*it++);
        }
        if (it != end && *it == '}') {
          (void)ctx.check_arg_id(fmt::string_view(name));
        }
      }
    } else {
      (void)ctx.next_arg_id();
    }

    return it;
  }

  template <typename FormatContext>
  auto format(const Probe& value, FormatContext& ctx) const
      -> decltype(ctx.out()) {
    // Public API path to the focal function:
    // basic_format_args<...>::get_id(name) is used by get_arg.
    auto arg = ctx.arg(fmt::string_view(value.requested_name));
    if (!arg) {
      return fmt::format_to(ctx.out(), "missing");
    }

    return fmt::visit_format_arg(
        [&](const auto& v) {
          return fmt::format_to(ctx.out(), "{}", v);
        },
        arg);
  }
};

TEST(FmtCoreGetIdTest, FindsExistingNamedArgument) {
  Probe p{"answer"};
  std::string s = fmt::format("{}", p, fmt::arg("answer", 42));
  EXPECT_EQ(s, "42");
}

TEST(FmtCoreGetIdTest, MissingNamedArgumentReturnsNoArg) {
  Probe p{"missing_name"};
  std::string s = fmt::format("{}", p, fmt::arg("present", 7));
  EXPECT_EQ(s, "missing");
}

TEST(FmtCoreGetIdTest, NoNamedArgumentsAtAllReturnsNoArg) {
  Probe p{"anything"};
  std::string s = fmt::format("{}", p, 123, "abc");
  EXPECT_EQ(s, "missing");
}

TEST(FmtCoreGetIdTest, DistinguishesEmptyNamedArgument) {
  Probe p{""};
  std::string s = fmt::format("{}", p, fmt::arg("", 9), fmt::arg("x", 1));
  EXPECT_EQ(s, "9");
}

TEST(FmtCoreGetIdTest, EmptyNameDoesNotMatchNonEmptyName) {
  Probe p{""};
  std::string s = fmt::format("{}", p, fmt::arg("nonempty", 9));
  EXPECT_EQ(s, "missing");
}

TEST(FmtCoreGetIdTest, ExactMatchRequiredPrefixDoesNotMatch) {
  Probe p{"ab"};
  std::string s = fmt::format("{}", p, fmt::arg("abc", 11), fmt::arg("a", 22));
  EXPECT_EQ(s, "missing");
}

TEST(FmtCoreGetIdTest, ExactMatchRequiredSuffixDoesNotMatch) {
  Probe p{"abc"};
  std::string s = fmt::format("{}", p, fmt::arg("ab", 11), fmt::arg("bc", 22));
  EXPECT_EQ(s, "missing");
}

TEST(FmtCoreGetIdTest, CaseSensitiveLookup) {
  Probe p{"Name"};
  std::string s = fmt::format("{}", p, fmt::arg("name", 1), fmt::arg("NAME", 2));
  EXPECT_EQ(s, "missing");
}

TEST(FmtCoreGetIdTest, CanFindNamedArgumentAmongMany) {
  Probe p{"target"};
  std::string s = fmt::format(
      "{}", p,
      fmt::arg("a0", 0), fmt::arg("a1", 1), fmt::arg("a2", 2), fmt::arg("a3", 3),
      fmt::arg("a4", 4), fmt::arg("a5", 5), fmt::arg("a6", 6), fmt::arg("a7", 7),
      fmt::arg("a8", 8), fmt::arg("a9", 9), fmt::arg("target", 12345),
      fmt::arg("z", 99));
  EXPECT_EQ(s, "12345");
}

TEST(FmtCoreGetIdTest, ReturnsFirstMatchingNamedArgumentWhenDuplicatesExist) {
  Probe p{"dup"};
  std::string s = fmt::format("{}", p, fmt::arg("dup", 10), fmt::arg("dup", 20));
  EXPECT_EQ(s, "10");
}

TEST(FmtCoreGetIdTest, ParseContextAcceptsExistingNamedId) {
  EXPECT_NO_THROW({
    std::string s = fmt::format("{named}", fmt::arg("named", 17));
    EXPECT_EQ(s, "17");
  });
}

TEST(FmtCoreGetIdTest, ParseContextRejectsMissingNamedId) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{missing}", fmt::arg("present", 17));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("argument not found", e.what());
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtCoreGetIdTest, ParseContextRejectsNamedReferenceWhenNoNamedArgsExist) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{name}", 1, 2, 3);
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ("argument not found", e.what());
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtCoreGetIdTest, ParseContextAllowsEmptyNamedIdWhenPresent) {
  EXPECT_NO_THROW({
    std::string s = fmt::format("{0}", fmt::arg("", 5));
    EXPECT_EQ(s, "5");
  });
}

TEST(FmtCoreGetIdTest, CustomFormatterCanRetrieveStringNamedArgument) {
  Probe p{"greeting"};
  std::string s = fmt::format("{}", p, fmt::arg("greeting", "hello"));
  EXPECT_EQ(s, "hello");
}

TEST(FmtCoreGetIdTest, CustomFormatterCanRetrieveMultipleDifferentNamedArgumentsIndependently) {
  Probe first{"x"};
  Probe second{"y"};
  std::string s =
      fmt::format("{} {}", first, second, fmt::arg("x", 1), fmt::arg("y", 2));
  EXPECT_EQ(s, "1 2");
}

TEST(FmtCoreGetIdTest, LookupOfMissingNameDoesNotAccidentallyReturnPositionalArg) {
  Probe p{"0"};
  std::string s = fmt::format("{}", p, 999, fmt::arg("one", 1));
  EXPECT_EQ(s, "missing");
}

TEST(FmtCoreGetIdTest, NamedLookupWorksWithPackedArgumentsPath) {
  // Small number of arguments exercises the packed representation in fmt.
  Probe p{"n"};
  std::string s = fmt::format("{}", p, fmt::arg("n", 314));
  EXPECT_EQ(s, "314");
}

TEST(FmtCoreGetIdTest, NamedLookupWorksWithNonPackedArgumentsPath) {
  // Larger and mixed argument list is intended to exercise the unpacked path.
  Probe p{"needle"};
  std::string s = fmt::format(
      "{}", p,
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
      fmt::arg("a", 10), fmt::arg("b", 11), fmt::arg("c", 12),
      fmt::arg("needle", 777), fmt::arg("d", 13), fmt::arg("e", 14));
  EXPECT_EQ(s, "777");
}