#include "string/string.h"
#include "../testcommon.h"
#include <gtest/gtest.h>

const struct test_find
{
  std::wstring haystack;
  std::wstring needle;
  int32_t expected;
  int32_t from;

  friend std::ostream& operator <<(std::ostream& os, const test_find& obj)
  {
    return os
      << "Given : '" << myodd::strings::WString2String(obj.haystack) << "'"
      << " Expected : " << obj.expected ;
  }
};

struct MyOddStringCaseSensitiveFind : testing::Test, testing::WithParamInterface<test_find>
{
};

struct MyOddStringCaseSensitiveFindFrom : MyOddStringCaseSensitiveFind
{
}; 

struct MyOddStringCaseInSensitiveFind : testing::Test, testing::WithParamInterface<test_find>
{
};

struct MyOddStringCaseInSensitiveFindFrom : MyOddStringCaseInSensitiveFind 
{
};

TEST_P(MyOddStringCaseSensitiveFind, CaseSensitiveReplace)
{
  auto haystack = GetParam().haystack;
  auto needle = GetParam().needle;
  auto expected = GetParam().expected;
  // default 'from'

  ASSERT_EQ(expected, myodd::strings::Find(haystack, needle ));
}

TEST_P(MyOddStringCaseInSensitiveFind, CaseInsSensitiveReplace)
{
  auto haystack = GetParam().haystack;
  auto needle = GetParam().needle;
  auto expected = GetParam().expected;

  ASSERT_EQ(expected, myodd::strings::Find(haystack, needle, 0, false ));
}

TEST_P(MyOddStringCaseInSensitiveFindFrom, CaseInsSensitiveReplaceWithFrom)
{
  auto haystack = GetParam().haystack;
  auto needle = GetParam().needle;
  auto expected = GetParam().expected;
  auto from = GetParam().from;

  ASSERT_EQ(expected, myodd::strings::Find(haystack, needle, from, false));
}

TEST_P(MyOddStringCaseSensitiveFindFrom, CaseSensitiveReplace)
{
  auto haystack = GetParam().haystack;
  auto needle = GetParam().needle;
  auto expected = GetParam().expected;
  auto from = GetParam().from;

  ASSERT_EQ(expected, myodd::strings::Find(haystack, needle, from));
}

INSTANTIATE_TEST_SUITE_P(VariousCaseSensitiveFind, MyOddStringCaseSensitiveFind,
  testing::Values(
    test_find{ L"Hello", L"Hello", 0, 0 },
    test_find{ L"Hello", L"hello", -1, 0 }, // case sensitive
    test_find{ L"helloHello", L"Hello", 5, 0 },
    test_find{ L"AxAxAx", L"Ax", 0, 0 }
  ));

INSTANTIATE_TEST_SUITE_P(VariousCaseInSensitiveFind, MyOddStringCaseInSensitiveFind,
  testing::Values(
    test_find{ L"Hello", L"Hello", 0, 0 },
    test_find{ L"Hello", L"hello", 0, 0 },
    test_find{ L"helloHello", L"Hello", 0, 0 },
    test_find{ L"AxAxAx", L"Ax", 0, 0 },
    test_find{ L"AXaxAX", L"Ax", 0, 0 },
    test_find{ L"AXaxAX", L"ax", 0, 0 }
)); 

INSTANTIATE_TEST_SUITE_P(VariousCaseSensitiveFindWithFrom, MyOddStringCaseSensitiveFindFrom,
  testing::Values(
    test_find{ L"Hello", L"Hello", 0, 0 },
    test_find{ L"Hello", L"hello", -1, 0 }, // case insensitive
    test_find{ L"Hello", L"hello", -1, 1 }, // start from 1 + case insensitive
    test_find{ L"Hello", L"Hello", -1, 1 }, // start from 1
    test_find{ L"helloHello", L"Hello", 5, 0 },
    test_find{ L"AxAxAx", L"Ax", 4, 3 },
    test_find{ L"AxAxAx", L"Ax", 4, 4 },
    test_find{ L"AxAxAx", L"Ax", -1, 10 }  // past the len
));

INSTANTIATE_TEST_SUITE_P(VariousCaseInSensitiveFindWithFrom, MyOddStringCaseInSensitiveFindFrom,
  testing::Values(
    test_find{ L"Hello", L"Hello", 0, 0 },
    test_find{ L"Hello", L"hello", 0, 0 },
    test_find{ L"helloHello", L"Hello", 0, 0 },
    test_find{ L"AxAxAx", L"Ax", 0, 0 },
    test_find{ L"AXaxAX", L"Ax", 4, 3 },
    test_find{ L"AXaxAX", L"ax", 4, 4 }
));
