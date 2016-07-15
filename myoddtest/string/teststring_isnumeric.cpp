#include "string\string.h"
#include "..\testcommon.h"
#include <gtest/gtest.h>

const struct test_isnumeric
{
  std::wstring given;
  bool is;
  bool allowDecimals;

  friend std::ostream& operator <<(std::ostream& os, const test_isnumeric& obj)
  {
    return os
      << "Given : " << myodd::strings::WString2String(obj.given)
      << " Expected : " << (obj.is ? "true" : "false")
      << " Allow decimals : " << (obj.allowDecimals ? "true" : "false");
  }
};

struct MyOddStringIsNumeric : testing::Test, testing::WithParamInterface<test_isnumeric>
{
};

struct MyOddStringIsNumericWithAllowDecimals : testing::Test, testing::WithParamInterface<test_isnumeric>
{
};

TEST_P(MyOddStringIsNumeric, IsNumericDefaultParams)
{
  auto param = GetParam();

  auto given = GetParam().given;
  auto is = GetParam().is;

  ASSERT_EQ(is, myodd::strings::IsNumeric(given));
}

TEST_P(MyOddStringIsNumericWithAllowDecimals, UseAllowDecimalsFlag)
{
  auto param = GetParam();

  auto given = GetParam().given;
  auto is = GetParam().is;
  auto allowDecimals = GetParam().allowDecimals;

  ASSERT_EQ(is, myodd::strings::IsNumeric(given, allowDecimals));
}

INSTANTIATE_TEST_CASE_P(NegativeStrings, MyOddStringIsNumeric,
  testing::Values(
    test_isnumeric{ L"-1", true },
    test_isnumeric{ L"-0", true },
    test_isnumeric{ L"-.1", true },
    test_isnumeric{ L"-0.1", true },
    test_isnumeric{ L"-1.1", true },
    test_isnumeric{ L"-999", true },
    test_isnumeric{ L"-999.a", false },
    test_isnumeric{ L"-1.1.1", false }
));

INSTANTIATE_TEST_CASE_P(NegativeStringsDecimalsAllowed, MyOddStringIsNumericWithAllowDecimals,
  testing::Values(
    test_isnumeric{ L"-1", true, true },
    test_isnumeric{ L"-0", true, true },
    test_isnumeric{ L"-.1", true, true },
    test_isnumeric{ L"-0.1", true, true },
    test_isnumeric{ L"-1.1", true, true },
    test_isnumeric{ L"-999", true, true },
    test_isnumeric{ L"-999.a", false, true },
    test_isnumeric{ L"-1.1.1", false, true }
));

INSTANTIATE_TEST_CASE_P(NegativeStringsDecimalsNotAllowed, MyOddStringIsNumericWithAllowDecimals,
  testing::Values(
    test_isnumeric{ L"-1", true, false },
    test_isnumeric{ L"-0", true, false },
    test_isnumeric{ L"-1.", true, false },   //  1. is not a decimal
    test_isnumeric{ L"-.1", false, false },
    test_isnumeric{ L"-0.1", false, false },
    test_isnumeric{ L"-1.1", false, false },
    test_isnumeric{ L"-999", true, false },
    test_isnumeric{ L"-999.a", false, false },
    test_isnumeric{ L"-1.1.1", false, false }
));

INSTANTIATE_TEST_CASE_P(PositiveStrings, MyOddStringIsNumeric,
  testing::Values(
    test_isnumeric{ L"1", true },
    test_isnumeric{ L"+0", true },
    test_isnumeric{ L".1", true },
    test_isnumeric{ L"0.1", true },
    test_isnumeric{ L"+0.1", true },
    test_isnumeric{ L"+.1", true },
    test_isnumeric{ L"1.1", true },
    test_isnumeric{ L"-999", true },
    test_isnumeric{ L"-999.a", false },
    test_isnumeric{ L"-1.1.1", false },
    test_isnumeric{ L"1.", true },
    test_isnumeric{ L"   1.   ", true },
    test_isnumeric{ L"   10   ", true },
    test_isnumeric{ L"   10.4   ", true },
    test_isnumeric{ L"   10 . 4   ", false }
));

INSTANTIATE_TEST_CASE_P(PositiveStringsDecimalsNotAllowed, MyOddStringIsNumericWithAllowDecimals,
  testing::Values(
    test_isnumeric{ L"1", true, false },
    test_isnumeric{ L"+0", true, false },
    test_isnumeric{ L".1", false, false },
    test_isnumeric{ L"0.1", false, false },
    test_isnumeric{ L"+0.1", false, false },
    test_isnumeric{ L"+.1", false, false },
    test_isnumeric{ L"1.1", false, false },
    test_isnumeric{ L"-999", true, false },
    test_isnumeric{ L"-999.a", false, false },
    test_isnumeric{ L"-1.1.1", false, false },
    test_isnumeric{ L"1.", true, false },
    test_isnumeric{ L"   1.   ", true, false },
    test_isnumeric{ L"   10   ", true, false },
    test_isnumeric{ L"   10.4   ", false, false },
    test_isnumeric{ L"   10 . 4   ", false, false }
));

INSTANTIATE_TEST_CASE_P(MiscStrings, MyOddStringIsNumeric,
  testing::Values(
    test_isnumeric{ L"-", false },
    test_isnumeric{ L".", false },
    test_isnumeric{ L"+", false },
    test_isnumeric{ L"A1,1", false },
    test_isnumeric{ L"1,1", false },
    test_isnumeric{ L"+A11", false },
    test_isnumeric{ L"A11", false },
    test_isnumeric{ L"11A", false },
    test_isnumeric{ L"11A", false },
    test_isnumeric{ L"  1        2   ", false },
    test_isnumeric{ L"-999.1   2", false },
    test_isnumeric{ L"999.1   2", false },
    test_isnumeric{ L"", false },  //  empty
    test_isnumeric{ L"        ", false } //  empty
));

INSTANTIATE_TEST_CASE_P(MiscStringsWithDecimalFlag, MyOddStringIsNumericWithAllowDecimals,
  testing::Values(
    test_isnumeric{ L"-", false, false },
    test_isnumeric{ L".", false, false },
    test_isnumeric{ L"+", false, false },
    test_isnumeric{ L"A1,1", false, false },
    test_isnumeric{ L"1,1", false, false },
    test_isnumeric{ L"+A11", false, false },
    test_isnumeric{ L"A11", false, false },
    test_isnumeric{ L"11A", false, false },
    test_isnumeric{ L"11A", false, false },
    test_isnumeric{ L"  1        2   ", false, false },
    test_isnumeric{ L"-999.1   2", false, false },
    test_isnumeric{ L"999.1   2", false, false },
    test_isnumeric{ L"", false, false },  //  empty
    test_isnumeric{ L"        ", false, false }, //  empty

    test_isnumeric{ L"-", false, true },
    test_isnumeric{ L".", false, true },
    test_isnumeric{ L"+", false, true },
    test_isnumeric{ L"A1,1", false, true },
    test_isnumeric{ L"1,1", false, true },
    test_isnumeric{ L"+A11", false, true },
    test_isnumeric{ L"A11", false, true },
    test_isnumeric{ L"11A", false, true },
    test_isnumeric{ L"11A", false, true },
    test_isnumeric{ L"  1        2   ", false, true },
    test_isnumeric{ L"-999.1   2", false, true },
    test_isnumeric{ L"999.1   2", false, true },
    test_isnumeric{ L"", false, true },  //  empty
    test_isnumeric{ L"        ", false, true } //  empty

));
