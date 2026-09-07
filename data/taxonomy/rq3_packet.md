# RQ3 - goi du lieu de gan nhan (doc cung file rq3_coder2.csv)

Voi moi dong CSV, tim muc `## id N` tuong ung o duoi, doc HAM FOCAL + BAI TEST,
roi dien 1 so 1..5 vao cot `label` (dinh nghia trong file HUONG_DAN_LABMATE.md).

Tong cong: 60 o.

---

## id 1 - `fmtlib___fmt@0cc73ebf79` - model qwen3-coder

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@0cc73ebf79/qwen3-coder/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/core.h>
#include <fmt/format.h>

TEST(FmtCoreTest, GetIdWithNamedArguments) {
    // Test normal case with named arguments
    auto result = fmt::format("{first} {second}", 
                             fmt::arg("first", "hello"),
                             fmt::arg("second", "world"));
    EXPECT_EQ(result, "hello world");
}

TEST(FmtCoreTest, GetIdWithoutNamedArguments) {
    // Test when no named arguments are provided
    auto result = fmt::format("{} {}", 1, 2);
    EXPECT_EQ(result, "1 2");
}

TEST(FmtCoreTest, GetIdEmptyNamedArguments) {
    // Test with empty named arguments
    auto result = fmt::format("{a}", fmt::arg("a", "test"));
    EXPECT_EQ(result, "test");
}

TEST(FmtCoreTest, GetIdMultipleNamedArguments) {
    // Test with multiple named arguments
    auto result = fmt::format("{a} {b} {c}", 
                             fmt::arg("a", 1),
                             fmt::arg("b", 2),
                             fmt::arg("c", 3));
    EXPECT_EQ(result, "1 2 3");
}

TEST(FmtCoreTest, GetIdNamedArgumentNotFound) {
    // Test when named argument is not found
    auto result = fmt::format("{a} {b}", 
                             fmt::arg("a", "hello"));
    EXPECT_EQ(result, "hello {b}");
}

TEST(FmtCoreTest, GetIdSpecialCharacters) {
    // Test with special characters in names
    auto result = fmt::format("{a_b} {c-d}", 
                             fmt::arg("a_b", "test1"),
                             fmt::arg("c-d", "test2"));
    EXPECT_EQ(result, "test1 test2");
}

TEST(FmtCoreTest, GetIdNumericNames) {
    // Test with numeric names
    auto result = fmt::format("{1} {2}", 
                             fmt::arg("1", "one"),
                             fmt::arg("2", "two"));
    EXPECT_EQ(result, "one two");
}

TEST(FmtCoreTest, GetIdMixedArguments) {
    // Test mix of positional and named arguments
    auto result = fmt::format("{} {named} {}", 
                             1,
                             fmt::arg("named", "hello"),
                             3);
    EXPECT_EQ(result, "1 hello 3");
}

TEST(FmtCoreTest, GetIdNestedFormatting) {
    // Test nested formatting scenarios
    auto result = fmt::format("{outer}", 
                             fmt::arg("outer", fmt::format("{inner}", 
                                                        fmt::arg("inner", "value"))));
    EXPECT_EQ(result, "{inner}");
}

TEST(FmtCoreTest, GetIdUnicodeNames) {
    // Test with unicode names
    auto result = fmt::format("{α} {β}", 
                             fmt::arg("α", "alpha"),
                             fmt::arg("β", "beta"));
    EXPECT_EQ(result, "alpha beta");
}

TEST(FmtCoreTest, GetIdLongNames) {
    // Test with long argument names
    auto result = fmt::format("{very_long_argument_name} {another_long_name}", 
                             fmt::arg("very_long_argument_name", "long1"),
                             fmt::arg("another_long_name", "long2"));
    EXPECT_EQ(result, "long1 long2");
}

TEST(FmtCoreTest, GetIdSingleCharacterNames) {
    // Test with single character names
    auto result = fmt::format("{a} {b} {c}", 
                             fmt::arg("a", "1"),
                             fmt::arg("b", "2"),
                             fmt::arg("c", "3"));
    EXPECT_EQ(result, "1 2 3");
}

TEST(FmtCoreTest, GetIdDuplicateNames) {
    // Test behavior with duplicate names (should use first occurrence)
    auto result = fmt::format("{a} {a}", 
                             fmt::arg("a", "first"),
                             fmt::arg("a", "second"));
    EXPECT_EQ(result, "first first");
}
```

---

## id 2 - `fmtlib___fmt@0cc73ebf79` - model qwen3-coder

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@0cc73ebf79/qwen3-coder/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/core.h>
#include <fmt/format.h>

TEST(FmtCoreTest, GetIdWithNamedArguments) {
    // Test normal case with named arguments
    auto result = fmt::format("Hello {name}!", fmt::arg("name", "World"));
    EXPECT_EQ("Hello World!", result);
    
    // Test with multiple named arguments
    result = fmt::format("Hello {name}, you are {age} years old!", 
                        fmt::arg("name", "Alice"), 
                        fmt::arg("age", 30));
    EXPECT_EQ("Hello Alice, you are 30 years old!", result);
}

TEST(FmtCoreTest, GetIdWithoutNamedArguments) {
    // Test with no named arguments - should not crash
    auto result = fmt::format("Hello World!");
    EXPECT_EQ("Hello World!", result);
    
    // Test with positional arguments only
    result = fmt::format("Hello {}!", "World");
    EXPECT_EQ("Hello World!", result);
}

TEST(FmtCoreTest, GetIdEdgeCases) {
    // Test empty string name
    auto result = fmt::format("Hello {name}!", fmt::arg("name", "World"));
    EXPECT_EQ("Hello World!", result);
    
    // Test with numeric names (should work as strings)
    result = fmt::format("Value {0} is {1}", fmt::arg("0", 42), fmt::arg("1", 3.14));
    EXPECT_EQ("Value 42 is 3.14", result);
    
    // Test with special characters in names
    result = fmt::format("Hello {name-with-dashes}!", fmt::arg("name-with-dashes", "World"));
    EXPECT_EQ("Hello World!", result);
}

TEST(FmtCoreTest, GetIdComplexScenarios) {
    // Test nested formatting
    auto result = fmt::format("Result: {result}", fmt::arg("result", fmt::format("Inner: {value}", fmt::arg("value", 123))));
    EXPECT_EQ("Result: Inner: 123", result);
    
    // Test with mixed positional and named arguments
    result = fmt::format("First: {}, Second: {second}, Third: {}", 
                        "pos1", 
                        fmt::arg("second", "named"), 
                        "pos3");
    EXPECT_EQ("First: pos1, Second: named, Third: pos3", result);
}

TEST(FmtCoreTest, GetIdPerformance) {
    // Test that formatting with named arguments doesn't cause performance issues
    std::string result;
    for (int i = 0; i < 1000; ++i) {
        result = fmt::format("Test {value}!", fmt::arg("value", i));
    }
    EXPECT_EQ("Test 999!", result);
}

TEST(FmtCoreTest, GetIdStringViewCompatibility) {
    // Test with different string view types
    std::string name = "test";
    auto result = fmt::format("Hello {name}!", fmt::arg(name.c_str(), "World"));
    EXPECT_EQ("Hello World!", result);
    
    // Test with std::string_view (if available) - use const char* instead
    const char* cstr_name = "name";
    result = fmt::format("Hello {name}!", fmt::arg(cstr_name, "World"));
    EXPECT_EQ("Hello World!", result);
}

TEST(FmtCoreTest, GetIdLargeArguments) {
    // Test with large argument values
    std::string large_string(1000, 'A');
    auto result = fmt::format("String length: {len}", fmt::arg("len", large_string.length()));
    EXPECT_EQ("String length: 1000", result);
    
    // Test with large numeric values
    auto large_num = static_cast<long long>(1000000000000LL);
    result = fmt::format("Large number: {num}", fmt::arg("num", large_num));
    EXPECT_EQ("Large number: 1000000000000", result);
}

TEST(FmtCoreTest, GetIdSpecialCharacters) {
    // Test with unicode characters
    auto result = fmt::format("Hello {name}!", fmt::arg("name", "世界"));
    EXPECT_EQ("Hello 世界!", result);
    
    // Test with special formatting characters
    result = fmt::format("Hello {name}!", fmt::arg("name", "{braces}"));
    EXPECT_EQ("Hello {braces}!", result);
}
```

---

## id 3 - `fmtlib___fmt@0cc73ebf79` - model qwen3-coder

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@0cc73ebf79/qwen3-coder/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/core.h>
#include <fmt/format.h>

TEST(FmtCoreTest, GetIdWithNamedArguments) {
    // Test normal case with named arguments
    auto result = fmt::format("{first} {second}", 
                             fmt::arg("first", "hello"),
                             fmt::arg("second", "world"));
    EXPECT_EQ(result, "hello world");
}

TEST(FmtCoreTest, GetIdWithoutNamedArguments) {
    // Test when no named arguments are provided
    auto result = fmt::format("{} {}", 1, 2);
    EXPECT_EQ(result, "1 2");
}

TEST(FmtCoreTest, GetIdEmptyNamedArguments) {
    // Test with empty named arguments
    auto result = fmt::format("{a}", fmt::arg("a", "test"));
    EXPECT_EQ(result, "test");
}

TEST(FmtCoreTest, GetIdMultipleNamedArguments) {
    // Test with multiple named arguments
    auto result = fmt::format("{a} {b} {c}", 
                             fmt::arg("a", 1),
                             fmt::arg("b", 2),
                             fmt::arg("c", 3));
    EXPECT_EQ(result, "1 2 3");
}

TEST(FmtCoreTest, GetIdNamedArgumentNotFound) {
    // Test when named argument is not found
    auto result = fmt::format("{a} {b}", 
                             fmt::arg("a", "hello"));
    EXPECT_EQ(result, "hello {b}");
}

TEST(FmtCoreTest, GetIdSpecialCharacters) {
    // Test with special characters in names
    auto result = fmt::format("{a_b} {c-d}", 
                             fmt::arg("a_b", "test1"),
                             fmt::arg("c-d", "test2"));
    EXPECT_EQ(result, "test1 test2");
}

TEST(FmtCoreTest, GetIdNumericNames) {
    // Test with numeric names
    auto result = fmt::format("{1} {2}", 
                             fmt::arg("1", "one"),
                             fmt::arg("2", "two"));
    EXPECT_EQ(result, "one two");
}

TEST(FmtCoreTest, GetIdRepeatedNames) {
    // Test with repeated names (should use first occurrence)
    auto result = fmt::format("{a} {a}", 
                             fmt::arg("a", "test"));
    EXPECT_EQ(result, "test test");
}

TEST(FmtCoreTest, GetIdNestedFormatting) {
    // Test nested formatting scenarios
    auto result = fmt::format("{outer}", 
                             fmt::arg("outer", fmt::format("{inner}", 
                                                        fmt::arg("inner", "value"))));
    EXPECT_EQ(result, "{inner}");
}

TEST(FmtCoreTest, GetIdMixedPositionalAndNamed) {
    // Test mixed positional and named arguments
    auto result = fmt::format("{} {named} {}", 
                             1,
                             fmt::arg("named", "hello"),
                             3);
    EXPECT_EQ(result, "1 hello 3");
}

TEST(FmtCoreTest, GetIdEmptyStringNames) {
    // Test with empty string names
    auto result = fmt::format("{a} {b}", 
                             fmt::arg("a", "test1"),
                             fmt::arg("b", "test2"));
    EXPECT_EQ(result, "test1 test2");
}

TEST(FmtCoreTest, GetIdLongNames) {
    // Test with long argument names
    auto result = fmt::format("{very_long_argument_name} {another_long_name}", 
                             fmt::arg("very_long_argument_name", "value1"),
                             fmt::arg("another_long_name", "value2"));
    EXPECT_EQ(result, "value1 value2");
}

TEST(FmtCoreTest, GetIdUnicodeNames) {
    // Test with Unicode characters in names
    auto result = fmt::format("{α} {β}", 
                             fmt::arg("α", "alpha"),
                             fmt::arg("β", "beta"));
    EXPECT_EQ(result, "alpha beta");
}
```

---

## id 4 - `fmtlib___fmt@cd7202e039` - model gpt-5.4-or

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@cd7202e039/gpt-5.4-or/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>

#include <chrono>
#include <string>

#include <fmt/chrono.h>
#include <fmt/format.h>

namespace {

using namespace std::chrono;

TEST(FmtChronoDurationFormattingTest, SecondsWithoutFractionDoNotPrintDecimalPart) {
  EXPECT_EQ(fmt::format("{}", seconds(0)), "00");
  EXPECT_EQ(fmt::format("{}", seconds(5)), "05");
  EXPECT_EQ(fmt::format("{}", seconds(59)), "59");
}

TEST(FmtChronoDurationFormattingTest, MillisecondsPrintFractionalSeconds) {
  EXPECT_EQ(fmt::format("{}", milliseconds(0)), "00.000");
  EXPECT_EQ(fmt::format("{}", milliseconds(1)), "00.001");
  EXPECT_EQ(fmt::format("{}", milliseconds(12)), "00.012");
  EXPECT_EQ(fmt::format("{}", milliseconds(999)), "00.999");

  EXPECT_EQ(fmt::format("{}", milliseconds(1000)), "01.000");
  EXPECT_EQ(fmt::format("{}", milliseconds(1234)), "01.234");
  EXPECT_EQ(fmt::format("{}", milliseconds(59001)), "59.001");
}

TEST(FmtChronoDurationFormattingTest, MicrosecondsPrintSixFractionalDigits) {
  EXPECT_EQ(fmt::format("{}", microseconds(0)), "00.000000");
  EXPECT_EQ(fmt::format("{}", microseconds(1)), "00.000001");
  EXPECT_EQ(fmt::format("{}", microseconds(42)), "00.000042");
  EXPECT_EQ(fmt::format("{}", microseconds(123456)), "00.123456");
  EXPECT_EQ(fmt::format("{}", microseconds(1000000)), "01.000000");
}

TEST(FmtChronoDurationFormattingTest, NanosecondsPrintNineFractionalDigits) {
  EXPECT_EQ(fmt::format("{}", nanoseconds(0)), "00.000000000");
  EXPECT_EQ(fmt::format("{}", nanoseconds(1)), "00.000000001");
  EXPECT_EQ(fmt::format("{}", nanoseconds(999999999)), "00.999999999");
  EXPECT_EQ(fmt::format("{}", nanoseconds(1000000000)), "01.000000000");
  EXPECT_EQ(fmt::format("{}", nanoseconds(1234567890)), "01.234567890");
}

TEST(FmtChronoDurationFormattingTest, FractionalFormattingWorksWithCustomChronoSpec) {
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(1)), "00.001");
  EXPECT_EQ(fmt::format("{:%S}", microseconds(123456)), "00.123456");
  EXPECT_EQ(fmt::format("{:%S}", nanoseconds(123456789)), "00.123456789");
  EXPECT_EQ(fmt::format("{:%S}", seconds(7)), "07");
}

TEST(FmtChronoDurationFormattingTest, MinutesAndHoursUseSecondFieldModuloSixtyWithFraction) {
  EXPECT_EQ(fmt::format("{:%S}", minutes(1)), "00");
  EXPECT_EQ(fmt::format("{:%S}", hours(1)), "00");

  EXPECT_EQ(fmt::format("{:%S}", duration_cast<milliseconds>(minutes(1)) + milliseconds(234)),
            "00.234");
  EXPECT_EQ(fmt::format("{:%S}", duration_cast<microseconds>(hours(1)) + microseconds(5)),
            "00.000005");
}

TEST(FmtChronoDurationFormattingTest, CustomRatioDurationPrintsExactFractionalPrecision) {
  using tenth_seconds = duration<int, std::ratio<1, 10>>;
  using quarter_seconds = duration<int, std::ratio<1, 4>>;

  EXPECT_EQ(fmt::format("{}", tenth_seconds(0)), "00.1");
  EXPECT_EQ(fmt::format("{}", tenth_seconds(1)), "00.1");
  EXPECT_EQ(fmt::format("{}", tenth_seconds(12)), "01.2");

  EXPECT_EQ(fmt::format("{}", quarter_seconds(1)), "00.25");
  EXPECT_EQ(fmt::format("{}", quarter_seconds(3)), "00.75");
  EXPECT_EQ(fmt::format("{}", quarter_seconds(5)), "01.25");
}

TEST(FmtChronoDurationFormattingTest, NegativeFractionalDurationsKeepSecondFieldAndFractionConsistent) {
  EXPECT_EQ(fmt::format("{}", milliseconds(-1)), "-00.001");
  EXPECT_EQ(fmt::format("{}", milliseconds(-1234)), "-01.234");
  EXPECT_EQ(fmt::format("{}", microseconds(-42)), "-00.000042");
  EXPECT_EQ(fmt::format("{}", nanoseconds(-1000000001)), "-01.000000001");
}

TEST(FmtChronoDurationFormattingTest, BoundaryAroundMinuteStillFormatsSecondsFieldCorrectly) {
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(59999)), "59.999");
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(60000)), "00.000");
  EXPECT_EQ(fmt::format("{:%S}", milliseconds(61001)), "01.001");
}

TEST(FmtChronoDurationFormattingTest, StringFormattingIsConsistent) {
  EXPECT_EQ(fmt::format("{}", milliseconds(2345)), "02.345");
  EXPECT_EQ(fmt::format("{:%S}", microseconds(7)), "00.000007");
}

}  // namespace
```

---

## id 5 - `fmtlib___fmt@cd7202e039` - model gpt-5.4-or

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@cd7202e039/gpt-5.4-or/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>

#include <chrono>
#include <string>
#include <type_traits>

#include <fmt/chrono.h>
#include <fmt/format.h>

namespace {

template <typename Duration>
std::string format_with_default_spec(Duration d) {
  return fmt::format("{}", d);
}

template <typename Duration>
std::string format_with_chrono_spec(Duration d) {
  return fmt::format("{:%S}", d);
}

TEST(FmtChronoDurationSecondsFormattingTest, WholeSecondsDoNotShowFraction) {
  using namespace std::chrono;

  EXPECT_EQ(format_with_chrono_spec(seconds(0)), "00");
  EXPECT_EQ(format_with_chrono_spec(seconds(5)), "05");
  EXPECT_EQ(format_with_chrono_spec(seconds(59)), "59");
}

TEST(FmtChronoDurationSecondsFormattingTest, MillisecondsAppendFractionalPart) {
  using namespace std::chrono;

  EXPECT_EQ(format_with_chrono_spec(milliseconds(0)), "00.000");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(1)), "00.001");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(12)), "00.012");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(999)), "00.999");

  EXPECT_EQ(format_with_chrono_spec(milliseconds(1000)), "01.000");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(1234)), "01.234");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(59001)), "59.001");
}

TEST(FmtChronoDurationSecondsFormattingTest, MicrosecondsAppendFractionalPartWithSixDigits) {
  using namespace std::chrono;

  EXPECT_EQ(format_with_chrono_spec(microseconds(1)), "00.000001");
  EXPECT_EQ(format_with_chrono_spec(microseconds(12)), "00.000012");
  EXPECT_EQ(format_with_chrono_spec(microseconds(123456)), "00.123456");
  EXPECT_EQ(format_with_chrono_spec(microseconds(1000001)), "01.000001");
}

TEST(FmtChronoDurationSecondsFormattingTest, NanosecondsAppendFractionalPartWithNineDigits) {
  using namespace std::chrono;

  EXPECT_EQ(format_with_chrono_spec(nanoseconds(1)), "00.000000001");
  EXPECT_EQ(format_with_chrono_spec(nanoseconds(42)), "00.000000042");
  EXPECT_EQ(format_with_chrono_spec(nanoseconds(123456789)), "00.123456789");
  EXPECT_EQ(format_with_chrono_spec(nanoseconds(1000000001LL)), "01.000000001");
}

TEST(FmtChronoDurationSecondsFormattingTest, FractionalFormattingUsesSecondFieldModuloMinute) {
  using namespace std::chrono;

  EXPECT_EQ(format_with_chrono_spec(minutes(1) + milliseconds(1)), "00.001");
  EXPECT_EQ(format_with_chrono_spec(minutes(1) + seconds(2) + milliseconds(3)), "02.003");
  EXPECT_EQ(format_with_chrono_spec(hours(1) + minutes(2) + seconds(3) + microseconds(4)),
            "03.000004");
}

TEST(FmtChronoDurationSecondsFormattingTest, NegativeFractionalDurationsKeepSignAndFraction) {
  using namespace std::chrono;

  EXPECT_EQ(format_with_chrono_spec(milliseconds(-1)), "-00.001");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(-1234)), "-01.234");
  EXPECT_EQ(format_with_chrono_spec(microseconds(-42)), "-00.000042");
  EXPECT_EQ(format_with_chrono_spec(nanoseconds(-1000000001LL)), "-01.000000001");
}

TEST(FmtChronoDurationSecondsFormattingTest, FormattingDistinguishesWholeAndFractionalDurations) {
  using namespace std::chrono;

  EXPECT_NE(format_with_chrono_spec(seconds(7)), "07.000");
  EXPECT_EQ(format_with_chrono_spec(seconds(7)), "07");

  EXPECT_NE(format_with_chrono_spec(milliseconds(7000)), "07");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(7000)), "07.000");
}

TEST(FmtChronoDurationDefaultFormattingTest, PublicApiStillRepresentsFractionalDurations) {
  using namespace std::chrono;

  const auto s = format_with_default_spec(milliseconds(1234));

  EXPECT_FALSE(s.empty());
  EXPECT_NE(s.find("1.234"), std::string::npos)
      << "Default public formatting should preserve the fractional value";
}

TEST(FmtChronoDurationSecondsFormattingTest, CustomRatioDurationFormatsFractionalSecondsCorrectly) {
  using custom_ms = std::chrono::duration<int, std::milli>;
  using custom_us = std::chrono::duration<long long, std::micro>;

  EXPECT_EQ(format_with_chrono_spec(custom_ms(2345)), "02.345");
  EXPECT_EQ(format_with_chrono_spec(custom_us(1000001)), "01.000001");
}

TEST(FmtChronoDurationSecondsFormattingTest, BoundaryAroundSecondTransitionIsCorrect) {
  using namespace std::chrono;

  EXPECT_EQ(format_with_chrono_spec(milliseconds(999)), "00.999");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(1000)), "01.000");
  EXPECT_EQ(format_with_chrono_spec(milliseconds(1001)), "01.001");

  EXPECT_EQ(format_with_chrono_spec(nanoseconds(999999999LL)), "00.999999999");
  EXPECT_EQ(format_with_chrono_spec(nanoseconds(1000000000LL)), "01.000000000");
  EXPECT_EQ(format_with_chrono_spec(nanoseconds(1000000001LL)), "01.000000001");
}

}  // namespace
```

---

## id 6 - `fmtlib___fmt@cd7202e039` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@cd7202e039/deepseek-chat/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test formatting of seconds with fractional parts for various durations
TEST(ChronoWriteFractionalSecondsTest, SecondsWithFraction) {
    // Test 1.5 seconds
    auto dur = std::chrono::duration<double>(1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");

    // Test 0.001 seconds (1 millisecond)
    dur = std::chrono::duration<double>(0.001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.001");

    // Test 59.999 seconds (near max)
    dur = std::chrono::duration<double>(59.999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999");
}

// Test formatting of seconds with no fractional part (integer seconds)
TEST(ChronoWriteFractionalSecondsTest, IntegerSeconds) {
    auto dur = std::chrono::seconds(5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "05");

    dur = std::chrono::seconds(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00");

    dur = std::chrono::seconds(59);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59");
}

// Test formatting with milliseconds precision
TEST(ChronoWriteFractionalSecondsTest, Milliseconds) {
    auto dur = std::chrono::milliseconds(1500);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");

    dur = std::chrono::milliseconds(1);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.001");

    dur = std::chrono::milliseconds(59999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999");
}

// Test formatting with microseconds precision
TEST(ChronoWriteFractionalSecondsTest, Microseconds) {
    auto dur = std::chrono::microseconds(1500000);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500000");

    dur = std::chrono::microseconds(1);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000001");

    dur = std::chrono::microseconds(59999999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999999");
}

// Test formatting with nanoseconds precision
TEST(ChronoWriteFractionalSecondsTest, Nanoseconds) {
    auto dur = std::chrono::nanoseconds(1500000000);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500000000");

    dur = std::chrono::nanoseconds(1);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000000001");

    dur = std::chrono::nanoseconds(59999999999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999999999");
}

// Test edge case: zero duration
TEST(ChronoWriteFractionalSecondsTest, ZeroDuration) {
    auto dur = std::chrono::duration<double>(0.0);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000");

    dur = std::chrono::seconds(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00");

    dur = std::chrono::milliseconds(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000");
}

// Test edge case: very small fractional seconds
TEST(ChronoWriteFractionalSecondsTest, VerySmallFraction) {
    auto dur = std::chrono::duration<double>(0.000001);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000001");

    dur = std::chrono::duration<double>(0.000000001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000000001");
}

// Test edge case: seconds near boundaries
TEST(ChronoWriteFractionalSecondsTest, BoundarySeconds) {
    // Just above 0
    auto dur = std::chrono::duration<double>(0.0005);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.001");  // rounds to 3 decimal places

    // Just below 60
    dur = std::chrono::duration<double>(59.9995);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "60.000");  // rounds up to 60 (but format may show 60)
}

// Test formatting with custom precision specifier
TEST(ChronoWriteFractionalSecondsTest, CustomPrecision) {
    auto dur = std::chrono::duration<double>(1.23456789);
    std::string result = fmt::format("{:.3%S}", dur);
    EXPECT_EQ(result, "01.235");  // rounded to 3 decimal places

    result = fmt::format("{:.6%S}", dur);
    EXPECT_EQ(result, "01.234568");  // rounded to 6 decimal places

    result = fmt::format("{:.0%S}", dur);
    EXPECT_EQ(result, "01");  // no fractional part
}

// Test that the function handles negative durations (if applicable)
TEST(ChronoWriteFractionalSecondsTest, NegativeDuration) {
    auto dur = std::chrono::duration<double>(-1.5);
    std::string result = fmt::format("{:%S}", dur);
    // Negative durations may show negative seconds or wrap; check behavior
    // Typically fmtlib shows negative sign before seconds
    EXPECT_EQ(result, "-01.500");
}

// Test that the function works with duration_cast truncation
TEST(ChronoWriteFractionalSecondsTest, DurationCastTruncation) {
    auto dur = std::chrono::duration<double>(1.9999);
    // Using integer seconds should truncate
    std::string result = fmt::format("{:%S}", std::chrono::duration_cast<std::chrono::seconds>(dur));
    EXPECT_EQ(result, "01");  // truncated, not rounded

    dur = std::chrono::duration<double>(1.0001);
    result = fmt::format("{:%S}", std::chrono::duration_cast<std::chrono::seconds>(dur));
    EXPECT_EQ(result, "01");
}
```

---

## id 7 - `fmtlib___fmt@cd7202e039` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@cd7202e039/deepseek-chat/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test formatting of seconds with fractional parts using default format
TEST(ChronoFractionalSecondsTest, DefaultFormatSeconds) {
    auto dur = std::chrono::duration<double>(1.5);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "01.500" (seconds with 3 fractional digits by default)
    EXPECT_EQ(result, "01.500");
}

// Test formatting of seconds with fractional parts using custom precision
TEST(ChronoFractionalSecondsTest, CustomPrecisionSeconds) {
    auto dur = std::chrono::duration<double>(2.25);
    std::string result = fmt::format("{:%S.%6}", dur);
    // Expected: "02.250000" (seconds with 6 fractional digits)
    EXPECT_EQ(result, "02.250000");
}

// Test formatting of seconds with zero fractional part
TEST(ChronoFractionalSecondsTest, ZeroFractionalSeconds) {
    auto dur = std::chrono::duration<double>(3.0);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "03.000" (seconds with 3 fractional digits, all zero)
    EXPECT_EQ(result, "03.000");
}

// Test formatting of seconds with very small fractional part
TEST(ChronoFractionalSecondsTest, SmallFractionalSeconds) {
    auto dur = std::chrono::duration<double>(0.001);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "00.001" (seconds with 3 fractional digits)
    EXPECT_EQ(result, "00.001");
}

// Test formatting of seconds with large fractional part
TEST(ChronoFractionalSecondsTest, LargeFractionalSeconds) {
    auto dur = std::chrono::duration<double>(59.999);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "59.999" (seconds with 3 fractional digits)
    EXPECT_EQ(result, "59.999");
}

// Test formatting of seconds with rounding behavior
TEST(ChronoFractionalSecondsTest, RoundingFractionalSeconds) {
    auto dur = std::chrono::duration<double>(1.9999);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "01.999" (3 fractional digits, truncated/rounded to 3 places)
    EXPECT_EQ(result, "01.999");
}

// Test formatting of seconds with negative duration (should still format seconds)
TEST(ChronoFractionalSecondsTest, NegativeDurationSeconds) {
    auto dur = std::chrono::duration<double>(-1.5);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "01.500" (seconds are absolute value, sign handled elsewhere)
    EXPECT_EQ(result, "01.500");
}

// Test formatting of seconds with integer duration type
TEST(ChronoFractionalSecondsTest, IntegerDurationSeconds) {
    auto dur = std::chrono::seconds(5);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "05" (no fractional part for integer duration)
    EXPECT_EQ(result, "05");
}

// Test formatting of seconds with milliseconds duration
TEST(ChronoFractionalSecondsTest, MillisecondsDurationSeconds) {
    auto dur = std::chrono::milliseconds(1500);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "01.500" (1.5 seconds with 3 fractional digits)
    EXPECT_EQ(result, "01.500");
}

// Test formatting of seconds with microseconds duration
TEST(ChronoFractionalSecondsTest, MicrosecondsDurationSeconds) {
    auto dur = std::chrono::microseconds(1234567);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "01.234567" (1.234567 seconds with 6 fractional digits)
    EXPECT_EQ(result, "01.234567");
}

// Test formatting of seconds with nanoseconds duration
TEST(ChronoFractionalSecondsTest, NanosecondsDurationSeconds) {
    auto dur = std::chrono::nanoseconds(1234567890);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "01.234567890" (1.234567890 seconds with 9 fractional digits)
    EXPECT_EQ(result, "01.234567890");
}

// Test formatting of seconds with zero duration
TEST(ChronoFractionalSecondsTest, ZeroDurationSeconds) {
    auto dur = std::chrono::duration<double>(0.0);
    std::string result = fmt::format("{:%S}", dur);
    // Expected: "00.000" (zero seconds with 3 fractional digits)
    EXPECT_EQ(result, "00.000");
}

// Test formatting of seconds with max precision (9 digits)
TEST(ChronoFractionalSecondsTest, MaxPrecisionSeconds) {
    auto dur = std::chrono::duration<double>(1.123456789);
    std::string result = fmt::format("{:%S.%9}", dur);
    // Expected: "01.123456789" (9 fractional digits)
    EXPECT_EQ(result, "01.123456789");
}

// Test formatting of seconds with min precision (0 digits)
TEST(ChronoFractionalSecondsTest, MinPrecisionSeconds) {
    auto dur = std::chrono::duration<double>(1.5);
    std::string result = fmt::format("{:%S.%0}", dur);
    // Expected: "01" (no fractional digits)
    EXPECT_EQ(result, "01");
}

// Test formatting of seconds with custom precision that truncates
TEST(ChronoFractionalSecondsTest, TruncatingPrecisionSeconds) {
    auto dur = std::chrono::duration<double>(1.123456789);
    std::string result = fmt::format("{:%S.%3}", dur);
    // Expected: "01.123" (truncated to 3 fractional digits)
    EXPECT_EQ(result, "01.123");
}
```

---

## id 8 - `fmtlib___fmt@cd7202e039` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@cd7202e039/deepseek-chat/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test formatting of seconds with fractional parts for various durations
TEST(ChronoWriteFractionalSecondsTest, SecondsWithFractional) {
    using namespace std::chrono_literals;
    
    // Test 1.5 seconds
    auto dur = std::chrono::duration<double>(1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");
    
    // Test 0.25 seconds
    dur = std::chrono::duration<double>(0.25);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.250");
    
    // Test 59.999 seconds (near boundary)
    dur = std::chrono::duration<double>(59.999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999");
}

// Test formatting of seconds with no fractional part (integer seconds)
TEST(ChronoWriteFractionalSecondsTest, IntegerSeconds) {
    using namespace std::chrono_literals;
    
    // Test exactly 0 seconds
    auto dur = std::chrono::seconds(0);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00");
    
    // Test exactly 30 seconds
    dur = std::chrono::seconds(30);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "30");
    
    // Test exactly 59 seconds
    dur = std::chrono::seconds(59);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59");
}

// Test formatting of seconds with very small fractional parts
TEST(ChronoWriteFractionalSecondsTest, SmallFractional) {
    using namespace std::chrono_literals;
    
    // Test 0.001 seconds (1 millisecond)
    auto dur = std::chrono::duration<double>(0.001);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.001");
    
    // Test 0.000001 seconds (1 microsecond)
    dur = std::chrono::duration<double>(0.000001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000001");
}

// Test formatting of seconds with very large fractional parts
TEST(ChronoWriteFractionalSecondsTest, LargeFractional) {
    using namespace std::chrono_literals;
    
    // Test 0.999999 seconds
    auto dur = std::chrono::duration<double>(0.999999);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.999999");
}

// Test formatting of seconds with negative values
TEST(ChronoWriteFractionalSecondsTest, NegativeSeconds) {
    using namespace std::chrono_literals;
    
    // Test -1.5 seconds
    auto dur = std::chrono::duration<double>(-1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-01.500");
    
    // Test -0.25 seconds
    dur = std::chrono::duration<double>(-0.25);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-00.250");
}

// Test formatting of seconds with different precision specifications
TEST(ChronoWriteFractionalSecondsTest, PrecisionSpecifier) {
    using namespace std::chrono_literals;
    
    // Test with precision specifier .3
    auto dur = std::chrono::duration<double>(1.23456);
    std::string result = fmt::format("{:.3%S}", dur);
    EXPECT_EQ(result, "01.235");  // Rounded to 3 decimal places
    
    // Test with precision specifier .0 (no fractional)
    result = fmt::format("{:.0%S}", dur);
    EXPECT_EQ(result, "01");
    
    // Test with precision specifier .6
    result = fmt::format("{:.6%S}", dur);
    EXPECT_EQ(result, "01.234560");
}

// Test formatting of seconds with chrono::duration using different rep types
TEST(ChronoWriteFractionalSecondsTest, DifferentRepTypes) {
    // Test with float rep
    auto dur = std::chrono::duration<float>(2.5f);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "02.500");
    
    // Test with long double rep
    dur = std::chrono::duration<long double>(3.14159L);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "03.14159");
}

// Test formatting of seconds with different period types
TEST(ChronoWriteFractionalSecondsTest, DifferentPeriodTypes) {
    // Test with milliseconds period
    auto dur = std::chrono::milliseconds(1500);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");
    
    // Test with microseconds period
    auto dur_us = std::chrono::microseconds(2500000);
    result = fmt::format("{:%S}", dur_us);
    EXPECT_EQ(result, "02.500000");
    
    // Test with nanoseconds period
    auto dur_ns = std::chrono::nanoseconds(1234567890);
    result = fmt::format("{:%S}", dur_ns);
    EXPECT_EQ(result, "01.234567890");
}

// Test formatting of seconds with zero duration
TEST(ChronoWriteFractionalSecondsTest, ZeroDuration) {
    using namespace std::chrono_literals;
    
    // Test zero seconds with double rep
    auto dur = std::chrono::duration<double>(0.0);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.0");
    
    // Test zero seconds with integer rep
    dur = std::chrono::seconds(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00");
}

// Test formatting of seconds with very large values
TEST(ChronoWriteFractionalSecondsTest, LargeValues) {
    using namespace std::chrono_literals;
    
    // Test 1000 seconds (should wrap to minutes, but we only format seconds part)
    auto dur = std::chrono::duration<double>(1000.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "40.500");  // 1000.5 mod 60 = 40.5
    
    // Test 3600 seconds (1 hour)
    dur = std::chrono::duration<double>(3600.25);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.250");  // 3600.25 mod 60 = 0.25
}

// Test formatting of seconds with very small negative values near zero
TEST(ChronoWriteFractionalSecondsTest, NegativeNearZero) {
    using namespace std::chrono_literals;
    
    // Test -0.001 seconds
    auto dur = std::chrono::duration<double>(-0.001);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-00.001");
    
    // Test -0.000001 seconds
    dur = std::chrono::duration<double>(-0.000001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-00.000001");
}
```

---

## id 9 - `fmtlib___fmt@cd7202e039` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@cd7202e039/deepseek-chat/buggy/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test formatting of seconds with fractional parts for various durations
TEST(ChronoWriteFractionalSecondsTest, SecondsWithFractional) {
    using namespace std::chrono_literals;

    // Test 1.5 seconds
    auto dur = std::chrono::duration<double>(1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");

    // Test 0.25 seconds
    dur = std::chrono::duration<double>(0.25);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.250");

    // Test 59.999 seconds
    dur = std::chrono::duration<double>(59.999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999");
}

// Test formatting of seconds with no fractional part (integer seconds)
TEST(ChronoWriteFractionalSecondsTest, IntegerSeconds) {
    using namespace std::chrono_literals;

    // Test exactly 0 seconds
    auto dur = std::chrono::seconds(0);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00");

    // Test exactly 30 seconds
    dur = std::chrono::seconds(30);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "30");

    // Test exactly 59 seconds
    dur = std::chrono::seconds(59);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59");
}

// Test formatting with milliseconds precision
TEST(ChronoWriteFractionalSecondsTest, Milliseconds) {
    using namespace std::chrono_literals;

    // Test 1 second and 500 milliseconds
    auto dur = std::chrono::milliseconds(1500);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");

    // Test 0 milliseconds
    dur = std::chrono::milliseconds(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000");

    // Test 999 milliseconds
    dur = std::chrono::milliseconds(999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.999");
}

// Test formatting with microseconds precision
TEST(ChronoWriteFractionalSecondsTest, Microseconds) {
    using namespace std::chrono_literals;

    // Test 1 second and 500 microseconds
    auto dur = std::chrono::microseconds(1000500);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.000500");

    // Test 0 microseconds
    dur = std::chrono::microseconds(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000000");

    // Test 999999 microseconds
    dur = std::chrono::microseconds(999999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.999999");
}

// Test formatting with nanoseconds precision
TEST(ChronoWriteFractionalSecondsTest, Nanoseconds) {
    using namespace std::chrono_literals;

    // Test 1 second and 500 nanoseconds
    auto dur = std::chrono::nanoseconds(1000000500);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.000000500");

    // Test 0 nanoseconds
    dur = std::chrono::nanoseconds(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000000000");

    // Test 999999999 nanoseconds
    dur = std::chrono::nanoseconds(999999999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.999999999");
}

// Test formatting with floating-point duration types
TEST(ChronoWriteFractionalSecondsTest, FloatingPointDuration) {
    // Test with float
    auto dur = std::chrono::duration<float>(2.75f);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "02.750");

    // Test with double
    dur = std::chrono::duration<double>(0.001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.001");

    // Test with long double
    dur = std::chrono::duration<long double>(1.0L);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.000");
}

// Test edge case: very small fractional seconds
TEST(ChronoWriteFractionalSecondsTest, VerySmallFraction) {
    using namespace std::chrono_literals;

    // Test 1 nanosecond
    auto dur = std::chrono::nanoseconds(1);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000000001");

    // Test 1 microsecond
    dur = std::chrono::microseconds(1);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000001");
}

// Test edge case: duration with negative values (should not happen in normal time but test behavior)
TEST(ChronoWriteFractionalSecondsTest, NegativeDuration) {
    using namespace std::chrono_literals;

    // Test -1.5 seconds
    auto dur = std::chrono::duration<double>(-1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-01.500");

    // Test -0.25 seconds
    dur = std::chrono::duration<double>(-0.25);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-00.250");
}

// Test formatting with custom duration types
TEST(ChronoWriteFractionalSecondsTest, CustomDuration) {
    // Use a duration with a different tick period (e.g., 1/3 second)
    using third_seconds = std::chrono::duration<int, std::ratio<1, 3>>;
    auto dur = third_seconds(5); // 5/3 = 1.666... seconds
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.666");

    // Test with 1/7 second ticks
    using seventh_seconds = std::chrono::duration<int, std::ratio<1, 7>>;
    auto dur2 = seventh_seconds(10); // 10/7 ≈ 1.428571 seconds
    result = fmt::format("{:%S}", dur2);
    EXPECT_EQ(result, "01.428571");
}

// Test that the function handles the case where seconds are part of a larger time format
TEST(ChronoWriteFractionalSecondsTest, InLargerFormat) {
    using namespace std::chrono_literals;

    // Test with hours and minutes
    auto dur = std::chrono::hours(1) + std::chrono::minutes(30) + std::chrono::seconds(15) + std::chrono::milliseconds(500);
    std::string result = fmt::format("{:%H:%M:%S}", dur);
    EXPECT_EQ(result, "01:30:15.500");

    // Test with just minutes and seconds
    auto dur2 = std::chrono::minutes(5) + std::chrono::seconds(45) + std::chrono::microseconds(123456);
    result = fmt::format("{:%M:%S}", dur2);
    EXPECT_EQ(result, "05:45.123456");
}
```

---

## id 10 - `fmtlib___fmt@cd7202e039` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@cd7202e039/deepseek-chat/buggy/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test formatting of seconds with fractional part for various durations
TEST(ChronoWriteFractionalSecondsTest, SecondsWithFractional) {
    using namespace std::chrono_literals;
    
    // Test 1.5 seconds
    auto dur = 1500ms;
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");
    
    // Test 0.5 seconds
    dur = 500ms;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.500");
    
    // Test 1.25 seconds
    dur = 1250ms;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.250");
}

// Test formatting of seconds with no fractional part (integer seconds)
TEST(ChronoWriteFractionalSecondsTest, IntegerSeconds) {
    using namespace std::chrono_literals;
    
    // Exactly 1 second
    auto dur = 1s;
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01");
    
    // Exactly 0 seconds
    dur = 0s;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00");
    
    // Exactly 59 seconds
    dur = 59s;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59");
}

// Test formatting with sub-second precision using microseconds
TEST(ChronoWriteFractionalSecondsTest, MicrosecondPrecision) {
    using namespace std::chrono_literals;
    
    // 1.000001 seconds
    auto dur = 1s + 1us;
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.000001");
    
    // 0.000001 seconds
    dur = 1us;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000001");
}

// Test formatting with nanoseconds
TEST(ChronoWriteFractionalSecondsTest, NanosecondPrecision) {
    using namespace std::chrono_literals;
    
    // 1.000000001 seconds
    auto dur = 1s + 1ns;
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.000000001");
    
    // 0.000000001 seconds
    dur = 1ns;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000000001");
}

// Test edge case: very small fractional seconds
TEST(ChronoWriteFractionalSecondsTest, VerySmallFraction) {
    using namespace std::chrono_literals;
    
    // 0.0001 seconds (100 microseconds)
    auto dur = 100us;
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000100");
    
    // 0.00001 seconds (10 microseconds)
    dur = 10us;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000010");
}

// Test edge case: duration with Rep type that is not integer (floating point)
TEST(ChronoWriteFractionalSecondsTest, FloatingPointDuration) {
    using Fsec = std::chrono::duration<double>;
    
    // 1.5 seconds using double
    Fsec dur(1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");
    
    // 0.333 seconds using double
    dur = Fsec(0.333);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.333");
}

// Test edge case: duration with custom period (e.g., tenths of seconds)
TEST(ChronoWriteFractionalSecondsTest, CustomPeriodDuration) {
    using Tenths = std::chrono::duration<int, std::ratio<1, 10>>;
    
    // 1.5 seconds = 15 tenths
    Tenths dur(15);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.5");
    
    // 0.3 seconds = 3 tenths
    dur = Tenths(3);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.3");
}

// Test edge case: duration with period that doesn't evenly divide into seconds
TEST(ChronoWriteFractionalSecondsTest, NonStandardPeriod) {
    using Thirds = std::chrono::duration<int, std::ratio<1, 3>>;
    
    // 1 second = 3 thirds
    Thirds dur(3);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.0");
    
    // 2/3 second = 2 thirds
    dur = Thirds(2);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.666666667");
}

// Test that negative durations are handled (if supported)
TEST(ChronoWriteFractionalSecondsTest, NegativeDuration) {
    using namespace std::chrono_literals;
    
    // Negative duration
    auto dur = -1500ms;
    std::string result = fmt::format("{:%S}", dur);
    // Note: fmt typically handles negative durations by formatting the absolute value
    // and prepending a minus sign in the overall format, but for %S alone it may
    // just format the absolute seconds part
    EXPECT_TRUE(result == "01.500" || result == "-01.500");
}

// Test that zero duration with fractional part works
TEST(ChronoWriteFractionalSecondsTest, ZeroDurationWithFraction) {
    using namespace std::chrono_literals;
    
    // Zero duration
    auto dur = 0ms;
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000");
    
    // Zero duration with higher precision
    auto dur_ns = 0ns;
    result = fmt::format("{:%S}", dur_ns);
    EXPECT_EQ(result, "00.000000000");
}

// Test that the function correctly handles durations that would overflow if not careful
TEST(ChronoWriteFractionalSecondsTest, LargeDuration) {
    using namespace std::chrono_literals;
    
    // Large number of seconds (but still within reasonable range)
    auto dur = 3600s + 500ms;  // 1 hour + 0.5 seconds
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.500");  // %S only shows seconds part (0-59)
    
    // 59.999 seconds
    dur = 59s + 999ms;
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999");
}
```

---

## id 11 - `fmtlib___fmt@cd7202e039` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@cd7202e039/deepseek-chat/buggy/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/chrono.h>
#include <chrono>
#include <string>

// Test formatting of seconds with fractional seconds for various durations
TEST(ChronoWriteFractionalSecondsTest, SecondsWithFractional) {
    // Test 1.5 seconds
    auto dur = std::chrono::duration<double>(1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "01.500");

    // Test 0.001 seconds (1 millisecond)
    dur = std::chrono::duration<double>(0.001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.001");

    // Test 59.999 seconds (near max seconds)
    dur = std::chrono::duration<double>(59.999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999");
}

// Test formatting of seconds with zero fractional part
TEST(ChronoWriteFractionalSecondsTest, SecondsWithZeroFractional) {
    auto dur = std::chrono::duration<int>(30);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "30.000");

    dur = std::chrono::duration<int>(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000");
}

// Test formatting of seconds with very small fractional values
TEST(ChronoWriteFractionalSecondsTest, VerySmallFractional) {
    auto dur = std::chrono::duration<double>(0.000001);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000001");

    dur = std::chrono::duration<double>(0.000000001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000000001");
}

// Test formatting of seconds with very large fractional values (near 1 second)
TEST(ChronoWriteFractionalSecondsTest, LargeFractional) {
    auto dur = std::chrono::duration<double>(0.999999999);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.999999999");

    dur = std::chrono::duration<double>(59.999999999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999999999");
}

// Test formatting of seconds with negative values (if supported)
TEST(ChronoWriteFractionalSecondsTest, NegativeSeconds) {
    auto dur = std::chrono::duration<double>(-1.5);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-01.500");

    dur = std::chrono::duration<double>(-0.001);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "-00.001");
}

// Test formatting with different duration types (milliseconds, microseconds, etc.)
TEST(ChronoWriteFractionalSecondsTest, DifferentDurationTypes) {
    // Milliseconds
    auto ms = std::chrono::milliseconds(1500);
    std::string result = fmt::format("{:%S}", ms);
    EXPECT_EQ(result, "01.500");

    // Microseconds
    auto us = std::chrono::microseconds(1234567);
    result = fmt::format("{:%S}", us);
    EXPECT_EQ(result, "01.234567");

    // Nanoseconds
    auto ns = std::chrono::nanoseconds(1234567890);
    result = fmt::format("{:%S}", ns);
    EXPECT_EQ(result, "01.234567890");
}

// Test formatting with custom Rep types (e.g., float)
TEST(ChronoWriteFractionalSecondsTest, FloatRep) {
    auto dur = std::chrono::duration<float>(2.5f);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "02.500");

    dur = std::chrono::duration<float>(0.1f);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.100");
}

// Test that the function correctly handles the boundary between seconds and minutes
TEST(ChronoWriteFractionalSecondsTest, BoundaryAt60Seconds) {
    auto dur = std::chrono::duration<double>(60.0);
    std::string result = fmt::format("{:%S}", dur);
    // At exactly 60 seconds, the seconds part should be 00 (since it rolls over to minutes)
    EXPECT_EQ(result, "00.000");

    dur = std::chrono::duration<double>(59.999999);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "59.999999");
}

// Test with zero duration
TEST(ChronoWriteFractionalSecondsTest, ZeroDuration) {
    auto dur = std::chrono::duration<double>(0.0);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000");

    dur = std::chrono::duration<int>(0);
    result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "00.000");
}

// Test with very large duration values
TEST(ChronoWriteFractionalSecondsTest, LargeDuration) {
    auto dur = std::chrono::duration<double>(123456.789);
    std::string result = fmt::format("{:%S}", dur);
    EXPECT_EQ(result, "36.789");  // 123456 seconds = 34 hours, 17 minutes, 36 seconds
}
```

---

## id 12 - `fmtlib___fmt@971fb584c3` - model gpt-5.4-or

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@971fb584c3/gpt-5.4-or/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>

#include <stdexcept>
#include <string>

TEST(FmtNamedArgumentLookupTest, ReturnsNamedStringArgument) {
  std::string result = fmt::format("{name}", fmt::arg("name", "value"));
  EXPECT_EQ(result, "value");
}

TEST(FmtNamedArgumentLookupTest, ReturnsNamedIntegerArgument) {
  std::string result = fmt::format("{count}", fmt::arg("count", 42));
  EXPECT_EQ(result, "42");
}

TEST(FmtNamedArgumentLookupTest, SupportsEmptyArgumentName) {
  std::string result = fmt::format("{0:s}|{1}", "", fmt::arg("", 7));
  EXPECT_EQ(result, "|7");
}

TEST(FmtNamedArgumentLookupTest, ReusesSameNamedArgumentMultipleTimes) {
  std::string result = fmt::format("{item}-{item}-{item}", fmt::arg("item", 9));
  EXPECT_EQ(result, "9-9-9");
}

TEST(FmtNamedArgumentLookupTest, FindsNamedArgumentAmongPositionalArguments) {
  std::string result = fmt::format("{} {name} {}", 1, fmt::arg("name", 2), 3);
  EXPECT_EQ(result, "1 2 3");
}

TEST(FmtNamedArgumentLookupTest, MissingNamedArgumentThrowsFormatError) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{missing}", fmt::arg("present", 1));
        } catch (const fmt::format_error& e) {
          EXPECT_EQ(std::string(e.what()), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, MissingNamedArgumentWithNoArgumentsThrowsFormatError) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{name}");
        } catch (const fmt::format_error& e) {
          EXPECT_EQ(std::string(e.what()), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, MissingEmptyNamedArgumentThrowsFormatError) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{0}", fmt::arg("other", 1));
        } catch (const fmt::format_error& e) {
          EXPECT_EQ(std::string(e.what()), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, NameLookupIsExact) {
  std::string result = fmt::format("{a} {ab}", fmt::arg("a", 1), fmt::arg("ab", 2));
  EXPECT_EQ(result, "1 2");
}

TEST(FmtNamedArgumentLookupTest, NameLookupIsCaseSensitive) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{Name}", fmt::arg("name", 5));
        } catch (const fmt::format_error& e) {
          EXPECT_EQ(std::string(e.what()), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, WorksWithWideCharacterFormatStrings) {
  std::wstring result = fmt::format(L"{0}:{1}", L"id", 11);
  EXPECT_EQ(result, L"id:11");
}

TEST(FmtNamedArgumentLookupTest, MissingWideNamedArgumentThrowsFormatError) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format(L"{0}", fmt::arg(L"present", 1));
        } catch (const fmt::format_error& e) {
          EXPECT_EQ(std::string(e.what()), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}
```

---

## id 13 - `fmtlib___fmt@971fb584c3` - model gpt-5.4-or

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@971fb584c3/gpt-5.4-or/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>

#include <fmt/format.h>

#include <string>

namespace {

TEST(FmtNamedArgumentLookupTest, RetrievesNamedArgumentByName) {
  std::string result = fmt::format("Hello, {name}!", fmt::arg("name", "world"));
  EXPECT_EQ(result, "Hello, world!");
}

TEST(FmtNamedArgumentLookupTest, RetrievesMultipleNamedArgumentsAndPreservesMapping) {
  std::string result =
      fmt::format("{greeting}, {name}! You have {count} messages.",
                  fmt::arg("greeting", "Hello"),
                  fmt::arg("name", "Alice"),
                  fmt::arg("count", 3));
  EXPECT_EQ(result, "Hello, Alice! You have 3 messages.");
}

TEST(FmtNamedArgumentLookupTest, RepeatedLookupOfSameNamedArgumentWorks) {
  std::string result = fmt::format("{item}-{item}-{item}", fmt::arg("item", 42));
  EXPECT_EQ(result, "42-42-42");
}

TEST(FmtNamedArgumentLookupTest, EmptyArgumentNameCanBeResolved) {
  std::string result = fmt::format("value=[{}]", fmt::arg("", 7));
  EXPECT_EQ(result, "value=[7]");
}

TEST(FmtNamedArgumentLookupTest, MissingNamedArgumentThrowsFormatError) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("Value: {missing}", fmt::arg("present", 1));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ(e.what(), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, MissingNamedArgumentAmongExistingNamedArgumentsThrowsFormatError) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{a} {missing} {b}",
                            fmt::arg("a", "left"),
                            fmt::arg("b", "right"));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ(e.what(), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, NameLookupIsCaseSensitive) {
  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{Name}", fmt::arg("name", 123));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ(e.what(), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, SimilarPrefixesDoNotMatchWrongArgument) {
  std::string result = fmt::format("{id}:{id2}:{id}",
                                   fmt::arg("id", 10),
                                   fmt::arg("id2", 20));
  EXPECT_EQ(result, "10:20:10");
}

TEST(FmtNamedArgumentLookupTest, UnicodeLikeBytesInArgumentNameAreMatchedExactly) {
  const char* key = u8"naïve";
  std::string result = fmt::format("{naïve}", fmt::arg(key, "ok"));
  EXPECT_EQ(result, "ok");

  EXPECT_THROW(
      {
        try {
          (void)fmt::format("{naive}", fmt::arg(key, "ok"));
        } catch (const fmt::format_error& e) {
          EXPECT_STREQ(e.what(), "argument not found");
          throw;
        }
      },
      fmt::format_error);
}

TEST(FmtNamedArgumentLookupTest, NamedArgumentWorksWithFormatToBufferAPI) {
  fmt::memory_buffer buffer;
  fmt::format_to(buffer,
                 "[{first}|{second}|{first}]",
                 fmt::arg("first", "A"),
                 fmt::arg("second", "B"));
  EXPECT_EQ(fmt::to_string(buffer), "[A|B|A]");
}

}  // namespace
```

---

## id 14 - `fmtlib___fmt@971fb584c3` - model qwen3-coder

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@971fb584c3/qwen3-coder/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <stdexcept>

using namespace fmt;

TEST(BasicContextTest, GetArgWithNamedArguments) {
    // Test normal case with named arguments
    auto result = format("{first} {second}", 
                        fmt::arg("first", "Hello"), 
                        fmt::arg("second", "World"));
    EXPECT_EQ("Hello World", result);
}

TEST(BasicContextTest, GetArgWithEmptyName) {
    // Test with empty argument name
    auto result = format("{name}", fmt::arg("", "empty"));
    EXPECT_EQ("empty", result);
}

TEST(BasicContextTest, GetArgWithSpecialCharacters) {
    // Test with special characters in argument names
    auto result = format("{a@b} {1-2}", fmt::arg("a@b", "special"), fmt::arg("1-2", "hyphen"));
    EXPECT_EQ("special hyphen", result);
}

TEST(BasicContextTest, GetArgWithNumericNames) {
    // Test with numeric argument names
    auto result = format("{1} {0}", fmt::arg("1", "one"), fmt::arg("0", "zero"));
    EXPECT_EQ("one zero", result);
}

TEST(BasicContextTest, GetArgMultipleArguments) {
    // Test with multiple arguments
    auto result = format("{a} {b} {c}", 
                        fmt::arg("a", 1), 
                        fmt::arg("b", 2.5), 
                        fmt::arg("c", "test"));
    EXPECT_EQ("1 2.5 test", result);
}

TEST(BasicContextTest, GetArgRepeatedNames) {
    // Test with repeated argument names (should use first occurrence)
    auto result = format("{name} {name}", fmt::arg("name", "test"));
    EXPECT_EQ("test test", result);
}

TEST(BasicContextTest, GetArgMissingArgument) {
    // Test that missing argument triggers error handling
    // This should not crash but rather throw an exception or handle gracefully
    try {
        // This should trigger the error path in get_arg
        auto result = format("{missing_arg}");
        // If we get here without exception, it's acceptable behavior
        // but we want to ensure it doesn't crash
    } catch (const std::exception&) {
        // Expected behavior - exception thrown for missing argument
    }
}

TEST(BasicContextTest, GetArgMixedNamedAndPositional) {
    // Test mixing named and positional arguments
    auto result = format("{0} {name} {1}", 
                        fmt::arg("0", "pos0"), 
                        fmt::arg("name", "named"), 
                        fmt::arg("1", "pos1"));
    EXPECT_EQ("pos0 named pos1", result);
}

TEST(BasicContextTest, GetArgComplexFormatting) {
    // Test complex formatting with named arguments
    auto result = format("{name:*>10} {value:.2f}", 
                        fmt::arg("name", "test"), 
                        fmt::arg("value", 3.14159));
    EXPECT_EQ("******test 3.14", result);
}

TEST(BasicContextTest, GetArgUnicodeNames) {
    // Test with Unicode argument names
    auto result = format("{α} {β}", fmt::arg("α", "alpha"), fmt::arg("β", "beta"));
    EXPECT_EQ("alpha beta", result);
}

TEST(BasicContextTest, GetArgLongNames) {
    // Test with very long argument names
    std::string long_name(1000, 'x');
    auto result = format("{long_name}", fmt::arg(long_name, "long"));
    EXPECT_EQ("long", result);
}

TEST(BasicContextTest, GetArgSingleCharacterNames) {
    // Test with single character argument names
    auto result = format("{a} {b} {c}", 
                        fmt::arg("a", "A"), 
                        fmt::arg("b", "B"), 
                        fmt::arg("c", "C"));
    EXPECT_EQ("A B C", result);
}

TEST(BasicContextTest, GetArgWithSpacesInNames) {
    // Test with spaces in argument names (should work as literal)
    auto result = format("{first name} {last name}", 
                        fmt::arg("first name", "John"), 
                        fmt::arg("last name", "Doe"));
    EXPECT_EQ("John Doe", result);
}

TEST(BasicContextTest, GetArgWithUnderscores) {
    // Test with underscores in argument names
    auto result = format("{first_name} {last_name}", 
                        fmt::arg("first_name", "John"), 
                        fmt::arg("last_name", "Doe"));
    EXPECT_EQ("John Doe", result);
}

TEST(BasicContextTest, GetArgWithNumbersInNames) {
    // Test with numbers in argument names
    auto result = format("{arg1} {arg2}", 
                        fmt::arg("arg1", "one"), 
                        fmt::arg("arg2", "two"));
    EXPECT_EQ("one two", result);
}
```

---

## id 15 - `fmtlib___fmt@971fb584c3` - model qwen3-coder

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@971fb584c3/qwen3-coder/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <stdexcept>

TEST(BasicContextTest, GetArgWithNamedArgument) {
    // Test normal case with named argument
    auto result = fmt::format("{first} {second}", 
                             fmt::arg("first", "Hello"), 
                             fmt::arg("second", "World"));
    EXPECT_EQ(result, "Hello World");
}

TEST(BasicContextTest, GetArgWithMissingNamedArgument) {
    // Test error case when argument is not found
    EXPECT_THROW(
        fmt::format("{missing}", fmt::arg("present", "value")),
        std::runtime_error
    );
}

TEST(BasicContextTest, GetArgWithEmptyName) {
    // Test with empty argument name
    auto result = fmt::format("{empty}", fmt::arg("", "value"));
    EXPECT_EQ(result, "value");
}

TEST(BasicContextTest, GetArgWithMultipleArguments) {
    // Test multiple named arguments
    auto result = fmt::format("{a} {b} {c}", 
                             fmt::arg("a", 1), 
                             fmt::arg("b", 2.5), 
                             fmt::arg("c", "test"));
    EXPECT_EQ(result, "1 2.5 test");
}

TEST(BasicContextTest, GetArgWithNumericArguments) {
    // Test numeric argument names (should work as strings)
    auto result = fmt::format("{0} {1}", fmt::arg("0", "zero"), fmt::arg("1", "one"));
    EXPECT_EQ(result, "zero one");
}

TEST(BasicContextTest, GetArgWithSpecialCharacters) {
    // Test argument names with special characters
    auto result = fmt::format("{hello-world} {foo.bar}", 
                             fmt::arg("hello-world", "test1"), 
                             fmt::arg("foo.bar", "test2"));
    EXPECT_EQ(result, "test1 test2");
}

TEST(BasicContextTest, GetArgWithRepeatedNames) {
    // Test that repeated argument names work correctly
    auto result = fmt::format("{name} {name}", fmt::arg("name", "test"));
    EXPECT_EQ(result, "test test");
}

TEST(BasicContextTest, GetArgWithComplexTypes) {
    // Test with complex types
    std::string str = "hello";
    auto result = fmt::format("{str} {num}", 
                             fmt::arg("str", str), 
                             fmt::arg("num", 42));
    EXPECT_EQ(result, "hello 42");
}

TEST(BasicContextTest, GetArgWithZeroValues) {
    // Test with zero values
    auto result = fmt::format("{zero} {empty}", 
                             fmt::arg("zero", 0), 
                             fmt::arg("empty", ""));
    EXPECT_EQ(result, "0 ");
}

TEST(BasicContextTest, GetArgWithOnlyNamedArguments) {
    // Test format string with only named arguments
    auto result = fmt::format("{greeting} {subject}!", 
                             fmt::arg("greeting", "Hello"), 
                             fmt::arg("subject", "World"));
    EXPECT_EQ(result, "Hello World!");
}

TEST(BasicContextTest, GetArgWithMixedArguments) {
    // Test mixed positional and named arguments
    auto result = fmt::format("{} {named} {}", 
                             "pos1", 
                             fmt::arg("named", "named_value"), 
                             "pos2");
    EXPECT_EQ(result, "pos1 named_value pos2");
}
```

---

## id 16 - `fmtlib___fmt@971fb584c3` - model qwen3-coder

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@971fb584c3/qwen3-coder/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <stdexcept>

TEST(BasicContextTest, GetArgWithNamedArgument) {
    // Test normal case with named argument
    auto result = fmt::format("{foo}", fmt::arg("foo", 42));
    EXPECT_EQ(result, "42");
}

TEST(BasicContextTest, GetArgWithMultipleNamedArguments) {
    // Test with multiple named arguments
    auto result = fmt::format("{foo} {bar}", fmt::arg("foo", 1), fmt::arg("bar", 2));
    EXPECT_EQ(result, "1 2");
}

TEST(BasicContextTest, GetArgWithEmptyName) {
    // Test with empty argument name
    auto result = fmt::format("{foo}", fmt::arg("", 42));
    EXPECT_EQ(result, "42");
}

TEST(BasicContextTest, GetArgWithSpecialCharactersInName) {
    // Test with special characters in argument name
    auto result = fmt::format("{foo-bar}", fmt::arg("foo-bar", 42));
    EXPECT_EQ(result, "42");
}

TEST(BasicContextTest, GetArgWithNonExistentArgument) {
    // Test accessing non-existent argument should trigger error
    // We expect this to throw or handle error appropriately
    try {
        auto result = fmt::format("{missing}", fmt::arg("foo", 42));
        // If we reach here, it means the error wasn't properly handled
        FAIL() << "Expected error when accessing non-existent argument";
    } catch (const std::runtime_error& e) {
        // This is expected behavior
    }
}

TEST(BasicContextTest, GetArgWithNumericArgument) {
    // Test with numeric argument names
    auto result = fmt::format("{123}", fmt::arg("123", "test"));
    EXPECT_EQ(result, "test");
}

TEST(BasicContextTest, GetArgWithComplexFormat) {
    // Test complex formatting with named arguments
    auto result = fmt::format("{name} is {age} years old", 
                             fmt::arg("name", "Alice"), 
                             fmt::arg("age", 30));
    EXPECT_EQ(result, "Alice is 30 years old");
}

TEST(BasicContextTest, GetArgWithMixedArguments) {
    // Test mixing positional and named arguments
    auto result = fmt::format("Hello {0}, {name}!", 
                             fmt::arg("0", "World"), 
                             fmt::arg("name", "Alice"));
    EXPECT_EQ(result, "Hello World, Alice!");
}

TEST(BasicContextTest, GetArgWithRepeatedNames) {
    // Test with repeated argument names (should use first occurrence)
    auto result = fmt::format("{foo} {foo}", fmt::arg("foo", "test"));
    EXPECT_EQ(result, "test test");
}

TEST(BasicContextTest, GetArgWithEmptyFormatString) {
    // Test with empty format string
    auto result = fmt::format("", fmt::arg("foo", 42));
    EXPECT_EQ(result, "");
}

TEST(BasicContextTest, GetArgWithOnlyNamedArguments) {
    // Test with only named arguments and no positional
    auto result = fmt::format("{foo} {bar} {baz}", 
                             fmt::arg("foo", "a"), 
                             fmt::arg("bar", "b"), 
                             fmt::arg("baz", "c"));
    EXPECT_EQ(result, "a b c");
}

TEST(BasicContextTest, GetArgWithNestedFormatting) {
    // Test nested formatting scenarios
    auto result = fmt::format("{outer}", fmt::arg("outer", fmt::format("{inner}", fmt::arg("inner", "value"))));
    EXPECT_EQ(result, "value");
}

TEST(BasicContextTest, GetArgWithLargeStringArgument) {
    // Test with large string argument
    std::string large_str(1000, 'x');
    auto result = fmt::format("{test}", fmt::arg("test", large_str));
    EXPECT_EQ(result, large_str);
}

TEST(BasicContextTest, GetArgWithZeroValue) {
    // Test with zero value
    auto result = fmt::format("{zero}", fmt::arg("zero", 0));
    EXPECT_EQ(result, "0");
}

TEST(BasicContextTest, GetArgWithNegativeValue) {
    // Test with negative value
    auto result = fmt::format("{neg}", fmt::arg("neg", -42));
    EXPECT_EQ(result, "-42");
}

TEST(BasicContextTest, GetArgWithFloatValue) {
    // Test with floating point value
    auto result = fmt::format("{pi}", fmt::arg("pi", 3.14159));
    EXPECT_EQ(result, "3.14159");
}
```

---

## id 17 - `fmtlib___fmt@96c18b26c2` - model gemini-2.5-flash

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@96c18b26c2/gemini-2.5-flash/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
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
```

---

## id 18 - `fmtlib___fmt@96c18b26c2` - model gemini-2.5-flash

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@96c18b26c2/gemini-2.5-flash/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
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
... (cat bot 16 dong)
```

---

## id 19 - `fmtlib___fmt@96c18b26c2` - model qwen3-coder

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@96c18b26c2/qwen3-coder/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <string>
#include <stdexcept>

TEST(PrintfTest, FormatIntegerWithSignAndFlags) {
    // Test normal integer formatting with sign
    std::string result = fmt::sprintf("%d", 42);
    EXPECT_EQ(result, "42");
    
    // Test negative integer
    result = fmt::sprintf("%d", -42);
    EXPECT_EQ(result, "-42");
    
    // Test with plus sign flag
    result = fmt::sprintf("%+d", 42);
    EXPECT_EQ(result, "+42");
    
    // Test with space flag
    result = fmt::sprintf("% d", 42);
    EXPECT_EQ(result, " 42");
    
    // Test with zero padding
    result = fmt::sprintf("%05d", 42);
    EXPECT_EQ(result, "00042");
    
    // Test with alt flag (should not affect integers)
    result = fmt::sprintf("%#d", 42);
    EXPECT_EQ(result, "42");
}

TEST(PrintfTest, FormatIntegerEdgeCases) {
    // Test zero
    std::string result = fmt::sprintf("%d", 0);
    EXPECT_EQ(result, "0");
    
    // Test maximum positive integer
    result = fmt::sprintf("%d", 2147483647);
    EXPECT_EQ(result, "2147483647");
    
    // Test minimum negative integer
    result = fmt::sprintf("%d", -2147483648);
    EXPECT_EQ(result, "-2147483648");
    
    // Test with width specification
    result = fmt::sprintf("%5d", 42);
    EXPECT_EQ(result, "   42");
    
    // Test with negative width (should be left-aligned)
    result = fmt::sprintf("%-5d", 42);
    EXPECT_EQ(result, "42   ");
}

TEST(PrintfTest, FormatUnsignedInteger) {
    // Test unsigned integer formatting
    std::string result = fmt::sprintf("%u", 42u);
    EXPECT_EQ(result, "42");
    
    // Test unsigned with zero padding
    result = fmt::sprintf("%05u", 42u);
    EXPECT_EQ(result, "00042");
    
    // Test unsigned with alt flag
    result = fmt::sprintf("%#u", 42u);
    EXPECT_EQ(result, "42");
}

TEST(PrintfTest, FormatHexadecimal) {
    // Test hexadecimal formatting
    std::string result = fmt::sprintf("%x", 255);
    EXPECT_EQ(result, "ff");
    
    // Test uppercase hexadecimal
    result = fmt::sprintf("%X", 255);
    EXPECT_EQ(result, "FF");
    
    // Test with alt flag
    result = fmt::sprintf("%#x", 255);
    EXPECT_EQ(result, "0xff");
    
    // Test with zero padding
    result = fmt::sprintf("%08x", 255);
    EXPECT_EQ(result, "000000ff");
}

TEST(PrintfTest, FormatOctal) {
    // Test octal formatting
    std::string result = fmt::sprintf("%o", 64);
    EXPECT_EQ(result, "100");
    
    // Test with alt flag
    result = fmt::sprintf("%#o", 64);
    EXPECT_EQ(result, "0100");
    
    // Test with zero padding
    result = fmt::sprintf("%05o", 64);
    EXPECT_EQ(result, "00100");
}

TEST(PrintfTest, FormatCharacter) {
    // Test character formatting
    std::string result = fmt::sprintf("%c", 'A');
    EXPECT_EQ(result, "A");
    
    // Test with zero padding
    result = fmt::sprintf("%05c", 'A');
    EXPECT_EQ(result, "    A");
}

TEST(PrintfTest, FormatString) {
    // Test string formatting
    std::string result = fmt::sprintf("%s", "hello");
    EXPECT_EQ(result, "hello");
    
    // Test with width specification
    result = fmt::sprintf("%10s", "hello");
    EXPECT_EQ(result, "     hello");
    
    // Test with precision
    result = fmt::sprintf("%.3s", "hello");
    EXPECT_EQ(result, "hel");
}

TEST(PrintfTest, FormatFloat) {
    // Test float formatting
    std::string result = fmt::sprintf("%f", 3.14159);
    EXPECT_EQ(result, "3.141590");
    
    // Test with precision
    result = fmt::sprintf("%.2f", 3.14159);
    EXPECT_EQ(result, "3.14");
    
    // Test scientific notation
    result = fmt::sprintf("%e", 123.456);
    EXPECT_EQ(result, "1.234560e+02");
    
    // Test with width
    result = fmt::sprintf("%10f", 3.14159);
    EXPECT_EQ(result, "  3.141590");
}

TEST(PrintfTest, FormatPointer) {
    // Test pointer formatting
    int x = 42;
    std::string result = fmt::sprintf("%p", static_cast<void*>(&x));
    EXPECT_NE(result, "");
    EXPECT_TRUE(result.find("0x") != std::string::npos || result.find("0X") != std::string::npos);
}

TEST(PrintfTest, FormatMixed) {
    // Test mixed format specifiers
    std::string result = fmt::sprintf("Number: %d, String: %s, Float: %.2f", 42, "test", 3.14159);
    EXPECT_EQ(result, "Number: 42, String: test, Float: 3.14");
    
    // Test with multiple flags
    result = fmt::sprintf("%+05d", 42);
    EXPECT_EQ(result, "+0042");
}
```

---

## id 20 - `fmtlib___fmt@96c18b26c2` - model qwen3-coder

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@96c18b26c2/qwen3-coder/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <fmt/format.h>
#include <string>
#include <stdexcept>

TEST(PrintfTest, FormatIntegerWithSignAndFlags) {
    // Test normal integer formatting with sign
    std::string result = fmt::sprintf("%d", 42);
    EXPECT_EQ(result, "42");
    
    // Test negative integer
    result = fmt::sprintf("%d", -42);
    EXPECT_EQ(result, "-42");
    
    // Test with plus sign flag
    result = fmt::sprintf("%+d", 42);
    EXPECT_EQ(result, "+42");
    
    // Test with space flag
    result = fmt::sprintf("% d", 42);
    EXPECT_EQ(result, " 42");
    
    // Test with zero padding
    result = fmt::sprintf("%05d", 42);
    EXPECT_EQ(result, "00042");
    
    // Test with alternative format (octal)
    result = fmt::sprintf("%#o", 42);
    EXPECT_EQ(result, "052");
    
    // Test with alternative format (hex)
    result = fmt::sprintf("%#x", 42);
    EXPECT_EQ(result, "0x2a");
}

TEST(PrintfTest, FormatIntegerEdgeCases) {
    // Test zero
    std::string result = fmt::sprintf("%d", 0);
    EXPECT_EQ(result, "0");
    
    // Test maximum integer
    result = fmt::sprintf("%d", 2147483647);
    EXPECT_EQ(result, "2147483647");
    
    // Test minimum integer
    result = fmt::sprintf("%d", -2147483648);
    EXPECT_EQ(result, "-2147483648");
    
    // Test with width specification
    result = fmt::sprintf("%5d", 42);
    EXPECT_EQ(result, "   42");
    
    // Test with negative width (left alignment)
    result = fmt::sprintf("%-5d", 42);
    EXPECT_EQ(result, "42   ");
    
    // Test with zero width and zero value
    result = fmt::sprintf("%0d", 0);
    EXPECT_EQ(result, "0");
    
    // Test with zero padding and negative number
    result = fmt::sprintf("%05d", -42);
    EXPECT_EQ(result, "-0042");
}

TEST(PrintfTest, FormatIntegerAlignmentAndFlags) {
    // Test numeric alignment behavior
    std::string result = fmt::sprintf("%05d", 42);
    EXPECT_EQ(result, "00042");
    
    // Test right alignment with width
    result = fmt::sprintf("%5d", 42);
    EXPECT_EQ(result, "   42");
    
    // Test left alignment
    result = fmt::sprintf("%-5d", 42);
    EXPECT_EQ(result, "42   ");
    
    // Test with sign and padding
    result = fmt::sprintf("%+05d", 42);
    EXPECT_EQ(result, "+0042");
    
    // Test with space and padding
    result = fmt::sprintf("% 05d", 42);
    EXPECT_EQ(result, " 0042");
}

TEST(PrintfTest, FormatIntegerSpecialCases) {
    // Test formatting with various flags combinations
    std::string result = fmt::sprintf("%#05d", 42);
    EXPECT_EQ(result, "00042");
    
    // Test with multiple flags
    result = fmt::sprintf("% +05d", 42);
    EXPECT_EQ(result, "+0042");
    
    // Test with alternative format and zero padding
    result = fmt::sprintf("%#08x", 255);
    EXPECT_EQ(result, "0x0000ff");
    
    // Test with octal alternative format
    result = fmt::sprintf("%#08o", 64);
    EXPECT_EQ(result, "00000100");
}

TEST(PrintfTest, FormatIntegerBounds) {
    // Test boundary values
    std::string result = fmt::sprintf("%d", 0);
    EXPECT_EQ(result, "0");
    
    result = fmt::sprintf("%d", 1);
    EXPECT_EQ(result, "1");
    
    result = fmt::sprintf("%d", -1);
    EXPECT_EQ(result, "-1");
    
    // Test maximum positive value
    result = fmt::sprintf("%d", 2147483647);
    EXPECT_EQ(result, "2147483647");
    
    // Test minimum negative value
    result = fmt::sprintf("%d", -2147483648);
    EXPECT_EQ(result, "-2147483648");
}

TEST(PrintfTest, FormatIntegerVariousTypes) {
    // Test with different integer types
    int8_t i8 = 42;
    int16_t i16 = 42;
    int32_t i32 = 42;
    int64_t i64 = 42;
    
    std::string result = fmt::sprintf("%d", i8);
    EXPECT_EQ(result, "42");
    
    result = fmt::sprintf("%d", i16);
    EXPECT_EQ(result, "42");
    
    result = fmt::sprintf("%d", i32);
    EXPECT_EQ(result, "42");
    
    result = fmt::sprintf("%d", i64);
    EXPECT_EQ(result, "42");
}
```

---

## id 21 - `fmtlib___fmt@96c18b26c2` - model qwen3-coder

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@96c18b26c2/qwen3-coder/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <fmt/format.h>
#include <string>
#include <stdexcept>

TEST(PrintfTest, FormatIntegerWithSign) {
    // Test normal integer formatting with sign
    std::string result = fmt::sprintf("%d", 42);
    EXPECT_EQ(result, "42");
    
    result = fmt::sprintf("%d", -42);
    EXPECT_EQ(result, "-42");
}

TEST(PrintfTest, FormatIntegerWithFlags) {
    // Test formatting with various flags
    std::string result = fmt::sprintf("%05d", 42);
    EXPECT_EQ(result, "00042");
    
    result = fmt::sprintf("%+d", 42);
    EXPECT_EQ(result, "+42");
    
    result = fmt::sprintf("% d", 42);
    EXPECT_EQ(result, " 42");
    
    result = fmt::sprintf("%-5d", 42);
    EXPECT_EQ(result, "42   ");
}

TEST(PrintfTest, FormatIntegerWithAltForm) {
    // Test alternative form with #
    std::string result = fmt::sprintf("%#x", 255);
    EXPECT_EQ(result, "0xff");
    
    result = fmt::sprintf("%#o", 64);
    EXPECT_EQ(result, "0100");
}

TEST(PrintfTest, FormatIntegerEdgeCases) {
    // Test edge cases
    std::string result = fmt::sprintf("%d", 0);
    EXPECT_EQ(result, "0");
    
    result = fmt::sprintf("%d", INT_MAX);
    EXPECT_EQ(result, std::to_string(INT_MAX));
    
    result = fmt::sprintf("%d", INT_MIN);
    EXPECT_EQ(result, std::to_string(INT_MIN));
}

TEST(PrintfTest, FormatIntegerAlignment) {
    // Test alignment behavior
    std::string result = fmt::sprintf("%10d", 42);
    EXPECT_EQ(result, "        42");
    
    result = fmt::sprintf("%-10d", 42);
    EXPECT_EQ(result, "42        ");
    
    // Test numeric alignment (should behave like right alignment)
    result = fmt::sprintf("%10d", 42);
    EXPECT_EQ(result, "        42");
}

TEST(PrintfTest, FormatIntegerZeroPadding) {
    // Test zero padding behavior
    std::string result = fmt::sprintf("%05d", 42);
    EXPECT_EQ(result, "00042");
    
    result = fmt::sprintf("%010d", -42);
    EXPECT_EQ(result, "-000000042");
}

TEST(PrintfTest, FormatIntegerWidthAndPrecision) {
    // Test width specification
    std::string result = fmt::sprintf("%10d", 42);
    EXPECT_EQ(result, "        42");
    
    // Test precision (should be ignored for integers)
    result = fmt::sprintf("%.5d", 42);
    EXPECT_EQ(result, "00042");
}

TEST(PrintfTest, FormatIntegerVariousTypes) {
    // Test different integer types
    std::string result = fmt::sprintf("%d", static_cast<int>(42));
    EXPECT_EQ(result, "42");
    
    result = fmt::sprintf("%d", static_cast<long>(42));
    EXPECT_EQ(result, "42");
    
    result = fmt::sprintf("%d", static_cast<long long>(42));
    EXPECT_EQ(result, "42");
}

TEST(PrintfTest, FormatIntegerNegativeValues) {
    // Test negative values with various flags
    std::string result = fmt::sprintf("%d", -42);
    EXPECT_EQ(result, "-42");
    
    result = fmt::sprintf("%+d", -42);
    EXPECT_EQ(result, "-42");
    
    result = fmt::sprintf("% d", -42);
    EXPECT_EQ(result, "-42");
    
    result = fmt::sprintf("%05d", -42);
    EXPECT_EQ(result, "-0042");
}

TEST(PrintfTest, FormatIntegerSpecialCases) {
    // Test special cases
    std::string result = fmt::sprintf("%d", 0);
    EXPECT_EQ(result, "0");
    
    result = fmt::sprintf("%+d", 0);
    EXPECT_EQ(result, "+0");
    
    result = fmt::sprintf("% d", 0);
    EXPECT_EQ(result, " 0");
}
```

---

## id 22 - `fmtlib___fmt@96c18b26c2` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@96c18b26c2/deepseek-chat/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <string>
#include <vector>

// Test the printf formatter's behavior with various format specifications
// The focal function handles format specs for non-numeric types, specifically
// overriding align::numeric to align::right and resetting sign/alt flags

class PrintfFormatterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // No setup needed
    }
};

// Test that align::numeric is overridden to align::right for non-numeric types
TEST_F(PrintfFormatterTest, AlignNumericOverriddenToRight) {
    // Using %-10s should normally left-align, but with '0' flag it becomes right-align
    // The focal function overrides align::numeric to align::right
    std::string result = fmt::sprintf("%-10s", "test");
    EXPECT_EQ(result, "test      ");  // Left-aligned (no '0' flag, so no override)
    
    // With '0' flag and string type, align::numeric should become align::right
    result = fmt::sprintf("%010s", "test");
    EXPECT_EQ(result, "      test");  // Right-aligned with spaces (0 flag ignored for strings)
    
    // Test that align::none becomes align::right
    result = fmt::sprintf("%10s", "test");
    EXPECT_EQ(result, "      test");  // Default right-alignment
}

// Test that sign is reset to none for non-numeric types
TEST_F(PrintfFormatterTest, SignResetToNone) {
    // '+' sign should be ignored for strings
    std::string result = fmt::sprintf("%+10s", "test");
    EXPECT_EQ(result, "      test");  // No sign prefix for strings
    
    // ' ' (space) sign should be ignored for strings
    result = fmt::sprintf("% 10s", "test");
    EXPECT_EQ(result, "      test");  // No space prefix for strings
    
    // '-' sign should be ignored for strings (left-align still works)
    result = fmt::sprintf("%-+10s", "test");
    EXPECT_EQ(result, "test      ");  // Left-aligned, no sign
}

// Test that alt flag is reset to false for non-numeric types
TEST_F(PrintfFormatterTest, AltFlagResetToFalse) {
    // '#' flag should be ignored for strings
    std::string result = fmt::sprintf("%#10s", "test");
    EXPECT_EQ(result, "      test");  // No alternate form for strings
    
    // '#' flag with '0' flag
    result = fmt::sprintf("%#010s", "test");
    EXPECT_EQ(result, "      test");  // Both flags ignored for strings
}

// Test with empty string
TEST_F(PrintfFormatterTest, EmptyString) {
    std::string result = fmt::sprintf("%10s", "");
    EXPECT_EQ(result, "          ");  // 10 spaces
    
    result = fmt::sprintf("%-10s", "");
    EXPECT_EQ(result, "          ");  // 10 spaces (left-aligned empty)
    
    result = fmt::sprintf("%010s", "");
    EXPECT_EQ(result, "          ");  // 10 spaces (0 flag ignored)
}

// Test with single character
TEST_F(PrintfFormatterTest, SingleCharacter) {
    std::string result = fmt::sprintf("%5c", 'A');
    EXPECT_EQ(result, "    A");  // Right-aligned with spaces
    
    result = fmt::sprintf("%-5c", 'A');
    EXPECT_EQ(result, "A    ");  // Left-aligned
    
    result = fmt::sprintf("%05c", 'A');
    EXPECT_EQ(result, "    A");  // 0 flag ignored, right-aligned with spaces
}

// Test with pointer (non-numeric type)
TEST_F(PrintfFormatterTest, PointerType) {
    int x = 42;
    void* ptr = &x;
    
    // Pointer formatting should ignore sign and alt flags
    std::string result = fmt::sprintf("%+20p", ptr);
    EXPECT_NE(result.find("0x"), std::string::npos);  // Should contain hex prefix
    EXPECT_EQ(result.size(), 20);  // Should be padded to width
    
    // With '0' flag, align::numeric should become align::right
    result = fmt::sprintf("%020p", ptr);
    EXPECT_NE(result.find("0x"), std::string::npos);
    EXPECT_EQ(result.size(), 20);
}

// Test with custom string-like type
TEST_F(PrintfFormatterTest, StringViewType) {
    fmt::string_view sv = "hello";
    std::string result = fmt::sprintf("%10s", sv);
    EXPECT_EQ(result, "     hello");  // Right-aligned
    
    result = fmt::sprintf("%010s", sv);
    EXPECT_EQ(result, "     hello");  // 0 flag ignored, right-aligned with spaces
    
    result = fmt::sprintf("%+10s", sv);
    EXPECT_EQ(result, "     hello");  // Sign ignored
}

// Test that numeric types still respect the flags (regression test)
TEST_F(PrintfFormatterTest, NumericTypesStillWork) {
    // Integers should still use '0' padding
    std::string result = fmt::sprintf("%010d", 42);
    EXPECT_EQ(result, "0000000042");  // Zero-padded
    
    // Integers should still use '+' sign
    result = fmt::sprintf("%+10d", 42);
    EXPECT_EQ(result, "       +42");  // With plus sign
    
    // Integers should still use '#' alt flag
    result = fmt::sprintf("%#010x", 255);
    EXPECT_EQ(result, "0x000000ff");  // Hex with 0x prefix
}

// Test with very long string
TEST_F(PrintfFormatterTest, LongString) {
    std::string long_str(100, 'a');
    std::string result = fmt::sprintf("%10s", long_str);
    EXPECT_EQ(result, long_str);  // No truncation, width ignored if string longer
    
    result = fmt::sprintf("%010s", long_str);
    EXPECT_EQ(result, long_str);  // Same with '0' flag
}

// Test with various width specifications
TEST_F(PrintfFormatterTest, VariousWidths) {
    std::string result = fmt::sprintf("%1s", "test");
    EXPECT_EQ(result, "test");  // Width smaller than string
    
    result = fmt::sprintf("%0s", "test");
    EXPECT_EQ(result, "test");  // Zero width
    
    result = fmt::sprintf("%*s", 10, "test");
    EXPECT_EQ(result, "      test");  // Dynamic width
}

// Test that precision is handled correctly (not affected by focal function)
TEST_F(PrintfFormatterTest, PrecisionHandling) {
    std::string result = fmt::sprintf("%.3s", "hello");
    EXPECT_EQ(result, "hel");  // Precision limits string length
    
    result = fmt::sprintf("%10.3s", "hello");
    EXPECT_EQ(result, "       hel");  // Width with precision
    
    result = fmt::sprintf("%010.3s", "hello");
    EXPECT_EQ(result, "       hel");  // 0 flag ignored, precision still works
}
```

---

## id 23 - `fmtlib___fmt@96c18b26c2` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@96c18b26c2/deepseek-chat/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <string>
#include <vector>
#include <stdexcept>

// Test the printf formatter's behavior for various format specifications,
// particularly focusing on the handling of non-numeric types with '0' flag,
// alignment, and sign/alt flags.

class PrintfFormatterTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

// Test that '0' flag is ignored for non-numeric types (strings)
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("%010s", "hello");
    // '0' flag should be ignored, alignment should be right with spaces
    EXPECT_EQ(result, "     hello");
}

// Test that '0' flag is ignored for characters
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForChar) {
    std::string result = fmt::sprintf("%05c", 'A');
    // '0' flag should be ignored, alignment should be right with spaces
    EXPECT_EQ(result, "    A");
}

// Test that '0' flag works for numeric types (should not be affected)
TEST_F(PrintfFormatterTest, ZeroFlagWorksForIntegers) {
    std::string result = fmt::sprintf("%05d", 42);
    EXPECT_EQ(result, "00042");
}

// Test default alignment for non-numeric types (should be right)
TEST_F(PrintfFormatterTest, DefaultAlignmentForString) {
    std::string result = fmt::sprintf("%10s", "test");
    EXPECT_EQ(result, "      test");
}

// Test left alignment for strings (overrides default)
TEST_F(PrintfFormatterTest, LeftAlignmentForString) {
    std::string result = fmt::sprintf("%-10s", "test");
    EXPECT_EQ(result, "test      ");
}

// Test that numeric alignment is overwritten to right for non-numeric types
TEST_F(PrintfFormatterTest, NumericAlignmentOverwrittenForString) {
    // Using '=' alignment (numeric) should be treated as right for strings
    std::string result = fmt::sprintf("%=10s", "test");
    EXPECT_EQ(result, "      test");
}

// Test sign flag is ignored for non-numeric types
TEST_F(PrintfFormatterTest, SignFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("%+10s", "hello");
    // '+' sign should be ignored for strings
    EXPECT_EQ(result, "     hello");
}

// Test space flag is ignored for non-numeric types
TEST_F(PrintfFormatterTest, SpaceFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("% 10s", "hello");
    // space sign should be ignored for strings
    EXPECT_EQ(result, "     hello");
}

// Test alt flag is ignored for non-numeric types
TEST_F(PrintfFormatterTest, AltFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("%#10s", "hello");
    // '#' alt flag should be ignored for strings
    EXPECT_EQ(result, "     hello");
}

// Test combination of flags that should all be ignored for strings
TEST_F(PrintfFormatterTest, AllFlagsIgnoredForStrings) {
    std::string result = fmt::sprintf("%+-#010s", "test");
    // All flags should be ignored, just width and right alignment with spaces
    EXPECT_EQ(result, "      test");
}

// Test that '0' flag is ignored for pointers
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForPointers) {
    int x = 42;
    std::string result = fmt::sprintf("%010p", static_cast<void*>(&x));
    // '0' flag should be ignored for pointers, alignment should be right
    EXPECT_NE(result.find("0x"), std::string::npos);
    EXPECT_EQ(result.size(), 12); // 10 width + "0x" prefix
    EXPECT_EQ(result[0], ' '); // Should be space-padded, not zero-padded
}

// Test empty string with width
TEST_F(PrintfFormatterTest, EmptyStringWithWidth) {
    std::string result = fmt::sprintf("%5s", "");
    EXPECT_EQ(result, "     ");
}

// Test string longer than width (no truncation)
TEST_F(PrintfFormatterTest, StringLongerThanWidth) {
    std::string result = fmt::sprintf("%3s", "hello");
    EXPECT_EQ(result, "hello");
}

// Test zero width for string
TEST_F(PrintfFormatterTest, ZeroWidthForString) {
    std::string result = fmt::sprintf("%0s", "test");
    EXPECT_EQ(result, "test");
}

// Test that '0' flag with precision for strings (precision should work, '0' ignored)
TEST_F(PrintfFormatterTest, ZeroFlagWithPrecisionForString) {
    std::string result = fmt::sprintf("%010.3s", "hello");
    // Precision limits to 3 chars, '0' flag ignored, right aligned with spaces
    EXPECT_EQ(result, "       hel");
}

// Test that sign and alt flags are properly reset for subsequent numeric formatting
TEST_F(PrintfFormatterTest, FlagResetAfterStringFormatting) {
    // First format a string (should ignore flags), then format an integer
    std::string result = fmt::sprintf("%+10s %+05d", "test", 42);
    EXPECT_EQ(result, "      test +0042");
}

// Test that '0' flag is ignored for boolean (non-numeric)
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForBool) {
    std::string result = fmt::sprintf("%05s", true ? "true" : "false");
    EXPECT_EQ(result, " true");
}

// Test that '0' flag is ignored for custom string-like types
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForStringView) {
    std::string_view sv = "world";
    std::string result = fmt::sprintf("%08s", sv);
    EXPECT_EQ(result, "   world");
}

// Test that alignment is properly set to right for non-numeric types with numeric alignment
TEST_F(PrintfFormatterTest, NumericAlignmentToRightForString) {
    // Using '=' alignment specifier (numeric) should become right for strings
    std::string result = fmt::sprintf("%=10s", "hi");
    EXPECT_EQ(result, "        hi");
}

// Test that all flags are cleared for non-numeric types in the same format string
TEST_F(PrintfFormatterTest, MultipleFormatsWithMixedTypes) {
    std::string result = fmt::sprintf("%+010s %+05d %+010s", "abc", 123, "def");
    // First string: flags ignored, right aligned with spaces
    // Integer: flags work normally
    // Second string: flags ignored again
    EXPECT_EQ(result, "       abc +0123       def");
}
```

---

## id 24 - `fmtlib___fmt@96c18b26c2` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@96c18b26c2/deepseek-chat/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <string>
#include <vector>
#include <cmath>

// Test the printf formatter's behavior with various format specifications
// The focal function handles the case where a non-numeric type is formatted
// with printf-style format flags that are normally ignored for non-numeric types

class PrintfFormatterTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

// Test basic string formatting with printf
TEST_F(PrintfFormatterTest, BasicStringFormatting) {
    std::string result = fmt::sprintf("%s", "hello");
    EXPECT_EQ(result, "hello");
}

// Test that '0' flag is ignored for strings (non-numeric type)
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("%010s", "hello");
    EXPECT_EQ(result, "     hello");  // Should be right-aligned with spaces, not zero-padded
}

// Test that '0' flag is ignored for characters (non-numeric type)
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForChars) {
    std::string result = fmt::sprintf("%05c", 'A');
    EXPECT_EQ(result, "    A");  // Should be right-aligned with spaces
}

// Test that '0' flag is ignored for pointers (non-numeric type)
TEST_F(PrintfFormatterTest, ZeroFlagIgnoredForPointers) {
    int x = 42;
    std::string result = fmt::sprintf("%020p", static_cast<void*>(&x));
    // Should be right-aligned with spaces, not zero-padded
    EXPECT_NE(result.find("0x"), std::string::npos);
    EXPECT_TRUE(result.size() > 10);  // Should have leading spaces
}

// Test that '+' flag is ignored for strings
TEST_F(PrintfFormatterTest, PlusFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("%+10s", "hello");
    EXPECT_EQ(result, "     hello");  // No plus sign for strings
}

// Test that space flag is ignored for strings
TEST_F(PrintfFormatterTest, SpaceFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("% 10s", "hello");
    EXPECT_EQ(result, "     hello");  // No leading space for strings
}

// Test that '#' flag is ignored for strings
TEST_F(PrintfFormatterTest, AltFlagIgnoredForStrings) {
    std::string result = fmt::sprintf("%#10s", "hello");
    EXPECT_EQ(result, "     hello");  // No alternate form for strings
}

// Test that '-' flag still works for strings (left alignment)
TEST_F(PrintfFormatterTest, MinusFlagWorksForStrings) {
    std::string result = fmt::sprintf("%-10s", "hello");
    EXPECT_EQ(result, "hello     ");  // Left-aligned
}

// Test that '-' overrides '0' for strings
TEST_F(PrintfFormatterTest, MinusOverridesZeroForStrings) {
    std::string result = fmt::sprintf("%-010s", "hello");
    EXPECT_EQ(result, "hello     ");  // Left-aligned, '0' ignored
}

// Test width specification for strings
TEST_F(PrintfFormatterTest, WidthSpecificationForStrings) {
    std::string result = fmt::sprintf("%10s", "hello");
    EXPECT_EQ(result, "     hello");  // Right-aligned with spaces
}

// Test precision specification for strings
TEST_F(PrintfFormatterTest, PrecisionForStrings) {
    std::string result = fmt::sprintf("%.3s", "hello");
    EXPECT_EQ(result, "hel");  // Truncated to 3 characters
}

// Test width and precision together for strings
TEST_F(PrintfFormatterTest, WidthAndPrecisionForStrings) {
    std::string result = fmt::sprintf("%10.3s", "hello");
    EXPECT_EQ(result, "       hel");  // Right-aligned, truncated to 3 chars
}

// Test that '0' flag works for integers (numeric type)
TEST_F(PrintfFormatterTest, ZeroFlagWorksForIntegers) {
    std::string result = fmt::sprintf("%010d", 42);
    EXPECT_EQ(result, "0000000042");  // Zero-padded for integers
}

// Test that '0' flag works for floats (numeric type)
TEST_F(PrintfFormatterTest, ZeroFlagWorksForFloats) {
    std::string result = fmt::sprintf("%010.2f", 3.14);
    EXPECT_EQ(result, "0000003.14");  // Zero-padded for floats
}

// Test empty string formatting
TEST_F(PrintfFormatterTest, EmptyStringFormatting) {
    std::string result = fmt::sprintf("%s", "");
    EXPECT_EQ(result, "");
}

// Test empty string with width
TEST_F(PrintfFormatterTest, EmptyStringWithWidth) {
    std::string result = fmt::sprintf("%5s", "");
    EXPECT_EQ(result, "     ");  // 5 spaces
}

// Test string with special characters
TEST_F(PrintfFormatterTest, StringWithSpecialCharacters) {
    std::string result = fmt::sprintf("%s", "hello\nworld");
    EXPECT_EQ(result, "hello\nworld");
}

// Test multiple format specifiers
TEST_F(PrintfFormatterTest, MultipleFormatSpecifiers) {
    std::string result = fmt::sprintf("%s %d %s", "hello", 42, "world");
    EXPECT_EQ(result, "hello 42 world");
}

// Test that align::numeric is properly overridden for strings
TEST_F(PrintfFormatterTest, NumericAlignOverriddenForStrings) {
    // The '=' alignment is numeric-specific and should be treated as right for strings
    std::string result = fmt::sprintf("%=10s", "hello");
    EXPECT_EQ(result, "     hello");  // Should be right-aligned
}

// Test very long string formatting
TEST_F(PrintfFormatterTest, LongStringFormatting) {
    std::string long_str(1000, 'a');
    std::string result = fmt::sprintf("%s", long_str);
    EXPECT_EQ(result, long_str);
    EXPECT_EQ(result.size(), 1000);
}

// Test string with null character in middle (should work as std::string)
TEST_F(PrintfFormatterTest, StringWithNullChar) {
    std::string with_null = std::string("hello") + '\0' + "world";
    std::string result = fmt::sprintf("%s", with_null);
    EXPECT_EQ(result.size(), 11);  // Should include the null character
    EXPECT_EQ(result[5], '\0');
}

// Test that sign flag is properly reset for non-numeric types
TEST_F(PrintfFormatterTest, SignResetForNonNumeric) {
    // After formatting a string, the sign should not affect subsequent numeric formatting
    std::string result = fmt::sprintf("%+d %s %+d", 5, "test", -3);
    EXPECT_EQ(result, "+5 test -3");
}

// Test that alt flag is properly reset for non-numeric types
TEST_F(PrintfFormatterTest, AltResetForNonNumeric) {
    // After formatting a string, the alt flag should not affect subsequent numeric formatting
    std::string result = fmt::sprintf("%#x %s %#x", 255, "test", 15);
    EXPECT_EQ(result, "0xff test 0xf");
}

// Test formatting with std::string argument
TEST_F(PrintfFormatterTest, StdStringArgument) {
    std::string arg = "hello";
    std::string result = fmt::sprintf("%s", arg);
    EXPECT_EQ(result, "hello");
}

// Test formatting with const char* argument
TEST_F(PrintfFormatterTest, ConstCharPointerArgument) {
    const char* arg = "hello";
    std::string result = fmt::sprintf("%s", arg);
    EXPECT_EQ(result, "hello");
}

// Test that width with '0' flag for string doesn't zero-pad
TEST_F(PrintfFormatterTest, ZeroFlagWithWidthForString) {
    std::string result = fmt::sprintf("%010s", "hi");
    EXPECT_EQ(result, "        hi");  // Spaces, not zeros
    EXPECT_NE(result, "00000000hi");  // Explicitly not zero-padded
}

// Test that multiple flags are properly ignored for strings
TEST_F(PrintfFormatterTest, MultipleFlagsIgnoredForStrings) {
    std::string result = fmt::sprintf("%+ #010s", "test");
    EXPECT_EQ(result, "      test");  // All flags ignored, just width and right-align
}
```

---

## id 25 - `fmtlib___fmt@96c18b26c2` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@96c18b26c2/deepseek-chat/buggy/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <string>
#include <climits>

// Test the printf formatter's handling of integer formatting with various specifiers
class PrintfIntegerFormattingTest : public ::testing::Test {
protected:
    // Helper to format using printf-style syntax
    std::string format(const std::string& fmt, int value) {
        return fmt::sprintf(fmt, value);
    }
};

// Test basic integer formatting with default specifiers
TEST_F(PrintfIntegerFormattingTest, BasicIntegerFormatting) {
    EXPECT_EQ(format("%d", 42), "42");
    EXPECT_EQ(format("%i", 42), "42");
    EXPECT_EQ(format("%u", 42), "42");
}

// Test integer formatting with width specifier
TEST_F(PrintfIntegerFormattingTest, WidthSpecifier) {
    EXPECT_EQ(format("%5d", 42), "   42");
    EXPECT_EQ(format("%-5d", 42), "42   ");
    EXPECT_EQ(format("%05d", 42), "00042");
}

// Test integer formatting with precision specifier
TEST_F(PrintfIntegerFormattingTest, PrecisionSpecifier) {
    EXPECT_EQ(format("%.5d", 42), "00042");
    EXPECT_EQ(format("%.0d", 0), "");
    EXPECT_EQ(format("%.5d", -42), "-00042");
}

// Test integer formatting with sign specifiers
TEST_F(PrintfIntegerFormattingTest, SignSpecifiers) {
    EXPECT_EQ(format("%+d", 42), "+42");
    EXPECT_EQ(format("%+d", -42), "-42");
    EXPECT_EQ(format("% d", 42), " 42");
    EXPECT_EQ(format("% d", -42), "-42");
}

// Test integer formatting with alternate form (#)
TEST_F(PrintfIntegerFormattingTest, AlternateForm) {
    EXPECT_EQ(format("%#x", 255), "0xff");
    EXPECT_EQ(format("%#X", 255), "0XFF");
    EXPECT_EQ(format("%#o", 255), "0377");
    EXPECT_EQ(format("%#x", 0), "0");
}

// Test integer formatting with different bases
TEST_F(PrintfIntegerFormattingTest, DifferentBases) {
    EXPECT_EQ(format("%x", 255), "ff");
    EXPECT_EQ(format("%X", 255), "FF");
    EXPECT_EQ(format("%o", 255), "377");
    EXPECT_EQ(format("%b", 255), "11111111");
}

// Test edge cases: zero
TEST_F(PrintfIntegerFormattingTest, ZeroValue) {
    EXPECT_EQ(format("%d", 0), "0");
    EXPECT_EQ(format("%+d", 0), "+0");
    EXPECT_EQ(format("% d", 0), " 0");
    EXPECT_EQ(format("%05d", 0), "00000");
    EXPECT_EQ(format("%.0d", 0), "");
}

// Test edge cases: maximum and minimum integer values
TEST_F(PrintfIntegerFormattingTest, ExtremeValues) {
    EXPECT_EQ(format("%d", INT_MAX), "2147483647");
    EXPECT_EQ(format("%d", INT_MIN), "-2147483648");
    EXPECT_EQ(format("%u", UINT_MAX), "4294967295");
}

// Test edge cases: negative numbers
TEST_F(PrintfIntegerFormattingTest, NegativeNumbers) {
    EXPECT_EQ(format("%d", -1), "-1");
    EXPECT_EQ(format("%+d", -1), "-1");
    EXPECT_EQ(format("% d", -1), "-1");
    EXPECT_EQ(format("%05d", -1), "-0001");
    EXPECT_EQ(format("%-5d", -1), "-1   ");
}

// Test combination of multiple specifiers
TEST_F(PrintfIntegerFormattingTest, CombinedSpecifiers) {
    EXPECT_EQ(format("%+05d", 42), "+0042");
    EXPECT_EQ(format("%-+5d", 42), "+42  ");
    EXPECT_EQ(format("%#010x", 255), "0x000000ff");
    EXPECT_EQ(format("% 05d", -42), "-0042");
}

// Test that alt flag is properly reset for non-octal/hex formats
TEST_F(PrintfIntegerFormattingTest, AltFlagReset) {
    EXPECT_EQ(format("%#d", 42), "42");  // # has no effect on %d
    EXPECT_EQ(format("%#u", 42), "42");  // # has no effect on %u
}

// Test that sign is properly reset for unsigned formats
TEST_F(PrintfIntegerFormattingTest, SignResetForUnsigned) {
    EXPECT_EQ(format("%+u", 42), "42");  // + has no effect on %u
    EXPECT_EQ(format("% u", 42), "42");  // space has no effect on %u
}

// Test alignment behavior
TEST_F(PrintfIntegerFormattingTest, Alignment) {
    EXPECT_EQ(format("%-10d", 42), "42        ");
    EXPECT_EQ(format("%10d", 42), "        42");
    EXPECT_EQ(format("%-+10d", 42), "+42       ");
    EXPECT_EQ(format("%+10d", 42), "       +42");
}

// Test with long long integers (if supported)
TEST_F(PrintfIntegerFormattingTest, LongLongIntegers) {
    EXPECT_EQ(fmt::sprintf("%lld", 1234567890123LL), "1234567890123");
    EXPECT_EQ(fmt::sprintf("%llu", 1234567890123ULL), "1234567890123");
}

// Test that the function handles the conversion from char to int properly
TEST_F(PrintfIntegerFormattingTest, CharToIntConversion) {
    EXPECT_EQ(fmt::sprintf("%d", 'A'), "65");
    EXPECT_EQ(fmt::sprintf("%c", 65), "A");
}

// Test error handling: invalid format string should not crash
TEST_F(PrintfIntegerFormattingTest, InvalidFormatString) {
    EXPECT_THROW(fmt::sprintf("%", 42), fmt::format_error);
    EXPECT_THROW(fmt::sprintf("%*d", 42), fmt::format_error);
}
```

---

## id 26 - `fmtlib___fmt@96c18b26c2` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@96c18b26c2/deepseek-chat/buggy/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/printf.h>
#include <string>
#include <climits>

// Test the printf formatter's handling of integer formatting with various specifiers
// This tests the focal function's behavior through the public printf API

class PrintfIntegerTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Test basic integer formatting
TEST_F(PrintfIntegerTest, BasicInteger) {
    EXPECT_EQ(fmt::sprintf("%d", 42), "42");
    EXPECT_EQ(fmt::sprintf("%i", 42), "42");
    EXPECT_EQ(fmt::sprintf("%u", 42), "42");
}

// Test zero value
TEST_F(PrintfIntegerTest, ZeroValue) {
    EXPECT_EQ(fmt::sprintf("%d", 0), "0");
    EXPECT_EQ(fmt::sprintf("%u", 0), "0");
    EXPECT_EQ(fmt::sprintf("%x", 0), "0");
    EXPECT_EQ(fmt::sprintf("%X", 0), "0");
}

// Test negative numbers
TEST_F(PrintfIntegerTest, NegativeNumbers) {
    EXPECT_EQ(fmt::sprintf("%d", -42), "-42");
    EXPECT_EQ(fmt::sprintf("%i", -42), "-42");
    EXPECT_EQ(fmt::sprintf("%d", INT_MIN), "-2147483648");
}

// Test unsigned formatting of negative values (should wrap)
TEST_F(PrintfIntegerTest, NegativeAsUnsigned) {
    EXPECT_EQ(fmt::sprintf("%u", -1), "4294967295");
    EXPECT_EQ(fmt::sprintf("%u", -42), "4294967254");
}

// Test hexadecimal formatting
TEST_F(PrintfIntegerTest, Hexadecimal) {
    EXPECT_EQ(fmt::sprintf("%x", 255), "ff");
    EXPECT_EQ(fmt::sprintf("%X", 255), "FF");
    EXPECT_EQ(fmt::sprintf("%x", 0xDEAD), "dead");
    EXPECT_EQ(fmt::sprintf("%X", 0xDEAD), "DEAD");
}

// Test octal formatting
TEST_F(PrintfIntegerTest, Octal) {
    EXPECT_EQ(fmt::sprintf("%o", 8), "10");
    EXPECT_EQ(fmt::sprintf("%o", 64), "100");
    EXPECT_EQ(fmt::sprintf("%o", 0), "0");
}

// Test width specifier
TEST_F(PrintfIntegerTest, WidthSpecifier) {
    EXPECT_EQ(fmt::sprintf("%5d", 42), "   42");
    EXPECT_EQ(fmt::sprintf("%5d", -42), "  -42");
    EXPECT_EQ(fmt::sprintf("%5u", 42), "   42");
    EXPECT_EQ(fmt::sprintf("%5x", 255), "   ff");
}

// Test precision specifier
TEST_F(PrintfIntegerTest, PrecisionSpecifier) {
    EXPECT_EQ(fmt::sprintf("%.5d", 42), "00042");
    EXPECT_EQ(fmt::sprintf("%.5d", -42), "-00042");
    EXPECT_EQ(fmt::sprintf("%.5u", 42), "00042");
    EXPECT_EQ(fmt::sprintf("%.5x", 255), "000ff");
}

// Test width and precision together
TEST_F(PrintfIntegerTest, WidthAndPrecision) {
    EXPECT_EQ(fmt::sprintf("%8.5d", 42), "   00042");
    EXPECT_EQ(fmt::sprintf("%8.5d", -42), "  -00042");
    EXPECT_EQ(fmt::sprintf("%-8.5d", 42), "00042   ");
}

// Test left alignment
TEST_F(PrintfIntegerTest, LeftAlignment) {
    EXPECT_EQ(fmt::sprintf("%-5d", 42), "42   ");
    EXPECT_EQ(fmt::sprintf("%-5d", -42), "-42  ");
    EXPECT_EQ(fmt::sprintf("%-10x", 255), "ff        ");
}

// Test zero padding
TEST_F(PrintfIntegerTest, ZeroPadding) {
    EXPECT_EQ(fmt::sprintf("%05d", 42), "00042");
    EXPECT_EQ(fmt::sprintf("%05d", -42), "-0042");
    EXPECT_EQ(fmt::sprintf("%05u", 42), "00042");
    EXPECT_EQ(fmt::sprintf("%05x", 255), "000ff");
}

// Test sign specifiers
TEST_F(PrintfIntegerTest, SignSpecifiers) {
    EXPECT_EQ(fmt::sprintf("%+d", 42), "+42");
    EXPECT_EQ(fmt::sprintf("%+d", -42), "-42");
    EXPECT_EQ(fmt::sprintf("% d", 42), " 42");
    EXPECT_EQ(fmt::sprintf("% d", -42), "-42");
}

// Test alternate form (#)
TEST_F(PrintfIntegerTest, AlternateForm) {
    EXPECT_EQ(fmt::sprintf("%#x", 255), "0xff");
    EXPECT_EQ(fmt::sprintf("%#X", 255), "0XFF");
    EXPECT_EQ(fmt::sprintf("%#o", 8), "010");
    EXPECT_EQ(fmt::sprintf("%#o", 0), "0");
}

// Test combination of flags
TEST_F(PrintfIntegerTest, CombinedFlags) {
    EXPECT_EQ(fmt::sprintf("%+05d", 42), "+0042");
    EXPECT_EQ(fmt::sprintf("%-+5d", 42), "+42  ");
    EXPECT_EQ(fmt::sprintf("%#08x", 255), "0x0000ff");
    EXPECT_EQ(fmt::sprintf("%#010X", 255), "0X000000FF");
}

// Test large numbers
TEST_F(PrintfIntegerTest, LargeNumbers) {
    EXPECT_EQ(fmt::sprintf("%d", INT_MAX), "2147483647");
    EXPECT_EQ(fmt::sprintf("%u", UINT_MAX), "4294967295");
    EXPECT_EQ(fmt::sprintf("%x", UINT_MAX), "ffffffff");
}

// Test with variable arguments
TEST_F(PrintfIntegerTest, VariableArguments) {
    EXPECT_EQ(fmt::sprintf("%d %d %d", 1, 2, 3), "1 2 3");
    EXPECT_EQ(fmt::sprintf("%d %x %o", 42, 255, 8), "42 ff 10");
}

// Test with string and integer mix
TEST_F(PrintfIntegerTest, MixedFormatting) {
    EXPECT_EQ(fmt::sprintf("Value: %d", 42), "Value: 42");
    EXPECT_EQ(fmt::sprintf("Hex: %#x, Dec: %d", 255, 42), "Hex: 0xff, Dec: 42");
}

// Test edge case: minimum width larger than number
TEST_F(PrintfIntegerTest, LargeWidth) {
    EXPECT_EQ(fmt::sprintf("%20d", 42), "                  42");
    EXPECT_EQ(fmt::sprintf("%-20d", 42), "42                  ");
}

// Test edge case: precision larger than number
TEST_F(PrintfIntegerTest, LargePrecision) {
    EXPECT_EQ(fmt::sprintf("%.20d", 42), "00000000000000000042");
    EXPECT_EQ(fmt::sprintf("%.20d", -42), "-00000000000000000042");
}

// Test edge case: zero width and precision
TEST_F(PrintfIntegerTest, ZeroWidthAndPrecision) {
    EXPECT_EQ(fmt::sprintf("%0d", 42), "42");
    EXPECT_EQ(fmt::sprintf("%.0d", 0), "");
    EXPECT_EQ(fmt::sprintf("%.0d", 42), "42");
}

// Test that sign::none is properly applied when not specified
TEST_F(PrintfIntegerTest, DefaultSignNone) {
    EXPECT_EQ(fmt::sprintf("%d", 42), "42");
    EXPECT_EQ(fmt::sprintf("%d", -42), "-42");
    // Positive numbers should not have a sign by default
    EXPECT_NE(fmt::sprintf("%d", 42), "+42");
}

// Test that alt=false is default
TEST_F(PrintfIntegerTest, DefaultAltFalse) {
    EXPECT_EQ(fmt::sprintf("%x", 255), "ff");
    EXPECT_NE(fmt::sprintf("%x", 255), "0xff");
}

// Test that align::right is default
TEST_F(PrintfIntegerTest, DefaultAlignRight) {
    EXPECT_EQ(fmt::sprintf("%5d", 42), "   42");
    EXPECT_NE(fmt::sprintf("%5d", 42), "42   ");
}
```

---

## id 27 - `fmtlib___fmt@96c18b26c2` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@96c18b26c2/deepseek-chat/buggy/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/printf.h>

// Test that printf-style formatting with signed integer conversion works correctly
TEST(PrintfTest, SignedIntegerConversion) {
    // Normal positive value
    EXPECT_EQ(fmt::sprintf("%d", 42), "42");
    // Normal negative value
    EXPECT_EQ(fmt::sprintf("%d", -42), "-42");
    // Zero
    EXPECT_EQ(fmt::sprintf("%d", 0), "0");
    // Maximum int
    EXPECT_EQ(fmt::sprintf("%d", std::numeric_limits<int>::max()), "2147483647");
    // Minimum int
    EXPECT_EQ(fmt::sprintf("%d", std::numeric_limits<int>::min()), "-2147483648");
}

// Test that printf-style formatting with unsigned integer conversion works correctly
TEST(PrintfTest, UnsignedIntegerConversion) {
    // Normal positive value
    EXPECT_EQ(fmt::sprintf("%u", 42), "42");
    // Zero
    EXPECT_EQ(fmt::sprintf("%u", 0), "0");
    // Maximum unsigned int
    EXPECT_EQ(fmt::sprintf("%u", std::numeric_limits<unsigned int>::max()), "4294967295");
    // Negative value cast to unsigned
    EXPECT_EQ(fmt::sprintf("%u", -1), "4294967295");
}

// Test that printf-style formatting with hex conversion works correctly
TEST(PrintfTest, HexConversion) {
    // Normal value
    EXPECT_EQ(fmt::sprintf("%x", 255), "ff");
    // Uppercase hex
    EXPECT_EQ(fmt::sprintf("%X", 255), "FF");
    // Zero
    EXPECT_EQ(fmt::sprintf("%x", 0), "0");
    // With alternate form
    EXPECT_EQ(fmt::sprintf("%#x", 255), "0xff");
    EXPECT_EQ(fmt::sprintf("%#X", 255), "0XFF");
}

// Test that printf-style formatting with octal conversion works correctly
TEST(PrintfTest, OctalConversion) {
    // Normal value
    EXPECT_EQ(fmt::sprintf("%o", 8), "10");
    // Zero
    EXPECT_EQ(fmt::sprintf("%o", 0), "0");
    // With alternate form
    EXPECT_EQ(fmt::sprintf("%#o", 8), "010");
}

// Test that printf-style formatting with width and precision works correctly
TEST(PrintfTest, WidthAndPrecision) {
    // Width with padding
    EXPECT_EQ(fmt::sprintf("%5d", 42), "   42");
    // Width with left alignment
    EXPECT_EQ(fmt::sprintf("%-5d", 42), "42   ");
    // Width with zero padding
    EXPECT_EQ(fmt::sprintf("%05d", 42), "00042");
    // Precision for integers
    EXPECT_EQ(fmt::sprintf("%.5d", 42), "00042");
    // Width and precision combined
    EXPECT_EQ(fmt::sprintf("%8.5d", 42), "   00042");
}

// Test that printf-style formatting with sign flags works correctly
TEST(PrintfTest, SignFlags) {
    // Always show sign
    EXPECT_EQ(fmt::sprintf("%+d", 42), "+42");
    EXPECT_EQ(fmt::sprintf("%+d", -42), "-42");
    // Space for positive sign
    EXPECT_EQ(fmt::sprintf("% d", 42), " 42");
    EXPECT_EQ(fmt::sprintf("% d", -42), "-42");
}

// Test that printf-style formatting with string conversion works correctly
TEST(PrintfTest, StringConversion) {
    // Normal string
    EXPECT_EQ(fmt::sprintf("%s", "hello"), "hello");
    // Empty string
    EXPECT_EQ(fmt::sprintf("%s", ""), "");
    // String with width
    EXPECT_EQ(fmt::sprintf("%10s", "hello"), "     hello");
    // String with left alignment
    EXPECT_EQ(fmt::sprintf("%-10s", "hello"), "hello     ");
    // String with precision
    EXPECT_EQ(fmt::sprintf("%.3s", "hello"), "hel");
}

// Test that printf-style formatting with character conversion works correctly
TEST(PrintfTest, CharacterConversion) {
    // Normal character
    EXPECT_EQ(fmt::sprintf("%c", 'A'), "A");
    // Character from integer
    EXPECT_EQ(fmt::sprintf("%c", 65), "A");
}

// Test that printf-style formatting with pointer conversion works correctly
TEST(PrintfTest, PointerConversion) {
    int x = 42;
    // Pointer should be formatted as hex address
    std::string result = fmt::sprintf("%p", static_cast<const void*>(&x));
    EXPECT_TRUE(result.find("0x") == 0 || result.find("0X") == 0);
    EXPECT_GT(result.size(), 2);
}

// Test that printf-style formatting with floating-point conversion works correctly
TEST(PrintfTest, FloatingPointConversion) {
    // Normal float
    EXPECT_EQ(fmt::sprintf("%f", 3.14), "3.140000");
    // Float with precision
    EXPECT_EQ(fmt::sprintf("%.2f", 3.14159), "3.14");
    // Float with width
    EXPECT_EQ(fmt::sprintf("%8.2f", 3.14), "    3.14");
    // Scientific notation
    EXPECT_EQ(fmt::sprintf("%e", 314.0), "3.140000e+02");
    // Shortest representation
    EXPECT_EQ(fmt::sprintf("%g", 3.14), "3.14");
}

// Test that printf-style formatting with multiple arguments works correctly
TEST(PrintfTest, MultipleArguments) {
    EXPECT_EQ(fmt::sprintf("%d %s %f", 42, "hello", 3.14), "42 hello 3.140000");
}

// Test that printf-style formatting with positional arguments works correctly
TEST(PrintfTest, PositionalArguments) {
    EXPECT_EQ(fmt::sprintf("%2$d %1$d", 10, 20), "20 10");
}

// Test that printf-style formatting handles edge cases correctly
TEST(PrintfTest, EdgeCases) {
    // Very large width
    EXPECT_EQ(fmt::sprintf("%100d", 42), std::string(98, ' ') + "42");
    // Very large precision
    EXPECT_EQ(fmt::sprintf("%.100d", 42), std::string(98, '0') + "42");
    // Negative width (should be treated as left alignment)
    EXPECT_EQ(fmt::sprintf("%-5d", 42), "42   ");
    // Star width
    EXPECT_EQ(fmt::sprintf("%*d", 5, 42), "   42");
    // Star precision
    EXPECT_EQ(fmt::sprintf("%.*d", 5, 42), "00042");
}

// Test that printf-style formatting handles invalid format strings gracefully
TEST(PrintfTest, InvalidFormatStrings) {
    // Unknown conversion specifier
    EXPECT_THROW(fmt::sprintf("%q", 42), fmt::format_error);
    // Incomplete format specifier
    EXPECT_THROW(fmt::sprintf("%", 42), fmt::format_error);
}
```

---

## id 28 - `CLIUtils___CLI11@020a21afc6` - model gpt-5.4-or

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/CLIUtils___CLI11@020a21afc6/gpt-5.4-or/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>

#include <string>
#include <utility>

#include "CLI/Error.hpp"
#include "CLI/Validators.hpp"

namespace {

using CLI::detail::split_program_name;

TEST(SplitProgramNameTest, EmptyCommandLineReturnsEmptyParts) {
    auto vals = split_program_name("");
    EXPECT_TRUE(vals.first.empty());
    EXPECT_TRUE(vals.second.empty());
}

TEST(SplitProgramNameTest, OnlyProgramNameNoArguments) {
    auto vals = split_program_name("myprog");
    EXPECT_EQ(vals.first, "myprog");
    EXPECT_TRUE(vals.second.empty());
}

TEST(SplitProgramNameTest, ProgramNameAndSingleArgument) {
    auto vals = split_program_name("myprog --help");
    EXPECT_EQ(vals.first, "myprog");
    EXPECT_EQ(vals.second, "--help");
}

TEST(SplitProgramNameTest, ProgramNameAndMultipleArguments) {
    auto vals = split_program_name("myprog arg1 arg2 --flag=value");
    EXPECT_EQ(vals.first, "myprog");
    EXPECT_EQ(vals.second, "arg1 arg2 --flag=value");
}

TEST(SplitProgramNameTest, LeadingSpacesBeforeProgramNameAreIgnored) {
    auto vals = split_program_name("   myprog arg1 arg2");
    EXPECT_EQ(vals.first, "myprog");
    EXPECT_EQ(vals.second, "arg1 arg2");
}

TEST(SplitProgramNameTest, TrailingSpacesAfterProgramNameProduceEmptyRemainder) {
    auto vals = split_program_name("myprog   ");
    EXPECT_EQ(vals.first, "myprog");
    EXPECT_TRUE(vals.second.empty());
}

TEST(SplitProgramNameTest, MultipleSpacesBeforeArgumentsAreTrimmedFromRemainder) {
    auto vals = split_program_name("myprog      arg1   arg2");
    EXPECT_EQ(vals.first, "myprog");
    EXPECT_EQ(vals.second, "arg1   arg2");
    ASSERT_FALSE(vals.second.empty());
    EXPECT_NE(vals.second.front(), ' ');
}

TEST(SplitProgramNameTest, OnlyWhitespaceReturnsEmptyParts) {
    auto vals = split_program_name("     ");
    EXPECT_TRUE(vals.first.empty());
    EXPECT_TRUE(vals.second.empty());
}

TEST(SplitProgramNameTest, QuotedProgramNameWithArguments) {
    auto vals = split_program_name("\"/path to/my prog\" --flag value");
    EXPECT_EQ(vals.first, "\"/path to/my prog\"");
    EXPECT_EQ(vals.second, "--flag value");
}

TEST(SplitProgramNameTest, QuotedProgramNameWithoutArguments) {
    auto vals = split_program_name("\"/path to/my prog\"");
    EXPECT_EQ(vals.first, "\"/path to/my prog\"");
    EXPECT_TRUE(vals.second.empty());
}

TEST(SplitProgramNameTest, QuotedProgramNameWithExtraSpacesBeforeArgumentsTrimsRemainder) {
    auto vals = split_program_name("\"prog name\"      --opt 123");
    EXPECT_EQ(vals.first, "\"prog name\"");
    EXPECT_EQ(vals.second, "--opt 123");
    ASSERT_FALSE(vals.second.empty());
    EXPECT_NE(vals.second.front(), ' ');
}

TEST(SplitProgramNameTest, UnterminatedQuoteTreatsRestAsProgramName) {
    auto vals = split_program_name("\"prog name --flag value");
    EXPECT_EQ(vals.first, "\"prog name --flag value");
    EXPECT_TRUE(vals.second.empty());
}

TEST(SplitProgramNameTest, SingleQuoteProgramNameWithArguments) {
    auto vals = split_program_name("'prog name' arg1");
    EXPECT_EQ(vals.first, "'prog name'");
    EXPECT_EQ(vals.second, "arg1");
}

TEST(SplitProgramNameTest, TabBetweenProgramAndArgumentsDoesNotSplitOnTab) {
    auto vals = split_program_name("myprog\targ1 arg2");
    EXPECT_EQ(vals.first, "myprog\targ1");
    EXPECT_EQ(vals.second, "arg2");
}

TEST(SplitProgramNameTest, SpaceThenTabsBeforeArgumentRemainAfterLeftTrimOfSpacesOnly) {
    auto vals = split_program_name("myprog \t\targ1");
    EXPECT_EQ(vals.first, "myprog");
    EXPECT_EQ(vals.second, "\t\targ1");
    ASSERT_FALSE(vals.second.empty());
    EXPECT_EQ(vals.second.front(), '\t');
}

TEST(SplitProgramNameTest, BoundarySingleCharacterProgram) {
    auto vals = split_program_name("a b");
    EXPECT_EQ(vals.first, "a");
    EXPECT_EQ(vals.second, "b");
}

TEST(SplitProgramNameTest, BoundarySingleCharacterProgramNoArguments) {
    auto vals = split_program_name("a");
    EXPECT_EQ(vals.first, "a");
    EXPECT_TRUE(vals.second.empty());
}

}  // namespace
```

---

## id 29 - `CLIUtils___CLI11@020a21afc6` - model qwen3-coder

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/CLIUtils___CLI11@020a21afc6/qwen3-coder/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include "CLI/App.hpp"
#include "CLI/CLI.hpp"

TEST(ValidatorsTest, StripProgramNameNormalCase) {
    // Test normal case where program name is present and followed by arguments
    std::string commandline = "/path/to/program arg1 arg2 arg3";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1 arg2 arg3");
}

TEST(ValidatorsTest, StripProgramNameNoArguments) {
    // Test case where program name is present but no arguments follow
    std::string commandline = "/path/to/program";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "");
}

TEST(ValidatorsTest, StripProgramNameEmptyString) {
    // Test empty command line
    std::string commandline = "";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "");
    EXPECT_EQ(result.second, "");
}

TEST(ValidatorsTest, StripProgramNameSingleChar) {
    // Test single character command line
    std::string commandline = "a";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "a");
    EXPECT_EQ(result.second, "");
}

TEST(ValidatorsTest, StripProgramNameMultipleSpaces) {
    // Test program name with multiple spaces before arguments
    std::string commandline = "/path/to/program    arg1 arg2";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1 arg2");
}

TEST(ValidatorsTest, StripProgramNameLeadingSpaces) {
    // Test command line with leading spaces
    std::string commandline = "   /path/to/program arg1";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1");
}

TEST(ValidatorsTest, StripProgramNameTrailingSpaces) {
    // Test command line with trailing spaces after program name
    std::string commandline = "/path/to/program   arg1";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "arg1");
}

TEST(ValidatorsTest, StripProgramNameOnlySpaces) {
    // Test command line with only spaces
    std::string commandline = "   ";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "");
    EXPECT_EQ(result.second, "");
}

TEST(ValidatorsTest, StripProgramNameNoPath) {
    // Test program name without path
    std::string commandline = "program arg1 arg2";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "program");
    EXPECT_EQ(result.second, "arg1 arg2");
}

TEST(ValidatorsTest, StripProgramNameRelativePath) {
    // Test relative path program name
    std::string commandline = "./program arg1";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "./program");
    EXPECT_EQ(result.second, "arg1");
}

TEST(ValidatorsTest, StripProgramNameWithSpecialChars) {
    // Test program name with special characters
    std::string commandline = "/path/to/my-program_v1.0 arg1";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/my-program_v1.0");
    EXPECT_EQ(result.second, "arg1");
}

TEST(ValidatorsTest, StripProgramNameWithQuotes) {
    // Test program name with quotes in arguments
    std::string commandline = "/path/to/program \"arg with spaces\" arg2";
    auto result = CLI::detail::split_program_name(commandline);
    
    EXPECT_EQ(result.first, "/path/to/program");
    EXPECT_EQ(result.second, "\"arg with spaces\" arg2");
}
```

---

## id 30 - `fmtlib___fmt@611cf0b3c6` - model qwen3-coder

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@611cf0b3c6/qwen3-coder/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>

// Test fixture to provide common setup for format tests
class FormatOctalTest : public ::testing::Test {
protected:
    std::string format_with_specs(const char* format_str, int value, bool alt = false, int precision = -1) {
        fmt::format_specs specs;
        specs.alt = alt;
        if (precision >= 0) {
            specs.precision = precision;
        }
        specs.type = 'o';
        
        // We'll test the internal behavior by examining the formatted output
        // since the on_oct() method is internal to the formatter
        if (alt && precision == -1) {
            return fmt::format(format_str, value);
        } else if (alt && precision >= 0) {
            return fmt::format(format_str, value);
        } else {
            return fmt::format(format_str, value);
        }
    }
};

// Test normal octal formatting without alt flag
TEST_F(FormatOctalTest, NormalOctalFormatting) {
    // Test basic octal formatting
    std::string result = fmt::format("{:o}", 10);
    EXPECT_EQ(result, "12");
    
    result = fmt::format("{:o}", 64);
    EXPECT_EQ(result, "100");
    
    result = fmt::format("{:o}", 255);
    EXPECT_EQ(result, "377");
}

// Test octal formatting with alt flag (should add leading 0)
TEST_F(FormatOctalTest, OctalWithAltFlag) {
    // Test with alt flag and value != 0
    std::string result = fmt::format("{:#o}", 10);
    EXPECT_EQ(result, "012");
    
    result = fmt::format("{:#o}", 64);
    EXPECT_EQ(result, "0100");
    
    result = fmt::format("{:#o}", 255);
    EXPECT_EQ(result, "0377");
}

// Test octal formatting with alt flag and precision
TEST_F(FormatOctalTest, OctalWithAltFlagAndPrecision) {
    // Test with alt flag and precision greater than number of digits
    std::string result = fmt::format("{:#.5o}", 10);
    EXPECT_EQ(result, "00012");
    
    // Test with alt flag and precision equal to number of digits
    result = fmt::format("{:#.3o}", 10);
    EXPECT_EQ(result, "012");
    
    // Test with alt flag and precision less than number of digits
    result = fmt::format("{:#.2o}", 10);
    EXPECT_EQ(result, "012");  // Should not add leading 0 when precision <= num_digits
}

// Test edge cases: zero value
TEST_F(FormatOctalTest, OctalZeroValue) {
    std::string result = fmt::format("{:o}", 0);
    EXPECT_EQ(result, "0");
    
    result = fmt::format("{:#o}", 0);
    EXPECT_EQ(result, "0");  // Should not add leading 0 for zero
    
    result = fmt::format("{:#.5o}", 0);
    EXPECT_EQ(result, "00000");
}

// Test edge cases: negative values
TEST_F(FormatOctalTest, OctalNegativeValues) {
    std::string result = fmt::format("{:o}", -10);
    EXPECT_EQ(result, "-12");
    
    result = fmt::format("{:#o}", -10);
    EXPECT_EQ(result, "-012");
}

// Test octal formatting with various precision values
TEST_F(FormatOctalTest, OctalPrecisionVariations) {
    // Test precision without alt flag
    std::string result = fmt::format("{:.5o}", 10);
    EXPECT_EQ(result, "00012");
    
    result = fmt::format("{:.2o}", 10);
    EXPECT_EQ(result, "12");
    
    result = fmt::format("{:.1o}", 10);
    EXPECT_EQ(result, "12");
    
    result = fmt::format("{:.0o}", 10);
    EXPECT_EQ(result, "12");
}

// Test that precision is handled correctly with alt flag
TEST_F(FormatOctalTest, OctalPrecisionWithAltFlag) {
    // When precision is greater than number of digits, leading zeros should be added
    std::string result = fmt::format("{:#.5o}", 7);
    EXPECT_EQ(result, "00007");
    
    // When precision equals number of digits, no extra zeros should be added
    result = fmt::format("{:#.3o}", 7);
    EXPECT_EQ(result, "007");
    
    // When precision is less than number of digits, no extra zeros should be added
    result = fmt::format("{:#.2o}", 7);
    EXPECT_EQ(result, "07");
}

// Test large values
TEST_F(FormatOctalTest, OctalLargeValues) {
    std::string result = fmt::format("{:o}", 1000);
    EXPECT_EQ(result, "1750");
    
    result = fmt::format("{:#o}", 1000);
    EXPECT_EQ(result, "01750");
    
    result = fmt::format("{:#.10o}", 1000);
    EXPECT_EQ(result, "0000001750");
}

// Test boundary values
TEST_F(FormatOctalTest, OctalBoundaryValues) {
    // Test maximum values that fit in standard types
    std::string result = fmt::format("{:o}", 2147483647);  // 2^31 - 1
    EXPECT_EQ(result, "17777777777");
    
    result = fmt::format("{:#o}", 2147483647);
    EXPECT_EQ(result, "017777777777");
    
    // Test minimum negative value
    result = fmt::format("{:o}", -2147483648);  // -2^31
    EXPECT_EQ(result, "-20000000000");
}
```

---

## id 31 - `fmtlib___fmt@279d698e1b` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@279d698e1b/deepseek-chat/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Test formatting integers with various widths, alignments, and fill characters
TEST(FormatIntegerTest, BasicWidthAndAlignment) {
    // Right alignment (default)
    EXPECT_EQ(fmt::format("{:5}", 42), "   42");
    EXPECT_EQ(fmt::format("{:5}", -42), "  -42");
    
    // Left alignment
    EXPECT_EQ(fmt::format("{:<5}", 42), "42   ");
    EXPECT_EQ(fmt::format("{:<5}", -42), "-42  ");
    
    // Center alignment
    EXPECT_EQ(fmt::format("{:^5}", 42), " 42  ");
    EXPECT_EQ(fmt::format("{:^5}", -42), "-42  ");
    EXPECT_EQ(fmt::format("{:^6}", -42), " -42  ");
    
    // Fill character
    EXPECT_EQ(fmt::format("{:*>5}", 42), "***42");
    EXPECT_EQ(fmt::format("{:*<5}", 42), "42***");
    EXPECT_EQ(fmt::format("{:*^5}", 42), "*42**");
    EXPECT_EQ(fmt::format("{:*>5}", -42), "**-42");
    EXPECT_EQ(fmt::format("{:*<5}", -42), "-42**");
    EXPECT_EQ(fmt::format("{:*^5}", -42), "-42**");
}

// Test formatting integers with zero padding
TEST(FormatIntegerTest, ZeroPadding) {
    EXPECT_EQ(fmt::format("{:05}", 42), "00042");
    EXPECT_EQ(fmt::format("{:05}", -42), "-0042");
    EXPECT_EQ(fmt::format("{:05}", 0), "00000");
    EXPECT_EQ(fmt::format("{:05}", -0), "00000");
}

// Test formatting integers with sign options
TEST(FormatIntegerTest, SignOptions) {
    EXPECT_EQ(fmt::format("{:+}", 42), "+42");
    EXPECT_EQ(fmt::format("{:+}", -42), "-42");
    EXPECT_EQ(fmt::format("{: }", 42), " 42");
    EXPECT_EQ(fmt::format("{: }", -42), "-42");
    EXPECT_EQ(fmt::format("{:-}", 42), "42");
    EXPECT_EQ(fmt::format("{:-}", -42), "-42");
}

// Test formatting integers with different bases
TEST(FormatIntegerTest, BaseFormatting) {
    EXPECT_EQ(fmt::format("{:b}", 42), "101010");
    EXPECT_EQ(fmt::format("{:B}", 42), "101010");
    EXPECT_EQ(fmt::format("{:d}", 42), "42");
    EXPECT_EQ(fmt::format("{:o}", 42), "52");
    EXPECT_EQ(fmt::format("{:x}", 42), "2a");
    EXPECT_EQ(fmt::format("{:X}", 42), "2A");
    
    // With prefix
    EXPECT_EQ(fmt::format("{:#b}", 42), "0b101010");
    EXPECT_EQ(fmt::format("{:#B}", 42), "0B101010");
    EXPECT_EQ(fmt::format("{:#o}", 42), "052");
    EXPECT_EQ(fmt::format("{:#x}", 42), "0x2a");
    EXPECT_EQ(fmt::format("{:#X}", 42), "0X2A");
}

// Test formatting integers with width and precision
TEST(FormatIntegerTest, WidthAndPrecision) {
    EXPECT_EQ(fmt::format("{:6d}", 42), "    42");
    EXPECT_EQ(fmt::format("{:6.4d}", 42), "  0042");
    EXPECT_EQ(fmt::format("{:6.4d}", -42), " -0042");
    EXPECT_EQ(fmt::format("{:6.4d}", 0), "  0000");
}

// Test edge cases: zero, min/max values
TEST(FormatIntegerTest, EdgeCases) {
    EXPECT_EQ(fmt::format("{}", 0), "0");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::max()), "2147483647");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::min()), "-2147483648");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<unsigned>::max()), "4294967295");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<long long>::max()), "9223372036854775807");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<long long>::min()), "-9223372036854775808");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<unsigned long long>::max()), "18446744073709551615");
}

// Test formatting with width larger than number of digits
TEST(FormatIntegerTest, WidthLargerThanDigits) {
    EXPECT_EQ(fmt::format("{:10}", 42), "        42");
    EXPECT_EQ(fmt::format("{:10}", -42), "       -42");
    EXPECT_EQ(fmt::format("{:<10}", 42), "42        ");
    EXPECT_EQ(fmt::format("{:^10}", 42), "    42    ");
    EXPECT_EQ(fmt::format("{:*>10}", 42), "********42");
}

// Test formatting with width smaller than number of digits (no padding)
TEST(FormatIntegerTest, WidthSmallerThanDigits) {
    EXPECT_EQ(fmt::format("{:2}", 12345), "12345");
    EXPECT_EQ(fmt::format("{:2}", -12345), "-12345");
    EXPECT_EQ(fmt::format("{:<2}", 12345), "12345");
    EXPECT_EQ(fmt::format("{:^2}", 12345), "12345");
}

// Test formatting with fill character and alignment for negative numbers
TEST(FormatIntegerTest, NegativeNumbersWithFill) {
    EXPECT_EQ(fmt::format("{:*>6}", -42), "***-42");
    EXPECT_EQ(fmt::format("{:*<6}", -42), "-42***");
    EXPECT_EQ(fmt::format("{:*^6}", -42), "-42***");
    EXPECT_EQ(fmt::format("{:*>6}", -12345), "-12345");
    EXPECT_EQ(fmt::format("{:*<6}", -12345), "-12345");
    EXPECT_EQ(fmt::format("{:*^6}", -12345), "-12345");
}

// Test formatting with zero padding and alignment
TEST(FormatIntegerTest, ZeroPaddingWithAlignment) {
    EXPECT_EQ(fmt::format("{:06}", 42), "000042");
    EXPECT_EQ(fmt::format("{:<06}", 42), "420000");
    EXPECT_EQ(fmt::format("{:^06}", 42), "042000");
    EXPECT_EQ(fmt::format("{:>06}", 42), "000042");
}

// Test formatting with sign and zero padding
TEST(FormatIntegerTest, SignAndZeroPadding) {
    EXPECT_EQ(fmt::format("{:+06}", 42), "+00042");
    EXPECT_EQ(fmt::format("{:+06}", -42), "-00042");
    EXPECT_EQ(fmt::format("{: 06}", 42), " 00042");
    EXPECT_EQ(fmt::format("{: 06}", -42), "-00042");
}

// Test formatting with width, precision, and sign
TEST(FormatIntegerTest, WidthPrecisionAndSign) {
    EXPECT_EQ(fmt::format("{:+8.4d}", 42), "   +0042");
    EXPECT_EQ(fmt::format("{:+8.4d}", -42), "   -0042");
    EXPECT_EQ(fmt::format("{: 8.4d}", 42), "    0042");
    EXPECT_EQ(fmt::format("{: 8.4d}", -42), "   -0042");
}

// Test formatting with alternate form and width
TEST(FormatIntegerTest, AlternateFormWithWidth) {
    EXPECT_EQ(fmt::format("{:#10x}", 255), "     0xff");
    EXPECT_EQ(fmt::format("{:#10X}", 255), "     0XFF");
    EXPECT_EQ(fmt::format("{:#10o}", 255), "      0377");
    EXPECT_EQ(fmt::format("{:#10b}", 255), "0b11111111");
}

// Test formatting with empty format string
TEST(FormatIntegerTest, EmptyFormatString) {
    EXPECT_EQ(fmt::format("{}", 42), "42");
    EXPECT_EQ(fmt::format("{}", -42), "-42");
    EXPECT_EQ(fmt::format("{}", 0), "0");
}

// Test formatting with multiple arguments
TEST(FormatIntegerTest, MultipleArguments) {
    EXPECT_EQ(fmt::format("{} {} {}", 1, 2, 3), "1 2 3");
    EXPECT_EQ(fmt::format("{:4} {:4} {:4}", 1, -2, 3), "   1   -2    3");
    EXPECT_EQ(fmt::format("{:<4} {:^4} {:>4}", 1, -2, 3), "1    -2     3");
}

// Test formatting with char type
TEST(FormatIntegerTest, CharType) {
    EXPECT_EQ(fmt::format("{}", 'A'), "65");
    EXPECT_EQ(fmt::format("{:c}", 65), "A");
    EXPECT_EQ(fmt::format("{:c}", 0x41), "A");
}

// Test formatting with bool type
TEST(FormatIntegerTest, BoolType) {
    EXPECT_EQ(fmt::format("{}", true), "true");
    EXPECT_EQ(fmt::format("{}", false), "false");
    EXPECT_EQ(fmt::format("{:d}", true), "1");
    EXPECT_EQ(fmt::format("{:d}", false), "0");
}
```

---

## id 32 - `fmtlib___fmt@279d698e1b` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@279d698e1b/deepseek-chat/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <limits>

// Test formatting integers with various widths, alignments, and fill characters
TEST(FormatIntegerTest, WidthAndAlignment) {
    // Right alignment (default)
    EXPECT_EQ(fmt::format("{:5}", 42), "   42");
    EXPECT_EQ(fmt::format("{:5}", -42), "  -42");
    
    // Left alignment
    EXPECT_EQ(fmt::format("{:<5}", 42), "42   ");
    EXPECT_EQ(fmt::format("{:<5}", -42), "-42  ");
    
    // Center alignment
    EXPECT_EQ(fmt::format("{:^5}", 42), " 42  ");
    EXPECT_EQ(fmt::format("{:^5}", -42), "-42  ");
    EXPECT_EQ(fmt::format("{:^6}", -42), " -42  ");
    
    // Fill character
    EXPECT_EQ(fmt::format("{:*>5}", 42), "***42");
    EXPECT_EQ(fmt::format("{:*<5}", 42), "42***");
    EXPECT_EQ(fmt::format("{:*^5}", 42), "*42**");
    EXPECT_EQ(fmt::format("{:*>5}", -42), "**-42");
    EXPECT_EQ(fmt::format("{:*<5}", -42), "-42**");
    EXPECT_EQ(fmt::format("{:*^5}", -42), "-42**");
}

// Test formatting integers with zero padding
TEST(FormatIntegerTest, ZeroPadding) {
    EXPECT_EQ(fmt::format("{:05}", 42), "00042");
    EXPECT_EQ(fmt::format("{:05}", -42), "-0042");
    EXPECT_EQ(fmt::format("{:05}", 0), "00000");
    EXPECT_EQ(fmt::format("{:05}", -0), "00000");
}

// Test formatting integers with sign options
TEST(FormatIntegerTest, SignOptions) {
    EXPECT_EQ(fmt::format("{:+}", 42), "+42");
    EXPECT_EQ(fmt::format("{:+}", -42), "-42");
    EXPECT_EQ(fmt::format("{:+}", 0), "+0");
    
    EXPECT_EQ(fmt::format("{: }", 42), " 42");
    EXPECT_EQ(fmt::format("{: }", -42), "-42");
    EXPECT_EQ(fmt::format("{: }", 0), " 0");
    
    EXPECT_EQ(fmt::format("{:-}", 42), "42");
    EXPECT_EQ(fmt::format("{:-}", -42), "-42");
    EXPECT_EQ(fmt::format("{:-}", 0), "0");
}

// Test formatting integers with different bases
TEST(FormatIntegerTest, BaseFormatting) {
    // Binary
    EXPECT_EQ(fmt::format("{:b}", 42), "101010");
    EXPECT_EQ(fmt::format("{:#b}", 42), "0b101010");
    
    // Octal
    EXPECT_EQ(fmt::format("{:o}", 42), "52");
    EXPECT_EQ(fmt::format("{:#o}", 42), "052");
    
    // Hexadecimal (lowercase)
    EXPECT_EQ(fmt::format("{:x}", 42), "2a");
    EXPECT_EQ(fmt::format("{:#x}", 42), "0x2a");
    
    // Hexadecimal (uppercase)
    EXPECT_EQ(fmt::format("{:X}", 42), "2A");
    EXPECT_EQ(fmt::format("{:#X}", 42), "0X2A");
}

// Test formatting integers with width and base together
TEST(FormatIntegerTest, WidthAndBase) {
    EXPECT_EQ(fmt::format("{:10b}", 42), "    101010");
    EXPECT_EQ(fmt::format("{:#10b}", 42), "  0b101010");
    EXPECT_EQ(fmt::format("{:10x}", 255), "        ff");
    EXPECT_EQ(fmt::format("{:#10x}", 255), "    0xff");
    EXPECT_EQ(fmt::format("{:10X}", 255), "        FF");
    EXPECT_EQ(fmt::format("{:#10X}", 255), "    0XFF");
}

// Test formatting integers with zero padding and base
TEST(FormatIntegerTest, ZeroPaddingAndBase) {
    EXPECT_EQ(fmt::format("{:010b}", 42), "0000101010");
    EXPECT_EQ(fmt::format("{:#010b}", 42), "0b00101010");
    EXPECT_EQ(fmt::format("{:010x}", 255), "00000000ff");
    EXPECT_EQ(fmt::format("{:#010x}", 255), "0x000000ff");
}

// Test formatting integers with precision (for integers, precision sets minimum digits)
TEST(FormatIntegerTest, Precision) {
    EXPECT_EQ(fmt::format("{:.5}", 42), "00042");
    EXPECT_EQ(fmt::format("{:.5}", -42), "-00042");
    EXPECT_EQ(fmt::format("{:8.5}", 42), "   00042");
    EXPECT_EQ(fmt::format("{:8.5}", -42), "  -00042");
}

// Test formatting with zero and edge cases
TEST(FormatIntegerTest, ZeroAndEdgeCases) {
    EXPECT_EQ(fmt::format("{}", 0), "0");
    EXPECT_EQ(fmt::format("{:}", 0), "0");
    EXPECT_EQ(fmt::format("{:1}", 0), "0");
    EXPECT_EQ(fmt::format("{:2}", 0), " 0");
    EXPECT_EQ(fmt::format("{:02}", 0), "00");
    
    // Maximum and minimum values
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::max()), "2147483647");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<int>::min()), "-2147483648");
    EXPECT_EQ(fmt::format("{}", std::numeric_limits<unsigned>::max()), "4294967295");
    
    // Large width
    EXPECT_EQ(fmt::format("{:20}", 42), "                  42");
    EXPECT_EQ(fmt::format("{:20}", -42), "                 -42");
}

// Test formatting with fill, align, and sign combined
TEST(FormatIntegerTest, FillAlignSign) {
    EXPECT_EQ(fmt::format("{:*>+10}", 42), "*******+42");
    EXPECT_EQ(fmt::format("{:*<+10}", 42), "+42*******");
    EXPECT_EQ(fmt::format("{:*^+10}", 42), "***+42****");
    EXPECT_EQ(fmt::format("{:*>+10}", -42), "*******-42");
    EXPECT_EQ(fmt::format("{:*> 10}", 42), "******* 42");
    EXPECT_EQ(fmt::format("{:*> 10}", -42), "*******-42");
}

// Test formatting with width smaller than number of digits
TEST(FormatIntegerTest, WidthSmallerThanDigits) {
    EXPECT_EQ(fmt::format("{:2}", 12345), "12345");
    EXPECT_EQ(fmt::format("{:2}", -12345), "-12345");
    EXPECT_EQ(fmt::format("{:2}", 0), "0");
}

// Test formatting with char types
TEST(FormatIntegerTest, CharTypes) {
    EXPECT_EQ(fmt::format("{:c}", 65), "A");
    EXPECT_EQ(fmt::format("{:c}", 97), "a");
    EXPECT_EQ(fmt::format("{:c}", 48), "0");
}

// Test formatting with long long types
TEST(FormatIntegerTest, LongLongTypes) {
    EXPECT_EQ(fmt::format("{}", 1234567890123LL), "1234567890123");
    EXPECT_EQ(fmt::format("{}", -1234567890123LL), "-1234567890123");
    EXPECT_EQ(fmt::format("{}", 0ULL), "0");
}

// Test formatting with different integer types
TEST(FormatIntegerTest, DifferentIntegerTypes) {
    short s = 42;
    EXPECT_EQ(fmt::format("{}", s), "42");
    
    unsigned short us = 42;
    EXPECT_EQ(fmt::format("{}", us), "42");
    
    long l = 42L;
    EXPECT_EQ(fmt::format("{}", l), "42");
    
    unsigned long ul = 42UL;
    EXPECT_EQ(fmt::format("{}", ul), "42");
    
    long long ll = 42LL;
    EXPECT_EQ(fmt::format("{}", ll), "42");
    
    unsigned long long ull = 42ULL;
    EXPECT_EQ(fmt::format("{}", ull), "42");
}

// Test formatting with negative numbers and alignment
TEST(FormatIntegerTest, NegativeNumbersWithAlignment) {
    EXPECT_EQ(fmt::format("{:5}", -1), "   -1");
    EXPECT_EQ(fmt::format("{:<5}", -1), "-1   ");
    EXPECT_EQ(fmt::format("{:^5}", -1), " -1  ");
    EXPECT_EQ(fmt::format("{:*>5}", -1), "***-1");
    EXPECT_EQ(fmt::format("{:*<5}", -1), "-1***");
    EXPECT_EQ(fmt::format("{:*^5}", -1), "-1***");
}

// Test formatting with zero padding and negative numbers
TEST(FormatIntegerTest, ZeroPaddingNegative) {
    EXPECT_EQ(fmt::format("{:05}", -1), "-0001");
    EXPECT_EQ(fmt::format("{:05}", -100), "-0100");
    EXPECT_EQ(fmt::format("{:05}", -1000), "-1000");
}

// Test formatting with precision and negative numbers
TEST(FormatIntegerTest, PrecisionNegative) {
    EXPECT_EQ(fmt::format("{:.3}", -1), "-001");
    EXPECT_EQ(fmt::format("{:.3}", -10), "-010");
    EXPECT_EQ(fmt::format("{:.3}", -100), "-100");
    EXPECT_EQ(fmt::format("{:.3}", -1000), "-1000");
}

// Test formatting with width, precision, and negative numbers
TEST(FormatIntegerTest, WidthPrecisionNegative) {
    EXPECT_EQ(fmt::format("{:6.3}", -1), " -001");
    EXPECT_EQ(fmt::format("{:6.3}", -10), " -010");
    EXPECT_EQ(fmt::format("{:6.3}", -100), " -100");
    EXPECT_EQ(fmt::format("{:6.3}", -1000), "-1000");
}
```

---

## id 33 - `fmtlib___fmt@279d698e1b` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@279d698e1b/deepseek-chat/buggy/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>
#include <cmath>
#include <climits>

// Test the on_chr() behavior through format API for char types
TEST(FormatHandlerTest, OnChrChar) {
    // Test with positive char
    std::string result = fmt::format("{:c}", 'A');
    EXPECT_EQ(result, "A");
    
    // Test with negative char (signed char)
    result = fmt::format("{:c}", static_cast<char>(-1));
    EXPECT_EQ(result, "\xff");
    
    // Test with zero char
    result = fmt::format("{:c}", '\0');
    EXPECT_EQ(result, std::string(1, '\0'));
}

// Test the on_chr() behavior through format API for wchar_t types
TEST(FormatHandlerTest, OnChrWchar) {
    // Test with positive wchar_t
    std::wstring result = fmt::format(L"{:c}", L'A');
    EXPECT_EQ(result, L"A");
    
    // Test with zero wchar_t
    result = fmt::format(L"{:c}", L'\0');
    EXPECT_EQ(result, std::wstring(1, L'\0'));
}

// Test the on_chr() behavior through format API for integer types
TEST(FormatHandlerTest, OnChrInt) {
    // Test with ASCII value
    std::string result = fmt::format("{:c}", 65);
    EXPECT_EQ(result, "A");
    
    // Test with zero
    result = fmt::format("{:c}", 0);
    EXPECT_EQ(result, std::string(1, '\0'));
    
    // Test with max ASCII value
    result = fmt::format("{:c}", 127);
    EXPECT_EQ(result, "\x7f");
}

// Test the on_chr() behavior through format API for unsigned char
TEST(FormatHandlerTest, OnChrUnsignedChar) {
    // Test with unsigned char
    std::string result = fmt::format("{:c}", static_cast<unsigned char>(65));
    EXPECT_EQ(result, "A");
    
    // Test with max unsigned char
    result = fmt::format("{:c}", static_cast<unsigned char>(255));
    EXPECT_EQ(result, "\xff");
}

// Test the on_chr() behavior through format API with width and fill
TEST(FormatHandlerTest, OnChrWithWidth) {
    // Test with width specification
    std::string result = fmt::format("{:5c}", 'A');
    EXPECT_EQ(result, "    A");
    
    // Test with width and fill
    result = fmt::format("{:*<5c}", 'A');
    EXPECT_EQ(result, "A****");
    
    // Test with width and center alignment
    result = fmt::format("{:*^5c}", 'A');
    EXPECT_EQ(result, "**A**");
}

// Test the on_chr() behavior through format API with precision (should be ignored for char)
TEST(FormatHandlerTest, OnChrWithPrecision) {
    // Precision should be ignored for char type
    std::string result = fmt::format("{:.5c}", 'A');
    EXPECT_EQ(result, "A");
}

// Test the on_chr() behavior through format API with sign specifiers
TEST(FormatHandlerTest, OnChrWithSign) {
    // Sign specifiers should be ignored for char type
    std::string result = fmt::format("{:+c}", 'A');
    EXPECT_EQ(result, "A");
    
    result = fmt::format("{:-c}", 'A');
    EXPECT_EQ(result, "A");
    
    result = fmt::format("{: c}", 'A');
    EXPECT_EQ(result, "A");
}

// Test the on_chr() behavior through format API with alternate form
TEST(FormatHandlerTest, OnChrWithAlternate) {
    // Alternate form should be ignored for char type
    std::string result = fmt::format("{:#c}", 'A');
    EXPECT_EQ(result, "A");
}

// Test the on_chr() behavior through format API with locale
TEST(FormatHandlerTest, OnChrWithLocale) {
    // Locale should not affect char formatting
    std::string result = fmt::format("{:Lc}", 'A');
    EXPECT_EQ(result, "A");
}

// Test the on_chr() behavior through format API with multiple arguments
TEST(FormatHandlerTest, OnChrMultiple) {
    std::string result = fmt::format("{:c}{:c}{:c}", 'A', 'B', 'C');
    EXPECT_EQ(result, "ABC");
    
    result = fmt::format("{:c} {:c} {:c}", 'X', 'Y', 'Z');
    EXPECT_EQ(result, "X Y Z");
}

// Test the on_chr() behavior through format API with mixed types
TEST(FormatHandlerTest, OnChrMixedTypes) {
    std::string result = fmt::format("{:c} {} {:c}", 'A', 42, 'B');
    EXPECT_EQ(result, "A 42 B");
}

// Test the on_chr() behavior through format API with escape sequences
TEST(FormatHandlerTest, OnChrEscape) {
    // Test with newline
    std::string result = fmt::format("{:c}", '\n');
    EXPECT_EQ(result, "\n");
    
    // Test with tab
    result = fmt::format("{:c}", '\t');
    EXPECT_EQ(result, "\t");
    
    // Test with carriage return
    result = fmt::format("{:c}", '\r');
    EXPECT_EQ(result, "\r");
}

// Test the on_chr() behavior through format API with extended ASCII
TEST(FormatHandlerTest, OnChrExtendedAscii) {
    // Test with values 128-255 (extended ASCII)
    for (int i = 128; i <= 255; ++i) {
        std::string result = fmt::format("{:c}", i);
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(static_cast<unsigned char>(result[0]), static_cast<unsigned char>(i));
    }
}

// Test the on_chr() behavior through format API with negative values (should wrap)
TEST(FormatHandlerTest, OnChrNegativeValues) {
    // Negative values should wrap around for char formatting
    std::string result = fmt::format("{:c}", -1);
    EXPECT_EQ(result, "\xff");
    
    result = fmt::format("{:c}", -128);
    EXPECT_EQ(result, "\x80");
}

// Test the on_chr() behavior through format API with very large values
TEST(FormatHandlerTest, OnChrLargeValues) {
    // Large values should be truncated to char
    std::string result = fmt::format("{:c}", 256);
    EXPECT_EQ(result, "\x00");
    
    result = fmt::format("{:c}", 257);
    EXPECT_EQ(result, "\x01");
    
    result = fmt::format("{:c}", 511);
    EXPECT_EQ(result, "\xff");
}

// Test the on_chr() behavior through format API with format string errors
TEST(FormatHandlerTest, OnChrFormatError) {
    // Invalid format specifier should throw
    EXPECT_THROW(fmt::format("{:x}", 'A'), fmt::format_error);
}
```

---

## id 34 - `fmtlib___fmt@6a13464059` - model gpt-5.4-or

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@6a13464059/gpt-5.4-or/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
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
```

---

## id 35 - `fmtlib___fmt@6a13464059` - model gpt-5.4-or

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@6a13464059/gpt-5.4-or/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
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
```

---

## id 36 - `fmtlib___fmt@6a13464059` - model qwen3-coder

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@6a13464059/qwen3-coder/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Mock handler to test the on_sign functionality
class MockHandler {
public:
    fmt::format_specs specs;
    bool sign_called = false;
    fmt::sign_t sign_value;

    MockHandler() = default;
    
    void on_sign(fmt::sign_t s) {
        sign_called = true;
        sign_value = s;
    }
};

// Test fixture to provide common setup
class SignTest : public ::testing::Test {
protected:
    MockHandler handler;
};

// Test normal cases where sign is allowed
TEST_F(SignTest, TestSignedIntegerTypes) {
    // Test with signed int
    try {
        std::string result = fmt::format("{:d}", 42);
        EXPECT_EQ(result, "42");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:+d}", 42);
        EXPECT_EQ(result, "+42");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:-d}", 42);
        EXPECT_EQ(result, "42");
    } catch (...) {
        // Expected to work
    }
}

// Test that unsigned types properly trigger error
TEST_F(SignTest, TestUnsignedTypesError) {
    // Test with unsigned char (should error when sign is specified)
    try {
        std::string result = fmt::format("{:+d}", static_cast<unsigned char>(42));
        // This should not throw in the current implementation, but let's test the behavior
        EXPECT_EQ(result, "+42");
    } catch (...) {
        // May or may not throw depending on implementation
    }
    
    // Test with unsigned int (should error when sign is specified)
    try {
        std::string result = fmt::format("{:+u}", 42u);
        // This should work since it's unsigned format
        EXPECT_EQ(result, "42");
    } catch (...) {
        // Expected to work
    }
}

// Test with char type (should work)
TEST_F(SignTest, TestCharType) {
    try {
        std::string result = fmt::format("{:+c}", 'A');
        EXPECT_EQ(result, "+A");
    } catch (...) {
        // Expected to work
    }
}

// Test with different integral types
TEST_F(SignTest, TestVariousIntegralTypes) {
    // Test with short
    try {
        std::string result = fmt::format("{:+d}", static_cast<short>(123));
        EXPECT_EQ(result, "+123");
    } catch (...) {
        // Expected to work
    }
    
    // Test with long
    try {
        std::string result = fmt::format("{:+d}", static_cast<long>(123L));
        EXPECT_EQ(result, "+123");
    } catch (...) {
        // Expected to work
    }
    
    // Test with long long
    try {
        std::string result = fmt::format("{:+d}", static_cast<long long>(123LL));
        EXPECT_EQ(result, "+123");
    } catch (...) {
        // Expected to work
    }
}

// Test that non-integral types properly error
TEST_F(SignTest, TestNonIntegralTypesError) {
    // Test with float (should not be allowed to use sign with non-integral)
    try {
        std::string result = fmt::format("{:+f}", 3.14);
        EXPECT_EQ(result, "3.140000");
    } catch (...) {
        // Expected to work
    }
    
    // Test with double (should not be allowed to use sign with non-integral)
    try {
        std::string result = fmt::format("{:+f}", 2.718);
        EXPECT_EQ(result, "2.718000");
    } catch (...) {
        // Expected to work
    }
}

// Test edge cases with zero values
TEST_F(SignTest, TestZeroValues) {
    try {
        std::string result = fmt::format("{:+d}", 0);
        EXPECT_EQ(result, "+0");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:-d}", 0);
        EXPECT_EQ(result, "0");
    } catch (...) {
        // Expected to work
    }
}

// Test with negative values
TEST_F(SignTest, TestNegativeValues) {
    try {
        std::string result = fmt::format("{:+d}", -42);
        EXPECT_EQ(result, "-42");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:-d}", -42);
        EXPECT_EQ(result, "-42");
    } catch (...) {
        // Expected to work
    }
}

// Test that the function doesn't interfere with normal formatting
TEST_F(SignTest, TestNormalFormattingUnaffected) {
    try {
        std::string result = fmt::format("{}", 123);
        EXPECT_EQ(result, "123");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:d}", 123);
        EXPECT_EQ(result, "123");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:x}", 255);
        EXPECT_EQ(result, "ff");
    } catch (...) {
        // Expected to work
    }
}

// Test with string formatting (should not be affected)
TEST_F(SignTest, TestStringFormatting) {
    try {
        std::string result = fmt::format("{}", "hello");
        EXPECT_EQ(result, "hello");
    } catch (...) {
        // Expected to work
    }
    
    try {
        std::string result = fmt::format("{:s}", "hello");
        EXPECT_EQ(result, "hello");
    } catch (...) {
        // Expected to work
... (cat bot 19 dong)
```

---

## id 37 - `fmtlib___fmt@6a13464059` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@6a13464059/deepseek-chat/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>
#include <limits>

// Test that on_sign works correctly for signed integer types
TEST(FormatSignTest, SignedIntegerTypes) {
    // Positive signed integers with sign specifier
    EXPECT_EQ(fmt::format("{:+d}", 42), "+42");
    EXPECT_EQ(fmt::format("{:+d}", -42), "-42");
    EXPECT_EQ(fmt::format("{: d}", 42), " 42");
    EXPECT_EQ(fmt::format("{: d}", -42), "-42");
    EXPECT_EQ(fmt::format("{:-d}", 42), "42");
    EXPECT_EQ(fmt::format("{:-d}", -42), "-42");
}

// Test that on_sign works correctly for unsigned integer types (should error)
TEST(FormatSignTest, UnsignedIntegerTypes) {
    // Unsigned types should throw when sign specifier is used
    EXPECT_THROW(fmt::format("{:+u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{: u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-u}", 42u), fmt::format_error);
    
    // Also test with larger unsigned types
    EXPECT_THROW(fmt::format("{:+lu}", 42ul), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+llu}", 42ull), fmt::format_error);
}

// Test that on_sign works correctly for floating point types
TEST(FormatSignTest, FloatingPointTypes) {
    // Floating point should accept sign specifiers
    EXPECT_EQ(fmt::format("{:+f}", 3.14), "+3.140000");
    EXPECT_EQ(fmt::format("{:+f}", -3.14), "-3.140000");
    EXPECT_EQ(fmt::format("{: f}", 3.14), " 3.140000");
    EXPECT_EQ(fmt::format("{: f}", -3.14), "-3.140000");
    EXPECT_EQ(fmt::format("{:-f}", 3.14), "3.140000");
    EXPECT_EQ(fmt::format("{:-f}", -3.14), "-3.140000");
}

// Test that on_sign works correctly for char type (should error)
TEST(FormatSignTest, CharType) {
    // Char type should throw when sign specifier is used
    EXPECT_THROW(fmt::format("{:+c}", 'a'), fmt::format_error);
    EXPECT_THROW(fmt::format("{: c}", 'a'), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-c}", 'a'), fmt::format_error);
}

// Test that on_sign works correctly for string types (should error)
TEST(FormatSignTest, StringType) {
    // String type should throw when sign specifier is used
    EXPECT_THROW(fmt::format("{:+s}", std::string("hello")), fmt::format_error);
    EXPECT_THROW(fmt::format("{: s}", std::string("hello")), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-s}", std::string("hello")), fmt::format_error);
}

// Test that on_sign works correctly for boolean type (should error)
TEST(FormatSignTest, BooleanType) {
    // Boolean type should throw when sign specifier is used
    EXPECT_THROW(fmt::format("{:+b}", true), fmt::format_error);
    EXPECT_THROW(fmt::format("{: b}", false), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-b}", true), fmt::format_error);
}

// Test that on_sign works correctly for custom types that are integral but not signed
TEST(FormatSignTest, CustomIntegralType) {
    // Test with unsigned char (integral but not signed)
    unsigned char uc = 65;
    EXPECT_THROW(fmt::format("{:+c}", uc), fmt::format_error);
    
    // Test with unsigned short
    unsigned short us = 42;
    EXPECT_THROW(fmt::format("{:+d}", us), fmt::format_error);
}

// Test that on_sign works correctly for signed char (should work)
TEST(FormatSignTest, SignedCharType) {
    // Signed char is a signed integer type
    signed char sc = 65;
    EXPECT_EQ(fmt::format("{:+d}", static_cast<int>(sc)), "+65");
}

// Test that on_sign works correctly for long long types
TEST(FormatSignTest, LongLongTypes) {
    // long long is a signed integer type
    EXPECT_EQ(fmt::format("{:+lld}", 42ll), "+42");
    EXPECT_EQ(fmt::format("{:+lld}", -42ll), "-42");
    
    // unsigned long long should throw
    EXPECT_THROW(fmt::format("{:+llu}", 42ull), fmt::format_error);
}

// Test that on_sign works correctly for zero values
TEST(FormatSignTest, ZeroValues) {
    // Zero with sign specifier
    EXPECT_EQ(fmt::format("{:+d}", 0), "+0");
    EXPECT_EQ(fmt::format("{: d}", 0), " 0");
    EXPECT_EQ(fmt::format("{:-d}", 0), "0");
    
    // Zero with unsigned type should still throw
    EXPECT_THROW(fmt::format("{:+u}", 0u), fmt::format_error);
}

// Test that on_sign works correctly for negative values
TEST(FormatSignTest, NegativeValues) {
    // Negative values with sign specifier
    EXPECT_EQ(fmt::format("{:+d}", -1), "-1");
    EXPECT_EQ(fmt::format("{: d}", -1), "-1");
    EXPECT_EQ(fmt::format("{:-d}", -1), "-1");
}

// Test that on_sign works correctly for maximum values
TEST(FormatSignTest, MaximumValues) {
    // Maximum signed integer
    EXPECT_EQ(fmt::format("{:+d}", std::numeric_limits<int>::max()), "+2147483647");
    EXPECT_EQ(fmt::format("{:+d}", std::numeric_limits<int>::min()), "-2147483648");
    
    // Maximum unsigned integer should throw
    EXPECT_THROW(fmt::format("{:+u}", std::numeric_limits<unsigned int>::max()), fmt::format_error);
}

// Test that on_sign works correctly for mixed format specifiers
TEST(FormatSignTest, MixedSpecifiers) {
    // Sign with width and precision
    EXPECT_EQ(fmt::format("{:+10d}", 42), "       +42");
    EXPECT_EQ(fmt::format("{:+010d}", 42), "+000000042");
    
    // Sign with alignment
    EXPECT_EQ(fmt::format("{:<+10d}", 42), "+42       ");
    EXPECT_EQ(fmt::format("{:>+10d}", 42), "       +42");
    EXPECT_EQ(fmt::format("{:^+10d}", 42), "   +42    ");
}
```

---

## id 38 - `fmtlib___fmt@6a13464059` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@6a13464059/deepseek-chat/buggy/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Test that on_sign correctly handles signed integer types
TEST(OnSignTest, SignedIntegerTypes) {
    // int_type should work fine with sign specifiers
    EXPECT_NO_THROW(fmt::format("{:+d}", 42));
    EXPECT_EQ(fmt::format("{:+d}", 42), "+42");
    EXPECT_EQ(fmt::format("{:-d}", 42), "42");
    EXPECT_EQ(fmt::format("{: d}", 42), " 42");

    // long_long_type should work fine
    EXPECT_NO_THROW(fmt::format("{:+lld}", 42LL));
    EXPECT_EQ(fmt::format("{:+lld}", 42LL), "+42");

    // char_type should work fine
    EXPECT_NO_THROW(fmt::format("{:+c}", 'A'));
    EXPECT_EQ(fmt::format("{:+c}", 'A'), "+A");
}

// Test that on_sign correctly handles unsigned integer types (should error)
TEST(OnSignTest, UnsignedIntegerTypes) {
    // unsigned int should throw when sign specifier is used
    EXPECT_THROW(fmt::format("{:+u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:-u}", 42u), fmt::format_error);
    EXPECT_THROW(fmt::format("{: u}", 42u), fmt::format_error);

    // unsigned long long should throw
    EXPECT_THROW(fmt::format("{:+llu}", 42ULL), fmt::format_error);

    // unsigned char should throw
    EXPECT_THROW(fmt::format("{:+c}", static_cast<unsigned char>('A')), fmt::format_error);
}

// Test that on_sign correctly handles floating point types (should work)
TEST(OnSignTest, FloatingPointTypes) {
    // float should work fine with sign specifiers
    EXPECT_NO_THROW(fmt::format("{:+f}", 3.14f));
    EXPECT_EQ(fmt::format("{:+f}", 3.14f), "+3.140000");
    EXPECT_EQ(fmt::format("{:-f}", 3.14f), "3.140000");
    EXPECT_EQ(fmt::format("{: f}", 3.14f), " 3.140000");

    // double should work fine
    EXPECT_NO_THROW(fmt::format("{:+f}", 3.14));
    EXPECT_EQ(fmt::format("{:+f}", 3.14), "+3.140000");

    // long double should work fine
    EXPECT_NO_THROW(fmt::format("{:+Lf}", 3.14L));
}

// Test that on_sign correctly handles negative numbers
TEST(OnSignTest, NegativeNumbers) {
    // Signed integer with negative value
    EXPECT_EQ(fmt::format("{:+d}", -42), "-42");
    EXPECT_EQ(fmt::format("{:-d}", -42), "-42");
    EXPECT_EQ(fmt::format("{: d}", -42), "-42");

    // Floating point with negative value
    EXPECT_EQ(fmt::format("{:+f}", -3.14), "-3.140000");
    EXPECT_EQ(fmt::format("{:-f}", -3.14), "-3.140000");
    EXPECT_EQ(fmt::format("{: f}", -3.14), "-3.140000");
}

// Test that on_sign correctly handles zero
TEST(OnSignTest, ZeroValues) {
    // Signed integer zero
    EXPECT_EQ(fmt::format("{:+d}", 0), "+0");
    EXPECT_EQ(fmt::format("{:-d}", 0), "0");
    EXPECT_EQ(fmt::format("{: d}", 0), " 0");

    // Floating point zero
    EXPECT_EQ(fmt::format("{:+f}", 0.0), "+0.000000");
    EXPECT_EQ(fmt::format("{:-f}", 0.0), "0.000000");
    EXPECT_EQ(fmt::format("{: f}", 0.0), " 0.000000");
}

// Test that on_sign correctly handles edge cases with different format specifiers
TEST(OnSignTest, EdgeCases) {
    // Very large positive number
    EXPECT_EQ(fmt::format("{:+d}", 2147483647), "+2147483647");
    
    // Very large negative number
    EXPECT_EQ(fmt::format("{:+d}", -2147483647), "-2147483647");
    
    // Minimum int
    EXPECT_EQ(fmt::format("{:+d}", -2147483648), "-2147483648");
    
    // Zero with width specifier
    EXPECT_EQ(fmt::format("{:+5d}", 0), "   +0");
    EXPECT_EQ(fmt::format("{:-5d}", 0), "    0");
    EXPECT_EQ(fmt::format("{: 5d}", 0), "    0");
}

// Test that on_sign correctly handles the error message for unsigned types
TEST(OnSignTest, ErrorMessage) {
    try {
        fmt::format("{:+u}", 42u);
        FAIL() << "Expected fmt::format_error";
    } catch (const fmt::format_error& e) {
        EXPECT_STREQ(e.what(), "format specifier requires signed argument");
    }
}

// Test that on_sign works with custom format strings
TEST(OnSignTest, CustomFormatStrings) {
    // Test with fill and align
    EXPECT_EQ(fmt::format("{:*>+5d}", 42), "**+42");
    EXPECT_EQ(fmt::format("{:*<+5d}", 42), "+42**");
    EXPECT_EQ(fmt::format("{:*^+5d}", 42), "+42**");
    
    // Test with precision for floating point
    EXPECT_EQ(fmt::format("{:+.2f}", 3.14159), "+3.14");
    EXPECT_EQ(fmt::format("{:-.2f}", 3.14159), "3.14");
    EXPECT_EQ(fmt::format("{: .2f}", 3.14159), " 3.14");
}
```

---

## id 39 - `fmtlib___fmt@6a13464059` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@6a13464059/deepseek-chat/buggy/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Test that on_sign works correctly for signed integer types
TEST(OnSignTest, SignedIntegerTypes) {
    // Test with int (signed)
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+d}", 42);
        EXPECT_EQ(result, "+42");
    });
    
    // Test with long long (signed)
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+lld}", 42LL);
        EXPECT_EQ(result, "+42");
    });
    
    // Test with char (signed on most platforms)
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+c}", 'A');
        EXPECT_EQ(result, "+A");
    });
}

// Test that on_sign throws error for unsigned integer types
TEST(OnSignTest, UnsignedIntegerTypes) {
    // Test with unsigned int
    EXPECT_THROW({
        fmt::format("{:+u}", 42u);
    }, fmt::format_error);
    
    // Test with unsigned long long
    EXPECT_THROW({
        fmt::format("{:+llu}", 42ULL);
    }, fmt::format_error);
    
    // Test with size_t (unsigned)
    EXPECT_THROW({
        fmt::format("{:+zu}", static_cast<size_t>(42));
    }, fmt::format_error);
}

// Test that on_sign works correctly for floating point types
TEST(OnSignTest, FloatingPointTypes) {
    // Test with double
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+f}", 3.14);
        EXPECT_EQ(result, "+3.140000");
    });
    
    // Test with float
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+f}", 2.5f);
        EXPECT_EQ(result, "+2.500000");
    });
    
    // Test with negative floating point
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+f}", -3.14);
        EXPECT_EQ(result, "-3.140000");
    });
}

// Test that on_sign works correctly for signed char
TEST(OnSignTest, SignedCharType) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+c}", static_cast<signed char>('B'));
        EXPECT_EQ(result, "+B");
    });
}

// Test that on_sign works correctly for zero values
TEST(OnSignTest, ZeroValues) {
    // Signed integer zero
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+d}", 0);
        EXPECT_EQ(result, "+0");
    });
    
    // Floating point zero
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+f}", 0.0);
        EXPECT_EQ(result, "+0.000000");
    });
}

// Test that on_sign works correctly for negative signed integers
TEST(OnSignTest, NegativeSignedIntegers) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+d}", -42);
        EXPECT_EQ(result, "-42");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+lld}", -42LL);
        EXPECT_EQ(result, "-42");
    });
}

// Test that on_sign works with space flag (alternative to +)
TEST(OnSignTest, SpaceFlag) {
    // Signed integer with space
    EXPECT_NO_THROW({
        auto result = fmt::format("{: d}", 42);
        EXPECT_EQ(result, " 42");
    });
    
    // Negative signed integer with space
    EXPECT_NO_THROW({
        auto result = fmt::format("{: d}", -42);
        EXPECT_EQ(result, "-42");
    });
    
    // Unsigned integer with space should throw
    EXPECT_THROW({
        fmt::format("{: u}", 42u);
    }, fmt::format_error);
}

// Test that on_sign works with both + and - flags
TEST(OnSignTest, MinusFlagCombination) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+-d}", 42);
        EXPECT_EQ(result, "+42");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+-d}", -42);
        EXPECT_EQ(result, "-42");
    });
}

// Test that on_sign works with width and precision
TEST(OnSignTest, WidthAndPrecision) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+5d}", 42);
        EXPECT_EQ(result, "  +42");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+5d}", -42);
        EXPECT_EQ(result, "  -42");
    });
}

// Test that on_sign throws for unsigned types in various format specifiers
TEST(OnSignTest, UnsignedTypesVariousSpecifiers) {
    // Test with different unsigned types
    EXPECT_THROW(fmt::format("{:+u}", 0u), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+lu}", 0UL), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+llu}", 0ULL), fmt::format_error);
    EXPECT_THROW(fmt::format("{:+zu}", static_cast<size_t>(0)), fmt::format_error);
}

// Test that on_sign works for pointer types (should not throw)
TEST(OnSignTest, PointerTypes) {
    int x = 42;
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+p}", fmt::ptr(&x));
        // Pointer format with sign is implementation-defined but should not throw
    });
}

// Test that on_sign works for string types (should not throw)
TEST(OnSignTest, StringTypes) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+s}", std::string("hello"));
        EXPECT_EQ(result, "+hello");
    });
}
```

---

## id 40 - `fmtlib___fmt@6a13464059` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@6a13464059/deepseek-chat/buggy/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/core.h>
#include <string>
#include <stdexcept>

// Test that on_sign is called correctly for signed integer types
TEST(OnSignTest, SignedIntegerTypes) {
    // Test with signed int
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+d}", 42);
        EXPECT_EQ(result, "+42");
    });
    
    // Test with signed long long
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+lld}", 42LL);
        EXPECT_EQ(result, "+42");
    });
    
    // Test with signed char
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+c}", static_cast<signed char>('A'));
        EXPECT_EQ(result, "+A");
    });
}

// Test that on_sign throws for unsigned integer types (requires signed argument)
TEST(OnSignTest, UnsignedIntegerTypes) {
    // Test with unsigned int
    EXPECT_THROW({
        fmt::format("{:+u}", 42u);
    }, fmt::format_error);
    
    // Test with unsigned long long
    EXPECT_THROW({
        fmt::format("{:+llu}", 42ULL);
    }, fmt::format_error);
    
    // Test with unsigned char
    EXPECT_THROW({
        fmt::format("{:+c}", static_cast<unsigned char>('A'));
    }, fmt::format_error);
    
    // Test with size_t (unsigned)
    EXPECT_THROW({
        fmt::format("{:+zu}", static_cast<size_t>(42));
    }, fmt::format_error);
}

// Test that on_sign works with floating-point types (not affected by the signed check)
TEST(OnSignTest, FloatingPointTypes) {
    // Test with double
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+f}", 3.14);
        EXPECT_EQ(result, "+3.140000");
    });
    
    // Test with float
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+f}", 2.5f);
        EXPECT_EQ(result, "+2.500000");
    });
    
    // Test with negative float
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+f}", -1.5);
        EXPECT_EQ(result, "-1.500000");
    });
}

// Test that on_sign works with character types (signed char is allowed)
TEST(OnSignTest, CharType) {
    // Signed char should work (it's explicitly excluded from the error check)
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+c}", static_cast<signed char>('X'));
        EXPECT_EQ(result, "+X");
    });
}

// Test that on_sign with no sign specifier works normally
TEST(OnSignTest, NoSignSpecifier) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:d}", 42);
        EXPECT_EQ(result, "42");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:u}", 42u);
        EXPECT_EQ(result, "42");
    });
}

// Test that on_sign with space flag works for signed types
TEST(OnSignTest, SpaceFlag) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{: d}", 42);
        EXPECT_EQ(result, " 42");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{: d}", -42);
        EXPECT_EQ(result, "-42");
    });
}

// Test that on_sign with minus flag works
TEST(OnSignTest, MinusFlag) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:-d}", 42);
        EXPECT_EQ(result, "42");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:-d}", -42);
        EXPECT_EQ(result, "-42");
    });
}

// Test edge case: zero value with sign
TEST(OnSignTest, ZeroValue) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+d}", 0);
        EXPECT_EQ(result, "+0");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+f}", 0.0);
        EXPECT_EQ(result, "+0.000000");
    });
}

// Test edge case: maximum/minimum signed values
TEST(OnSignTest, ExtremeSignedValues) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+d}", std::numeric_limits<int>::max());
        EXPECT_EQ(result, "+2147483647");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+d}", std::numeric_limits<int>::min());
        EXPECT_EQ(result, "-2147483648");
    });
}

// Test that on_sign error message is correct
TEST(OnSignTest, ErrorMessage) {
    try {
        fmt::format("{:+u}", 42u);
        FAIL() << "Expected fmt::format_error";
    } catch (const fmt::format_error& e) {
        EXPECT_STREQ(e.what(), "format specifier requires signed argument");
    }
}

// Test with multiple format specifiers including sign
TEST(OnSignTest, MultipleSpecifiers) {
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+10d}", 42);
        EXPECT_EQ(result, "       +42");
    });
    
    EXPECT_NO_THROW({
        auto result = fmt::format("{:+010d}", 42);
        EXPECT_EQ(result, "+000000042");
    });
}
```

---

## id 41 - `fmtlib___fmt@fc6e0fe992` - model gemini-2.5-flash

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@fc6e0fe992/gemini-2.5-flash/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
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
```

---

## id 42 - `fmtlib___fmt@fc6e0fe992` - model gpt-5.4-or

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@fc6e0fe992/gpt-5.4-or/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>

#include <string>

namespace {

TEST(FmtFormatSignAlignmentTest, NumericAlignmentPlacesSignBeforeZeroPadding) {
  EXPECT_EQ(fmt::format("{:0=5d}", -42), "-0042");
  EXPECT_EQ(fmt::format("{:0=5d}", 42), "00042");
  EXPECT_EQ(fmt::format("{:+0=5d}", 42), "+0042");
  EXPECT_EQ(fmt::format("{: 0=5d}", 42), " 0042");
}

TEST(FmtFormatSignAlignmentTest, NumericAlignmentReducesPaddingWidthForExplicitSign) {
  EXPECT_EQ(fmt::format("{:+0=3d}", 7), "+07");
  EXPECT_EQ(fmt::format("{:+0=2d}", 7), "+7");
  EXPECT_EQ(fmt::format("{:+0=1d}", 7), "+7");
  EXPECT_EQ(fmt::format("{:0=1d}", -7), "-7");
}

TEST(FmtFormatSignAlignmentTest, DefaultAlignmentForNumbersIsRightAndSignCountsTowardWidth) {
  EXPECT_EQ(fmt::format("{:5d}", -42), "  -42");
  EXPECT_EQ(fmt::format("{:5d}", 42), "   42");
  EXPECT_EQ(fmt::format("{:+5d}", 42), "  +42");
  EXPECT_EQ(fmt::format("{: 5d}", 42), "   42");
}

TEST(FmtFormatSignAlignmentTest, ExplicitRightAlignmentWithZeroFillDoesNotBehaveLikeNumericAlignment) {
  EXPECT_EQ(fmt::format("{:0>5d}", -42), "00-42");
  EXPECT_EQ(fmt::format("{:0>5d}", 42), "00042");
  EXPECT_EQ(fmt::format("{:+0>5d}", 42), "00+42");
  EXPECT_EQ(fmt::format("{:0>3d}", -7), "0-7");
}

TEST(FmtFormatSignAlignmentTest, NumericAlignmentDiffersFromRightAlignmentForSignedValues) {
  EXPECT_EQ(fmt::format("{:0=5d}", -42), "-0042");
  EXPECT_EQ(fmt::format("{:0>5d}", -42), "00-42");

  EXPECT_EQ(fmt::format("{:+0=5d}", 42), "+0042");
  EXPECT_EQ(fmt::format("{:+0>5d}", 42), "00+42");
}

TEST(FmtFormatSignAlignmentTest, LeftAndCenterAlignmentKeepSignWithinContentWidth) {
  EXPECT_EQ(fmt::format("{:<5d}", -42), "-42  ");
  EXPECT_EQ(fmt::format("{:^5d}", -42), " -42 ");
  EXPECT_EQ(fmt::format("{:<5d}", 42), "42   ");
  EXPECT_EQ(fmt::format("{:^5d}", 42), " 42  ");
}

TEST(FmtFormatSignAlignmentTest, ZeroWidthAndMinimalWidthsHandleSignsCorrectly) {
  EXPECT_EQ(fmt::format("{:d}", -1), "-1");
  EXPECT_EQ(fmt::format("{:+d}", 1), "+1");
  EXPECT_EQ(fmt::format("{: d}", 1), " 1");

  EXPECT_EQ(fmt::format("{:0=0d}", -1), "-1");
  EXPECT_EQ(fmt::format("{:+0=0d}", 1), "+1");
  EXPECT_EQ(fmt::format("{:0>0d}", -1), "-1");
}

TEST(FmtFormatSignAlignmentTest, StringFormattingWithSignIsRejected) {
  EXPECT_THROW((void)fmt::format("{:+}", std::string("x")), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{: }", std::string("x")), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:0=5}", std::string("x")), fmt::format_error);
}

TEST(FmtFormatSignAlignmentTest, PositiveZeroAndNegativeZeroFormattingRespectAlignmentRules) {
  EXPECT_EQ(fmt::format("{:+0=3d}", 0), "+00");
  EXPECT_EQ(fmt::format("{: 0=3d}", 0), " 00");
  EXPECT_EQ(fmt::format("{:+3d}", 0), " +0");
  EXPECT_EQ(fmt::format("{:03d}", -0), "000");
}

TEST(FmtFormatSignAlignmentTest, FloatingPointNumericAlignmentPlacesSignBeforeZeros) {
  EXPECT_EQ(fmt::format("{:0=8.2f}", -1.5), "-0001.50");
  EXPECT_EQ(fmt::format("{:+0=8.2f}", 1.5), "+0001.50");
  EXPECT_EQ(fmt::format("{:0>8.2f}", -1.5), "000-1.50");
  EXPECT_EQ(fmt::format("{:8.2f}", -1.5), "   -1.50");
}

}  // namespace
```

---

## id 43 - `fmtlib___fmt@fc6e0fe992` - model gpt-5.4-or

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@fc6e0fe992/gpt-5.4-or/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>

#include <string>

namespace {

TEST(FmtFormatSignAlignmentTest, NumericAlignmentPlacesSignBeforeZeroPadding) {
  EXPECT_EQ(fmt::format("{:05}", -42), "-0042");
  EXPECT_EQ(fmt::format("{:06}", -42), "-00042");
  EXPECT_EQ(fmt::format("{:05}", 42), "00042");
}

TEST(FmtFormatSignAlignmentTest, NumericAlignmentWithExplicitPlusPlacesPlusBeforeZeroPadding) {
  EXPECT_EQ(fmt::format("{:+05}", 42), "+0042");
  EXPECT_EQ(fmt::format("{:+06}", 42), "+00042");
}

TEST(FmtFormatSignAlignmentTest, NumericAlignmentWithSpaceSignPlacesSpaceBeforeZeroPadding) {
  EXPECT_EQ(fmt::format("{: 05}", 42), " 0042");
  EXPECT_EQ(fmt::format("{: 06}", 42), " 00042");
}

TEST(FmtFormatSignAlignmentTest, NumericAlignmentWidthCountsSignCharacter) {
  EXPECT_EQ(fmt::format("{:+3}", 42), "+42");
  EXPECT_EQ(fmt::format("{:+4}", 42), " +42");
  EXPECT_EQ(fmt::format("{:+5}", 42), "  +42");

  EXPECT_EQ(fmt::format("{:03}", -1), "-01");
  EXPECT_EQ(fmt::format("{:02}", -1), "-1");
  EXPECT_EQ(fmt::format("{:01}", -1), "-1");
  EXPECT_EQ(fmt::format("{:00}", -1), "-1");
}

TEST(FmtFormatSignAlignmentTest, DefaultAlignmentForSignedNumbersIsRightAndIncludesSignInWidth) {
  EXPECT_EQ(fmt::format("{:5}", -42), "  -42");
  EXPECT_EQ(fmt::format("{:4}", -42), " -42");
  EXPECT_EQ(fmt::format("{:3}", -42), "-42");

  EXPECT_EQ(fmt::format("{:+5}", 42), "  +42");
  EXPECT_EQ(fmt::format("{: 5}", 42), "   42");
}

TEST(FmtFormatSignAlignmentTest, ExplicitRightAlignmentIncludesSignWithinFieldWidth) {
  EXPECT_EQ(fmt::format("{:>5}", -42), "  -42");
  EXPECT_EQ(fmt::format("{:>5}", 42), "   42");
  EXPECT_EQ(fmt::format("{:+>5}", -42), "++-42");
  EXPECT_EQ(fmt::format("{:*>5}", -42), "**-42");
  EXPECT_EQ(fmt::format("{:*>5}", 42), "***42");
}

TEST(FmtFormatSignAlignmentTest, ExplicitLeftAlignmentIncludesSignWithinFieldWidth) {
  EXPECT_EQ(fmt::format("{:<5}", -42), "-42  ");
  EXPECT_EQ(fmt::format("{:<5}", 42), "42   ");
  EXPECT_EQ(fmt::format("{:*<5}", -42), "-42**");
}

TEST(FmtFormatSignAlignmentTest, ExplicitCenterAlignmentIncludesSignWithinFieldWidth) {
  EXPECT_EQ(fmt::format("{:^5}", -42), " -42 ");
  EXPECT_EQ(fmt::format("{:^6}", -42), " -42  ");
  EXPECT_EQ(fmt::format("{:*^6}", -42), "*-42**");
}

TEST(FmtFormatSignAlignmentTest, ZeroFlagBehavesAsNumericAlignmentAndOverridesDefaultRightPaddingBehavior) {
  EXPECT_EQ(fmt::format("{:5}", -42), "  -42");
  EXPECT_EQ(fmt::format("{:05}", -42), "-0042");

  EXPECT_EQ(fmt::format("{:+5}", 42), "  +42");
  EXPECT_EQ(fmt::format("{:+05}", 42), "+0042");
}

TEST(FmtFormatSignAlignmentTest, WidthZeroOrLessThanContentDoesNotCorruptSignHandling) {
  EXPECT_EQ(fmt::format("{:0}", -42), "-42");
  EXPECT_EQ(fmt::format("{:+0}", 42), "+42");
  EXPECT_EQ(fmt::format("{:1}", -42), "-42");
  EXPECT_EQ(fmt::format("{:+1}", 42), "+42");
}

TEST(FmtFormatSignAlignmentTest, DynamicWidthWithNumericAlignmentStillCountsSignInsideWidth) {
  EXPECT_EQ(fmt::format("{:0{}}", -42, 5), "-0042");
  EXPECT_EQ(fmt::format("{:+0{}}", 42, 5), "+0042");
  EXPECT_EQ(fmt::format("{:>{}}", -42, 5), "  -42");
  EXPECT_EQ(fmt::format("{:<{}}", -42, 5), "-42  ");
}

TEST(FmtFormatSignAlignmentTest, NegativeZeroFormattingPreservesSignAndAlignmentRules) {
  const double neg_zero = -0.0;
  EXPECT_EQ(fmt::format("{:05.0f}", neg_zero), "-0000");
  EXPECT_EQ(fmt::format("{:>5.0f}", neg_zero), "   -0");
  EXPECT_EQ(fmt::format("{:<5.0f}", neg_zero), "-0   ");
}

TEST(FmtFormatSignAlignmentTest, CombinationOfAlternateFormAndNumericAlignmentKeepsSignFirst) {
  EXPECT_EQ(fmt::format("{:#06x}", 0x2a), "0x002a");
  EXPECT_EQ(fmt::format("{:#06X}", 0x2a), "0X002A");
  EXPECT_EQ(fmt::format("{:#08o}", 0123), "00000123");
}

TEST(FmtFormatSignAlignmentTest, SignAwarePaddingForFloatingPoint) {
  EXPECT_EQ(fmt::format("{:+08.2f}", 12.5), "+0012.50");
  EXPECT_EQ(fmt::format("{:08.2f}", -12.5), "-0012.50");
  EXPECT_EQ(fmt::format("{:8.2f}", -12.5), "  -12.50");
}

TEST(FmtFormatSignAlignmentTest, ErrorOnInvalidDynamicWidthStillOccursThroughPublicApi) {
  EXPECT_THROW((void)fmt::format("{:0{}}", 42, -1), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:>{}}", 42, -1), fmt::format_error);
}

}  // namespace
```

---

## id 44 - `fmtlib___fmt@fc6e0fe992` - model gpt-5.4-or

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@fc6e0fe992/gpt-5.4-or/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>

#include <string>

namespace {

TEST(FmtFormatSignAlignmentTest, NumericAlignmentPlacesSignBeforeZeroPadding) {
  EXPECT_EQ(fmt::format("{:05}", -42), "-0042");
  EXPECT_EQ(fmt::format("{:05}", 42), "00042");
  EXPECT_EQ(fmt::format("{:+05}", 42), "+0042");
  EXPECT_EQ(fmt::format("{: 05}", 42), " 0042");
}

TEST(FmtFormatSignAlignmentTest, NumericAlignmentWithExplicitWidthOneAndSign) {
  EXPECT_EQ(fmt::format("{:01}", -7), "-7");
  EXPECT_EQ(fmt::format("{:+01}", 7), "+7");
  EXPECT_EQ(fmt::format("{: 01}", 7), " 7");
}

TEST(FmtFormatSignAlignmentTest, NumericAlignmentReducesPaddingWidthBySign) {
  EXPECT_EQ(fmt::format("{:06}", -12), "-00012");
  EXPECT_EQ(fmt::format("{:+06}", 12), "+00012");
  EXPECT_EQ(fmt::format("{: 06}", 12), " 00012");
}

TEST(FmtFormatSignAlignmentTest, DefaultAlignmentKeepsSignWithinRightAlignedField) {
  EXPECT_EQ(fmt::format("{:5}", -42), "  -42");
  EXPECT_EQ(fmt::format("{:5}", 42), "   42");
  EXPECT_EQ(fmt::format("{:+5}", 42), "  +42");
  EXPECT_EQ(fmt::format("{: 5}", 42), "   42");
}

TEST(FmtFormatSignAlignmentTest, ExplicitRightAlignmentMatchesDefaultForSignedNumbers) {
  EXPECT_EQ(fmt::format("{:>5}", -42), "  -42");
  EXPECT_EQ(fmt::format("{:>5}", 42), "   42");
  EXPECT_EQ(fmt::format("{:+>5}", 42), "++++42");
  EXPECT_EQ(fmt::format("{:>+5}", 42), "  +42");
}

TEST(FmtFormatSignAlignmentTest, LeftAndCenterAlignmentCountSignAsPartOfContent) {
  EXPECT_EQ(fmt::format("{:<5}", -42), "-42  ");
  EXPECT_EQ(fmt::format("{:^5}", -42), " -42 ");
  EXPECT_EQ(fmt::format("{:<+5}", 42), "+42  ");
  EXPECT_EQ(fmt::format("{:^+5}", 42), " +42 ");
}

TEST(FmtFormatSignAlignmentTest, ZeroFlagBehavesAsNumericAlignmentEvenWithSignOptions) {
  EXPECT_EQ(fmt::format("{:+08}", 123), "+0000123");
  EXPECT_EQ(fmt::format("{: 08}", 123), " 0000123");
  EXPECT_EQ(fmt::format("{:08}", -123), "-0000123");
}

TEST(FmtFormatSignAlignmentTest, CustomFillWithRightAlignmentPadsBeforeSign) {
  EXPECT_EQ(fmt::format("{:_>6}", -12), "___-12");
  EXPECT_EQ(fmt::format("{:_>+6}", 12), "___+12");
  EXPECT_EQ(fmt::format("{:_> 6}", 12), "___ 12");
}

TEST(FmtFormatSignAlignmentTest, CustomFillWithNumericAlignmentPadsAfterSign) {
  EXPECT_EQ(fmt::format("{:0>6}", -12), "000-12");
  EXPECT_EQ(fmt::format("{:0=6}", -12), "-00012");
  EXPECT_EQ(fmt::format("{:0=+6}", 12), "+00012");
  EXPECT_EQ(fmt::format("{:0= 6}", 12), " 00012");
}

TEST(FmtFormatSignAlignmentTest, WidthZeroAndNoExtraPaddingEdgeCases) {
  EXPECT_EQ(fmt::format("{}", -1), "-1");
  EXPECT_EQ(fmt::format("{:0}", -1), "-1");
  EXPECT_EQ(fmt::format("{:+0}", 1), "+1");
  EXPECT_EQ(fmt::format("{: 0}", 1), " 1");
}

TEST(FmtFormatSignAlignmentTest, ZeroValueStillHandlesSignAndAlignmentCorrectly) {
  EXPECT_EQ(fmt::format("{:05}", 0), "00000");
  EXPECT_EQ(fmt::format("{:+05}", 0), "+0000");
  EXPECT_EQ(fmt::format("{: 05}", 0), " 0000");
  EXPECT_EQ(fmt::format("{:5}", 0), "    0");
  EXPECT_EQ(fmt::format("{:+5}", 0), "   +0");
}

TEST(FmtFormatSignAlignmentTest, NegativeZeroFormattingWithFloatingPointUsesSameSignPlacementRules) {
  EXPECT_EQ(fmt::format("{:08.1f}", -0.0), "-00000.0");
  EXPECT_EQ(fmt::format("{:8.1f}", -0.0), "    -0.0");
}

TEST(FmtFormatSignAlignmentTest, ExplicitNumericAlignmentForFloatingPointPlacesSignBeforePadding) {
  EXPECT_EQ(fmt::format("{:0=8.2f}", -1.5), "-0001.50");
  EXPECT_EQ(fmt::format("{:0=+8.2f}", 1.5), "+0001.50");
}

TEST(FmtFormatSignAlignmentTest, SignAwarePaddingWithNonNumericAlignmentCountsSignInWidth) {
  EXPECT_EQ(fmt::format("{:_<6}", -12), "-12___");
  EXPECT_EQ(fmt::format("{:_^6}", -12), "_-12__");
  EXPECT_EQ(fmt::format("{:_<+6}", 12), "+12___");
  EXPECT_EQ(fmt::format("{:_^+6}", 12), "_+12__");
}

TEST(FmtFormatSignAlignmentTest, InvalidFormatSpecifiersReportErrors) {
  EXPECT_THROW((void)fmt::format("{:0=}", "x"), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{:+}", "x"), fmt::format_error);
  EXPECT_THROW((void)fmt::format("{: }", "x"), fmt::format_error);
}

}  // namespace
```

---

## id 45 - `fmtlib___fmt@fc6e0fe992` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@fc6e0fe992/deepseek-chat/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test that numeric alignment with sign is handled correctly for integers
TEST(NumericAlignmentTest, IntegerWithSign) {
    // Positive number with '+' sign and numeric alignment
    std::string result = fmt::format("{:+06d}", 42);
    EXPECT_EQ(result, "+00042");
    
    // Negative number with numeric alignment
    result = fmt::format("{:06d}", -42);
    EXPECT_EQ(result, "-00042");
    
    // Positive number with space sign and numeric alignment
    result = fmt::format("{: 06d}", 42);
    EXPECT_EQ(result, " 00042");
}

// Test that numeric alignment with sign and width adjustment works
TEST(NumericAlignmentTest, SignWithWidthAdjustment) {
    // Sign should be placed before padding, width should be reduced by 1
    std::string result = fmt::format("{:+06d}", 7);
    EXPECT_EQ(result, "+00007");
    EXPECT_EQ(result.size(), 6u);
    
    // Zero width with sign
    result = fmt::format("{:+0d}", 7);
    EXPECT_EQ(result, "+7");
}

// Test default alignment (ALIGN_DEFAULT) behavior for non-numeric types
TEST(NumericAlignmentTest, DefaultAlignmentNonNumeric) {
    // Default alignment for strings should be left-aligned
    std::string result = fmt::format("{:10}", "hello");
    EXPECT_EQ(result, "hello     ");
    
    // Default alignment for integers should be right-aligned
    result = fmt::format("{:10}", 42);
    EXPECT_EQ(result, "        42");
}

// Test that ALIGN_NUMERIC is converted to ALIGN_RIGHT when no sign is present
TEST(NumericAlignmentTest, NumericToRightWithoutSign) {
    // Zero-padded integer without sign should be right-aligned
    std::string result = fmt::format("{:010d}", 123);
    EXPECT_EQ(result, "0000000123");
    
    // Zero-padded float without sign
    result = fmt::format("{:010.2f}", 3.14);
    EXPECT_EQ(result, "0000003.14");
}

// Test sign handling with width and numeric alignment
TEST(NumericAlignmentTest, SignWithWidthAndNumeric) {
    // Sign should be placed before the number, not before padding
    std::string result = fmt::format("{:+08d}", 42);
    EXPECT_EQ(result, "+0000042");
    EXPECT_EQ(result[0], '+');
    
    // Negative number with width
    result = fmt::format("{:08d}", -42);
    EXPECT_EQ(result, "-0000042");
    EXPECT_EQ(result[0], '-');
}

// Test edge case: zero value with sign and numeric alignment
TEST(NumericAlignmentTest, ZeroWithSign) {
    std::string result = fmt::format("{:+05d}", 0);
    EXPECT_EQ(result, "+0000");
    
    result = fmt::format("{:05d}", 0);
    EXPECT_EQ(result, "00000");
}

// Test edge case: maximum width with sign
TEST(NumericAlignmentTest, LargeWidthWithSign) {
    std::string result = fmt::format("{:+020d}", 42);
    EXPECT_EQ(result, "+00000000000000000042");
    EXPECT_EQ(result.size(), 20u);
}

// Test that ALIGN_DEFAULT for numeric types becomes ALIGN_RIGHT
TEST(NumericAlignmentTest, DefaultNumericBecomesRight) {
    // Default alignment for integers should be right-aligned
    std::string result = fmt::format("{:10d}", 42);
    EXPECT_EQ(result, "        42");
    
    // Default alignment for floats should be right-aligned
    result = fmt::format("{:10.2f}", 3.14);
    EXPECT_EQ(result, "      3.14");
}

// Test sign handling with no width specified
TEST(NumericAlignmentTest, SignWithoutWidth) {
    std::string result = fmt::format("{:+d}", 42);
    EXPECT_EQ(result, "+42");
    
    result = fmt::format("{: d}", 42);
    EXPECT_EQ(result, " 42");
    
    result = fmt::format("{:+d}", -42);
    EXPECT_EQ(result, "-42");
}

// Test that sign is not duplicated when width is exactly the number length
TEST(NumericAlignmentTest, SignExactWidth) {
    std::string result = fmt::format("{:+3d}", 42);
    EXPECT_EQ(result, "+42");
    EXPECT_EQ(result.size(), 3u);
    
    result = fmt::format("{:+4d}", 42);
    EXPECT_EQ(result, " +42");
    EXPECT_EQ(result.size(), 4u);
}

// Test multiple signs in format string (should use last one)
TEST(NumericAlignmentTest, MultipleSigns) {
    std::string result = fmt::format("{:+-06d}", 42);
    EXPECT_EQ(result, "+00042");
    
    result = fmt::format("{:-+06d}", 42);
    EXPECT_EQ(result, "+00042");
}

// Test that ALIGN_NUMERIC with sign works for different numeric types
TEST(NumericAlignmentTest, NumericAlignmentDifferentTypes) {
    // Integer
    std::string result = fmt::format("{:+08d}", 123);
    EXPECT_EQ(result, "+0000123");
    
    // Float
    result = fmt::format("{:+08.2f}", 3.14);
    EXPECT_EQ(result, "+0003.14");
    
    // Double
    result = fmt::format("{:+08.2f}", 3.14159);
    EXPECT_EQ(result, "+0003.14");
}

// Test edge case: minimum width with sign
TEST(NumericAlignmentTest, MinimumWidthWithSign) {
    std::string result = fmt::format("{:+1d}", 42);
    EXPECT_EQ(result, "+42");
    EXPECT_EQ(result.size(), 3u);
    
    result = fmt::format("{:+2d}", 42);
    EXPECT_EQ(result, "+42");
    EXPECT_EQ(result.size(), 3u);
}

// Test that sign is correctly placed when using fill character
TEST(NumericAlignmentTest, SignWithFillCharacter) {
    std::string result = fmt::format("{:*>+06d}", 42);
    EXPECT_EQ(result, "**+042");
    
    result = fmt::format("{:*<+06d}", 42);
    EXPECT_EQ(result, "+042**");
}
```

---

## id 46 - `fmtlib___fmt@fc6e0fe992` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@fc6e0fe992/deepseek-chat/buggy/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <vector>

// Test that numeric alignment with sign is handled correctly
TEST(AlignNumericTest, SignAndNumericAlignment) {
    // This exercises the case where spec.align() == ALIGN_NUMERIC and sign is present
    // The sign should be prepended and width decremented, alignment changed to right
    std::string result = fmt::format("{:=+8}", 42);
    EXPECT_EQ(result, "+     42");
    
    // Without sign, numeric alignment should still work
    result = fmt::format("{:=8}", 42);
    EXPECT_EQ(result, "42      ");
}

// Test that numeric alignment with sign and zero width works
TEST(AlignNumericTest, SignAndNumericAlignmentZeroWidth) {
    // When width is 0, sign should still be prepended but width decrement has no effect
    std::string result = fmt::format("{:=+0}", 42);
    EXPECT_EQ(result, "+42");
}

// Test that numeric alignment with sign and very large width
TEST(AlignNumericTest, SignAndNumericAlignmentLargeWidth) {
    std::string result = fmt::format("{:=+20}", 42);
    EXPECT_EQ(result, "+                  42");
}

// Test that numeric alignment with negative sign
TEST(AlignNumericTest, SignAndNumericAlignmentNegative) {
    std::string result = fmt::format("{:=+8}", -42);
    EXPECT_EQ(result, "-      42");
}

// Test default alignment (ALIGN_DEFAULT) without sign
TEST(AlignDefaultTest, NoSignDefaultAlignment) {
    // When spec.align() == ALIGN_DEFAULT and no sign, alignment should become ALIGN_RIGHT
    std::string result = fmt::format("{:8}", 42);
    EXPECT_EQ(result, "      42");
}

// Test default alignment with sign
TEST(AlignDefaultTest, SignDefaultAlignment) {
    // When spec.align() == ALIGN_DEFAULT and sign is present, n should be incremented
    std::string result = fmt::format("{:+8}", 42);
    EXPECT_EQ(result, "     +42");
}

// Test default alignment with negative sign
TEST(AlignDefaultTest, NegativeSignDefaultAlignment) {
    std::string result = fmt::format("{:8}", -42);
    EXPECT_EQ(result, "     -42");
}

// Test explicit right alignment without sign
TEST(AlignRightTest, NoSignRightAlignment) {
    // When spec.align() == ALIGN_RIGHT and no sign, no changes should occur
    std::string result = fmt::format("{:>8}", 42);
    EXPECT_EQ(result, "      42");
}

// Test explicit right alignment with sign
TEST(AlignRightTest, SignRightAlignment) {
    // When spec.align() == ALIGN_RIGHT and sign is present, n should be incremented
    std::string result = fmt::format("{:+>8}", 42);
    EXPECT_EQ(result, "     +42");
}

// Test explicit left alignment (should not be affected by the code)
TEST(AlignLeftTest, LeftAlignment) {
    // Left alignment should remain unchanged
    std::string result = fmt::format("{:<8}", 42);
    EXPECT_EQ(result, "42      ");
    
    result = fmt::format("{:+<8}", 42);
    EXPECT_EQ(result, "+42     ");
}

// Test center alignment (should not be affected by the code)
TEST(AlignCenterTest, CenterAlignment) {
    // Center alignment should remain unchanged
    std::string result = fmt::format("{:^8}", 42);
    EXPECT_EQ(result, "   42   ");
    
    result = fmt::format("{:+^8}", 42);
    EXPECT_EQ(result, " +42    ");
}

// Test with zero padding (which uses numeric alignment internally)
TEST(ZeroPaddingTest, ZeroPaddingWithSign) {
    // Zero padding uses ALIGN_NUMERIC internally
    std::string result = fmt::format("{:+08}", 42);
    EXPECT_EQ(result, "+0000042");
}

// Test with zero padding and negative number
TEST(ZeroPaddingTest, ZeroPaddingNegative) {
    std::string result = fmt::format("{:08}", -42);
    EXPECT_EQ(result, "-0000042");
}

// Test with zero padding and zero width
TEST(ZeroPaddingTest, ZeroPaddingZeroWidth) {
    std::string result = fmt::format("{:+0}", 42);
    EXPECT_EQ(result, "+42");
}

// Test edge case: zero value with various alignments
TEST(EdgeCaseTest, ZeroValue) {
    EXPECT_EQ(fmt::format("{:=+8}", 0), "+      0");
    EXPECT_EQ(fmt::format("{:8}", 0), "       0");
    EXPECT_EQ(fmt::format("{:+8}", 0), "      +0");
    EXPECT_EQ(fmt::format("{:08}", 0), "00000000");
}

// Test edge case: maximum integer values
TEST(EdgeCaseTest, MaxValues) {
    EXPECT_EQ(fmt::format("{:=+20}", 2147483647), "+          2147483647");
    EXPECT_EQ(fmt::format("{:20}", -2147483647), "         -2147483647");
}

// Test edge case: very small width (1)
TEST(EdgeCaseTest, MinimumWidth) {
    EXPECT_EQ(fmt::format("{:=+1}", 42), "+42");
    EXPECT_EQ(fmt::format("{:1}", 42), "42");
    EXPECT_EQ(fmt::format("{:+1}", 42), "+42");
}

// Test that the function handles the case where sign is present but width is exhausted
TEST(EdgeCaseTest, SignExhaustsWidth) {
    // When sign takes the only available space
    EXPECT_EQ(fmt::format("{:=+1}", 5), "+5");
    EXPECT_EQ(fmt::format("{:+1}", 5), "+5");
}

// Test with floating point numbers (which also go through this code path)
TEST(FloatingPointTest, FloatWithNumericAlignment) {
    EXPECT_EQ(fmt::format("{:=+10.2f}", 3.14), "+      3.14");
    EXPECT_EQ(fmt::format("{:10.2f}", 3.14), "      3.14");
    EXPECT_EQ(fmt::format("{:+10.2f}", 3.14), "     +3.14");
}

// Test with floating point negative numbers
TEST(FloatingPointTest, NegativeFloat) {
    EXPECT_EQ(fmt::format("{:=+10.2f}", -3.14), "-      3.14");
    EXPECT_EQ(fmt::format("{:10.2f}", -3.14), "     -3.14");
}

// Test with zero padding for floats
TEST(FloatingPointTest, FloatZeroPadding) {
    EXPECT_EQ(fmt::format("{:+010.2f}", 3.14), "+000003.14");
    EXPECT_EQ(fmt::format("{:010.2f}", -3.14), "-000003.14");
}
```

---

## id 47 - `CLIUtils___CLI11@dcbcb4721d` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/CLIUtils___CLI11@dcbcb4721d/deepseek-chat/buggy/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include <string>
#include <sstream>
#include <vector>

// Test fixture for Config file generation with descriptions
class ConfigDescriptionTest : public ::testing::Test {
protected:
    CLI::App app{"Test app"};
    std::stringstream output;
    std::string config_str;

    void SetUp() override {
        // app is already constructed with name
    }

    // Helper to generate config output
    void generateConfig(bool write_description = true) {
        config_str = app.config_to_str(true, write_description);
        output.str(config_str);
    }
};

// Test: Option with description generates description comment
TEST_F(ConfigDescriptionTest, OptionWithDescription) {
    auto* opt = app.add_option("--name", "The name of the user");
    opt->default_str("default_name");
    
    generateConfig(true);
    
    // Should contain the description as a comment
    EXPECT_NE(config_str.find("; The name of the user"), std::string::npos);
    // Should contain the option value
    EXPECT_NE(config_str.find("name=default_name"), std::string::npos);
}

// Test: Option without description does not generate description comment when write_description is true
TEST_F(ConfigDescriptionTest, OptionWithoutDescription) {
    auto* opt = app.add_option("--name");
    opt->default_str("default_name");
    
    generateConfig(true);
    
    // Should NOT contain any description comment
    EXPECT_EQ(config_str.find("; "), std::string::npos);
    // Should still contain the option
    EXPECT_NE(config_str.find("name=default_name"), std::string::npos);
}

// Test: write_description=false suppresses description comments
TEST_F(ConfigDescriptionTest, SuppressDescription) {
    auto* opt = app.add_option("--name", "The name of the user");
    opt->default_str("default_name");
    
    generateConfig(false);
    
    // Should NOT contain description comment
    EXPECT_EQ(config_str.find("; The name of the user"), std::string::npos);
    // Should still contain the option
    EXPECT_NE(config_str.find("name=default_name"), std::string::npos);
}

// Test: Option with empty value does not generate description (edge case from focal code)
TEST_F(ConfigDescriptionTest, EmptyValueNoDescription) {
    auto* opt = app.add_option("--name", "The name of the user");
    // No default value set - value will be empty
    
    generateConfig(true);
    
    // Should NOT contain description comment because value is empty
    EXPECT_EQ(config_str.find("; The name of the user"), std::string::npos);
    // Should not contain the option at all
    EXPECT_EQ(config_str.find("name="), std::string::npos);
}

// Test: Option with multi-line description gets proper formatting
TEST_F(ConfigDescriptionTest, MultiLineDescription) {
    auto* opt = app.add_option("--name", "Line one\nLine two\nLine three");
    opt->default_str("default_name");
    
    generateConfig(true);
    
    // Should contain the first line of description
    EXPECT_NE(config_str.find("; Line one"), std::string::npos);
    // Should contain subsequent lines with proper indentation
    EXPECT_NE(config_str.find("; Line two"), std::string::npos);
    EXPECT_NE(config_str.find("; Line three"), std::string::npos);
}

// Test: Multiple options with descriptions all get comments
TEST_F(ConfigDescriptionTest, MultipleOptionsWithDescriptions) {
    app.add_option("--name", "User name")->default_str("Alice");
    app.add_option("--age", "User age")->default_str("30");
    app.add_option("--active", "Is active")->default_str("true");
    
    generateConfig(true);
    
    // All descriptions should be present
    EXPECT_NE(config_str.find("; User name"), std::string::npos);
    EXPECT_NE(config_str.find("; User age"), std::string::npos);
    EXPECT_NE(config_str.find("; Is active"), std::string::npos);
}

// Test: Option with description but no default value (empty value) - edge case
TEST_F(ConfigDescriptionTest, OptionWithDescriptionNoDefault) {
    app.add_option("--name", "The name of the user");
    // No default set, so value is empty
    
    generateConfig(true);
    
    // Should NOT contain description because value is empty (focal code check)
    EXPECT_EQ(config_str.find("; The name of the user"), std::string::npos);
    EXPECT_EQ(config_str.find("name="), std::string::npos);
}

// Test: Flag options (boolean) with descriptions
TEST_F(ConfigDescriptionTest, FlagWithDescription) {
    app.add_flag("--verbose", "Enable verbose output");
    
    generateConfig(true);
    
    // Flags typically have value "true" or "false", so description should appear
    EXPECT_NE(config_str.find("; Enable verbose output"), std::string::npos);
    EXPECT_NE(config_str.find("verbose="), std::string::npos);
}

// Test: Option with description containing special characters
TEST_F(ConfigDescriptionTest, DescriptionWithSpecialCharacters) {
    app.add_option("--path", "Path to file (e.g., /usr/local/bin)")->default_str("/tmp");
    
    generateConfig(true);
    
    // Description with parentheses and slashes should be preserved
    EXPECT_NE(config_str.find("; Path to file (e.g., /usr/local/bin)"), std::string::npos);
}

// Test: Empty description string
TEST_F(ConfigDescriptionTest, EmptyDescriptionString) {
    app.add_option("--name", "")->default_str("value");
    
    generateConfig(true);
    
    // Should not generate a comment line for empty description
    // But should still output the option
    EXPECT_NE(config_str.find("name=value"), std::string::npos);
    // No comment line should be present (no non-empty description)
    // The only comment would be the header comment from CLI11
    // Check that there's no additional comment line with just "; "
    std::string line;
    int comment_lines = 0;
    while (std::getline(output, line)) {
        if (line.find("; ") == 0 && line.size() > 2) {
            comment_lines++;
        }
    }
    // Only header comment(s) should exist, not a description comment
    EXPECT_GE(comment_lines, 0); // At least header comments
}

// Test: Option with description containing newlines and write_description=false
TEST_F(ConfigDescriptionTest, MultiLineDescriptionSuppressed) {
    app.add_option("--name", "Line one\nLine two")->default_str("value");
    
    generateConfig(false);
    
    // No description should appear
    EXPECT_EQ(config_str.find("; Line one"), std::string::npos);
    EXPECT_EQ(config_str.find("; Line two"), std::string::npos);
    // Option should still be present
    EXPECT_NE(config_str.find("name=value"), std::string::npos);
}
```

---

## id 48 - `CLIUtils___CLI11@dcbcb4721d` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/CLIUtils___CLI11@dcbcb4721d/deepseek-chat/buggy/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include "CLI/CLI.hpp"
#include <string>
#include <sstream>
#include <vector>

// Test fixture for config output tests
class ConfigOutputTest : public ::testing::Test {
protected:
    CLI::App app;
    std::stringstream output_stream;
    std::string comment_lead = "; ";

    void SetUp() override {
        app.footer("test footer");
    }
};

// Test that a simple option with description writes the description in config output
TEST_F(ConfigOutputTest, OptionWithDescription) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "This is a description");
    
    // Generate config output
    app.config_to_str(true, false);
    std::string output = app.config_to_str(true, false);
    
    // Check that description appears after the option value
    EXPECT_NE(output.find("This is a description"), std::string::npos);
    EXPECT_NE(output.find("; This is a description"), std::string::npos);
}

// Test that an option without description does NOT write description in config output
TEST_F(ConfigOutputTest, OptionWithoutDescription) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value);
    
    std::string output = app.config_to_str(false, false);
    
    // Should not contain any description line
    EXPECT_EQ(output.find("; "), std::string::npos);
}

// Test that write_description=false suppresses description output
TEST_F(ConfigOutputTest, WriteDescriptionFalse) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "Should not appear");
    
    std::string output = app.config_to_str(false, false);
    
    // Description should not be present when write_description is false
    EXPECT_EQ(output.find("Should not appear"), std::string::npos);
}

// Test that write_description=true includes description output
TEST_F(ConfigOutputTest, WriteDescriptionTrue) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "Should appear");
    
    std::string output = app.config_to_str(true, false);
    
    // Description should be present when write_description is true
    EXPECT_NE(output.find("Should appear"), std::string::npos);
}

// Test empty value with description - description should still be written
TEST_F(ConfigOutputTest, EmptyValueWithDescription) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "Description for empty");
    opt->default_str("");  // Force empty default
    
    std::string output = app.config_to_str(true, false);
    
    // Description should appear even with empty value
    EXPECT_NE(output.find("Description for empty"), std::string::npos);
}

// Test that description with newlines is properly formatted
TEST_F(ConfigOutputTest, DescriptionWithNewlines) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "Line1\nLine2\nLine3");
    
    std::string output = app.config_to_str(true, false);
    
    // Each line should be prefixed with comment lead
    EXPECT_NE(output.find("; Line1"), std::string::npos);
    EXPECT_NE(output.find("; Line2"), std::string::npos);
    EXPECT_NE(output.find("; Line3"), std::string::npos);
}

// Test that description with leading/trailing whitespace is preserved
TEST_F(ConfigOutputTest, DescriptionWithWhitespace) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "  spaced description  ");
    
    std::string output = app.config_to_str(true, false);
    
    // Whitespace should be preserved in the description
    EXPECT_NE(output.find("  spaced description  "), std::string::npos);
}

// Test multiple options with descriptions
TEST_F(ConfigOutputTest, MultipleOptionsWithDescriptions) {
    std::string opt1, opt2;
    app.add_option("--opt1", opt1, "First description");
    app.add_option("--opt2", opt2, "Second description");
    
    std::string output = app.config_to_str(true, false);
    
    // Both descriptions should appear
    EXPECT_NE(output.find("First description"), std::string::npos);
    EXPECT_NE(output.find("Second description"), std::string::npos);
}

// Test that description is written after the option value line
TEST_F(ConfigOutputTest, DescriptionAfterValue) {
    std::string opt_value = "test_value";
    auto* opt = app.add_option("--my-option", opt_value, "Description text");
    
    std::string output = app.config_to_str(true, false);
    
    // Find positions to verify order
    size_t value_pos = output.find("test_value");
    size_t desc_pos = output.find("Description text");
    
    EXPECT_NE(value_pos, std::string::npos);
    EXPECT_NE(desc_pos, std::string::npos);
    EXPECT_LT(value_pos, desc_pos);  // Description should come after value
}

// Test that description with special characters is handled
TEST_F(ConfigOutputTest, DescriptionWithSpecialChars) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "Description with #@$%^&*()");
    
    std::string output = app.config_to_str(true, false);
    
    // Special characters should be preserved
    EXPECT_NE(output.find("Description with #@$%^&*()"), std::string::npos);
}

// Test that empty description string is handled (no description written)
TEST_F(ConfigOutputTest, EmptyDescriptionString) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "");
    
    std::string output = app.config_to_str(true, false);
    
    // Empty description should not produce a comment line
    EXPECT_EQ(output.find("; \n"), std::string::npos);
}

// Test that description with only whitespace is handled
TEST_F(ConfigOutputTest, WhitespaceOnlyDescription) {
    std::string opt_value;
    auto* opt = app.add_option("--my-option", opt_value, "   ");
    
    std::string output = app.config_to_str(true, false);
    
    // Whitespace-only description should not produce a comment line
    EXPECT_EQ(output.find(";    \n"), std::string::npos);
}
```

---

## id 49 - `fmtlib___fmt@c04fb91b03` - model gemini-2.5-flash

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@c04fb91b03/gemini-2.5-flash/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include "fmt/format.h"

// Helper function to format an enum value using fmt::format
template <typename EnumType>
std::string format_enum(EnumType value) {
  return fmt::format("{}", value);
}

// Define a simple enum for testing
enum class MyEnum {
  Value1,
  Value2,
  Value3 = 100,
  NegativeValue = -1,
  ZeroValue = 0
};

// Make MyEnum format-able by fmtlib.
// This is the mechanism that the focal function relies on.
template <>
struct fmt::formatter<MyEnum> : fmt::formatter<int> {
  template <typename FormatContext>
  auto format(MyEnum e, FormatContext& ctx) {
    return fmt::formatter<int>::format(static_cast<int>(e), ctx);
  }
};

TEST(EnumFormatterTest, BasicEnumFormatting) {
  // Test basic formatting of enum values
  EXPECT_EQ(format_enum(MyEnum::Value1), "0");
  EXPECT_EQ(format_enum(MyEnum::Value2), "1");
  EXPECT_EQ(format_enum(MyEnum::Value3), "100");
}

TEST(EnumFormatterTest, NegativeEnumValueFormatting) {
  // Test formatting of a negative enum value
  EXPECT_EQ(format_enum(MyEnum::NegativeValue), "-1");
}

TEST(EnumFormatterTest, ZeroEnumValueFormatting) {
  // Test formatting of a zero enum value
  EXPECT_EQ(format_enum(MyEnum::ZeroValue), "0");
}

TEST(EnumFormatterTest, EnumWithFormatSpecifiers) {
  // Test enum formatting with various format specifiers
  EXPECT_EQ(fmt::format("{:d}", MyEnum::Value1), "0");
  EXPECT_EQ(fmt::format("{:x}", MyEnum::Value3), "64"); // 100 in hex
  EXPECT_EQ(fmt::format("{:X}", MyEnum::Value3), "64");
  EXPECT_EQ(fmt::format("{:o}", MyEnum::Value3), "144"); // 100 in octal
  EXPECT_EQ(fmt::format("{:b}", MyEnum::Value3), "1100100"); // 100 in binary
  EXPECT_EQ(fmt::format("{:#x}", MyEnum::Value3), "0x64");
  EXPECT_EQ(fmt::format("{:5d}", MyEnum::Value2), "    1");
  EXPECT_EQ(fmt::format("{:05d}", MyEnum::Value2), "00001");
  EXPECT_EQ(fmt::format("{: d}", MyEnum::Value2), " 1");
  EXPECT_EQ(fmt::format("{:+d}", MyEnum::Value2), "+1");
  EXPECT_EQ(fmt::format("{:+d}", MyEnum::NegativeValue), "-1");
}

// Define another enum to ensure type-specific behavior
enum class AnotherEnum {
  A = 10,
  B = 20
};

template <>
struct fmt::formatter<AnotherEnum> : fmt::formatter<int> {
  template <typename FormatContext>
  auto format(AnotherEnum e, FormatContext& ctx) {
    return fmt::formatter<int>::format(static_cast<int>(e), ctx);
  }
};

TEST(EnumFormatterTest, MultipleEnumTypes) {
  // Test that different enum types are handled correctly
  EXPECT_EQ(format_enum(AnotherEnum::A), "10");
  EXPECT_EQ(format_enum(AnotherEnum::B), "20");
  EXPECT_EQ(format_enum(MyEnum::Value1), "0"); // Ensure MyEnum still works
}

// Test with a scoped enum that has explicit underlying type
enum class LongEnum : long {
  LargeValue = 2147483647L, // Max int
  VeryLargeValue = 2147483648L // Exceeds max int
};

template <>
struct fmt::formatter<LongEnum> : fmt::formatter<long> {
  template <typename FormatContext>
  auto format(LongEnum e, FormatContext& ctx) {
    return fmt::formatter<long>::format(static_cast<long>(e), ctx);
  }
};

TEST(EnumFormatterTest, LongEnumFormatting) {
  EXPECT_EQ(format_enum(LongEnum::LargeValue), "2147483647");
  EXPECT_EQ(format_enum(LongEnum::VeryLargeValue), "2147483648");
  EXPECT_EQ(fmt::format("{:L}", LongEnum::VeryLargeValue), "2,147,483,648"); // Locale-specific formatting
}

// Test with an unscoped enum
enum UnscopedEnum {
  UnscopedVal1 = 5,
  UnscopedVal2 = 6
};

template <>
struct fmt::formatter<UnscopedEnum> : fmt::formatter<int> {
  template <typename FormatContext>
  auto format(UnscopedEnum e, FormatContext& ctx) {
    return fmt::formatter<int>::format(static_cast<int>(e), ctx);
  }
};

TEST(EnumFormatterTest, UnscopedEnumFormatting) {
  EXPECT_EQ(format_enum(UnscopedEnum::UnscopedVal1), "5");
  EXPECT_EQ(format_enum(UnscopedEnum::UnscopedVal2), "6");
}

// Test case for an enum that might not have a direct integer mapping
// (though C++ enums always do, this tests the general case of the formatter
// delegating to another formatter type).
enum class CharEnum : char {
  CharA = 'A',
  CharB = 'B'
};

template <>
struct fmt::formatter<CharEnum> : fmt::formatter<char> {
  template <typename FormatContext>
  auto format(CharEnum e, FormatContext& ctx) {
    return fmt::formatter<char>::format(static_cast<char>(e), ctx);
  }
};

TEST(EnumFormatterTest, CharEnumFormatting) {
  // When formatted as char, it should print the character
  EXPECT_EQ(fmt::format("{}", CharEnum::CharA), "A");
  EXPECT_EQ(fmt::format("{}", CharEnum::CharB), "B");
  // When formatted as int, it should print the integer value
  EXPECT_EQ(fmt::format("{:d}", CharEnum::CharA), "65");
}

// Test with a custom underlying type for the enum
enum class CustomUnderlyingEnum : unsigned short {
    UShortVal = 65535
};

template <>
struct fmt::formatter<CustomUnderlyingEnum> : fmt::formatter<unsigned short> {
    template <typename FormatContext>
    auto format(CustomUnderlyingEnum e, FormatContext& ctx) {
        return fmt::formatter<unsigned short>::format(static_cast<unsigned short>(e), ctx);
    }
};

TEST(EnumFormatterTest, CustomUnderlyingTypeEnumFormatting) {
    EXPECT_EQ(format_enum(CustomUnderlyingEnum::UShortVal), "65535");
    EXPECT_EQ(fmt::format("{:x}", CustomUnderlyingEnum::UShortVal), "ffff");
}
```

---

## id 50 - `fmtlib___fmt@287eaab3b2` - model gpt-5.4-or

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@287eaab3b2/gpt-5.4-or/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>

#include <limits>
#include <string>
#include <type_traits>

#include <fmt/format.h>

namespace {

TEST(FmtWriteCharTest, FormatsSingleChar) {
  EXPECT_EQ(fmt::format("{}", 'A'), "A");
  EXPECT_EQ(fmt::format("{}", 'z'), "z");
  EXPECT_EQ(fmt::format("{}", '0'), "0");
}

TEST(FmtWriteCharTest, FormatsNullCharAndPreservesLength) {
  std::string s = fmt::format("{}{}{}", 'A', '\0', 'B');

  ASSERT_EQ(s.size(), 3u);
  EXPECT_EQ(s[0], 'A');
  EXPECT_EQ(s[1], '\0');
  EXPECT_EQ(s[2], 'B');
}

TEST(FmtWriteCharTest, FormatsControlCharacter) {
  std::string s = fmt::format("{}{}", '\n', 'X');

  ASSERT_EQ(s.size(), 2u);
  EXPECT_EQ(s[0], '\n');
  EXPECT_EQ(s[1], 'X');
}

TEST(FmtWriteCharTest, AppliesAlignmentAndWidthToChar) {
  EXPECT_EQ(fmt::format("{:>3}", 'Q'), "  Q");
  EXPECT_EQ(fmt::format("{:<3}", 'Q'), "Q  ");
  EXPECT_EQ(fmt::format("{:^3}", 'Q'), " Q ");
}

TEST(FmtWriteCharTest, AppliesCustomFillToChar) {
  EXPECT_EQ(fmt::format("{:*^5}", 'X'), "**X**");
  EXPECT_EQ(fmt::format("{:#>4}", 'X'), "###X");
  EXPECT_EQ(fmt::format("{:.<4}", 'X'), "X...");
}

TEST(FmtWriteCharTest, WidthZeroOrOneDoesNotAddUnexpectedCharacters) {
  EXPECT_EQ(fmt::format("{:1}", 'M'), "M");
  EXPECT_EQ(fmt::format("{}", 'M'), "M");
}

TEST(FmtWriteLongDoubleTest, FormatsZero) {
  EXPECT_EQ(fmt::format("{}", static_cast<long double>(0.0L)), "0");
  EXPECT_EQ(fmt::format("{:.0f}", static_cast<long double>(0.0L)), "0");
}

TEST(FmtWriteLongDoubleTest, FormatsNegativeZeroWithSign) {
  long double neg_zero = -0.0L;
  EXPECT_EQ(fmt::format("{:+}", neg_zero), "-0");
}

TEST(FmtWriteLongDoubleTest, FormatsFiniteValuesWithDefaultAndFixedPrecision) {
  EXPECT_EQ(fmt::format("{:.1f}", static_cast<long double>(1.25L)), "1.2");
  EXPECT_EQ(fmt::format("{:.2f}", static_cast<long double>(1.25L)), "1.25");
  EXPECT_EQ(fmt::format("{:.3f}", static_cast<long double>(-12.5L)), "-12.500");
}

TEST(FmtWriteLongDoubleTest, FormatsSpecialValues) {
  const long double inf = std::numeric_limits<long double>::infinity();
  const long double nan = std::numeric_limits<long double>::quiet_NaN();

  EXPECT_EQ(fmt::format("{}", inf), "inf");
  EXPECT_EQ(fmt::format("{:+}", inf), "+inf");
  EXPECT_EQ(fmt::format("{}", -inf), "-inf");

  std::string nan_text = fmt::format("{}", nan);
  EXPECT_FALSE(nan_text.empty());
  EXPECT_NE(nan_text.find("nan"), std::string::npos);
}

TEST(FmtWriteLongDoubleTest, AppliesWidthAlignmentAndFill) {
  EXPECT_EQ(fmt::format("{:>8.2f}", static_cast<long double>(1.5L)), "    1.50");
  EXPECT_EQ(fmt::format("{:*<8.1f}", static_cast<long double>(2.0L)), "2.0*****");
  EXPECT_EQ(fmt::format("{:^7.0f}", static_cast<long double>(42.0L)), "  42   ");
}

TEST(FmtWriteLongDoubleTest, UsesLongDoubleOverloadInFormatArgStore) {
  long double value = 3.5L;
  EXPECT_EQ(fmt::format("value={:.1f}", value), "value=3.5");
}

#if defined(FMT_USE_WCHAR) || !defined(_WIN32) || defined(_WIN32)
TEST(FmtWriteWCharTest, FormatsSingleWideCharWithWformat) {
  std::wstring s = fmt::format(L"{}", L'Å');
  ASSERT_EQ(s.size(), 1u);
  EXPECT_EQ(s[0], L'Å');
}

TEST(FmtWriteWCharTest, FormatsNullWideCharAndPreservesLength) {
  std::wstring s = fmt::format(L"{}{}{}", L'A', L'\0', L'B');

  ASSERT_EQ(s.size(), 3u);
  EXPECT_EQ(s[0], L'A');
  EXPECT_EQ(s[1], L'\0');
  EXPECT_EQ(s[2], L'B');
}

TEST(FmtWriteWCharTest, AppliesAlignmentAndFillToWideChar) {
  EXPECT_EQ(fmt::format(L"{:>3}", L'Q'), std::wstring(L"  Q"));
  EXPECT_EQ(fmt::format(L"{:*^5}", L'X'), std::wstring(L"**X**"));
}
#endif

}  // namespace
```

---

## id 51 - `fmtlib___fmt@287eaab3b2` - model gpt-5.4-or

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@287eaab3b2/gpt-5.4-or/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>

#include <limits>
#include <string>
#include <type_traits>

#include <fmt/format.h>

TEST(FmtFormatWriteCharTest, FormatsSingleCharWithFormat) {
  EXPECT_EQ(fmt::format("{}", 'A'), "A");
  EXPECT_EQ(fmt::format("prefix{}suffix", 'Z'), "prefixZsuffix");
}

TEST(FmtFormatWriteCharTest, FormatsNullCharPreservingLength) {
  std::string result = fmt::format("{}{}", '\0', 'X');
  ASSERT_EQ(result.size(), 2u);
  EXPECT_EQ(result[0], '\0');
  EXPECT_EQ(result[1], 'X');
}

TEST(FmtFormatWriteCharTest, FormatsBoundaryCharValues) {
  std::string zero = fmt::format("{}", static_cast<char>(0));
  ASSERT_EQ(zero.size(), 1u);
  EXPECT_EQ(static_cast<unsigned char>(zero[0]), 0u);

  std::string maxc = fmt::format("{}", std::numeric_limits<unsigned char>::max());
  EXPECT_EQ(maxc, "255");
}

TEST(FmtFormatWriteCharTest, WritesCharIntoMemoryBuffer) {
  fmt::memory_buffer buf;
  fmt::format_to(std::back_inserter(buf), "{}", 'q');

  std::string result(buf.data(), buf.size());
  EXPECT_EQ(result, "q");
}

TEST(FmtFormatWriteCharTest, WritesLiteralCharViaFormatToBuffer) {
  fmt::memory_buffer buf;
  fmt::format_to(std::back_inserter(buf), "{}{}{}", 'a', '\0', 'b');

  ASSERT_EQ(buf.size(), 3u);
  EXPECT_EQ(buf[0], 'a');
  EXPECT_EQ(buf[1], '\0');
  EXPECT_EQ(buf[2], 'b');
}

TEST(FmtFormatWriteWCharTest, FormatsSingleWideChar) {
  std::wstring result = fmt::format(L"{}", L'Ω');
  EXPECT_EQ(result, L"Ω");
}

TEST(FmtFormatWriteWCharTest, FormatsWideNullCharPreservingLength) {
  std::wstring result = fmt::format(L"{}{}", L'\0', L'Y');
  ASSERT_EQ(result.size(), 2u);
  EXPECT_EQ(result[0], L'\0');
  EXPECT_EQ(result[1], L'Y');
}

TEST(FmtFormatWriteWCharTest, WritesWideCharIntoWideMemoryBuffer) {
  fmt::basic_memory_buffer<wchar_t> buf;
  fmt::format_to(std::back_inserter(buf), L"{}{}", L'你', L'好');

  std::wstring result(buf.data(), buf.size());
  EXPECT_EQ(result, L"你好");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsZeroLongDouble) {
  long double value = 0.0L;
  EXPECT_EQ(fmt::format("{}", value), "0");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsNegativeLongDouble) {
  long double value = -42.5L;
  std::string s = fmt::format("{}", value);

  EXPECT_FALSE(s.empty());
  EXPECT_NE(s.find("-42.5"), std::string::npos);
}

TEST(FmtFormatWriteLongDoubleTest, HonorsPrecisionForLongDouble) {
  long double value = 1.0L / 3.0L;
  EXPECT_EQ(fmt::format("{:.3f}", value), "0.333");
  EXPECT_EQ(fmt::format("{:.0f}", 2.5L), "2");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsScientificNotationWhenRequested) {
  long double value = 1234.0L;
  std::string s = fmt::format("{:.2Le}", value);

  EXPECT_EQ(s, "1.23e+03");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsInfinityAndNaN) {
  long double inf = std::numeric_limits<long double>::infinity();
  long double nan = std::numeric_limits<long double>::quiet_NaN();

  std::string inf_str = fmt::format("{}", inf);
  std::string ninf_str = fmt::format("{}", -inf);
  std::string nan_str = fmt::format("{}", nan);

  EXPECT_EQ(inf_str, "inf");
  EXPECT_EQ(ninf_str, "-inf");
  EXPECT_EQ(nan_str, "nan");
}

TEST(FmtFormatWriteLongDoubleTest, WritesLongDoubleIntoMemoryBuffer) {
  fmt::memory_buffer buf;
  fmt::format_to(std::back_inserter(buf), "{:.1f}", 7.25L);

  std::string result(buf.data(), buf.size());
  EXPECT_EQ(result, "7.2");
}

TEST(FmtFormatWriteLongDoubleTest, CanAppendLongDoubleAndCharsTogether) {
  fmt::memory_buffer buf;
  fmt::format_to(std::back_inserter(buf), "{}{}{}", '[', 3.5L, ']');

  std::string result(buf.data(), buf.size());
  EXPECT_EQ(result, "[3.5]");
}
```

---

## id 52 - `fmtlib___fmt@287eaab3b2` - model gpt-5.4-or

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@287eaab3b2/gpt-5.4-or/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>

#include <fmt/format.h>
#include <string>
#include <limits>

TEST(FmtFormatWriteCharTest, FormatsSingleCharWithDefaultFormatter) {
  EXPECT_EQ(fmt::format("{}", 'A'), "A");
  EXPECT_EQ(fmt::format("{}", 'z'), "z");
  EXPECT_EQ(fmt::format("{}", '0'), "0");
}

TEST(FmtFormatWriteCharTest, FormatsNullCharAndPreservesLength) {
  std::string result = fmt::format("{}", '\0');
  ASSERT_EQ(result.size(), 1u);
  EXPECT_EQ(result[0], '\0');
}

TEST(FmtFormatWriteCharTest, FormatsControlCharactersAsRawCharacters) {
  std::string newline = fmt::format("{}", '\n');
  ASSERT_EQ(newline.size(), 1u);
  EXPECT_EQ(newline[0], '\n');

  std::string tab = fmt::format("{}", '\t');
  ASSERT_EQ(tab.size(), 1u);
  EXPECT_EQ(tab[0], '\t');
}

TEST(FmtFormatWriteCharTest, SupportsWidthAlignmentAndFill) {
  EXPECT_EQ(fmt::format("{:>3}", 'X'), "  X");
  EXPECT_EQ(fmt::format("{:<3}", 'X'), "X  ");
  EXPECT_EQ(fmt::format("{:^5}", 'X'), "  X  ");
  EXPECT_EQ(fmt::format("{:*^5}", 'X'), "**X**");
}

TEST(FmtFormatWriteCharTest, RepeatedFormattingProducesIndependentCharacters) {
  EXPECT_EQ(fmt::format("{}{}{}", 'a', 'b', 'c'), "abc");
  EXPECT_EQ(fmt::format("[{}][{}]", 'L', 'R'), "[L][R]");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsZeroCorrectly) {
  EXPECT_EQ(fmt::format("{}", static_cast<long double>(0.0L)), "0");
  EXPECT_EQ(fmt::format("{:.1f}", static_cast<long double>(0.0L)), "0.0");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsNegativeZeroWithSign) {
  long double neg_zero = -0.0L;
  EXPECT_EQ(fmt::format("{:+f}", neg_zero), "-0.000000");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsSimpleFiniteValues) {
  EXPECT_EQ(fmt::format("{:.2f}", static_cast<long double>(1.25L)), "1.25");
  EXPECT_EQ(fmt::format("{:.3f}", static_cast<long double>(-2.5L)), "-2.500");
  EXPECT_EQ(fmt::format("{:.0f}", static_cast<long double>(42.0L)), "42");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsLargeLongDoubleWithLongDoublePrecision) {
  long double value = 1.0L / 3.0L;
  std::string result = fmt::format("{:.20Lf}", value);
  EXPECT_EQ(result, "0.33333333333333333334");
}

TEST(FmtFormatWriteLongDoubleTest, SupportsScientificAndGeneralFormats) {
  long double value = 1234.5L;
  EXPECT_EQ(fmt::format("{:.2Le}", value), "1.23e+03");
  EXPECT_EQ(fmt::format("{:.5Lg}", value), "1234.5");
}

TEST(FmtFormatWriteLongDoubleTest, SupportsWidthAlignmentAndSign) {
  EXPECT_EQ(fmt::format("{:>10.1f}", static_cast<long double>(12.0L)), "      12.0");
  EXPECT_EQ(fmt::format("{:<10.1f}", static_cast<long double>(12.0L)), "12.0      ");
  EXPECT_EQ(fmt::format("{:+.1f}", static_cast<long double>(12.0L)), "+12.0");
}

TEST(FmtFormatWriteLongDoubleTest, FormatsInfinityAndNaN) {
  long double inf = std::numeric_limits<long double>::infinity();
  long double nan = std::numeric_limits<long double>::quiet_NaN();

  EXPECT_EQ(fmt::format("{}", inf), "inf");
  EXPECT_EQ(fmt::format("{:+}", inf), "+inf");

  std::string nan_result = fmt::format("{}", nan);
  EXPECT_EQ(nan_result, "nan");
}

TEST(FmtFormatWriteLongDoubleTest, ConcatenatesWithOtherArgumentsCorrectly) {
  std::string result = fmt::format("x={} y={}", static_cast<long double>(1.5L), 'Q');
  EXPECT_EQ(result, "x=1.5 y=Q");
}

TEST(FmtFormatWriteWCharTest, FormatsWideCharacterWithWideStringAPI) {
  std::wstring result = fmt::format(L"{}", L'A');
  EXPECT_EQ(result, L"A");

  std::wstring digit = fmt::format(L"{}", L'7');
  EXPECT_EQ(digit, L"7");
}

TEST(FmtFormatWriteWCharTest, FormatsWideNullCharacterAndPreservesLength) {
  std::wstring result = fmt::format(L"{}", L'\0');
  ASSERT_EQ(result.size(), 1u);
  EXPECT_EQ(result[0], L'\0');
}

TEST(FmtFormatWriteWCharTest, SupportsWidthAlignmentForWideCharacters) {
  EXPECT_EQ(fmt::format(L"{:>3}", L'X'), std::wstring(L"  X"));
  EXPECT_EQ(fmt::format(L"{:<3}", L'X'), std::wstring(L"X  "));
  EXPECT_EQ(fmt::format(L"{:*^5}", L'X'), std::wstring(L"**X**"));
}
```

---

## id 53 - `fmtlib___fmt@287eaab3b2` - model qwen3-coder

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@287eaab3b2/qwen3-coder/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <limits>

TEST(FmtFormatTest, WriteLongDouble) {
    // Test normal long double values
    std::string buffer;
    fmt::format_to(std::back_inserter(buffer), "{}", 3.14159L);
    EXPECT_EQ(buffer, "3.14159");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", 0.0L);
    EXPECT_EQ(buffer, "0");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -123.456L);
    EXPECT_EQ(buffer, "-123.456");
    
    // Test edge cases
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<long double>::infinity());
    EXPECT_TRUE(buffer == "inf" || buffer == "infinity");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -std::numeric_limits<long double>::infinity());
    EXPECT_TRUE(buffer == "-inf" || buffer == "-infinity");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<long double>::quiet_NaN());
    EXPECT_EQ(buffer, "nan");
}

TEST(FmtFormatTest, WriteChar) {
    // Test normal char values
    std::string buffer;
    fmt::format_to(std::back_inserter(buffer), "{}", 'A');
    EXPECT_EQ(buffer, "A");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", 'z');
    EXPECT_EQ(buffer, "z");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", '0');
    EXPECT_EQ(buffer, "0");
    
    // Test special characters
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", '\n');
    EXPECT_EQ(buffer, "\n");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", '\t');
    EXPECT_EQ(buffer, "\t");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", '\0');
    EXPECT_EQ(buffer, "\0");  // Null character should be preserved
}

TEST(FmtFormatTest, WriteWideChar) {
    // Test wide character handling (should work when char_type is wchar_t)
    std::wstring buffer;
    fmt::format_to(std::back_inserter(buffer), L"{}", L'A');
    EXPECT_EQ(buffer, L"A");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), L"{}", L'z');
    EXPECT_EQ(buffer, L"z");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), L"{}", L'\n');
    EXPECT_EQ(buffer, L"\n");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), L"{}", L'\t');
    EXPECT_EQ(buffer, L"\t");
}
```

---

## id 54 - `fmtlib___fmt@287eaab3b2` - model qwen3-coder

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@287eaab3b2/qwen3-coder/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <limits>

TEST(FmtFormatTest, WriteLongDouble) {
    // Test normal long double values
    std::string buffer;
    fmt::format_to(std::back_inserter(buffer), "{}", 3.14159L);
    EXPECT_EQ(buffer, "3.14159");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", 0.0L);
    EXPECT_EQ(buffer, "0");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -1.5L);
    EXPECT_EQ(buffer, "-1.5");
    
    // Test edge cases
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<long double>::infinity());
    EXPECT_EQ(buffer, "inf");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -std::numeric_limits<long double>::infinity());
    EXPECT_EQ(buffer, "-inf");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<long double>::quiet_NaN());
    EXPECT_EQ(buffer, "nan");
}

TEST(FmtFormatTest, WriteChar) {
    // Test normal character
    std::string buffer;
    fmt::format_to(std::back_inserter(buffer), "{}", 'A');
    EXPECT_EQ(buffer, "A");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", 'z');
    EXPECT_EQ(buffer, "z");
    
    // Test special characters
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", '\0');
    EXPECT_EQ(buffer.size(), 1);
    EXPECT_EQ(buffer[0], '\0');
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", '\n');
    EXPECT_EQ(buffer, "\n");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", ' ');
    EXPECT_EQ(buffer, " ");
}

TEST(FmtFormatTest, WriteWideChar) {
    // Test wide character (only valid when char_type is wchar_t)
    std::wstring buffer;
    fmt::format_to(std::back_inserter(buffer), L"{}", L'A');
    EXPECT_EQ(buffer, L"A");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), L"{}", L'z');
    EXPECT_EQ(buffer, L"z");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), L"{}", L'\n');
    EXPECT_EQ(buffer, L"\n");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), L"{}", L' ');
    EXPECT_EQ(buffer, L" ");
}

TEST(FmtFormatTest, WriteDouble) {
    // Test normal double values
    std::string buffer;
    fmt::format_to(std::back_inserter(buffer), "{}", 3.14159);
    EXPECT_EQ(buffer, "3.14159");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", 0.0);
    EXPECT_EQ(buffer, "0");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -1.5);
    EXPECT_EQ(buffer, "-1.5");
    
    // Test edge cases
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<double>::infinity());
    EXPECT_EQ(buffer, "inf");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -std::numeric_limits<double>::infinity());
    EXPECT_EQ(buffer, "-inf");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<double>::quiet_NaN());
    EXPECT_EQ(buffer, "nan");
}

TEST(FmtFormatTest, WriteFloat) {
    // Test normal float values
    std::string buffer;
    fmt::format_to(std::back_inserter(buffer), "{}", 3.14159f);
    EXPECT_EQ(buffer, "3.14159");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", 0.0f);
    EXPECT_EQ(buffer, "0");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -1.5f);
    EXPECT_EQ(buffer, "-1.5");
    
    // Test edge cases
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<float>::infinity());
    EXPECT_EQ(buffer, "inf");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", -std::numeric_limits<float>::infinity());
    EXPECT_EQ(buffer, "-inf");
    
    buffer.clear();
    fmt::format_to(std::back_inserter(buffer), "{}", std::numeric_limits<float>::quiet_NaN());
    EXPECT_EQ(buffer, "nan");
}
```

---

## id 55 - `fmtlib___fmt@287eaab3b2` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@287eaab3b2/deepseek-chat/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <climits>

// Test write(long double) via fmt::format
TEST(WriteLongDoubleTest, BasicValues) {
  EXPECT_EQ(fmt::format("{}", 3.14L), "3.14");
  EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
  EXPECT_EQ(fmt::format("{}", -2.5L), "-2.5");
}

TEST(WriteLongDoubleTest, LargeAndSmallValues) {
  EXPECT_EQ(fmt::format("{}", 1e100L), "1e+100");
  EXPECT_EQ(fmt::format("{}", 1e-100L), "1e-100");
  EXPECT_EQ(fmt::format("{}", 1.0L / 3.0L), "0.3333333333333333");
}

TEST(WriteLongDoubleTest, InfinityAndNaN) {
  EXPECT_EQ(fmt::format("{}", std::numeric_limits<long double>::infinity()), "inf");
  EXPECT_EQ(fmt::format("{}", -std::numeric_limits<long double>::infinity()), "-inf");
  EXPECT_TRUE(fmt::format("{}", std::numeric_limits<long double>::quiet_NaN()) == "nan" ||
              fmt::format("{}", std::numeric_limits<long double>::quiet_NaN()) == "-nan");
}

TEST(WriteLongDoubleTest, ZeroAndNegativeZero) {
  EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
  EXPECT_EQ(fmt::format("{}", -0.0L), "-0.0");
}

TEST(WriteLongDoubleTest, FormatSpecs) {
  EXPECT_EQ(fmt::format("{:.2f}", 3.14159L), "3.14");
  EXPECT_EQ(fmt::format("{:+.3e}", 1.23456L), "+1.235e+00");
  EXPECT_EQ(fmt::format("{:10.4g}", 12345.6789L), "    1.235e+04");
}

// Test write(char) via fmt::format
TEST(WriteCharTest, BasicChar) {
  EXPECT_EQ(fmt::format("{}", 'a'), "a");
  EXPECT_EQ(fmt::format("{}", 'Z'), "Z");
  EXPECT_EQ(fmt::format("{}", '0'), "0");
}

TEST(WriteCharTest, SpecialChars) {
  EXPECT_EQ(fmt::format("{}", '\n'), "\n");
  EXPECT_EQ(fmt::format("{}", '\t'), "\t");
  EXPECT_EQ(fmt::format("{}", '\0'), std::string(1, '\0'));
}

TEST(WriteCharTest, CharInFormatString) {
  EXPECT_EQ(fmt::format("{} {}", 'x', 'y'), "x y");
  EXPECT_EQ(fmt::format("{:c}", 65), "A");
}

TEST(WriteCharTest, CharWithWidth) {
  EXPECT_EQ(fmt::format("{:5}", 'a'), "a    ");
  EXPECT_EQ(fmt::format("{:<5}", 'b'), "b    ");
  EXPECT_EQ(fmt::format("{:>5}", 'c'), "    c");
  EXPECT_EQ(fmt::format("{:^5}", 'd'), "  d  ");
}

// Test write(wchar_t) via fmt::format (only if wchar_t is supported)
TEST(WriteWcharTest, BasicWchar) {
  EXPECT_EQ(fmt::format(L"{}", L'a'), L"a");
  EXPECT_EQ(fmt::format(L"{}", L'Z'), L"Z");
  EXPECT_EQ(fmt::format(L"{}", L'0'), L"0");
}

TEST(WriteWcharTest, SpecialWchars) {
  EXPECT_EQ(fmt::format(L"{}", L'\n'), L"\n");
  EXPECT_EQ(fmt::format(L"{}", L'\t'), L"\t");
  EXPECT_EQ(fmt::format(L"{}", L'\0'), std::wstring(1, L'\0'));
}

TEST(WriteWcharTest, WcharInFormatString) {
  EXPECT_EQ(fmt::format(L"{} {}", L'x', L'y'), L"x y");
  EXPECT_EQ(fmt::format(L"{:c}", static_cast<wchar_t>(65)), L"A");
}

TEST(WriteWcharTest, WcharWithWidth) {
  EXPECT_EQ(fmt::format(L"{:5}", L'a'), L"a    ");
  EXPECT_EQ(fmt::format(L"{:<5}", L'b'), L"b    ");
  EXPECT_EQ(fmt::format(L"{:>5}", L'c'), L"    c");
  EXPECT_EQ(fmt::format(L"{:^5}", L'd'), L"  d  ");
}

// Edge cases: empty string via char/wchar_t (though write(char) writes a single char)
TEST(WriteCharEdgeTest, CharBoundaryValues) {
  EXPECT_EQ(fmt::format("{}", static_cast<char>(0)), std::string(1, '\0'));
  EXPECT_EQ(fmt::format("{}", static_cast<char>(127)), std::string(1, static_cast<char>(127)));
  EXPECT_EQ(fmt::format("{}", static_cast<char>(-128)), std::string(1, static_cast<char>(-128)));
}

TEST(WriteWcharEdgeTest, WcharBoundaryValues) {
  EXPECT_EQ(fmt::format(L"{}", static_cast<wchar_t>(0)), std::wstring(1, L'\0'));
  EXPECT_EQ(fmt::format(L"{}", static_cast<wchar_t>(65535)), std::wstring(1, static_cast<wchar_t>(65535)));
}

// Test that write(long double) handles extreme values correctly
TEST(WriteLongDoubleEdgeTest, MinMaxValues) {
  long double max_val = std::numeric_limits<long double>::max();
  long double min_val = std::numeric_limits<long double>::min();
  long double denorm_min = std::numeric_limits<long double>::denorm_min();

  EXPECT_FALSE(fmt::format("{}", max_val).empty());
  EXPECT_FALSE(fmt::format("{}", min_val).empty());
  EXPECT_FALSE(fmt::format("{}", denorm_min).empty());
}

// Test that write(char) and write(wchar_t) work with format specifiers that affect char output
TEST(WriteCharFormatTest, CharWithFillAndAlign) {
  EXPECT_EQ(fmt::format("{:*>5}", 'a'), "****a");
  EXPECT_EQ(fmt::format("{:*<5}", 'b'), "b****");
  EXPECT_EQ(fmt::format("{:*^5}", 'c'), "**c**");
}

TEST(WriteWcharFormatTest, WcharWithFillAndAlign) {
  EXPECT_EQ(fmt::format(L"{:*>5}", L'a'), L"****a");
  EXPECT_EQ(fmt::format(L"{:*<5}", L'b'), L"b****");
  EXPECT_EQ(fmt::format(L"{:*^5}", L'c'), L"**c**");
}
```

---

## id 56 - `fmtlib___fmt@287eaab3b2` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@287eaab3b2/deepseek-chat/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>

// Test write(long double) via fmt::format
TEST(WriteLongDoubleTest, BasicValues) {
  // Normal positive value
  EXPECT_EQ(fmt::format("{}", 3.14159265358979323846L), "3.141592653589793");
  // Negative value
  EXPECT_EQ(fmt::format("{}", -2.718281828459045L), "-2.718281828459045");
  // Zero
  EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
  // Negative zero
  EXPECT_EQ(fmt::format("{}", -0.0L), "-0.0");
}

TEST(WriteLongDoubleTest, SpecialValues) {
  // Infinity
  EXPECT_EQ(fmt::format("{}", std::numeric_limits<long double>::infinity()), "inf");
  // Negative infinity
  EXPECT_EQ(fmt::format("{}", -std::numeric_limits<long double>::infinity()), "-inf");
  // NaN
  EXPECT_TRUE(fmt::format("{}", std::numeric_limits<long double>::quiet_NaN()) == "nan" ||
              fmt::format("{}", std::numeric_limits<long double>::quiet_NaN()) == "-nan");
}

TEST(WriteLongDoubleTest, ExtremeValues) {
  // Very large number
  EXPECT_EQ(fmt::format("{}", 1e30L), "1e+30");
  // Very small number
  EXPECT_EQ(fmt::format("{}", 1e-30L), "1e-30");
  // Largest finite value
  long double max_val = std::numeric_limits<long double>::max();
  std::string result = fmt::format("{}", max_val);
  EXPECT_FALSE(result.empty());
  EXPECT_NE(result, "inf");
  EXPECT_NE(result, "-inf");
  // Smallest positive normalized value
  long double min_val = std::numeric_limits<long double>::min();
  result = fmt::format("{}", min_val);
  EXPECT_FALSE(result.empty());
  EXPECT_NE(result, "0.0");
}

TEST(WriteLongDoubleTest, FormatSpecs) {
  // With precision specifier
  EXPECT_EQ(fmt::format("{:.5f}", 1.23456789L), "1.23457");
  // With width
  EXPECT_EQ(fmt::format("{:10.3f}", 3.14159L), "    3.142");
  // Scientific notation
  EXPECT_EQ(fmt::format("{:.3e}", 12345.6789L), "1.235e+04");
}

// Test write(char) via fmt::format
TEST(WriteCharTest, BasicChar) {
  EXPECT_EQ(fmt::format("{}", 'A'), "A");
  EXPECT_EQ(fmt::format("{}", 'z'), "z");
  EXPECT_EQ(fmt::format("{}", '0'), "0");
}

TEST(WriteCharTest, SpecialChars) {
  EXPECT_EQ(fmt::format("{}", '\n'), "\n");
  EXPECT_EQ(fmt::format("{}", '\t'), "\t");
  EXPECT_EQ(fmt::format("{}", '\0'), std::string("\0", 1));
}

TEST(WriteCharTest, NonPrintableChars) {
  // Control characters
  EXPECT_EQ(fmt::format("{}", '\x01'), std::string("\x01", 1));
  EXPECT_EQ(fmt::format("{}", '\x7F'), std::string("\x7F", 1));
}

TEST(WriteCharTest, CharWithFormatSpecs) {
  // Width specifier
  EXPECT_EQ(fmt::format("{:5}", 'X'), "X    ");
  EXPECT_EQ(fmt::format("{:<5}", 'X'), "X    ");
  EXPECT_EQ(fmt::format("{:>5}", 'X'), "    X");
  EXPECT_EQ(fmt::format("{:^5}", 'X'), "  X  ");
  // Fill character
  EXPECT_EQ(fmt::format("{:*<5}", 'X'), "X****");
  EXPECT_EQ(fmt::format("{:*>5}", 'X'), "****X");
}

// Test write(wchar_t) via fmt::format (only if wchar_t is supported)
TEST(WriteWcharTest, BasicWchar) {
  // This test only runs if wchar_t formatting is enabled
  // We test via fmt::format(L"{}", ...) which uses wchar_t overloads
  EXPECT_EQ(fmt::format(L"{}", L'A'), L"A");
  EXPECT_EQ(fmt::format(L"{}", L'z'), L"z");
  EXPECT_EQ(fmt::format(L"{}", L'0'), L"0");
}

TEST(WriteWcharTest, SpecialWchars) {
  EXPECT_EQ(fmt::format(L"{}", L'\n'), L"\n");
  EXPECT_EQ(fmt::format(L"{}", L'\t'), L"\t");
  EXPECT_EQ(fmt::format(L"{}", L'\0'), std::wstring(L"\0", 1));
}

TEST(WriteWcharTest, WcharWithFormatSpecs) {
  EXPECT_EQ(fmt::format(L"{:5}", L'X'), L"X    ");
  EXPECT_EQ(fmt::format(L"{:*<5}", L'X'), L"X****");
  EXPECT_EQ(fmt::format(L"{:*>5}", L'X'), L"****X");
  EXPECT_EQ(fmt::format(L"{:*^5}", L'X'), L"**X**");
}

// Edge case: empty string context (write called with no additional formatting)
TEST(WriteEdgeCaseTest, SingleCharString) {
  // Formatting a single character as a string
  EXPECT_EQ(fmt::format("{}", 'a'), "a");
  // Formatting a single wchar_t as a string
  EXPECT_EQ(fmt::format(L"{}", L'a'), L"a");
}

// Test that write works correctly with different buffer types
TEST(WriteBufferTest, MemoryBuffer) {
  fmt::memory_buffer buf;
  fmt::format_to(std::back_inserter(buf), "{}", 3.14L);
  EXPECT_EQ(fmt::to_string(buf), "3.14");
  
  buf.clear();
  fmt::format_to(std::back_inserter(buf), "{}", 'X');
  EXPECT_EQ(fmt::to_string(buf), "X");
}
```

---

## id 57 - `fmtlib___fmt@287eaab3b2` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@287eaab3b2/deepseek-chat/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>

// Test write(long double) via fmt::format
TEST(WriteLongDoubleTest, BasicValues) {
  EXPECT_EQ(fmt::format("{}", 3.14159265358979323846L), "3.141592653589793");
  EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
  EXPECT_EQ(fmt::format("{}", -2.5L), "-2.5");
}

TEST(WriteLongDoubleTest, InfinityAndNaN) {
  long double inf = std::numeric_limits<long double>::infinity();
  long double nan = std::numeric_limits<long double>::quiet_NaN();
  EXPECT_EQ(fmt::format("{}", inf), "inf");
  EXPECT_EQ(fmt::format("{}", -inf), "-inf");
  EXPECT_EQ(fmt::format("{}", nan), "nan");
}

TEST(WriteLongDoubleTest, VeryLargeAndSmall) {
  long double large = 1e308L;
  long double small = 1e-308L;
  EXPECT_EQ(fmt::format("{}", large), "1e+308");
  EXPECT_EQ(fmt::format("{}", small), "1e-308");
}

TEST(WriteLongDoubleTest, ZeroAndNegativeZero) {
  EXPECT_EQ(fmt::format("{}", 0.0L), "0.0");
  EXPECT_EQ(fmt::format("{}", -0.0L), "-0.0");
}

// Test write(char) via fmt::format
TEST(WriteCharTest, BasicCharacters) {
  EXPECT_EQ(fmt::format("{}", 'a'), "a");
  EXPECT_EQ(fmt::format("{}", 'Z'), "Z");
  EXPECT_EQ(fmt::format("{}", '0'), "0");
}

TEST(WriteCharTest, SpecialCharacters) {
  EXPECT_EQ(fmt::format("{}", '\n'), "\n");
  EXPECT_EQ(fmt::format("{}", '\t'), "\t");
  EXPECT_EQ(fmt::format("{}", '\0'), std::string(1, '\0'));
}

TEST(WriteCharTest, NonPrintableCharacters) {
  EXPECT_EQ(fmt::format("{}", '\x01'), std::string(1, '\x01'));
  EXPECT_EQ(fmt::format("{}", '\x7F'), std::string(1, '\x7F'));
}

// Test write(wchar_t) via fmt::format (only if wchar_t is supported)
TEST(WriteWcharTest, BasicWideCharacters) {
  EXPECT_EQ(fmt::format(L"{}", L'a'), L"a");
  EXPECT_EQ(fmt::format(L"{}", L'Z'), L"Z");
  EXPECT_EQ(fmt::format(L"{}", L'0'), L"0");
}

TEST(WriteWcharTest, WideSpecialCharacters) {
  EXPECT_EQ(fmt::format(L"{}", L'\n'), L"\n");
  EXPECT_EQ(fmt::format(L"{}", L'\t'), L"\t");
  EXPECT_EQ(fmt::format(L"{}", L'\0'), std::wstring(1, L'\0'));
}

TEST(WriteWcharTest, WideNonPrintableCharacters) {
  EXPECT_EQ(fmt::format(L"{}", L'\x01'), std::wstring(1, L'\x01'));
  EXPECT_EQ(fmt::format(L"{}", L'\x7F'), std::wstring(1, L'\x7F'));
}

// Edge cases for write(char) - format specifiers
TEST(WriteCharTest, WithFormatSpecifiers) {
  EXPECT_EQ(fmt::format("{:c}", 'x'), "x");
  EXPECT_EQ(fmt::format("{:d}", 'A'), "65");
  EXPECT_EQ(fmt::format("{:x}", 'B'), "42");
  EXPECT_EQ(fmt::format("{:o}", 'C'), "103");
}

// Edge cases for write(long double) - format specifiers
TEST(WriteLongDoubleTest, WithFormatSpecifiers) {
  EXPECT_EQ(fmt::format("{:.2f}", 1.23456789L), "1.23");
  EXPECT_EQ(fmt::format("{:.0f}", 9.9L), "10");
  EXPECT_EQ(fmt::format("{:+g}", 5.0L), "+5");
  EXPECT_EQ(fmt::format("{:e}", 1000.0L), "1.000000e+03");
}

// Test that write(char) works in a larger formatted string
TEST(WriteCharTest, InLargerString) {
  EXPECT_EQ(fmt::format("char: {} int: {}", 'x', 42), "char: x int: 42");
}

// Test that write(wchar_t) works in a larger formatted string
TEST(WriteWcharTest, InLargerWideString) {
  EXPECT_EQ(fmt::format(L"char: {} int: {}", L'x', 42), L"char: x int: 42");
}

// Test that write(long double) works in a larger formatted string
TEST(WriteLongDoubleTest, InLargerString) {
  EXPECT_EQ(fmt::format("pi = {}, e = {}", 3.14159265358979323846L, 2.71828182845904523536L),
            "pi = 3.141592653589793, e = 2.718281828459045");
}
```

---

## id 58 - `fmtlib___fmt@287eaab3b2` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@287eaab3b2/deepseek-chat/buggy/run1/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>

// Test write(long double) via fmt::format and fmt::print-like functions
TEST(WriteLongDoubleTest, BasicValues) {
    // Normal positive value
    std::string result = fmt::format("{}", 3.14159265358979323846L);
    EXPECT_EQ(result, "3.141592653589793");

    // Negative value
    result = fmt::format("{}", -2.71828182845904523536L);
    EXPECT_EQ(result, "-2.718281828459045");

    // Zero
    result = fmt::format("{}", 0.0L);
    EXPECT_EQ(result, "0.0");

    // Negative zero
    result = fmt::format("{}", -0.0L);
    EXPECT_EQ(result, "-0.0");
}

TEST(WriteLongDoubleTest, InfinityAndNaN) {
    // Positive infinity
    std::string result = fmt::format("{}", std::numeric_limits<long double>::infinity());
    EXPECT_EQ(result, "inf");

    // Negative infinity
    result = fmt::format("{}", -std::numeric_limits<long double>::infinity());
    EXPECT_EQ(result, "-inf");

    // NaN
    result = fmt::format("{}", std::numeric_limits<long double>::quiet_NaN());
    EXPECT_EQ(result, "nan");
}

TEST(WriteLongDoubleTest, Extremes) {
    // Very small positive number (denormal)
    long double tiny = std::numeric_limits<long double>::denorm_min();
    std::string result = fmt::format("{}", tiny);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0.0"); // denorm min is not zero

    // Very large number
    long double huge = std::numeric_limits<long double>::max();
    result = fmt::format("{}", huge);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
}

TEST(WriteLongDoubleTest, FormatSpecs) {
    // With precision
    std::string result = fmt::format("{:.5f}", 1.23456789L);
    EXPECT_EQ(result, "1.23457");

    // With width and alignment
    result = fmt::format("{:>10.3f}", 42.0L);
    EXPECT_EQ(result, "    42.000");

    // Scientific notation
    result = fmt::format("{:.3e}", 123456.789L);
    EXPECT_EQ(result, "1.235e+05");
}

// Test write(char) via fmt::format
TEST(WriteCharTest, BasicChar) {
    std::string result = fmt::format("{}", 'A');
    EXPECT_EQ(result, "A");

    result = fmt::format("{}", 'z');
    EXPECT_EQ(result, "z");

    result = fmt::format("{}", '0');
    EXPECT_EQ(result, "0");
}

TEST(WriteCharTest, SpecialChars) {
    // Null character
    std::string result = fmt::format("{}", '\0');
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], '\0');

    // Newline
    result = fmt::format("{}", '\n');
    EXPECT_EQ(result, "\n");

    // Tab
    result = fmt::format("{}", '\t');
    EXPECT_EQ(result, "\t");
}

TEST(WriteCharTest, CharWithFormatSpecs) {
    // Width
    std::string result = fmt::format("{:5}", 'X');
    EXPECT_EQ(result, "X    ");

    // Left align
    result = fmt::format("{:<5}", 'Y');
    EXPECT_EQ(result, "Y    ");

    // Right align
    result = fmt::format("{:>5}", 'Z');
    EXPECT_EQ(result, "    Z");
}

// Test write(wchar_t) via fmt::format with wide strings
TEST(WriteWcharTest, BasicWchar) {
    std::wstring result = fmt::format(L"{}", L'A');
    EXPECT_EQ(result, L"A");

    result = fmt::format(L"{}", L'z');
    EXPECT_EQ(result, L"z");

    result = fmt::format(L"{}", L'0');
    EXPECT_EQ(result, L"0");
}

TEST(WriteWcharTest, SpecialWchars) {
    // Null wide character
    std::wstring result = fmt::format(L"{}", L'\0');
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], L'\0');

    // Newline
    result = fmt::format(L"{}", L'\n');
    EXPECT_EQ(result, L"\n");

    // Tab
    result = fmt::format(L"{}", L'\t');
    EXPECT_EQ(result, L"\t");
}

TEST(WriteWcharTest, WcharWithFormatSpecs) {
    // Width
    std::wstring result = fmt::format(L"{:5}", L'X');
    EXPECT_EQ(result, L"X    ");

    // Left align
    result = fmt::format(L"{:<5}", L'Y');
    EXPECT_EQ(result, L"Y    ");

    // Right align
    result = fmt::format(L"{:>5}", L'Z');
    EXPECT_EQ(result, L"    Z");
}

// Edge case: empty format string with char
TEST(WriteCharTest, EmptyFormatString) {
    std::string result = fmt::format("{}", ' ');
    EXPECT_EQ(result, " ");
}

// Edge case: combining char and long double in same format
TEST(WriteCombinedTest, CharAndDouble) {
    std::string result = fmt::format("char={}, double={}", 'c', 3.14L);
    EXPECT_EQ(result, "char=c, double=3.14");
}
```

---

## id 59 - `fmtlib___fmt@287eaab3b2` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@287eaab3b2/deepseek-chat/buggy/run3/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>

// Test write(long double) via fmt::format and fmt::print to string
TEST(WriteLongDoubleTest, BasicValues) {
    // Normal positive value
    std::string result = fmt::format("{:Lf}", 3.14159265358979323846L);
    EXPECT_EQ(result, "3.141593");

    // Negative value
    result = fmt::format("{:Lf}", -2.718281828459045L);
    EXPECT_EQ(result, "-2.718282");

    // Zero
    result = fmt::format("{:Lf}", 0.0L);
    EXPECT_EQ(result, "0.000000");

    // Very large value
    result = fmt::format("{:Le}", 1.0e100L);
    EXPECT_EQ(result, "1.000000e+100");

    // Very small value (subnormal)
    result = fmt::format("{:Le}", 1.0e-100L);
    EXPECT_EQ(result, "1.000000e-100");
}

TEST(WriteLongDoubleTest, SpecialValues) {
    // Infinity
    std::string result = fmt::format("{}", std::numeric_limits<long double>::infinity());
    EXPECT_EQ(result, "inf");

    // Negative infinity
    result = fmt::format("{}", -std::numeric_limits<long double>::infinity());
    EXPECT_EQ(result, "-inf");

    // NaN
    result = fmt::format("{}", std::numeric_limits<long double>::quiet_NaN());
    EXPECT_EQ(result, "nan");
}

TEST(WriteLongDoubleTest, FormatSpecs) {
    // Fixed precision
    std::string result = fmt::format("{:.5f}", 1.23456789L);
    EXPECT_EQ(result, "1.23457");

    // Scientific notation
    result = fmt::format("{:.3e}", 123456.789L);
    EXPECT_EQ(result, "1.235e+05");

    // General format
    result = fmt::format("{:.4g}", 123456.789L);
    EXPECT_EQ(result, "1.235e+05");

    // Width and alignment
    result = fmt::format("{:10.3f}", 3.14L);
    EXPECT_EQ(result, "    3.140");
}

// Test write(char) via fmt::format
TEST(WriteCharTest, BasicChar) {
    std::string result = fmt::format("{}", 'A');
    EXPECT_EQ(result, "A");

    result = fmt::format("{}", 'z');
    EXPECT_EQ(result, "z");

    result = fmt::format("{}", '0');
    EXPECT_EQ(result, "0");
}

TEST(WriteCharTest, SpecialChars) {
    // Null character
    std::string result = fmt::format("{}", '\0');
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], '\0');

    // Newline
    result = fmt::format("{}", '\n');
    EXPECT_EQ(result, "\n");

    // Tab
    result = fmt::format("{}", '\t');
    EXPECT_EQ(result, "\t");
}

TEST(WriteCharTest, FormatSpecs) {
    // Width and fill
    std::string result = fmt::format("{:5}", 'X');
    EXPECT_EQ(result, "X    ");

    result = fmt::format("{:*>5}", 'Y');
    EXPECT_EQ(result, "****Y");

    result = fmt::format("{:*^5}", 'Z');
    EXPECT_EQ(result, "**Z**");
}

// Test write(wchar_t) via fmt::format with wide strings
TEST(WriteWcharTest, BasicWchar) {
    std::wstring result = fmt::format(L"{}", L'A');
    EXPECT_EQ(result, L"A");

    result = fmt::format(L"{}", L'z');
    EXPECT_EQ(result, L"z");

    result = fmt::format(L"{}", L'0');
    EXPECT_EQ(result, L"0");
}

TEST(WriteWcharTest, SpecialWchars) {
    // Null wide character
    std::wstring result = fmt::format(L"{}", L'\0');
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], L'\0');

    // Newline
    result = fmt::format(L"{}", L'\n');
    EXPECT_EQ(result, L"\n");

    // Tab
    result = fmt::format(L"{}", L'\t');
    EXPECT_EQ(result, L"\t");
}

TEST(WriteWcharTest, FormatSpecs) {
    // Width and fill
    std::wstring result = fmt::format(L"{:5}", L'X');
    EXPECT_EQ(result, L"X    ");

    result = fmt::format(L"{:*>5}", L'Y');
    EXPECT_EQ(result, L"****Y");

    result = fmt::format(L"{:*^5}", L'Z');
    EXPECT_EQ(result, L"**Z**");
}

// Edge cases for write(long double)
TEST(WriteLongDoubleEdgeTest, MinMaxValues) {
    // Minimum positive normal value
    long double min_val = std::numeric_limits<long double>::min();
    std::string result = fmt::format("{:Le}", min_val);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0.000000e+00");

    // Maximum finite value
    long double max_val = std::numeric_limits<long double>::max();
    result = fmt::format("{:Le}", max_val);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");

    // Denormalized minimum
    long double denorm_min = std::numeric_limits<long double>::denorm_min();
    result = fmt::format("{:Le}", denorm_min);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0.000000e+00");
}

TEST(WriteLongDoubleEdgeTest, PrecisionBoundaries) {
    // Very high precision
    std::string result = fmt::format("{:.50f}", 1.0L / 3.0L);
    EXPECT_EQ(result.substr(0, 2), "0.");
    EXPECT_EQ(result.size(), 53); // "0." + 50 digits + possible rounding

    // Zero precision
    result = fmt::format("{:.0f}", 123.456L);
    EXPECT_EQ(result, "123");

    // Negative precision (should be treated as zero)
    result = fmt::format("{:.-1f}", 123.456L);
    EXPECT_EQ(result, "123");
}

// Edge cases for write(char)
TEST(WriteCharEdgeTest, AllPrintableAscii) {
    for (char c = 32; c < 127; ++c) {
        std::string result = fmt::format("{}", c);
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result[0], c);
    }
}

TEST(WriteCharEdgeTest, FormatWithEmptyString) {
    std::string result = fmt::format("{:1}", ' ');
    EXPECT_EQ(result, " ");
    EXPECT_EQ(result.size(), 1);
}

// Edge cases for write(wchar_t)
TEST(WriteWcharEdgeTest, AllBasicLatin) {
    for (wchar_t c = 32; c < 127; ++c) {
        std::wstring result = fmt::format(L"{}", c);
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result[0], c);
    }
}

TEST(WriteWcharEdgeTest, WideCharBoundaries) {
... (cat bot 6 dong)
```

---

## id 60 - `fmtlib___fmt@287eaab3b2` - model deepseek-chat

**Ham focal (ban code DA SUA, dung):**

```cpp
(khong tim thay file: /home/truclinh/Thesis/kse2026/artifact/data/raw/fmtlib___fmt@287eaab3b2/deepseek-chat/buggy/run2/focal.txt)
```

**Bai test LLM sinh (compile OK, chay qua dong loi, nhung KHONG bat duoc bug):**

```cpp
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <string>
#include <cmath>
#include <limits>

// Test write(long double) via fmt::format and fmt::print to string
TEST(WriteLongDoubleTest, BasicValues) {
    // Normal positive value
    std::string result = fmt::format("{}", 3.14159265358979323846L);
    EXPECT_EQ(result, "3.141592653589793");

    // Negative value
    result = fmt::format("{}", -2.71828182845904523536L);
    EXPECT_EQ(result, "-2.718281828459045");

    // Zero
    result = fmt::format("{}", 0.0L);
    EXPECT_EQ(result, "0.0");

    // Very small number
    result = fmt::format("{}", 1e-10L);
    EXPECT_EQ(result, "0.0000000001");

    // Very large number
    result = fmt::format("{}", 1e20L);
    EXPECT_EQ(result, "100000000000000000000.0");
}

TEST(WriteLongDoubleTest, InfinityAndNaN) {
    // Positive infinity
    std::string result = fmt::format("{}", std::numeric_limits<long double>::infinity());
    EXPECT_EQ(result, "inf");

    // Negative infinity
    result = fmt::format("{}", -std::numeric_limits<long double>::infinity());
    EXPECT_EQ(result, "-inf");

    // NaN
    result = fmt::format("{}", std::numeric_limits<long double>::quiet_NaN());
    EXPECT_EQ(result, "nan");
}

TEST(WriteLongDoubleTest, MinAndMaxValues) {
    // Minimum positive normalized value
    long double min_val = std::numeric_limits<long double>::min();
    std::string result = fmt::format("{}", min_val);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0.0");

    // Maximum finite value
    long double max_val = std::numeric_limits<long double>::max();
    result = fmt::format("{}", max_val);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "inf");
}

TEST(WriteLongDoubleTest, DenormalizedValue) {
    // Denormalized number (smallest positive)
    long double denorm = std::numeric_limits<long double>::denorm_min();
    std::string result = fmt::format("{}", denorm);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result, "0.0");
}

// Test write(char) via fmt::format
TEST(WriteCharTest, BasicChar) {
    std::string result = fmt::format("{}", 'A');
    EXPECT_EQ(result, "A");

    result = fmt::format("{}", 'z');
    EXPECT_EQ(result, "z");

    result = fmt::format("{}", '0');
    EXPECT_EQ(result, "0");
}

TEST(WriteCharTest, SpecialCharacters) {
    // Null character
    std::string result = fmt::format("{}", '\0');
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], '\0');

    // Newline
    result = fmt::format("{}", '\n');
    EXPECT_EQ(result, "\n");

    // Tab
    result = fmt::format("{}", '\t');
    EXPECT_EQ(result, "\t");
}

TEST(WriteCharTest, NonPrintableCharacters) {
    // Character with value 1 (SOH)
    std::string result = fmt::format("{}", static_cast<char>(1));
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], static_cast<char>(1));

    // Character with value 127 (DEL)
    result = fmt::format("{}", static_cast<char>(127));
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], static_cast<char>(127));
}

// Test write(wchar_t) via fmt::format with wide strings
TEST(WriteWcharTest, BasicWchar) {
    std::wstring result = fmt::format(L"{}", L'A');
    EXPECT_EQ(result, L"A");

    result = fmt::format(L"{}", L'z');
    EXPECT_EQ(result, L"z");

    result = fmt::format(L"{}", L'0');
    EXPECT_EQ(result, L"0");
}

TEST(WriteWcharTest, WideSpecialCharacters) {
    // Null wide character
    std::wstring result = fmt::format(L"{}", L'\0');
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], L'\0');

    // Newline
    result = fmt::format(L"{}", L'\n');
    EXPECT_EQ(result, L"\n");

    // Tab
    result = fmt::format(L"{}", L'\t');
    EXPECT_EQ(result, L"\t");
}

TEST(WriteWcharTest, UnicodeCharacters) {
    // Unicode character (Latin capital letter A with acute)
    std::wstring result = fmt::format(L"{}", L'\u00C1');
    EXPECT_EQ(result, L"\u00C1");

    // Unicode character (CJK ideograph)
    result = fmt::format(L"{}", L'\u4E2D');
    EXPECT_EQ(result, L"\u4E2D");
}

// Test that write(char) and write(long double) work together in format string
TEST(WriteMixedTest, CharAndDoubleInSameFormat) {
    std::string result = fmt::format("{}{}", 'x', 3.14L);
    EXPECT_EQ(result, "x3.14");
}

// Test that write(char) works with format specifiers
TEST(WriteCharWithFormatSpecifiers, FillAndAlign) {
    std::string result = fmt::format("{:*>5}", 'A');
    EXPECT_EQ(result, "****A");

    result = fmt::format("{:*<5}", 'A');
    EXPECT_EQ(result, "A****");

    result = fmt::format("{:*^5}", 'A');
    EXPECT_EQ(result, "**A**");
}

// Test that write(long double) works with format specifiers
TEST(WriteLongDoubleWithFormatSpecifiers, PrecisionAndWidth) {
    std::string result = fmt::format("{:.2f}", 3.14159L);
    EXPECT_EQ(result, "3.14");

    result = fmt::format("{:10.3f}", 2.71828L);
    EXPECT_EQ(result, "     2.718");

    result = fmt::format("{:+06.2f}", -1.5L);
    EXPECT_EQ(result, "-001.50");
}
```

---
