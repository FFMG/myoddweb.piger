#include "string\string.h"
#include "..\testcommon.h"
#include <gtest/gtest.h>

const struct test_is
{
  std::wstring given;
  bool is;
  bool allowDecimals;
};

struct MyOddStringIsNumeric : testing::Test, testing::WithParamInterface<test_is>
{
};

struct MyOddStringIsNumericWithAllowDecimals : testing::Test, testing::WithParamInterface<test_is>
{
};

TEST_P(MyOddStringIsNumeric, DefaultParams)
{
  auto param = GetParam();

  auto given = GetParam().given;
  auto is = GetParam().is;

  ASSERT_EQ( is, myodd::strings::IsNumeric(given));
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
    test_is{ L"-1", true },
    test_is{ L"-0", true },
    test_is{ L"-.1", true },
    test_is{ L"-0.1", true },
    test_is{ L"-1.1", true },
    test_is{ L"-999", true },
    test_is{ L"-999.a", false },
    test_is{ L"-1.1.1", false }
  ));

INSTANTIATE_TEST_CASE_P(NegativeStringsDecimalsAllowed, MyOddStringIsNumericWithAllowDecimals,
  testing::Values(
    test_is{ L"-1", true, true },
    test_is{ L"-0", true, true },
    test_is{ L"-.1", true, true },
    test_is{ L"-0.1", true, true },
    test_is{ L"-1.1", true, true },
    test_is{ L"-999", true, true },
    test_is{ L"-999.a", false, true },
    test_is{ L"-1.1.1", false, true }
));

INSTANTIATE_TEST_CASE_P(NegativeStringsDecimalsNotAllowed, MyOddStringIsNumericWithAllowDecimals,
  testing::Values(
    test_is{ L"-1", true, false },
    test_is{ L"-0", true, false },
    test_is{ L"-1.", true, false },   //  1. is not a decimal
    test_is{ L"-.1", false, false },
    test_is{ L"-0.1", false, false },
    test_is{ L"-1.1", false, false },
    test_is{ L"-999", true, false },
    test_is{ L"-999.a", false, false },
    test_is{ L"-1.1.1", false, false }
));

INSTANTIATE_TEST_CASE_P(PositiveStrings, MyOddStringIsNumeric,
  testing::Values(
    test_is{ L"1", true },
    test_is{ L"+0", true },
    test_is{ L".1", true },
    test_is{ L"0.1", true },
    test_is{ L"+0.1", true },
    test_is{ L"+.1", true },
    test_is{ L"1.1", true },
    test_is{ L"-999", true },
    test_is{ L"-999.a", false },
    test_is{ L"-1.1.1", false },
    test_is{ L"1.", true },
    test_is{ L"   1.   ", true },
    test_is{ L"   10   ", true },
    test_is{ L"   10.4   ", true },
    test_is{ L"   10 . 4   ", false }
));

INSTANTIATE_TEST_CASE_P(PositiveStringsDecimalsNotAllowed, MyOddStringIsNumericWithAllowDecimals,
  testing::Values(
    test_is{ L"1", true, false },
    test_is{ L"+0", true, false },
    test_is{ L".1", false, false },
    test_is{ L"0.1", false, false },
    test_is{ L"+0.1", false, false },
    test_is{ L"+.1", false, false },
    test_is{ L"1.1", false, false },
    test_is{ L"-999", true, false },
    test_is{ L"-999.a", false, false },
    test_is{ L"-1.1.1", false, false },
    test_is{ L"1.", true, false },
    test_is{ L"   1.   ", true, false },
    test_is{ L"   10   ", true, false },
    test_is{ L"   10.4   ", false, false },
    test_is{ L"   10 . 4   ", false, false }
));

INSTANTIATE_TEST_CASE_P(MiscStrings, MyOddStringIsNumeric,
  testing::Values(
    test_is{ L"-", false },
    test_is{ L".", false },
    test_is{ L"+", false },
    test_is{ L"A1,1", false },
    test_is{ L"1,1", false },
    test_is{ L"+A11", false },
    test_is{ L"A11", false },
    test_is{ L"11A", false },
    test_is{ L"11A", false },
    test_is{ L"  1        2   ", false },
    test_is{ L"-999.1   2", false },
    test_is{ L"999.1   2", false },
    test_is{ L"", false },  //  empty
    test_is{ L"        ", false } //  empty
));

INSTANTIATE_TEST_CASE_P(MiscStringsWithDecimalFlag, MyOddStringIsNumericWithAllowDecimals,
  testing::Values(
    test_is{ L"-", false, false },
    test_is{ L".", false, false },
    test_is{ L"+", false, false },
    test_is{ L"A1,1", false, false },
    test_is{ L"1,1", false, false },
    test_is{ L"+A11", false, false },
    test_is{ L"A11", false, false },
    test_is{ L"11A", false, false },
    test_is{ L"11A", false, false },
    test_is{ L"  1        2   ", false, false },
    test_is{ L"-999.1   2", false, false },
    test_is{ L"999.1   2", false, false },
    test_is{ L"", false, false },  //  empty
    test_is{ L"        ", false, false }, //  empty

    test_is{ L"-", false, true },
    test_is{ L".", false, true },
    test_is{ L"+", false, true },
    test_is{ L"A1,1", false, true },
    test_is{ L"1,1", false, true },
    test_is{ L"+A11", false, true },
    test_is{ L"A11", false, true },
    test_is{ L"11A", false, true },
    test_is{ L"11A", false, true },
    test_is{ L"  1        2   ", false, true },
    test_is{ L"-999.1   2", false, true },
    test_is{ L"999.1   2", false, true },
    test_is{ L"", false, true },  //  empty
    test_is{ L"        ", false, true } //  empty

));